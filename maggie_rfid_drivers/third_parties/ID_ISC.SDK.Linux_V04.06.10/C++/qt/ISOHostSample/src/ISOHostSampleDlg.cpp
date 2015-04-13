/*-------------------------------------------------------
|                                                       |
|                  ISOHostSampleDlg.cpp                 |
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
Begin        		:	11.12.2006

Version       		:	01.00.00 / 24.01.2007 / Benjamin Stadin

Operation Systems	:	Linux
*/

#include <QMainWindow>
#include "ISOHostSampleDlg.h"

// FE-includes
#include "ISO_IEC_7816_6_ICManufacturerRegistration.h"
#include "../../../../feisc-lib/include/feisc.h" 
#include "../../../../feusb-lib/include/feusb.h" 


static const char * const aboutMessage = "<p><b>ISOHost Linux Sample - copyright FEIG ELECTRONIC GmbH</b></p>";

//------------------------------------------------------------------------------
// Name: displayNewTag() 
// Desc: displays new tags in the tag table. Triggered by the inventory thread
//------------------------------------------------------------------------------
void ISOHostSampleDlg::displayNewTag()
{
	unsigned char ucTagType = 0;
	unsigned char ucEpcType = 0;
	unsigned char ucSnr[32] = {0};
	int iCnt=0;
	int iBack;
	unsigned int  uiHeader = 0;
	char sSnr[64];
	char sTagType[256] = {0};
	char sTags[256] = {0};
	char sMfr[256] = {0};
	unsigned int uiEPClen;

	//Get TableLength (how many Tags)
	int iLen = m_iscReader->GetTableLength(FEDM_ISC_ISO_TABLE);
	/*if (iLen > tagTable->rowCount())
		tagTable->setRowCount(iLen);*/

	if(iLen == 0)
	{
		char cErrorText[256];
		FEISC_GetLastState(m_iscReader->GetReaderHnd(), cErrorText);
		ui.statusLineEdit->setText(cErrorText);
		clearTagList();
		return;
	}
	else
	{
		tagTable->clear();
		tagTable->setRowCount(iLen);
		for (iCnt= 0; iCnt<iLen; iCnt++)
		{
			//insert serial number into taglist
			iBack = m_iscReader->GetTableData(iCnt,FEDM_ISC_ISO_TABLE,FEDM_ISC_DATA_SNR, ucSnr, sizeof(ucSnr));
			if (iBack == 0)
				iBack = m_iscReader->GetTableData(iCnt,FEDM_ISC_ISO_TABLE,FEDM_ISC_DATA_TRTYPE, &ucTagType);

			if((ucTagType & 0x80) == 0 && iBack == 0)	// HF-Transponder
			{
				switch(ucTagType)
				{
				case FEDM_ISC_TR_TYPE_ICODE1:
					strcpy(sTagType, "Philips I-Code1");
					break;
				
				case FEDM_ISC_TR_TYPE_TAGIT:
					strcpy(sTagType, "Texas Instruments Tag-it HF");
					break;

				case FEDM_ISC_TR_TYPE_ISO15693:
					memset(ucSnr, 0, 8);
					m_iscReader->GetTableData(iCnt, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, ucSnr, 8);
					GetISOMfrCode(ucSnr[1], sMfr);
					sprintf(sTagType, "ISO15693 - %s", sMfr);
					break;
			
				case FEDM_ISC_TR_TYPE_ISO14443A:
					strcpy(sTagType, "ISO14443-A");
					break;
				
				case FEDM_ISC_TR_TYPE_ISO14443B:
					strcpy(sTagType, "ISO14443-B");
					break;

				case FEDM_ISC_TR_TYPE_EPC:
					m_iscReader->GetTableData(iCnt, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_EPC_HEADER, &uiHeader);
					m_iscReader->GetTableData(iCnt, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_EPC_TYPE, &ucEpcType);

					switch(ucEpcType)
					{
					case FEDM_ISC_EPC_TYPE_1:
						strcpy(sTagType, "EPC 96 bit");
						m_iscReader->GetTableData(iCnt, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_EPC, ucSnr);
						break;
					case FEDM_ISC_EPC_TYPE_2:
						strcpy(sTagType, "EPC 64 bit type I");
						m_iscReader->GetTableData(iCnt, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_EPC, ucSnr);
						break;
					case FEDM_ISC_EPC_TYPE_3:
						strcpy(sTagType, "EPC 64 bit type II");
						m_iscReader->GetTableData(iCnt, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_EPC, ucSnr);
						break;
					case FEDM_ISC_EPC_TYPE_4:
						strcpy(sTagType, "EPC 64 bit type III");
						m_iscReader->GetTableData(iCnt, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_EPC, ucSnr);
						break;
					}
					break;

				case FEDM_ISC_TR_TYPE_ICODE_UID:
					strcpy(sTagType, "I-CODE UID");
					break;
				}
			}
			else if((ucTagType & 0x80) == 0x80)	// UHF-Transponder
			{
				switch(ucTagType)
				{
				case FEDM_ISC_TR_TYPE_ISO18000_6_A:
					strcpy(sTagType, "ISO18000-6-A");
					break;
				case FEDM_ISC_TR_TYPE_ISO18000_6_B:
					strcpy(sTagType, "ISO18000-6-B");
					break;
				case FEDM_ISC_TR_TYPE_EM4222:
					strcpy(sTagType, "EM4222");
					break;
				case FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2:
					strcpy(sTagType, "EPC Generation 2");
					break;
				case FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN1:
					strcpy(sTagType, "EPC Class 1");
					break;

				default:  strcpy(sTagType, "unknown");
				}
			}
			m_iscReader->GetTableData(iCnt,FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR_LEN, &uiEPClen);
			//Show in table
			tagTable->edit(iCnt, "TagType", sTagType);
			FEDM_ConvHexUCharToHexChar(ucSnr, uiEPClen, sSnr, sizeof(sSnr));
			tagTable->edit(iCnt, "SerialNumber", sSnr);
		}
		
		if (iLen == 1)
		{
			strcpy(sTags, " 1 Tag in Field ");
		}
		else 
		{
			sprintf(sTags, " %i Tags in Field ", iLen);
		}
		ui.statusLineEdit->setText(sTags);
	}
	
	// Thread is in conditional wait state. Send wake signal to continue
	m_readTagsThread->readTags();
	
}

