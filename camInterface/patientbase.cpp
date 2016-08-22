#include "patientbase.h"
#include "ui_patientbase.h"
#include "utilities.h"
#include "patientdialog.h"

patientBase::patientBase(QWidget *parent,QString callerObject) :
    QDialog(parent),
    ui(new Ui::patientBase)
{
    caller = callerObject;
    ui->setupUi(this);
    populatePtList();
    if(caller == "newRec")
    {
        QWidget::setWindowTitle("Select user");
        ui->ptDeleteButton->setEnabled(false);
        ui->ptEditButton->setEnabled(false);
        ui->tabWidget->setTabEnabled(1,false);
    }
}

patientBase::~patientBase()
{
    delete ui;
}

void patientBase::populatePtList()
{
     QString ptPath = QDir::currentPath()+"/users/";
     QString ptFolderPath;
     QString tempPtData;
     QStringList ptStringList;
     QDirIterator iterDir(ptPath,QDir::NoDotAndDotDot|QDir::Dirs, QDirIterator::NoIteratorFlags);

     while(iterDir.hasNext())
     {
         //single patient folder
         ptFolderPath = iterDir.next();

         //open patient file in folder
         tempPtData = ptFolderPath.section("/",-1);
         QSettings ptFile(ptFolderPath+"/ptData.dat",QSettings::IniFormat);
         tempPtData += " " + ptFile.value("Patient name").toString() + " " + ptFile.value("Patient surname").toString();
         ptStringList.append(tempPtData);

     }

     //populate listview
     ptStringList.sort();
     ptListModel->setStringList(ptStringList);
     ui->ptListView->setModel(ptListModel);
}

void patientBase::populateExamList()
{
    QString recFolderPath;
    QStringList recStringList;
    //get user folder
    QString ptFolder = ptListModel->data(ui->ptListView->currentIndex(),Qt::DisplayRole).toString();
    QString ptID = ptFolder.section(" ",0,0);
    ptFolder = QDir::currentPath()+"/users/"+ptID+"/records/";
    //get subfolders
    QDirIterator iterDir(ptFolder,QDir::NoDotAndDotDot|QDir::Files, QDirIterator::NoIteratorFlags);

    while(iterDir.hasNext())
    {
        recFolderPath = iterDir.next().section(".avi",-2,-2).section("/",-1);
        qDebug() << recFolderPath;
        if(recFolderPath.compare("")!=0)
            recStringList.append(recFolderPath);
    }
    //populate listview
    recStringList.sort();
    recListModel->setStringList(recStringList);
    ui->ptExamListView->setModel(recListModel);
}

bool patientBase::getPtData()
{
    QString line, lineType, lineValue;
    //user folder
    QString ptFolder = ptListModel->data(ui->ptListView->currentIndex(),Qt::DisplayRole).toString();
    if(ptFolder.compare("")!=0)
    {
        ptFolder = ptFolder.section(" ",0,0);
        ptIstance.ptId = ptFolder;
        ptFolder = QDir::currentPath()+"/users/"+ ptFolder+"/ptData.dat";
        //open user file
        QSettings ptFile(ptFolder,QSettings::IniFormat);

        //get general data
        ptIstance.ptName = ptFile.value("Patient Name").toString();
        ptIstance.ptSurName = ptFile.value("Patient surname").toString();
        ptIstance.ptSex = ptFile.value("Patient biological sex").toString();
        ptIstance.ptBDay = QDate::fromString(ptFile.value("Patient birthday").toString(),"dd-MM-yyyy");

        //switch top last update group
        ptFile.beginGroup("Update " + ptFile.value("Last update").toString());

        //get remaining data
        ptIstance.ptAFType = ptFile.value("Patient AF type").toString();
        ptIstance.ptRhyTrea = ptFile.value("Rhythm treatment").toString();
        ptIstance.ptRhyDose = ptFile.value("Rhythm dosage").toString();
        ptIstance.ptRhyTime = QDate::fromString(ptFile.value("Rhythm start").toString(),"dd-MM-yyyy");
        ptIstance.ptFreqTrea = ptFile.value("Frequency treatment").toString();
        ptIstance.ptFreqDose = ptFile.value("Frequency dosage").toString();
        ptIstance.ptFreqTime = QDate::fromString(ptFile.value("Frequency start").toString(),"dd-MM-yyyy");
        ptIstance.ptAnticTrea = ptFile.value("Anticoagulant treatment").toString();
        ptIstance.ptAnticDose = ptFile.value("Anticoagulant dosage").toString();
        ptIstance.ptAnticTime = QDate::fromString(ptFile.value("Anticoagulant start").toString(),"dd-MM-yyyy");
        ptIstance.ptNotes = ptFile.value("Notes").toString();
        return true;
    }
    else if(ptFolder.compare("")==0 && caller!="newRec")
    {
        QMessageBox::warning(this,"Error","No patient selected or available for data access");
        this->show();
        return false;
    }
}

