#include "patientdialog.h"
#include "ui_patientdialog.h"
#include "patientbase.h"

PatientDialog::PatientDialog(QWidget *parent, QString callerObject) :
    QDialog(parent),
    ui(new Ui::PatientDialog)
{
    ui->setupUi(this);
    //build window from User edit
    if(QString(parent->metaObject()->className()).compare("patientBase")==0 && callerObject=="editData")
    {
        //QObject::connect(parent,SIGNAL(sendPtData(PatientData)),this,SLOT(getExternalPtData(PatientData)));
        ui->IdEdit->setEnabled(false);
        ui->NameEdit->setEnabled(false);
        ui->SurnameEdit->setEnabled(false);
        ui->birthdayEdit->setEnabled(false);
        ui->sexCBox->setEnabled(false);
    }
}


PatientDialog::~PatientDialog()
{
    delete ui;
}

void PatientDialog::on_buttonBox_accepted()
{
    QMessageBox::StandardButton msgBox;
    if(checkPtData())
    {
        msgBox = QMessageBox::question(this,"Continue","Are you sure?",QMessageBox::Yes | QMessageBox::No);
        if(msgBox == QMessageBox::Yes)
        {
            savePtData();
            emit this->accepted();
        }
        else if(msgBox == QMessageBox::No)
            this->show();
    }
    else
    {
        QMessageBox::warning(this,"Error in data","Missing or invalid data, check again.");
        this->show();
    }

}

bool PatientDialog::checkPtData()
{
    //codes and anagraphy
    if(!ui->IdEdit->text().isEmpty())
        ptIstance.ptId = ui->IdEdit->text();
    else
        return false;

    if(!ui->NameEdit->text().isEmpty())
        ptIstance.ptName = ui->NameEdit->text();
    else
        return false;

    if(!ui->SurnameEdit->text().isEmpty())
        ptIstance.ptSurName = ui->SurnameEdit->text();
    else
        return false;

    if(!ui->birthdayEdit->text().isEmpty() && ui->birthdayEdit->text() != "INVALID DATE")
        ptIstance.ptBDay = QDate::fromString(ui->birthdayEdit->text(),"dd-MM-yyyy");
    else
        return false;

    if(!ui->sexCBox->currentText().isEmpty())
        ptIstance.ptSex = ui->sexCBox->currentText()[0];
    else
        return false;

    //treatments
    if(ui->groupRhy->isChecked())
    {
        if(!ui->rhyTreaEdit->text().isEmpty())
            ptIstance.ptRhyTrea = ui->rhyTreaEdit->text();
        else
            return false;

        if(!ui->rhyTreaDose->text().isEmpty())
            ptIstance.ptRhyDose = ui->rhyTreaDose->text();
        else
            return false;

        if(!ui->rhyTreaFrom->text().isEmpty())
            ptIstance.ptRhyTime = QDate::fromString(ui->rhyTreaFrom->text(),"dd-MM-yyyy");
        else
            return false;
    }
    else
    {
        ptIstance.ptRhyTrea = "NA";
        ptIstance.ptRhyDose = "NA";
        ptIstance.ptRhyTime = QDate::fromString("01-01-1900","dd-MM-yyyy");
    }

    if(ui->groupFreq->isChecked())
    {
        if(!ui->freqTreaEdit->text().isEmpty())
            ptIstance.ptFreqTrea = ui->freqTreaEdit->text();
        else
            return false;

        if(!ui->freqTreaDose->text().isEmpty())
            ptIstance.ptFreqDose = ui->freqTreaDose->text();
        else
            return false;

        if(!ui->freqTreaFrom->text().isEmpty())
            ptIstance.ptFreqTime = QDate::fromString(ui->freqTreaFrom->text(),"dd-MM-yyyy");
        else
            return false;
    }
    else
    {
        ptIstance.ptFreqTrea = "NA";
        ptIstance.ptFreqDose = "NA";
        ptIstance.ptFreqTime = QDate::fromString("01-01-1900","dd-MM-yyyy");
    }

    if(ui->groupAntic->isChecked())
    {
        if(!ui->anticTreaEdit->text().isEmpty())
            ptIstance.ptAnticTrea = ui->anticTreaEdit->text();
        else
            return false;

        if(!ui->anticTreaDose->text().isEmpty())
            ptIstance.ptAnticDose = ui->anticTreaDose->text();
        else
            return false;

        if(!ui->anticTreaFrom->text().isEmpty())
            ptIstance.ptAnticTime = QDate::fromString(ui->anticTreaFrom->text(),"dd-MM-yyyy");
        else
            return false;
    }
    else
    {
        ptIstance.ptAnticTrea = "NA";
        ptIstance.ptAnticDose = "NA";
        ptIstance.ptAnticTime = QDate::fromString("01-01-1900","dd-MM-yyyy");
    }

    //MD Notes
    if(!ui->notesEdit->toPlainText().isEmpty())
    {
        ptIstance.ptNotes = ui->notesEdit->toPlainText();
    }
    else
    {
        ptIstance.ptNotes = "No notes available";
    }

    return true;
}

