/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO18000_3M3.cpp				|
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

Version       		:	04.03.01 / 14.11.2012 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID i-scan® HF transponder ISO 18000-3M3

NOTE				:	this class supports only addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_NO_TAG_HANDLER_ISO18000_3M3)

#include "FedmIscTagHandler_ISO18000_3M3.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"
#include "../../FEDM_Functions.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif


//####################################################################
// class FedmIscTagHandler_ISO18000_3M3
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_ISO18000_3M3::FedmIscTagHandler_ISO18000_3M3(
	FEDM_ISCReader* pReader,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_EPC_Class1_Gen2(pReader, FedmIscTagHandler::TYPE_ISO18000_3M3, pTabItem)
{
	m_sTagName = "ISO 18000-3M3";
}

// protected constructor for internal use
FedmIscTagHandler_ISO18000_3M3::FedmIscTagHandler_ISO18000_3M3(
	FEDM_ISCReader* pReader,
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem)
	: FedmIscTagHandler_EPC_Class1_Gen2(pReader, uiTagHandlerType, pTabItem)
{
	m_sTagName = "ISO 18000-3M3";
}

FedmIscTagHandler_ISO18000_3M3::~FedmIscTagHandler_ISO18000_3M3()
{
}

// set all data members to 0
/*void FedmIscTagHandler_ISO18000_3M3::Init()
{
}*/

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_EPC_C1_G2)
