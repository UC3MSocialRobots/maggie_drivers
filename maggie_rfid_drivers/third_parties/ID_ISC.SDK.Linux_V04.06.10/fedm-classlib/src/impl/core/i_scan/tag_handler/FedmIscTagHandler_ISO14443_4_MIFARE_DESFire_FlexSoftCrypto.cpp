/*-----------------------------------------------------------------------
|                                                       				|
|      FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto.cpp	|
|                                                       				|
-------------------------------------------------------------------------

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

Version       		:	03.02.07 / 10.11.2010 / M. Hultsch

Operation Systems	:	independent

Function			:	implementation class for OBID® classic-pro transponder ISO14443-4 MIFARE DESFire
						with [0xC3] commands and fast software cryptographic

NOTE				:	this class supports only selected mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "../../FEDM.h"
#include "FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto.h"
#include "FedmIscTagHandler_ISO14443_4_MIFARE_DESFire.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto()
{
}

FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::~FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto()
{
}

unsigned char FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::GetErrorSource()
{
	return m_pParent->m_pImpl_C3->m_ucErrorSource;
}

unsigned int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::GetErrorCode()
{
	return m_pParent->m_pImpl_C3->m_uiErrorCode;
}


//---------------------------------------------------------------------------------
// ------------------------ SECURITY RELATED COMMANDS -----------------------------
//---------------------------------------------------------------------------------
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::Authenticate(
	unsigned char ucReaderKeyIndex,
	unsigned char ucDESFireKeyNo )
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->Authenticate(0x00, RFU, RFU, ucReaderKeyIndex, RFU, ucDESFireKeyNo, RFU, RFU, RFU, RFU, NULL);
}


int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::ChangeKeySettings(unsigned char ucNewKeySettings )
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->ChangeKeySettings(0x00, RFU, RFU, ucNewKeySettings);
}


int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::GetKeySettings(
	unsigned char *pucResponseData, 
	unsigned int uiResponseBufferLength, 
	unsigned int &uiResponseLength )
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->GetKeySettings(0x00, RFU, RFU, pucResponseData, uiResponseBufferLength, uiResponseLength);
}


int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::ChangeKey(
	unsigned char ucKeyNoToBeChanged,
	unsigned char ucAESNewKeyVersion,
	std::string sOldKey,
	std::string sNewKey )
{
	unsigned char ucRFU = 0x00;
	return m_pParent->m_pImpl_C3->ChangeKey(0x00, 
											ucRFU, 
											ucRFU, 
											ucKeyNoToBeChanged, 
											ucAESNewKeyVersion, 
											sOldKey, 
											sNewKey, 
											ucRFU, 
											ucRFU, 
											ucRFU, 
											ucRFU, 
											ucRFU, 
											ucRFU, 
											ucRFU, 
											NULL);
}


int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::GetKeyVersion(
	unsigned char ucDESFireKeyNo, 
	unsigned char *pucResponseData, 
	unsigned int uiResponseBufferLength, 
	unsigned int &uiResponseLength)
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->GetKeyVersion(0x00, RFU, RFU, ucDESFireKeyNo, pucResponseData, uiResponseBufferLength, uiResponseLength);
}

//---------------------------------------------------------------------------------
// ------------------------ PICC LEVEL COMMANDS -----------------------------------
//---------------------------------------------------------------------------------
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::CreateApplication(
	unsigned char ucPiccLevelFlags, 
	unsigned int uiApplicationID, 
	unsigned char ucKeySetting1, 
	unsigned char ucKeySetting2, 
	unsigned int uiISOFileID, 
	std::string sDFName )
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->CreateApplication(0x00, RFU, RFU, ucPiccLevelFlags, uiApplicationID, ucKeySetting1, ucKeySetting2, uiISOFileID, sDFName);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::DeleteApplication(
	unsigned char ucPiccLevelFlags,
	unsigned int uiApplicationID )
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->DeleteApplication(0x00, RFU, RFU, ucPiccLevelFlags, uiApplicationID);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::GetApplicationIDs(
	unsigned char ucPiccLevelFlags, 
	unsigned char *pucResponseData, 
	unsigned int uiResponseBufferLength, 
	unsigned int &uiResponseLength)
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->GetApplicationIDs(0x00, RFU, RFU, ucPiccLevelFlags, pucResponseData, uiResponseBufferLength, uiResponseLength);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::FreeMemory( 
	unsigned char ucPiccLevelFlags,
	unsigned char* pucResponseData, 
	unsigned int uiResponseBufferLength, 
	unsigned int &uiResponseLength )
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->FreeMemory(0x00, RFU, RFU, ucPiccLevelFlags, pucResponseData, uiResponseBufferLength, uiResponseLength);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::GetDFNames(
	unsigned char ucPiccLevelFlags, 
	unsigned char *pucResponseData, 
	unsigned int uiResponseBufferLength, 
	unsigned int &uiResponseLength)
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->GetDFNames(0x00, RFU, RFU, ucPiccLevelFlags, pucResponseData, uiResponseBufferLength, uiResponseLength);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::SelectApplication(
	unsigned char ucPiccLevelFlags, 
	unsigned int uiApplicationID)
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->SelectApplication(0x00, RFU, RFU, ucPiccLevelFlags, uiApplicationID);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::FormatPICC(	unsigned char ucPiccLevelFlags )
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->FormatPICC(0x00, RFU, RFU, ucPiccLevelFlags);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::GetVersion(
	unsigned char ucPiccLevelFlags, 
	unsigned char *pucResponseData, 
	unsigned int uiResponseBufferLength, 
	unsigned int &uiResponseLength )
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->GetVersion(0x00, RFU, RFU, ucPiccLevelFlags, pucResponseData, uiResponseBufferLength, uiResponseLength);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::GetCardUID(
	unsigned char ucPiccLevelFlags, 
	unsigned char *pucResponseData, 
	unsigned int uiResponseBufferLength, 
	unsigned int &uiResponseLength )
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->GetCardUID(0x00, RFU, RFU, ucPiccLevelFlags, pucResponseData, uiResponseBufferLength, uiResponseLength);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::SetConfiguration(
	unsigned char ucPiccLevelFlags, 
	unsigned char *pucRequestData, 
	unsigned int uiRequestLength )
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->SetConfiguration(0x00, RFU, RFU, ucPiccLevelFlags, pucRequestData, uiRequestLength);
}

//---------------------------------------------------------------------------------
// ------------------------ APPLICATION LEVEL COMMANDS ----------------------------
//---------------------------------------------------------------------------------
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::ChangeFileSettings(
	unsigned char ucApplicationLevelFlags, 
	unsigned char ucFileNo, 
	unsigned char ucNewFileCommSettings, 
	unsigned char ucNewFileReadWriteAccessRights, 
	unsigned char ucNewFileChangeAccessRights, 
	unsigned char ucNewFileReadAccessRights, 
	unsigned char ucNewFileWriteAccessRights)
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->ChangeFileSettings(0x00, RFU, RFU, ucApplicationLevelFlags, ucFileNo, ucNewFileCommSettings, ucNewFileReadWriteAccessRights, ucNewFileChangeAccessRights, ucNewFileReadAccessRights, ucNewFileWriteAccessRights);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::GetFileIDs(
	unsigned char ucApplicationLevelFlags, 
	unsigned char *pucResponseData, 
	unsigned int uiResponseBufferLength, 
	unsigned int &uiResponseLength )
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->GetFileIDs(0x00, RFU, RFU, ucApplicationLevelFlags, pucResponseData, uiResponseBufferLength, uiResponseLength);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::GetFileSettings(
	unsigned char ucApplicationLevelFlags, 
	unsigned char ucFileNo, 
	unsigned char *pucResponseData, 
	unsigned int uiResponseBufferLength, 
	unsigned int &uiResponseLength )
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->GetFileSettings(0x00, RFU, RFU, ucApplicationLevelFlags, ucFileNo, pucResponseData, uiResponseBufferLength, uiResponseLength);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::CreateStdDataFile(
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
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->CreateStdDataFile(0x00, RFU, RFU, ucApplicationLevelFlags, ucFileNo, uiISOFileID, ucFileCommSettings, ucFileReadWriteAccessRights, ucFileChangeAccessRights, ucFileReadAccessRights, ucFileWriteAccessRights, uiFileSize);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::CreateBackupDataFile(
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
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->CreateBackupDataFile(0x00, RFU, RFU, ucApplicationLevelFlags, ucFileNo, uiISOFileID, ucFileCommSettings, ucFileReadWriteAccessRights, ucFileChangeAccessRights, ucFileReadAccessRights, ucFileWriteAccessRights, uiFileSize);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::CreateValueFile(
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
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->CreateValueFile(0x00, RFU, RFU, ucApplicationLevelFlags, ucFileNo, ucFileCommSettings, ucFileReadWriteAccessRights, ucFileChangeAccessRights, ucFileReadAccessRights, ucFileWriteAccessRights, uiLowerLimit, uiUpperLimit, uiValue, ucLimitedCreditEnabled);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::CreateLinearRecordFile(
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
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->CreateLinearRecordFile(0x00, RFU, RFU, ucApplicationLevelFlags, ucFileNo, uiISOFileID, ucFileCommSettings, ucFileReadWriteAccessRights, ucFileChangeAccessRights, ucFileReadAccessRights, ucFileWriteAccessRights, uiRecordSize, uiMaxNoOfRecords);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::CreateCyclicRecordFile(
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
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->CreateCyclicRecordFile(0x00, RFU, RFU, ucApplicationLevelFlags, ucFileNo, uiISOFileID, ucFileCommSettings, ucFileReadWriteAccessRights, ucFileChangeAccessRights, ucFileReadAccessRights, ucFileWriteAccessRights, uiRecordSize, uiMaxNoOfRecords);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::DeleteFile(
	unsigned char ucApplicationLevelFlags, 
	unsigned char ucFileNo )
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->DeleteFile(0x00, RFU, RFU, ucApplicationLevelFlags, ucFileNo);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::GetISOFileIDs(
	unsigned char ucApplicationLevelFlags, 
	unsigned char *pucResponseData, 
	unsigned int uiResponseBufferLength, 
	unsigned int &uiResponseLength )
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->GetISOFileIDs(0x00, RFU, RFU, ucApplicationLevelFlags, pucResponseData, uiResponseBufferLength, uiResponseLength);
}

//---------------------------------------------------------------------------------
// ------------------------ DATA MANIPULATION COMMANDS ----------------------------
//---------------------------------------------------------------------------------
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::ReadStandardData(
	unsigned char ucDataManipulationFlags, 
	unsigned char ucFileNo, 
	unsigned char ucFileCommSettings, 
	unsigned int uiFileOffset, 
	unsigned int uiFileLen, 
	unsigned char *pucFileData)
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->ReadStandardData(0x00, RFU, RFU, ucDataManipulationFlags, ucFileNo, ucFileCommSettings, uiFileOffset, uiFileLen, pucFileData);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::WriteStandardData(
	unsigned char ucDataManipulationFlags, 
	unsigned char ucFileNo, 
	unsigned char ucFileCommSettings, 
	unsigned int uiFileOffset, 
	unsigned int uiFileLen, 
	unsigned char *pucFileData)
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->WriteStandardData(0x00, RFU, RFU, ucDataManipulationFlags, ucFileNo, ucFileCommSettings, uiFileOffset, uiFileLen, pucFileData);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::GetValue(
	unsigned char ucDataManipulationFlags, 
	unsigned char ucFileNo, 
	unsigned char ucFileCommSettings, 
	unsigned int &uiValue)
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->GetValue(0x00, RFU, RFU, ucDataManipulationFlags, ucFileNo, ucFileCommSettings, uiValue);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::Debit(
	unsigned char ucDataManipulationFlags, 
	unsigned char ucFileNo, 
	unsigned char ucFileCommSettings, 
	unsigned int uiDebitValue)
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->Debit(0x00, RFU, RFU, ucDataManipulationFlags, ucFileNo, ucFileCommSettings, uiDebitValue);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::Credit(
	unsigned char ucDataManipulationFlags, 
	unsigned char ucFileNo, 
	unsigned char ucFileCommSettings, 
	unsigned int uiCreditValue)
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->Credit(0x00, RFU, RFU, ucDataManipulationFlags, ucFileNo, ucFileCommSettings, uiCreditValue);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::LimitedCredit(
	unsigned char ucDataManipulationFlags, 
	unsigned char ucFileNo, 
	unsigned char ucFileCommSettings, 
	unsigned int uiCreditValue)
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->LimitedCredit(0x00, RFU, RFU, ucDataManipulationFlags, ucFileNo, ucFileCommSettings, uiCreditValue);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::WriteRecord(
	unsigned char ucDataManipulationFlags, 
	unsigned char ucFileNo, 
	unsigned char ucFileCommSettings, 
	unsigned int uiFileOffset, 
	unsigned int uiRecordLen, 
	unsigned char *pucRecordData)
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->WriteRecord(0x00, RFU, RFU, ucDataManipulationFlags, ucFileNo, ucFileCommSettings, uiFileOffset, uiRecordLen, pucRecordData);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::ReadRecords(
	unsigned char ucDataManipulationFlags, 
	unsigned char ucFileNo, 
	unsigned char ucFileCommSettings, 
	unsigned int uiRecordOffset, 
	unsigned int uiNoOfRecords, 
	unsigned int uiRecordSize, 
	unsigned char *pucRecordData)
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->ReadRecords(0x00, RFU, RFU, ucDataManipulationFlags, ucFileNo, ucFileCommSettings, uiRecordOffset, uiNoOfRecords, uiRecordSize, pucRecordData);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::ClearRecordFile(
	unsigned char ucDataManipulationFlags,
	unsigned char ucFileNo,
	unsigned char ucFileCommSettings)
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->ClearRecordFile(0x00, RFU, RFU, ucDataManipulationFlags, ucFileNo, ucFileCommSettings);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::CommitTransaction( unsigned char ucDataManipulationFlags )
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->CommitTransaction(0x00, RFU, RFU, ucDataManipulationFlags);
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto::AbortTransaction( unsigned char ucDataManipulationFlags )
{
	unsigned char RFU = 0x00;
	return m_pParent->m_pImpl_C3->AbortTransaction(0x00, RFU, RFU, ucDataManipulationFlags);
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
