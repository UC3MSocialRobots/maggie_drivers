/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO18000_3M3.h					|
|                                                       |
---------------------------------------------------------

Copyright © 2012		FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	30.03.2009

Version       		:	04.03.01 / 15.11.2012 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID i-scan® HF transponder ISO 18000-3M3

NOTE				:	this class supports only addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_TAG_HANDLER_ISO18000_3M3_H_INCLUDED_)
#define _FEDM_ISC_TAG_HANDLER_ISO18000_3M3_H_INCLUDED_

#if !defined(_FEDM_NO_TAG_HANDLER_ISO18000_3M3)

#include "../../FEDM.h"
#include "FedmIscTagHandler_EPC_Class1_Gen2.h"

class FEDM_ISOTabItem;


//#####################################################################################
// constants
//#####################################################################################





//####################################################################
// class FedmIscTagHandler_ISO18000_3M3
// >> supports only ISO Host Commands <<
//####################################################################

/// @brief Transponder class providing basic commands for all ISO 18000-3M3 compliant Transponders
///
/// <b>Description</b><br>
/// High-level class for easy-to-use Transponder specific communication. See OBID::FEDM::Core::ITagGroup for additional information.<br>
///
/// @note Programmers should be familiar with the EPC Class 1 Gen 2 standard (<A HREF="http:///www.epcglobal.org">www.epcglobal.org</A>)
/// as ISO 18000-3M3 is based on this standard
class _FEDM_ISC_CORE_EXT_CLASS FedmIscTagHandler_ISO18000_3M3 : public FedmIscTagHandler_EPC_Class1_Gen2
{
public:

	// friend classes have access rights to protected members
	friend class FEDM_ISCReaderModule;
	friend class FEDM_ISCReader;
	friend class FEDM_ISOTabItem;

//	virtual void Init();					// set all data members to 0

protected:
	// constructor
	FedmIscTagHandler_ISO18000_3M3(FEDM_ISCReader* pReader, FEDM_ISOTabItem* pTabItem);
	// 2. constructor
	FedmIscTagHandler_ISO18000_3M3(FEDM_ISCReader* pReader, unsigned int uiTagHandlerType, FEDM_ISOTabItem* pTabItem);
	// destructor
	virtual ~FedmIscTagHandler_ISO18000_3M3();

};

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO18000_3M3)

#endif // !defined(_FEDM_ISC_TAG_HANDLER_ISO18000_3M3_H_INCLUDED_)
