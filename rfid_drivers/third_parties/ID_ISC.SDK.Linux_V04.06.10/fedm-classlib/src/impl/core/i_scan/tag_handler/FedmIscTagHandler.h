/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler.h                              |
|                                                       |
---------------------------------------------------------

Copyright © 2009-2014	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	30.03.2009

Version       		:	04.06.01 / 07.03.2014 / M. Hultsch
						- methods ReadMultipleBlocks and WriteMultipleBlocks now virtual

						04.03.01 / 25.09.2012 / M. Hultsch

Operation Systems	:	independent

Function			:	base class for OBID i-scan® and OBID® classic-pro transponder

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_TAG_HANDLER_H_INCLUDED_)
#define _FEDM_ISC_TAG_HANDLER_H_INCLUDED_

#include <string>
#include "../../FEDM.h"
#include "../../FEDM_Logger.h"

// STL - Standard Template Library
using namespace std;

class FEDM_ISOTabItem;
class FEDM_ISCReaderModule;
class FEDM_ISCReader;



//#####################################################################################
// constants
//#####################################################################################





//####################################################################
// class FedmIscTagHandler
// >> supports only ISO Host Commands <<
//####################################################################

/// @brief Transponder class providing basic commands for all Transponders
///
/// <b>Description</b><br>
/// High-level class for easy-to-use Transponder specific communication. See OBID::FEDM::Core::ITagGroup for additional information.<br>
/// This class supports non-addressed, selected and addressed mode in Host-Mode.
class FedmIscTagHandler
{
public:

	// friend classes have access rights to protected members
	friend class FEDM_ISCReaderModule;
	friend class FEDM_ISCReader;
	friend class FEDM_ISOTabItem;

	/// @brief Initialization method
	///
	/// <b>Description</b><br>
	/// Clears all data members
	_FEDM_ISC_CORE_EXT_CLASS virtual void Init();

	/// @brief Return of TagHandler Type Number
	///
	/// @return Type Number
	///
	/// <b>Description</b><br>
	/// Return of TagHandler Type Number
	_FEDM_ISC_CORE_EXT_CLASS unsigned int	GetTagHandlerType();

	/// @brief Return of Transponder Driver Type Number
	///
	/// @return Driver Type Number
	///
	/// <b>Description</b><br>
	/// Return of Transponder Driver Type Number. See Reader&apos;s System Manual for the full list of all Transponder Driver Type Numbers.
	_FEDM_ISC_CORE_EXT_CLASS unsigned int	GetTagDriverType();

#if defined(__BORLANDC__)
	const char*	GetTagName();
	const char*	GetUID();
#else
	/// @brief Return of TagHandler Type Number
	///
	/// @return Transponder Name
	///
	/// <b>Description</b><br>
	/// Return of TagHandler Type Number
	_FEDM_ISC_CORE_EXT_CLASS const char*	GetTagName();

	/// @brief Return of Unified Identified (UID)
	///
	/// @return Array with UID
	///
	/// <b>Description</b><br>
	/// Return of Unified Identifier (UID). Sometimes also called as Serial Number. For EPC Class 1 Gen 2 or ISO 18000-3M3 standards, the Electronic Product Code (EPC) is returned.
	/// If the Reader is configured to return the EPC with additional TID, then both is returned.
	_FEDM_ISC_CORE_EXT_CLASS const char*	GetUID();
#endif

	/// @brief Return of Received Signal Strength Indication (RSSI) value
	///
	/// @param[out] ucNumber Array with antenna numbers
	/// @param[out] ucStatus Array with status values
	/// @param[out] ucRSSI Array with RSSI values
	/// @return 
	/// - > 0 Number of antennas
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	///
	/// <b>Description</b><br>
	/// Return arrays with additional antenna values. The array dimension is the return value of this method.
	///
	/// @note This additional antenna values are not available with all Readers. See Reader&apos;s System Manual for additional information.
	_FEDM_ISC_CORE_EXT_CLASS int GetRSSI(unsigned char* ucNumber, unsigned char* ucStatus, unsigned char* ucRSSI);

	/// @brief Return of Transponder address after an error
	///
	/// @return Address number, where an error occured
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS unsigned int GetErrorDBAddress()			{ return m_uiErrorDBAddress; }

	/// @brief Return of additional, Transponder specific error code
	///
	/// @return Error code
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS unsigned char GetLastISOErrorCode()		{ return m_ucISOErrorCode; }

