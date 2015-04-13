/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO14443_4_MIFARE_DESFire.cpp  |
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
Begin        		:	30.03.2009

Version       		:	03.02.05 / 20.09.2010 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID® classic-pro transponder ISO14443-4 MIFARE DESFire

NOTE				:	this class supports only selected mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


/*############### Questions ##################################################
- CID und NAD für T=CL?

*/

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "FedmIscTagHandler_ISO14443_4_MIFARE_DESFire.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif


const unsigned int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire::PLAIN_COMMUNICATION		= 0x00;
const unsigned int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire::PLAIN_SECURED_BY_MAC	= 0x01;
const unsigned int FedmIscTagHandler_ISO14443_4_MIFARE_DESFire::FULLY_ENCIPHERED		= 0x03;


//####################################################################
// class FedmIscTagHandler_ISO14443_4_MIFARE_DESFire
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_ISO14443_4_MIFARE_DESFire::FedmIscTagHandler_ISO14443_4_MIFARE_DESFire(
	FEDM_ISCReader* pReader,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_4(pReader, FedmIscTagHandler::TYPE_ISO14443_4_MIFARE_DESFIRE, pTabItem)
{
	m_sTagName = "ISO 14443-4 Type A : MIFARE DESFire";

	m_pImpl_C1 = new FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1(pTabItem, this);
	m_pImpl_C3 = new FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3(pTabItem, this);

	ISoftCrypto.m_pParent		= this;
	IFlexSoftCrypto.m_pParent	= this;
	ISamCrypto.m_pParent		= this;

	Init();
}

FedmIscTagHandler_ISO14443_4_MIFARE_DESFire::FedmIscTagHandler_ISO14443_4_MIFARE_DESFire(
	FEDM_ISCReader* pReader,
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_4(pReader, uiTagHandlerType, pTabItem)
{
	m_sTagName = "ISO 14443-4 Type A : MIFARE DESFire";

	m_pImpl_C1 = new FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1(pTabItem, this);
	m_pImpl_C3 = new FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3(pTabItem, this);

	ISoftCrypto.m_pParent		= this;
	IFlexSoftCrypto.m_pParent	= this;
	ISamCrypto.m_pParent		= this;

	Init();
}

FedmIscTagHandler_ISO14443_4_MIFARE_DESFire::~FedmIscTagHandler_ISO14443_4_MIFARE_DESFire()
{
	delete m_pImpl_C1;
	delete m_pImpl_C3;
}

// set all data members to 0
void FedmIscTagHandler_ISO14443_4_MIFARE_DESFire::Init()
{
	FedmIscTagHandler::Init();
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