//------------------------------------------------------------------------------
// Name: displayError(int error)
// Desc: displays an error in the line edit. Triggered by the inventory thread
//------------------------------------------------------------------------------
void ISOHostSampleDlg::displayError(int error)
{
	char sErrorText[256] = {0};
	
	m_iscReader->GetErrorText(sErrorText, error);
	ui.statusLineEdit->setText(sErrorText);
	m_readTagsThread->readTags();
}

//------------------------------------------------------------------------------
// Name: displayNoTag()
// Desc: disyplays messahe if no tag is in field. Triggered by inventory thread
//------------------------------------------------------------------------------
void ISOHostSampleDlg::displayNoTag()
{
	ui.statusLineEdit->setText("No tag in field");
	clearTagList();
	m_readTagsThread->readTags();
}

//------------------------------------------------------------------------------
// Name: GetISOMfrCode(unsigned char ucMfr, char* sMfr)
// Desc: returns a string description of the ISO code
//------------------------------------------------------------------------------
void ISOHostSampleDlg::GetISOMfrCode(unsigned char ucMfr, char* sMfr)
{
	switch(ucMfr)
	{
	case 0x01:
		strcpy(sMfr, FEISC_ISO_MOTOROLA);
		break;
	case 0x02:
		strcpy(sMfr, FEISC_ISO_STM);
		break;
	case 0x03:
		strcpy(sMfr, FEISC_ISO_HITACHI);
		break;
	case 0x04:
		strcpy(sMfr, FEISC_ISO_PHILIPS);
		break;
	case 0x05:
		strcpy(sMfr, FEISC_ISO_INFINEON);
		break;
	case 0x06:
		strcpy(sMfr, FEISC_ISO_CYLINC);
		break;
	case 0x07:
		strcpy(sMfr, FEISC_ISO_TI);
		break;
	case 0x08:
		strcpy(sMfr, FEISC_ISO_FUJITSU);
		break;
	case 0x09:
		strcpy(sMfr, FEISC_ISO_MATSUSHITA);
		break;
	case 0x0A:
		strcpy(sMfr, FEISC_ISO_NEC);
		break;
	case 0x0B:
		strcpy(sMfr, FEISC_ISO_OKI);
		break;
	case 0x0C:
		strcpy(sMfr, FEISC_ISO_TOSHIBA);
		break;
	case 0x0D:
		strcpy(sMfr, FEISC_ISO_MITSUBISHI);
		break;
	case 0x0E:
		strcpy(sMfr, FEISC_ISO_SAMSUNG);
		break;
	case 0x0F:
		strcpy(sMfr, FEISC_ISO_HYUNDAI);
		break;
	case 0x10:
		strcpy(sMfr, FEISC_ISO_LG);
		break;
	case 0x11:
		strcpy(sMfr, FEISC_ISO_EMOSYN);
		break;
	case 0x12:
		strcpy(sMfr, FEISC_ISO_INSIDE);
		break;
	case 0x13:
		strcpy(sMfr, FEISC_ISO_ORGA);
		break;
	case 0x14:
		strcpy(sMfr, FEISC_ISO_SHARP);
		break;
	case 0x15:
		strcpy(sMfr, FEISC_ISO_ATMEL);
		break;
	case 0x16:
		strcpy(sMfr, FEISC_ISO_EM);
		break;
	case 0x17:
		strcpy(sMfr, FEISC_ISO_KSW);
		break;
	case 0x19:
		strcpy(sMfr, FEISC_ISO_XICOR);
		break;
	default:
		strcpy(sMfr, "unknown manufacturer");
	}
}

