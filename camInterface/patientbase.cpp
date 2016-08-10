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
     QFile *ptFile;
     QDirIterator iterDir(ptPath,QDir::NoDotAndDotDot|QDir::Dirs, QDirIterator::NoIteratorFlags);

     while(iterDir.hasNext())
     {
         //single patient folder
         ptFolderPath = iterDir.next();

         //open patient file in folder
         tempPtData = ptFolderPath.section("/",-1);
         ptFile = new QFile(ptFolderPath+"/ptData.txt");
         ptFile->open(QIODevice::ReadOnly);
         QTextStream textstream(ptFile);

         //read name and surname
         for(int i=0;i<2;i++)
         {
             QString line = textstream.readLine();
             tempPtData += " " + line.section("\t",1);
         }
         ptStringList.append(tempPtData);

         ptFile->close();
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
    QDirIterator iterDir(ptFolder,QDir::NoDotAndDotDot|QDir::Dirs, QDirIterator::Subdirectories);

    while(iterDir.hasNext())
    {

        recFolderPath = iterDir.next().section("_",-1);
        qDebug() << recFolderPath;
        recStringList.append(ptID + "_" + recFolderPath);
    }
    //populate listview
    recStringList.sort();
    recListModel->setStringList(recStringList);
    ui->ptExamListView->setModel(recListModel);
}

void patientBase::getPtData()
{
    QFile *ptFile;
    QString line, lineType, lineValue;
    //user folder
    QString ptFolder = ptListModel->data(ui->ptListView->currentIndex(),Qt::DisplayRole).toString();
    ptFolder = ptFolder.section(" ",0,0);
    ptIstance.ptId = ptFolder;
    ptFolder = QDir::currentPath()+"/users/"+ ptFolder+"/ptData.txt";
    //open user file
    ptFile = new QFile(ptFolder);
    ptFile->open(QIODevice::ReadOnly);

    line = ptFile->readLine();
    while(!ptFile->atEnd())
    {
        lineType = line.section("\t",0,0);
        lineValue = line.section("\t",1,1).trimmed();
        //qDebug() << lineType << " " << qHash(lineType);
        //qDebug() << lineValue;

        //control
        switch(qHash(lineType))
        {
        //name and surname
        case 263592885:
            ptIstance.ptName = lineValue;
            break;

        case 254849781:
            ptIstance.ptSurName = lineValue;
            break;

        //calculate Age
        case 252019977:
            ptIstance.ptBDay = QDate::fromString(lineValue,"dd-MM-yyyy");
            break;

        //biological sex
        case 243670376:
            ptIstance.ptSex = lineValue[0];
            break;

        //AF type
        case 263659493:
            ptIstance.ptAFType = lineValue;
            break;

        //Treatments
            //Rhythm
        case 239308660:
            ptIstance.ptRhyTrea = lineValue;
            break;

        case 73055572:
            ptIstance.ptRhyTime = QDate::fromString(lineValue,"dd-MM-yyyy");
            break;

        case 79154005:
            ptIstance.ptRhyDose = lineValue;
            break;

            //Frequency
        case 82829876:
            ptIstance.ptFreqTrea = lineValue;
            break;

        case 40638452:
            ptIstance.ptFreqTime = QDate::fromString(lineValue,"dd-MM-yyyy");
            break;

        case 131948949:
            ptIstance.ptFreqDose = lineValue;
            break;

            //Anticoagulants
        case 3820564:
            ptIstance.ptAnticTrea = lineValue;
            break;

        case 146342708:
            ptIstance.ptAnticTime = QDate::fromString(lineValue,"dd-MM-yyyy");
            break;

        case 176958677:
            ptIstance.ptAnticDose = lineValue;
            break;
        }
        line = ptFile->readLine();
    }

    ptFile->close();
}

void patientBase::on_ptAccessButton_clicked()
{
    getPtData();
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

void patientBase::on_ptEditButton_clicked()
{
    getPtData();
    PatientDialog *ptDialog = new PatientDialog(this,"editData");
    QObject::connect(this,SIGNAL(sendPtData(PatientDialog::PatientData)),ptDialog,SLOT(getExternalPtData(PatientDialog::PatientData)));
    emit sendPtData(ptIstance);
    ptDialog->show();
}

void patientBase::on_ptNewButton_clicked()
{
    PatientDialog * ptDialog = new PatientDialog(this);
    QObject::connect(ptDialog,SIGNAL(accepted()),this,SLOT(populatePtList()));
    ptDialog->show();
}

void patientBase::on_ptDeleteButton_clicked()
{
    getPtData();
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

void patientBase::on_buttonBox_accepted()
{
    if(caller=="newRec")
    {
        getPtData();
        qDebug() << this->parent();
        QObject::connect(this,SIGNAL(sendPtData(PatientDialog::PatientData)),this->parent(),SLOT(getExternalPtData(PatientDialog::PatientData)));
        emit sendPtData(ptIstance);
    }
}

//PROCESS VIDEOS

void patientBase::on_pushButton_clicked()
{
    //get exam folder
    QString tempData = recListModel->data(ui->ptExamListView->currentIndex(),Qt::DisplayRole).toString();
    QString examFile = QDir::currentPath()+"/users/"+tempData.section("_",0,0)+"/records/" + tempData +
            "/examData.dat";
    qDebug() << examFile;
    QSettings examData(examFile,QSettings::NativeFormat);
    qDebug() << examData.value("processed").toString();
    //check if the video has been already processed
    if(examData.value("processed").toString() == "yes")
    {
        QMessageBox::StandardButton msgBox;
        msgBox = QMessageBox::question(this,"Continue processing","The exam has been already processed. Do you want to continue?",
                                       QMessageBox::Yes | QMessageBox::No);
        /***Video processing***/
        examData.setValue("processed","yes");
        examData.setValue("processDate",QDate::currentDate().toString("dd-MM-yyyy"));
    }
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
