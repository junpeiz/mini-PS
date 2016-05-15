#ifndef DESIGN_H
#define DESIGN_H

#include <QMainWindow>
#include<QAction>
#include<QMenuBar>
#include<QMenu>
#include<QToolBar>
#include<QSize>
#include<QGuiApplication>
#include<QScreen>
#include<QFileDialog>
#include<QMessageBox>
#include<QString>
#include<QLabel>
#include<QIcon>
#include<iostream>
#include <QMouseEvent>
#include <QPoint>
#include <vector>
#include <time.h>
#include"image.h"
#include"stitch.h"
#include"facetracking.h"

namespace Ui {
class design;
}

class design : public QMainWindow
{
    Q_OBJECT

public:
    explicit design(QWidget *parent = 0);
    ~design();


private slots:
    void on_actionSave_triggered();
    void on_actionOpen_triggered();
    void on_actionDetectface_triggered();
    void on_actionAddNoise_triggered();

    void on_actionDeNoise_triggered();

    void on_actionEraser_triggered();

    void on_actionExit_triggered();

    void on_alphaSlider_sliderMoved(int position);

    void on_alphaSlider_valueChanged(int value);

    void on_betaSlider_sliderMoved(int position);

    void on_betaSlider_valueChanged(int value);

    void on_actionWhiteBalance_triggered();

    void on_actionIconAdd_triggered();

    void on_actionIconSave_triggered();

    void on_actionAddBoard_triggered();

    void on_actionSave_2_triggered();

    void on_actionEqualization_triggered();

    void equal_change_gray(bool flag);

    void on_actionBeauty_triggered();

    void on_actionCutout_triggered();

    void on_actionDetectEdge_triggered();

    void on_actionRedo_triggered();

    void on_actionIconCancel_triggered();

    void on_actionStitch_triggered();

    void on_actionImageMatch_triggered();

    void on_actionFaceTracking_triggered();

    void faceTrack(bool flag);

    void on_actionFudiao_triggered();

    void on_actionHuaijiu_triggered();

    void on_actionSketch_triggered();

    void on_actionCartoon_triggered();

private:
    Ui::design *ui;
    Image img;
    //控制对比度
    double alpha;
    //控制亮度
    int beta;

    Mat logo,mask,imageROI;
    double x,y;
    int IsAddingIcon;
    int IsErasing;
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent * evnet);
    void overlayImage(const cv::Mat &background, const cv::Mat &foreground,
      cv::Mat &output, cv::Point2i location);
    void showImage();
    void showImage(int flag);
    void showImage(const Mat & image);
};

#endif // DESIGN_H
