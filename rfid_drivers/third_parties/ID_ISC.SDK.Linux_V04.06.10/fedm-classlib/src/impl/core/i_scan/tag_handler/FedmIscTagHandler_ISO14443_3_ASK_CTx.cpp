/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO14443_3_ASK_CTx.cpp         |
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
Begin        		:	06.08.2012

Version       		:	04.02.06 / 06.08.2012 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID® classic-pro transponder ISO14443-3 ASK CTx

NOTE				:	this class supports only selected mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "FedmIscTagHandler_ISO14443_3_ASK_CTx.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscTagHandler_ISO14443_3_ASK_CTx
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_ISO14443_3_ASK_CTx::FedmIscTagHandler_ISO14443_3_ASK_CTx(
	FEDM_ISCReader* pReader, 
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_3(pReader, FedmIscTagHandler::TYPE_ISO14443_3, pTabItem)
{
	if(m_pTabItem != NULL)
	{
		m_sTagName = "ISO 14443-3 ASK CTx";
	}
}

FedmIscTagHandler_ISO14443_3_ASK_CTx::FedmIscTagHandler_ISO14443_3_ASK_CTx(
	FEDM_ISCReader* pReader, 
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_3(pReader, uiTagHandlerType, pTabItem)
{
	if(m_pTabItem != NULL)
	{
		m_sTagName = "ISO 14443-3 ASK CTx";
	}
}

FedmIscTagHandler_ISO14443_3_ASK_CTx::~FedmIscTagHandler_ISO14443_3_ASK_CTx()
{
}

unsigned char FedmIscTagHandler_ISO14443_3_ASK_CTx::GetProductCode()
{
	return m_pTabItem->m_ucProductCode;
}

unsigned char FedmIscTagHandler_ISO14443_3_ASK_CTx::GetFabCode()
{
	return m_pTabItem->m_ucFabCode;
}

unsigned char FedmIscTagHandler_ISO14443_3_ASK_CTx::GetAppCode()
{
	return m_pTabItem->m_ucAppCode;
}

unsigned char FedmIscTagHandler_ISO14443_3_ASK_CTx::GetEmbedderCode()
{
	return m_pTabItem->m_ucEmbedderCode;
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
