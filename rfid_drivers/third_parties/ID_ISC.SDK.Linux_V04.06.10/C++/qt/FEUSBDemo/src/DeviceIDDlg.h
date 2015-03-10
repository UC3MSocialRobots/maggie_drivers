#ifndef DEVICEIDDLG_H
#define DEVICEIDDLG_H

#include <QDialog>
#include <QMessageBox>
#include "ui_DeviceIDDlg.h"

class DeviceIDDlg: public QDialog
{
    Q_OBJECT
public:
    DeviceIDDlg(QWidget *parent = 0);
    ~DeviceIDDlg();
    Ui::DeviceIDDlg ui;    
};


#endif

