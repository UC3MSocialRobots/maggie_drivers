/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO14443_4_Maxim_MAX66000.cpp	|
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
					
Author         		:	Markus Hultsch
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

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "FedmIscTagHandler_ISO14443_4_Maxim_MAX66000.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscTagHandler_ISO14443_Maxim
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_ISO14443_4_Maxim_MAX66000::FedmIscTagHandler_ISO14443_4_Maxim_MAX66000(
	FEDM_ISCReader* pReader, 
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_4_Maxim(pReader, FedmIscTagHandler::TYPE_ISO14443_4_MAXIM_MAX66000, pTabItem)
{
	m_sTagName = "ISO 14443 Type B : Maxim MAX66000";
}

FedmIscTagHandler_ISO14443_4_Maxim_MAX66000::FedmIscTagHandler_ISO14443_4_Maxim_MAX66000(
	FEDM_ISCReader* pReader, 
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_4_Maxim(pReader, uiTagHandlerType, pTabItem)
{
	m_sTagName = "ISO 14443 Type B : Maxim MAX66000";
}

FedmIscTagHandler_ISO14443_4_Maxim_MAX66000::~FedmIscTagHandler_ISO14443_4_Maxim_MAX66000()
{
}

// set all data members to 0
/*void FedmIscTagHandler_ISO14443::Init()
{
}*/

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
