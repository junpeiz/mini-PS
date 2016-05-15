#include "image.h"
#include <vector>

Image::Image()
{
}

bool Image::setImg(const QString &fileName)
{
    if(!fileName.isEmpty())
    {
        img = imread(fileName.toStdString());
        Mat temp;
        img.copyTo(temp);
        storeImg.push_back(temp);
        return true;
    }
    return false;
}

Mat & Image::getImg()
{
    return img;
}
Mat & Image::getDstImg()
{
    return dstImg;
}

vector<Mat> & Image::getStoreImg()
{
    return storeImg;
}

bool Image::saveImg(const QString &fileName) const
{
    if(img.empty()) return false;
    if(!fileName.isEmpty())
    {
        if(!imwrite(fileName.toStdString(),img))
            return false;
    }
    else
        return false;
    return true;
}

bool Image::detectFace()
{
    if(img.empty()) return false;
    CascadeClassifier cascade;
    cascade.load("haarcascade_frontalface_alt.xml");
    if(cascade.empty()) return false;
    vector<Rect> faces;
    Mat grey;
    cvtColor(img,grey,CV_RGB2GRAY);
    double scale = 2;
    Mat smallImg(cvRound(img.rows/scale),cvRound(img.cols/scale),CV_8UC1);
    cv::resize(grey,smallImg,smallImg.size());
    equalizeHist(smallImg,smallImg);
    cascade.detectMultiScale(smallImg,faces,1.1,2,0);
    for(vector<Rect>::const_iterator itr = faces.begin(); itr != faces.end(); itr++)
    {
        Point center;
        Scalar color = CV_RGB(255,0,0);
        int radius;
        double ratio = (double)itr->width / itr->height;

        if(0.75 < ratio && ratio < 1.3)
        {
            center.x = cvRound((itr->x + itr->width * 0.5) * scale);
            center.y = cvRound((itr->y + itr->height * 0.5) * scale);
            radius = cvRound((itr->width + itr->height) * 0.25 * scale);
            circle(img,center,radius,color,3);
        }
        else
            rectangle(img,cvPoint(cvRound(itr->x * scale),cvRound(itr->y * scale)),cvPoint(cvRound((itr->x + itr->width - 1) * scale), cvRound(itr->y + itr->height - 1) * scale),color);
    }
    return true;
}

bool Image::addNoise()
{
    if (dstImg.empty()) return false;

    int channels = dstImg.channels();
    int nRows = dstImg.rows;
    int nCols = dstImg.cols * channels;

    if (dstImg.isContinuous()){
        nCols *= nRows;
        nRows = 1;
    }

    srand((int)time(0));
    for (int i = 0; i < nRows; ++i){
        uchar * p = dstImg.ptr<uchar>(i);
        for (int j = 0; j < 10000; ++j){
            int randIndex = rand()/double(RAND_MAX) * nCols;
            double val = p[randIndex] + generateGaussianNoise() * 128;
            if (val < 0) val = 0;
            if (val > 255) val = 255;
            p[randIndex] = (uchar)val;
        }
    }
    Mat temp;
    dstImg.copyTo(temp);
    storeImg.push_back(temp);
    return true;
}

bool Image::denoise()
{
    if(dstImg.empty()) return false;
    medianBlur(dstImg,dstImg,3);
    Mat temp;
    dstImg.copyTo(temp);
    storeImg.push_back(temp);
    return true;
}

void eraser(int event, int x, int y, int flags, void *param)
{
    Mat* image = (Mat*)param;
    Mat temp = Mat::zeros(image->size(),image->type());
    if (event == CV_EVENT_MOUSEMOVE && flags== CV_EVENT_FLAG_LBUTTON)
    {
        if (x - 20 < 0 || y - 20 < 0) return;
        if (x + 20 > image->cols || y + 20 > image->rows) return;
        Rect tmp = Rect(Point(x - 20, y - 20), Point(x + 20, y + 20));
        image->copyTo(temp);
        rectangle(temp, tmp, Scalar(0, 0, 0), 1);
        Mat roi = (*image)(tmp);
        roi.setTo(Scalar(255, 255, 255));
        imshow("image", temp);
    }
    else if (event == CV_EVENT_LBUTTONUP)
    {
        imshow("image", *image);
    }
}

