#-------------------------------------------------
#
# Project created by QtCreator 2016-05-10T08:28:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = camInterface
TEMPLATE = app

CVPATH = Add\here\the\path\to\opencv #C:\Users\Bio-tec\Documents\opencv
ICPATH = Add\here\the\path\to\ImagingSource IC Imaging Control 3.4\folder
INCLUDEPATH += ${CVPATH}\build\include
INCLUDEPATH += ${ICPATH}\classlib\include
INCLUDEPATH += src/includes/
Debug:LIBS += ${CVPATH}\build\x64\vc14\lib\opencv_world310d.lib
Release:LIBS += ${CVPATH}\build\x64\vc14\lib\opencv_world310.lib
Debug:LIBS += ${ICPATH}\classlib\x64\debug\TIS_UDSHL11d_x64.lib
Release:LIBS += ${ICPATH}\classlib\x64\release\TIS_UDSHL11_x64.lib

Debug:3rdparty.path   = $${OUT_PWD}/debug
Release:3rdparty.path   = $${OUT_PWD}/release
3rdparty.files  = $${PWD}/externals/*
INSTALLS       += 3rdparty

SOURCES += src/main.cpp\
	src/mainwindow.cpp \
    src/cvcamerareader.cpp \
    src/patientdialog.cpp \
    src/patientbase.cpp \
    src/utilities.cpp \
    src/camerasetupdialog.cpp

HEADERS  += src/includes/mainwindow.h \
    src/includes/cvcamerareader.h \
    src/includes/patientdialog.h \
    src/includes/patientbase.h \
    src/includes/utilities.h \
    src/includes/camerasetupdialog.h

FORMS    += ui/mainwindow.ui \
    ui/patientdialog.ui \
    ui/patientbase.ui \
    ui/camerasetupdialog.ui


