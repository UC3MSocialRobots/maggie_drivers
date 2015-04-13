/*-------------------------------------------------------------------
|                                                       			|
|      FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto.cpp	|
|                                                       			|
---------------------------------------------------------------------

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

Version       		:	03.02.07 / 27.10.2010 / M. Hultsch

Operation Systems	:	independent

Function			:	implementation class for OBID® classic-pro transponder ISO14443-4 MIFARE DESFire
						with [0xC1] commands

NOTE				:	this class supports only selected mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "../../FEDM.h"
#include "FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto.h"
#include "FedmIscTagHandler_ISO14443_4_MIFARE_DESFire.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto()
{
}

FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::~FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto()
{
}

unsigned char FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::GetErrorSource()
{
	return m_pParent->m_pImpl_C1->m_ucErrorSource;
}

unsigned int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::GetErrorCode()
{
	return m_pParent->m_pImpl_C1->m_uiErrorCode;
}

int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::Authenticate(	unsigned int uiApplicationID,
																			unsigned char ucReaderKeyIndex,
																			unsigned char ucDESFireKeyNo )
{
	return m_pParent->m_pImpl_C1->Authenticate(uiApplicationID, 
											ucReaderKeyIndex, 
											ucDESFireKeyNo );
}


	//####################################################################
	// 5 security related commands
	
	// [0x54] : [0xC1] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::ChangeKeySettings(
	unsigned int uiApplicationID,
	unsigned char ucReaderKeyIndex,
	unsigned char ucNewKeySettings )
{
	return m_pParent->m_pImpl_C1->ChangeKeySettings(uiApplicationID, 
													ucReaderKeyIndex, 
													ucNewKeySettings );
}

	// [0x45] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::GetKeySettings(
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	return m_pParent->m_pImpl_C1->GetKeySettings(	pucResponseData, 
												uiResponseBufferLength, 
												uiResponseLength );
}

	// [0xC4] : [0xC1] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::ChangeKey(
	unsigned int uiApplicationID,
	unsigned char ucReaderKeyIndex,
	unsigned char ucDESFireKeyNo,
	unsigned char ucKeyNoToBeChanged,
	unsigned char ucAESNewKeyVersion,
	string sOldKey,
	string sNewKey )
{
	return m_pParent->m_pImpl_C1->ChangeKey(	uiApplicationID, 
											ucReaderKeyIndex, 
											ucDESFireKeyNo, 
											ucKeyNoToBeChanged, 
											ucAESNewKeyVersion, 
											sOldKey,
											sNewKey );
}

	// [0x64] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::GetKeyVersion(
	unsigned char ucDESFireKeyNo,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	return m_pParent->m_pImpl_C1->GetKeyVersion(	ucDESFireKeyNo,
												pucResponseData, 
												uiResponseBufferLength, 
												uiResponseLength );
}


	//####################################################################
	// 10 PICC level commands

	// [0xCA] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::CreateApplication(
	unsigned int uiApplicationID,
	unsigned char ucKeySetting1,
	unsigned char ucKeySetting2,
	unsigned int uiISOFileID,
	string sDFName,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	return m_pParent->m_pImpl_C1->CreateApplication(	uiApplicationID,
													ucKeySetting1,
													ucKeySetting2,
													uiISOFileID,
													sDFName,
													pucResponseData,
													uiResponseBufferLength,
													uiResponseLength );
}

	// [0xDA] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::DeleteApplication(
	unsigned int uiApplicationID,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	return m_pParent->m_pImpl_C1->DeleteApplication(	uiApplicationID,
													pucResponseData, 
													uiResponseBufferLength, 
													uiResponseLength );
}

	// [0x6A] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::GetApplicationIDs(
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	return m_pParent->m_pImpl_C1->GetApplicationIDs(	pucResponseData, 
													uiResponseBufferLength, 
													uiResponseLength );
}

	// [0x6E] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::FreeMemory(
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	return m_pParent->m_pImpl_C1->FreeMemory(	pucResponseData, 
											uiResponseBufferLength, 
											uiResponseLength );
}

	// [0x6D] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::GetDFNames(
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	return m_pParent->m_pImpl_C1->GetDFNames(	pucResponseData, 
											uiResponseBufferLength, 
											uiResponseLength );
}

	// [0x5A] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::SelectApplication(
	unsigned int uiApplicationID,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	return m_pParent->m_pImpl_C1->SelectApplication(	uiApplicationID, 
													pucResponseData, 
													uiResponseBufferLength, 
													uiResponseLength );
}

	// [0xFC] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::FormatPICC(
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	return m_pParent->m_pImpl_C1->FormatPICC(	pucResponseData, 
											uiResponseBufferLength, 
											uiResponseLength );
}

	// [0x60] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::GetVersion(
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	return m_pParent->m_pImpl_C1->GetVersion(	pucResponseData, 
											uiResponseBufferLength, 
											uiResponseLength );
}

	// [0x51] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::GetCardUID(
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	return m_pParent->m_pImpl_C1->GetCardUID(	pucResponseData,
											uiResponseBufferLength,
											uiResponseLength );
}

	// [0x5C] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::SetConfiguration(
	unsigned char* pucRequestData,
	unsigned int uiRequestLength,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	return m_pParent->m_pImpl_C1->SetConfiguration(pucRequestData,
												uiRequestLength, 
												pucResponseData, 
												uiResponseBufferLength,
												uiResponseLength );
}


	//####################################################################
	// 10 application level commands

	// [0x5F] : [0xC1] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::ChangeFileSettings(
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
	return m_pParent->m_pImpl_C1->ChangeFileSettings(	uiApplicationID, 
													ucReaderKeyIndex, 
													ucDESFireKeyNo, 
													ucFileNo,
													ucNewFileCommSettings,
													ucNewFileReadWriteAccessRights,
													ucNewFileChangeAccessRights,
													ucNewFileReadAccessRights,
													ucNewFileWriteAccessRights );
}

	// [0x6F] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::GetFileIDs(
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	return m_pParent->m_pImpl_C1->GetFileIDs(	pucResponseData,
											uiResponseBufferLength,
											uiResponseLength );
}

	// [0xF5] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::GetFileSettings(
	unsigned char ucFileNo,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	return m_pParent->m_pImpl_C1->GetFileSettings(	ucFileNo,
												pucResponseData, 
												uiResponseBufferLength, 
												uiResponseLength );
}

	// [0xCD] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::CreateStdDataFile(
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
	return m_pParent->m_pImpl_C1->CreateStdDataFile(	ucFileNo,
													bWithISOFileID,
													uiISOFileID,
													ucFileCommSettings,
													ucFileReadWriteAccessRights,
													ucFileChangeAccessRights,
													ucFileReadAccessRights,
													ucFileWriteAccessRights,
													uiFileSize,
													pucResponseData,
													uiResponseBufferLength,
													uiResponseLength );
}

	// [0xCB] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::CreateBackupDataFile(
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
	return m_pParent->m_pImpl_C1->CreateBackupDataFile(ucFileNo,
													bWithISOFileID,
													uiISOFileID,
													ucFileCommSettings,
													ucFileReadWriteAccessRights,
													ucFileChangeAccessRights,
													ucFileReadAccessRights,
													ucFileWriteAccessRights,
													uiFileSize,
													pucResponseData,
													uiResponseBufferLength,
													uiResponseLength );
}

	// [0xCC] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::CreateValueFile(
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
	return m_pParent->m_pImpl_C1->CreateValueFile(	ucFileNo,
												ucFileCommSettings,
												ucFileReadWriteAccessRights,
												ucFileChangeAccessRights,
												ucFileReadAccessRights,
												ucFileWriteAccessRights,
												uiLowerLimit,
												uiUpperLimit,
												uiValue,
												ucLimitedCreditEnabled,
												pucResponseData,
												uiResponseBufferLength,
												uiResponseLength );
}

	// [0xC1] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::CreateLinearRecordFile(
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
	return m_pParent->m_pImpl_C1->CreateLinearRecordFile(	ucFileNo,
														bWithISOFileID,
														uiISOFileID,
														ucFileCommSettings,
														ucFileReadWriteAccessRights,
														ucFileChangeAccessRights,
														ucFileReadAccessRights,
														ucFileWriteAccessRights,
														uiRecordSize,
														uiMaxNoOfRecords,
														pucResponseData,
														uiResponseBufferLength,
														uiResponseLength );
}

	// [0xC0] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::CreateCyclicRecordFile(
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
	return m_pParent->m_pImpl_C1->CreateCyclicRecordFile(	ucFileNo,
														bWithISOFileID,
														uiISOFileID,
														ucFileCommSettings,
														ucFileReadWriteAccessRights,
														ucFileChangeAccessRights,
														ucFileReadAccessRights,
														ucFileWriteAccessRights,
														uiRecordSize,
														uiMaxNoOfRecords,
														pucResponseData,
														uiResponseBufferLength,
														uiResponseLength );
}

	// [0xDF] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::DeleteFile(
	unsigned char ucFileNo,
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	return m_pParent->m_pImpl_C1->DeleteFile(	ucFileNo, 
											pucResponseData,
											uiResponseBufferLength,
											uiResponseLength );
}

	// [0x61] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::GetISOFileIDs(
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	return m_pParent->m_pImpl_C1->GetISOFileIDs(	pucResponseData, 
												uiResponseBufferLength, 
												uiResponseLength );
}


	//####################################################################
	// 11 data manipulation commands
	
	// [0xBD] : [0xC1] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::ReadStandardData(
	unsigned int uiApplicationID,
	unsigned char ucReaderKeyIndex,
	unsigned char ucDESFireKeyNo,
	unsigned char ucFileNo,
	unsigned char ucFileCommSettings,
	unsigned int uiFileOffset,
	unsigned int uiFileLen,
	unsigned char* pucFileData )
{
	return m_pParent->m_pImpl_C1->ReadStandardData(uiApplicationID,
												ucReaderKeyIndex,
												ucDESFireKeyNo,
												ucFileNo,
												ucFileCommSettings,
												uiFileOffset,
												uiFileLen,
												pucFileData );
}
	
	// [0x3D] : [0xC1] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::WriteStandardData(
	unsigned int uiApplicationID,
	unsigned char ucReaderKeyIndex,
	unsigned char ucDESFireKeyNo,
	unsigned char ucFileNo,
	unsigned char ucFileCommSettings,
	unsigned int uiFileOffset,
	unsigned int uiFileLen,
	unsigned char* pucFileData )
{
	return m_pParent->m_pImpl_C1->WriteStandardData(	uiApplicationID,
													ucReaderKeyIndex,
													ucDESFireKeyNo,
													ucFileNo,
													ucFileCommSettings,
													uiFileOffset,
													uiFileLen,
													pucFileData );
}

	// [0x6C] : [0xC1] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::GetValue(
	unsigned int uiApplicationID,
	unsigned char ucReaderKeyIndex,
	unsigned char ucDESFireKeyNo,
	unsigned char ucFileNo,
	unsigned char ucFileCommSettings,
	unsigned int& uiValue )
{
	return m_pParent->m_pImpl_C1->GetValue(uiApplicationID,
										ucReaderKeyIndex,
										ucDESFireKeyNo,
										ucFileNo,
										ucFileCommSettings,
										uiValue );
}

	// [0x0C] : [0xC1] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::Credit(
	unsigned int uiApplicationID,
	unsigned char ucReaderKeyIndex,
	unsigned char ucDESFireKeyNo,
	unsigned char ucFileNo,
	unsigned char ucFileCommSettings,
	unsigned int uiCreditValue )
{
	return m_pParent->m_pImpl_C1->Credit(	uiApplicationID,
										ucReaderKeyIndex,
										ucDESFireKeyNo,
										ucFileNo,
										ucFileCommSettings,
										uiCreditValue );
}

	// [0xDC] : [0xC1] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::Debit(
	unsigned int uiApplicationID,
	unsigned char ucReaderKeyIndex,
	unsigned char ucDESFireKeyNo,
	unsigned char ucFileNo,
	unsigned char ucFileCommSettings,
	unsigned int uiDebitValue )
{
	return m_pParent->m_pImpl_C1->Debit(	uiApplicationID,
										ucReaderKeyIndex,
										ucDESFireKeyNo,
										ucFileNo,
										ucFileCommSettings,
										uiDebitValue );
}


	// [0x1C] : [0xC1] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::LimitedCredit(
	unsigned int uiApplicationID,
	unsigned char ucReaderKeyIndex,
	unsigned char ucDESFireKeyNo,
	unsigned char ucFileNo,
	unsigned char ucFileCommSettings,
	unsigned int uiCreditValue )
{
	return m_pParent->m_pImpl_C1->LimitedCredit(	uiApplicationID,
												ucReaderKeyIndex,
												ucDESFireKeyNo,
												ucFileNo,
												ucFileCommSettings,
												uiCreditValue );
}

	// [0x3B] : [0xC1] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::WriteRecord(
	unsigned int uiApplicationID,
	unsigned char ucReaderKeyIndex,
	unsigned char ucDESFireKeyNo,
	unsigned char ucFileNo,
	unsigned char ucFileCommSettings,
	unsigned int uiFileOffset,
	unsigned int uiRecordLen,
	unsigned char* pucRecordData )
{
	return m_pParent->m_pImpl_C1->WriteRecord(	uiApplicationID,
											ucReaderKeyIndex,
											ucDESFireKeyNo,
											ucFileNo,
											ucFileCommSettings,
											uiFileOffset,
											uiRecordLen,
											pucRecordData );
}

	// [0xBB] : [0xC1] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::ReadRecords(
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
	return m_pParent->m_pImpl_C1->ReadRecords(	uiApplicationID,
											ucReaderKeyIndex,
											ucDESFireKeyNo,
											ucFileNo,
											ucFileCommSettings,
											uiRecordOffset,
											uiNoOfRecords,
											uiRecordSize,
											pucRecordData );
}

	// [0xEB] : [0xC1] DESFire Commands
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::ClearRecordFile(
	unsigned int uiApplicationID,
	unsigned char ucReaderKeyIndex,
	unsigned char ucDESFireKeyNo,
	unsigned char ucFileNo,
	unsigned char ucFileCommSettings )
{
	return m_pParent->m_pImpl_C1->ClearRecordFile(	uiApplicationID,
												ucReaderKeyIndex,
												ucDESFireKeyNo,
												ucFileNo,
												ucFileCommSettings );
}

	// [0xC7] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::CommitTransaction(
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	return m_pParent->m_pImpl_C1->CommitTransaction(	pucResponseData, 
													uiResponseBufferLength, 
													uiResponseLength );
}

	// [0xA7] : T=CL
int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto::AbortTransaction(
	unsigned char* pucResponseData,
	unsigned int uiResponseBufferLength,
	unsigned int& uiResponseLength )
{
	return m_pParent->m_pImpl_C1->AbortTransaction(	pucResponseData, 
													uiResponseBufferLength, 
													uiResponseLength );
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
