/*-------------------------------------------------------
|                                                       |
|                  NotifySampleDlg.cpp                  |
|                                                       |
---------------------------------------------------------

Copyright  2007-2008	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : info@feig.de
						Internet : http://www.feig.de
					
Author         		:	Benjamin Stadin
Begin        		:	12.01.2007

Version       		:	01.00.00 / 15.01.2007 / Benjamin Stadin

Operation Systems	:	Linux
*/

#include <QMainWindow>
#include "NotifySampleDlg.h"

// FE-includes
#include "../../../../feisc-lib/include/feisc.h" 
#include "../../../../fedm-classlib/src/FedmIscCore.h"
#include "ISO_IEC_7816_6_ICManufacturerRegistration.h"

// for log file
#include <iostream>
#include <fstream>


using namespace std;


//------------------------------------------------------------------------------
// Name: displayData() 
// Desc: displays new tags in the tag table. Triggered by inventory thread
//------------------------------------------------------------------------------
void NotifySampleDlg::displayData(void* pAny, int iError, unsigned char ucCmd)
{
	bool bSNR = false;
	bool bDB = false;
	bool bTime = false;
	bool bDate = false;
	bool bAnt = false;
	bool bIn = false;
	bool bExt = false;
	bool bEpc = false;
	unsigned char	ucTrType = 0;
	//unsigned char	ucDBAdr = 0;
	unsigned char ucReadBlock[512] = {0};
	unsigned char	ucDBSize = 0;
	unsigned char	ucTmp;
	unsigned char	ucSnr[32];
	unsigned char	ucSnrLen = 0;
	unsigned char	ucTimer[4];
	unsigned char	ucDate[5];
	unsigned char	ucAntNr = 0;
	unsigned char	ucInput = 0;
	unsigned char	ucState = 0;
	unsigned int	uiDBN = 0;
	unsigned int	uiTmp = 0;
	static char str[1024] = {0};
	char sMfr[256] = {0};
	char sTmp[256] = {0};
	
	if (ucCmd != 0x22)
		return;

	if(iError < 0)
		return;

	if(!(iError==0x00 || 
	     iError==0x83 || 
	     iError==0x84 || 
	     iError==0x93 || 
	     iError==0x94 ))
	{
		return;
	}
	
	m_iscReader->GetData(FEDM_ISC_TMP_ADV_BRM_TRDATA1_SNR,	&bSNR);
	m_iscReader->GetData(FEDM_ISC_TMP_ADV_BRM_TRDATA1_DB,	&bDB);
	m_iscReader->GetData(FEDM_ISC_TMP_ADV_BRM_TRDATA1_ANT,	&bAnt);
	m_iscReader->GetData(FEDM_ISC_TMP_ADV_BRM_TRDATA1_TIME,	&bTime);
	m_iscReader->GetData(FEDM_ISC_TMP_ADV_BRM_TRDATA1_DATE,	&bDate);
	m_iscReader->GetData(FEDM_ISC_TMP_ADV_BRM_TRDATA1_EXT,	&bExt);
	m_iscReader->GetData(FEDM_ISC_TMP_ADV_BRM_TRDATA2_IN,	&bIn);

	memset(m_cLineTxt, 0, 1024);
	memset(m_cLogTxt, 0, 1024);

	for(int iIdx=0; iIdx<m_iscReader->GetTableLength(FEDM_ISC_BRM_TABLE); iIdx++)
	{
		sprintf(m_cLineTxt, "%07d", m_iLineCnt);
		sprintf(m_cLogTxt, "%07d;", m_iLineCnt++);
		memset(sMfr, 0, sizeof(sMfr));
		memset(sTmp, 0, sizeof(sTmp));
		
		m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_SNR_LEN, &ucSnrLen);

		if(bSNR)
		{
			if(ucSnrLen > 0)
			{
				m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_TRTYPE, &ucTrType);

				sprintf(str, " - 0x%02X", ucTrType);
				strcat(m_cLineTxt, str);
				strcat(m_cLogTxt, str);

				switch(ucTrType)
				{
				case FEDM_ISC_TR_TYPE_ICODE1:
					sprintf(str, "%s", " (Philips I-Code1)");
					break;

				case FEDM_ISC_TR_TYPE_TAGIT:
					sprintf(str, "%s", " (Texas Instruments Tag-it HF)");
					break;
				
				case FEDM_ISC_TR_TYPE_ISO15693:
					m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_SNR, ucSnr, 8);
					strcpy(sMfr, getISOMfrCode(ucSnr[1]));
					sprintf(str, " (ISO15693 - %s)", sMfr);
					break;
				
				case FEDM_ISC_TR_TYPE_ISO14443A:
					sprintf(str, "%s", " (ISO14443-A)");
					break;
				
				case FEDM_ISC_TR_TYPE_ISO14443B:
					sprintf(str, "%s", " (ISO14443-B)");
					break;

				case FEDM_ISC_TR_TYPE_EPC:
					m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_IS_EPC, &bEpc);
					if(bEpc)
					{
						m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC_TYPE, &ucTmp);
						switch(ucTmp)
						{
						case FEDM_ISC_EPC_TYPE_1:
							sprintf(str, "%s", " (EPC 96 bit)");
//							m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC, sTmp, sizeof(sTmp));
							break;
						case FEDM_ISC_EPC_TYPE_2:
							sprintf(str, "%s", " (EPC 64 bit type I)");
//							m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC, sTmp, sizeof(sTmp));
							break;
						case FEDM_ISC_EPC_TYPE_3:
							sprintf(str, "%s", " (EPC 64 bit type II)");
//							m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC, sTmp, sizeof(sTmp));
							break;
						case FEDM_ISC_EPC_TYPE_4:
							sprintf(str, "%s", " (EPC 64 bit type III)");
//							m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC, sTmp, sizeof(sTmp));
							break;
						default:
							sprintf(str, "%s", " (unknown EPC type)");
						}
					}
					break;
			
				case FEDM_ISC_TR_TYPE_0x64:
					sprintf(str, "%s", " (Unknown)");
					break;

				case FEDM_ISC_TR_TYPE_ICODE_UID:
					sprintf(str, "%s", " (I-CODE UID)");
					break;

				case FEDM_ISC_TR_TYPE_ISO18000_6_A:	// 0x80
					sprintf(str, "%s", " (18000-6-A)");
					break;

				case FEDM_ISC_TR_TYPE_ISO18000_6_B:	// 0x81
					sprintf(str, "%s", " (18000-6-B)");
					break;
				
				case FEDM_ISC_TR_TYPE_EM4222:		// 0x83
					sprintf(str, "%s", " (EM4222)");
					break;
				
				case FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2:	// 0x84
					m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_IS_EPC, &bEpc);
					if(bEpc)
					{
						m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC_TYPE, &ucTmp);
						switch(ucTmp)
						{
						case FEDM_ISC_EPC_TYPE_1:
							sprintf(str, "%s", " (EPC Class 1 Gen 2 - EPC 96 bit)");
							break;
						case FEDM_ISC_EPC_TYPE_2:
							sprintf(str, "%s", " (EPC Class 1 Gen 2 - EPC 64 bit type I)");
							break;
						case FEDM_ISC_EPC_TYPE_3:
							sprintf(str, "%s", " (EPC Class 1 Gen 2 - EPC 64 bit type II)");
							break;
						case FEDM_ISC_EPC_TYPE_4:
							sprintf(str, "%s", " (EPC Class 1 Gen 2 - EPC 64 bit type III)");
							break;
						default:
							sprintf(str, "%s", " (EPC Class 1 Gen 2 - unknown EPC type)");
						}
					}
					else
					{
						sprintf(str, "%s", " (EPC Class 1 Gen 2)");
					}
					break;
				
				case FEDM_ISC_TR_TYPE_EPC_CLASS0:	// 0x88
					m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_IS_EPC, &bEpc);
					if(bEpc)
					{
						m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC_TYPE, &ucTmp);
						switch(ucTmp)
						{
						case FEDM_ISC_EPC_TYPE_1:
							sprintf(str, "%s", " (EPC Class 0 - EPC 96 bit)");
							break;
						case FEDM_ISC_EPC_TYPE_2:
							sprintf(str, "%s", " (EPC Class 0 - EPC 64 bit type I)");
							break;
						case FEDM_ISC_EPC_TYPE_3:
							sprintf(str, "%s", " (EPC Class 0 - EPC 64 bit type II)");
							break;
						case FEDM_ISC_EPC_TYPE_4:
							sprintf(str, "%s", " (EPC Class 0 - EPC 64 bit type III)");
							break;
						default:
							sprintf(str, "%s", " (EPC Class 0 - unknown EPC type)");
						}
					}
					else
					{
						sprintf(str, "%s", " (EPC Class 0)");
					}
					break;
				
				case FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN1:	// 0x89
					m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_IS_EPC, &bEpc);
					if(bEpc)
					{
						m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC_TYPE, &ucTmp);
						switch(ucTmp)
						{
						case FEDM_ISC_EPC_TYPE_1:
							sprintf(str, "%s", " (EPC Class 1 Gen 1 - EPC 96 bit)");
							break;
						case FEDM_ISC_EPC_TYPE_2:
							sprintf(str, "%s", " (EPC Class 1 Gen 1 - EPC 64 bit type I)");
							break;
						case FEDM_ISC_EPC_TYPE_3:
							sprintf(str, "%s", " (EPC Class 1 Gen 1 - EPC 64 bit type II)");
							break;
						case FEDM_ISC_EPC_TYPE_4:
							sprintf(str, "%s", " (EPC Class 1 Gen 1 - EPC 64 bit type III)");
							break;
						default:
							sprintf(str, "%s", " (EPC Class 1 Gen 1 - unknown EPC type)");
						}
					}
					else
					{
						sprintf(str, "%s", " (EPC Class 1 Gen 1)");
					}
					break;
				default:
					sprintf(str, "%s", " (unknown)");
					break;
				}

				// add transponder name
				strcat(m_cLineTxt, str);
				strcat(m_cLogTxt, str);
				strcat(m_cLogTxt, ";");

				// PC fr gen2
				if(ucTrType == FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2)
				{
					memset(sTmp, 0, sizeof(sTmp));
					m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC_CL1_GEN2_PC, sTmp, sizeof(sTmp));
					sprintf(str, "     PC.........: %s", sTmp);
					strcat(m_cLineTxt, str);
					strcat(m_cLogTxt, str);
					strcat(m_cLogTxt, ";");
				}


				if(bEpc)
				{
					memset(sTmp, 0, sizeof(sTmp));
					m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC, sTmp, sizeof(sTmp));
					m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC_TYPE, &ucTmp);

					switch(ucTmp)
					{
					case FEDM_ISC_EPC_TYPE_1:
					case FEDM_ISC_EPC_TYPE_2:
					case FEDM_ISC_EPC_TYPE_3:
					case FEDM_ISC_EPC_TYPE_4:
						sprintf(str, "%s", sTmp);
						break;
					default:
						sprintf(str, "%s", "EPC ??");
					}
				}
				else
				{
					memset(sTmp, 0, sizeof(sTmp));
					m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_SNR, sTmp, sizeof(sTmp));
					sprintf(str, " - SNR..%s \n", sTmp);
				}

				// add uid
				strcat(m_cLineTxt, str);
				strcat(m_cLogTxt, str);
				strcat(m_cLogTxt, ";");
			}
			else
			{
				strcat(m_cLogTxt, ";"); // for empty transponder type
				strcat(m_cLogTxt, ";");	// for empty uid
			}
		
		} // if(bSNR)
		else
		{
			strcat(m_cLogTxt, ";"); // for empty transponder type
			strcat(m_cLogTxt, ";");	// for empty uid
		}


		if(bDB)
		{
			m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_DBN, &uiDBN);
			if(uiDBN > 0)
			{
				sprintf(str, " - DB-N..0x%04X", uiDBN);

				m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_BLOCK_SIZE, &ucDBSize);
				sprintf(str, " - DB-SIZE..0x%02X", ucDBSize);

				sprintf(str, "%s", " - DB..");
				strcat(m_cLineTxt, str);
				strcat(m_cLogTxt, str);
				

				for(int iCnt=0; iCnt<(int)uiDBN; iCnt++)
				{
					memset(sTmp, 0, sizeof(sTmp));
					memset(ucReadBlock, 0, sizeof(ucReadBlock));
					m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_RxDB, iCnt, ucReadBlock, ucDBSize);
					for (int pos=0; pos<(int)ucDBSize; pos++)
					{
						sprintf(sTmp, "%02X", ucReadBlock[pos]);
						strcat(m_cLineTxt, sTmp);
						strcat(m_cLineTxt, " ");				
					}
				}

				strcat(m_cLogTxt, ";");
			}
			else
			{
				strcat(m_cLogTxt, ";");	// for empty datablocks
			}
		
		} // if(bDB)
		else
		{
			strcat(m_cLogTxt, ";");	// for empty datablocks
		}


		if(bDate)
		{
			m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_DATE, ucDate, 5);

			sprintf(str, " - DATE..%02d/%02d/%02d%02d", ucDate[2], ucDate[3], ucDate[0], ucDate[1]);
			strcat(m_cLineTxt, str);
			strcat(m_cLogTxt, str);
			strcat(m_cLogTxt, ";");
		}


		if(bTime)
		{
			m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_TIMER, ucTimer, 4);
			m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_TIMER, &uiTmp);

			unsigned int uiHours	 = ucTimer[0];
			unsigned int uiMinutes	 = ucTimer[1];
			unsigned int uiMilliSecs = (ucTimer[2]<<8) + ucTimer[3];
			unsigned int uiSeconds	 = uiMilliSecs / 1000;
			uiMilliSecs = uiMilliSecs % 1000;
			
			sprintf(str, " - TIME..%02d:%02d:%02d.%03d (%08X)", uiHours, uiMinutes, uiSeconds, uiMilliSecs, uiTmp);
			strcat(m_cLineTxt, str);
			strcat(m_cLogTxt, str);
			strcat(m_cLogTxt, ";");
		}

		
		if(bAnt && ((bSNR && ucSnrLen>0) || (bDB && uiDBN>0)))
		{
			m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_ANT_NR, &ucAntNr);
			
			sprintf(str, " - ANT..0x%02X", ucAntNr);
			strcat(m_cLineTxt, str);
			strcat(m_cLogTxt, str);
			strcat(m_cLogTxt, ";");
		}
		else
		{
			strcat(m_cLogTxt, ";");	// for empty ant
		}


		if(bExt && bIn)
		{
			m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_INPUT, &ucInput);
			
			sprintf(str, " - INPUT..0x%02X", ucInput);
			strcat(m_cLineTxt, str);
			strcat(m_cLogTxt, str);
			strcat(m_cLogTxt, ";");

			m_iscReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_STATE, &ucState);
			
			sprintf(str, " - STATE..0x%02X", ucState);
			strcat(m_cLineTxt, str);
			strcat(m_cLogTxt, str);
			strcat(m_cLogTxt, ";");
		}

		QString outStr;
		outStr = m_cLineTxt;
		outStr += "\n";
		ui.receiveWindowTextEdit->append(outStr);

		// write into logfile
		if(ui.fileCheckBox->isChecked())
		{
			strcat(m_cLogTxt, "\r\n");
			writeLogFile();
		}
	
	} // for(int iIdx=0; iIdx<m_iscReader->GetTableLength(FEDM_ISC_BRM_TABLE); iIdx++)

}

