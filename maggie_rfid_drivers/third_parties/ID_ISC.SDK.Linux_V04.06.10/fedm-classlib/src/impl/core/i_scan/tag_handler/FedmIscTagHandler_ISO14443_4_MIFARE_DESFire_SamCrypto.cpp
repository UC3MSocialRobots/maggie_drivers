/*-------------------------------------------------------------------
|                                                       			|
|      FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto.cpp	|
|                                                       			|
---------------------------------------------------------------------

Copyright © 2009-2011	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	30.03.2009

Version       		:	03.03.00 / 27.01.2011 / M. Sahm, M. Hultsch

Operation Systems	:	independent

Function			:	implementation class for OBID® classic-pro transponder ISO14443-4 MIFARE DESFire
						with [0xC3] commands and internal SAM crypto commands

NOTE				:	this class supports only selected mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "../../FEDM.h"
#include "../FEDM_ISCReader.h"
#include "FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto.h"
#include "FedmIscTagHandler_ISO14443_4_MIFARE_DESFire.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto()
{
	m_uiTimeout = 30; // 30 x 100ms
}

FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::~FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto()
{
}

void FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::SetTimeout(unsigned int uiTimeout)
{
	m_uiTimeout = uiTimeout;
}

unsigned char FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::GetErrorSource()
{
	return m_pParent->m_pImpl_C3->m_ucErrorSource;
}

unsigned int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::GetErrorCode()
{
	return m_pParent->m_pImpl_C3->m_uiErrorCode;
}

//---------------------------------------------------------------------------------
// ------------------------ SECURITY RELATED COMMANDS -----------------------------
//---------------------------------------------------------------------------------
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::Authenticate(
	unsigned char ucSamSlotNo,
	unsigned char ucDESFireAuthMode,
	unsigned char ucDESFireKeyNo,
	unsigned char ucSamAuthMode,
	unsigned char ucSamKeyNo,
	unsigned char ucSamKeyVersion,
	unsigned char ucSamDiversificationInputLen,
	unsigned char* pucSamDiversificationInput )
{
	unsigned char RFU = 0x00;
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->Authenticate(0x01, ucSamSlotNo, m_uiTimeout, RFU, ucDESFireAuthMode, ucDESFireKeyNo, ucSamAuthMode, ucSamKeyNo, ucSamKeyVersion, ucSamDiversificationInputLen, pucSamDiversificationInput);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}


int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::ChangeKeySettings(
	unsigned char ucSamSlotNo, 
	unsigned char ucNewKeySettings )
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->ChangeKeySettings(0x01, ucSamSlotNo, m_uiTimeout, ucNewKeySettings);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}


int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::GetKeySettings(
	unsigned char ucSamSlotNo, 
	unsigned char *pucResponseData, 
	unsigned int uiResponseBufferLength, 
	unsigned int &uiResponseLength )
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout);
	int iBack = m_pParent->m_pImpl_C3->GetKeySettings(0x01, ucSamSlotNo, m_uiTimeout, pucResponseData, uiResponseBufferLength, uiResponseLength);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}


int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::ChangeKey(
	unsigned char ucSamSlotNo, 
	unsigned char ucSamKeyCompilationMethod, 
	unsigned char ucSamCfg, 
	unsigned char ucSamKeyNoCurrentKey, 
	unsigned char ucSamKeyVersionCurrentKey, 
	unsigned char ucSamKeyNoNewKey, 
	unsigned char ucSamKeyVersionNewKey, 
	unsigned char ucSamDiversificationInputLen, 
	unsigned char *pucSamDiversificationInput )
{
	unsigned char ucRFU = 0x00;
	unsigned int uiRFU = 0;
	string sRFU = "";
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->ChangeKey(	0x01, 
													ucSamSlotNo, 
													m_uiTimeout, 
													ucRFU, 
													ucRFU, 
													sRFU, 
													sRFU, 
													ucSamKeyCompilationMethod, 
													ucSamCfg, 
													ucSamKeyNoCurrentKey, 
													ucSamKeyVersionCurrentKey, 
													ucSamKeyNoNewKey, 
													ucSamKeyVersionNewKey, 
													ucSamDiversificationInputLen, 
													pucSamDiversificationInput);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}


int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::GetKeyVersion(
	unsigned char ucSamSlotNo, 
	unsigned char ucDESFireKeyNo, 
	unsigned char *pucResponseData, 
	unsigned int uiResponseBufferLength, 
	unsigned int &uiResponseLength)
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->GetKeyVersion(0x01, ucSamSlotNo, m_uiTimeout, ucDESFireKeyNo, pucResponseData, uiResponseBufferLength, uiResponseLength);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

//---------------------------------------------------------------------------------
// ------------------------ PICC LEVEL COMMANDS -----------------------------------
//---------------------------------------------------------------------------------
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::CreateApplication(
	unsigned char ucSamSlotNo, 
	unsigned char ucPiccLevelFlags, 
	unsigned int uiApplicationID, 
	unsigned char ucKeySetting1, 
	unsigned char ucKeySetting2, 
	unsigned int uiISOFileID, 
	std::string sDFName )
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->CreateApplication(0x01, ucSamSlotNo, m_uiTimeout, ucPiccLevelFlags, uiApplicationID, ucKeySetting1, ucKeySetting2, uiISOFileID, sDFName);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::DeleteApplication(
	unsigned char ucSamSlotNo, 
	unsigned char ucPiccLevelFlags,
	unsigned int uiApplicationID )
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->DeleteApplication(0x01, ucSamSlotNo, m_uiTimeout, ucPiccLevelFlags, uiApplicationID);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::GetApplicationIDs(
	unsigned char ucSamSlotNo, 
	unsigned char ucPiccLevelFlags, 
	unsigned char *pucResponseData, 
	unsigned int uiResponseBufferLength, 
	unsigned int &uiResponseLength)
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->GetApplicationIDs(0x01, ucSamSlotNo, m_uiTimeout, ucPiccLevelFlags, pucResponseData, uiResponseBufferLength, uiResponseLength);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::FreeMemory(
	unsigned char ucSamSlotNo, 
	unsigned char ucPiccLevelFlags,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->FreeMemory(0x01, ucSamSlotNo, m_uiTimeout, ucPiccLevelFlags, pucResponseData, uiResponseBufferLength, uiResponseLength);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::GetDFNames(
	unsigned char ucSamSlotNo,
	unsigned char ucPiccLevelFlags, 
	unsigned char *pucResponseData, 
	unsigned int uiResponseBufferLength, 
	unsigned int &uiResponseLength)
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->GetDFNames(0x01, ucSamSlotNo, m_uiTimeout, ucPiccLevelFlags, pucResponseData, uiResponseBufferLength, uiResponseLength);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::SelectApplication(
	unsigned char ucSamSlotNo, 
	unsigned char ucPiccLevelFlags, 
	unsigned int uiApplicationID)
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->SelectApplication(0x01, ucSamSlotNo, m_uiTimeout, ucPiccLevelFlags, uiApplicationID);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::FormatPICC(
	unsigned char ucSamSlotNo, 
	unsigned char ucPiccLevelFlags)
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->FormatPICC(0x01, ucSamSlotNo, m_uiTimeout, ucPiccLevelFlags);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::GetVersion(
	unsigned char ucSamSlotNo, 
	unsigned char ucPiccLevelFlags, 
	unsigned char *pucResponseData, 
	unsigned int uiResponseBufferLength, 
	unsigned int &uiResponseLength )
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->GetVersion(0x01, ucSamSlotNo, m_uiTimeout, ucPiccLevelFlags, pucResponseData, uiResponseBufferLength, uiResponseLength);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::GetCardUID(
	unsigned char ucSamSlotNo, 
	unsigned char ucPiccLevelFlags, 
	unsigned char *pucResponseData, 
	unsigned int uiResponseBufferLength, 
	unsigned int &uiResponseLength )
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->GetCardUID(0x01, ucSamSlotNo, m_uiTimeout, ucPiccLevelFlags, pucResponseData, uiResponseBufferLength, uiResponseLength);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::SetConfiguration(
	unsigned char ucSamSlotNo, 
	unsigned char ucPiccLevelFlags, 
	unsigned char *pucRequestData, 
	unsigned int uiRequestLength )
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->SetConfiguration(0x01, ucSamSlotNo, m_uiTimeout, ucPiccLevelFlags, pucRequestData, uiRequestLength);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

//---------------------------------------------------------------------------------
// ------------------------ APPLICATION LEVEL COMMANDS ----------------------------
//---------------------------------------------------------------------------------
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::ChangeFileSettings(
	unsigned char ucSamSlotNo, 
	unsigned char ucApplicationLevelFlags, 
	unsigned char ucFileNo, 
	unsigned char ucNewFileCommSettings, 
	unsigned char ucNewFileReadWriteAccessRights, 
	unsigned char ucNewFileChangeAccessRights, 
	unsigned char ucNewFileReadAccessRights, 
	unsigned char ucNewFileWriteAccessRights)
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->ChangeFileSettings(0x01, ucSamSlotNo, m_uiTimeout, ucApplicationLevelFlags, ucFileNo, ucNewFileCommSettings, ucNewFileReadWriteAccessRights, ucNewFileChangeAccessRights, ucNewFileReadAccessRights, ucNewFileWriteAccessRights);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::GetFileIDs(
	unsigned char ucSamSlotNo, 
	unsigned char ucApplicationLevelFlags, 
	unsigned char *pucResponseData, 
	unsigned int uiResponseBufferLength, 
	unsigned int &uiResponseLength )
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->GetFileIDs(0x01, ucSamSlotNo, m_uiTimeout, ucApplicationLevelFlags, pucResponseData, uiResponseBufferLength, uiResponseLength);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::GetFileSettings(
	unsigned char ucSamSlotNo, 
	unsigned char ucApplicationLevelFlags, 
	unsigned char ucFileNo, 
	unsigned char *pucResponseData, 
	unsigned int uiResponseBufferLength, 
	unsigned int &uiResponseLength )
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->GetFileSettings(0x01, ucSamSlotNo, m_uiTimeout, ucApplicationLevelFlags, ucFileNo, pucResponseData, uiResponseBufferLength, uiResponseLength);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::CreateStdDataFile(
	unsigned char ucSamSlotNo, 
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
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->CreateStdDataFile(0x01, ucSamSlotNo, m_uiTimeout, ucApplicationLevelFlags, ucFileNo, uiISOFileID, ucFileCommSettings, ucFileReadWriteAccessRights, ucFileChangeAccessRights, ucFileReadAccessRights, ucFileWriteAccessRights, uiFileSize);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::CreateBackupDataFile(
	unsigned char ucSamSlotNo, 
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
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->CreateBackupDataFile(0x01, ucSamSlotNo, m_uiTimeout, ucApplicationLevelFlags, ucFileNo, uiISOFileID, ucFileCommSettings, ucFileReadWriteAccessRights, ucFileChangeAccessRights, ucFileReadAccessRights, ucFileWriteAccessRights, uiFileSize);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::CreateValueFile(
	unsigned char ucSamSlotNo, 
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
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->CreateValueFile(0x01, ucSamSlotNo, m_uiTimeout, ucApplicationLevelFlags, ucFileNo, ucFileCommSettings, ucFileReadWriteAccessRights, ucFileChangeAccessRights, ucFileReadAccessRights, ucFileWriteAccessRights, uiLowerLimit, uiUpperLimit, uiValue, ucLimitedCreditEnabled);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::CreateLinearRecordFile(
	unsigned char ucSamSlotNo, 
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
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->CreateLinearRecordFile(0x01, ucSamSlotNo, m_uiTimeout, ucApplicationLevelFlags, ucFileNo, uiISOFileID, ucFileCommSettings, ucFileReadWriteAccessRights, ucFileChangeAccessRights, ucFileReadAccessRights, ucFileWriteAccessRights, uiRecordSize, uiMaxNoOfRecords);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::CreateCyclicRecordFile(
	unsigned char ucSamSlotNo, 
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
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->CreateCyclicRecordFile(0x01, ucSamSlotNo, m_uiTimeout, ucApplicationLevelFlags, ucFileNo, uiISOFileID, ucFileCommSettings, ucFileReadWriteAccessRights, ucFileChangeAccessRights, ucFileReadAccessRights, ucFileWriteAccessRights, uiRecordSize, uiMaxNoOfRecords);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::DeleteFile(
	unsigned char ucSamSlotNo, 
	unsigned char ucApplicationLevelFlags, 
	unsigned char ucFileNo )
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->DeleteFile(0x01, ucSamSlotNo, m_uiTimeout, ucApplicationLevelFlags, ucFileNo);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::GetISOFileIDs(
	unsigned char ucSamSlotNo, 
	unsigned char ucApplicationLevelFlags, 
	unsigned char *pucResponseData, 
	unsigned int uiResponseBufferLength, 
	unsigned int &uiResponseLength )
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->GetISOFileIDs(0x01, ucSamSlotNo, m_uiTimeout, ucApplicationLevelFlags, pucResponseData, uiResponseBufferLength, uiResponseLength);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

//---------------------------------------------------------------------------------
// ------------------------ DATA MANIPULATION COMMANDS ----------------------------
//---------------------------------------------------------------------------------
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::ReadStandardData(
	unsigned char ucSamSlotNo, 
	unsigned char ucDataManipulationFlags, 
	unsigned char ucFileNo, 
	unsigned char ucFileCommSettings, 
	unsigned int uiFileOffset, 
	unsigned int uiFileLen, 
	unsigned char *pucFileData)
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->ReadStandardData(0x01, ucSamSlotNo, m_uiTimeout, ucDataManipulationFlags, ucFileNo, ucFileCommSettings, uiFileOffset, uiFileLen, pucFileData);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::WriteStandardData(
	unsigned char ucSamSlotNo, 
	unsigned char ucDataManipulationFlags, 
	unsigned char ucFileNo, 
	unsigned char ucFileCommSettings, 
	unsigned int uiFileOffset, 
	unsigned int uiFileLen, 
	unsigned char *pucFileData)
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->WriteStandardData(0x01, ucSamSlotNo, m_uiTimeout, ucDataManipulationFlags, ucFileNo, ucFileCommSettings, uiFileOffset, uiFileLen, pucFileData);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::GetValue(
	unsigned char ucSamSlotNo, 
	unsigned char ucDataManipulationFlags, 
	unsigned char ucFileNo, 
	unsigned char ucFileCommSettings, 
	unsigned int &uiValue)
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->GetValue(0x01, ucSamSlotNo, m_uiTimeout, ucDataManipulationFlags, ucFileNo, ucFileCommSettings, uiValue);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::Debit(
	unsigned char ucSamSlotNo, 
	unsigned char ucDataManipulationFlags, 
	unsigned char ucFileNo, 
	unsigned char ucFileCommSettings, 
	unsigned int uiDebitValue)
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->Debit(0x01, ucSamSlotNo, m_uiTimeout, ucDataManipulationFlags, ucFileNo, ucFileCommSettings, uiDebitValue);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::Credit(
	unsigned char ucSamSlotNo, 
	unsigned char ucDataManipulationFlags, 
	unsigned char ucFileNo, 
	unsigned char ucFileCommSettings, 
	unsigned int uiCreditValue)
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->Credit(0x01, ucSamSlotNo, m_uiTimeout, ucDataManipulationFlags, ucFileNo, ucFileCommSettings, uiCreditValue);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::LimitedCredit(
	unsigned char ucSamSlotNo, 
	unsigned char ucDataManipulationFlags, 
	unsigned char ucFileNo, 
	unsigned char ucFileCommSettings, 
	unsigned int uiCreditValue)
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->LimitedCredit(0x01, ucSamSlotNo, m_uiTimeout, ucDataManipulationFlags, ucFileNo, ucFileCommSettings, uiCreditValue);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::WriteRecord(
	unsigned char ucSamSlotNo, 
	unsigned char ucDataManipulationFlags, 
	unsigned char ucFileNo, 
	unsigned char ucFileCommSettings, 
	unsigned int uiFileOffset, 
	unsigned int uiRecordLen, 
	unsigned char *pucRecordData)
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->WriteRecord(0x01, ucSamSlotNo, m_uiTimeout, ucDataManipulationFlags, ucFileNo, ucFileCommSettings, uiFileOffset, uiRecordLen, pucRecordData);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::ReadRecords(
	unsigned char ucSamSlotNo, 
	unsigned char ucDataManipulationFlags, 
	unsigned char ucFileNo, 
	unsigned char ucFileCommSettings, 
	unsigned int uiRecordOffset, 
	unsigned int uiNoOfRecords, 
	unsigned int uiRecordSize, 
	unsigned char *pucRecordData)
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->ReadRecords(0x01, ucSamSlotNo, m_uiTimeout, ucDataManipulationFlags, ucFileNo, ucFileCommSettings, uiRecordOffset, uiNoOfRecords, uiRecordSize, pucRecordData);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::ClearRecordFile(
	unsigned char ucSamSlotNo,
	unsigned char ucDataManipulationFlags,
	unsigned char ucFileNo,
	unsigned char ucFileCommSettings)
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->ClearRecordFile(0x01, ucSamSlotNo, m_uiTimeout, ucDataManipulationFlags, ucFileNo, ucFileCommSettings);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::CommitTransaction(
	unsigned char ucSamSlotNo, 
	unsigned char ucDataManipulationFlags )
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->CommitTransaction(0x01, ucSamSlotNo, m_uiTimeout, ucDataManipulationFlags);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::AbortTransaction(
	unsigned char ucSamSlotNo, 
	unsigned char ucDataManipulationFlags )
{
	unsigned int uiOldTimeout = 0;
	GetPortTimeout(&uiOldTimeout);
	SetPortTimeout(m_uiTimeout*100);
	int iBack = m_pParent->m_pImpl_C3->AbortTransaction(0x01, ucSamSlotNo, m_uiTimeout, ucDataManipulationFlags);
	SetPortTimeout(uiOldTimeout);
	return iBack;
}


/***************************************************************************
  Begin        		:	10.11.2010 / M. Hultsch

  Version       	:	03.02.07 / 10.11.2010 / M. Hultsch

  Function			:	gets port timeout from port library

  Parameters		:	unsigned int*  uiTimeout	-	pointer to value

  Return value		:	FEDM_OK or error code (<0)
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::GetPortTimeout(unsigned int* uiTimeout)
{
	int iPortHnd = m_pParent->m_pReader->GetPortHnd();
	if(iPortHnd < 0)
		return FEDM_ERROR_NOT_CONNECTED;


	char cValue[16];
	int iErr;

#ifdef _FEDM_COM_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
	LPFN_FECOM_GET_PORT_PARA lpfnGetPortPara = (LPFN_FECOM_GET_PORT_PARA)m_pParent->m_pReader->GetFeComFunction(FECOM_GET_PORT_PARA);
	FEDM_CHK3(lpfnGetPortPara);
#endif
#endif

#ifdef _FEDM_USB_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
	LPFN_FEUSB_GET_DEVICE_PARA lpfnGetDevicePara = (LPFN_FEUSB_GET_DEVICE_PARA)m_pParent->m_pReader->GetFeUsbFunction(FEUSB_GET_DEVICE_PARA);
	FEDM_CHK3(lpfnGetDevicePara);
#endif
#endif

#ifdef _FEDM_TCP_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
	LPFN_FETCP_GET_SOCKET_PARA lpfnGetSocketPara = (LPFN_FETCP_GET_SOCKET_PARA)m_pParent->m_pReader->GetFeTcpFunction(FETCP_GET_SOCKET_PARA);
	FEDM_CHK3(lpfnGetSocketPara);
#endif
#endif

	if(FEDM_IS_COMPORT(iPortHnd))
	{
#ifdef _FEDM_COM_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
		FEDM_CHK1(iErr, lpfnGetPortPara(iPortHnd, "Timeout", cValue));
#else
		FEDM_CHK1(iErr, FECOM_GetPortPara(iPortHnd, "Timeout", cValue));
#endif
		sscanf(cValue,"%u", uiTimeout);
#else
		return FEDM_ERROR_UNSUPPORTED;
#endif
	}
	else if(FEDM_IS_TCPPORT(iPortHnd))
	{
#ifdef _FEDM_TCP_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
		FEDM_CHK1(iErr, lpfnGetSocketPara(iPortHnd, "Timeout", cValue));
#else
		FEDM_CHK1(iErr, FETCP_GetSocketPara(iPortHnd, "Timeout", cValue));
#endif
		sscanf(cValue,"%u", uiTimeout);
#else
		return FEDM_ERROR_UNSUPPORTED;
#endif
	}
	else if(FEDM_IS_USBPORT(iPortHnd))
	{
#ifdef _FEDM_USB_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
		FEDM_CHK1(iErr, lpfnGetDevicePara(iPortHnd, "Timeout", cValue));
#else
		FEDM_CHK1(iErr, FEUSB_GetDevicePara(iPortHnd, "Timeout", cValue));
#endif
		sscanf(cValue,"%u", uiTimeout);
#endif
	}
	else
	{
		return FEDM_ERROR_UNSUPPORTED_PORT_DRIVER;
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        		:	10.11.2010 / M. Hultsch

  Version       	:	03.02.07 / 10.11.2010 / M. Hultsch

  Function			:	aets port timeout in port library

  Parameters		:	unsigned int uiTimeout	-	new timeout value

  Return value		:	FEDM_OK or error code (<0)
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto::SetPortTimeout(unsigned int uiTimeout)
{
	char cValue[16];

	int iPortHnd = m_pParent->m_pReader->GetPortHnd();
	if(iPortHnd < 0)
		return FEDM_ERROR_NOT_CONNECTED;


#ifdef _FEDM_COM_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
	LPFN_FECOM_SET_PORT_PARA lpfnSetPortPara = (LPFN_FECOM_SET_PORT_PARA)m_pParent->m_pReader->GetFeComFunction(FECOM_SET_PORT_PARA);
	FEDM_CHK3(lpfnSetPortPara);
#endif
#endif

#ifdef _FEDM_USB_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
	LPFN_FEUSB_SET_DEVICE_PARA lpfnSetDevicePara = (LPFN_FEUSB_SET_DEVICE_PARA)m_pParent->m_pReader->GetFeUsbFunction(FEUSB_SET_DEVICE_PARA);
	FEDM_CHK3(lpfnSetDevicePara);
#endif
#endif

#ifdef _FEDM_TCP_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
	LPFN_FETCP_SET_SOCKET_PARA lpfnSetSocketPara = (LPFN_FETCP_SET_SOCKET_PARA)m_pParent->m_pReader->GetFeTcpFunction(FETCP_SET_SOCKET_PARA);
	FEDM_CHK3(lpfnSetSocketPara);
#endif
#endif

	if(FEDM_IS_COMPORT(iPortHnd))
	{
#ifdef _FEDM_COM_SUPPORT
		sprintf(cValue,"%u", uiTimeout);
#if !defined(_FEDM_SUPPORT_SLINK)
		return lpfnSetPortPara(iPortHnd, "Timeout", cValue);
#else
		return FECOM_SetPortPara(iPortHnd, "Timeout", cValue);
#endif
#else
		return FEDM_ERROR_UNSUPPORTED;
#endif
	}
	else if(FEDM_IS_TCPPORT(iPortHnd))
	{
#ifdef _FEDM_TCP_SUPPORT
		sprintf(cValue,"%u", uiTimeout);
#if !defined(_FEDM_SUPPORT_SLINK)
		return lpfnSetSocketPara(iPortHnd, "Timeout", cValue);
#else
		return FETCP_SetSocketPara(iPortHnd, "Timeout", cValue);
#endif
#else
		return FEDM_ERROR_UNSUPPORTED;
#endif
	}
	else if(FEDM_IS_USBPORT(iPortHnd))
	{
#ifdef _FEDM_USB_SUPPORT
		sprintf(cValue,"%u", uiTimeout);
#if !defined(_FEDM_SUPPORT_SLINK)
		return lpfnSetDevicePara(iPortHnd, "Timeout", cValue);
#else
		return FEUSB_SetDevicePara(iPortHnd, "Timeout", cValue);
#endif
#else
		return FEDM_ERROR_UNSUPPORTED;
#endif
	}

	return FEDM_ERROR_UNSUPPORTED;
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