bool Image::erase()
{
    namedWindow("image");
    imshow("image",img);
    setMouseCallback("image",eraser,&img);
    Mat temp;
    img.copyTo(temp);
    storeImg.push_back(temp);
    return true;
}

bool Image::empty()
{
    return img.empty();
}

void Image::redo()
{
    if(storeImg.size() == 0 || storeImg.size() == 1) return;
    Mat temp = storeImg.at(storeImg.size()-2);
    temp.copyTo(img);
    temp.copyTo(dstImg);
    storeImg.pop_back();
}

//private function
double Image::generateGaussianNoise()
{
    static bool hasSpare = false;
    static double rand1, rand2;
    const static double TWO_PI = 6.2831853071795864769252866;

    if (hasSpare)
    {
        hasSpare = false;
        return sqrt(rand1) * sin(rand2);
    }

    hasSpare = true;

    rand1 = rand() / ((double)RAND_MAX);
    if (rand1 < 1e-100) rand1 = 1e-100;
    rand1 = -2 * log(rand1);
    rand2 = (rand() / ((double)RAND_MAX)) * TWO_PI;

    return sqrt(rand1) * cos(rand2);
}
bool Image::changeAlphaAndBeta(double alpha,double beta)
{
    if(img.empty()) return false;
    img.convertTo(dstImg,-1,alpha*0.01,beta);
    Mat temp;
    img.copyTo(temp);
    storeImg.push_back(temp);
    return true;
}
void Image::myBGR2YUV(const Mat image,Mat &result)
{
    Mat_<Vec3b>::const_iterator it=image.begin<Vec3b>();
    Mat_<Vec3f>::iterator rit=result.begin<Vec3f>();
    Mat_<Vec3b>::const_iterator itend=image.end<Vec3b>();
//遍历所有像素 并转为YUV，注意在opencv中每个彩色像素是按BGR顺序存储的
    for(;it!=itend;++it,++rit)
    {
        (*rit)[0]=0.114*(*it)[0]+0.587*(*it)[1]+0.299*(*it)[2];//Y
        (*rit)[1]=0.5*(*it)[0]-0.331264*(*it)[1]-0.168736*(*it)[2];//Cb
        (*rit)[2]=-0.081312*(*it)[0]-0.418688*(*it)[1]+0.5*(*it)[2];//Cr
    }
}
void Image::CountTemperature(const Mat result,Mat &Temperature ,const short phi)
{
    Mat_<Vec3f>::const_iterator rit=result.begin<Vec3f>();
    Mat_<Vec3f>::const_iterator ritend=result.end<Vec3f>();
    //遍历所有像素 估计色温
    float Y=0,Cb=0,Cr=0,n=0,Z=0,Y1=0,Cb1=0,Cr1=0;
    //const int phi=180;
    for(;rit!=ritend;++rit,++n)
    {
        Y1=(*rit)[0];
        Cb1=(*rit)[1];
        Cr1=(*rit)[2];
        Z=Y1-abs(Cb1)-abs(Cr1);
        if (Z>phi)
            {Y+=Y1;
             Cb+=Cb1;
             Cr+=Cr1;
            }
    }
    //所有像素处理完了
    Temperature.at<Vec3f>(0,0)[0]=Y/n;
    Temperature.at<Vec3f>(0,0)[1]=Cb/n;
    Temperature.at<Vec3f>(0,0)[2]=Cr/n;
}
void Image::gain(Mat Temperature,float &u,float&v,float lamda)
{
    float Y_T=Temperature.at<Vec3f>(0,0)[0];
    float Cb_T=Temperature.at<Vec3f>(0,0)[1];
    float Cr_T=Temperature.at<Vec3f>(0,0)[2];
    //增益计算
// float u=1,lamda=0.05,v=1;//u是B通道增益，v是R通道增益
    if(abs(Cb_T)>abs(Cr_T))
        if(Cb_T>0)
            u-=lamda;
        else
           u+=lamda;
    else
        if(Cr_T>0)
            v-=lamda;
        else
           v+=lamda;
}
void Image::correctionImage(const Mat image,Mat &result,const float u ,const float v){
    Mat_<Vec3b>::const_iterator it=image.begin<Vec3b>();
    Mat_<Vec3b>::const_iterator itend=image.end<Vec3b>();

    Mat_<Vec3b>::iterator rit=result.begin<Vec3b>();
    Mat_<Vec3b>::iterator ritend=result.end<Vec3b>();
    //遍历所有像素 改变B和R通道的值
    for(;it!=itend;++it,++rit)
    {
        (*rit)[0]=saturate_cast<uchar>(u*(*it)[0]);//B
        (*rit)[1]=saturate_cast<uchar>((*it)[1]);//G
        (*rit)[2]=saturate_cast<uchar>(v*(*it)[2]);//R
    }
}
void Image::WhiteBalance(){
    Mat result;//YUV格式
    result.create(img.rows,img.cols,CV_32FC3);
    //dstImg 白平衡后的BGR图像
    dstImg.create(img.rows,img.cols,img.type());

    Mat Temperature(1,1, CV_32FC3,Scalar(100));//初始化色温为100
    //BGR转为YUV
    myBGR2YUV(img,result);
     //估计色温
    CountTemperature(result,Temperature);
    float Cb_T=Temperature.at<Vec3f>(0,0)[1];
    float Cr_T=Temperature.at<Vec3f>(0,0)[2];
    float C=sqrt(Cb_T*Cb_T+Cr_T*Cr_T);
    float u=1,lamda=0.05,v=1;//u是B通道增益，v是R通道增益

    int index=0;//迭代次数
    while (C>0.1&&index<80)
    {

        //增益计算 u,v
        if (index>25)//
            lamda=0.02;
        if (index>40)//
            lamda=0.005;
            if (index>55)//
            lamda=0.001;
        gain(Temperature,u,v,lamda);

        //色温校正后图像为dstImg
        correctionImage(img,dstImg,u ,v);

        //BGR转为YUV
        myBGR2YUV(dstImg,result);
        //估计色温
        CountTemperature(result,Temperature);
        //cb,cr是否足够小
        Cb_T=Temperature.at<Vec3f>(0,0)[1];
        Cr_T=Temperature.at<Vec3f>(0,0)[2];
        C=sqrt(Cb_T*Cb_T+Cr_T*Cr_T);
        ++index;
        //cout<<"index="<<index<<" u="<<u<<" v="<<v<<endl;

    }
    if(!index) img.copyTo(dstImg);//拷贝
    Mat temp;
    img.copyTo(temp);
    storeImg.push_back(temp);
    //cout<<"C="<<C<<" Cb="<<Cb_T<<" Cr="<<Cr_T<<endl;

    //cout<<"图像加载成功"<<result.type()<<endl;
    namedWindow("WhiteBalance");
    imshow("WhiteBalance",dstImg);
    waitKey();
}


