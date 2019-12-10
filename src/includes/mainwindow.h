#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "cvcamerareader.h"
#include "patientdialog.h"
#include "patientbase.h"
#include "camerasetupdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	//display video frame in UI
	void updateVideoUI(QImage img, qint64 timeElapsed);
	void updateDetectedFace(bool value);
	void on_playButton_clicked();

	void on_initButton_clicked();

	void on_actionInsert_Data_triggered();

	void on_actionSearch_subjects_triggered();

	void on_actionSearch_camera_triggered();

	void on_actionCamera_setup_triggered();

	void on_newRecButton_clicked();

	void getExternalPtData(PatientDialog::PatientData extPtIstance);

	void on_recButton_clicked();

public slots:
	void onRecordCompleted();

signals:
	void recordStarted(bool value);

private:
	Ui::MainWindow *ui;
	cameraReader *Reader;

	// Window connections
	PatientDialog *ptDialog;
	patientBase *ptBase;
	PatientDialog::PatientData PtIstance;

	// Global variables
	bool isRecording = false;
};

#endif // MAINWINDOW_H
