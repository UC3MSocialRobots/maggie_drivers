/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO15693_Infineon.cpp          |
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
Begin        		:	02.10.2009

Version       		:	03.01.00 / 10.02.2010 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID i-scan® HF transponder ISO15693 of manufacturer Infineon Technologies AG
						specialized tag handler on top with custom specific commands

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)

#include "FedmIscTagHandler_ISO15693_Infineon.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"
#include "../../FEDM_Functions.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscTagHandler_ISO15693_Infineon
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_ISO15693_Infineon::FedmIscTagHandler_ISO15693_Infineon(
	FEDM_ISCReader* pReader,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO15693(pReader, FedmIscTagHandler::TYPE_ISO15693_Infineon, pTabItem)
{
	m_sTagName = "ISO 15693 : ";
	m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_INFINEON);
}

FedmIscTagHandler_ISO15693_Infineon::FedmIscTagHandler_ISO15693_Infineon(
	FEDM_ISCReader* pReader,
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO15693(pReader, uiTagHandlerType, pTabItem)
{
	m_sTagName = "ISO 15693 : ";
	m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_INFINEON);
}

FedmIscTagHandler_ISO15693_Infineon::~FedmIscTagHandler_ISO15693_Infineon()
{
}

// set all data members to 0
/*void FedmIscTagHandler_ISO15693_Infineon::Init()
{
}*/

/***************************************************************************
  Begin        		:	09.10.2009 / M. Hultsch

  Version       	:	03.01.00 / 24.11.2009 / M. Hultsch

  Function			:	[0xB1][0x10] Read

  Parameters		:

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_Infineon::Read(
	unsigned int uiFirstDataBlock, 
	unsigned int& uiNoOfDataBlocks,
	unsigned int& uiBlockSize,
	unsigned char* pucSecurityStatus,
	unsigned char* pucData)
{
	int iErr = 0;
	int iBack=0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0x10));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_SEL));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_INFINEON_DB_ADR, uiFirstDataBlock));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_INFINEON_DB_N, uiNoOfDataBlocks));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_INFINEON));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB1);
	if(iBack == 0x00)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B1_RSP_INFINEON_DB_N, &uiNoOfDataBlocks));
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B1_RSP_INFINEON_DB_SIZE, &uiBlockSize));
		memcpy(pucSecurityStatus, &m_pTabItem->m_ucSecStatus[uiFirstDataBlock], uiNoOfDataBlocks);
		memcpy(pucData, &m_pTabItem->m_ucRxDB[uiFirstDataBlock * uiBlockSize], uiNoOfDataBlocks * uiBlockSize);
	}

	return iBack;
}
	
/***************************************************************************
  Begin        		:	09.10.2009 / M. Hultsch

  Version       	:	03.01.00 / 12.10.2009 / M. Hultsch

  Function			:	[0xB1][0x30] Write

  Parameters		:

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_Infineon::Write(
	unsigned int uiFirstDataBlock, 
	unsigned int uiNoOfDataBlocks, 
	unsigned int uiBlockSize, 
	unsigned char* pucData)
{
	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0x30));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_SEL));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_INFINEON_DB_ADR, uiFirstDataBlock));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_INFINEON_DB_N, uiNoOfDataBlocks));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_INFINEON_DB_SIZE, uiBlockSize));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_INFINEON_DB_SIZE, uiBlockSize));

	memcpy(&m_pTabItem->m_ucTxDB[uiFirstDataBlock * uiBlockSize], pucData, uiNoOfDataBlocks * uiBlockSize);

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_INFINEON));
	
	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB1);
	if(iBack == 0x03 || iBack == 0x95)
	{
		// if write error, save db address which causes the error
		// one or two byte db address is supported
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B1_RSP_DB_ADR_E, &m_uiErrorDBAddress));
	}

	return iBack;
}
	
/***************************************************************************
  Begin        		:	09.10.2009 / M. Hultsch

  Version       	:	03.01.00 / 12.10.2009 / M. Hultsch

  Function			:	[0xB1][0x90] WriteByte

  Parameters		:

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_Infineon::WriteByte(unsigned int uiFirstDataBlock, unsigned int uiNoOfDataBlocks)
{
	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0x90));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_SEL));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_INFINEON_DB_ADR, uiFirstDataBlock));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_INFINEON_DB_N, uiNoOfDataBlocks));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_INFINEON));
	
	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB1);
	if(iBack == 0x03 || iBack == 0x95)
	{
		// if write error, save db address which causes the error
		// one or two byte db address is supported
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B1_RSP_DB_ADR_E, &m_uiErrorDBAddress));
	}

	return iBack;
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)
