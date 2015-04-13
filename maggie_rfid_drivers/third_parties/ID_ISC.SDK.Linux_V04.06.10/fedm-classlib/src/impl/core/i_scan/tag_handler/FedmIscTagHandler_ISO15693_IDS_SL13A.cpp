/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO15693_IDS_SL13A.cpp         |
|                                                       |
---------------------------------------------------------

Copyright © 2011		FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	03.03.01 / 28.03.2011

Version       		:	03.03.01 / 05.04.2011 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID i-scan® HF transponder ISO15693 of manufacturer IDS Microchip 
						specialized tag handler on top with custom specific commands

NOTE				:	this class supports only non-addressed or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)

#include "FedmIscTagHandler_ISO15693_IDS_SL13A.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"
#include "../../FEDM_Functions.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif


#if _MSC_VER <= 1200 // for Visual Studio 6
	// constants for [0xA0] Set Password
	static const unsigned char PW_LEVEL_SYSTEM			= 0x01;
	static const unsigned char PW_LEVEL_USER			= 0x02;
	static const unsigned char PW_LEVEL_MEASUREMENT		= 0x03;
	static const unsigned char PW_LEVEL_STORAGE_EEPROM	= 0x80;
	// constants for [0xA1] Set Log Mode
	static const unsigned char LOG_MODE_DENSE						= 0x00;
	static const unsigned char LOG_MODE_ALL_VALUES_OUT_OF_LIMITS	= 0x01;
	static const unsigned char LOG_MODE_LIMITS_CROSSING				= 0x03;
#endif


//####################################################################
// class FedmIscTagHandler_ISO15693_IDS_SL13A
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_ISO15693_IDS_SL13A::FedmIscTagHandler_ISO15693_IDS_SL13A(
	FEDM_ISCReader* pReader,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO15693(pReader, FedmIscTagHandler::TYPE_ISO15693_KSW, pTabItem)
{
	m_sTagName = "ISO 15693 : ";
	m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_IDS);
	m_sTagName += " SL13A";
	Init();
}

FedmIscTagHandler_ISO15693_IDS_SL13A::FedmIscTagHandler_ISO15693_IDS_SL13A(
	FEDM_ISCReader* pReader,
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO15693(pReader, uiTagHandlerType, pTabItem)
{
	m_sTagName = "ISO 15693 : ";
	m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_IDS);
	m_sTagName += " SL13A";
	Init();
}

FedmIscTagHandler_ISO15693_IDS_SL13A::~FedmIscTagHandler_ISO15693_IDS_SL13A()
{
}

// set all data members to 0
void FedmIscTagHandler_ISO15693_IDS_SL13A::Init()
{
	FedmIscTagHandler_ISO15693::Init();
	
	m_ucTagError = 0;
}

unsigned char FedmIscTagHandler_ISO15693_IDS_SL13A::GetTagError()
{
	return m_ucTagError;
}

