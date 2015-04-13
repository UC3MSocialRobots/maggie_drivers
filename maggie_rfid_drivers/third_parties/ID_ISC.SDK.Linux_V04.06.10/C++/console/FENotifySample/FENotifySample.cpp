/*-------------------------------------------------------
|                                                       |
|                  fenotifysample.cpp                   |
|                                                       |
---------------------------------------------------------

Copyright  2006-2014	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : info@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	11.07.2006

Version       		:	01.00.01 / 27.10.2014 / M. Hultsch

Operation Systems	:	Linux
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include "feisc.h"
#include "FedmIscCore.h"
#include "ISO_IEC_7816_6_ICManufacturerRegistration.h"

// STL - Standard Template Library
using namespace std;



void cbsFct1(void* pAny, int iError, unsigned char ucCmd);	// callback-function
void Display0x22Response(FEDM_ISCReaderModule* pReader, int iError);
void Display0x6EResponse(FEDM_ISCReaderModule* pReader, int iError);
char* GetISOMfrCode(unsigned char ucMfr);




int main(int argc, char *argv[])
{
	int iBack = 0;
	char cErrorText[256];

	FEDM_ISCReaderModule*	pReader = new FEDM_ISCReaderModule;
	FEDM_TASK_INIT			TaskInit;

	// initialize reader object
	iBack = pReader->SetTableSize(FEDM_ISC_BRM_TABLE, 256);	// max 256 transponder with each notification
	if(iBack < 0)
	{
		pReader->GetErrorText(cErrorText, iBack);
		printf("SetTableSize returns %d: %s.  FENotifySample finished\n", iBack, cErrorText);
		delete pReader;
		return -1;
	}


	// setup initialization structur
	TaskInit.iConnectByHost	= 0;
	TaskInit.pAny			= (void*)pReader;
	TaskInit.uiFlag			= FEDM_TASKCB1;
	TaskInit.uiUse			= FEDM_TASKID_NOTIFICATION;
	memset(TaskInit.cIPAdr, 0, 16);
	TaskInit.iPortNr		= 10005;
	TaskInit.cbFct1			= cbsFct1;
	TaskInit.iNotifyWithAck	= 0;
	TaskInit.uiTimeout		= 30;
    TaskInit.bCryptoMode    = false;
    TaskInit.bKeepAlive     = true;
    TaskInit.uiKeepAliveIdleTime    = 500;
    TaskInit.uiKeepAliveProbeCount  = 5;
    TaskInit.uiKeepAliveIntervalTime = 500;


	iBack = pReader->StartAsyncTask(&TaskInit);
	if(iBack < 0)
	{
		pReader->GetErrorText(cErrorText, iBack);
		printf("StartAsyncTask returns %d: %s.  FENotifySample finished\n", iBack, cErrorText);
		delete pReader;
		return -1;
	}


	// main loop
	while(true)
	{
		// do nothing
		usleep(10000);	// 10.000 s
	}


	pReader->CancelAsyncTask();

	printf("FENotifySample finished\n");
	
	delete pReader;
	return EXIT_SUCCESS;
}


// callback-function
void cbsFct1(void* pAny, int iError, unsigned char ucCmd)
{
	FEDM_ISCReaderModule* pReader = (FEDM_ISCReaderModule*)pAny;

	switch(ucCmd)
	{
	case 0x22:	// Read Buffer Response
		Display0x22Response(pReader, iError);
		break;

	case 0x6E:	// Reader Diagnostic Response
		Display0x6EResponse(pReader, iError);
		break;
	}
}



void Display0x22Response(FEDM_ISCReaderModule* pReader, int iError)
{
	bool bSNR = false;
	bool bDB = false;
	bool bTime = false;
	bool bDate = false;
	bool bAnt = false;
	bool bIn = false;
	bool bExt = false;
	bool bEpc = false;

	unsigned char	ucTrData[3];
	unsigned char	ucTrType = 0;
	unsigned char	ucDBAdr = 0;
	unsigned char	ucDBSize = 0;
	unsigned char	ucTmp;
	unsigned char	ucSnr[32];
	unsigned char	ucSnrLen = 0;
	unsigned char	ucC1G2PC[2];
	unsigned char	ucTimer[4];
	unsigned char	ucDate[5];
	unsigned char	ucAntNr = 0;
	unsigned char	ucInput = 0;
	unsigned char	ucState = 0;
	
	unsigned int	uiDBN = 0;
	unsigned int	uiTmp = 0;
	
	string sTmp;

	static char g_str[1024];
	static char g_cErrorText[256];

	if(iError < 0)
	{
		pReader->GetErrorText(g_cErrorText, iError);
		printf("%s\n", g_cErrorText);
		return;
	}

	if(! (	iError==0x00 || 
			iError==0x83 || 
			iError==0x84 || 
			iError==0x93 || 
			iError==0x94 ) )
	{
		pReader->GetErrorText(g_cErrorText, iError);
		printf("%s\n", g_cErrorText);
		return;
	}


	pReader->GetData(FEDM_ISC_TMP_ADV_BRM_TRDATA1_SNR,	&bSNR);
	pReader->GetData(FEDM_ISC_TMP_ADV_BRM_TRDATA1_DB,	&bDB);
	pReader->GetData(FEDM_ISC_TMP_ADV_BRM_TRDATA1_ANT,	&bAnt);
	pReader->GetData(FEDM_ISC_TMP_ADV_BRM_TRDATA1_TIME,	&bTime);
	pReader->GetData(FEDM_ISC_TMP_ADV_BRM_TRDATA1_DATE,	&bDate);
	pReader->GetData(FEDM_ISC_TMP_ADV_BRM_TRDATA1_EXT,	&bExt);
	pReader->GetData(FEDM_ISC_TMP_ADV_BRM_TRDATA2_IN,	&bIn);

	printf("\n[0x22] Read Buffer   >> ");
	int iState = FEISC_GetLastState(pReader->GetReaderHnd(), g_str);
	printf("Statusbyte: 0x%02X (%s)\n", (unsigned char)iState, g_str);


	pReader->GetData(FEDM_ISC_TMP_ADV_BRM_TRDATA1, &ucTrData[0]);
	pReader->GetData(FEDM_ISC_TMP_ADV_BRM_TRDATA2, &ucTrData[1]);
	
	if(ucTrData[0] & 0x80) // EXT-Flag
		printf("  TR-DATA....0x%02X 0x%02X\n", ucTrData[0], ucTrData[1]);
	else
		printf("  TR-DATA....0x%02X\n", ucTrData[0]);

	pReader->GetData(FEDM_ISC_TMP_ADV_BRM_RECSETS, &uiTmp);
	printf("  DATA-SETS..%d\n", uiTmp);

	for(int iIdx=0; iIdx<pReader->GetTableLength(FEDM_ISC_BRM_TABLE); iIdx++)
	{
		bEpc = false;
		printf("  %d. Transponder\n", (int)(iIdx+1));

		pReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_SNR_LEN, &ucSnrLen);

		if(bSNR && ucSnrLen>0)
		{
			pReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_TRTYPE, &ucTrType);

			switch(ucTrType)
			{
			// HF-Transponder type
			case FEDM_ISC_TR_TYPE_ICODE1:
				sprintf(g_str, " (Philips I-Code1)");
				break;

			// HF-Transponder type
			case FEDM_ISC_TR_TYPE_TAGIT:
				sprintf(g_str, " (Texas Instruments Tag-it HF)");
				break;
			
			// HF-Transponder type
			case FEDM_ISC_TR_TYPE_ISO15693:
				pReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_SNR, ucSnr, 8);
				sprintf(g_str, " (ISO15693 - %s)", GetISOMfrCode(ucSnr[1]));
				break;
			
			// HF-Transponder type
			case FEDM_ISC_TR_TYPE_ISO14443A:
				sprintf(g_str, " (ISO14443-A)");
				break;
			
			// HF-Transponder type
			case FEDM_ISC_TR_TYPE_ISO14443B:
				sprintf(g_str, " (ISO14443-B)");
				break;

			// HF-Transponder type
			case FEDM_ISC_TR_TYPE_EPC:
				pReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_IS_EPC, &bEpc);
				if(bEpc)
				{
					pReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC_TYPE, &ucTmp);
					switch(ucTmp)
					{
					case FEDM_ISC_EPC_TYPE_1:
						sprintf(g_str, " (EPC 96 bit)");
						break;
					case FEDM_ISC_EPC_TYPE_2:
						sprintf(g_str, " (EPC 64 bit type I)");
						break;
					case FEDM_ISC_EPC_TYPE_3:
						sprintf(g_str, " (EPC 64 bit type II)");
						break;
					case FEDM_ISC_EPC_TYPE_4:
						sprintf(g_str, " (EPC 64 bit type III)");
						break;
					default:
						sprintf(g_str, " (unknown EPC type)");
					}
				}
				break;

			// HF-Transponder type
			case FEDM_ISC_TR_TYPE_ICODE_UID:
				sprintf(g_str, " (I-CODE UID)");
				break;

			// UHF-Transponder type
			case FEDM_ISC_TR_TYPE_ISO18000_6_A:	// 0x80
				sprintf(g_str, " (18000-6-A)");
				break;

			// UHF-Transponder type
			case FEDM_ISC_TR_TYPE_ISO18000_6_B:	// 0x81
				sprintf(g_str, " (18000-6-B)");
				break;
			
			// UHF-Transponder type
			case FEDM_ISC_TR_TYPE_EM4222:		// 0x83
				sprintf(g_str, " (EM4222)");
				break;
			
			// UHF-Transponder type
			case FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2:	// 0x84
				pReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_IS_EPC, &bEpc);
				if(bEpc)
				{
					pReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC_TYPE, &ucTmp);
					switch(ucTmp)
					{
					case FEDM_ISC_EPC_TYPE_1:
						sprintf(g_str, " (EPC Class 1 Gen 2 - EPC 96 bit)");
						break;
					case FEDM_ISC_EPC_TYPE_2:
						sprintf(g_str, " (EPC Class 1 Gen 2 - EPC 64 bit type I)");
						break;
					case FEDM_ISC_EPC_TYPE_3:
						sprintf(g_str, " (EPC Class 1 Gen 2 - EPC 64 bit type II)");
						break;
					case FEDM_ISC_EPC_TYPE_4:
						sprintf(g_str, " (EPC Class 1 Gen 2 - EPC 64 bit type III)");
						break;
					default:
						sprintf(g_str, " (EPC Class 1 Gen 2 - unknown EPC type)");
					}
				}
				else
				{
					sprintf(g_str, " (EPC Class 1 Gen 2)");
				}
				break;
			
			// UHF-Transponder type
			case FEDM_ISC_TR_TYPE_EPC_CLASS0:	// 0x88
				pReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_IS_EPC, &bEpc);
				if(bEpc)
				{
					pReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC_TYPE, &ucTmp);
					switch(ucTmp)
					{
					case FEDM_ISC_EPC_TYPE_1:
						sprintf(g_str, " (EPC Class 0 - EPC 96 bit)");
						break;
					case FEDM_ISC_EPC_TYPE_2:
						sprintf(g_str, " (EPC Class 0 - EPC 64 bit type I)");
						break;
					case FEDM_ISC_EPC_TYPE_3:
						sprintf(g_str, " (EPC Class 0 - EPC 64 bit type II)");
						break;
					case FEDM_ISC_EPC_TYPE_4:
						sprintf(g_str, " (EPC Class 0 - EPC 64 bit type III)");
						break;
					default:
						sprintf(g_str, " (EPC Class 0 - unknown EPC type)");
					}
				}
				else
				{
					sprintf(g_str, " (EPC Class 0)");
				}
				break;
			
			// UHF-Transponder type
			case FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN1:	// 0x89
				pReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_IS_EPC, &bEpc);
				if(bEpc)
				{
					pReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC_TYPE, &ucTmp);
					switch(ucTmp)
					{
					case FEDM_ISC_EPC_TYPE_1:
						sprintf(g_str, " (EPC Class 1 Gen 1 - EPC 96 bit)");
						break;
					case FEDM_ISC_EPC_TYPE_2:
						sprintf(g_str, " (EPC Class 1 Gen 1 - EPC 64 bit type I)");
						break;
					case FEDM_ISC_EPC_TYPE_3:
						sprintf(g_str, " (EPC Class 1 Gen 1 - EPC 64 bit type II)");
						break;
					case FEDM_ISC_EPC_TYPE_4:
						sprintf(g_str, " (EPC Class 1 Gen 1 - EPC 64 bit type III)");
						break;
					default:
						sprintf(g_str, " (EPC Class 1 Gen 1 - unknown EPC type)");
					}
				}
				else
				{
					sprintf(g_str, " (EPC Class 1 Gen 1)");
				}
				break;

			default:
				sprintf(g_str, " (unknown)");
				break;
			}

			printf("    TR-TYPE....: 0x%02X (%s)\n", ucTrType, g_str);


			// PC of gen2
			if(ucTrType == FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2)
			{
				pReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC_CL1_GEN2_PC, ucC1G2PC, 2);
				printf("    PC.........: %02X%02X\n", ucC1G2PC[0], ucC1G2PC[1]);
			}


			if(bEpc)
			{
				pReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC, sTmp);
				pReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_EPC_TYPE, &ucTmp);

				switch(ucTmp)
				{
				case FEDM_ISC_EPC_TYPE_1:
				case FEDM_ISC_EPC_TYPE_2:
				case FEDM_ISC_EPC_TYPE_3:
				case FEDM_ISC_EPC_TYPE_4:
					printf("    EPC........: %s\n", sTmp.c_str());
					break;
				default:
					printf("    EPC........: ??\n");
				}
			}
			else
			{
				pReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_SNR, sTmp);
				printf("    SNR........: %s\n", sTmp.c_str());
			}
		}

		if(bDB)
		{
			pReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_DBN, &uiDBN);
			if(uiDBN > 0)
			{
				printf("    DB-N.......: 0x%04X\n", uiDBN);

				pReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_BLOCK_SIZE, &ucDBSize);
				printf("    DB-SIZE....: 0x%02X\n", ucDBSize);

				printf("    DB.........: ");
				for(int iCnt=0; iCnt<(int)uiDBN; iCnt++)
				{
					pReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_RxDB, ucDBAdr+iCnt, sTmp);
					printf(sTmp.c_str());

					if(iCnt<(int)(uiDBN-1))
						printf("\n                  ");
				}

				printf("\n");
			}
		}

		if(bDate)
		{
			pReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_DATE, ucDate, 5);

			printf("    DATE.......: %02d/%02d/%02d%02d\n", ucDate[2], ucDate[3], ucDate[0], ucDate[1]);
		}

		if(bTime)
		{
			pReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_TIMER, ucTimer, 4);
			pReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_TIMER, &uiTmp);

			unsigned int uiHours	 = ucTimer[0];
			unsigned int uiMinutes	 = ucTimer[1];
			unsigned int uiMilliSecs = (ucTimer[2]<<8) + ucTimer[3];
			unsigned int uiSeconds	 = uiMilliSecs / 1000;
			uiMilliSecs = uiMilliSecs % 1000;
			
			printf("    TIME ......: %08X, (%02d:%02d:%02d.%03d)\n", uiTmp, uiHours, uiMinutes, uiSeconds, uiMilliSecs);
		}

		if(bAnt && ((bSNR && ucSnrLen>0) || (bDB && uiDBN>0)))
		{
			pReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_ANT_NR, &ucAntNr);
			
			printf("    Antenna No.: 0x%02X\n", ucAntNr);
		}

		if(bExt && bIn)
		{
			pReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_INPUT, &ucInput);
			
			printf("    INPUT......: 0x%02X\n", ucInput);

			pReader->GetTableData(iIdx, FEDM_ISC_BRM_TABLE, FEDM_ISC_DATA_STATE, &ucState);
			
			printf("    STATE......: 0x%02X\n", ucState);
		}

	} // for(int iIdx=0; iIdx<pReader->GetTableLength(FEDM_ISC_BRM_TABLE); iIdx++)
	
}



void Display0x6EResponse(FEDM_ISCReaderModule* pReader, int iError)
{
	unsigned char	ucData[13];
static char g_str[1024];

	pReader->GetData(FEDM_ISC_TMP_DIAG_DATA, &ucData[0], 13);
	
	printf("\n[0x6E] Reader Diagnostic   >> ");
	int iState = FEISC_GetLastState(pReader->GetReaderHnd(), g_str);
	printf("Statusbyte: 0x%02X (%s)\n", (unsigned char)iState, g_str);

	if(ucData[0] & 0x01)
		printf("  0: RF-Hardware.......Error\n");
	else
		printf("  0: RF-Hardware..........OK\n");

	if(ucData[0] & 0x02)
		printf("  1: Noise-Level.......Error\n");
	else
		printf("  1: Noise-Level..........OK\n");

	if(ucData[0] & 0x10)
		printf("  4: False Power.........Yes\n");
	else
		printf("  4: False Power..........No\n");

	if(ucData[0] & 0x20)
		printf("  5: Channel Allocation..Yes\n");
	else
		printf("  5: Channel Allocation...No\n");

	printf("\n");

	if(ucData[1] & 0x01)
		printf("  0: Ant1 |Z| <> 50 Ohm.......Yes\n");
	else
		printf("  0: Ant1 |Z| <> 50 Ohm........No\n");

	if(ucData[1] & 0x02)
		printf("  1: Ant2 |Z| <> 50 Ohm.......Yes\n");
	else
		printf("  1: Ant2 |Z| <> 50 Ohm........No\n");

	if(ucData[1] & 0x04)
		printf("  2: Ant3 |Z| <> 50 Ohm.......Yes\n");
	else
		printf("  2: Ant3 |Z| <> 50 Ohm........No\n");

	if(ucData[1] & 0x08)
		printf("  3: Ant4 |Z| <> 50 Ohm.......Yes\n");
	else
		printf("  3: Ant4 |Z| <> 50 Ohm........No\n");

}


char* GetISOMfrCode(unsigned char ucMfr)
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
