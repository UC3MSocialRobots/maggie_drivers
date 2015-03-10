#ifndef SETDEVICEPARADLG_H
#define SETDEVICEPARADLG_H

#include <QDialog>
#include <QMessageBox>
#include "ui_SetDeviceParaDlg.h"

class SetDeviceParaDlg: public QDialog
{
    Q_OBJECT
public:
    SetDeviceParaDlg(QWidget *parent = 0);
    ~SetDeviceParaDlg();
    Ui::SetDeviceParaDlg ui;    
};


#endif

