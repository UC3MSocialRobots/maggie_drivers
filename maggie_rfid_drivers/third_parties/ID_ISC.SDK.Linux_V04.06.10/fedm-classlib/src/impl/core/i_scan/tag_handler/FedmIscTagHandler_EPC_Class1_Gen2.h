/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_EPC_Class1_Gen2.h              |
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
Begin        		:	30.03.2009

Version       		:	04.03.01 / 14.11.2012 / M. Hultsch
						- Kill with extended parameter list for support of Recommissioning Bits
						- new methods: GetTagModelNumber()
									   GetMaskDesignerID()
									   GetMaskDesignerName()
									   IsUidWithTid()
									   IsExtendedPC_W1()
									   GetExtendedProtocolControlW1()

						04.00.07 / 12.12.2011 / M. Hultsch
						- new method: simplified Lock

						04.00.02 / 11.08.2011 / M. Hultsch
						- new method: ReadCompleteBank

						04.00.00 / 21.07.2011 / M. Hultsch
						- new methods: GetProtocolControl
									   GetEpcOfUid
									   GetTidOfUid

						03.03.04 / 16.06.2011 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID i-scan® HF/UHF transponder EPC Class 1 Generation 2

NOTE				:	this class supports only addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_TAG_HANDLER_EPC_CLASS1_GEN2_H_INCLUDED_)
#define _FEDM_ISC_TAG_HANDLER_EPC_CLASS1_GEN2_H_INCLUDED_

#if !defined(_FEDM_NO_TAG_HANDLER_EPC_C1_G2)

#include "../../FEDM.h"
#include "FedmIscTagHandler.h"

class FEDM_ISOTabItem;


//#####################################################################################
// constants
//#####################################################################################





//####################################################################
// class FedmIscTagHandler_EPC_Class1_Gen2
// >> supports only ISO Host Commands <<
//####################################################################

/// @brief Transponder class providing basic commands for all EPC Class 1 Generation 2 compliant Transponders
///
/// <b>Description</b><br>
/// High-level class for easy-to-use Transponder specific communication. See OBID::FEDM::Core::ITagGroup for additional information.<br>
///
/// @note Programmers should be familiar with the EPC Class 1 Gen 2 standard (<A HREF="http:///www.epcglobal.org">www.epcglobal.org</A>)
class _FEDM_ISC_CORE_EXT_CLASS FedmIscTagHandler_EPC_Class1_Gen2 : public FedmIscTagHandler
{
public:

	// friend classes have access rights to protected members
	friend class FEDM_ISCReaderModule;
	friend class FEDM_ISCReader;
	friend class FEDM_ISOTabItem;

//	virtual void Init();					// set all data members to 0

	/// @brief Method to query if the UID is a combination of EPC and TID
	///
	/// @return true, if UID is EPC + TID
	///
	/// <b>Description</b><br>
	/// 
	bool IsUidWithTid();

	/// @brief Method to query if the Protocol-Control (PC) is extended
	///
	/// @return true, if PC is extended, otherwise false
	///
	/// <b>Description</b><br>
	/// Method to query if the Protocol-Control (PC) is extended with 1 additional block of 2 bytes.
	bool IsExtendedPC_W1();

	/// @brief Method to query the Protocol-Control (PC)
	///
	/// @return Protocol Control
	///
	/// <b>Description</b><br>
	/// 
	unsigned int GetProtocolControl();

	/// @brief Method to query the Extended Protocol-Control (XPC_W1)
	///
	/// @return Extended Protocol Control XPC_W1
	///
	/// <b>Description</b><br>
	/// 
	unsigned int GetExtendedProtocolControlW1();

	/// @brief Returns the EPC from the UID
	///
	/// @return String with EPC
	///
	/// <b>Description</b><br>
	/// The EPC is extracted from the UID and returned.
	const char* GetEpcOfUid();

	/// @brief Returns the TID from the UID
	///
	/// @return String with TID
	///
	/// <b>Description</b><br>
	/// The TID is extracted from the UID and returned.
	const char* GetTidOfUid();
	
	/// @brief Method to query the Tag Model Number
	///
	/// @return Tag Model Number
	///
	/// <b>Description</b><br>
	/// The Tag Model Number is part of the TID and can only be returned, if the TID is part of the UID or if the first 2 blocks of the TID memory is read separately.<br>
	/// The EPC Class 1 Gen 2 standard reserves the bits 14h .. 1Fh of the TID memory as a vendor-defined 12-bit Tag Model Number.
	unsigned int GetTagModelNumber();

