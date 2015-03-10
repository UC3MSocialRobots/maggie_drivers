/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3.h   |
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
Begin        		:	24.08.2009

Version       		:	03.01.00 / 10.02.2010 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID® classic-pro transponder ISO14443-4 MIFARE Plus  in Security Level 3

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_4_MIFARE_PLUS_SL3_H_INCLUDED_)
#define _FEDM_ISC_TAG_HANDLER_ISO14443_4_MIFARE_PLUS_SL3_H_INCLUDED_

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "../../FEDM.h"
#include "FedmIscTagHandler_ISO14443_4_MIFARE_Plus.h"

class FEDM_ISOTabItem;



//#####################################################################################
// constants
//#####################################################################################





//####################################################################
// class FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3
// >> supports only ISO Host Commands <<
//####################################################################

/// @brief Transponder class providing manufacturer specific commands for ISO 14443-4 compliant Transponders of type MIFARE Plus in Security Level 3 from manufacturer NXP Semiconductors
///
/// <b>Description</b><br>
/// High-level class for easy-to-use Transponder specific communication. See OBID::FEDM::Core::ITagGroup for additional information.<br>
/// This class implements the communication with [0xC2] Reader commands.<br>
/// Detailed description about the command interface can be found in:
/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
///
/// @note Programmers should be familiar with the ISO 14443 standards
class _FEDM_ISC_CORE_EXT_CLASS FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3 : public FedmIscTagHandler_ISO14443_4_MIFARE_Plus
{
public:

	// friend classes have access rights to protected members
	friend class FEDM_ISCReaderModule;
	friend class FEDM_ISCReader;
	friend class FEDM_ISOTabItem;

//	virtual void Init();					// set all data members to 0


	// Security Level 3 commands

	/// @brief MIFARE Plus Command [0x78], realized with Reader command [0xC2][0x78] SL3 Reset Authent
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a Security Level 3 command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int ResetAuthentication();

	/// @brief MIFARE Plus Command [0x30], realized with Reader command [0xC2][0x30] SL3 Read Encrypted
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a Security Level 3 command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int Read_Encrypted(			unsigned int uiBlockNo, 
								unsigned char ucExt, 
								unsigned char* pucResponseData );

	/// @brief MIFARE Plus Command [0x31], realized with Reader command [0xC2][0x31] SL3 Read Encrypted Maced
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a Security Level 3 command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int Read_EncryptedMaced(	unsigned int uiBlockNo, 
								unsigned char ucExt, 
								unsigned char* pucResponseData );

	/// @brief MIFARE Plus Command [0x32], realized with Reader command [0xC2][0x32] SL3 Read Plain
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a Security Level 3 command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int Read_Plain(				unsigned int uiBlockNo, 
								unsigned char ucExt, 
								unsigned char* pucResponseData );

	/// @brief MIFARE Plus Command [0x33], realized with Reader command [0xC2][0x33] SL3 Read Plain Maced
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a Security Level 3 command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int Read_PlainMaced(		unsigned int uiBlockNo, 
								unsigned char ucExt, 
								unsigned char* pucResponseData );

	/// @brief MIFARE Plus Command [0x34], realized with Reader command [0xC2][0x34] SL3 Read Encrypted Unmaced
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a Security Level 3 command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int Read_EncryptedUnmaced(	unsigned int uiBlockNo, 
								unsigned char ucExt, 
								unsigned char* pucResponseData );

	/// @brief MIFARE Plus Command [0x35], realized with Reader command [0xC2][0x35] SL3 Read Encrypted Unmaced Respond Maced
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a Security Level 3 command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int Read_EncryptedUnmaced_RespondMaced(	unsigned int uiBlockNo, 
											unsigned char ucExt, 
											unsigned char* pucResponseData );

	/// @brief MIFARE Plus Command [0x36], realized with Reader command [0xC2][0x36] SL3 Read Plain Unmaced
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a Security Level 3 command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int Read_PlainUnmaced(		unsigned int uiBlockNo, 
								unsigned char ucExt, 
								unsigned char* pucResponseData );

	/// @brief MIFARE Plus Command [0x37], realized with Reader command [0xC2][0x37] SL3 Read Plain Unmaced Respond Maced
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a Security Level 3 command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int Read_PlainUnmaced_RespondMaced(		unsigned int uiBlockNo, 
											unsigned char ucExt, 
											unsigned char* pucResponseData );

	/// @brief MIFARE Plus Command [0xA0], realized with Reader command [0xC2][0xA0] SL3 Write Encrypted
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a Security Level 3 command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int Write_Encrypted(		unsigned int uiBlockNo, 
								unsigned char ucLength,
								unsigned char* pucRequestData );

