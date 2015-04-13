/*-------------------------------------------------------
|                                                       |
|                  BRMDemoDlg.cpp                       |
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
Begin        		:	10.01.2007

Version       		:	01.00.00 / 24.01.2007 / Benjamin Stadin

Operation Systems	:	Linux
*/

#include <QMainWindow>
#include "BRMDemoDlg.h"

// FE-includes
#include "ISO_IEC_7816_6_ICManufacturerRegistration.h"
#include "../../../../feisc-lib/include/feisc.h" 
#include "../../../../fedm-classlib/src/FedmIscCore.h"

static const char * const aboutMessage = "<p><b>BRM Demo Linux Sample - copyright 2007 FEIG ELECTRONIC GmbH</b></p>";

//------------------------------------------------------------------------------
// Name: displayData() 
// Desc: displays new tags in the tag table. Triggered by inventory thread
//------------------------------------------------------------------------------
void BRMDemoDlg::displayData()
{
	switch(m_iscReader->GetReaderType())
	{
	case FEDM_ISC_TYPE_ISCLR200:
		OnDataProt21();
		break;
	default:
		OnDataProt22();
		break;
	}
	
	// Thread is waiting for ui to update. Calling readTags() again sends signal to wake up
	m_readTagsThread->readTags();
}

void BRMDemoDlg::OnDataProt21()
{
	unsigned char ucTmp;
	unsigned char ucDB[4] = {0};
	unsigned char ucAnt;
	char data[512] = {0};
	//QString str;
	bool bSNR = false;
	bool bDB = false;
	bool bTime = false;
	bool bAnt = false;
	int row = 0;
	int iBack = 0;


	for (int i = 0; i < m_iscReader->GetTableLength(FEDM_ISC_BRM_TABLE); i++) 
	{
		tagTable->newRow();
		row = tagTable->rowCount()-1;
		// Show in ListView
		memset(data, 0, sizeof(data));
		sprintf(data, "%d", row);
		tagTable->edit(row, "No", data);

		m_iscReader->GetData(FEDM_ISCLR_TMP_BRM_TRDATA_SNR,  &bSNR);
		m_iscReader->GetData(FEDM_ISCLR_TMP_BRM_TRDATA_DB,	  &bDB);
		m_iscReader->GetData(FEDM_ISCLR_TMP_BRM_TRDATA_ANT,  &bAnt);
		m_iscReader->GetData(FEDM_ISCLR_TMP_BRM_TRDATA_TIME, &bTime);

		if(bTime)
		{
			unsigned char ucTime[4];

			memset(data, 0, sizeof(data));
			m_iscReader->GetTableData(i, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_TIMER, ucTime, sizeof(ucTime));
			unsigned int uiHours	 = ucTime[0];
			unsigned int uiMinutes	 = ucTime[1];
			unsigned int uiMilliSecs = (ucTime[2]<<8) + ucTime[3];
			unsigned int uiSeconds	 = uiMilliSecs / 1000;
			uiMilliSecs = uiMilliSecs % 1000;
			
			sprintf(data, "%02d:%02d:%02d.%03d", uiHours, uiMinutes, uiSeconds, uiMilliSecs);
			tagTable->edit(row, "Time", data); 
		}

		// get serial number
		if (bSNR)
		{
			// get tag name
			char cTagType[128] = {0};
			unsigned char ucSnr[32];
			char cSnr[64];
			char cMfr[256] = {0};
			unsigned int uiHeader;
			unsigned int uiEPClen;
			unsigned char ucEpcType;
			
			m_iscReader->GetTableData(i, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_TRTYPE, &ucTmp);
			
			if((ucTmp & 0x80) == 0 && iBack == 0)	// HF-Transponder
			{
				switch(ucTmp)
				{
				case FEDM_ISC_TR_TYPE_ICODE1:
					strcpy(cTagType, "Philips I-Code1");
					break;
				
				case FEDM_ISC_TR_TYPE_TAGIT:
					strcpy(cTagType, "Texas Instruments Tag-it HF");
					break;

				case FEDM_ISC_TR_TYPE_ISO15693:
					memset(ucSnr, 0, 8);
					m_iscReader->GetTableData(i, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_SNR, ucSnr, 8);
					GetISOMfrCode(ucSnr[1], cMfr);
					sprintf(cTagType, "ISO15693 - %s", cMfr);
					break;
			
				case FEDM_ISC_TR_TYPE_ISO14443A:
					strcpy(cTagType, "ISO14443-A");
					break;
				
				case FEDM_ISC_TR_TYPE_ISO14443B:
					strcpy(cTagType, "ISO14443-B");
					break;

				case FEDM_ISC_TR_TYPE_EPC:
					m_iscReader->GetTableData(i, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC_HEADER, &uiHeader);
					m_iscReader->GetTableData(i, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC_TYPE, &ucEpcType);

					switch(ucEpcType)
					{
					case FEDM_ISC_EPC_TYPE_1:
						strcpy(cTagType, "EPC 96 bit");
						m_iscReader->GetTableData(i, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC, ucSnr);
						break;
					case FEDM_ISC_EPC_TYPE_2:
						strcpy(cTagType, "EPC 64 bit type I");
						m_iscReader->GetTableData(i, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC, ucSnr);
						break;
					case FEDM_ISC_EPC_TYPE_3:
						strcpy(cTagType, "EPC 64 bit type II");
						m_iscReader->GetTableData(i, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC, ucSnr);
						break;
					case FEDM_ISC_EPC_TYPE_4:
						strcpy(cTagType, "EPC 64 bit type III");
						m_iscReader->GetTableData(i, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC, ucSnr);
						break;
					}
					break;

				case FEDM_ISC_TR_TYPE_ICODE_UID:
					strcpy(cTagType, "I-CODE UID");
					break;
				}
			}
			else if((ucTmp & 0x80) == 0x80)	// UHF-Transponder
			{
				switch(ucTmp)
				{
				case FEDM_ISC_TR_TYPE_ISO18000_6_A:
					strcpy(cTagType, "ISO18000-6-A");
					break;
				case FEDM_ISC_TR_TYPE_ISO18000_6_B:
					strcpy(cTagType, "ISO18000-6-B");
					break;
				case FEDM_ISC_TR_TYPE_EM4222:
					strcpy(cTagType, "EM4222");
					break;
				case FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2:
					strcpy(cTagType, "EPC Generation 2");
					break;
				case FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN1:
					strcpy(cTagType, "EPC Class 1");
					break;

				default:  strcpy(cTagType, "unknown");
				}
			}
			//Show in table
			tagTable->edit(row, "Type", cTagType);
			m_iscReader->GetTableData(i,FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_SNR_LEN, &uiEPClen);
			FEDM_ConvHexUCharToHexChar(ucSnr, uiEPClen, cSnr, sizeof(cSnr));
			tagTable->edit(row, "Serial No", cSnr);	
		
		}
         
		if (bDB)
		{
			memset(data, 0, sizeof(data));
			m_iscReader->GetTableData(i, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_RxDB, 1, ucDB, 4);
			sprintf(data, "%02X", (unsigned int)(ucDB));
			tagTable->edit(row, "Data Block", data); 
		}

		if (bAnt)
		{
			memset(data, 0, sizeof(data));
			// get antenna byte. each bit represents one antenna
			m_iscReader->GetTableData(i, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_ANT_NR, &ucAnt);
			char buf[4] = {0};
			int antNo=1;
			for (int p=1; p<=256; p*=2)
			{
				if (ucAnt & p)
				{
					if (strlen(data) > 0)
						strcat(data, ", ");
					sprintf(buf, "%d", antNo);
					strcat(data, buf);
				}
				antNo++;
			}
			tagTable->edit(row, "Ant No", data);
		}
	}
}