//------------------------------------------------------------------------------
// Name: clearTagList()
// Desc: clears the tag table
//------------------------------------------------------------------------------
void ISOHostSampleDlg::clearTagList()
{
	tagTable->clear();
	tagTable->setRowCount(256);
}

//------------------------------------------------------------------------------
// Name: ISOHostSampleDlg()
// Desc: constructor
//------------------------------------------------------------------------------
ISOHostSampleDlg::ISOHostSampleDlg()
{
	ui.setupUi(this);
	
	QStringList list;
	list += "TagType";
	list += "SerialNumber";
	tagTable = new FEUITable(list, ui.tagListGroupBox);
	hexTable = new FEUISimpleHexTable(ui.hexDataGroupBox);
	
	// Just fill the hex and tag table with something
	hexTable->setRowCount(256);
	tagTable->setRowCount(256);
	
	setupMenuBar();
	
	m_iscReader = new FEDM_ISCReaderModule;
	
	connect(ui.startPushButton, SIGNAL(clicked()), this, SLOT(startButtonClicked()));
	
	initReader();
	
	m_readTagsThread = new FEReadTagsThread(m_iscReader);
	
	connect(m_readTagsThread, SIGNAL(onNewTag(int)), this, SLOT(displayNewTag()));
	connect(m_readTagsThread, SIGNAL(onError(int)), this, SLOT(displayError(int)));
	connect(m_readTagsThread, SIGNAL(onNoTag(int)), this, SLOT(displayNoTag()));
	connect(ui.clearListPushButton, SIGNAL(clicked()), this, SLOT(clearTagList()));
	connect(ui.clearProtPushButton, SIGNAL(clicked()), ui.protocolTextEdit, SLOT(clear()));
	connect(ui.readTagPushButton, SIGNAL(clicked()), this, SLOT(readTagButtonClicked()));
	connect(ui.writeTagPushButton, SIGNAL(clicked()), this, SLOT(writeTagButtonClicked()));
	
	//AddEventHandler for protocol output to FERichEditCtrl
	int iReaderHnd = m_iscReader->GetReaderHnd();
	FEISC_EVENT_INIT taskInit;
	taskInit.cbFct =	&protCallBackHelper;
	taskInit.uiUse =	FEISC_PRT_EVENT;
	taskInit.uiFlag =	FEISC_CALLBACK;
	int iBack =		FEISC_AddEventHandler (iReaderHnd,&taskInit);
	if (iBack <0)
	{
		char cErrorText[256];
		m_iscReader->GetErrorText(cErrorText, iBack);
		QMessageBox::critical(this, tr("FEISC_AddEventHandler() error"), cErrorText,
						QMessageBox::Abort);
		return;
	}
	// Saves current pointer to class instance which is needed by the static 
	// callback. See ISOHostSampleDlg.h 
	setGetClassInstance(this);
	connect(this, SIGNAL(onProtocolEvent(int,int)), this, SLOT(displayProtocol(int,int)));
	
	// callbach fct for usb device removal. The handle may become invalid when the device is removed
	connect(this, SIGNAL(onUSBDisconnect(int, unsigned int)), this, SLOT(disconnectUSBEvent(int, unsigned int)));
	FEUSB_EVENT_INIT usbInit;
	usbInit.uiFlag = FEUSB_CALLBACK;
	usbInit.uiUse = FEUSB_DEV_DISCONNECT_EVENT;
	usbInit.cbFct = &disconnectUSBEventHelper;
	FEUSB_AddEventHandler(0,&usbInit);
}

