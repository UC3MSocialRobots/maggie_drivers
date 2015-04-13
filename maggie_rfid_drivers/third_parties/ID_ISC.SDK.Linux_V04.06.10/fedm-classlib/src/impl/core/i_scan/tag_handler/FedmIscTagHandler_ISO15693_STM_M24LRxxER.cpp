/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO15693_STM_M24LRxxER.cpp     |
|                                                       |
---------------------------------------------------------

Copyright © 2009-2012	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	25.09.2012

Version       		:	04.03.01 / 25.09.2012 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID i-scan® HF transponder ISO15693 of manufacturer STMicroelectronics SA
						specialized tag handler on top with custom specific commands

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)

#include "FedmIscTagHandler_ISO15693_STM_M24LRxxER.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"
#include "../../FEDM_Functions.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscTagHandler_ISO15693_STM_M24LRxxER
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_ISO15693_STM_M24LRxxER::FedmIscTagHandler_ISO15693_STM_M24LRxxER(
	FEDM_ISCReader* pReader,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO15693_STM_M24LR64R(pReader, FedmIscTagHandler::TYPE_ISO15693_STM_M24LR64R, pTabItem)
{
	if( (pTabItem->m_ucSnr[2]&0xFC) == 0x5C )	// M24LR64E-R
	{
		m_sTagName = "ISO 15693 : ";
		m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_STM);
		m_sTagName += " M24LR64E-R";
	}
	else if( (pTabItem->m_ucSnr[2]&0xFC) == 0x4C )	// M24LR16E-R
	{
		m_sTagName = "ISO 15693 : ";
		m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_STM);
		m_sTagName += " M24LR16E-R";
	}
	else if( (pTabItem->m_ucSnr[2]&0xFC) == 0x58 )	// M24LR04E-R
	{
		m_sTagName = "ISO 15693 : ";
		m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_STM);
		m_sTagName += " M24LR04E-R";
	}
	else
	{
		m_sTagName = "ISO 15693 : ";
		m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_STM);
		m_sTagName += " M24LRxxE-R";
	}
}

FedmIscTagHandler_ISO15693_STM_M24LRxxER::FedmIscTagHandler_ISO15693_STM_M24LRxxER(
	FEDM_ISCReader* pReader,
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO15693_STM_M24LR64R(pReader, uiTagHandlerType, pTabItem)
{
	if( (pTabItem->m_ucSnr[2]&0xFC) == 0x5C )	// M24LR64E-R
	{
		m_sTagName = "ISO 15693 : ";
		m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_STM);
		m_sTagName += " M24LR64E-R";
	}
	else if( (pTabItem->m_ucSnr[2]&0xFC) == 0x4C )	// M24LR16E-R
	{
		m_sTagName = "ISO 15693 : ";
		m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_STM);
		m_sTagName += " M24LR16E-R";
	}
	else if( (pTabItem->m_ucSnr[2]&0xFC) == 0x58 )	// M24LR04E-R
	{
		m_sTagName = "ISO 15693 : ";
		m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_STM);
		m_sTagName += " M24LR04E-R";
	}
	else
	{
		m_sTagName = "ISO 15693 : ";
		m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_STM);
		m_sTagName += " M24LRxxE-R";
	}
}

FedmIscTagHandler_ISO15693_STM_M24LRxxER::~FedmIscTagHandler_ISO15693_STM_M24LRxxER()
{
}

// set all data members to 0
/*void FedmIscTagHandler_ISO15693_STM_M24LRxxER::Init()
{
}*/


// #########################################################################
// #################### methods for energy harvesting mode
// #########################################################################


/***************************************************************************
  Begin        		:	25.09.2012 / M. Hultsch

  Version       	:	04.03.01 / 25.09.2012 / M. Hultsch

  Function			:	[0xB1][0xA0] Read Configuration Byte

  Parameters		:	unsigned char& ucData	- [out] reference to the configuration byte

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_STM_M24LRxxER::ReadCfg(unsigned char& ucData)
{
	int iErr = 0;

	ucData = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xA0));
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
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_STM));
	
	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB1);
	if(iBack == 0)
	{
		// return received data
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B1_RSP_STM_DATA, &ucData));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	25.09.2012 / M. Hultsch

  Version       	:	04.03.01 / 25.09.2012 / M. Hultsch

  Function			:	[0xB1][0xA1] Write Enegy Harvesting Configuration

  Parameters		:	unsigned char ucData	- [in] configuration byte

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_STM_M24LRxxER::WriteEHCfg(unsigned char ucData)
{
	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xA1));
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
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_STM));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_STM_DATA, ucData));
	
	// ... and execute
	return m_pReader->SendProtocol(0xB1);
}

/***************************************************************************
  Begin        		:	25.09.2012 / M. Hultsch

  Version       	:	04.03.01 / 25.09.2012 / M. Hultsch

  Function			:	[0xB1][0xA2] Set or Reset Energy Harvesting Enable

  Parameters		:	unsigned char ucData	- [in] configuration byte

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_STM_M24LRxxER::SetRstEHEn(unsigned char ucData)
{
	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xA2));
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
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_STM));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_STM_DATA, ucData));
	
	// ... and execute
	return m_pReader->SendProtocol(0xB1);
}

/***************************************************************************
  Begin        		:	25.09.2012 / M. Hultsch

  Version       	:	04.03.01 / 25.09.2012 / M. Hultsch

  Function			:	[0xB1][0xA3] Check Energy Harvesting Control Register

  Parameters		:	unsigned char& ucData	- [out] reference to the configuration byte

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_STM_M24LRxxER::CheckEHEn(unsigned char& ucData)
{
	int iErr = 0;

	ucData = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xA3));
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
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_STM));
	
	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB1);
	if(iBack == 0)
	{
		// return received data
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B1_RSP_STM_DATA, &ucData));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	25.09.2012 / M. Hultsch

  Version       	:	04.03.01 / 25.09.2012 / M. Hultsch

  Function			:	[0xB1][0xA4] Write RF WIP/BUSY bit into Configuration Register

  Parameters		:	unsigned char ucData	- [in] configuration byte

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_STM_M24LRxxER::WriteDOCfg(unsigned char ucData)
{
	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xA4));
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
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_STM));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_STM_DATA, ucData));
	
	// ... and execute
	return m_pReader->SendProtocol(0xB1);
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)
