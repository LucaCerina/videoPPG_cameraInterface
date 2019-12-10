#ifndef PATIENTSEARCH_H
#define PATIENTSEARCH_H

#include "patientdialog.h"
namespace Ui {
class PatientSearch;
}

class PatientSearch : public PatientDialog
{
    Q_OBJECT

public:
    explicit PatientSearch(QWidget *parent = 0);
    ~PatientSearch();

private:
    Ui::PatientSearch *ui;

};

#endif // PATIENTSEARCH_H
