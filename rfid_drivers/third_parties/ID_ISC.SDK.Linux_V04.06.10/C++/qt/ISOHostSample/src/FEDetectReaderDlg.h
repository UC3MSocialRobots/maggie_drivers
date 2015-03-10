#ifndef FEDLGDETECTREADER_H
#define FEDLGDETECTREADER_H

#include <QDialog>
#include <QMessageBox>
#include "InterfaceConfig.h"
#include "ui_FEDetectReaderDlg.h"
#include "FESerialPortSettingsDlg.h"

#include "../../../../fedm-classlib/src/FedmIscCore.h"

class FEDetectReaderDlg: public QDialog
{
    Q_OBJECT
public:
	FEDetectReaderDlg(QWidget *parent = 0);
	~FEDetectReaderDlg();
	void setReader(FEDM_ISCReaderModule *reader);
	FEDM_ISCReader *getReader();
	bool isReaderDetected();

private slots:
	void onDetectButton();
	void onMoreButton();
	void onOkButton();

private:
	FEDM_ISCReaderModule *m_reader;
    	Ui::FEDetectReaderDlg ui;
	bool m_bReaderDetected;
	InterfaceConfig config;
	FESerialPortSettingsDlg serialPortSettingsDlg;
};

#endif