bool PatientDialog::savePtData()
{
    QString ptPath = QDir::currentPath()+"/users/"+ptIstance.ptId+"/";
    //check if folder exists
    if(!QDir(ptPath).exists())
    {
        QDir().mkdir(ptPath);
        QSettings outFile(ptPath+"ptData.dat",QSettings::IniFormat);
        outFile.setValue("Patient name",ptIstance.ptName);
        outFile.setValue("Patient surname",ptIstance.ptSurName);
        outFile.setValue("Patient birthday",ptIstance.ptBDay.toString("dd-MM-yyyy"));
        outFile.setValue("Patient biological sex",ptIstance.ptSex);
        outFile.setValue("Last update",QDate::currentDate().toString("dd-MM-yyyy"));
        outFile.beginGroup("Update " + QDate::currentDate().toString("dd-MM-yyyy"));
        outFile.setValue("Patient AF type",ptIstance.ptAFType);
        outFile.setValue("Rhythm treatment",ptIstance.ptRhyTrea);
        outFile.setValue("Rhythm start",ptIstance.ptRhyTime.toString("dd-MM-yyyy"));
        outFile.setValue("Rhythm dosage",ptIstance.ptRhyDose);
        outFile.setValue("Frequency treatment",ptIstance.ptFreqTrea);
        outFile.setValue("Frequency start",ptIstance.ptFreqTime.toString("dd-MM-yyyy"));
        outFile.setValue("Frequency dosage",ptIstance.ptFreqDose);
        outFile.setValue("Anticoagulant treatment",ptIstance.ptAnticTrea);
        outFile.setValue("Anticoagulant start",ptIstance.ptAnticTime.toString("dd-MM-yyyy"));
        outFile.setValue("Anticoagulant dosage",ptIstance.ptAnticDose);
        outFile.setValue("Notes",ptIstance.ptNotes);
        outFile.endGroup();
        outFile.sync();
        return true;
    }
    else
    {
        QSettings outFile(ptPath+"ptData.dat",QSettings::IniFormat);
        outFile.setValue("Last update",QDate::currentDate().toString("dd-MM-yyyy"));
        outFile.beginGroup("Update " + QDate::currentDate().toString("dd-MM-yyyy"));
        outFile.setValue("Patient AF type",ptIstance.ptAFType);
        outFile.setValue("Rhythm treatment",ptIstance.ptRhyTrea);
        outFile.setValue("Rhythm start",ptIstance.ptRhyTime.toString("dd-MM-yyyy"));
        outFile.setValue("Rhythm dosage",ptIstance.ptRhyDose);
        outFile.setValue("Frequency treatment",ptIstance.ptFreqTrea);
        outFile.setValue("Frequency start",ptIstance.ptFreqTime.toString("dd-MM-yyyy"));
        outFile.setValue("Frequency dosage",ptIstance.ptFreqDose);
        outFile.setValue("Anticoagulant treatment",ptIstance.ptAnticTrea);
        outFile.setValue("Anticoagulant start",ptIstance.ptAnticTime.toString("dd-MM-yyyy"));
        outFile.setValue("Anticoagulant dosage",ptIstance.ptAnticDose);
        outFile.setValue("Notes",ptIstance.ptNotes);
        outFile.endGroup();
        outFile.sync();
        return true;
    }
}

