#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QLibrary>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //create patient data folder
    if(!QDir(QDir::currentPath()+"/users/").exists())
        QDir().mkdir(QDir::currentPath()+"/users/");
    if(!QDir(QDir::currentPath()+"/del_users/").exists())
        QDir().mkdir(QDir::currentPath()+"/del_users/");

    //w.setAttribute(Qt::WA_DeleteOnClose,true);
    w.show();

    return a.exec();
}
