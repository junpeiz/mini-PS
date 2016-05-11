#ifndef IMAGE_H
#define IMAGE_H
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/objdetect/objdetect.hpp>
#include<opencv2/opencv_modules.hpp>
#include<opencv2/opencv.hpp>
#include<QString>
#include<QMessageBox>
#include<vector>
#include<time.h>

using namespace cv;
using namespace std;

class Image
{
public:
    Image();

    bool setImg(const QString & fileName);
    Mat & getImg() ;
    Mat & getDstImg() ;

    bool saveImg(const QString &filename) const;

    bool detectFace();
    bool addNoise();
    bool denoise();
    friend void eraser(int event, int x, int y, int flags, void * param);
    bool erase();
    bool empty();
    bool changeAlphaAndBeta(double alpha,double beta);
    void myBGR2YUV(const Mat image,Mat &result);
    void CountTemperature(const Mat result,Mat &Temperature ,const short phi=180);
    void gain(Mat Temperature,float &u,float&v,float lamda=0.05);
    void correctionImage(const Mat image,Mat &result,const float u ,const float v);
    void WhiteBalance();

    void change_to_gray();
    int equalization();
    void global_beautify();

private:
    Mat img;
    Mat storeImg;
    Mat dstImg;

    double generateGaussianNoise();
};



#endif // IMAGE_H
