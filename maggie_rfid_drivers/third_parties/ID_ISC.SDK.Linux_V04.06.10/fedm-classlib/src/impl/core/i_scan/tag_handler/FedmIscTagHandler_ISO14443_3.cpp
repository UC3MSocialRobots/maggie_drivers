/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO14443_3.cpp                 |
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
					
Author         		:	Markus Hultsch
Begin        		:	03.04.2009

Version       		:	03.01.00 / 10.02.2010 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID® classic-pro transponder ISO14443-3

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "FedmIscTagHandler_ISO14443_3.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscTagHandler_ISO14443_3
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_ISO14443_3::FedmIscTagHandler_ISO14443_3(
	FEDM_ISCReader* pReader, 
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_2(pReader, FedmIscTagHandler::TYPE_ISO14443_3, pTabItem)
{
	if(m_pTabItem != NULL)
	{
		if(m_pTabItem->m_ucTrType == FEDM_ISC_TR_TYPE_ISO14443A)
			m_sTagName = "ISO 14443-3 Type A";
		else if(m_pTabItem->m_ucTrType == FEDM_ISC_TR_TYPE_ISO14443B)
			m_sTagName = "ISO 14443-3 Type B";
	}
}

FedmIscTagHandler_ISO14443_3::FedmIscTagHandler_ISO14443_3(
	FEDM_ISCReader* pReader, 
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_2(pReader, uiTagHandlerType, pTabItem)
{
	if(m_pTabItem != NULL)
	{
		if(m_pTabItem->m_ucTrType == FEDM_ISC_TR_TYPE_ISO14443A)
			m_sTagName = "ISO 14443-3 Type A";
		else if(m_pTabItem->m_ucTrType == FEDM_ISC_TR_TYPE_ISO14443B)
			m_sTagName = "ISO 14443-3 Type B";
	}
}

FedmIscTagHandler_ISO14443_3::~FedmIscTagHandler_ISO14443_3()
{
}

// set all data members to 0
/*void FedmIscTagHandler_ISO14443_3::Init()
{
	FedmIscTagHandler_ISO14443_2::Init();
}*/

int FedmIscTagHandler_ISO14443_3::GetProtocolInfo(
	unsigned char& ucMaxFrameSize,
	unsigned char& ucProtocolType )
{
	FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__);

	ucMaxFrameSize = ((m_pTabItem->m_ucProtoInfo & 0xF0) >> 4);
	ucProtocolType = (m_pTabItem->m_ucProtoInfo & 0x0F);

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

  Return value		:	length (>1)- number of bytes in Card Info
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_3::GetCardInfo(
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
		if(uiCardInfoBufferSize < (unsigned int)(4 + m_pReader->m_TmpData[2048 + 3] - 1))
		{
			FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - uiCardInfoBufferSize < " << (unsigned int)(4 + m_pReader->m_TmpData[2048 + 3] - 1));
			return FEDM_ERROR_BUFFER_LENGTH;
		}

		memcpy(&pucCardInfo[0], &m_pReader->m_TmpData[2048], 4);
		memcpy(&pucCardInfo[4], &m_pReader->m_TmpData[2052], pucCardInfo[3]-1); // pucCardInfo[3]-1 is TL
		iLen = 4 + pucCardInfo[3]-1;
		break;

	case 0x02: // card information of an ISO14443-4 Type-B Transponder
		if(uiCardInfoBufferSize < 4)
		{
			FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - uiCardInfoBufferSize < 4");
			return FEDM_ERROR_BUFFER_LENGTH;
		}

		memcpy(&pucCardInfo[0], &m_pReader->m_TmpData[2048], 4);
		iLen = 4;
		break;

	case 0x03: // card information of an ISO14443-3 Type-A Transponder
		if(uiCardInfoBufferSize < 3)
		{
			FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - uiCardInfoBufferSize < 3");
			return FEDM_ERROR_BUFFER_LENGTH;
		}

		memcpy(&pucCardInfo[0], &m_pReader->m_TmpData[2048], 3);
		iLen = 3;
		break;

	default:
		return FEDM_ERROR_NO_DATA;
	}

	return iLen;
}


/***************************************************************************
  Begin        		:	31.03.2009 / M. Hultsch

  Version       	:	03.01.00 / 11.05.2009 / M. Hultsch

  Function			:	[0xB0][0xC0] Halt command

						NOTE: only for selected tags

  Parameters		:	-

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_3::Halt()
{
	FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__);

	FEDM_CHK3(m_pTabItem);
	FEDM_CHK3(m_pReader);

	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char)0xC0));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE, (unsigned char)FEDM_ISC_ISO_MODE_SEL));

	// ... and execute
	return m_pReader->SendProtocol(0xB0);
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
