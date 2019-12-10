#ifndef PATIENTDIALOG_H
#define PATIENTDIALOG_H

#include <QDialog>
#include <QString>
#include <QMessageBox>
#include <QDate>
#include <QStringList>
#include <QDir>
#include <QTextStream>
#include <QChar>
#include <QDebug>

namespace Ui {
class PatientDialog;
}

class PatientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PatientDialog(QWidget *parent = 0,QString callerObject = "NULL");
    ~PatientDialog();

    typedef struct{
        QString ptId;
        QString ptName;
        QString ptSurName;
        QDate ptBDay;
        QString ptSex;
        QString ptAFType;
        QString ptRhyTrea;
        QString ptFreqTrea;
        QString ptAnticTrea;
        QString ptRhyDose;
        QString ptFreqDose;
        QString ptAnticDose;
        QDate ptRhyTime;
        QDate ptFreqTime;
        QDate ptAnticTime;
        QString ptNotes;
    }PatientData;

private slots:
    void on_buttonBox_accepted();

    void on_radioNormal_clicked();

    void on_radioApc_clicked();

    void on_radioAf_clicked();

    void on_IdEdit_editingFinished();

    void on_birthdayEdit_editingFinished();

public slots:
    void getExternalPtData(PatientDialog::PatientData);

private:
    Ui::PatientDialog *ui;
    bool BDayValid = false;
    bool checkPtData();
    bool savePtData();
    PatientData ptIstance;
};

#endif // PATIENTDIALOG_H
