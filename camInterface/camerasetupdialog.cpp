#include "camerasetupdialog.h"
#include "ui_camerasetupdialog.h"

camerasetupdialog::camerasetupdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::camerasetupdialog)
{
    settingsFile = QApplication::applicationDirPath()+"/cameraSettings.ini";
    QSettings cameraSetting(settingsFile,QSettings::IniFormat);
    ui->setupUi(this);
    //load camera settings
    if(QFile(settingsFile).exists())
    {
        //Camera Index
        ui->cameraList->addItem(cameraSetting.value("camIndex").toString());
        ui->cameraList->setCurrentIndex(cameraSetting.value("camIndex").toInt());
        //FPS slider
        on_fpsSlider_valueChanged(cameraSetting.value("camFPS").toInt());
        ui->fpsSlider->setSliderPosition(cameraSetting.value("camFPS").toInt());
        cameraSetting.sync();
    }
}

camerasetupdialog::~camerasetupdialog()
{
    delete ui;
}

void camerasetupdialog::on_searchButton_clicked()
{
    cameraReader *Reader = new cameraReader();
    bool cameraAvailable = false;
    if(!Reader->isInitialized())
    {
        for(int i=0;i<1;i++) //DA SISTEMARE
        {
            if(Reader->checkCamera(i))
            {
                ui->cameraList->addItem(QString::number(i));
                cameraAvailable = true;
            }
            populateFormatList();
        }
        if(!cameraAvailable)
        {
            QMessageBox msgBox;
            msgBox.setText("Cannot open any camera! Check connections.");
            msgBox.exec();
        }
    }
}

void camerasetupdialog::on_fpsSlider_valueChanged(int value)
{
    QSettings cameraSetting(settingsFile,QSettings::IniFormat);
    ui->labelFPS->setText(QString::number(value));
    cameraSetting.setValue("camFPS",value);
}

void camerasetupdialog::on_buttonBox_accepted()
{
    QSettings cameraSetting(settingsFile,QSettings::IniFormat);
    cameraSetting.sync();
    qDebug() << cameraSetting.fileName();
}

void camerasetupdialog::on_cameraList_currentIndexChanged(int index)
{
    QSettings cameraSetting(settingsFile,QSettings::IniFormat);
    cameraSetting.setValue("camIndex",ui->cameraList->currentText());
}

void camerasetupdialog::populateFormatList()
{
    DShowLib::Grabber* m_pGrabber = new DShowLib::Grabber();
    m_pGrabber->openDevByDisplayName("DFK 23UM021");

    DShowLib::Grabber::tVidFmtListPtr avlFmtList = m_pGrabber->getAvailableVideoFormats();

    for(DShowLib::Grabber::tVidFmtList::iterator it = avlFmtList->begin();
        it != avlFmtList->end();++it)
    {
        ui->formatList->addItem(QString::fromUtf8((it->c_str())));
    }
}

void camerasetupdialog::populateFPSList()
{
    DShowLib::Grabber* m_pGrabber = new DShowLib::Grabber();
    m_pGrabber->openDevByDisplayName("DFK 23UM021");

    if(m_pGrabber->isFrameRateListAvailable())
    {
        DShowLib::Grabber::tFPSListPtr avlFpsList = m_pGrabber->getAvailableFPS();
        for(DShowLib::Grabber::tFPSList::iterator it = avlFpsList->begin();
            it != avlFpsList->end();++it)
        {
            ui->fpsList->addItem(QString::number((*it)));
        }
    }
    m_pGrabber->closeDev();
}