//------------------------------------------------------------------------------
// Name: writeLogFile()
// Desc: writes tag data to the log file
//------------------------------------------------------------------------------
void NotifySampleDlg::writeLogFile()
{
	ofstream file;
	
	if(strlen(m_cFileName) == 0)
		return;
		
	file.open(m_cFileName, ios::app);
	if (file.is_open())
	{
		file.seekp(0, ios::end);
		file << m_cLogTxt;
		file.close();
	}
}

//------------------------------------------------------------------------------
// Name: displayError(int error)
// Desc: displays an error in the line edit. Triggered by the inventory thread
//------------------------------------------------------------------------------
void NotifySampleDlg::displayError(int error)
{
	char sErrorText[256] = {0};
	
	m_iscReader->GetErrorText(sErrorText, error);
	ui.statusLineEdit->setText(sErrorText);
}

//------------------------------------------------------------------------------
// Name: NotifySampleDlg()
// Desc: constructor
//------------------------------------------------------------------------------
NotifySampleDlg::NotifySampleDlg()
{
	ui.setupUi(this);
	
	memset(m_cFileName, 0, sizeof(m_cFileName));
	memset(m_cLogTxt, 0, sizeof(m_cLogTxt));
	strcpy(m_cFileName, "log.txt");
	
	m_bRunning = false;
	m_iLineCnt = 0;
	
	m_iscReader = new FEDM_ISCReaderModule;
	
	int iBack = m_iscReader->SetTableSize(FEDM_ISC_BRM_TABLE, 512);
	if(iBack < 0)
	{
		delete m_iscReader;
		printf("FEDM_ISCReaderModule::SetTableSize() failed. Error code = %d\n", iBack);
		return;
	}
	
	connect(ui.clearRecWndPushButton, SIGNAL(clicked()), ui.receiveWindowTextEdit, SLOT(clear()));
	connect(ui.clearProtPushButton, SIGNAL(clicked()), ui.protocolTextEdit, SLOT(clear()));
	connect(ui.listenPushButton, SIGNAL(clicked()), this, SLOT(listenButtonClicked()));
	connect(ui.fileCheckBox, SIGNAL(clicked()), this, SLOT(writeToFileClicked()));
		
	// Saves current pointer to class instance which is needed by the static 
	// callback. See NotifySampleDlg.h 
	setGetClassInstance(this);
	// Indirection of the above callback function protCallBackHelper() to trigger displayProtocol()
	connect(this, SIGNAL(onProtocolEvent(int,int)), this, SLOT(displayProtocol(int,int)));
	
	// Add event handlers	
	int iReaderHnd = m_iscReader->GetReaderHnd();
	FEISC_EVENT_INIT taskInit;
	taskInit.cbFct =	&protCallBackHelper;
	taskInit.uiUse =	FEISC_PRT_EVENT;
	taskInit.uiFlag =	FEISC_CALLBACK;
	iBack =		FEISC_AddEventHandler(iReaderHnd,&taskInit);
	if(iBack < 0)
	{
		displayError(iBack);
	}

}