//------------------------------------------------------------------------------
// Name: dsconnectUSBEventHelper(int i, unsigned int k)
// Desc: static helper function to emit a signal for USB disconnect event callback
//------------------------------------------------------------------------------
void ISOHostSampleDlg::disconnectUSBEventHelper(int i, unsigned int k)
{
	ISOHostSampleDlg* classInstance = setGetClassInstance(NULL);
	
	emit(classInstance->onUSBDisconnect(i, k));
}

//------------------------------------------------------------------------------
// Name: dsconnectUSBEvent(int i, unsigned int k)
// Desc: called by static helper fct above
//------------------------------------------------------------------------------
void ISOHostSampleDlg::disconnectUSBEvent(int i, unsigned int k)
{
	// Finish the thread
	if (m_readTagsThread->running())
		startButtonClicked();
}

//------------------------------------------------------------------------------
// Name: ~ISOHostSampleDlg()
// Desc: destructor
//------------------------------------------------------------------------------
ISOHostSampleDlg::~ISOHostSampleDlg()
{
	// Delete protocol output handler
	if (m_iscReader)
	{
		int iReaderHnd = m_iscReader->GetReaderHnd();
		if (iReaderHnd > 0)
		{
			FEISC_EVENT_INIT taskInit;
			taskInit.cbFct =	&protCallBackHelper;
			taskInit.uiUse =	FEISC_PRT_EVENT;
			taskInit.uiFlag =	FEISC_CALLBACK;
			FEISC_DelEventHandler (iReaderHnd,&taskInit);
			
			// delete usb disconnect event handler
			FEUSB_EVENT_INIT usbInit;
			usbInit.uiFlag = FEUSB_CALLBACK;
			usbInit.uiUse = FEUSB_DEV_DISCONNECT_EVENT; 
			usbInit.cbFct = &disconnectUSBEventHelper;
			FEUSB_DelEventHandler(0,&usbInit);
		}
	}
	
	if (m_readTagsThread)
	{
		m_readTagsThread->quit();
		sleep(0);
		delete m_readTagsThread;
	}
	
	if (m_iscReader)
		delete m_iscReader;

	if (tagTable)
		delete tagTable;
		
	if (hexTable)
		delete hexTable;
}

