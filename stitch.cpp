#include "stitch.h"
#include "ui_stitch.h"

Stitch::Stitch(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Stitch)
{
    ui->setupUi(this);
}

Stitch::~Stitch()
{
    delete ui;
}

void Stitch::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Open Image",".","Image Files (*.png *.jpg *.bmp)");
    Mat img = imread(fileName.toStdString());
    if(img.empty())
    {
        QMessageBox::information(this,"Warning","You don't choose any image",QMessageBox::Ok);
        return;
    }
    else if(!img.empty()){
        namedWindow(fileName.toStdString());
        imshow(fileName.toStdString(),img);
        imgs.push_back(img);
    }
    else
        QMessageBox::information(this,"Warning","Please choose a right image",QMessageBox::Ok);
}

void Stitch::on_actionStitch_triggered()
{
    Stitcher stitcher = Stitcher::createDefault(false);
    Stitcher::Status status = stitcher.stitch(imgs, result);
    if(status == Stitcher::OK)
    {
        Mat rgb;
        cvtColor(result,rgb,CV_BGR2RGB);
        ui->imageLabel->setPixmap(QPixmap::fromImage(QImage(rgb.data,rgb.cols,rgb.rows,rgb.cols*rgb.channels(),QImage::Format_RGB888)));
        ui->imageLabel->resize(ui->imageLabel->pixmap()->size());
        ui->scrollAreaWidgetContents->setFixedSize(ui->imageLabel->size());
    }
}

void Stitch::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,"Save Image",".","Image Files (*.png *.jpg *.bmp)");
    if(!imwrite(fileName.toStdString(),result))
        QMessageBox::information(this,"Warning","The image or filename is wrong",QMessageBox::Ok);
}