void BRMDemoDlg::OnDataProt22()
{
	unsigned char ucTmp;
	unsigned char ucAnt;
	char data[512] = {0};
	bool bSNR = false;
	bool bDB = false;
	bool bTime = false;
	bool bAnt = false;
	bool bDate = false;
	int row = 0;
	int iBack = 0;

	for (int i = 0; i < m_iscReader->GetTableLength(FEDM_ISC_BRM_TABLE); i++) 
	{
		tagTable->newRow();
		row = tagTable->rowCount()-1;
		// Show in ListView
		memset(data, 0, sizeof(data));
		sprintf(data, "%d", row);
		tagTable->edit(row, "No", data);

		m_iscReader->GetData(FEDM_ISC_TMP_ADV_BRM_TRDATA1_SNR,	 &bSNR);
		m_iscReader->GetData(FEDM_ISC_TMP_ADV_BRM_TRDATA1_DB,	 &bDB);
		m_iscReader->GetData(FEDM_ISC_TMP_ADV_BRM_TRDATA1_ANT,	 &bAnt);
		m_iscReader->GetData(FEDM_ISC_TMP_ADV_BRM_TRDATA1_TIME, &bTime);
		m_iscReader->GetData(FEDM_ISC_TMP_ADV_BRM_TRDATA1_DATE, &bDate);

		if(bDate)
		{
			unsigned char ucDate[5];
			memset(data, 0, sizeof(data));
			m_iscReader->GetTableData(i, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_DATE, ucDate, 5);
			sprintf(data, "%02d/%02d/%02d%02d", ucDate[2], ucDate[3], ucDate[0], ucDate[1]);
			tagTable->edit(row, "Date", data); 
		}

		if(bTime)
		{
			unsigned char ucTime[4];
			memset(data, 0, sizeof(data));
			m_iscReader->GetTableData(i, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_TIMER, ucTime, 4);
			unsigned int uiHours	 = ucTime[0];
			unsigned int uiMinutes	 = ucTime[1];
			unsigned int uiMilliSecs = (ucTime[2]<<8) + ucTime[3];
			unsigned int uiSeconds	 = uiMilliSecs / 1000;
			uiMilliSecs = uiMilliSecs % 1000;
			
			sprintf(data, "%02d:%02d:%02d.%03d", uiHours, uiMinutes, uiSeconds, uiMilliSecs);
			tagTable->edit(row, "Time", data); 
		}

		// get serial number
		if (bSNR)
		{
			// get tag name
			char cTagType[128] = {0};
			unsigned char ucSnr[32];
			char cSnr[64];
			char cMfr[256] = {0};
			unsigned int uiHeader;
			unsigned int uiEPClen;
			unsigned char ucEpcType;
			
			m_iscReader->GetTableData(i, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_TRTYPE, &ucTmp);
			
			if((ucTmp & 0x80) == 0 && iBack == 0)	// HF-Transponder
			{
				switch(ucTmp)
				{
				case FEDM_ISC_TR_TYPE_ICODE1:
					strcpy(cTagType, "Philips I-Code1");
					break;
				
				case FEDM_ISC_TR_TYPE_TAGIT:
					strcpy(cTagType, "Texas Instruments Tag-it HF");
					break;

				case FEDM_ISC_TR_TYPE_ISO15693:
					memset(ucSnr, 0, 8);
					m_iscReader->GetTableData(i, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_SNR, ucSnr, 8);
					GetISOMfrCode(ucSnr[1], cMfr);
					sprintf(cTagType, "ISO15693 - %s", cMfr);
					break;
			
				case FEDM_ISC_TR_TYPE_ISO14443A:
					strcpy(cTagType, "ISO14443-A");
					break;
				
				case FEDM_ISC_TR_TYPE_ISO14443B:
					strcpy(cTagType, "ISO14443-B");
					break;

				case FEDM_ISC_TR_TYPE_EPC:
					m_iscReader->GetTableData(i, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC_HEADER, &uiHeader);
					m_iscReader->GetTableData(i, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC_TYPE, &ucEpcType);

					switch(ucEpcType)
					{
					case FEDM_ISC_EPC_TYPE_1:
						strcpy(cTagType, "EPC 96 bit");
						m_iscReader->GetTableData(i, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC, ucSnr);
						break;
					case FEDM_ISC_EPC_TYPE_2:
						strcpy(cTagType, "EPC 64 bit type I");
						m_iscReader->GetTableData(i, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC, ucSnr);
						break;
					case FEDM_ISC_EPC_TYPE_3:
						strcpy(cTagType, "EPC 64 bit type II");
						m_iscReader->GetTableData(i, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC, ucSnr);
						break;
					case FEDM_ISC_EPC_TYPE_4:
						strcpy(cTagType, "EPC 64 bit type III");
						m_iscReader->GetTableData(i, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC, ucSnr);
						break;
					}
					break;

				case FEDM_ISC_TR_TYPE_ICODE_UID:
					strcpy(cTagType, "I-CODE UID");
					break;
				}
			}
			else if((ucTmp & 0x80) == 0x80)	// UHF-Transponder
			{
				switch(ucTmp)
				{
				case FEDM_ISC_TR_TYPE_ISO18000_6_A:
					strcpy(cTagType, "ISO18000-6-A");
					break;
				case FEDM_ISC_TR_TYPE_ISO18000_6_B:
					strcpy(cTagType, "ISO18000-6-B");
					break;
				case FEDM_ISC_TR_TYPE_EM4222:
					strcpy(cTagType, "EM4222");
					break;
				case FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2:
					strcpy(cTagType, "EPC Generation 2");
					break;
				case FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN1:
					strcpy(cTagType, "EPC Class 1");
					break;

				default:  strcpy(cTagType, "unknown");
				}
			}
			//Show in table
			tagTable->edit(row, "Type", cTagType);
			m_iscReader->GetTableData(i,FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_SNR_LEN, &uiEPClen);
			FEDM_ConvHexUCharToHexChar(ucSnr, uiEPClen, cSnr, sizeof(cSnr));
			tagTable->edit(row, "Serial No", cSnr);	
		
		}

		if (bDB)
		{
			// get number of entries in db
			unsigned int numEntries;
			m_iscReader->GetTableData(i, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_DBN, &numEntries);
			// get database address offset
			unsigned char dbAddress = 0;
			m_iscReader->GetTableData(i, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_DB_ADR, &dbAddress);
			// get data block size
			unsigned char	ucBlockSize;
			unsigned char 	ucReadBlock[256] = {0};
			m_iscReader->GetTableData(i, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_BLOCK_SIZE, &ucBlockSize);
			// get data blocks
			memset(data, 0, sizeof(data));
			char tmp[516] = {0}; // 512 + 1 at least
			for (unsigned int dbEntry=0; dbEntry<numEntries; dbEntry++)
			{
				memset(tmp, 0, sizeof(tmp));
				memset(ucReadBlock, 0, sizeof(ucReadBlock));
				iBack = m_iscReader->GetTableData(i, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_RxDB, dbAddress+dbEntry, ucReadBlock, int(ucBlockSize));
				if (iBack < 0)
					displayError(iBack);
				for (int pos=0; pos<(int)ucBlockSize; pos++)
				{
					sprintf(tmp, "%02X", ucReadBlock[pos]);
					strcat(data, tmp);
					strcat(data, " ");				
				}
			}
			tagTable->edit(row, "Data Block", data); 
		}

		if (bAnt)
		{
			memset(data, 0, sizeof(data));
			// get antenna byte. each bit represents one antenna
			m_iscReader->GetTableData(i, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_ANT_NR, &ucAnt);
			char buf[4] = {0};
			int antNo=1;
			for (int p=1; p<=256; p*=2)
			{
				if (ucAnt & p)
				{
					if (strlen(data) > 0)
						strcat(data, ", ");
					sprintf(buf, "%d", antNo);
					strcat(data, buf);
				}
				antNo++;
			}
			tagTable->edit(row, "Ant No", data);
		}
	}
}

