#ifndef GETSCANLISTARADLG_H
#define GETSCANLISTARADLG_H

#include <QDialog>
#include <QMessageBox>
#include "ui_GetScanListParaDlg.h"

class GetScanListParaDlg: public QDialog
{
    Q_OBJECT
public:
    GetScanListParaDlg(QWidget *parent = 0);
    ~GetScanListParaDlg();
    Ui::GetScanListParaDlg ui;    
};


#endif

