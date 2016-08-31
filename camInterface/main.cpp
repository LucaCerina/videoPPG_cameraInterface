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
	if(!QDir(QApplication::applicationDirPath() + "/users/").exists())
		QDir().mkdir(QApplication::applicationDirPath() + "/users/");
	if(!QDir(QApplication::applicationDirPath()+ "/del_users/").exists())
		QDir().mkdir(QApplication::applicationDirPath() + "/del_users/");

	//w.setAttribute(Qt::WA_DeleteOnClose,true);
	w.show();

	return a.exec();
}
