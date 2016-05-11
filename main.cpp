#include "mainwindow.h"
#include "design.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    design w;
    w.show();

    return a.exec();
}