	/// @brief Method to query the Mask Designer Identifier
	///
	/// @return Mask Designer Identifier
	///
	/// <b>Description</b><br>
	/// The Mask Designer Identifier is part of the TID and can only be returned, if the TID is part of the UID or if the first 2 blocks of the TID memory is read separately.<br>
	/// The EPC Class 1 Gen 2 standard reserves the bits 08h .. 13h of the TID memory as a 12-bit Mask Designer Identifier (obtainable from the registration authority).
	unsigned int GetMaskDesignerID();

	/// @brief Method to query the Manufacturer Name
	///
	/// @return String with Manufacturer Name
	///
	/// <b>Description</b><br>
	/// The Manufacturer Name is specified by the Mask Designer Identifier, which is part of the TID.<br>
	/// The Manufacturer name can only be returned, if the TID is part of the UID or if the first 2 blocks of the TID memory is read separately.<br>
	/// The EPC Class 1 Gen 2 standard reserves the bits 08h .. 13h of the TID memory as a 12-bit Mask Designer Identifier (obtainable from the registration authority).
	const char* GetMaskDesignerName();

	/// @brief Read the complete content of a bank
	///
	/// @param[in] uiBank Bank number. Use [BANK_RESERVED](@ref BANK_RESERVED), [BANK_EPC](@ref BANK_EPC), [BANK_TID](@ref BANK_TID) or [BANK_USER](@ref BANK_USER)
	/// @param[in] sPassword Optional hex-coded password with even number of chars. Set to "" (empty string) disables password.
	/// @param[out] pucData Pointer to array for received data blocks
	/// @param[in] uiDataBufferSize Size of the array for received data blocks
	/// @param[out] uiNoOfDataBlocksRead Return of the number of data blocks read
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// [0xB0][0x23] Read Multiple Blocks command with READ_COMPLETE_BANK option<br>
	///
	/// @note Hex-coded string means, that a hex value of 0xAB39F1D9 must be set as "AB39F1D9".
	int ReadCompleteBank(		unsigned int uiBank,
								std::string sPassword,
								unsigned char* pucData,
								unsigned int uiDataBufferSize,
								unsigned int& uiNoOfDataBlocksRead );

	/// @brief [0xB0][0x23] Read Multiple Blocks command
	///
	/// @param[in] uiBank Bank number. Use [BANK_RESERVED](@ref BANK_RESERVED), [BANK_EPC](@ref BANK_EPC), [BANK_TID](@ref BANK_TID) or [BANK_USER](@ref BANK_USER)
	/// @param[in] uiFirstDataBlock First data block address
	/// @param[in] uiNoOfDataBlocks Number of requested data blocks
	/// @param[in] sPassword Optional hex-coded password with even number of chars. Set to "" (empty string) disables password.
	/// @param[out] pucData Pointer to array for received data blocks
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// [0xB0][0x23] Read Multiple Blocks command<br>
	///
	/// @note Hex-coded string means, that a hex value of 0xAB39F1D9 must be set as "AB39F1D9".
	int ReadMultipleBlocks(		unsigned int uiBank,
								unsigned int uiFirstDataBlock,
								unsigned int uiNoOfDataBlocks,
								std::string sPassword,
								unsigned char* pucData );

	/// @brief [0xB0][0x24] Write Multiple Blocks command
	///
	/// @param[in] uiBank Bank number. Use [BANK_RESERVED](@ref BANK_RESERVED), [BANK_EPC](@ref BANK_EPC), [BANK_TID](@ref BANK_TID) or [BANK_USER](@ref BANK_USER)
	/// @param[in] uiFirstDataBlock First data block address
	/// @param[in] uiNoOfDataBlocks Number of data blocks to transmit to
	/// @param[in] sPassword Optional hex-coded password with even number of chars. Set to "" (empty string) disables password.
	/// @param[in] pucData Pointer to array with data blocks to transmit to
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Basic [0xB0][0x24] Write Multiple Blocks command<br>
	///
	/// @note Hex-coded string means, that a hex value of 0xAB39F1D9 must be set as "AB39F1D9".
	int WriteMultipleBlocks(	unsigned int uiBank,
								unsigned int uiFirstDataBlock,
								unsigned int uiNoOfDataBlocks,
								std::string sPassword,
								unsigned char* pucData );

	/// @brief Write of an EPC into the EPC Memory
	///
	/// @param[in] sNewEPC Hex-coded string with the new EPC with even number of chars
	/// @param[in] sPassword Optional hex-coded password with even number of chars. Set to "" (empty string) disables password (preset: disabled).
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Special [0xB0][0x24] Write Multiple Blocks command to write a new EPC number to the EPC bank of a transponder<br>
	/// The EPC-lenght must be multiple of 2 bytes. Thus, the EPC string must contain multiple of 4 characters.
	///
	/// @note Hex-coded string means, that a hex value of 0xAB39F1D9 must be set as "AB39F1D9".
	int WriteEPC(	std::string sNewEPC, std::string sPassword="");
	
