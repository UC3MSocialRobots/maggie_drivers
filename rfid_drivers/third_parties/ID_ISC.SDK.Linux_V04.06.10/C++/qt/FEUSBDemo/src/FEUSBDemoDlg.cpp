/*-------------------------------------------------------
|                                                       |
|                  FEUSBDemoDlg.cpp                     |
|                                                       |
---------------------------------------------------------

Copyright  2007		FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : info@feig.de
						Internet : http://www.feig.de
					
Author         		:	Benjamin Stadin
Begin        		:	10.12.2006

Version       		:	01.00.00 / 24.01.2007 / Benjamin Stadin

Operation Systems	:	Linux
*/

#include <QMainWindow>
#include <QApplication>
#include "FEUSBDemoDlg.h"
#include "ScanAndOpenDlg.h"
#include "GetScanListParaDlg.h"
#include "SetDeviceParaDlg.h"
#include "GetDeviceParaDlg.h"
#include "DeviceIDDlg.h"
#include "DeviceHandleDlg.h"
#include "TransceiveDlg.h"
#include <usb.h>

//------------------------------------------------------------------------------
// Name: FEUSBDemoDlg() 
// Desc: constructor
//------------------------------------------------------------------------------
FEUSBDemoDlg::FEUSBDemoDlg()
{
	ui.setupUi(this);
	setObjectName("FEUSBDemoDlg");
	setWindowTitle("FEIG ELECTRONIC GmbH - FEUSB Demo");
	
	// Scan functions
	connect(ui.scanButton, SIGNAL(clicked()), this, SLOT(scanButtonClicked()));
	connect(ui.scanAndOpenButton, SIGNAL(clicked()), this, SLOT(scanAndOpenButtonClicked()));
	connect(ui.getScanListParaButton, SIGNAL(clicked()), this, SLOT(getScanListParaButtonClicked()));
	connect(ui.getScanListSizeButton, SIGNAL(clicked()), this, SLOT(getScanListSizeButtonClicked()));    
	connect(ui.clearScanListButton, SIGNAL(clicked()), this, SLOT(clearScanListButtonClicked()));
	
	// Device functions
	connect(ui.openDeviceButton, SIGNAL(clicked()), this, SLOT(openDeviceButtonClicked()));   
	connect(ui.closeDeviceButton, SIGNAL(clicked()), this, SLOT(closeDeviceButtonClicked()));
	connect(ui.getDeviceParaButton, SIGNAL(clicked()), this, SLOT(getDeviceParaButtonClicked()));
	connect(ui.setDeviceParaButton, SIGNAL(clicked()), this, SLOT(setDeviceParaButtonClicked()));
	connect(ui.transceiveButton, SIGNAL(clicked()), this, SLOT(transceiveButtonClicked()));
	connect(ui.getDeviceListButton, SIGNAL(clicked()), this, SLOT(getDeviceListButtonClicked()));
	connect(ui.getDeviceHandleButton, SIGNAL(clicked()), this, SLOT(getDeviceHandleButtonClicked()));
	connect(ui.isDevicePresentButton, SIGNAL(clicked()), this, SLOT(isDevicePresentButtonClicked()));
	connect(ui.versionButton, SIGNAL(clicked()), this, SLOT(versionButtonClicked()));
	connect(ui.exitButton, SIGNAL(clicked()), this, SLOT(close()));
	
	// static helper function for callbacks to indirect callback fcts to current class instance
	setGetClassInstance(this);
	// callbacks for device removal / insertion
	connect(this, SIGNAL(onConnect(int, unsigned int)), this, SLOT(connectEvent(int, unsigned int)));
	connect(this, SIGNAL(onDisconnect(int, unsigned int)), this, SLOT(disconnectEvent(int, unsigned int)));
	
	FEUSB_EVENT_INIT init;
	init.uiFlag = FEUSB_CALLBACK;
	init.uiUse = FEUSB_DEV_CONNECT_EVENT; //FEUSB_DEV_DISCONNECT_EVENT
	init.cbFct = &connectEventHelper;	
	
	FEUSB_EVENT_INIT init2;
	init2.uiFlag = FEUSB_CALLBACK;
	init2.uiUse = FEUSB_DEV_DISCONNECT_EVENT;
	init2.cbFct = &disconnectEventHelper;
	FEUSB_AddEventHandler(0,&init);
	FEUSB_AddEventHandler(0,&init2);
	
	// hide the standard qt column index on the left of the scanTable
	QHeaderView *h = ui.scanTable->verticalHeader();
	h->hide();
}

