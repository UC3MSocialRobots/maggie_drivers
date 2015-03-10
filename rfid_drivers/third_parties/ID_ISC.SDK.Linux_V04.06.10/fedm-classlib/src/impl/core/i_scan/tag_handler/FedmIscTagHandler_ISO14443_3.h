/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO14443_3.h                   |
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

Function			:	class for OBID® classic-pro transponder ISO14443-3

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_3_H_INCLUDED_)
#define _FEDM_ISC_TAG_HANDLER_ISO14443_3_H_INCLUDED_

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "../../FEDM.h"
#include "FedmIscTagHandler_ISO14443_2.h"

class FEDM_ISOTabItem;



//#####################################################################################
// constants
//#####################################################################################





//####################################################################
// class FedmIscTagHandler_ISO14443_3
// >> supports only ISO Host Commands <<
//####################################################################

/// @brief Transponder class providing commands for all ISO 14443-3 compliant Transponders
///
/// <b>Description</b><br>
/// High-level class for easy-to-use Transponder specific communication. See OBID::FEDM::Core::ITagGroup for additional information.<br>
///
/// @note Programmers should be familiar with the ISO 14443 standards
class _FEDM_ISC_CORE_EXT_CLASS FedmIscTagHandler_ISO14443_3 : public FedmIscTagHandler_ISO14443_2
{
public:

	// friend classes have access rights to protected members
	friend class FEDM_ISCReaderModule;
	friend class FEDM_ISCReader;
	friend class FEDM_ISOTabItem;

//	virtual void Init();					// set all data members to 0

	/// @brief Return of Protocol Info, read with last Inventory call
	///
	/// @param[out] ucMaxFrameSize Maximal Frame Size according ISO 14443-3-2001
	/// @param[out] ucProtocolType Protocol Type according ISO 14443-3-2001
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	///
	/// <b>Description</b><br>
	/// See Reader&apos;s System Manual for detailed information about the method parameters
	int GetProtocolInfo(unsigned char& ucMaxFrameSize,
						unsigned char& ucProtocolType );

	/// @brief Return of Card Info, read with last TagSelect call
	///
	/// @param[out] ucCardInfoFormat Reference to Format
	/// @param[out] pucCardInfo Pointer to array for Card Info
	/// @param[in]  uiCardInfoBufferSize size of pucCardInfo
	/// @return 
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 number of bytes in Card Info
	///
	/// <b>Description</b><br>
	/// See Reader&apos;s System Manual for detailed information about the command and the method parameters
	int GetCardInfo(	unsigned char& ucCardInfoFormat,
						unsigned char* pucCardInfo,
						unsigned int   uiCardInfoBufferSize );

	/// @brief [0xB2][0xC0] : Halt command (only for selected Transponder)
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// See Reader&apos;s System Manual for detailed information about the command
	int Halt();

protected:
	// constructor
	FedmIscTagHandler_ISO14443_3(FEDM_ISCReader* pReader, FEDM_ISOTabItem* pTabItem);
	// 2. constructor
	FedmIscTagHandler_ISO14443_3(FEDM_ISCReader* pReader, unsigned int uiTagHandlerType, FEDM_ISOTabItem* pTabItem);
	// destructor
	virtual ~FedmIscTagHandler_ISO14443_3();

};

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#endif // !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_3_H_INCLUDED_)
