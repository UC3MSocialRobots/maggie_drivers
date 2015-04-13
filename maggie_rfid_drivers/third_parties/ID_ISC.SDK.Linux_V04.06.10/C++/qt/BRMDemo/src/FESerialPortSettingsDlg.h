#ifndef FESERIALPORTSETTINGS_H
#define FESERIALPORTSETTINGS_H

#include <QDialog>
#include "ui_FESerialPortSettingsDlg.h"


class FESerialPortSettingsDlg: public QDialog
{
    Q_OBJECT
public:
	FESerialPortSettingsDlg(QWidget *parent = 0);
	~FESerialPortSettingsDlg();

	Ui::FESerialPortSettingsDlg ui;
};

#endif
