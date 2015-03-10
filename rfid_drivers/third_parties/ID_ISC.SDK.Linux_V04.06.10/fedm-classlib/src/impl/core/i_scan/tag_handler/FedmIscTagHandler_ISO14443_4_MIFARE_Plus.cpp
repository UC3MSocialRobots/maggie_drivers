/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO14443_4_MIFARE_Plus.cpp     |
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
Begin        		:	24.08.2009

Version       		:	03.01.00 / 10.02.2010 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID® classic-pro transponder ISO14443-4 MIFARE Plus

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "FedmIscTagHandler_ISO14443_4_MIFARE_Plus.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscTagHandler_ISO14443_4_MIFARE_Plus
// >> supports only ISO Host Commands <<
//####################################################################

	// public constructor not necessary, as this class represents not a real tag type
/*FedmIscTagHandler_ISO14443_4_MIFARE_Plus::FedmIscTagHandler_ISO14443_4_MIFARE_Plus(
	FEDM_ISCReader* pReader, 
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_4(pReader, FedmIscTagHandler::TYPE_ISO14443_4, pTabItem)
{
	m_sTagName = "ISO 14443-4 Type A : MIFARE Plus";

	Init();
}*/

FedmIscTagHandler_ISO14443_4_MIFARE_Plus::FedmIscTagHandler_ISO14443_4_MIFARE_Plus(
	FEDM_ISCReader* pReader, 
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_4(pReader, uiTagHandlerType, pTabItem)
{
	m_sTagName = "ISO 14443-4 Type A : MIFARE Plus";

	Init();
}

FedmIscTagHandler_ISO14443_4_MIFARE_Plus::~FedmIscTagHandler_ISO14443_4_MIFARE_Plus()
{
}

// set all data members to 0
void FedmIscTagHandler_ISO14443_4_MIFARE_Plus::Init()
{
	FedmIscTagHandler::Init();

	m_ucErrorSource	= 0;
	m_uiErrorCode	= 0;
}

unsigned char FedmIscTagHandler_ISO14443_4_MIFARE_Plus::GetErrorSource()
{
	return m_ucErrorSource;
}

unsigned int FedmIscTagHandler_ISO14443_4_MIFARE_Plus::GetErrorCode()
{
	return m_uiErrorCode;
}


/***************************************************************************
  Begin        		:	24.08.2009 / M. Hultsch

  Version       	:	03.01.00 / 25.08.2009 / M. Hultsch

  Function			:	[0xC2][0x70] First Authentication with key from reader

						NOTE: only for selected tags

  Parameters		:	unsigned int uiAesKeyBlockNumber	- [in] AES Key-Block-Number
						unsigned char ucPcdCapLength		- [in] Length of PCD capabilities
						unsigned char* pucPcdCap2			- [in] PCD capabilities
						unsigned char ucKeyIndex			- [in] reader key index

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus::FirstAuthent(	
	unsigned int uiAesKeyBlockNumber,
	unsigned char ucPcdCapLength,
	unsigned char* pucPcdCap2,
	unsigned char ucReaderKeyIndex )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_CMD, (unsigned char)0x70));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_AES_KEY_BLOCK_NO, uiAesKeyBlockNumber));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_PCD_CAP_LEN, ucPcdCapLength));
	if(ucPcdCapLength > 0)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_PCD_CAP, pucPcdCap2, ucPcdCapLength));
	}
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_KEY_INDEX, ucReaderKeyIndex));

	// ... and execute
	int iBack = m_pReader->SendProtocol(0xC2);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xC2_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xC2_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}


/***************************************************************************
  Begin        		:	24.08.2009 / M. Hultsch

  Version       	:	03.01.00 / 25.08.2009 / M. Hultsch

  Function			:	[0xC2][0x76] Following Authentication with key from reader

						NOTE: only for selected tags

  Parameters		:	unsigned int uiAesKeyBlockNumber	- [in] AES Key-Block-Number
						unsigned char ucKeyIndex			- [in] reader key index

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus::FollowingAuthent(	
	unsigned int uiAesKeyBlockNumber,
	unsigned char ucReaderKeyIndex )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_CMD, (unsigned char)0x76));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_AES_KEY_BLOCK_NO, uiAesKeyBlockNumber));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_KEY_INDEX, ucReaderKeyIndex));

	// ... and execute
	int iBack = m_pReader->SendProtocol(0xC2);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xC2_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xC2_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
