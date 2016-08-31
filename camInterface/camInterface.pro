#-------------------------------------------------
#
# Project created by QtCreator 2016-05-10T08:28:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = camInterface
TEMPLATE = app

#INCLUDEPATH += /usr/local/include/opencv2
#LIBS += -L /usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_videoio -lopencv_imgproc -lopencv_objdetect -lopencv_highgui
INCLUDEPATH += C:\Users\Bio-tec\Documents\opencv\build\include
INCLUDEPATH += "C:\Users\Bio-tec\Documents\IC Imaging Control 3.4\classlib\include"
LIBS += C:\Users\Bio-tec\Documents\opencv\build\x64\vc14\lib\opencv_world310d.lib
Debug:LIBS += "C:\Users\Bio-tec\Documents\IC Imaging Control 3.4\classlib\x64\debug\TIS_UDSHL11d_x64.lib"
Release:LIBS += "C:\Users\Bio-tec\Documents\IC Imaging Control 3.4\classlib\x64\release\TIS_UDSHL11_x64.lib"

Debug:3rdparty.path   = $${OUT_PWD}/3rdparty
Release:3rdparty.path   = $${OUT_PWD}/release
3rdparty.files  = $${PWD}/externals/*
INSTALLS       += 3rdparty

SOURCES += main.cpp\
	mainwindow.cpp \
    cvcamerareader.cpp \
    patientdialog.cpp \
    patientbase.cpp \
    utilities.cpp \
    camerasetupdialog.cpp

HEADERS  += mainwindow.h \
    cvcamerareader.h \
    patientdialog.h \
    patientbase.h \
    utilities.h \
    camerasetupdialog.h

FORMS    += mainwindow.ui \
    signalwidget.ui \
    patientdialog.ui \
    patientbase.ui \
    camerasetupdialog.ui