//------------------------------------------------------------------------------
// Name: readTagButtonClicked()
// Desc: reads a tag and displays it in the hex table
//------------------------------------------------------------------------------
void ISOHostSampleDlg::readTagButtonClicked()
{
	unsigned char ucReadBlock[1024];		// Byte-Block for Receive-Function
	unsigned char ucBlockSize;		// BlockSize
	unsigned int uiEPClen;
	int row;			// Index of internal table and tagTable (same)
	int i;				// best variable of the world	
	char cErrorText[256] = {0};	// for Messagetext
	unsigned char ucSecStatus = 0;		// SecurityStatus
	int iBack;
	char cSnr[64] = {0};
	int iHexSets = 0;
	unsigned char ucTrType;
	
	hexTable->clear();

	// which tag is selected?
	row = tagTable->currentRow();
	QTableWidgetItem *item = tagTable->item(row, 1);
	if (!item || item->text().length() <= 0)
	{
		ui.statusLineEdit->setText("No tag selected");
		return;
	}

	char* sSnr = qstrdup(item->text().toLatin1());
	strcpy(cSnr, sSnr);
	delete sSnr;
		
	int iDbn;
	char* sDbn = qstrdup(ui.dbnLineEdit->text().toLatin1());
	iDbn = atoi(sDbn);
	delete sDbn;
	
	int iDbAdr;
	char* sDbAdr = qstrdup(ui.dbAdrLineEdit->text().toLatin1());
	iDbAdr = atoi(sDbAdr);
	delete sDbAdr;
	
	m_iscReader->SetData(FEDM_ISC_TMP_B0_REQ_UID, cSnr, strlen(cSnr));
	m_iscReader->GetTableData(row,FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR_LEN, &uiEPClen);
	m_iscReader->SetData(FEDM_ISC_TMP_B0_REQ_UID_LEN,uiEPClen);
	m_iscReader->SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char)0x23);	
	m_iscReader->SetData(FEDM_ISC_TMP_B0_MODE,(unsigned char)0x00); // reset mode byte
	m_iscReader->SetData(FEDM_ISC_TMP_B0_MODE_SEC, (unsigned char)0x01);
	m_iscReader->SetData(FEDM_ISC_TMP_B0_MODE, (unsigned char)FEDM_ISC_ISO_MODE_ADR);
	
	// get tag type
	m_iscReader->GetTableData(row, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_TRTYPE, &ucTrType);
	switch (ucTrType)
	{
		case FEDM_ISC_TR_TYPE_ISO18000_6_B: // ISO18000-6-B
		case FEDM_ISC_TR_TYPE_EPC:
		case FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2:
		case FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN1:
		case 0x86:
			m_iscReader->SetData(FEDM_ISC_TMP_B0_MODE_UID_LF,true);
			m_iscReader->SetData(FEDM_ISC_TMP_B0_MODE_EXT_ADR,(unsigned char)0x01);
			m_iscReader->SetData(FEDM_ISC_TMP_B0_REQ_BANK_BANK_NR,(unsigned char)0x03);
			m_iscReader->SetData(FEDM_ISC_TMP_B0_REQ_BANK_ACCESS_FLAG,(unsigned char)0x00);	
			iHexSets = 256;
			break;
			
		case FEDM_ISC_TR_TYPE_ICODE1: // Philips I-CODE
			iHexSets = 11;
			break;

		case FEDM_ISC_TR_TYPE_TAGIT: // Texas Intruments Tag-it HF
			iHexSets = 8;
			break;
		
		case FEDM_ISC_TR_TYPE_ISO15693: // ISO15693
		case FEDM_ISC_TR_TYPE_ISO14443A: // ISO14443A
		case FEDM_ISC_TR_TYPE_ISO14443B: // ISO14443B
			iHexSets = 256; 
			break;

		default:
			iHexSets = 0;
			break;
	}

	// request data
	m_iscReader->SetData(FEDM_ISC_TMP_B0_REQ_DBN, (unsigned char)iDbn);
	m_iscReader->SetData(FEDM_ISC_TMP_B0_REQ_DB_ADR, (unsigned char)iDbAdr);
	
	// protocol transfer
	iBack = m_iscReader->SendProtocol(0xB0);

	if(iBack < 0)
	{
		m_iscReader->GetErrorText(cErrorText,iBack);
		QMessageBox::critical(this, tr("SendProtocol(0xB0) error"), cErrorText,
					QMessageBox::Abort);
		return;
	}

	if(! (iBack == 0x00 || iBack == 0x83 || iBack == 0x84 || iBack ==0x90 || 
		iBack == 0x93 || iBack == 0x94 || iBack == 0x95 ) )
	{
		m_iscReader->GetStatusText(cErrorText,iBack);
		QMessageBox::critical(this, tr("SendProtocol(0xB0) error"), cErrorText,
					QMessageBox::Abort);
		return;
	}

	// get blocksize
	m_iscReader->GetTableData(row,FEDM_ISC_ISO_TABLE,FEDM_ISC_DATA_BLOCK_SIZE,&ucBlockSize);
	
	char sDbSize[16] = {0};
	sprintf(sDbSize, "%d", int(ucBlockSize));
	ui.dbSizeLineEdit->setText(sDbSize);
	
	hexTable->initTable(iHexSets, (int)ucBlockSize);
	for(i=iDbAdr; i<iDbn+iDbAdr; i++ )
	{
		memset(ucReadBlock, 0, sizeof(ucReadBlock));
		m_iscReader->GetTableData(row, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_RxDB, 
				(unsigned char)i, ucReadBlock, 
				(int) ucBlockSize);	
		m_iscReader->GetTableData(row, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SEC_STATUS, 
				i, &ucSecStatus, 1);				
		
		if(ucSecStatus == 1)
		{
			hexTable->edit(i, ucReadBlock, true);
		}
		else
		{
			hexTable->edit(i, ucReadBlock, false);
		}
	}		
}

