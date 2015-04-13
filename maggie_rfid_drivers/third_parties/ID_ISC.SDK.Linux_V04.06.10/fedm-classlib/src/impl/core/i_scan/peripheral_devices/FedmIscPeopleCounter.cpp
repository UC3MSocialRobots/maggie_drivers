/*-------------------------------------------------------
|                                                       |
|			FedmIscPeopleCounter.cpp					|
|                                                       |
---------------------------------------------------------

Copyright © 2010		FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Manuel Sahm
Begin        		:	18.02.2010

Version       		:	03.02.04 / 25.08.2010 / M. Hultsch

Operation Systems	:	independent

Function			:	this class supports the peripheral device called People Counter


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_NO_PD_SUPPORT)

#include "FedmIscPeopleCounter.h"
#include "../FEDM_ISCReader.h"
#include "feisc.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscPeopleCounter
//####################################################################

//////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////
FedmIscPeopleCounter::FedmIscPeopleCounter(FEDM_ISCReader* pReader, unsigned char ucBusAdr)
	: FedmIscPeripheralDevice(pReader, ucBusAdr, FEDM_ISC_PD_PORT_TYPE_RS4XX, FEDM_ISC_PD_TYPE_PEOPLE_COUNTER)
{
}

//////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////
FedmIscPeopleCounter::~FedmIscPeopleCounter()
{
}


/***************************************************************************
  Begin        		:	18.02.2010 / M. Sahm
  
  Version       	:	03.01.01 / 18.02.2010 / M. Sahm

  Function			:	Set Outputs 

  Parameters		:	FEISC_GPC_OUTPUT structure for Output 1, 2 and 3

  Return value		:	0:		FEDM_OK
						<0:		Error Code
***************************************************************************/
int FedmIscPeopleCounter::SetOutput( FEISC_GPC_OUTPUT* pOutput1,
									 FEISC_GPC_OUTPUT* pOutput2,
									 FEISC_GPC_OUTPUT* pOutput3)
{
	int iBack = 0;
	unsigned char* pData = NULL;
	unsigned char ucRecBusAddr = 0x00;
	unsigned char ucCmd = 0x00;


	// Check if at least on pointer is valid
	if((pOutput1 == NULL) && (pOutput2 == NULL) && (pOutput3 == NULL))
	{
		FEDM_RETURN(FEDM_ERROR_NULL_POINTER);
	}

	m_iSendDataLen = 0;

	// Build the "internal" protocol
	m_ucSendData[0] = 0x01;	// Mode Byte
	m_ucSendData[1] = 0x00;	// OUT-N
	m_iSendDataLen += 2;
	pData = &m_ucSendData[2];
	if(pOutput1 != NULL && pOutput1->ucNumber > 0)
	{
		m_ucSendData[1] += 1;
		*pData++ = (pOutput1->ucNumber & 0x0F) | ((pOutput1->ucType << 5) & 0xE0);
		*pData++ = (pOutput1->ucMode & 0x03) | ((pOutput1->ucFrequency << 2) & 0x0C);
		*pData++ = (pOutput1->uiHoldTime >> 8) & 0xFF;
		*pData++ = pOutput1->uiHoldTime & 0xFF;
		m_iSendDataLen += 4;
	}
	if(pOutput2 != NULL && pOutput2->ucNumber > 0)
	{
		m_ucSendData[1] += 1;
		*pData++ = (pOutput2->ucNumber & 0x0F) | ((pOutput2->ucType << 5) & 0xE0);
		*pData++ = (pOutput2->ucMode & 0x03) | ((pOutput2->ucFrequency << 2) & 0x0C);
		*pData++ = (pOutput2->uiHoldTime >> 8) & 0xFF;
		*pData++ = pOutput2->uiHoldTime & 0xFF;
		m_iSendDataLen += 4;
	}
	if(pOutput3 != NULL && pOutput3->ucNumber > 0)
	{
		m_ucSendData[1] += 1;
		*pData++ = (pOutput3->ucNumber & 0x0F) | ((pOutput3->ucType << 5) & 0xE0);
		*pData++ = (pOutput3->ucMode & 0x03) | ((pOutput3->ucFrequency << 2) & 0x0C);
		*pData++ = (pOutput3->uiHoldTime >> 8) & 0xFF;
		*pData++ = pOutput3->uiHoldTime & 0xFF;
		m_iSendDataLen += 4;
	}


	// Build Protocol : SetOutput (0x72)
	m_iSendProtLen = FEISC_BuildSendProtocol(m_iReaderHnd, m_ucBusAdr, FEISC_SET_OUTPUT_EX, m_ucSendData , m_iSendDataLen, m_ucSendProt, 0);
	if(m_iSendProtLen <= 0)
	{
		FEDM_RETURN(m_iSendProtLen);
	}

	// Send Protocol within Piggyback frame
	iBack = FEISC_0x9F_Piggyback_Command(m_pReader->GetReaderHnd(),m_pReader->GetBusAddress(), 0x00, FEDM_ISC_TYPE_ANT_GPC, FEDM_ISC_PD_PORT_TYPE_RS4XX, m_ucSendProt, m_iSendProtLen, m_ucRspProt, &m_iRspProtLen);
	if(iBack < 0)
	{
		FEDM_RETURN(iBack);
	}
	else if(iBack == 0x20) // Reader: External Device communication error
	{
		FEDM_RETURN(iBack);
	}

	// Get Response out of Answer from Protocol 0x72 (SetOutput)
	iBack = FEISC_SplitRecProtocol(m_iReaderHnd, m_ucRspProt, m_iRspProtLen, &ucRecBusAddr, &ucCmd, m_ucRspData, &m_iRspDataLen, 0);
	if(iBack != 0)
	{
		FEDM_RETURN(iBack);
	}

	// Anaylze the Response Data
	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        		:	18.02.2010 / M. Sahm
  
  Version       	:	03.02.00 / 15.06.2010 / M. Sahm

  Function			:	Get the Counter Values

  Parameters		:	<unsigned int&> uiRadar1_Cnt1:	Counter Value 1 of Radar Detector 1
						<unsigned int&> uiRadar1_Cnt2:	Counter Value 2 of Radar Detector 1 
						<unsigned int&> uiRadar2_Cnt1:	Counter Value 1 of Radar Detector 2
						<unsigned int&> uiRadar2_Cnt2:	Counter Value 2 of Radar Detector 2

  Return value		:	0:		FEDM_OK
						<0:		Error Code
***************************************************************************/
int FedmIscPeopleCounter::GetCounter(unsigned int& uiPC_Sense1, unsigned int& uiPC_Sense2, unsigned int& uiPC_Sense3, unsigned int& uiPC_Sense4)
{
	int iBack = 0;
	unsigned char ucRecBusAddr = 0x00;
	unsigned char ucCmd = 0x00;

	//Build the "internal" protocol
	m_ucSendData[0] = 0x00;				//Mode-Byte
	
	// Build Protocol : GetCounter (0x77)
	m_iSendProtLen = FEISC_BuildSendProtocol(m_iReaderHnd, m_ucBusAdr, FEISC_GET_COUNTER, m_ucSendData , 1, m_ucSendProt, 0);
	if(m_iSendProtLen <= 0)
	{
		FEDM_RETURN(m_iSendProtLen);
	}

	// Send Protocol within Piggyback frame
	iBack = FEISC_0x9F_Piggyback_Command(m_pReader->GetReaderHnd(),m_pReader->GetBusAddress(), 0x00, FEDM_ISC_TYPE_ANT_GPC, FEDM_ISC_PD_PORT_TYPE_RS4XX, m_ucSendProt, m_iSendProtLen, m_ucRspProt, &m_iRspProtLen);
	if(iBack < 0)
	{
		FEDM_RETURN(iBack);
	}
	else if(iBack == 0x20) // Reader: External Device communication error
	{
		FEDM_RETURN(iBack);
	}

	// Get Response out of Answer from Protocol 0x77 (GetCounter)
	iBack = FEISC_SplitRecProtocol(m_iReaderHnd, m_ucRspProt, m_iRspProtLen, &ucRecBusAddr, &ucCmd, m_ucRspData, &m_iRspDataLen, 0);
	if(iBack != 0)
	{
		FEDM_RETURN(iBack);
	}

	// Anaylze the Response Data
	if(m_iRspDataLen == 16)
	{
		uiPC_Sense1 = m_ucRspData[3] | m_ucRspData [2] << 8 | m_ucRspData[1] << 16 | m_ucRspData[0] << 24;
		uiPC_Sense2 = m_ucRspData[7] | m_ucRspData [6] << 8 | m_ucRspData[5] << 16 | m_ucRspData[4] << 24;
		uiPC_Sense3 = m_ucRspData[11] | m_ucRspData [10] << 8 | m_ucRspData[9] << 16 | m_ucRspData[8] << 24;
		uiPC_Sense4 = m_ucRspData[15] | m_ucRspData [14] << 8 | m_ucRspData[13] << 16 | m_ucRspData[12] << 24;
	}
	else
	{
		uiPC_Sense1 = 0;
		uiPC_Sense2 = 0;
		uiPC_Sense3 = 0;
		uiPC_Sense4 = 0;
		FEDM_RETURN(FEDM_ERROR_NO_DATA);
	}

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        		:	18.02.2010 / M. Sahm
  
  Version       	:	03.01.01 / 18.02.2010 / M. Sahm

  Function			:	Set the Counter Values

  Parameters		:	<unsigned int> uiRadar1_Cnt1:	Counter Value 1 of Radar Detector 1
						<unsigned int> uiRadar1_Cnt2:	Counter Value 2 of Radar Detector 1
						<unsigned int> uiRadar2_Cnt1:	Counter Value 1 of Radar Detector 2
						<unsigned int> uiRadar2_Cnt2:	Counter Value 2 of Radar Detector 2

  Return value		:	0:		FEDM_OK
						<0:		Error Code
***************************************************************************/
int FedmIscPeopleCounter::SetCounter(unsigned int uiPC_Sense1, unsigned int uiPC_Sense2, unsigned int uiPC_Sense3, unsigned int uiPC_Sense4)
{
	int iBack = 0;
	unsigned char ucRecBusAddr = 0x00;
	unsigned char ucCmd = 0x00;



	// Build the "internal" Protocol
	m_ucSendData[0] = 0x00;						// Mode Byte
	m_ucSendData[1] = uiPC_Sense1 >> 24;
	m_ucSendData[2] = uiPC_Sense1 >> 16;
	m_ucSendData[3] = uiPC_Sense1 >> 8;
	m_ucSendData[4] = uiPC_Sense1;
	m_ucSendData[5] = uiPC_Sense2 >> 24;
	m_ucSendData[6] = uiPC_Sense2 >> 16;
	m_ucSendData[7] = uiPC_Sense2 >> 8;
	m_ucSendData[8] = uiPC_Sense2;
	m_ucSendData[9] = uiPC_Sense3 >> 24;
	m_ucSendData[10] = uiPC_Sense3 >> 16;
	m_ucSendData[11] = uiPC_Sense3 >> 8;
	m_ucSendData[12] = uiPC_Sense3;
	m_ucSendData[13] = uiPC_Sense4 >> 24;
	m_ucSendData[14] = uiPC_Sense4 >> 16;
	m_ucSendData[15] = uiPC_Sense4 >> 8;
	m_ucSendData[16] = uiPC_Sense4;


	// Build Protocol : SetCounter (0x78)
	m_iSendProtLen = FEISC_BuildSendProtocol(m_iReaderHnd, m_ucBusAdr, FEISC_SET_COUNTER, m_ucSendData , 17, m_ucSendProt, 0);
	if(m_iSendProtLen <= 0)
	{
		FEDM_RETURN(m_iSendProtLen);
	}

	// Send Protocol within Piggyback frame
	iBack = FEISC_0x9F_Piggyback_Command(m_pReader->GetReaderHnd(),m_pReader->GetBusAddress(), 0x00, FEDM_ISC_TYPE_ANT_GPC, FEDM_ISC_PD_PORT_TYPE_RS4XX, m_ucSendProt, m_iSendProtLen, m_ucRspProt, &m_iRspProtLen);
	if(iBack < 0)
	{
		FEDM_RETURN(iBack);
	}
	else if(iBack == 0x20) // Reader: External Device communication error
	{
		FEDM_RETURN(iBack);
	}

	// Get Response out of Answer from Protocol 0x78 (SetCounter)
	iBack = FEISC_SplitRecProtocol(m_iReaderHnd, m_ucRspProt, m_iRspProtLen, &ucRecBusAddr, &ucCmd, m_ucRspData, &m_iRspDataLen, 0);
	if(iBack != 0)
	{
		FEDM_RETURN(iBack);
	}

	// Anaylze the Response Data
	FEDM_RETURN(FEDM_OK);
}

#endif // #if !defined(_FEDM_NO_PD_SUPPORT)
