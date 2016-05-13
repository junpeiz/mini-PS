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
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/legacy/legacy.hpp>

using namespace cv;
using namespace std;

class Image
{
public:
    Image();

    bool setImg(const QString & fileName);
    Mat & getImg();
    Mat & getDstImg();
    vector<Mat> &getStoreImg();

    bool saveImg(const QString &filename) const;

    bool detectFace();
    bool addNoise();
    bool denoise();
    friend void eraser(int event, int x, int y, int flags, void * param);
    bool erase();
    bool empty();
    void redo();
    bool changeAlphaAndBeta(double alpha,double beta);
    void myBGR2YUV(const Mat image,Mat &result);
    void CountTemperature(const Mat result,Mat &Temperature ,const short phi=180);
    void gain(Mat Temperature,float &u,float&v,float lamda=0.05);
    void correctionImage(const Mat image,Mat &result,const float u ,const float v);
    void WhiteBalance();
    int imageMatch(Mat& img2);

    void change_to_gray();
    int equalization();
    void global_beautify();
    void cut_out();
    void detect_edge();

private:
    Mat img;
    vector<Mat> storeImg;
    Mat dstImg;

    double generateGaussianNoise();
};



#endif // IMAGE_H
