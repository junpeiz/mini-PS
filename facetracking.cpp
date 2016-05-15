#include "facetracking.h"
#include "ui_facetracking.h"

faceTracking::faceTracking(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::faceTracking)
{
    ui->setupUi(this);
}

faceTracking::~faceTracking()
{
    delete ui;
}

void faceTracking::on_buttonBox_accepted()
{
    emit sendData(true);
}

void faceTracking::on_buttonBox_rejected()
{
    emit sendData(false);
}