//------------------------------------------------------------------------------
// Name: FEUSBDemoDlg() 
// Desc: destructor
//------------------------------------------------------------------------------
FEUSBDemoDlg::~FEUSBDemoDlg()
{
	//DelEventHandler -> Connect oder DisConnect
	FEUSB_EVENT_INIT init3;
	init3.uiFlag = FEUSB_CALLBACK;
	init3.uiUse = FEUSB_DEV_DISCONNECT_EVENT; 
	init3.cbFct = &disconnectEventHelper;

	FEUSB_DelEventHandler(0,&init3);
	
	FEUSB_EVENT_INIT init4;
	init4.uiFlag = FEUSB_CALLBACK;
	init4.uiUse = FEUSB_DEV_CONNECT_EVENT; 
	init4.cbFct = &connectEventHelper;

	FEUSB_DelEventHandler(0,&init4);
}

//------------------------------------------------------------------------------
// Name: buildScanList() 
// Desc: builds and displays the scan list table
//------------------------------------------------------------------------------
void FEUSBDemoDlg::buildScanList()
{
	char cValue[32];
	int iScanCnt = 0;
	int iCnt;
	int iDevHnd;
	unsigned long dwDeviceID;
	char tmpStr[256];
	QString str;
	QString sMsg;
	QTableWidgetItem *item;

	ui.scanTable->clearContents();
	
	iScanCnt = FEUSB_GetScanListSize();
	sprintf(tmpStr, "%d", iScanCnt);
	
	if(iScanCnt > 0)
	{
		ui.scanTable->setRowCount(iScanCnt);
		
		for(iCnt=0; iCnt<iScanCnt; iCnt++)
		{
			sprintf(tmpStr, "%d", iCnt);
			str = tmpStr;
			item = ui.scanTable->item(iCnt, 0);
			if (!item)
				ui.scanTable->setItem(iCnt, 0, new QTableWidgetItem(tmpStr));
			else
				item->setText(tmpStr);
			// DeviceID 
			int iBack = FEUSB_GetScanListPara(iCnt, "DEVICE-ID", cValue);
			if(iBack)
			{
				ui.returnStatuslineEdit->setText("Error in FEUSB_GetScanListPara(..DEVICE-ID..)");
			}
			else
			{
				sscanf((const char*)cValue, "%lx",&dwDeviceID);
				sprintf(tmpStr, "%lu", dwDeviceID);
				str = tmpStr;
				item = ui.scanTable->item(iCnt, 1);
				ui.scanTable->setItem(iCnt, 1, new QTableWidgetItem(tmpStr));
				iDevHnd = FEUSB_GetDeviceHnd(dwDeviceID);
				if(iDevHnd >= 0)
				{
					sprintf(tmpStr, "%d", iDevHnd);
					str = tmpStr;
					item = ui.scanTable->item(iCnt, 2);
					ui.scanTable->setItem(iCnt, 2, new QTableWidgetItem(tmpStr));
					iBack = FEUSB_GetScanListPara(iCnt, "PRESENT", cValue);
					if(iBack)
					{
						ui.returnStatuslineEdit->setText("Error in FEUSB_GetScanListPara(..PRESENT..)");
					}
					else
					{
						int iPresent = atoi(cValue);
						if(iPresent)
						{
							item = ui.scanTable->item(iCnt, 3);
							ui.scanTable->setItem(iCnt, 3, new QTableWidgetItem("yes"));
						}
						else
						{
							item = ui.scanTable->item(iCnt, 3);
								ui.scanTable->setItem(iCnt, 3, new QTableWidgetItem("no"));
						}
					}
				}
			}
		}
	}	
}

//------------------------------------------------------------------------------
// Name: scanButtonClicked() 
// Desc: displays found devices
//------------------------------------------------------------------------------
void FEUSBDemoDlg::scanButtonClicked()
{
	scan(false);
}

