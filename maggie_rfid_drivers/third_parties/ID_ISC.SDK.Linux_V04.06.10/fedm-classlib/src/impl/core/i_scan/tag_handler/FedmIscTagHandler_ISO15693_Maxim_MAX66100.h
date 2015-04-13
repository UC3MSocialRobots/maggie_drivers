/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO15693_Maxim_MAX66100.h		|
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
					
Author         		:	Marcel Raner
Begin        		:	27.09.2010

Version       		:	03.02.05 / 27.09.2010 / M. Raner

Operation Systems	:	independent

Function			:	class for OBID i-scan® HF transponder ISO15693 of manufacturer MAXIM

NOTE				:	this class supports selected, addressed and Nonaddressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_TAG_HANDLER_ISO15693_MAXIM_MAX66100_H_INCLUDED_)
#define _FEDM_ISC_TAG_HANDLER_ISO15693_MAXIM_MAX66100_H_INCLUDED_

#if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)

#include "../../FEDM.h"
#include "FedmIscTagHandler_ISO15693_Maxim.h"

class FEDM_ISOTabItem;

/*
	The Transponder MAC66100 have no memory and also no memory functions.
*/

/// @brief Transponder class providing manufacturer specific commands for ISO 15693 compliant Transponders of type MAX66100 from manufacturer Maxim
///
/// <b>Description</b><br>
/// High-level class for easy-to-use Transponder specific communication. See OBID::FEDM::Core::ITagGroup for additional information.<br>
///
/// @note Programmers should be familiar with the ISO 15693 standard
class _FEDM_ISC_CORE_EXT_CLASS FedmIscTagHandler_ISO15693_Maxim_MAX66100 : public FedmIscTagHandler_ISO15693_Maxim
{
public:

	// friend classes have access rights to protected members
	friend class FEDM_ISCReaderModule;
	friend class FEDM_ISCReader;
	friend class FEDM_ISOTabItem;

	// set all data members to 0
	//virtual void Init();

	

protected:
	// constructor
	FedmIscTagHandler_ISO15693_Maxim_MAX66100(FEDM_ISCReader* pReader, FEDM_ISOTabItem* pTabItem);
	// 2. constructor
	FedmIscTagHandler_ISO15693_Maxim_MAX66100(FEDM_ISCReader* pReader, unsigned int uiTagHandlerType, FEDM_ISOTabItem* pTabItem);
	// destructor
	virtual ~FedmIscTagHandler_ISO15693_Maxim_MAX66100();

};

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)

#endif // !defined(_FEDM_ISC_TAG_HANDLER_ISO15693_MAXIM_MAX66100_H_INCLUDED_)