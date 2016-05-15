#ifndef FACETRACKING_H
#define FACETRACKING_H

#include <QDialog>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/objdetect/objdetect.hpp>
#include<QDebug>
#include<QAbstractButton>
#include<vector>
#include<iostream>

using namespace cv;
using namespace std;

namespace Ui {
class faceTracking;
}

class faceTracking : public QDialog
{
    Q_OBJECT

public:
    explicit faceTracking(QWidget *parent = 0);
    ~faceTracking();

signals:
    void sendData(bool);

private slots:

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::faceTracking *ui;

};

#endif // FACETRACKING_H