	/// @brief MIFARE Plus Command [0xA1], realized with Reader command [0xC2][0xA1] SL3 Write Encrypted Maced
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a Security Level 3 command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int Write_EncryptedMaced(	unsigned int uiBlockNo, 
								unsigned char ucLength,
								unsigned char* pucRequestData );

	/// @brief MIFARE Plus Command [0xA2], realized with Reader command [0xC2][0xA2] SL3 Write Plain
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a Security Level 3 command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int Write_Plain(			unsigned int uiBlockNo, 
								unsigned char ucLength,
								unsigned char* pucRequestData );

	/// @brief MIFARE Plus Command [0xA3], realized with Reader command [0xC2][0xA3] SL3 Write Plain Maced
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a Security Level 3 command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int Write_PlainMaced(		unsigned int uiBlockNo, 
								unsigned char ucLength,
								unsigned char* pucRequestData );

	/// @brief MIFARE Plus Command [0xB0], realized with Reader command [0xC2][0xB0] SL3 Increment Encrypted
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a Security Level 3 command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int Increment_Encrypted(		unsigned int uiSourceBlockNo, 
									unsigned int uiData );

	/// @brief MIFARE Plus Command [0xB1], realized with Reader command [0xC2][0xB1] SL3 Increment Encrypted Maced
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a Security Level 3 command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int Increment_EncryptedMaced(	unsigned int uiSourceBlockNo, 
									unsigned int uiData );

	/// @brief MIFARE Plus Command [0xB2], realized with Reader command [0xC2][0xB2] SL3 Decrement Encrypted
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a Security Level 3 command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int Decrement_Encrypted(		unsigned int uiSourceBlockNo, 
									unsigned int uiData );

	/// @brief MIFARE Plus Command [0xB3], realized with Reader command [0xC2][0xB3] SL3 Decrement Encrypted Maced
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a Security Level 3 command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int Decrement_EncryptedMaced(	unsigned int uiSourceBlockNo, 
									unsigned int uiData );

	/// @brief MIFARE Plus Command [0xB4], realized with Reader command [0xC2][0xB4] SL3 Transfer
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a Security Level 3 command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int Transfer(					unsigned int uiDestinationBlockNo );

	/// @brief MIFARE Plus Command [0xB5], realized with Reader command [0xC2][0xB5] SL3 Transfer Maced
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a Security Level 3 command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int Transfer_Maced(				unsigned int uiDestinationBlockNo );

	/// @brief MIFARE Plus Command [0xB6], realized with Reader command [0xC2][0xB6] SL3 Increment Transfer Encrypted
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a Security Level 3 command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int Increment_Transfer_Encrypted(		unsigned int uiSourceBlockNo,
											unsigned int uiDestinationBlockNo,
											unsigned int uiData );

	/// @brief MIFARE Plus Command [0xB7], realized with Reader command [0xC2][0xB7] SL3 Increment Transfer Encrypted Maced
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a Security Level 3 command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int Increment_Transfer_EncryptedMaced(	unsigned int uiSourceBlockNo,
											unsigned int uiDestinationBlockNo,
											unsigned int uiData );

	/// @brief MIFARE Plus Command [0xB8], realized with Reader command [0xC2][0xB8] SL3 Decrement Transfer Encrypted
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a Security Level 3 command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int Decrement_Transfer_Encrypted(		unsigned int uiSourceBlockNo,
											unsigned int uiDestinationBlockNo,
											unsigned int uiData );

	/// @brief MIFARE Plus Command [0xB9], realized with Reader command [0xC2][0xB9] SL3 Decrement Transfer Encrypted Maced
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a Security Level 3 command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int Decrement_Transfer_EncryptedMaced(	unsigned int uiSourceBlockNo,
											unsigned int uiDestinationBlockNo,
											unsigned int uiData );

	/// @brief MIFARE Plus Command [0xC2], realized with Reader command [0xC2][0xC2] SL3 Restore
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a Security Level 3 command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int Restore(					unsigned int uiSourceBlockNo );

	/// @brief MIFARE Plus Command [0xC3], realized with Reader command [0xC2][0xC3] SL3 Restore Maced
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a Security Level 3 command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int Restore_Maced(				unsigned int uiSourceBlockNo );


protected:
	// constructor
	FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3(FEDM_ISCReader* pReader, FEDM_ISOTabItem* pTabItem);
	// 2. constructor
	FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3(FEDM_ISCReader* pReader, unsigned int uiTagHandlerType, FEDM_ISOTabItem* pTabItem);
	// destructor
	virtual ~FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3();

	int Read(	unsigned char ucCmdByte,
				unsigned int uiBlockNo, 
				unsigned char ucExt, 
				unsigned char* pucResponseData );

	int Write(	unsigned char ucCmdByte,
				unsigned int uiBlockNo, 
				unsigned char ucLength, 
				unsigned char* pucRequestData );

};

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#endif // !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_4_MIFARE_PLUS_SL3_H_INCLUDED_)
