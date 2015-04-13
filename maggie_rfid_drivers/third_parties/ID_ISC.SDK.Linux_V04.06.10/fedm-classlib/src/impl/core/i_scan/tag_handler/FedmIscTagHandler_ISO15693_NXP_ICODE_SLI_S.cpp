/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_S.cpp   |
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

Version       		:	03.02.04 / 19.08.2010 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID i-scan® HF transponder ISO15693 of manufacturer NXP Semiconductors
						specialized tag handler on top with custom specific commands

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)

#include "FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_S.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"
#include "../../FEDM_Functions.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_S
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_S::FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_S(
	FEDM_ISCReader* pReader,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_L(pReader, FedmIscTagHandler::TYPE_ISO15693_NXP_ICODE_SLI_S, pTabItem)
{
	m_sTagName = "ISO 15693 : ";
	m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_NXP);
	m_sTagName += " I-Code SLI-S";
}

FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_S::FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_S(
	FEDM_ISCReader* pReader,
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_L(pReader, uiTagHandlerType, pTabItem)
{
	m_sTagName = "ISO 15693 : ";
	m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_NXP);
	m_sTagName += " I-Code SLI-S";
}

FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_S::~FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_S()
{
}

// set all data members to 0
/*void FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_S::Init()
{
}*/

/***************************************************************************
  Begin        		:	09.10.2009 / M. Hultsch

  Version       	:	03.02.04 / 19.08.2010 / M. Hultsch

  Function			:	[0xB1][0xA8] ReadEPC

  Parameters		:	unsigned char* ucEpc		- pointer to epc buffer
						unsigned int uiEpcBufferLen	- length of epc buffer

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_S::ReadEPC(
	unsigned char* ucEpc,
	unsigned int uiEpcBufferLen )
{
	FEDM_CHK3(ucEpc);

	if(uiEpcBufferLen < 12)
		return FEDM_ERROR_BUFFER_LENGTH;

	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xA8));
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

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xA8));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_NXP));
	
	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB1);
	if(iBack)
		return iBack;

	FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B1_RSP_NXP_EPC_DATA, ucEpc, 12));

	return FEDM_OK;
}

/***************************************************************************
  Begin        		:	09.10.2009 / M. Hultsch

  Version       	:	03.01.00 / 09.10.2009 / M. Hultsch

  Function			:	[0xB1][0xB6] ProtectPage

  Parameters		:	unsigned char ucPageNumber,unsigned char ucProtectionStatus

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_S::ProtectPage(
	unsigned char ucPageNumber,
	unsigned char ucProtectionStatus )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xB6));
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
		
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_NXP_PAGE_NO, ucPageNumber));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_NXP_PROTECTION_STATUS, ucProtectionStatus));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xB6));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_NXP));
	
	// ... and execute
	return m_pReader->SendProtocol(0xB1);
}

/***************************************************************************
  Begin        		:	09.10.2009 / M. Hultsch

  Version       	:	03.01.00 / 09.10.2009 / M. Hultsch

  Function			:	[0xB1][0xB7] LockPageProtectionCondition

  Parameters		:	unsigned char ucPageNumber

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_S::LockPageProtectionCondition(
	unsigned char ucPageNumber )
{
	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xB7));
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
		
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_NXP_PAGE_NO, ucPageNumber));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xB7));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_NXP));
	
	// ... and execute
	return m_pReader->SendProtocol(0xB1);
}

/***************************************************************************
  Begin        		:	09.10.2009 / M. Hultsch

  Version       	:	03.01.00 / 09.10.2009 / M. Hultsch

  Function			:	[0xB1][0xB8] GetMultipleBlockProtectionStatus

  Parameters		:	unsigned char ucDBAdr, unsigned char ucDBN

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_S::GetMultipleBlockProtectionStatus(
	unsigned int uiFirstDataBlock,
	unsigned int uiNoOfDataBlocks,
	unsigned char* pucSecurityStatus )
{
	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xB8));
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
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_NXP_DB_ADR, uiFirstDataBlock));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_NXP_DB_N, uiNoOfDataBlocks));


	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xB8));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_NXP));
	
	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB1);
	if(iBack == 0)
	{
		// save received data blocks in internal memory
		memcpy(pucSecurityStatus, &m_pTabItem->m_ucSecStatus[uiFirstDataBlock], uiNoOfDataBlocks);
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	09.10.2009 / M. Hultsch

  Version       	:	03.01.00 / 09.10.2009 / M. Hultsch

  Function			:	[0xB1][0xB9] DestroySLI_S

  Parameters		:	

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_S::DestroySLI_S()
{	
	// ... and execute
	return DestroySLI_L();
}

/***************************************************************************
  Begin        		:	09.10.2009 / M. Hultsch

  Version       	:	03.01.00 / 09.10.2009 / M. Hultsch

  Function			:	[0xB1][0xBB] PasswordProtection

  Parameters		:	unsigned char ucPasswordParameter

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_S::PasswordProtection(
	unsigned char ucPasswordParameter )
{
	int iErr = 0;	

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xBB));
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

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_NXP_64BIT_PWD_PARAMETER, ucPasswordParameter));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xBB));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_NXP));
	
	// ... and execute
	return m_pReader->SendProtocol(0xB1);
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)