void patientBase::on_ptAccessButton_clicked()
{
    getPtData();
    if(ptIstance.ptId != "")
    {
        //Age
        ui->labelValueAge->setText(QString::number(calculateAge(ptIstance.ptBDay,QDate::currentDate())));
        //Sex
        ui->labelValueSex->setText(ptIstance.ptSex);
        //AF
        ui->labelValueAFType->setText(ptIstance.ptAFType);
        //Treatments
        //Rhythm
        if(!ptIstance.ptRhyTrea.compare("NA"))
        {
            ui->labelValueType_r->setText("No Treatment");
            ui->labelValueFrom_r->setText(ptIstance.ptRhyTime.toString("dd-MM-yyyy"));
            ui->tabTreatment->setTabEnabled(0,false);
        }
        else
        {
            ui->tabTreatment->setTabEnabled(0,true);
            ui->labelValueType_r->setText(ptIstance.ptRhyTrea);
        }
        ui->labelValueFrom_r->setText(ptIstance.ptRhyTime.toString("dd-MM-yyyy"));
        ui->labelValueDose_r->setText(ptIstance.ptRhyDose);
        //Frequency
        if(!ptIstance.ptFreqTrea.compare("NA"))
        {
            ui->labelValueType_f->setText("No Treatment");
            ui->labelValueFrom_f->setText(ptIstance.ptRhyTime.toString("dd-MM-yyyy"));
            ui->tabTreatment->setTabEnabled(1,false);
        }
        else
        {
            ui->tabTreatment->setTabEnabled(1,true);
            ui->labelValueType_f->setText(ptIstance.ptFreqTrea);
        }
        ui->labelValueFrom_f->setText(ptIstance.ptFreqTime.toString("dd-MM-yyyy"));
        ui->labelValueDose_f->setText(ptIstance.ptFreqDose);
        //Anticoagulant
        if(!ptIstance.ptAnticTrea.compare("NA"))
        {
            ui->labelValueType_a->setText("No Treatment");
            ui->labelValueFrom_a->setText(ptIstance.ptRhyTime.toString("dd-MM-yyyy"));
            ui->tabTreatment->setTabEnabled(2,false);
        }
        else
        {
            ui->tabTreatment->setTabEnabled(2,true);
            ui->labelValueType_a->setText(ptIstance.ptAnticTrea);
        }
        ui->labelValueFrom_a->setText(ptIstance.ptAnticTime.toString("dd-MM-yyyy"));
        ui->labelValueDose_a->setText(ptIstance.ptAnticDose);

        //populate exams list
        populateExamList();
    }
    else if(caller =="newRec")
    {
        QMessageBox::warning(this,"Error","No patient selected or available for data access");
        this->show();
    }
}

