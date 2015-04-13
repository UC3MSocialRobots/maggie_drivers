/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3.cpp |
|                                                       |
---------------------------------------------------------

Copyright © 2009-2011	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	24.08.2009

Version       		:	03.03.00 / 26.01.2011 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID® classic-pro transponder ISO14443-4 MIFARE Plus in Security Level 3

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3(
	FEDM_ISCReader* pReader, 
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_4_MIFARE_Plus(pReader, FedmIscTagHandler::TYPE_ISO14443_4_MIFARE_PLUS_SL3, pTabItem)
{
	m_sTagName = "ISO 14443-4 Type A : MIFARE Plus in Security Level 3";
}

FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3(
	FEDM_ISCReader* pReader, 
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_4_MIFARE_Plus(pReader, uiTagHandlerType, pTabItem)
{
	m_sTagName = "ISO 14443-4 Type A : MIFARE Plus in Security Level 3";
}

FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::~FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3()
{
}

// set all data members to 0
/*void FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::Init()
{
}*/



/***************************************************************************
  Begin        		:	23.09.2009 / M. Hultsch

  Version       	:	03.01.00 / 23.09.2009 / M. Hultsch

  Function			:	[0xC2][0x78] SL3 Reset Authent

						NOTE: only for selected tags

  Parameters		:	-

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::ResetAuthentication()
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_CMD, (unsigned char)0x78));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_MODE, (unsigned char)0x00));

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

	// [0xC2][0x30] SL3 Read Encrypted
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::Read_Encrypted(
	unsigned int uiBlockNo, 
	unsigned char ucExt, 
	unsigned char* pucResponseData )
{
	return Read(0x30, uiBlockNo, ucExt, pucResponseData);
}

	// [0xC2][0x31] SL3 Read Encrypted Maced
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::Read_EncryptedMaced(
	unsigned int uiBlockNo, 
	unsigned char ucExt, 
	unsigned char* pucResponseData )
{
	return Read(0x31, uiBlockNo, ucExt, pucResponseData);
}

	// [0xC2][0x32] SL3 Read Plain
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::Read_Plain(
	unsigned int uiBlockNo, 
	unsigned char ucExt, 
	unsigned char* pucResponseData )
{
	return Read(0x32, uiBlockNo, ucExt, pucResponseData);
}

	// [0xC2][0x33] SL3 Read Plain Maced
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::Read_PlainMaced(
	unsigned int uiBlockNo, 
	unsigned char ucExt, 
	unsigned char* pucResponseData )
{
	return Read(0x33, uiBlockNo, ucExt, pucResponseData);
}

	// [0xC2][0x34] SL3 Read Encrypted Unmaced
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::Read_EncryptedUnmaced(
	unsigned int uiBlockNo, 
	unsigned char ucExt, 
	unsigned char* pucResponseData )
{
	return Read(0x34, uiBlockNo, ucExt, pucResponseData);
}

	// [0xC2][0x35] SL3 Read Encrypted Unmaced Respond Maced
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::Read_EncryptedUnmaced_RespondMaced(
	unsigned int uiBlockNo, 
	unsigned char ucExt, 
	unsigned char* pucResponseData )
{
	return Read(0x35, uiBlockNo, ucExt, pucResponseData);
}

	// [0xC2][0x36] SL3 Read Plain Unmaced
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::Read_PlainUnmaced(
	unsigned int uiBlockNo, 
	unsigned char ucExt, 
	unsigned char* pucResponseData )
{
	return Read(0x36, uiBlockNo, ucExt, pucResponseData);
}

	// [0xC2][0x37] SL3 Read Plain Unmaced Respond Maced
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::Read_PlainUnmaced_RespondMaced(
	unsigned int uiBlockNo, 
	unsigned char ucExt, 
	unsigned char* pucResponseData )
{
	return Read(0x37, uiBlockNo, ucExt, pucResponseData);
}

	// [0xC2][0xA0] SL3 Write Encrypted
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::Write_Encrypted(
	unsigned int uiBlockNo, 
	unsigned char ucLength,
	unsigned char* pucRequestData )
{
	return Write(0xA0, uiBlockNo, ucLength, pucRequestData);
}

	// [0xC2][0xA1] SL3 Write Encrypted Maced
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::Write_EncryptedMaced(
	unsigned int uiBlockNo, 
	unsigned char ucLength,
	unsigned char* pucRequestData )
{
	return Write(0xA1, uiBlockNo, ucLength, pucRequestData);
}

	// [0xC2][0xA2] SL3 Write Plain
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::Write_Plain(
	unsigned int uiBlockNo, 
	unsigned char ucLength,
	unsigned char* pucRequestData )
{
	return Write(0xA2, uiBlockNo, ucLength, pucRequestData);
}

	// [0xC2][0xA3] SL3 Write Plain Maced
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::Write_PlainMaced(
	unsigned int uiBlockNo, 
	unsigned char ucLength,
	unsigned char* pucRequestData )
{
	return Write(0xA3, uiBlockNo, ucLength, pucRequestData);
}


/***************************************************************************
  Begin        		:	23.09.2009 / M. Hultsch

  Version       	:	03.01.00 / 23.09.2009 / M. Hultsch

  Function			:	[0xC2][0xB0] SL3 Increment Encrypted

						NOTE: only for selected tags

  Parameters		:	unsigned int uiSourceBlockNo	- [in] source data block address
						unsigned int uiValue			- [in] increment value

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::Increment_Encrypted(
	unsigned int uiSourceBlockNo, 
	unsigned int uiValue )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_CMD, (unsigned char)0xB0));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_SRC_BLOCK_NO, uiSourceBlockNo));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_VALUE, uiValue));

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
  Begin        		:	23.09.2009 / M. Hultsch

  Version       	:	03.01.00 / 23.09.2009 / M. Hultsch

  Function			:	[0xC2][0xB1] SL3 Increment Encrypted Maced

						NOTE: only for selected tags

  Parameters		:	unsigned int uiSourceBlockNo	- [in] source data block address
						unsigned int uiValue			- [in] increment value

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::Increment_EncryptedMaced(
	unsigned int uiSourceBlockNo, 
	unsigned int uiValue )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_CMD, (unsigned char)0xB1));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_SRC_BLOCK_NO, uiSourceBlockNo));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_VALUE, uiValue));

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
  Begin        		:	23.09.2009 / M. Hultsch

  Version       	:	03.01.00 / 23.09.2009 / M. Hultsch

  Function			:	[0xC2][0xB2] SL3 Decrement Encrypted

						NOTE: only for selected tags

  Parameters		:	unsigned int uiSourceBlockNo	- [in] source data block address
						unsigned int uiValue			- [in] decrement value

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::Decrement_Encrypted(
	unsigned int uiSourceBlockNo, 
	unsigned int uiValue )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_CMD, (unsigned char)0xB2));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_SRC_BLOCK_NO, uiSourceBlockNo));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_VALUE, uiValue));

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
  Begin        		:	23.09.2009 / M. Hultsch

  Version       	:	03.01.00 / 23.09.2009 / M. Hultsch

  Function			:	[0xC2][0xB3] SL3 Decrement Encrypted Maced

						NOTE: only for selected tags

  Parameters		:	unsigned int uiSourceBlockNo	- [in] source data block address
						unsigned int uiValue			- [in] decrement value

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::Decrement_EncryptedMaced(
	unsigned int uiSourceBlockNo, 
	unsigned int uiValue )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_CMD, (unsigned char)0xB3));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_SRC_BLOCK_NO, uiSourceBlockNo));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_VALUE, uiValue));

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
  Begin        		:	23.09.2009 / M. Hultsch

  Version       	:	03.01.00 / 23.09.2009 / M. Hultsch

  Function			:	[0xC2][0xB4] SL3 Transfer

						NOTE: only for selected tags

  Parameters		:	unsigned int uiDestinationBlockNo	- [in] destination data block address

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::Transfer(
	unsigned int uiDestinationBlockNo )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_CMD, (unsigned char)0xB4));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_DST_BLOCK_NO, uiDestinationBlockNo));

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
  Begin        		:	23.09.2009 / M. Hultsch

  Version       	:	03.01.00 / 23.09.2009 / M. Hultsch

  Function			:	[0xC2][0xB5] SL3 Transfer Maced

						NOTE: only for selected tags

  Parameters		:	unsigned int uiDestinationBlockNo	- [in] destination data block address

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::Transfer_Maced(
	unsigned int uiDestinationBlockNo )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_CMD, (unsigned char)0xB5));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_DST_BLOCK_NO, uiDestinationBlockNo));

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
  Begin        		:	23.09.2009 / M. Hultsch

  Version       	:	03.01.00 / 23.09.2009 / M. Hultsch

  Function			:	[0xC2][0xB6] SL3 Increment Transfer Encrypted

						NOTE: only for selected tags

  Parameters		:	unsigned int uiSourceBlockNo		- [in] source data block address
						unsigned int uiDestinationBlockNo	- [in] destination data block address
						unsigned int uiValue				- [in] increment value

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::Increment_Transfer_Encrypted(
	unsigned int uiSourceBlockNo,
	unsigned int uiDestinationBlockNo,
	unsigned int uiValue )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_CMD, (unsigned char)0xB6));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_SRC_BLOCK_NO, uiSourceBlockNo));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_DST_BLOCK_NO, uiDestinationBlockNo));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_VALUE, uiValue));

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
  Begin        		:	23.09.2009 / M. Hultsch

  Version       	:	03.01.00 / 23.09.2009 / M. Hultsch

  Function			:	[0xC2][0xB7] SL3 Increment Transfer Encrypted Maced

						NOTE: only for selected tags

  Parameters		:	unsigned int uiSourceBlockNo		- [in] source data block address
						unsigned int uiDestinationBlockNo	- [in] destination data block address
						unsigned int uiValue				- [in] increment value

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::Increment_Transfer_EncryptedMaced(
	unsigned int uiSourceBlockNo,
	unsigned int uiDestinationBlockNo,
	unsigned int uiValue )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_CMD, (unsigned char)0xB7));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_SRC_BLOCK_NO, uiSourceBlockNo));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_DST_BLOCK_NO, uiDestinationBlockNo));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_VALUE, uiValue));

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
  Begin        		:	23.09.2009 / M. Hultsch

  Version       	:	03.01.00 / 23.09.2009 / M. Hultsch

  Function			:	[0xC2][0xB8] SL3 Decrement Transfer Encrypted

						NOTE: only for selected tags

  Parameters		:	unsigned int uiSourceBlockNo		- [in] source data block address
						unsigned int uiDestinationBlockNo	- [in] destination data block address
						unsigned int uiValue				- [in] decrement value

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::Decrement_Transfer_Encrypted(
	unsigned int uiSourceBlockNo,
	unsigned int uiDestinationBlockNo,
	unsigned int uiValue )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_CMD, (unsigned char)0xB8));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_SRC_BLOCK_NO, uiSourceBlockNo));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_DST_BLOCK_NO, uiDestinationBlockNo));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_VALUE, uiValue));

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
  Begin        		:	23.09.2009 / M. Hultsch

  Version       	:	03.01.00 / 23.09.2009 / M. Hultsch

  Function			:	[0xC2][0xB9] SL3 Decrement Transfer Encrypted Maced

						NOTE: only for selected tags

  Parameters		:	unsigned int uiSourceBlockNo		- [in] source data block address
						unsigned int uiDestinationBlockNo	- [in] destination data block address
						unsigned int uiValue				- [in] decrement value

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::Decrement_Transfer_EncryptedMaced(
	unsigned int uiSourceBlockNo,
	unsigned int uiDestinationBlockNo,
	unsigned int uiValue )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_CMD, (unsigned char)0xB9));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_SRC_BLOCK_NO, uiSourceBlockNo));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_DST_BLOCK_NO, uiDestinationBlockNo));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_VALUE, uiValue));

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
  Begin        		:	23.09.2009 / M. Hultsch

  Version       	:	03.01.00 / 24.09.2009 / M. Hultsch

  Function			:	[0xC2][0xC2] SL3 Restore

						NOTE: only for selected tags

  Parameters		:	unsigned int uiSourceBlockNo	- [in] source data block address

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::Restore(
	unsigned int uiSourceBlockNo )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_CMD, (unsigned char)0xC2));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_SRC_BLOCK_NO, uiSourceBlockNo));

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
  Begin        		:	23.09.2009 / M. Hultsch

  Version       	:	03.01.00 / 24.09.2009 / M. Hultsch

  Function			:	[0xC2][0xC3] SL3 Restore Maced

						NOTE: only for selected tags

  Parameters		:	unsigned int uiSourceBlockNo	- [in] source data block address

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::Restore_Maced(
	unsigned int uiSourceBlockNo )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_CMD, (unsigned char)0xC3));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_SRC_BLOCK_NO, uiSourceBlockNo));

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



// ############################################################################
// protected methods

/***************************************************************************
  Begin        		:	24.08.2009 / M. Hultsch

  Version       	:	03.03.00 / 26.01.2011 / M. Hultsch

  Function			:	[0xC2][0x..] Read

						NOTE: only for selected tags

  Parameters		:	unsigned char ucCmdByte			- [in] sub command byte
						unsigned int uiBlockNo			- [in] first data block address
						unsigned char ucLength			- [in] number of blocks to be read
						unsigned char* pucResponseData	- [in] pointer to response data buffer

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::Read(
	unsigned char ucCmdByte,
	unsigned int uiBlockNo, 
	unsigned char ucExt, 
	unsigned char* pucResponseData )
{
	int iErr = 0;
	unsigned int uiBlockNoLsb = 0;

	if(ucExt * 16 > FEDM_ISC_ISO_TABLE_RxDB_SIZE)
		return FEDM_ERROR_ARRAY_BOUNDARY;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_CMD, ucCmdByte));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_BLOCK_NO, uiBlockNo));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_EXT, ucExt));

	// ... and execute
	int iBack = m_pReader->SendProtocol(0xC2);
	if( iBack==0x00 )
	{
		uiBlockNoLsb = (uiBlockNo & 0x000000FF);
		memcpy(pucResponseData, &m_pTabItem->m_ucRxDB[uiBlockNoLsb * 16], ucExt * 16);
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

  Version       	:	03.03.00 / 26.01.2011 / M. Hultsch

  Function			:	[0xC2][0xA?] Write

						NOTE: only for selected tags

  Parameters		:	unsigned char ucCmdByte			- [in] sub command byte
						unsigned int uiBlockNo			- [in] first data block address
						unsigned char ucLength			- [in] number of blocks to be read
						unsigned char* pucRequestData	- [in] pointer to request data buffer

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3::Write(
	unsigned char ucCmdByte,
	unsigned int uiBlockNo, 
	unsigned char ucLength,
	unsigned char* pucRequestData )
{
	int iErr = 0;
	unsigned int uiBlockNoLsb = 0;

	if(ucLength * 16 > FEDM_ISC_ISO_TABLE_TxDB_SIZE)
		return FEDM_ERROR_ARRAY_BOUNDARY;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_CMD, ucCmdByte));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_BLOCK_NO, uiBlockNo));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xC2_EXT, ucLength));

	uiBlockNoLsb = (uiBlockNo & 0x000000FF);

	memcpy(&m_pTabItem->m_ucTxDB[uiBlockNoLsb * 16], pucRequestData, ucLength * 16);

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
