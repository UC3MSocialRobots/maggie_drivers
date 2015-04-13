/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO15693_Maxim_MAX66120.cpp    |
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

#include "FedmIscTagHandler_ISO15693_Maxim_MAX66120.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"
#include "../../FEDM_Functions.h"
#include "feisc.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif

FedmIscTagHandler_ISO15693_Maxim_MAX66120::FedmIscTagHandler_ISO15693_Maxim_MAX66120(
	FEDM_ISCReader* pReader,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO15693_Maxim(pReader, FedmIscTagHandler::TYPE_ISO15693_MAXIM_MAX66120, pTabItem)
{
	m_sTagName = "ISO 15693 : ";
	m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_MAXIM);
	m_sTagName += " MAX66120";
}

FedmIscTagHandler_ISO15693_Maxim_MAX66120::FedmIscTagHandler_ISO15693_Maxim_MAX66120(
	FEDM_ISCReader* pReader,
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO15693_Maxim(pReader, uiTagHandlerType, pTabItem)
{
	m_sTagName = "ISO 15693 : ";
	m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_MAXIM);
	m_sTagName += " MAX66120";
}

FedmIscTagHandler_ISO15693_Maxim_MAX66120::~FedmIscTagHandler_ISO15693_Maxim_MAX66120()
{
}

// set all data members to 0
/*void Init()
{
}*/

