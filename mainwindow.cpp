#include "mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Image Processor");
    resize(QGuiApplication::primaryScreen()->availableSize()*3/5);

    imageLabel = new QLabel;

    createActions();
    createMenus();
    createTools();

    setCentralWidget(imageLabel);
}

void MainWindow::createActions()
{
    openAction = new QAction(QIcon("open.png"),"Open",this);  //将images里面的open.png放在debug目录中就能显示啦
    connect(openAction,SIGNAL(triggered(bool)),this,SLOT(openFile()));

    saveAction = new QAction(QIcon("save.png"),"Save",this);
    connect(saveAction,SIGNAL(triggered(bool)),this,SLOT(saveFile()));

    exitAction = new QAction("Exit",this);
    connect(exitAction,SIGNAL(triggered(bool)),this,SLOT(close()));

    detectFaceAction = new QAction("Detect face",this);
    connect(detectFaceAction,SIGNAL(triggered(bool)),this,SLOT(detectFace()));
}

MainWindow::~MainWindow()
{

}

//private functions
void MainWindow::createMenus()
{
    QMenu * fileMenu = new QMenu("File");
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
    menuBar()->addMenu(fileMenu);

    QMenu * processMenu = new QMenu("Process");
    processMenu->addAction(detectFaceAction);
    menuBar()->addMenu(processMenu);
}

void MainWindow::createTools()
{
    QToolBar * fileTool = addToolBar("File");
    fileTool->addAction(openAction);
    fileTool->addAction(saveAction);

    QToolBar * processTool = addToolBar("Process");
    processTool->addAction(detectFaceAction);
}

void MainWindow::showImage()
{
    Mat rgb;
    cvtColor(img.getImg(),rgb,CV_BGR2RGB);
    imageLabel->setPixmap(QPixmap::fromImage(QImage(rgb.data,rgb.cols,rgb.rows,rgb.cols*rgb.channels(),QImage::Format_RGB888)));
    imageLabel->resize(imageLabel->pixmap()->size());
}

//private slots
void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Open Image",".","Image Files (*.png *.jpg *.bmp *.JPG *.PNG)");
    if(!img.setImg(fileName))
        QMessageBox::information(this,"Warning","You don't choose any image",QMessageBox::Ok);
    if(!img.empty())
        showImage();
    else
        QMessageBox::information(this,"Warning","Please choose a right image",QMessageBox::Ok);
}

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,"Save Image",".","Image Files (*.png *.jpg *.bmp)");
    if(!img.saveImg(fileName))
        QMessageBox::information(this,"Warning","The image or filename is wrong",QMessageBox::Ok);
}

void MainWindow::detectFace()
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