/***************************************************************************
  Begin        		:	30.03.2011 / M. Hultsch

  Version       	:	03.03.01 / 30.03.2011 / M. Hultsch

  Function			:	[0xB1][0xA0] SetPassword

  Parameters		:	unsigned char ucPasswordLevel
						string sPassword

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_IDS_SL13A::SetPassword(
	unsigned char ucPasswordLevel,
	string sPassword )
{
	int iBack = 0;
	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xA0));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB1_REQ_IDS_PASSWORD_LEVEL, ucPasswordLevel));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB1_REQ_IDS_PASSWORD, sPassword));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB1);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B1_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	30.03.2011 / M. Hultsch

  Version       	:	03.03.01 / 30.03.2011 / M. Hultsch

  Function			:	[0xB1][0xA1] Set Log Mode
						writes parameters to System Area Block 0x0A
						- with composed parameters

  Parameters		:	unsigned int uiLogMode

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_IDS_SL13A::SetLogMode(
	unsigned int uiLogMode )
{
	int iBack = 0;
	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xA1));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB1_REQ_IDS_LOG_MODE, uiLogMode));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB1);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B1_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	30.03.2011 / M. Hultsch

  Version       	:	03.03.01 / 31.03.2011 / M. Hultsch

  Function			:	[0xB1][0xA1] Set Log Mode
						writes parameters to System Area Block 0x0A
						- with single parameters (composed internal)

  Parameters		:	bool bExternalSensor
						unsigned char ucLogFormat
						bool bBatteryCheck
						bool bStorageRule
						unsigned char ucLogInterval
						unsigned int uiExtremeUpperLimit

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_IDS_SL13A::SetLogMode(
	bool bExternalSensor,
	unsigned char ucLogFormat,
	bool bBatteryCheck,
	bool bStorageRule,
	unsigned int uiLogInterval,
	unsigned int uiExtremeUpperLimit )
{
	int iBack = 0;
	int iErr = 0;
	unsigned int uiLogMode = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xA1));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	// compose log mode parameter
	uiLogMode  =  (uiExtremeUpperLimit & 0x000003FF);
	uiLogMode |= ((uiLogInterval & 0x00007FFF) << 10);
	if(bStorageRule)
		uiLogMode |= 0x08000000;
	if(bBatteryCheck)
		uiLogMode |= 0x10000000;
	uiLogMode |= (((unsigned int)(ucLogFormat & 0x03)) << 29);
	if(bExternalSensor)
		uiLogMode |= 0x80000000;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB1_REQ_IDS_LOG_MODE, uiLogMode));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB1);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B1_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	31.03.2011 / M. Hultsch

  Version       	:	03.03.01 / 31.03.2011 / M. Hultsch

  Function			:	[0xB1][0xA2] Set Log Limits
						writes parameters to System Area Block 0x09

  Parameters		:	unsigned int uiExtremeLowerLimit
						unsigned int uiLowerLimit
						unsigned int uiUppererLimit

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_IDS_SL13A::SetLogLimits(	
	unsigned int uiExtremeLowerLimit,
	unsigned int uiLowerLimit,
	unsigned int uiUpperLimit )
{
	int iBack = 0;
	int iErr = 0;
	unsigned int uiLogLimits = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xA2));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	// compose log limits
	uiLogLimits  =  (uiExtremeLowerLimit & 0x000003FF);
	uiLogLimits |= ((uiLowerLimit & 0x000003FF) << 10);
	uiLogLimits |= ((uiUpperLimit & 0x000003FF) << 20);

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB1_REQ_IDS_LOG_LIMITS, uiLogLimits));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB1);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B1_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	31.03.2011 / M. Hultsch

  Version       	:	03.03.01 / 31.03.2011 / M. Hultsch

  Function			:	[0xB1][0xA3] Get Measurement Setup
						reads parameters from System Area Blocks 0x08..0x0B

  Parameters		:	unsigned int& uiStartTime,
						unsigned int& uiLogLimits
						unsigned int& uiLogMode
						unsigned int& uiDelayTime

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_IDS_SL13A::GetMeasurementSetup(	
	unsigned int& uiStartTime,
	unsigned int& uiLogLimits,
	unsigned int& uiLogMode,
	unsigned int& uiDelayTime )
{
	int iBack = 0;
	int iErr = 0;
	unsigned int uiLogLimit = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xA3));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB1);
	if(iBack == 0)
	{
		// get measurement setup data
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB1_RSP_IDS_START_TIME, &uiStartTime));
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB1_RSP_IDS_LOG_LIMITS, &uiLogLimits));
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB1_RSP_IDS_LOG_MODE, &uiLogMode));
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB1_RSP_IDS_DELAY_TIME, &uiDelayTime));
	}
	else if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B1_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	31.03.2011 / M. Hultsch

  Version       	:	03.03.01 / 31.03.2011 / M. Hultsch

  Function			:	[0xB1][0xA4] Set External Calibration Data
						writes external calibration data to System Area Block 0x06

  Parameters		:	unsigned int uiCalibrationData

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_IDS_SL13A::SetExternalCalibrationData(	
	unsigned int uiCalibrationData )
{
	int iBack = 0;
	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xA4));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB1_REQ_IDS_EXT_CAL_DATA, uiCalibrationData));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB1);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B1_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	31.03.2011 / M. Hultsch

  Version       	:	03.03.01 / 31.03.2011 / M. Hultsch

  Function			:	[0xB1][0xA5] Set Internal Calibration Data
						writes external calibration data to System Area Block 0x05

  Parameters		:	unsigned int uiCalibrationData

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_IDS_SL13A::SetInternalCalibrationData(	
	unsigned int uiCalibrationData )
{
	int iBack = 0;
	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xA5));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB1_REQ_IDS_INT_CAL_DATA, uiCalibrationData));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB1);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B1_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	30.03.2011 / M. Hultsch

  Version       	:	03.03.01 / 31.03.2011 / M. Hultsch

  Function			:	[0xB1][0xA6] Set Passive
						stops logging process, stops timer and sets tag in standby mode

  Parameters		:	-

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_IDS_SL13A::SetPassive()
{
	int iBack = 0;
	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xA6));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB1);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B1_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	31.03.2011 / M. Hultsch

  Version       	:	03.03.01 / 31.03.2011 / M. Hultsch

  Function			:	[0xB1][0xA7] Start Logging
						start logging process and sets the start time in UTC format
						- with composed parameters

  Parameters		:	unsigned int uiStartTime

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_IDS_SL13A::StartLog(	
	unsigned int uiStartTime )
{
	int iBack = 0;
	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xA7));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB1_REQ_IDS_START_TIME, uiStartTime));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB1);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B1_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	31.03.2011 / M. Hultsch

  Version       	:	03.03.01 / 31.03.2011 / M. Hultsch

  Function			:	[0xB1][0xA7] Start Logging
						start logging process and sets the start time in UTC format
						- with single parameters (composed internal)

  Parameters		:	unsigned char ucYear
						unsigned char ucMonth
						unsigned char ucDay
						unsigned char ucHour
						unsigned char ucMinute
						unsigned char ucSecond

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_IDS_SL13A::StartLog(	
	unsigned char ucYear,
	unsigned char ucMonth,
	unsigned char ucDay,
	unsigned char ucHour,
	unsigned char ucMinute,
	unsigned char ucSecond )
{
	int iBack = 0;
	int iErr = 0;
	unsigned int uiStartTime = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xA7));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	// compose start time
	uiStartTime  =  (ucSecond & 0x3F);
	uiStartTime |=  (((unsigned int)(ucMinute & 0x3F)) << 6);
	uiStartTime |=  (((unsigned int)(ucHour & 0x1F)) << 12);
	uiStartTime |=  (((unsigned int)(ucDay & 0x1F)) << 17);
	uiStartTime |=  (((unsigned int)(ucMonth & 0x0F)) << 22);
	uiStartTime |=  (((unsigned int)(ucYear & 0x3F)) << 26);

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB1_REQ_IDS_START_TIME, uiStartTime));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB1);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B1_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	31.03.2011 / M. Hultsch

  Version       	:	03.03.01 / 01.04.2011 / M. Hultsch

  Function			:	[0xB1][0xA8] Get Log State
						returns the logging state with single status parameters

  Parameters		:	bool& bActiveFlag,
						unsigned int&  uiErrorsAndEvents
						unsigned char& ucNoOfMemReplacements,
						unsigned int&  uiNoOfMeasurements,
						unsigned char& ucMeasurementAddressPointer,
						unsigned char& ucExtremeLowerLimitCounter
						unsigned char& ucLowerLimitCounter
						unsigned char& ucHigherLimitCounter
						unsigned char& ucExtremeHigherLimitCounter

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_IDS_SL13A::GetLogState(	
	bool& bActiveFlag,
	unsigned int&  uiErrorsAndEvents,
	unsigned char& ucNoOfMemReplacements,
	unsigned int&  uiNoOfMeasurements,
	unsigned char& ucMeasurementAddressPointer,
	unsigned char& ucExtremeLowerLimitCounter,
	unsigned char& ucLowerLimitCounter,
	unsigned char& ucHigherLimitCounter,
	unsigned char& ucExtremeHigherLimitCounter )
{
	int iBack = 0;
	int iErr = 0;
	unsigned int uiMeasurementStatus = 0;
	unsigned int uiLimitsCounter = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xA8));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB1);
	if(iBack == 0)
	{
		// get measurement setup data
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB1_RSP_IDS_MEAS_STATE, &uiMeasurementStatus));
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB1_RSP_IDS_LIMITS_COUNTER, &uiLimitsCounter));

		if(uiMeasurementStatus & 0x00000001)
			bActiveFlag = true;
		else
			bActiveFlag = false;

		uiErrorsAndEvents			= ((uiMeasurementStatus & 0x000007FE) >> 1);
		ucNoOfMemReplacements		= (unsigned char)((uiMeasurementStatus & 0x00003800) >> 11);
		uiNoOfMeasurements			= ((uiMeasurementStatus & 0x00FFC000) >> 14);
		ucMeasurementAddressPointer	= (unsigned char)((uiMeasurementStatus & 0xFF000000) >> 24);

		ucExtremeLowerLimitCounter	= (unsigned char) (uiLimitsCounter & 0x000000FF);
		ucLowerLimitCounter			= (unsigned char)((uiLimitsCounter & 0x0000FF00) >>  8);
		ucHigherLimitCounter		= (unsigned char)((uiLimitsCounter & 0x00FF0000) >> 16);
		ucExtremeHigherLimitCounter	= (unsigned char)((uiLimitsCounter & 0xFF000000) >> 24);
	}
	else if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B1_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	31.03.2011 / M. Hultsch

  Version       	:	03.03.01 / 31.03.2011 / M. Hultsch

  Function			:	[0xB1][0xA8] Get Log State
						returns the logging state with composed status parameters

  Parameters		:	unsigned int& uiMeasurementStatus
						unsigned char& ucLimitCounters

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_IDS_SL13A::GetLogState(	
	unsigned int& uiMeasurementStatus,
	unsigned int& uiLimitCounters )
{
	int iBack = 0;
	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xA8));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB1);
	if(iBack == 0)
	{
		// get measurement setup data
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB1_RSP_IDS_MEAS_STATE, &uiMeasurementStatus));
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB1_RSP_IDS_LIMITS_COUNTER, &uiLimitCounters));
	}
	else if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B1_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	31.03.2011 / M. Hultsch

  Version       	:	03.03.01 / 31.03.2011 / M. Hultsch

  Function			:	[0xB1][0xA9] Get Calibration Data
						reads internal and external calibration data from System Area Blocks 0x05 and 0x06

  Parameters		:	unsigned int& uiInternalCalibrationData
						unsigned int& uiExternalCalibrationData

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_IDS_SL13A::GetCalibrationData(	
	unsigned int& uiInternalCalibrationData,
	unsigned int& uiExternalCalibrationData )
{
	int iBack = 0;
	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xA9));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB1);
	if(iBack == 0)
	{
		// get calibration data
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB1_RSP_IDS_INT_CAL_DATA, &uiInternalCalibrationData));
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB1_RSP_IDS_EXT_CAL_DATA, &uiExternalCalibrationData));
	}
	else if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B1_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	31.03.2011 / M. Hultsch

  Version       	:	03.03.01 / 31.03.2011 / M. Hultsch

  Function			:	[0xB1][0xAA] Get Battery Level
						returns the actual battery level

  Parameters		:	unsigned char& ucBatteryLevel

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_IDS_SL13A::GetBatteryLevel(	
	unsigned char& ucBatteryLevel )
{
	int iBack = 0;
	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xAA));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB1);
	if(iBack == 0)
	{
		// get measurement setup data
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB1_RSP_IDS_BATTERY_LEVEL, &ucBatteryLevel));
	}
	else if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B1_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	31.03.2011 / M. Hultsch

  Version       	:	03.03.01 / 31.03.2011 / M. Hultsch

  Function			:	[0xB1][0xAB] Verify Password
						compares the password of the specified password level

  Parameters		:	unsigned char ucPasswordLevel

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_IDS_SL13A::VerifyPassword(	
	unsigned char ucPasswordLevel )
{
	int iBack = 0;
	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xAB));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB1_REQ_IDS_PASSWORD_LEVEL, ucPasswordLevel));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB1);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B1_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	31.03.2011 / M. Hultsch

  Version       	:	03.03.01 / 31.03.2011 / M. Hultsch

  Function			:	[0xB1][0xAC] Initialize
						writes parameters to System Area Block 0x0B

  Parameters		:	unsigned char ucNoOfUserBlocks
						unsigned int uiDelayTime
						bool bSingleUse

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_IDS_SL13A::Initialize(	
	unsigned char ucNoOfUserBlocks,
	unsigned int uiDelayTime,
	bool bSingleUse )
{
	int iBack = 0;
	int iErr = 0;
	unsigned int uiIniData = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xAC));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	// compose initialization data
	uiIniData  = ucNoOfUserBlocks;
	uiIniData |= ((uiDelayTime & 0x00000FFF) << 8);
	if(bSingleUse)
		uiIniData |= 0x80000000;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB1_REQ_IDS_INIT_DATA, uiIniData));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB1);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B1_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	31.03.2011 / M. Hultsch

  Version       	:	03.03.01 / 31.03.2011 / M. Hultsch

  Function			:	[0xB1][0xAD] Get Temperature
						starts the AD conversion and returns the measured temperature

  Parameters		:	unsigned int& uiTemperature

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_IDS_SL13A::GetTemperature(	
	unsigned int& uiTemperature )
{
	int iBack = 0;
	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xAD));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB1);
	if(iBack == 0)
	{
		// get measurement setup data
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB1_RSP_IDS_MEAS_TEMP, &uiTemperature));
	}
	else if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B1_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)
