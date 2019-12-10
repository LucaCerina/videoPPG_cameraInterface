#-------------------------------------------------
#
# Project created by QtCreator 2016-05-10T08:28:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = camInterface
TEMPLATE = app

!win32{
    #INCLUDEPATH += /usr/local/include/opencv2
    #LIBS += -L /usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_videoio -lopencv_imgproc -lopencv_objdetect -lopencv_highgui
    SRCFOLDER = "src/"
    HEADERFOLDER = "src/includes/"
    UIFOLDER = "ui/"
}
win32{
    SRCFOLDER = "src\"
    HEADERFOLDER = "src\includes\"
    UIFOLDER = "ui\"
    CVPATH = Add\here\the\path\to\opencv #C:\Users\Bio-tec\Documents\opencv
    ICPATH = Add\here\the\path\to\ImagingSource IC Imaging Control 3.4\folder
    INCLUDEPATH += ${CVPATH}\build\include
    INCLUDEPATH += ${ICPATH}\classlib\include
    Debug:LIBS += ${CVPATH}\build\x64\vc14\lib\opencv_world310d.lib
    Release:LIBS += ${CVPATH}\build\x64\vc14\lib\opencv_world310.lib
    Debug:LIBS += ${ICPATH}\classlib\x64\debug\TIS_UDSHL11d_x64.lib
    Release:LIBS += ${ICPATH}\classlib\x64\release\TIS_UDSHL11_x64.lib
}

SOURCES += ${SRCFOLDER}main.cpp\
	${SRCFOLDER}mainwindow.cpp \
    ${SRCFOLDER}cvcamerareader.cpp \
    ${SRCFOLDER}patientdialog.cpp \
    ${SRCFOLDER}patientbase.cpp \
    ${SRCFOLDER}utilities.cpp \
    ${SRCFOLDER}camerasetupdialog.cpp

HEADERS  += ${HEADERFOLDER}mainwindow.h \
    ${HEADERFOLDER}cvcamerareader.h \
    ${HEADERFOLDER}patientdialog.h \
    ${HEADERFOLDER}patientbase.h \
    ${HEADERFOLDER}utilities.h \
    ${HEADERFOLDER}camerasetupdialog.h

FORMS    += ${UIFOLDER}mainwindow.ui \
    ${UIFOLDER}patientdialog.ui \
    ${UIFOLDER}patientbase.ui \
    ${UIFOLDER}camerasetupdialog.ui

Debug:3rdparty.path   = $${OUT_PWD}/debug
Release:3rdparty.path   = $${OUT_PWD}/release
3rdparty.files  = $${PWD}/externals/*
INSTALLS       += 3rdparty

