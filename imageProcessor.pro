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
    equal_dialog.cpp \
    stitch.cpp

HEADERS  += \
    image.h \
    design.h \
    equal_dialog.h \
    stitch.h

CONFIG += c++11

INCLUDEPATH+=F:/programming/qtopencv_out/install/include
INCLUDEPATH+=F:/programming/qtopencv_out/install/include/opencv
INCLUDEPATH+=F:/programming/qtopencv_out/install/include/opencv2

LIBS+=F:\programming\qtopencv_out\bin\libopencv_videostab2412d.dll\
        F:\programming\qtopencv_out\bin\libopencv_superres2412d.dll\
        F:\programming\qtopencv_out\bin\libopencv_stitching2412d.dll\
        F:\programming\qtopencv_out\bin\libopencv_contrib2412d.dll\
        F:\programming\qtopencv_out\bin\libopencv_nonfree2412d.dll\
        F:\programming\qtopencv_out\bin\libopencv_ocl2412d.dll\
        F:\programming\qtopencv_out\bin\libopencv_gpu2412d.dll\
        F:\programming\qtopencv_out\bin\libopencv_photo2412d.dll\
        F:\programming\qtopencv_out\bin\libopencv_objdetect2412d.dll\
        F:\programming\qtopencv_out\bin\libopencv_legacy2412d.dll\
        F:\programming\qtopencv_out\bin\libopencv_ml2412d.dll\
        F:\programming\qtopencv_out\bin\libopencv_video2412d.dll\
        F:\programming\qtopencv_out\bin\libopencv_calib3d2412d.dll\
        F:\programming\qtopencv_out\bin\libopencv_features2d2412d.dll\
        F:\programming\qtopencv_out\bin\opencv_ffmpeg2412.dll\
        F:\programming\qtopencv_out\bin\libopencv_highgui2412d.dll\
        F:\programming\qtopencv_out\bin\libopencv_imgproc2412d.dll\
        F:\programming\qtopencv_out\bin\libopencv_flann2412d.dll\
        F:\programming\qtopencv_out\bin\libopencv_core2412d.dll
FORMS += \
    design.ui \
    equal_dialog.ui \
    stitch.ui

RESOURCES += \
    imageprocessor.qrc