int Image::imageMatch(Mat& img2){
    if(img.empty()) return 0;
    // vector of keyPoints
    vector<KeyPoint> keys1;
    vector<KeyPoint> keys2;
    // construction of the fast feature detector object
//    FastFeatureDetector fast1(40);  // 检测的阈值为40
//    FastFeatureDetector fast2(40);
      SurfFeatureDetector fast1(400);
      SurfFeatureDetector fast2(400);
//    MserFeatureDetector fast1(40);
//    MserFeatureDetector fast2(40);
//    StarFeatureDetector fast2(40);
//    StarFeatureDetector fast1(40);
//    SiftFeatureDetector fast1(40);
//    SiftFeatureDetector fast2(40);
    // feature point detection
    fast1.detect(img,keys1);
    double t;
    t=getTickCount();
    fast2.detect(img2,keys2);
    t=getTickCount()-t;
    t=t*1000/getTickFrequency();
    cout<<"KeyPoint Size:"<<keys2.size()<<endl;
    cout<<"extract time:"<<t<<"ms"<<endl;
    drawKeypoints(img, keys1, img, Scalar::all(-1), DrawMatchesFlags::DRAW_OVER_OUTIMG);
    drawKeypoints(img2, keys2, img2, Scalar::all(-1), DrawMatchesFlags::DRAW_OVER_OUTIMG);
   // imshow("FAST feature1", img);
   // imshow("FAST feature2", img2);
   // cvWaitKey(0);
    t=getTickCount();

    SurfDescriptorExtractor Extractor;//Run:BruteForceMatcher< L2<float> > matcher
    //ORB Extractor;//Not Run;
    //BriefDescriptorExtractor Extractor;//RUN:BruteForceMatcher< Hamming > matcher
    //BriefDescriptorExtractor Extractor;
    Mat descriptors1, descriptors2;
    Extractor.compute(img,keys1,descriptors1);
    Extractor.compute(img2,keys2,descriptors2);

    //BruteForceMatcher< Hamming > matcher;
    BruteForceMatcher < L2 <float> > matcher;
   // FlannBasedMatcher matcher;
    vector< DMatch > matches;

     Mat img_matches;

     matcher.match( descriptors1, descriptors2, matches );
     t=getTickCount()-t;
     t=t*1000/getTickFrequency();
     cout<<"match time:"<<t<<"ms"<<endl;
     std::nth_element(matches.begin(),matches.begin()+24,matches.end());
     matches.erase(matches.begin()+25,matches.end());
     drawMatches( img, keys1, img2, keys2, matches, img_matches,
             Scalar::all(-1), Scalar::all(-1),vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

     imshow("draw",img_matches);
     waitKey(0);
}

int Image::equalization()
{
    if(img.empty()) return -1;
    //由于直方图均衡化只能用于灰度图，所以如果不是灰度图要特别处理
    if(img.channels()!=1) return 0;
    equalizeHist(img, dstImg);
    return 1;
}

void Image::change_to_gray()
{
    Mat temp;
    cvtColor(img, temp, CV_BGR2GRAY);
    equalizeHist(temp, dstImg);
    namedWindow("After change to gray");
    imshow("After change to gray", temp);
    namedWindow("After equalization");
    imshow("After equalization", dstImg);
    Mat tempImg;
    dstImg.copyTo(tempImg);
    storeImg.push_back(tempImg);
}

/*****************************************磨皮部分分割线************************************************/
//实现相关函数
int FMax(const int X, const int Y)
{
    return (X < Y ? Y : X);
}

int FMin(const int X, const int Y)
{
    return (Y < X ? Y : X);
}

void BalanceColor(Mat& bitmap,int value)
{
    //定义转换数组
    double  highlights_add[256], highlights_sub[256];
    double  midtones_add[256], midtones_sub[256];
    double  shadows_add[256], shadows_sub[256];
    //初始化转换数组
    for (int i = 0; i < 256; i++)
    {
        highlights_add[i] = shadows_sub[255 - i] = (1.075 - 1 / ((double) i / 16.0 + 1));
        midtones_add[i] = midtones_sub[i] = 0.667 * (1 - (((double) i - 127.0) / 127.0)*(((double) i - 127.0) / 127.0));
        shadows_add[i] = highlights_sub[i] = 0.667 * (1 - (((double) i - 127.0) / 127.0)*(((double) i - 127.0) / 127.0));
    }
    int red, green, blue;
    unsigned char r_lookup[256],g_lookup[256],b_lookup[256];
    for (int i = 0; i < 256; i++)
    {
        red = i;
        green = i;
        blue = i;

        red += (int)( 0.0 * shadows_sub[red] + value * midtones_add[red] + 0.0 * highlights_sub[red]);
        red = FMax(0,FMin(0xFF,red));

        green += (int)( 0.0 * shadows_sub[green] + value * midtones_add[green] + 0.0 * highlights_sub[green]);
        green = FMax(0,FMin(0xFF,green));

        blue += (int)( 0.0 * shadows_sub[blue] + value * midtones_add[blue] + 0.0 * highlights_sub[blue]);
        blue = FMax(0,FMin(0xFF,blue));

        r_lookup[i] = (unsigned char)red;
        g_lookup[i] = (unsigned char)green;
        b_lookup[i] = (unsigned char)blue;
    }
    for (int row = 0; row < bitmap.rows; row++)
        for (int col = 0; col < bitmap.cols; col++)
        {
            bitmap.at<Vec3b>(row, col)[0] = b_lookup[bitmap.at<Vec3b>(row, col)[0]];
            bitmap.at<Vec3b>(row, col)[1] = g_lookup[bitmap.at<Vec3b>(row, col)[1]];
            bitmap.at<Vec3b>(row, col)[2] = r_lookup[bitmap.at<Vec3b>(row, col)[2]];
        }
}

void Image::global_beautify()
{
    int KERNEL_SIZE = 15;
    for (int i = 1; i < KERNEL_SIZE; i = i + 2)
    {
        bilateralFilter(img,dstImg,i,i*2,i/2);
    }
    BalanceColor(dstImg, 60);
    Mat temp;
    dstImg.copyTo(temp);
    storeImg.push_back(temp);
    namedWindow("Beautify");
    imshow("Beautify",dstImg);
}
/**********************************磨皮部分结束分割线*****************************************/

/**********************************抠图部分开始分割线*****************************************/
Point cut_out_p0 = Point(-1,-1);
bool select_flag = false;
Mat cut_out_img, cut_out_showImg, cut_out_imgmask;

void onMouse(int event, int x, int y, int, void*)
{
    if(event==CV_EVENT_LBUTTONDOWN)
    {
        cut_out_p0.x = x;
        cut_out_p0.y = y;
        select_flag = true;
    }
    else if(select_flag &&event == CV_EVENT_MOUSEMOVE)
    {
        Point pt = Point(x,y);
        line(cut_out_showImg,cut_out_p0,pt,Scalar(0,255,0),2,8,0);
        line(cut_out_imgmask,cut_out_p0,pt,Scalar::all(0),2,8,0);
        cut_out_p0 = pt;
        imshow("showImg",cut_out_showImg);
    }
    else if(select_flag && event == CV_EVENT_LBUTTONUP)
    {
        select_flag = false;
        cut_out_p0 = Point(-1,-1);
    }
    else if(event == CV_EVENT_RBUTTONUP)//右击显示抠出的图
    {
        Mat dst;//若希望将圈选的结果相加，定义在外头
        floodFill(cut_out_imgmask,Point(x,y),Scalar(0));//point种子点所在的连通域被填充
        cut_out_img.copyTo(dst,cut_out_imgmask);//mask中所有不为零的点被dst对应的值填充
        imshow("dst",dst);
        cut_out_img.copyTo(cut_out_showImg);
        cut_out_imgmask.setTo(Scalar(255));//重新取值
    }

}
//框图超过画面时会报错
void Image::cut_out()
{
    img.copyTo(cut_out_img);
    img.copyTo(cut_out_showImg);
    cut_out_imgmask.create(img.size(),CV_8UC1);
    cut_out_imgmask.setTo(Scalar(255));
    namedWindow("showImg");
    imshow("showImg",cut_out_showImg);
    setMouseCallback("showImg",onMouse,0);
}
/**********************************抠图部分结束分割线*****************************************/

/**********************************边缘检测开始分割线*****************************************/
Mat detect_edge_temp, detect_edge_dist;

void on_trackbar(int threshold, void *)
{
    //canny边缘检测
    Canny(detect_edge_temp, detect_edge_dist, threshold, threshold * 3);
    imshow("The edge detected", detect_edge_dist);
}

void Image::detect_edge()
{
    cvtColor(img, detect_edge_temp, CV_BGR2GRAY);
    namedWindow("After change to gray");
    imshow("After change to gray", detect_edge_temp);
    int nThresholdEdge = 100;
    namedWindow("The edge detected");
    createTrackbar("Threshold", "The edge detected", &nThresholdEdge, 255, on_trackbar);
}
/**********************************边缘检测结束分割线*****************************************/