//------------------------------------------------------------------------------
// Name: writeTagButtonClicked()
// Desc: writes data to the tag
//------------------------------------------------------------------------------
void ISOHostSampleDlg::writeTagButtonClicked()
{
	unsigned char ucWriteBlock[1024];	// Bytes to write
	unsigned char ucBlockSize;		// BlockSize
	unsigned char ucTrType;
	unsigned int uiEPClen;
	int i;						
	char cErrorText[256] = {0};	// for Messagetext
	int idx;
	int row = 0;
	char cSnr[64] = {0};
	int iDbn;
	int iDbAdr;
	
	row = tagTable->currentRow();
	QTableWidgetItem *item = tagTable->item(row, 1);
	if (!item || item->text().length() <= 0)
	{
		ui.statusLineEdit->setText("No tag selected");
		return;
	}
		 
	// serial no of selected tag
	char* sSnr = qstrdup(item->text().toLatin1());
	strcpy(cSnr, sSnr);
	delete sSnr;
	
	char* sDbn = qstrdup(ui.dbnLineEdit->text().toLatin1());
	iDbn = atoi(sDbn);
	delete sDbn;
	
	char* sDbAdr = qstrdup(ui.dbAdrLineEdit->text().toLatin1());
	iDbAdr = atoi(sDbAdr);
	delete sDbAdr;
	
	m_iscReader->SetData(FEDM_ISC_TMP_B0_REQ_UID, cSnr, strlen(cSnr));
	m_iscReader->GetTableData(row,FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR_LEN, &uiEPClen);
	m_iscReader->SetData(FEDM_ISC_TMP_B0_REQ_UID_LEN,uiEPClen);
	// get blocksize
	m_iscReader->GetTableData(row,FEDM_ISC_ISO_TABLE,FEDM_ISC_DATA_BLOCK_SIZE,&ucBlockSize);
	//Prepare Protocol-Parameter
	m_iscReader->SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char)0x24);
	m_iscReader->SetData(FEDM_ISC_TMP_B0_MODE,(unsigned char)0x00); // reset mode byte
	m_iscReader->SetData(FEDM_ISC_TMP_B0_MODE, (unsigned char)FEDM_ISC_ISO_MODE_ADR);

	// get tag type
	m_iscReader->GetTableData(row, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_TRTYPE, &ucTrType);
	switch (ucTrType)
	{
		case FEDM_ISC_TR_TYPE_ISO18000_6_B: // ISO18000-6-B
		case FEDM_ISC_TR_TYPE_EPC:
		case FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2:
		case FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN1:
		case 0x86:
			m_iscReader->SetData(FEDM_ISC_TMP_B0_MODE_UID_LF,true);
			m_iscReader->SetData(FEDM_ISC_TMP_B0_MODE_EXT_ADR,(unsigned char)0x01);
			m_iscReader->SetData(FEDM_ISC_TMP_B0_REQ_BANK_BANK_NR,(unsigned char)0x03);
			m_iscReader->SetData(FEDM_ISC_TMP_B0_REQ_BANK_ACCESS_FLAG,(unsigned char)0x00);	
			break;
			
		default:
			break;
	}
	
	// request data
	m_iscReader->SetData(FEDM_ISC_TMP_B0_REQ_DBN, (unsigned char)iDbn);
	m_iscReader->SetData(FEDM_ISC_TMP_B0_REQ_DB_ADR, (unsigned char)iDbAdr);
	idx = m_iscReader->FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sSnr);
	
	// get data from Hex Edit 
	for(i=0; i<iDbn; i++ )
	{
		char hexStr [1024] = {0};
		hexTable->getHexString(hexStr, iDbAdr+i);
		FEDM_ConvHexCharToHexUChar(hexStr, strlen(hexStr), ucWriteBlock, int(ucBlockSize));
		m_iscReader->SetTableData(row, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_TxDB, i+iDbAdr, ucWriteBlock, (int)ucBlockSize);
	}
	
	// write data to Tag
	int iBack = m_iscReader->SendProtocol(0xB0);
	if(iBack < 0)
	{
		m_iscReader->GetErrorText(cErrorText, iBack);
				QMessageBox::critical(this, tr("SendProtocol(0xB0) error"), cErrorText,
					QMessageBox::Abort);
		return;
	}

	if(! (iBack == 0x00 || iBack == 0x83 || iBack == 0x84 || iBack ==0x90 || 
		iBack == 0x93 || iBack == 0x94 || iBack == 0x95 ) )
	{
		m_iscReader->GetStatusText(cErrorText, iBack);
		QMessageBox::critical(this, tr("SendProtocol() error"), cErrorText,
					QMessageBox::Abort);
		return;
	}
	
	// copy data from Transmit-Buffer to Receive-Buffer (only for HexEdit)
	for(i=0; i<iDbn; i++ )
	{
		m_iscReader->GetTableData(row,FEDM_ISC_ISO_TABLE,FEDM_ISC_DATA_TxDB, i+iDbAdr, ucWriteBlock, (int) ucBlockSize);
		m_iscReader->SetTableData(row,FEDM_ISC_ISO_TABLE,FEDM_ISC_DATA_RxDB, i+iDbAdr, ucWriteBlock, (int) ucBlockSize);	
	}
}

