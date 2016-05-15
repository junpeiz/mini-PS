#include "design.h"
#include "ui_design.h"
#include "equal_dialog.h"


design::design(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::design)
{
    alpha=80;
    beta=80;
    IsErasing = 0;
    IsAddingIcon = 0;
    ui->setupUi(this);
}

design::~design()
{
    delete ui;
}

void design::on_actionSave_triggered()
{

    QString fileName = QFileDialog::getSaveFileName(this,"Save Image",".","Image Files (*.png *.jpg *.bmp)");
    if(!img.saveImg(fileName))
        QMessageBox::information(this,"Warning","The image or filename is wrong",QMessageBox::Ok);
}

void design::on_actionOpen_triggered()
{

    QString fileName = QFileDialog::getOpenFileName(this,"Open Image",".","Image Files (*.png *.jpg *.bmp)");
    if(!img.setImg(fileName))
    {
        QMessageBox::information(this,"Warning","You don't choose any image",QMessageBox::Ok);
        return;
    }
    if(!img.empty()){
        img.getImg().copyTo(img.getDstImg());
        showImage(1);
    }
    else
        QMessageBox::information(this,"Warning","Please choose a right image",QMessageBox::Ok);
}

void design::on_actionDetectface_triggered()
{

    if(!img.detectFace())
    {
        if(img.empty())
            QMessageBox::information(this,"Warning","You don't choose any image",QMessageBox::Ok);
        else
            QMessageBox::information(this,"Warning","The cascadeClassifier do not load correctly",QMessageBox::Ok);
    }
    showImage();
}
void design::showImage()
{
    if(img.empty()) return;
    Mat rgb;
    cvtColor(img.getImg(),rgb,CV_BGR2RGB);
    ui->imageLabel->setPixmap(QPixmap::fromImage(QImage(rgb.data,rgb.cols,rgb.rows,rgb.cols*rgb.channels(),QImage::Format_RGB888)));
    ui->imageLabel->resize(ui->imageLabel->pixmap()->size());
    ui->scrollAreaWidgetContents->setFixedSize(ui->imageLabel->size());
}
void design::showImage(int flag){   //flag==1  Img   else   dstImg
    if(flag==1){
        if(img.empty()) return;
        Mat rgb;
        cvtColor(img.getImg(),rgb,CV_BGR2RGB);
        ui->imageLabel->setPixmap(QPixmap::fromImage(QImage(rgb.data,rgb.cols,rgb.rows,rgb.cols*rgb.channels(),QImage::Format_RGB888)));
        ui->imageLabel->resize(ui->imageLabel->pixmap()->size());
        ui->scrollAreaWidgetContents->setFixedSize(ui->imageLabel->size());
    }
    else {
        if(img.empty()) return;
        Mat rgb;
        cvtColor(img.getDstImg(),rgb,CV_BGR2RGB);
        ui->imageLabel->setPixmap(QPixmap::fromImage(QImage(rgb.data,rgb.cols,rgb.rows,rgb.cols*rgb.channels(),QImage::Format_RGB888)));
        ui->imageLabel->resize(ui->imageLabel->pixmap()->size());
        ui->scrollAreaWidgetContents->setFixedSize(ui->imageLabel->size());
    }
}

void design::showImage(const Mat &image)
{
    if(image.empty()) return;
    Mat rgb;
    cvtColor(image,rgb,CV_BGR2RGB);
    ui->imageLabel->setPixmap(QPixmap::fromImage(QImage(rgb.data,rgb.cols,rgb.rows,rgb.cols*rgb.channels(),QImage::Format_RGB888)));
    ui->imageLabel->resize(ui->imageLabel->pixmap()->size());
    ui->scrollAreaWidgetContents->setFixedSize(ui->imageLabel->size());
}

void design::on_actionAddNoise_triggered()
{
    if(!img.addNoise())
        QMessageBox::information(this,"Warning","there is no image",QMessageBox::Ok);
    showImage(0);
}

void design::on_actionDeNoise_triggered()
{
    if(!img.denoise())
        QMessageBox::information(this,"Warning","there is no image",QMessageBox::Ok);
    showImage(0);
}

void design::on_actionEraser_triggered()
{
    if(img.getImg().empty()) {
        QMessageBox::information(this,"Warning","there is no image",QMessageBox::Ok);
        return ;
    }
    img.getImg().copyTo(img.getDstImg());
    IsErasing = 1;
}

void design::on_actionExit_triggered()
{
    close();
}


void design::on_alphaSlider_sliderMoved(int position)
{
    alpha=position;
    img.changeAlphaAndBeta(alpha,beta);
    ui->TextLabel->setNum(alpha);
    if(img.getImg().empty()) {
        return ;
    }
    showImage(0);
}

