/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO15693_Maxim.cpp           |
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
Begin        		:	20.09.2010

Version       		:	03.03.02 / 02.05.2011 / M. Raner

Operation Systems	:	independent

Function			:	class for OBID i-scan® HF transponder ISO15693 of manufacturer MAXIM

NOTE				:	this class supports selected, addressed and Nonaddressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_TAG_HANDLER_ISO15693_MAXIM_H_INCLUDED_)
#define _FEDM_ISC_TAG_HANDLER_ISO15693_MAXIM_H_INCLUDED_

#if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)

#include "../../FEDM.h"
#include "FedmIscTagHandler_ISO15693.h"

class FEDM_ISOTabItem;

/// @brief Transponder class providing manufacturer specific commands for ISO 15693 compliant Transponders from manufacturer Maxim
///
/// <b>Description</b><br>
/// High-level class for easy-to-use Transponder specific communication. See OBID::FEDM::Core::ITagGroup for additional information.<br>
///
/// @note Programmers should be familiar with the ISO 15693 standard
class _FEDM_ISC_CORE_EXT_CLASS FedmIscTagHandler_ISO15693_Maxim : public FedmIscTagHandler_ISO15693
{
public:

	// friend classes have access rights to protected members
	friend class FEDM_ISCReaderModule;
	friend class FEDM_ISCReader;
	friend class FEDM_ISOTabItem;

	// set all data members to 0
	//virtual void Init();

	/// @brief Return of an error text
	///
	/// <b>Description</b><br>
	/// This method returns an error text for a Maxim specific error code
	static std::string GetErrorText(unsigned char ucErrorCode);

protected:
	// constructor
	FedmIscTagHandler_ISO15693_Maxim(FEDM_ISCReader* pReader, FEDM_ISOTabItem* pTabItem);
	// 2. constructor
	FedmIscTagHandler_ISO15693_Maxim(FEDM_ISCReader* pReader, unsigned int uiTagHandlerType, FEDM_ISOTabItem* pTabItem);
	// destructor
	virtual ~FedmIscTagHandler_ISO15693_Maxim();

};

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)

#endif // !defined(_FEDM_ISC_TAG_HANDLER_ISO15693_MAXIM_H_INCLUDED_)