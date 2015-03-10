/*-------------------------------------------------------
|                                                       |
|                  myAXXESS_NotifySample.cpp            |
|                                                       |
---------------------------------------------------------

Copyright  2009-2010	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : info@feig.de
						Internet : http://www.feig.de
					
Author         	:	Manuel Sahm
Begin        	:	05.10.2009

Version       	:	01.01.00 / 12.02.2010 / M. Hultsch

Operation Systems	:	Linux
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include "../../../fedm-classlib/src/myaxxess/FedmIscMyAxxessReader.h"

// STL - Standard Template Library
using namespace std;

int cbsEvent( void* pAny, int iError, FEDM_ISC_MYAXXESS_EVENT_TABLE_ITEM* pEventItem, unsigned char& ucAction, char* cIPAdr, int iPortNr );
void cbsKeepAlive( void* pAny, int iError, unsigned int uiErrorFlags, unsigned int uiTableSize, unsigned int uiTableLength, char* cIPAdr, int iPortNr);

int main(int argc, char *argv[])
{
	int iBack = 0;
	int iReaderHnd = 0;
	FedmIscMyAxxessReader* m_pReader = NULL;
	unsigned int m_uiPortNr = 10005;

	m_pReader = new FedmIscMyAxxessReader(7, FEDM_MYAXXESS_IDD_FORMAT_HEX);

	iBack = m_pReader->StartEventHandler(m_uiPortNr, m_pReader, cbsEvent, cbsKeepAlive, -1, NULL);
	if(iBack < 0)
	{
		printf("StartEventHandler returns %d: %s.  myAXXESS_NotifySample finished\n", iBack, m_pReader->GetErrorText(iBack));
		delete m_pReader;
		return -1;
	}
	else
	{
		printf("myAXXESS_NotifySample is running on Port %d\n", m_uiPortNr);
	}


	// main loop
	while(true)
	{
		// do nothing
		usleep(10000);	// 10.000 s
	}


	iReaderHnd = m_pReader->GetReaderObject()->GetReaderHnd();
	if (iReaderHnd != 0)
	{
		m_pReader->StopEventHandler();
	}

	printf("myAXXESS_NotifySample finished\n");
	
	delete m_pReader;
	return EXIT_SUCCESS;
}

int cbsEvent(	void* pAny,					// pAny
		int iError,					// error value
		FEDM_ISC_MYAXXESS_EVENT_TABLE_ITEM* pEventItem, // event record
		unsigned char& ucAction,			// reference to action
		char* cIPAdr,					// IP-Address
		int iPortNr )					// IP-Port
{
	if((FedmIscMyAxxessReader*)pAny == NULL)
		return 0;

	if(pEventItem == NULL)
		return 0;

	FedmIscMyAxxessReader* pMyAxxessReader = (FedmIscMyAxxessReader*)pAny;

	char	szTimestamp[64];
	char	szTmp[32];
	string sReport;
	string sTmp;

	sReport = "\n";
	sReport += "[Event]\n";
	sReport += "EventID.......";
	switch(pEventItem->ucEventID)
	{
	case FEDM_MYAXXESS_EVENT_ACCESS:
		sReport += "ACCESS";
		break;

	case FEDM_MYAXXESS_EVENT_IO:
		sReport += "INPUT";
		break;

/*	case FEDM_MYAXXESS_EVENT_ALARM:
		sReport += "ALARM";
		break;

	case FEDM_MYAXXESS_EVENT_REQUEST:
		sReport += "ACCESS REQUEST";
		break;*/
	
	default:
		sReport += "Unknown";
		break;
	}
	sReport += "\n";

	if(iError != 0x00 && iError != 0x93)
	{
		sReport += pMyAxxessReader->GetErrorText(iError);
		printf("%s\n", sReport.c_str());
		return 0;
	}

	if(pEventItem->ucDataLayout & 0x01)
	{
		sReport += "IDD...........";
		FEDM_ConvHexUCharToHexStr(pEventItem->ucIDD, pEventItem->ucIDDLength, sTmp);
		sReport += sTmp;
		sReport += "\n";
	}

	if(pEventItem->ucDataLayout & 0x02)
	{
		sReport += "Timestamp.....";
		sprintf(szTimestamp,
				"Date=20%02d-%02d-%02d Time=%02d:%02d",	
				pEventItem->ucYear, 
				pEventItem->ucMonth, 
				pEventItem->ucDay, 
				pEventItem->ucHour, 
				pEventItem->ucMinute);
		sReport += szTimestamp;
		sReport += "\n";
	}

	if(pEventItem->ucDataLayout & 0x04)
	{
		sReport += "Action........";
		sprintf(szTmp, "0x%02X    (", pEventItem->ucAction);
		sReport += szTmp;
		if(pEventItem->ucAction & 0x01)
			sReport += "Access permitted";
		else if(! (pEventItem->ucAction & 0x01))
			sReport += "Access denied";
		
		sReport += ")\n";

		sReport += "Error.........";
		sprintf(szTmp, "0x%04X  (", pEventItem->uiError);
		sReport += szTmp;
		if(pEventItem->uiError & 0x0001)
			sReport += "RTC-Error";
		if( (pEventItem->uiError & 0x0001) && (pEventItem->uiError & (~0x0001)))
			sReport += ", ";
		if(pEventItem->uiError & 0x0002)
			sReport += "Metadata corrupt";
		if( (pEventItem->uiError & 0x0002) && (pEventItem->uiError & (~0x0003)))
			sReport += ", ";
		if(pEventItem->uiError & 0x0004)
			sReport += "Timezone Table CRC-Error";
		if( (pEventItem->uiError & 0x0004) && (pEventItem->uiError & (~0x0007)))
			sReport += ", ";
		if(pEventItem->uiError & 0x0008)
			sReport += "Holiday Table CRC-Error";
		if( (pEventItem->uiError & 0x0008) && (pEventItem->uiError & (~0x000F)))
			sReport += ", ";
		if(pEventItem->uiError & 0x0010)
			sReport += "Access Table CRC-Error";
		if( (pEventItem->uiError & 0x0010) && (pEventItem->uiError & (~0x001F)))
			sReport += ", ";
		if(pEventItem->uiError & 0x0040)
			sReport += "Reader Config mismatch";
		if( (pEventItem->uiError & 0x0040) && (pEventItem->uiError & (~0x007F)))
			sReport += ", ";
		if(pEventItem->uiError & 0x0080)
			sReport += "Table Update";
		if( (pEventItem->uiError & 0x0080) && (pEventItem->uiError & (~0x00FF)))
			sReport += ", ";
		if(pEventItem->uiError & 0x8000)
			sReport += "Overflow";

		sReport += ")\n";
	}

	if(pEventItem->ucDataLayout & 0x08)
	{
		sReport += "Input.........";
		sprintf(szTmp, "0x%02X", pEventItem->ucInput);
		sReport += szTmp;
		sReport += "\n";
	}
	
	sReport += "Source........";
	sReport += cIPAdr;
	sReport += ":";
	sprintf(szTmp, "%d", iPortNr);
	sReport += szTmp;
	sReport += "\n";

	printf("%s\n", sReport.c_str());
	
	return 0;
}