void patientBase::on_ptEditButton_clicked()
{
    if(getPtData())
    {
        PatientDialog *ptDialog = new PatientDialog(this,"editData");
        QObject::connect(this,SIGNAL(sendPtData(PatientDialog::PatientData)),ptDialog,SLOT(getExternalPtData(PatientDialog::PatientData)));
        emit sendPtData(ptIstance);
        ptDialog->show();
    }
}

void patientBase::on_ptNewButton_clicked()
{
    PatientDialog * ptDialog = new PatientDialog(this);
    QObject::connect(ptDialog,SIGNAL(accepted()),this,SLOT(populatePtList()));
    ptDialog->show();
}

void patientBase::on_ptDeleteButton_clicked()
{
    if(getPtData())
    {
        QMessageBox::StandardButton msgBox;
        msgBox = QMessageBox::question(this,"Delete","Are you sure? This operation cannot be undone!",QMessageBox::Yes|QMessageBox::No);

        if(msgBox==QMessageBox::Yes)
        {
            QDir dir;
            QString currentFolder = QDir::currentPath()+"/users/"+ ptIstance.ptId;
            QString deleteFolder = QDir::currentPath()+"/del_users/"+ ptIstance.ptId + "_deleted_"+ QDate::currentDate().toString("dd-MM-yyyy");
            dir.rename(currentFolder,deleteFolder);
            msgBox = QMessageBox::information(this,"Delete executed","Patient with ID " + ptIstance.ptId + " has been deleted");
        }
        populatePtList();
    }
}

void patientBase::on_buttonBox_accepted()
{
    if(caller=="newRec" && getPtData())
    {
        QObject::connect(this,SIGNAL(sendPtData(PatientDialog::PatientData)),this->parent(),SLOT(getExternalPtData(PatientDialog::PatientData)));
        emit sendPtData(ptIstance);
    }
}

//PROCESS VIDEOS

void patientBase::on_pushButton_clicked()
{
    bool processFlag = true;
    //get exam folder
    QString tempData = recListModel->data(ui->ptExamListView->currentIndex(),Qt::DisplayRole).toString();
    QString examFile = QDir::currentPath()+"/users/"+tempData.section("_",1,1)+"/records/" + tempData +
            ".dat";
    qDebug() << examFile;
    QSettings examData(examFile,QSettings::IniFormat);
    qDebug() << examData.value("processed").toString();
    //check if the video has been already processed
    if(examData.value("processed").toString() == "yes")
    {
        QMessageBox::StandardButton msgBox;
        msgBox = QMessageBox::question(this,"Continue processing","The exam has been already processed. Do you want to continue?",
                                       QMessageBox::Yes | QMessageBox::No);
        if(msgBox == QMessageBox::No)
            processFlag = false;
    }

    //start video processing
    if(processFlag)
    {
        process1.setStandardOutputProcess(&process2);
        connect(&process2, SIGNAL(readyReadStandardOutput()),this,SLOT(printOutput()));
        process1.start("ProgSender.exe");
        process2.start("ProgReceiver.exe");
        //process1.waitForFinished(-1);
    }
    /***Video processing***/
    examData.setValue("processed","yes");
    examData.setValue("processDate",QDate::currentDate().toString("dd-MM-yyyy"));
}

void patientBase::on_sigButton_clicked()
{
    qint64 elapsedTime;
    QProcess proc;
    QString command = "python /home/exeless/DEV/QT/camInterface/Codex_HR.py ";
    QString argfile = "/home/exeless/DEV/QT/camInterface/subject1_rest_rgb.csv";
    qDebug() << command+argfile;
    proc.start(command+argfile);
    timerExec->start();
    proc.waitForFinished(-1);
    QString pDebug = proc.readAllStandardOutput();
    qDebug() << pDebug;
    elapsedTime = timerExec->elapsed();
    double elapsedSecs = double(elapsedTime)/1000.0;
    qDebug() << "Execution completed in " + QString::number(elapsedSecs) + " seconds";
}

void patientBase::printOutput()
{
    QByteArray strdata = process2.readAllStandardOutput();
    qDebug() << strdata.data();
}
