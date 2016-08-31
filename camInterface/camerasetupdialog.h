#ifndef CAMERASETUPDIALOG_H
#define CAMERASETUPDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QMessageBox>
#include "cvcamerareader.h"
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include<Grabber.h>
#include<tisudshl.h>

typedef struct{
	QString cameraName;
	QString cameraFormat;
	double cameraFps;
	int videoDuration;
}cameraData;

namespace Ui {
class camerasetupdialog;
}

class camerasetupdialog : public QDialog
{
	Q_OBJECT

public:
	explicit camerasetupdialog(QWidget *parent = 0);
	~camerasetupdialog();

private slots:
	void on_searchButton_clicked();

	void on_buttonBox_accepted();

	void on_cameraList_currentIndexChanged(int index);

	void on_formatList_currentIndexChanged(int index);

	void on_fpsList_currentIndexChanged(int index);

	void on_lineEdit_editingFinished();

private:
	Ui::camerasetupdialog *ui;
	QString settingsFile;
	cameraData camData;
};

#endif // CAMERASETUPDIALOG_H
