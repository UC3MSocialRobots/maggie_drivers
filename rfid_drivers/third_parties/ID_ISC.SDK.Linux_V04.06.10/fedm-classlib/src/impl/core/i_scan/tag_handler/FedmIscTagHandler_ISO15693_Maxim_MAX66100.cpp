/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO15693_Maxim_MAX66100.cpp    |
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
Begin        		:	27.09.2010

Version       		:	03.03.02 / 29.04.2011 / M. Raner, M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID i-scan® HF transponder ISO15693 of manufacturer MAXIM

NOTE				:	this class supports selected, addressed and Nonaddressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)

#include "FedmIscTagHandler_ISO15693_Maxim_MAX66100.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"
#include "../../FEDM_Functions.h"
#include "feisc.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif

FedmIscTagHandler_ISO15693_Maxim_MAX66100::FedmIscTagHandler_ISO15693_Maxim_MAX66100(
	FEDM_ISCReader* pReader,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO15693_Maxim(pReader, FedmIscTagHandler::TYPE_ISO15693_MAXIM_MAX66100, pTabItem)
{
	m_sTagName = "ISO 15693 : ";
	m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_MAXIM);
	m_sTagName += " MAX66100";
}

FedmIscTagHandler_ISO15693_Maxim_MAX66100::FedmIscTagHandler_ISO15693_Maxim_MAX66100(
	FEDM_ISCReader* pReader,
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO15693_Maxim(pReader, uiTagHandlerType, pTabItem)
{
	m_sTagName = "ISO 15693 : ";
	m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_MAXIM);
	m_sTagName += " MAX66100";
}

FedmIscTagHandler_ISO15693_Maxim_MAX66100::~FedmIscTagHandler_ISO15693_Maxim_MAX66100()
{
}

// set all data members to 0
/*void Init()
{
}*/

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)