void design::on_alphaSlider_valueChanged(int value)
{
    alpha=value;
    img.changeAlphaAndBeta(alpha,beta);
    ui->TextLabel->setNum(alpha);
    if(img.getImg().empty()) {
        return ;
    }
    showImage(0);
}

void design::on_betaSlider_sliderMoved(int position)
{
    beta=position;
    img.changeAlphaAndBeta(alpha,beta);
    ui->TextLabel_2->setNum(beta);
    if(img.getImg().empty()) {
        return ;
    }
    showImage(0);
}

void design::on_betaSlider_valueChanged(int value)
{
    beta=value;
    img.changeAlphaAndBeta(alpha,beta);
    ui->TextLabel_2->setNum(beta);
    if(img.getImg().empty()) {
        return ;
    }
    showImage(0);
}

void design::on_actionWhiteBalance_triggered()
{
    if(img.getImg().empty()) {
        QMessageBox::information(this,"Warning","there is no image",QMessageBox::Ok);
        return ;
    }
    img.WhiteBalance();
}
void design::mouseMoveEvent(QMouseEvent* event)
{
    QPoint m = event->pos();
    x=m.x()-10;
    y=m.y()-169;
    if(IsAddingIcon)
    {
        img.getImg().copyTo(img.getDstImg());
        cout<<"x and y:"<<x<<" "<<y<<endl;
        if(!img.getDstImg().empty()&&x-logo.cols/2>=0&&y-logo.rows/2>=0){
            if((y+logo.rows/2<img.getImg().rows))
                if(x+logo.cols/2<img.getImg().cols)
                    imageROI = img.getDstImg()(cv::Rect(Point(x-logo.cols/2,y-logo.rows/2),Point(x+logo.cols/2,y+logo.rows/2)));
        }
        if(!imageROI.empty()){
            logo.copyTo(imageROI,mask);
        }
        showImage(0);
    }
    else if(IsErasing)
    {
        Mat temp = Mat::zeros(img.getDstImg().size(),img.getDstImg().type());
        if (x - 20 < 0 || y - 20 < 0) return;
        if (x + 20 > img.getDstImg().cols || y + 20 > img.getDstImg().rows) return;
        Rect tmp = Rect(Point(x - 20, y - 20), Point(x + 20, y + 20));
        img.getDstImg().copyTo(temp);
        rectangle(temp, tmp, Scalar(0, 0, 0), 1);
        Mat roi = img.getDstImg()(tmp);
        roi.setTo(Scalar(255, 255, 255));
        showImage(temp);
    }
    else
        showImage(0);
    //    cv::namedWindow("result");
    //    cv::imshow("result",img.getImg());

}

void design::mouseReleaseEvent(QMouseEvent *evnet)
{
    showImage(0);
}

void design::on_actionIconAdd_triggered()
{
    IsAddingIcon = 1;
    if(img.getImg().empty()) {
        QMessageBox::information(this,"Warning","there is no image",QMessageBox::Ok);
        return ;
    }
    QString fileName = QFileDialog::getOpenFileName(this,"Open Image",".","Image Files (*.png *.jpg *.bmp)");
    if(!fileName.isEmpty()){
        logo = imread(fileName.toStdString());
        mask = imread(fileName.toStdString(),0);
    }
}

void design::on_actionIconSave_triggered()
{
    if(img.getImg().empty()) {
        QMessageBox::information(this,"Warning","there is no image",QMessageBox::Ok);
        return ;
    }
    if(!img.getDstImg().empty()){
        img.getDstImg().copyTo(img.getImg());
    }
    IsAddingIcon = 0;
}

void design::on_actionIconCancel_triggered()
{
    IsAddingIcon = 0;
    showImage(1);
}

void design::on_actionAddBoard_triggered()
{
    if(img.getImg().empty()) {
        QMessageBox::information(this,"Warning","there is no image",QMessageBox::Ok);
        return ;
    }
    QString fileName = QFileDialog::getOpenFileName(this,"Open Image",".","Image Files (*.png *.jpg *.bmp)");
    if(!fileName.isEmpty()){
        //        srcAlpha = imread(fileName.toStdString(),-1);
        //        dest= imread(fileName.toStdString());
        logo = imread(fileName.toStdString(),-1);
        mask = imread(fileName.toStdString(),0);


        Size dsize = Size(img.getImg().cols,img.getImg().rows);
        cv::resize(logo,logo,dsize);
        //   mapToMat(srcAlpha, dest, 50, 100);
        overlayImage(img.getImg(), logo, img.getDstImg(), cv::Point(0,0));
//        cv::namedWindow("result1");
//        cv::imshow("result1",result);
//        cv::waitKey();

        showImage(0);
    }
}

