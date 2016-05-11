#include "image.h"

Image::Image()
{
}

bool Image::setImg(const QString &fileName)
{
    if(!fileName.isEmpty())
    {
        img = imread(fileName.toStdString());
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
    return true;
}

bool Image::denoise()
{
    if(dstImg.empty()) return false;
    medianBlur(dstImg,dstImg,3);
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
    return true;
}

bool Image::empty()
{
    return img.empty();
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
    //cout<<"C="<<C<<" Cb="<<Cb_T<<" Cr="<<Cr_T<<endl;

    //cout<<"图像加载成功"<<result.type()<<endl;
    namedWindow("WhiteBalance");
    imshow("WhiteBalance",dstImg);
    waitKey();
}




