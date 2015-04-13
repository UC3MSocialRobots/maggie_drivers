/*-----------------------------------------------------------
|															|
| FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3.cpp	|
|															|
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

Version       		:	03.02.05 / 08.09.2010 / M. Sahm

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

#include "FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3.h"
#include "FedmIscTagHandler_ISO14443_4_MIFARE_DESFire.h"
#include "FedmIscTagHandler_ISO14443_4.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"
#include "../../FEDM_Functions.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3(
	FEDM_ISOTabItem* pTabItem, 
	FedmIscTagHandler_ISO14443_4_MIFARE_DESFire* pParent )
{
	m_pTabItem			= pTabItem;
	m_pParent			= pParent;

	Init();
}

FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::~FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3()
{
}

// set all data members to 0
void FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::Init()
{
	m_ucErrorSource	= 0;
	m_uiErrorCode	= 0;
}


	//####################################################################
	// 5 security related commands
	
	// [0xFA] : [0xC3] DESFire Commands (Authenticate, AuthenticateISO, AuthenticateAES)
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::Authenticate(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucReaderKeyIndex,
	unsigned char ucDESFireAuthMode,
	unsigned char ucDESFireKeyNo,
	unsigned char ucSamAuthMode,
	unsigned char ucSamKeyNo,
	unsigned char ucSamKeyVersion,
	unsigned char ucSamDivInputLen,
	unsigned char* pucDivInput )
{
	int iErr = 0;

	if(ucMode == 0x00)
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0xFA));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, (unsigned char)0x00));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_KEY_INDEX, ucReaderKeyIndex));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_KEY_NO, ucDESFireKeyNo));
	}
	else //if(ucMode == 0x01)
	{
		if(ucSamDivInputLen > 0)
		{
			FEDM_CHK3(pucDivInput);
		}
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0xFA));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, (unsigned char)0x01));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_DESFIRE_AUTH_MODE, ucDESFireAuthMode));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_KEY_NO, ucDESFireKeyNo));		
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SAM_AUTH_MODE, ucSamAuthMode));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SAM_KEY_NO, ucSamKeyNo));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SAM_KEY_VERSION, ucSamKeyVersion));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SAM_DIV_INPUT_LEN, ucSamDivInputLen));
		if(ucSamDivInputLen > 0)
		{
			FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SAM_DIV_INPUT, pucDivInput, ucSamDivInputLen));
		}
	}

	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0x54] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::ChangeKeySettings(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucNewKeySettings )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0x54));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_NEW_KEY_SETTINGS, ucNewKeySettings));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0x45] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::GetKeySettings(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned char* pData = NULL;
	int iErr = 0;
	int iAdr = 0;
	unsigned int uiLen = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0x45));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		// Response Daten holen...OK
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;

		iAdr = FEDM_GetAdrOfID(FEDM_ISC_TMP_0xC3_RSP_DATA, 32);
		if(iAdr < 0)
			return iAdr;
		pData = &m_pParent->m_pReader->m_TmpData[iAdr];
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_RSP_DATA_LEN, &uiLen));
		if(uiResponseBufferLength < uiLen)
			return FEDM_ERROR_BUFFER_LENGTH;
		uiResponseLength = uiLen;
		memcpy(pucResponseData, pData, uiLen);
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
		uiResponseLength = 0;
	}

	return iBack;
}

	// [0xC4] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::ChangeKey(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucKeyNoToBeChanged,
	unsigned char ucAESNewKeyVersion,
	std::string sOldKey,
	std::string sNewKey,
	unsigned char ucSamKeyCompMethod,
	unsigned char ucSamCfg,
	unsigned char ucSamKeyNoCurrentKey,
	unsigned char ucSamKeyVersionCurrentKey,
	unsigned char ucSamKeyNoNewKey,
	unsigned char ucSamKeyVersionNewKey,
	unsigned char ucSamDivInputLen,
	unsigned char* pucDivInput )
{
	int iErr = 0;
	size_t iPadd = 0;

	if(ucMode == 0x00)
	{
		if(sOldKey.length() != sNewKey.length())
			return FEDM_ERROR_STRING_LENGTH;

		if(sOldKey.length() != 16 && sOldKey.length() != 32 && sOldKey.length() != 48)
			return FEDM_ERROR_STRING_LENGTH;

		if(sNewKey.length() != 16 && sNewKey.length() != 32 && sNewKey.length() != 48)
			return FEDM_ERROR_STRING_LENGTH;


		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0xC4));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, (unsigned char)0x00));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_KEY_NO_TB_CHANGED, ucKeyNoToBeChanged));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_KEY_LEN, (unsigned char)(sNewKey.length()>>1))); // :2
		//Padding with '00' to 24 Byte KeyLength
		for(iPadd = sOldKey.length(); iPadd < 48; iPadd++)
		{
			sOldKey += "0";
		}
		for(iPadd = sNewKey.length(); iPadd < 48; iPadd++)
		{
			sNewKey += "0";
		}
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_OLD_KEY, sOldKey));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_NEW_KEY, sNewKey));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_NEW_KEY_VERSION, ucAESNewKeyVersion));
	}
	else //if(ucMode == 0x01)
	{
		if(ucSamDivInputLen > 0)
		{
			FEDM_CHK3(pucDivInput);
		}
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0xC4));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, (unsigned char)0x01));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SAM_KEY_COMP_METHOD, ucSamKeyCompMethod));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SAM_CFG, ucSamCfg));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SAM_KEY_NO_CURRENT_KEY, ucSamKeyNoCurrentKey));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SAM_KEY_VERSION_CURRENT_KEY, ucSamKeyVersionCurrentKey));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SAM_KEY_NO_NEW_KEY, ucSamKeyNoNewKey));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SAM_KEY_VERSION_NEW_KEY, ucSamKeyVersionNewKey));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SAM_DIV_INPUT_LEN, ucSamDivInputLen));
		if(ucSamDivInputLen > 0)
		{
			FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SAM_DIV_INPUT, pucDivInput, ucSamDivInputLen));
		}
	}

	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0x64] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::GetKeyVersion(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucDESFireKeyNo,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned char* pData = NULL;
	int iErr = 0;
	int iAdr = 0;
	unsigned int uiLen = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0x64));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_KEY_NO, ucDESFireKeyNo));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		// Response Daten holen...OK
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;

		iAdr = FEDM_GetAdrOfID(FEDM_ISC_TMP_0xC3_RSP_DATA, 32);
		if(iAdr < 0)
			return iAdr;
		pData = &m_pParent->m_pReader->m_TmpData[iAdr];
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_RSP_DATA_LEN, &uiLen));
		if(uiResponseBufferLength < uiLen)
			return FEDM_ERROR_BUFFER_LENGTH;
		uiResponseLength = uiLen;
		memcpy(pucResponseData, pData, uiLen);
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
		uiResponseLength = 0;
	}

	return iBack;
}


	//####################################################################
	// 10 PICC level commands

	// [0xCA] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::CreateApplication(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucPICCLevelFlags,
	unsigned int uiApplicationID,
	unsigned char ucKeySetting1,
	unsigned char ucKeySetting2,
	unsigned int uiISOFileID,
	std::string sDFName )
{
	int iErr = 0;

	// swap MSB with LSB
	unsigned int uiTmp = 0;
	uiTmp |= ((uiApplicationID & 0x000000FF) << 16);
	uiTmp |= ((uiApplicationID & 0x0000FF00) );
	uiTmp |= ((uiApplicationID & 0x00FF0000) >> 16);


	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0xCA));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_PICC_LEVEL_FLAGS, ucPICCLevelFlags));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_APP_ID, uiTmp));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_KEY_SETTINGS1, ucKeySetting1));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_KEY_SETTINGS2, ucKeySetting2));
	if((ucKeySetting2 & 0x40) || (ucPICCLevelFlags & 0x01))
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_ISO7816_FILE_ID, uiISOFileID));
	}
	if((sDFName.length() != 0) || (ucPICCLevelFlags & 0x02))
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_ISO7816_DF_NAME_LEN, (unsigned char)sDFName.length()));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_ISO7816_DF_NAME, sDFName));
	}
	
	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0xDA] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::DeleteApplication(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucPICCLevelFlags,
	unsigned int uiApplicationID )
{
	// swap MSB with LSB
	unsigned int uiTmp = 0;
	uiTmp |= ((uiApplicationID & 0x000000FF) << 16);
	uiTmp |= ((uiApplicationID & 0x0000FF00) );
	uiTmp |= ((uiApplicationID & 0x00FF0000) >> 16);

	int iErr = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0xDA));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_PICC_LEVEL_FLAGS, ucPICCLevelFlags));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_APP_ID, uiTmp));
	
	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0x6A] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::GetApplicationIDs(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucPICCLevelFlags,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned char* pData = NULL;
	int iErr = 0;
	int iAdr = 0;
	unsigned int uiLen = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0x6A));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_PICC_LEVEL_FLAGS, ucPICCLevelFlags));
	
	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		// Response Daten holen...OK
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;

		iAdr = FEDM_GetAdrOfID(FEDM_ISC_TMP_0xC3_RSP_DATA, 32);
		if(iAdr < 0)
			return iAdr;
		pData = &m_pParent->m_pReader->m_TmpData[iAdr];
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_RSP_DATA_LEN, &uiLen));
		if(uiResponseBufferLength < uiLen)
			return FEDM_ERROR_BUFFER_LENGTH;
		uiResponseLength = uiLen;
		memcpy(pucResponseData, pData, uiLen);
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
		uiResponseLength = 0;
	}

	return iBack;
}

	// [0x6E] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::FreeMemory(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucPICCLevelFlags,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	unsigned char* pData = NULL;
	int iErr = 0;
	int iAdr = 0;
	unsigned int uiLen = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0x6E));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_PICC_LEVEL_FLAGS, ucPICCLevelFlags));
	
	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		// Response Daten holen...OK
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;

		iAdr = FEDM_GetAdrOfID(FEDM_ISC_TMP_0xC3_RSP_DATA, 32);
		if(iAdr < 0)
			return iAdr;
		pData = &m_pParent->m_pReader->m_TmpData[iAdr];
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_RSP_DATA_LEN, &uiLen));
		if(uiResponseBufferLength < uiLen)
			return FEDM_ERROR_BUFFER_LENGTH;
		uiResponseLength = uiLen;
		memcpy(pucResponseData, pData, uiLen);
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
		uiResponseLength = 0;
	}

	return iBack;
}

	// [0x6D] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::GetDFNames(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucPICCLevelFlags,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned char* pData = NULL;
	int iErr = 0;
	int iAdr = 0;
	unsigned int uiLen = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0x6D));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_PICC_LEVEL_FLAGS, ucPICCLevelFlags));
	
	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		// Response Daten holen...OK
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;

		iAdr = FEDM_GetAdrOfID(FEDM_ISC_TMP_0xC3_RSP_DATA, 32);
		if(iAdr < 0)
			return iAdr;
		pData = &m_pParent->m_pReader->m_TmpData[iAdr];
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_RSP_DATA_LEN, &uiLen));
		if(uiResponseBufferLength < uiLen)
			return FEDM_ERROR_BUFFER_LENGTH;
		uiResponseLength = uiLen;
		memcpy(pucResponseData, pData, uiLen);
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
		uiResponseLength = 0;
	}

	return iBack;
}

	// [0x5A] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::SelectApplication(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucPICCLevelFlags,
	unsigned int uiApplicationID )
{
	// swap MSB with LSB
	unsigned int uiTmp = 0;
	uiTmp |= ((uiApplicationID & 0x000000FF) << 16);
	uiTmp |= ((uiApplicationID & 0x0000FF00) );
	uiTmp |= ((uiApplicationID & 0x00FF0000) >> 16);

	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0x5A));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_PICC_LEVEL_FLAGS, ucPICCLevelFlags));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_APP_ID, uiTmp));
	
	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0xFC] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::FormatPICC(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucPICCLevelFlags )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0xFC));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_PICC_LEVEL_FLAGS, ucPICCLevelFlags));
	
	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0x60] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::GetVersion(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucPICCLevelFlags,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned char* pData = NULL;
	int iErr = 0;
	int iAdr = 0;
	unsigned int uiLen = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0x60));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_PICC_LEVEL_FLAGS, ucPICCLevelFlags));
	
	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		// Response Daten holen...OK
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;

		iAdr = FEDM_GetAdrOfID(FEDM_ISC_TMP_0xC3_RSP_DATA, 32);
		if(iAdr < 0)
			return iAdr;
		pData = &m_pParent->m_pReader->m_TmpData[iAdr];
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_RSP_DATA_LEN, &uiLen));
		if(uiResponseBufferLength < uiLen)
			return FEDM_ERROR_BUFFER_LENGTH;
		uiResponseLength = uiLen;
		memcpy(pucResponseData, pData, uiLen);
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
		uiResponseLength = 0;
	}

	return iBack;
}

	// [0x51] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::GetCardUID(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucPICCLevelFlags,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned char* pData = NULL;
	int iErr = 0;
	int iAdr = 0;
	unsigned int uiLen = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0x51));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_PICC_LEVEL_FLAGS, ucPICCLevelFlags));
	
	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		// Response Daten holen...OK
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;

		iAdr = FEDM_GetAdrOfID(FEDM_ISC_TMP_0xC3_RSP_DATA, 32);
		if(iAdr < 0)
			return iAdr;
		pData = &m_pParent->m_pReader->m_TmpData[iAdr];
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_RSP_DATA_LEN, &uiLen));
		if(uiResponseBufferLength < uiLen)
			return FEDM_ERROR_BUFFER_LENGTH;
		uiResponseLength = uiLen;
		memcpy(pucResponseData, pData, uiLen);
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
		uiResponseLength = 0;
	}

	return iBack;
}

	// [0x5C] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::SetConfiguration(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucPICCLevelFlags,
	unsigned char* pucRequestData,
	unsigned int uiRequestLength )
{
	FEDM_CHK3(pucRequestData);

	int iErr = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0x5C));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_PICC_LEVEL_FLAGS, ucPICCLevelFlags));
	unsigned char Option = pucRequestData[0];
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_0x6E_OPTION, Option));
	if(Option == 0x00)
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_0x6E_CONFIG, pucRequestData[1]));
	}
	else if(Option == 0x01)
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_0x6E_DEFAULT_KEY_VERSION, pucRequestData[1]));
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_0x6E_DEFAULT_KEY, &pucRequestData[2], 24));
	}
	else // if(Option == 0x02)
	{
		// first byte of ATS contains length information TL
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_0x6E_USER_DEF_ATS, &pucRequestData[1], pucRequestData[1]));
	}
	
	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}


	//####################################################################
	// 10 application level commands

	// [0x5F] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::ChangeFileSettings(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucApplicationLevelFlags,
	unsigned char ucFileNo,
	unsigned char ucNewFileCommSettings,
	unsigned char ucNewFileReadWriteAccessRights,
	unsigned char ucNewFileChangeAccessRights,
	unsigned char ucNewFileReadAccessRights,
	unsigned char ucNewFileWriteAccessRights )
{
	int iErr = 0;

	unsigned char ucAccessRights[2];
	ucAccessRights[0] = ((ucNewFileReadWriteAccessRights & 0x0F) << 4) + (ucNewFileChangeAccessRights & 0x0F);
	ucAccessRights[1] = ((ucNewFileReadAccessRights & 0x0F) << 4) + (ucNewFileWriteAccessRights & 0x0F);

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0x5F));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_APPLICATION_LEVEL_FLAGS, ucApplicationLevelFlags));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_NO, ucFileNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, ucNewFileCommSettings));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_ACCESS_RIGHTS, ucAccessRights, 2));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0x6F] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::GetFileIDs(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucApplicationLevelFlags,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned char* pData = NULL;
	int iErr = 0;
	int iAdr = 0;
	unsigned int uiLen = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0x6F));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_APPLICATION_LEVEL_FLAGS, ucApplicationLevelFlags));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		// Response Daten holen...OK
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;

		iAdr = FEDM_GetAdrOfID(FEDM_ISC_TMP_0xC3_RSP_DATA, 32);
		if(iAdr < 0)
			return iAdr;
		pData = &m_pParent->m_pReader->m_TmpData[iAdr];
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_RSP_DATA_LEN, &uiLen));
		if(uiResponseBufferLength < uiLen)
			return FEDM_ERROR_BUFFER_LENGTH;
		uiResponseLength = uiLen;
		memcpy(pucResponseData, pData, uiLen);
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
		uiResponseLength = 0;
	}

	return iBack;
}

	// [0xF5] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::GetFileSettings(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucApplicationLevelFlags,
	unsigned char ucFileNo,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned char* pData = NULL;
	int iErr = 0;
	int iAdr = 0;
	unsigned int uiLen = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0xF5));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_APPLICATION_LEVEL_FLAGS, ucApplicationLevelFlags));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_NO, ucFileNo));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		// Response Daten holen...OK
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;

		iAdr = FEDM_GetAdrOfID(FEDM_ISC_TMP_0xC3_RSP_DATA, 32);
		if(iAdr < 0)
			return iAdr;
		pData = &m_pParent->m_pReader->m_TmpData[iAdr];
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_RSP_DATA_LEN, &uiLen));
		if(uiResponseBufferLength < uiLen)
			return FEDM_ERROR_BUFFER_LENGTH;
		uiResponseLength = uiLen;
		memcpy(pucResponseData, pData, uiLen);
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
		uiResponseLength = 0;
	}

	return iBack;
}

	// [0xCD] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::CreateStdDataFile(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucApplicationLevelFlags,
	unsigned char ucFileNo,
	unsigned int uiISOFileID,
	unsigned char ucFileCommSettings,
	unsigned char ucFileReadWriteAccessRights,
	unsigned char ucFileChangeAccessRights,
	unsigned char ucFileReadAccessRights,
	unsigned char ucFileWriteAccessRights,
	unsigned int uiFileSize )
{
	unsigned char TmpAccessRights[2];
	TmpAccessRights[0] = ((ucFileReadWriteAccessRights & 0x0F) << 4) + (ucFileChangeAccessRights & 0x0F);
	TmpAccessRights[1] = ((ucFileReadAccessRights & 0x0F) << 4) + (ucFileWriteAccessRights & 0x0F);

	// swap MSB with LSB
	unsigned int TmpFileSize = 0;
	TmpFileSize |= ((uiFileSize & 0x000000FF) << 16);
	TmpFileSize |= ((uiFileSize & 0x0000FF00) );
	TmpFileSize |= ((uiFileSize & 0x00FF0000) >> 16);

	int iErr = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0xCD));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_APPLICATION_LEVEL_FLAGS, ucApplicationLevelFlags));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_NO, ucFileNo));
	if(ucApplicationLevelFlags & 0x01)
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_ISO7816_FILE_ID, uiISOFileID));
	}
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, ucFileCommSettings));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_ACCESS_RIGHTS, TmpAccessRights, 2));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_SIZE, TmpFileSize));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0xCB] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::CreateBackupDataFile(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucApplicationLevelFlags,
	unsigned char ucFileNo,
	unsigned int uiISOFileID,
	unsigned char ucFileCommSettings,
	unsigned char ucFileReadWriteAccessRights,
	unsigned char ucFileChangeAccessRights,
	unsigned char ucFileReadAccessRights,
	unsigned char ucFileWriteAccessRights,
	unsigned int uiFileSize )
{
	unsigned char TmpAccessRights[2];
	TmpAccessRights[0] = ((ucFileReadWriteAccessRights & 0x0F) << 4) + (ucFileChangeAccessRights & 0x0F);
	TmpAccessRights[1] = ((ucFileReadAccessRights & 0x0F) << 4) + (ucFileWriteAccessRights & 0x0F);

	// swap MSB with LSB
	unsigned int TmpFileSize = 0;
	TmpFileSize |= ((uiFileSize & 0x000000FF) << 16);
	TmpFileSize |= ((uiFileSize & 0x0000FF00) );
	TmpFileSize |= ((uiFileSize & 0x00FF0000) >> 16);

	int iErr = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0xCB));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_APPLICATION_LEVEL_FLAGS, ucApplicationLevelFlags));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_NO, ucFileNo));
	if(ucApplicationLevelFlags & 0x01)
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_ISO7816_FILE_ID, uiISOFileID));
	}
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, ucFileCommSettings));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_ACCESS_RIGHTS, TmpAccessRights, 2));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_SIZE, TmpFileSize));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0xCC] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::CreateValueFile(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucApplicationLevelFlags,
	unsigned char ucFileNo,
	unsigned char ucFileCommSettings,
	unsigned char ucFileReadWriteAccessRights,
	unsigned char ucFileChangeAccessRights,
	unsigned char ucFileReadAccessRights,
	unsigned char ucFileWriteAccessRights,
	unsigned int uiLowerLimit,
	unsigned int uiUpperLimit,
	unsigned int uiValue,
	unsigned char ucLimitedCreditEnabled )
{
	unsigned char TmpAccessRights[2];
	TmpAccessRights[0] = ((ucFileReadWriteAccessRights & 0x0F) << 4) + (ucFileChangeAccessRights & 0x0F);
	TmpAccessRights[1] = ((ucFileReadAccessRights & 0x0F) << 4) + (ucFileWriteAccessRights & 0x0F);

	// swap MSB with LSB
	unsigned int TmpLowerLimit = 0;
	TmpLowerLimit |= ((uiLowerLimit & 0x000000FF) << 24);
	TmpLowerLimit |= ((uiLowerLimit & 0x0000FF00) << 8);
	TmpLowerLimit |= ((uiLowerLimit & 0x00FF0000) >> 8);
	TmpLowerLimit |= ((uiLowerLimit & 0xFF000000) >> 24);

	unsigned int TmpUpperLimit = 0;
	TmpUpperLimit |= ((uiUpperLimit & 0x000000FF) << 24);
	TmpUpperLimit |= ((uiUpperLimit & 0x0000FF00) << 8);
	TmpUpperLimit |= ((uiUpperLimit & 0x00FF0000) >> 8);
	TmpUpperLimit |= ((uiUpperLimit & 0xFF000000) >> 24);

	unsigned int TmpValue = 0;
	TmpValue |= ((uiValue & 0x000000FF) << 24);
	TmpValue |= ((uiValue & 0x0000FF00) << 8);
	TmpValue |= ((uiValue & 0x00FF0000) >> 8);
	TmpValue |= ((uiValue & 0xFF000000) >> 24);

	int iErr = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0xCC));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_APPLICATION_LEVEL_FLAGS, ucApplicationLevelFlags));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_NO, ucFileNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, ucFileCommSettings));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_ACCESS_RIGHTS, TmpAccessRights, 2));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_LOWER_LIMIT, TmpLowerLimit));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_UPPER_LIMIT, TmpUpperLimit));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_VALUE, TmpValue));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_LIMITED_CREDIT_ENABLED, ucLimitedCreditEnabled));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0xC1] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::CreateLinearRecordFile(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucApplicationLevelFlags,
	unsigned char ucFileNo,
	unsigned int uiISOFileID,
	unsigned char ucFileCommSettings,
	unsigned char ucFileReadWriteAccessRights,
	unsigned char ucFileChangeAccessRights,
	unsigned char ucFileReadAccessRights,
	unsigned char ucFileWriteAccessRights,
	unsigned int uiRecordSize,
	unsigned int uiMaxNoOfRecords )
{
	unsigned char TmpAccessRights[2];
	TmpAccessRights[0] = ((ucFileReadWriteAccessRights & 0x0F) << 4) + (ucFileChangeAccessRights & 0x0F);
	TmpAccessRights[1] = ((ucFileReadAccessRights & 0x0F) << 4) + (ucFileWriteAccessRights & 0x0F);

	// swap MSB with LSB
	unsigned int TmpRecordSize = 0;
	TmpRecordSize |= ((uiRecordSize & 0x000000FF) << 16);
	TmpRecordSize |= ((uiRecordSize & 0x0000FF00));
	TmpRecordSize |= ((uiRecordSize & 0x00FF0000) >> 16);

	unsigned int TmpMaxNoOfRecords = 0;
	TmpMaxNoOfRecords |= ((uiMaxNoOfRecords & 0x000000FF) << 16);
	TmpMaxNoOfRecords |= ((uiMaxNoOfRecords & 0x0000FF00));
	TmpMaxNoOfRecords |= ((uiMaxNoOfRecords & 0x00FF0000) >> 16);

	int iErr = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0xC1));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_APPLICATION_LEVEL_FLAGS, ucApplicationLevelFlags));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_NO, ucFileNo));
	if(ucApplicationLevelFlags & 0x01)
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_ISO7816_FILE_ID, uiISOFileID));
	}
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, ucFileCommSettings));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_ACCESS_RIGHTS, TmpAccessRights, 2));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_RECORD_SIZE, TmpRecordSize));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_NO_OF_RECORDS, TmpMaxNoOfRecords));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0xC0] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::CreateCyclicRecordFile(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucApplicationLevelFlags,
	unsigned char ucFileNo,
	unsigned int uiISOFileID,
	unsigned char ucFileCommSettings,
	unsigned char ucFileReadWriteAccessRights,
	unsigned char ucFileChangeAccessRights,
	unsigned char ucFileReadAccessRights,
	unsigned char ucFileWriteAccessRights,
	unsigned int uiRecordSize,
	unsigned int uiMaxNoOfRecords )
{
	unsigned char TmpAccessRights[2];
	TmpAccessRights[0] = ((ucFileReadWriteAccessRights & 0x0F) << 4) + (ucFileChangeAccessRights & 0x0F);
	TmpAccessRights[1] = ((ucFileReadAccessRights & 0x0F) << 4) + (ucFileWriteAccessRights & 0x0F);

	// swap MSB with LSB
	unsigned int TmpRecordSize = 0;
	TmpRecordSize |= ((uiRecordSize & 0x000000FF) << 16);
	TmpRecordSize |= ((uiRecordSize & 0x0000FF00));
	TmpRecordSize |= ((uiRecordSize & 0x00FF0000) >> 16);

	unsigned int TmpMaxNoOfRecords = 0;
	TmpMaxNoOfRecords |= ((uiMaxNoOfRecords & 0x000000FF) << 16);
	TmpMaxNoOfRecords |= ((uiMaxNoOfRecords & 0x0000FF00));
	TmpMaxNoOfRecords |= ((uiMaxNoOfRecords & 0x00FF0000) >> 16);

	int iErr = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0xC0));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_APPLICATION_LEVEL_FLAGS, ucApplicationLevelFlags));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_NO, ucFileNo));
	if(ucApplicationLevelFlags & 0x01)
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_ISO7816_FILE_ID, uiISOFileID));
	}
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, ucFileCommSettings));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_ACCESS_RIGHTS, TmpAccessRights, 2));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_RECORD_SIZE, TmpRecordSize));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_NO_OF_RECORDS, TmpMaxNoOfRecords));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0xDF] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::DeleteFile(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucApplicationLevelFlags,
	unsigned char ucFileNo )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0xDF));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_APPLICATION_LEVEL_FLAGS, ucApplicationLevelFlags));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_NO, ucFileNo));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0x61] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::GetISOFileIDs(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucApplicationLevelFlags,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	FEDM_CHK3(pucResponseData);

	unsigned char* pData = NULL;
	int iErr = 0;
	int iAdr = 0;
	unsigned int uiLen = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0x61));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_APPLICATION_LEVEL_FLAGS, ucApplicationLevelFlags));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		// Response Daten holen...OK
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;

		iAdr = FEDM_GetAdrOfID(FEDM_ISC_TMP_0xC3_RSP_DATA, 32);
		if(iAdr < 0)
			return iAdr;
		pData = &m_pParent->m_pReader->m_TmpData[iAdr];
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_RSP_DATA_LEN, &uiLen));
		if(uiResponseBufferLength < uiLen)
			return FEDM_ERROR_BUFFER_LENGTH;
		uiResponseLength = uiLen;
		memcpy(pucResponseData, pData, uiLen);
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
		uiResponseLength = 0;
	}

	return iBack;
}


	//####################################################################
	// 11 data manipulation commands
	
	// [0xBD] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::ReadStandardData(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucDataManipulationFlags,
	unsigned char ucFileNo,
	unsigned char ucFileCommSettings,
	unsigned int uiFileOffset,
	unsigned int uiFileLen,
	unsigned char* pucFileData )
{
	FEDM_CHK3(pucFileData);

	int iErr = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0xBD));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_DATA_MANIPULATION_FLAGS, ucDataManipulationFlags));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_NO, ucFileNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, ucFileCommSettings));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_OFFSET, uiFileOffset));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_DATA_LEN, uiFileLen));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		memcpy(pucFileData, &m_pTabItem->m_ucRxDB[uiFileOffset], uiFileLen);
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}
	
	// [0x3D] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::WriteStandardData(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucDataManipulationFlags,
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

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0x3D));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_DATA_MANIPULATION_FLAGS, ucDataManipulationFlags));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_NO, ucFileNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, ucFileCommSettings));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_OFFSET, uiFileOffset));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_DATA_LEN, uiFileLen));

	memcpy(&m_pTabItem->m_ucTxDB[uiFileOffset], pucFileData, uiFileLen);

	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0x6C] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::GetValue(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucDataManipulationFlags,
	unsigned char ucFileNo,
	unsigned char ucFileCommSettings,
	unsigned int& uiValue )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0x6C));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_DATA_MANIPULATION_FLAGS, ucDataManipulationFlags));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_NO, ucFileNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, ucFileCommSettings));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		uiValue = m_pTabItem->m_uiValue;
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0x0C] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::Credit(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucDataManipulationFlags,
	unsigned char ucFileNo,
	unsigned char ucFileCommSettings,
	unsigned int uiCreditValue)
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0x0C));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_DATA_MANIPULATION_FLAGS, ucDataManipulationFlags));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_NO, ucFileNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, ucFileCommSettings));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CREDIT_VALUE, uiCreditValue));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0xDC] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::Debit(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucDataManipulationFlags,
	unsigned char ucFileNo,
	unsigned char ucFileCommSettings,
	unsigned int uiDebitValue )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0xDC));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_DATA_MANIPULATION_FLAGS, ucDataManipulationFlags));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_NO, ucFileNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, ucFileCommSettings));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_DEBIT_VALUE, uiDebitValue));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}


	// [0x1C] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::LimitedCredit(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucDataManipulationFlags,
	unsigned char ucFileNo,
	unsigned char ucFileCommSettings,
	unsigned int uiCreditValue )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0x1C));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_DATA_MANIPULATION_FLAGS, ucDataManipulationFlags));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_NO, ucFileNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, ucFileCommSettings));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CREDIT_VALUE, uiCreditValue));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0x3B] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::WriteRecord(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucDataManipulationFlags,
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

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0x3B));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_DATA_MANIPULATION_FLAGS, ucDataManipulationFlags));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_NO, ucFileNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, ucFileCommSettings));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_OFFSET, uiFileOffset));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_DATA_LEN, uiRecordLen));

	memcpy(&m_pTabItem->m_ucTxDB[uiFileOffset], pucRecordData, uiRecordLen);

	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0xBB] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::ReadRecords(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucDataManipulationFlags,
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


	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0xBB));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_DATA_MANIPULATION_FLAGS, ucDataManipulationFlags));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_NO, ucFileNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, ucFileCommSettings));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_RECORD_OFFSET, uiRecordOffset));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_NO_OF_RECORDS, uiNoOfRecords));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_RECORD_SIZE, uiRecordSize));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		memcpy(pucRecordData, &m_pTabItem->m_ucRxDB[0], uiNoOfRecords*uiRecordSize);
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0xEB] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::ClearRecordFile(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucDataManipulationFlags,
	unsigned char ucFileNo,
	unsigned char ucFileCommSettings )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0xEB));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_DATA_MANIPULATION_FLAGS, ucDataManipulationFlags));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_NO, ucFileNo));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, ucFileCommSettings));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0xC7] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::CommitTransaction(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucDataManipulationFlags )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0xC7));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_DATA_MANIPULATION_FLAGS, ucDataManipulationFlags));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

	// [0xA7] : [0xC3] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3::AbortTransaction(
	unsigned char ucMode,
	unsigned char ucSlot,
	unsigned char ucTimeout,
	unsigned char ucDataManipulationFlags )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_CMD, (unsigned char)0xA7));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_MODE, ucMode));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_SLOT_NO, ucSlot));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_TIMEOUT, ucTimeout));
	FEDM_CHK1(iErr, m_pParent->m_pReader->SetData(FEDM_ISC_TMP_0xC3_DATA_MANIPULATION_FLAGS, ucDataManipulationFlags));

	int iBack = m_pParent->m_pReader->SendProtocol(0xC3);
	if( iBack==0x00 )
	{
		m_ucErrorSource = 0;
		m_uiErrorCode = 0;
	}
	else if( iBack==0x97 )
	{
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, &m_ucErrorSource));
		FEDM_CHK1(iErr, m_pParent->m_pReader->GetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_uiErrorCode));
	}

	return iBack;
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
