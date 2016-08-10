#include "camerasetupdialog.h"
#include "ui_camerasetupdialog.h"

camerasetupdialog::camerasetupdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::camerasetupdialog)
{
    settingsFile = QApplication::applicationDirPath()+"/cameraSettings.conf";
    ui->setupUi(this);
    //load camera settings
    if(QFile(settingsFile).exists())
    {
        QSettings cameraSetting(settingsFile,QSettings::NativeFormat);
        //Camera Index
        ui->cameraList->addItem(cameraSetting.value("camIndex").toString());
        ui->cameraList->setCurrentIndex(cameraSetting.value("camIndex").toInt());
        //FPS slider
        on_fpsSlider_valueChanged(cameraSetting.value("camFPS").toInt());
        ui->fpsSlider->setSliderPosition(cameraSetting.value("camFPS").toInt());
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
    QSettings cameraSetting(settingsFile,QSettings::NativeFormat);
    ui->labelFPS->setText(QString::number(value));
    cameraSetting.setValue("camFPS",value);
}

void camerasetupdialog::on_buttonBox_accepted()
{
    QSettings cameraSetting(settingsFile,QSettings::NativeFormat);
    cameraSetting.sync();
    qDebug() << cameraSetting.fileName();
}

void camerasetupdialog::on_cameraList_currentIndexChanged(int index)
{
    QSettings cameraSetting(settingsFile,QSettings::NativeFormat);
    cameraSetting.setValue("camIndex",index);
}
