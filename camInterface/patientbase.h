#ifndef PATIENTBASE_H
#define PATIENTBASE_H

#include <QDialog>
#include <QDirIterator>
#include <QStringListModel>
#include <QTextStream>
#include <QDebug>
#include <QSettings>
#include <QProcess>
#include <QElapsedTimer>
#include <QDesktopServices>
#include <QUrl>
#include "patientdialog.h"
//#include "mainwindow.h"

namespace Ui {
class patientBase;
}

class patientBase : public QDialog
{
	Q_OBJECT

public:
	explicit patientBase(QWidget *parent = 0, QString callerObject = "NULL");
	~patientBase();

private slots:
	void on_ptAccessButton_clicked();

	void on_ptEditButton_clicked();

	void on_ptNewButton_clicked();

	void on_buttonBox_accepted();

	void populatePtList();

	void on_ptDeleteButton_clicked();

	void on_sigButton_clicked();

	void printOutput();

	void on_videoSelectionChanged(QModelIndex current);

	void on_spinReload_valueChanged(int arg1);

	void on_spinTracker_valueChanged(int arg1);

	void on_openVideoButton_clicked();

	void on_window_rejected();

	void on_videoProcessEnded();

	void on_videoProcessButton_clicked();

private:
	Ui::patientBase *ui;
	QStringListModel *ptListModel = new QStringListModel(this);
	QStringListModel *recListModel = new QStringListModel(this);
	PatientDialog::PatientData ptIstance;
	QElapsedTimer *timerExec = new QElapsedTimer();
	void populateExamList();
	bool getPtData();
	QString caller;

	//video meta data
	double videoFps;
	int videoDuration;

	//video process settings
	QProcess *videoProcess;
	int reloadSteps = 1;
	int frameSub = 1;

signals:
	void sendPtData(PatientDialog::PatientData);

};

#endif // PATIENTBASE_H
