#ifndef PATIENTBASE_H
#define PATIENTBASE_H

#include <QDialog>
#include <QDirIterator>
#include <QStringListModel>
#include <QTextStream>
#include <QDebug>
#include <QSettings>
#include <QProcess>
#include <QElapsedTimer>
#include "patientdialog.h"
//#include "mainwindow.h"

namespace Ui {
class patientBase;
}

class patientBase : public QDialog
{
    Q_OBJECT

public:
    explicit patientBase(QWidget *parent = 0, QString callerObject = "NULL");
    ~patientBase();

private slots:
    void on_ptAccessButton_clicked();

    void on_ptEditButton_clicked();

    void on_ptNewButton_clicked();

    void on_buttonBox_accepted();

    void populatePtList();

    void on_pushButton_clicked();

    void on_ptDeleteButton_clicked();

    void on_sigButton_clicked();

    void printOutput();

private:
    Ui::patientBase *ui;
    QStringListModel *ptListModel = new QStringListModel(this);
    QStringListModel *recListModel = new QStringListModel(this);
    PatientDialog::PatientData ptIstance;
    QElapsedTimer *timerExec = new QElapsedTimer();
    void populateExamList();
    bool getPtData();
    QString caller;
    QProcess process1,process2;

signals:
    void sendPtData(PatientDialog::PatientData);

};

#endif // PATIENTBASE_H
