/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO14443_3_MIFARE_Plus.cpp     |
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
Begin        		:	25.08.2009

Version       		:	03.01.00 / 10.02.2010 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID® classic-pro transponder ISO14443-3 MIFARE Plus

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "FedmIscTagHandler_ISO14443_3_MIFARE_Plus.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscTagHandler_ISO14443_3_MIFARE_Plus
// >> supports only ISO Host Commands <<
//####################################################################

	// public constructor not necessary, as this class represents not a real tag type
/*FedmIscTagHandler_ISO14443_3_MIFARE_Plus::FedmIscTagHandler_ISO14443_3_MIFARE_Plus(
	FEDM_ISCReader* pReader, 
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_3_MIFARE_Classic(pReader, FedmIscTagHandler::TYPE_ISO14443_3, pTabItem)
{
	m_sTagName = "ISO 14443-3 Type A : MIFARE Plus";

	Init();
}*/

FedmIscTagHandler_ISO14443_3_MIFARE_Plus::FedmIscTagHandler_ISO14443_3_MIFARE_Plus(
	FEDM_ISCReader* pReader, 
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_3_MIFARE_Classic(pReader, uiTagHandlerType, pTabItem)
{
//	m_sTagName = "ISO 14443-3 Type A : MIFARE Plus";

	Init();
}

FedmIscTagHandler_ISO14443_3_MIFARE_Plus::~FedmIscTagHandler_ISO14443_3_MIFARE_Plus()
{
}

// set all data members to 0
void FedmIscTagHandler_ISO14443_3_MIFARE_Plus::Init()
{
	FedmIscTagHandler::Init();

	m_ucErrorSource	= 0;
	m_uiErrorCode	= 0;
}

unsigned char FedmIscTagHandler_ISO14443_3_MIFARE_Plus::GetErrorSource()
{
	return m_ucErrorSource;
}

unsigned int FedmIscTagHandler_ISO14443_3_MIFARE_Plus::GetErrorCode()
{
	return m_uiErrorCode;
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
