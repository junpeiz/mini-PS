#ifndef STITCH_H
#define STITCH_H

#include <QMainWindow>
#include<QFile>
#include<QString>
#include<QMessageBox>
#include<QFileDialog>
#include<QWidget>
#include<QPixmap>
#include<QImage>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/stitching/stitcher.hpp>
#include<vector>

using namespace cv;
using namespace std;

namespace Ui {
class Stitch;
}

class Stitch : public QMainWindow
{
    Q_OBJECT

public:
    explicit Stitch(QWidget *parent = 0);
    ~Stitch();

private slots:
    void on_actionOpen_triggered();

    void on_actionStitch_triggered();

    void on_actionSave_triggered();

private:
    Ui::Stitch *ui;

    vector<Mat> imgs;
    Mat result;
};

#endif // STITCH_H