//AF Type button management
void PatientDialog::on_radioNormal_clicked()
{
    ptIstance.ptAFType = "Normal Sinus";
    ui->radioAf->setChecked(false);
    ui->radioApc->setChecked(false);
}

void PatientDialog::on_radioApc_clicked()
{
    ptIstance.ptAFType = "Other arrhytmias";
    ui->radioAf->setChecked(false);
    ui->radioNormal->setChecked(false);
}

void PatientDialog::on_radioAf_clicked()
{
    ptIstance.ptAFType = "Atrial fibrillation";
    ui->radioNormal->setChecked(false);
    ui->radioApc->setChecked(false);
}

void PatientDialog::getExternalPtData(PatientDialog::PatientData extPtIstance)
{
    ptIstance = extPtIstance;
    //UPDATE data
    ui->IdEdit->setText(ptIstance.ptId);
    for(int i=0;i < ui->sexCBox->count();i++)
    {
        ui->sexCBox->setCurrentIndex(i);
        if(ui->sexCBox->currentText().compare(ptIstance.ptSex)==0)
            break;
    }
    ui->NameEdit->setText(ptIstance.ptName);
    ui->SurnameEdit->setText(ptIstance.ptSurName);
    ui->birthdayEdit->setText(ptIstance.ptBDay.toString("dd-MM-yyyy"));
        //AF type
    if(!ptIstance.ptAFType.compare("Normal Sinus"))
        ui->radioNormal->setChecked(true);
    else if (!ptIstance.ptAFType.compare("Atrial fibrillation"))
        ui->radioAf->setChecked(true);
    else
        ui->radioApc->setChecked(true);
        //rhythm
    if(ptIstance.ptRhyTrea.compare("NA"))
    {
        ui->groupRhy->setChecked(true);
        ui->rhyTreaEdit->setText(ptIstance.ptRhyTrea);
        ui->rhyTreaFrom->setText(ptIstance.ptRhyTime.toString("dd-MM-yyyy"));
        ui->rhyTreaDose->setText(ptIstance.ptRhyDose);
    }
        //frequency
    if(ptIstance.ptFreqTrea.compare("NA"))
    {
        ui->groupFreq->setChecked(true);
        ui->freqTreaEdit->setText(ptIstance.ptFreqTrea);
        ui->freqTreaFrom->setText(ptIstance.ptFreqTime.toString("dd-MM-yyyy"));
        ui->freqTreaDose->setText(ptIstance.ptFreqDose);
    }
        //anticoagulants
    if(ptIstance.ptAnticTrea.compare("NA"))
    {
        ui->groupAntic->setChecked(true);
        ui->anticTreaEdit->setText(ptIstance.ptAnticTrea);
        ui->anticTreaFrom->setText(ptIstance.ptAnticTime.toString("dd-MM-yyyy"));
        ui->anticTreaDose->setText(ptIstance.ptAnticDose);
    }


}

void PatientDialog::on_IdEdit_editingFinished()
{
    QString ptPath = QDir::currentPath()+"/users/"+ui->IdEdit->text()+"/";
    if(QDir(ptPath).exists() && ui->IdEdit->text() != "")
    {
        QMessageBox::warning(this,"Id not available","Id already used.Change the patient Id");
        ui->IdEdit->setText("");
        this->show();
    }
}

void PatientDialog::on_birthdayEdit_editingFinished()
{
    if(!QDate::fromString(ui->birthdayEdit->text(),"dd-MM-yyyy").isValid())
    {
        ui->birthdayEdit->setText("INVALID DATE");
    }
}
