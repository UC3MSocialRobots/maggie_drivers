/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO14443_4_Maxim_MAX66020.cpp  |
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

Function			:	class for OBID® classic-pro transponder ISO14443 of manufacturer MAXIM

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "FedmIscTagHandler_ISO14443_4_Maxim_MAX66020.h"
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

FedmIscTagHandler_ISO14443_4_Maxim_MAX66020::FedmIscTagHandler_ISO14443_4_Maxim_MAX66020(
	FEDM_ISCReader* pReader, 
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_4_Maxim(pReader, FedmIscTagHandler::TYPE_ISO14443_4_MAXIM_MAX66020, pTabItem)
{
	m_sTagName = "ISO 14443 Type B : Maxim MAX66020";
}

FedmIscTagHandler_ISO14443_4_Maxim_MAX66020::FedmIscTagHandler_ISO14443_4_Maxim_MAX66020(
	FEDM_ISCReader* pReader, 
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_4_Maxim(pReader, uiTagHandlerType, pTabItem)
{
	m_sTagName = "ISO 14443 Type B : Maxim MAX66020";
}

FedmIscTagHandler_ISO14443_4_Maxim_MAX66020::~FedmIscTagHandler_ISO14443_4_Maxim_MAX66020()
{
}

// set all data members to 0
/*void FedmIscTagHandler_ISO14443::Init()
{
}*/


/***************************************************************************
  Begin        		:	06.10.2010 / M. Raner

  Version       	:	03.03.01 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xB2][0xBE][0x20] ReadSingleBlock
  
  Parameters		:	unsigned char	ucBlockNumber		- [in]	number of block(0x00 to 0x12)
						unsigned char&	ucIndicator			- [out]	response indicator byte (if 0x00 then command execution was successful)
						unsigned char*	pucMemoryData		- [out]	readed memory data

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_Maxim_MAX66020::ReadSingleBlock(					
		unsigned char ucBlockNumber,
		unsigned char& ucIndicator,
		unsigned char* pucMemoryData)
{
	//********************************************************************
	// ParameterTesting
	//Test Pointer for NULL
	FEDM_CHK3(pucMemoryData);

	//********************************************************************
	// Declaration
	unsigned int uiLengthOfResponseData = 0;

	//********************************************************************
	// RequestData
	m_ucRequestData[0] = 0x20;	// Command
	m_ucRequestData[1] = ucBlockNumber;

	//********************************************************************
	// CommandExecute
	int iBack = Apdu(m_ucRequestData, 2, m_ucResponseData, 256, uiLengthOfResponseData);
	if(iBack)
		return iBack;

	//********************************************************************
	// ResponseData
	if(uiLengthOfResponseData > 0)
	{
		ucIndicator = m_ucResponseData[0];
		if(ucIndicator == 0)
		{
			for(int iCnt = 0; iCnt < 8; iCnt++)
			{
				pucMemoryData[iCnt] = m_ucResponseData[1 + iCnt];
			}
		}
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	06.10.2010 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xB2][0xBE][0xB0] ReadSingleBlockWithBlockSecurity
  
  Parameters		:	unsigned char	ucBlockNumber		- [in]	number of block(0x00 to 0x12)
						unsigned char&	ucIndicator			- [out]	response indicator byte (if 0x00 then command execution was successful)
						unsigned char&	ucSecurityStatus	- [out]	security status code
						unsigned char*	pucMemoryData		- [out]	readed memory data

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_Maxim_MAX66020::ReadSingleBlockWithBlockSecurity(	
		unsigned char ucBlockNumber,
		unsigned char& ucIndicator,
		unsigned char& ucSecurityStatus,
		unsigned char* pucMemoryData)
{
	//********************************************************************
	// ParameterTesting
	//Test Pointer for NULL
	FEDM_CHK3(pucMemoryData);

	//********************************************************************
	// Declaration
	unsigned int uiLengthOfResponseData = 0;

	//********************************************************************
	// RequestData
	m_ucRequestData[0] = 0xB0;	// Command
	m_ucRequestData[1] = ucBlockNumber;

	//********************************************************************
	// CommandExecute
	int iBack = Apdu(m_ucRequestData, 2, m_ucResponseData, 256, uiLengthOfResponseData);
	if(iBack)
		return iBack;

	//********************************************************************
	// ResponseData
	if(uiLengthOfResponseData > 0)
	{
		ucIndicator = m_ucResponseData[0];
		if(ucIndicator == 0)
		{
			ucSecurityStatus = m_ucResponseData[1];
			for(int iCnt = 0; iCnt < 8; iCnt++)
			{
				pucMemoryData[iCnt] = m_ucResponseData[2 + iCnt];
			}
		}
	}


	return iBack;
}

/***************************************************************************
  Begin        		:	06.10.2010 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xB2][0xBE][0xA4] CustomReadBlock
  
  Parameters		:	unsigned char	ucBlockNumber		- [in]	number of block(0x00 to 0x12)
						unsigned char&	ucIndicator			- [out]	response indicator byte (if 0x00 then command execution was successful)
						unsigned char*	pucMemoryData		- [out]	readed memory data
						unsigned char*	pucIntegrityBytes	- [out]	integrity bytes(write counter) of datablock

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_Maxim_MAX66020::CustomReadBlock(					
		unsigned char ucBlockNumber,
		unsigned char& ucIndicator,
		unsigned char* pucMemoryData,
		unsigned char* pucIntegrityBytes)
{
	//********************************************************************
	// ParameterTesting
	//Test Pointer for NULL
	FEDM_CHK3(pucMemoryData);
	FEDM_CHK3(pucIntegrityBytes);

	//********************************************************************
	// Declaration
	unsigned int uiLengthOfResponseData = 0;

	//********************************************************************
	// RequestData
	m_ucRequestData[0] = 0xA4;	// Command
	m_ucRequestData[1] = ucBlockNumber;

	//********************************************************************
	// CommandExecute
	int iBack = Apdu(m_ucRequestData, 2, m_ucResponseData, 256, uiLengthOfResponseData);
	if(iBack)
		return iBack;

	//********************************************************************
	// ResponseData
	if(uiLengthOfResponseData > 0)
	{
		ucIndicator = m_ucResponseData[0];
		if(ucIndicator == 0)
		{
			for(int iCnt = 0; iCnt < 8; iCnt++)
			{
				pucMemoryData[iCnt] = m_ucResponseData[1 + iCnt];
			}
			pucIntegrityBytes[0] = m_ucResponseData[9];
			pucIntegrityBytes[1] = m_ucResponseData[10];
		}
	}

	return iBack;
}


/*******************************************************************************
  Begin        		:	06.10.2010 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xB2][0xBE][0x27] WriteAFI
  
  Parameters		:	unsigned char	ucAfiValue			- [in]	new AFI Value that will be set on tag
						unsigned char&	ucIndicator			- [out]	response indicator byte (if 0x00 then command execution was successful)

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
*******************************************************************************/ 
int FedmIscTagHandler_ISO14443_4_Maxim_MAX66020::WriteAFI(							
		unsigned char ucAfiValue,
		unsigned char& ucIndicator)
{
	//********************************************************************
	// Declaration
	unsigned int uiLengthOfResponseData = 0;

	//********************************************************************
	// RequestData
	m_ucRequestData[0] = 0x27;	// Command
	m_ucRequestData[1] = ucAfiValue;

	//********************************************************************
	// CommandExecute
	int iBack = Apdu(m_ucRequestData, 2, m_ucResponseData, 256, uiLengthOfResponseData);
	if(iBack)
		return iBack;

	//********************************************************************
	// ResponseData
	if(uiLengthOfResponseData > 0)
	{
		ucIndicator = m_ucResponseData[0];
	}

	return iBack;
}

/*******************************************************************************
  Begin        		:	06.10.2010 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xB2][0xBE][0x28] LockAFI
  
  Parameters		:	unsigned char&	ucIndicator			- [out]	response indicator byte (if 0x00 then command execution was successful)

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
*******************************************************************************/ 
int FedmIscTagHandler_ISO14443_4_Maxim_MAX66020::LockAFI(							
		unsigned char& ucIndicator)
{
	//********************************************************************
	// Declaration
	unsigned int uiLengthOfResponseData = 0;

	//********************************************************************
	// RequestData
	m_ucRequestData[0] = 0x28;	// Command

	//********************************************************************
	// CommandExecute
	int iBack = Apdu(m_ucRequestData, 1, m_ucResponseData, 256, uiLengthOfResponseData);
	if(iBack)
		return iBack;

	//********************************************************************
	// ResponseData
	if(uiLengthOfResponseData > 0)
	{
		ucIndicator = m_ucResponseData[0];
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	21.04.2011 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xB2][0xBE][0x21] WriteSingleBlock
  
  Parameters		:	unsigned char	ucBlockNumber		- [in]	number of block(0x00 to 0x12)
						unsigned char&	ucIndicator			- [out]	response indicator byte (if 0x00 then command execution was successful)
						unsigned char*	pucNewBlockData		- [out]	the new block data, expect to be written into choosen memory block

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_Maxim_MAX66020::WriteSingleBlock(					
	unsigned char ucBlockNumber,
	unsigned char* pucNewBlockData,
	unsigned char& ucIndicator)
{
	//********************************************************************
	// ParameterTesting
	//Test Pointer for NULL
	FEDM_CHK3(pucNewBlockData)


	//********************************************************************
	// Declaration
	unsigned int uiLengthOfResponseData = 0;

	//********************************************************************
	// RequestData
	m_ucRequestData[0] = 0x21;	// Command
	m_ucRequestData[1] = ucBlockNumber;

	//new block data
	for(int i = 0; i < 8; i++)
	{
		m_ucRequestData[2 + i] = pucNewBlockData[i];
	}

	//********************************************************************
	// CommandExecute
	int iBack = Apdu(m_ucRequestData, 10, m_ucResponseData, 256, uiLengthOfResponseData);
	if(iBack)
		return iBack;

	//********************************************************************
	// ResponseData
	if(uiLengthOfResponseData > 0)
	{
		ucIndicator = m_ucResponseData[0];
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	29.04.2011 / M. Raner

  Version       	:	03.03.02 / 29.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xB2][0xBE][0x22] LockBlock
  
  Parameters		:	unsigned char	ucBlockNumber		- [in]	number of block(0x00 to 0x12)
						unsigned char&	ucIndicator			- [out]	response indicator byte (if 0x00 then command execution was successful)

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_Maxim_MAX66020::LockBlock(					
		unsigned char ucBlockNumber,
		unsigned char& ucIndicator)
{
	//********************************************************************
	// Declaration
	unsigned int uiLengthOfResponseData = 0;

	//********************************************************************
	// RequestData
	m_ucRequestData[0] = 0x22;	// Command
	m_ucRequestData[1] = ucBlockNumber;

	//********************************************************************
	// CommandExecute
	int iBack = Apdu(m_ucRequestData, 2, m_ucResponseData, 256, uiLengthOfResponseData);
	if(iBack)
		return iBack;

	//********************************************************************
	// ResponseData
	if(uiLengthOfResponseData > 0)
	{
		ucIndicator = m_ucResponseData[0];
	}

	return iBack;
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