//------------------------------------------------------------------------------
// Name: writeToFileClicked()
// Desc: Enable / disable logging
//------------------------------------------------------------------------------
void NotifySampleDlg::writeToFileClicked() 
{
	if(ui.fileCheckBox->isChecked())
	{
		FEISC_SetReaderPara(m_iscReader->GetReaderHnd(), "LogProt", "1");
	}
	else
	{
		FEISC_SetReaderPara(m_iscReader->GetReaderHnd(), "LogProt", "0");
	}
}

//------------------------------------------------------------------------------
// Name: ~NotifySampleDlg()
// Desc: destructor
//------------------------------------------------------------------------------
NotifySampleDlg::~NotifySampleDlg()
{
	// Delete protocol output handler
	if (m_iscReader != NULL)
	{
		int iReaderHnd = m_iscReader->GetReaderHnd();
		if (iReaderHnd > 0) // alway > 0
		{
			FEISC_EVENT_INIT taskInit;
			taskInit.cbFct =	&protCallBackHelper;
			taskInit.uiUse =	FEISC_PRT_EVENT;
			taskInit.uiFlag =	FEISC_CALLBACK;
			FEISC_DelEventHandler (iReaderHnd,&taskInit);
		}
		if (m_bRunning)
			m_iscReader->CancelAsyncTask();
		delete m_iscReader;
		m_iscReader = NULL;
	}
}

