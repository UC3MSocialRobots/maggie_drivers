#ifndef DEVICEHANDLEDLG_H
#define DEVICEHANDLEDLG_H

#include <QDialog>
#include <QMessageBox>
#include "ui_DeviceHandleDlg.h"

class DeviceHandleDlg: public QDialog
{
    Q_OBJECT
public:
    DeviceHandleDlg(QWidget *parent = 0);
    ~DeviceHandleDlg();
    Ui::DeviceHandleDlg ui;    
};


#endif

