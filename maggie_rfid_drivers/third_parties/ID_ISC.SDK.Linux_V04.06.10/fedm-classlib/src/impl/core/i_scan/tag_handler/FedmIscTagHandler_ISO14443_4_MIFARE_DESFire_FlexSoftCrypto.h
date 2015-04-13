/*-----------------------------------------------------------------------
|                                                       				|
|      FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto.h		|
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
						with [0xC3] commands and flexible software cryptographic

NOTE				:	this class supports only selected mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_4_MIFARE_DESFIRE_FLEX_SOFT_CRYPTO_H_INCLUDED_)
#define _FEDM_ISC_TAG_HANDLER_ISO14443_4_MIFARE_DESFIRE_FLEX_SOFT_CRYPTO_H_INCLUDED_

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include <string>
#include "../../FEDM.h"

class FedmIscTagHandler_ISO14443_4_MIFARE_DESFire;



//#####################################################################################
// constants
//#####################################################################################





//####################################################################
// class FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto
// >> supports only ISO Host Commands <<
//####################################################################

/// @brief Transponder class, nested in class FedmIscTagHandler_ISO14443_4_MIFARE_DESFire, providing manufacturer specific commands for ISO 14443-4 compliant Transponders of type MIFARE DESFire from manufacturer NXP Semiconductors
///
/// <b>Description</b><br>
/// High-level class for easy-to-use Transponder specific communication. See OBID::FEDM::Core::ITagGroup for additional information.<br>
/// This class implements the communication with [0xC3] Reader commands and flexible software cryptographic.<br>
/// Detailed description about the command interface can be found in:
/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
///
/// @note Programmers should be familiar with the ISO 14443 standards
class _FEDM_ISC_CORE_EXT_CLASS FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto
{
public:
	friend class FedmIscTagHandler_ISO14443_4_MIFARE_DESFire;
	friend class FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1;
	friend class FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3;

	unsigned char	GetErrorSource();
	unsigned int	GetErrorCode();

	//####################################################################
	// 5 security related commands
	
	/// @brief DESFire command [0xFA], realized with Reader command [0xC3][0xFA] Authenticate, AuthenticateISO, AuthenticateAES
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a security related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int Authenticate(		unsigned char ucReaderKeyIndex,
							unsigned char ucDESFireKeyNo );
	
	/// @brief DESFire command [0x54] Change Key Settings, realized with Reader command [0xC3][0x54]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a security related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int ChangeKeySettings(	unsigned char ucNewKeySettings );
	
	/// @brief DESFire command [0x45] Get Key Settings, realized with Reader command [0xC3][0x45]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a security related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int GetKeySettings(		unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );

	/// @brief DESFire command [0xC4] Change Key, realized with Reader command [0xC3][0xC4]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a security related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int ChangeKey(			unsigned char ucKeyNoToBeChanged,
							unsigned char ucAESNewKeyVersion,
							std::string sOldKey,
							std::string sNewKey );
	
	/// @brief DESFire command [0x64] Get Key Version, realized with Reader command [0xC3][0x64]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a security related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int GetKeyVersion(		unsigned char ucDESFireKeyNo,
							unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );


	//####################################################################
	// 10 PICC level commands

	/// @brief DESFire command [0xCA] Create Application, realized with Reader command [0xC3][0xCA]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a PICC level related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int CreateApplication(	unsigned char ucPiccLevelFlags,
							unsigned int uiApplicationID,
							unsigned char ucKeySetting1,
							unsigned char ucKeySetting2,
							unsigned int uiISOFileID,
							std::string sDFName );
	
	/// @brief DESFire command [0xDA] Delete Application, realized with Reader command [0xC3][0xDA]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a PICC level related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int DeleteApplication(	unsigned char ucPiccLevelFlags,
							unsigned int uiApplicationID );
	
	/// @brief DESFire command [0x6A] Get Application IDs, realized with Reader command [0xC3][0x6A]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a PICC level related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int GetApplicationIDs(	unsigned char ucPiccLevelFlags,
							unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );

	/// @brief DESFire command [0x6E] Free Memory, realized with Reader command [0xC3][0x6E]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a PICC level related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int FreeMemory(			unsigned char ucPiccLevelFlags,
							unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );

	/// @brief DESFire command [0x6D] Get DF-Names, realized with Reader command [0xC3][0x6D]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a PICC level related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int GetDFNames(			unsigned char ucPiccLevelFlags,
							unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );
	
	/// @brief DESFire command [0x5A] Select Application, realized with Reader command [0xC3][0x5A]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a PICC level related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int SelectApplication(	unsigned char ucPiccLevelFlags,
							unsigned int uiApplicationID );

	/// @brief DESFire command [0xFC] Format PICC, realized with Reader command [0xC3][0xFC]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a PICC level related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int FormatPICC(			unsigned char ucPiccLevelFlags );

	/// @brief DESFire command [0x60] Get Version, realized with Reader command [0xC3][0x60]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a PICC level related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int GetVersion(			unsigned char ucPiccLevelFlags,
							unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );

	/// @brief DESFire command [0x51] Get Card-UID, realized with Reader command [0xC3][0x51]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a PICC level related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int GetCardUID(			unsigned char ucPiccLevelFlags,
							unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );
	
	/// @brief DESFire command [0x5C] Set Configuration, realized with Reader command [0xC3][0x5C]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a PICC level related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int SetConfiguration(	unsigned char ucPiccLevelFlags,
							unsigned char* pucRequestData,
							unsigned int uiRequestLength );


	//####################################################################
	// 10 application level commands

	/// @brief DESFire command [0x5F] Change File Settings, realized with Reader command [0xC3][0x5F]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is an application level related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int ChangeFileSettings(	unsigned char ucApplicationLevelFlags,
							unsigned char ucFileNo,
							unsigned char ucNewFileCommSettings,
							unsigned char ucNewFileReadWriteAccessRights,
							unsigned char ucNewFileChangeAccessRights,
							unsigned char ucNewFileReadAccessRights,
							unsigned char ucNewFileWriteAccessRights );

	/// @brief DESFire command [0x6F] , realized with Reader command [0xC3][0x6F]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is an application level related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int GetFileIDs(			unsigned char ucApplicationLevelFlags,
							unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );

	/// @brief DESFire command [0xF5] , realized with Reader command [0xC3][0x5F]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is an application level related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int GetFileSettings(	unsigned char ucApplicationLevelFlags,
							unsigned char ucFileNo,
							unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );

	/// @brief DESFire command [0xCD] , realized with Reader command [0xC3][0xCD]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is an application level related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int CreateStdDataFile(	unsigned char ucApplicationLevelFlags,
							unsigned char ucFileNo,
							unsigned int uiISOFileID,
							unsigned char ucFileCommSettings,
							unsigned char ucFileReadWriteAccessRights,
							unsigned char ucFileChangeAccessRights,
							unsigned char ucFileReadAccessRights,
							unsigned char ucFileWriteAccessRights,
							unsigned int uiFileSize );

	/// @brief DESFire command [0xCB] , realized with Reader command [0xC3][0xCB]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is an application level related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int CreateBackupDataFile(	unsigned char ucApplicationLevelFlags,
								unsigned char ucFileNo,
								unsigned int uiISOFileID,
								unsigned char ucFileCommSettings,
								unsigned char ucFileReadWriteAccessRights,
								unsigned char ucFileChangeAccessRights,
								unsigned char ucFileReadAccessRights,
								unsigned char ucFileWriteAccessRights,
								unsigned int uiFileSize );

	/// @brief DESFire command [0xCC] , realized with Reader command [0xC3][0xCC]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is an application level related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int CreateValueFile(	unsigned char ucApplicationLevelFlags,
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

	/// @brief DESFire command [0xC1] , realized with Reader command [0xC3][0xC1]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is an application level related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int CreateLinearRecordFile(	unsigned char ucApplicationLevelFlags,
								unsigned char ucFileNo,
								unsigned int uiISOFileID,
								unsigned char ucFileCommSettings,
								unsigned char ucFileReadWriteAccessRights,
								unsigned char ucFileChangeAccessRights,
								unsigned char ucFileReadAccessRights,
								unsigned char ucFileWriteAccessRights,
								unsigned int uiRecordSize,
								unsigned int uiMaxNoOfRecords );

	/// @brief DESFire command [0xC0] , realized with Reader command [0xC3][0xC0]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is an application level related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int CreateCyclicRecordFile(	unsigned char ucApplicationLevelFlags,
								unsigned char ucFileNo,
								unsigned int uiISOFileID,
								unsigned char ucFileCommSettings,
								unsigned char ucFileReadWriteAccessRights,
								unsigned char ucFileChangeAccessRights,
								unsigned char ucFileReadAccessRights,
								unsigned char ucFileWriteAccessRights,
								unsigned int uiRecordSize,
								unsigned int uiMaxNoOfRecords );

	/// @brief DESFire command [0xDF] , realized with Reader command [0xC3][0xDF]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is an application level related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int DeleteFile(			unsigned char ucApplicationLevelFlags,
							unsigned char ucFileNo );

	/// @brief DESFire command [0x61] , realized with Reader command [0xC3][0x61]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is an application level related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int GetISOFileIDs(		unsigned char ucApplicationLevelFlags,
							unsigned char* pucResponseData,
							unsigned int uiResponseBufferLength,
							unsigned int& uiResponseLength );


	//####################################################################
	// 11 data manipulation commands
	
	/// @brief DESFire command [0xBD] Read Standard Data, realized with Reader command [0xC1][0xBD]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a data manipulation related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int ReadStandardData(	unsigned char ucDataManipulationFlags,
							unsigned char ucFileNo,
							unsigned char ucFileCommSettings,
							unsigned int uiFileOffset,
							unsigned int uiFileLen,
							unsigned char* pucFileData );
	
	/// @brief DESFire command [0x3D] Write Standard Data, realized with Reader command [0xC3][0x3D]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a data manipulation related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int WriteStandardData(	unsigned char ucDataManipulationFlags,
							unsigned char ucFileNo,
							unsigned char ucFileCommSettings,
							unsigned int uiFileOffset,
							unsigned int uiFileLen,
							unsigned char* pucFileData );

	/// @brief DESFire command [0x6C] Get Value, realized with Reader command [0xC3][0x6C]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a data manipulation related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int GetValue(			unsigned char ucDataManipulationFlags,
							unsigned char ucFileNo,
							unsigned char ucFileCommSettings,
							unsigned int& uiValue );

	/// @brief DESFire command [0x0C] Credit, realized with Reader command [0xC3][0x0C]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a data manipulation related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int Credit(				unsigned char ucDataManipulationFlags,
							unsigned char ucFileNo,
							unsigned char ucFileCommSettings,
							unsigned int uiCreditValue );

	/// @brief DESFire command [0xDC] Debit, realized with Reader command [0xC3][0xDC]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a data manipulation related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int Debit(				unsigned char ucDataManipulationFlags,
							unsigned char ucFileNo,
							unsigned char ucFileCommSettings,
							unsigned int uiDebitValue );


	/// @brief DESFire command [0x1C] Limited Credit, realized with Reader command [0xC3][0x1C]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a data manipulation related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int LimitedCredit(		unsigned char ucDataManipulationFlags,
							unsigned char ucFileNo,
							unsigned char ucFileCommSettings,
							unsigned int uiCreditValue );

	/// @brief DESFire command [0x3B] Write Record, realized with Reader command [0xC3][0x3B]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a data manipulation related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int WriteRecord(		unsigned char ucDataManipulationFlags,
							unsigned char ucFileNo,
							unsigned char ucFileCommSettings,
							unsigned int uiFileOffset,
							unsigned int uiRecordLen,
							unsigned char* pucRecordData );

	/// @brief DESFire command [0xBB] Read Records, realized with Reader command [0xC3][0xBB]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a data manipulation related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int ReadRecords(		unsigned char ucDataManipulationFlags,
							unsigned char ucFileNo,
							unsigned char ucFileCommSettings,
							unsigned int uiRecordOffset,
							unsigned int uiNoOfRecords,
							unsigned int uiRecordSize,
							unsigned char* pucRecordData );

	/// @brief DESFire command [0xEB] Clear Record File, realized with Reader command [0xC3][0xEB]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a data manipulation related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int ClearRecordFile(	unsigned char ucDataManipulationFlags,
							unsigned char ucFileNo,
							unsigned char ucFileCommSettings );

	/// @brief DESFire command [0xC7] Commit Transaction, realized with Reader command [0xC3][0xC7]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a data manipulation related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int CommitTransaction(	unsigned char ucDataManipulationFlags );

	/// @brief DESFire command [0xA7] Abort Transaction, realized with Reader command [0xC3][0xA7]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a data manipulation related command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE DESFire from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int AbortTransaction(	unsigned char ucDataManipulationFlags );

protected:
	// constructor
	FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto();
	// destructor
	virtual ~FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto();

	FedmIscTagHandler_ISO14443_4_MIFARE_DESFire* m_pParent;

};

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#endif // !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_4_MIFARE_DESFIRE_FLEX_SOFT_CRYPTO_H_INCLUDED_)