	/// @brief Switch into non-addressed status (only for internal use)
	///
	/// @param bEnable If true, TagHandler will be prepared for non-addressed Transponder communication
	///
	/// <b>Description</b><br>
	/// Do not use in applications! Use instead OBID::FEDM::Core::ITagGroup::CreateNonAddressedTagHandler
	_FEDM_ISC_CORE_EXT_CLASS void EnableNonAddressedMode(bool bEnable)	{ m_bNonAddressedMode = bEnable; }

	/// @brief Request of non-addressed status
	///
	/// @return non-addressed status
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS bool IsNonAddressedModeEnabled()			{ return m_bNonAddressedMode; }


	/// @brief Basic [0xB0][0x23] Read Multiple Blocks command
	///
	/// @param[in] uiFirstDataBlock First data block address
	/// @param[in] uiNoOfDataBlocks Number of requested data blocks
	/// @param[out] uiBlockSize Number of bytes in each data block
	/// @param[out] pucData Pointer to array for received data blocks. The array must have the dimension of uiNoOfDataBlocks x Expected Block Size
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Basic [0xB0][0x23] Read Multiple Blocks command<br>
	/// - Without support for extended address mode<br>
	/// - Without request of security status<br>
	_FEDM_ISC_CORE_EXT_CLASS virtual int ReadMultipleBlocks(	unsigned int uiFirstDataBlock,
																unsigned int uiNoOfDataBlocks,
																unsigned int& uiBlockSize,
																unsigned char* pucData );

	/// @brief Basic [0xB0][0x23] Read Multiple Blocks command with additional security status
	///
	/// @param[in] uiFirstDataBlock First data block address
	/// @param[in] uiNoOfDataBlocks Number of requested data blocks
	/// @param[out] uiBlockSize Number of bytes in each data block
	/// @param[out] pucSecurityStatus Array with received status information, one byte for each data block. The array must have the dimension of uiNoOfDataBlocks
	/// @param[out] pucData Pointer to array for received data blocks. The array must have the dimension of uiNoOfDataBlocks x Expected Block Size
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Basic [0xB0][0x23] Read Multiple Blocks command with additional security status<br>
	/// - Without support for extended address mode<br>
	_FEDM_ISC_CORE_EXT_CLASS int virtual ReadMultipleBlocks(	unsigned int uiFirstDataBlock,
																unsigned int uiNoOfDataBlocks,
																unsigned int& uiBlockSize,
																unsigned char* pucSecurityStatus,
																unsigned char* pucData );

