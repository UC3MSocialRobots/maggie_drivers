/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO14443_Innovatron.cpp        |
|                                                       |
---------------------------------------------------------

Copyright © 2012		FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	06.08.2012

Version       		:	04.02.06 / 06.08.2012 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID® classic-pro transponder ISO14443 Innovatron (ISO 14443B')

NOTE				:	this class supports only selected mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "FedmIscTagHandler_ISO14443_Innovatron.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "feisc.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscTagHandler_ISO14443_Innovatron
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_ISO14443_Innovatron::FedmIscTagHandler_ISO14443_Innovatron(
	FEDM_ISCReader* pReader, 
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443(pReader, FedmIscTagHandler::TYPE_ISO14443, pTabItem)
{
	if(m_pTabItem != NULL)
	{
		m_sTagName = "Innovatron (ISO 14443B')";
	}
}

// protected constructor for internal use
FedmIscTagHandler_ISO14443_Innovatron::FedmIscTagHandler_ISO14443_Innovatron(
	FEDM_ISCReader* pReader, 
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443(pReader, uiTagHandlerType, pTabItem)
{
	if(m_pTabItem != NULL)
	{
		m_sTagName = "Innovatron (ISO 14443B')";
	}
}

FedmIscTagHandler_ISO14443_Innovatron::~FedmIscTagHandler_ISO14443_Innovatron()
{
}

unsigned char FedmIscTagHandler_ISO14443_Innovatron::GetVerlog()
{
	return m_pTabItem->m_ucVerlog;
}

unsigned char FedmIscTagHandler_ISO14443_Innovatron::GetConfig()
{
	return m_pTabItem->m_ucConfig;
}

/***************************************************************************
  Begin        		:	06.08.2012 / M. Hultsch

  Version       	:	04.02.06 / 06.08.2012 / M. Hultsch

  Function			:	Returns the ATR, returned with Inventory

  Parameters		:	unsigned char*	pucAtr			- [in] buffer for ATR
						unsigned int	uiAtrBufferLen	- [in]  length of response buffer
						unsigned int&	uiAtrLen		- [out] length of ATR (number of bytes)

  Return value		:	0				- OK, ATR is valid
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_Innovatron::GetAtr(unsigned char* pucAtr, unsigned int uiAtrBufferLen, unsigned int& uiAtrLen)
{
	if(uiAtrBufferLen < (unsigned int)m_pTabItem->m_ucAtrLen)
		return FEDM_ERROR_BUFFER_LENGTH;

	memcpy(pucAtr, m_pTabItem->m_ucAtr, m_pTabItem->m_ucAtrLen);
	uiAtrLen = (unsigned int)m_pTabItem->m_ucAtrLen;

	return FEDM_OK;
}


/***************************************************************************
  Begin        		:	20.11.2009 / M. Hultsch

  Version       	:	03.01.00 / 23.11.2009 / M. Hultsch

  Function			:	return of Card Info read with last TagSelect call

						NOTE: only for selected tags

  Parameters		:	unsigned char& ucFormat				- reference to Format
						unsigned char* pucCardInfo			- pointer to Card Info
						unsigned int   uiCardInfoBufferSize	- size of pucCardInfo

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_Innovatron::GetCardInfo(
	unsigned char& ucCardInfoFormat,
	unsigned char* pucCardInfo,
	unsigned int   uiCardInfoBufferSize )
{
	FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__);

	int iErr = 0;
	int iLen = 0;

	FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B0_RSP_FORMAT, &ucCardInfoFormat));

	switch(ucCardInfoFormat)
	{
	case 0x00: // no card information
		return FEDM_ERROR_NO_DATA;

	case 0x01: // card information of an ISO14443-4 Type-A Transponder
	case 0x02: // card information of an ISO14443-4 Type-B Transponder
	case 0x03: // card information of an ISO14443-3 Type-A Transponder
		return FEDM_ERROR_UNSUPPORTED;

	case 0x04: // card information of an Innovatron (ISO 14443B') Transponder
		if(uiCardInfoBufferSize < 1)
		{
			FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - uiCardInfoBufferSize < 1");
			return FEDM_ERROR_BUFFER_LENGTH;
		}

		pucCardInfo[0] = m_pReader->m_TmpData[2048];
		iLen = 1;
		break;

	default:
		return FEDM_ERROR_NO_DATA;
	}

	return iLen;
}


/***************************************************************************
  Begin        		:	06.08.2012 / M. Hultsch

  Version       	:	04.02.06 / 06.08.2012 / M. Hultsch

  Function			:	[0xB2][0xBB] Innovatron (ISO 14443B') Data Exchange - synchronous call

						NOTE: only for selected tags

  Parameters		:	unsigned char*	pucRequestData		- [in] buffer with APDU request
						unsigned int	uiRequestLen		- [in] length of APDU (number of bytes)
						unsigned char*	pucResponseData		- [out] for synchronous mode: buffer for APDU response
						unsigned int	uiResponseBufferLen	- [in]  for synchronous mode: legth of response bufer
						unsigned int&	uiResponseLen		- [out] length of APDU response (number of bytes)

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_Innovatron::Apdu(
	unsigned char*	pucRequestData,
	unsigned int	uiRequestLen,
	unsigned char*	pucResponseData,
	unsigned int	uiResponseBufferLen,
	unsigned int&	uiResponseLen )
{
	FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__);

	int iDataLen = 2;

	m_ucRequestData[0] = 0xBB;
	m_ucRequestData[1] = 0x00;	// mode

	if(pucRequestData == NULL)
		return FEDM_ERROR_NULL_POINTER;

	if(uiRequestLen == 0)
		return FEDM_ERROR_PARAMETER;

	if(uiRequestLen > (FEDM_ISO14443_INNOVATRON_APDU_BUFFER_SIZE-2))
		return FEDM_ERROR_SENDBUFFER_OVERFLOW;

	memcpy(&m_ucRequestData[2], pucRequestData, uiRequestLen);
	iDataLen += uiRequestLen;

	return FEISC_0xB2_ISOCmd(	m_pReader->GetReaderHnd(),
								m_pReader->GetBusAddress(),
								m_ucRequestData,
								iDataLen,
								m_ucResponseData,
								(int*)&m_uiResponseLen,
								0 );
}

int FedmIscTagHandler_ISO14443_Innovatron::Discard()
{
	FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__);

	m_ucRequestData[0] = 0xBB;
	m_ucRequestData[1] = 0x01;	// mode


	return FEISC_0xB2_ISOCmd(	m_pReader->GetReaderHnd(),
								m_pReader->GetBusAddress(),
								m_ucRequestData,
								2,
								m_ucResponseData,
								(int*)&m_uiResponseLen,
								0 );
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
