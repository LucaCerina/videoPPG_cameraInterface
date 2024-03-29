#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	Reader = new cameraReader();
	QObject::connect(Reader, SIGNAL(processedImage(QImage, qint64)),
					 this, SLOT(updateVideoUI(QImage, qint64)));
	QObject::connect(Reader, SIGNAL(detectedFace(bool)),
					 this, SLOT(updateDetectedFace(bool)));
	QObject::connect(this, SIGNAL(recordStarted(bool)),
					 Reader, SLOT(onRecordStarted(bool)));
	QObject::connect(Reader, SIGNAL(recordCompleted()),
					 this, SLOT(onRecordCompleted()));
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete Reader;
	delete ui;
}

void MainWindow::on_playButton_clicked()
{
	if(Reader->isStopped())
	{
		Reader->Play();
		ui->playButton->setText(tr("Stop"));
		ui->statusLabel->setText("Preview");
		ui->recButton->setEnabled(true);
	}
	else
	{
		Reader->Stop();
		ui->playButton->setText(tr("Play"));
		ui->playButton->setEnabled(false);
		ui->statusLabel->setText("Idle");
		ui->recButton->setEnabled(false);
	}

}

void MainWindow::on_initButton_clicked()
{
	QMessageBox msgBox;
	bool cameraAvailable = false;
	if(!Reader->isInitialized())
	{
		// Try to open at least one camera
		if(Reader->checkCamera(0))
		{
			ui->cameraList->addItem(QString::number(0));
			cameraAvailable = true;
		}
		if(!cameraAvailable)
		{
			msgBox.setText("Cannot open any camera! Check connections.");
			msgBox.exec();
		}
		else
		{
			ui->initButton->setText("Init camera");
		}
	}
	else
	{
		// Create folder for video file
		QString outPath = QApplication::applicationDirPath() + "/users/" + PtIstance.ptId
				+ "/records/";
		if(!QDir(outPath).exists())
			QDir().mkdir(outPath);

		//Create video filename
		QString outName = outPath + "video_" + PtIstance.ptId + "_"
				+ QDate::currentDate().toString("dd-MM-yyyy") + "_"
				+ QString::number(QTime::currentTime().hour()) + "-"
				+ QString::number(QTime::currentTime().minute()) + ".avi";
		if(QFile(outName).exists())
		{
			msgBox.setText("Attention! File already present,it will be overwritten");
			msgBox.exec();
		}

		//Initialize camera
		if(!Reader->initCamera(ui->cameraList->currentText().toInt(), outName))
		{
			msgBox.setText("Cannot open camera! Check connection.");
			msgBox.exec();
			ui->initButton->setText("Search cameras");
		}
		else
		{
			ui->playButton->setEnabled(true);
		}
	}
}

void MainWindow::updateVideoUI(QImage img, qint64 timeElapsed)
{
	/* Visualize preview in mainwindow */
	if(!img.isNull())
	{
		ui->videoLabel->setPixmap(QPixmap::fromImage(img).scaled(ui->videoLabel->size(),
																 Qt::KeepAspectRatio, Qt::FastTransformation));
	}
	if(isRecording)
		ui->recordTimeLabel->setText(QDateTime::fromTime_t(timeElapsed/1000).toUTC().toString("hh:mm:ss"));
}

void MainWindow::updateDetectedFace(bool value)
{
	QPalette sPalette;
	if(value)
	{
		ui->faceLabel->setText("Correct recording");
		sPalette.setColor(QPalette::Window, Qt::green);
		ui->faceLabel->setPalette(sPalette);
	}
	else
	{
		ui->faceLabel->setText("Face not in frame");
		sPalette.setColor(QPalette::Window, Qt::red);
		ui->faceLabel->setPalette(sPalette);
	}
}

//MENU ACTIONS
void MainWindow::on_actionInsert_Data_triggered()
{
	ptDialog = new PatientDialog(this);
	ptDialog->show();
}

void MainWindow::on_actionSearch_subjects_triggered()
{
	ptBase = new patientBase(this, "sbjSearch");
	ptBase->show();
}

void MainWindow::on_actionSearch_camera_triggered()
{
	on_initButton_clicked();
}

void MainWindow::on_actionCamera_setup_triggered()
{
	camerasetupdialog *camSetup = new camerasetupdialog(this);
	camSetup->show();
}

void MainWindow::on_newRecButton_clicked()
{
	ptBase = new patientBase(this, "newRec");
	ptBase->exec();

	//check if a subject was selected
	if(PtIstance.ptId != "")
	{
		ui->IDvalue->setText(PtIstance.ptId);
		ui->nameValue->setText(PtIstance.ptName);
		ui->surnameValue->setText(PtIstance.ptSurName);
		ui->AFValue->setText(PtIstance.ptAFType);
		//enable camera init
		ui->initButton->setEnabled(true);
		//search if camera was already set in settings
		QString settingsFile = QApplication::applicationDirPath() + "/cameraSettings.ini";
		QSettings cameraSetting(settingsFile, QSettings::IniFormat);
		ui->statusLabel->setText("Idle");
	}
	else
	{
		QMessageBox::warning(this, "Error", "No patient selected for data recording");
		this->show();
	}
}

//SLOTS

void MainWindow::getExternalPtData(PatientDialog::PatientData extPtIstance)
{
	PtIstance = extPtIstance;
}

void MainWindow::on_recButton_clicked()
{
	ui->statusLabel->setText("Recording");
	isRecording = true;
	emit(recordStarted(true));
}

void MainWindow::onRecordCompleted()
{
	ui->statusLabel->setText("Record completed");
	ui->playButton->setText(tr("Play"));
	ui->playButton->setEnabled(false);
	ui->recButton->setEnabled(false);
}
