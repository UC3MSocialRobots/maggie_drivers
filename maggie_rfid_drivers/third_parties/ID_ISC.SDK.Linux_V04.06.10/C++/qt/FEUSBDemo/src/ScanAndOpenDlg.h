#ifndef SCANANDOPENDLG_H
#define SCANANDOPENDLG_H

#include <QDialog>
#include <QMessageBox>
#include "ui_ScanAndOpenDlg.h"

class ScanAndOpenDlg: public QDialog
{
    Q_OBJECT
public:
    ScanAndOpenDlg(QWidget *parent = 0);
    ~ScanAndOpenDlg();
    Ui::ScanAndOpenDlg ui;    
};


#endif

