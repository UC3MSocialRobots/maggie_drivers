/*-------------------------------------------------------
|                                                       |
|			FedmIscExternalIO.cpp						|
|                                                       |
---------------------------------------------------------

Copyright © 2009-2010	FEIG ELECTRONIC GmbH, All Rights Reserved.
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

Function			:	this class supports the peripheral device called External IO


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_NO_PD_SUPPORT)

#include "FedmIscExternalIO.h"
#include "../FEDM_ISCReader.h"
#include "feisc.h"

#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscExternalIO
//####################################################################

//////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////
FedmIscExternalIO::FedmIscExternalIO(FEDM_ISCReader* pReader, unsigned char ucBusAdr)
	: FedmIscPeripheralDevice(pReader, ucBusAdr, FEDM_ISC_PD_PORT_TYPE_RS4XX, FEDM_ISC_PD_TYPE_EXTERNAL_IO)
{
}

//////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////
FedmIscExternalIO::~FedmIscExternalIO()
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
int FedmIscExternalIO::SetOutput(	FEISC_GPC_OUTPUT* pOutput1,
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

#endif // #if !defined(_FEDM_NO_PD_SUPPORT)