//------------------------------------------------------------------------------
// Name: scan() 
// Desc: scans the usb bus. If bOpen is true the device(s) will be opened
//------------------------------------------------------------------------------
void FEUSBDemoDlg::scan(bool bOpen)
{
	int iBack = 0;
	int iScanOpt = 0;
	char *sDeviceID;
	char *sFamilyID;
	char *sProductID;
	ScanAndOpenDlg dlg(this);
	char tmp[16] = {0};
	
	if (dlg.exec() != QDialog::Accepted)
		return;
	
	iScanOpt = dlg.ui.scanOptionComboBox->currentIndex();
	sDeviceID = qstrdup(dlg.ui.deviceIDLineEdit->text().toLatin1());
	sFamilyID = qstrdup(dlg.ui.familyIDLineEdit->text().toLatin1());
	sProductID = qstrdup(dlg.ui.productIDLineEdit->text().toLatin1());

	
	if(iScanOpt == 0)
		m_iScanOpt = FEUSB_SCAN_FIRST;
		
	if(iScanOpt == 1)
		m_iScanOpt = FEUSB_SCAN_NEXT;

	if(iScanOpt == 2)
		m_iScanOpt = FEUSB_SCAN_NEW;

	if(iScanOpt == 3)
		m_iScanOpt = FEUSB_SCAN_ALL;
	
	if(dlg.ui.scanPackCheckBox->checkState())
		m_iScanOpt |= FEUSB_SCAN_PACK;
	
	m_SearchOpt.iMask		= 0;
	m_SearchOpt.cFamilyName[0]	= '\0';
	m_SearchOpt.cDeviceName[0]	= '\0';
	m_SearchOpt.cInterface[0]	= '\0';
	m_SearchOpt.cDeviceID[0]	= '\0';

	if(strlen(sFamilyID) > 0)
	{
		m_SearchOpt.iMask |= FEUSB_SEARCH_FAMILY;
		strcpy(m_SearchOpt.cFamilyName, sFamilyID);
		m_iScanOpt |= FEUSB_SCAN_SEARCH;
	}
	if(strlen(sProductID) > 0)
	{
		m_SearchOpt.iMask |= FEUSB_SEARCH_PRODUCT;
		strcpy(m_SearchOpt.cDeviceName, sProductID);
		m_iScanOpt |= FEUSB_SCAN_SEARCH;
	}
	if(strlen(sDeviceID) > 0)
	{
		m_SearchOpt.iMask |= FEUSB_SEARCH_DEVICEID;
		unsigned long dwDeviceID = 0;
		sscanf((const char*)sDeviceID, "%lx", &dwDeviceID);
		char sHexDeviceID[64];
		sprintf(sHexDeviceID, "%08X", dwDeviceID);
		strcpy(m_SearchOpt.cDeviceID, sHexDeviceID);
		m_iScanOpt |= FEUSB_SCAN_SEARCH;
	}

	if(bOpen)
	{
		iBack = FEUSB_ScanAndOpen(m_iScanOpt, &m_SearchOpt);
	}
	else
	{
		iBack = FEUSB_Scan(m_iScanOpt, &m_SearchOpt);
	}
	
	delete sDeviceID;
	delete sFamilyID;
	delete sProductID;
	
	if (iBack < 0)
	{
		QString errString = "";
		char cErrorText[256] = {0};
		FEUSB_GetErrorText(iBack, cErrorText);
		errString.sprintf("Error: %d : %s", iBack, cErrorText);
		ui.returnStatuslineEdit->setText(errString);
	}
	else
	{
		sprintf(tmp, "%d", iBack);
		ui.returnStatuslineEdit->setText(tmp);
	}
	buildScanList();
}

//------------------------------------------------------------------------------
// Name: scanAndOpenButtonClicked() 
// Desc: scans and opens devices and displays them
//------------------------------------------------------------------------------
void FEUSBDemoDlg::scanAndOpenButtonClicked()
{
	scan(true);
}

