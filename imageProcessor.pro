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

INCLUDEPATH+=C:/qtopencv_out/install/include
INCLUDEPATH+=C:/qtopencv_out/install/include/opencv
INCLUDEPATH+=C:/qtopencv_out/install/include/opencv2


LIBS+=C:\qtopencv_out\lib\libopencv_calib3d2412d.dll.a\
        C:\qtopencv_out\lib\libopencv_contrib2412d.dll.a\
        C:\qtopencv_out\lib\libopencv_core2412d.dll.a\
        C:\qtopencv_out\lib\libopencv_features2d2412d.dll.a\
        C:\qtopencv_out\lib\libopencv_flann2412d.dll.a\
        C:\qtopencv_out\lib\libopencv_gpu2412d.dll.a\
        C:\qtopencv_out\lib\libopencv_highgui2412d.dll.a\
        C:\qtopencv_out\lib\libopencv_imgproc2412d.dll.a\
        C:\qtopencv_out\lib\libopencv_legacy2412d.dll.a\
        C:\qtopencv_out\lib\libopencv_ml2412d.dll.a\
        C:\qtopencv_out\lib\libopencv_objdetect2412d.dll.a\
        C:\qtopencv_out\lib\libopencv_video2412d.dll.a

FORMS += \
    design.ui \
    equal_dialog.ui

RESOURCES += \
    imageprocessor.qrc