	/// @brief Basic [0xB0][0x24] Write Multiple Blocks command
	///
	/// @param[in] uiFirstDataBlock First data block address
	/// @param[in] uiNoOfDataBlocks Number of data blocks to transmit to
	/// @param[in] uiBlockSize Number of bytes in each data block
	/// @param[in] pucData Pointer to array with data blocks to transmit to
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Basic [0xB0][0x24] Write Multiple Blocks command<br>
	/// - Without support for extended address mode<br>
	_FEDM_ISC_CORE_EXT_CLASS int virtual WriteMultipleBlocks(	unsigned int uiFirstDataBlock,
																unsigned int uiNoOfDataBlocks,
																unsigned int uiBlockSize,
																unsigned char* pucData );

public:
	// declaration of tag handler types for run-time identification
#if _MSC_VER > 1200
	/// @brief Identifier for Base TagHandler class
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_BASIC = 1;
	/// @brief Identifier for Transponder class for EPC Class1 Generation2
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_EPC_CLASS1_GEN2 = 10;
	/// @brief Identifier for Transponder class for EPC Class1 Generation2, with extension for SL900A of Manufacturer IDS Microchip AG
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_EPC_CLASS1_GEN2_IDS_SL900A = 11;
	/// @brief Identifier for ISO 14443 Transponder class
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443 = 20;
	/// @brief Identifier for ISO 14443-4 Transponder class, with extension for Innovatron (ISO 14443B')
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_INNOVATRON = 21;
	/// @brief Identifier for ISO 14443-2 Transponder class
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_2 = 30;
	/// @brief Identifier for ISO 14443-2 Transponder class, with extension for Innovision Jewel
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_2_INNOVISION_JEWEL = 31;
	/// @brief Identifier for ISO 14443-2 Transponder class, with extension for SR176 of manufacturer STMicroelectronics SA
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_2_STM_SR176 = 32;
	/// @brief Identifier for ISO 14443-2 Transponder class, with extension for SRIxxx series of manufacturer STMicroelectronics SA
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_2_STM_SRIxxx = 33;
	/// @brief Identifier for ISO 14443-3 Transponder class
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_3 = 40;
	/// @brief Identifier for ISO 14443-3 Transponder class, with extensions for my-d move of Manufacturer Infineon Technologies AG
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_3_INFINEON_MY_D = 41;
	/// @brief Identifier for ISO 14443-3 Transponder class, with extensions for my-d move of Manufacturer Infineon Technologies AG
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_3_INFINEON_MY_D_MOVE = 42;
	/// @brief Identifier for ISO 14443-3 Transponder class, with extensions for MIFARE Classic (Mini, 1K, 4K) of Manufacturer NXP Semiconductors
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_3_MIFARE_CLASSIC = 43;
	/// @brief Identifier for ISO 14443-3 Transponder class, with extensions for MIFARE Ultralight of Manufacturer NXP Semiconductors
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_3_MIFARE_ULTRALIGHT = 44;
	/// @brief Identifier for ISO 14443-3 Transponder class, with extensions for MIFARE Plus in Security Level 1 of Manufacturer NXP Semiconductors
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_3_MIFARE_PLUS_SL1 = 45;
	/// @brief Identifier for ISO 14443-3 Transponder class, with extensions for MIFARE Plus in Security Level 2 of Manufacturer NXP Semiconductors
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_3_MIFARE_PLUS_SL2 = 46;
	/// @brief Identifier for ISO 14443-3 Transponder class, with extensions for CTx series of Manufacturer ASK
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_3_ASK_CTX = 47;
	/// @brief Identifier for ISO 14443-4 Transponder class
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_4 = 50;
	/// @brief Identifier for ISO 14443-4 Transponder class, with extensions for MIFARE DESFire of Manufacturer NXP Semiconductors
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_4_MIFARE_DESFIRE = 51;
	/// @brief Identifier for ISO 14443-4 Transponder class, with extensions for MIFARE Plus in Security Level 1 of Manufacturer NXP Semiconductors
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_4_MIFARE_PLUS_SL1 = 52;
	/// @brief Identifier for ISO 14443-4 Transponder class, with extensions for MIFARE Plus in Security Level 2 of Manufacturer NXP Semiconductors
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_4_MIFARE_PLUS_SL2 = 53;
	/// @brief Identifier for ISO 14443-4 Transponder class, with extensions for MIFARE Plus in Security Level 3 of Manufacturer NXP Semiconductors
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_4_MIFARE_PLUS_SL3 = 54;
	/// @brief Identifier for ISO 14443-4 Transponder class, with extensions for Manufacturer Maxim
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_4_MAXIM = 60;
	/// @brief Identifier for ISO 14443-4 Transponder class, with extensions for MAX66000 of Manufacturer Maxim
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_4_MAXIM_MAX66000 = 61;
	/// @brief Identifier for ISO 14443-4 Transponder class, with extensions for MAX66020 of Manufacturer Maxim
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_4_MAXIM_MAX66020 = 62;
	/// @brief Identifier for ISO 14443-4 Transponder class, with extensions for MAX66040 of Manufacturer Maxim
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_4_MAXIM_MAX66040 = 63;
	/// @brief Identifier for Transponder class for ISO 18000-3M3
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO18000_3M3 = 100;
	/// @brief ISO 15693 Transponder class
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693 = 0xE0000000;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for Manufacturer STMicroelectronics SA
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_STM = 0xE0020000;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for LRI2K of Manufacturer STMicroelectronics SA
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_STM_LRI2K = 0xE0022000;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for LRIS2K of Manufacturer STMicroelectronics SA
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_STM_LRIS2K = 0xE0020280;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for LRIS64K of Manufacturer STMicroelectronics SA
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_STM_LRIS64K = 0xE0020440;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for M24LR64-R of Manufacturer STMicroelectronics SA
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_STM_M24LR64R = 0xE00202C0;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for M24LRxxE-R series of Manufacturer STMicroelectronics SA
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_STM_M24LRxxER = 0xE00205C0;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for Manufacturer NXP Semiconductors
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_NXP = 0xE0040000;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for I-CODE SLI of Manufacturer NXP Semiconductors
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_NXP_ICODE_SLI = 0xE0040100;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for I-CODE SLI of Manufacturer NXP Semiconductors
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_NXP_ICODE_SLIX = 0xE0040110;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for I-CODE SLI-L of Manufacturer NXP Semiconductors
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_NXP_ICODE_SLI_L = 0xE0040300;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for I-CODE SLI-L of Manufacturer NXP Semiconductors
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_NXP_ICODE_SLIX_L = 0xE0040310;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for I-CODE SLI-S of Manufacturer NXP Semiconductors
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_NXP_ICODE_SLI_S = 0xE0040200;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for I-CODE SLI-S of Manufacturer NXP Semiconductors
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_NXP_ICODE_SLIX_S = 0xE0040210;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for Manufacturer Infineon Technologies AG
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_Infineon = 0xE0050000;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for my-d of Manufacturer Infineon Technologies AG
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_Infineon_my_d = 0xE005FFFF;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for Manufacturer Texas Instruments
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_TI = 0xE0070000;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for Tag-it HFI Pro of Manufacturer Texas Instruments
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_TI_Tag_it_HFI_Pro = 0xE007E000;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for Tag-it HFI Plus of Manufacturer Texas Instruments
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_TI_Tag_it_HFI_Plus = 0xE0078000;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for Manufacturer Fujitsu Limited
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_Fujitsu = 0xE0080000;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for MB89R1xx series of Manufacturer Fujitsu Limited
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_Fujitsu_MB89R1xx = 0xE0080001;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for Manufacturer EM Microelectronic-Marin SA
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_EM = 0xE0160000;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for EM4034 of Manufacturer EM Microelectronic-Marin SA
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_EM_4034 = 0xE0160004;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for Manufacturer KSW Microtec GmbH
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_KSW = 0xE0170000;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for Manufacturer Maxim
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_MAXIM = 0xE02B0000;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for MAX66100 of Manufacturer Maxim
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_MAXIM_MAX66100 = 0xE02B0010;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for MAX66120 of Manufacturer Maxim
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_MAXIM_MAX66120 = 0xE02B0020;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for MAX66140 of Manufacturer Maxim
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_MAXIM_MAX66140 = 0xE02B0030;
	/// @brief Identifier for ISO 15693 Transponder class, with extensions for SL13A of Manufacturer IDS Microchip AG
	///
	/// <b>Description</b><br>
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_IDS_SL13A = 0xE036FFFF;
#else
	 // for Visual Studio 6
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_BASIC;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_EPC_CLASS1_GEN2;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_EPC_CLASS1_GEN2_IDS_SL900A;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_INNOVATRON;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_2;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_2_INNOVISION_JEWEL;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_2_STM_SR176;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_2_STM_SRIxxx;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_3;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_3_INFINEON_MY_D;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_3_INFINEON_MY_D_MOVE;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_3_MIFARE_CLASSIC;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_3_MIFARE_ULTRALIGHT;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_3_MIFARE_PLUS_SL1;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_3_MIFARE_PLUS_SL2;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_3_ASK_CTX;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_4;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_4_MIFARE_DESFIRE;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_4_MIFARE_PLUS_SL1;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_4_MIFARE_PLUS_SL2;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_4_MIFARE_PLUS_SL3;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_4_MAXIM;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_4_MAXIM_MAX66000;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_4_MAXIM_MAX66020;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO14443_4_MAXIM_MAX66040;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO18000_3M3;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_STM;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_STM_LRI2K;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_STM_LRIS2K;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_STM_M24LR64R;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_STM_LRIS64K;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_STM_M24LRxxER;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_NXP;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_NXP_ICODE_SLI;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_NXP_ICODE_SLIX;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_NXP_ICODE_SLI_L;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_NXP_ICODE_SLIX_L;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_NXP_ICODE_SLI_S;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_NXP_ICODE_SLIX_S;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_Infineon;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_Infineon_my_d;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_TI;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_TI_Tag_it_HFI_Pro;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_TI_Tag_it_HFI_Plus;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_Fujitsu;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_Fujitsu_MB89R1xx;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_EM;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_EM_4034;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_KSW;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_MAXIM;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_MAXIM_MAX66100;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_MAXIM_MAX66120;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_MAXIM_MAX66140;
	_FEDM_ISC_CORE_EXT_CLASS static const unsigned int TYPE_ISO15693_IDS_SL13A;
#endif

	// protected methods
protected:
	// constructor
	_FEDM_ISC_CORE_EXT_CLASS FedmIscTagHandler(FEDM_ISCReader* pReader,	unsigned int uiTagHandlerType, FEDM_ISOTabItem* pTabItem);
	// destructor
	_FEDM_ISC_CORE_EXT_CLASS virtual ~FedmIscTagHandler();

	// for friend class FedmIscTagDialog_Base
	FEDM_ISCReader*		m_pReader;
	FEDM_ISOTabItem*	GetTabItem()		{ return m_pTabItem; }

	// protected attributes
protected:
	unsigned int	m_uiTagHandlerType;
	unsigned int	m_uiErrorDBAddress;
	unsigned char	m_ucISOErrorCode;

	FEDM_ISOTabItem* m_pTabItem;

	string			m_sUid;	// for temporary use
	string			m_sTagName;

	bool			m_bNonAddressedMode;
};

#endif // !defined(_FEDM_ISC_TAG_HANDLER_H_INCLUDED_)
