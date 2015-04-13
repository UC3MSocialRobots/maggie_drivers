/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO14443_2.cpp                 |
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

Function			:	class for OBID® classic-pro transponder ISO14443-2

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "FedmIscTagHandler_ISO14443_2.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscTagHandler_ISO14443_2
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_ISO14443_2::FedmIscTagHandler_ISO14443_2(
	FEDM_ISCReader* pReader,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443(pReader, FedmIscTagHandler::TYPE_ISO14443_2, pTabItem)
{
	if(m_pTabItem != NULL)
	{
		if(m_pTabItem->m_ucTrType == FEDM_ISC_TR_TYPE_ISO14443A)
			m_sTagName = "ISO 14443-2 Type A";
		else if(m_pTabItem->m_ucTrType == FEDM_ISC_TR_TYPE_ISO14443B)
			m_sTagName = "ISO 14443-2 Type B";
	}
}

FedmIscTagHandler_ISO14443_2::FedmIscTagHandler_ISO14443_2(
	FEDM_ISCReader* pReader,
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443(pReader, uiTagHandlerType, pTabItem)
{
	if(m_pTabItem != NULL)
	{
		if(m_pTabItem->m_ucTrType == FEDM_ISC_TR_TYPE_ISO14443A)
			m_sTagName = "ISO 14443-2 Type A";
		else if(m_pTabItem->m_ucTrType == FEDM_ISC_TR_TYPE_ISO14443B)
			m_sTagName = "ISO 14443-2 Type B";
	}
}

FedmIscTagHandler_ISO14443_2::~FedmIscTagHandler_ISO14443_2()
{
}

// set all data members to 0
/*void FedmIscTagHandler_ISO14443_2::Init()
{
}*/

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