//------------------------------------------------------------------------------
// Name: getScanListParaButtonClicked() 
// Desc: shows a dialog for scan list parameters
//------------------------------------------------------------------------------
void FEUSBDemoDlg::getScanListParaButtonClicked()
{
	int iBack = 0;
	int idx;
	char *cParaID;
	char *sIdx;
	char cValue[256] = {0};
	QString str;
	GetScanListParaDlg dlg(this);
	
	int row = ui.scanTable->currentRow();
	QTableWidgetItem *item; item = ui.scanTable->item(row, 0);
	if (item)
		str = item->text();
	else
		str = "";
		
	dlg.ui.scanlistIndexLineEdit->setText(str);

	if (dlg.exec() != QDialog::Accepted)
		return;
		
	sIdx = qstrdup(dlg.ui.scanlistIndexLineEdit->text().toLatin1());	
	cParaID = qstrdup(dlg.ui.parameterIDComboBox->currentText().toLatin1());	
	idx = atoi(sIdx);
	iBack = FEUSB_GetScanListPara(idx, cParaID, cValue);
	if (iBack < 0)
	{
		QString errString = "";
		char cErrorText[256] = {0};
		FEUSB_GetErrorText(iBack, cErrorText);
		errString.sprintf("Error: %d : %s", iBack, cErrorText);
		ui.returnStatuslineEdit->setText(errString);
	}
	else
	{
		str.sprintf("Search Result for ParameterID = %s, Index=%d ==> %s", cParaID, idx, cValue);
		ui.returnStatuslineEdit->setText(str);
	}
	delete cParaID;
	delete sIdx;
}

//------------------------------------------------------------------------------
// Name: getScanListSizeButtonClicked() 
// Desc: displays the current number of devices in the usb scan list
//------------------------------------------------------------------------------
void FEUSBDemoDlg::getScanListSizeButtonClicked()
{
	QString str;
	char tmp[256];
		
	str = sprintf(tmp, "ScanListSize = %d", FEUSB_GetScanListSize());
	str = tmp;
	ui.returnStatuslineEdit->setText(str);
}

//------------------------------------------------------------------------------
// Name: clearScanListButtonClicked() 
// Desc: clears the internal usb scan list and the display table
//------------------------------------------------------------------------------
void FEUSBDemoDlg::clearScanListButtonClicked()
{
	FEUSB_ClearScanList();
	ui.returnStatuslineEdit->setText("0");
	ui.scanTable->clearContents();
	// seems to be a qt bug, doesn't repaint table
	/*ui.scanTable->repaint();
	ui.scanTable->update();*/
	ui.scanTable->setFocus();
}

//------------------------------------------------------------------------------
// Name: openDeviceButtonClicked() 
// Desc: opens the currently selected device from the table
//------------------------------------------------------------------------------
void FEUSBDemoDlg::openDeviceButtonClicked()
{
	QString str;
	int row;
	QTableWidgetItem *item;
	QString sId = "";
	DeviceIDDlg dlg(this);
	unsigned long dwDeviceID;
	char *tmp;
	char res[16] = {0};
	int cnt;
	
	cnt = ui.scanTable->selectedItems().count();
	row = ui.scanTable->currentRow();
	item = ui.scanTable->item(row, 1);
	if (item)
		sId = item->text();
	else
		sId = "";
	
	// Initialize the DeviceIDDlg window with the ID of the current selection
	dlg.ui.deviceIDLineEdit->setText(sId);
	
	// Open the device
	if (dlg.exec() != QDialog::Accepted)
		return;
	
	// Try to open the device ID
	sId = dlg.ui.deviceIDLineEdit->text();
	tmp = qstrdup(sId.toLatin1());
	sscanf((const char*)tmp, "%lx",&dwDeviceID);
	int iBack = FEUSB_OpenDevice(dwDeviceID);
	if (iBack < 0)
	{
		QString errString = "";
		char cErrorText[256] = {0};
		FEUSB_GetErrorText(iBack, cErrorText);
		errString.sprintf("Error: %d : %s", iBack, cErrorText);
		ui.returnStatuslineEdit->setText(errString);
	}
	else
	{
		sprintf(res, "%d", iBack); 
		ui.returnStatuslineEdit->setText(res);
	}
	delete tmp;
	
	buildScanList();
	
	// Set the table selection in the main window to the device id
	QList<QTableWidgetItem*> items = ui.scanTable->findItems(sId, Qt::MatchExactly);
	// Normally just one device with the same id should be there
	foreach(QTableWidgetItem *item, items)
	{
		ui.scanTable->setItemSelected(item, true);
	}
	
}