//------------------------------------------------------------------------------
// Name: GetISOMfrCode(unsigned char ucMfr, char* sMfr)
// Desc: returns a string description of the ISO code
//------------------------------------------------------------------------------
void BRMDemoDlg::GetISOMfrCode(unsigned char ucMfr, char* sMfr)
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
// Name: displayError(int error)
// Desc: displays an error in the line edit. Triggered by the inventory thread
//------------------------------------------------------------------------------
void BRMDemoDlg::displayError(int error)
{
	char sErrorText[256] = {0};
	
	m_iscReader->GetErrorText(sErrorText, error);
	ui.statusLineEdit->setText(sErrorText);
	m_readTagsThread->readTags();
}

//------------------------------------------------------------------------------
// Name: clearTagList()
// Desc: clears the tag table
//------------------------------------------------------------------------------
void BRMDemoDlg::clearTagList()
{
	ui.statusLineEdit->setText("");
	m_iListItem = 0;
	tagTable->clear();
}

//------------------------------------------------------------------------------
// Name: BRMDemoDlg()
// Desc: constructor
//------------------------------------------------------------------------------
BRMDemoDlg::BRMDemoDlg()
{
	ui.setupUi(this);
	
	QSplitter page;
	
	m_iListItem = 0;
	
	QStringList list;
	list += "No";
	list += "Date";
	list += "Time";
	list += "Type";
	list += "Serial No";
	list += "Data Block";
	list += "Ant No";
	tagTable = new FEUITable(list, ui.tagListGroupBox);

	setupMenuBar();
	
	m_iscReader = new FEDM_ISCReaderModule;
	
	initReader();
	
	m_readTagsThread = new FEReadTagsThread(m_iscReader);
	
	connect(m_readTagsThread, SIGNAL(onNewData(int)), this, SLOT(displayData()));
	connect(m_readTagsThread, SIGNAL(onError(int)), this, SLOT(displayError(int)));
	connect(ui.clearListPushButton, SIGNAL(clicked()), this, SLOT(clearTagList()));
	connect(ui.clearProtPushButton, SIGNAL(clicked()), ui.protocolTextEdit, SLOT(clear()));
	connect(ui.startPushButton, SIGNAL(clicked()), this, SLOT(startButtonClicked()));
	
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
	// callback. See BRMDemoDlg.h 
	setGetClassInstance(this);
	connect(this, SIGNAL(onProtocolEvent(int,int)), this, SLOT(displayProtocol(int,int)));
}

