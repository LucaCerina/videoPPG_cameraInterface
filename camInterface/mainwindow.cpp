#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    Reader = new cameraReader();
    QObject::connect(Reader,SIGNAL(processedImage(QImage)),
                     this,SLOT(updateVideoUI(QImage)));
    QObject::connect(Reader,SIGNAL(detectedFace(bool)),
                     this,SLOT(updateDetectedFace(bool)));
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_playButton_clicked()
{
    if(Reader->isStopped())
    {
        Reader->Play();
        ui->playButton->setText(tr("Stop"));
    }
    else
    {
        Reader->Stop();
        ui->playButton->setText(tr("Play"));
    }

}

void MainWindow::on_initButton_clicked()
{
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
        else
        {
            ui->initButton->setText("Init camera");
        }
    }
    else
    {
        if(!Reader->initCamera(ui->cameraList->currentText().toInt()))
        {
            QMessageBox msgBox;
            msgBox.setText("Cannot open camera! Check connection.");
            msgBox.exec();
        }
        else
        {
            ui->playButton->setEnabled(true);
        }
    }
}

void MainWindow::updateVideoUI(QImage img)
{
    if(!img.isNull())
    {
        ui->videoLabel->setPixmap(QPixmap::fromImage(img).scaled(ui->videoLabel->size(),
                                                                 Qt::KeepAspectRatio,Qt::FastTransformation));
    }
}

void MainWindow::updateDetectedFace(bool value)
{
    QPalette sPalette;
    if(value)
    {
        ui->faceLabel->setText("Correct recording");
        sPalette.setColor(QPalette::Window,Qt::green);
        ui->faceLabel->setPalette(sPalette);
    }
    else
    {
        ui->faceLabel->setText("Face not in frame");
        sPalette.setColor(QPalette::Window,Qt::red);
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
    ptBase = new patientBase(this,"sbjSearch");
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
    ptBase = new patientBase(this,"newRec");
    ptBase->exec();

    //check if a subject was selected
    if(PtIstance.ptId != "")
    {
        //enable camera init
        ui->initButton->setEnabled(true);
        //search if camera was already set in settings
        QString settingsFile = QApplication::applicationDirPath()+"/cameraSettings.conf";
        QSettings cameraSetting(settingsFile,QSettings::NativeFormat);
        //check if camera index exists
        if(cameraSetting.contains("camIndex"))
        {
            int camIndex = cameraSetting.value("camIndex").toInt();
            if(camIndex != -1)
            {
                ui->cameraList->addItem(QString::number(camIndex));
                if(Reader->checkCamera(camIndex))
                {
                    qDebug() << "ready to record";
                    ui->playButton->setEnabled(true);
                }
                else
                    qDebug() << "camera not available";
            }
        }
        else
            qDebug() << "No camera index present in settings";
    }
}

void MainWindow::getExternalPtData(PatientDialog::PatientData extPtIstance)
{
    PtIstance = extPtIstance;
    ui->IDvalue->setText(PtIstance.ptId);
    ui->nameValue->setText(PtIstance.ptName);
    ui->surnameValue->setText(PtIstance.ptSurName);
    ui->AFValue->setText(PtIstance.ptAFType);
}
