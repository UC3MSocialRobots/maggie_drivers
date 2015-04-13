/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO15693_EM_4034.cpp           |
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

Function			:	class for OBID i-scan® HF transponder ISO15693 of manufacturer EM Microelectronic-Marin SA
						specialized tag handler on top with custom specific commands

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)

#include "FedmIscTagHandler_ISO15693_EM_4034.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"
#include "../../FEDM_Functions.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscTagHandler_ISO15693_EM_4034
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_ISO15693_EM_4034::FedmIscTagHandler_ISO15693_EM_4034(
	FEDM_ISCReader* pReader,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO15693_EM(pReader, FedmIscTagHandler::TYPE_ISO15693_EM_4034, pTabItem)
{
	m_sTagName = "ISO 15693 : ";
	m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_EM);
	m_sTagName += " 4034";
}

FedmIscTagHandler_ISO15693_EM_4034::FedmIscTagHandler_ISO15693_EM_4034(
	FEDM_ISCReader* pReader,
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO15693_EM(pReader, uiTagHandlerType, pTabItem)
{
	m_sTagName = "ISO 15693 : ";
	m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_EM);
	m_sTagName += " 4034";
}

FedmIscTagHandler_ISO15693_EM_4034::~FedmIscTagHandler_ISO15693_EM_4034()
{
}

// set all data members to 0
/*void FedmIscTagHandler_ISO15693_EM_4034::Init()
{
}*/

/***************************************************************************
  Begin        		:	09.10.2009 / M. Hultsch

  Version       	:	03.01.00 / 09.10.2009 / M. Hultsch

  Function			:	[0xB1][0xA0] ToggleEAS
						only non addressed

  Parameters		:	string sPassword

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_EM_4034::ToggleEAS(
	string sPassword)
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xA0));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)0));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_EM_PASSWORD, sPassword));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_EM));
	
	// ... and execute
	return m_pReader->SendProtocol(0xB1);
}

/***************************************************************************
  Begin        		:	09.10.2009 / M. Hultsch

  Version       	:	03.01.00 / 09.10.2009 / M. Hultsch

  Function			:	[0xB1][0xE4] Login

  Parameters		:	string sPassword

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_EM_4034::Login(
	string sPassword)
{
	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xE4));
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

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_EM_PASSWORD, sPassword));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_EM));
	
	// ... and execute
	return m_pReader->SendProtocol(0xB1);
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)