//------------------------------------------------------------------------------
// Name: initReader()
// Desc: shows the detect reader dialog, initializes the reader and sets callback functions
//------------------------------------------------------------------------------
void NotifySampleDlg::startTask()
{
	ui.statusLineEdit->setText("");
	
	if(!m_iscReader)
	{
    		QMessageBox::critical(this, tr("GetReaderHnd() error "), "No reader module", QMessageBox::Abort);
		return;
	}
	else
	{
		int iBack = 0;
						
		// callback for notification events
		char *sPort = qstrdup(ui.portLineEdit->text().toLatin1());
		int iPort;
		sscanf(sPort, "%d", &iPort);
		FEDM_TASK_INIT event;
		event.iConnectByHost	= 0;
		event.pAny		= this;
		event.uiFlag		= FEDM_TASKCB1;
		event.uiUse		= FEDM_TASKID_NOTIFICATION;
		memset(event.cIPAdr, 0, 16);
		event.iPortNr		= iPort;
		event.cbFct1		= &cbsFct1;
		event.iNotifyWithAck	= ui.withAckCheckBox->isChecked();
		event.uiTimeout		= 30;
		delete [] sPort;
		
		connect(this, SIGNAL(onDataEvent(void*,int,unsigned char)), 
			this, SLOT(displayData(void*,int,unsigned char)));
	
		iBack = m_iscReader->StartAsyncTask(&event);
		if(iBack < 0)
		{
			displayError(iBack);
			return;
		}
		
	}		
}

