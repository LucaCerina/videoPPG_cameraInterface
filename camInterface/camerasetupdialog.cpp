#include "camerasetupdialog.h"
#include "ui_camerasetupdialog.h"

using namespace DShowLib;

camerasetupdialog::camerasetupdialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::camerasetupdialog)
{
	settingsFile = QApplication::applicationDirPath()+"/cameraSettings.ini";
	QSettings cameraSetting(settingsFile,QSettings::IniFormat);
	ui->setupUi(this);
	// Load camera settings
	if(QFile(settingsFile).exists())
	{
		Grabber *m_pGrabber = new Grabber();
		// Camera Index
		ui->cameraLabel->setText(cameraSetting.value("camIndex").toString());
		m_pGrabber->openDevByDisplayName(cameraSetting.value("camIndex").toString().toStdWString());
		// Camera format
		ui->formatLabel->setText(cameraSetting.value("camFormat").toString());
		m_pGrabber->setVideoFormat(cameraSetting.value("camFormat").toString().toStdWString());
		//camera Fps
		ui->fpsLabel->setText(cameraSetting.value("camFps").toString());
		m_pGrabber->setFPS(cameraSetting.value("camFps").toDouble());
		// Video duration
		ui->timeLabel->setText(cameraSetting.value("videoDuration").toString());
		cameraSetting.sync();
		m_pGrabber->closeDev();
	}
}

camerasetupdialog::~camerasetupdialog()
{
	delete ui;
}

void camerasetupdialog::on_searchButton_clicked()
{
	ui->cameraList->clear();
	Grabber *m_pGrabber = new Grabber();
	Grabber::tVidCapDevListPtr avlCamera = m_pGrabber->getAvailableVideoCaptureDevices();
	if(avlCamera->size() == 0)
	{
		QMessageBox msgBox;
		msgBox.setText("Cannot open any camera! Check connections.");
		msgBox.exec();
	}
	else
	{
		for(Grabber::tVidCapDevList::iterator it = avlCamera->begin();
			it != avlCamera->end(); ++it)
		{
			ui->cameraList->addItem(QString::fromUtf8(it->c_str()));
		}
	}
}

void camerasetupdialog::on_buttonBox_accepted()
{
	QSettings cameraSetting(settingsFile,QSettings::IniFormat);
	cameraSetting.sync();
}

void camerasetupdialog::on_cameraList_currentIndexChanged(int index)
{
	/* Update video format dependent from the chosen camera */
	camData.cameraName = ui->cameraList->currentText();
	QSettings cameraSetting(settingsFile, QSettings::IniFormat);
	cameraSetting.setValue("camIndex", camData.cameraName);

	DShowLib::Grabber* m_pGrabber = new DShowLib::Grabber();
	m_pGrabber->openDevByDisplayName(camData.cameraName.toStdWString());

	DShowLib::Grabber::tVidFmtListPtr avlFmtList = m_pGrabber->getAvailableVideoFormats();

	ui->formatList->clear();
	for(DShowLib::Grabber::tVidFmtList::iterator it = avlFmtList->begin();
		it != avlFmtList->end(); ++it)
	{
		ui->formatList->addItem(QString::fromUtf8((it->c_str())));
	}

	m_pGrabber->closeDev();
}

void camerasetupdialog::on_formatList_currentIndexChanged(int index)
{
	/* Update camera framerate dependent from the chosen video format */
	//save format on settings file
	camData.cameraFormat = ui->formatList->currentText();
	QSettings cameraSetting(settingsFile,QSettings::IniFormat);
	cameraSetting.setValue("camFormat",camData.cameraFormat);

	//open camera and set format
	DShowLib::Grabber* m_pGrabber = new DShowLib::Grabber();
	m_pGrabber->openDevByDisplayName(camData.cameraName.toStdWString());
	m_pGrabber->setVideoFormat(camData.cameraFormat.toStdWString());

	if(m_pGrabber->isFrameRateListAvailable())
	{
		DShowLib::Grabber::tFPSListPtr avlFpsList = m_pGrabber->getAvailableFPS();
		ui->fpsList->clear();
		for(DShowLib::Grabber::tFPSList::iterator it = avlFpsList->begin();
			it != avlFpsList->end(); ++it)
		{
			ui->fpsList->addItem(QString::number((*it)));
		}
	}
	m_pGrabber->closeDev();
}

void camerasetupdialog::on_fpsList_currentIndexChanged(int index)
{
	/* Set the camera framerate */
	camData.cameraFps = ui->fpsList->currentText().toDouble();
	QSettings cameraSetting(settingsFile, QSettings::IniFormat);
	cameraSetting.setValue("camFps", camData.cameraFps);

	// Set fps to camera
	DShowLib::Grabber* m_pGrabber = new DShowLib::Grabber();
	m_pGrabber->openDevByDisplayName(camData.cameraName.toStdWString());
	m_pGrabber->setFPS(camData.cameraFps);
	m_pGrabber->closeDev();
}

void camerasetupdialog::on_lineEdit_editingFinished()
{
	camData.videoDuration = ui->lineEdit->text().toInt();
	QSettings cameraSetting(settingsFile, QSettings::IniFormat);
	cameraSetting.setValue("videoDuration", camData.videoDuration);
}
