/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO15693_STM_LRIS64K.cpp		|
|                                                       |
---------------------------------------------------------

Copyright © 2009-2012	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	25.09.2012

Version       		:	04.03.01 / 25.09.2012 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID i-scan® HF transponder ISO15693 of manufacturer STMicroelectronics SA
						specialized tag handler on top with custom specific commands

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)

#include "FedmIscTagHandler_ISO15693_STM_LRIS64K.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscTagHandler_ISO15693_STM_LRIS64K
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_ISO15693_STM_LRIS64K::FedmIscTagHandler_ISO15693_STM_LRIS64K(
	FEDM_ISCReader* pReader,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO15693_STM_M24LR64R(pReader, FedmIscTagHandler::TYPE_ISO15693_STM_LRIS64K, pTabItem)
{
	m_sTagName = "ISO 15693 : ";
	m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_STM);
	m_sTagName += " LRIS64K";
}

FedmIscTagHandler_ISO15693_STM_LRIS64K::FedmIscTagHandler_ISO15693_STM_LRIS64K(
	FEDM_ISCReader* pReader,
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO15693_STM_M24LR64R(pReader, uiTagHandlerType, pTabItem)
{
	m_sTagName = "ISO 15693 : ";
	m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_STM);
	m_sTagName += " LRIS64K";
}

FedmIscTagHandler_ISO15693_STM_LRIS64K::~FedmIscTagHandler_ISO15693_STM_LRIS64K()
{
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)