//------------------------------------------------------------------------------
// Name: static setGetClassInstance(NotifySampleDlg* classInstance)
// Desc: set / get function to share a pointer of the class instance of the UI with the static callback.
// Return: Null if new pointer is set, or a pointer to the instance if instance == NULL 
//------------------------------------------------------------------------------
NotifySampleDlg* NotifySampleDlg::setGetClassInstance(NotifySampleDlg* classInstance)
{
	static NotifySampleDlg* statInstance = NULL;
	
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
// NOTE: FEISC callback doesn't provide a pAny, that's why this workaround is neccessary
//	 for FEISC_EVENT_INIT but not for FEDM_TASK_INIT
//------------------------------------------------------------------------------
void NotifySampleDlg::protCallBackHelper(int iMsg,int iLen)
{
	NotifySampleDlg* classInstance = setGetClassInstance(NULL);
	
	emit(classInstance->onProtocolEvent(iMsg, iLen));
}

//------------------------------------------------------------------------------
// Name: displayProtocol(int iMsg,int iLen)
// Desc: displays protocol messages in the protocol window. Triggered by 
// 	 onProtocolEvent() which is triggered in the static helper function protCallBackHelper()
//------------------------------------------------------------------------------
void NotifySampleDlg::displayProtocol(int iMsg,int iLen)
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
// Name: cbsFct1(void* pAny, int iError, unsigned char ucCmd)
// Desc: Static callback for notification event. Indirects call to displayData()
//------------------------------------------------------------------------------
void NotifySampleDlg::cbsFct1(void* pAny, int iError, unsigned char ucCmd)
{
	NotifySampleDlg* classInstance = (NotifySampleDlg*)pAny;
	
	emit(classInstance->onDataEvent(pAny, iError, ucCmd));
}

//------------------------------------------------------------------------------
// Name: listenButtonClicked()
// Desc: starts / stops listening to inventory notifications
//------------------------------------------------------------------------------
void NotifySampleDlg::listenButtonClicked()
{

	if (m_bRunning)
	{	
		ui.listenPushButton->setText("&Listen");
		m_bRunning = false;
		m_iscReader->CancelAsyncTask();
	}
	else
	{
		
		ui.listenPushButton->setText("&Stop");
		m_bRunning = true;
		startTask();
		
	}
}

char* NotifySampleDlg::getISOMfrCode(unsigned char ucMfr)
{
	switch(ucMfr)
	{
	case 0x01:
		return FEISC_ISO_MOTOROLA;
	case 0x02:
		return FEISC_ISO_STM;
	case 0x03:
		return FEISC_ISO_HITACHI;
	case 0x04:
		return FEISC_ISO_PHILIPS;
	case 0x05:
		return FEISC_ISO_INFINEON;
	case 0x06:
		return FEISC_ISO_CYLINC;
	case 0x07:
		return FEISC_ISO_TI;
	case 0x08:
		return FEISC_ISO_FUJITSU;
	case 0x09:
		return FEISC_ISO_MATSUSHITA;
	case 0x0A:
		return FEISC_ISO_NEC;
	case 0x0B:
		return FEISC_ISO_OKI;
	case 0x0C:
		return FEISC_ISO_TOSHIBA;
	case 0x0D:
		return FEISC_ISO_MITSUBISHI;
	case 0x0E:
		return FEISC_ISO_SAMSUNG;
	case 0x0F:
		return FEISC_ISO_HYUNDAI;
	case 0x10:
		return FEISC_ISO_LG;
	case 0x11:
		return FEISC_ISO_EMOSYN;
	case 0x12:
		return FEISC_ISO_INSIDE;
	case 0x13:
		return FEISC_ISO_ORGA;
	case 0x14:
		return FEISC_ISO_SHARP;
	case 0x15:
		return FEISC_ISO_ATMEL;
	case 0x16:
		return FEISC_ISO_EM;
	case 0x17:
		return FEISC_ISO_KSW;
	case 0x19:
		return FEISC_ISO_XICOR;
	}

	return "unknown manufacturer";
}