	/// @brief Kill command
	///
	/// @param[in] sPassword Optional hex-coded password with even number of chars. Set to "" (empty string) disables password.
	/// @param[in] bRecom_AssertedLSB Recommissioning Bit Asserted LSB (preset: disabled)
	/// @param[in] bRecom_Asserted2SB Recommissioning Bit Asserted 2LSB (preset: disabled)
	/// @param[in] bRecom_Asserted3SB Recommissioning Bit Asserted 3LSB (preset: disabled)
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// The kill command can destroy the Transponder. See EPC Class 1 Gen 2 standard for detailed information, especially how to handle the Kill command with Recommissioning Bits.
	///
	/// @note Hex-coded string means, that a hex value of 0xAB39F1D9 must be set as "AB39F1D9".
	int Kill(		std::string sPassword,
					bool bRecom_AssertedLSB=false,
					bool bRecom_Asserted2SB=false,
					bool bRecom_Asserted3SB=false );

	/// @brief Lock command (simplified version)
	///
	/// @param[in] sPassword Optional hex-coded password with even number of chars. Set to "" (empty string) disables password.
	/// @param[in] ucLockKill lock settings for kill
	/// @param[in] ucLockAccess lock settings for access pasword
	/// @param[in] ucLockEPC lock settings for  EPC memory
	/// @param[in] ucLockTID lock settings for TID memory
	/// @param[in] ucLockUser lock settings for User memory
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// [0xB3][0x22] Lock command to secure the access to a transponder.<br>
	/// The Lock command can lock permanently one bank or multiple banks of the Transponder. See EPC Class 1 Gen 2 standard for detailed information.<br>
	/// <br>
	/// Use one of the constants for each lock setting:<br>
	/// - [UNCHANGED](@ref UNCHANGED)<br>
	/// - [UNLOCK](@ref UNLOCK)<br>
	/// - [UNLOCK_PERMANENTLY](@ref UNLOCK_PERMANENTLY)<br>
	/// - [LOCK](@ref LOCK)<br>
	/// - [LOCK_PERMANENTLY](@ref LOCK_PERMANENTLY)<br>
	///
	/// @note Hex-coded string means, that a hex value of 0xAB39F1D9 must be set as "AB39F1D9".
	int Lock(		std::string sPassword,
					unsigned char ucLockKill,
					unsigned char ucLockAccess,
					unsigned char ucLockEPC,
					unsigned char ucLockTID,
					unsigned char ucLockUser );

	/// @brief Lock command (parameters according EPC Class 1 Gen 2 specification)
	///
	/// @param[in] sPassword Optional hex-coded password with even number of chars. Set to "" (empty string) disables password.
	/// @param[in] ucKillMask		2 Bit mask settings for kill
	/// @param[in] ucKillAction		2 Bit action settings for kill
	/// @param[in] ucAccessMask		2 Bit mask settings for access pasword
	/// @param[in] ucAccessAction	2 Bit action settings for access pasword
	/// @param[in] ucEPCMask		2 Bit mask settings for  EPC memory
	/// @param[in] ucEPCAction		2 Bit action settings for  EPC memory
	/// @param[in] ucTIDMask		2 Bit mask settings for TID memory
	/// @param[in] ucTIDAction		2 Bit action settings for TID memory
	/// @param[in] ucUserMask		2 Bit mask settings for User memory
	/// @param[in] ucUserAction		2 Bit action settings for User memory
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// [0xB3][0x22] Lock command to secure the access to a transponder.<br>
	/// The Lock command can lock permanently one bank or multiple banks of the Transponder. See EPC Class 1 Gen 2 standard for detailed information.<br>
	/// <br>
	/// @note Hex-coded string means, that a hex value of 0xAB39F1D9 must be set as "AB39F1D9".
	int Lock(		std::string sPassword,
					unsigned char ucKillMask,
					unsigned char ucKillAction,
					unsigned char ucAccessMask,
					unsigned char ucAccessAction,
					unsigned char ucEPCMask,
					unsigned char ucEPCAction,
					unsigned char ucTIDMask,
					unsigned char ucTIDAction,
					unsigned char ucUserMask,
					unsigned char ucUserAction );

