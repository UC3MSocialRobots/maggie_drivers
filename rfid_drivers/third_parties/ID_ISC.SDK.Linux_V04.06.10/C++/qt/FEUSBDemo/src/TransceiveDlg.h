#ifndef TRANSCEIVEDLG_H
#define TRANSCEIVEDLG_H

#include <QDialog>
#include <QMessageBox>
#include "ui_TransceiveDlg.h"

class TransceiveDlg: public QDialog
{
    Q_OBJECT
public:
    TransceiveDlg(QWidget *parent = 0);
    ~TransceiveDlg();
    Ui::TransceiveDlg ui;    
};


#endif

