#ifndef CAMERASETUPDIALOG_H
#define CAMERASETUPDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QMessageBox>
#include "cvcamerareader.h"
#include <QDir>
#include <QDebug>
#include <QFile>


namespace Ui {
class camerasetupdialog;
}

class camerasetupdialog : public QDialog
{
    Q_OBJECT

public:
    explicit camerasetupdialog(QWidget *parent = 0);
    ~camerasetupdialog();

private slots:
    void on_searchButton_clicked();

    void on_fpsSlider_valueChanged(int value);

    void on_buttonBox_accepted();

    void on_cameraList_currentIndexChanged(int index);

private:
    Ui::camerasetupdialog *ui;
    QString settingsFile;
};

#endif // CAMERASETUPDIALOG_H
