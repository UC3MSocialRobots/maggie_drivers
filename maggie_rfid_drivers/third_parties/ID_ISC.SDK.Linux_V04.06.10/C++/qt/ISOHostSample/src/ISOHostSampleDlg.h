#ifndef ISOHOSTSAMPLEDLG_H
#define ISOHOSTSAMPLEDLG_H

#include <QtGui>
#include <pthread.h>
#include "ui_ISOHostSampleDlg.h"
#include "FEUITable.h"
#include "FEUISimpleHexTable.h"
#include "FEDetectReaderDlg.h"
#include "FEReadTagsThread.h"

#include "../../../../fedm-classlib/src/FedmIscCore.h"


class QMenu;
class QSignalMapper;

class ISOHostSampleDlg : public QMainWindow
{
    Q_OBJECT
    	QSignalMapper *mapper;
public:
    	ISOHostSampleDlg();
	~ISOHostSampleDlg();

	void GetISOMfrCode(unsigned char ucMfr, char* sMfr);
	/* The static callback helper function below must know of the actual instance
	to trigger onProtocolEvent(). setGetClassInstance() does the trick to save 
	the pointer to the current instance which we need by the static callback function 
	protCallBackHelper */
	static ISOHostSampleDlg* setGetClassInstance(ISOHostSampleDlg* classInstance);
	/* This static helper function is called by the FEISC_PRT_EVENT event. 
	It triggers the onProtocolEvent signal which can then be used to update the ui. */
	static void protCallBackHelper(int iMsg,int iLen);
	static void disconnectUSBEventHelper(int i, unsigned int k);
signals:
	void onProtocolEvent(int iMsg,int iLen);
	void onUSBDisconnect(int i, unsigned int k);

public slots:
	// signals triggered by thread
	void displayProtocol(int iMsg,int iLen);
	void disconnectUSBEvent(int i, unsigned int k);
	void displayNewTag();
	void displayError(int);
	void displayNoTag();
	
	void startButtonClicked();
	void readTagButtonClicked();
	void writeTagButtonClicked();
	void clearTagList();
	void initReader();
	void showAbout();

private:
	Ui::ISOHostSampleDlg ui;
	void setupMenuBar();
	FEDetectReaderDlg detectReaderDlg;
	FEUITable* tagTable;
	FEUISimpleHexTable* hexTable;
	FEReadTagsThread *m_readTagsThread;
	FEDM_ISCReaderModule*	m_iscReader;
	FEDM_TASK_INIT		m_taskInit;
	
protected:
	void resizeEvent(QResizeEvent *event);
};

#endif