//------------------------------------------------------------------------------
// Name: closeDeviceButtonClicked() 
// Desc: closes the selected device from the table
//------------------------------------------------------------------------------
void FEUSBDemoDlg::closeDeviceButtonClicked()
{
	QString str;
	int row;
	QTableWidgetItem *item;
	QString sHandle = "";
	DeviceHandleDlg dlg(this);
	int dwDeviceID;
	char *tmp;
	char res[16] = {0};
	int cnt;
	
	ui.returnStatuslineEdit->setText(res);
	
	cnt = ui.scanTable->selectedItems().count();
	row = ui.scanTable->currentRow();
	item = ui.scanTable->item(row, 2);
	if (item)
		sHandle = item->text();
	else
		sHandle = "";
	
	// Initialize the DeviceIDDlg window with the ID of the current selection
	dlg.ui.deviceHandleLineEdit->setText(sHandle);
	
	// Open the device
	if (dlg.exec() != QDialog::Accepted)
		return;
	
	// Try to open the device ID
	sHandle = dlg.ui.deviceHandleLineEdit->text();
	tmp = qstrdup(sHandle.toLatin1());
	dwDeviceID = atoi(tmp);
	int iBack = FEUSB_CloseDevice(dwDeviceID);
	if (iBack < 0)
	{
		QString errString = "";
		char cErrorText[256] = {0};
		FEUSB_GetErrorText(iBack, cErrorText);
		errString.sprintf("Error: %d : %s", iBack, cErrorText);
		ui.returnStatuslineEdit->setText(errString);
	}
	else
	{
		sprintf(res, "%d", iBack); 
		ui.returnStatuslineEdit->setText(res);
	}
	delete tmp;
	
	buildScanList();
}

//------------------------------------------------------------------------------
// Name: getDeviceParaButtonClicked() 
// Desc: shows a dialog with options to get device parameters from the selected device
//------------------------------------------------------------------------------
void FEUSBDemoDlg::getDeviceParaButtonClicked()
{
	int iBack = 0;
	char *sParaID;
	char *sDevHnd;
	char sValue[256] = {0};
	QString str;
	GetDeviceParaDlg dlg(this);
	int iDevHnd;
	
	int row = ui.scanTable->currentRow();
	QTableWidgetItem *item; item = ui.scanTable->item(row, 2);
	if (item)
		str = item->text();
	else
		str = "";
		
	dlg.ui.deviceHandleLineEdit->setText(str);

	if (dlg.exec() != QDialog::Accepted)
		return;
		
	sDevHnd = qstrdup(dlg.ui.deviceHandleLineEdit->text().toLatin1());	
	sParaID = qstrdup(dlg.ui.parameterIDComboBox->currentText().toLatin1());	
	sscanf((const char*)sDevHnd, "%d", &iDevHnd);
	iBack = FEUSB_GetDevicePara(iDevHnd, sParaID, sValue);
	if (iBack < 0)
	{
		QString errString = "";
		char cErrorText[256] = {0};
		FEUSB_GetErrorText(iBack, cErrorText);
		errString.sprintf("Error: %d : %s", iBack, cErrorText);
		ui.returnStatuslineEdit->setText(errString);
	}
	else
	{
		str.sprintf("Search Result for ParameterID=%s, Handle=%d ==> %s", sParaID, iDevHnd, sValue);
		ui.returnStatuslineEdit->setText(str);
	}
	delete sParaID;
	delete sDevHnd;
}

//------------------------------------------------------------------------------
// Name: setDeviceParaButtonClicked() 
// Desc: -
//------------------------------------------------------------------------------
void FEUSBDemoDlg::setDeviceParaButtonClicked()
{
	int iBack = 0;
	char *sParaID;
	char *sDevHandle;
	char *sValue;
	int iDevHnd;
	QString str;
	SetDeviceParaDlg dlg(this);
	
	int row = ui.scanTable->currentRow();
	QTableWidgetItem *item; item = ui.scanTable->item(row, 2);
	if (item)
		str = item->text();
	else
		str = "";
		
	dlg.ui.deviceHandleLineEdit->setText(str);

	if (dlg.exec() != QDialog::Accepted)
		return;
		
	sDevHandle = qstrdup(dlg.ui.deviceHandleLineEdit->text().toLatin1());	
	sParaID = qstrdup(dlg.ui.parameterIDComboBox->currentText().toLatin1());
	sValue = qstrdup(dlg.ui.parameterValueLineEdit->text().toLatin1());
	sscanf((const char*)sDevHandle, "%d", &iDevHnd);
	iBack = FEUSB_SetDevicePara(iDevHnd, sParaID, sValue);
	if (iBack < 0)
	{
		QString errString = "";
		char cErrorText[256] = {0};
		FEUSB_GetErrorText(iBack, cErrorText);
		errString.sprintf("Error: %d : %s", iBack, cErrorText);
		ui.returnStatuslineEdit->setText(errString);
	}
	else
	{
		str.sprintf("Result for ParameterID=%s, Handle=%d ==> %s", sParaID, iDevHnd, sValue);
		ui.returnStatuslineEdit->setText(str);
	}
	delete sParaID;
	delete sDevHandle;
}

