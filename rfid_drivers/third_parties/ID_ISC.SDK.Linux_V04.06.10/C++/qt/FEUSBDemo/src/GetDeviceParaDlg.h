#ifndef GETDEVICEPARADLG_H
#define GETDEVICEPARADLG_H

#include <QDialog>
#include <QMessageBox>
#include "ui_GetDeviceParaDlg.h"

class GetDeviceParaDlg: public QDialog
{
    Q_OBJECT
public:
    GetDeviceParaDlg(QWidget *parent = 0);
    ~GetDeviceParaDlg();
    Ui::GetDeviceParaDlg ui;    
};


#endif

