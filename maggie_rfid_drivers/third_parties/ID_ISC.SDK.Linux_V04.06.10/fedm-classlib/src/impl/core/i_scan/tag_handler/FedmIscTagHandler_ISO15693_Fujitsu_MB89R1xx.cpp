/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx.cpp  |
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

Function			:	class for OBID i-scan® HF transponder ISO15693 of manufacturer Fujitsu Limited
						specialized tag handler on top with custom specific commands

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)

#include "FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"
#include "../../FEDM_Functions.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx::FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx(
	FEDM_ISCReader* pReader,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO15693_Fujitsu(pReader, FedmIscTagHandler::TYPE_ISO15693_Fujitsu_MB89R1xx, pTabItem)
{
	m_sTagName = "ISO 15693 : ";
	m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_FUJITSU);
	m_sTagName += " MB89R1xx";
}

FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx::FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx(
	FEDM_ISCReader* pReader,
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO15693_Fujitsu(pReader, uiTagHandlerType, pTabItem)
{
	m_sTagName = "ISO 15693 : ";
	m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_FUJITSU);
	m_sTagName += " MB89R1xx";
}

FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx::~FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx()
{
}

// set all data members to 0
/*void FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx::Init()
{
}*/

/***************************************************************************
  Begin        		:	09.10.2009 / M. Hultsch

  Version       	:	03.01.00 / 12.10.2009 / M. Hultsch

  Function			:	[0xB1][0xA0] EAS

  Parameters		:

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx::EAS()
{
	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xA0));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_SEL));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_FUJITSU));
	
	// ... and execute
	return m_pReader->SendProtocol(0xB1);
}

/***************************************************************************
  Begin        		:	09.10.2009 / M. Hultsch

  Version       	:	03.01.00 / 12.10.2009 / M. Hultsch

  Function			:	[0xB1][0xA1] WriteEAS

  Parameters		:	unsigned char ucData

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx::WriteEAS(
	unsigned char ucData)
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

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_FUJITSU_DATA, ucData));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_FUJITSU));
	
	// ... and execute
	return m_pReader->SendProtocol(0xB1);
}

/***************************************************************************
  Begin        		:	09.10.2009 / M. Hultsch

  Version       	:	03.01.00 / 12.10.2009 / M. Hultsch

  Function			:	[0xB1][0xA2] Refresh

  Parameters		:

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx::Refresh()
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

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_FUJITSU));
	
	// ... and execute
	return m_pReader->SendProtocol(0xB1);
}

/***************************************************************************
  Begin        		:	09.10.2009 / M. Hultsch

  Version       	:	03.01.00 / 12.10.2009 / M. Hultsch

  Function			:	[0xB1][0xA6] Kill

  Parameters		:

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx::Kill()
{
	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xA6));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)0));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)FEDM_ISC_ISO_MODE_ADR));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_FUJITSU));
	
	// ... and execute
	return m_pReader->SendProtocol(0xB1);
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)