//------------------------------------------------------------------------------
// Name: resizeEvent(QResizeEvent *event)
// Desc: handles resizing of the custom table
//------------------------------------------------------------------------------
void ISOHostSampleDlg::resizeEvent(QResizeEvent *event)
{
	//event->accept();
	QRect tagRect = ui.tagListGroupBox->geometry();
	int tagTableWidth = tagRect.width() - 18;
	int tagTableHeight = tagRect.height() - 24;
	tagTable->setGeometry(10, 20, tagTableWidth, tagTableHeight);
	
	QRect hexRect = ui.hexDataGroupBox->geometry();
	int hexTableWidth = hexRect.width() - 18;
	int hexTableHeight = hexRect.height() - 24;
	hexTable->setGeometry(10, 20, hexTableWidth, hexTableHeight);
}

//------------------------------------------------------------------------------
// Name: showAbout()
// Desc: shows an "About" message
//------------------------------------------------------------------------------
void ISOHostSampleDlg::showAbout()
{
	QMessageBox::information(this, tr("About"), aboutMessage, QMessageBox::Abort);
}

//------------------------------------------------------------------------------
// Name: setupMenuBar()
// Desc: sets up the menu bar
//------------------------------------------------------------------------------
void ISOHostSampleDlg::setupMenuBar()
{
	QMenu *fileMenu = menuBar()->addMenu(tr("&File"));   
	fileMenu->addAction(tr("&Exit"), this, SLOT(close()));

	QMenu *optionsMenu = menuBar()->addMenu(tr("&Options"));
	optionsMenu->addAction(tr("&Find reader"), this, SLOT(initReader()));    
	
	QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(tr("&About"), this, SLOT(showAbout()));    
}

