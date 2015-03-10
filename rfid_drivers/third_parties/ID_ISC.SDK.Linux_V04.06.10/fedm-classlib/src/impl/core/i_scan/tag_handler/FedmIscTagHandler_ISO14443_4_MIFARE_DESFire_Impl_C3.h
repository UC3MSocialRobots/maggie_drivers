/*-------------------------------------------------------
|                                                       |
| FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3.h |
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

Version       		:	03.02.05 / 08.09.2010 / M. Sahm

Operation Systems	:	independent

Function			:	implementation class for OBID® classic-pro transponder ISO14443-4 MIFARE DESFire

NOTE				:	this class supports only selected mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_4_MIFARE_DESFIRE_IMPL_C3_H_INCLUDED_)
#define _FEDM_ISC_TAG_HANDLER_ISO14443_4_MIFARE_DESFIRE_IMPL_C3_H_INCLUDED_

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include <string>
#include "../../FEDM.h"


class FedmIscTagHandler_ISO14443_4_MIFARE_DESFire;
class FEDM_ISOTabItem;



//#####################################################################################
// constants
//#####################################################################################





//####################################################################
// class FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3
// >> supports only ISO Host Commands <<
//####################################################################

class FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3 //: public FedmIscTagHandler_ISO14443_4
{
public:
	// constructor
	FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3(FEDM_ISOTabItem* pTabItem, FedmIscTagHandler_ISO14443_4_MIFARE_DESFire* pParent);
	// destructor
	virtual ~FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3();

	virtual void Init();					// set all data members to 0

	FEDM_ISOTabItem* m_pTabItem;
	FedmIscTagHandler_ISO14443_4_MIFARE_DESFire* m_pParent;


	//####################################################################
	// 5 security related commands
	
	// [0xFA] : [0xC3] DESFire Commands (Authenticate, AuthenticateISO, AuthenticateAES)
	int Authenticate(		unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucReaderKeyIndex,
							unsigned char ucDESFireAuthMode,
							unsigned char ucDESFireKeyNo,
							unsigned char ucSamAuthMode,
							unsigned char ucSamKeyNo,
							unsigned char ucSamKeyVersion,
							unsigned char ucSamDivInputLen,
							unsigned char* pucDivInput );
	
	// [0x54] : [0xC3] DESFire Commands
	int ChangeKeySettings(	unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucNewKeySettings );
	
	// [0x45] : [0xC3] DESFire Commands
	int GetKeySettings(		unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );

	// [0xC4] : [0xC3] DESFire Commands
	int ChangeKey(			unsigned char ucMode,
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
							unsigned char* pucDivInput );
	
	// [0x64] : [0xC3] DESFire Commands
	int GetKeyVersion(		unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucDESFireKeyNo,
							unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );


	//####################################################################
	// 10 PICC level commands

	// [0xCA] : [0xC3] DESFire Commands
	int CreateApplication(	unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucPICCLevelFlags,
							unsigned int uiApplicationID,
							unsigned char ucKeySetting1,
							unsigned char ucKeySetting2,
							unsigned int uiISOFileID,
							std::string sDFName );
	
	// [0xDA] : [0xC3] DESFire Commands
	int DeleteApplication(	unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucPICCLevelFlags,
							unsigned int uiApplicationID );
	
	// [0x6A] : [0xC3] DESFire Commands
	int GetApplicationIDs(	unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucPICCLevelFlags,
							unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );

	// [0x6E] : [0xC3] DESFire Commands
	int FreeMemory(			unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucPICCLevelFlags,
							unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength);

	// [0x6D] : [0xC3] DESFire Commands
	int GetDFNames(			unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucPICCLevelFlags,
							unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );
	
	// [0x5A] : [0xC3] DESFire Commands
	int SelectApplication(	unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucPICCLevelFlags,
							unsigned int uiApplicationID );

	// [0xFC] : [0xC3] DESFire Commands
	int FormatPICC(			unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucPICCLevelFlags );

	// [0x60] : [0xC3] DESFire Commands
	int GetVersion(			unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucPICCLevelFlags,
							unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );

	// [0x51] : [0xC3] DESFire Commands
	int GetCardUID(			unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucPICCLevelFlags,
							unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );
	
	// [0x5C] : [0xC3] DESFire Commands
	int SetConfiguration(	unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucPICCLevelFlags,
							unsigned char* pucRequestData,
							unsigned int uiRequestLength );


	//####################################################################
	// 10 application level commands

	// [0x5F] : [0xC3] DESFire Commands
	int ChangeFileSettings(	unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucApplicationLevelFlags,
							unsigned char ucFileNo,
							unsigned char ucNewFileCommSettings,
							unsigned char ucNewFileReadWriteAccessRights,
							unsigned char ucNewFileChangeAccessRights,
							unsigned char ucNewFileReadAccessRights,
							unsigned char ucNewFileWriteAccessRights );

	// [0x6F] : [0xC3] DESFire Commands
	int GetFileIDs(			unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucApplicationLevelFlags,
							unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );

	// [0xF5] : [0xC3] DESFire Commands
	int GetFileSettings(	unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucApplicationLevelFlags,
							unsigned char ucFileNo,
							unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );

	// [0xCD] : [0xC3] DESFire Commands
	int CreateStdDataFile(	unsigned char ucMode,
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
							unsigned int uiFileSize );

	// [0xCB] : [0xC3] DESFire Commands
	int CreateBackupDataFile(	unsigned char ucMode,
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
								unsigned int uiFileSize );

	// [0xCC] : [0xC3] DESFire Commands
	int CreateValueFile(	unsigned char ucMode,
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
							unsigned char ucLimitedCreditEnabled );

	// [0xC1] : [0xC3] DESFire Commands
	int CreateLinearRecordFile(	unsigned char ucMode,
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
								unsigned int uiMaxNoOfRecords );

	// [0xC0] : [0xC3] DESFire Commands
	int CreateCyclicRecordFile(	unsigned char ucMode,
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
								unsigned int uiMaxNoOfRecords );

	// [0xDF] : [0xC3] DESFire Commands
	int DeleteFile(			unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucApplicationLevelFlags,
							unsigned char ucFileNo );

	// [0x61] : [0xC3] DESFire Commands
	int GetISOFileIDs(		unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucApplicationLevelFlags,
							unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );


	//####################################################################
	// 11 data manipulation commands
	
	// [0xBD] : [0xC3] DESFire Commands
	int ReadStandardData(	unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucDataManipulationFlags,
							unsigned char ucFileNo,
							unsigned char ucFileCommSettings,
							unsigned int uiFileOffset,
							unsigned int uiFileLen,
							unsigned char* pucFileData );
	
	// [0x3D] : [0xC3] DESFire Commands
	int WriteStandardData(	unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucDataManipulationFlags,
							unsigned char ucFileNo,
							unsigned char ucFileCommSettings,
							unsigned int uiFileOffset,
							unsigned int uiFileLen,
							unsigned char* pucFileData );

	// [0x6C] : [0xC3] DESFire Commands
	int GetValue(			unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucDataManipulationFlags,
							unsigned char ucFileNo,
							unsigned char ucFileCommSettings,
							unsigned int& uiValue );

	// [0x0C] : [0xC3] DESFire Commands
	int Credit(				unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucDataManipulationFlags,
							unsigned char ucFileNo,
							unsigned char ucFileCommSettings,
							unsigned int uiCreditValue );

	// [0xDC] : [0xC3] DESFire Commands
	int Debit(				unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucDataManipulationFlags,
							unsigned char ucFileNo,
							unsigned char ucFileCommSettings,
							unsigned int uiDebitValue );


	// [0x1C] : [0xC3] DESFire Commands
	int LimitedCredit(		unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucDataManipulationFlags,
							unsigned char ucFileNo,
							unsigned char ucFileCommSettings,
							unsigned int uiCreditValue );

	// [0x3B] : [0xC3] DESFire Commands
	int WriteRecord(		unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucDataManipulationFlags,
							unsigned char ucFileNo,
							unsigned char ucFileCommSettings,
							unsigned int uiFileOffset,
							unsigned int uiRecordLen,
							unsigned char* pucRecordData );

	// [0xBB] : [0xC3] DESFire Commands
	int ReadRecords(		unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucDataManipulationFlags,
							unsigned char ucFileNo,
							unsigned char ucFileCommSettings,
							unsigned int uiRecordOffset,
							unsigned int uiNoOfRecords,
							unsigned int uiRecordSize,
							unsigned char* pucRecordData );

	// [0xEB] : [0xC3] DESFire Commands
	int ClearRecordFile(	unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucDataManipulationFlags,
							unsigned char ucFileNo,
							unsigned char ucFileCommSettings );

	// [0xC7] : [0xC3] DESFire Commands
	int CommitTransaction(	unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucDataManipulationFlags );

	// [0xA7] : [0xC3] DESFire Commands
	int AbortTransaction(	unsigned char ucMode,
							unsigned char ucSlot,
							unsigned char ucTimeout,
							unsigned char ucDataManipulationFlags );


	unsigned char m_ucErrorSource;
	unsigned int m_uiErrorCode;

};

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#endif // !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_4_MIFARE_DESFIRE_IMPL_C3_H_INCLUDED_)
