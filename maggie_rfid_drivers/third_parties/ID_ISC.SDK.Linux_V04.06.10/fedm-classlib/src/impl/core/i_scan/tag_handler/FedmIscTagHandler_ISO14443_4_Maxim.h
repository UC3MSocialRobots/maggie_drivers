/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO14443_4_Maxim.h             |
|                                                       |
---------------------------------------------------------

Copyright © 2010-2011	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Marcel Raner
Begin        		:	04.10.2010

Version       		:	03.03.02 / 29.04.2011 / M. Raner, M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID® classic-pro transponder ISO14443

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_4_MAXIM_H_INCLUDED_)
#define _FEDM_ISC_TAG_HANDLER_ISO14443_4_MAXIM_H_INCLUDED_

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "../../FEDM.h"
#include "FedmIscTagHandler_ISO14443_4.h"

class FEDM_ISOTabItem;



//#####################################################################################
// constants
//#####################################################################################





//####################################################################
// class FedmIscTagHandler_ISO14443_Maxim
// >> supports only ISO Host Commands <<
//####################################################################

/// @brief Transponder class providing manufacturer specific commands for ISO 14443-4 compliant Transponders from manufacturer Maxim
///
/// <b>Description</b><br>
/// High-level class for easy-to-use Transponder specific communication. See OBID::FEDM::Core::ITagGroup for additional information.<br>
///
/// @note Programmers should be familiar with the ISO 14443 standards
class _FEDM_ISC_CORE_EXT_CLASS FedmIscTagHandler_ISO14443_4_Maxim : public FedmIscTagHandler_ISO14443_4
{
public:

	// friend classes have access rights to protected members
	friend class FEDM_ISCReaderModule;
	friend class FEDM_ISCReader;
	friend class FEDM_ISOTabItem;

//	virtual void Init();					// set all data members to 0

	/// @brief Command [0x2B]: Get System Information of Tag
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is realized with Reader command [0xB2][0xBE] : T=CL %APDU for synchronous call.<br>
	/// Detailed description about the command and the parameters can be found in:<br>
	/// - Product Datasheet for MAX66000, MAX66020 or MAX 66040 from MAXIM
	int GetSystemInformation(			unsigned char &ucIndicator,
										unsigned char &ucInfoFlags,
										unsigned char* ucUID,
										unsigned char &ucU1,
										unsigned char &ucAFI,
										unsigned char &ucNumberOfBlocks,
										unsigned char &ucMemoryBlockSize,
										unsigned char &ucIcReference);

	/// @brief Command [0x30]: Get UID of Tag
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is realized with Reader command [0xB2][0xBE] : T=CL %APDU for synchronous call.<br>
	/// Detailed description about the command and the parameters can be found in:<br>
	/// - Product Datasheet for MAX66000, MAX66020 or MAX 66040 from MAXIM
	int GetUID(							unsigned char& ucIndicator,
										unsigned char* pucUID);

	/// @brief Return a text according the given error code
	///
	/// @param[in] ucIndicator Error value
	/// @return Error message text
	///
	/// <b>Description</b><br>
	static std::string GetErrorText(	unsigned char ucIndicator);

protected:
	// constructor
	FedmIscTagHandler_ISO14443_4_Maxim(FEDM_ISCReader* pReader, FEDM_ISOTabItem* pTabItem);
	// 2. constructor
	FedmIscTagHandler_ISO14443_4_Maxim(FEDM_ISCReader* pReader, unsigned int uiTagHandlerType, FEDM_ISOTabItem* pTabItem);
	// destructor
	virtual ~FedmIscTagHandler_ISO14443_4_Maxim();

};

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#endif // !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_4_MAXIM_H_INCLUDED_)