//------------------------------------------------------------------------------
// Name: transceiveButtonClicked() 
// Desc: transmits data to the selected device and displays the received data
//------------------------------------------------------------------------------
void FEUSBDemoDlg::transceiveButtonClicked()
{
	int iBack = 0;
	unsigned char ucRecData[4096] = {0}; // max prot length
	QString str;
	TransceiveDlg dlg(this);
	int deviceHandle;
	
	// Set the device handle in the transceive dialog from the current table selection
	int row = ui.scanTable->currentRow();
	QTableWidgetItem *item; 
	item = ui.scanTable->item(row, 2);
	if (item)
		str = item->text();
	else
		str = "";
		
	dlg.ui.deviceHandleLineEdit->setText(str);

	if (dlg.exec() != QDialog::Accepted)
		return;

	// sw version: 05FF65E5CB
	// Copy values of Transceive dialog and Transceive
	char *cData;
	char *cDevHandle;
	cDevHandle = qstrdup(dlg.ui.deviceHandleLineEdit->text().toLatin1());	
	cData = qstrdup(dlg.ui.dataLineEdit->text().toLatin1());	
	deviceHandle = atoi (cDevHandle);
	
	// Create a hex record twice the size of the input string
	int hexLen = 2 * strlen(cData) + 1;
	unsigned char *ucHexData = (unsigned char*)malloc(hexLen);
	//memset(ucHexData, '\0', hexLen-1);
	int iSendLen = FEDM_ConvHexCharToHexUChar(cData, strlen(cData), ucHexData, hexLen); // <<- böös
		
	char p[32];
	sprintf(p, "FEDM_ConvHexCharToHexUChar() = %d", iSendLen);
	ui.returnStatuslineEdit->setText(p);
	if (iSendLen <0)
		return;

	iBack = FEUSB_Transceive(deviceHandle, FEUSB_INTERFACE_OBID_RCI_V2, 0,
				 ucHexData, iSendLen, ucRecData, sizeof(ucRecData));
	
	if (cData)
		delete cData;
	if (cDevHandle)
		delete cDevHandle;
	if (ucHexData)
		free(ucHexData);

	if(iBack == 0) // No data returned
	{
		char tmp[64];
		sprintf(tmp, "FEUSB_Transceive() = %d", iBack);
		ui.returnStatuslineEdit->setText(tmp);
		return;
	}
	else if (iBack < 0) // Error
	{
		QString errString = "";
		char cErrorText[256] = {0};
		FEUSB_GetErrorText(iBack, cErrorText);
		errString.sprintf("Error: %d : %s", iBack, cErrorText);
		ui.returnStatuslineEdit->setText(errString);
		return;
	}
	
	// Create a hex record twice the size of the returned data and 
	// convert the return value into a hex string
	int hexRecLen = 2 * iBack + 1;
	char *cHexRecData;  
	cHexRecData = (char*)malloc(hexRecLen);
	memset(cHexRecData, 0, hexRecLen);	
	int iHexStrLen;
	if (FEDM_ConvHexUCharToHexChar(ucRecData, iBack, cHexRecData, hexRecLen) == 0)
		iHexStrLen = strlen(cHexRecData);
	else
		iHexStrLen = 0;
	ui.returnStatuslineEdit->setText((char*)cHexRecData);
	str = "";
	for (int iCnt=0; iCnt < iHexStrLen-1; iCnt +=2)
	{
		if (iHexStrLen %2 >= 0) // For convenience make sure the string is large enough
		{
			str += cHexRecData[iCnt];
			str += cHexRecData[iCnt+1];
		}
		if ((iHexStrLen-1) - iCnt > 2)
			str += " ";
	}
	ui.returnStatuslineEdit->setText(str);
	
	if (cHexRecData)
		free(cHexRecData);
}

