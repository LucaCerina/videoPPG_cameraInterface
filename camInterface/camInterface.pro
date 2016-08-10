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
LIBS += C:\Users\Bio-tec\Documents\opencv\build\x64\vc14\lib\opencv_world310d.lib

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

DISTFILES +=