void design::overlayImage(const cv::Mat &background, const cv::Mat &foreground,
                              cv::Mat &output, cv::Point2i location)
{
    background.copyTo(output);


    // start at the row indicated by location, or at row 0 if location.y is negative.
    for(int y = std::max(location.y , 0); y < background.rows; ++y)
    {
        int fY = y - location.y; // because of the translation

        // we are done of we have processed all rows of the foreground image.
        if(fY >= foreground.rows)
            break;

        // start at the column indicated by location,

        // or at column 0 if location.x is negative.
        for(int x = std::max(location.x, 0); x < background.cols; ++x)
        {
            int fX = x - location.x; // because of the translation.

            // we are done with this row if the column is outside of the foreground image.
            if(fX >= foreground.cols)
                break;

            // determine the opacity of the foregrond pixel, using its fourth (alpha) channel.
            double opacity =
                    ((double)foreground.data[fY * foreground.step + fX * foreground.channels() + 3])

                    / 255.;


            // and now combine the background and foreground pixel, using the opacity,

            // but only if opacity > 0.
            for(int c = 0; opacity > 0 && c < output.channels(); ++c)
            {
                unsigned char foregroundPx =
                        foreground.data[fY * foreground.step + fX * foreground.channels() + c];
                unsigned char backgroundPx =
                        background.data[y * background.step + x * background.channels() + c];
                output.data[y*output.step + output.channels()*x + c] =
                        backgroundPx * (1.-opacity) + foregroundPx * opacity;
            }
        }
    }
}

void design::on_actionSave_2_triggered()
{
    if(img.getImg().empty()) {
        QMessageBox::information(this,"Warning","there is no image",QMessageBox::Ok);
        return ;
    }
    IsAddingIcon = 0;
    IsErasing = 0;
    if(img.getDstImg().empty()) return;
    img.getDstImg().copyTo(img.getImg());
    Mat temp;
    img.getDstImg().copyTo(temp);
    img.getStoreImg().push_back(temp);
    showImage();
    namedWindow("img");
    imshow("img",img.getImg());
}

void design::on_actionEqualization_triggered()
{
    if(img.getImg().empty()) {
        QMessageBox::information(this,"Warning","there is no image",QMessageBox::Ok);
        return ;
    }
    if(img.equalization() == 0){
        equal_dialog dialog;
        dialog.setWindowTitle("灰度图转化确认");
        connect(&dialog, SIGNAL(sendData(bool)), this, SLOT(equal_change_gray(bool)));
        dialog.exec();
    }
}

void design::equal_change_gray(bool flag)
{
    if(flag) img.change_to_gray();
}

void design::on_actionBeauty_triggered()
{
    if(img.getImg().empty()) {
        QMessageBox::information(this,"Warning","there is no image",QMessageBox::Ok);
        return ;
    }
    img.global_beautify();
}


void design::on_actionCutout_triggered()
{
    if(img.getImg().empty()) {
        QMessageBox::information(this,"Warning","there is no image",QMessageBox::Ok);
        return ;
    }
    img.cut_out();
}


void design::on_actionDetectEdge_triggered()
{
    if(img.getImg().empty()) {
        QMessageBox::information(this,"Warning","there is no image",QMessageBox::Ok);
        return ;
    }
    img.detect_edge();
}

void design::on_actionRedo_triggered()
{
    img.redo();
    img.getImg().copyTo(img.getDstImg());
    showImage();
}

void design::on_actionStitch_triggered()
{
    Stitch* stitch = new Stitch;
    stitch->setWindowTitle("Stitch");
    stitch->show();
}

void design::on_actionImageMatch_triggered()
{
    Mat img2;
    QString fileName = QFileDialog::getOpenFileName(this,"Open Image",".","Image Files (*.png *.jpg *.bmp)");
    img2=imread(fileName.toStdString());
    if(img2.empty())
    {
        QMessageBox::information(this,"Warning","You don't choose any image",QMessageBox::Ok);
        return;
    }
    if(!img.imageMatch(img2)){
        QMessageBox::information(this,"Warning","Fail to match the image",QMessageBox::Ok);
        return ;
    }
}

void design::on_actionFaceTracking_triggered()
{
    faceTracking dialog;
    dialog.setWindowTitle("track or not");
    connect(&dialog,SIGNAL(sendData(bool)),this,SLOT(faceTrack(bool)));
    dialog.exec();
}

void design::faceTrack(bool flag)
{
    if(flag)
    {
        img.track();
    }
}