void cbsKeepAlive(	void* pAny,			// pAny
			int iError,			// error value
			unsigned int uiErrorFlags,	// error flags from my axxess Reader
			unsigned int uiTableSize,	// event table size
			unsigned int uiTableLength,	// event table length
			char* cIPAdr,			// IP-Address
			int iPortNr)			// IP-Port
{
	if((FedmIscMyAxxessReader*)pAny == NULL)
		return;

	char	szTmp[32];
	string sReport;

	sReport = "\n";
	sReport += "[KeepAlive]\n";

	sReport += "Error.........";
	sprintf(szTmp, "0x%04X  (", uiErrorFlags);
	sReport += szTmp;
	if(uiErrorFlags & 0x0001)
		sReport += "RTC-Error";
	if( (uiErrorFlags & 0x0001) && (uiErrorFlags & (~0x0001)))
		sReport += ", ";
	if(uiErrorFlags & 0x0002)
		sReport += "Metadata corrupt";
	if( (uiErrorFlags & 0x0002) && (uiErrorFlags & (~0x0003)))
		sReport += ", ";
	if(uiErrorFlags & 0x0004)
		sReport += "Timezone Table CRC-Error";
	if( (uiErrorFlags & 0x0004) && (uiErrorFlags & (~0x0007)))
		sReport += ", ";
	if(uiErrorFlags & 0x0008)
		sReport += "Holiday Table CRC-Error";
	if( (uiErrorFlags & 0x0008) && (uiErrorFlags & (~0x000F)))
		sReport += ", ";
	if(uiErrorFlags & 0x0010)
		sReport += "Access Table CRC-Error";
	if( (uiErrorFlags & 0x0010) && (uiErrorFlags & (~0x001F)))
		sReport += ", ";
	if(uiErrorFlags & 0x0040)
		sReport += "Reader Config mismatch";
	if( (uiErrorFlags & 0x0040) && (uiErrorFlags & (~0x007F)))
		sReport += ", ";
	if(uiErrorFlags & 0x0080)
		sReport += "Table Update";
	if( (uiErrorFlags & 0x0080) && (uiErrorFlags & (~0x00FF)))
		sReport += ", ";
	if(uiErrorFlags & 0x8000)
		sReport += "Overflow";

	sReport += ")\n";

	sReport += "Table-Size....";
	sprintf(szTmp, "%d\n", uiTableSize);
	sReport += szTmp;
	sReport += "Table-Length..";
	sprintf(szTmp, "%d\n", uiTableLength);
	sReport += szTmp;
	
	sReport += "Source........";
	sReport += cIPAdr;
	sReport += ":";
	sprintf(szTmp, "%d", iPortNr);
	sReport += szTmp;
	sReport += "\n";

	printf("%s\n", sReport.c_str());
}
