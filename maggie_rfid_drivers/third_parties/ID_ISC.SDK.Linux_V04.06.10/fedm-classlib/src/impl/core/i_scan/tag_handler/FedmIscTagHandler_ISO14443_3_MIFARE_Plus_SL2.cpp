/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL2.cpp |
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

Function			:	class for OBID® classic-pro transponder ISO14443-3 MIFARE Plus in Security Level 2

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL2.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL2
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL2::FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL2(
	FEDM_ISCReader* pReader, 
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_3_MIFARE_Plus(pReader, FedmIscTagHandler::TYPE_ISO14443_3_MIFARE_PLUS_SL2, pTabItem)
{
	m_sTagName = "ISO 14443-3 Type A : MIFARE Plus in Security Level 2";
}

FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL2::FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL2(
	FEDM_ISCReader* pReader, 
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_3_MIFARE_Plus(pReader, uiTagHandlerType, pTabItem)
{
	m_sTagName = "ISO 14443-3 Type A : MIFARE Plus in Security Level 2";
}

FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL2::~FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL2()
{
}

// set all data members to 0
/*void FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL2::Init()
{
}*/


/***************************************************************************
  Begin        		:	01.09.2009 / M. Hultsch

  Version       	:	03.01.00 / 01.09.2009 / M. Hultsch

  Function			:	[0xC2][0x75] SL2 AES and Crypto1 Authent

						NOTE: only for selected tags

  Parameters		:	unsigned int uiDBAddr		- [in] address of data block to be authenticated
						unsigned char ucKeyType		- [in] key type of crypto1 key
						unsigned char ucKeyNo		- [in] EEPROM address where the crypto1 key is stored in the reader
						unsigned char ucKeyIndex	- [in] reader key index

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL2::AESandCrypto1Authent(	
	unsigned int uiDBAddr,
	unsigned char ucKeyType,
	unsigned char ucKeyNo,
	unsigned char ucReaderKeyIndex )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_CMD, (unsigned char)0x75));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_KEY_INDEX, ucReaderKeyIndex));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_KEY_TYPE, ucKeyType));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_KEY_NO, ucKeyNo));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_DB_ADR, uiDBAddr));

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
  Begin        		:	04.09.2009 / M. Hultsch

  Version       	:	03.01.00 / 04.09.2009 / M. Hultsch

  Function			:	[0xC2][0x38] SL2 MultiBlock Read

						NOTE: only for selected tags

  Parameters		:	unsigned int uiBlockNo			- [in] first data block address
						unsigned char ucLength			- [in] number of blocks to be read
						unsigned char* pucResponseData	- [in] pointer to response data buffer

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL2::MultiBlockRead(	
	unsigned int uiBlockNo, 
	unsigned char ucLength, 
	unsigned char* pucResponseData )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_CMD, (unsigned char)0x38));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_SRC_BLOCK_NO, uiBlockNo));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_EXT, ucLength));

	// ... and execute
	int iBack = m_pReader->SendProtocol(0xC2);
	if( iBack==0x00 )
	{
		memcpy(pucResponseData, &m_pTabItem->m_ucRxDB[uiBlockNo * 16], ucLength * 16);
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
  Begin        		:	04.09.2009 / M. Hultsch

  Version       	:	03.01.00 / 04.09.2009 / M. Hultsch

  Function			:	[0xC2][0xA8] SL2 MultiBlock Write

						NOTE: only for selected tags

  Parameters		:	unsigned int uiBlockNo			- [in] first data block address
						unsigned char ucLength			- [in] number of blocks to be read
						unsigned char* pucRequestData	- [in] pointer to request data buffer

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL2::MultiBlockWrite(
	unsigned int uiBlockNo, 
	unsigned char ucLength, 
	unsigned char* pucRequestData )
{
	int iErr = 0;

	if(ucLength * 16 > FEDM_ISC_ISO_TABLE_TxDB_SIZE)
		return FEDM_ERROR_ARRAY_BOUNDARY;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_CMD, (unsigned char)0xA8));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_BLOCK_NO, uiBlockNo));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_EXT, ucLength));

	memcpy(&m_pTabItem->m_ucTxDB[uiBlockNo * 16], pucRequestData, ucLength * 16);

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
