/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO14443_3_Infineon_my_d.h     |
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
Begin        		:	03.04.2009

Version       		:	03.01.00 / 10.02.2010 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID® classic-pro transponder ISO14443-3 Infineon my-d

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_3_INFINEON_MY_D_H_INCLUDED_)
#define _FEDM_ISC_TAG_HANDLER_ISO14443_3_INFINEON_MY_D_H_INCLUDED_

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "../../FEDM.h"
#include "FedmIscTagHandler_ISO14443_3.h"

class FEDM_ISOTabItem;



//#####################################################################################
// constants
//#####################################################################################





//####################################################################
// class FedmIscTagHandler_ISO14443_3_Infineon_my_d
// >> supports only ISO Host Commands <<
//####################################################################

/// @brief Transponder class providing manufacturer specific commands for ISO 14443-3 compliant Transponders of type my-d from manufacturer Infineon Technologies AG
///
/// <b>Description</b><br>
/// High-level class for easy-to-use Transponder specific communication. See OBID::FEDM::Core::ITagGroup for additional information.<br>
///
/// @note Programmers should be familiar with the ISO 14443 standards
class _FEDM_ISC_CORE_EXT_CLASS FedmIscTagHandler_ISO14443_3_Infineon_my_d : public FedmIscTagHandler_ISO14443_3
{
public:

	// friend classes have access rights to protected members
	friend class FEDM_ISCReaderModule;
	friend class FEDM_ISCReader;
	friend class FEDM_ISOTabItem;

//	virtual void Init();					// set all data members to 0


	/// @brief Authentication command
	///
	/// @param[in] ucKeyTagAddress Address where the key is stored in the Transponder. The address can have any value between 4 and 31.
	/// @param[in] ucAuthentCounterAddress Address of the Authentication Counter of the Transponder (Default: 0x03).
	/// @param[in] ucAuthentSequence Defines the authentication sequence.<br>
	/// - 0x00: Accelerated card authentication<br>
	/// - 0x01: Entire card authentication<br>
	/// @param[in] ucKeyAddressSAM Address of the key in the security access module (SAM). The address can have any value between 1 and 28.
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
    /// The Authent my-d command is realized with the Reader command [0xB2][0xB1] ISO14443 Special Host Commands.<br>
	int Authent(	unsigned char ucKeyTagAddress,
					unsigned char ucAuthentCounterAddress,
					unsigned char ucAuthentSequence,
					unsigned char ucKeyAddressSAM );


protected:
	// constructor
	FedmIscTagHandler_ISO14443_3_Infineon_my_d(FEDM_ISCReader* pReader, FEDM_ISOTabItem* pTabItem);
	// 2. constructor
	FedmIscTagHandler_ISO14443_3_Infineon_my_d(FEDM_ISCReader* pReader, unsigned int uiTagHandlerType, FEDM_ISOTabItem* pTabItem);
	// destructor
	virtual ~FedmIscTagHandler_ISO14443_3_Infineon_my_d();

};

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#endif // !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_3_INFINEON_MY_D_H_INCLUDED_)
