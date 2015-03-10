/*---------------------------------------------------------
|                                                         |
|      FedmIscTagHandler_ISO14443_3_MIFARE_Ultralight.cpp |
|                                                         |
-----------------------------------------------------------

Copyright © 2009-2010	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	30.03.2009

Version       		:	03.01.00 / 10.02.2010 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID® classic-pro transponder ISO14443-3 MIFARE Ultralight

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "FedmIscTagHandler_ISO14443_3_MIFARE_Ultralight.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscTagHandler_ISO14443_3_MIFARE_Ultralight
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_ISO14443_3_MIFARE_Ultralight::FedmIscTagHandler_ISO14443_3_MIFARE_Ultralight(
	FEDM_ISCReader* pReader,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_3(pReader, FedmIscTagHandler::TYPE_ISO14443_3_MIFARE_ULTRALIGHT, pTabItem)
{
	m_sTagName = "ISO 14443-3 Type A : MIFARE Ultralight";
}

FedmIscTagHandler_ISO14443_3_MIFARE_Ultralight::FedmIscTagHandler_ISO14443_3_MIFARE_Ultralight(
	FEDM_ISCReader* pReader,
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_3(pReader, uiTagHandlerType, pTabItem)
{
	m_sTagName = "ISO 14443-3 Type A : MIFARE Ultralight";
}

FedmIscTagHandler_ISO14443_3_MIFARE_Ultralight::~FedmIscTagHandler_ISO14443_3_MIFARE_Ultralight()
{
}

// set all data members to 0
/*void FedmIscTagHandler_ISO14443_3_MIFARE_Ultralight::Init()
{
}*/


/***************************************************************************
  Begin        		:	31.03.2009 / M. Hultsch

  Version       	:	03.01.00 / 11.05.2009 / M. Hultsch

  Function			:	[0xB2][0xB2] Authent Mifare Ultralight C with key from reader

						NOTE: only for selected tags

  Parameters		:	unsigned char ucKeyIndex		- [in] reader key index

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_3_MIFARE_Ultralight::Authent(
	unsigned char ucReaderKeyIndex )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B2_CMD, (unsigned char)0xB2));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B2_MODE, (unsigned char)FEDM_ISC_ISO_MODE_SEL));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B2_REQ_KEY_INDEX, ucReaderKeyIndex));

	// ... and execute
	return m_pReader->SendProtocol(0xB2);
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
