#ifndef BRMDEMODLG_H
#define BRMDEMODLG_H

#include <QtGui>
#include <pthread.h>
#include "ui_BRMDemoDlg.h"
#include "FEUITable.h"
#include "FEDetectReaderDlg.h"
#include "FEReadTagsThread.h"

#include "../../../../fedm-classlib/src/FedmIscCore.h"


class QMenu;
class QSignalMapper;

class BRMDemoDlg : public QMainWindow
{
    Q_OBJECT
    	QSignalMapper *mapper;
public:
    	BRMDemoDlg();
	~BRMDemoDlg();

	/* The static callback helper function below must know of the actual instance
	to trigger onProtocolEvent(). setGetClassInstance() does the trick to save 
	the pointer to the current instance which we need by the static callback function 
	protCallBackHelper */
	static BRMDemoDlg* setGetClassInstance(BRMDemoDlg* classInstance);
	/* This static helper function is called by the FEISC_PRT_EVENT event. 
	It triggers the onProtocolEvent signal which can then be used to update the ui. */
	static void protCallBackHelper(int iMsg,int iLen);
signals:
	void onProtocolEvent(int iMsg,int iLen);

public slots:
	void displayProtocol(int iMsg,int iLen);
	// signals triggered by thread
	void displayData();
	void displayError(int);
	
	void startButtonClicked();
	void clearTagList();
	void initReader();
	void showAbout();

private:
	Ui::BRMDemoDlg ui;
	int m_iListItem;
	void setupMenuBar();
	void OnDataProt21();
	void OnDataProt22();
	void GetISOMfrCode(unsigned char ucMfr, char* sMfr);
	FEDetectReaderDlg detectReaderDlg;
	FEUITable* tagTable;
	FEReadTagsThread *m_readTagsThread;
	FEDM_ISCReaderModule*	m_iscReader;
	FEDM_TASK_INIT		m_taskInit;
	
protected:
	void resizeEvent(QResizeEvent *event);
};

#endif
