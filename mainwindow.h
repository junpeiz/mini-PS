#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include"image.h"

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openFile();
    void saveFile();

    void detectFace();

private:
    void createActions();
    void createMenus();
    void createTools();

    void showImage();

    QAction * openAction;
    QAction * saveAction;
    QAction * exitAction;

    QAction * detectFaceAction;

    QLabel * imageLabel;

    Image img;
};

#endif // MAINWINDOW_H
