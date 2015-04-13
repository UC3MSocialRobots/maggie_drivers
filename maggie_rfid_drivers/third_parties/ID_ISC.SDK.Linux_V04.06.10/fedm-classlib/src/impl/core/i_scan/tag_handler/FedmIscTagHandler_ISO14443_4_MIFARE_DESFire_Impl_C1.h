/*-------------------------------------------------------
|                                                       |
| FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1.h    |
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

Version       		:	03.02.07 / 27.10.2010 / M. Hultsch

Operation Systems	:	independent

Function			:	implementation class for OBID® classic-pro transponder ISO14443-4 MIFARE DESFire

NOTE				:	this class supports only selected mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_4_MIFARE_DESFIRE_IMPL_C1_H_INCLUDED_)
#define _FEDM_ISC_TAG_HANDLER_ISO14443_4_MIFARE_DESFIRE_IMPL_C1_H_INCLUDED_

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include <string>
#include "../../FEDM.h"


class FedmIscTagHandler_ISO14443_4_MIFARE_DESFire;
class FEDM_ISOTabItem;



//#####################################################################################
// constants
//#####################################################################################





//####################################################################
// class FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1
// >> supports only ISO Host Commands <<
//####################################################################

class FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1 //: public FedmIscTagHandler_ISO14443_4
{
public:
	// constructor
	FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1(FEDM_ISOTabItem* pTabItem, FedmIscTagHandler_ISO14443_4_MIFARE_DESFire* pParent);
	// destructor
	virtual ~FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1();

	virtual void Init();					// set all data members to 0

	FEDM_ISOTabItem* m_pTabItem;
	FedmIscTagHandler_ISO14443_4_MIFARE_DESFire* m_pParent;


	//####################################################################
	// 5 security related commands
	
	// [0xFA] : [0xC1] DESFire Commands (Authenticate, AuthenticateISO, AuthenticateAES)
	int Authenticate(		unsigned int uiApplicationID,
							unsigned char ucReaderKeyIndex,
							unsigned char ucDESFireKeyNo );
	
	// [0x54] : [0xC1] DESFire Commands
	int ChangeKeySettings(	unsigned int uiApplicationID,
							unsigned char ucReaderKeyIndex,
							unsigned char ucNewKeySettings );
	
	// [0x45] : T=CL
	int GetKeySettings(		unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );

	// [0xC4] : [0xC1] DESFire Commands
	int ChangeKey(			unsigned int uiApplicationID,
							unsigned char ucReaderKeyIndex,
							unsigned char ucDESFireKeyNo,
							unsigned char ucKeyNoToBeChanged,
							unsigned char ucAESNewKeyVersion,
							std::string sOldKey,
							std::string sNewKey );
	
	// [0x64] : T=CL
	int GetKeyVersion(		unsigned char ucDESFireKeyNo,
							unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );


	//####################################################################
	// 10 PICC level commands

	// [0xCA] : T=CL
	int CreateApplication(	unsigned int uiApplicationID,
							unsigned char ucKeySetting1,
							unsigned char ucKeySetting2,
							unsigned int uiISOFileID,
							std::string sDFName,
							unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );
	
	// [0xDA] : T=CL
	int DeleteApplication(	unsigned int uiApplicationID,
							unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );
	
	// [0x6A] : T=CL
	int GetApplicationIDs(	unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );

	// [0x6E] : T=CL
	int FreeMemory(			unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );

	// [0x6D] : T=CL
	int GetDFNames(			unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );
	
	// [0x5A] : T=CL
	int SelectApplication(	unsigned int uiApplicationID,
							unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );

	// [0xFC] : T=CL
	int FormatPICC(			unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );

	// [0x60] : T=CL
	int GetVersion(			unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );

	// [0x51] : T=CL
	int GetCardUID(			unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );
	
	// [0x5C] : T=CL
	int SetConfiguration(	unsigned char* pucRequestData,
							unsigned int uiRequestLength,
							unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );


	//####################################################################
	// 10 application level commands

	// [0x5F] : [0xC1] DESFire Commands
	int ChangeFileSettings(	unsigned int uiApplicationID,
							unsigned char ucReaderKeyIndex,
							unsigned char ucDESFireKeyNo,
							unsigned char ucFileNo,
							unsigned char ucNewFileCommSettings,
							unsigned char ucNewFileReadWriteAccessRights,
							unsigned char ucNewFileChangeAccessRights,
							unsigned char ucNewFileReadAccessRights,
							unsigned char ucNewFileWriteAccessRights );

	// [0x6F] : T=CL
	int GetFileIDs(			unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );

	// [0xF5] : T=CL
	int GetFileSettings(	unsigned char ucFileNo,
							unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );

	// [0xCD] : T=CL
	int CreateStdDataFile(	unsigned char ucFileNo,
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
							unsigned int& uiResponseLength );

	// [0xCB] : T=CL
	int CreateBackupDataFile(	unsigned char ucFileNo,
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
								unsigned int& uiResponseLength );

	// [0xCC] : T=CL
	int CreateValueFile(	unsigned char ucFileNo,
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
							unsigned int& uiResponseLength );

	// [0xC1] : T=CL
	int CreateLinearRecordFile(	unsigned char ucFileNo,
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
								unsigned int& uiResponseLength );

	// [0xC0] : T=CL
	int CreateCyclicRecordFile(	unsigned char ucFileNo,
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
								unsigned int& uiResponseLength );

	// [0xDF] : T=CL
	int DeleteFile(			unsigned char ucFileNo,
							unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );

	// [0x61] : T=CL
	int GetISOFileIDs(		unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );


	//####################################################################
	// 11 data manipulation commands
	
	// [0xBD] : [0xC1] DESFire Commands
	int ReadStandardData(	unsigned int uiApplicationID,
							unsigned char ucReaderKeyIndex,
							unsigned char ucDESFireKeyNo,
							unsigned char ucFileNo,
							unsigned char ucFileCommSettings,
							unsigned int uiFileOffset,
							unsigned int uiFileLen,
							unsigned char* pucFileData );
	
	// [0x3D] : [0xC1] DESFire Commands
	int WriteStandardData(	unsigned int uiApplicationID,
							unsigned char ucReaderKeyIndex,
							unsigned char ucDESFireKeyNo,
							unsigned char ucFileNo,
							unsigned char ucFileCommSettings,
							unsigned int uiFileOffset,
							unsigned int uiFileLen,
							unsigned char* pucFileData );

	// [0x6C] : [0xC1] DESFire Commands
	int GetValue(			unsigned int uiApplicationID,
							unsigned char ucReaderKeyIndex,
							unsigned char ucDESFireKeyNo,
							unsigned char ucFileNo,
							unsigned char ucFileCommSettings,
							unsigned int& uiValue );

	// [0x0C] : [0xC1] DESFire Commands
	int Credit(				unsigned int uiApplicationID,
							unsigned char ucReaderKeyIndex,
							unsigned char ucDESFireKeyNo,
							unsigned char ucFileNo,
							unsigned char ucFileCommSettings,
							unsigned int uiCreditValue );

	// [0xDC] : [0xC1] DESFire Commands
	int Debit(				unsigned int uiApplicationID,
							unsigned char ucReaderKeyIndex,
							unsigned char ucDESFireKeyNo,
							unsigned char ucFileNo,
							unsigned char ucFileCommSettings,
							unsigned int uiDebitValue );


	// [0x1C] : [0xC1] DESFire Commands
	int LimitedCredit(		unsigned int uiApplicationID,
							unsigned char ucReaderKeyIndex,
							unsigned char ucDESFireKeyNo,
							unsigned char ucFileNo,
							unsigned char ucFileCommSettings,
							unsigned int uiCreditValue );

	// [0x3B] : [0xC1] DESFire Commands
	int WriteRecord(		unsigned int uiApplicationID,
							unsigned char ucReaderKeyIndex,
							unsigned char ucDESFireKeyNo,
							unsigned char ucFileNo,
							unsigned char ucFileCommSettings,
							unsigned int uiFileOffset,
							unsigned int uiRecordLen,
							unsigned char* pucRecordData );

	// [0xBB] : [0xC1] DESFire Commands
	int ReadRecords(		unsigned int uiApplicationID,
							unsigned char ucReaderKeyIndex,
							unsigned char ucDESFireKeyNo,
							unsigned char ucFileNo,
							unsigned char ucFileCommSettings,
							unsigned int uiRecordOffset,
							unsigned int uiNoOfRecords,
							unsigned int uiRecordSize,
							unsigned char* pucRecordData );

	// [0xEB] : [0xC1] DESFire Commands
	int ClearRecordFile(	unsigned int uiApplicationID,
							unsigned char ucReaderKeyIndex,
							unsigned char ucDESFireKeyNo,
							unsigned char ucFileNo,
							unsigned char ucFileCommSettings );

	// [0xC7] : T=CL
	int CommitTransaction(	unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );

	// [0xA7] : T=CL
	int AbortTransaction(	unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );


	unsigned char m_ucErrorSource;
	unsigned int m_uiErrorCode;

};

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#endif // !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_4_MIFARE_DESFIRE_IMPL_H_INCLUDED_)