//------------------------------------------------------------------------------
// Name: getDeviceListButtonClicked() 
// Desc: gets the device list
//------------------------------------------------------------------------------
void FEUSBDemoDlg::getDeviceListButtonClicked()
{
	QString str;
	int row;
	QTableWidgetItem *item;
	QString sHandle = "";
	DeviceHandleDlg dlg(this);
	int deviceHandle;
	char *cHandle;
	char res[64] = {0};
	int cnt;
	
	cnt = ui.scanTable->selectedItems().count();
	row = ui.scanTable->currentRow();
	item = ui.scanTable->item(row, 2);
	if (item)
		sHandle = item->text();
	else
		sHandle = "";
	
	// Initialize the DeviceHandleDlg window with the ID of the current selection
	dlg.ui.deviceHandleLineEdit->setText(sHandle);
	
	// Open the device
	if (dlg.exec() != QDialog::Accepted)
		return;
	
	// Try to open the device ID
	sHandle = dlg.ui.deviceHandleLineEdit->text();
	cHandle = qstrdup(sHandle.toLatin1());
	deviceHandle = atoi(cHandle);
	int iBack = FEUSB_GetDeviceList(deviceHandle);
	if (iBack < 0)
	{
		QString errString = "";
		char cErrorText[256] = {0};
		FEUSB_GetErrorText(iBack, cErrorText);
		errString.sprintf("Error: %d : %s", iBack, cErrorText);
		ui.returnStatuslineEdit->setText(errString);
	}
	else
	{
		sprintf(res, "Next Device Handle after %d is %d", deviceHandle, iBack); 
		ui.returnStatuslineEdit->setText(res);
	}
	delete cHandle;
	
	buildScanList();
}

//------------------------------------------------------------------------------
// Name: getDeviceHandleButtonClicked() 
// Desc: gets the device handle of the selected device
//------------------------------------------------------------------------------
void FEUSBDemoDlg::getDeviceHandleButtonClicked()
{
	QString str;
	int row;
	QTableWidgetItem *item;
	QString sID = "";
	DeviceIDDlg dlg(this);
	unsigned long dwDeviceID;
	char *tmp;
	char res[64] = {0};
	
	row = ui.scanTable->currentRow();
	item = ui.scanTable->item(row, 1);
	if (item)
		sID = item->text();
	else
		sID = "";
	
	// Initialize the DeviceIDDlg window with the ID of the current selection
	dlg.ui.deviceIDLineEdit->setText(sID);
	
	// Open the device
	if (dlg.exec() != QDialog::Accepted)
		return;
	
	// Try to open the device ID
	sID = dlg.ui.deviceIDLineEdit->text();
	tmp = qstrdup(sID.toLatin1());
	sscanf((const char*)tmp, "%lu", &dwDeviceID);
	delete tmp;
	int iBack = FEUSB_GetDeviceHnd(dwDeviceID);
	if (iBack < 0)
	{
		QString errString = "";
		char cErrorText[256] = {0};
		FEUSB_GetErrorText(iBack, cErrorText);
		errString.sprintf("Error: %d : %s", iBack, cErrorText);
		ui.returnStatuslineEdit->setText(errString);
	}
	else
	{
		sprintf(res, "Device Handle for DeviceID %lu is %d", dwDeviceID, iBack); 
		ui.returnStatuslineEdit->setText(res);
	}
	
	buildScanList();
}

//------------------------------------------------------------------------------
// Name: static setGetClassInstance(FEUSBDemoDlg* classInstance)
// Desc: set / get function to share a pointer of the class instance of the UI with the static callback.
// Return: Null if new pointer is set, or a pointer to the instance if instance == NULL 
//------------------------------------------------------------------------------
FEUSBDemoDlg* FEUSBDemoDlg::setGetClassInstance(FEUSBDemoDlg* classInstance)
{
	static FEUSBDemoDlg* statInstance = NULL;
	
	if (classInstance != NULL)
	{
		statInstance = classInstance;
		return NULL;
	}
	else
	{
		return statInstance;
	}
}

