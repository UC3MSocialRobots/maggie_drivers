/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A.cpp |
|                                                       |
---------------------------------------------------------

Copyright © 2011-2013	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	03.03.01 / 06.04.2011

Version       		:	04.05.03 / 23.08.2013 / M. Hultsch

Operation Systems	:	independent

Function			:	class for UHF transponder EPC Class 1 Generation 2 of manufacturer IDS Microchip 
						specialized tag handler on top with custom specific commands

NOTE				:	this class supports only non-addressed or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_NO_TAG_HANDLER_EPC_C1_G2)

#include "FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"
#include "../../FEDM_Functions.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif


#if _MSC_VER <= 1200 // for Visual Studio 6
	// constants for [0xA0] Set Password and [0xAE] Open Area
	static const unsigned char PW_LEVEL_SYSTEM			= 0x01;
	static const unsigned char PW_LEVEL_USER			= 0x02;
	static const unsigned char PW_LEVEL_MEASUREMENT		= 0x03;
	// constants for [0xA1] Set Log Mode
	static const unsigned char LOG_MODE_DENSE						= 0x00;
	static const unsigned char LOG_MODE_ALL_VALUES_OUT_OF_LIMITS	= 0x01;
	static const unsigned char LOG_MODE_LIMITS_CROSSING				= 0x03;
	static const unsigned char LOG_MODE_IRQ_EXT1					= 0x05;
	static const unsigned char LOG_MODE_IRQ_EXT2					= 0x06;
	static const unsigned char LOG_MODE_IRQ_EXT1_EXT2				= 0x07;
	// constants for [0xAF] Access FIFO
	static const unsigned char SUB_CMD_READ_FIFO		= 0x04;
	static const unsigned char SUB_CMD_WRITE_FIFO		= 0x05;
	static const unsigned char SUB_CMD_READ_STATUS		= 0x06;
#endif


//####################################################################
// class FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A::FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A(
	FEDM_ISCReader* pReader,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_EPC_Class1_Gen2(pReader, FedmIscTagHandler::TYPE_EPC_CLASS1_GEN2_IDS_SL900A, pTabItem)
{
	m_sTagName = "EPC_Class1_Gen2 : IDS SL900A";
	Init();
}

FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A::FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A(
	FEDM_ISCReader* pReader,
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_EPC_Class1_Gen2(pReader, uiTagHandlerType, pTabItem)
{
	m_sTagName = "EPC_Class1_Gen2 : IDS SL900A";
	Init();
}

FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A::~FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A()
{
}

// set all data members to 0
void FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A::Init()
{
	FedmIscTagHandler_EPC_Class1_Gen2::Init();
	
	m_ucTagError = 0;
}

unsigned char FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A::GetTagError()
{
	return m_ucTagError;
}

/***************************************************************************
  Begin        		:	06.04.2011 / M. Hultsch

  Version       	:	03.03.01 / 07.04.2011 / M. Hultsch

  Function			:	[0xB4][0xA0] SetPassword

  Parameters		:	string sAccessPassword			- [in] optional authent password
						unsigned char ucPasswordLevel
						string sPassword

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A::SetPassword(
	string sAccessPassword,
	unsigned char ucPasswordLevel,
	string sPassword )
{
	int iBack = 0;
	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_CMD, (unsigned char)0xA0));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EPC_LF, (bool)true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC_LEN, m_pTabItem->m_ucSnrLen));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK, (unsigned char)0));
	if(sAccessPassword.length() > 0)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK_ACCESS_FLAG, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW_LENGTH, (unsigned char)(sAccessPassword.length()/2)));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW, sAccessPassword));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, false));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB4_REQ_IDS_PASSWORD_LEVEL, ucPasswordLevel));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB4_REQ_IDS_PASSWORD, sPassword));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB4);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B4_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	06.04.2011 / M. Hultsch

  Version       	:	03.03.01 / 07.04.2011 / M. Hultsch

  Function			:	[0xB4][0xA1] Set Log Mode
						writes parameters to System Area Blocks 0x026..0x028

  Parameters		:	string sAccessPassword			- [in] optional authent password
						LOG_MODE* pLogMode
						LOG_INTERVAL* pLogInterval

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A::SetLogMode(
	string sAccessPassword,
	LOG_MODE* pLogMode,
	LOG_INTERVAL* pLogInterval )
{
	int iBack = 0;
	int iErr = 0;
	unsigned int uiLogMode = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_CMD, (unsigned char)0xA1));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EPC_LF, (bool)true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC_LEN, m_pTabItem->m_ucSnrLen));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK, (unsigned char)0));
	if(sAccessPassword.length() > 0)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK_ACCESS_FLAG, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW_LENGTH, (unsigned char)(sAccessPassword.length()/2)));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW, sAccessPassword));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, false));
	}

	// compose log mode parameter
	uiLogMode = ((pLogInterval->uiLogInterval & 0x00007FFF) << 1);
	if(pLogMode->bBatteryCheck)
		uiLogMode |= 0x00010000;
	if(pLogMode->bTemperatureSensor)
		uiLogMode |= 0x00020000;
	if(pLogMode->bExternalSensor1)
		uiLogMode |= 0x00040000;
	if(pLogMode->bExternalSensor2)
		uiLogMode |= 0x00080000;
	if(pLogMode->bStorageRule)
		uiLogMode |= 0x00100000;
	uiLogMode |= (((unsigned int)(pLogMode->ucLogginForm & 0x07)) << 21);

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB4_REQ_IDS_LOG_MODE, uiLogMode));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB4);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B4_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	06.04.2011 / M. Hultsch

  Version       	:	03.03.01 / 07.04.2011 / M. Hultsch

  Function			:	[0xB4][0xA2] Set Log Limits
						writes parameters to System Area Blocks 0x019..0x01D

  Parameters		:	string sAccessPassword			- [in] optional authent password
						LIMITS* pLogLimits

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A::SetLogLimits(	
	string sAccessPassword,
	LIMITS* pLogLimits )
{
	int iBack = 0;
	int iErr = 0;
	unsigned char ucLogLimits[5];

	memset(ucLogLimits, 0, 5);
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_CMD, (unsigned char)0xA2));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EPC_LF, (bool)true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC_LEN, m_pTabItem->m_ucSnrLen));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK, (unsigned char)0));
	if(sAccessPassword.length() > 0)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK_ACCESS_FLAG, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW_LENGTH, (unsigned char)(sAccessPassword.length()/2)));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW, sAccessPassword));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, false));
	}

	// compose log limits
	ucLogLimits[4]  =  (unsigned char) (pLogLimits->uiExtremeUpperLimit & 0x00FF);
	ucLogLimits[3]  =  (unsigned char)((pLogLimits->uiExtremeUpperLimit & 0x0300)>>8);
	ucLogLimits[3] |=  (unsigned char)((pLogLimits->uiUpperLimit & 0x003F)<<2);
	ucLogLimits[2]  =  (unsigned char)((pLogLimits->uiUpperLimit & 0x03C0)>>6);
	ucLogLimits[2] |=  (unsigned char)((pLogLimits->uiLowerLimit & 0x000F)<<4);
	ucLogLimits[1]  =  (unsigned char)((pLogLimits->uiLowerLimit & 0x03F0)>>4);
	ucLogLimits[1] |=  (unsigned char)((pLogLimits->uiExtremeLowerLimit & 0x0003)<<6);
	ucLogLimits[0]  =  (unsigned char)((pLogLimits->uiExtremeLowerLimit & 0x03FC)>>2);

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB4_REQ_IDS_LOG_LIMITS, ucLogLimits, 5));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB4);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B4_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	06.04.2011 / M. Hultsch

  Version       	:	03.03.01 / 07.04.2011 / M. Hultsch

  Function			:	[0xB4][0xA3] Get Measurement Setup
						reads parameters from System Area Blocks 0x019..0x01D, 0x026..0x02C

  Parameters		:	string sAccessPassword			- [in] optional authent password
						START_TIME* pStartTime
						LIMITS* pLogLimits
						LOG_MODE* pLogMode
						LOG_INTERVAL* pLogInterval
						DELAY_TIME* pDelayTime
						USER_DATA* pUserData

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A::GetMeasurementSetup(	
	string sAccessPassword,
	START_TIME* pStartTime,
	LIMITS* pLogLimits,
	LOG_MODE* pLogMode,
	LOG_INTERVAL* pLogInterval,
	DELAY_TIME* pDelayTime,
	APP_DATA* pAppData )
{
	int iBack = 0;
	int iErr = 0;
	unsigned int uiStartTime = 0;
	unsigned char ucLogLimits[5];
	unsigned char ucLogMode = 0;
	unsigned int uiLogInterval = 0;
	unsigned int uiDelayTime = 0;
	unsigned int uiAppData = 0;

	memset(ucLogLimits, 0, 5);
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_CMD, (unsigned char)0xA3));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EPC_LF, (bool)true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC_LEN, m_pTabItem->m_ucSnrLen));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK, (unsigned char)0));
	if(sAccessPassword.length() > 0)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK_ACCESS_FLAG, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW_LENGTH, (unsigned char)(sAccessPassword.length()/2)));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW, sAccessPassword));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, false));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB4);
	if(iBack == 0)
	{
		// get measurement setup data
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB4_RSP_IDS_START_TIME, &uiStartTime));
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB4_RSP_IDS_LOG_LIMITS, ucLogLimits, 5));
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB4_RSP_IDS_LOG_MODE, &ucLogMode));
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB4_RSP_IDS_LOG_INTERVAL, &uiLogInterval));
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB4_RSP_IDS_DELAY_TIME, &uiDelayTime));
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB4_RSP_IDS_APP_DATA, &uiAppData));

		pStartTime->ucSecond	= (unsigned char) (uiStartTime		  & 0x0000003F);
		pStartTime->ucMinute	= (unsigned char)((uiStartTime >>  6) & 0x0000003F);
		pStartTime->ucHour		= (unsigned char)((uiStartTime >> 12) & 0x0000001F);
		pStartTime->ucDay		= (unsigned char)((uiStartTime >> 17) & 0x0000001F);
		pStartTime->ucMonth		= (unsigned char)((uiStartTime >> 22) & 0x0000000F);
		pStartTime->ucYear		= (unsigned char)((uiStartTime >> 26) & 0x0000003F);

		pLogLimits->uiExtremeUpperLimit  =   (unsigned int) ucLogLimits[4];
		pLogLimits->uiExtremeUpperLimit |= (((unsigned int)(ucLogLimits[3] & 0x03)) << 8);
		pLogLimits->uiUpperLimit		 = (((unsigned int)(ucLogLimits[3] & 0xFC)) >> 2);
		pLogLimits->uiUpperLimit		|= (((unsigned int)(ucLogLimits[2] & 0x0F)) << 6);
		pLogLimits->uiLowerLimit		 = (((unsigned int)(ucLogLimits[2] & 0xF0)) >> 4);
		pLogLimits->uiLowerLimit		|= (((unsigned int)(ucLogLimits[1] & 0x3F)) << 4);
		pLogLimits->uiExtremeLowerLimit	 = (((unsigned int)(ucLogLimits[1] & 0xC0)) >> 6);
		pLogLimits->uiExtremeLowerLimit	|= (((unsigned int)ucLogLimits[0]) << 2);

		if(ucLogMode & 0x01)
			pLogMode->bBatteryCheck = true;
		else
			pLogMode->bBatteryCheck = false;

		if(ucLogMode & 0x02)
			pLogMode->bTemperatureSensor = true;
		else
			pLogMode->bTemperatureSensor = false;

		if(ucLogMode & 0x04)
			pLogMode->bExternalSensor1 = true;
		else
			pLogMode->bExternalSensor1 = false;

		if(ucLogMode & 0x08)
			pLogMode->bExternalSensor2 = true;
		else
			pLogMode->bExternalSensor2 = false;

		if(ucLogMode & 0x10)
			pLogMode->bStorageRule = true;
		else
			pLogMode->bStorageRule = false;

		pLogMode->ucLogginForm = ((ucLogMode & 0xE0) >> 5);

		pLogInterval->uiLogInterval = ((uiLogInterval & 0x0000FFFE) >> 1);

		if(uiDelayTime & 0x00000001)
			pDelayTime->bTimer = true;
		else
			pDelayTime->bTimer = false;

		if(uiDelayTime & 0x00000002)
			pDelayTime->bDelayMode_External = true;
		else
			pDelayTime->bDelayMode_External = false;

		pDelayTime->uiDelayTime = ((uiDelayTime & 0x0000FFF0) >> 4);

		pAppData->ucBrokenWordPointer	 =  (uiAppData & 0x00000007);
		pAppData->uiNoOfWordsForAppData  = ((uiAppData & 0x0000FF80) >> 7);
	}
	else if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B4_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	06.04.2011 / M. Hultsch

  Version       	:	03.03.01 / 07.04.2011 / M. Hultsch

  Function			:	[0xB4][0xA4] Set Sensor Front End parameters
						writes data to System Area Blocks 0x017..0x018

  Parameters		:	string sAccessPassword			- [in] optional authent password
						SFE_PARAMETERS* pSfeData

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A::SetSFEData(
	string sAccessPassword,
	SFE_PARAMETERS* pSfeData )
{
	int iBack = 0;
	int iErr = 0;
	unsigned int uiSfeData = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_CMD, (unsigned char)0xA4));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EPC_LF, (bool)true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC_LEN, m_pTabItem->m_ucSnrLen));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK, (unsigned char)0));
	if(sAccessPassword.length() > 0)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK_ACCESS_FLAG, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW_LENGTH, (unsigned char)(sAccessPassword.length()/2)));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW, sAccessPassword));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, false));
	}

	uiSfeData = (unsigned int)(pSfeData->ucVerifySensorID & 0x03);
	if(pSfeData->bAutoRangePreset)
		uiSfeData |= 0x00000004;

	uiSfeData |= (((unsigned int)(pSfeData->ucSext2 & 0x01)) << 3);
	uiSfeData |= (((unsigned int)(pSfeData->ucSext1 & 0x03)) << 4);
	uiSfeData |= (((unsigned int)(pSfeData->ucSeti & 0x1F)) << 6);
	uiSfeData |= (((unsigned int)(pSfeData->ucRang & 0x1F)) << 11);

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB4_REQ_IDS_SFE_PARA, uiSfeData));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB4);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B4_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	06.04.2011 / M. Hultsch

  Version       	:	03.03.01 / 07.04.2011 / M. Hultsch

  Function			:	[0xB4][0xA5] Set Calibration Data
						writes sensor calibration data to System Area Blocks 0x010..0x016

  Parameters		:	string sAccessPassword			- [in] optional authent password
						CALIBRATION* pCalData

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A::SetCalibrationData(	
	string sAccessPassword,
	CALIBRATION* pCalData )
{
	int iBack = 0;
	int iErr = 0;
	__int64 i64CalibrationData = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_CMD, (unsigned char)0xA5));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EPC_LF, (bool)true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC_LEN, m_pTabItem->m_ucSnrLen));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK, (unsigned char)0));
	if(sAccessPassword.length() > 0)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK_ACCESS_FLAG, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW_LENGTH, (unsigned char)(sAccessPassword.length()/2)));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW, sAccessPassword));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, false));
	}

	i64CalibrationData  =   (__int64)(pCalData->ucRefTc & 0x0F);
	i64CalibrationData |= (((__int64)(pCalData->ucOffInt & 0x7F)) << 7);
	i64CalibrationData |= (((__int64)(pCalData->ucRingCall & 0x1F)) << 14);
	i64CalibrationData |= (((__int64)(pCalData->ucIRLev & 0x03)) << 19);
	i64CalibrationData |= (((__int64)(pCalData->ucSelp22 & 0x03)) << 21);
	if(pCalData->bSwExtEn)
		i64CalibrationData |= (((__int64)1) << 23);
	i64CalibrationData |= (((__int64)pCalData->ucDf) << 24);
	i64CalibrationData |= (((__int64)(pCalData->ucAdf & 0x1F)) << 32);
	i64CalibrationData |= (((__int64)(pCalData->ucSelp12 & 0x03)) << 37);
	if(pCalData->bGndSwitch)
		i64CalibrationData |= (((__int64)1) << 39);
	i64CalibrationData |= (((__int64)(pCalData->ucCoars2 & 0x07)) << 40);
	i64CalibrationData |= (((__int64)(pCalData->ucAd2 & 0x1F)) << 43);
	i64CalibrationData |= (((__int64)(pCalData->ucCoars1 & 0x07)) << 48);
	i64CalibrationData |= (((__int64)(pCalData->ucAd1 & 0x1F)) << 51);

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB4_REQ_IDS_CAL_DATA, i64CalibrationData));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB4);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B4_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	06.04.2011 / M. Hultsch

  Version       	:	03.03.01 / 07.04.2011 / M. Hultsch

  Function			:	[0xB4][0xA6] EndLog
						stops logging process, stops timer and sets tag in standby mode

  Parameters		:	string sAccessPassword			- [in] optional authent password

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A::EndLog(
	string sAccessPassword)
{
	int iBack = 0;
	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_CMD, (unsigned char)0xA6));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EPC_LF, (bool)true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC_LEN, m_pTabItem->m_ucSnrLen));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK, (unsigned char)0));
	if(sAccessPassword.length() > 0)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK_ACCESS_FLAG, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW_LENGTH, (unsigned char)(sAccessPassword.length()/2)));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW, sAccessPassword));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, false));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB4);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B4_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	06.04.2011 / M. Hultsch

  Version       	:	03.03.01 / 07.04.2011 / M. Hultsch

  Function			:	[0xB4][0xA7] Start Logging
						start logging process and sets the start time in UTC format

  Parameters		:	string sAccessPassword			- [in] optional authent password
						START_TIME* pStartTime

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A::StartLog(	
	string sAccessPassword,
	START_TIME* pStartTime )
{
	int iBack = 0;
	int iErr = 0;
	unsigned int uiStartTime = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_CMD, (unsigned char)0xA7));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EPC_LF, (bool)true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC_LEN, m_pTabItem->m_ucSnrLen));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK, (unsigned char)0));
	if(sAccessPassword.length() > 0)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK_ACCESS_FLAG, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW_LENGTH, (unsigned char)(sAccessPassword.length()/2)));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW, sAccessPassword));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, false));
	}

	// compose start time
	uiStartTime  =  (pStartTime->ucSecond & 0x3F);
	uiStartTime |=  (((unsigned int)(pStartTime->ucMinute & 0x3F)) << 6);
	uiStartTime |=  (((unsigned int)(pStartTime->ucHour & 0x1F)) << 12);
	uiStartTime |=  (((unsigned int)(pStartTime->ucDay & 0x1F)) << 17);
	uiStartTime |=  (((unsigned int)(pStartTime->ucMonth & 0x0F)) << 22);
	uiStartTime |=  (((unsigned int)(pStartTime->ucYear & 0x3F)) << 26);

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB4_REQ_IDS_START_TIME, uiStartTime));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB4);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B4_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	06.04.2011 / M. Hultsch

  Version       	:	03.03.01 / 07.04.2011 / M. Hultsch

  Function			:	[0xB4][0xA8] Get Log State
						returns the logging state

  Parameters		:	string sAccessPassword			- [in] optional authent password
						LIMITS_COUNTER* pLimitsCounter
						SYSTEM_STATUS* pSystemStatus
						SHELF_LIFE_0* pSF0
						SHELF_LIFE_1* pSF1
						unsigned int& uiCurrentShelfLife
						unsigned char& ucStatusFlags

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A::GetLogState(
	string sAccessPassword,
	LIMITS_COUNTER* pLimitsCounter,
	SYSTEM_STATUS* pSystemStatus,
	SHELF_LIFE_0* pSF0,
	SHELF_LIFE_1* pSF1,
	unsigned int& uiCurrentShelfLife,
	unsigned char& ucStatusFlags )
{
	int iBack = 0;
	int iErr = 0;
	unsigned int uiSystemStatus = 0;
	unsigned int uiLimitsCounter = 0;
	unsigned int uiSF0 = 0;
	unsigned int uiSF1 = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_CMD, (unsigned char)0xA8));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EPC_LF, (bool)true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC_LEN, m_pTabItem->m_ucSnrLen));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK, (unsigned char)0));
	if(sAccessPassword.length() > 0)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK_ACCESS_FLAG, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW_LENGTH, (unsigned char)(sAccessPassword.length()/2)));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW, sAccessPassword));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, false));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB4);
	if(iBack == 0)
	{
		// get measurement setup data
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB4_RSP_IDS_LIMITS_COUNTER, &uiLimitsCounter));
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB4_RSP_IDS_SYSTEM_STATUS, &uiSystemStatus));
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB4_RSP_IDS_SL_BLOCK_0, &uiSF0));
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB4_RSP_IDS_SL_BLOCK_1, &uiSF1));
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB4_RSP_IDS_CURRENT_SHELF_LIFE, &uiCurrentShelfLife));
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB4_RSP_IDS_STATUS_FLAG, &ucStatusFlags));

		pLimitsCounter->ucExtremeUpperLimitsCounter	= (unsigned char) (uiLimitsCounter & 0x000000FF);
		pLimitsCounter->ucUpperLimitsCounter		= (unsigned char)((uiLimitsCounter & 0x0000FF00) >>  8);
		pLimitsCounter->ucLowerLimitsCounter		= (unsigned char)((uiLimitsCounter & 0x00FF0000) >> 16);
		pLimitsCounter->ucExtremeLowerLimitsCounter	= (unsigned char)((uiLimitsCounter & 0xFF000000) >> 24);

		if(uiSystemStatus & 0x00000001)
			pSystemStatus->bActive = true;
		else
			pSystemStatus->bActive = false;

		pSystemStatus->uiNoOfMeasurements = ((uiSystemStatus & 0x0000FFFE) >> 1);
		pSystemStatus->ucNoOfMemoryReplacements = (unsigned char)((uiSystemStatus & 0x003F0000) >> 16);
		pSystemStatus->uiMeasurementAddressPointer = ((uiSystemStatus & 0xFFC0000) >> 22);

		pSF1->ucSkipLog = (unsigned char)(uiSF1 & 0x00000003);
		if(uiSF1 & 0x00000004)
			pSF1->bEnableAlgorithm = true;
		else
			pSF1->bEnableAlgorithm = false;

		if(uiSF1 & 0x00000008)
			pSF1->bEnableNegativeShelfLife = true;
		else
			pSF1->bEnableNegativeShelfLife = false;

		pSF1->ucSensorID	= (unsigned char)((uiSF1 & 0x00000030) >> 4);
		pSF1->uiTinit		= (unsigned int)((uiSF1 & 0x0000FFC0) >> 6);
		pSF1->uiSLinit		= (unsigned int)((uiSF1 & 0xFFFF0000) >> 16);
		pSF0->ucActivationEnergy	= (unsigned char) (uiSF0 & 0x000000FF);
		pSF0->ucNormalTemperature	= (unsigned char)((uiSF0 & 0x0000FF00) >>  8);
		pSF0->ucMinimalTemperature	= (unsigned char)((uiSF0 & 0x00FF0000) >> 16);
		pSF0->ucMaximalTemperature	= (unsigned char)((uiSF0 & 0xFF000000) >> 24);
	}
	else if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B4_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	06.04.2011 / M. Hultsch

  Version       	:	03.03.01 / 08.04.2011 / M. Hultsch

  Function			:	[0xB4][0xA9] Get Calibration Data
						reads internal and external calibration data from System Area Blocks 0x010..0x016, 0x030..0x037

  Parameters		:	string sAccessPassword			- [in] optional authent password
						CALIBRATION* pCalData
						SFE_PARAMETERS* pSfeData


  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A::GetCalibrationData(	
	string sAccessPassword,
	CALIBRATION* pCalData,
	SFE_PARAMETERS* pSfeData )
{
	int iBack = 0;
	int iErr = 0;
	unsigned int uiSfeData = 0;
	__int64 i64CalibrationData = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_CMD, (unsigned char)0xA9));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EPC_LF, (bool)true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC_LEN, m_pTabItem->m_ucSnrLen));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK, (unsigned char)0));
	if(sAccessPassword.length() > 0)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK_ACCESS_FLAG, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW_LENGTH, (unsigned char)(sAccessPassword.length()/2)));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW, sAccessPassword));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, false));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB4);
	if(iBack == 0)
	{
		// get calibration data
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB4_RSP_IDS_CAL_DATA, &i64CalibrationData));
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB4_RSP_IDS_SFE_PARA, &uiSfeData));

#ifdef _FEDM_WINDOWS
	#if _MSC_VER <= 1200
		pCalData->ucRefTc		= (unsigned char)((i64CalibrationData & 0x0000000000000078) >>  3);
		pCalData->ucOffInt		= (unsigned char)((i64CalibrationData & 0x0000000000003F80) >>  7);
		pCalData->ucRingCall	= (unsigned char)((i64CalibrationData & 0x000000000007C000) >> 14);
		pCalData->ucIRLev		= (unsigned char)((i64CalibrationData & 0x0000000000180000) >> 19);
		pCalData->ucSelp22		= (unsigned char)((i64CalibrationData & 0x0000000000600000) >> 21);
		
		if(i64CalibrationData & 0x0000000000800000)
			pCalData->bSwExtEn	= true;
		else
			pCalData->bSwExtEn	= false;

		pCalData->ucDf			= (unsigned char)((i64CalibrationData & 0x00000000FF000000) >> 24);
		pCalData->ucAdf			= (unsigned char)((i64CalibrationData & 0x0000001F00000000) >> 32);
		pCalData->ucSelp12		= (unsigned char)((i64CalibrationData & 0x0000006000000000) >> 37);
		
		if(i64CalibrationData & 0x0000008000000000)
			pCalData->bGndSwitch	= true;
		else
			pCalData->bGndSwitch	= false;

		pCalData->ucCoars2		= (unsigned char)((i64CalibrationData & 0x0000070000000000) >> 40);
		pCalData->ucAd2			= (unsigned char)((i64CalibrationData & 0x0000F80000000000) >> 43);
		pCalData->ucCoars1		= (unsigned char)((i64CalibrationData & 0x0007000000000000) >> 48);
		pCalData->ucAd1			= (unsigned char)((i64CalibrationData & 0x00F8000000000000) >> 51);
	#else
		pCalData->ucRefTc		= (unsigned char)((i64CalibrationData & 0x0000000000000078LL) >>  3);
		pCalData->ucOffInt		= (unsigned char)((i64CalibrationData & 0x0000000000003F80LL) >>  7);
		pCalData->ucRingCall	= (unsigned char)((i64CalibrationData & 0x000000000007C000LL) >> 14);
		pCalData->ucIRLev		= (unsigned char)((i64CalibrationData & 0x0000000000180000LL) >> 19);
		pCalData->ucSelp22		= (unsigned char)((i64CalibrationData & 0x0000000000600000LL) >> 21);
		
		if(i64CalibrationData & 0x0000000000800000LL)
			pCalData->bSwExtEn	= true;
		else
			pCalData->bSwExtEn	= false;

		pCalData->ucDf			= (unsigned char)((i64CalibrationData & 0x00000000FF000000LL) >> 24);
		pCalData->ucAdf			= (unsigned char)((i64CalibrationData & 0x0000001F00000000LL) >> 32);
		pCalData->ucSelp12		= (unsigned char)((i64CalibrationData & 0x0000006000000000LL) >> 37);
		
		if(i64CalibrationData & 0x0000008000000000LL)
			pCalData->bGndSwitch	= true;
		else
			pCalData->bGndSwitch	= false;

		pCalData->ucCoars2		= (unsigned char)((i64CalibrationData & 0x0000070000000000LL) >> 40);
		pCalData->ucAd2			= (unsigned char)((i64CalibrationData & 0x0000F80000000000LL) >> 43);
		pCalData->ucCoars1		= (unsigned char)((i64CalibrationData & 0x0007000000000000LL) >> 48);
		pCalData->ucAd1			= (unsigned char)((i64CalibrationData & 0x00F8000000000000LL) >> 51);
		#endif
#endif
#ifdef _FEDM_LINUX
		pCalData->ucRefTc		= (unsigned char)((i64CalibrationData & 0x0000000000000078LL) >>  3);
		pCalData->ucOffInt		= (unsigned char)((i64CalibrationData & 0x0000000000003F80LL) >>  7);
		pCalData->ucRingCall	= (unsigned char)((i64CalibrationData & 0x000000000007C000LL) >> 14);
		pCalData->ucIRLev		= (unsigned char)((i64CalibrationData & 0x0000000000180000LL) >> 19);
		pCalData->ucSelp22		= (unsigned char)((i64CalibrationData & 0x0000000000600000LL) >> 21);

		if(i64CalibrationData & 0x0000000000800000LL)
			pCalData->bSwExtEn	= true;
		else
			pCalData->bSwExtEn	= false;

		pCalData->ucDf			= (unsigned char)((i64CalibrationData & 0x00000000FF000000LL) >> 24);
		pCalData->ucAdf			= (unsigned char)((i64CalibrationData & 0x0000001F00000000LL) >> 32);
		pCalData->ucSelp12		= (unsigned char)((i64CalibrationData & 0x0000006000000000LL) >> 37);

		if(i64CalibrationData & 0x0000008000000000LL)
			pCalData->bGndSwitch	= true;
		else
			pCalData->bGndSwitch	= false;

		pCalData->ucCoars2		= (unsigned char)((i64CalibrationData & 0x0000070000000000LL) >> 40);
		pCalData->ucAd2			= (unsigned char)((i64CalibrationData & 0x0000F80000000000LL) >> 43);
		pCalData->ucCoars1		= (unsigned char)((i64CalibrationData & 0x0007000000000000LL) >> 48);
		pCalData->ucAd1			= (unsigned char)((i64CalibrationData & 0x00F8000000000000LL) >> 51);
#endif

		pSfeData->ucVerifySensorID	= (unsigned char)(uiSfeData & 0x00000003);
		
		if(uiSfeData & 0x00000004)
			pSfeData->bAutoRangePreset = true;
		else
			pSfeData->bAutoRangePreset = false;

		pSfeData->ucSext2	= (unsigned char)((uiSfeData & 0x00000008) >>  3);
		pSfeData->ucSext1	= (unsigned char)((uiSfeData & 0x00000030) >>  4);
		pSfeData->ucSeti	= (unsigned char)((uiSfeData & 0x000007C0) >>  6);
		pSfeData->ucRang	= (unsigned char)((uiSfeData & 0x0000F800) >> 11);
	}
	else if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B4_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	06.04.2011 / M. Hultsch

  Version       	:	03.03.01 / 08.04.2011 / M. Hultsch

  Function			:	[0xB4][0xAA] Get Battery Level
						returns the actual battery level

  Parameters		:	string sAccessPassword			- [in] optional authent password
						unsigned char ucBatteryRetrigger
						unsigned char& ucBatteryType
						unsigned int& ucBatteryLevel

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A::GetBatteryLevel(	
	string sAccessPassword,
	unsigned char ucBatteryRetrigger,
	bool& bError,
	unsigned char& ucBatteryType,
	unsigned int& uiBatteryLevel )
{
	int iBack = 0;
	int iErr = 0;
	unsigned int uiData = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_CMD, (unsigned char)0xAA));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EPC_LF, (bool)true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC_LEN, m_pTabItem->m_ucSnrLen));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK, (unsigned char)0));
	if(sAccessPassword.length() > 0)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK_ACCESS_FLAG, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW_LENGTH, (unsigned char)(sAccessPassword.length()/2)));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW, sAccessPassword));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, false));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB4_REQ_IDS_BATTERY_RETRIGGER, ucBatteryRetrigger));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB4);
	if(iBack == 0)
	{
		// get measurement setup data
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB4_RSP_IDS_BATTERY_LEVEL, &uiData));

		if(uiData & 0x00008000)
			bError = true;
		else
			bError = false;

		ucBatteryType	= (unsigned char)((uiData & 0x00004000) >> 14);
		uiBatteryLevel	= (uiData & 0x000003FF);
	}
	else if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B4_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	06.04.2011 / M. Hultsch

  Version       	:	03.03.01 / 08.04.2011 / M. Hultsch

  Function			:	[0xB4][0xAB] Set Shelf-Life
						set shelf-life parameters for the dynamic shelf-life algorithm

  Parameters		:	string sAccessPassword			- [in] optional authent password
						SHELF_LIFE_0* pSF0
						SHELF_LIFE_1* pSF1

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A::SetShelfLife(
	string sAccessPassword,
	SHELF_LIFE_0* pSF0,
	SHELF_LIFE_1* pSF1 )
{
	int iBack = 0;
	int iErr = 0;
	unsigned int uiSF0 = 0;
	unsigned int uiSF1 = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_CMD, (unsigned char)0xAB));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EPC_LF, (bool)true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC_LEN, m_pTabItem->m_ucSnrLen));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK, (unsigned char)0));
	if(sAccessPassword.length() > 0)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK_ACCESS_FLAG, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW_LENGTH, (unsigned char)(sAccessPassword.length()/2)));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW, sAccessPassword));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, false));
	}

	uiSF0  =  (unsigned int)pSF0->ucActivationEnergy;
	uiSF0 |= (((unsigned int)pSF0->ucNormalTemperature) << 8);
	uiSF0 |= (((unsigned int)pSF0->ucMinimalTemperature) << 16);
	uiSF0 |= (((unsigned int)pSF0->ucMaximalTemperature) << 24);

	uiSF1 = (unsigned int)(pSF1->ucSkipLog & 0x03);
	if(pSF1->bEnableAlgorithm)
		 uiSF1 |= 0x00000004;

	if(pSF1->bEnableNegativeShelfLife)
		 uiSF1 |= 0x00000008;

	uiSF1 |= (((unsigned int)(pSF1->ucSensorID & 0x03)) << 4);
	uiSF1 |= (((unsigned int)(pSF1->uiTinit & 0x000003FF)) << 6);
	uiSF1 |= (((unsigned int)(pSF1->uiSLinit & 0x0000FFFF)) << 16);

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB4_REQ_IDS_SL_BLOCK_0, uiSF0));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB4_REQ_IDS_SL_BLOCK_1, uiSF1));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB4);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B4_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	06.04.2011 / M. Hultsch

  Version       	:	03.03.01 / 08.04.2011 / M. Hultsch

  Function			:	[0xB4][0xAC] Initialize
						writes parameters to System Area Blocks 0x029..0x02C

  Parameters		:	string sAccessPassword			- [in] optional authent password
						DELAY_TIME* pDelayTime
						USER_DATA* pUserData

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A::Initialize(	
	string sAccessPassword,
	DELAY_TIME* pDelayTime,
	APP_DATA* pAppData )
{
	int iBack = 0;
	int iErr = 0;
	unsigned int uiDelayTime = 0;
	unsigned int uiAppData = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_CMD, (unsigned char)0xAC));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EPC_LF, (bool)true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC_LEN, m_pTabItem->m_ucSnrLen));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK, (unsigned char)0));
	if(sAccessPassword.length() > 0)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK_ACCESS_FLAG, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW_LENGTH, (unsigned char)(sAccessPassword.length()/2)));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW, sAccessPassword));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, false));
	}

	// compose start time
	if(pDelayTime->bTimer)
		uiDelayTime |= 0x00000001;

	if(pDelayTime->bDelayMode_External)
		uiDelayTime |= 0x00000002;

	uiDelayTime |= ((pDelayTime->uiDelayTime & 0x00000FFF) << 4);

	// compose initialization data
	uiAppData  = (unsigned int)(pAppData->ucBrokenWordPointer & 0x07);
	uiAppData |= ((pAppData->uiNoOfWordsForAppData & 0x000001FF) << 7);

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB4_REQ_IDS_DELAY_TIME, uiDelayTime));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB4_REQ_IDS_APP_DATA, uiAppData));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB4);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B4_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	06.04.2011 / M. Hultsch

  Version       	:	03.03.01 / 08.04.2011 / M. Hultsch

  Function			:	[0xB4][0xAD] Get Sensor Value
						starts the AD conversion on the specified sensor and returns the measured value

  Parameters		:	string sAccessPassword
						unsigned char ucSensorType
						bool& bError
						unsigned char& ucRangeOrLimit
						unsigned int& uiSensorValue

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A::GetSensorValue(	
	string sAccessPassword,
	unsigned char ucSensorType,
	bool& bError,
	unsigned char& ucRangeOrLimit,
	unsigned int& uiSensorValue )
{
	int iBack = 0;
	int iErr = 0;
	unsigned int uiSensorInfo = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_CMD, (unsigned char)0xAD));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EPC_LF, (bool)true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC_LEN, m_pTabItem->m_ucSnrLen));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK, (unsigned char)0));
	if(sAccessPassword.length() > 0)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK_ACCESS_FLAG, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW_LENGTH, (unsigned char)(sAccessPassword.length()/2)));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW, sAccessPassword));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, false));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB4_REQ_IDS_SENSOR_VALUE, (unsigned char)(ucSensorType & 0x03)));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB4);
	if(iBack == 0)
	{
		// get measurement setup data
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB4_RSP_IDS_SENSOR_INFO, &uiSensorInfo));

		if(uiSensorInfo & 0x00008000)
			bError = true;
		else
			bError = false;

		ucRangeOrLimit = (unsigned char)((uiSensorInfo & 0x00007C00) >> 10);
		uiSensorValue = (uiSensorInfo & 0x000003FF);
	}
	else if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B4_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	06.04.2011 / M. Hultsch

  Version       	:	03.03.01 / 08.04.2011 / M. Hultsch

  Function			:	[0xB4][0xAE] Open Area

  Parameters		:	string sAccessPassword			- [in] optional authent password
						unsigned char ucPasswordLevel
						string sPassword

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A::OpenArea(
	string sAccessPassword,
	unsigned char ucPasswordLevel,
	string sPassword )
{
	int iBack = 0;
	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_CMD, (unsigned char)0xAE));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EPC_LF, (bool)true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC_LEN, m_pTabItem->m_ucSnrLen));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK, (unsigned char)0));
	if(sAccessPassword.length() > 0)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK_ACCESS_FLAG, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW_LENGTH, (unsigned char)(sAccessPassword.length()/2)));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW, sAccessPassword));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, false));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB4_REQ_IDS_PASSWORD_LEVEL, ucPasswordLevel));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB4_REQ_IDS_PASSWORD, sPassword));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB4);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B4_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	08.04.2011 / M. Hultsch

  Version       	:	03.03.01 / 08.04.2011 / M. Hultsch

  Function			:	[0xB4][0xAF] Access FIFO
						can read and write data from the FIFO and can also read the FIFO status register.

  Parameters		:	string sAccessPassword			- [in] optional authent password
						unsigned char ucSubCommand
						unsigned char ucNoOfBytes
						unsigned char* pucBuffer

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A::AccessFIFO(
	string sAccessPassword,
	unsigned char ucSubCommand,
	unsigned char ucNoOfBytes,
	unsigned char* pucBuffer )
{
	FEDM_CHK3(pucBuffer);

	if(ucSubCommand == 0x04 || ucSubCommand == 0x05) // read/write from/to FIFO
	{
		if(ucNoOfBytes > 8)
			return FEDM_ERROR_BUFFER_LENGTH;
	}

	int iBack = 0;
	int iErr = 0;
	unsigned char ucReqData = 0;
	unsigned char ucPayload[8];

	memset(ucPayload, 0, 8);
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_CMD, (unsigned char)0xAF));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		return FEDM_ERROR_UNSUPPORTED;
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EPC_LF, (bool)true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC_LEN, m_pTabItem->m_ucSnrLen));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_EPC, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK, (unsigned char)0));
	if(sAccessPassword.length() > 0)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_REQ_BANK_ACCESS_FLAG, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW_LENGTH, (unsigned char)(sAccessPassword.length()/2)));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_ACCESS_PW, sAccessPassword));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, false));
	}

	ucReqData |= ((ucSubCommand & 0x07) << 5);
	if(ucSubCommand == 0x04 || ucSubCommand == 0x05)
		ucReqData |= (ucNoOfBytes & 0x0F);

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB4_REQ_IDS_SUB_COMMAND, ucReqData));
	if(ucSubCommand == 0x05) // write to FIFO
	{
		memcpy(ucPayload, pucBuffer, (size_t)ucNoOfBytes);
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_0xB4_REQ_IDS_PAYLOAD, ucPayload, ucNoOfBytes));
	}


	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B4_MFR, (unsigned char)FEDM_ISC_ISO_MFR_IDS));
	
	// ... and execute
	iBack = m_pReader->SendProtocol(0xB4);
	if(iBack == 0)
	{
		if(ucSubCommand == 0x04) // read from FIFO
		{
			FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB4_RSP_IDS_PAYLOAD, ucPayload, ucNoOfBytes));
			memcpy(pucBuffer, ucPayload, (size_t)ucNoOfBytes);
		}
		else if(ucSubCommand == 0x06) // read status register
		{
			FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_0xB4_RSP_IDS_PAYLOAD, ucPayload, 1));
			pucBuffer[0] = ucPayload[0];
		}
	}
	else if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B4_ISO_ERROR, &m_ucTagError));
	}

	return iBack;
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_EPC_C1_G2)
