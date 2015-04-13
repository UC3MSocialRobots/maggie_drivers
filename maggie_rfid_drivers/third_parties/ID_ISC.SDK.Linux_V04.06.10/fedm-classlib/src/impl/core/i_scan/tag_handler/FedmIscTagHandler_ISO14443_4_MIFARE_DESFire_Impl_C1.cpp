/*-------------------------------------------------------
|                                                       |
| FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1.cpp  |
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
Begin        		:	30.03.2009

Version       		:	03.02.05 / 07.09.2010 / M. Hultsch

Operation Systems	:	independent

Function			:	implementation class for OBID® classic-pro transponder ISO14443-4 MIFARE DESFire

NOTE				:	this class supports only selected mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


/*############### Questions ##################################################
- CID und NAD für T=CL?

*/

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1.h"
#include "FedmIscTagHandler_ISO14443_4_MIFARE_DESFire.h"
#include "FedmIscTagHandler_ISO14443_4.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1(
	FEDM_ISOTabItem* pTabItem, 
	FedmIscTagHandler_ISO14443_4_MIFARE_DESFire* pParent )
{
	m_pTabItem			= pTabItem;
	m_pParent			= pParent;

	Init();
}

FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::~FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1()
{
}

// set all data members to 0
void FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::Init()
{
	m_ucErrorSource	= 0;
	m_uiErrorCode	= 0;
}


	//####################################################################
	// 5 security related commands
	
	// [0xFA] : [0xC1] DESFire Commands (Authenticate, AuthenticateISO, AuthenticateAES)
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::Authenticate(
	unsigned int uiApplicationID,
	unsigned char ucReaderKeyIndex,
	unsigned char ucDESFireKeyNo )
{
	int iErr = 0;

	// swap MSB with LSB
	unsigned int uiTmp = 0;
	uiTmp |= ((uiApplicationID & 0x000000FF) << 16);
	uiTmp |= ((uiApplicationID & 0x0000FF00) );
	uiTmp |= ((uiApplicationID & 0x00FF0000) >> 16);


	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_CMD, (unsigned char)0xFA));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_APP_ID, uiTmp));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_KEY_INDEX, ucReaderKeyIndex));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_KEY_NO, ucDESFireKeyNo));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC1);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC1_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC1_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0x54] : [0xC1] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::ChangeKeySettings(
	unsigned int uiApplicationID,
	unsigned char ucReaderKeyIndex,
	unsigned char ucNewKeySettings )
{
	int iErr = 0;

	// swap MSB with LSB
	unsigned int uiTmp = 0;
	uiTmp |= ((uiApplicationID & 0x000000FF) << 16);
	uiTmp |= ((uiApplicationID & 0x0000FF00) );
	uiTmp |= ((uiApplicationID & 0x00FF0000) >> 16);

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_CMD, (unsigned char)0x54));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_APP_ID, uiTmp));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_KEY_INDEX, ucReaderKeyIndex));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_NEW_KEY_SETTINGS, ucNewKeySettings));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC1);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC1_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC1_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0x45] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::GetKeySettings(
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned int uiCnt = 0;

	m_pParent->m_ucRequestData[uiCnt++] = 0x45;

	return m_pParent->Apdu(m_pParent->m_ucRequestData, uiCnt, pucResponseData, uiResponseBufferLength, uiResponseLength);
}

	// [0xC4] : [0xC1] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::ChangeKey(
	unsigned int uiApplicationID,
	unsigned char ucReaderKeyIndex,
	unsigned char ucDESFireKeyNo,
	unsigned char ucKeyNoToBeChanged,
	unsigned char ucAESNewKeyVersion,
	string sOldKey,
	string sNewKey )
{
	int iErr = 0;
	size_t iPadd = 0;

	if(sOldKey.length() != sNewKey.length())
		return FEDM_ERROR_STRING_LENGTH;

	if(sOldKey.length() != 16 && sOldKey.length() != 32 && sOldKey.length() != 48)
		return FEDM_ERROR_STRING_LENGTH;

	if(sNewKey.length() != 16 && sNewKey.length() != 32 && sNewKey.length() != 48)
		return FEDM_ERROR_STRING_LENGTH;

	// swap MSB with LSB
	unsigned int uiTmp = 0;
	uiTmp |= ((uiApplicationID & 0x000000FF) << 16);
	uiTmp |= ((uiApplicationID & 0x0000FF00) );
	uiTmp |= ((uiApplicationID & 0x00FF0000) >> 16);

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_CMD, (unsigned char)0xC4));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_APP_ID, uiTmp));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_KEY_INDEX, ucReaderKeyIndex));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_KEY_NO, ucDESFireKeyNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_KEY_NO_TB_CHANGED, ucKeyNoToBeChanged));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_KEY_LEN, (unsigned char)(sNewKey.length()>>1))); // :2
	//Padding with '00' to 24 Byte KeyLength
	for(iPadd = sOldKey.length(); iPadd < 48; iPadd++)
	{
		sOldKey += "0";
	}
	for(iPadd = sNewKey.length(); iPadd < 48; iPadd++)
	{
		sNewKey += "0";
	}
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_OLD_KEY, sOldKey));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_NEW_KEY, sNewKey));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_NEW_KEY_VERSION, ucAESNewKeyVersion));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC1);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC1_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC1_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0x64] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::GetKeyVersion(
	unsigned char ucDESFireKeyNo,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned int uiCnt = 0;

	m_pParent->m_ucRequestData[uiCnt++] = 0x64;
	m_pParent->m_ucRequestData[uiCnt++] = ucDESFireKeyNo;

	return m_pParent->Apdu(m_pParent->m_ucRequestData, uiCnt, pucResponseData, uiResponseBufferLength, uiResponseLength);
}


	//####################################################################
	// 10 PICC level commands

	// [0xCA] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::CreateApplication(
	unsigned int uiApplicationID,
	unsigned char ucKeySetting1,
	unsigned char ucKeySetting2,
	unsigned int uiISOFileID,
	string sDFName,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned int uiCnt = 0;

	m_pParent->m_ucRequestData[uiCnt++] = 0xCA;
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char) (uiApplicationID & 0x000000FF);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiApplicationID & 0x0000FF00) >>  8);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiApplicationID & 0x00FF0000) >> 16);
	m_pParent->m_ucRequestData[uiCnt++] = ucKeySetting1;
	m_pParent->m_ucRequestData[uiCnt++] = ucKeySetting2;
	if(ucKeySetting2 & 0x40)
	{
		m_pParent->m_ucRequestData[uiCnt++] = (unsigned char) (uiISOFileID & 0x000000FF);
		m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiISOFileID & 0x0000FF00) >> 8);
	}
	strncpy((char*)&m_pParent->m_ucRequestData[uiCnt], sDFName.c_str(), sDFName.length());
	uiCnt += (unsigned int)sDFName.length();

	return m_pParent->Apdu(m_pParent->m_ucRequestData, uiCnt, pucResponseData, uiResponseBufferLength, uiResponseLength);
}

	// [0xDA] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::DeleteApplication(
	unsigned int uiApplicationID,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned int uiCnt = 0;

	m_pParent->m_ucRequestData[uiCnt++] = 0xDA;
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char) (uiApplicationID & 0x000000FF);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiApplicationID & 0x0000FF00) >>  8);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiApplicationID & 0x00FF0000) >> 16);

	return m_pParent->Apdu(m_pParent->m_ucRequestData, uiCnt, pucResponseData, uiResponseBufferLength, uiResponseLength);
}

	// [0x6A] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::GetApplicationIDs(
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	int iBack = 0;
	unsigned int uiRspLen = 0;
	unsigned int uiCnt = 0;

	uiResponseLength = 1; // increment for status byte
	
	m_pParent->m_ucRequestData[uiCnt++] = 0x6A;

	iBack = m_pParent->Apdu(m_pParent->m_ucRequestData, uiCnt, m_pParent->m_ucResponseData, FEDM_ISO14443_4_APDU_BUFFER_SIZE, uiRspLen);
	while( iBack == 0 && uiRspLen > 0 && m_pParent->m_ucResponseData[0] == 0xAF)
	{
		if((uiResponseLength + uiRspLen - 1) > uiResponseBufferLength)
			return FEDM_ERROR_BUFFER_LENGTH;

		// copy receive data into application buffer (without status 0xAF)
		memcpy(&pucResponseData[uiResponseLength], &m_pParent->m_ucResponseData[1], uiRspLen - 1);

		uiCnt = 0;
		uiResponseLength += uiRspLen - 1;
		m_pParent->m_ucRequestData[uiCnt++] = 0xAF;
		iBack = m_pParent->Apdu(m_pParent->m_ucRequestData, uiCnt, m_pParent->m_ucResponseData, FEDM_ISO14443_4_APDU_BUFFER_SIZE, uiRspLen);
	}

	if(iBack == 0 && uiRspLen > 0)
	{
		if((uiResponseLength + uiRspLen - 1) > uiResponseBufferLength)
			return FEDM_ERROR_BUFFER_LENGTH;

		pucResponseData[0] = m_pParent->m_ucResponseData[0]; // status
		memcpy(&pucResponseData[uiResponseLength], &m_pParent->m_ucResponseData[1], uiRspLen - 1);
		uiResponseLength += uiRspLen - 1;
	}

	return iBack;
}

	// [0x6E] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::FreeMemory(
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned int uiCnt = 0;

	m_pParent->m_ucRequestData[uiCnt++] = 0x6E;

	return m_pParent->Apdu(m_pParent->m_ucRequestData, uiCnt, pucResponseData, uiResponseBufferLength, uiResponseLength);
}

	// [0x6D] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::GetDFNames(
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	int iBack = 0;
	unsigned int uiRspLen = 0;
	unsigned int uiCnt = 0;

	uiResponseLength = 1; // increment for status byte
	
	m_pParent->m_ucRequestData[uiCnt++] = 0x6D;

	iBack = m_pParent->Apdu(m_pParent->m_ucRequestData, uiCnt, m_pParent->m_ucResponseData, FEDM_ISO14443_4_APDU_BUFFER_SIZE, uiRspLen);
	while( iBack == 0 && uiRspLen > 0 && m_pParent->m_ucResponseData[0] == 0xAF)
	{
		if((uiResponseLength + uiRspLen - 1) > uiResponseBufferLength)
			return FEDM_ERROR_BUFFER_LENGTH;

		// copy receive data into application buffer (without status 0xAF)
		memcpy(&pucResponseData[uiResponseLength], &m_pParent->m_ucResponseData[1], uiRspLen - 1);

		uiCnt = 0;
		uiResponseLength += uiRspLen - 1;
		m_pParent->m_ucRequestData[uiCnt++] = 0xAF;
		iBack = m_pParent->Apdu(m_pParent->m_ucRequestData, uiCnt, m_pParent->m_ucResponseData, FEDM_ISO14443_4_APDU_BUFFER_SIZE, uiRspLen);
	}

	if(iBack == 0 && uiRspLen > 0)
	{
		if((uiResponseLength + uiRspLen - 1) > uiResponseBufferLength)
			return FEDM_ERROR_BUFFER_LENGTH;

		pucResponseData[0] = m_pParent->m_ucResponseData[0]; // status
		memcpy(&pucResponseData[uiResponseLength], &m_pParent->m_ucResponseData[1], uiRspLen - 1);
		uiResponseLength += uiRspLen - 1;
	}

	return iBack;
}

	// [0x5A] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::SelectApplication(
	unsigned int uiApplicationID,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned int uiCnt = 0;

	m_pParent->m_ucRequestData[uiCnt++] = 0x5A;
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char) (uiApplicationID & 0x000000FF);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiApplicationID & 0x0000FF00) >>  8);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiApplicationID & 0x00FF0000) >> 16);

	return m_pParent->Apdu(m_pParent->m_ucRequestData, uiCnt, pucResponseData, uiResponseBufferLength, uiResponseLength);
}

	// [0xFC] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::FormatPICC(
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned int uiCnt = 0;

	m_pParent->m_ucRequestData[uiCnt++] = 0xFC;

	return m_pParent->Apdu(m_pParent->m_ucRequestData, uiCnt, pucResponseData, uiResponseBufferLength, uiResponseLength);
}

	// [0x60] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::GetVersion(
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	int iBack = 0;
	unsigned int uiRspLen = 0;
	unsigned int uiCnt = 0;

	uiResponseLength = 1; // increment for status byte
	
	m_pParent->m_ucRequestData[uiCnt++] = 0x60;

	iBack = m_pParent->Apdu(m_pParent->m_ucRequestData, uiCnt, m_pParent->m_ucResponseData, FEDM_ISO14443_4_APDU_BUFFER_SIZE, uiRspLen);
	while( iBack == 0 && uiRspLen > 0 && m_pParent->m_ucResponseData[0] == 0xAF)
	{
		if((uiResponseLength + uiRspLen - 1) > uiResponseBufferLength)
			return FEDM_ERROR_BUFFER_LENGTH;

		// copy receive data into application buffer (without status 0xAF)
		memcpy(&pucResponseData[uiResponseLength], &m_pParent->m_ucResponseData[1], uiRspLen - 1);

		uiCnt = 0;
		uiResponseLength += uiRspLen - 1;
		m_pParent->m_ucRequestData[uiCnt++] = 0xAF;
		iBack = m_pParent->Apdu(m_pParent->m_ucRequestData, uiCnt, m_pParent->m_ucResponseData, FEDM_ISO14443_4_APDU_BUFFER_SIZE, uiRspLen);
	}

	if(iBack == 0 && uiRspLen > 0)
	{
		if((uiResponseLength + uiRspLen - 1) > uiResponseBufferLength)
			return FEDM_ERROR_BUFFER_LENGTH;

		pucResponseData[0] = 0x00; // status
		memcpy(&pucResponseData[uiResponseLength], &m_pParent->m_ucResponseData[1], uiRspLen - 1);
		uiResponseLength += uiRspLen - 1;
	}

	return iBack;
}

	// [0x51] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::GetCardUID(
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned int uiCnt = 0;

	m_pParent->m_ucRequestData[uiCnt++] = 0x51;

	return m_pParent->Apdu(m_pParent->m_ucRequestData, uiCnt, pucResponseData, uiResponseBufferLength, uiResponseLength);
}

	// [0x5C] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::SetConfiguration(
	unsigned char* pucRequestData,
	unsigned int uiRequestLength,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned int uiCnt = 0;

	if(uiRequestLength > FEDM_ISO14443_4_APDU_BUFFER_SIZE - 1)
		return FEDM_ERROR_BUFFER_LENGTH;

	m_pParent->m_ucRequestData[uiCnt++] = 0x64;
	if(pucRequestData != NULL)
	{
		memcpy(&m_pParent->m_ucRequestData[uiCnt], pucRequestData, uiRequestLength);
		uiCnt += uiRequestLength;
	}

	m_pParent->Apdu(m_pParent->m_ucRequestData, uiCnt, pucResponseData, uiResponseBufferLength, uiResponseLength);

	return FEDM_OK;
}


	//####################################################################
	// 10 application level commands

	// [0x5F] : [0xC1] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::ChangeFileSettings(
	unsigned int uiApplicationID,
	unsigned char ucReaderKeyIndex,
	unsigned char ucDESFireKeyNo,
	unsigned char ucFileNo,
	unsigned char ucNewFileCommSettings,
	unsigned char ucNewFileReadWriteAccessRights,
	unsigned char ucNewFileChangeAccessRights,
	unsigned char ucNewFileReadAccessRights,
	unsigned char ucNewFileWriteAccessRights )
{
	int iErr = 0;

	// swap MSB with LSB
	unsigned int uiTmp = 0;
	uiTmp |= ((uiApplicationID & 0x000000FF) << 16);
	uiTmp |= ((uiApplicationID & 0x0000FF00) );
	uiTmp |= ((uiApplicationID & 0x00FF0000) >> 16);

	unsigned char ucAccessRights[2];
	ucAccessRights[0] = ((ucNewFileReadWriteAccessRights & 0x0F) << 4) + (ucNewFileChangeAccessRights & 0x0F);
	ucAccessRights[1] = ((ucNewFileReadAccessRights & 0x0F) << 4) + (ucNewFileWriteAccessRights & 0x0F);

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_CMD, (unsigned char)0x5F));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_APP_ID, uiTmp));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_KEY_INDEX, ucReaderKeyIndex));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_KEY_NO, ucDESFireKeyNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_FILE_NO, ucFileNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_FILE_COMM_SETTINGS, ucNewFileCommSettings));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_FILE_ACCESS_RIGHTS, ucAccessRights, 2));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC1);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC1_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC1_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0x6F] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::GetFileIDs(
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned int uiCnt = 0;

	m_pParent->m_ucRequestData[uiCnt++] = 0x6F;

	return m_pParent->Apdu(m_pParent->m_ucRequestData, uiCnt, pucResponseData, uiResponseBufferLength, uiResponseLength);
}

	// [0xF5] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::GetFileSettings(
	unsigned char ucFileNo,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned int uiCnt = 0;

	m_pParent->m_ucRequestData[uiCnt++] = 0xF5;
	m_pParent->m_ucRequestData[uiCnt++] = ucFileNo;

	return m_pParent->Apdu(m_pParent->m_ucRequestData, uiCnt, pucResponseData, uiResponseBufferLength, uiResponseLength);
}

	// [0xCD] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::CreateStdDataFile(
	unsigned char ucFileNo,
	bool bWithISOFileID,
	unsigned int uiISOFileID,
	unsigned char ucFileCommSettings,
	unsigned char ucFileReadWriteAccessRights,
	unsigned char ucFileChangeAccessRights,
	unsigned char ucFileReadAccessRights,
	unsigned char ucFileWriteAccessRights,
	unsigned int uiFileSize,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned int uiCnt = 0;

	m_pParent->m_ucRequestData[uiCnt++] = 0xCD;
	m_pParent->m_ucRequestData[uiCnt++] = ucFileNo;
	if(bWithISOFileID)
	{
		m_pParent->m_ucRequestData[uiCnt++] = (unsigned char) (uiISOFileID & 0x000000FF);
		m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiISOFileID & 0x0000FF00) >> 8);
	}
	m_pParent->m_ucRequestData[uiCnt++] = ucFileCommSettings;
	m_pParent->m_ucRequestData[uiCnt++] = ((ucFileReadWriteAccessRights & 0x0F) << 4) + (ucFileChangeAccessRights & 0x0F);
	m_pParent->m_ucRequestData[uiCnt++] = ((ucFileReadAccessRights & 0x0F) << 4) + (ucFileWriteAccessRights & 0x0F);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char) (uiFileSize & 0x000000FF);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiFileSize & 0x0000FF00) >>  8);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiFileSize & 0x00FF0000) >> 16);

	return m_pParent->Apdu(m_pParent->m_ucRequestData, uiCnt, pucResponseData, uiResponseBufferLength, uiResponseLength);
}

	// [0xCB] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::CreateBackupDataFile(
	unsigned char ucFileNo,
	bool bWithISOFileID,
	unsigned int uiISOFileID,
	unsigned char ucFileCommSettings,
	unsigned char ucFileReadWriteAccessRights,
	unsigned char ucFileChangeAccessRights,
	unsigned char ucFileReadAccessRights,
	unsigned char ucFileWriteAccessRights,
	unsigned int uiFileSize,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned int uiCnt = 0;

	m_pParent->m_ucRequestData[uiCnt++] = 0xCB;
	m_pParent->m_ucRequestData[uiCnt++] = ucFileNo;
	if(bWithISOFileID)
	{
		m_pParent->m_ucRequestData[uiCnt++] = (unsigned char) (uiISOFileID & 0x000000FF);
		m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiISOFileID & 0x0000FF00) >> 8);
	}
	m_pParent->m_ucRequestData[uiCnt++] = ucFileCommSettings;
	m_pParent->m_ucRequestData[uiCnt++] = ((ucFileReadWriteAccessRights & 0x0F) << 4) + (ucFileChangeAccessRights & 0x0F);
	m_pParent->m_ucRequestData[uiCnt++] = ((ucFileReadAccessRights & 0x0F) << 4) + (ucFileWriteAccessRights & 0x0F);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char) (uiFileSize & 0x000000FF);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiFileSize & 0x0000FF00) >>  8);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiFileSize & 0x00FF0000) >> 16);

	return m_pParent->Apdu(m_pParent->m_ucRequestData, uiCnt, pucResponseData, uiResponseBufferLength, uiResponseLength);
}

	// [0xCC] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::CreateValueFile(
	unsigned char ucFileNo,
	unsigned char ucFileCommSettings,
	unsigned char ucFileReadWriteAccessRights,
	unsigned char ucFileChangeAccessRights,
	unsigned char ucFileReadAccessRights,
	unsigned char ucFileWriteAccessRights,
	unsigned int uiLowerLimit,
	unsigned int uiUpperLimit,
	unsigned int uiValue,
	unsigned char ucLimitedCreditEnabled,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned int uiCnt = 0;

	m_pParent->m_ucRequestData[uiCnt++] = 0xCC;
	m_pParent->m_ucRequestData[uiCnt++] = ucFileNo;
	m_pParent->m_ucRequestData[uiCnt++] = ucFileCommSettings;
	m_pParent->m_ucRequestData[uiCnt++] = ((ucFileReadWriteAccessRights & 0x0F) << 4) + (ucFileChangeAccessRights & 0x0F);
	m_pParent->m_ucRequestData[uiCnt++] = ((ucFileReadAccessRights & 0x0F) << 4) + (ucFileWriteAccessRights & 0x0F);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char) (uiLowerLimit & 0x000000FF);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiLowerLimit & 0x0000FF00) >>  8);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiLowerLimit & 0x00FF0000) >> 16);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiLowerLimit & 0xFF000000) >> 24);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char) (uiUpperLimit & 0x000000FF);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiUpperLimit & 0x0000FF00) >>  8);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiUpperLimit & 0x00FF0000) >> 16);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiUpperLimit & 0xFF000000) >> 24);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char) (uiValue & 0x000000FF);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiValue & 0x0000FF00) >>  8);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiValue & 0x00FF0000) >> 16);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiValue & 0xFF000000) >> 24);
	m_pParent->m_ucRequestData[uiCnt++] = ucLimitedCreditEnabled;

	return m_pParent->Apdu(m_pParent->m_ucRequestData, uiCnt, pucResponseData, uiResponseBufferLength, uiResponseLength);
}

	// [0xC1] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::CreateLinearRecordFile(
	unsigned char ucFileNo,
	bool bWithISOFileID,
	unsigned int uiISOFileID,
	unsigned char ucFileCommSettings,
	unsigned char ucFileReadWriteAccessRights,
	unsigned char ucFileChangeAccessRights,
	unsigned char ucFileReadAccessRights,
	unsigned char ucFileWriteAccessRights,
	unsigned int uiRecordSize,
	unsigned int uiMaxNoOfRecords,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned int uiCnt = 0;

	m_pParent->m_ucRequestData[uiCnt++] = 0xC1;
	m_pParent->m_ucRequestData[uiCnt++] = ucFileNo;
	if(bWithISOFileID)
	{
		m_pParent->m_ucRequestData[uiCnt++] = (unsigned char) (uiISOFileID & 0x000000FF);
		m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiISOFileID & 0x0000FF00) >> 8);
	}
	m_pParent->m_ucRequestData[uiCnt++] = ucFileCommSettings;
	m_pParent->m_ucRequestData[uiCnt++] = ((ucFileReadWriteAccessRights & 0x0F) << 4) + (ucFileChangeAccessRights & 0x0F);
	m_pParent->m_ucRequestData[uiCnt++] = ((ucFileReadAccessRights & 0x0F) << 4) + (ucFileWriteAccessRights & 0x0F);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char) (uiRecordSize & 0x000000FF);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiRecordSize & 0x0000FF00) >>  8);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiRecordSize & 0x00FF0000) >> 16);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char) (uiMaxNoOfRecords & 0x000000FF);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiMaxNoOfRecords & 0x0000FF00) >>  8);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiMaxNoOfRecords & 0x00FF0000) >> 16);

	return m_pParent->Apdu(m_pParent->m_ucRequestData, uiCnt, pucResponseData, uiResponseBufferLength, uiResponseLength);
}

	// [0xC0] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::CreateCyclicRecordFile(
	unsigned char ucFileNo,
	bool bWithISOFileID,
	unsigned int uiISOFileID,
	unsigned char ucFileCommSettings,
	unsigned char ucFileReadWriteAccessRights,
	unsigned char ucFileChangeAccessRights,
	unsigned char ucFileReadAccessRights,
	unsigned char ucFileWriteAccessRights,
	unsigned int uiRecordSize,
	unsigned int uiMaxNoOfRecords,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned int uiCnt = 0;

	m_pParent->m_ucRequestData[uiCnt++] = 0xC0;
	m_pParent->m_ucRequestData[uiCnt++] = ucFileNo;
	if(bWithISOFileID)
	{
		m_pParent->m_ucRequestData[uiCnt++] = (unsigned char) (uiISOFileID & 0x000000FF);
		m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiISOFileID & 0x0000FF00) >> 8);
	}
	m_pParent->m_ucRequestData[uiCnt++] = ucFileCommSettings;
	m_pParent->m_ucRequestData[uiCnt++] = ((ucFileReadWriteAccessRights & 0x0F) << 4) + (ucFileChangeAccessRights & 0x0F);
	m_pParent->m_ucRequestData[uiCnt++] = ((ucFileReadAccessRights & 0x0F) << 4) + (ucFileWriteAccessRights & 0x0F);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char) (uiRecordSize & 0x000000FF);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiRecordSize & 0x0000FF00) >>  8);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiRecordSize & 0x00FF0000) >> 16);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char) (uiMaxNoOfRecords & 0x000000FF);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiMaxNoOfRecords & 0x0000FF00) >>  8);
	m_pParent->m_ucRequestData[uiCnt++] = (unsigned char)((uiMaxNoOfRecords & 0x00FF0000) >> 16);

	return m_pParent->Apdu(m_pParent->m_ucRequestData, uiCnt, pucResponseData, uiResponseBufferLength, uiResponseLength);
}

	// [0xDF] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::DeleteFile(
	unsigned char ucFileNo,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned int uiCnt = 0;

	m_pParent->m_ucRequestData[uiCnt++] = 0xDF;
	m_pParent->m_ucRequestData[uiCnt++] = ucFileNo;

	return m_pParent->Apdu(m_pParent->m_ucRequestData, uiCnt, pucResponseData, uiResponseBufferLength, uiResponseLength);
}

	// [0x61] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::GetISOFileIDs(
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	int iBack = 0;
	unsigned int uiRspLen = 0;
	unsigned int uiCnt = 0;

	uiResponseLength = 1; // increment for status byte
	
	m_pParent->m_ucRequestData[uiCnt++] = 0x61;

	iBack = m_pParent->Apdu(m_pParent->m_ucRequestData, uiCnt, m_pParent->m_ucResponseData, FEDM_ISO14443_4_APDU_BUFFER_SIZE, uiRspLen);
	while( iBack == 0 && uiRspLen > 0 && m_pParent->m_ucResponseData[0] == 0xAF)
	{
		if((uiResponseLength + uiRspLen - 1) > uiResponseBufferLength)
			return FEDM_ERROR_BUFFER_LENGTH;

		// copy receive data into application buffer (without status 0xAF)
		memcpy(&pucResponseData[uiResponseLength], &m_pParent->m_ucResponseData[1], uiRspLen - 1);

		uiCnt = 0;
		uiResponseLength += uiRspLen - 1;
		m_pParent->m_ucRequestData[uiCnt++] = 0xAF;
		iBack = m_pParent->Apdu(m_pParent->m_ucRequestData, uiCnt, m_pParent->m_ucResponseData, FEDM_ISO14443_4_APDU_BUFFER_SIZE, uiRspLen);
	}

	if(iBack == 0 && uiRspLen > 0)
	{
		if((uiResponseLength + uiRspLen - 1) > uiResponseBufferLength)
			return FEDM_ERROR_BUFFER_LENGTH;

		pucResponseData[0] = m_pParent->m_ucResponseData[0]; // status
		memcpy(&pucResponseData[uiResponseLength], &m_pParent->m_ucResponseData[1], uiRspLen - 1);
		uiResponseLength += uiRspLen - 1;
	}

	return iBack;
}


	//####################################################################
	// 11 data manipulation commands
	
	// [0xBD] : [0xC1] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::ReadStandardData(
	unsigned int uiApplicationID,
	unsigned char ucReaderKeyIndex,
	unsigned char ucDESFireKeyNo,
	unsigned char ucFileNo,
	unsigned char ucFileCommSettings,
	unsigned int uiFileOffset,
	unsigned int uiFileLen,
	unsigned char* pucFileData )
{
	FEDM_CHK3(pucFileData);

	int iErr = 0;

	// swap MSB with LSB
	unsigned int uiTmp = 0;
	uiTmp |= ((uiApplicationID & 0x000000FF) << 16);
	uiTmp |= ((uiApplicationID & 0x0000FF00) );
	uiTmp |= ((uiApplicationID & 0x00FF0000) >> 16);

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_CMD, (unsigned char)0xBD));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_APP_ID, uiTmp));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_KEY_INDEX, ucReaderKeyIndex));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_KEY_NO, ucDESFireKeyNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_FILE_NO, ucFileNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_FILE_COMM_SETTINGS, ucFileCommSettings));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_FILE_OFFSET, uiFileOffset));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_FILE_DATA_LEN, uiFileLen));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC1);
	if( iBack==0x00 )
	{
		memcpy(pucFileData, &m_pTabItem->m_ucRxDB[uiFileOffset], uiFileLen);
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC1_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC1_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}
	
	// [0x3D] : [0xC1] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::WriteStandardData(
	unsigned int uiApplicationID,
	unsigned char ucReaderKeyIndex,
	unsigned char ucDESFireKeyNo,
	unsigned char ucFileNo,
	unsigned char ucFileCommSettings,
	unsigned int uiFileOffset,
	unsigned int uiFileLen,
	unsigned char* pucFileData )
{
	FEDM_CHK3(pucFileData);

	int iErr = 0;

	if(uiFileOffset + uiFileLen > FEDM_ISC_ISO_TABLE_RxDB_SIZE)
		return FEDM_ERROR_ARRAY_BOUNDARY;

	// swap MSB with LSB
	unsigned int uiTmp = 0;
	uiTmp |= ((uiApplicationID & 0x000000FF) << 16);
	uiTmp |= ((uiApplicationID & 0x0000FF00) );
	uiTmp |= ((uiApplicationID & 0x00FF0000) >> 16);

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_CMD, (unsigned char)0x3D));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_APP_ID, uiTmp));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_KEY_INDEX, ucReaderKeyIndex));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_KEY_NO, ucDESFireKeyNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_FILE_NO, ucFileNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_FILE_COMM_SETTINGS, ucFileCommSettings));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_FILE_OFFSET, uiFileOffset));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_FILE_DATA_LEN, uiFileLen));

	memcpy(&m_pTabItem->m_ucTxDB[uiFileOffset], pucFileData, uiFileLen);

	int iBack = m_pParent->m_pReader->SendProtocol(0xC1);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC1_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC1_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0x6C] : [0xC1] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::GetValue(
	unsigned int uiApplicationID,
	unsigned char ucReaderKeyIndex,
	unsigned char ucDESFireKeyNo,
	unsigned char ucFileNo,
	unsigned char ucFileCommSettings,
	unsigned int& uiValue )
{
	int iErr = 0;

	// swap MSB with LSB
	unsigned int uiTmp = 0;
	uiTmp |= ((uiApplicationID & 0x000000FF) << 16);
	uiTmp |= ((uiApplicationID & 0x0000FF00) );
	uiTmp |= ((uiApplicationID & 0x00FF0000) >> 16);

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_CMD, (unsigned char)0x6C));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_APP_ID, uiTmp));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_KEY_INDEX, ucReaderKeyIndex));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_KEY_NO, ucDESFireKeyNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_FILE_NO, ucFileNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_FILE_COMM_SETTINGS, ucFileCommSettings));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC1);
	if( iBack==0x00 )
	{
		uiValue = m_pTabItem->m_uiValue;
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC1_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC1_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0x0C] : [0xC1] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::Credit(
	unsigned int uiApplicationID,
	unsigned char ucReaderKeyIndex,
	unsigned char ucDESFireKeyNo,
	unsigned char ucFileNo,
	unsigned char ucFileCommSettings,
	unsigned int uiCreditValue )
{
	int iErr = 0;

	// swap MSB with LSB
	unsigned int uiTmp = 0;
	uiTmp |= ((uiApplicationID & 0x000000FF) << 16);
	uiTmp |= ((uiApplicationID & 0x0000FF00) );
	uiTmp |= ((uiApplicationID & 0x00FF0000) >> 16);

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_CMD, (unsigned char)0x0C));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_APP_ID, uiTmp));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_KEY_INDEX, ucReaderKeyIndex));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_KEY_NO, ucDESFireKeyNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_FILE_NO, ucFileNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_FILE_COMM_SETTINGS, ucFileCommSettings));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_CREDIT_VALUE, uiCreditValue));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC1);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC1_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC1_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0xDC] : [0xC1] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::Debit(
	unsigned int uiApplicationID,
	unsigned char ucReaderKeyIndex,
	unsigned char ucDESFireKeyNo,
	unsigned char ucFileNo,
	unsigned char ucFileCommSettings,
	unsigned int uiDebitValue )
{
	int iErr = 0;

	// swap MSB with LSB
	unsigned int uiTmp = 0;
	uiTmp |= ((uiApplicationID & 0x000000FF) << 16);
	uiTmp |= ((uiApplicationID & 0x0000FF00) );
	uiTmp |= ((uiApplicationID & 0x00FF0000) >> 16);

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_CMD, (unsigned char)0xDC));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_APP_ID, uiTmp));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_KEY_INDEX, ucReaderKeyIndex));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_KEY_NO, ucDESFireKeyNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_FILE_NO, ucFileNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_FILE_COMM_SETTINGS, ucFileCommSettings));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_CREDIT_VALUE, uiDebitValue));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC1);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC1_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC1_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}


	// [0x1C] : [0xC1] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::LimitedCredit(
	unsigned int uiApplicationID,
	unsigned char ucReaderKeyIndex,
	unsigned char ucDESFireKeyNo,
	unsigned char ucFileNo,
	unsigned char ucFileCommSettings,
	unsigned int uiCreditValue )
{
	int iErr = 0;

	// swap MSB with LSB
	unsigned int uiTmp = 0;
	uiTmp |= ((uiApplicationID & 0x000000FF) << 16);
	uiTmp |= ((uiApplicationID & 0x0000FF00) );
	uiTmp |= ((uiApplicationID & 0x00FF0000) >> 16);

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_CMD, (unsigned char)0x1C));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_APP_ID, uiTmp));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_KEY_INDEX, ucReaderKeyIndex));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_KEY_NO, ucDESFireKeyNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_FILE_NO, ucFileNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_FILE_COMM_SETTINGS, ucFileCommSettings));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_CREDIT_VALUE, uiCreditValue));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC1);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC1_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC1_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0x3B] : [0xC1] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::WriteRecord(
	unsigned int uiApplicationID,
	unsigned char ucReaderKeyIndex,
	unsigned char ucDESFireKeyNo,
	unsigned char ucFileNo,
	unsigned char ucFileCommSettings,
	unsigned int uiFileOffset,
	unsigned int uiRecordLen,
	unsigned char* pucRecordData )
{
	FEDM_CHK3(pucRecordData);

	int iErr = 0;

	if(uiFileOffset + uiRecordLen > FEDM_ISC_ISO_TABLE_TxDB_SIZE)
		return FEDM_ERROR_ARRAY_BOUNDARY;

	// swap MSB with LSB
	unsigned int uiTmp = 0;
	uiTmp |= ((uiApplicationID & 0x000000FF) << 16);
	uiTmp |= ((uiApplicationID & 0x0000FF00) );
	uiTmp |= ((uiApplicationID & 0x00FF0000) >> 16);

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_CMD, (unsigned char)0x3B));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_APP_ID, uiTmp));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_KEY_INDEX, ucReaderKeyIndex));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_KEY_NO, ucDESFireKeyNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_FILE_NO, ucFileNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_FILE_COMM_SETTINGS, ucFileCommSettings));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_FILE_OFFSET, uiFileOffset));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_FILE_DATA_LEN, uiRecordLen));

	memcpy(&m_pTabItem->m_ucTxDB[uiFileOffset], pucRecordData, uiRecordLen);

	int iBack = m_pParent->m_pReader->SendProtocol(0xC1);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC1_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC1_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0xBB] : [0xC1] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::ReadRecords(
	unsigned int uiApplicationID,
	unsigned char ucReaderKeyIndex,
	unsigned char ucDESFireKeyNo,
	unsigned char ucFileNo,
	unsigned char ucFileCommSettings,
	unsigned int uiRecordOffset,
	unsigned int uiNoOfRecords,
	unsigned int uiRecordSize,
	unsigned char* pucRecordData )
{
	FEDM_CHK3(pucRecordData);

	int iErr = 0;

	if(uiNoOfRecords * uiRecordSize > FEDM_ISC_ISO_TABLE_RxDB_SIZE)
		return FEDM_ERROR_ARRAY_BOUNDARY;

	// swap MSB with LSB
	unsigned int uiTmp = 0;
	uiTmp |= ((uiApplicationID & 0x000000FF) << 16);
	uiTmp |= ((uiApplicationID & 0x0000FF00) );
	uiTmp |= ((uiApplicationID & 0x00FF0000) >> 16);

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_CMD, (unsigned char)0xBB));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_APP_ID, uiTmp));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_KEY_INDEX, ucReaderKeyIndex));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_KEY_NO, ucDESFireKeyNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_FILE_NO, ucFileNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_FILE_COMM_SETTINGS, ucFileCommSettings));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_RECORD_OFFSET, uiRecordOffset));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_NO_OF_RECORDS, uiNoOfRecords));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_RECORD_SIZE, uiRecordSize));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC1);
	if( iBack==0x00 )
	{
		memcpy(pucRecordData, &m_pTabItem->m_ucRxDB[0], uiNoOfRecords*uiRecordSize);
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC1_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC1_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0xEB] : [0xC1] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::ClearRecordFile(
	unsigned int uiApplicationID,
	unsigned char ucReaderKeyIndex,
	unsigned char ucDESFireKeyNo,
	unsigned char ucFileNo,
	unsigned char ucFileCommSettings )
{
	int iErr = 0;

	// swap MSB with LSB
	unsigned int uiTmp = 0;
	uiTmp |= ((uiApplicationID & 0x000000FF) << 16);
	uiTmp |= ((uiApplicationID & 0x0000FF00) );
	uiTmp |= ((uiApplicationID & 0x00FF0000) >> 16);

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_CMD, (unsigned char)0xEB));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_MODE, (unsigned char)0x00));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_APP_ID, uiTmp));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_KEY_INDEX, ucReaderKeyIndex));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_KEY_NO, ucDESFireKeyNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_FILE_NO, ucFileNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC1_FILE_COMM_SETTINGS, ucFileCommSettings));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC1);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC1_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC1_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0xC7] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::CommitTransaction(
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned int uiCnt = 0;

	m_pParent->m_ucRequestData[uiCnt++] = 0xC7;

	return m_pParent->Apdu(m_pParent->m_ucRequestData, uiCnt, pucResponseData, uiResponseBufferLength, uiResponseLength);
}

	// [0xA7] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1::AbortTransaction(
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned int uiCnt = 0;

	m_pParent->m_ucRequestData[uiCnt++] = 0xA7;

	return m_pParent->Apdu(m_pParent->m_ucRequestData, uiCnt, pucResponseData, uiResponseBufferLength, uiResponseLength);
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