//------------------------------------------------------------------------------
// Name: connectEventHelper(int i, unsigned int k)
// Desc: static helper function to emit a signal for connect event callback
//------------------------------------------------------------------------------
void FEUSBDemoDlg::connectEventHelper(int i, unsigned int k)
{
	FEUSBDemoDlg* classInstance = setGetClassInstance(NULL);
	
	emit(classInstance->onConnect(i, k));
}

//------------------------------------------------------------------------------
// Name: disconnectEventHelper(int i, unsigned int k)
// Desc: static helper function to emit a signal for disconnect event callback
//------------------------------------------------------------------------------
void FEUSBDemoDlg::disconnectEventHelper(int i, unsigned int k)
{
	FEUSBDemoDlg* classInstance = setGetClassInstance(NULL);
	
	emit(classInstance->onDisconnect(i, k));
}

//------------------------------------------------------------------------------
// Name: connectEvent(int i, unsigned int k)
// Desc: callback is triggered when a usb device is plugged in
//------------------------------------------------------------------------------
void FEUSBDemoDlg::connectEvent(int i, unsigned int k)
{
	// You may wish to automatically open or close devices. Ths is the place to do it
	//FEUSB_ScanAndOpen(FEUSB_SCAN_ALL, NULL);
	buildScanList();
	char keks[256] = {0};
	sprintf(keks, "New device with serial number %ld (device handle %d) is connected", k, i);
	ui.returnStatuslineEdit->setText(keks);
	
}

//------------------------------------------------------------------------------
// Name: disconnectEvent(int i, unsigned int k)
// Desc: callback is triggered when a usb device is disconnected
//------------------------------------------------------------------------------
void FEUSBDemoDlg::disconnectEvent(int i, unsigned int k)
{
	/*if(i > 0)
		FEUSB_CloseDevice(i);
	else
		ui.returnStatuslineEdit->setText("disconnect event: device handle = 0");
	*/
	buildScanList();
	char keks[256] = {0};
	sprintf(keks, "Device with serial number %ld (device handle %d) was removed", k, i);
	ui.returnStatuslineEdit->setText(keks);
}

//------------------------------------------------------------------------------
// Name: isDevicePresentButtonClicked() 
// Desc: checks wether a device is present
//------------------------------------------------------------------------------
void FEUSBDemoDlg::isDevicePresentButtonClicked()
{
	QString str;
	int row;
	QTableWidgetItem *item;
	QString sHandle = "";
	DeviceHandleDlg dlg(this);
	unsigned int deviceHandle;
	char *cHandle;
	int cnt;
	
	cnt = ui.scanTable->selectedItems().count();
	row = ui.scanTable->currentRow();
	item = ui.scanTable->item(row, 2);
	if (item)
		sHandle = item->text();
	else
		sHandle = "";
	
	// Initialize the DeviceHandleDlg window with the ID of the current selection
	dlg.ui.deviceHandleLineEdit->setText(sHandle);
	
	// Open the device
	if (dlg.exec() != QDialog::Accepted)
		return;
	
	// Try to open the device ID
	sHandle = dlg.ui.deviceHandleLineEdit->text();
	cHandle = qstrdup(sHandle.toLatin1());
	deviceHandle = atoi(cHandle);
	int iBack = FEUSB_IsDevicePresent(deviceHandle);
	if (iBack > 0)
		ui.returnStatuslineEdit->setText("Device is present");
	else if (iBack == 0)
		ui.returnStatuslineEdit->setText("Device is not present");
	else
	{
		QString errString = "";
		char cErrorText[256] = {0};
		FEUSB_GetErrorText(iBack, cErrorText);
		errString.sprintf("Error: %d : %s", iBack, cErrorText);
		ui.returnStatuslineEdit->setText(errString);
	}
	delete cHandle;
	
	buildScanList();
}

//------------------------------------------------------------------------------
// Name: versionButtonClicked() 
// Desc: displays the feusb version
//------------------------------------------------------------------------------
void FEUSBDemoDlg::versionButtonClicked()
{
	char cVersion[100] = "";
	FEUSB_GetDLLVersion(cVersion);
	ui.returnStatuslineEdit->setText((char*)cVersion);
}