/***************************************************************************
  Begin        		:	21.09.2010 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xBF][0x20] ReadSingleBlock
  
  Parameters		:	unsigned char ucRequestFlags		- [in]	request flags byte
						unsigned char ucBlockNumber			- [in]	number of block(0x00 to 0x12)
						unsigned char& ucResponseFlags		- [out]	response flags(if 0x00 then command execution was successful)
						unsigned char& ucSecurityStatus		- [out]	security status code
						unsigned char* pucMemoryData		- [out]	readed memory data

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_Maxim_MAX66120::ReadSingleBlock(
						unsigned char ucRequestFlags,
						unsigned char ucBlockNumber,
						unsigned char& ucResponseFlags,
						unsigned char& ucSecurityStatus, /* <<<<<<<<<<<< Optional */
						unsigned char* pucMemoryData)
{
	//******************************************************************************
	// ParameterTesting
	//Test Pointer for NULL
	FEDM_CHK3(pucMemoryData);

	//Check if request flags are addressed and selected
	if(((ucRequestFlags & 0x30) == 0x30))
	{
		//Error: Flags = Addressed and Selected
		return FEDM_ERROR_PARAMETER;
	}

	//Check BlockNumber for valid values
	if(!(ucBlockNumber >= 0x00 && ucBlockNumber <= 0x12))
	{
		//Error: block number not valid
		return FEDM_ERROR_PARAMETER;
	}

	//******************************************************************************
	// Declaration
	int iLengthOfRequestData = 5;
	if((ucRequestFlags & 0x20) == 0x20)
	{
		iLengthOfRequestData += 8;	//Addressed mode, length with uid
	}

	int iLengthOfResponseData = 0;
	int iResponseDataOffset = 0;


	//******************************************************************************
	// RequestData
	m_ucRequestData[0] = 0x02;	//command response delay time = 0x201f * 590ns = 320.9us
	m_ucRequestData[1] = 0x1f;

	m_ucRequestData[2] = ucRequestFlags & 0x73;	//important request flags
	m_ucRequestData[3] = 0x20;					//command Code 

	//if addressed mode then uid
	if((ucRequestFlags & 0x20) == 0x20)
	{
		for(int iCnt = 0; iCnt < 8; iCnt++)
		{
			m_ucRequestData[11 - iCnt] = m_pTabItem->m_ucSnr[iCnt];	//uid
		}
	}

	//block number
	m_ucRequestData[iLengthOfRequestData - 1] = ucBlockNumber;


	//******************************************************************************
	// CommandExecute
	int iBack = FEISC_0xBF_ISOTranspCmd(	m_pReader->GetReaderHnd(), 
											m_pReader->GetBusAddress(), 
											0x02, 
											0x58, 
											m_ucRequestData, 
											iLengthOfRequestData, 
											m_ucResponseData, 
											&iLengthOfResponseData, 
											0);

	if(iBack == 0x95)
	{
		m_ucISO15693ErrorCode = m_ucResponseData[0];
	}
	else if(iBack == 0)
	{
		//******************************************************************************
		// ResponseData
		ucResponseFlags = m_ucResponseData[0];
		if(ucResponseFlags == 0)
		{
			if((ucRequestFlags & 0x40) == 0x40)
			{
				ucSecurityStatus = m_ucResponseData[1];
				iResponseDataOffset++;
			}
			for(int iCnt = 0; iCnt < 8; iCnt++)
			{
				pucMemoryData[iCnt] = m_ucResponseData[1 + iResponseDataOffset +iCnt];
			}
		}
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	21.09.2010 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xBF][0xA4] CustomReadBlock
  
  Parameters		:	unsigned char ucRequestFlags		- [in]	request flags byte
						unsigned char ucBlockNumber			- [in]	number of block(0x00 to 0x12)
						unsigned char& ucResponseFlags		- [out]	response flags(if 0x00 then command execution was successful)
						unsigned char& ucSecurityStatus		- [out]	security status code
						unsigned char* pucMemoryData		- [out]	readed memory data
						unsigned char* pucIntegrityBytes	- [out]	integrity bytes(write counter) of datablock

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_Maxim_MAX66120::CustomReadBlock(
						unsigned char ucRequestFlags,
						unsigned char ucBlockNumber,
						unsigned char& ucResponseFlags,
						unsigned char& ucSecurityStatus, /* <<<<<<<<<<<< Optional */
						unsigned char* pucMemoryData,
						unsigned char* pucIntegrityBytes)
{
	//******************************************************************************
	// ParameterTesting
	//Test Pointer for NULL
	FEDM_CHK3(pucMemoryData);
	FEDM_CHK3(pucIntegrityBytes);

	//Check if request flags are addressed and selected
	if(((ucRequestFlags & 0x30) == 0x30))
	{
		//Error: Flags = Addressed and Selected
		return FEDM_ERROR_PARAMETER;
	}

	//Check BlockNumber for valid values
	if(!(ucBlockNumber >= 0x00 && ucBlockNumber <= 0x12))
	{
		//Error: block number not valid
		return FEDM_ERROR_PARAMETER;
	}


	//******************************************************************************
	// Declaration
	int iLengthOfRequestData = 6;
	if((ucRequestFlags & 0x20) == 0x20)
	{
		iLengthOfRequestData += 8;	//Addressed mode, length with uid
	}

	int iLengthOfResponseData = 0;
	int iResponseDataOffset = 0;


	//******************************************************************************
	// RequestData
	m_ucRequestData[0] = 0x02;	//command response delay time = 0x021f * 590ns = 320.9us
	m_ucRequestData[1] = 0x1f;

	m_ucRequestData[2] = ucRequestFlags & 0x73;	//important request flags
	m_ucRequestData[3] = 0xA4;					//command Code 
	m_ucRequestData[4] = 0x2B;					//MFG Code

	//if addressed mode then uid
	if((ucRequestFlags & 0x20) == 0x20)
	{
		for(int iCnt = 0; iCnt < 8; iCnt++)
		{
			m_ucRequestData[12 - iCnt] = m_pTabItem->m_ucSnr[iCnt];	//uid
		}
	}

	//block number
	m_ucRequestData[iLengthOfRequestData - 1] = ucBlockNumber;


	//******************************************************************************
	// CommandExecute
	int iBack = FEISC_0xBF_ISOTranspCmd(	m_pReader->GetReaderHnd(), 
											m_pReader->GetBusAddress(), 
											0x02, 
											0x68, 
											m_ucRequestData, 
											iLengthOfRequestData, 
											m_ucResponseData, 
											&iLengthOfResponseData, 
											0);
	if(iBack == 0x95)
	{
		m_ucISO15693ErrorCode = m_ucResponseData[0];
	}
	else if(iBack == 0)
	{
		//******************************************************************************
		// ResponseData
		ucResponseFlags = m_ucResponseData[0];
		if(ucResponseFlags == 0)
		{
			if((ucRequestFlags & 0x40) == 0x40)
			{
				ucSecurityStatus = m_ucResponseData[1];
				iResponseDataOffset++;
			}
			for(int iCnt = 0; iCnt < 8; iCnt++)
			{
				pucMemoryData[iCnt] = m_ucResponseData[1 + iResponseDataOffset +iCnt];
			}
			pucIntegrityBytes[0] = m_ucResponseData[9 + iResponseDataOffset];
			pucIntegrityBytes[1] = m_ucResponseData[10 + iResponseDataOffset];
		}
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	21.04.2011 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xBF][0x21] WriteSingleBlock
  
  Parameters		:	unsigned char ucRequestFlags		- [in]	request flags byte
						unsigned char ucBlockNumber			- [in]	number of block(0x00 to 0x12)
						unsigned char* pucNewBlockData		- [out]	the new block data, expect to be written into choosen memory block

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_Maxim_MAX66120::WriteSingleBlock(
	unsigned char ucRequestFlags,
	unsigned char ucBlockNumber,
	unsigned char* pucNewBlockData,
	unsigned char& ucResponseFlags)
{
	//******************************************************************************
	// ParameterTesting
	//Test Pointer for NULL
	FEDM_CHK3(pucNewBlockData)

	//Check if request flags are addressed and selected
	if(((ucRequestFlags & 0x30) == 0x30))
	{
		//Error: Flags = Addressed and Selected
		return FEDM_ERROR_PARAMETER;
	}

	//Check BlockNumber for valid values
	if(!(ucBlockNumber >= 0x00 && ucBlockNumber <= 0x11))
	{
		//Error: block number not valid
		return FEDM_ERROR_PARAMETER;
	}


	//******************************************************************************
	// Declaration
	int iLengthOfRequestData = 13;
	if((ucRequestFlags & 0x20) == 0x20)
	{
		iLengthOfRequestData += 8;	//Addressed mode, length with uid
	}

	int iLengthOfResponseData = 0;
	int iResponseDataOffset = 0;


	//******************************************************************************
	// RequestData
	m_ucRequestData[0] = 0x02;	//command response delay time = 0x0240 * 590ns = 339.84us
	m_ucRequestData[1] = 0x40;

	m_ucRequestData[2] = ucRequestFlags & 0x73;	//important request flags
	m_ucRequestData[3] = 0x21;					//command Code 

	//if addressed mode then uid
	if((ucRequestFlags & 0x20) == 0x20)
	{
		for(int iCnt = 0; iCnt < 8; iCnt++)
		{
			m_ucRequestData[11 - iCnt] = m_pTabItem->m_ucSnr[iCnt];	//uid
		}
	}

	//block number
	m_ucRequestData[iLengthOfRequestData - 9] = ucBlockNumber;

	//new block data
	for(int i = 0; i < 8; i++)
	{
		m_ucRequestData[iLengthOfRequestData - 8 + i] = pucNewBlockData[i];
	}


	//******************************************************************************
	// CommandExecute
	int iBack = FEISC_0xBF_ISOTranspCmd(	m_pReader->GetReaderHnd(), 
											m_pReader->GetBusAddress(), 
											0x02, 
											0x18, 
											m_ucRequestData, 
											iLengthOfRequestData, 
											m_ucResponseData, 
											&iLengthOfResponseData, 
											0);

	if(iBack == 0x95)
	{
		m_ucISO15693ErrorCode = m_ucResponseData[0];
	}
	else if(iBack == 0)
	{
		//******************************************************************************
		// ResponseData
		ucResponseFlags = m_ucResponseData[0];
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	29.04.2011 / M. Raner

  Version       	:	03.03.02 / 29.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xBF][0x22] LockBlock
  
  Parameters		:	unsigned char ucRequestFlags		- [in]	request flags byte
						unsigned char ucBlockNumber			- [in]	number of block(0x00 to 0x12)
						unsigned char& ucResponseFlags		- [out]	response flags(if 0x00 then command execution was successful)

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_Maxim_MAX66120::LockBlock(
	unsigned char ucRequestFlags,
	unsigned char ucBlockNumber,
	unsigned char& ucResponseFlags)
{
	//Check if request flags are addressed and selected
	if(((ucRequestFlags & 0x30) == 0x30))
	{
		//Error: Flags = Addressed and Selected
		return FEDM_ERROR_PARAMETER;
	}

	//Check BlockNumber for valid values
	if(!(ucBlockNumber >= 0x00 && ucBlockNumber <= 0x12))
	{
		//Error: block number not valid
		return FEDM_ERROR_PARAMETER;
	}

	//******************************************************************************
	// Declaration
	int iLengthOfRequestData = 5;
	if((ucRequestFlags & 0x20) == 0x20)
	{
		iLengthOfRequestData += 8;	//Addressed mode, length with uid
	}

	int iLengthOfResponseData = 0;
	int iResponseDataOffset = 0;


	//******************************************************************************
	// RequestData
	m_ucRequestData[0] = 0x02;	//command response delay time = 0x201f * 590ns = 320.9us
	m_ucRequestData[1] = 0x40;

	m_ucRequestData[2] = ucRequestFlags & 0x73;	//important request flags
	m_ucRequestData[3] = 0x22;					//command Code 

	//if addressed mode then uid
	if((ucRequestFlags & 0x20) == 0x20)
	{
		for(int iCnt = 0; iCnt < 8; iCnt++)
		{
			m_ucRequestData[11 - iCnt] = m_pTabItem->m_ucSnr[iCnt];	//uid
		}
	}

	//block number
	m_ucRequestData[iLengthOfRequestData - 1] = ucBlockNumber;


	//******************************************************************************
	// CommandExecute
	int iBack = FEISC_0xBF_ISOTranspCmd(	m_pReader->GetReaderHnd(), 
											m_pReader->GetBusAddress(), 
											0x02, 
											0x18, 
											m_ucRequestData, 
											iLengthOfRequestData, 
											m_ucResponseData, 
											&iLengthOfResponseData, 
											0);

	if(iBack == 0x95)
	{
		m_ucISO15693ErrorCode = m_ucResponseData[0];
	}
	else if(iBack == 0)
	{
		//******************************************************************************
		// ResponseData
		ucResponseFlags = m_ucResponseData[0];
	}

	return iBack;
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)