//------------------------------------------------------------------------------
// Name: initReader()
// Desc: shows the detect reader dialog, initializes the reader and sets callback functions
//------------------------------------------------------------------------------
void ISOHostSampleDlg::initReader()
{
	int iReaderHnd;
	
	detectReaderDlg.setReader(m_iscReader);
	
	if (detectReaderDlg.exec() != QDialog::Accepted)
		return;

	
	iReaderHnd = m_iscReader->GetReaderHnd();
	// can't happen with reader module, left for general purpose anyways
	if(iReaderHnd < 0)
	{
		char cErrorText[256];
		m_iscReader->GetErrorText(cErrorText, iReaderHnd);
    		QMessageBox::critical(this, tr("GetReaderHnd() error "),
                                    cErrorText,
                                    QMessageBox::Abort);
	}
	else
	{
		//Set the TableSize
		m_iscReader->SetTableSize(FEDM_ISC_ISO_TABLE, 512);		
	}	
	
	// log protocol to output file
	//FEISC_SetReaderPara(m_iscReader->GetReaderHnd(), "LogProt", "1");
	
}


//------------------------------------------------------------------------------
// Name: static setGetClassInstance(ISOHostSampleDlg* classInstance)
// Desc: set / get function to share a pointer of the class instance of the UI with the static callback.
// Return: Null if new pointer is set, or a pointer to the instance if instance == NULL 
//------------------------------------------------------------------------------
ISOHostSampleDlg* ISOHostSampleDlg::setGetClassInstance(ISOHostSampleDlg* classInstance)
{
	static ISOHostSampleDlg* statInstance = NULL;
	
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
// Name: static protCallBackHelper(int iMsg,int iLen)
// Desc: delegates between static callback function and UI and triggers the callback
//	 event to show the protocol messages in the protocol window
//------------------------------------------------------------------------------
void ISOHostSampleDlg::protCallBackHelper(int iMsg,int iLen)
{
	ISOHostSampleDlg* classInstance = setGetClassInstance(NULL);
	
	emit(classInstance->onProtocolEvent(iMsg, iLen));
}

//------------------------------------------------------------------------------
// Name: displayProtocol(int iMsg,int iLen)
// Desc: displays protocol messages in the protocol window. Triggered by 
// 	 onProtocolEvent() which is triggered in the static helper function protCallBackHelper()
//------------------------------------------------------------------------------
void ISOHostSampleDlg::displayProtocol(int iMsg,int iLen)
{
	if ((char*)iMsg == NULL)
		return;
		
	int len = strlen((char*)iMsg);
	if (len > 0)
	{
		QString outStr;
		outStr = (char*)iMsg;
		outStr += "\n";
		ui.protocolTextEdit->append(outStr);
	}
}

//------------------------------------------------------------------------------
// Name: startButtonClicked()
// Desc: starts / stops the inventory thread
//------------------------------------------------------------------------------
void ISOHostSampleDlg::startButtonClicked()
{

	if (m_readTagsThread->running())
	{	
		//pause thread
		m_readTagsThread->pauseThread();
		ui.startPushButton->setText("&Start");
		
		//Set Buttons enabled
		ui.readTagPushButton->setEnabled(true);
		ui.writeTagPushButton->setEnabled(true);
	}
	else
	{
		//Set Buttons unenabled
		ui.readTagPushButton->setEnabled(false);
	
		//Delete all tags from the ListView
		tagTable->clear();
		ui.startPushButton->setText("&Stop");
		
		ui.readTagPushButton->setEnabled(false);
		ui.writeTagPushButton->setEnabled(false);
		
		// start thread (or resume if already created)
		m_readTagsThread->readTags();
		
	}
}
