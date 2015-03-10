#ifndef NOTIFYSAMPLEDLG_H
#define NOTIFYSAMPLEDLG_H

#include <QtGui>
#include "ui_NotifySampleDlg.h"

#include "../../../../fedm-classlib/src/FedmIscCore.h"

//void cbsFct1(void* pAny, int iError, unsigned char ucCmd);

class QMenu;
class QSignalMapper;

class NotifySampleDlg : public QMainWindow
{
    Q_OBJECT
    	QSignalMapper *mapper;
public:
    	NotifySampleDlg();
	~NotifySampleDlg();
	
	void startTask();
	
	/* The static callback helper function for FEISC_EVENT_INIT 
	below must know of the actual instance to trigger onProtocolEvent(). 
	setGetClassInstance() does the trick to save the pointer to the current 
	instance which we need by the static callback function protCallBackHelper */
	static NotifySampleDlg* setGetClassInstance(NotifySampleDlg* classInstance);
	/* This static helper function is called by the FEISC_PRT_EVENT event. 
	It triggers the onProtocolEvent signal which can then be used to update the ui. */
	static void protCallBackHelper(int iMsg,int iLen);
	
	/* FEDM_TASK_INIT can take a parameter pAny. The helper function above isn't 
	   needed for this callback */
	static void cbsFct1(void* pAny, int iError, unsigned char ucCmd);
signals:
	void onProtocolEvent(int iMsg,int iLen);
	void onDataEvent(void* pAny, int iError, unsigned char ucCmd);

public slots:
	void displayProtocol(int iMsg,int iLen);
	void displayData(void* pAny, int iError, unsigned char ucCmd);
	void displayError(int iError);
	
	void writeToFileClicked();
	void listenButtonClicked();

private:
	char* getISOMfrCode(unsigned char ucMfr);
	void writeLogFile();
	Ui::NotifySampleDlg ui;
	FEDM_ISCReaderModule*	m_iscReader;
	//FEDM_TASK_INIT		m_taskInit;
	bool m_bRunning;
	char m_cLogTxt[1024];
	char m_cLineTxt[1024];
	char m_cFileName[256];
	int m_iLineCnt;
	
};

#endif
