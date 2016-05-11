#-------------------------------------------------
#
# Project created by QtCreator 2016-05-03T12:21:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = imageProcessor
TEMPLATE = app


SOURCES += main.cpp\
    image.cpp \
    design.cpp \
    equal_dialog.cpp

HEADERS  += \
    image.h \
    design.h \
    equal_dialog.h

CONFIG += c++11

INCLUDEPATH += D:/opencv/MinGW/install/include
INCLUDEPATH += D:/opencv/MinGW/install/include/opencv
INCLUDEPATH += D:/opencv/MinGW/install/include/opencv2

LIBS += "D:/opencv/MinGW/bin/*.dll"

FORMS += \
    design.ui \
    equal_dialog.ui

RESOURCES += \
    imageprocessor.qrc
