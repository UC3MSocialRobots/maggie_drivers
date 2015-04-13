/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO14443_4_Maxim.cpp           |
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

#include "FedmIscTagHandler_ISO14443_4_Maxim.h"
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

FedmIscTagHandler_ISO14443_4_Maxim::FedmIscTagHandler_ISO14443_4_Maxim(
	FEDM_ISCReader* pReader, 
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_4(pReader, FedmIscTagHandler::TYPE_ISO14443_4_MAXIM, pTabItem)
{
	m_sTagName = "ISO 14443 Type B : Maxim";
}

FedmIscTagHandler_ISO14443_4_Maxim::FedmIscTagHandler_ISO14443_4_Maxim(
	FEDM_ISCReader* pReader, 
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_4(pReader, uiTagHandlerType, pTabItem)
{
	m_sTagName = "ISO 14443 Type B : Maxim";
}

FedmIscTagHandler_ISO14443_4_Maxim::~FedmIscTagHandler_ISO14443_4_Maxim()
{
}

// set all data members to 0
/*void FedmIscTagHandler_ISO14443_4_Maxim::Init()
{
}*/


/***************************************************************************
  Begin        		:	06.10.2010 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	Get System Information of Tag
  
  Parameters		:	unsigned char&	ucIndicator			- [out]	response indicator byte (if 0x00 then command execution was successful)
						unsigned char&	ucInfoFlags			- [out] info flags of tag
						unsigned char*	ucUID				- [out] uid of tag
						unsigned char&	ucU1				- [out] U1 Byte from Memory
						unsigned char&	ucAFI				- [out]	AFI Byte from Memory
						unsigned char&	ucNumberOfBlocks	- [out] Number of memory blocks aviable on tag
						unsigned char&	ucMemoryBlockSize	- [out] size of a single memory block
						unsigned char&	ucIcReference		- [out] revision of tag such as 0xA1, 0xA2, 0xB1, etc.

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_Maxim::GetSystemInformation(				
		unsigned char& ucIndicator,
		unsigned char& ucInfoFlags,
		unsigned char* ucUID,
		unsigned char& ucU1,
		unsigned char& ucAFI,
		unsigned char& ucNumberOfBlocks,
		unsigned char& ucMemoryBlockSize,
		unsigned char& ucIcReference)
{
	//********************************************************************
	// Declaration
	unsigned char ucRequestData[1];
	unsigned char ucResponseData[256];
	unsigned int uiLengthOfResponseData = 0;

	//********************************************************************
	// RequestData
	ucRequestData[0] = 0x2B;	// Command

	//********************************************************************
	// CommandExecute
	int iBack = Apdu(ucRequestData, 1, ucResponseData, 256, uiLengthOfResponseData);
	if(iBack)
		return iBack;

	//********************************************************************
	// ResponseData
	if(uiLengthOfResponseData > 0)
	{
		ucIndicator = ucResponseData[0];
		if(ucIndicator == 0)
		{
			ucInfoFlags = ucResponseData[1];
			for(int i = 0; i < 8; i++)
			{
				ucUID[i] = ucResponseData[2+i];
			}
			ucU1 = ucResponseData[10];
			ucAFI = ucResponseData[11];
			ucNumberOfBlocks = ucResponseData[12];
			ucMemoryBlockSize = ucResponseData[13];
			ucIcReference = ucResponseData[14];
		}
	}

	return iBack;
}


/*******************************************************************************
  Begin        		:	06.10.2010 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xB2][0xBE][0x30] GetUID
  
  Parameters		:	unsigned char&	ucIndicator			- [out]	response indicator byte (if 0x00 then command execution was successful)
						unsigned char*	pucUID				- [out]	UID of tag

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
*******************************************************************************/ 
int FedmIscTagHandler_ISO14443_4_Maxim::GetUID(
		unsigned char& ucIndicator,
		unsigned char* pucUID)
{
	//********************************************************************
	// ParameterTesting
	//Test Pointer for NULL
	FEDM_CHK3(pucUID);

	//********************************************************************
	// Declaration
	unsigned char ucRequestData[1];
	unsigned char ucResponseData[256];
	unsigned int uiLengthOfResponseData = 0;

	//********************************************************************
	// RequestData
	ucRequestData[0] = 0x30;	// Command

	//********************************************************************
	// CommandExecute
	int iBack = Apdu(ucRequestData, 1, ucResponseData, 256, uiLengthOfResponseData);
	if(iBack != 0)
    {
        return iBack;
    }

	//********************************************************************
	// ResponseData
	if(uiLengthOfResponseData > 0)
	{
		ucIndicator = ucResponseData[0];
		if(ucIndicator == 0)
		{
			for(int iCnt = 0; iCnt < 8; iCnt++)
			{
				pucUID[iCnt] = ucResponseData[1 + iCnt];
			}
		}
	}

	return iBack;
}

std::string FedmIscTagHandler_ISO14443_4_Maxim::GetErrorText(unsigned char ucIndicator)
{
	switch(ucIndicator)
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

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
