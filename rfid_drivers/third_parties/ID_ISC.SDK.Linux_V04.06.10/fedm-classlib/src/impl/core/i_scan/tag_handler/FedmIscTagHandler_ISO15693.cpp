/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO15693.cpp                   |
|                                                       |
---------------------------------------------------------

Copyright © 2009-2011	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	30.03.2009

Version       		:	03.03.02 / 02.05.2011 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID i-scan® HF transponder ISO15693

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)

#include "FedmIscTagHandler_ISO15693.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"
#include "../../FEDM_Functions.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscTagHandler_ISO15693
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_ISO15693::FedmIscTagHandler_ISO15693(
	FEDM_ISCReader* pReader,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler(pReader, FedmIscTagHandler::TYPE_ISO15693, pTabItem)
{
	m_sTagName = "ISO 15693";

	Init();
}

FedmIscTagHandler_ISO15693::FedmIscTagHandler_ISO15693(
	FEDM_ISCReader* pReader,
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler(pReader, uiTagHandlerType, pTabItem)
{
	m_sTagName = "ISO 15693";

	Init();
}

FedmIscTagHandler_ISO15693::~FedmIscTagHandler_ISO15693()
{
}

// set all data members to 0
void FedmIscTagHandler_ISO15693::Init()
{
	FedmIscTagHandler::Init();
}


const char* FedmIscTagHandler_ISO15693::GetManufacturerName()
{
	return m_pTabItem->GetISO15693Manufacturer();
}

/***************************************************************************
  Begin        		:	31.03.2009 / M. Hultsch

  Version       	:	03.03.02 / 02.05.2011 / M. Hultsch

  Function			:	basic [0xB0][0x02] Stay Quiet command

  Parameters		:	-

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693::StayQuiet()
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE, (unsigned char)0));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char)0x02));

    m_ucISOErrorCode = 0;
	
	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB0);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B0_ISO_ERROR, &m_ucISOErrorCode));
	}

	return iBack;
}


/***************************************************************************
  Begin        		:	31.03.2009 / M. Hultsch

  Version       	:	03.03.02 / 02.05.2011 / M. Hultsch

  Function			:	basic [0xB0][0x22] Lock Multiple Blocks command
						without support for extended address mode

  Parameters		:	unsigned int uiFirstDataBlock		- [in] first data block address
						unsigned int uiNoOfDataBlocks		- [in] number of requested data blocks

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693::LockMultipleBlocks(
	unsigned int uiFirstDataBlock,
	unsigned int uiNoOfDataBlocks )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else
	{
		if(m_pTabItem->m_bIsSelected)
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_SEL));
		}
		else
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
		}
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DB_ADR, uiFirstDataBlock));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DBN, uiNoOfDataBlocks));
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char)0x22));

    m_ucISOErrorCode = 0;

	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB0);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B0_ISO_ERROR, &m_ucISOErrorCode));
	}

	return iBack;
}


/***************************************************************************
  Begin        		:	31.03.2009 / M. Hultsch

  Version       	:	03.03.02 / 02.05.2011 / M. Hultsch

  Function			:	basic [0xB0][0x26] Reset to Ready command

  Parameters		:	-

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693::ResetToReady()
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else
	{
		if(m_pTabItem->m_bIsSelected)
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_SEL));
		}
		else
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
		}
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char)0x26));

    m_ucISOErrorCode = 0;
	
	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB0);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B0_ISO_ERROR, &m_ucISOErrorCode));
	}

	return iBack;
}


/***************************************************************************
  Begin        		:	31.03.2009 / M. Hultsch

  Version       	:	03.03.02 / 02.05.2011 / M. Hultsch

  Function			:	basic [0xB0][0x27] Write AFI command

  Parameters		:	unsigned char ucAFI	- [in] AFI value

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693::WriteAFI(unsigned char ucAFI)
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else
	{
		if(m_pTabItem->m_bIsSelected)
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_SEL));
		}
		else
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
		}
	}

	m_pTabItem->m_ucAFI = ucAFI;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char)0x27));

    m_ucISOErrorCode = 0;
	
	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB0);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B0_ISO_ERROR, &m_ucISOErrorCode));
	}

	return iBack;
}


/***************************************************************************
  Begin        		:	31.03.2009 / M. Hultsch

  Version       	:	03.03.02 / 02.05.2011 / M. Hultsch

  Function			:	basic [0xB0][0x28] Lock AFI command

  Parameters		:	-

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693::LockAFI()
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else
	{
		if(m_pTabItem->m_bIsSelected)
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_SEL));
		}
		else
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
		}
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char)0x28));

    m_ucISOErrorCode = 0;
	
	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB0);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B0_ISO_ERROR, &m_ucISOErrorCode));
	}

	return iBack;
}


/***************************************************************************
  Begin        		:	31.03.2009 / M. Hultsch

  Version       	:	03.03.02 / 02.05.2011 / M. Hultsch

  Function			:	basic [0xB0][0x29] Write DSFID command

  Parameters		:	unsigned char ucDSFID	- [in] DSFID value

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693::WriteDSFID(unsigned char ucDSFID)
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else
	{
		if(m_pTabItem->m_bIsSelected)
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_SEL));
		}
		else
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
		}
	}

	m_pTabItem->m_ucDsfID = ucDSFID;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char)0x29));

    m_ucISOErrorCode = 0;
	
	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB0);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B0_ISO_ERROR, &m_ucISOErrorCode));
	}

	return iBack;
}


/***************************************************************************
  Begin        		:	31.03.2009 / M. Hultsch

  Version       	:	03.03.02 / 02.05.2011 / M. Hultsch

  Function			:	basic [0xB0][0x2A] Lock DSFID command

  Parameters		:	-

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693::LockDSFID()
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else
	{
		if(m_pTabItem->m_bIsSelected)
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_SEL));
		}
		else
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
		}
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char)0x2A));

    m_ucISOErrorCode = 0;
	
	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB0);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B0_ISO_ERROR, &m_ucISOErrorCode));
	}

	return iBack;
}


/***************************************************************************
  Begin        		:	31.03.2009 / M. Hultsch

  Version       	:	03.03.02 / 02.05.2011 / M. Hultsch

  Function			:	basic [0xB0][0x2B] Get System Information command
						without support for extended address mode

  Parameters		:	string& sUid				- [out] UID
						unsigned char& ucAFI		- [out] AFI
						unsigned int& uiMemSize		- [out] memory size
						unsigned char& ucICRef		- [out] IC reference

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693::GetSystemInformation(
	string& sUid,
	unsigned char& ucAFI,
	unsigned int& uiMemSize,
	unsigned char& ucICRef )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else
	{
		if(m_pTabItem->m_bIsSelected)
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_SEL));
		}
		else
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
		}
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char)0x2B));

    m_ucISOErrorCode = 0;
	
	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB0);
	if(iBack == 0)
	{
		FEDM_ConvHexUCharToHexStr(m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen, sUid);
		ucAFI = m_pTabItem->m_ucAFI;
		uiMemSize = (((unsigned int)m_pTabItem->m_ucMemSize[0]) << 8) + m_pTabItem->m_ucMemSize[1];
		ucICRef = m_pTabItem->m_ucICRef;
	}
	else if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B0_ISO_ERROR, &m_ucISOErrorCode));
	}

	return iBack;
}


/***************************************************************************
  Begin        		:	31.03.2009 / M. Hultsch

  Version       	:	03.03.02 / 02.05.2011 / M. Hultsch

  Function			:	basic [0xB0][0x2C] Get Multiple Block Security Status command
						without support for extended address mode

  Parameters		:	unsigned int uiFirstDataBlock		- [in] first data block address
						unsigned int uiNoOfDataBlocks		- [in] number of requested data blocks
						unsigned char* pucSecurityStatus	- [out] buffer with security info

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693::GetMultipleBlockSecurityStatus(
	unsigned int uiFirstDataBlock,
	unsigned int uiNoOfDataBlocks,
	unsigned char* pucSecurityStatus )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else
	{
		if(m_pTabItem->m_bIsSelected)
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_SEL));
		}
		else
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
		}
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DB_ADR, uiFirstDataBlock));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DBN, uiNoOfDataBlocks));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char)0x2C));

    m_ucISOErrorCode = 0;
	
	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB0);
	if(iBack == 0)
	{
		// save received data blocks in internal memory
		memcpy(pucSecurityStatus, &m_pTabItem->m_ucSecStatus[uiFirstDataBlock], uiNoOfDataBlocks);
	}
	else if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B0_ISO_ERROR, &m_ucISOErrorCode));
	}

	return iBack;
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)