//------------------------------------------------------------------------------
// Name: ~BRMDemoDlg()
// Desc: destructor
//------------------------------------------------------------------------------
BRMDemoDlg::~BRMDemoDlg()
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
		}
	}
	
	if (m_readTagsThread != NULL)
	{
		m_readTagsThread->quit();
		sleep(0);
		delete m_readTagsThread;
		m_readTagsThread = NULL;
	}
	
	if (m_iscReader != NULL)
	{
		m_iscReader->DisConnect();
		delete m_iscReader;
		m_iscReader = NULL;
	}

	if (tagTable != NULL)
	{
		delete tagTable;
		tagTable = NULL;
	}

}

//------------------------------------------------------------------------------
// Name: resizeEvent(QResizeEvent *event)
// Desc: handles resizing of the custom table
//------------------------------------------------------------------------------
void BRMDemoDlg::resizeEvent(QResizeEvent *event)
{
	//event->accept();
	QRect tagRect = ui.tagListGroupBox->geometry();
	int tagTableWidth = tagRect.width() - 18;
	int tagTableHeight = tagRect.height() - 24;
	tagTable->setGeometry(10, 20, tagTableWidth, tagTableHeight);
}

//------------------------------------------------------------------------------
// Name: showAbout()
// Desc: shows an "About" message
//------------------------------------------------------------------------------
void BRMDemoDlg::showAbout()
{
	QMessageBox::information(this, tr("About"), aboutMessage, QMessageBox::Abort);
}

