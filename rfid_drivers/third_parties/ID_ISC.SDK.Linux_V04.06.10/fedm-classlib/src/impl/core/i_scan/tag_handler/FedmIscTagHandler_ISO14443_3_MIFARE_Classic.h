/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO14443_3_MIFARE_Classic.h    |
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

Version       		:	03.01.00 / 10.02.2010 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID® classic-pro transponder ISO14443-3 MIFARE Classic (Mini, 1K, 4K)

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_3_MIFARE_CLASSIC_H_INCLUDED_)
#define _FEDM_ISC_TAG_HANDLER_ISO14443_3_MIFARE_CLASSIC_H_INCLUDED_

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "../../FEDM.h"
#include "FedmIscTagHandler_ISO14443_3.h"

class FEDM_ISOTabItem;



//#####################################################################################
// constants
//#####################################################################################





//####################################################################
// class FedmIscTagHandler_ISO14443_3_MIFARE_Classic
// >> supports only ISO Host Commands <<
//####################################################################

/// @brief Transponder class providing manufacturer specific commands for ISO 14443-3 compliant Transponders of type MIFARE Classic (Mini, 1K, 4K) from manufacturer NXP Semiconductors
///
/// <b>Description</b><br>
/// High-level class for easy-to-use Transponder specific communication. See OBID::FEDM::Core::ITagGroup for additional information.<br>
///
/// @note Programmers should be familiar with the ISO 14443 standards
class _FEDM_ISC_CORE_EXT_CLASS FedmIscTagHandler_ISO14443_3_MIFARE_Classic : public FedmIscTagHandler_ISO14443_3
{
public:

	// friend classes have access rights to protected members
	friend class FEDM_ISCReaderModule;
	friend class FEDM_ISCReader;
	friend class FEDM_ISOTabItem;

//	virtual void Init();					// set all data members to 0


	/// @brief Value command
	///
	/// @param[in] ucCommand First data block address
	/// @param[in] ucSourceAddress Sourec address
	/// @param[in] uiValue Value
	/// @param[in] ucDestinationAddress Destination address
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
    /// The Value command is realized with the Reader command [0xB2][0x30] ISO14443 Special Host Commands and is applicable only with a selected Transponder.<br>
	/// See Reader&apos;s System Manual for a detailed description of this command.<br>
	int ValueCommand(	unsigned char ucCommand,
						unsigned char ucSourceAddress,
						unsigned int uiValue,
						unsigned char ucDestinationAddress );

	/// @brief Authent Mifare with key from Reader&apos;s EEPROM
	///
	/// @param[in] ucFirstDataBlock First data block address
	/// @param[in] ucKeyType Key type
	/// @param[in] ucKeyAddress Key address
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
    /// The Authent command is realized with the Reader command [0xB2][0xB0] ISO14443 Special Host Commands and is applicable only with a selected Transponder.<br>
	/// See Reader&apos;s System Manual for a detailed description of this command.<br>
	int Authent(		unsigned char ucFirstDataBlock,
						unsigned char ucKeyType,
						unsigned char ucKeyAddress );

	/// @brief Authent Mifare with key from protocol
	///
	/// @param[in] ucFirstDataBlock First data block address
	/// @param[in] ucKeyType Key type
	/// @param[in] sKey Authentication key
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
    /// The Authent command is realized with the Reader command [0xB2][0xB0] ISO14443 Special Host Commands and is applicable only with a selected Transponder.<br>
	/// See Reader&apos;s System Manual for a detailed description of this command.<br>
	int Authent(		unsigned char ucFirstDataBlock,
						unsigned char ucKeyType,
						string sKey );

protected:
	// constructor
	FedmIscTagHandler_ISO14443_3_MIFARE_Classic(FEDM_ISCReader* pReader, FEDM_ISOTabItem* pTabItem);
	// 2. constructor
	FedmIscTagHandler_ISO14443_3_MIFARE_Classic(FEDM_ISCReader* pReader, unsigned int uiTagHandlerType, FEDM_ISOTabItem* pTabItem);
	// destructor
	virtual ~FedmIscTagHandler_ISO14443_3_MIFARE_Classic();

};

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#endif // !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_3_MIFARE_CLASSIC_H_INCLUDED_)