	/// @brief [0xB3][0x25] Block Permalock command
	///
	/// @param[in] uiBank Bank number. Use [BANK_RESERVED](@ref BANK_RESERVED), [BANK_EPC](@ref BANK_EPC), [BANK_TID](@ref BANK_TID) or [BANK_USER](@ref BANK_USER)
	/// @param[in] uiBlockPointer first block (consider: vendor specific block size)
	/// @param[in] uiBlockRange number of blocks (limited to 4)
	/// @param[in] sPassword Optional hex-coded password with even number of chars. Set to "" (empty string) disables password.
	/// @param[in] pucMask Pointer to array with mask data to transmit to
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This is an optional command and must not be supported by the Transponder.<br>
	/// See EPC Class 1 Gen 2 standard for detailed information.
	///
	/// @note Hex-coded string means, that a hex value of 0xAB39F1D9 must be set as "AB39F1D9".
	int BlockPermalock(			unsigned int uiBank,
								unsigned int uiBlockPointer,
								unsigned int uiBlockRange,
								std::string sPassword,
								unsigned char* pucMask );

	/// @brief [0xB3][0x25] Block Permalock command
	///
	/// @param[in] uiBank Bank number. Use [BANK_RESERVED](@ref BANK_RESERVED), [BANK_EPC](@ref BANK_EPC), [BANK_TID](@ref BANK_TID) or [BANK_USER](@ref BANK_USER)
	/// @param[in] uiBlockPointer first block (consider: vendor specific block size)
	/// @param[in] uiBlockRange number of blocks (limited to 4)
	/// @param[in] sPassword Optional hex-coded password with even number of chars. Set to "" (empty string) disables password.
	/// @param[out] pucMask Pointer to array for received mask data
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This is an optional command and must not be supported by the Transponder.<br>
	/// See EPC Class 1 Gen 2 standard for detailed information.
	///
	/// @note Hex-coded string means, that a hex value of 0xAB39F1D9 must be set as "AB39F1D9".
	int ReadPermalockStatus(	unsigned int uiBank,
								unsigned int uiBlockPointer,
								unsigned int uiBlockRange,
								std::string sPassword,
								unsigned char* pucMask );

	// Tag Data Standard http://www.epcglobalinc.org/standards/tds/tds_1_4-standard-20080611.pdf
	// GTIN:  GS1 Global Trade Item Number, (here, a serialized version)
	// SSCC:  GS1 Serial Shipping Container Code
	// GLN:  GS1 Global Location Number
	// GRAI:  GS1 Global Returnable Asset Identifier
	// GIAI:  GS1 Global Individual Asset Identifier
	// GID:  GS1 General Identifier
	// GDTI:  GS1 Global Document Type Identifier
	// GSRN:  GS1 Global Service Relation Number

	// constants for Bank
#if _MSC_VER > 1200
	/// @brief Constant for bank type Reserved Memory
	///
	/// 
	static const unsigned int BANK_RESERVED	= 0;
	/// @brief Constant for bank type EPC Memory
	///
	/// 
	static const unsigned int BANK_EPC		= 1;
	/// @brief Constant for bank type TID Memory
	///
	/// 
	static const unsigned int BANK_TID		= 2;
	/// @brief Constant for bank type User Memory
	///
	/// 
	static const unsigned int BANK_USER		= 3;

	/// @brief Constant for Lock command to keep lock status unchanged
	///
	/// 
	static const unsigned int UNCHANGED				= 0x00;
	/// @brief Constant for Lock command to unlock a bank
	///
	/// 
	static const unsigned int UNLOCK				= 0x20;
	/// @brief Constant for Lock command to unlock a bank permanently
	///
	/// 
	static const unsigned int UNLOCK_PERMANENTLY	= 0x31;
	/// @brief Constant for Lock command to lock a bank
	///
	/// 
	static const unsigned int LOCK					= 0x22;
	/// @brief Constant for Lock command to lock a bank permanently
	///
	/// 
	static const unsigned int LOCK_PERMANENTLY		= 0x33;
#else
	static const unsigned int BANK_RESERVED;
	static const unsigned int BANK_EPC;
	static const unsigned int BANK_TID;
	static const unsigned int BANK_USER;

	// constants for [0x22] Lock
	static const unsigned char UNCHANGED;
	static const unsigned char UNLOCK;
	static const unsigned char UNLOCK_PERMANENTLY;
	static const unsigned char LOCK;
	static const unsigned char LOCK_PERMANENTLY;
#endif

protected:
	// constructor
	FedmIscTagHandler_EPC_Class1_Gen2(FEDM_ISCReader* pReader, FEDM_ISOTabItem* pTabItem);
	// 2. constructor
	FedmIscTagHandler_EPC_Class1_Gen2(FEDM_ISCReader* pReader, unsigned int uiTagHandlerType, FEDM_ISOTabItem* pTabItem);
	// destructor
	virtual ~FedmIscTagHandler_EPC_Class1_Gen2();

	std::string m_sEpc;
	std::string m_sTid;

};

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_EPC_C1_G2)

#endif // !defined(_FEDM_ISC_TAG_HANDLER_EPC_CLASS1_GEN2_H_INCLUDED_)
