#ifndef FEUSBDEMODLG_H
#define FEUSBDEMODLG_H

#include <QtGui>
#include "ui_FEUSBDemoDlg.h"
#include "../../../../feusb-lib/include/feusb.h"
#include "../../../../fedm-classlib/src/FedmIscCore.h"

typedef unsigned long DWORD;

class QMenu;
class QSignalMapper;

class FEUSBDemoDlg : public QMainWindow
{
    Q_OBJECT

    QSignalMapper *mapper;

    public:
	FEUSBDemoDlg();
	~FEUSBDemoDlg();
	static FEUSBDemoDlg* setGetClassInstance(FEUSBDemoDlg* classInstance);
	
    signals:
    	void onConnect(int i, unsigned int k);
	void onDisconnect(int i, unsigned int k);
	
    public slots:
	// Scan functions
	void scanButtonClicked();
	void scanAndOpenButtonClicked();
	void getScanListParaButtonClicked();
	void getScanListSizeButtonClicked();
	void clearScanListButtonClicked();
	//Device functions
	void openDeviceButtonClicked();
	void closeDeviceButtonClicked();
	void getDeviceParaButtonClicked();
	void setDeviceParaButtonClicked();
	void transceiveButtonClicked();
	void getDeviceListButtonClicked();
	void getDeviceHandleButtonClicked();
	void isDevicePresentButtonClicked();
	// callbacks, called indirectly by the helper functions below
	void connectEvent(int i, unsigned int k);
	void disconnectEvent(int i, unsigned int k);
	// static helper functions for the callbacks above. called by feusb
	static void connectEventHelper(int i, unsigned int k);
	static void disconnectEventHelper(int i, unsigned int k);
	// NULL
	void versionButtonClicked();
		
    private:
	Ui::FEUSBDemoDlg ui;
	void buildScanList();
	void scan(bool bOpen);
	FEUSB_SCANSEARCH m_SearchOpt;
	int	m_iScanOpt;	
};

#endif
