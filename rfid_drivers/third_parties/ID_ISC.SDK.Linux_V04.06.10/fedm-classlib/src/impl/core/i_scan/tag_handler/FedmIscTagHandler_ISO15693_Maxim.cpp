/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO15693_Maxim.cpp				|
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
Begin        		:	20.09.2010

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

#include "FedmIscTagHandler_ISO15693_Maxim.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"
#include "../../FEDM_Functions.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif

// constructor
FedmIscTagHandler_ISO15693_Maxim::FedmIscTagHandler_ISO15693_Maxim(
	FEDM_ISCReader* pReader,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO15693(pReader, FedmIscTagHandler::TYPE_ISO15693_MAXIM, pTabItem)
{
	m_sTagName = "ISO 15693 : ";
	m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_MAXIM);
}

FedmIscTagHandler_ISO15693_Maxim::FedmIscTagHandler_ISO15693_Maxim(
	FEDM_ISCReader* pReader,
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO15693(pReader, uiTagHandlerType, pTabItem)
{
	m_sTagName = "ISO 15693 : ";
	m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_MAXIM);
}

// destructor
FedmIscTagHandler_ISO15693_Maxim::~FedmIscTagHandler_ISO15693_Maxim()
{
}

// set all data members to 0
/*void Init()
{
}*/

std::string FedmIscTagHandler_ISO15693_Maxim::GetErrorText(unsigned char ucErrorCode)
{
	switch(ucErrorCode)
	{
	case 0x01:
		return "The memory block is write protected";
		break;
	case 0x02: 
		return "The memory block is read protected";
		break;
	case 0x03: 
		return "The memory block is read protected and write protected";
		break;
	case 0x50: 
		return "The memory page involved in the computation did not end with a correct 8-bit CRC";
		break;
	case 0x10: 
		return "Invalid block number";
		break;
	case 0x11: 
		return "Already locked";
		break;
	case 0x12: 
		return "Write access failed because block is locked";
		break;
	case 0x13: 
		return "Power failure during generate secret";
		break;
	case 0xA0: 
		return "Invalid generate secret control code";
		break;
	case 0xA1: 
		return "The computed secret was not locked because secret locking was not enabled";
		break;
	case 0xA2: 
		return "Invalid compute page MAC page number";
		break;
	case 0xA3: 
		return "No valid data in read/write buffer";
		break;
	case 0xA4: 
		return "Copy buffer block number mismatch";
		break;
	case 0xA6: 
		return "Copy buffer invalid MAC";
		break;
	case 0xA7: 
		return "Secret in memory corrupted";
		break;
	case 0xA8: 
		return "The computed secret was not locked because secret locking was not enabled in conjunction with 8-bit page CRC";
		break;
	default:
		return "";
	}
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)