//------------------------------------------------------------------------------
// Name: setupMenuBar()
// Desc: sets up the menu bar
//------------------------------------------------------------------------------
void BRMDemoDlg::setupMenuBar()
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
void BRMDemoDlg::initReader()
{
	int iReaderHnd;
	
	detectReaderDlg.setReader(m_iscReader);
	
	if (detectReaderDlg.exec() != QDialog::Accepted)
		return;

	
	iReaderHnd = m_iscReader->GetReaderHnd();
		
	if(iReaderHnd < 0)
	{
		char cErrorText[256];
		m_iscReader->GetErrorText(cErrorText, iReaderHnd);
    		QMessageBox::critical(this, tr("GetReaderHnd() error "),
                                    cErrorText,
                                    QMessageBox::Abort);
		return;
	}
	else
	{
		//Set the TableSize
		//m_reader->SetTableSize(, 500);
		m_iscReader->SetTableSize(FEDM_ISC_BRM_TABLE, 500);
				
		ui.startPushButton->setEnabled(true);
		
	}	
	
	// log protocol to output file
	//FEISC_SetReaderPara(m_iscReader->GetReaderHnd(), "LogProt", "1");
	
}


//------------------------------------------------------------------------------
// Name: static setGetClassInstance(BRMDemoDlg* classInstance)
// Desc: set / get function to share a pointer of the class instance of the UI with the static callback.
// Return: Null if new pointer is set, or a pointer to the instance if instance == NULL 
//------------------------------------------------------------------------------
BRMDemoDlg* BRMDemoDlg::setGetClassInstance(BRMDemoDlg* classInstance)
{
	static BRMDemoDlg* statInstance = NULL;
	
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
void BRMDemoDlg::protCallBackHelper(int iMsg,int iLen)
{
	BRMDemoDlg* classInstance = setGetClassInstance(NULL);
	
	emit(classInstance->onProtocolEvent(iMsg, iLen));
}

//------------------------------------------------------------------------------
// Name: displayProtocol(int iMsg,int iLen)
// Desc: displays protocol messages in the protocol window. Triggered by 
// 	 onProtocolEvent() which is triggered in the static helper function protCallBackHelper()
//------------------------------------------------------------------------------
void BRMDemoDlg::displayProtocol(int iMsg,int iLen)
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
void BRMDemoDlg::startButtonClicked()
{
	ui.statusLineEdit->setText("");
	
	if (m_readTagsThread->running())
	{	
		//pause thread
		m_readTagsThread->pauseThread();
		ui.startPushButton->setText("&Start BRM");
	}
	else
	{
		//Delete all tags from the ListView
		tagTable->clear();
		ui.startPushButton->setText("&Stop BRM");
		
		// start thread (or resume if already created)
		m_readTagsThread->readTags();
		
	}
}
