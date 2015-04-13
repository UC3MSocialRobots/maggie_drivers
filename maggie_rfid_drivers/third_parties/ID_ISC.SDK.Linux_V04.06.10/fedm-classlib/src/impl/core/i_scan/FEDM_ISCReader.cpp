/*-------------------------------------------------------
|                                                       |
|                 FEDM_ISCReader.cpp                    |
|                                                       |
---------------------------------------------------------

Copyright © 2000-2014	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	07.06.2000
	
Version       		:	04.06.10 / 02.12.2014 / M. Hultsch
						- new [0xB4] NXP Custom Commands: 0x10, 0x11, 0x12
						- new [0xB4] EM Custom Commands: 0x00, 0x01, 0x02, 0x04, 0xD1

						04.06.09 / 30.10.2014 / M. Hultsch
						- support for new baudrates: 921600 and 460800
  
						04.06.08 / 13.10.2014 / M. Hultsch
						- [0x66] Reader Info: additional LAN link information with mode=0x50

						04.06.06 / 23.09.2013 / M. Hultsch
						- only for EPC Class1 Gen2:
						  bugfix for addressed Host commands, if Reader is configured for UID = EPC + TID

						04.06.04 / 20.08.2014 / M. Hultsch
						- bugfix in SendProtocol: [0xB0] with received status 0x02 (data false) returns now at once for all sub-commands, except [0x01] Inventory

						04.06.03 / 26.06.2014 / M. Hultsch
						- [0x2C] Get Multiple Block Security Status with extended addressed mode: bugfix for received data above address 255
						- new reader: ID MAX.U1002

						04.06.01 / 07.03.2014 / M. Hultsch
						- support for [0x22] Lock Multiple Blocks with extended addressed mode
						- support for [0x2C] Get Multiple Block Security Status with extended addressed mode
						- [0x6E] Reader Diagnostic: support for new mode 0x07

						04.05.05 / 29.11.2013 / M. Hultsch
						- exception for ISO 14443 Transponder after FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true), when Reader is configuered with Anticollision = off
						- new reader: ID CPR.POS and ID ISC.PRH200
						- [0x66] Reader Info: new mode 0x09 for WLAN-Firmware
						
						04.05.04 / 03.09.2013 / M. Hultsch
						- new reader: ID ISC.LRU1002

						04.05.03 / 30.08.2013 / M. Hultsch
						- new reader: OBID myAXXESS onTop
						- bugfix for LRU3x00: max number of CFG-pages increased from 86 to 88

						04.04.03 / 05.03.2013 / M. Hultsch
						- bugfix for EPC with 8-Byte length
						- new method: GetDependentLibVersions

						
						04.04.02 / 18.01.2013 / M. Hultsch
						- support for new reader ID CPR47
						- SendProtocol: [0xB0] with status 0x84 (RF-Warning) takes returned records

						04.03.01 / 05.11.2012 / M. Hultsch
						- new methods: SetCommandPara/GetCommandPara
						- support for Recommissioning Bits in [0xB3][0x18] Kill
						- Reader Diagnostic: support for new mode 0x21

						04.02.06 / 03.08.2012 / M. Hultsch
						- FinBaudRate returns crypto processing error

						04.02.06 / 03.08.2012 / M. Hultsch
						- new reader: ID CPR46
						- [0x66] Read Reader Info: new mode 0x08 (with firmware supported tag drivers)

						04.02.05 / 05.07.2012 / M. Hultsch
						- new reader: ID CPR20

						04.02.04 / 19.06.2012 / M. Hultsch
						- BRM: support for direction information (in combination with Gate People Counter)
						- UHF Class1 Gen2: support for [0xB3][0x25] Block Permalock and [0xB3][0x26] Read Permalock Status

						04.02.01 / 31.05.2012 / M. Hultsch
						- support for new tag types: Innovatron and ASK CTx

						04.00.10 / 10.02.2012 / M. Hultsch
						- support for external logmanager (FELOGMNG.dll), when #define _FELOGMNG_SUPPORT is set

						04.00.09 / 23.01.2012 / M. Hultsch
						- new reader: ID ISC.LR1002

						04.00.06 / 01.12.2011 / M. Hultsch
						- new nPA reader: OBID myAXXESS standard, OBID myAXXESS comfort
						- workaround for LRU3000 for get/set selection mask
					
						04.00.02 / 09.08.2011 / M. Hultsch
						- new method for setting OEM Reader names

						04.00.01 / 04.08.2011 / M. Hultsch
						- bugfix for LR2500-A/B and LRU3000 concerning m_iUsedEEDataBlocks, m_iUsedRAMDataBlocks

						04.00.00 / 29.07.2011 / M. Hultsch
						- max. UID length increased to 96 Bytes
						- LRU3000: ACC configuration moved to device RFC
						- only for EPC Class1 Gen2:
						  if Reader is configured for UID = EPC + TID, set TID as UID for next Host-Mode request in addressed mode
						- support for [0x74] Input Event from CPR50

						03.03.03 / 09.05.2011 / M. Hultsch
						- parameter m_ucReaderType moved from unsigned char to unsigned int

						03.03.01 / 19.04.2011 / B. Tschunko
						- FindBaudRate() alternating (iFrameSupport = 3), beginning with standard protocol frame 

						03.03.01 / 31.03.2011 / M. Hultsch
						- new TagHandler classes for ISO 15693 IDS SL13A
						  and EPC Class1 Gen2 IDS SL900A
						- FindBaudRate() with modified default parameter (iFrameSupport = 3)

						03.03.00 / 21.02.2011 / M. Hultsch
						- ID ISC.LR2500-A added

						03.02.09 / 21.11.2010 / M. Hultsch
						- GetTableData extended for MAC Address

						03.02.07 / 29.10.2010 / M. Hultsch
						- Updated [0x66] Reader Info
						- Support for [0xC3] DESFire commands

						03.02.05 / 18.10.2010 / M. Sahm
						- Get_0xC3_IsoTableData(): Command 0xFA (Authent): No ApplicationID used anymore
						
						03.02.05 / 08.09.2010 / M. Hultsch
						- support for [0xC3] DESFire Command

						03.01.05 / 23.04.2010 / M. Hultsch
						- new UHF custom commands for NXP
						- ID ISC.MR102 and ID ISC.LR2500-B added

						03.01.03 / 25.03.2010 / M. Hultsch
						- modifications for peripheral devices

						03.01.02 / 18.03.2010 / M. Hultsch
						- handling of new USB enumeration after [0x64] System Reset

						03.01.01 / 25.02.2010 / M. Hultsch
						- support for peripheral devices
						- Reader Info addings

						03.01.00 / 05.02.2010 / M. Hultsch
						- Reader Info addings
						- new FU-Type: ID ISC.ANT.GPC
						- full support for CPR50.XX

						03.00.18 / 15.12.2009 / M. Hultsch
						- ID MLC.COM and ID MLC.EMPS added

						03.00.17 / 24.11.2009 / M. Hultsch
						03.00.16 / 20.11.2009 / M. Hultsch
						03.00.15 / 17.11.2009 / M. Hultsch				
						03.00.14 / 29.10.2009 / M. Hultsch
						- ID MAX50.xx added
						- [0xBx] Host Commands in non-addressed mode from table index > 0
						- ISO 15693 Cust & Prop Commands

Operation Systems	:	independent

Function			:	classes for OBID i-scan® and OBID® classic-pro reader family

Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#include <time.h>
#include <stdio.h>
#include <math.h>
#include "FEDM_ISCReader.h"
#include "FEDM_BRMTabItem.h"
#include "FEDM_ISOTabItem.h"
#include "FEDM_ISCReaderID.h"
#if !defined(_FEDM_NO_PD_SUPPORT)
	#include "./peripheral_devices/FedmIscPeripheralDevice.h"
	#include "./peripheral_devices/FedmIscPeopleCounter.h"
	#include "./peripheral_devices/FedmIscExternalIO.h"
#endif
#include "../FEDM_Functions.h"
#include "../FEDM_XmlReaderCfgDataModul.h"
#include "../FEDM_XmlReaderCfgProfileModul.h"
#include "feisc.h"
#ifdef _FEDM_FU_SUPPORT
	#include "fefu.h"
#endif
#ifdef _FEDM_TAG_HANDLER
  #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
	#include "fetcl.h"
  #endif
#endif

#if defined(_FEDM_ISC_CORE_DLL) || defined(_OBIDISC4J_DLL) || defined(_OBIDISC4NETnativePI_DLL)
	#include "feswversions.h"
#endif

#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif

#ifdef _FEDM_LINUX
	#include <stdlib.h>
#endif


//####################################################################
// class FEDM_CADTable
//####################################################################

#if !defined(__BORLANDC__)
FEDM_CADTable::FEDM_CADTable()
{
	Init();
}

FEDM_CADTable::~FEDM_CADTable()
{
	FEDM_CAD_HISTORY_ARRAY_ITOR Itor;

	for(Itor = m_History.begin(); Itor != m_History.end(); Itor++)
	{
		if((*Itor) != NULL)
			delete (*Itor);
	}

	m_History.clear();
}

void FEDM_CADTable::Init()
{
	FEDM_CAD_HISTORY_ARRAY_ITOR Itor;

	for(Itor = m_History.begin(); Itor != m_History.end(); Itor++)
	{
		if((*Itor) != NULL)
			delete (*Itor);
	}

	m_History.clear();
	memset(m_ucPowerAvg,	0, 15);
	memset(m_ucPowerPeak,	0, 15);
	memset(m_nData,			0, 4*sizeof(long));

	m_ucMode			= 0;
	m_bValid			= false;
}
#endif



//####################################################################
// class FEDM_ISCReader
//####################################################################

//////////////////////////////////////////////////////////////////////
// Construction/Destruction	
//////////////////////////////////////////////////////////////////////

FEDM_ISCReader::FEDM_ISCReader()
{
	int i;

	m_bDisableReadCfgBeforeWriteCfg = false;

	m_bEnableTagHandler			= false;	// must be enabled manually
#ifdef _FEDM_TAG_HANDLER
	m_bTagHandlerOption_CreateCondition_Smart = false;
#endif

	m_bSelectWithCinf			= false;	// is set after detecting reader type
	m_bDisableSelectWithCinf	= false;	// is a general setting

	m_uiBRMTableLength	= 0;
	m_uiISOTableLength	= 0;
	m_uiNonAddressedTabIndex = 0;

	memset( m_ucData, 0, FEDM_MAX_PROTOCOL_SIZE+1);
	memset( m_nData,  0,  516*sizeof(long));

	m_ReaderInfo.Init();
	m_ReaderDiagnostic.Init();

	m_iUsedEEDataBlocks		= 64;	// do not change this setting !!!
	m_iUsedRAMDataBlocks	= 64;	// do not change this setting !!!
	m_iUsedAccEEDataBlocks	= 128;	// do not change this setting !!!
	m_iUsedAccRAMDataBlocks	= 128;	// do not change this setting !!!
	m_iUsedTmpDataBlocks	= 128;	// do not change this setting !!!
	m_iUsedMjpDataBlocks	= 0;	// do not change this setting !!!
	m_iUsedSNMemBlocks		= 0;	// do not change this setting !!!
	m_iUsedIDMemBlocks		= 0;	// do not change this setting !!!
	m_iUsedACMemBlocks		= 0;	// do not change this setting !!!
	m_iUsedPubMemBlocks		= 0;	// do not change this setting !!!
	m_iUsedSecMemBlocks		= 0;	// do not change this setting !!!
	m_iUsedConfMemBlocks	= 0;	// do not change this setting !!!
	m_iUsedDateMemBlocks	= 0;	// do not change this setting !!!

	// identifies this object as an OBID i-scan and OBID classic-pro object
	strcpy(m_sFamCode, FEDM_STR_ISC_FAMILY);

	// memory for reader
	m_RFC_EEData.reserve(FEDM_ISC_MAX_RFC_EEDATA_MEM);		// parameter in EEPROM
	m_RFC_EEData.resize(FEDM_ISC_MAX_RFC_EEDATA_MEM);		// parameter in EEPROM
	m_RFC_RAMData.reserve(FEDM_ISC_MAX_RFC_RAMDATA_MEM);	// parameter in EEPROM
	m_RFC_RAMData.resize(FEDM_ISC_MAX_RFC_RAMDATA_MEM);		// parameter in EEPROM

	m_ACC_EEData.reserve(FEDM_ISC_MAX_ACC_EEDATA_MEM);		// parameter in EEPROM
	m_ACC_EEData.resize(FEDM_ISC_MAX_ACC_EEDATA_MEM);		// parameter in EEPROM
	m_ACC_RAMData.reserve(FEDM_ISC_MAX_ACC_RAMDATA_MEM);	// parameter in RAM
	m_ACC_RAMData.resize(FEDM_ISC_MAX_ACC_RAMDATA_MEM);		// parameter in RAM

	// memory for temporary data
	m_TmpData.reserve(FEDM_ISC_MAX_TMPDATA_MEM);
	m_TmpData.resize(FEDM_ISC_MAX_TMPDATA_MEM);

	// memories for profile masks
	m_RFC_CfgProfileMask.reserve(FEDM_ISC_MAX_RFC_EEDATA_MEM);
	m_RFC_CfgProfileMask.resize(FEDM_ISC_MAX_RFC_EEDATA_MEM);
	m_ACC_CfgProfileMask.reserve(FEDM_ISC_MAX_ACC_EEDATA_MEM);
	m_ACC_CfgProfileMask.resize(FEDM_ISC_MAX_ACC_EEDATA_MEM);

	// initialize memories with 0x00
	for( i=0; i<FEDM_ISC_MAX_RFC_EEDATA_MEM; i++ )		{ m_RFC_EEData[i]			= 0x00; }
	for( i=0; i<FEDM_ISC_MAX_RFC_RAMDATA_MEM; i++ )		{ m_RFC_RAMData[i]			= 0x00; }
	
	for( i=0; i<FEDM_ISC_MAX_ACC_EEDATA_MEM; i++ )		{ m_ACC_EEData[i]			= 0x00; }
	for( i=0; i<FEDM_ISC_MAX_ACC_RAMDATA_MEM; i++ )		{ m_ACC_RAMData[i]			= 0x00; }

	for( i=0; i<FEDM_ISC_MAX_TMPDATA_MEM; i++ )			{ m_TmpData[i]				= 0x00; }

	for( i=0; i<FEDM_ISC_MAX_RFC_EEDATA_MEM; i++ )		{ m_RFC_CfgProfileMask[i]	= 0x00; }
	for( i=0; i<FEDM_ISC_MAX_ACC_EEDATA_MEM; i++ )		{ m_ACC_CfgProfileMask[i]	= 0x00; }
}

FEDM_ISCReader::~FEDM_ISCReader()
{
	unsigned int i;
#if !defined(_FEDM_NO_PD_SUPPORT)
	FEDM_PD_MAP_ITOR itor;
#endif

	// clean up
	m_RFC_EEData.clear();
	m_RFC_RAMData.clear();
	m_ACC_EEData.clear();
	m_ACC_RAMData.clear();
	m_TmpData.clear();
	m_RFC_CfgProfileMask.clear();
	m_ACC_CfgProfileMask.clear();

	for( i=0; i<m_BRMTable.size(); i++ )
		delete m_BRMTable[i];

	for( i=0; i<m_ISOTable.size(); i++ )
		delete m_ISOTable[i];

	m_BRMTable.clear();
	m_ISOTable.clear();

#if !defined(_FEDM_NO_PD_SUPPORT)
	for(itor  = m_mapPeripheralDevices.begin();
		itor != m_mapPeripheralDevices.end();
		itor++ )
	{
		if(itor->second != NULL)
			delete itor->second;
	}
#endif
}



/***************************************************************************
  Begin        	:	23.04.2008 / M. Hultsch
  Version       :	03.00.11 / 06.08.2009 / M. Hultsch

  Function		:	check version numbers of dependent libraries
					FECOM, FETCP, FEUSB, FEISC, FEFU, FETCL

  Parameters	:	string& sMessage	- reference to STL-string
					
  Return value	:	FEDM_OK or error code

***************************************************************************/
int FEDM_ISCReader::EvalLibDependencies(string& sMessage)
{
	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL, __FUNCTION__);

#if !defined(_FEDM_ISC_CORE_DLL) && !defined(_OBIDISC4J_DLL) && !defined(_OBIDISC4NETnativePI_DLL)
	int iBack = FEDM_ERROR_UNSUPPORTED;
	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
#else
	int iBack = FEDM_OK;
	unsigned int uiTmp[3];
	unsigned int uiVersionLib = 0;
	unsigned int uiVersionMin = 0;
	char cVersion[256];
	char cTmp[256];
	
	// check FEISC
	memset(uiTmp, 0, 3);
	uiVersionLib = 0;
	uiVersionMin = 0;
	FEISC_GetDLLVersion(cVersion);
	sscanf(cVersion, "%d.%d.%d", &uiTmp[0], &uiTmp[1], &uiTmp[2]);
	uiVersionLib = (uiTmp[0]<<16) + (uiTmp[1]<<8) + uiTmp[2];
#ifdef _FEDM_WINDOWS
	#ifdef _WIN32_WCE
	sscanf(FEISC_CE_DLL_VERSION, "%d.%d.%d", &uiTmp[0], &uiTmp[1], &uiTmp[2]);
	#else
	sscanf(FEISC_DLL_VERSION, "%d.%d.%d", &uiTmp[0], &uiTmp[1], &uiTmp[2]);
	#endif
#endif
#ifdef _FEDM_LINUX
	sscanf(FEISC_TUX_DLL_VERSION, "%d.%d.%d", &uiTmp[0], &uiTmp[1], &uiTmp[2]);
#endif
	uiVersionMin = (uiTmp[0]<<16) + (uiTmp[1]<<8) + uiTmp[2];
	if(uiVersionLib < uiVersionMin)
	{
#ifdef _FEDM_WINDOWS
	#ifdef _WIN32_WCE
		sprintf(cTmp, "FEISC: V%s detected, but at least V%s expected", cVersion, FEISC_CE_DLL_VERSION);
	#else
		sprintf(cTmp, "FEISC: V%s detected, but at least V%s expected", cVersion, FEISC_DLL_VERSION);
	#endif
#endif
#ifdef _FEDM_LINUX
		sprintf(cTmp, "FEISC: V%s detected, but at least V%s expected", cVersion, FEISC_TUX_DLL_VERSION);
#endif
		sMessage = cTmp;
		iBack = FEDM_ERROR_VERSION_CONFLICT;
	}
	
#ifdef _FEDM_FU_SUPPORT
	// check FEFU
	memset(uiTmp, 0, 3);
	uiVersionLib = 0;
	uiVersionMin = 0;
	FEFU_GetDLLVersion(cVersion);
	sscanf(cVersion, "%d.%d.%d", &uiTmp[0], &uiTmp[1], &uiTmp[2]);
	uiVersionLib = (uiTmp[0]<<16) + (uiTmp[1]<<8) + uiTmp[2];
	sscanf(FEFU_DLL_VERSION, "%d.%d.%d", &uiTmp[0], &uiTmp[1], &uiTmp[2]);
	uiVersionMin = (uiTmp[0]<<16) + (uiTmp[1]<<8) + uiTmp[2];
	if(uiVersionLib < uiVersionMin)
	{
		sprintf(cTmp, "FEFU: V%s detected, but at least V%s expected", cVersion, FEFU_DLL_VERSION);
		if(sMessage.length() > 0)
			sMessage += "\n";

		sMessage += cTmp;
		iBack = FEDM_ERROR_VERSION_CONFLICT;
	}
#endif
	
#ifdef _FEDM_TAG_HANDLER
  #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
	// check FETCL
	memset(uiTmp, 0, 3);
	uiVersionLib = 0;
	uiVersionMin = 0;
	FETCL_GetDLLVersion(cVersion);
	sscanf(cVersion, "%d.%d.%d", &uiTmp[0], &uiTmp[1], &uiTmp[2]);
	uiVersionLib = (uiTmp[0]<<16) + (uiTmp[1]<<8) + uiTmp[2];
	sscanf(FETCL_DLL_VERSION, "%d.%d.%d", &uiTmp[0], &uiTmp[1], &uiTmp[2]);
	uiVersionMin = (uiTmp[0]<<16) + (uiTmp[1]<<8) + uiTmp[2];
	if(uiVersionLib < uiVersionMin)
	{
		sprintf(cTmp, "FETCL: V%s detected, but at least V%s expected", cVersion, FETCL_DLL_VERSION);
		if(sMessage.length() > 0)
			sMessage += "\n";

		sMessage += cTmp;
		iBack = FEDM_ERROR_VERSION_CONFLICT;
	}
  #endif
#endif
	
#ifdef _FEDM_COM_SUPPORT
	// check FECOM
	memset(uiTmp, 0, 3);
	uiVersionLib = 0;
	uiVersionMin = 0;
#if defined(_FEDM_SUPPORT_SLINK)
	FECOM_GetDLLVersion(cVersion);
#else
	LPFN_FECOM_GET_DLL_VERSION lpfnFecom = (LPFN_FECOM_GET_DLL_VERSION)GetFeComFunction(FECOM_GET_DLL_VERSION);
	if(lpfnFecom == NULL)
	{
#ifdef _FEDM_WINDOWS
		sprintf(cTmp, "FECOM.DLL is missing");
		if(sMessage.length() > 0)
			sMessage += "\n";

		sMessage += cTmp;
#endif
#ifdef _FEDM_LINUX
		sprintf(cTmp, "libfecom.so is missing");
		if(sMessage.length() > 0)
			sMessage += "\n";

		sMessage += cTmp;
#endif
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << cTmp << " " << GetErrorText(FEDM_ERROR_NULL_POINTER));
		FEDM_RETURN(FEDM_ERROR_NULL_POINTER);
	}
	lpfnFecom(cVersion);
#endif
	sscanf(cVersion, "%d.%d.%d", &uiTmp[0], &uiTmp[1], &uiTmp[2]);
	uiVersionLib = (uiTmp[0]<<16) + (uiTmp[1]<<8) + uiTmp[2];
#ifdef _FEDM_WINDOWS
	#ifdef _WIN32_WCE
	sscanf(FECOM_CE_DLL_VERSION, "%d.%d.%d", &uiTmp[0], &uiTmp[1], &uiTmp[2]);
	#else
	sscanf(FECOM_DLL_VERSION, "%d.%d.%d", &uiTmp[0], &uiTmp[1], &uiTmp[2]);
	#endif
#endif
#ifdef _FEDM_LINUX
	sscanf(FECOM_TUX_DLL_VERSION, "%d.%d.%d", &uiTmp[0], &uiTmp[1], &uiTmp[2]);
#endif
	uiVersionMin = (uiTmp[0]<<16) + (uiTmp[1]<<8) + uiTmp[2];
	if(uiVersionLib < uiVersionMin)
	{
#ifdef _FEDM_WINDOWS
	#ifdef _WIN32_WCE
		sprintf(cTmp, "FECOM: V%s detected, but at least V%s expected", cVersion, FECOM_CE_DLL_VERSION);
	#else
		sprintf(cTmp, "FECOM: V%s detected, but at least V%s expected", cVersion, FECOM_DLL_VERSION);
	#endif
#endif
#ifdef _FEDM_LINUX
		sprintf(cTmp, "FECOM: V%s detected, but at least V%s expected", cVersion, FECOM_TUX_DLL_VERSION);
#endif
		if(sMessage.length() > 0)
			sMessage += "\n";

		sMessage += cTmp;
		iBack = FEDM_ERROR_VERSION_CONFLICT;
	}
#endif

#ifdef _FEDM_USB_SUPPORT
	// check FEUSB
	memset(uiTmp, 0, 3);
	uiVersionLib = 0;
	uiVersionMin = 0;
#if defined(_FEDM_SUPPORT_SLINK)
	FEUSB_GetDLLVersion(cVersion);
#else
	LPFN_FEUSB_GET_DLL_VERSION lpfnFeusb = (LPFN_FEUSB_GET_DLL_VERSION)GetFeUsbFunction(FEUSB_GET_DLL_VERSION);
	if(lpfnFeusb == NULL)
	{
#ifdef _FEDM_WINDOWS
		sprintf(cTmp, "FEUSB.DLL is missing");
		if(sMessage.length() > 0)
			sMessage += "\n";

		sMessage += cTmp;
#endif
#ifdef _FEDM_LINUX
		sprintf(cTmp, "libfeusb.so is missing");
		if(sMessage.length() > 0)
			sMessage += "\n";

		sMessage += cTmp;
#endif
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << cTmp << " " << GetErrorText(FEDM_ERROR_NULL_POINTER));
		FEDM_RETURN(FEDM_ERROR_NULL_POINTER);
	}
	lpfnFeusb(cVersion);
#endif
	sscanf(cVersion, "%d.%d.%d", &uiTmp[0], &uiTmp[1], &uiTmp[2]);
	uiVersionLib = (uiTmp[0]<<16) + (uiTmp[1]<<8) + uiTmp[2];
#ifdef _FEDM_WINDOWS
	#ifdef _WIN32_WCE
	sscanf(FEUSB_CE_DLL_VERSION, "%d.%d.%d", &uiTmp[0], &uiTmp[1], &uiTmp[2]);
	#else
	sscanf(FEUSB_DLL_VERSION, "%d.%d.%d", &uiTmp[0], &uiTmp[1], &uiTmp[2]);
	#endif
#endif
#ifdef _FEDM_LINUX
	sscanf(FEUSB_TUX_DLL_VERSION, "%d.%d.%d", &uiTmp[0], &uiTmp[1], &uiTmp[2]);
#endif
	uiVersionMin = (uiTmp[0]<<16) + (uiTmp[1]<<8) + uiTmp[2];
	if(uiVersionLib < uiVersionMin)
	{
#ifdef _FEDM_WINDOWS
	#ifdef _WIN32_WCE
		sprintf(cTmp, "FEUSB: V%s detected, but at least V%s expected", cVersion, FEUSB_CE_DLL_VERSION);
	#else
		sprintf(cTmp, "FEUSB: V%s detected, but at least V%s expected", cVersion, FEUSB_DLL_VERSION);
	#endif
#endif
#ifdef _FEDM_LINUX
		sprintf(cTmp, "FEUSB: V%s detected, but at least V%s expected", cVersion, FEUSB_TUX_DLL_VERSION);
#endif
		if(sMessage.length() > 0)
			sMessage += "\n";

		sMessage += cTmp;
		iBack = FEDM_ERROR_VERSION_CONFLICT;
	}
#endif

#ifdef _FEDM_TCP_SUPPORT
	// check FETCP
	memset(uiTmp, 0, 3);
	uiVersionLib = 0;
	uiVersionMin = 0;
#if defined(_FEDM_SUPPORT_SLINK)
	FETCP_GetDLLVersion(cVersion);
#else
	LPFN_FETCP_GET_DLL_VERSION lpfnFetcp = (LPFN_FETCP_GET_DLL_VERSION)GetFeTcpFunction(FETCP_GET_DLL_VERSION);
	if(lpfnFetcp == NULL)
	{
#ifdef _FEDM_WINDOWS
		sprintf(cTmp, "FETCP.DLL is missing");
		if(sMessage.length() > 0)
			sMessage += "\n";

		sMessage += cTmp;
#endif
#ifdef _FEDM_LINUX
		sprintf(cTmp, "libfetcp.so is missing");
		if(sMessage.length() > 0)
			sMessage += "\n";

		sMessage += cTmp;
#endif
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << cTmp << " " << GetErrorText(FEDM_ERROR_NULL_POINTER));
		FEDM_RETURN(FEDM_ERROR_NULL_POINTER);
	}
	lpfnFetcp(cVersion);
#endif
	sscanf(cVersion, "%d.%d.%d", &uiTmp[0], &uiTmp[1], &uiTmp[2]);
	uiVersionLib = (uiTmp[0]<<16) + (uiTmp[1]<<8) + uiTmp[2];
	sscanf(FETCP_DLL_VERSION, "%d.%d.%d", &uiTmp[0], &uiTmp[1], &uiTmp[2]);
	uiVersionMin = (uiTmp[0]<<16) + (uiTmp[1]<<8) + uiTmp[2];
	if(uiVersionLib < uiVersionMin)
	{
		sprintf(cTmp, "FETCP: V%s detected, but at least V%s expected", cVersion, FETCP_DLL_VERSION);
		if(sMessage.length() > 0)
			sMessage += "\n";

		sMessage += cTmp;
		iBack = FEDM_ERROR_VERSION_CONFLICT;
	}
#endif
#endif // #if !defined(_FEDM_ISC_CORE_DLL) && !defined(_OBIDISC4J_DLL) && !defined(_OBIDISC4NETnativePI_DLL)

	if(iBack < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " Version conflict: " << sMessage.c_str());
	}

	FEDM_RETURN(iBack);
}



/***************************************************************************
  Begin        	:	05.03.2013 / M. Hultsch
  Version       :	04.04.03 / 05.03.2013 / M. Hultsch

  Function		:	return version numbers of dependent libraries
					FECOM, FETCP, FEUSB, FEISC, FEFU, FETCL

  Parameters	:	string& sMessage	- reference to STL-string
					
  Return value	:	FEDM_OK or error code

***************************************************************************/
string FEDM_ISCReader::GetDependentLibVersions()
{
	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL, __FUNCTION__);

#if !defined(_FEDM_ISC_CORE_DLL) && !defined(_OBIDISC4J_DLL) && !defined(_OBIDISC4NETnativePI_DLL)
	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
	return "";
#else
	char cVersion[256];
	char cTmp[256];
	string sMessage;
	
#ifdef _FEDM_COM_SUPPORT
	// FECOM
#if defined(_FEDM_SUPPORT_SLINK)
	FECOM_GetDLLVersion(cVersion);
#else
	LPFN_FECOM_GET_DLL_VERSION lpfnFecom = (LPFN_FECOM_GET_DLL_VERSION)GetFeComFunction(FECOM_GET_DLL_VERSION);
	if(lpfnFecom == NULL)
	{
#ifdef _FEDM_WINDOWS
		sprintf(cTmp, "FECOM.DLL is missing\n");
		sMessage += cTmp;
#endif
#ifdef _FEDM_LINUX
		sprintf(cTmp, "libfecom.so is missing\n");
		sMessage += cTmp;
#endif
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << cTmp << " " << GetErrorText(FEDM_ERROR_NULL_POINTER));
	}
	else
	{
		lpfnFecom(cVersion);
#endif
#ifdef _FEDM_WINDOWS
	#ifdef _WIN32_WCE
		sprintf(cTmp, "FECOM: V%s detected, (at least V%s expected)\n", cVersion, FECOM_CE_DLL_VERSION);
	#else
		sprintf(cTmp, "FECOM: V%s detected, (at least V%s expected)\n", cVersion, FECOM_DLL_VERSION);
	#endif
#endif
#ifdef _FEDM_LINUX
		sprintf(cTmp, "FECOM: V%s detected, (at least V%s expected)\n", cVersion, FECOM_TUX_DLL_VERSION);
#endif
		sMessage += cTmp;
	}
#endif // _FEDM_COM_SUPPORT

#ifdef _FEDM_USB_SUPPORT
	// FEUSB
#if defined(_FEDM_SUPPORT_SLINK)
	FEUSB_GetDLLVersion(cVersion);
#else
	LPFN_FEUSB_GET_DLL_VERSION lpfnFeusb = (LPFN_FEUSB_GET_DLL_VERSION)GetFeUsbFunction(FEUSB_GET_DLL_VERSION);
	if(lpfnFeusb == NULL)
	{
#ifdef _FEDM_WINDOWS
		sprintf(cTmp, "FEUSB.DLL is missing\n");
		sMessage += cTmp;
#endif
#ifdef _FEDM_LINUX
		sprintf(cTmp, "libfeusb.so is missing\n");
		sMessage += cTmp;
#endif
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << cTmp << " " << GetErrorText(FEDM_ERROR_NULL_POINTER));
	}
	else
	{
		lpfnFeusb(cVersion);
#endif
#ifdef _FEDM_WINDOWS
	#ifdef _WIN32_WCE
		sprintf(cTmp, "FEUSB: V%s detected, (at least V%s expected)\n", cVersion, FEUSB_CE_DLL_VERSION);
	#else
		sprintf(cTmp, "FEUSB: V%s detected, (at least V%s expected)\n", cVersion, FEUSB_DLL_VERSION);
	#endif
#endif
#ifdef _FEDM_LINUX
		sprintf(cTmp, "FEUSB: V%s detected, (at least V%s expected)\n", cVersion, FEUSB_TUX_DLL_VERSION);
#endif
		sMessage += cTmp;
	}
#endif

#ifdef _FEDM_TCP_SUPPORT
	// FETCP
#if defined(_FEDM_SUPPORT_SLINK)
	FETCP_GetDLLVersion(cVersion);
#else
	LPFN_FETCP_GET_DLL_VERSION lpfnFetcp = (LPFN_FETCP_GET_DLL_VERSION)GetFeTcpFunction(FETCP_GET_DLL_VERSION);
	if(lpfnFetcp == NULL)
	{
#ifdef _FEDM_WINDOWS
		sprintf(cTmp, "FETCP.DLL is missing\n");
		sMessage += cTmp;
#endif
#ifdef _FEDM_LINUX
		sprintf(cTmp, "libfetcp.so is missing\n");
		sMessage += cTmp;
#endif
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << cTmp << " " << GetErrorText(FEDM_ERROR_NULL_POINTER));
	}
	else
	{
		lpfnFetcp(cVersion);
#endif
		sprintf(cTmp, "FETCP: V%s detected, (at least V%s expected)\n", cVersion, FETCP_DLL_VERSION);
		sMessage += cTmp;
	}
#endif
	
	// FEISC
	FEISC_GetDLLVersion(cVersion);
	sprintf(cTmp, "FEISC: V%s detected (at least V%s expected)\n", cVersion, FEISC_TUX_DLL_VERSION);
	sMessage += cTmp;
	
#ifdef _FEDM_FU_SUPPORT
	// FEFU
	FEFU_GetDLLVersion(cVersion);
	sprintf(cTmp, "FEFU:  V%s detected, (at least V%s expected)\n", cVersion, FEFU_DLL_VERSION);
	sMessage += cTmp;
#endif
	
#ifdef _FEDM_TAG_HANDLER
  #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
	// FETCL
	FETCL_GetDLLVersion(cVersion);
	sprintf(cTmp, "FETCL: V%s detected, (at least V%s expected\n", cVersion, FETCL_DLL_VERSION);
	sMessage += cTmp;
  #endif
#endif

	return sMessage;

#endif // #if !defined(_FEDM_ISC_CORE_DLL) && !defined(_OBIDISC4J_DLL) && !defined(_OBIDISC4NETnativePI_DLL)
}

//////////////////////////////////////////////////////////////////////
// Settings
//////////////////////////////////////////////////////////////////////

int FEDM_ISCReader::SetPortHnd(int iPortHnd)
{
	// no reader handle -> no communication
	if( m_iReaderHnd == 0 )
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_NO_READER_HANDLE));
		FEDM_RETURN(FEDM_ERROR_NO_READER_HANDLE);
	}

	int iErr;
	char cPortHnd[11];
	memset(cPortHnd, 0, 11);
	sprintf(cPortHnd, "%d", iPortHnd);

	// Set port handle into DLL
	FEDM_CHK2(iErr, FEISC_SetReaderPara(m_iReaderHnd, "PortHnd", cPortHnd));

	return FEDM_OK;
}


int FEDM_ISCReader::SetProtocolFrameSupport(unsigned int uiType)
{
	switch(uiType)
	{
	case FEDM_PRT_FRAME_STANDARD:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_INFO, __FUNCTION__ << "(STANDARD)");
		return FEISC_SetReaderPara(m_iReaderHnd, "FRAMESUPPORT", "STANDARD");

	case FEDM_PRT_FRAME_ADVANCED:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_INFO, __FUNCTION__ << "(ADVANCED)");
		return FEISC_SetReaderPara(m_iReaderHnd, "FRAMESUPPORT", "ADVANCED");
	}

	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_PARAMETER));
	FEDM_RETURN(FEDM_ERROR_PARAMETER);
}

#ifdef _FEDM_TAG_HANDLER
void FEDM_ISCReader::SetTagHandlerOption(string sParameter, string sValue)
{
	FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_ADMIN_SET_GET, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "sParameter=" << sParameter << ", sValue=" << sValue);

	if(sParameter == "CreateCondition_Smart")
	{
		if(sValue == "On" || sValue == "1")
		{
			m_bTagHandlerOption_CreateCondition_Smart = true;
		}
		else
		{
			m_bTagHandlerOption_CreateCondition_Smart = false;
		}
	}
}
#endif

int FEDM_ISCReader::SetTableSize(unsigned int uiTableID, int iSize)
{
	unsigned int i;

	if(iSize <= 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_PARAMETER));
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
		FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_ADMIN_SET_GET, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "BRMTable, Size=" << iSize);
		// delete old table entries
		for( i=0; i<m_BRMTable.size(); i++ )
			delete m_BRMTable[i];

		m_BRMTable.clear();
		m_BRMTable.resize(iSize);

		// allocate memory for table entries
		for( i=0; i<(unsigned int)iSize; i++ )
		{
			m_BRMTable[i] = (FEDM_BRMTabItem*) new FEDM_BRMTabItem(i);
			if(m_BRMTable[i] == NULL)
				break;
			m_BRMTable[i]->Init();
		}
		break;

	case FEDM_ISC_ISO_TABLE:
		FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_ADMIN_SET_GET, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "ISOTable, Size=" << iSize);
		// delete old table entries
		for( i=0; i<m_ISOTable.size(); i++ )
			delete m_ISOTable[i];

		m_ISOTable.clear();
		m_ISOTable.resize(iSize);

		// allocate memory for table entries
		for( i=0; i<(unsigned int)iSize; i++ )
		{
			m_ISOTable[i] = (FEDM_ISOTabItem*) new FEDM_ISOTabItem(i, this);
			if(m_ISOTable[i] == NULL)
				break;
			m_ISOTable[i]->Init();
		}
		break;

	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	return FEDM_OK;
}


int FEDM_ISCReader::SetTableSize(	unsigned int uiTableID, 
									int iSize, 
									int iRxDB_BlockCount, 
									int iRxDB_BlockSize, 
									int iTxDB_BlockCount,
									int iTxDB_BlockSize )
{
	unsigned int i;

	if(iSize <= 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_PARAMETER));
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
		FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_ADMIN_SET_GET, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "BRMTable, Size=" << iSize << ", RxDB_BlockCount=" << iRxDB_BlockCount << ", RxDB_BlockSize=" << iRxDB_BlockSize << ", TxDB_BlockCount=" << iTxDB_BlockCount << ", TxDB_BlockSize=" << iTxDB_BlockSize);
		// delete old table entries
		for( i=0; i<m_BRMTable.size(); i++ )
			delete m_BRMTable[i];

		m_BRMTable.clear();
		m_BRMTable.resize(iSize);

		// allocate memory for table entries
		for( i=0; i<(unsigned int)iSize; i++ )
		{
			m_BRMTable[i] = (FEDM_BRMTabItem*) new FEDM_BRMTabItem(i);
			if(m_BRMTable[i] == NULL)
				break;
			m_BRMTable[i]->Init(iRxDB_BlockCount, 
								iRxDB_BlockSize);
		}
		break;

	case FEDM_ISC_ISO_TABLE:
		FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_ADMIN_SET_GET, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "ISOTable, Size=" << iSize << ", RxDB_BlockCount=" << iRxDB_BlockCount << ", RxDB_BlockSize=" << iRxDB_BlockSize << ", TxDB_BlockCount=" << iTxDB_BlockCount << ", TxDB_BlockSize=" << iTxDB_BlockSize);
		// delete old table entries
		for( i=0; i<m_ISOTable.size(); i++ )
			delete m_ISOTable[i];

		m_ISOTable.clear();
		m_ISOTable.resize(iSize);

		// allocate memory for table entries
		for( i=0; i<(unsigned int)iSize; i++ )
		{
			m_ISOTable[i] = (FEDM_ISOTabItem*) new FEDM_ISOTabItem(i, this);
			if(m_ISOTable[i] == NULL)
				break;
			m_ISOTable[i]->Init(iRxDB_BlockCount, 
								iRxDB_BlockSize, 
								iTxDB_BlockCount,
								iTxDB_BlockSize);
		}
		break;

	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	return FEDM_OK;
}



/***************************************************************************
  Begin        	:	10.05.2001 / M. Hultsch
  Version       :	03.00.06 / 31.03.2006 / M. Hultsch
					- remove of all tag handler in every tag item

					02.03.00 / 28.07.2005 / M. Hultsch

  Function		:	delete all data entries in table (list)

  Parameters	:	unsigned int uiTableID		- identifies the table
					unsigned int uiDataFlags	- additional reset conditions
										  0 or FEDM_ISC_DATA_ALL

  Return value	:	FEDM_OK or error code
***************************************************************************/
int FEDM_ISCReader::ResetTable(unsigned int uiTableID, unsigned int uiDataFlags)
{
	unsigned int iCnt, iIdx, iCnt1;

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
		FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_ADMIN, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "BRMTable, DataFlags=0x" << std::hex << std::uppercase << uiDataFlags);
		if(m_BRMTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(BRMTable, ..) " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		m_uiBRMTableLength = 0;
		break;

	case FEDM_ISC_ISO_TABLE:
		FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_ADMIN, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "ISOTable, DataFlags=0x" << std::hex << std::uppercase << uiDataFlags);
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(ISOTable, ..) " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if(m_uiISOTableLength == 0)
			return FEDM_OK;


		// delete all flags in the table
		for(iIdx=0; iIdx<m_uiISOTableLength; iIdx++)
			m_ISOTable[iIdx]->m_bIsSelected = false;

		for(iIdx=0; iIdx<m_uiISOTableLength; iIdx++)
			m_ISOTable[iIdx]->m_bIsBlockSizeSet = false;

		for(iIdx=0; iIdx<m_uiISOTableLength; iIdx++)
			m_ISOTable[iIdx]->m_bIsSnr = false;

		for(iIdx=0; iIdx<m_uiISOTableLength; iIdx++)
			m_ISOTable[iIdx]->m_bIsEpc = false;

		for(iIdx=0; iIdx<m_uiISOTableLength; iIdx++)
			m_ISOTable[iIdx]->m_bIsAFI = false;

		for(iIdx=0; iIdx<m_uiISOTableLength; iIdx++)
			m_ISOTable[iIdx]->m_bIsSnr = false;

		for(iIdx=0; iIdx<m_uiISOTableLength; iIdx++)
			m_ISOTable[iIdx]->m_bIsSnr = false;

		for(iIdx=0; iIdx<m_uiISOTableLength; iIdx++)
			m_ISOTable[iIdx]->m_bIsSysInfo = false;

		for(iIdx=0; iIdx<m_uiISOTableLength; iIdx++)
			m_ISOTable[iIdx]->m_bIsISO14443_4Info = false;

		if(uiDataFlags != FEDM_ISC_DATA_ALL)
		{
			m_uiISOTableLength = 0;	// set length to zero
			return FEDM_OK;
		}

		// overwrite complete table with zero
		for(iIdx=0; iIdx<m_uiISOTableLength; iIdx++)
			for(iCnt=0; iCnt<FEDM_ISC_MAX_UID_LENGTH; iCnt++)
				m_ISOTable[iIdx]->m_ucSnr[iCnt] = 0x00;

		for(iIdx=0; iIdx<m_uiISOTableLength; iIdx++)
			m_ISOTable[iIdx]->m_ucTrType = 0x00;

		for(iIdx=0; iIdx<m_uiISOTableLength; iIdx++)
			m_ISOTable[iIdx]->m_ucEpcType = 0x00;

		for(iIdx=0; iIdx<m_uiISOTableLength; iIdx++)
			m_ISOTable[iIdx]->m_ucAFI = 0x00;

		for(iIdx=0; iIdx<m_uiISOTableLength; iIdx++)
			m_ISOTable[iIdx]->m_ucDsfID = 0x00;

		for(iIdx=0; iIdx<m_uiISOTableLength; iIdx++)
			for(iCnt=0; iCnt<2; iCnt++)
				m_ISOTable[iIdx]->m_ucMemSize[iCnt] = 0x00;

		for(iIdx=0; iIdx<m_uiISOTableLength; iIdx++)
			m_ISOTable[iIdx]->m_ucICRef = 0x00;

		for(iIdx=0; iIdx<m_uiISOTableLength; iIdx++)
			m_ISOTable[iIdx]->m_ucBlockSize = 0x00;

		for(iIdx=0; iIdx<m_uiISOTableLength; iIdx++)
			for(iCnt=0; iCnt<m_ISOTable[iIdx]->m_ucSecStatus.size(); iCnt++)
				m_ISOTable[iIdx]->m_ucSecStatus[iCnt] = 0x00;

		for(iIdx=0; iIdx<m_uiISOTableLength; iIdx++)
		{
			for(iCnt=0; iCnt<m_ISOTable[iIdx]->m_ucTxDB.size(); iCnt++)
				m_ISOTable[iIdx]->m_ucTxDB[iCnt] = 0x00;

			for(iCnt=0; iCnt<m_ISOTable[iIdx]->m_ucTxDB_EpcBank.size(); iCnt++)
				m_ISOTable[iIdx]->m_ucTxDB_EpcBank[iCnt] = 0x00;

			for(iCnt=0; iCnt<m_ISOTable[iIdx]->m_ucTxDB_TidBank.size(); iCnt++)
				m_ISOTable[iIdx]->m_ucTxDB_TidBank[iCnt] = 0x00;

			for(iCnt=0; iCnt<m_ISOTable[iIdx]->m_ucTxDB_ResBank.size(); iCnt++)
				m_ISOTable[iIdx]->m_ucTxDB_ResBank[iCnt] = 0x00;
		}

		for(iIdx=0; iIdx<m_uiISOTableLength; iIdx++)
		{
			for(iCnt=0; iCnt<m_ISOTable[iIdx]->m_ucRxDB.size(); iCnt++)
				m_ISOTable[iIdx]->m_ucRxDB[iCnt] = 0x00;

			for(iCnt=0; iCnt<m_ISOTable[iIdx]->m_ucRxDB_EpcBank.size(); iCnt++)
				m_ISOTable[iIdx]->m_ucRxDB_EpcBank[iCnt] = 0x00;

			for(iCnt=0; iCnt<m_ISOTable[iIdx]->m_ucRxDB_TidBank.size(); iCnt++)
				m_ISOTable[iIdx]->m_ucRxDB_TidBank[iCnt] = 0x00;

			for(iCnt=0; iCnt<m_ISOTable[iIdx]->m_ucRxDB_ResBank.size(); iCnt++)
				m_ISOTable[iIdx]->m_ucRxDB_ResBank[iCnt] = 0x00;
		}

		for(iIdx=0; iIdx<m_uiISOTableLength; iIdx++)
			for(iCnt=0; iCnt<4; iCnt++)
				for(iCnt1=0; iCnt1<4; iCnt1++)
					m_ISOTable[iIdx]->m_ucTxCB[iCnt][iCnt1] = 0x00;

		for(iIdx=0; iIdx<m_uiISOTableLength; iIdx++)
			for(iCnt=0; iCnt<4; iCnt++)
				for(iCnt1=0; iCnt1<4; iCnt1++)
					m_ISOTable[iIdx]->m_ucRxCB[iCnt][iCnt1] = 0x00;


		m_uiISOTableLength = 0;	// set length to zero

		break;
	
#if !defined(__BORLANDC__)
	case FEDM_ISC_CAD_TABLE:
		m_CADTable.Init();
		break;
#endif

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	return FEDM_OK;
}


//////////////////////////////////////////////////////////////////////
// Queries
//////////////////////////////////////////////////////////////////////

int  FEDM_ISCReader::GetPortHnd()
{
	// no reader handle -> no communication
	if( m_iReaderHnd == 0 )
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_NO_READER_HANDLE));
		FEDM_RETURN(FEDM_ERROR_NO_READER_HANDLE);
	}

	int iErr;
	char cPortHnd[13];

	// Get port handle from DLL
	FEDM_CHK2(iErr, FEISC_GetReaderPara(m_iReaderHnd, "PortHnd", cPortHnd));

	return atoi(cPortHnd);
}


int FEDM_ISCReader::GetProtocolFrameSupport()
{
	int iErr;
	char cValue[24];

	FEDM_CHK2(iErr, FEISC_GetReaderPara(m_iReaderHnd, "FRAMESUPPORT", cValue));

	if(strcmp(cValue, "STANDARD") == 0)
	{
		SetLastError(FEDM_OK);
		return FEDM_PRT_FRAME_STANDARD;
	}
	else if(strcmp(cValue, "ADVANCED") == 0)
	{
		SetLastError(FEDM_OK);
		return FEDM_PRT_FRAME_ADVANCED;
	}

	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_PARAMETER));
	FEDM_RETURN(FEDM_ERROR_PARAMETER);
}


int FEDM_ISCReader::GetTableSize(unsigned int uiTableID)
{
	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
		return (int)m_BRMTable.size();

	case FEDM_ISC_ISO_TABLE:
		return (int)m_ISOTable.size();

#if !defined(__BORLANDC__)
	case FEDM_ISC_CAD_TABLE:
		if(m_CADTable.m_ucMode == 0x04)			// CH_HISTORY
		{
			return (int)m_CADTable.m_History.size();
		}
		else if(m_CADTable.m_ucMode == 0x08)	// CH_POWER
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
		}
		else
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
		}
		break;
#endif

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	return FEDM_OK;
}

int FEDM_ISCReader::GetTableLength(unsigned int uiTableID)
{
	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
		return (int)m_uiBRMTableLength;

	case FEDM_ISC_ISO_TABLE:
		return (int)m_uiISOTableLength;
	
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	return FEDM_OK;
}

int FEDM_ISCReader::SetTableLength(unsigned int uiTableID, unsigned int uiLength)
{
	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(BRMTable, Length=" << uiLength << ")" << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		return FEDM_ERROR_UNSUPPORTED;

	case FEDM_ISC_ISO_TABLE:
		if(uiLength == 0 && uiLength > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(ISOTable, Length=" << uiLength << ") " << GetErrorText(FEDM_ERROR_PARAMETER));
			return FEDM_ERROR_PARAMETER;
		}

		m_uiISOTableLength = uiLength;
		return FEDM_OK;
	
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	return FEDM_OK;
}

FEDM_ISOTabItem* FEDM_ISCReader::GetISOTableItem(unsigned int uiIdx) 
{
	if(uiIdx > m_ISOTable.size())
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " Index > table size");
		return NULL;
	}

#ifdef _FEDM_WINDOWS
  #if _MSC_VER >= 1400
	vector<FEDM_ISOTabItem*>::pointer ptr = &m_ISOTable[uiIdx];
	return *ptr;
  #else
	vector<FEDM_ISOTabItem*>::iterator itor;
	for(itor  = m_ISOTable.begin();
		itor != m_ISOTable.end();
		itor++)
	{
		if( (*itor)->m_uiTabIndex == uiIdx )
			return *itor;
	}
  #endif
#else
	vector<FEDM_ISOTabItem*>::pointer ptr = &m_ISOTable[uiIdx];
	return *ptr;
#endif

	return NULL;
}

FEDM_BRMTabItem* FEDM_ISCReader::GetBRMTableItem(unsigned int uiIdx) 
{
	if(uiIdx > m_BRMTable.size())
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " Index > table size");
		return NULL;
	}

#ifdef _FEDM_WINDOWS
  #if _MSC_VER >= 1400
	vector<FEDM_BRMTabItem*>::pointer ptr = &m_BRMTable[uiIdx];
	return *ptr;
  #else
	vector<FEDM_BRMTabItem*>::iterator itor;
	for(itor  = m_BRMTable.begin();
		itor != m_BRMTable.end();
		itor++)
	{
		if( (*itor)->m_uiTabIndex == uiIdx )
			return *itor;
	}
  #endif
#else
	vector<FEDM_BRMTabItem*>::pointer ptr = &m_BRMTable[uiIdx];
	return *ptr;
#endif

	return NULL;
}



//////////////////////////////////////////////////////////////////////
// Data I/O
//////////////////////////////////////////////////////////////////////


/***************************************************************************
  Begin        		:	14.08.2003 / M. Hultsch
  Version       	:	02.04.04 / 22.08.2006 / M. Hultsch

  Function			:	main-function for XML file I/O

  Parameters		:	bool bRead		-	if true : serialize from XML-File
											if false: serialize into XML-File
						char* sFileName	-	pointer to file name with directory

  Return value		:	OK or error code

***************************************************************************/
int FEDM_ISCReader::Serialize(bool bRead, char* sFileName)
{
#ifdef _FEDM_XML_SUPPORT
	FEDM_XMLReaderCfgDataModul xml;

	int iBack = FEDM_DataBase::Serialize(&xml, bRead, sFileName);

	FEDM_RETURN(iBack);
#else
	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
	FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
#endif
}

/***************************************************************************
  Begin        		:	23.04.2003 / M. Hultsch
  Version       	:	03.00.00 / 20.10.2006 / M. Hultsch

  Function			:	sub-function for XML file input

  Parameters		:	FEDM_XMLBase xml		- pointer to the xml handler
						FEDM_XML_TREEITEM* root	- pointer to the root item

  Return value		:	OK or error code

***************************************************************************/
int FEDM_ISCReader::SerializeIn(FEDM_XMLBase* xml, FEDM_XML_TREEITEM* root)
{
	int iBack = 0;
#ifdef _FEDM_XML_SUPPORT
#ifndef FEDM_NEW_LRU3000_ACC_CFG
	switch(m_uiReaderType)
	{
	case FEDM_ISC_TYPE_ISCLRU3000:

	//case FEDM_ISC_TYPE_ISCLR2500_A:
		iBack = FEDM_DataBase::SerializeIn(FEDM_RFC_EEDATA_MEM, xml, root);
		if(iBack)
			return iBack;

		iBack = FEDM_DataBase::SerializeIn(FEDM_RFC_RAMDATA_MEM, xml, root);
		if(iBack)
			return iBack;

		iBack = FEDM_DataBase::SerializeIn(FEDM_ACC_EEDATA_MEM, xml, root);
		if(iBack)
			return iBack;

		iBack = FEDM_DataBase::SerializeIn(FEDM_ACC_RAMDATA_MEM, xml, root);
		if(iBack)
			return iBack;

		break;
	default:
#endif
		iBack = FEDM_DataBase::SerializeIn(FEDM_RFC_EEDATA_MEM, xml, root);
		if(iBack)
			return iBack;

		iBack = FEDM_DataBase::SerializeIn(FEDM_RFC_RAMDATA_MEM, xml, root);
		if(iBack)
			return iBack;

#ifndef FEDM_NEW_LRU3000_ACC_CFG
		break;
	}
#endif

	return FEDM_OK;
#else
	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
	return FEDM_ERROR_UNSUPPORTED;
#endif
}

/***************************************************************************
  Begin        		:	23.04.2003 / M. Hultsch
  Version       	:	03.00.00 / 20.10.2006 / M. Hultsch

  Function			:	sub-function for XML file output

  Parameters		:	FEDM_XMLBase xml		- pointer to the xml handler
						FEDM_XML_TREEITEM* root	- pointer to the root item

  Return value		:	OK or error code

***************************************************************************/
int FEDM_ISCReader::SerializeOut(FEDM_XMLBase* xml, FEDM_XML_TREEITEM* root)
{
	int iBack = 0;
#ifdef _FEDM_XML_SUPPORT
#ifndef FEDM_NEW_LRU3000_ACC_CFG
	switch(m_uiReaderType)
	{
	case FEDM_ISC_TYPE_ISCLRU3000:
	//case FEDM_ISC_TYPE_ISCLR2500_A:
		iBack = FEDM_DataBase::SerializeOut(FEDM_RFC_EEDATA_MEM, xml, root);
		if(iBack)
			return iBack;

		iBack = FEDM_DataBase::SerializeOut(FEDM_RFC_RAMDATA_MEM, xml, root);
		if(iBack)
			return iBack;

		iBack = FEDM_DataBase::SerializeOut(FEDM_ACC_EEDATA_MEM, xml, root);
		if(iBack)
			return iBack;

		iBack = FEDM_DataBase::SerializeOut(FEDM_ACC_RAMDATA_MEM, xml, root);
		if(iBack)
			return iBack;

		break;
	default:
#endif
		iBack = FEDM_DataBase::SerializeOut(FEDM_RFC_EEDATA_MEM, xml, root);
		if(iBack)
			return iBack;

		iBack = FEDM_DataBase::SerializeOut(FEDM_RFC_RAMDATA_MEM, xml, root);
		if(iBack)
			return iBack;

#ifndef FEDM_NEW_LRU3000_ACC_CFG
		break;
	}
#endif

	return FEDM_OK;
#else
	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
	return FEDM_ERROR_UNSUPPORTED;
#endif
}

/***************************************************************************
  Begin        		:	07.06.2000 / M. Hultsch
  Version       	:	01.00.00 / 19.11.2000 / M. Hultsch

  Function			:	function for proprietary file-I/O
						for every iMemType call Serialize

  Parameters		:	CArchive ar		-	MFC archive class
						int iMemType	-	mem type identifier

  Return value		:	OK or error code

***************************************************************************/

#ifdef _FEDM_MFC_SUPPORT
int FEDM_ISCReader::Serialize(CArchive& ar, int iMemType)
{
/*	FEDM_CHK3(&ar);

	int i;

	if( ar.IsStoring() )	// write data to file
	{
		switch( iMemType )
		{
		case FEDM_RFC_EEDATA_MEM:		
			for( i=0; i<FEDM_ISC_MAX_RFC_EEDATA_MEM; i++ )
				ar << m_RFC_EEData[i];
			break;
		
		case FEDM_RFC_RAMDATA_MEM:
			for( i=0; i<FEDM_ISC_MAX_RFC_RAMDATA_MEM; i++ )
				ar << m_RFC_RAMData[i];
			break;

		case FEDM_TMPDATA_MEM:
			for( i=0; i<FEDM_ISC_MAX_TMPDATA_MEM; i++ )
				ar << m_TmpData[i];
			break;

		default:
			return FEDM_ERROR_UNKNOWN_MEM_ID;
		}
	}
	else if( ar.IsLoading() )	// read data from file
	{
		switch( iMemType )
		{
		case FEDM_RFC_EEDATA_MEM:		
			for( i=0; i<FEDM_ISC_MAX_RFC_EEDATA_MEM; i++ )
				ar >> m_RFC_EEData[i];
			break;

		case FEDM_RFC_RAMDATA_MEM:
			for( i=0; i<FEDM_ISC_MAX_RFC_RAMDATA_MEM; i++ )
				ar >> m_RFC_RAMData[i];
			break;

		case FEDM_TMPDATA_MEM:
			for( i=0; i<FEDM_ISC_MAX_TMPDATA_MEM; i++ )
				ar >> m_TmpData[i];
			break;

		default:
			return FEDM_ERROR_UNKNOWN_MEM_ID;
		}
	}
*/
	return FEDM_ERROR_UNSUPPORTED;
}
#endif



//////////////////////////////////////////////////////////////////////
// Communication
//////////////////////////////////////////////////////////////////////

/***************************************************************************
  Begin        		:	07.06.2000 / M. Hultsch
  
  Version       	:	04.06.04 / 20.08.2014 / M. Hultsch
						- [0xB0] with status 0x02 (data false) returns now at once for all sub-commands, except [0x01] Inventory

						04.04.02 / 22.01.2013 / M. Hultsch
						- [0xB0] with status 0x84 (RF-Warning) takes returned records

						03.00.14 / 29.10.2009 / M. Hultsch

  Function			:	main function for communication with i-scan reader and transponder

  Parameters		:	unsigned char ucCmdByte		-	command byte

  Return value		:	status byte (>=0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::SendProtocol(unsigned char ucCmdByte)
{
	bool	bExtAdr = false;
	int		iErr = 0;
	int     iCnt = 0;
	int     iByteCnt = 0;
	int     iBlockCnt = 0;
	int		iBack = 0;
	int		iLen = 0;
	int		iIdx = 0;
	int		iPortHnd = 0;
	int		iTmpMemAdr = 0;
	unsigned int uiAdr = 0;
	char	cPortHnd[11];
	unsigned char	ucIsoCmd = 0;
	unsigned char	ucEpcCmd = 0;
	unsigned char ucDesFireCmd = 0;
	unsigned char ucMifarePlusCmd = 0;
	unsigned char	ucIsoMode = 0;
	unsigned char	ucHeader = 0;
	unsigned char	ucValue[40];	memset( ucValue, '\0', 40*sizeof(unsigned char) );
	unsigned char	ucPW[4];		memset( ucPW, '\0', 4);
	unsigned int	uiValue[5];		memset( uiValue, 0, 5*sizeof(unsigned int) );
	int		iValue[5];		memset( iValue, 0, 5*sizeof(int) );

	FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_UNI, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte);

	SetLastError(FEDM_OK);

	// no reader handle -> no communication
	if( m_iReaderHnd == 0 )
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_NO_READER_HANDLE) );
		FEDM_RETURN(FEDM_ERROR_NO_READER_HANDLE);
	}

	// get port handle from DLL
	FEDM_CHK2(iErr, FEISC_GetReaderPara(m_iReaderHnd, "PortHnd", cPortHnd));
	
	iPortHnd = atoi(cPortHnd);
	if( iPortHnd == 0 )
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_NO_PORT_HANDLE) );
		FEDM_RETURN(FEDM_ERROR_NO_PORT_HANDLE);
	}

	// reset last error
	FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_LAST_ERROR, (unsigned int)0));


	switch(ucCmdByte)
	{
	case 0x18:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_EPC_DESTROY_MODE, &ucValue[0]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_DESTROY_EPC, &ucValue[1], 16));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_EPC_DESTROY_PASSWORD, &ucPW[0], 3));

		switch(ucValue[0])	// mode byte
		{
		case 0x00: // EPC (HF and UHF)
			ucHeader = (ucValue[1] & 0xC0) >> 6;
			switch(ucHeader) // mask first two bits
			{
			case FEDM_ISC_EPC_TYPE_1: // 96 bit EPC
				iLen = 12;
				break;
			case FEDM_ISC_EPC_TYPE_2: // 64 bit Type 1 EPC
			case FEDM_ISC_EPC_TYPE_3: // 64 bit Type 2 EPC
			case FEDM_ISC_EPC_TYPE_4: // 64 bit Type 3 EPC
				iLen = 8;
				break;
			default:
				FEDM_RETURN(FEDM_ERROR_UNKNOWN_EPC_TYPE);
			}
			
			break;

		case 0x01: // I-Code UID
			iLen = 19;
			break;

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x18) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}

		if(iLen <= 32)
		{
			FEDM_CHK2(iErr, GetData(FEDM_GetAdrOfID(FEDM_ISC_TMP_DESTROY_EPC, 32), &ucValue[1], iLen, FEDM_TMPDATA_MEM));
		}
		else
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x18) - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
			FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
		}

		iBack = FEISC_0x18_Destroy( m_iReaderHnd, 
								    m_ucBusAdr,
									ucValue[0],
									&ucValue[1],
									&ucPW[0]);
		break;

	case 0x1A:
		iBack = FEISC_0x1A_Halt( m_iReaderHnd, m_ucBusAdr );
		break;

	case 0x1B:
		iBack = FEISC_0x1B_ResetQuietBit( m_iReaderHnd, m_ucBusAdr );
		break;

	case 0x1C:
		iBack = FEISC_0x1C_EASRequest( m_iReaderHnd, m_ucBusAdr );
		break;

	case 0x21:
		FEDM_CHK2(iErr, GetData(FEDM_ISCLR_TMP_BRM_SETS, &ucValue[0]));
		
		iBack = FEISC_0x21_ReadBuffer( m_iReaderHnd, 
									   m_ucBusAdr, 
									   ucValue[0], 
									   &ucValue[1], 
									   &ucValue[2], 
									   &m_ucData[0], 
									   0 );

		if( iBack==0x00 || 
			iBack==0x83 || 
			iBack==0x84 || 
			iBack==0x85 || 
			iBack==0x90 || 
			iBack==0x93 || 
			iBack==0x94 )
		{	
			FEDM_CHK2(iErr, SetData(FEDM_ISCLR_TMP_BRM_TRDATA,	ucValue[1]));
			FEDM_CHK2(iErr, SetData(FEDM_ISCLR_TMP_BRM_RECSETS, ucValue[2]));

			if(ucValue[2] == 0)	// no more data
			{
				// reset BRM-table
				m_uiBRMTableLength = 0;

				FEDM_RETURN(iBack);
			}

			// save new buffered read mode data in iso-table
			FEDM_CHK2(iErr, SetBrmTableData(0x21, &m_ucData[0]));
		}
		else
		{
			// reset BRM-table
			m_uiBRMTableLength = 0;
		}
		break;
	
	case 0x22:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_ADV_BRM_SETS, (unsigned int*)&iValue[0]));
		
		iBack = FEISC_0x22_ReadBuffer( m_iReaderHnd, 
									   m_ucBusAdr, 
									   iValue[0], 
									   &ucValue[0], 
									   &iValue[1], 
									   &m_ucData[0], 
									   0 );

		if( iBack==0x00 || 
			iBack==0x83 || // RF Communication Error
			iBack==0x84 || // RF Warning
			iBack==0x85 || 
			iBack==0x90 || 
			iBack==0x93 || 
			iBack==0x94 )
		{	
			FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_ADV_BRM_TRDATA1,  ucValue[0]));
			if(ucValue[0] & 0x80)	// extension flag?
			{
				FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_ADV_BRM_TRDATA2,  ucValue[1]));
			}
			else
			{
				FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_ADV_BRM_TRDATA2,  (unsigned char)0));
			}

			FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_ADV_BRM_RECSETS, (unsigned int)iValue[1]));

			if(iValue[1] == 0)	// no more data
			{
				// reset BRM-table
				m_uiBRMTableLength = 0;

				FEDM_RETURN(iBack);
			}

			// save new buffered read mode data in iso-table
			FEDM_CHK2(iErr, SetBrmTableData(0x22, &m_ucData[0]));
		}
		else
		{
			// reset BRM-table
			m_uiBRMTableLength = 0;
		}
		break;
	
	case 0x31:
		iBack = FEISC_0x31_ReadDataBufferInfo( m_iReaderHnd, 
											   m_ucBusAdr, 
											   &ucValue[0], 
											   &ucValue[2], 
											   &ucValue[4], 
											   0 );

		if( iBack==0x00 || iBack==0x84 || iBack==0x85 || iBack==0x90 || iBack==0x93 )
		{
			FEDM_CHK2(iErr, SetData(FEDM_ISCLR_TMP_TAB_SIZE,	&ucValue[0], 2));
			FEDM_CHK2(iErr, SetData(FEDM_ISCLR_TMP_TAB_START,	&ucValue[2], 2));
			FEDM_CHK2(iErr, SetData(FEDM_ISCLR_TMP_TAB_LEN,		&ucValue[4], 2));
		}
		else
		{
			if(iBack) 
				FEDM_RETURN(iBack);
		}
		break;
	
	case 0x32:
		iBack = FEISC_0x32_ClearDataBuffer( m_iReaderHnd, m_ucBusAdr );
		break;
	
	case 0x33:
		iBack = FEISC_0x33_InitBuffer( m_iReaderHnd, m_ucBusAdr );
		break;
	
	case 0x34:
		iBack = FEISC_0x34_ForceNotifyTrigger( m_iReaderHnd, m_ucBusAdr, 0 );
		break;
	
	case 0x35:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x35_MODE, &ucValue[0]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x35_OPTION, &ucValue[1]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x35_CYCLE, &ucValue[2]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x35_CYCLE_TIMEOUT, &uiValue[0]));
		iBack = FEISC_0x35_SoftwareTrigger( m_iReaderHnd, m_ucBusAdr, ucValue[0], ucValue[1], ucValue[2], uiValue[0] );
		break;
	
	case 0x52:
		iBack = FEISC_0x52_GetBaud( m_iReaderHnd, m_ucBusAdr );
		break;
	
	case 0x55:
		iBack = FEISC_0x55_StartFlashLoaderEx( m_iReaderHnd, m_ucBusAdr );
		break;
	
	case 0x63:
		iBack = FEISC_0x63_CPUReset( m_iReaderHnd, m_ucBusAdr );

		// we must wait for Reader finishing CPU-Reset
#ifdef _FEDM_USB_SUPPORT
		HandleUsbEnumeration();
#endif
		break;
	
	case 0x64:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_SYSTEM_RESET_MODE, &ucValue[0]));
		iBack = FEISC_0x64_SystemReset( m_iReaderHnd, m_ucBusAdr, ucValue[0] );

		// we must wait for Reader finishing CPU-Reset
#ifdef _FEDM_USB_SUPPORT
		HandleUsbEnumeration();
#endif
		break;
	
	case 0x65:
		iBack = FEISC_0x65_SoftVersion( m_iReaderHnd, m_ucBusAdr, &ucValue[0], 0 );
		if(iBack == 0x80)
		{
			// command not available is the status, if a Reader doesn't support Standard Frames
			// we switch to Advanced Frame and repeat the command
			SetProtocolFrameSupport(FEDM_PRT_FRAME_ADVANCED);
			iBack = FEISC_0x65_SoftVersion( m_iReaderHnd, m_ucBusAdr, &ucValue[0], 0 );
		}
		
		if(iBack!=0x00 && iBack!=0x84 && iBack!=0xF1)
		{
			FEDM_RETURN(iBack);
		}

		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_SOFTVER, &ucValue[0], 16));
		// save reader type in member attribute m_ucReaderType
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_SOFTVER_SW_TYPE, &ucValue[0]));
		if(ucValue[0] & 0x80)
		{
			// reader class: read more information
			FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x00));
			iBack = SendProtocol(0x66);
		}
		else
		{
			FEDM_CHK2(iErr, SetReaderType((unsigned int)ucValue[0]));
		}
		break;
	
	case 0x66:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_READER_INFO_MODE, &ucValue[0]));
		iBack = FEISC_0x66_ReaderInfo( m_iReaderHnd, m_ucBusAdr, ucValue[0], m_ucData, 0 );
		if(iBack == 0x80 && (ucValue[0]==0x00 || ucValue[0]==0xFF))
		{
			// command not available in the status, if a Reader doesn't support Standard Frames
			// we switch to Advanced Frame and repeat the command
			SetProtocolFrameSupport(FEDM_PRT_FRAME_ADVANCED);
			iBack = FEISC_0x66_ReaderInfo( m_iReaderHnd, m_ucBusAdr, ucValue[0], m_ucData, 0 );
		}
		
		if(iBack!=0x00 && iBack!=0x84 && iBack!=0xF1)
			FEDM_RETURN(iBack);

		switch(ucValue[0])
		{
		case 0x00: // RF-Controller
			FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_SOFTVER, &m_ucData[0], 16));
			FEDM_CHK2(iErr, SetData(FEDM_GetAdrOfID(FEDM_ISC_TMP_READER_INFO, 32), &m_ucData[0], 31, FEDM_TMPDATA_MEM));
			FEDM_CHK2(iErr, Set_0x66_ReaderInfoData(ucValue[0], m_ucData));
			break;

		case 0xFF: // all
			FEDM_CHK2(iErr, Set_0x66_ReaderInfoData(ucValue[0], m_ucData));
			break;

		default:
			FEDM_CHK2(iErr, SetData(FEDM_GetAdrOfID(FEDM_ISC_TMP_READER_INFO, 32), &m_ucData[0], 31, FEDM_TMPDATA_MEM));
			FEDM_CHK2(iErr, Set_0x66_ReaderInfoData(ucValue[0], m_ucData));
			break;
		}
		break;
	
#if !defined(__BORLANDC__)
	case 0x68:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x68_CA_MODE, &ucValue[0]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x68_CA_RES1, &ucValue[1]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x68_CA_RES2, &ucValue[2]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x68_CA_RES3, &ucValue[3]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x68_CA_RES4, (unsigned int*)&iValue[0]));
		
		iBack = FEISC_0x68_ChannelAnalyze(	m_iReaderHnd, 
											m_ucBusAdr,
											ucValue[0],	// mode
											ucValue[1],	// reserved
											ucValue[2],	// reserved
											ucValue[3],	// reserved
											iValue[0],	// reserved
											m_nData );
		if(iBack!=0x00 && iBack!=0x94)
			FEDM_RETURN(iBack);

		FEDM_CHK2(iErr, SetCADTableData(ucValue[0], m_nData));
		break;
#endif

	case 0x69:
		iBack = FEISC_0x69_RFReset( m_iReaderHnd, m_ucBusAdr );
		if(iBack==0)
		{
			// remove a selection flag inside ISO table
			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
			if(iIdx >= 0)
				m_ISOTable[iIdx]->m_bIsSelected = false;
		}
		break;
	
	case 0x6A:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_RF_ONOFF, &ucValue[0]));
		iBack = FEISC_0x6A_RFOnOff( m_iReaderHnd, m_ucBusAdr, ucValue[0] );
		break;
	
	case 0x6B:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x6B_MODE, &ucValue[0]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x6B_TX_CHANNEL, &ucValue[1]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x6B_TX_PERIOD, &uiValue[0]));
		iBack = FEISC_0x6B_CentralizedRFSync(	m_iReaderHnd, 
												m_ucBusAdr, 
												ucValue[0], // mode byte
												ucValue[1], // RF transmit channel
												uiValue[0], // RF transmit period
												0,			// reserved
												0 );		// reserved
		break;
	
	case 0x6C:
		FEDM_CHK2(iErr, GetData(FEDM_ISCLR_TMP_NOISE_LEVEL, &ucValue[0], 6));
		iBack = FEISC_0x6C_SetNoiseLevel( m_iReaderHnd, m_ucBusAdr, &ucValue[0], 0 );
		break;
	
	case 0x6D:
		iBack = FEISC_0x6D_GetNoiseLevel( m_iReaderHnd, m_ucBusAdr, &ucValue[0], 0 );
		if(iBack!=0x00 && iBack!=0x84)
			FEDM_RETURN(iBack);

		FEDM_CHK2(iErr, SetData(FEDM_ISCLR_TMP_NOISE_LEVEL, &ucValue[0], 6));
		break;
	
	case 0x6E:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_DIAG_MODE, &ucValue[0]));
		if(ucValue[0] == 0x21)
		{
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_DIAG_REQ_DATA, &m_ucData[0]));
		}
		iBack = FEISC_0x6E_RdDiag( m_iReaderHnd, m_ucBusAdr, ucValue[0], &m_ucData[0] );
		if(iBack!=0x00 && iBack!=0x84)
			FEDM_RETURN(iBack);
		
		if(ucValue[0] == 0xFF)
		{
			FEDM_CHK2(iErr, Set_0x6E_ReaderDiagnosticData(ucValue[0], m_ucData));
		}
		else
		{
			FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_DIAG_DATA, &m_ucData[0], 31));
			FEDM_CHK2(iErr, Set_0x6E_ReaderDiagnosticData(ucValue[0], m_ucData));
		}

		break;
	
	case 0x6F:
		iBack = FEISC_0x6F_AntennaTuning( m_iReaderHnd, m_ucBusAdr );
		break;
	
	case 0x71:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_OUT_OS,		&uiValue[0]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_OUT_OSF,		&uiValue[1]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_OUT_OSTIME,	&uiValue[2]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_OUT_OUTTIME,	&uiValue[3]));
		
		iBack = FEISC_0x71_SetOutput( m_iReaderHnd, 
									  m_ucBusAdr, 
									  uiValue[0], 
									  uiValue[1], 
									  (int)uiValue[2], 
									  (int)uiValue[3] );

		break;
	
	case 0x72:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x72_OUT_MODE,	&ucValue[0]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x72_OUT_N,	&ucValue[1]));
		FEDM_CHK2(iErr, Get_0x72_OutputData(ucValue[1], &ucValue[2]));
		
		iBack = FEISC_0x72_SetOutput( m_iReaderHnd, 
									  m_ucBusAdr, 
									  ucValue[0], 
									  ucValue[1], 
									  &ucValue[2] );

		break;
	
	case 0x74:
		iBack = FEISC_0x74_ReadInput( m_iReaderHnd, m_ucBusAdr, &ucValue[0] );
		if(iBack!=0x00 && iBack!=0x84)
			FEDM_RETURN(iBack);

		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_INP_STATE, ucValue[0]));
		break;
	
	case 0x75:
		iBack = FEISC_0x75_AdjAntenna( m_iReaderHnd, m_ucBusAdr, &ucValue[0], 0 );
		if(iBack!=0x00 && iBack!=0xF1)
			FEDM_RETURN(iBack);

		FEDM_CHK2(iErr, SetData(FEDM_ISCM_TMP_ANTENNA_VALUE, &ucValue[0], 2));
		break;
	
	case 0x76:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x76_MODE, &ucValue[0]));
		iBack = FEISC_0x76_CheckAntennas( m_iReaderHnd, m_ucBusAdr, ucValue[0], &ucValue[1], &iValue[0] );
		if(iBack!=0x00)
			FEDM_RETURN(iBack);

		if(iValue[0] > 0)
		{
			FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_ANTENNA_OUT, &ucValue[1], 5));
		}
		break;
	
	case 0x80:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_READ_CFG,		&ucValue[0]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_READ_CFG_ADR,	&ucValue[1]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_READ_CFG_LOC,	&ucValue[2]));
		iBack = FEISC_0x80_ReadConfBlock( m_iReaderHnd, m_ucBusAdr, ucValue[0], &ucValue[3], 0 );
		if(iBack!=0x00 && iBack!=0x17 && iBack!=0x18 && iBack!=0x84 && iBack!=0xF1)
			FEDM_RETURN(iBack);

		// save new configuration data in RAM or EEPROM memory
		switch(ucValue[2])
		{
		case 0x00:	// RAM
//			for( iCnt=0; iCnt<14; iCnt++ )
//				m_RFC_RAMData[16*ucValue[1]+iCnt] = ucValue[3+iCnt];
			for( iCnt=0; iCnt<(m_iRFC_DataBlockSize-2); iCnt++ )
				m_RFC_RAMData[m_iRFC_DataBlockSize*ucValue[1] + iCnt] = ucValue[3+iCnt];
			
			// mark cfg block as read with a 0x01 in last (unused) byte
			// unmark cfg block as modified
			//m_RFC_RAMData[16*ucValue[1]+15] = 0x01;
			m_RFC_RAMData[m_iRFC_DataBlockSize*ucValue[1] + m_iRFC_DataBlockSize-1] = 0x01;
			break;
		case 0x01:	// EEPROM
//			for( iCnt=0; iCnt<14; iCnt++ )
//				m_RFC_EEData[16*ucValue[1]+iCnt] = ucValue[3+iCnt];
			for( iCnt=0; iCnt<(m_iRFC_DataBlockSize-2); iCnt++ )
				m_RFC_EEData[m_iRFC_DataBlockSize*ucValue[1] + iCnt] = ucValue[3+iCnt];
			
			// mark cfg block as read with a 0x01 in last (unused) byte
			// unmark cfg block as modified
//			m_RFC_EEData[16*ucValue[1]+15] = 0x01;
			m_RFC_EEData[m_iRFC_DataBlockSize*ucValue[1] + m_iRFC_DataBlockSize-1] = 0x01;
			break;
		}
		break;

	case 0x81:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_WRITE_CFG,	 &ucValue[0]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_WRITE_CFG_ADR, &ucValue[1]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_WRITE_CFG_LOC, &ucValue[2]));
		switch(ucValue[2])
		{
		case 0x00: // RAM
			if(!m_bDisableReadCfgBeforeWriteCfg)
			{
//				if( (m_RFC_RAMData[16*ucValue[1]+15] & 0x01) == 0x00)
				if( (m_RFC_RAMData[m_iRFC_DataBlockSize*ucValue[1]+m_iRFC_DataBlockSize-1] & 0x01) == 0x00)
				{
					FEDM_RETURN(FEDM_ERROR_CFG_BLOCK_PREVIOUSLY_NOT_READ);
				}
			}

//			for( iCnt=0; iCnt<14; iCnt++ )
//				ucValue[3+iCnt] = m_RFC_RAMData[16*ucValue[1]+iCnt];
			for( iCnt=0; iCnt<m_iRFC_DataBlockSize-2; iCnt++ )
				ucValue[3+iCnt] = m_RFC_RAMData[m_iRFC_DataBlockSize*ucValue[1]+iCnt];
			break;
		case 0x01: // RAM and EEPROM
			if(!m_bDisableReadCfgBeforeWriteCfg)
			{
//				if( (m_RFC_EEData[16*ucValue[1]+15] & 0x01) == 0x00)
				if( (m_RFC_EEData[m_iRFC_DataBlockSize*ucValue[1]+m_iRFC_DataBlockSize-1] & 0x01) == 0x00)
				{
					FEDM_RETURN(FEDM_ERROR_CFG_BLOCK_PREVIOUSLY_NOT_READ);
				}
			}

//			for( iCnt=0; iCnt<14; iCnt++ )
//				ucValue[3+iCnt] = m_RFC_EEData[16*ucValue[1]+iCnt];
			for( iCnt=0; iCnt<m_iRFC_DataBlockSize-2; iCnt++ )
				ucValue[3+iCnt] = m_RFC_EEData[m_iRFC_DataBlockSize*ucValue[1]+iCnt];
			break;
		}
		
		iBack = FEISC_0x81_WriteConfBlock( m_iReaderHnd, m_ucBusAdr, ucValue[0], &ucValue[3], 0 );
		if(iBack == 0x00 || iBack == 0x16)
		{
			switch(ucValue[2])
			{
			case 0x00: // RAM
//				m_RFC_RAMData[16*ucValue[1]+15] &= (~0x02); // unmark as modified
				m_RFC_RAMData[m_iRFC_DataBlockSize*ucValue[1]+m_iRFC_DataBlockSize-1] &= (~0x02); // unmark as modified
				break;
			case 0x01: // RAM and EEPROM
//				m_RFC_EEData[16*ucValue[1]+15] &= (~0x02); // unmark as modified
//				m_RFC_RAMData[16*ucValue[1]+15] &= (~0x02); // unmark as modified
				m_RFC_EEData[m_iRFC_DataBlockSize*ucValue[1]+m_iRFC_DataBlockSize-1] &= (~0x02); // unmark as modified
				m_RFC_RAMData[m_iRFC_DataBlockSize*ucValue[1]+m_iRFC_DataBlockSize-1] &= (~0x02); // unmark as modified
				break;
			}
		}

		break;

	case 0x82:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_SAVE_CFG, &ucValue[0]));
		iBack = FEISC_0x82_SaveConfBlock( m_iReaderHnd, m_ucBusAdr, ucValue[0] );
		break;
	
	case 0x83:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_RESET_CFG, &ucValue[0]));
		iBack = FEISC_0x83_ResetConfBlock( m_iReaderHnd, m_ucBusAdr, ucValue[0] );
		break;

	case 0x85:
		FEDM_CHK2(iErr, GetData(FEDM_ISCLR_TMP_TIME, &ucValue[0], 4));
		iBack = FEISC_0x85_SetSysTimer( m_iReaderHnd, m_ucBusAdr, &ucValue[0], 0 );
		break;

	case 0x86:
		iBack = FEISC_0x86_GetSysTimer( m_iReaderHnd, m_ucBusAdr, &ucValue[0], 0 );
		if(iBack!=0x00 && iBack!=0x84)
			FEDM_RETURN(iBack);

		FEDM_CHK2(iErr, SetData(FEDM_ISCLR_TMP_TIME, &ucValue[0], 4));
		break;

	case 0x87:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_DATE_CENTURY,	&ucValue[0]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_DATE_YEAR,		&ucValue[1]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_DATE_MONTH,	&ucValue[2]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_DATE_DAY,		&ucValue[3]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_DATE_TIMEZONE,	&ucValue[4]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_DATE_HOUR,		&ucValue[5]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_DATE_MINUTE,	&ucValue[6]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_DATE_MILLISECOND,	&uiValue[0]));

		iBack = FEISC_0x87_SetSystemDate(	m_iReaderHnd, 
											m_ucBusAdr, 
											ucValue[0], 
											ucValue[1], 
											ucValue[2], 
											ucValue[3], 
											ucValue[4], 
											ucValue[5], 
											ucValue[6], 
											(int)uiValue[0] );
		break;

	case 0x88:
		iBack = FEISC_0x88_GetSystemDate(	m_iReaderHnd, 
											m_ucBusAdr, 
											&ucValue[0], 
											&ucValue[1], 
											&ucValue[2], 
											&ucValue[3], 
											&ucValue[4], 
											&ucValue[5], 
											&ucValue[6], 
											(int*)&uiValue[0] );
		if(iBack!=0x00 && iBack!=0x84)
			FEDM_RETURN(iBack);

		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_DATE_CENTURY,	ucValue[0]));
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_DATE_YEAR,		ucValue[1]));
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_DATE_MONTH,	ucValue[2]));
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_DATE_DAY,		ucValue[3]));
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_DATE_TIMEZONE,	ucValue[4]));
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_DATE_HOUR,		ucValue[5]));
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_DATE_MINUTE,	ucValue[6]));
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_DATE_MILLISECOND,	uiValue[0]));
		break;
	
	case 0x8A:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x8A_READ_DEVICE,		&ucValue[0]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x8A_READ_BANK,		&ucValue[1]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x8A_READ_MODE,		&ucValue[2]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x8A_READ_REQ_CFG_ADR,	(unsigned int*)&iValue[0]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x8A_READ_REQ_CFG_N,	&ucValue[3]));

		iBack = FEISC_0x8A_ReadConfiguration(	m_iReaderHnd, 
												m_ucBusAdr, 
												ucValue[0],		// in: device
												ucValue[1],		// in: bank
												ucValue[2],		// in: mode
												(unsigned int)iValue[0],		// in: first block address
												ucValue[3],		// in: number of blocks
												&ucValue[4],	// out: number of blocks
												&ucValue[5],	// out: blocksize
												m_ucData );		// out: configuration data

		if(iBack!=0x00 && iBack!=0x17 && iBack!=0x18 && iBack!=0x84 && iBack!=0xF1)
			FEDM_RETURN(iBack);

		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8A_READ_RSP_CFG_N,	 ucValue[4]));
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8A_READ_RSP_CFG_SIZE, ucValue[5]));

		// save new configuration data in RAM or EEPROM memory
		switch(ucValue[2] & 0x01)
		{
		case 0x00:	// RAM
			if(ucValue[0] == FEDM_CNTRL_TYPE_RFC)
			{
				for( iBlockCnt=0; iBlockCnt<(int)(ucValue[4]); iBlockCnt++ )
				{
					uiAdr = (m_ucData[iBlockCnt * (ucValue[5]+2)] << 8) + m_ucData[iBlockCnt * (ucValue[5]+2)+1];
					for( iCnt=0; iCnt<(m_iRFC_DataBlockSize-2); iCnt++ )
						m_RFC_RAMData[m_iRFC_DataBlockSize*uiAdr + iCnt] = m_ucData[2 + iBlockCnt * (ucValue[5]+2) + iCnt];
					
					// mark cfg block as read with a 0x01 in last (unused) byte
					// unmark cfg block as modified
					m_RFC_RAMData[m_iRFC_DataBlockSize*uiAdr + m_iRFC_DataBlockSize-1] = 0x01;
				}
			}
			else if(ucValue[0] == FEDM_CNTRL_TYPE_ACC)
			{
				for( iBlockCnt=0; iBlockCnt<(int)(ucValue[4]); iBlockCnt++ )
				{
					uiAdr = (m_ucData[iBlockCnt * (ucValue[5]+2)] << 8) + m_ucData[iBlockCnt * (ucValue[5]+2)+1];
					if(uiAdr < 64)
					{
						for( iCnt=0; iCnt<14; iCnt++ )
							m_RFC_RAMData[16*uiAdr + iCnt] = m_ucData[2 + iBlockCnt * (ucValue[5]+2) + iCnt];
					
						// mark cfg block as read with a 0x01 in last (unused) byte
						// unmark cfg block as modified
						m_RFC_RAMData[16*uiAdr + 15] = 0x01;
					}
					else
					{
						for( iCnt=0; iCnt<(int)(ucValue[5]); iCnt++ )
							m_ACC_RAMData[(int)(ucValue[5]+2)*uiAdr + iCnt] = m_ucData[2 + iBlockCnt * (ucValue[5]+2) + iCnt];
					
						// mark cfg block as read with a 0x01 in last (unused) byte
						// unmark cfg block as modified
						m_ACC_RAMData[(int)(ucValue[5]+2)*uiAdr + (int)(ucValue[5]+1)] = 0x01;
					}
				}
			}
			break;

		case 0x01:	// EEPROM
			if(ucValue[0] == FEDM_CNTRL_TYPE_RFC)
			{
				for( iBlockCnt=0; iBlockCnt<(int)(ucValue[4]); iBlockCnt++ )
				{
					uiAdr = (m_ucData[iBlockCnt * (ucValue[5]+2)] << 8) + m_ucData[iBlockCnt * (ucValue[5]+2)+1];
					for( iCnt=0; iCnt<(m_iRFC_DataBlockSize-2); iCnt++ )
						m_RFC_EEData[m_iRFC_DataBlockSize*uiAdr + iCnt] = m_ucData[2 + iBlockCnt * (ucValue[5]+2) + iCnt];
					
					// mark cfg block as read with a 0x01 in last (unused) byte
					// unmark cfg block as modified
					m_RFC_EEData[m_iRFC_DataBlockSize*uiAdr + m_iRFC_DataBlockSize-1] = 0x01;
				}
			}
			else if(ucValue[0] == FEDM_CNTRL_TYPE_ACC)
			{
				for( iBlockCnt=0; iBlockCnt<(int)(ucValue[4]); iBlockCnt++ )
				{
					uiAdr = (m_ucData[iBlockCnt * (ucValue[5]+2)] << 8) + m_ucData[iBlockCnt * (ucValue[5]+2)+1];
					if(uiAdr < 64)
					{
						for( iCnt=0; iCnt<14; iCnt++ )
							m_RFC_EEData[16*uiAdr + iCnt] = m_ucData[2 + iBlockCnt * (ucValue[5]+2) + iCnt];
						
						// mark cfg block as read with a 0x01 in last (unused) byte
						// unmark cfg block as modified
						m_RFC_EEData[16*uiAdr + 15] = 0x01;
					}
					else
					{
						for( iCnt=0; iCnt<(int)ucValue[5]; iCnt++ )
							m_ACC_EEData[(int)(ucValue[5]+2)*uiAdr + iCnt] = m_ucData[2 + iBlockCnt * (ucValue[5]+2) + iCnt];
						
						// mark cfg block as read with a 0x01 in last (unused) byte
						// unmark cfg block as modified
						m_ACC_EEData[(int)(ucValue[5]+2)*uiAdr + (int)(ucValue[5]+1)] = 0x01;
					}
				}
			}
			break;
		}
		break;

	case 0x8B:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x8B_WRITE_DEVICE,		&ucValue[0]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x8B_WRITE_BANK,		&ucValue[1]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x8B_WRITE_MODE,		&ucValue[2]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x8B_WRITE_CFG_N,		&ucValue[3]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x8B_WRITE_CFG_SIZE,	&ucValue[4]));
		iTmpMemAdr = FEDM_GetAdrOfID(FEDM_ISC_TMP_0x8B_WRITE_CFG_ADR, 32);	// start address in memory

		switch(ucValue[2] & 0x01)
		{
		case 0x00: // RAM
			if(ucValue[0] == FEDM_CNTRL_TYPE_RFC)
			{
				iByteCnt = 0;
				for( iBlockCnt=0; iBlockCnt<(int)(ucValue[3]); iBlockCnt++ )
				{
					uiAdr = ((unsigned int)m_TmpData[iTmpMemAdr]<<8) + m_TmpData[iTmpMemAdr+1];
					if(!m_bDisableReadCfgBeforeWriteCfg)
					{
						if( (m_RFC_RAMData[m_iRFC_DataBlockSize*uiAdr + m_iRFC_DataBlockSize-1] & 0x01) == 0x00)
							FEDM_RETURN(FEDM_ERROR_CFG_BLOCK_PREVIOUSLY_NOT_READ);
					}

					// insert first CFG-ADR...
					m_ucData[iByteCnt++] = m_TmpData[iTmpMemAdr];
					m_ucData[iByteCnt++] = m_TmpData[iTmpMemAdr+1];
					iTmpMemAdr += 2;

					// ...then add CFG-DATA
					for( iCnt=0; iCnt<(m_iRFC_DataBlockSize-2); iCnt++ )
						m_ucData[iByteCnt++] = m_RFC_RAMData[m_iRFC_DataBlockSize*uiAdr + iCnt];
				}
			}
			else if(ucValue[0] == FEDM_CNTRL_TYPE_ACC)
			{
				iByteCnt = 0;
				for( iBlockCnt=0; iBlockCnt<(int)(ucValue[3]); iBlockCnt++ )
				{
					uiAdr = ((unsigned int)m_TmpData[iTmpMemAdr]<<8) + m_TmpData[iTmpMemAdr+1];
					if(uiAdr < 64)
					{
						if(!m_bDisableReadCfgBeforeWriteCfg)
						{
							if( (m_RFC_RAMData[16*uiAdr + 15] & 0x01) == 0x00)
								FEDM_RETURN(FEDM_ERROR_CFG_BLOCK_PREVIOUSLY_NOT_READ);
						}
					}
					else
					{
						if(!m_bDisableReadCfgBeforeWriteCfg)
						{
							if( (m_ACC_RAMData[(int)(ucValue[4]+2)*uiAdr + (int)(ucValue[4]+1)] & 0x01) == 0x00)
								FEDM_RETURN(FEDM_ERROR_CFG_BLOCK_PREVIOUSLY_NOT_READ);
						}
					}

					// insert first CFG-ADR...
					m_ucData[iByteCnt++] = m_TmpData[iTmpMemAdr];
					m_ucData[iByteCnt++] = m_TmpData[iTmpMemAdr+1];
					iTmpMemAdr += 2;

					// ...then add CFG-DATA
					if(uiAdr < 64)
					{
						for( iCnt=0; iCnt<14; iCnt++ )
							m_ucData[iByteCnt++] = m_RFC_RAMData[16*uiAdr + iCnt];
					}
					else
					{
						for( iCnt=0; iCnt<(int)(ucValue[4]); iCnt++ )
							m_ucData[iByteCnt++] = m_ACC_RAMData[(int)(ucValue[4]+2)*uiAdr + iCnt];
					}
				}
			}
			break;

		case 0x01: // RAM and EEPROM
			if(ucValue[0] == FEDM_CNTRL_TYPE_RFC)
			{
				iByteCnt = 0;
				for( iBlockCnt=0; iBlockCnt<(int)(ucValue[3]); iBlockCnt++ )
				{
					uiAdr = ((unsigned int)m_TmpData[iTmpMemAdr]<<8) + m_TmpData[iTmpMemAdr+1];
					if(!m_bDisableReadCfgBeforeWriteCfg)
					{
						if( (m_RFC_EEData[m_iRFC_DataBlockSize*uiAdr + m_iRFC_DataBlockSize-1] & 0x01) == 0x00)
							FEDM_RETURN(FEDM_ERROR_CFG_BLOCK_PREVIOUSLY_NOT_READ);
					}

					// insert first CFG-ADR...
					m_ucData[iByteCnt++] = m_TmpData[iTmpMemAdr];
					m_ucData[iByteCnt++] = m_TmpData[iTmpMemAdr+1];
					iTmpMemAdr += 2;

					// ...then add CFG-DATA
					for( iCnt=0; iCnt<(m_iRFC_DataBlockSize-2); iCnt++ )
						m_ucData[iByteCnt++] = m_RFC_EEData[m_iRFC_DataBlockSize*uiAdr + iCnt];
				}
			}
			else if(ucValue[0] == FEDM_CNTRL_TYPE_ACC)
			{
				iByteCnt = 0;
				for( iBlockCnt=0; iBlockCnt<(int)(ucValue[3]); iBlockCnt++ )
				{
					uiAdr = ((unsigned int)m_TmpData[iTmpMemAdr]<<8) + m_TmpData[iTmpMemAdr+1];
					if(uiAdr < 64)
					{
						if(!m_bDisableReadCfgBeforeWriteCfg)
						{
							if( (m_RFC_EEData[16*uiAdr + 15] & 0x01) == 0x00)
								FEDM_RETURN(FEDM_ERROR_CFG_BLOCK_PREVIOUSLY_NOT_READ);
						}
					}
					else
					{
						if(!m_bDisableReadCfgBeforeWriteCfg)
						{
							if( (m_ACC_EEData[(int)(ucValue[4]+2)*uiAdr + (int)(ucValue[4]+1)] & 0x01) == 0x00)
								FEDM_RETURN(FEDM_ERROR_CFG_BLOCK_PREVIOUSLY_NOT_READ);
						}
					}

					// insert first CFG-ADR...
					m_ucData[iByteCnt++] = m_TmpData[iTmpMemAdr];
					m_ucData[iByteCnt++] = m_TmpData[iTmpMemAdr+1];
					iTmpMemAdr += 2;

					// ...then add CFG-DATA
					if(uiAdr < 64)
					{
						for( iCnt=0; iCnt<14; iCnt++ )
							m_ucData[iByteCnt++] = m_RFC_EEData[16*uiAdr + iCnt];
					}
					else
					{
						for( iCnt=0; iCnt<(int)(ucValue[4]); iCnt++ )
							m_ucData[iByteCnt++] = m_ACC_EEData[(int)(ucValue[4]+2)*uiAdr + iCnt];
					}
				}
			}
			break;
		}

		iBack = FEISC_0x8B_WriteConfiguration(	m_iReaderHnd, 
												m_ucBusAdr, 
												ucValue[0],		// in: device
												ucValue[1],		// in: bank
												ucValue[2],		// in: mode
												ucValue[3],		// in: number of blocks
												ucValue[4],		// in: blocksize
												m_ucData );		// in: configuration data

		if(iBack == 0x00 || iBack == 0x16)
		{
			iTmpMemAdr = FEDM_GetAdrOfID(FEDM_ISC_TMP_0x8B_WRITE_CFG_ADR, 32);	// start address in memory
			iByteCnt = 0;
			for( iBlockCnt=0; iBlockCnt<(int)(ucValue[3]); iBlockCnt++ )
			{
				uiAdr = ((unsigned int)m_TmpData[iTmpMemAdr]<<8) + m_TmpData[iTmpMemAdr+1];
				iTmpMemAdr += 2;

				if(ucValue[0] == FEDM_CNTRL_TYPE_RFC)
				{
					switch(ucValue[2])
					{
					case 0x00: // RAM
						m_RFC_RAMData[m_iRFC_DataBlockSize*uiAdr + m_iRFC_DataBlockSize-1] &= (~0x02); // unmark as modified
						break;
					case 0x01: // RAM and EEPROM
						m_RFC_RAMData[m_iRFC_DataBlockSize*uiAdr + m_iRFC_DataBlockSize-1] &= (~0x02); // unmark as modified
						m_RFC_EEData[m_iRFC_DataBlockSize*uiAdr + m_iRFC_DataBlockSize-1] &= (~0x02); // unmark as modified
						break;
					}
				}
				else if(ucValue[0] == FEDM_CNTRL_TYPE_ACC)
				{
					if(uiAdr < 64)
					{
						switch(ucValue[2])
						{
						case 0x00: // RAM
							m_RFC_RAMData[16*uiAdr + 15] &= (~0x02); // unmark as modified
							break;
						case 0x01: // RAM and EEPROM
							m_RFC_RAMData[16*uiAdr + 15] &= (~0x02); // unmark as modified
							m_RFC_EEData[16*uiAdr + 15] &= (~0x02); // unmark as modified
							break;
						}
					}
					else
					{
						switch(ucValue[2])
						{
						case 0x00: // RAM
							m_ACC_RAMData[(int)(ucValue[4]+2)*uiAdr + (int)(ucValue[4]+1)] &= (~0x02); // unmark as modified
							break;
						case 0x01: // RAM and EEPROM
							m_ACC_RAMData[(int)(ucValue[4]+2)*uiAdr + (int)(ucValue[4]+1)] &= (~0x02); // unmark as modified
							m_ACC_EEData[(int)(ucValue[4]+2)*uiAdr + (int)(ucValue[4]+1)] &= (~0x02); // unmark as modified
							break;
						}
					}
				}
			}
		}
		break;
	
	case 0x8C:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x8C_RESET_DEVICE,		&ucValue[0]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x8C_RESET_BANK,		&ucValue[1]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x8C_RESET_MODE,		&ucValue[2]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x8C_RESET_CFG_ADR,	(unsigned int*)&iValue[0]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0x8C_RESET_CFG_N,		&ucValue[3]));

		iBack = FEISC_0x8C_ResetConfiguration(	m_iReaderHnd, 
												m_ucBusAdr, 
												ucValue[0],		// in: device
												ucValue[1],		// in: bank
												ucValue[2],		// in: mode
												(unsigned int)iValue[0],		// in: block address
												ucValue[3] );	// in: number of blocks
		break;
	
	case 0xA0:
		FEDM_CHK2(iErr, GetData(FEDM_ISCLR_TMP_READER_PW, ucPW, 4));
		iBack = FEISC_0xA0_RdLogin( m_iReaderHnd, m_ucBusAdr, ucPW, 0 );
		break;
	
	case 0xA2:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_ISO14443A_KEY_TYPE, &ucValue[0]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_ISO14443A_KEY_ADR, &ucValue[1]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_ISO14443A_KEY, &ucValue[2], 6));
		iBack = FEISC_0xA2_WriteMifareKeys( m_iReaderHnd, 
											m_ucBusAdr, 
											ucValue[0], 
											ucValue[1], 
											&ucValue[2], 
											0 );
		break;
	
	case 0xA3:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xA3_MODE, &ucValue[0]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xA3_KEY_INDEX, &ucValue[1]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xA3_AUTHENTICATE_MODE, &ucValue[2]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xA3_KEY_LEN, &ucValue[3]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xA3_KEY, &ucValue[4], ucValue[3]));
		iBack = FEISC_0xA3_Write_DES_AES_Keys(	m_iReaderHnd, 
												m_ucBusAdr, 
												ucValue[0], 
												ucValue[1], 
												ucValue[2], 
												ucValue[3], 
												&ucValue[4], 
												0 );
		break;
	
	case 0xAD:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xAD_MODE, &ucValue[0]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xAD_KEY_TYPE, &ucValue[1]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xAD_KEY_LEN, &ucValue[2]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xAD_KEY, &ucValue[3], ucValue[2]));
		iBack = FEISC_0xAD_WriteReaderAuthentKey(	m_iReaderHnd, 
													m_ucBusAdr, 
													ucValue[0], 
													ucValue[1], 
													ucValue[2], 
													&ucValue[3], 
													0 );
		break;
	
	case 0xAE:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xAE_MODE, &ucValue[0]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xAE_KEY_TYPE, &ucValue[1]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xAE_KEY_LEN, &ucValue[2]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xAE_KEY, &ucValue[3], ucValue[2]));
		iBack = FEISC_0xAE_ReaderAuthent(	m_iReaderHnd, 
											m_ucBusAdr, 
											ucValue[0], 
											ucValue[1], 
											ucValue[2], 
											&ucValue[3], 
											0 );
		break;

	case 0xB0:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_B0_CMD, &ucIsoCmd));
		FEDM_CHK2(iLen, Get_0xB0_IsoTableData(ucIsoCmd, &m_ucData[0]));

		iBack = FEISC_0xB0_ISOCmd( m_iReaderHnd, 
								   m_ucBusAdr, 
								   &m_ucData[0], 
								   iLen, 
								   &m_ucData[0], 
								   &iLen, 
								   0 );

		// evaluate status byte
		if( iBack==0x00 ||	// ok
			iBack==0x01 ||	// no transponder
			iBack==0x02 ||	// data false
			iBack==0x03 ||	// write error
			iBack==0x83 ||	// rf communication error
			iBack==0x84 ||	// rf warning
			iBack==0x90 ||	// no valid data
			iBack==0x93 ||	// data buffer overflow
			iBack==0x94 ||	// more data
			iBack==0x95 )	// tag error
		{	
			if(iBack == 0x01)
			{
				// if command in selected mode and transponder doesn't respond
				// then remove selection flag
				FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_B0_MODE_ADR, &ucIsoMode));
				if(ucIsoMode == FEDM_ISC_ISO_MODE_SEL)
				{
					iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
					if(iIdx >= 0)
						m_ISOTable[iIdx]->m_bIsSelected = false;
				}
			}
			else if(iBack == 0x02)
			{
				if (ucIsoCmd == 0x01) // exception for Inventory
				{
					// save new transponder data in iso-table
					FEDM_CHK2(iErr, Set_0xB0_IsoTableData(&m_ucData[0], iLen, iBack));
				}
				else
				{
					FEDM_RETURN(iBack);
				}
			}
			else if(iBack == 0x03)
			{
				// additional data with
				// [0x22] Lock Multiple Blocks or 
				// [0x24] Write Multiple Blocks
				if( ucIsoCmd==0x22 || ucIsoCmd==0x24 )
				{
					FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_B0_MODE_EXT_ADR, &bExtAdr));
					if(bExtAdr)
					{
						// verify data length (some old readers don't support additional bytes)
						if(iLen == 2)
						{
							FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_B0_RSP_DB_EXT_ADR_E, &m_ucData[0], 2));
						}
						else
						{
							FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_B0_RSP_DB_ADR_E, m_ucData[0]));
						}
					}
					else
					{
						// verify data length (some old readers don't support additional bytes)
						if(iLen == 1)
						{
							FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_B0_RSP_DB_ADR_E, m_ucData[0]));
						}
					}
				}
			}
			else if(iBack == 0x95)
			{
				FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_LAST_STATE, (unsigned char)iBack));
				FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_B0_ISO_ERROR, m_ucData[0]));

				// additional data with
				// [0x22] Lock Multiple Blocks or 
				// [0x24] Write Multiple Blocks
				if( ucIsoCmd==0x22 || ucIsoCmd==0x24 )
				{
					FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_B0_MODE_EXT_ADR, &bExtAdr));
					if(bExtAdr)
					{
						// verify data length (some old readers don't support additional bytes)
						if(iLen == 3)
						{
							FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_B0_RSP_DB_EXT_ADR_E, &m_ucData[1], 2));
						}
						else
						{
							FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_B0_RSP_DB_ADR_E, m_ucData[1]));
						}
					}
					else
					{
						// verify data length (some old readers don't support additional bytes)
						if(iLen == 2)
						{
							FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_B0_RSP_DB_ADR_E, m_ucData[1]));
						}
					}
				}
			}
			else
			{
				// save new transponder data in iso-table
				FEDM_CHK2(iErr, Set_0xB0_IsoTableData(&m_ucData[0], iLen, iBack));
			}
		}
		else
		{
			FEDM_RETURN(iBack);
		}
		
		break;
	
	case 0xB1:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_B1_MFR, &ucValue[0]));
		switch(ucValue[0])
		{
		case FEDM_ISC_ISO_MFR_NXP:
		case FEDM_ISC_ISO_MFR_TI:
		case FEDM_ISC_ISO_MFR_STM:
		case FEDM_ISC_ISO_MFR_EM:
		case FEDM_ISC_ISO_MFR_FUJITSU:
		case FEDM_ISC_ISO_MFR_KSW:
		case FEDM_ISC_ISO_MFR_INFINEON:
		case FEDM_ISC_ISO_MFR_IDS:
			break;
		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0xB1, Manufacturer=0x" << std::hex << std::uppercase << (int)ucValue[0] << ") - is not supported");
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
		}
			
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_CMD, &ucIsoCmd));
		FEDM_CHK2(iLen, Get_0xB1_IsoTableData(ucIsoCmd, &m_ucData[0]));
		
		iBack = FEISC_0xB1_ISOCustAndPropCmd( m_iReaderHnd, 
											  m_ucBusAdr, 
											  ucValue[0], // mfr
											  &m_ucData[0], 
											  iLen, 
											  &m_ucData[0], 
											  &iLen, 
											  0 );

		if(iBack == 0x00)
		{
			FEDM_CHK2(iErr, Set_0xB1_IsoTableData(ucIsoCmd, &m_ucData[0], iLen));
		}
		else if(iBack == 0x01)
		{
			// if command in selected mode and transponder doesn't respond
			// then remove selection flag
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_B1_MODE_ADR, &ucIsoMode));
			if(ucIsoMode == FEDM_ISC_ISO_MODE_SEL)
			{
				iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
				if(iIdx >= 0)
					m_ISOTable[iIdx]->m_bIsSelected = false;
			}
		}
		else if(iBack == 0x03)
		{
			// for Infineon:
			// [0x30] Write
			// [0x90] Write Byte
			// [0xAB] Set Advanced Quiet Bit
			if(iLen == 1)
			{
				FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_B1_RSP_DB_ADR_E, m_ucData[0]));
			}
		}
		else if(iBack == 0x95)
		{
			FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_LAST_STATE, (unsigned char)iBack));
			FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_B1_ISO_ERROR, m_ucData[0]));
			// for Infineon:
			// [0x30] Write
			// [0x90] Write Byte
			// [0xAB] Set Advanced Quiet Bit
			if(iLen == 2)
			{
				FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_B1_RSP_DB_ADR_E, m_ucData[1]));
			}
		}

		break;

	case 0xB2:
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B2_CMD, &ucIsoCmd));
		FEDM_CHK2(iLen, Get_0xB2_IsoTableData(ucIsoCmd, &m_ucData[0]));

		iBack = FEISC_0xB2_ISOCmd( m_iReaderHnd, 
								   m_ucBusAdr, 
								   &m_ucData[0], 
								   iLen, 
								   &m_ucData[0], 
								   &iLen, 
								   0 );
		
		if(iBack == 0x00)
		{
			// save new transponder data in iso-table
			FEDM_CHK2(iErr, Set_0xB2_IsoTableData(&m_ucData[0], iLen, iBack));
		}
		else if(iBack == 0x01)
		{
			// if command in selected mode and transponder doesn't respond
			// then remove selection flag
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_B2_MODE_ADR, &ucIsoMode));
			if(ucIsoMode == FEDM_ISC_ISO_MODE_SEL)
			{
				iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
				if(iIdx >= 0)
					m_ISOTable[iIdx]->m_bIsSelected = false;
			}
		}
		else if(iBack == 0x95)
		{
			FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_LAST_STATE, (unsigned char)iBack));
			FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_B2_ISO_ERROR, m_ucData[0]));
		}
		
		break;

	case 0xB3:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_B3_CMD, &ucEpcCmd));
		FEDM_CHK2(iLen, Get_0xB3_IsoTableData(ucEpcCmd, &m_ucData[0]));
	
		iBack = FEISC_0xB3_EPCCmd( m_iReaderHnd, 
								   m_ucBusAdr, 
								   &m_ucData[0], 
								   iLen, 
								   &m_ucData[0], 
								   &iLen, 
								   0 );

		if( iBack==0x00 || 
			iBack==0x03 || 
			iBack==0x83 || 
			iBack==0x84 || 
			iBack==0x90 || 
			iBack==0x93 || 
			iBack==0x94 || 
			iBack==0x95 )
		{	
			if(iBack == 0x03)
			{
				// additional data with
				// [0x24] Write Multiple Blocks
				if( ucEpcCmd==0x24 )
				{
					FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_B3_MODE_EXT_ADR, &bExtAdr));
					if(bExtAdr)
					{
						// verify data length (some old readers don't support additional bytes)
						if(iLen == 2)
						{
							FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_B3_RSP_DB_EXT_ADR_E, &m_ucData[0], 2));
						}
						else
						{
							FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_B3_RSP_DB_ADR_E, m_ucData[0]));
						}
					}
					else
					{
						FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_B3_RSP_DB_ADR_E, m_ucData[0]));
					}
				}
			}
			else if(iBack == 0x95)
			{
				FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_LAST_STATE, (unsigned char)iBack));
				FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_B3_TAG_ERROR, m_ucData[0]));

				// additional data with
				// [0x24] Write Multiple Blocks
				if( ucEpcCmd==0x24 )
				{
					FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_B3_MODE_EXT_ADR, &bExtAdr));
					if(bExtAdr)
					{
						// verify data length (some old readers don't support additional bytes)
						if(iLen == 3)
						{
							FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_B3_RSP_DB_EXT_ADR_E, &m_ucData[1], 2));
						}
						else
						{
							FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_B3_RSP_DB_ADR_E, m_ucData[1]));
						}
					}
					else
					{
						FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_B3_RSP_DB_ADR_E, m_ucData[1]));
					}
				}
			}
			else
			{
				// save data
				FEDM_CHK2(iErr, Set_0xB3_IsoTableData(&m_ucData[0], iLen, iBack));
			}
		}
		else
		{
			FEDM_RETURN(iBack);
		}
		
		break;

	case 0xB4:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_B4_MFR, &ucValue[0]));
		switch(ucValue[0])
		{
		case FEDM_ISC_ISO_MFR_NXP:
		case FEDM_ISC_ISO_MFR_EM:
		case FEDM_ISC_ISO_MFR_IDS:
			break;
		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0xB4), Manufacturer=0x" << std::hex << std::uppercase << (int)ucValue[0] << ") - is not supported");
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
		}
			
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_B4_CMD, &ucEpcCmd));
		FEDM_CHK2(iLen, Get_0xB4_IsoTableData(ucEpcCmd, &m_ucData[0]));
	
		iBack = FEISC_0xB4_EPC_UHF_Cmd( m_iReaderHnd, 
										m_ucBusAdr, 
										ucValue[0],		// MFR
										&m_ucData[0], 
										iLen, 
										&m_ucData[0], 
										&iLen, 
										0 );

		if( iBack==0x00 || 
			iBack==0x03 || 
			iBack==0x83 || 
			iBack==0x84 || 
			iBack==0x90 || 
			iBack==0x93 || 
			iBack==0x94 || 
			iBack==0x95 )
		{	
			if(iBack == 0x00)
			{
				FEDM_CHK2(iErr, Set_0xB4_IsoTableData(ucEpcCmd, &m_ucData[0], iLen));
			}
			else if(iBack == 0x03)
			{
				if(ucValue[0] == FEDM_ISC_ISO_MFR_NXP)
				{
					// additional data with
					// [0x00] Block Lock
					if( ucEpcCmd==0x00 )
					{
						FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, &bExtAdr));
						if(bExtAdr)
						{
							// verify data length (some old readers don't support additional bytes)
							if(iLen == 2)
							{
								FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_B4_RSP_DB_EXT_ADR_E, &m_ucData[0], 2));
							}
							else
							{
								FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_B4_RSP_DB_ADR_E, m_ucData[0]));
							}
						}
						else
						{
							FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_B4_RSP_DB_ADR_E, m_ucData[0]));
						}
					}
				}
			}
			else if(iBack == 0x95)
			{
				FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_LAST_STATE, (unsigned char)iBack));
				FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_B4_ISO_ERROR, m_ucData[0]));

				if(ucValue[0] == FEDM_ISC_ISO_MFR_NXP)
				{
					// additional data with
					// [0x00] Block Lock
					if( ucEpcCmd==0x00 )
					{
						FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, &bExtAdr));
						if(bExtAdr)
						{
							// verify data length (some old readers don't support additional bytes)
							if(iLen == 3)
							{
								FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_B4_RSP_DB_EXT_ADR_E, &m_ucData[1], 2));
							}
							else
							{
								FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_B4_RSP_DB_ADR_E, m_ucData[1]));
							}
						}
						else
						{
							FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_B4_RSP_DB_ADR_E, m_ucData[1]));
						}
					}
				}
			}
			else
			{
			}
		}
		else
		{
			FEDM_RETURN(iBack);
		}
		
		break;

	case 0xC1:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_CMD, &ucDesFireCmd));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_MODE, &m_ucData[0]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_APP_ID, &m_ucData[1], 3));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_KEY_INDEX, &m_ucData[4]));
		FEDM_CHK2(iLen, Get_0xC1_IsoTableData(ucDesFireCmd, &m_ucData[5]));
	
		iBack = FEISC_0xC1_DESFireCmd( m_iReaderHnd, 
									   m_ucBusAdr, 
									   ucDesFireCmd,	// sub command
									   m_ucData[0],		// mode
									   &m_ucData[1],	// application ID
									   m_ucData[4],		// reader key index
									   &m_ucData[5],	// request data
									   iLen,
									   &m_ucData[0], 
									   &iLen,
									   0 );

		if( iBack==0x00 )
		{
			FEDM_CHK2(iErr, Set_0xC1_IsoTableData(ucDesFireCmd, &m_ucData[0], iLen));
		}
		else if( iBack==0x97 )
		{
			FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0xC1_ERROR_SOURCE, m_ucData[0]));
			FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0xC1_ERROR_CODE, &m_ucData[1], 2));
		}
		else
		{
			FEDM_RETURN(iBack);
		}

		break;

	case 0xC2:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_CMD, &ucMifarePlusCmd));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_MODE, &m_ucData[0]));
		FEDM_CHK2(iLen, Get_0xC2_IsoTableData(ucMifarePlusCmd, &m_ucData[1]));
	
		iBack = FEISC_0xC2_MifarePlusCmd(	m_iReaderHnd, 
											m_ucBusAdr, 
											ucMifarePlusCmd,	// sub command
											m_ucData[0],		// mode
											&m_ucData[1],		// request data
											iLen,
											&m_ucData[0], 
											&iLen,
											0 );

		if( iBack==0x00 )
		{
			FEDM_CHK2(iErr, Set_0xC2_IsoTableData(ucMifarePlusCmd, &m_ucData[0], iLen));
		}
		else if( iBack==0x97 )
		{
			FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0xC2_ERROR_SOURCE, m_ucData[0]));
			FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0xC2_ERROR_CODE, &m_ucData[1], 2));
		}
		else
		{
			FEDM_RETURN(iBack);
		}

		break;

	case 0xC3:
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_CMD, &ucDesFireCmd));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_MODE, &m_ucData[0]));
		FEDM_CHK2(iLen, Get_0xC3_IsoTableData(ucDesFireCmd, m_ucData[0], &m_ucData[1]));
	
		iBack = FEISC_0xC3_DESFireCmd( m_iReaderHnd, 
									   m_ucBusAdr, 
									   ucDesFireCmd,	// sub command
									   m_ucData[0],		// mode
									   &m_ucData[1],	// request data
									   iLen,
									   &m_ucData[0], 
									   &iLen,
									   0 );

		if( iBack==0x00 )
		{
			FEDM_CHK2(iErr, Set_0xC3_IsoTableData(ucDesFireCmd, &m_ucData[0], iLen));
		}
		else if( iBack==0x97 )
		{
			FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0xC3_ERROR_SOURCE, m_ucData[0]));
			FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0xC3_ERROR_CODE, &m_ucData[1], 2));
		}
		else
		{
			FEDM_RETURN(iBack);
		}

		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNKNOWN_CONTROL_BYTE));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_CONTROL_BYTE);
	}

	FEDM_RETURN(iBack);
}


/***************************************************************************
  Begin        		:	16.08.2000 / M. Hultsch
  Version       	:	04.06.09 / 30.10.2014 / M. Hultsch
						- support for new baudrates: 921600 and 460800
  
						04.02.07 / 09.08.2012 / M. Hultsch
						- return of crypto processing error
  
						03.01.06 / 10.05.2010 / M. Hultsch
						- new parameters cbFct and pAny
						- iFrameSupport = 3

						02.04.02 / 11.05.2006 / M. Hultsch

  Function			:	find the baudrate and frame of reader
						!!! no multi-threading !!!
						!!! only for serial port !!!

  Parameters		:	bool* pStop			- optional: when true, function returns at once
						int iTimeout		- optional: timeout to be used (default: 300ms)
						int iFrameSupport	- optional: protocol frame to be used (default: 2)
														0: both protocol frames
														1: only standard protocol frame
														2: only advanced protocol frame
														3: alternating, beginning with standard protocol frame
						FEDM_CB_FIND_BAUDRATE cbFct	-	callback function (s. FEDM_DataBase.h) to signal application actual action
						void* pAny			- pointer to anything which is reflected as 1st parameter in cbFct

  Return value		:	OK (0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::FindBaudRate(bool* pStop, int iTimeout, int iFrameSupport, FEDM_CB_FIND_BAUDRATE cbFct, void* pAny)
{
#ifdef _FEDM_COM_SUPPORT
	int		iBack = 0;
	int		iErr = 0;
	int		iPortHnd = 0;
	int		iPortNr = 0;
	char	cPortHnd[11];
	char	cTimeout[8];
	char	cOldBaud[8];
	char	cOldFrame[8];
	char	cOldTimeout[8];
	char	cPortNr[8];
	char*	baud[]	= { "921600", "460800", "230400", "115200", "57600", "38400", "19200", "9600", "4800" };
	char*	frame[] = { "8E1", "8N1", "8O1" };

	size_t uiNoOfBaudrates = sizeof(baud)/sizeof(baud[0]);

	if( m_iReaderHnd == 0 )
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_NO_READER_HANDLE));
		return FEDM_ERROR_NO_READER_HANDLE;
	}

	// get port handle from reader object
	FEDM_CHK1(iErr, FEISC_GetReaderPara( m_iReaderHnd, "PortHnd", cPortHnd ));

	iPortHnd = atoi( cPortHnd );
	if( iPortHnd == 0 )
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_NO_PORT_HANDLE));
		return FEDM_ERROR_NO_PORT_HANDLE;
	}

	if(FEDM_IS_USBPORT(iPortHnd) || FEDM_IS_TCPPORT(iPortHnd))
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED_PORT_DRIVER));
		return FEDM_ERROR_UNSUPPORTED_PORT_DRIVER;
	}

	if(iTimeout < 0 || iTimeout > 99999)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_PARAMETER));
		return FEDM_ERROR_PARAMETER;
	}

	sprintf(cTimeout, "%d", iTimeout);

#if !defined(_FEDM_SUPPORT_SLINK)
	LPFN_FECOM_GET_PORT_PARA lpfnGetPortPara = (LPFN_FECOM_GET_PORT_PARA)GetFeComFunction(FECOM_GET_PORT_PARA);
	LPFN_FECOM_SET_PORT_PARA lpfnSetPortPara = (LPFN_FECOM_SET_PORT_PARA)GetFeComFunction(FECOM_SET_PORT_PARA);

	FEDM_CHK5(lpfnGetPortPara);
	FEDM_CHK5(lpfnSetPortPara);

	// save actual baudrate
	FEDM_CHK1(iErr, lpfnGetPortPara( iPortHnd, "baud", cOldBaud ));
	// save actual frame
	FEDM_CHK1(iErr, lpfnGetPortPara( iPortHnd, "frame", cOldFrame ));
	// save actual timeout
	FEDM_CHK1(iErr, lpfnGetPortPara( iPortHnd, "timeout", cOldTimeout ));
	// get the port number
	FEDM_CHK1(iErr, lpfnGetPortPara( iPortHnd, "PortNr", cPortNr ));

	// reduce timeout
	FEDM_CHK1(iErr, lpfnSetPortPara( iPortHnd, "timeout", cTimeout ));
#else
	// save actual baudrate
	FEDM_CHK1(iErr, FECOM_GetPortPara( iPortHnd, "baud", cOldBaud ));
	// save actual frame
	FEDM_CHK1(iErr, FECOM_GetPortPara( iPortHnd, "frame", cOldFrame ));
	// save actual timeout
	FEDM_CHK1(iErr, FECOM_GetPortPara( iPortHnd, "timeout", cOldTimeout ));
	// get the port number
	FEDM_CHK1(iErr, FECOM_GetPortPara( iPortHnd, "PortNr", cPortNr ));

	// reduce timeout
	FEDM_CHK1(iErr, FECOM_SetPortPara( iPortHnd, "timeout", cTimeout ));
#endif

	sscanf(cPortNr, "%d", &iPortNr);

	switch(iFrameSupport)
	{
	case 0:
		break; // use the preset and change after first loop
	case FEDM_PRT_FRAME_STANDARD:
		SetProtocolFrameSupport(FEDM_PRT_FRAME_STANDARD);
		break;
	case FEDM_PRT_FRAME_ADVANCED:
		SetProtocolFrameSupport(FEDM_PRT_FRAME_ADVANCED);
		break;
	case 3:
		SetProtocolFrameSupport(FEDM_PRT_FRAME_STANDARD);
		break;
	}

	for( unsigned int k=0; k<2; k++) // try all protocol frame support (standard, advanced)
	{
		for( unsigned int j=0; j<3; j++ )	// try all frames
		{
			if(pStop!=NULL && *pStop==true)
				break;

			// set frame im Porttreiber setzen
#if !defined(_FEDM_SUPPORT_SLINK)
			FEDM_CHK1(iErr, lpfnSetPortPara( iPortHnd, "frame", frame[j] ));
#else
			FEDM_CHK1(iErr, FECOM_SetPortPara( iPortHnd, "frame", frame[j] ));
#endif

			for( unsigned int i=0; i<uiNoOfBaudrates; i++ )	// try all baudrates
			{
				if(pStop!=NULL && *pStop==true)
					break;

				// set baudrate in port driver
#if !defined(_FEDM_SUPPORT_SLINK)
				iErr = lpfnSetPortPara( iPortHnd, "baud", baud[i] );
				if(iErr) // if baudrate not available, continue with next baudrate
					continue;
#else
				iErr = FECOM_SetPortPara( iPortHnd, "baud", baud[i] );
				if(iErr) // if baudrate not available, continue with next baudrate
					continue;
#endif

				if(iFrameSupport == 3) // alternating protocol frames
				{
					for( unsigned int h=0; h<2; h++ ) // try both protocol frames
					{
						// optional message to user application
						if(cbFct != NULL)
							cbFct(pAny, iPortNr, m_ucBusAdr, baud[i], frame[j], GetProtocolFrameSupport());

						// send test protocol [0x52]
						iBack = SendProtocol(0x52);
						if( iBack == 0 ||		// OK
							iBack == 0x19 )		// crypto processing error
						{
							// restore timeout
#if !defined(_FEDM_SUPPORT_SLINK)
							FEDM_CHK1(iErr, lpfnSetPortPara( iPortHnd, "timeout", cOldTimeout ));
#else
							FEDM_CHK1(iErr, FECOM_SetPortPara( iPortHnd, "timeout", cOldTimeout ));
#endif
							
							return iBack;
						}

						if(GetProtocolFrameSupport() == FEDM_PRT_FRAME_STANDARD)
							SetProtocolFrameSupport(FEDM_PRT_FRAME_ADVANCED);
						else
							SetProtocolFrameSupport(FEDM_PRT_FRAME_STANDARD);

					} // for( int h=0; h<2; h++ ) // try both protocol frames
				}
				else
				{
					// optional message to user application
					if(cbFct != NULL)
						cbFct(pAny, iPortNr, m_ucBusAdr, baud[i], frame[j], GetProtocolFrameSupport());

					// send test protocol [0x52]
					iBack = SendProtocol(0x52);
					if( iBack == 0 ||		// OK
						iBack == 0x19 )		// crypto processing error
					{
						// restore timeout
#if !defined(_FEDM_SUPPORT_SLINK)
						FEDM_CHK1(iErr, lpfnSetPortPara( iPortHnd, "timeout", cOldTimeout ));
#else
						FEDM_CHK1(iErr, FECOM_SetPortPara( iPortHnd, "timeout", cOldTimeout ));
#endif
						
						return iBack;
					}
				}
			}

		} // for( int j=0; j<3; j++ )	// try all frames (8E1, 8N1, 8O1)

		if(iFrameSupport == 0)
		{
			if(GetProtocolFrameSupport() == FEDM_PRT_FRAME_STANDARD)
				SetProtocolFrameSupport(FEDM_PRT_FRAME_ADVANCED);
			else
				SetProtocolFrameSupport(FEDM_PRT_FRAME_STANDARD);
		}
		else if(iFrameSupport == 3) // alternating protocol frames
		{
			break;
		}
	
	} // for( int k=0; k<2; k++) // try all protocol frame support (standard, advanced)

#if !defined(_FEDM_SUPPORT_SLINK)
	// restore old baudrate
	FEDM_CHK1(iErr, lpfnSetPortPara( iPortHnd, "baud", cOldBaud ));
	// restore old frame
	FEDM_CHK1(iErr, lpfnSetPortPara( iPortHnd, "frame", cOldFrame ));
	// restore timeout
	FEDM_CHK1(iErr, lpfnSetPortPara( iPortHnd, "timeout", cOldTimeout ));
#else
	// restore old baudrate
	FEDM_CHK1(iErr, FECOM_SetPortPara( iPortHnd, "baud", cOldBaud ));
	// restore old frame
	FEDM_CHK1(iErr, FECOM_SetPortPara( iPortHnd, "frame", cOldFrame ));
	// restore timeout
	FEDM_CHK1(iErr, FECOM_SetPortPara( iPortHnd, "timeout", cOldTimeout ));
#endif

	return FEDM_ERROR_NO_READER_FOUND;
#else
	return FEDM_ERROR_UNSUPPORTED;
#endif
}


#ifdef _FEDM_USB_SUPPORT
void FEDM_ISCReader::HandleUsbEnumeration()
{
	if(! FEDM_IS_USBPORT(GetPortHnd()))
		return;

#if !defined(_FEDM_SUPPORT_SLINK)
	LPFN_FEUSB_IS_DEVICE_PRESENT lpfnIsDevicePresent = (LPFN_FEUSB_IS_DEVICE_PRESENT)GetFeUsbFunction(FEUSB_IS_DEVICE_PRESENT);
	if(lpfnIsDevicePresent==NULL)
		return;
#endif

	FEDM_SLEEP(FE_1000MS);
	FEDM_SLEEP(FE_500MS);

	// reader enumerates new -> we must wait for presence event
	int iWaitCnt = 20;
	while(iWaitCnt > 0)
	{
#if !defined(_FEDM_SUPPORT_SLINK)
		if(lpfnIsDevicePresent(GetPortHnd()))
			break;
#else
		if(FEUSB_IsDevicePresent(GetPortHnd()))
			break;
#endif
		//TRACE("FEDM_ISCReader::HandleUsbEnumeration()\n");
		FEDM_SLEEP(FE_500MS);

		iWaitCnt--;
	}
}
#endif

//////////////////////////////////////////////////////////////////////
// miscellaneous functions
//////////////////////////////////////////////////////////////////////


/***************************************************************************
  Begin        		:	08.06.2000 / M. Hultsch
  Version       	:	01.06.00 / 23.09.2002 / M. Hultsch

  Function			:	returns the last communication protocol

  Parameters		:	sID		-	parameter id
						sProt	-	pointer to buffer for string

  Return value		:	status byte (>=0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::GetLastProt(char* sID, char* sProt)
{
	FEDM_CHK3(sID);
	FEDM_CHK3(sProt);

	int		iErr;
	unsigned char	ucBuf[1024];

	memset(ucBuf, '\0', 1024);

	// no handle -> no communication
	if( m_iReaderHnd == 0 )
		FEDM_RETURN(FEDM_ERROR_NO_READER_HANDLE);

	// make upper case
	char ID[128];
	memset(ID, 0, 128);
	for(int i=0; i<(int)strlen(sID); ++i)
		ID[i] = (char)toupper(sID[i]);

	
	if( strcmp(ID, "SEND")==0 )		// last send protocol
	{
		FEDM_CHK2(iErr, FEISC_GetLastSendProt(m_iReaderHnd, ucBuf, 1));
		
		strcpy(sProt, (const char*)&ucBuf[0]);
		FEDM_RETURN(FEDM_OK);
	}
	else if(strcmp(ID, "SENDSTR")==0)		// last send protocol with date and time
	{
		FEDM_CHK2(iErr, FEISC_GetReaderPara(m_iReaderHnd, "SENDSTR", (char*)ucBuf));

		strcpy(sProt, (const char*)&ucBuf[0]);
		FEDM_RETURN(FEDM_OK);
	}
	else if(strcmp(ID, "REC")==0)	// last receive protocol
	{
		FEDM_CHK2(iErr, FEISC_GetLastRecProt(m_iReaderHnd, ucBuf, 1));

		strcpy(sProt, (const char*)&ucBuf[0]);
		FEDM_RETURN(FEDM_OK);
	}
	else if(strcmp(ID, "RECSTR")==0)	// last receive protocol with date and time
	{
		FEDM_CHK2(iErr, FEISC_GetReaderPara(m_iReaderHnd, "RECSTR", (char*)ucBuf));
		
		strcpy(sProt, (const char*)&ucBuf[0]);
		FEDM_RETURN(FEDM_OK);
	}

	FEDM_RETURN(FEDM_ERROR_PARAMETER);
}


/***************************************************************************
  Begin        		:	22.11.2000 / M. Hultsch
  Version       	:	01.00.00 / 22.11.2000 / M. Hultsch

  Function			:	returns the last protocol status

  Parameters		:	-

  Return value		:	status byte (>=0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::GetLastStatus()
{
	unsigned char	ucStatus = 0;
	int		iErr;
	
	FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_LAST_STATE, &ucStatus));

	return (int)ucStatus;
}


/***************************************************************************
  Begin        		:	22.11.2000 / M. Hultsch
  Version       	:	01.03.00 / 09.07.2001 / M. Hultsch
						- error correction
  
						01.00.00 / 22.11.2000 / M. Hultsch

  Function			:	returns the last error code

  Parameters		:	-

  Return value		:	error code (<0)

***************************************************************************/
int FEDM_ISCReader::GetLastError()
{
	int	iErr;
	int iErrorCode = 0;
	
	FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_LAST_ERROR, (unsigned int*)&iErrorCode));

	if(iErr < 0)
		return iErr;
	else
		return iErrorCode;
}


/***************************************************************************
  Begin        		:	25.02.2009 / M. Hultsch
  Version       	:	03.00.05 / 25.02.2009 / M. Hultsch

  Function			:	return an error text for error code

  Parameters		:	int iErrCode	-	error code

  Return value		:	pointer to char with error text
***************************************************************************/
char* FEDM_ISCReader::GetErrorText(int iErrorCode)
{
	static char cMsgText[256];
	memset(cMsgText, 0, 256);

	this->GetErrorText(cMsgText, iErrorCode);
	
	return cMsgText;
}

/***************************************************************************
  Begin        		:	10.09.2000 / M. Hultsch
  Version       	:	01.09.06 / 01.04.2004 / M. Hultsch

  Function			:	return an error text for error code

  Parameters		:	char* sTxt		-	pointer to text buffer
						int iErrCode	-	error code

  Return value		:	OK (0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::GetErrorText(char* cErrorText, int iErrorCode)
{
	FEDM_CHK3(cErrorText);

	if(iErrorCode == 0)
	{
		strcpy(cErrorText, "OK");
		return FEDM_OK;
	}
	else if(iErrorCode > -1000 && iErrorCode <= -100)
	{
		// errors from this object
		return FEDM_Base::GetErrorText(cErrorText, iErrorCode);
	}
	else if( iErrorCode > -1100 && iErrorCode <= -1000 )
	{
#ifdef _FEDM_COM_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
		LPFN_FECOM_GET_ERROR_TEXT lpfn = (LPFN_FECOM_GET_ERROR_TEXT)GetFeComFunction(FECOM_GET_ERROR_TEXT);
		FEDM_CHK3(lpfn);

		// errors from fecom library
		return lpfn(iErrorCode, cErrorText);
#else
		// errors from fecom library
		return FECOM_GetErrorText(iErrorCode, cErrorText);
#endif
#else
		return FEDM_ERROR_UNSUPPORTED;
#endif
	}
	else if( iErrorCode > -1200 && iErrorCode <= -1100 )
	{
#ifdef _FEDM_USB_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
		LPFN_FEUSB_GET_ERROR_TEXT lpfn = (LPFN_FEUSB_GET_ERROR_TEXT)GetFeUsbFunction(FEUSB_GET_ERROR_TEXT);
		FEDM_CHK3(lpfn);

		// errors from feusb library
		return lpfn(iErrorCode, cErrorText);
#else
		// errors from feusb library
		return FEUSB_GetErrorText(iErrorCode, cErrorText);
#endif
#else
		return FEDM_ERROR_UNSUPPORTED;
#endif
	}
	else if( iErrorCode > -1300 && iErrorCode <= -1200 )
	{
#ifdef _FEDM_TCP_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
		LPFN_FETCP_GET_ERROR_TEXT lpfn = (LPFN_FETCP_GET_ERROR_TEXT)GetFeTcpFunction(FETCP_GET_ERROR_TEXT);
		FEDM_CHK3(lpfn);

		// errors from fetcp library
		return lpfn(iErrorCode, cErrorText);
#else
		// errors from fetcp library
		return FETCP_GetErrorText(iErrorCode, cErrorText);
#endif
#else
		return FEDM_ERROR_UNSUPPORTED;
#endif
	}
	else if(iErrorCode > -4100 && iErrorCode < -4000)
	{
		// errors from feisc library
		return FEISC_GetErrorText(iErrorCode, cErrorText);
	}
#ifdef _FEDM_FU_SUPPORT
	else if(iErrorCode > -4200 && iErrorCode < -4100)
	{
		// errors from fefu library
		return FEFU_GetErrorText(iErrorCode, cErrorText);
	}
#endif
#ifdef _FEDM_TAG_HANDLER
  #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
	else if(iErrorCode > -4300 && iErrorCode < -4200)
	{
		// errors from fetcl library
		return FETCL_GetErrorText(iErrorCode, cErrorText);
	}
  #endif
#endif
	else if(iErrorCode >= (int)0xE0000001 && iErrorCode <= (int)0xE8001000)
	{
#ifdef _FEDM_USB_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
		LPFN_FEUSB_GET_ERROR_TEXT lpfn = (LPFN_FEUSB_GET_ERROR_TEXT)GetFeUsbFunction(FEUSB_GET_ERROR_TEXT);
		FEDM_CHK3(lpfn);

		// error codes from USB Kernel-Driver OBIDUSB.SYS
		return lpfn(iErrorCode, cErrorText);
#else
		// error codes from USB Kernel-Driver OBIDUSB.SYS
		return FEUSB_GetErrorText(iErrorCode, cErrorText);
#endif
#else
		return FEDM_ERROR_UNSUPPORTED;
#endif
	}
	else
	{
		FEDM_Base::GetErrorText(cErrorText, iErrorCode);
	}

	return FEDM_ERROR_UNKNOWN_ERROR_CODE;
}



/***************************************************************************
  Begin        		:	25.02.2009 / M. Hultsch
  Version       	:	03.00.05 / 25.02.2009 / M. Hultsch

  Function			:	return a status text for status byte

  Parameters		:	unsigned char ucStatus	-	status byte

  Return value		:	pointer to char with error text
***************************************************************************/
char* FEDM_ISCReader::GetStatusText(unsigned char ucStatus)
{
	static char cMsgText[256];
	memset(cMsgText, 0, 256);

	this->GetStatusText(cMsgText, ucStatus);
	
	return cMsgText;
}


/***************************************************************************
  Begin        		:	05.03.2001 / M. Hultsch
  Version       	:	01.01.00 / 05.03.2001 / M. Hultsch


  Function			:	return a status text for status byte

  Parameters		:	char* sTxt		-	pointer to text buffer
						unsigned char ucStatus	-	status byte

  Return value		:	OK (0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::GetStatusText(char* cStatusText, unsigned char ucStatus)
{
	FEDM_CHK3(cStatusText);

	// status text from feisc library
	return FEISC_GetStatusText(ucStatus, cStatusText);
}




//#####################################################################################
// access functions
//#####################################################################################
	
// methods for getting command parameter values


int FEDM_ISCReader::GetCommandPara( string sParaName, bool* Data )
{
	return GetData(sParaName.c_str(), Data);
}

#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_ISCReader::GetCommandPara( string sParaName, BOOL* Data )
{
	return GetData(sParaName.c_str(), Data);
}
#endif

int FEDM_ISCReader::GetCommandPara( string sParaName, unsigned char* Data )
{
	return GetData(sParaName.c_str(), Data);
}

int FEDM_ISCReader::GetCommandPara( string sParaName, unsigned char* Data, int DataLen )
{
	return GetData(sParaName.c_str(), Data, DataLen);
}

int FEDM_ISCReader::GetCommandPara( string sParaName, unsigned int* Data )
{
	return GetData(sParaName.c_str(), Data);
}

int FEDM_ISCReader::GetCommandPara( string sParaName, __int64* Data )
{
	return GetData(sParaName.c_str(), Data);
}

#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_ISCReader::GetCommandPara( string sParaName, CString Data )
{
	return GetData(sParaName.c_str(), Data);
}
#endif

int FEDM_ISCReader::GetCommandPara( string sParaName, string Data )
{
	return GetData(sParaName.c_str(), Data);
}

int FEDM_ISCReader::GetCommandPara( string sParaName, char* Data, int DataLen )
{
	return GetData(sParaName.c_str(), Data, DataLen);
}


// methods for setting command parameter values

int FEDM_ISCReader::SetCommandPara( string sParaName, bool Data )
{
	return SetData(sParaName.c_str(), Data);
}

#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_ISCReader::SetCommandPara( string sParaName, BOOL Data )
{
	return SetData(sParaName.c_str(), Data);
}
#endif

int FEDM_ISCReader::SetCommandPara( string sParaName, unsigned char Data )
{
	return SetData(sParaName.c_str(), Data);
}

int FEDM_ISCReader::SetCommandPara( string sParaName, unsigned char* Data, int DataLen )
{
	return SetData(sParaName.c_str(), Data, DataLen);
}

int FEDM_ISCReader::SetCommandPara( string sParaName, unsigned int Data )
{
	return SetData(sParaName.c_str(), Data);
}

int FEDM_ISCReader::SetCommandPara( string sParaName, __int64 Data )
{
	return SetData(sParaName.c_str(), Data);
}

#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_ISCReader::SetCommandPara( string sParaName, CString Data )
{
	return SetData(sParaName.c_str(), Data);
}
#endif

int FEDM_ISCReader::SetCommandPara( string sParaName, string Data )
{
	return SetData(sParaName.c_str(), Data);
}

int FEDM_ISCReader::SetCommandPara( string sParaName, char* Data, int DataLen )
{
	return SetData(sParaName.c_str(), Data, DataLen);
}


/***************************************************************************
  Begin        	:	19.08.2008 / M. Hultsch
  Version       :	03.00.00 / 19.08.2008 / M. Hultsch

  Function		:	test a namespace for using for the reader type

  Parameter		:	string sParaName	-	parameter identifier

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::TestConfigPara(string sParaName)
{
	FEDM_CHK8(sParaName.size());

	FEDM_MAP_ACCESS_ID_ITOR		itor;

	itor = m_mapAccessID.find(sParaName);
	if(itor == m_mapAccessID.end())
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED_NAMESPACE);

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	15.08.2008 / M. Hultsch
  Version       :	03.00.00 / 20.08.2008 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	string sParaName	-	parameter identifier
					bool* Data		-	pointer to value
					bool bEEPROM	-	true : from EEPROM
										false: from RAM

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::GetConfigPara(string sParaName, bool* Data, bool bEEPROM)
{
	FEDM_CHK8(sParaName.size());
	FEDM_CHK3(Data);

	int iErr = 0;
	string sAccessID;

	FEDM_CHK1(iErr, GetAccessID(sParaName, sAccessID, bEEPROM));

	return FEDM_DataBase::GetData(sAccessID.c_str(), Data);
}

/***************************************************************************
  Begin        	:	15.08.2008 / M. Hultsch
  Version       :	03.00.00 / 20.08.2008 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	string sParaName	-	parameter identifier
					BOOL* Data		-	pointer to value
					bool bEEPROM	-	true : from EEPROM
										false: from RAM

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_ISCReader::GetConfigPara(string sParaName, BOOL* Data, bool bEEPROM)
{
	FEDM_CHK8(sParaName.size());
	FEDM_CHK3(Data);

	int iErr = 0;
	string sAccessID;

	FEDM_CHK1(iErr, GetAccessID(sParaName, sAccessID, bEEPROM));

	return FEDM_DataBase::GetData(sAccessID.c_str(), Data);
}
#endif

/***************************************************************************
  Begin        	:	15.08.2008 / M. Hultsch
  Version       :	03.00.00 / 20.08.2008 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	string sParaName		-	parameter identifier
					unsigned char* Data		-	pointer to value
					bool bEEPROM			-	true : from EEPROM
												false: from RAM

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::GetConfigPara(string sParaName, unsigned char* Data, bool bEEPROM)
{
	FEDM_CHK8(sParaName.size());
	FEDM_CHK3(Data);

	int iErr = 0;
	string sAccessID;

	FEDM_CHK1(iErr, GetAccessID(sParaName, sAccessID, bEEPROM));

	return FEDM_DataBase::GetData(sAccessID.c_str(), Data);
}

/***************************************************************************
  Begin        	:	15.08.2008 / M. Hultsch
  Version       :	03.00.00 / 20.08.2008 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	string sParaName		-	parameter identifier
					unsigned char* Data		-	pointer to value
					int Cnt					-	length of Data
					bool bEEPROM			-	true : from EEPROM
												false: from RAM

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::GetConfigPara(string sParaName, unsigned char* Data, int Cnt, bool bEEPROM)
{
	FEDM_CHK8(sParaName.size());
	FEDM_CHK3(Data);

	int iErr = 0;
	string sAccessID;

	FEDM_CHK1(iErr, GetAccessID(sParaName, sAccessID, bEEPROM));

	return FEDM_DataBase::GetData(sAccessID.c_str(), Data, Cnt);
}

/***************************************************************************
  Begin        	:	15.08.2008 / M. Hultsch
  Version       :	03.00.00 / 20.08.2008 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	string sParaName		-	parameter identifier
					unsigned int* Data		-	pointer to value
					bool bEEPROM			-	true : from EEPROM
												false: from RAM

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::GetConfigPara(string sParaName, unsigned int* Data, bool bEEPROM)
{
	FEDM_CHK8(sParaName.size());
	FEDM_CHK3(Data);

	int iErr = 0;
	string sAccessID;

	FEDM_CHK1(iErr, GetAccessID(sParaName, sAccessID, bEEPROM));

	return FEDM_DataBase::GetData(sAccessID.c_str(), Data);
}

/***************************************************************************
  Begin        	:	15.08.2008 / M. Hultsch
  Version       :	03.00.00 / 20.08.2008 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	string sParaName	-	parameter identifier
					__int64* Data		-	pointer to value
					bool bEEPROM		-	true : from EEPROM
											false: from RAM

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::GetConfigPara(string sParaName, __int64* Data, bool bEEPROM)
{
	FEDM_CHK8(sParaName.size());
	FEDM_CHK3(Data);

	int iErr = 0;
	string sAccessID;

	FEDM_CHK1(iErr, GetAccessID(sParaName, sAccessID, bEEPROM));

	return FEDM_DataBase::GetData(sAccessID.c_str(), Data);
}

/***************************************************************************
  Begin        	:	15.08.2008 / M. Hultsch
  Version       :	04.00.06 / 01.12.2011 / M. Hultsch
					- workaround for LRU3000

					03.00.00 / 20.08.2008 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	string sParaName	-	parameter identifier
					CString& Data	-	reference of value
					bool bEEPROM	-	true : from EEPROM
										false: from RAM

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_ISCReader::GetConfigPara(string sParaName, CString& Data, bool bEEPROM)
{
	FEDM_CHK8(sParaName.size());
	FEDM_CHK3(&Data);

	int iErr = 0;
	string sAccessID;

	FEDM_CHK1(iErr, GetAccessID(sParaName, sAccessID, bEEPROM));

	// workaround for selection masks, stored in RFC of LRU3000
	// get of new mask must be split into two steps
	if(m_uiReaderType == FEDM_ISC_TYPE_ISCLRU3000)
	{
		if( sParaName.find("Transponder.UHF.EPC_Class1Gen2.SelectionMask.No1.Mask") != string::npos ||
			sParaName.find("Transponder.UHF.EPC_Class1Gen2.SelectionMask.No2.Mask") != string::npos ||
			sParaName.find("Transponder.UHF.EPC_Class1Gen2.SelectionMask.No3.Mask") != string::npos )
		{
			char cID[32];
			int FamID=0, MemID=0, Block=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
			// extract access parameters
			sscanf( sAccessID.c_str(), "%d %d %d %d %d %d %d", &FamID, &MemID, &Block, &BytePos, &ByteCnt, &BitPos, &BitCnt );

			CString sData1;
			CString sData2;

			// build new access id for 1st step
			sprintf(cID, "%02d %02d %02d %02d %02d %02d %02d", FamID, MemID, Block, BytePos, 10, BitPos, BitCnt);
			int back = FEDM_DataBase::GetData(cID, sData1);
			if(back < 0)
				return back;

			// build new access id for 2nd step
			sprintf(cID, "%02d %02d %02d %02d %02d %02d %02d", FamID, MemID, Block+1, 0, 14, BitPos, BitCnt);
			back = FEDM_DataBase::GetData(cID, sData2);
			if(back < 0)
				return back;

			Data = sData1 + sData2;
			return 0;
		}
	}

	return FEDM_DataBase::GetData(sAccessID.c_str(), Data);
}
#endif

/***************************************************************************
  Begin        	:	15.08.2008 / M. Hultsch
  Version       :	04.00.06 / 01.12.2011 / M. Hultsch
					- workaround for LRU3000

					03.00.00 / 20.08.2008 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	string sParaName	-	parameter identifier
					string& Data	-	reference of value
					bool bEEPROM	-	true : from EEPROM
										false: from RAM

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::GetConfigPara(string sParaName, string& Data, bool bEEPROM)
{
	FEDM_CHK8(sParaName.size());
	FEDM_CHK3(&Data);

	int iErr = 0;
	string sAccessID;

	FEDM_CHK1(iErr, GetAccessID(sParaName, sAccessID, bEEPROM));

	// workaround for selection masks, stored in RFC of LRU3000
	// get of new mask must be split into two steps
	if(m_uiReaderType == FEDM_ISC_TYPE_ISCLRU3000)
	{
		if( sParaName.find("Transponder.UHF.EPC_Class1Gen2.SelectionMask.No1.Mask") != string::npos ||
			sParaName.find("Transponder.UHF.EPC_Class1Gen2.SelectionMask.No2.Mask") != string::npos ||
			sParaName.find("Transponder.UHF.EPC_Class1Gen2.SelectionMask.No3.Mask") != string::npos )
		{
			char cID[32];
			int FamID=0, MemID=0, Block=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
			// extract access parameters
			sscanf( sAccessID.c_str(), "%d %d %d %d %d %d %d", &FamID, &MemID, &Block, &BytePos, &ByteCnt, &BitPos, &BitCnt );

			string sData1;
			string sData2;

			// build new access id for 1st step
			sprintf(cID, "%02d %02d %02d %02d %02d %02d %02d", FamID, MemID, Block, BytePos, 10, BitPos, BitCnt);
			int back = FEDM_DataBase::GetData(cID, sData1);
			if(back < 0)
				return back;

			// build new access id for 2nd step
			sprintf(cID, "%02d %02d %02d %02d %02d %02d %02d", FamID, MemID, Block+1, 0, 14, BitPos, BitCnt);
			back = FEDM_DataBase::GetData(cID, sData2);
			if(back < 0)
				return back;

			Data = sData1 + sData2;
			return 0;
		}
	}

	return FEDM_DataBase::GetData(sAccessID.c_str(), Data);
}

/***************************************************************************
  Begin        	:	15.08.2008 / M. Hultsch
  Version       :	04.00.06 / 01.12.2011 / M. Hultsch
					- workaround for LRU3000

					03.00.00 / 20.08.2008 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	string sParaName	-	parameter identifier
					char* Data		-	pointer to value
					int DataLen		-	length of Data
					bool bEEPROM	-	true : from EEPROM
										false: from RAM

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::GetConfigPara(string sParaName, char* Data, int DataLen, bool bEEPROM)
{
	FEDM_CHK8(sParaName.size());
	FEDM_CHK3(Data);

	int iErr = 0;
	string sAccessID;

	FEDM_CHK1(iErr, GetAccessID(sParaName, sAccessID, bEEPROM));

	// workaround for selection masks, stored in RFC of LRU3000
	// get of new mask must be split into two steps
	if(m_uiReaderType == FEDM_ISC_TYPE_ISCLRU3000)
	{
		if( sParaName.find("Transponder.UHF.EPC_Class1Gen2.SelectionMask.No1.Mask") != string::npos ||
			sParaName.find("Transponder.UHF.EPC_Class1Gen2.SelectionMask.No2.Mask") != string::npos ||
			sParaName.find("Transponder.UHF.EPC_Class1Gen2.SelectionMask.No3.Mask") != string::npos )
		{
			char cID[32];
			int FamID=0, MemID=0, Block=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
			// extract access parameters
			sscanf( sAccessID.c_str(), "%d %d %d %d %d %d %d", &FamID, &MemID, &Block, &BytePos, &ByteCnt, &BitPos, &BitCnt );

			char sData1[21];
			char sData2[29];

			// build new access id for 1st step
			sprintf(cID, "%02d %02d %02d %02d %02d %02d %02d", FamID, MemID, Block, BytePos, 10, BitPos, BitCnt);
			int back = FEDM_DataBase::GetData(cID, sData1, 20);
			if(back < 0)
				return back;

			sData1[20] = '\0';

			// build new access id for 2nd step
			sprintf(cID, "%02d %02d %02d %02d %02d %02d %02d", FamID, MemID, Block+1, 0, 14, BitPos, BitCnt);
			back = FEDM_DataBase::GetData(cID, sData2, 28);
			if(back < 0)
				return back;

			sData2[28] = '\0';

			memcpy(&Data[0], sData1, 20);
			memcpy(&Data[20], sData2, 28);
			Data[48] = '\0';
			return 0;
		}
	}

	return FEDM_DataBase::GetData(sAccessID.c_str(), Data, DataLen);
}

/***************************************************************************
  Begin        	:	15.08.2008 / M. Hultsch
  Version       :	03.00.00 / 20.08.2008 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	string sParaName	-	parameter identifier
					bool Data		-	value
					bool bEEPROM	-	true : from EEPROM
										false: from RAM

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::SetConfigPara( string sParaName, bool Data, bool bEEPROM )
{
	FEDM_CHK8(sParaName.size());

	int iErr = 0;
	string sAccessID;

	FEDM_CHK1(iErr, GetAccessID(sParaName, sAccessID, bEEPROM));

	return FEDM_DataBase::SetData(sAccessID.c_str(), Data);
}

/***************************************************************************
  Begin        	:	15.08.2008 / M. Hultsch
  Version       :	03.00.00 / 20.08.2008 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	string sParaName	-	parameter identifier
					BOOL Data		-	value
					bool bEEPROM	-	true : from EEPROM
										false: from RAM

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_ISCReader::SetConfigPara( string sParaName, BOOL Data, bool bEEPROM )
{
	FEDM_CHK8(sParaName.size());

	int iErr = 0;
	string sAccessID;

	FEDM_CHK1(iErr, GetAccessID(sParaName, sAccessID, bEEPROM));

	return FEDM_DataBase::SetData(sAccessID.c_str(), Data);
}
#endif

/***************************************************************************
  Begin        	:	15.08.2008 / M. Hultsch
  Version       :	03.00.00 / 20.08.2008 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	string sParaName		-	parameter identifier
					unsigned char Data		-	value
					bool bEEPROM			-	true : from EEPROM
												false: from RAM

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::SetConfigPara( string sParaName, unsigned char Data, bool bEEPROM )
{
	FEDM_CHK8(sParaName.size());

	int iErr = 0;
	string sAccessID;

	FEDM_CHK1(iErr, GetAccessID(sParaName, sAccessID, bEEPROM));

	return FEDM_DataBase::SetData(sAccessID.c_str(), Data);
}

/***************************************************************************
  Begin        	:	15.08.2008 / M. Hultsch
  Version       :	03.00.00 / 20.08.2008 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	string sParaName	-	parameter identifier
					unsigned char* Data	-	pointer to value
					int Cnt				-	length of Data
					bool bEEPROM		-	true : from EEPROM
											false: from RAM

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::SetConfigPara( string sParaName, unsigned char* Data, int Cnt, bool bEEPROM )
{
	FEDM_CHK8(sParaName.size());
	FEDM_CHK3(Data);

	int iErr = 0;
	string sAccessID;

	FEDM_CHK1(iErr, GetAccessID(sParaName, sAccessID, bEEPROM));

	return FEDM_DataBase::SetData(sAccessID.c_str(), Data, Cnt);
}

/***************************************************************************
  Begin        	:	15.08.2008 / M. Hultsch
  Version       :	03.00.00 / 20.08.2008 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	string sParaName	-	parameter identifier
					unsigned int Data	-	value
					bool bEEPROM		-	true : from EEPROM
											false: from RAM

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::SetConfigPara( string sParaName, unsigned int Data, bool bEEPROM )
{
	FEDM_CHK8(sParaName.size());

	int iErr = 0;
	string sAccessID;

	FEDM_CHK1(iErr, GetAccessID(sParaName, sAccessID, bEEPROM));

	return FEDM_DataBase::SetData(sAccessID.c_str(), Data);
}

/***************************************************************************
  Begin        	:	15.08.2008 / M. Hultsch
  Version       :	03.00.00 / 20.08.2008 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	string sParaName	-	parameter identifier
					__int64 Data	-	value
					bool bEEPROM	-	true : from EEPROM
										false: from RAM

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::SetConfigPara( string sParaName, __int64 Data, bool bEEPROM )
{
	FEDM_CHK8(sParaName.size());

	int iErr = 0;
	string sAccessID;

	FEDM_CHK1(iErr, GetAccessID(sParaName, sAccessID, bEEPROM));

	return FEDM_DataBase::SetData(sAccessID.c_str(), Data);
}

/***************************************************************************
  Begin        	:	15.08.2008 / M. Hultsch
  Version       :	04.00.06 / 01.12.2011 / M. Hultsch
					- workaround for LRU3000

					03.00.00 / 20.08.2008 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	string sParaName	-	parameter identifier
					CString Data	-	value
					bool bEEPROM	-	true : from EEPROM
										false: from RAM

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_ISCReader::SetConfigPara( string sParaName, CString Data, bool bEEPROM )
{
	FEDM_CHK8(sParaName.size());

	int iErr = 0;
	string sAccessID;

	FEDM_CHK1(iErr, GetAccessID(sParaName, sAccessID, bEEPROM));

	// workaround for selection masks, stored in RFC of LRU3000
	// set of new mask must be split into two steps
	if(m_uiReaderType == FEDM_ISC_TYPE_ISCLRU3000)
	{
		if( sParaName.find("Transponder.UHF.EPC_Class1Gen2.SelectionMask.No1.Mask") != string::npos ||
			sParaName.find("Transponder.UHF.EPC_Class1Gen2.SelectionMask.No2.Mask") != string::npos ||
			sParaName.find("Transponder.UHF.EPC_Class1Gen2.SelectionMask.No3.Mask") != string::npos )
		{
			if(Data.GetLength() > 20)
			{
				char cID[32];
				int FamID=0, MemID=0, Block=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
				// extract access parameters
				sscanf( sAccessID.c_str(), "%d %d %d %d %d %d %d", &FamID, &MemID, &Block, &BytePos, &ByteCnt, &BitPos, &BitCnt );

				CString sData1 = Data.Left(20);
				CString sData2 = Data.Mid(20);

				// build new access id for 1st step
				sprintf(cID, "%02d %02d %02d %02d %02d %02d %02d", FamID, MemID, Block, BytePos, 10, BitPos, BitCnt);
				int back = FEDM_DataBase::SetData(cID, sData1);
				if(back < 0)
					return back;

				// build new access id for 2nd step
				sprintf(cID, "%02d %02d %02d %02d %02d %02d %02d", FamID, MemID, Block+1, 0, 14, BitPos, BitCnt);
				return FEDM_DataBase::SetData(cID, sData2);
			}
		}
	}

	return FEDM_DataBase::SetData(sAccessID.c_str(), Data);
}
#endif

/***************************************************************************
  Begin        	:	15.08.2008 / M. Hultsch
  Version       :	04.00.06 / 01.12.2011 / M. Hultsch
					- workaround for LRU3000

					03.00.00 / 20.08.2008 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	string sParaName	-	parameter identifier
					string Data		-	value
					bool bEEPROM	-	true : from EEPROM
										false: from RAM

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::SetConfigPara( string sParaName, string Data, bool bEEPROM )
{
	FEDM_CHK8(sParaName.size());

	int iErr = 0;
	string sAccessID;

	FEDM_CHK1(iErr, GetAccessID(sParaName, sAccessID, bEEPROM));

	// workaround for selection masks, stored in RFC of LRU3000
	// set of new mask must be split into two steps
	if(m_uiReaderType == FEDM_ISC_TYPE_ISCLRU3000)
	{
		if( sParaName.find("Transponder.UHF.EPC_Class1Gen2.SelectionMask.No1.Mask") != string::npos ||
			sParaName.find("Transponder.UHF.EPC_Class1Gen2.SelectionMask.No2.Mask") != string::npos ||
			sParaName.find("Transponder.UHF.EPC_Class1Gen2.SelectionMask.No3.Mask") != string::npos )
		{
			if(Data.size() > 20)
			{
				char cID[32];
				int FamID=0, MemID=0, Block=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
				// extract access parameters
				sscanf( sAccessID.c_str(), "%d %d %d %d %d %d %d", &FamID, &MemID, &Block, &BytePos, &ByteCnt, &BitPos, &BitCnt );

				string sData1 = Data.substr(0, 20);
				string sData2 = Data.substr(20);

				// build new access id for 1st step
				sprintf(cID, "%02d %02d %02d %02d %02d %02d %02d", FamID, MemID, Block, BytePos, 10, BitPos, BitCnt);
				int back = FEDM_DataBase::SetData(cID, sData1);
				if(back < 0)
					return back;

				// build new access id for 2nd step
				sprintf(cID, "%02d %02d %02d %02d %02d %02d %02d", FamID, MemID, Block+1, 0, 14, BitPos, BitCnt);
				return FEDM_DataBase::SetData(cID, sData2);
			}
		}
	}

	return FEDM_DataBase::SetData(sAccessID.c_str(), Data);
}

/***************************************************************************
  Begin        	:	15.08.2008 / M. Hultsch
  Version       :	04.00.06 / 01.12.2011 / M. Hultsch
					- workaround for LRU3000

					03.00.00 / 20.08.2008 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	string sParaName	-	parameter identifier
					char* Data		-	pointer to value
					int DataLen		-	length of Data
					bool bEEPROM	-	true : from EEPROM
										false: from RAM

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::SetConfigPara( string sParaName, char* Data, int DataLen, bool bEEPROM )
{
	FEDM_CHK8(sParaName.size());
	FEDM_CHK3(Data);
	if(strlen(Data) != DataLen)
		return FEDM_ERROR_STRING_LENGTH;

	int iErr = 0;
	string sAccessID;

	FEDM_CHK1(iErr, GetAccessID(sParaName, sAccessID, bEEPROM));

	// workaround for selection masks, stored in RFC of LRU3000
	// set of new mask must be split into two steps
	if(m_uiReaderType == FEDM_ISC_TYPE_ISCLRU3000)
	{
		if( sParaName.find("Transponder.UHF.EPC_Class1Gen2.SelectionMask.No1.Mask") != string::npos ||
			sParaName.find("Transponder.UHF.EPC_Class1Gen2.SelectionMask.No2.Mask") != string::npos ||
			sParaName.find("Transponder.UHF.EPC_Class1Gen2.SelectionMask.No3.Mask") != string::npos )
		{
			if(DataLen > 20)
			{
				char cID[32];
				int FamID=0, MemID=0, Block=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
				// extract access parameters
				sscanf( sAccessID.c_str(), "%d %d %d %d %d %d %d", &FamID, &MemID, &Block, &BytePos, &ByteCnt, &BitPos, &BitCnt );

				char* sData1 = &Data[0];
				char* sData2 = &Data[20];

				// build new access id for 1st step
				sprintf(cID, "%02d %02d %02d %02d %02d %02d %02d", FamID, MemID, Block, BytePos, 10, BitPos, BitCnt);
				int back = FEDM_DataBase::SetData(cID, sData1, 20);
				if(back < 0)
					return back;

				// build new access id for 2nd step
				sprintf(cID, "%02d %02d %02d %02d %02d %02d %02d", FamID, MemID, Block+1, 0, 14, BitPos, BitCnt);
				return FEDM_DataBase::SetData(cID, sData2, 28);
			}
		}
	}

	return FEDM_DataBase::SetData(sAccessID.c_str(), Data, DataLen);
}


//////////////////////////////////////////////////////////////////////
// functions for data member access in iso-table
//////////////////////////////////////////////////////////////////////

/***************************************************************************
  Begin        		:	20.08.2003 / M. Hultsch
  Version       	:	03.03.00 / 21.02.2011 / M. Hultsch

  Function			:	get one data member from a table
						(only one byte)

  Parameters		:	int iIdx		-	index of table
						unsigned int uiTableID	-	identifies the table
						unsigned int uiDataID	-	identifies the table data
						bool* bData		-	pointer to data buffer

  Return value		:	0 or error code (<0)

***************************************************************************/
int FEDM_ISCReader::GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, bool* bData)
{
	FEDM_CHK3(bData);

	if(iIdx < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool*) - Index < 0");
		FEDM_RETURN(iIdx);
	}

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
		if(m_BRMTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool*) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_BRMTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool*) - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_IS_SNR:
			*bData = m_BRMTable[iIdx]->m_bIsSnr;
			break;

		case FEDM_ISC_DATA_IS_EPC:
			*bData = m_BRMTable[iIdx]->m_bIsEpc;
			break;

		case FEDM_ISC_DATA_IS_AFI:
			*bData = m_BRMTable[iIdx]->m_bIsAFI;
			break;

		case FEDM_ISC_DATA_IS_DB:
			*bData = m_BRMTable[iIdx]->m_bIsDB;
			break;

		case FEDM_ISC_DATA_IS_TIMER:
			*bData = m_BRMTable[iIdx]->m_bIsTimer;
			break;

		case FEDM_ISC_DATA_IS_DATE:
			*bData = m_BRMTable[iIdx]->m_bIsDate;
			break;

		case FEDM_ISC_DATA_IS_ANT_NR:
			*bData = m_BRMTable[iIdx]->m_bIsAntNr;
			break;

		case FEDM_ISC_DATA_IS_INPUT:
			*bData = m_BRMTable[iIdx]->m_bIsInput;
			break;

		case FEDM_ISC_DATA_IS_RSSI:
			*bData = m_BRMTable[iIdx]->m_bIsRSSI;
			break;

		case FEDM_ISC_DATA_IS_MAC_ADR:
			*bData = m_BRMTable[iIdx]->m_bIsMacAddr;
			break;

		case FEDM_ISC_DATA_TRTYPE:
		case FEDM_ISC_DATA_EPC_TYPE:
		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
		case FEDM_ISC_DATA_DB_ADR:
		case FEDM_ISC_DATA_DBN:
		case FEDM_ISC_DATA_ANT_NR:
		case FEDM_ISC_DATA_INPUT:
		case FEDM_ISC_DATA_STATE:
		case FEDM_ISC_DATA_BLOCK_SIZE:
		case FEDM_ISC_DATA_SNR:
		case FEDM_ISC_DATA_TIMER:
		case FEDM_ISC_DATA_RxDB:
		case FEDM_ISC_DATA_SNR_LEN:
		case FEDM_ISC_DATA_EPC:
		case FEDM_ISC_DATA_ANT_COUNT:
		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool*) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool*) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
			break;
		}
		break;
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool*) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool*) - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_IS_BLOCK_SIZE_SET:
			*bData = m_ISOTable[iIdx]->m_bIsBlockSizeSet;
			break;

		case FEDM_ISC_DATA_IS_SELECTED:
			*bData = m_ISOTable[iIdx]->m_bIsSelected;
			break;

		case FEDM_ISC_DATA_IS_SNR:
			*bData = m_ISOTable[iIdx]->m_bIsSnr;
			break;

		case FEDM_ISC_DATA_IS_EPC:
			*bData = m_ISOTable[iIdx]->m_bIsEpc;
			break;

		case FEDM_ISC_DATA_IS_AFI:
			*bData = m_ISOTable[iIdx]->m_bIsAFI;
			break;

		case FEDM_ISC_DATA_IS_DSFID:
		case FEDM_ISC_DATA_CHIP_ID:
			*bData = m_ISOTable[iIdx]->m_bIsSnr;
			break;

		case FEDM_ISC_DATA_IS_TRINFO:
			*bData = m_ISOTable[iIdx]->m_bIsSnr;
			break;

		case FEDM_ISC_DATA_IS_SYSINFO:
			*bData = m_ISOTable[iIdx]->m_bIsSysInfo;
			break;

		case FEDM_ISC_DATA_IS_ISO14443_4_INFO:
			*bData = m_ISOTable[iIdx]->m_bIsISO14443_4Info;
			break;

		case FEDM_ISC_DATA_IS_RSSI:
			*bData = m_ISOTable[iIdx]->m_bIsRSSI;
			break;

		case FEDM_ISC_DATA_TRTYPE:
		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
		case FEDM_ISC_DATA_AFI:
		case FEDM_ISC_DATA_DSFID:
		case FEDM_ISC_DATA_IC_REF:
		case FEDM_ISC_DATA_BLOCK_SIZE:
		case FEDM_ISC_DATA_TRINFO:
		case FEDM_ISC_DATA_OPTINFO:
		case FEDM_ISC_DATA_PROTOINFO:
		case FEDM_ISC_DATA_EPC:
		case FEDM_ISC_DATA_EPC_TYPE:
		case FEDM_ISC_DATA_EPC_HEADER:
		case FEDM_ISC_DATA_EPC_DOMAIN:
		case FEDM_ISC_DATA_EPC_OBJECT:
		case FEDM_ISC_DATA_EPC_SNR:
		case FEDM_ISC_DATA_MEM_SIZE:
		case FEDM_ISC_DATA_SNR:
		case FEDM_ISC_DATA_SNR_LEN:
		case FEDM_ISC_DATA_SEC_STATUS:
		case FEDM_ISC_DATA_TxDB:
		case FEDM_ISC_DATA_RxDB:
		case FEDM_ISC_DATA_TxCB:
		case FEDM_ISC_DATA_RxCB:
		case FEDM_ISC_DATA_RxDB_EPC_BANK:
		case FEDM_ISC_DATA_TxDB_EPC_BANK:
		case FEDM_ISC_DATA_RxDB_TID_BANK:
		case FEDM_ISC_DATA_TxDB_TID_BANK:
		case FEDM_ISC_DATA_RxDB_RES_BANK:
		case FEDM_ISC_DATA_TxDB_RES_BANK:
		case FEDM_ISC_DATA_ANT_COUNT:
		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool*) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool*) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
			break;
		}
		break;
	
	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool*) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool*) - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        		:	08.06.2000 / M. Hultsch
  Version       	:	03.03.00 / 21.02.2011 / M. Hultsch

  Function			:	get one data member from a table
						(only one byte)

  Parameters		:	int iIdx		-	index of table
						unsigned int uiTableID	-	identifies the table
						unsigned int uiDataID	-	identifies the table data
						unsigned char* ucData	-	pointer to data buffer

  Return value		:	0 or error code (<0)

***************************************************************************/
int FEDM_ISCReader::GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, unsigned char* ucData)
{
	FEDM_CHK3(ucData);

	if(iIdx < 0)
		FEDM_RETURN(iIdx);

	int iCnt;

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
		if(m_BRMTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_BRMTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*) - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_TRTYPE:
			*ucData = m_BRMTable[iIdx]->m_ucTrType;
			break;

		case FEDM_ISC_DATA_EPC_TYPE:
			*ucData = m_BRMTable[iIdx]->m_ucEpcType;
			break;

		case FEDM_ISC_DATA_TRINFO:
			*ucData = m_BRMTable[iIdx]->m_ucIDDT; // for ISO 14443
			break;

		case FEDM_ISC_DATA_AFI:
			*ucData = m_BRMTable[iIdx]->m_ucAFI;
			break;

		case FEDM_ISC_DATA_DSFID:
			*ucData = m_BRMTable[iIdx]->m_ucDsfID;
			break;

		case FEDM_ISC_DATA_DB_ADR:
			*ucData = m_BRMTable[iIdx]->m_ucDBAdr;
			break;

		case FEDM_ISC_DATA_BLOCK_SIZE:
			*ucData = m_BRMTable[iIdx]->m_ucBlockSize;
			break;

		case FEDM_ISC_DATA_ANT_NR:
			*ucData = m_BRMTable[iIdx]->m_ucAntNr;
			break;

		case FEDM_ISC_DATA_SNR_LEN:
			*ucData = m_BRMTable[iIdx]->m_ucSnrLen;
			break;

		case FEDM_ISC_DATA_INPUT:
			*ucData = m_BRMTable[iIdx]->m_ucInput;
			break;

		case FEDM_ISC_DATA_STATE:
			*ucData = m_BRMTable[iIdx]->m_ucState;
			break;

		case FEDM_ISC_DATA_ANT_COUNT:
			*ucData = m_BRMTable[iIdx]->m_ucAntCount;
			break;

		case FEDM_ISC_DATA_DBN:
		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
		case FEDM_ISC_DATA_SNR:
		case FEDM_ISC_DATA_TIMER:
		case FEDM_ISC_DATA_DATE:
		case FEDM_ISC_DATA_RxDB:
		case FEDM_ISC_DATA_EPC:
		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
		case FEDM_ISC_DATA_MAC_ADR:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
			break;
		}
		break;
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*) - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_TRTYPE:
			*ucData = m_ISOTable[iIdx]->m_ucTrType;
			break;

		case FEDM_ISC_DATA_SNR_LEN:
			*ucData = m_ISOTable[iIdx]->m_ucSnrLen;
			break;

		case FEDM_ISC_DATA_AFI:
			*ucData = m_ISOTable[iIdx]->m_ucAFI;
			break;

		case FEDM_ISC_DATA_DSFID:
			*ucData = m_ISOTable[iIdx]->m_ucDsfID;
			break;

		case FEDM_ISC_DATA_CHIP_ID:
			*ucData = m_ISOTable[iIdx]->m_ucChipID;
			break;

		case FEDM_ISC_DATA_IC_REF:
			*ucData = m_ISOTable[iIdx]->m_ucICRef;
			break;

		case FEDM_ISC_DATA_IS_SELECTED:
			*ucData = (unsigned char)m_ISOTable[iIdx]->m_bIsSelected;
			break;

		case FEDM_ISC_DATA_IS_ISO14443_4_INFO:
			*ucData = (unsigned char)m_ISOTable[iIdx]->m_bIsISO14443_4Info;
			break;

		case FEDM_ISC_DATA_IS_EPC:
			*ucData = (unsigned char)m_ISOTable[iIdx]->m_bIsEpc;
			break;

		case FEDM_ISC_DATA_BLOCK_SIZE:
			*ucData = m_ISOTable[iIdx]->m_ucBlockSize;
			break;

		case FEDM_ISC_DATA_IS_BLOCK_SIZE_SET:
			*ucData = (unsigned char)m_ISOTable[iIdx]->m_bIsBlockSizeSet;
			break;

		case FEDM_ISC_DATA_TRINFO:
			*ucData = m_ISOTable[iIdx]->m_ucTrInfo;
			break;

		case FEDM_ISC_DATA_OPTINFO:
			*ucData = m_ISOTable[iIdx]->m_ucOptInfo;
			break;

		case FEDM_ISC_DATA_PROTOINFO:
			*ucData = m_ISOTable[iIdx]->m_ucProtoInfo;
			break;

		case FEDM_ISC_DATA_EPC_TYPE:
			*ucData = m_ISOTable[iIdx]->m_ucEpcType;
			break;

		case FEDM_ISC_DATA_FSCI:
			*ucData = m_ISOTable[iIdx]->m_ucFSCI;
			break;

		case FEDM_ISC_DATA_FWI:
			*ucData = m_ISOTable[iIdx]->m_ucFWI;
			break;

		case FEDM_ISC_DATA_DSI:
			*ucData = m_ISOTable[iIdx]->m_ucDSI;
			break;

		case FEDM_ISC_DATA_DRI:
			*ucData = m_ISOTable[iIdx]->m_ucDRI;
			break;

		case FEDM_ISC_DATA_NAD:
			*ucData = m_ISOTable[iIdx]->m_ucNad;
			break;

		case FEDM_ISC_DATA_CID:
			*ucData = m_ISOTable[iIdx]->m_ucCid;
			break;

		case FEDM_ISC_DATA_MEM_SIZE_LEN:
			*ucData = m_ISOTable[iIdx]->m_ucMemSizeLen;
			break;

		case FEDM_ISC_DATA_ANT_COUNT:
			*ucData = m_ISOTable[iIdx]->m_ucAntCount;
			break;

		case FEDM_ISC_DATA_EPC:
		case FEDM_ISC_DATA_EPC_HEADER:
		case FEDM_ISC_DATA_EPC_DOMAIN:
		case FEDM_ISC_DATA_EPC_OBJECT:
		case FEDM_ISC_DATA_EPC_SNR:
		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
		case FEDM_ISC_DATA_MEM_SIZE:
		case FEDM_ISC_DATA_SNR:
		case FEDM_ISC_DATA_SEC_STATUS:
		case FEDM_ISC_DATA_TxDB:
		case FEDM_ISC_DATA_RxDB:
		case FEDM_ISC_DATA_TxCB:
		case FEDM_ISC_DATA_RxCB:
		case FEDM_ISC_DATA_RxDB_EPC_BANK:
		case FEDM_ISC_DATA_TxDB_EPC_BANK:
		case FEDM_ISC_DATA_RxDB_TID_BANK:
		case FEDM_ISC_DATA_TxDB_TID_BANK:
		case FEDM_ISC_DATA_RxDB_RES_BANK:
		case FEDM_ISC_DATA_TxDB_RES_BANK:
		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
			break;
		}
		break;
	
#if !defined(__BORLANDC__)
	case FEDM_ISC_CAD_TABLE:
		switch(uiDataID)
		{
		case FEDM_ISC_DATA_POWER_AVG:
			if(iIdx<0 || iIdx>=15)
				FEDM_RETURN(FEDM_ERROR_PARAMETER);

			*ucData = m_CADTable.m_ucPowerAvg[iIdx];
			break;

		case FEDM_ISC_DATA_POWER_PEAK:
			if(iIdx<0 || iIdx>=15)
				FEDM_RETURN(FEDM_ERROR_PARAMETER);

			*ucData = m_CADTable.m_ucPowerPeak[iIdx];
			break;

		case FEDM_ISC_DATA_HISTORY_ALLOC:
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		case FEDM_ISC_DATA_HISTORY_TX:
			// request of number of record sets
			iCnt = (int)m_CADTable.m_History.size();
			if(iIdx<0 || iIdx>=iCnt)
				FEDM_RETURN(FEDM_ERROR_PARAMETER);

			*ucData = m_CADTable.m_History[iIdx]->ucTxChannel;
			break;

		case FEDM_ISC_DATA_HISTORY_RX:
			// request of number of record sets
			iCnt = (int)m_CADTable.m_History.size();
			if(iIdx<0 || iIdx>=iCnt)
				FEDM_RETURN(FEDM_ERROR_PARAMETER);

			*ucData = m_CADTable.m_History[iIdx]->ucRxChannel;
			break;

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
#endif

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*) - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
		break;
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        		:	08.06.2000 / M. Hultsch
  Version       	:	03.03.00 / 21.02.2011 / M. Hultsch

  Function			:	get one data member from a table
						(one or more than one byte)

  Parameters		:	int iIdx		-	index of table
						unsigned int uiTableID	-	identifies the table
						unsigned int uiDataID	-	identifies the table data
						unsigned char* ucData	-	pointer to data buffer
						int iDataBufLen	-	length of ucData

  Return value		:	0 or error code (<0)

***************************************************************************/
int FEDM_ISCReader::GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, unsigned char* ucData, int iDataBufLen)
{
	FEDM_CHK3(ucData);

	if(iIdx < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - Index < 0");
		FEDM_RETURN(iIdx);
	}

	if(iDataBufLen == 0)
		return FEDM_OK; // nothing to do

	if(iDataBufLen < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
		FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
	}

	int i;
	int iCnt = 0;
	int iLen = 0;
	int iErr = 0;

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
		if(m_BRMTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_BRMTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_EPC:
		case FEDM_ISC_DATA_SNR:
			if(iDataBufLen < m_BRMTable[iIdx]->m_ucSnrLen)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}

			for(iCnt=0; iCnt<m_BRMTable[iIdx]->m_ucSnrLen; iCnt++)
				ucData[iCnt] = m_BRMTable[iIdx]->m_ucSnr[iCnt];
			break;

		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
			ucData[0] = m_BRMTable[iIdx]->m_ucClass1Gen2_PC[0];
			ucData[1] = m_BRMTable[iIdx]->m_ucClass1Gen2_PC[1];
			break;

		case FEDM_ISC_DATA_TRTYPE:
			ucData[0] = m_BRMTable[iIdx]->m_ucTrType;
			break;

		case FEDM_ISC_DATA_EPC_TYPE:
			ucData[0] = m_BRMTable[iIdx]->m_ucEpcType;
			break;

		case FEDM_ISC_DATA_TRINFO:
			ucData[0] = m_BRMTable[iIdx]->m_ucIDDT; // for ISO 14443
			break;

		case FEDM_ISC_DATA_AFI:
			ucData[0] = m_BRMTable[iIdx]->m_ucAFI;
			break;

		case FEDM_ISC_DATA_DSFID:
			ucData[0] = m_BRMTable[iIdx]->m_ucDsfID;
			break;

		case FEDM_ISC_DATA_TIMER:
			if(iDataBufLen < 4)
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);

			for(iCnt=0; iCnt<4; iCnt++)
				ucData[iCnt] = m_BRMTable[iIdx]->m_ucTimer[iCnt];
			break;

		case FEDM_ISC_DATA_DATE:
			if(iDataBufLen < 5)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}

			for(iCnt=0; iCnt<5; iCnt++)
				ucData[iCnt] = m_BRMTable[iIdx]->m_ucDate[iCnt];
			break;

		case FEDM_ISC_DATA_DB_ADR:
			ucData[0] = m_BRMTable[iIdx]->m_ucDBAdr;
			break;

		case FEDM_ISC_DATA_ANT_NR:
			ucData[0] = m_BRMTable[iIdx]->m_ucAntNr;
			break;

		case FEDM_ISC_DATA_SNR_LEN:
			ucData[0] = m_BRMTable[iIdx]->m_ucSnrLen;
			break;

		case FEDM_ISC_DATA_INPUT:
			ucData[0] = m_BRMTable[iIdx]->m_ucInput;
			break;

		case FEDM_ISC_DATA_STATE:
			ucData[0] = m_BRMTable[iIdx]->m_ucState;
			break;

		case FEDM_ISC_DATA_DBN:
			ucData[0] = ((m_BRMTable[iIdx]->m_uiDBN & 0xFF00) >> 8);
			ucData[1] =  (m_BRMTable[iIdx]->m_uiDBN & 0x00FF);
			break;

		case FEDM_ISC_DATA_BLOCK_SIZE:
			ucData[0] = m_BRMTable[iIdx]->m_ucBlockSize;
			break;

		case FEDM_ISC_DATA_RxDB:
			if(iDataBufLen < FEDM_ISC_BRM_TABLE_RxDB_SIZE)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}
			memcpy(ucData, &m_BRMTable[iIdx]->m_ucRxDB[0], FEDM_ISC_BRM_TABLE_RxDB_SIZE);
			break;

		case FEDM_ISC_DATA_ANT_COUNT:
			ucData[0] = m_BRMTable[iIdx]->m_ucAntCount;
			break;

		case FEDM_ISC_DATA_ANT_RSSI:
			if(m_BRMTable[iIdx]->m_ucAntCount > FEDM_ISC_BRM_TABLE_MAX_ANTENNA)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				return FEDM_ERROR_ARRAY_BOUNDARY;
			}

			ucData[iCnt++] = m_BRMTable[iIdx]->m_ucAntCount;
			for(i=0; i<m_BRMTable[iIdx]->m_ucAntCount; i++)
			{
				ucData[iCnt++] = m_BRMTable[iIdx]->m_ucAntNumber[i];
				ucData[iCnt++] = m_BRMTable[iIdx]->m_ucAntRSSI[i];
			}
			break;

		case FEDM_ISC_DATA_MAC_ADR:
			for(i=0; i<6; i++)
				ucData[iCnt++] = m_BRMTable[iIdx]->m_ucMacAddress[i];
			break;

		case FEDM_ISC_DATA_ANT_STATUS:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_SNR:
			if(iDataBufLen < m_ISOTable[iIdx]->m_ucSnrLen)
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);

			for(iCnt=0; iCnt<m_ISOTable[iIdx]->m_ucSnrLen; iCnt++)
				ucData[iCnt] = m_ISOTable[iIdx]->m_ucSnr[iCnt];
			break;

		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
			ucData[0] = m_ISOTable[iIdx]->m_ucClass1Gen2_PC[0];
			ucData[1] = m_ISOTable[iIdx]->m_ucClass1Gen2_PC[1];
			break;

		case FEDM_ISC_DATA_TRTYPE:
			ucData[0] = m_ISOTable[iIdx]->m_ucTrType;
			break;

		case FEDM_ISC_DATA_SNR_LEN:
			ucData[0] = m_ISOTable[iIdx]->m_ucSnrLen;
			break;

		case FEDM_ISC_DATA_AFI:
			ucData[0] = m_ISOTable[iIdx]->m_ucAFI;
			break;

		case FEDM_ISC_DATA_DSFID:
			ucData[0] = m_ISOTable[iIdx]->m_ucDsfID;
			break;

		case FEDM_ISC_DATA_CHIP_ID:
			ucData[0] = m_ISOTable[iIdx]->m_ucChipID;
			break;

		case FEDM_ISC_DATA_MEM_SIZE:
			if(iDataBufLen < 2)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}
			else if(iDataBufLen == 2)
			{
				iLen = 2;
			}
			else if(iDataBufLen > 2)
			{
				if(m_ISOTable[iIdx]->m_ucMemSizeLen == 2)
					iLen = 2;
				else
					iLen = 3;
			}

			for(iCnt=0; iCnt<iLen; iCnt++)
				ucData[iCnt] = m_ISOTable[iIdx]->m_ucMemSize[iCnt];
			break;

		case FEDM_ISC_DATA_IC_REF:
			ucData[0] = m_ISOTable[iIdx]->m_ucICRef;
			break;

		case FEDM_ISC_DATA_IS_SELECTED:
			ucData[0] = (unsigned char)m_ISOTable[iIdx]->m_bIsSelected;
			break;

		case FEDM_ISC_DATA_IS_ISO14443_4_INFO:
			ucData[0] = (unsigned char)m_ISOTable[iIdx]->m_bIsISO14443_4Info;
			break;

		case FEDM_ISC_DATA_IS_EPC:
			ucData[0] = (unsigned char)m_ISOTable[iIdx]->m_bIsEpc;
			break;

		case FEDM_ISC_DATA_BLOCK_SIZE:
			ucData[0] = m_ISOTable[iIdx]->m_ucBlockSize;
			break;

		case FEDM_ISC_DATA_IS_BLOCK_SIZE_SET:
			ucData[0] = (unsigned char)m_ISOTable[iIdx]->m_bIsBlockSizeSet;
			break;

		case FEDM_ISC_DATA_TRINFO:
			ucData[0] = m_ISOTable[iIdx]->m_ucTrInfo;
			break;

		case FEDM_ISC_DATA_OPTINFO:
			ucData[0] = m_ISOTable[iIdx]->m_ucOptInfo;
			break;

		case FEDM_ISC_DATA_PROTOINFO:
			ucData[0] = m_ISOTable[iIdx]->m_ucProtoInfo;
			break;

		case FEDM_ISC_DATA_EPC_TYPE:
			ucData[0] = m_ISOTable[iIdx]->m_ucEpcType;
			break;

		case FEDM_ISC_DATA_FSCI:
			ucData[0] = m_ISOTable[iIdx]->m_ucFSCI;
			break;

		case FEDM_ISC_DATA_FWI:
			ucData[0] = m_ISOTable[iIdx]->m_ucFWI;
			break;

		case FEDM_ISC_DATA_DSI:
			ucData[0] = m_ISOTable[iIdx]->m_ucDSI;
			break;

		case FEDM_ISC_DATA_DRI:
			ucData[0] = m_ISOTable[iIdx]->m_ucDRI;
			break;

		case FEDM_ISC_DATA_NAD:
			ucData[0] = m_ISOTable[iIdx]->m_ucNad;
			break;

		case FEDM_ISC_DATA_CID:
			ucData[0] = m_ISOTable[iIdx]->m_ucCid;
			break;

		case FEDM_ISC_DATA_EPC:
			switch(m_ISOTable[iIdx]->m_ucEpcType)
			{
			case FEDM_ISC_EPC_TYPE_1:
				if(iDataBufLen < 12)
				{
					FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
					FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
				}
				FEDM_CHK2(iErr, m_ISOTable[iIdx]->GetEpcRaw(ucData, 12));
				break;
			case FEDM_ISC_EPC_TYPE_2:
			case FEDM_ISC_EPC_TYPE_3:
			case FEDM_ISC_EPC_TYPE_4:
				if(iDataBufLen < 8)
				{
					FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
					FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
				}
				FEDM_CHK2(iErr, m_ISOTable[iIdx]->GetEpcRaw(ucData, 8));
				break;
			default:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_UNKNOWN_EPC_TYPE));
				FEDM_RETURN(FEDM_ERROR_UNKNOWN_EPC_TYPE);
			}

			break;

		case FEDM_ISC_DATA_ANT_RSSI:
			iCnt = 0;
			if(m_ISOTable[iIdx]->m_ucAntCount > FEDM_ISC_ISO_TABLE_MAX_ANTENNA)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				return FEDM_ERROR_ARRAY_BOUNDARY;
			}

			ucData[iCnt++] = m_ISOTable[iIdx]->m_ucAntCount;
			for(i=0; i<m_ISOTable[iIdx]->m_ucAntCount; i++)
			{
				ucData[iCnt++] = m_ISOTable[iIdx]->m_ucAntNumber[i];
				ucData[iCnt++] = m_ISOTable[iIdx]->m_ucAntStatus[i];
				ucData[iCnt++] = m_ISOTable[iIdx]->m_ucAntRSSI[i];
			}
			break;

		case FEDM_ISC_DATA_EPC_HEADER:
		case FEDM_ISC_DATA_EPC_DOMAIN:
		case FEDM_ISC_DATA_EPC_OBJECT:
		case FEDM_ISC_DATA_EPC_SNR:
		case FEDM_ISC_DATA_ANT_STATUS:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		case FEDM_ISC_DATA_SEC_STATUS:
			if((unsigned int)iDataBufLen < m_ISOTable[iIdx]->m_ucSecStatus.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}
			memcpy(ucData, &m_ISOTable[iIdx]->m_ucSecStatus[0], m_ISOTable[iIdx]->m_ucSecStatus.size());
			break;

		case FEDM_ISC_DATA_TxDB:
			if((unsigned int)iDataBufLen < m_ISOTable[iIdx]->m_ucTxDB.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}
			memcpy(ucData, &m_ISOTable[iIdx]->m_ucTxDB[0], m_ISOTable[iIdx]->m_ucTxDB.size());
			break;

		case FEDM_ISC_DATA_RxDB:
			if((unsigned int)iDataBufLen < m_ISOTable[iIdx]->m_ucRxDB.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}
			memcpy(ucData, &m_ISOTable[iIdx]->m_ucRxDB[0], m_ISOTable[iIdx]->m_ucRxDB.size());
			break;

		case FEDM_ISC_DATA_TxCB:
			if(iDataBufLen < 16)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}
			memcpy(ucData, &m_ISOTable[iIdx]->m_ucTxCB[0], 16);
			break;

		case FEDM_ISC_DATA_RxCB:
			if(iDataBufLen < 16)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}
			memcpy(ucData, &m_ISOTable[iIdx]->m_ucRxCB[0], 16);
			break;

		case FEDM_ISC_DATA_RxDB_EPC_BANK:
			if((unsigned int)iDataBufLen < m_ISOTable[iIdx]->m_ucRxDB_EpcBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}
			memcpy(ucData, &m_ISOTable[iIdx]->m_ucRxDB_EpcBank[0], m_ISOTable[iIdx]->m_ucRxDB_EpcBank.size());
			break;

		case FEDM_ISC_DATA_TxDB_EPC_BANK:
			if((unsigned int)iDataBufLen < m_ISOTable[iIdx]->m_ucTxDB_EpcBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}
			memcpy(ucData, &m_ISOTable[iIdx]->m_ucTxDB_EpcBank[0], m_ISOTable[iIdx]->m_ucTxDB_EpcBank.size());
			break;

		case FEDM_ISC_DATA_RxDB_TID_BANK:
			if((unsigned int)iDataBufLen < m_ISOTable[iIdx]->m_ucRxDB_TidBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}
			memcpy(ucData, &m_ISOTable[iIdx]->m_ucRxDB_TidBank[0], m_ISOTable[iIdx]->m_ucRxDB_TidBank.size());
			break;

		case FEDM_ISC_DATA_TxDB_TID_BANK:
			if((unsigned int)iDataBufLen < m_ISOTable[iIdx]->m_ucTxDB_TidBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}
			memcpy(ucData, &m_ISOTable[iIdx]->m_ucTxDB_TidBank[0], m_ISOTable[iIdx]->m_ucTxDB_TidBank.size());
			break;

		case FEDM_ISC_DATA_RxDB_RES_BANK:
			if((unsigned int)iDataBufLen < m_ISOTable[iIdx]->m_ucRxDB_ResBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}
			memcpy(ucData, &m_ISOTable[iIdx]->m_ucRxDB_ResBank[0], m_ISOTable[iIdx]->m_ucRxDB_ResBank.size());
			break;

		case FEDM_ISC_DATA_TxDB_RES_BANK:
			if((unsigned int)iDataBufLen < m_ISOTable[iIdx]->m_ucTxDB_ResBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}
			memcpy(ucData, &m_ISOTable[iIdx]->m_ucTxDB_ResBank[0], m_ISOTable[iIdx]->m_ucTxDB_ResBank.size());
			break;

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
	
	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
		
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        		:	17.11.2000 / M. Hultsch
  Version       	:	03.03.00 / 21.02.2011 / M. Hultsch

  Function			:	get one data member from a table

  Parameters		:	int iIdx		-	index of table
						unsigned int uiTableID	-	identifies the table
						unsigned int uiDataID	-	identifies the table data
						unsigned int* uiData	-	pointer to data buffer

  Return value		:	0 or error code (<0)

***************************************************************************/
int FEDM_ISCReader::GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, unsigned int* uiData)
{
	FEDM_CHK3(uiData);

	if(iIdx < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint*) - Index < 0");
		FEDM_RETURN(iIdx);
	}

	int iCnt = 0;

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
		if(m_BRMTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint*) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_BRMTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint*) - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_TRTYPE:
			*uiData = m_BRMTable[iIdx]->m_ucTrType;
			break;

		case FEDM_ISC_DATA_EPC_TYPE:
			*uiData = m_BRMTable[iIdx]->m_ucEpcType;
			break;

		case FEDM_ISC_DATA_TRINFO:
			*uiData = m_BRMTable[iIdx]->m_ucIDDT; // for ISO 14443
			break;

		case FEDM_ISC_DATA_AFI:
			*uiData = m_BRMTable[iIdx]->m_ucAFI;
			break;

		case FEDM_ISC_DATA_DSFID:
			*uiData = m_BRMTable[iIdx]->m_ucDsfID;
			break;

		case FEDM_ISC_DATA_DB_ADR:
			*uiData = m_BRMTable[iIdx]->m_ucDBAdr;
			break;

		case FEDM_ISC_DATA_DBN:
			*uiData = m_BRMTable[iIdx]->m_uiDBN;
			break;

		case FEDM_ISC_DATA_BLOCK_SIZE:
			*uiData = m_BRMTable[iIdx]->m_ucBlockSize;
			break;

		case FEDM_ISC_DATA_ANT_NR:
			*uiData = m_BRMTable[iIdx]->m_ucAntNr;
			break;

		case FEDM_ISC_DATA_SNR_LEN:
			*uiData = m_BRMTable[iIdx]->m_ucSnrLen;
			break;

		case FEDM_ISC_DATA_INPUT:
			*uiData = m_BRMTable[iIdx]->m_ucInput;
			break;

		case FEDM_ISC_DATA_STATE:
			*uiData = m_BRMTable[iIdx]->m_ucState;
			break;

		case FEDM_ISC_DATA_ANT_COUNT:
			*uiData = m_BRMTable[iIdx]->m_ucAntCount;
			break;

		case FEDM_ISC_DATA_TIMER:
			*uiData  = m_BRMTable[iIdx]->m_ucTimer[0]<<24;
			*uiData += m_BRMTable[iIdx]->m_ucTimer[1]<<16;
			*uiData += m_BRMTable[iIdx]->m_ucTimer[2]<<8;
			*uiData += m_BRMTable[iIdx]->m_ucTimer[3];
			break;

		case FEDM_ISC_DATA_DATE:
		case FEDM_ISC_DATA_SNR:
		case FEDM_ISC_DATA_RxDB:
		case FEDM_ISC_DATA_EPC:
		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
		case FEDM_ISC_DATA_MAC_ADR:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint*) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint*) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;

	case FEDM_ISC_ISO_TABLE:

		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint*) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint*) - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}
		
		switch(uiDataID)
		{
		case FEDM_ISC_DATA_TRTYPE:
			*uiData = m_ISOTable[iIdx]->m_ucTrType;
			break;

		case FEDM_ISC_DATA_SNR_LEN:
			*uiData = m_ISOTable[iIdx]->m_ucSnrLen;
			break;

		case FEDM_ISC_DATA_AFI:
			*uiData = m_ISOTable[iIdx]->m_ucAFI;
			break;

		case FEDM_ISC_DATA_DSFID:
			*uiData = m_ISOTable[iIdx]->m_ucDsfID;
			break;

		case FEDM_ISC_DATA_CHIP_ID:
			*uiData = m_ISOTable[iIdx]->m_ucChipID;
			break;

		case FEDM_ISC_DATA_IC_REF:
			*uiData = m_ISOTable[iIdx]->m_ucICRef;
			break;

		case FEDM_ISC_DATA_IS_SELECTED:
			*uiData = (unsigned int)m_ISOTable[iIdx]->m_bIsSelected;
			break;

		case FEDM_ISC_DATA_IS_ISO14443_4_INFO:
			*uiData = (unsigned int)m_ISOTable[iIdx]->m_bIsISO14443_4Info;
			break;

		case FEDM_ISC_DATA_IS_EPC:
			*uiData = (unsigned int)m_ISOTable[iIdx]->m_bIsEpc;
			break;

		case FEDM_ISC_DATA_BLOCK_SIZE:
			*uiData = m_ISOTable[iIdx]->m_ucBlockSize;
			break;

		case FEDM_ISC_DATA_IS_BLOCK_SIZE_SET:
			*uiData = (unsigned int)m_ISOTable[iIdx]->m_bIsBlockSizeSet;
			break;

		case FEDM_ISC_DATA_TRINFO:
			*uiData = m_ISOTable[iIdx]->m_ucTrInfo;
			break;

		case FEDM_ISC_DATA_OPTINFO:
			*uiData = m_ISOTable[iIdx]->m_ucOptInfo;
			break;

		case FEDM_ISC_DATA_PROTOINFO:
			*uiData = m_ISOTable[iIdx]->m_ucProtoInfo;
			break;

		case FEDM_ISC_DATA_EPC_TYPE:
			*uiData = m_ISOTable[iIdx]->m_ucEpcType;
			break;

		case FEDM_ISC_DATA_EPC_HEADER:
			*uiData = m_ISOTable[iIdx]->GetEpcHeader();
			if(((int)*uiData) < 0)
				FEDM_RETURN((int)*uiData);
			break;
			
		case FEDM_ISC_DATA_MEM_SIZE:
			if(m_ISOTable[iIdx]->m_ucMemSizeLen == 3)
			{
				*uiData  = ((unsigned int)(m_ISOTable[iIdx]->m_ucMemSize[0])<<16);
				*uiData += ((unsigned int)(m_ISOTable[iIdx]->m_ucMemSize[1])<<8);
				*uiData += ( unsigned int)(m_ISOTable[iIdx]->m_ucMemSize[2]);
			}
			else
			{
				*uiData  = ((unsigned int)(m_ISOTable[iIdx]->m_ucMemSize[0])<<8);
				*uiData += ( unsigned int)(m_ISOTable[iIdx]->m_ucMemSize[1]);
			}

			break;

		case FEDM_ISC_DATA_FSCI:
			*uiData = m_ISOTable[iIdx]->m_ucFSCI;
			break;

		case FEDM_ISC_DATA_FWI:
			*uiData = m_ISOTable[iIdx]->m_ucFWI;
			break;

		case FEDM_ISC_DATA_DSI:
			*uiData = m_ISOTable[iIdx]->m_ucDSI;
			break;

		case FEDM_ISC_DATA_DRI:
			*uiData = m_ISOTable[iIdx]->m_ucDRI;
			break;

		case FEDM_ISC_DATA_NAD:
			*uiData = m_ISOTable[iIdx]->m_ucNad;
			break;

		case FEDM_ISC_DATA_CID:
			*uiData = m_ISOTable[iIdx]->m_ucCid;
			break;

		case FEDM_ISC_DATA_VALUE:
			*uiData = m_ISOTable[iIdx]->m_uiValue;
			break;

		case FEDM_ISC_DATA_ANT_COUNT:
			*uiData = m_ISOTable[iIdx]->m_ucAntCount;
			break;

		case FEDM_ISC_DATA_EPC:
		case FEDM_ISC_DATA_EPC_DOMAIN:
		case FEDM_ISC_DATA_EPC_OBJECT:
		case FEDM_ISC_DATA_EPC_SNR:
		case FEDM_ISC_DATA_SNR:
		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
		case FEDM_ISC_DATA_SEC_STATUS:
		case FEDM_ISC_DATA_TxDB:
		case FEDM_ISC_DATA_RxDB:
		case FEDM_ISC_DATA_TxCB:
		case FEDM_ISC_DATA_RxCB:
		case FEDM_ISC_DATA_RxDB_EPC_BANK:
		case FEDM_ISC_DATA_TxDB_EPC_BANK:
		case FEDM_ISC_DATA_RxDB_TID_BANK:
		case FEDM_ISC_DATA_TxDB_TID_BANK:
		case FEDM_ISC_DATA_RxDB_RES_BANK:
		case FEDM_ISC_DATA_TxDB_RES_BANK:
		case FEDM_ISC_DATA_ANT_NR:
		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint*) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint*) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
	
#if !defined(__BORLANDC__)
	case FEDM_ISC_CAD_TABLE:
		switch(uiDataID)
		{
		case FEDM_ISC_DATA_POWER_AVG:
			if(iIdx<0 || iIdx>=15)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint*) - " << GetErrorText(FEDM_ERROR_PARAMETER));
				FEDM_RETURN(FEDM_ERROR_PARAMETER);
			}

			*uiData = m_CADTable.m_ucPowerAvg[iIdx];
			break;

		case FEDM_ISC_DATA_POWER_PEAK:
			if(iIdx<0 || iIdx>=15)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint*) - " << GetErrorText(FEDM_ERROR_PARAMETER));
				FEDM_RETURN(FEDM_ERROR_PARAMETER);
			}

			*uiData = m_CADTable.m_ucPowerPeak[iIdx];
			break;

		case FEDM_ISC_DATA_HISTORY_ALLOC:
			// request of number of record sets
			iCnt = (int)m_CADTable.m_History.size();
			if(iIdx<0 || iIdx>=iCnt)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint*) - " << GetErrorText(FEDM_ERROR_PARAMETER));
				FEDM_RETURN(FEDM_ERROR_PARAMETER);
			}

			*uiData = m_CADTable.m_History[iIdx]->uiAlloc;
			break;

		case FEDM_ISC_DATA_HISTORY_TX:
			// request of number of record sets
			iCnt = (int)m_CADTable.m_History.size();
			if(iIdx<0 || iIdx>=iCnt)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint*) - " << GetErrorText(FEDM_ERROR_PARAMETER));
				FEDM_RETURN(FEDM_ERROR_PARAMETER);
			}

			*uiData = m_CADTable.m_History[iIdx]->ucTxChannel;
			break;

		case FEDM_ISC_DATA_HISTORY_RX:
			// request of number of record sets
			iCnt = (int)m_CADTable.m_History.size();
			if(iIdx<0 || iIdx>=iCnt)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint*) - " << GetErrorText(FEDM_ERROR_PARAMETER));
				FEDM_RETURN(FEDM_ERROR_PARAMETER);
			}

			*uiData = m_CADTable.m_History[iIdx]->ucRxChannel;
			break;

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint*) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
#endif		
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint*) - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        		:	17.11.2000 / M. Hultsch
  Version       	:	03.03.00 / 21.02.2011 / M. Hultsch

  Function			:	get one data member from a table

  Parameters		:	int iIdx			-	index of table
						unsigned int uiTableID		-	identifies the table
						unsigned int uiDataID		-	identifies the table data
						__int64* i64Data	-	pointer to data buffer

  Return value		:	0 or error code (<0)

***************************************************************************/
int FEDM_ISCReader::GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, __int64* i64Data)
{
	FEDM_CHK3(i64Data);

	if(iIdx < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64*) - Index < 0");
		FEDM_RETURN(iIdx);
	}

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
		if(m_BRMTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64*) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_BRMTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64*) - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_SNR:
			*i64Data = m_BRMTable[iIdx]->GetSnr();	// only the lower 8 bytes
			break;

		case FEDM_ISC_DATA_TRTYPE:
		case FEDM_ISC_DATA_EPC_TYPE:
		case FEDM_ISC_DATA_AFI:
		case FEDM_ISC_DATA_DSFID:
		case FEDM_ISC_DATA_TIMER:
		case FEDM_ISC_DATA_DATE:
		case FEDM_ISC_DATA_DB_ADR:
		case FEDM_ISC_DATA_DBN:
		case FEDM_ISC_DATA_BLOCK_SIZE:
		case FEDM_ISC_DATA_RxDB:
		case FEDM_ISC_DATA_INPUT:
		case FEDM_ISC_DATA_STATE:
		case FEDM_ISC_DATA_SNR_LEN:
		case FEDM_ISC_DATA_EPC:
		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
		case FEDM_ISC_DATA_VALUE:
		case FEDM_ISC_DATA_ANT_NR:
		case FEDM_ISC_DATA_ANT_COUNT:
		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
		case FEDM_ISC_DATA_MAC_ADR:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64*) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64*) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64*) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64*) - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_SNR:
			*i64Data = m_ISOTable[iIdx]->GetSnr();	// only the lower 8 bytes
			break;

		case FEDM_ISC_DATA_EPC_DOMAIN:
			*i64Data = m_ISOTable[iIdx]->GetEpcDomainManager();
			if(*i64Data < 0)
				FEDM_RETURN((int)*i64Data);
			break;

		case FEDM_ISC_DATA_EPC_OBJECT:
			*i64Data = m_ISOTable[iIdx]->GetEpcObjectClass();
			if(*i64Data < 0)
				FEDM_RETURN((int)*i64Data);
			break;

		case FEDM_ISC_DATA_EPC_SNR:
			*i64Data = m_ISOTable[iIdx]->GetEpcSnr();
			if(*i64Data < 0)
				FEDM_RETURN((int)*i64Data);
			break;

		case FEDM_ISC_DATA_VALUE:
			*i64Data = m_ISOTable[iIdx]->m_uiValue;
			break;

		case FEDM_ISC_DATA_FSCI:
		case FEDM_ISC_DATA_FWI:
		case FEDM_ISC_DATA_DSI:
		case FEDM_ISC_DATA_DRI:
		case FEDM_ISC_DATA_NAD:
		case FEDM_ISC_DATA_CID:
		case FEDM_ISC_DATA_SNR_LEN:
		case FEDM_ISC_DATA_TRINFO:
		case FEDM_ISC_DATA_OPTINFO:
		case FEDM_ISC_DATA_PROTOINFO:
		case FEDM_ISC_DATA_EPC:
		case FEDM_ISC_DATA_EPC_TYPE:
		case FEDM_ISC_DATA_EPC_HEADER:
		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
		case FEDM_ISC_DATA_TRTYPE:
		case FEDM_ISC_DATA_AFI:
		case FEDM_ISC_DATA_DSFID:
		case FEDM_ISC_DATA_CHIP_ID:
		case FEDM_ISC_DATA_MEM_SIZE:
		case FEDM_ISC_DATA_IC_REF:
		case FEDM_ISC_DATA_IS_EPC:
		case FEDM_ISC_DATA_IS_SELECTED:
		case FEDM_ISC_DATA_SEC_STATUS:
		case FEDM_ISC_DATA_TxDB:
		case FEDM_ISC_DATA_RxDB:
		case FEDM_ISC_DATA_TxCB:
		case FEDM_ISC_DATA_RxCB:
		case FEDM_ISC_DATA_RxDB_EPC_BANK:
		case FEDM_ISC_DATA_TxDB_EPC_BANK:
		case FEDM_ISC_DATA_RxDB_TID_BANK:
		case FEDM_ISC_DATA_TxDB_TID_BANK:
		case FEDM_ISC_DATA_RxDB_RES_BANK:
		case FEDM_ISC_DATA_TxDB_RES_BANK:
		case FEDM_ISC_DATA_BLOCK_SIZE:
		case FEDM_ISC_DATA_IS_BLOCK_SIZE_SET:
		case FEDM_ISC_DATA_IS_ISO14443_4_INFO:
		case FEDM_ISC_DATA_ANT_NR:
		case FEDM_ISC_DATA_ANT_COUNT:
		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64*) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64*) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
	
	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64*) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
		
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64*) - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        		:	19.11.2000 / M. Hultsch
  Version       	:	03.03.00 / 21.02.2011 / M. Hultsch

  Function			:	get one data member from a table

  Parameters		:	int iIdx		-	index of table
						unsigned int uiTableID	-	identifies the table
						unsigned int uiDataID	-	identifies the table data
						CString& sData	-	reference to data buffer

  Return value		:	0 or error code (<0)

***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_ISCReader::GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, CString& sData)
{
	FEDM_CHK3(&sData);

	if(iIdx < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString&) - Index < 0");
		FEDM_RETURN(iIdx);
	}

	int iErr;
	char cEpc[FEDM_ISC_MAX_UID_LENGTH];
	CString sTmp;

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
		if(m_BRMTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString&) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_BRMTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString&) - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_SNR:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucSnr[0], (int)m_BRMTable[iIdx]->m_ucSnrLen, sData));
			break;		

		case FEDM_ISC_DATA_EPC:
			FEDM_CHK2(iErr, m_BRMTable[iIdx]->GetEpc(cEpc, FEDM_ISC_MAX_UID_LENGTH));
			sData = cEpc;
			break;

		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(m_BRMTable[iIdx]->m_ucClass1Gen2_PC, 2, sData));
			break;

		case FEDM_ISC_DATA_TRTYPE:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucTrType, 1, sData));
			break;		

		case FEDM_ISC_DATA_TRINFO:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucIDDT, 1, sData)); // for ISO 14443
			break;

		case FEDM_ISC_DATA_AFI:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucAFI, 1, sData));
			break;		

		case FEDM_ISC_DATA_DSFID:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucDsfID, 1, sData));
			break;		

		case FEDM_ISC_DATA_EPC_TYPE:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucEpcType, 1, sData));
			break;

		case FEDM_ISC_DATA_DB_ADR:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucDBAdr, 1, sData));
			break;		

		case FEDM_ISC_DATA_DBN:
			FEDM_CHK2(iErr, FEDM_ConvUIntToHexStr(m_BRMTable[iIdx]->m_uiDBN, sData));
			break;		

		case FEDM_ISC_DATA_BLOCK_SIZE:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucBlockSize, 1, sData));
			break;		

		case FEDM_ISC_DATA_ANT_NR:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucAntNr, 1, sData));
			break;		

		case FEDM_ISC_DATA_SNR_LEN:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucSnrLen, 1, sData));
			break;		

		case FEDM_ISC_DATA_INPUT:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucInput, 1, sData));
			break;		

		case FEDM_ISC_DATA_STATE:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucState, 1, sData));
			break;		

		case FEDM_ISC_DATA_ANT_COUNT:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucAntCount, 1, sData));
			break;		

		case FEDM_ISC_DATA_MAC_ADR:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucMacAddress[0], 1, sTmp));
			sData = sTmp;
			sData += ":";
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucMacAddress[1], 1, sTmp));
			sData = sTmp;
			sData += ":";
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucMacAddress[2], 1, sTmp));
			sData = sTmp;
			sData += ":";
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucMacAddress[3], 1, sTmp));
			sData = sTmp;
			sData += ":";
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucMacAddress[4], 1, sTmp));
			sData = sTmp;
			sData += ":";
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucMacAddress[5], 1, sTmp));
			sData = sTmp;
			break;		

		case FEDM_ISC_DATA_TIMER:
		case FEDM_ISC_DATA_DATE:
		case FEDM_ISC_DATA_RxDB:
		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString&) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString&) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString&) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString&) - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_SNR:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucSnr[0], (int)m_ISOTable[iIdx]->m_ucSnrLen, sData));
			break;		

		case FEDM_ISC_DATA_SNR_LEN:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucSnrLen, 1, sData));
			break;		

		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(m_ISOTable[iIdx]->m_ucClass1Gen2_PC, 2, sData));
			break;

		case FEDM_ISC_DATA_TRTYPE:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucTrType, 1, sData));
			break;		

		case FEDM_ISC_DATA_AFI:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucAFI, 1, sData));
			break;		

		case FEDM_ISC_DATA_DSFID:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucDsfID, 1, sData));
			break;		

		case FEDM_ISC_DATA_CHIP_ID:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucChipID, 1, sData));
			break;		

		case FEDM_ISC_DATA_MEM_SIZE:
			if(m_ISOTable[iIdx]->m_ucMemSizeLen == 3)
			{
				FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucMemSize[0], 3, sData));
			}
			else
			{
				FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucMemSize[0], 2, sData));
			}
			break;

		case FEDM_ISC_DATA_IC_REF:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucICRef, 1, sData));
			break;		

		case FEDM_ISC_DATA_TRINFO:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucTrInfo, 1, sData));
			break;

		case FEDM_ISC_DATA_OPTINFO:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucOptInfo, 1, sData));
			break;

		case FEDM_ISC_DATA_PROTOINFO:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucProtoInfo, 1, sData));
			break;

		case FEDM_ISC_DATA_EPC_TYPE:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucEpcType, 1, sData));
			break;

		case FEDM_ISC_DATA_EPC:
			FEDM_CHK2(iErr, m_ISOTable[iIdx]->GetEpc(cEpc, FEDM_ISC_MAX_UID_LENGTH));
			sData = cEpc;
			break;

		case FEDM_ISC_DATA_EPC_HEADER:
			FEDM_CHK2(iErr, m_ISOTable[iIdx]->GetEpcHeader());
			sprintf(cEpc, "%02X", iErr);
			sData = cEpc;
			break;

		case FEDM_ISC_DATA_EPC_DOMAIN:
			switch(m_ISOTable[iIdx]->m_ucEpcType)
			{
			case FEDM_ISC_EPC_TYPE_1:
				sprintf(cEpc, "%07lX", (unsigned long)m_ISOTable[iIdx]->GetEpcDomainManager());
				break;
			case FEDM_ISC_EPC_TYPE_2:
			case FEDM_ISC_EPC_TYPE_3:
			case FEDM_ISC_EPC_TYPE_4:
				sprintf(cEpc, "%06lX", (unsigned long)m_ISOTable[iIdx]->GetEpcDomainManager());
				break;
			default:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString&) - " << GetErrorText(FEDM_ERROR_UNKNOWN_EPC_TYPE));
				FEDM_RETURN(FEDM_ERROR_UNKNOWN_EPC_TYPE);
			}
			sData = cEpc;
			break;

		case FEDM_ISC_DATA_EPC_OBJECT:
			switch(m_ISOTable[iIdx]->m_ucEpcType)
			{
			case FEDM_ISC_EPC_TYPE_1:
				sprintf(cEpc, "%06lX", (unsigned long)m_ISOTable[iIdx]->GetEpcObjectClass());
				break;
			case FEDM_ISC_EPC_TYPE_2:
			case FEDM_ISC_EPC_TYPE_3:
			case FEDM_ISC_EPC_TYPE_4:
				sprintf(cEpc, "%05lX", (unsigned long)m_ISOTable[iIdx]->GetEpcObjectClass());
				break;
			default:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString&) - " << GetErrorText(FEDM_ERROR_UNKNOWN_EPC_TYPE));
				FEDM_RETURN(FEDM_ERROR_UNKNOWN_EPC_TYPE);
			}
			sData = cEpc;
			break;

		case FEDM_ISC_DATA_EPC_SNR:
			switch(m_ISOTable[iIdx]->m_ucEpcType)
			{
			case FEDM_ISC_EPC_TYPE_1:
				sprintf(cEpc, "%09lX", (unsigned long)m_ISOTable[iIdx]->GetEpcSnr());
				break;
			case FEDM_ISC_EPC_TYPE_2:
			case FEDM_ISC_EPC_TYPE_3:
			case FEDM_ISC_EPC_TYPE_4:
				sprintf(cEpc, "%06lX", (unsigned long)m_ISOTable[iIdx]->GetEpcSnr());
				break;
			default:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString&) - " << GetErrorText(FEDM_ERROR_UNKNOWN_EPC_TYPE));
				FEDM_RETURN(FEDM_ERROR_UNKNOWN_EPC_TYPE);
			}
			sData = cEpc;
			break;

		case FEDM_ISC_DATA_FSCI:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucFSCI, 1, sData));
			break;

		case FEDM_ISC_DATA_FWI:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucFWI, 1, sData));
			break;

		case FEDM_ISC_DATA_DSI:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucDSI, 1, sData));
			break;

		case FEDM_ISC_DATA_DRI:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucDRI, 1, sData));
			break;

		case FEDM_ISC_DATA_NAD:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucNad, 1, sData));
			break;

		case FEDM_ISC_DATA_CID:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucCid, 1, sData));
			break;

		case FEDM_ISC_DATA_VALUE:
			FEDM_CHK2(iErr, FEDM_ConvUIntToHexStr(m_ISOTable[iIdx]->m_uiValue, sData));
			break;

		case FEDM_ISC_DATA_ANT_COUNT:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucAntCount, 1, sData));
			break;

		case FEDM_ISC_DATA_ANT_NR:
		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
		case FEDM_ISC_DATA_IS_SELECTED:
		case FEDM_ISC_DATA_IS_EPC:
		case FEDM_ISC_DATA_SEC_STATUS:
		case FEDM_ISC_DATA_TxDB:
		case FEDM_ISC_DATA_RxDB:
		case FEDM_ISC_DATA_TxCB:
		case FEDM_ISC_DATA_RxCB:
		case FEDM_ISC_DATA_RxDB_EPC_BANK:
		case FEDM_ISC_DATA_TxDB_EPC_BANK:
		case FEDM_ISC_DATA_RxDB_TID_BANK:
		case FEDM_ISC_DATA_TxDB_TID_BANK:
		case FEDM_ISC_DATA_RxDB_RES_BANK:
		case FEDM_ISC_DATA_TxDB_RES_BANK:
		case FEDM_ISC_DATA_BLOCK_SIZE:
		case FEDM_ISC_DATA_IS_BLOCK_SIZE_SET:
		case FEDM_ISC_DATA_IS_ISO14443_4_INFO:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString&) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString&) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
	
	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString&) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
		
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString&) - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_RETURN(FEDM_OK);
}
#endif


/***************************************************************************
  Begin        		:	20.02.2006 / M. Hultsch
  Version       	:	03.03.00 / 21.02.2011 / M. Hultsch

  Function			:	get one data member from a table

  Parameters		:	int iIdx		-	index of table
						unsigned int uiTableID	-	identifies the table
						unsigned int uiDataID	-	identifies the table data
						char* cData		-	pointer to data buffer
						int iDataBufLen	-	length of cData (includes additional '\0')

  Return value		:	0 or error code (<0)

***************************************************************************/
int FEDM_ISCReader::GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, char* cData, int iDataBufLen)
{
	FEDM_CHK3(cData);

	if(iIdx < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", char*, " << iDataBufLen << ") - Index < 0");
		FEDM_RETURN(iIdx);
	}

	if(iDataBufLen == 0)
		return FEDM_OK; // nothing to do

	if(iDataBufLen < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", char*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
		FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
	}

	int iErr;
	string sTmp;

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
		if(m_BRMTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", char*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_BRMTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", char*, " << iDataBufLen << ") - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_SNR:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_BRMTable[iIdx]->m_ucSnr[0], (int)m_BRMTable[iIdx]->m_ucSnrLen, cData, iDataBufLen));
			break;		

		case FEDM_ISC_DATA_EPC:
			FEDM_CHK2(iErr, m_BRMTable[iIdx]->GetEpc(cData, iDataBufLen));
			break;

		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(m_BRMTable[iIdx]->m_ucClass1Gen2_PC, 2, cData, iDataBufLen));
			break;

		case FEDM_ISC_DATA_TRTYPE:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_BRMTable[iIdx]->m_ucTrType, 1, cData, iDataBufLen));
			break;		

		case FEDM_ISC_DATA_EPC_TYPE:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_BRMTable[iIdx]->m_ucEpcType, 1, cData, iDataBufLen));
			break;

		case FEDM_ISC_DATA_TRINFO:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_BRMTable[iIdx]->m_ucIDDT, 1, cData, iDataBufLen)); // for ISO 14443
			break;

		case FEDM_ISC_DATA_AFI:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_BRMTable[iIdx]->m_ucAFI, 1, cData, iDataBufLen));
			break;		

		case FEDM_ISC_DATA_DSFID:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_BRMTable[iIdx]->m_ucDsfID, 1, cData, iDataBufLen));
			break;		

		case FEDM_ISC_DATA_DB_ADR:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_BRMTable[iIdx]->m_ucDBAdr, 1, cData, iDataBufLen));
			break;		

		case FEDM_ISC_DATA_DBN:
			FEDM_CHK2(iErr, FEDM_ConvUIntToHexChar(m_BRMTable[iIdx]->m_uiDBN, cData, iDataBufLen));
			break;		

		case FEDM_ISC_DATA_BLOCK_SIZE:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_BRMTable[iIdx]->m_ucBlockSize, 1, cData, iDataBufLen));
			break;		

		case FEDM_ISC_DATA_ANT_NR:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_BRMTable[iIdx]->m_ucAntNr, 1, cData, iDataBufLen));
			break;		

		case FEDM_ISC_DATA_SNR_LEN:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_BRMTable[iIdx]->m_ucSnrLen, 1, cData, iDataBufLen));
			break;		

		case FEDM_ISC_DATA_INPUT:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_BRMTable[iIdx]->m_ucInput, 1, cData, iDataBufLen));
			break;		

		case FEDM_ISC_DATA_STATE:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_BRMTable[iIdx]->m_ucState, 1, cData, iDataBufLen));
			break;		

		case FEDM_ISC_DATA_ANT_COUNT:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_BRMTable[iIdx]->m_ucAntCount, 1, cData, iDataBufLen));
			break;		

		case FEDM_ISC_DATA_MAC_ADR:
			if(iDataBufLen < 18)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", char*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucMacAddress[0], 1, sTmp));
			strcpy(cData, sTmp.c_str());
			strcat(cData, ":");
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucMacAddress[1], 1, sTmp));
			strcat(cData, sTmp.c_str());
			strcat(cData, ":");
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucMacAddress[2], 1, sTmp));
			strcat(cData, sTmp.c_str());
			strcat(cData, ":");
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucMacAddress[3], 1, sTmp));
			strcat(cData, sTmp.c_str());
			strcat(cData, ":");
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucMacAddress[4], 1, sTmp));
			strcat(cData, sTmp.c_str());
			strcat(cData, ":");
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucMacAddress[5], 1, sTmp));
			strcat(cData, sTmp.c_str());
			break;		

		case FEDM_ISC_DATA_TIMER:
		case FEDM_ISC_DATA_DATE:
		case FEDM_ISC_DATA_RxDB:
		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", char*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", char*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", char*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", char*, " << iDataBufLen << ") - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_SNR:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_ISOTable[iIdx]->m_ucSnr[0], (int)m_ISOTable[iIdx]->m_ucSnrLen, cData, iDataBufLen));
			break;		

		case FEDM_ISC_DATA_SNR_LEN:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_ISOTable[iIdx]->m_ucSnrLen, 1, cData, iDataBufLen));
			break;		

		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(m_ISOTable[iIdx]->m_ucClass1Gen2_PC, 2, cData, iDataBufLen));
			break;

		case FEDM_ISC_DATA_TRTYPE:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_ISOTable[iIdx]->m_ucTrType, 1, cData, iDataBufLen));
			break;		

		case FEDM_ISC_DATA_AFI:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_ISOTable[iIdx]->m_ucAFI, 1, cData, iDataBufLen));
			break;		

		case FEDM_ISC_DATA_DSFID:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_ISOTable[iIdx]->m_ucDsfID, 1, cData, iDataBufLen));
			break;		

		case FEDM_ISC_DATA_CHIP_ID:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_ISOTable[iIdx]->m_ucChipID, 1, cData, iDataBufLen));
			break;		

		case FEDM_ISC_DATA_MEM_SIZE:
			if(m_ISOTable[iIdx]->m_ucMemSizeLen == 3)
			{
				FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_ISOTable[iIdx]->m_ucMemSize[0], 3, cData, iDataBufLen));
			}
			else
			{
				FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_ISOTable[iIdx]->m_ucMemSize[0], 2, cData, iDataBufLen));
			}
			break;

		case FEDM_ISC_DATA_IC_REF:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_ISOTable[iIdx]->m_ucICRef, 1, cData, iDataBufLen));
			break;		

		case FEDM_ISC_DATA_TRINFO:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_ISOTable[iIdx]->m_ucTrInfo, 1, cData, iDataBufLen));
			break;

		case FEDM_ISC_DATA_OPTINFO:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_ISOTable[iIdx]->m_ucOptInfo, 1, cData, iDataBufLen));
			break;

		case FEDM_ISC_DATA_PROTOINFO:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_ISOTable[iIdx]->m_ucProtoInfo, 1, cData, iDataBufLen));
			break;

		case FEDM_ISC_DATA_EPC_TYPE:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_ISOTable[iIdx]->m_ucEpcType, 1, cData, iDataBufLen));
			break;

		case FEDM_ISC_DATA_EPC:
			FEDM_CHK2(iErr, m_ISOTable[iIdx]->GetEpc(cData, iDataBufLen));

			break;

		case FEDM_ISC_DATA_EPC_HEADER:
			if(iDataBufLen < 3)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", char*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}

			FEDM_CHK2(iErr, m_ISOTable[iIdx]->GetEpcHeader());
			sprintf(cData, "%02X", iErr);
			break;

		case FEDM_ISC_DATA_EPC_DOMAIN:
			switch(m_ISOTable[iIdx]->m_ucEpcType)
			{
			case FEDM_ISC_EPC_TYPE_1:
				if(iDataBufLen < 8)
				{
					FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", char*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
					FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
				}

				sprintf(cData, "%07lX", (unsigned long)m_ISOTable[iIdx]->GetEpcDomainManager());
				break;
			case FEDM_ISC_EPC_TYPE_2:
			case FEDM_ISC_EPC_TYPE_3:
			case FEDM_ISC_EPC_TYPE_4:
				if(iDataBufLen < 7)
				{
					FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", char*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
					FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
				}

				sprintf(cData, "%06lX", (unsigned long)m_ISOTable[iIdx]->GetEpcDomainManager());
				break;
			default:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", char*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_UNKNOWN_EPC_TYPE));
				FEDM_RETURN(FEDM_ERROR_UNKNOWN_EPC_TYPE);
			}
			break;

		case FEDM_ISC_DATA_EPC_OBJECT:
			switch(m_ISOTable[iIdx]->m_ucEpcType)
			{
			case FEDM_ISC_EPC_TYPE_1:
				if(iDataBufLen < 7)
				{
					FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", char*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
					FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
				}

				sprintf(cData, "%06lX", (unsigned long)m_ISOTable[iIdx]->GetEpcObjectClass());
				break;
			case FEDM_ISC_EPC_TYPE_2:
			case FEDM_ISC_EPC_TYPE_3:
			case FEDM_ISC_EPC_TYPE_4:
				if(iDataBufLen < 6)
				{
					FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", char*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
					FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
				}

				sprintf(cData, "%05lX", (unsigned long)m_ISOTable[iIdx]->GetEpcObjectClass());
				break;
			default:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", char*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_UNKNOWN_EPC_TYPE));
				FEDM_RETURN(FEDM_ERROR_UNKNOWN_EPC_TYPE);
			}
			break;

		case FEDM_ISC_DATA_EPC_SNR:
			switch(m_ISOTable[iIdx]->m_ucEpcType)
			{
			case FEDM_ISC_EPC_TYPE_1:
				if(iDataBufLen < 10)
				{
					FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", char*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
					FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
				}

				sprintf(cData, "%09lX", (unsigned long)m_ISOTable[iIdx]->GetEpcSnr());
				break;
			case FEDM_ISC_EPC_TYPE_2:
			case FEDM_ISC_EPC_TYPE_3:
			case FEDM_ISC_EPC_TYPE_4:
				if(iDataBufLen < 7)
				{
					FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", char*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
					FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
				}

				sprintf(cData, "%06lX", (unsigned long)m_ISOTable[iIdx]->GetEpcSnr());
				break;
			default:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", char*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_UNKNOWN_EPC_TYPE));
				FEDM_RETURN(FEDM_ERROR_UNKNOWN_EPC_TYPE);
			}
			break;

		case FEDM_ISC_DATA_FSCI:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_ISOTable[iIdx]->m_ucFSCI, 1, cData, iDataBufLen));
			break;

		case FEDM_ISC_DATA_FWI:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_ISOTable[iIdx]->m_ucFWI, 1, cData, iDataBufLen));
			break;

		case FEDM_ISC_DATA_DSI:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_ISOTable[iIdx]->m_ucDSI, 1, cData, iDataBufLen));
			break;

		case FEDM_ISC_DATA_DRI:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_ISOTable[iIdx]->m_ucDRI, 1, cData, iDataBufLen));
			break;

		case FEDM_ISC_DATA_NAD:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_ISOTable[iIdx]->m_ucNad, 1, cData, iDataBufLen));
			break;

		case FEDM_ISC_DATA_CID:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_ISOTable[iIdx]->m_ucCid, 1, cData, iDataBufLen));
			break;

		case FEDM_ISC_DATA_VALUE:
			FEDM_CHK2(iErr, FEDM_ConvUIntToHexChar(m_ISOTable[iIdx]->m_uiValue, cData, iDataBufLen));
			break;

		case FEDM_ISC_DATA_ANT_COUNT:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexChar(&m_ISOTable[iIdx]->m_ucAntCount, 1, cData, iDataBufLen));
			break;

		case FEDM_ISC_DATA_ANT_NR:
		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
		case FEDM_ISC_DATA_IS_SELECTED:
		case FEDM_ISC_DATA_IS_EPC:
		case FEDM_ISC_DATA_SEC_STATUS:
		case FEDM_ISC_DATA_TxDB:
		case FEDM_ISC_DATA_RxDB:
		case FEDM_ISC_DATA_TxCB:
		case FEDM_ISC_DATA_RxCB:
		case FEDM_ISC_DATA_RxDB_EPC_BANK:
		case FEDM_ISC_DATA_TxDB_EPC_BANK:
		case FEDM_ISC_DATA_RxDB_TID_BANK:
		case FEDM_ISC_DATA_TxDB_TID_BANK:
		case FEDM_ISC_DATA_RxDB_RES_BANK:
		case FEDM_ISC_DATA_TxDB_RES_BANK:
		case FEDM_ISC_DATA_BLOCK_SIZE:
		case FEDM_ISC_DATA_IS_BLOCK_SIZE_SET:
		case FEDM_ISC_DATA_IS_ISO14443_4_INFO:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", char*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", char*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
	
	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", char*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
		
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", char*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        		:	19.08.2003 / M. Hultsch
  Version       	:	03.03.00 / 21.02.2011 / M. Hultsch

  Function			:	get one data member from a table

  Parameters		:	int iIdx		-	index of table
						unsigned int uiTableID	-	identifies the table
						unsigned int uiDataID	-	identifies the table data
						string& sData	-	reference to data buffer

  Return value		:	0 or error code (<0)

***************************************************************************/
int FEDM_ISCReader::GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, string& sData)
{
	FEDM_CHK3(&sData);

	if(iIdx < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string&) - Index < 0");
		FEDM_RETURN(iIdx);
	}

	int iErr;
	char cEpc[FEDM_ISC_MAX_UID_LENGTH];
	string sTmp;

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
		if(m_BRMTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string&) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_BRMTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string&) - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_SNR:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucSnr[0], (int)m_BRMTable[iIdx]->m_ucSnrLen, sData));
			break;		

		case FEDM_ISC_DATA_EPC:
			FEDM_CHK2(iErr, m_BRMTable[iIdx]->GetEpc(cEpc, FEDM_ISC_MAX_UID_LENGTH));
			sData = cEpc;
			break;

		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(m_BRMTable[iIdx]->m_ucClass1Gen2_PC, 2, sData));
			break;

		case FEDM_ISC_DATA_TRTYPE:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucTrType, 1, sData));
			break;		

		case FEDM_ISC_DATA_EPC_TYPE:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucEpcType, 1, sData));
			break;

		case FEDM_ISC_DATA_TRINFO:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucIDDT, 1, sData)); // for ISO 14443
			break;

		case FEDM_ISC_DATA_AFI:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucAFI, 1, sData));
			break;		

		case FEDM_ISC_DATA_DSFID:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucDsfID, 1, sData));
			break;		

		case FEDM_ISC_DATA_DB_ADR:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucDBAdr, 1, sData));
			break;		

		case FEDM_ISC_DATA_DBN:
			FEDM_CHK2(iErr, FEDM_ConvUIntToHexStr(m_BRMTable[iIdx]->m_uiDBN, sData));
			break;		

		case FEDM_ISC_DATA_BLOCK_SIZE:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucBlockSize, 1, sData));
			break;		

		case FEDM_ISC_DATA_ANT_NR:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucAntNr, 1, sData));
			break;		

		case FEDM_ISC_DATA_SNR_LEN:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucSnrLen, 1, sData));
			break;		

		case FEDM_ISC_DATA_INPUT:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucInput, 1, sData));
			break;		

		case FEDM_ISC_DATA_STATE:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucState, 1, sData));
			break;		

		case FEDM_ISC_DATA_ANT_COUNT:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucAntCount, 1, sData));
			break;		

		case FEDM_ISC_DATA_MAC_ADR:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucMacAddress[0], 1, sTmp));
			sData = sTmp;
			sData += ":";
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucMacAddress[1], 1, sTmp));
			sData = sTmp;
			sData += ":";
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucMacAddress[2], 1, sTmp));
			sData = sTmp;
			sData += ":";
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucMacAddress[3], 1, sTmp));
			sData = sTmp;
			sData += ":";
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucMacAddress[4], 1, sTmp));
			sData = sTmp;
			sData += ":";
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucMacAddress[5], 1, sTmp));
			sData = sTmp;
			break;		

		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
		case FEDM_ISC_DATA_TIMER:
		case FEDM_ISC_DATA_DATE:
		case FEDM_ISC_DATA_RxDB:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string&) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string&) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string&) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string&) - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_SNR:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucSnr[0], (int)m_ISOTable[iIdx]->m_ucSnrLen, sData));
			break;		

		case FEDM_ISC_DATA_SNR_LEN:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucSnrLen, 1, sData));
			break;		

		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(m_ISOTable[iIdx]->m_ucClass1Gen2_PC, 2, sData));
			break;

		case FEDM_ISC_DATA_TRTYPE:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucTrType, 1, sData));
			break;		

		case FEDM_ISC_DATA_AFI:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucAFI, 1, sData));
			break;		

		case FEDM_ISC_DATA_DSFID:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucDsfID, 1, sData));
			break;		

		case FEDM_ISC_DATA_CHIP_ID:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucChipID, 1, sData));
			break;		

		case FEDM_ISC_DATA_MEM_SIZE:
			if(m_ISOTable[iIdx]->m_ucMemSizeLen == 3)
			{
				FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucMemSize[0], 3, sData));
			}
			else
			{
				FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucMemSize[0], 2, sData));
			}
			break;

		case FEDM_ISC_DATA_IC_REF:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucICRef, 1, sData));
			break;		

		case FEDM_ISC_DATA_TRINFO:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucTrInfo, 1, sData));
			break;

		case FEDM_ISC_DATA_OPTINFO:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucOptInfo, 1, sData));
			break;

		case FEDM_ISC_DATA_PROTOINFO:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucProtoInfo, 1, sData));
			break;

		case FEDM_ISC_DATA_EPC_TYPE:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucEpcType, 1, sData));
			break;

		case FEDM_ISC_DATA_EPC:
			FEDM_CHK2(iErr, m_ISOTable[iIdx]->GetEpc(cEpc, FEDM_ISC_MAX_UID_LENGTH));
			sData = cEpc;
			break;

		case FEDM_ISC_DATA_EPC_HEADER:
			FEDM_CHK2(iErr, m_ISOTable[iIdx]->GetEpcHeader());
			sprintf(cEpc, "%02X", iErr);
			sData = cEpc;
			break;

		case FEDM_ISC_DATA_EPC_DOMAIN:
			switch(m_ISOTable[iIdx]->m_ucEpcType)
			{
			case FEDM_ISC_EPC_TYPE_1:
				sprintf(cEpc, "%07lX", (unsigned long)m_ISOTable[iIdx]->GetEpcDomainManager());
				break;
			case FEDM_ISC_EPC_TYPE_2:
			case FEDM_ISC_EPC_TYPE_3:
			case FEDM_ISC_EPC_TYPE_4:
				sprintf(cEpc, "%06lX", (unsigned long)m_ISOTable[iIdx]->GetEpcDomainManager());
				break;
			default:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string&) - " << GetErrorText(FEDM_ERROR_UNKNOWN_EPC_TYPE));
				FEDM_RETURN(FEDM_ERROR_UNKNOWN_EPC_TYPE);
			}
			sData = cEpc;
			break;

		case FEDM_ISC_DATA_EPC_OBJECT:
			switch(m_ISOTable[iIdx]->m_ucEpcType)
			{
			case FEDM_ISC_EPC_TYPE_1:
				sprintf(cEpc, "%06lX", (unsigned long)m_ISOTable[iIdx]->GetEpcObjectClass());
				break;
			case FEDM_ISC_EPC_TYPE_2:
			case FEDM_ISC_EPC_TYPE_3:
			case FEDM_ISC_EPC_TYPE_4:
				sprintf(cEpc, "%05lX", (unsigned long)m_ISOTable[iIdx]->GetEpcObjectClass());
				break;
			default:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string&) - " << GetErrorText(FEDM_ERROR_UNKNOWN_EPC_TYPE));
				FEDM_RETURN(FEDM_ERROR_UNKNOWN_EPC_TYPE);
			}
			sData = cEpc;
			break;

		case FEDM_ISC_DATA_EPC_SNR:
			switch(m_ISOTable[iIdx]->m_ucEpcType)
			{
			case FEDM_ISC_EPC_TYPE_1:
				sprintf(cEpc, "%09lX", (unsigned long)m_ISOTable[iIdx]->GetEpcSnr());
				break;
			case FEDM_ISC_EPC_TYPE_2:
			case FEDM_ISC_EPC_TYPE_3:
			case FEDM_ISC_EPC_TYPE_4:
				sprintf(cEpc, "%06lX", (unsigned long)m_ISOTable[iIdx]->GetEpcSnr());
				break;
			default:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string&) - " << GetErrorText(FEDM_ERROR_UNKNOWN_EPC_TYPE));
				FEDM_RETURN(FEDM_ERROR_UNKNOWN_EPC_TYPE);
			}
			sData = cEpc;
			break;

		case FEDM_ISC_DATA_FSCI:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucFSCI, 1, sData));
			break;

		case FEDM_ISC_DATA_FWI:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucFWI, 1, sData));
			break;

		case FEDM_ISC_DATA_DSI:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucDSI, 1, sData));
			break;

		case FEDM_ISC_DATA_DRI:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucDRI, 1, sData));
			break;

		case FEDM_ISC_DATA_NAD:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucNad, 1, sData));
			break;

		case FEDM_ISC_DATA_CID:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucCid, 1, sData));
			break;

		case FEDM_ISC_DATA_VALUE:
			FEDM_CHK2(iErr, FEDM_ConvUIntToHexStr(m_ISOTable[iIdx]->m_uiValue, sData));
			break;

		case FEDM_ISC_DATA_ANT_COUNT:
			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucAntCount, 1, sData));
			break;

		case FEDM_ISC_DATA_ANT_NR:
		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
		case FEDM_ISC_DATA_IS_SELECTED:
		case FEDM_ISC_DATA_IS_EPC:
		case FEDM_ISC_DATA_SEC_STATUS:
		case FEDM_ISC_DATA_TxDB:
		case FEDM_ISC_DATA_RxDB:
		case FEDM_ISC_DATA_TxCB:
		case FEDM_ISC_DATA_RxCB:
		case FEDM_ISC_DATA_RxDB_EPC_BANK:
		case FEDM_ISC_DATA_TxDB_EPC_BANK:
		case FEDM_ISC_DATA_RxDB_TID_BANK:
		case FEDM_ISC_DATA_TxDB_TID_BANK:
		case FEDM_ISC_DATA_RxDB_RES_BANK:
		case FEDM_ISC_DATA_TxDB_RES_BANK:
		case FEDM_ISC_DATA_BLOCK_SIZE:
		case FEDM_ISC_DATA_IS_BLOCK_SIZE_SET:
		case FEDM_ISC_DATA_IS_ISO14443_4_INFO:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string&) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string&) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
	
	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string&) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
		
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string&) - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        		:	20.08.2003 / M. Hultsch
  Version       	:	03.00.07 / 30.04.2009 / M. Hultsch

  Function			:	set one data member in a table

  Parameters		:	int iIdx		-	index of table
						unsigned int uiTableID	-	identifies the table
						unsigned int uiDataID	-	identifies the table data
						bool bData		-	data

  Return value		:	0 or error code (<0)

***************************************************************************/
int FEDM_ISCReader::SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, bool bData)
{
	int iSelIdx = 0;

	if(iIdx < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool) - Index < 0");
		FEDM_RETURN(iIdx);
	}

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool) - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_IS_BLOCK_SIZE_SET:
			m_ISOTable[iIdx]->m_bIsBlockSizeSet = bData;
			break;

		case FEDM_ISC_DATA_IS_SELECTED:
			// search old selected entry and deselect it
			iSelIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
			if(iSelIdx >= 0)
				m_ISOTable[iSelIdx]->m_bIsSelected = false;

			m_ISOTable[iIdx]->m_bIsSelected = bData;
			break;

		case FEDM_ISC_DATA_IS_EPC:
			m_ISOTable[iIdx]->m_bIsEpc = bData;
			break;

		case FEDM_ISC_DATA_IS_SNR:
			m_ISOTable[iIdx]->m_bIsSnr = bData;
			break;

		case FEDM_ISC_DATA_IS_AFI:
			m_ISOTable[iIdx]->m_bIsAFI = bData;
			break;

		case FEDM_ISC_DATA_IS_SYSINFO:
			m_ISOTable[iIdx]->m_bIsSysInfo = bData;
			break;

		case FEDM_ISC_DATA_IS_ISO14443_4_INFO:
			m_ISOTable[iIdx]->m_bIsISO14443_4Info = bData;
			break;

		case FEDM_ISC_DATA_SNR_LEN:
		case FEDM_ISC_DATA_AFI:
		case FEDM_ISC_DATA_DSFID:
		case FEDM_ISC_DATA_CHIP_ID:
		case FEDM_ISC_DATA_BLOCK_SIZE:
		case FEDM_ISC_DATA_OPTINFO:
		case FEDM_ISC_DATA_PROTOINFO:
		case FEDM_ISC_DATA_TRINFO:
		case FEDM_ISC_DATA_FSCI:
		case FEDM_ISC_DATA_FWI:
		case FEDM_ISC_DATA_DSI:
		case FEDM_ISC_DATA_DRI:
		case FEDM_ISC_DATA_NAD:
		case FEDM_ISC_DATA_CID:
		case FEDM_ISC_DATA_EPC:
		case FEDM_ISC_DATA_EPC_TYPE:
		case FEDM_ISC_DATA_EPC_HEADER:
		case FEDM_ISC_DATA_EPC_DOMAIN:
		case FEDM_ISC_DATA_EPC_OBJECT:
		case FEDM_ISC_DATA_EPC_SNR:
		case FEDM_ISC_DATA_SNR:
		case FEDM_ISC_DATA_TRTYPE:
		case FEDM_ISC_DATA_MEM_SIZE:
		case FEDM_ISC_DATA_IC_REF:
		case FEDM_ISC_DATA_SEC_STATUS:
		case FEDM_ISC_DATA_TxDB:
		case FEDM_ISC_DATA_RxDB:
		case FEDM_ISC_DATA_TxCB:
		case FEDM_ISC_DATA_RxCB:
		case FEDM_ISC_DATA_RxDB_EPC_BANK:
		case FEDM_ISC_DATA_TxDB_EPC_BANK:
		case FEDM_ISC_DATA_RxDB_TID_BANK:
		case FEDM_ISC_DATA_TxDB_TID_BANK:
		case FEDM_ISC_DATA_RxDB_RES_BANK:
		case FEDM_ISC_DATA_TxDB_RES_BANK:
		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
		case FEDM_ISC_DATA_ANT_NR:
		case FEDM_ISC_DATA_ANT_COUNT:
		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
		
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool) - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        		:	17.11.2000 / M. Hultsch
  Version       	:	03.00.07 / 30.04.2009 / M. Hultsch

  Function			:	set one data member in a table

  Parameters		:	int iIdx		-	index of table
						unsigned int uiTableID	-	identifies the table
						unsigned int uiDataID	-	identifies the table data
						unsigned char ucData	-	data

  Return value		:	0 or error code (<0)

***************************************************************************/
int FEDM_ISCReader::SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, unsigned char ucData)
{
	if(iIdx < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar) - Index < 0");
		FEDM_RETURN(iIdx);
	}

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar) - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_AFI:
			m_ISOTable[iIdx]->m_ucAFI = ucData;
			break;

		case FEDM_ISC_DATA_DSFID:
			m_ISOTable[iIdx]->m_ucDsfID = ucData;
			break;

		case FEDM_ISC_DATA_CHIP_ID:
			m_ISOTable[iIdx]->m_ucChipID = ucData;
			break;

		case FEDM_ISC_DATA_IS_SELECTED:
			if(ucData)
				m_ISOTable[iIdx]->m_bIsSelected = true;
			else
				m_ISOTable[iIdx]->m_bIsSelected = false;
			break;

		case FEDM_ISC_DATA_IS_ISO14443_4_INFO:
			if(ucData)
				m_ISOTable[iIdx]->m_bIsISO14443_4Info = true;
			else
				m_ISOTable[iIdx]->m_bIsISO14443_4Info = false;
			break;

		case FEDM_ISC_DATA_IS_EPC:
			if(ucData)
				m_ISOTable[iIdx]->m_bIsEpc = true;
			else
				m_ISOTable[iIdx]->m_bIsEpc = false;
			break;

		case FEDM_ISC_DATA_IS_BLOCK_SIZE_SET:
			if(ucData)
				m_ISOTable[iIdx]->m_bIsBlockSizeSet = true;
			else
				m_ISOTable[iIdx]->m_bIsBlockSizeSet = false;
			break;

		case FEDM_ISC_DATA_BLOCK_SIZE:
			m_ISOTable[iIdx]->m_ucBlockSize = ucData;
			break;

		case FEDM_ISC_DATA_TRTYPE:
			m_ISOTable[iIdx]->m_ucTrType = ucData;
			break;

		case FEDM_ISC_DATA_EPC_TYPE:
			m_ISOTable[iIdx]->m_ucEpcType = ucData;
			break;

		case FEDM_ISC_DATA_IC_REF:
			m_ISOTable[iIdx]->m_ucICRef = ucData;
			break;

		case FEDM_ISC_DATA_SNR_LEN:
			m_ISOTable[iIdx]->m_ucSnrLen = ucData;
			break;

		case FEDM_ISC_DATA_TRINFO:
			m_ISOTable[iIdx]->m_ucTrInfo = ucData;
			break;

		case FEDM_ISC_DATA_OPTINFO:
			m_ISOTable[iIdx]->m_ucOptInfo = ucData;
			break;

		case FEDM_ISC_DATA_PROTOINFO:
			m_ISOTable[iIdx]->m_ucProtoInfo = ucData;
			break;

		case FEDM_ISC_DATA_FSCI:
			m_ISOTable[iIdx]->m_ucFSCI = ucData;
			break;

		case FEDM_ISC_DATA_FWI:
			m_ISOTable[iIdx]->m_ucFWI = ucData;
			break;

		case FEDM_ISC_DATA_DSI:
			m_ISOTable[iIdx]->m_ucDSI = ucData;
			break;

		case FEDM_ISC_DATA_DRI:
			m_ISOTable[iIdx]->m_ucDRI = ucData;
			break;

		case FEDM_ISC_DATA_NAD:
			m_ISOTable[iIdx]->m_ucNad = ucData;
			break;

		case FEDM_ISC_DATA_CID:
			m_ISOTable[iIdx]->m_ucCid = ucData;
			break;

		case FEDM_ISC_DATA_EPC:
		case FEDM_ISC_DATA_EPC_HEADER:
		case FEDM_ISC_DATA_EPC_DOMAIN:
		case FEDM_ISC_DATA_EPC_OBJECT:
		case FEDM_ISC_DATA_EPC_SNR:
		case FEDM_ISC_DATA_SNR:
		case FEDM_ISC_DATA_MEM_SIZE:
		case FEDM_ISC_DATA_SEC_STATUS:
		case FEDM_ISC_DATA_TxDB:
		case FEDM_ISC_DATA_RxDB:
		case FEDM_ISC_DATA_TxCB:
		case FEDM_ISC_DATA_RxCB:
		case FEDM_ISC_DATA_RxDB_EPC_BANK:
		case FEDM_ISC_DATA_TxDB_EPC_BANK:
		case FEDM_ISC_DATA_RxDB_TID_BANK:
		case FEDM_ISC_DATA_TxDB_TID_BANK:
		case FEDM_ISC_DATA_RxDB_RES_BANK:
		case FEDM_ISC_DATA_TxDB_RES_BANK:
		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
		case FEDM_ISC_DATA_ANT_NR:
		case FEDM_ISC_DATA_ANT_COUNT:
		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
		
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar) - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        		:	11.11.2003 / M. Hultsch
  Version       	:	03.00.07 / 30.04.2009 / M. Hultsch

  Function			:	set one data member in a table
						(one or more than one byte)

  Parameters		:	int iIdx		-	index of table
						unsigned int uiTableID	-	identifies the table
						unsigned int uiDataID	-	identifies the table data
						unsigned char* ucData	-	pointer to data buffer
						int iDataBufLen	-	length of ucData

  Return value		:	0 or error code (<0)

***************************************************************************/
int FEDM_ISCReader::SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, unsigned char* ucData, int iDataBufLen)
{
	FEDM_CHK3(ucData);

	if(iIdx < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - Index < 0");
		FEDM_RETURN(iIdx);
	}

	if(iDataBufLen == 0)
		return FEDM_OK; // nothing to do

	if(iDataBufLen < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
		FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
	}

	int iCnt;
	int iLen = 0;

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_SNR:
			if(iDataBufLen < 8 || iDataBufLen > FEDM_ISC_MAX_UID_LENGTH)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}

			for(iCnt=0; iCnt<iDataBufLen; iCnt++)
				m_ISOTable[iIdx]->m_ucSnr[iCnt] = ucData[iCnt];
			break;

		case FEDM_ISC_DATA_MEM_SIZE:
			if(iDataBufLen < 2)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}
			else if(iDataBufLen == 2)
			{
				iLen = 2;
			}
			else if(iDataBufLen > 2)
			{
				if(m_ISOTable[iIdx]->m_ucMemSizeLen == 3)
					iLen = 3;
				else
					iLen = 2;
			}

			for(iCnt=0; iCnt<iLen; iCnt++)
				m_ISOTable[iIdx]->m_ucMemSize[iCnt] = ucData[iCnt];
			break;

		case FEDM_ISC_DATA_SNR_LEN:
		case FEDM_ISC_DATA_TRTYPE:
		case FEDM_ISC_DATA_AFI:
		case FEDM_ISC_DATA_DSFID:
		case FEDM_ISC_DATA_CHIP_ID:
		case FEDM_ISC_DATA_IC_REF:
		case FEDM_ISC_DATA_FSCI:
		case FEDM_ISC_DATA_FWI:
		case FEDM_ISC_DATA_DSI:
		case FEDM_ISC_DATA_DRI:
		case FEDM_ISC_DATA_NAD:
		case FEDM_ISC_DATA_CID:
		case FEDM_ISC_DATA_IS_SELECTED:
		case FEDM_ISC_DATA_IS_EPC:
		case FEDM_ISC_DATA_BLOCK_SIZE:
		case FEDM_ISC_DATA_IS_BLOCK_SIZE_SET:
		case FEDM_ISC_DATA_OPTINFO:
		case FEDM_ISC_DATA_PROTOINFO:
		case FEDM_ISC_DATA_TRINFO:
		case FEDM_ISC_DATA_EPC_TYPE:
		case FEDM_ISC_DATA_EPC:
		case FEDM_ISC_DATA_EPC_HEADER:
		case FEDM_ISC_DATA_EPC_DOMAIN:
		case FEDM_ISC_DATA_EPC_OBJECT:
		case FEDM_ISC_DATA_EPC_SNR:
		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
		case FEDM_ISC_DATA_ANT_NR:
		case FEDM_ISC_DATA_ANT_COUNT:
		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		case FEDM_ISC_DATA_SEC_STATUS:
			if((unsigned int)iDataBufLen < m_ISOTable[iIdx]->m_ucSecStatus.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}
			memcpy(&m_ISOTable[iIdx]->m_ucSecStatus[0], ucData, m_ISOTable[iIdx]->m_ucSecStatus.size());
			break;

		case FEDM_ISC_DATA_TxDB:
			if((unsigned int)iDataBufLen < m_ISOTable[iIdx]->m_ucTxDB.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}
			memcpy(&m_ISOTable[iIdx]->m_ucTxDB[0], ucData, m_ISOTable[iIdx]->m_ucTxDB.size());
			break;

		case FEDM_ISC_DATA_RxDB:
			if((unsigned int)iDataBufLen < m_ISOTable[iIdx]->m_ucRxDB.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}
			memcpy(&m_ISOTable[iIdx]->m_ucRxDB[0], ucData, m_ISOTable[iIdx]->m_ucRxDB.size());
			break;

		case FEDM_ISC_DATA_TxCB:
			if(iDataBufLen < 16)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}
			memcpy(&m_ISOTable[iIdx]->m_ucTxCB[0], ucData, 16);
			break;

		case FEDM_ISC_DATA_RxCB:
			if(iDataBufLen < 16)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}
			memcpy(&m_ISOTable[iIdx]->m_ucRxCB[0], ucData, 16);
			break;

		case FEDM_ISC_DATA_RxDB_EPC_BANK:
			if((unsigned int)iDataBufLen < m_ISOTable[iIdx]->m_ucRxDB_EpcBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}
			memcpy(&m_ISOTable[iIdx]->m_ucRxDB_EpcBank[0], ucData, m_ISOTable[iIdx]->m_ucRxDB_EpcBank.size());
			break;

		case FEDM_ISC_DATA_TxDB_EPC_BANK:
			if((unsigned int)iDataBufLen < m_ISOTable[iIdx]->m_ucTxDB_EpcBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}
			memcpy(&m_ISOTable[iIdx]->m_ucTxDB_EpcBank[0], ucData, m_ISOTable[iIdx]->m_ucTxDB_EpcBank.size());
			break;

		case FEDM_ISC_DATA_RxDB_TID_BANK:
			if((unsigned int)iDataBufLen < m_ISOTable[iIdx]->m_ucRxDB_TidBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}
			memcpy(&m_ISOTable[iIdx]->m_ucRxDB_TidBank[0], ucData, m_ISOTable[iIdx]->m_ucRxDB_TidBank.size());
			break;

		case FEDM_ISC_DATA_TxDB_TID_BANK:
			if((unsigned int)iDataBufLen < m_ISOTable[iIdx]->m_ucTxDB_TidBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}
			memcpy(&m_ISOTable[iIdx]->m_ucTxDB_TidBank[0], ucData, m_ISOTable[iIdx]->m_ucTxDB_TidBank.size());
			break;

		case FEDM_ISC_DATA_RxDB_RES_BANK:
			if((unsigned int)iDataBufLen < m_ISOTable[iIdx]->m_ucRxDB_ResBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}
			memcpy(&m_ISOTable[iIdx]->m_ucRxDB_ResBank[0], ucData, m_ISOTable[iIdx]->m_ucRxDB_ResBank.size());
			break;

		case FEDM_ISC_DATA_TxDB_RES_BANK:
			if((unsigned int)iDataBufLen < m_ISOTable[iIdx]->m_ucTxDB_ResBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}
			memcpy(&m_ISOTable[iIdx]->m_ucTxDB_ResBank[0], ucData, m_ISOTable[iIdx]->m_ucTxDB_ResBank.size());
			break;

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
		
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        		:	17.11.2000 / M. Hultsch
  Version       	:	03.00.07 / 30.04.2009 / M. Hultsch

  Function			:	set one data member in a table

  Parameters		:	int iIdx		-	index of table
						unsigned int uiTableID	-	identifies the table
						unsigned int uiDataID	-	identifies the table data
						unsigned int uiData		-	data

  Return value		:	0 or error code (<0)

***************************************************************************/
int FEDM_ISCReader::SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, unsigned int uiData)
{
	if(iIdx < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint - Index < 0");
		FEDM_RETURN(iIdx);
	}

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_SNR_LEN:
			m_ISOTable[iIdx]->m_ucSnrLen = (unsigned char)uiData;
			break;

		case FEDM_ISC_DATA_AFI:
			m_ISOTable[iIdx]->m_ucAFI = (unsigned char)uiData;
			break;

		case FEDM_ISC_DATA_DSFID:
			m_ISOTable[iIdx]->m_ucDsfID = (unsigned char)uiData;
			break;

		case FEDM_ISC_DATA_CHIP_ID:
			m_ISOTable[iIdx]->m_ucChipID = (unsigned char)uiData;
			break;

		case FEDM_ISC_DATA_IS_SELECTED:
			if(uiData)
				m_ISOTable[iIdx]->m_bIsSelected = true;
			else
				m_ISOTable[iIdx]->m_bIsSelected = false;
			break;

		case FEDM_ISC_DATA_IS_ISO14443_4_INFO:
			if(uiData)
				m_ISOTable[iIdx]->m_bIsISO14443_4Info = true;
			else
				m_ISOTable[iIdx]->m_bIsISO14443_4Info = false;
			break;

		case FEDM_ISC_DATA_IS_EPC:
			if(uiData)
				m_ISOTable[iIdx]->m_bIsEpc = true;
			else
				m_ISOTable[iIdx]->m_bIsEpc = false;
			break;

		case FEDM_ISC_DATA_IS_BLOCK_SIZE_SET:
			if(uiData)
				m_ISOTable[iIdx]->m_bIsBlockSizeSet = true;
			else
				m_ISOTable[iIdx]->m_bIsBlockSizeSet = false;
			break;

		case FEDM_ISC_DATA_BLOCK_SIZE:
			m_ISOTable[iIdx]->m_ucBlockSize = (unsigned char)uiData;
			break;

		case FEDM_ISC_DATA_MEM_SIZE:
			if(m_ISOTable[iIdx]->m_ucMemSizeLen == 3)
			{
				m_ISOTable[iIdx]->m_ucMemSize[0] = (unsigned char)((uiData&0xFF0000) >> 16);
				m_ISOTable[iIdx]->m_ucMemSize[1] = (unsigned char)((uiData&0x00FF00) >> 8);
				m_ISOTable[iIdx]->m_ucMemSize[2] = (unsigned char)( uiData&0x0000FF);
			}
			else
			{
				m_ISOTable[iIdx]->m_ucMemSize[0] = (unsigned char)((uiData&0xFF00) >> 8);
				m_ISOTable[iIdx]->m_ucMemSize[1] = (unsigned char)( uiData&0x00FF);
			}
			break;

		case FEDM_ISC_DATA_TRINFO:
		case FEDM_ISC_DATA_OPTINFO:
		case FEDM_ISC_DATA_PROTOINFO:
		case FEDM_ISC_DATA_EPC:
		case FEDM_ISC_DATA_EPC_TYPE:
		case FEDM_ISC_DATA_EPC_HEADER:
		case FEDM_ISC_DATA_EPC_DOMAIN:
		case FEDM_ISC_DATA_EPC_OBJECT:
		case FEDM_ISC_DATA_EPC_SNR:
		case FEDM_ISC_DATA_SNR:
		case FEDM_ISC_DATA_TRTYPE:
		case FEDM_ISC_DATA_IC_REF:
		case FEDM_ISC_DATA_SEC_STATUS:
		case FEDM_ISC_DATA_FSCI:
		case FEDM_ISC_DATA_FWI:
		case FEDM_ISC_DATA_DSI:
		case FEDM_ISC_DATA_DRI:
		case FEDM_ISC_DATA_NAD:
		case FEDM_ISC_DATA_CID:
		case FEDM_ISC_DATA_TxDB:
		case FEDM_ISC_DATA_RxDB:
		case FEDM_ISC_DATA_TxCB:
		case FEDM_ISC_DATA_RxCB:
		case FEDM_ISC_DATA_RxDB_EPC_BANK:
		case FEDM_ISC_DATA_TxDB_EPC_BANK:
		case FEDM_ISC_DATA_RxDB_TID_BANK:
		case FEDM_ISC_DATA_TxDB_TID_BANK:
		case FEDM_ISC_DATA_RxDB_RES_BANK:
		case FEDM_ISC_DATA_TxDB_RES_BANK:
		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
		case FEDM_ISC_DATA_ANT_NR:
		case FEDM_ISC_DATA_ANT_COUNT:
		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
		
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        		:	20.08.2003 / M. Hultsch
  Version       	:	03.00.07 / 30.04.2009 / M. Hultsch

  Function			:	set one data member in a table

  Parameters		:	int iIdx		-	index of table
						unsigned int uiTableID	-	identifies the table
						unsigned int uiDataID	-	identifies the table data
						__int64 i64Data	-	data

  Return value		:	0 or error code (<0)

***************************************************************************/
int FEDM_ISCReader::SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, __int64 i64Data)
{
	int iErr = 0;

	if(iIdx < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64 - Index < 0");
		FEDM_RETURN(iIdx);
	}

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64 - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64 - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64 - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_SNR:
			FEDM_CHK2(iErr, FEDM_ConvInt64ToHexUChar(i64Data, &m_ISOTable[iIdx]->m_ucSnr[0], 8));
			break;		

		case FEDM_ISC_DATA_SNR_LEN:
		case FEDM_ISC_DATA_BLOCK_SIZE:
		case FEDM_ISC_DATA_AFI:
		case FEDM_ISC_DATA_DSFID:
		case FEDM_ISC_DATA_CHIP_ID:
		case FEDM_ISC_DATA_OPTINFO:
		case FEDM_ISC_DATA_PROTOINFO:
		case FEDM_ISC_DATA_TRINFO:
		case FEDM_ISC_DATA_EPC:
		case FEDM_ISC_DATA_EPC_TYPE:
		case FEDM_ISC_DATA_EPC_HEADER:
		case FEDM_ISC_DATA_EPC_DOMAIN:
		case FEDM_ISC_DATA_EPC_OBJECT:
		case FEDM_ISC_DATA_EPC_SNR:
		case FEDM_ISC_DATA_TRTYPE:
		case FEDM_ISC_DATA_MEM_SIZE:
		case FEDM_ISC_DATA_IC_REF:
		case FEDM_ISC_DATA_IS_SELECTED:
		case FEDM_ISC_DATA_IS_EPC:
		case FEDM_ISC_DATA_SEC_STATUS:
		case FEDM_ISC_DATA_FSCI:
		case FEDM_ISC_DATA_FWI:
		case FEDM_ISC_DATA_DSI:
		case FEDM_ISC_DATA_DRI:
		case FEDM_ISC_DATA_NAD:
		case FEDM_ISC_DATA_CID:
		case FEDM_ISC_DATA_TxDB:
		case FEDM_ISC_DATA_RxDB:
		case FEDM_ISC_DATA_TxCB:
		case FEDM_ISC_DATA_RxCB:
		case FEDM_ISC_DATA_IS_BLOCK_SIZE_SET:
		case FEDM_ISC_DATA_RxDB_EPC_BANK:
		case FEDM_ISC_DATA_TxDB_EPC_BANK:
		case FEDM_ISC_DATA_RxDB_TID_BANK:
		case FEDM_ISC_DATA_TxDB_TID_BANK:
		case FEDM_ISC_DATA_RxDB_RES_BANK:
		case FEDM_ISC_DATA_TxDB_RES_BANK:
		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
		case FEDM_ISC_DATA_ANT_NR:
		case FEDM_ISC_DATA_ANT_COUNT:
		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64 - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64 - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
		
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64 - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        		:	19.08.2003 / M. Hultsch
  Version       	:	03.00.07 /30.04.2009 / M. Hultsch

  Function			:	set one data member in a table

  Parameters		:	int iIdx		-	index of table
						unsigned int uiTableID	-	identifies the table
						unsigned int uiDataID	-	identifies the table data
						string sData	-	data

  Return value		:	0 or error code (<0)

***************************************************************************/
int FEDM_ISCReader::SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, string sData)
{
	int iErr = 0;

	if(iIdx < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string - Index < 0");
		FEDM_RETURN(iIdx);
	}

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_SNR:
			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar(sData, &m_ISOTable[iIdx]->m_ucSnr[0], m_ISOTable[iIdx]->m_ucSnrLen));
			break;		

		case FEDM_ISC_DATA_SNR_LEN:
			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar(sData, &m_ISOTable[iIdx]->m_ucSnrLen, 1));
			break;

		case FEDM_ISC_DATA_BLOCK_SIZE:
			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar(sData, &m_ISOTable[iIdx]->m_ucBlockSize, 1));
			break;		

		case FEDM_ISC_DATA_AFI:
			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar(sData, &m_ISOTable[iIdx]->m_ucAFI, 1));
			break;		

		case FEDM_ISC_DATA_DSFID:
			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar(sData, &m_ISOTable[iIdx]->m_ucDsfID, 1));
			break;		

		case FEDM_ISC_DATA_CHIP_ID:
			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar(sData, &m_ISOTable[iIdx]->m_ucChipID, 1));
			break;		

		case FEDM_ISC_DATA_OPTINFO:
		case FEDM_ISC_DATA_PROTOINFO:
		case FEDM_ISC_DATA_TRINFO:
		case FEDM_ISC_DATA_EPC:
		case FEDM_ISC_DATA_EPC_TYPE:
		case FEDM_ISC_DATA_EPC_HEADER:
		case FEDM_ISC_DATA_EPC_DOMAIN:
		case FEDM_ISC_DATA_EPC_OBJECT:
		case FEDM_ISC_DATA_EPC_SNR:
		case FEDM_ISC_DATA_TRTYPE:
		case FEDM_ISC_DATA_MEM_SIZE:
		case FEDM_ISC_DATA_IC_REF:
		case FEDM_ISC_DATA_IS_SELECTED:
		case FEDM_ISC_DATA_IS_EPC:
		case FEDM_ISC_DATA_SEC_STATUS:
		case FEDM_ISC_DATA_FSCI:
		case FEDM_ISC_DATA_FWI:
		case FEDM_ISC_DATA_DSI:
		case FEDM_ISC_DATA_DRI:
		case FEDM_ISC_DATA_NAD:
		case FEDM_ISC_DATA_CID:
		case FEDM_ISC_DATA_TxDB:
		case FEDM_ISC_DATA_RxDB:
		case FEDM_ISC_DATA_TxCB:
		case FEDM_ISC_DATA_RxCB:
		case FEDM_ISC_DATA_RxDB_EPC_BANK:
		case FEDM_ISC_DATA_TxDB_EPC_BANK:
		case FEDM_ISC_DATA_RxDB_TID_BANK:
		case FEDM_ISC_DATA_TxDB_TID_BANK:
		case FEDM_ISC_DATA_RxDB_RES_BANK:
		case FEDM_ISC_DATA_TxDB_RES_BANK:
		case FEDM_ISC_DATA_IS_BLOCK_SIZE_SET:
		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
		case FEDM_ISC_DATA_ANT_NR:
		case FEDM_ISC_DATA_ANT_COUNT:
		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
		
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        		:	17.11.2000 / M. Hultsch
  Version       	:	03.00.07 / 30.04.2009 / M. Hultsch

  Function			:	set one data member in a table

  Parameters		:	int iIdx		-	index of table
						unsigned int uiTableID	-	identifies the table
						unsigned int uiDataID	-	identifies the table data
						CString sData	-	data

  Return value		:	0 or error code (<0)

***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_ISCReader::SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, CString sData)
{
	int iErr = 0;

	if(iIdx < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString - Index < 0");
		FEDM_RETURN(iIdx);
	}

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_SNR:
			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar(sData, &m_ISOTable[iIdx]->m_ucSnr[0], m_ISOTable[iIdx]->m_ucSnrLen));
			break;		

		case FEDM_ISC_DATA_SNR_LEN:
			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar(sData, &m_ISOTable[iIdx]->m_ucSnrLen, 1));
			break;

		case FEDM_ISC_DATA_BLOCK_SIZE:
			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar(sData, &m_ISOTable[iIdx]->m_ucBlockSize, 1));
			break;		

		case FEDM_ISC_DATA_AFI:
			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar(sData, &m_ISOTable[iIdx]->m_ucAFI, 1));
			break;		

		case FEDM_ISC_DATA_DSFID:
			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar(sData, &m_ISOTable[iIdx]->m_ucDsfID, 1));
			break;		

		case FEDM_ISC_DATA_CHIP_ID:
			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar(sData, &m_ISOTable[iIdx]->m_ucChipID, 1));
			break;		

		case FEDM_ISC_DATA_OPTINFO:
		case FEDM_ISC_DATA_PROTOINFO:
		case FEDM_ISC_DATA_TRINFO:
		case FEDM_ISC_DATA_EPC:
		case FEDM_ISC_DATA_EPC_TYPE:
		case FEDM_ISC_DATA_EPC_HEADER:
		case FEDM_ISC_DATA_EPC_DOMAIN:
		case FEDM_ISC_DATA_EPC_OBJECT:
		case FEDM_ISC_DATA_EPC_SNR:
		case FEDM_ISC_DATA_TRTYPE:
		case FEDM_ISC_DATA_MEM_SIZE:
		case FEDM_ISC_DATA_IC_REF:
		case FEDM_ISC_DATA_IS_SELECTED:
		case FEDM_ISC_DATA_IS_EPC:
		case FEDM_ISC_DATA_SEC_STATUS:
		case FEDM_ISC_DATA_FSCI:
		case FEDM_ISC_DATA_FWI:
		case FEDM_ISC_DATA_DSI:
		case FEDM_ISC_DATA_DRI:
		case FEDM_ISC_DATA_NAD:
		case FEDM_ISC_DATA_CID:
		case FEDM_ISC_DATA_TxDB:
		case FEDM_ISC_DATA_RxDB:
		case FEDM_ISC_DATA_TxCB:
		case FEDM_ISC_DATA_RxCB:
		case FEDM_ISC_DATA_RxDB_EPC_BANK:
		case FEDM_ISC_DATA_TxDB_EPC_BANK:
		case FEDM_ISC_DATA_RxDB_TID_BANK:
		case FEDM_ISC_DATA_TxDB_TID_BANK:
		case FEDM_ISC_DATA_RxDB_RES_BANK:
		case FEDM_ISC_DATA_TxDB_RES_BANK:
		case FEDM_ISC_DATA_IS_BLOCK_SIZE_SET:
		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
		case FEDM_ISC_DATA_ANT_NR:
		case FEDM_ISC_DATA_ANT_COUNT:
		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
		
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_RETURN(FEDM_OK);
}
#endif

//////////////////////////////////////////////////////////////////////
// functions for data block access in a table
//////////////////////////////////////////////////////////////////////

/***************************************************************************
  Begin        		:	17.11.2000 / M. Hultsch
  Version       	:	03.00.08 / 12.05.2009 / M. Hultsch

  Function			:	get data block from a table

  Parameters		:	int iIdx		-	index of table
						unsigned int uiTableID	-	identifies the table
						unsigned int uiDataID	-	identifies the table data
						int iBlockNr	-	block number
						unsigned char* ucData	-	pointer to data buffer
						int iDataBufLen	-	length of ucData

  Return value		:	0 or error code (<0)

***************************************************************************/
int FEDM_ISCReader::GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, int iBlockNr, unsigned char* ucData, int iDataBufLen)
{
	FEDM_CHK3(ucData);

	if(iIdx < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - Index < 0");
		FEDM_RETURN(iIdx);
	}

	if(iDataBufLen == 0)
		return FEDM_OK; // nothing to do

	if(iDataBufLen < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
		FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
	}

	unsigned char ucBlockSize = 0x04;
	int iCnt = 0;

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
		if(m_BRMTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_BRMTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_RxDB:
			ucBlockSize = m_BRMTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (iBlockNr+1)*ucBlockSize > FEDM_ISC_BRM_TABLE_RxDB_SIZE)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}


			if(iDataBufLen < ucBlockSize)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}

			for(iCnt=0; iCnt<ucBlockSize; iCnt++)
				ucData[iCnt] = m_BRMTable[iIdx]->m_ucRxDB[(iBlockNr*ucBlockSize)+iCnt];
			
			break;

		case FEDM_ISC_DATA_ANT_NR:
			if(iBlockNr < 0 || iBlockNr >= FEDM_ISC_BRM_TABLE_MAX_ANTENNA)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			ucData[0] = m_BRMTable[iIdx]->m_ucAntNumber[iBlockNr];
			break;

		case FEDM_ISC_DATA_ANT_RSSI:
			if(iBlockNr < 0 || iBlockNr >= FEDM_ISC_BRM_TABLE_MAX_ANTENNA)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			ucData[0] = m_BRMTable[iIdx]->m_ucAntRSSI[iBlockNr];
			break;

		case FEDM_ISC_DATA_ANT_COUNT:
		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_SNR:
		case FEDM_ISC_DATA_TRTYPE:
		case FEDM_ISC_DATA_TIMER:
		case FEDM_ISC_DATA_DATE:
		case FEDM_ISC_DATA_DB_ADR:
		case FEDM_ISC_DATA_DBN:
		case FEDM_ISC_DATA_SNR_LEN:
		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
		case FEDM_ISC_DATA_MAC_ADR:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_SEC_STATUS:
			if(iBlockNr < 0 || (unsigned int)iBlockNr > m_ISOTable[iIdx]->m_ucSecStatus.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			ucData[0] = m_ISOTable[iIdx]->m_ucSecStatus[iBlockNr];
			break;

		case FEDM_ISC_DATA_TxDB:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucTxDB.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(iDataBufLen < ucBlockSize)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}

			for(iCnt=0; iCnt<ucBlockSize; iCnt++)
				ucData[iCnt] = m_ISOTable[iIdx]->m_ucTxDB[(iBlockNr*ucBlockSize)+iCnt];

			break;

		case FEDM_ISC_DATA_RxDB:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucRxDB.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(iDataBufLen < ucBlockSize)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}

			for(iCnt=0; iCnt<ucBlockSize; iCnt++)
				ucData[iCnt] = m_ISOTable[iIdx]->m_ucRxDB[(iBlockNr*ucBlockSize)+iCnt];
			
			break;

		case FEDM_ISC_DATA_TxDB_PLAIN:
			// iBlockNr is byte offset
			// iDataBufLen is number of bytes to be copied to
			if(iBlockNr < 0 || (unsigned int)(iBlockNr+iDataBufLen) >= m_ISOTable[iIdx]->m_ucTxDB.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			memcpy(ucData, &m_ISOTable[iIdx]->m_ucTxDB[iBlockNr], iDataBufLen);
			
			break;

		case FEDM_ISC_DATA_RxDB_PLAIN:
			// iBlockNr is byte offset
			// iDataBufLen is number of bytes to be copied to
			if(iBlockNr < 0 || (unsigned int)(iBlockNr+iDataBufLen) >= m_ISOTable[iIdx]->m_ucRxDB.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			memcpy(ucData, &m_ISOTable[iIdx]->m_ucRxDB[iBlockNr], iDataBufLen);
			
			break;

		case FEDM_ISC_DATA_TxCB:
			if(iBlockNr < 0 || iBlockNr >= 4)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(iDataBufLen < 4)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}
			
			for(iCnt=0; iCnt<4; iCnt++)
				ucData[iCnt] = m_ISOTable[iIdx]->m_ucTxCB[iBlockNr][iCnt];

			break;
		
		case FEDM_ISC_DATA_RxCB:
			if(iBlockNr < 0 || iBlockNr >= 4)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(iDataBufLen < 4)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}
			
			for(iCnt=0; iCnt<4; iCnt++)
				ucData[iCnt] = m_ISOTable[iIdx]->m_ucRxCB[iBlockNr][iCnt];

			break;

		case FEDM_ISC_DATA_RxDB_EPC_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucRxDB_EpcBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(iDataBufLen < ucBlockSize)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}

			for(iCnt=0; iCnt<ucBlockSize; iCnt++)
				ucData[iCnt] = m_ISOTable[iIdx]->m_ucRxDB_EpcBank[(iBlockNr*ucBlockSize)+iCnt];
			
			break;

		case FEDM_ISC_DATA_TxDB_EPC_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucTxDB_EpcBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(iDataBufLen < ucBlockSize)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}

			for(iCnt=0; iCnt<ucBlockSize; iCnt++)
				ucData[iCnt] = m_ISOTable[iIdx]->m_ucTxDB_EpcBank[(iBlockNr*ucBlockSize)+iCnt];

			break;

		case FEDM_ISC_DATA_RxDB_TID_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucRxDB_TidBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(iDataBufLen < ucBlockSize)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}

			for(iCnt=0; iCnt<ucBlockSize; iCnt++)
				ucData[iCnt] = m_ISOTable[iIdx]->m_ucRxDB_TidBank[(iBlockNr*ucBlockSize)+iCnt];
			
			break;

		case FEDM_ISC_DATA_TxDB_TID_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucTxDB_TidBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(iDataBufLen < ucBlockSize)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}

			for(iCnt=0; iCnt<ucBlockSize; iCnt++)
				ucData[iCnt] = m_ISOTable[iIdx]->m_ucTxDB_TidBank[(iBlockNr*ucBlockSize)+iCnt];

			break;

		case FEDM_ISC_DATA_RxDB_RES_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucRxDB_ResBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(iDataBufLen < ucBlockSize)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}

			for(iCnt=0; iCnt<ucBlockSize; iCnt++)
				ucData[iCnt] = m_ISOTable[iIdx]->m_ucRxDB_ResBank[(iBlockNr*ucBlockSize)+iCnt];
			
			break;

		case FEDM_ISC_DATA_TxDB_RES_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;

			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucTxDB_ResBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(iDataBufLen < ucBlockSize)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}

			for(iCnt=0; iCnt<ucBlockSize; iCnt++)
				ucData[iCnt] = m_ISOTable[iIdx]->m_ucTxDB_ResBank[(iBlockNr*ucBlockSize)+iCnt];

			break;

		case FEDM_ISC_DATA_ANT_NR:
			if(iBlockNr < 0 || iBlockNr >= FEDM_ISC_ISO_TABLE_MAX_ANTENNA)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			ucData[0] = m_ISOTable[iIdx]->m_ucAntNumber[iBlockNr];
			break;

		case FEDM_ISC_DATA_ANT_STATUS:
			if(iBlockNr < 0 || iBlockNr >= FEDM_ISC_ISO_TABLE_MAX_ANTENNA)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			ucData[0] = m_ISOTable[iIdx]->m_ucAntStatus[iBlockNr];
			break;

		case FEDM_ISC_DATA_ANT_RSSI:
			if(iBlockNr < 0 || iBlockNr >= FEDM_ISC_ISO_TABLE_MAX_ANTENNA)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			ucData[0] = m_ISOTable[iIdx]->m_ucAntRSSI[iBlockNr];
			break;

		case FEDM_ISC_DATA_ANT_COUNT:
		case FEDM_ISC_DATA_SNR_LEN:
		case FEDM_ISC_DATA_OPTINFO:
		case FEDM_ISC_DATA_PROTOINFO:
		case FEDM_ISC_DATA_TRINFO:
		case FEDM_ISC_DATA_EPC:
		case FEDM_ISC_DATA_EPC_TYPE:
		case FEDM_ISC_DATA_EPC_HEADER:
		case FEDM_ISC_DATA_EPC_DOMAIN:
		case FEDM_ISC_DATA_EPC_OBJECT:
		case FEDM_ISC_DATA_EPC_SNR:
		case FEDM_ISC_DATA_SNR:
		case FEDM_ISC_DATA_TRTYPE:
		case FEDM_ISC_DATA_AFI:
		case FEDM_ISC_DATA_DSFID:
		case FEDM_ISC_DATA_CHIP_ID:
		case FEDM_ISC_DATA_MEM_SIZE:
		case FEDM_ISC_DATA_IC_REF:
		case FEDM_ISC_DATA_IS_SELECTED:
		case FEDM_ISC_DATA_IS_EPC:
		case FEDM_ISC_DATA_BLOCK_SIZE:
		case FEDM_ISC_DATA_IS_BLOCK_SIZE_SET:
		case FEDM_ISC_DATA_FSCI:
		case FEDM_ISC_DATA_FWI:
		case FEDM_ISC_DATA_DSI:
		case FEDM_ISC_DATA_DRI:
		case FEDM_ISC_DATA_NAD:
		case FEDM_ISC_DATA_CID:
		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
		
	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        		:	19.08.2003 / M. Hultsch
  Version       	:	03.00.08 / 26.05.2009 / M. Hultsch

  Function			:	get data block from a table

  Parameters		:	int iIdx		-	index of table
						unsigned int uiTableID	-	identifies the table
						unsigned int uiDataID	-	identifies the table data
						int iBlockNr	-	block number
						string& sData	-	referenz of data

  Return value		:	0 or error code (<0)

***************************************************************************/
int FEDM_ISCReader::GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, int iBlockNr, string& sData)
{
	FEDM_CHK3(&sData);

	unsigned char ucBlockSize = 0x04;
	int iErr;

	if(iIdx < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string&) - Index < 0");
		FEDM_RETURN(iIdx);
	}

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
		if(m_BRMTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string&) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_BRMTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string&) - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_RxDB:
			ucBlockSize = m_BRMTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (iBlockNr+1)*ucBlockSize > FEDM_ISC_BRM_TABLE_RxDB_SIZE)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string&) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr( &m_BRMTable[iIdx]->m_ucRxDB[(iBlockNr*ucBlockSize)], 
														(int)ucBlockSize, 
														sData ));

			break;


		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
		case FEDM_ISC_DATA_ANT_COUNT:
		case FEDM_ISC_DATA_SNR:
		case FEDM_ISC_DATA_TRTYPE:
		case FEDM_ISC_DATA_TIMER:
		case FEDM_ISC_DATA_DATE:
		case FEDM_ISC_DATA_DB_ADR:
		case FEDM_ISC_DATA_DBN:
		case FEDM_ISC_DATA_ANT_NR:
		case FEDM_ISC_DATA_SNR_LEN:
		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
		case FEDM_ISC_DATA_MAC_ADR:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string&) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string&) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string&) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string&) - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_SEC_STATUS:
			if(iBlockNr < 0 || (unsigned int)iBlockNr > m_ISOTable[iIdx]->m_ucSecStatus.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string&) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr( &m_ISOTable[iIdx]->m_ucSecStatus[iBlockNr], 
														1, 
														sData ));

			break;

		case FEDM_ISC_DATA_TxDB:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucTxDB.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string&) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr( &m_ISOTable[iIdx]->m_ucTxDB[(iBlockNr*ucBlockSize)], 
														(int)ucBlockSize, 
														sData ));

			break;

		case FEDM_ISC_DATA_RxDB:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucRxDB.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string&) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr( &m_ISOTable[iIdx]->m_ucRxDB[(iBlockNr*ucBlockSize)], 
														(int)ucBlockSize, 
														sData ));

			break;

		case FEDM_ISC_DATA_TxCB:
			if(iBlockNr < 0 || iBlockNr >= 4)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string&) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr( &m_ISOTable[iIdx]->m_ucTxCB[iBlockNr][0], 
														4, 
														sData ));

			break;
		
		case FEDM_ISC_DATA_RxCB:
			if(iBlockNr < 0 || iBlockNr >= 4)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string&) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr( &m_ISOTable[iIdx]->m_ucRxCB[iBlockNr][0], 
														4, 
														sData ));

			break;

		case FEDM_ISC_DATA_RxDB_EPC_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucRxDB_EpcBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string&) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr( &m_ISOTable[iIdx]->m_ucRxDB_EpcBank[(iBlockNr*ucBlockSize)], 
														(int)ucBlockSize, 
														sData ));

			break;

		case FEDM_ISC_DATA_TxDB_EPC_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucTxDB_EpcBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string&) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr( &m_ISOTable[iIdx]->m_ucTxDB_EpcBank[(iBlockNr*ucBlockSize)], 
														(int)ucBlockSize, 
														sData ));

			break;

		case FEDM_ISC_DATA_RxDB_TID_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucRxDB_TidBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string&) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr( &m_ISOTable[iIdx]->m_ucRxDB_TidBank[(iBlockNr*ucBlockSize)], 
														(int)ucBlockSize, 
														sData ));

			break;

		case FEDM_ISC_DATA_TxDB_TID_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucTxDB_TidBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string&) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr( &m_ISOTable[iIdx]->m_ucTxDB_TidBank[(iBlockNr*ucBlockSize)], 
														(int)ucBlockSize, 
														sData ));

			break;

		case FEDM_ISC_DATA_RxDB_RES_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucRxDB_ResBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string&) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr( &m_ISOTable[iIdx]->m_ucRxDB_ResBank[(iBlockNr*ucBlockSize)], 
														(int)ucBlockSize, 
														sData ));

			break;

		case FEDM_ISC_DATA_TxDB_RES_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucTxDB_ResBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string&) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr( &m_ISOTable[iIdx]->m_ucTxDB_ResBank[(iBlockNr*ucBlockSize)], 
														(int)ucBlockSize, 
														sData ));

			break;


		case FEDM_ISC_DATA_ANT_NR:
		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
		case FEDM_ISC_DATA_ANT_COUNT:
		case FEDM_ISC_DATA_SNR_LEN:
		case FEDM_ISC_DATA_OPTINFO:
		case FEDM_ISC_DATA_PROTOINFO:
		case FEDM_ISC_DATA_TRINFO:
		case FEDM_ISC_DATA_EPC:
		case FEDM_ISC_DATA_EPC_TYPE:
		case FEDM_ISC_DATA_EPC_HEADER:
		case FEDM_ISC_DATA_EPC_DOMAIN:
		case FEDM_ISC_DATA_EPC_OBJECT:
		case FEDM_ISC_DATA_EPC_SNR:
		case FEDM_ISC_DATA_SNR:
		case FEDM_ISC_DATA_TRTYPE:
		case FEDM_ISC_DATA_AFI:
		case FEDM_ISC_DATA_DSFID:
		case FEDM_ISC_DATA_CHIP_ID:
		case FEDM_ISC_DATA_MEM_SIZE:
		case FEDM_ISC_DATA_IC_REF:
		case FEDM_ISC_DATA_IS_SELECTED:
		case FEDM_ISC_DATA_IS_EPC:
		case FEDM_ISC_DATA_BLOCK_SIZE:
		case FEDM_ISC_DATA_IS_BLOCK_SIZE_SET:
		case FEDM_ISC_DATA_FSCI:
		case FEDM_ISC_DATA_FWI:
		case FEDM_ISC_DATA_DSI:
		case FEDM_ISC_DATA_DRI:
		case FEDM_ISC_DATA_NAD:
		case FEDM_ISC_DATA_CID:
		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string&) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string&) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;

#if !defined(__BORLANDC__)
	case FEDM_ISC_CAD_TABLE:
		switch(uiDataID)
		{
		case FEDM_ISC_DATA_MISC:
			if(iBlockNr < 0 || (unsigned int)iBlockNr > 3)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string&) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			FEDM_CHK2(iErr, FEDM_ConvIntToHexStr( m_CADTable.m_nData[iBlockNr], sData ));
			break;

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string&) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
#endif

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string&) - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        		:	15.02.2001 / M. Hultsch
  Version       	:	03.00.07 / 30.04.2009 / M. Hultsch

  Function			:	get data block from a table

  Parameters		:	int iIdx		-	index of table
						unsigned int uiTableID	-	identifies the table
						unsigned int uiDataID	-	identifies the table data
						int iBlockNr	-	block number
						CString& sData	-	referenz of data

  Return value		:	0 or error code (<0)

***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_ISCReader::GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, int iBlockNr, CString& sData)
{
	FEDM_CHK3(&sData);

	unsigned char ucBlockSize = 0x04;
	int iErr;

	if(iIdx < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString&) - Index < 0");
		FEDM_RETURN(iIdx);
	}

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
		if(m_BRMTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString&) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_BRMTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString&) - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_RxDB:
			ucBlockSize = m_BRMTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || ((iBlockNr+1)*ucBlockSize) > FEDM_ISC_BRM_TABLE_RxDB_SIZE)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString&) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr( &m_BRMTable[iIdx]->m_ucRxDB[(iBlockNr*ucBlockSize)], 
														(int)ucBlockSize, 
														sData ));

			break;

		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
		case FEDM_ISC_DATA_ANT_COUNT:
		case FEDM_ISC_DATA_SNR:
		case FEDM_ISC_DATA_TRTYPE:
		case FEDM_ISC_DATA_TIMER:
		case FEDM_ISC_DATA_DATE:
		case FEDM_ISC_DATA_DB_ADR:
		case FEDM_ISC_DATA_DBN:
		case FEDM_ISC_DATA_ANT_NR:
		case FEDM_ISC_DATA_SNR_LEN:
		case FEDM_ISC_DATA_EPC:
		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
		case FEDM_ISC_DATA_MAC_ADR:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString&) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString&) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString&) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString&) - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_SEC_STATUS:
			if(iBlockNr < 0 || (unsigned int)iBlockNr > m_ISOTable[iIdx]->m_ucSecStatus.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString&) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr( &m_ISOTable[iIdx]->m_ucSecStatus[iBlockNr], 
														1, 
														sData ));

			break;

		case FEDM_ISC_DATA_TxDB:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucTxDB.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString&) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr( &m_ISOTable[iIdx]->m_ucTxDB[(iBlockNr*ucBlockSize)], 
														(int)ucBlockSize, 
														sData ));

			break;

		case FEDM_ISC_DATA_RxDB:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucRxDB.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString&) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr( &m_ISOTable[iIdx]->m_ucRxDB[(iBlockNr*ucBlockSize)], 
														(int)ucBlockSize, 
														sData ));

			break;

		case FEDM_ISC_DATA_TxCB:
			if(iBlockNr < 0 || iBlockNr >= 4)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString&) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr( &m_ISOTable[iIdx]->m_ucTxCB[iBlockNr][0], 
														4, 
														sData ));

			break;
		
		case FEDM_ISC_DATA_RxCB:
			if(iBlockNr < 0 || iBlockNr >= 4)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString&) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr( &m_ISOTable[iIdx]->m_ucRxCB[iBlockNr][0], 
														4, 
														sData ));

			break;

		case FEDM_ISC_DATA_RxDB_EPC_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucRxDB_EpcBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString&) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr( &m_ISOTable[iIdx]->m_ucRxDB_EpcBank[(iBlockNr*ucBlockSize)], 
														(int)ucBlockSize, 
														sData ));

			break;

		case FEDM_ISC_DATA_TxDB_EPC_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucTxDB_EpcBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString&) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr( &m_ISOTable[iIdx]->m_ucTxDB_EpcBank[(iBlockNr*ucBlockSize)], 
														(int)ucBlockSize, 
														sData ));

			break;

		case FEDM_ISC_DATA_RxDB_TID_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucRxDB_TidBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString&) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr( &m_ISOTable[iIdx]->m_ucRxDB_TidBank[(iBlockNr*ucBlockSize)], 
														(int)ucBlockSize, 
														sData ));

			break;

		case FEDM_ISC_DATA_TxDB_TID_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucTxDB_TidBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString&) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr( &m_ISOTable[iIdx]->m_ucTxDB_TidBank[(iBlockNr*ucBlockSize)], 
														(int)ucBlockSize, 
														sData ));

			break;

		case FEDM_ISC_DATA_RxDB_RES_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucRxDB_ResBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString&) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr( &m_ISOTable[iIdx]->m_ucRxDB_ResBank[(iBlockNr*ucBlockSize)], 
														(int)ucBlockSize, 
														sData ));

			break;

		case FEDM_ISC_DATA_TxDB_RES_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucTxDB_ResBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString&) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr( &m_ISOTable[iIdx]->m_ucTxDB_ResBank[(iBlockNr*ucBlockSize)], 
														(int)ucBlockSize, 
														sData ));

			break;

		case FEDM_ISC_DATA_ANT_NR:
		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
		case FEDM_ISC_DATA_ANT_COUNT:
		case FEDM_ISC_DATA_SNR_LEN:
		case FEDM_ISC_DATA_OPTINFO:
		case FEDM_ISC_DATA_PROTOINFO:
		case FEDM_ISC_DATA_TRINFO:
		case FEDM_ISC_DATA_EPC:
		case FEDM_ISC_DATA_EPC_TYPE:
		case FEDM_ISC_DATA_EPC_HEADER:
		case FEDM_ISC_DATA_EPC_DOMAIN:
		case FEDM_ISC_DATA_EPC_OBJECT:
		case FEDM_ISC_DATA_EPC_SNR:
		case FEDM_ISC_DATA_SNR:
		case FEDM_ISC_DATA_TRTYPE:
		case FEDM_ISC_DATA_AFI:
		case FEDM_ISC_DATA_DSFID:
		case FEDM_ISC_DATA_CHIP_ID:
		case FEDM_ISC_DATA_MEM_SIZE:
		case FEDM_ISC_DATA_IC_REF:
		case FEDM_ISC_DATA_IS_SELECTED:
		case FEDM_ISC_DATA_IS_EPC:
		case FEDM_ISC_DATA_BLOCK_SIZE:
		case FEDM_ISC_DATA_IS_BLOCK_SIZE_SET:
		case FEDM_ISC_DATA_FSCI:
		case FEDM_ISC_DATA_FWI:
		case FEDM_ISC_DATA_DSI:
		case FEDM_ISC_DATA_DRI:
		case FEDM_ISC_DATA_NAD:
		case FEDM_ISC_DATA_CID:
		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString&) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString&) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;

	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString&) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
		
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString&) - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_RETURN(FEDM_OK);
}
#endif

/***************************************************************************
  Begin        		:	17.11.2000 / M. Hultsch
  Version       	:	03.00.07 / 30.04.2009 / M. Hultsch

  Function			:	set data block in a table

  Parameters		:	int iIdx		-	index of table
						unsigned int uiTableID	-	identifies the table
						unsigned int uiDataID	-	identifies the table data
						int iBlockNr	-	block number
						unsigned char* ucData	-	pointer to data buffer
						int iDataBufLen	-	length of ucData

  Return value		:	0 or error code (<0)

***************************************************************************/
int FEDM_ISCReader::SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, int iBlockNr, unsigned char* ucData, int iDataBufLen)
{
	FEDM_CHK3(ucData);

	if(iDataBufLen == 0)
		return FEDM_OK; // nothing to do

	if(iDataBufLen < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
		FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
	}

	unsigned char ucBlockSize = 0x04;
	int iCnt;

	if(iIdx < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - Index < 0");
		FEDM_RETURN(iIdx);
	}

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_SEC_STATUS:
			if(iBlockNr < 0 || (unsigned int)iBlockNr > m_ISOTable[iIdx]->m_ucSecStatus.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			m_ISOTable[iIdx]->m_ucSecStatus[iBlockNr] = ucData[0];
			break;

		case FEDM_ISC_DATA_TxDB:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucTxDB.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(iDataBufLen < ucBlockSize)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}

			for(iCnt=0; iCnt<ucBlockSize; iCnt++)
				m_ISOTable[iIdx]->m_ucTxDB[(iBlockNr*ucBlockSize)+iCnt] = ucData[iCnt];

			break;

		case FEDM_ISC_DATA_RxDB:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucRxDB.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(iDataBufLen < ucBlockSize)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}

			for(iCnt=0; iCnt<ucBlockSize; iCnt++)
				m_ISOTable[iIdx]->m_ucRxDB[(iBlockNr*ucBlockSize)+iCnt] = ucData[iCnt];

			break;

		case FEDM_ISC_DATA_TxDB_PLAIN:
			// iBlockNr is byte offset
			// iDataBufLen is number of bytes to be copied to
			if(iBlockNr < 0 || (unsigned int)(iBlockNr+iDataBufLen) >= m_ISOTable[iIdx]->m_ucTxDB.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			memcpy(&m_ISOTable[iIdx]->m_ucTxDB[iBlockNr], ucData, iDataBufLen);
			
			break;

		case FEDM_ISC_DATA_RxDB_PLAIN:
			// iBlockNr is byte offset
			// iDataBufLen is number of bytes to be copied to
			if(iBlockNr < 0 || (unsigned int)(iBlockNr+iDataBufLen) >= m_ISOTable[iIdx]->m_ucRxDB.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			memcpy(&m_ISOTable[iIdx]->m_ucRxDB[iBlockNr], ucData, iDataBufLen);
			
			break;

		case FEDM_ISC_DATA_TxCB:
			if(iBlockNr < 0 || iBlockNr >= 4)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(iDataBufLen < 4)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}

			for(iCnt=0; iCnt<4; iCnt++)
				m_ISOTable[iIdx]->m_ucTxCB[iBlockNr][iCnt] = ucData[iCnt];

			break;
		
		case FEDM_ISC_DATA_RxCB:
			if(iBlockNr < 0 || iBlockNr >= 4)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(iDataBufLen < 4)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}

			for(iCnt=0; iCnt<4; iCnt++)
				m_ISOTable[iIdx]->m_ucRxCB[iBlockNr][iCnt] = ucData[iCnt];

			break;

		case FEDM_ISC_DATA_RxDB_EPC_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucRxDB_EpcBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(iDataBufLen < ucBlockSize)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}

			for(iCnt=0; iCnt<ucBlockSize; iCnt++)
				m_ISOTable[iIdx]->m_ucRxDB_EpcBank[(iBlockNr*ucBlockSize)+iCnt] = ucData[iCnt];

			break;

		case FEDM_ISC_DATA_TxDB_EPC_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucTxDB_EpcBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(iDataBufLen < ucBlockSize)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}

			for(iCnt=0; iCnt<ucBlockSize; iCnt++)
				m_ISOTable[iIdx]->m_ucTxDB_EpcBank[(iBlockNr*ucBlockSize)+iCnt] = ucData[iCnt];

			break;

		case FEDM_ISC_DATA_RxDB_TID_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucRxDB_TidBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(iDataBufLen < ucBlockSize)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}

			for(iCnt=0; iCnt<ucBlockSize; iCnt++)
				m_ISOTable[iIdx]->m_ucRxDB_TidBank[(iBlockNr*ucBlockSize)+iCnt] = ucData[iCnt];

			break;

		case FEDM_ISC_DATA_TxDB_TID_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucTxDB_TidBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(iDataBufLen < ucBlockSize)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}

			for(iCnt=0; iCnt<ucBlockSize; iCnt++)
				m_ISOTable[iIdx]->m_ucTxDB_TidBank[(iBlockNr*ucBlockSize)+iCnt] = ucData[iCnt];

			break;

		case FEDM_ISC_DATA_RxDB_RES_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucRxDB_ResBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(iDataBufLen < ucBlockSize)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}

			for(iCnt=0; iCnt<ucBlockSize; iCnt++)
				m_ISOTable[iIdx]->m_ucRxDB_ResBank[(iBlockNr*ucBlockSize)+iCnt] = ucData[iCnt];

			break;

		case FEDM_ISC_DATA_TxDB_RES_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucTxDB_ResBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(iDataBufLen < ucBlockSize)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
			}

			for(iCnt=0; iCnt<ucBlockSize; iCnt++)
				m_ISOTable[iIdx]->m_ucTxDB_ResBank[(iBlockNr*ucBlockSize)+iCnt] = ucData[iCnt];

			break;

		case FEDM_ISC_DATA_ANT_NR:
		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
		case FEDM_ISC_DATA_ANT_COUNT:
		case FEDM_ISC_DATA_SNR_LEN:
		case FEDM_ISC_DATA_OPTINFO:
		case FEDM_ISC_DATA_PROTOINFO:
		case FEDM_ISC_DATA_TRINFO:
		case FEDM_ISC_DATA_EPC:
		case FEDM_ISC_DATA_EPC_TYPE:
		case FEDM_ISC_DATA_EPC_HEADER:
		case FEDM_ISC_DATA_EPC_DOMAIN:
		case FEDM_ISC_DATA_EPC_OBJECT:
		case FEDM_ISC_DATA_EPC_SNR:
		case FEDM_ISC_DATA_SNR:
		case FEDM_ISC_DATA_TRTYPE:
		case FEDM_ISC_DATA_AFI:
		case FEDM_ISC_DATA_DSFID:
		case FEDM_ISC_DATA_CHIP_ID:
		case FEDM_ISC_DATA_MEM_SIZE:
		case FEDM_ISC_DATA_IC_REF:
		case FEDM_ISC_DATA_IS_SELECTED:
		case FEDM_ISC_DATA_IS_EPC:
		case FEDM_ISC_DATA_BLOCK_SIZE:
		case FEDM_ISC_DATA_IS_BLOCK_SIZE_SET:
		case FEDM_ISC_DATA_FSCI:
		case FEDM_ISC_DATA_FWI:
		case FEDM_ISC_DATA_DSI:
		case FEDM_ISC_DATA_DRI:
		case FEDM_ISC_DATA_NAD:
		case FEDM_ISC_DATA_CID:
		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
		
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", uchar*, " << iDataBufLen << ") - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        		:	19.08.2003 / M. Hultsch
  Version       	:	03.00.07 / 30.04.2009 / M. Hultsch

  Function			:	set data block in a table

  Parameters		:	int iIdx		-	index of table
						unsigned int uiTableID	-	identifies the table
						unsigned int uiDataID	-	identifies the table data
						int iBlockNr	-	block number
						string sData	-	data

  Return value		:	0 or error code (<0)

***************************************************************************/
int FEDM_ISCReader::SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, int iBlockNr, string sData)
{
	unsigned char ucBlockSize = 0x04;
	int iErr;

	if(iIdx < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - Index < 0");
		FEDM_RETURN(iIdx);
	}

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_SEC_STATUS:
			if(iBlockNr < 0 || (unsigned int)iBlockNr > m_ISOTable[iIdx]->m_ucSecStatus.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}


			if(sData.size()%2)	// number of chars in data string has to be even
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_ODD_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_ODD_STRING_LENGTH);
			}

			if((sData.size()/2) != (basic_string <char>::size_type)ucBlockSize)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar( sData,
													   &m_ISOTable[iIdx]->m_ucSecStatus[iBlockNr],
													   1 ));
			break;

		case FEDM_ISC_DATA_TxDB:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucTxDB.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(sData.size()%2)	// number of chars in data string has to be even
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_ODD_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_ODD_STRING_LENGTH);
			}

			if((sData.size()/2) != (basic_string <char>::size_type)ucBlockSize)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar( sData,
													   &m_ISOTable[iIdx]->m_ucTxDB[(iBlockNr*ucBlockSize)],
													   (int)ucBlockSize ));

			break;

		case FEDM_ISC_DATA_RxDB:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucRxDB.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(sData.size()%2)	// number of chars in data string has to be even
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_ODD_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_ODD_STRING_LENGTH);
			}

			if((sData.size()/2) != (basic_string <char>::size_type)ucBlockSize)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar( sData,
													   &m_ISOTable[iIdx]->m_ucRxDB[(iBlockNr*ucBlockSize)],
													   (int)ucBlockSize ));

			break;

		case FEDM_ISC_DATA_TxCB:
			if(iBlockNr < 0 || iBlockNr >= 4)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(sData.size()%2)	// number of chars in data string has to be even
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_ODD_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_ODD_STRING_LENGTH);
			}

			if((sData.size()/2) != 4)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar( sData,
													   &m_ISOTable[iIdx]->m_ucTxCB[iBlockNr][0],

													   4 ));

			break;
		
		case FEDM_ISC_DATA_RxCB:
			if(iBlockNr < 0 || iBlockNr >= 4)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(sData.size()%2)	// number of chars in data string has to be even
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_ODD_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_ODD_STRING_LENGTH);
			}

			if((sData.size()/2) != 4)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar( sData,
													   &m_ISOTable[iIdx]->m_ucRxCB[iBlockNr][0],
													   4 ));

			break;

		case FEDM_ISC_DATA_RxDB_EPC_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucRxDB_EpcBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(sData.size()%2)	// number of chars in data string has to be even
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_ODD_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_ODD_STRING_LENGTH);
			}

			if((sData.size()/2) != (basic_string <char>::size_type)ucBlockSize)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar( sData,
													   &m_ISOTable[iIdx]->m_ucRxDB_EpcBank[(iBlockNr*ucBlockSize)],
													   (int)ucBlockSize ));

			break;

		case FEDM_ISC_DATA_TxDB_EPC_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucTxDB_EpcBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(sData.size()%2)	// number of chars in data string has to be even
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_ODD_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_ODD_STRING_LENGTH);
			}

			if((sData.size()/2) != (basic_string <char>::size_type)ucBlockSize)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar( sData,
													   &m_ISOTable[iIdx]->m_ucTxDB_EpcBank[(iBlockNr*ucBlockSize)],
													   (int)ucBlockSize ));

			break;

		case FEDM_ISC_DATA_RxDB_TID_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucRxDB_TidBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(sData.size()%2)	// number of chars in data string has to be even
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_ODD_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_ODD_STRING_LENGTH);
			}

			if((sData.size()/2) != (basic_string <char>::size_type)ucBlockSize)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar( sData,
													   &m_ISOTable[iIdx]->m_ucRxDB_TidBank[(iBlockNr*ucBlockSize)],
													   (int)ucBlockSize ));

			break;

		case FEDM_ISC_DATA_TxDB_TID_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucTxDB_TidBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(sData.size()%2)	// number of chars in data string has to be even
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_ODD_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_ODD_STRING_LENGTH);
			}

			if((sData.size()/2) != (basic_string <char>::size_type)ucBlockSize)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar( sData,
													   &m_ISOTable[iIdx]->m_ucTxDB_TidBank[(iBlockNr*ucBlockSize)],
													   (int)ucBlockSize ));

			break;

		case FEDM_ISC_DATA_RxDB_RES_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucRxDB_ResBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(sData.size()%2)	// number of chars in data string has to be even
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_ODD_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_ODD_STRING_LENGTH);
			}

			if((sData.size()/2) != (basic_string <char>::size_type)ucBlockSize)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar( sData,
													   &m_ISOTable[iIdx]->m_ucRxDB_ResBank[(iBlockNr*ucBlockSize)],
													   (int)ucBlockSize ));

			break;

		case FEDM_ISC_DATA_TxDB_RES_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucTxDB_ResBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

			if(sData.size()%2)	// number of chars in data string has to be even
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_ODD_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_ODD_STRING_LENGTH);
			}

			if((sData.size()/2) != (basic_string <char>::size_type)ucBlockSize)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar( sData,
													   &m_ISOTable[iIdx]->m_ucTxDB_ResBank[(iBlockNr*ucBlockSize)],
													   (int)ucBlockSize ));

			break;

		case FEDM_ISC_DATA_ANT_NR:
		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
		case FEDM_ISC_DATA_ANT_COUNT:
		case FEDM_ISC_DATA_SNR_LEN:
		case FEDM_ISC_DATA_OPTINFO:
		case FEDM_ISC_DATA_PROTOINFO:
		case FEDM_ISC_DATA_TRINFO:
		case FEDM_ISC_DATA_EPC:
		case FEDM_ISC_DATA_EPC_TYPE:
		case FEDM_ISC_DATA_EPC_HEADER:
		case FEDM_ISC_DATA_EPC_DOMAIN:
		case FEDM_ISC_DATA_EPC_OBJECT:
		case FEDM_ISC_DATA_EPC_SNR:
		case FEDM_ISC_DATA_SNR:
		case FEDM_ISC_DATA_TRTYPE:
		case FEDM_ISC_DATA_AFI:
		case FEDM_ISC_DATA_DSFID:
		case FEDM_ISC_DATA_CHIP_ID:
		case FEDM_ISC_DATA_MEM_SIZE:
		case FEDM_ISC_DATA_IC_REF:
		case FEDM_ISC_DATA_IS_SELECTED:
		case FEDM_ISC_DATA_IS_EPC:
		case FEDM_ISC_DATA_BLOCK_SIZE:
		case FEDM_ISC_DATA_IS_BLOCK_SIZE_SET:
		case FEDM_ISC_DATA_FSCI:
		case FEDM_ISC_DATA_FWI:
		case FEDM_ISC_DATA_DSI:
		case FEDM_ISC_DATA_DRI:
		case FEDM_ISC_DATA_NAD:
		case FEDM_ISC_DATA_CID:
		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
		
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", string) - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        		:	15.02.2001 / M. Hultsch
  Version       	:	03.00.07 / 30.04.2009 / M. Hultsch

  Function			:	set data block in a table

  Parameters		:	int iIdx		-	index of table
						unsigned int uiTableID	-	identifies the table
						unsigned int uiDataID	-	identifies the table data
						int iBlockNr	-	block number
						CString sData	-	data

  Return value		:	0 or error code (<0)

***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_ISCReader::SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, int iBlockNr, CString sData)
{
	unsigned char ucBlockSize = 0x04;
	int iErr;

	if(iIdx < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - Index < 0");
		FEDM_RETURN(iIdx);
	}

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if((unsigned int)iIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		switch(uiDataID)
		{
		case FEDM_ISC_DATA_SEC_STATUS:
			if(iBlockNr < 0 || (unsigned int)iBlockNr > m_ISOTable[iIdx]->m_ucSecStatus.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}


#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
			if(sData.GetLength()%2)	// number of chars in data string has to be even
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
			if(sData.Length()%2)	// number of chars in data string has to be even
#endif
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_ODD_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_ODD_STRING_LENGTH);
			}

#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
			if((sData.GetLength()/2) != (int)ucBlockSize)
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
			if((sData.Length()/2) != (int)ucBlockSize)
#endif
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar( sData,
													   &m_ISOTable[iIdx]->m_ucSecStatus[iBlockNr],
													   1 ));
			break;

		case FEDM_ISC_DATA_TxDB:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucTxDB.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
			if(sData.GetLength()%2)	// number of chars in data string has to be even
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
			if(sData.Length()%2)	// number of chars in data string has to be even
#endif
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_ODD_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_ODD_STRING_LENGTH);
			}

#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
			if((sData.GetLength()/2) != (int)ucBlockSize)
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
			if((sData.Length()/2) != (int)ucBlockSize)
#endif
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar( sData,
													   &m_ISOTable[iIdx]->m_ucTxDB[(iBlockNr*ucBlockSize)],
													   (int)ucBlockSize ));

			break;

		case FEDM_ISC_DATA_RxDB:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucRxDB.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
			if(sData.GetLength()%2)	// number of chars in data string has to be even
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
			if(sData.Length()%2)	// number of chars in data string has to be even
#endif
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_ODD_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_ODD_STRING_LENGTH);
			}

#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
			if((sData.GetLength()/2) != (int)ucBlockSize)
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
			if((sData.Length()/2) != (int)ucBlockSize)
#endif
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar( sData,
													   &m_ISOTable[iIdx]->m_ucRxDB[(iBlockNr*ucBlockSize)],
													   (int)ucBlockSize ));

			break;

		case FEDM_ISC_DATA_TxCB:
			if(iBlockNr < 0 || iBlockNr >= 4)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
			if(sData.GetLength()%2)	// number of chars in data string has to be even
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
			if(sData.Length()%2)	// number of chars in data string has to be even
#endif
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_ODD_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_ODD_STRING_LENGTH);
			}

#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
			if((sData.GetLength()/2) != 4)
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
			if((sData.Length()/2) != 4)
#endif
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar( sData,
													   &m_ISOTable[iIdx]->m_ucTxCB[iBlockNr][0],

													   4 ));

			break;
		
		case FEDM_ISC_DATA_RxCB:
			if(iBlockNr < 0 || iBlockNr >= 4)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
			if(sData.GetLength()%2)	// number of chars in data string has to be even
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
			if(sData.Length()%2)	// number of chars in data string has to be even
#endif
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_ODD_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_ODD_STRING_LENGTH);
			}

#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
			if((sData.GetLength()/2) != 4)
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
			if((sData.Length()/2) != 4)
#endif
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar( sData,
													   &m_ISOTable[iIdx]->m_ucRxCB[iBlockNr][0],
													   4 ));

			break;

		case FEDM_ISC_DATA_RxDB_EPC_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucRxDB_EpcBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
			if(sData.GetLength()%2)	// number of chars in data string has to be even
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
			if(sData.Length()%2)	// number of chars in data string has to be even
#endif
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_ODD_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_ODD_STRING_LENGTH);
			}

#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
			if((sData.GetLength()/2) != (int)ucBlockSize)
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
			if((sData.Length()/2) != (int)ucBlockSize)
#endif
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar( sData,
													   &m_ISOTable[iIdx]->m_ucRxDB_EpcBank[(iBlockNr*ucBlockSize)],
													   (int)ucBlockSize ));

			break;

		case FEDM_ISC_DATA_TxDB_EPC_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucTxDB_EpcBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
			if(sData.GetLength()%2)	// number of chars in data string has to be even
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
			if(sData.Length()%2)	// number of chars in data string has to be even
#endif
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_ODD_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_ODD_STRING_LENGTH);
			}

#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
			if((sData.GetLength()/2) != (int)ucBlockSize)
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
			if((sData.Length()/2) != (int)ucBlockSize)
#endif
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar( sData,
													   &m_ISOTable[iIdx]->m_ucTxDB_EpcBank[(iBlockNr*ucBlockSize)],
													   (int)ucBlockSize ));

			break;

		case FEDM_ISC_DATA_RxDB_TID_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucRxDB_TidBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
			if(sData.GetLength()%2)	// number of chars in data string has to be even
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
			if(sData.Length()%2)	// number of chars in data string has to be even
#endif
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_ODD_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_ODD_STRING_LENGTH);
			}

#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
			if((sData.GetLength()/2) != (int)ucBlockSize)
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
			if((sData.Length()/2) != (int)ucBlockSize)
#endif
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar( sData,
													   &m_ISOTable[iIdx]->m_ucRxDB_TidBank[(iBlockNr*ucBlockSize)],
													   (int)ucBlockSize ));

			break;

		case FEDM_ISC_DATA_TxDB_TID_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucTxDB_TidBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
			if(sData.GetLength()%2)	// number of chars in data string has to be even
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
			if(sData.Length()%2)	// number of chars in data string has to be even
#endif
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_ODD_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_ODD_STRING_LENGTH);
			}

#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
			if((sData.GetLength()/2) != (int)ucBlockSize)
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
			if((sData.Length()/2) != (int)ucBlockSize)
#endif
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar( sData,
													   &m_ISOTable[iIdx]->m_ucTxDB_TidBank[(iBlockNr*ucBlockSize)],
													   (int)ucBlockSize ));

			break;

		case FEDM_ISC_DATA_RxDB_RES_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucRxDB_ResBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
			if(sData.GetLength()%2)	// number of chars in data string has to be even
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
			if(sData.Length()%2)	// number of chars in data string has to be even
#endif
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_ODD_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_ODD_STRING_LENGTH);
			}

#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
			if((sData.GetLength()/2) != (int)ucBlockSize)
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
			if((sData.Length()/2) != (int)ucBlockSize)
#endif
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar( sData,
													   &m_ISOTable[iIdx]->m_ucRxDB_ResBank[(iBlockNr*ucBlockSize)],
													   (int)ucBlockSize ));

			break;

		case FEDM_ISC_DATA_TxDB_RES_BANK:
			ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
			
			if(iBlockNr < 0 || (unsigned int)((iBlockNr+1)*ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucTxDB_ResBank.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
			}

#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
			if(sData.GetLength()%2)	// number of chars in data string has to be even
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
			if(sData.Length()%2)	// number of chars in data string has to be even
#endif
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_ODD_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_ODD_STRING_LENGTH);
			}

#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
			if((sData.GetLength()/2) != (int)ucBlockSize)
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
			if((sData.Length()/2) != (int)ucBlockSize)
#endif
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
				FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
			}

			FEDM_CHK2(iErr, FEDM_ConvHexStrToHexUChar( sData,
													   &m_ISOTable[iIdx]->m_ucTxDB_ResBank[(iBlockNr*ucBlockSize)],
													   (int)ucBlockSize ));

			break;

		case FEDM_ISC_DATA_ANT_NR:
		case FEDM_ISC_DATA_ANT_STATUS:
		case FEDM_ISC_DATA_ANT_RSSI:
		case FEDM_ISC_DATA_ANT_COUNT:
		case FEDM_ISC_DATA_SNR_LEN:
		case FEDM_ISC_DATA_OPTINFO:
		case FEDM_ISC_DATA_PROTOINFO:
		case FEDM_ISC_DATA_TRINFO:
		case FEDM_ISC_DATA_EPC:
		case FEDM_ISC_DATA_EPC_TYPE:
		case FEDM_ISC_DATA_EPC_HEADER:
		case FEDM_ISC_DATA_EPC_DOMAIN:
		case FEDM_ISC_DATA_EPC_OBJECT:
		case FEDM_ISC_DATA_EPC_SNR:
		case FEDM_ISC_DATA_SNR:
		case FEDM_ISC_DATA_TRTYPE:
		case FEDM_ISC_DATA_AFI:
		case FEDM_ISC_DATA_DSFID:
		case FEDM_ISC_DATA_CHIP_ID:
		case FEDM_ISC_DATA_MEM_SIZE:
		case FEDM_ISC_DATA_IC_REF:
		case FEDM_ISC_DATA_IS_SELECTED:
		case FEDM_ISC_DATA_IS_EPC:
		case FEDM_ISC_DATA_BLOCK_SIZE:
		case FEDM_ISC_DATA_IS_BLOCK_SIZE_SET:
		case FEDM_ISC_DATA_FSCI:
		case FEDM_ISC_DATA_FWI:
		case FEDM_ISC_DATA_DSI:
		case FEDM_ISC_DATA_DRI:
		case FEDM_ISC_DATA_NAD:
		case FEDM_ISC_DATA_CID:
		case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		break;
		
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", " << iBlockNr << ", CString) - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_RETURN(FEDM_OK);
}
#endif

//////////////////////////////////////////////////////////////////////
// functions for verify data blocks in a table
//////////////////////////////////////////////////////////////////////

/***************************************************************************
  Begin        		:	28.03.2002 / M. Hultsch
  Version       	:	03.00.05 / 26.03.2009 / M. Hultsch

  Function			:	verify read data block(s) with write data block(s) in a table

  Parameters		:	int iIdx		-	index of table
						unsigned int uiTableID	-	identifies the table
						int iBlockNr	-	first block number
						int iBlockCnt	-	number of blocks to verify

  Return value		:	0 or error code (<0)

***************************************************************************/
int FEDM_ISCReader::VerifyTableData(int iIdx, unsigned int uiTableID, int iBlockNr, int iBlockCnt)
{
	int iCnt = 0;

	if(iIdx < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", " << iBlockNr << ", " << iBlockCnt << ") - Index < 0");
		FEDM_RETURN(iIdx);
	}

	if(iBlockCnt<=0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", " << iBlockNr << ", " << iBlockCnt << ") - " << GetErrorText(FEDM_ERROR_PARAMETER));
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", " << iBlockNr << ", " << iBlockCnt << ") - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", " << iBlockNr << ", " << iBlockCnt << ") - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if(m_uiISOTableLength == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", " << iBlockNr << ", " << iBlockCnt << ") - " << GetErrorText(FEDM_ERROR_NO_TABLE_DATA));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_DATA);
		}

		if((unsigned int)iIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", " << iBlockNr << ", " << iBlockCnt << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		if(iBlockNr < 0 || (unsigned int)((iBlockNr+iBlockCnt)*m_ISOTable[iIdx]->m_ucBlockSize - 1) >= m_ISOTable[iIdx]->m_ucRxDB.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", " << iBlockNr << ", " << iBlockCnt << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		for(iCnt=iBlockNr; iCnt<iBlockNr+iBlockCnt; ++iCnt)
		{
			if( memcmp( &m_ISOTable[iIdx]->m_ucRxDB[iCnt*m_ISOTable[iIdx]->m_ucBlockSize],
						&m_ISOTable[iIdx]->m_ucTxDB[iCnt*m_ISOTable[iIdx]->m_ucBlockSize],
						m_ISOTable[iIdx]->m_ucBlockSize ) )
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", " << iBlockNr << ", " << iBlockCnt << ") - " << GetErrorText(FEDM_ERROR_VERIFY));
				FEDM_RETURN(FEDM_ERROR_VERIFY);
			}
		}

		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", " << iBlockNr << ", " << iBlockCnt << ") - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_RETURN(FEDM_OK);
}


//////////////////////////////////////////////////////////////////////
// functions for copy data array inside a table
//////////////////////////////////////////////////////////////////////

/***************************************************************************
  Begin        		:	12.02.2004 / M. Hultsch
  Version       	:	03.00.05 / 26.03.2009 / M. Hultsch

  Function			:	copy data array inside a table

  Parameters		:	int iIdx		-	index of table
						unsigned int uiTableID	-	identifies the table
						int iSrcDataID	-	identifier of source
						int iDstDataID	-	identifier of destination

  Return value		:	0 or error code (<0)

***************************************************************************/
int FEDM_ISCReader::CopyTableData(int iIdx, unsigned int uiTableID, int iSrcDataID, int iDstDataID)
{
	if(iIdx < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", " << iSrcDataID << ", " << iDstDataID << ") - Index < 0");
		FEDM_RETURN(iIdx);
	}

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", " << iSrcDataID << ", " << iDstDataID << ") - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", " << iSrcDataID << ", " << iDstDataID << ") - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if(m_uiISOTableLength == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", " << iSrcDataID << ", " << iDstDataID << ") - " << GetErrorText(FEDM_ERROR_NO_TABLE_DATA));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_DATA);
		}

		if((unsigned int)iIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", " << iSrcDataID << ", " << iDstDataID << ") - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		if(iSrcDataID == iDstDataID)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", " << iSrcDataID << ", " << iDstDataID << ") - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}

		switch(iSrcDataID)
		{
		case FEDM_ISC_DATA_RxDB:
			switch(iDstDataID)
			{
			case FEDM_ISC_DATA_TxDB:
				memcpy( &m_ISOTable[iIdx]->m_ucTxDB[0], &m_ISOTable[iIdx]->m_ucRxDB[0], m_ISOTable[iIdx]->m_ucRxDB.size());
				break;
			default:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", " << iSrcDataID << ", " << iDstDataID << ") - " << GetErrorText(FEDM_ERROR_PARAMETER));
				FEDM_RETURN(FEDM_ERROR_PARAMETER);
			}
			break;
		
		case FEDM_ISC_DATA_TxDB:
			switch(iDstDataID)
			{
			case FEDM_ISC_DATA_RxDB:
				memcpy( &m_ISOTable[iIdx]->m_ucRxDB[0], &m_ISOTable[iIdx]->m_ucTxDB[0], m_ISOTable[iIdx]->m_ucTxDB.size());
				break;
			default:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", " << iSrcDataID << ", " << iDstDataID << ") - " << GetErrorText(FEDM_ERROR_PARAMETER));
				FEDM_RETURN(FEDM_ERROR_PARAMETER);
			}
		
		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", " << iSrcDataID << ", " << iDstDataID << ") - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}

		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iIdx << ", " << uiTableID << ", " << iSrcDataID << ", " << iDstDataID << ") - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_RETURN(FEDM_OK);
}

//////////////////////////////////////////////////////////////////////
// functions to find index of specific table data
//////////////////////////////////////////////////////////////////////

/***************************************************************************
  Begin        		:	24.08.2000 / M. Hultsch
  Version       	:	03.00.07 / 30.04.2009 / M. Hultsch

  Function			:	search the table index of bData member inside a table

  Parameters		:	int iStartIdx	-	start index of table (>=0)
						unsigned int uiTableID	-	identifies the table
						unsigned int uiDataID	-	identifies the table data
						bool bData		-	compare data

  Return value		:	0..<table size	: index
						< -1			: error code

***************************************************************************/
int FEDM_ISCReader::FindTableIndex(int iStartIdx, unsigned int uiTableID, unsigned int uiDataID, bool bData)
{
	int iIdx = 0;

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if(m_uiISOTableLength == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool) - " << GetErrorText(FEDM_ERROR_NO_TABLE_DATA));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_DATA);
		}

		if(iStartIdx < 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		
		if((unsigned int)iStartIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool) - Index > size");
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		for(iIdx=iStartIdx; (unsigned int)iIdx<m_uiISOTableLength; iIdx++)
		{
			switch(uiDataID)
			{
			case FEDM_ISC_DATA_IS_SELECTED:
				if( m_ISOTable[iIdx]->m_bIsSelected == bData )
					return iIdx;
				break;

			case FEDM_ISC_DATA_IS_EPC:
				if( m_ISOTable[iIdx]->m_bIsEpc == bData )
					return iIdx;
				break;

			case FEDM_ISC_DATA_IS_BLOCK_SIZE_SET:
				if( m_ISOTable[iIdx]->m_bIsBlockSizeSet == bData )
					return iIdx;
				break;

			case FEDM_ISC_DATA_ANT_NR:
			case FEDM_ISC_DATA_ANT_STATUS:
			case FEDM_ISC_DATA_ANT_RSSI:
			case FEDM_ISC_DATA_ANT_COUNT:
			case FEDM_ISC_DATA_SNR_LEN:
			case FEDM_ISC_DATA_OPTINFO:
			case FEDM_ISC_DATA_PROTOINFO:
			case FEDM_ISC_DATA_TRINFO:
			case FEDM_ISC_DATA_EPC:
			case FEDM_ISC_DATA_EPC_TYPE:
			case FEDM_ISC_DATA_EPC_HEADER:
			case FEDM_ISC_DATA_EPC_DOMAIN:
			case FEDM_ISC_DATA_EPC_OBJECT:
			case FEDM_ISC_DATA_EPC_SNR:
			case FEDM_ISC_DATA_TRTYPE:
			case FEDM_ISC_DATA_SNR:
			case FEDM_ISC_DATA_AFI:
			case FEDM_ISC_DATA_DSFID:
			case FEDM_ISC_DATA_CHIP_ID:
			case FEDM_ISC_DATA_MEM_SIZE:
			case FEDM_ISC_DATA_IC_REF:
			case FEDM_ISC_DATA_SEC_STATUS:
			case FEDM_ISC_DATA_TxDB:
			case FEDM_ISC_DATA_RxDB:
			case FEDM_ISC_DATA_TxCB:
			case FEDM_ISC_DATA_RxCB:
			case FEDM_ISC_DATA_RxDB_EPC_BANK:
			case FEDM_ISC_DATA_TxDB_EPC_BANK:
			case FEDM_ISC_DATA_RxDB_TID_BANK:
			case FEDM_ISC_DATA_TxDB_TID_BANK:
			case FEDM_ISC_DATA_RxDB_RES_BANK:
			case FEDM_ISC_DATA_TxDB_RES_BANK:
			case FEDM_ISC_DATA_BLOCK_SIZE:
			case FEDM_ISC_DATA_FSCI:
			case FEDM_ISC_DATA_FWI:
			case FEDM_ISC_DATA_DSI:
			case FEDM_ISC_DATA_DRI:
			case FEDM_ISC_DATA_NAD:
			case FEDM_ISC_DATA_CID:
			case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
				FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

			default:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool) - " << GetErrorText(FEDM_ERROR_PARAMETER));
				FEDM_RETURN(FEDM_ERROR_PARAMETER);
			}
		}
		break;
	
	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
		
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool) - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", bool) - " << GetErrorText(FEDM_ERROR_NO_TABLE_DATA));
	FEDM_RETURN(FEDM_ERROR_NO_TABLE_DATA);	// table data not found
}

/***************************************************************************
  Begin        		:	24.08.2000 / M. Hultsch
  Version       	:	03.00.07 / 30.04.2009 / M. Hultsch

  Function			:	search the table index of ucData member inside a table

  Parameters		:	int iStartIdx	-	start index of table (>=0)
						unsigned int uiTableID	-	identifies the table
						unsigned int uiDataID	-	identifies the table data
						unsigned char ucData	-	compare data

  Return value		:	0..<table size	: index
						< -1			: error code

***************************************************************************/
int FEDM_ISCReader::FindTableIndex(int iStartIdx, unsigned int uiTableID, unsigned int uiDataID, unsigned char ucData)
{
	int iIdx = 0;

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
		if(m_BRMTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if(m_uiBRMTableLength == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar) - " << GetErrorText(FEDM_ERROR_NO_TABLE_DATA));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_DATA);
		}

		if(iStartIdx < 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}

		if((unsigned int)iStartIdx > m_BRMTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		for(iIdx=iStartIdx; (unsigned int)iIdx<m_uiBRMTableLength; iIdx++)
		{
			switch(uiDataID)
			{
			case FEDM_ISC_DATA_TRTYPE:
				if( m_BRMTable[iIdx]->m_ucTrType == ucData )
					return iIdx;
				break;

			case FEDM_ISC_DATA_ANT_NR:
				if( m_BRMTable[iIdx]->m_ucAntNr == ucData )
					return iIdx;
				break;

			case FEDM_ISC_DATA_ANT_STATUS:
			case FEDM_ISC_DATA_ANT_RSSI:
			case FEDM_ISC_DATA_ANT_COUNT:
			case FEDM_ISC_DATA_SNR_LEN:
			case FEDM_ISC_DATA_SNR:
			case FEDM_ISC_DATA_EPC:
			case FEDM_ISC_DATA_TIMER:
			case FEDM_ISC_DATA_DATE:
			case FEDM_ISC_DATA_DB_ADR:
			case FEDM_ISC_DATA_DBN:
			case FEDM_ISC_DATA_INPUT:
			case FEDM_ISC_DATA_STATE:
			case FEDM_ISC_DATA_BLOCK_SIZE:
			case FEDM_ISC_DATA_RxDB:
			case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
				FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

			default:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar) - " << GetErrorText(FEDM_ERROR_PARAMETER));
				FEDM_RETURN(FEDM_ERROR_PARAMETER);
			}
		}
		break;
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if(m_uiISOTableLength == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar) - " << GetErrorText(FEDM_ERROR_NO_TABLE_DATA));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_DATA);
		}

		if(iStartIdx < 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}

		if((unsigned int)iStartIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		for(iIdx=iStartIdx; (unsigned int)iIdx<m_uiISOTableLength; iIdx++)
		{
			switch(uiDataID)
			{
			case FEDM_ISC_DATA_TRTYPE:
				if( m_ISOTable[iIdx]->m_ucTrType == ucData )
					return iIdx;
				break;

			case FEDM_ISC_DATA_AFI:
				if( m_ISOTable[iIdx]->m_ucAFI == ucData )
					return iIdx;
				break;

			case FEDM_ISC_DATA_DSFID:
				if( m_ISOTable[iIdx]->m_ucDsfID == ucData )
					return iIdx;
				break;

			case FEDM_ISC_DATA_CHIP_ID:
				if( m_ISOTable[iIdx]->m_ucChipID == ucData )
					return iIdx;
				break;

			case FEDM_ISC_DATA_MEM_SIZE:
				FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

			case FEDM_ISC_DATA_IC_REF:
				if( m_ISOTable[iIdx]->m_ucICRef == ucData )
					return iIdx;
				break;

			case FEDM_ISC_DATA_TRINFO:
				if( m_ISOTable[iIdx]->m_ucTrInfo == ucData )
					return iIdx;
				break;

			case FEDM_ISC_DATA_EPC_TYPE:
				if( m_ISOTable[iIdx]->m_ucEpcType == ucData )
					return iIdx;
				break;

			case FEDM_ISC_DATA_ANT_NR:
			case FEDM_ISC_DATA_ANT_STATUS:
			case FEDM_ISC_DATA_ANT_RSSI:
			case FEDM_ISC_DATA_ANT_COUNT:
			case FEDM_ISC_DATA_SNR_LEN:
			case FEDM_ISC_DATA_OPTINFO:
			case FEDM_ISC_DATA_PROTOINFO:
			case FEDM_ISC_DATA_EPC:
			case FEDM_ISC_DATA_EPC_HEADER:
			case FEDM_ISC_DATA_EPC_DOMAIN:
			case FEDM_ISC_DATA_EPC_OBJECT:
			case FEDM_ISC_DATA_EPC_SNR:
			case FEDM_ISC_DATA_SNR:
			case FEDM_ISC_DATA_IS_SELECTED:
			case FEDM_ISC_DATA_IS_EPC:
			case FEDM_ISC_DATA_SEC_STATUS:
			case FEDM_ISC_DATA_TxDB:
			case FEDM_ISC_DATA_RxDB:
			case FEDM_ISC_DATA_TxCB:
			case FEDM_ISC_DATA_RxCB:
			case FEDM_ISC_DATA_RxDB_EPC_BANK:
			case FEDM_ISC_DATA_TxDB_EPC_BANK:
			case FEDM_ISC_DATA_RxDB_TID_BANK:
			case FEDM_ISC_DATA_TxDB_TID_BANK:
			case FEDM_ISC_DATA_RxDB_RES_BANK:
			case FEDM_ISC_DATA_TxDB_RES_BANK:
			case FEDM_ISC_DATA_BLOCK_SIZE:
			case FEDM_ISC_DATA_IS_BLOCK_SIZE_SET:
			case FEDM_ISC_DATA_FSCI:
			case FEDM_ISC_DATA_FWI:
			case FEDM_ISC_DATA_DSI:
			case FEDM_ISC_DATA_DRI:
			case FEDM_ISC_DATA_NAD:
			case FEDM_ISC_DATA_CID:
			case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
				FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

			default:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar) - " << GetErrorText(FEDM_ERROR_PARAMETER));
				FEDM_RETURN(FEDM_ERROR_PARAMETER);
			}
		}
		break;
	
	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
		
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar) - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uchar) - " << GetErrorText(FEDM_ERROR_NO_TABLE_DATA));
	FEDM_RETURN(FEDM_ERROR_NO_TABLE_DATA);	// table data not found
}


/***************************************************************************
  Begin        		:	19.11.2000 / M. Hultsch
  Version       	:	03.00.07 / 30.04.2009 / M. Hultsch

  Function			:	search the table index of uiData member inside a table

  Parameters		:	int iStartIdx	-	start index of table (>=0)
						unsigned int uiTableID	-	identifies the table
						unsigned int uiDataID	-	identifies the table data
						unsigned int uiData		-	compare data

  Return value		:	0..<table size	: index
						< -1			: error code

***************************************************************************/
int FEDM_ISCReader::FindTableIndex(int iStartIdx, unsigned int uiTableID, unsigned int uiDataID, unsigned int uiData)
{
	int iIdx = 0;

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
		if(m_BRMTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if(m_uiBRMTableLength == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint) - " << GetErrorText(FEDM_ERROR_NO_TABLE_DATA));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_DATA);
		}

		if(iStartIdx < 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}

		if((unsigned int)iStartIdx > m_BRMTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		for(iIdx=iStartIdx; (unsigned int)iIdx<m_uiBRMTableLength; iIdx++)
		{
			switch(uiDataID)
			{
			case FEDM_ISC_DATA_TRTYPE:
				if( (unsigned int)m_BRMTable[iIdx]->m_ucTrType == uiData )
					return iIdx;
				break;

			case FEDM_ISC_DATA_ANT_NR:
				if( (unsigned int)m_BRMTable[iIdx]->m_ucAntNr == uiData )
					return iIdx;
				break;

			case FEDM_ISC_DATA_ANT_STATUS:
			case FEDM_ISC_DATA_ANT_RSSI:
			case FEDM_ISC_DATA_ANT_COUNT:
			case FEDM_ISC_DATA_SNR_LEN:
			case FEDM_ISC_DATA_SNR:
			case FEDM_ISC_DATA_EPC:
			case FEDM_ISC_DATA_TIMER:
			case FEDM_ISC_DATA_DATE:
			case FEDM_ISC_DATA_DB_ADR:
			case FEDM_ISC_DATA_INPUT:
			case FEDM_ISC_DATA_STATE:
			case FEDM_ISC_DATA_DBN:
			case FEDM_ISC_DATA_BLOCK_SIZE:
			case FEDM_ISC_DATA_RxDB:
			case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
				FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

			default:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint) - " << GetErrorText(FEDM_ERROR_PARAMETER));
				FEDM_RETURN(FEDM_ERROR_PARAMETER);
			}
		}
		break;
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if(m_uiISOTableLength == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint) - " << GetErrorText(FEDM_ERROR_NO_TABLE_DATA));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_DATA);
		}

		if(iStartIdx < 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}

		if((unsigned int)iStartIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		for(iIdx=iStartIdx; (unsigned int)iIdx<m_uiISOTableLength; iIdx++)
		{
			switch(uiDataID)
			{
			case FEDM_ISC_DATA_TRTYPE:
				if( (unsigned int)m_ISOTable[iIdx]->m_ucTrType == uiData )
					return iIdx;
				break;

			case FEDM_ISC_DATA_AFI:
				if( (unsigned int)m_ISOTable[iIdx]->m_ucAFI == uiData )
					return iIdx;
				break;
			
			case FEDM_ISC_DATA_DSFID:
				if( (unsigned int)m_ISOTable[iIdx]->m_ucDsfID == uiData )
					return iIdx;
				break;
			
			case FEDM_ISC_DATA_CHIP_ID:
				if( (unsigned int)m_ISOTable[iIdx]->m_ucChipID == uiData )
					return iIdx;
				break;

			case FEDM_ISC_DATA_IC_REF:
				if( (unsigned int)m_ISOTable[iIdx]->m_ucICRef == uiData )
					return iIdx;
				break;

			case FEDM_ISC_DATA_TRINFO:
				if( (unsigned int)m_ISOTable[iIdx]->m_ucTrInfo == uiData )
					return iIdx;
				break;

			case FEDM_ISC_DATA_EPC_TYPE:
				if( (unsigned int)m_ISOTable[iIdx]->m_ucEpcType == uiData )
					return iIdx;
				break;

			case FEDM_ISC_DATA_EPC_HEADER:
				if( (unsigned int)m_ISOTable[iIdx]->GetEpcHeader() == uiData )
					return iIdx;
				break;

			case FEDM_ISC_DATA_ANT_NR:
			case FEDM_ISC_DATA_ANT_STATUS:
			case FEDM_ISC_DATA_ANT_RSSI:
			case FEDM_ISC_DATA_ANT_COUNT:
			case FEDM_ISC_DATA_SNR_LEN:
			case FEDM_ISC_DATA_OPTINFO:
			case FEDM_ISC_DATA_PROTOINFO:
			case FEDM_ISC_DATA_EPC:
			case FEDM_ISC_DATA_EPC_DOMAIN:
			case FEDM_ISC_DATA_EPC_OBJECT:
			case FEDM_ISC_DATA_EPC_SNR:
			case FEDM_ISC_DATA_MEM_SIZE:
			case FEDM_ISC_DATA_SNR:
			case FEDM_ISC_DATA_IS_SELECTED:
			case FEDM_ISC_DATA_IS_EPC:
			case FEDM_ISC_DATA_SEC_STATUS:
			case FEDM_ISC_DATA_TxDB:
			case FEDM_ISC_DATA_RxDB:
			case FEDM_ISC_DATA_TxCB:
			case FEDM_ISC_DATA_RxCB:
			case FEDM_ISC_DATA_RxDB_EPC_BANK:
			case FEDM_ISC_DATA_TxDB_EPC_BANK:
			case FEDM_ISC_DATA_RxDB_TID_BANK:
			case FEDM_ISC_DATA_TxDB_TID_BANK:
			case FEDM_ISC_DATA_RxDB_RES_BANK:
			case FEDM_ISC_DATA_TxDB_RES_BANK:
			case FEDM_ISC_DATA_BLOCK_SIZE:
			case FEDM_ISC_DATA_IS_BLOCK_SIZE_SET:
			case FEDM_ISC_DATA_FSCI:
			case FEDM_ISC_DATA_FWI:
			case FEDM_ISC_DATA_DSI:
			case FEDM_ISC_DATA_DRI:
			case FEDM_ISC_DATA_NAD:
			case FEDM_ISC_DATA_CID:
			case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
				FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

			default:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint) - " << GetErrorText(FEDM_ERROR_PARAMETER));
				FEDM_RETURN(FEDM_ERROR_PARAMETER);
			}
		}
		break;
	
	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
		
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint) - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", uint) - " << GetErrorText(FEDM_ERROR_NO_TABLE_DATA));
	FEDM_RETURN(FEDM_ERROR_NO_TABLE_DATA);	// table data not found
}


/***************************************************************************
  Begin        		:	24.08.2000 / M. Hultsch
  Version       	:	03.00.07 / 30.04.2009 / M. Hultsch

  Function			:	search the table index of i64Data member inside a table

  Parameters		:	int iStartIdx	-	start index of table (>=0)
						unsigned int uiTableID	-	identifies the table
						unsigned int uiDataID	-	identifies the table data
						__int64 i64Data	-	data

  Return value		:	0..<table size	: index
						< -1			: error code

***************************************************************************/
int FEDM_ISCReader::FindTableIndex(int iStartIdx, unsigned int uiTableID, unsigned int uiDataID, __int64 i64Data)
{
	int iIdx;
	__int64 i64Tmp = 0;

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
		if(m_BRMTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if(m_uiBRMTableLength == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64) - " << GetErrorText(FEDM_ERROR_NO_TABLE_DATA));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_DATA);
		}

		if(iStartIdx < 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}

		if((unsigned int)iStartIdx > m_BRMTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		for(iIdx=0; (unsigned int)iIdx<m_uiBRMTableLength; iIdx++)
		{
			switch(uiDataID)
			{
			case FEDM_ISC_DATA_SNR:
				if( i64Data == m_BRMTable[iIdx]->GetSnr() )
					return iIdx;
				break;		

			case FEDM_ISC_DATA_ANT_NR:
			case FEDM_ISC_DATA_ANT_STATUS:
			case FEDM_ISC_DATA_ANT_RSSI:
			case FEDM_ISC_DATA_ANT_COUNT:
			case FEDM_ISC_DATA_SNR_LEN:
			case FEDM_ISC_DATA_TRTYPE:
			case FEDM_ISC_DATA_TIMER:
			case FEDM_ISC_DATA_DATE:
			case FEDM_ISC_DATA_DB_ADR:
			case FEDM_ISC_DATA_DBN:
			case FEDM_ISC_DATA_INPUT:
			case FEDM_ISC_DATA_STATE:
			case FEDM_ISC_DATA_BLOCK_SIZE:
			case FEDM_ISC_DATA_RxDB:
			case FEDM_ISC_DATA_EPC:
			case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
				FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

			default:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64) - " << GetErrorText(FEDM_ERROR_PARAMETER));
				FEDM_RETURN(FEDM_ERROR_PARAMETER);
			}
		}
		break;
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if(m_uiISOTableLength == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64) - " << GetErrorText(FEDM_ERROR_NO_TABLE_DATA));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_DATA);
		}

		if(iStartIdx < 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}

		if((unsigned int)iStartIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		for(iIdx=0; (unsigned int)iIdx<m_uiISOTableLength; iIdx++)
		{
			switch(uiDataID)
			{
			case FEDM_ISC_DATA_SNR:
				if( i64Data == m_ISOTable[iIdx]->GetSnr() )
					return iIdx;
				break;		

			case FEDM_ISC_DATA_OPTINFO:
			case FEDM_ISC_DATA_PROTOINFO:
			case FEDM_ISC_DATA_TRINFO:
			case FEDM_ISC_DATA_EPC_TYPE:
			case FEDM_ISC_DATA_EPC_HEADER:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
				FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

			case FEDM_ISC_DATA_EPC_DOMAIN:
				i64Tmp = m_ISOTable[iIdx]->GetEpcDomainManager();
				if(i64Tmp < 0)
					break;

				if(i64Tmp == i64Data)
					return iIdx;
				break;		

			case FEDM_ISC_DATA_EPC_SNR:
				i64Tmp = m_ISOTable[iIdx]->GetEpcSnr();
				if(i64Tmp < 0)
					break;

				if(i64Tmp == i64Data)
					return iIdx;
				break;		

			case FEDM_ISC_DATA_EPC_OBJECT:
				i64Tmp = m_ISOTable[iIdx]->GetEpcObjectClass();
				if(i64Tmp < 0)
					break;

				if(i64Tmp == i64Data)
					return iIdx;
				break;		

			case FEDM_ISC_DATA_ANT_NR:
			case FEDM_ISC_DATA_ANT_STATUS:
			case FEDM_ISC_DATA_ANT_RSSI:
			case FEDM_ISC_DATA_ANT_COUNT:
			case FEDM_ISC_DATA_SNR_LEN:
			case FEDM_ISC_DATA_EPC:
			case FEDM_ISC_DATA_TRTYPE:
			case FEDM_ISC_DATA_AFI:
			case FEDM_ISC_DATA_DSFID:
			case FEDM_ISC_DATA_CHIP_ID:
			case FEDM_ISC_DATA_MEM_SIZE:
			case FEDM_ISC_DATA_IC_REF:
			case FEDM_ISC_DATA_IS_SELECTED:
			case FEDM_ISC_DATA_IS_EPC:
			case FEDM_ISC_DATA_SEC_STATUS:
			case FEDM_ISC_DATA_TxDB:
			case FEDM_ISC_DATA_RxDB:
			case FEDM_ISC_DATA_TxCB:
			case FEDM_ISC_DATA_RxCB:
			case FEDM_ISC_DATA_RxDB_EPC_BANK:
			case FEDM_ISC_DATA_TxDB_EPC_BANK:
			case FEDM_ISC_DATA_RxDB_TID_BANK:
			case FEDM_ISC_DATA_TxDB_TID_BANK:
			case FEDM_ISC_DATA_RxDB_RES_BANK:
			case FEDM_ISC_DATA_TxDB_RES_BANK:
			case FEDM_ISC_DATA_BLOCK_SIZE:
			case FEDM_ISC_DATA_IS_BLOCK_SIZE_SET:
			case FEDM_ISC_DATA_FSCI:
			case FEDM_ISC_DATA_FWI:
			case FEDM_ISC_DATA_DSI:
			case FEDM_ISC_DATA_DRI:
			case FEDM_ISC_DATA_NAD:
			case FEDM_ISC_DATA_CID:
			case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
				FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

			default:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64) - " << GetErrorText(FEDM_ERROR_PARAMETER));
				FEDM_RETURN(FEDM_ERROR_PARAMETER);
			}
		}
		break;
	
	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
		
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64) - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", __int64) - " << GetErrorText(FEDM_ERROR_NO_TABLE_DATA));
	FEDM_RETURN(FEDM_ERROR_NO_TABLE_DATA);	// table data not found
}


/***************************************************************************
  Begin        		:	19.08.2003 / M. Hultsch
  Version       	:	03.00.07 / 30.04.2009 / M. Hultsch

  Function			:	search the table index of sData member inside a table

  Parameters		:	int iStartIdx	-	start index of table (>=0)
						unsigned int uiTableID	-	identifies the table
						unsigned int uiDataID	-	identifies the table data
						string sData	-	data

  Return value		:	0..<table size	: index
						<0				: error code

***************************************************************************/
int FEDM_ISCReader::FindTableIndex(int iStartIdx, unsigned int uiTableID, unsigned int uiDataID, string sData)
{
	char cEpc[FEDM_ISC_MAX_UID_LENGTH];
	int iIdx, iErr;
	__int64 i64Tmp = 0;
	string sStr;

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
		if(m_BRMTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if(m_uiBRMTableLength == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string) - " << GetErrorText(FEDM_ERROR_NO_TABLE_DATA));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_DATA);
		}

		if(iStartIdx < 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}

		if((unsigned int)iStartIdx > m_BRMTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		for(iIdx=0; iIdx<(int)m_uiBRMTableLength; iIdx++)
		{
			switch(uiDataID)
			{
			case FEDM_ISC_DATA_SNR:
				FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucSnr[0], (int)m_BRMTable[iIdx]->m_ucSnrLen, sStr));
				if(sStr == sData)
					return iIdx;
				break;		
			
			case FEDM_ISC_DATA_EPC:
				if(m_BRMTable[iIdx]->m_ucIDDT == 0x01)
				{
					FEDM_CHK2(iErr, m_BRMTable[iIdx]->GetEpc(cEpc, FEDM_ISC_MAX_UID_LENGTH));
					if(strcmp(sData.c_str(), cEpc)==0)
						return iIdx;
				}
				break;

			case FEDM_ISC_DATA_ANT_NR:
			case FEDM_ISC_DATA_ANT_STATUS:
			case FEDM_ISC_DATA_ANT_RSSI:
			case FEDM_ISC_DATA_ANT_COUNT:
			case FEDM_ISC_DATA_SNR_LEN:
			case FEDM_ISC_DATA_TRTYPE:
			case FEDM_ISC_DATA_TIMER:
			case FEDM_ISC_DATA_DATE:
			case FEDM_ISC_DATA_DB_ADR:
			case FEDM_ISC_DATA_DBN:
			case FEDM_ISC_DATA_INPUT:
			case FEDM_ISC_DATA_STATE:
			case FEDM_ISC_DATA_BLOCK_SIZE:
			case FEDM_ISC_DATA_RxDB:
			case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
				FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

			default:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string) - " << GetErrorText(FEDM_ERROR_PARAMETER));
				FEDM_RETURN(FEDM_ERROR_PARAMETER);
			}
		}
		break;
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if(m_uiISOTableLength == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string) - " << GetErrorText(FEDM_ERROR_NO_TABLE_DATA));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_DATA);
		}

		if(iStartIdx < 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}

		if((unsigned int)iStartIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		for(iIdx=0; iIdx<(int)m_uiISOTableLength; iIdx++)
		{
			switch(uiDataID)
			{
			case FEDM_ISC_DATA_SNR:
				FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucSnr[0], (int)m_ISOTable[iIdx]->m_ucSnrLen, sStr));
				if(sStr == sData)
					return iIdx;
				break;		

			case FEDM_ISC_DATA_EPC_DOMAIN:
				i64Tmp = m_ISOTable[iIdx]->GetEpcDomainManager();
				if(i64Tmp < 0)
					break;

				FEDM_CHK2(iErr, FEDM_ConvInt64ToHexStr(i64Tmp, sStr));
				if(sStr == sData)
					return iIdx;
				break;		

			case FEDM_ISC_DATA_EPC_SNR:
				i64Tmp = m_ISOTable[iIdx]->GetEpcSnr();
				if(i64Tmp < 0)
					break;

				FEDM_CHK2(iErr, FEDM_ConvInt64ToHexStr(i64Tmp, sStr));
				if(sStr == sData)
					return iIdx;
				break;		

			case FEDM_ISC_DATA_EPC_OBJECT:
				i64Tmp = m_ISOTable[iIdx]->GetEpcObjectClass();
				if(i64Tmp < 0)
					break;

				FEDM_CHK2(iErr, FEDM_ConvInt64ToHexStr(i64Tmp, sStr));
				if(sStr == sData)
					return iIdx;
				break;		

			case FEDM_ISC_DATA_EPC:
				FEDM_CHK2(iErr, m_ISOTable[iIdx]->GetEpc(cEpc, FEDM_ISC_MAX_UID_LENGTH));
				if(strcmp(sData.c_str(), cEpc)==0)
					return iIdx;
				break;

			case FEDM_ISC_DATA_TRTYPE:
			case FEDM_ISC_DATA_OPTINFO:
			case FEDM_ISC_DATA_PROTOINFO:
			case FEDM_ISC_DATA_TRINFO:
			case FEDM_ISC_DATA_EPC_TYPE:
			case FEDM_ISC_DATA_EPC_HEADER:
			case FEDM_ISC_DATA_ANT_NR:
			case FEDM_ISC_DATA_ANT_STATUS:
			case FEDM_ISC_DATA_ANT_RSSI:
			case FEDM_ISC_DATA_ANT_COUNT:
			case FEDM_ISC_DATA_SNR_LEN:
			case FEDM_ISC_DATA_AFI:
			case FEDM_ISC_DATA_DSFID:
			case FEDM_ISC_DATA_CHIP_ID:
			case FEDM_ISC_DATA_MEM_SIZE:
			case FEDM_ISC_DATA_IC_REF:
			case FEDM_ISC_DATA_IS_SELECTED:
			case FEDM_ISC_DATA_IS_EPC:
			case FEDM_ISC_DATA_SEC_STATUS:
			case FEDM_ISC_DATA_TxDB:
			case FEDM_ISC_DATA_RxDB:
			case FEDM_ISC_DATA_TxCB:
			case FEDM_ISC_DATA_RxCB:
			case FEDM_ISC_DATA_RxDB_EPC_BANK:
			case FEDM_ISC_DATA_TxDB_EPC_BANK:
			case FEDM_ISC_DATA_RxDB_TID_BANK:
			case FEDM_ISC_DATA_TxDB_TID_BANK:
			case FEDM_ISC_DATA_RxDB_RES_BANK:
			case FEDM_ISC_DATA_TxDB_RES_BANK:
			case FEDM_ISC_DATA_BLOCK_SIZE:
			case FEDM_ISC_DATA_IS_BLOCK_SIZE_SET:
			case FEDM_ISC_DATA_FSCI:
			case FEDM_ISC_DATA_FWI:
			case FEDM_ISC_DATA_DSI:
			case FEDM_ISC_DATA_DRI:
			case FEDM_ISC_DATA_NAD:
			case FEDM_ISC_DATA_CID:
			case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
				FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

			default:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string) - " << GetErrorText(FEDM_ERROR_PARAMETER));
				FEDM_RETURN(FEDM_ERROR_PARAMETER);
			}
		}
		break;
	
	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
		
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string) - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", string) - " << GetErrorText(FEDM_ERROR_NO_TABLE_DATA));
	FEDM_RETURN(FEDM_ERROR_NO_TABLE_DATA);	// table data not found
}


/***************************************************************************
  Begin        		:	19.11.2000 / M. Hultsch
  Version       	:	03.00.07 / 30.04.2009 / M. Hultsch

  Function			:	search the table index of sData member inside a table

  Parameters		:	int iStartIdx	-	start index of table (>=0)
						unsigned int uiTableID	-	identifies the table
						unsigned int uiDataID	-	identifies the table data
						CString sData	-	data

  Return value		:	0..<table size	: index
						<0				: error code

***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_ISCReader::FindTableIndex(int iStartIdx, unsigned int uiTableID, unsigned int uiDataID, CString sData)
{
	char cEpc[FEDM_ISC_MAX_UID_LENGTH];
	int iIdx, iErr;
	__int64 i64Tmp = 0;
	CString sStr;

	switch(uiTableID)
	{
	case FEDM_ISC_BRM_TABLE:
		if(m_BRMTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if(m_uiBRMTableLength == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString) - " << GetErrorText(FEDM_ERROR_NO_TABLE_DATA));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_DATA);
		}

		if(iStartIdx < 0 || (unsigned int)iStartIdx > m_BRMTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}

		if(iStartIdx < 0 || (unsigned int)iStartIdx > m_BRMTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		for(iIdx=0; iIdx<(int)m_uiBRMTableLength; iIdx++)
		{
			switch(uiDataID)
			{
			case FEDM_ISC_DATA_SNR:
				FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_BRMTable[iIdx]->m_ucSnr[0], (int)m_BRMTable[iIdx]->m_ucSnrLen, sStr));
				if(sStr == sData)
					return iIdx;
				break;		
			
			case FEDM_ISC_DATA_EPC:
				if(m_BRMTable[iIdx]->m_ucIDDT == 0x01)
				{
					FEDM_CHK2(iErr, m_BRMTable[iIdx]->GetEpc(cEpc, FEDM_ISC_MAX_UID_LENGTH));
					if(sData == cEpc)
						return iIdx;
				}
				break;
			
			case FEDM_ISC_DATA_ANT_NR:
			case FEDM_ISC_DATA_ANT_STATUS:
			case FEDM_ISC_DATA_ANT_RSSI:
			case FEDM_ISC_DATA_ANT_COUNT:
			case FEDM_ISC_DATA_SNR_LEN:
			case FEDM_ISC_DATA_TRTYPE:
			case FEDM_ISC_DATA_TIMER:
			case FEDM_ISC_DATA_DATE:
			case FEDM_ISC_DATA_DB_ADR:
			case FEDM_ISC_DATA_DBN:
			case FEDM_ISC_DATA_INPUT:
			case FEDM_ISC_DATA_STATE:
			case FEDM_ISC_DATA_BLOCK_SIZE:
			case FEDM_ISC_DATA_RxDB:
			case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
				FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

			default:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString) - " << GetErrorText(FEDM_ERROR_PARAMETER));
				FEDM_RETURN(FEDM_ERROR_PARAMETER);
			}
		}
		break;
	
	case FEDM_ISC_ISO_TABLE:
		if(m_ISOTable.size() == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString) - " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);
		}

		if(m_uiISOTableLength == 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString) - " << GetErrorText(FEDM_ERROR_NO_TABLE_DATA));
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_DATA);
		}

		if(iStartIdx < 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString) - " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}

		if((unsigned int)iStartIdx > m_ISOTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString) - " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
			FEDM_RETURN(FEDM_ERROR_ARRAY_BOUNDARY);
		}

		for(iIdx=0; iIdx<(int)m_uiISOTableLength; iIdx++)
		{
			switch(uiDataID)
			{	
			case FEDM_ISC_DATA_SNR:
				FEDM_CHK2(iErr, FEDM_ConvHexUCharToHexStr(&m_ISOTable[iIdx]->m_ucSnr[0], (int)m_ISOTable[iIdx]->m_ucSnrLen, sStr));
				if(sStr == sData)
					return iIdx;
				break;		

			case FEDM_ISC_DATA_EPC_DOMAIN:
				i64Tmp = m_ISOTable[iIdx]->GetEpcDomainManager();
				if(i64Tmp < 0)
					break;

				FEDM_CHK2(iErr, FEDM_ConvInt64ToHexStr(i64Tmp, sStr));
				if(sStr == sData)
					return iIdx;
				break;		

			case FEDM_ISC_DATA_EPC_SNR:
				i64Tmp = m_ISOTable[iIdx]->GetEpcSnr();
				if(i64Tmp < 0)
					break;

				FEDM_CHK2(iErr, FEDM_ConvInt64ToHexStr(i64Tmp, sStr));
				if(sStr == sData)
					return iIdx;
				break;		

			case FEDM_ISC_DATA_EPC_OBJECT:
				i64Tmp = m_ISOTable[iIdx]->GetEpcObjectClass();
				if(i64Tmp < 0)
					break;

				FEDM_CHK2(iErr, FEDM_ConvInt64ToHexStr(i64Tmp, sStr));
				if(sStr == sData)
					return iIdx;
				break;		

			case FEDM_ISC_DATA_EPC:
				FEDM_CHK2(iErr, m_ISOTable[iIdx]->GetEpc(cEpc, FEDM_ISC_MAX_UID_LENGTH));
				if(sData == cEpc)
					return iIdx;
				break;

			case FEDM_ISC_DATA_TRTYPE:
			case FEDM_ISC_DATA_OPTINFO:
			case FEDM_ISC_DATA_PROTOINFO:
			case FEDM_ISC_DATA_TRINFO:
			case FEDM_ISC_DATA_EPC_TYPE:
			case FEDM_ISC_DATA_EPC_HEADER:
			case FEDM_ISC_DATA_ANT_NR:
			case FEDM_ISC_DATA_ANT_STATUS:
			case FEDM_ISC_DATA_ANT_RSSI:
			case FEDM_ISC_DATA_ANT_COUNT:
			case FEDM_ISC_DATA_SNR_LEN:
			case FEDM_ISC_DATA_AFI:
			case FEDM_ISC_DATA_DSFID:
			case FEDM_ISC_DATA_CHIP_ID:
			case FEDM_ISC_DATA_MEM_SIZE:
			case FEDM_ISC_DATA_IC_REF:
			case FEDM_ISC_DATA_IS_SELECTED:
			case FEDM_ISC_DATA_IS_EPC:
			case FEDM_ISC_DATA_SEC_STATUS:
			case FEDM_ISC_DATA_TxDB:
			case FEDM_ISC_DATA_RxDB:
			case FEDM_ISC_DATA_TxCB:
			case FEDM_ISC_DATA_RxCB:
			case FEDM_ISC_DATA_RxDB_EPC_BANK:
			case FEDM_ISC_DATA_TxDB_EPC_BANK:
			case FEDM_ISC_DATA_RxDB_TID_BANK:
			case FEDM_ISC_DATA_TxDB_TID_BANK:
			case FEDM_ISC_DATA_RxDB_RES_BANK:
			case FEDM_ISC_DATA_TxDB_RES_BANK:
			case FEDM_ISC_DATA_BLOCK_SIZE:
			case FEDM_ISC_DATA_IS_BLOCK_SIZE_SET:
			case FEDM_ISC_DATA_FSCI:
			case FEDM_ISC_DATA_FWI:
			case FEDM_ISC_DATA_DSI:
			case FEDM_ISC_DATA_DRI:
			case FEDM_ISC_DATA_NAD:
			case FEDM_ISC_DATA_CID:
			case FEDM_ISC_DATA_EPC_CL1_GEN2_PC:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
				FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);

			default:
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString) - " << GetErrorText(FEDM_ERROR_PARAMETER));
				FEDM_RETURN(FEDM_ERROR_PARAMETER);
			}
		}
		break;
	
	case FEDM_ISC_CAD_TABLE:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString) - " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
		
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString) - " << GetErrorText(FEDM_ERROR_UNKNOWN_TABLE_ID));
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(" << iStartIdx << ", " << uiTableID << ", 0x" << std::hex << std::uppercase << uiDataID << ", CString) - " << GetErrorText(FEDM_ERROR_NO_TABLE_DATA));
	FEDM_RETURN(FEDM_ERROR_NO_TABLE_DATA);	// table data not found
}
#endif



//////////////////////////////////////////////////////////////////////
// protected functions
//////////////////////////////////////////////////////////////////////

/***************************************************************************
  Begin        		:	10.12.2008 / M. Hultsch
  
  Version       	:	04.06.08 / 13.10.2014 / M. Hultsch
						- additional LAN link information with mode=0x50

						04.05.05 / 29.11.2013 / M. Hultsch
						- new mode: 0x09 for WLAN-Firmware

						04.02.06 / 25.07.2012 / M. Hultsch
						- new mode 0x08 (with firmware supported tag drivers)

						04.00.08 / 24.01.2012 / M. Hultsch

  Function			:	sets the received data of last [0x66] Reader Info protocol
						with mode 0xFF (means all) into FEDM_ISC_READER_INFO

  Parameters		:	unsigned char ucMode	-	mode byte
						unsigned char* ucData	-	pointer to buffer with data

  Return value		:	OK (=0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Set_0x66_ReaderInfoData(unsigned char ucMode, unsigned char* ucData)
{
	int iErr = 0;
	unsigned int uiReaderType = m_uiReaderType;
	unsigned char ucNextMode = 0;
	unsigned int uiByteCnt = 0;
	unsigned int uiMaxRecCnt = 1;
	unsigned int uiRecCnt = 0;

	if(ucMode == 0xFF)
	{
		uiMaxRecCnt = ucData[uiByteCnt++];
		m_ReaderInfo.Init();
	}
	else
	{
		ucNextMode = ucMode;
	}

	for(uiRecCnt=0; uiRecCnt<uiMaxRecCnt; uiRecCnt++)
	{
		if(ucMode == 0xFF)
		{
			ucNextMode = ucData[uiByteCnt++];
		}

		switch(ucNextMode)
		{
		case 0x00: // RF-Controller
			FEDM_CHK2(iErr, SetData(FEDM_GetAdrOfID(FEDM_ISC_TMP_SOFTVER, 32), &ucData[uiByteCnt], 16, FEDM_TMPDATA_MEM));
			memcpy(&m_ReaderInfo.ucRfcSwVer[0], &ucData[uiByteCnt], 16);
			m_ReaderInfo.bIsMode0x00Read = true;
			uiReaderType = m_ReaderInfo.GetReaderType();
			FEDM_CHK2(iErr, SetReaderType(uiReaderType));
			uiByteCnt += 30;
	
			// ReaderInfo is partial or completely read
			m_ReaderInfo.bIsInfoRead = true;
			break;

		case 0x01: // Application Communication Controller
			memcpy(&m_ReaderInfo.ucAccSwVer[0], &ucData[uiByteCnt], 7);
			m_ReaderInfo.bIsMode0x01Read = true;
			uiByteCnt += 30;
			break;

		case 0x02: // USB Controller
			memcpy(&m_ReaderInfo.ucUsbSwVer[0], &ucData[uiByteCnt], 4);
			m_ReaderInfo.bIsMode0x02Read = true;
			uiByteCnt += 30;
			break;

		case 0x03: // RF-Decoder
			switch(uiReaderType)	// reader type
			{
			// megalock C
			case FEDM_ISC_TYPE_MLC_COM:
			// classic-pro
			case FEDM_ISC_TYPE_CPR20_XX:
			case FEDM_ISC_TYPE_CPR40_XX_U:
			case FEDM_ISC_TYPE_CPR40_XX:
			case FEDM_ISC_TYPE_CPR44_XX:
			case FEDM_ISC_TYPE_CPR46_XX:
			case FEDM_ISC_TYPE_CPR47_XX:
			//case FEDM_ISC_TYPE_MAX_ONTOP:
				memcpy(&m_ReaderInfo.ucDecoderType[0], &ucData[uiByteCnt], 6);
				m_ReaderInfo.bIsMode0x03Read = true;
				break;
			// HF-Reader
			case FEDM_ISC_TYPE_ISCLR2000:
			case FEDM_ISC_TYPE_ISCLR2500_A:
			case FEDM_ISC_TYPE_ISCLR2500_B:
			// myAXXESS
			case FEDM_ISC_TYPE_MAXU1002:
			// UHF-Reader
			case FEDM_ISC_TYPE_ISCMRU200:
			case FEDM_ISC_TYPE_ISCLRU1002:
			case FEDM_ISC_TYPE_ISCLRU2000:
			case FEDM_ISC_TYPE_ISCLRU3000:
				memcpy(&m_ReaderInfo.ucFpgaSwVer[0], &ucData[uiByteCnt], 4);
				m_ReaderInfo.bIsMode0x03Read = true;
				break;
			}
			uiByteCnt += 30;
			break;

		case 0x04: // Additional Firmware Functionality
			switch(uiReaderType)	// reader type
			{
			// LF classic types
			case 20: // ID RW40.30
			// megalock C
			case FEDM_ISC_TYPE_MLC_COM:
			// classic-pro
			case FEDM_ISC_TYPE_CPRM02:
			case FEDM_ISC_TYPE_CPR02:
			case FEDM_ISC_TYPE_CPR04_U:
			case FEDM_ISC_TYPE_CPR20_XX:
			case FEDM_ISC_TYPE_CPR40_XX_U:
			case FEDM_ISC_TYPE_CPR40_XX:
			case FEDM_ISC_TYPE_CPR44_XX:
			case FEDM_ISC_TYPE_CPR46_XX:
			case FEDM_ISC_TYPE_CPR47_XX:
			case FEDM_ISC_TYPE_MAX_ONTOP:
			case FEDM_ISC_TYPE_CPR30_XX:
			case FEDM_ISC_TYPE_CPR50_XX:
			case FEDM_ISC_TYPE_CPR52_XX:
			case FEDM_ISC_TYPE_CPR60_XX:
			case FEDM_ISC_TYPE_MAX50_XX:
			// myAXXESS
			case FEDM_ISC_TYPE_MAX_STANDARD:
			case FEDM_ISC_TYPE_MAX_COMFORT:
				memcpy(&m_ReaderInfo.ucCprFctTemplate, &ucData[uiByteCnt], 11);
				m_ReaderInfo.bIsMode0x04Read = true;
				break;

			// HF-Reader
			case FEDM_ISC_TYPE_ISCLR2000:
			case FEDM_ISC_TYPE_ISCLR2500_A:
			case FEDM_ISC_TYPE_ISCLR2500_B:
			case FEDM_ISC_TYPE_ISCLR1002:
			// myAXXESS
			case FEDM_ISC_TYPE_MAXU1002:
			// UHF-Reader
			case FEDM_ISC_TYPE_ISCMU02:
			case FEDM_ISC_TYPE_ISCMU95:
			case FEDM_ISC_TYPE_ISCLRU1002:
			case FEDM_ISC_TYPE_ISCLRU2000:
			case FEDM_ISC_TYPE_ISCLRU3000:
				memcpy(&m_ReaderInfo.ucReserved1, &ucData[uiByteCnt], 11);
				m_ReaderInfo.bIsMode0x04Read = true;
				break;

			default:
				memcpy(&m_ReaderInfo.ucReserved1, &ucData[uiByteCnt], 11);
				m_ReaderInfo.bIsMode0x04Read = true;
				break;
			}
			uiByteCnt += 30;
			break;

		case 0x05: // Bootloader Firmware
			memcpy(&m_ReaderInfo.ucBootSwVer[0], &ucData[uiByteCnt], 2);
			m_ReaderInfo.bIsMode0x05Read = true;
			uiByteCnt += 30;
			break;

		case 0x06: // information from keyboard unit
			memcpy(&m_ReaderInfo.ucKeybStatus[0], &ucData[uiByteCnt], 8);
			m_ReaderInfo.bIsMode0x06Read = true;
			uiByteCnt += 30;
			break;

		case 0x07: // information about CPRIO unit
			memcpy(&m_ReaderInfo.ucCprioSwVer[0], &ucData[uiByteCnt], 7);
			m_ReaderInfo.bIsMode0x07Read = true;
			uiByteCnt += 30;
			break;

		case 0x08: // information with supported tag drivers
			memcpy(&m_ReaderInfo.ucRfcSupportedTagDriver[0], &ucData[uiByteCnt], 14);
			m_ReaderInfo.bIsMode0x08Read = true;
			uiByteCnt += 30;
			break;

		case 0x09: // information with WLAN-Modul firmware
			memcpy(&m_ReaderInfo.ucWlanVer[0], &ucData[uiByteCnt], 14);
			m_ReaderInfo.bIsMode0x09Read = true;
			uiByteCnt += 30;
			break;

		case 0x10: // Hardware Information
			switch(uiReaderType)	// reader type
			{
			// HF-Reader
			case FEDM_ISC_TYPE_ISCMR102:
			case FEDM_ISC_TYPE_ISCMR10X:
			case FEDM_ISC_TYPE_ISCPRH102:
			case FEDM_ISC_TYPE_ISCPRH200:
			case FEDM_ISC_TYPE_ISCLR1002:
			case FEDM_ISC_TYPE_ISCLR2000:
			case FEDM_ISC_TYPE_ISCLR2500_A:
			case FEDM_ISC_TYPE_ISCLR2500_B:
				memcpy(&m_ReaderInfo.ucRfcHwInfo[0], &ucData[uiByteCnt], 11);
				m_ReaderInfo.bIsMode0x10Read	= true;
				break;
			// UHF-Reader
			case FEDM_ISC_TYPE_ISCMRU200:
				memcpy(&m_ReaderInfo.ucRfcHwInfo[0], &ucData[uiByteCnt], 11);
				m_ReaderInfo.bIsMode0x10Read	= true;
				m_ReaderInfo.ucRfcFrequency		= (m_ReaderInfo.ucRfcHwInfo[1] & 0x01) + 1;	// 0: EU; 1: FCC
				m_ReaderInfo.bIsRfcFrequency	= true;
				break;			
			// UHF-Reader
			case FEDM_ISC_TYPE_ISCLRU1000:
			case FEDM_ISC_TYPE_ISCLRU2000:
				memcpy(&m_ReaderInfo.ucRfcHwInfo[0], &ucData[uiByteCnt], 11);
				m_ReaderInfo.bIsMode0x10Read	= true;
				break;
			default:
				memcpy(&m_ReaderInfo.ucRfcHwInfo[0], &ucData[uiByteCnt], 11);
				m_ReaderInfo.bIsMode0x10Read	= true;
				m_ReaderInfo.bIsRfcFrequency	= true;
				break;
			}
			uiByteCnt += 30;
			break;

		case 0x11: // SAM information
			memcpy(&m_ReaderInfo.ucNoOfSlots, &ucData[uiByteCnt], 5);
			m_ReaderInfo.bIsMode0x11Read = true;
			uiByteCnt += 30;
			break;

		case 0x12: // CPU information of RFC
			memcpy(&m_ReaderInfo.ucRfcCpuManufacturer, &ucData[uiByteCnt], 8);
			m_ReaderInfo.bIsMode0x12Read = true;
			uiByteCnt += 30;
			break;

		case 0x40: // CFG information with read permission
			memcpy(&m_ReaderInfo.ucNoOfCfgPages[0], &ucData[uiByteCnt], 18);
			m_ReaderInfo.bIsMode0x40Read = true;
			uiByteCnt += 30;
			break;

		case 0x41: // CFG information with write permission
			memcpy(&m_ReaderInfo.ucNoOfCfgPages[0], &ucData[uiByteCnt], 2);
			uiByteCnt += 2;
			memcpy(&m_ReaderInfo.ucCfgWritePermissions[0], &ucData[uiByteCnt], 16);
			m_ReaderInfo.bIsMode0x41Read = true;
			uiByteCnt += 28;
			break;

		case 0x50: // information with MAC for LAN
			m_ReaderInfo.ucLanFlags = ucData[uiByteCnt++];
			memcpy(&m_ReaderInfo.ucLanMac[0], &ucData[uiByteCnt], 6);
			uiByteCnt += 6;
			m_ReaderInfo.ucLanLinkInfo = ucData[uiByteCnt++];
			m_ReaderInfo.bIsMode0x50Read = true;
			uiByteCnt += 22;
			break;

		case 0x51: // information with IP Address for LAN
			m_ReaderInfo.ucLanFlags = ucData[uiByteCnt++];
			memcpy(&m_ReaderInfo.ucLanIpV4[0], &ucData[uiByteCnt], 20);
			m_ReaderInfo.bIsMode0x51Read = true;
			uiByteCnt += 29;
			break;

		case 0x52: // information with Netmask for LAN
			m_ReaderInfo.ucLanFlags = ucData[uiByteCnt++];
			memcpy(&m_ReaderInfo.ucLanNetmaskV4[0], &ucData[uiByteCnt], 20);
			m_ReaderInfo.bIsMode0x52Read = true;
			uiByteCnt += 29;
			break;

		case 0x53: // information with Gateway Address for LAN
			m_ReaderInfo.ucLanFlags = ucData[uiByteCnt++];
			memcpy(&m_ReaderInfo.ucLanGatewayV4[0], &ucData[uiByteCnt], 20);
			m_ReaderInfo.bIsMode0x53Read = true;
			uiByteCnt += 29;
			break;

		case 0x54: // information with MAC for WLAN
			m_ReaderInfo.ucWlanFlags = ucData[uiByteCnt++];
			memcpy(&m_ReaderInfo.ucWlanMac[0], &ucData[uiByteCnt], 6);
			m_ReaderInfo.bIsMode0x54Read = true;
			uiByteCnt += 29;
			break;

		case 0x55: // information with IP Address for WLAN
			m_ReaderInfo.ucWlanFlags = ucData[uiByteCnt++];
			memcpy(&m_ReaderInfo.ucWlanIpV4[0], &ucData[uiByteCnt], 20);
			m_ReaderInfo.bIsMode0x55Read = true;
			uiByteCnt += 29;
			break;

		case 0x56: // information with Netmask for WLAN
			m_ReaderInfo.ucWlanFlags = ucData[uiByteCnt++];
			memcpy(&m_ReaderInfo.ucWlanNetmaskV4[0], &ucData[uiByteCnt], 20);
			m_ReaderInfo.bIsMode0x56Read = true;
			uiByteCnt += 29;
			break;

		case 0x57: // information with Gateway Address for WLAN
			m_ReaderInfo.ucWlanFlags = ucData[uiByteCnt++];
			memcpy(&m_ReaderInfo.ucWlanGatewayV4[0], &ucData[uiByteCnt], 20);
			m_ReaderInfo.bIsMode0x57Read = true;
			uiByteCnt += 29;
			break;

		case 0x60: // input/output capabilities information
			memcpy(&m_ReaderInfo.ucNoOfInputs, &ucData[uiByteCnt], 3);
			m_ReaderInfo.bIsMode0x60Read = true;
			uiByteCnt += 30;
			break;

		case 0x61: // external peripheral devices information
			if(ucData[uiByteCnt] > 0 && ucData[uiByteCnt] < 4)
			{
				memcpy(&m_ReaderInfo.ucNoOfPeripheralDevices, &ucData[uiByteCnt], ucData[uiByteCnt]*2+1);
				m_ReaderInfo.bIsMode0x61Read = true;
#if !defined(_FEDM_NO_PD_SUPPORT)
				BuildPDMap();
#endif
			}
			uiByteCnt += 30;
			break;

		case 0x80: // Device Info
			memcpy(&m_ReaderInfo.ucRfcDevID[0], &ucData[uiByteCnt], 18);
			m_ReaderInfo.bIsMode0x80Read = true;
			FEDM_SETPROP2(m_pLogger, FELOGMNG_PROP_DEVICED_ID, m_ReaderInfo.GetDeviceID());
			uiByteCnt += 30;
#ifdef _FELOGMNG_SUPPORT
			{
				// for log manager
				char cDeviceID[24];
				sprintf(cDeviceID, "%u", m_ReaderInfo.GetDeviceID());
				FEISC_SetReaderPara(m_iReaderHnd, "Device-ID", cDeviceID);
			}
#endif
			break;

		default:
			uiByteCnt += 30;
			break;
		}
	}

	return 0;
}


/***************************************************************************
  Begin        		:	24.02.2010 / M. Hultsch
  Version       	:	03.01.01 / 24.02.2010 / M. Hultsch

  Function			:	build or rebuild the map with detected peripheral devices

  Parameters		:	-

  Return value		:	-
***************************************************************************/
#if !defined(_FEDM_NO_PD_SUPPORT)
void FEDM_ISCReader::BuildPDMap()
{
	bool bFound = false;
	FEDM_PD_MAP_ITOR itor;

	// remove first all disappeared peripheral devices
	itor = m_mapPeripheralDevices.begin();
	while(itor != m_mapPeripheralDevices.end())
	{
		bFound = false;
		for(unsigned char ucCnt=0; ucCnt<m_ReaderInfo.ucNoOfPeripheralDevices; ucCnt++)
		{
			if(itor->second->GetBusAddress() == m_ReaderInfo.aPeripheralDevice[ucCnt].ucPDBusAdr)
			{
				bFound = true;
				break;
			}
		}

		if(!bFound)
		{
#ifdef _FEDM_LINUX
			m_mapPeripheralDevices.erase(itor++);
#else
			itor = m_mapPeripheralDevices.erase(itor);
#endif
		}
		else
		{
			itor++;
		}
	}

	// create now all new peripheral devices
	for(unsigned char ucCnt=0; ucCnt<m_ReaderInfo.ucNoOfPeripheralDevices; ucCnt++)
	{
		itor = m_mapPeripheralDevices.find(m_ReaderInfo.aPeripheralDevice[ucCnt].ucPDBusAdr);
		if(itor == m_mapPeripheralDevices.end())
		{
			// pd not fount >> create a new one
			switch(m_ReaderInfo.aPeripheralDevice[ucCnt].ucPDType)
			{
			case FEDM_ISC_PD_TYPE_PEOPLE_COUNTER:
				{
				FedmIscPeopleCounter* pPD = new FedmIscPeopleCounter(this, m_ReaderInfo.aPeripheralDevice[ucCnt].ucPDBusAdr);
				m_mapPeripheralDevices.insert(make_pair(m_ReaderInfo.aPeripheralDevice[ucCnt].ucPDBusAdr, (FedmIscPeripheralDevice*)pPD));
				}
				break;

			case FEDM_ISC_PD_TYPE_EXTERNAL_IO:
				{
				FedmIscExternalIO* pPD = new FedmIscExternalIO(this, m_ReaderInfo.aPeripheralDevice[ucCnt].ucPDBusAdr);
				m_mapPeripheralDevices.insert(make_pair(m_ReaderInfo.aPeripheralDevice[ucCnt].ucPDBusAdr, (FedmIscPeripheralDevice*)pPD));
				}
				break;
			}
			continue;
		}

		if(itor->second->GetPDType() != m_ReaderInfo.aPeripheralDevice[ucCnt].ucPDType)
		{
			// pd type changed >> remove old pd and create a new one
			delete itor->second;
			m_mapPeripheralDevices.erase(itor);

			switch(m_ReaderInfo.aPeripheralDevice[ucCnt].ucPDType)
			{
			case FEDM_ISC_PD_TYPE_PEOPLE_COUNTER:
				{
				FedmIscPeopleCounter* pPD = new FedmIscPeopleCounter(this, m_ReaderInfo.aPeripheralDevice[ucCnt].ucPDBusAdr);
				m_mapPeripheralDevices.insert(make_pair(m_ReaderInfo.aPeripheralDevice[ucCnt].ucPDBusAdr, (FedmIscPeripheralDevice*)pPD));
				}
				break;

			case FEDM_ISC_PD_TYPE_EXTERNAL_IO:
				{
				FedmIscExternalIO* pPD = new FedmIscExternalIO(this, m_ReaderInfo.aPeripheralDevice[ucCnt].ucPDBusAdr);
				m_mapPeripheralDevices.insert(make_pair(m_ReaderInfo.aPeripheralDevice[ucCnt].ucPDBusAdr, (FedmIscPeripheralDevice*)pPD));
				}
				break;
			}
		}
	}
}
#endif


int FEDM_ISCReader::Set_0x6E_ReaderDiagnosticData(unsigned char ucMode, unsigned char* ucData)
{
	return Set_0x6E_ReaderDiagnosticData(ucMode, ucData, &m_ReaderDiagnostic);
}


/***************************************************************************
  Begin        		:	11.12.2008 / M. Hultsch
  
  Version       	:	04.06.00 / 12.12.2013 / M. Hultsch
						- support for new mode 0x07

						04.03.01 / 05.11.2012 / M. Hultsch
						- support for new mode 0x21

						03.00.18 / 15.12.2009 / M. Hultsch

  Function			:	sets the received data of last [0x6E] Reader Diagnostic protocol
						with mode 0xFF (means all) into FEDM_ISC_READER_DIAGNOSTIC

  Parameters		:	unsigned char ucMode	-	mode byte
						unsigned char* ucData	-	pointer to buffer with data

  Return value		:	OK (=0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Set_0x6E_ReaderDiagnosticData(unsigned char ucMode, unsigned char* ucData, FEDM_ISC_READER_DIAGNOSTIC* pReaderDiagnostic)
{
	unsigned char ucNextMode = 0;
	unsigned int uiByteCnt = 0;
	unsigned int uiMaxRecCnt = 1;
	unsigned int uiRecCnt = 0;

	if(ucMode == 0xFF)
	{
		uiMaxRecCnt = ucData[uiByteCnt++];
		pReaderDiagnostic->Init();
	}
	else
	{
		ucNextMode = ucMode;
	}

	for(uiRecCnt=0; uiRecCnt<uiMaxRecCnt; uiRecCnt++)
	{
		if(ucMode == 0xFF)
		{
			ucNextMode = ucData[uiByteCnt++];
		}

		switch(ucNextMode)
		{
		case 0x01: // General RF Status
			memcpy(&pReaderDiagnostic->ucFlagsA, &ucData[uiByteCnt], 2);
			pReaderDiagnostic->bIsMode0x01Read = true;
			pReaderDiagnostic->bIsDiagnosticRead = true;
			uiByteCnt += 30;
			break;

		case 0x02: // RF Power Amplifier Status
			memcpy(&pReaderDiagnostic->ucRfPower, &ucData[uiByteCnt], 3);
			pReaderDiagnostic->bIsMode0x02Read = true;
			pReaderDiagnostic->bIsDiagnosticRead = true;
			uiByteCnt += 30;
			break;

		case 0x03: // Multiplexer Status
			memcpy(&pReaderDiagnostic->ucMuxStatusCh1, &ucData[uiByteCnt], 8);
			pReaderDiagnostic->bIsMode0x03Read = true;
			pReaderDiagnostic->bIsDiagnosticRead = true;
			uiByteCnt += 30;
			break;

		case 0x04: // Hardware and Configuration Status
			memcpy(&pReaderDiagnostic->ucIntErrorA, &ucData[uiByteCnt], 2);
			pReaderDiagnostic->bIsMode0x04Read = true;
			pReaderDiagnostic->bIsDiagnosticRead = true;
			uiByteCnt += 30;
			break;

		case 0x05: // Multiplexer Impedance Status
			switch(m_uiReaderType)
			{
			case FEDM_ISC_TYPE_ISCLRU2000:
				memcpy(&pReaderDiagnostic->CNTRL_OUT1.ucMux_PowerAmp, &ucData[uiByteCnt], 12);
				pReaderDiagnostic->bIsMode0x05Read = true;
				pReaderDiagnostic->bIsDiagnosticRead = true;
				uiByteCnt += 30;
				break;

			case FEDM_ISC_TYPE_ISCLRU3000:
				memcpy(&pReaderDiagnostic->CNTRL_OUT1.ucMux_PowerAmp, &ucData[uiByteCnt], 12);
				pReaderDiagnostic->bIsMode0x05Read = true;
				pReaderDiagnostic->bIsDiagnosticRead = true;
				uiByteCnt += 30;
				break;

			default:
				memcpy(&pReaderDiagnostic->CNTRL_OUT1.ucMux_PowerAmp, &ucData[uiByteCnt], 3);
				pReaderDiagnostic->bIsMode0x05Read = true;
				pReaderDiagnostic->bIsDiagnosticRead = true;
				uiByteCnt += 30;
				break;
			}
			break;

		case 0x06: // MAX Status
			memcpy((char*)&pReaderDiagnostic->usErrorFlags, &ucData[uiByteCnt], 6);
			pReaderDiagnostic->bIsMode0x06Read = true;
			pReaderDiagnostic->bIsDiagnosticRead = true;
			uiByteCnt += 30;
			break;

		case 0x07: // PRH200 LED Status
			memcpy((char*)&pReaderDiagnostic->ucNoOfLEDs, &ucData[uiByteCnt], 30);
			pReaderDiagnostic->bIsMode0x07Read = true;
			pReaderDiagnostic->bIsDiagnosticRead = true;
			uiByteCnt += 30;
			break;

		// ID ISC.LR2500-B Antenna Tuning Status
		case 0x08:
			memcpy((char*)&pReaderDiagnostic->usRes1, &ucData[uiByteCnt], 15);
			pReaderDiagnostic->bIsMode0x08Read = true;
			pReaderDiagnostic->bIsDiagnosticRead = true;
			uiByteCnt += 30;
			break;

		case 0x20: // Firmware Status
			memcpy(&pReaderDiagnostic->ucFirmwareStatus[0], &ucData[uiByteCnt], 30);
			pReaderDiagnostic->bIsMode0x20Read = true;
			pReaderDiagnostic->bIsDiagnosticRead = true;
			uiByteCnt += 30;
			break;

		case 0x21: // Config-Para Change
			pReaderDiagnostic->ucConfigParaChange = ucData[uiByteCnt];
			pReaderDiagnostic->bIsMode0x21Read = true;
			pReaderDiagnostic->bIsDiagnosticRead = true;
			uiByteCnt += 30;
			break;

		default:
			uiByteCnt += 30;
			break;
		}
	}

	return 0;
}

int FEDM_ISCReader::Get_0x72_OutputData(unsigned char ucOutN, unsigned char* pOutRec)
{
	if(ucOutN > 8)
		return FEDM_ERROR_PARAMETER;

	int iFirstAdr = FEDM_GetAdrOfID(FEDM_ISC_TMP_0x72_OUT_TYPE_1, 32);

	for(unsigned char i=0; i<ucOutN; ++i)
	{
		memcpy(&pOutRec[4*i], &m_TmpData[iFirstAdr+4*i], 4*sizeof(unsigned char));
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////
// function for buffered read mode
//////////////////////////////////////////////////////////////////////

/***************************************************************************
  Begin        		:	08.06.2000 / M. Hultsch
  Version       	:	04.02.04 / 19.06.2012 / M. Hultsch
						- support for direction information (in combination with Gate People Counter)

						03.03.04 / 04.07.2011 / M. Hultsch

  Function			:	sets the received data of last read buffer protocol
						into BRM-table
						
						!! Attention!!
						When serialnumber is multiple in data stream,
						then the record set is only insert once in the table

  Parameters		:	unsigned char* ucData	-	pointer to buffer with data

  Return value		:	OK (=0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::SetBrmTableData(unsigned char ucCmd, unsigned char* ucData)
{
	FEDM_CHK3(ucData);

	if(m_BRMTable.size() <= 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmd << ") " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
		return FEDM_ERROR_NO_TABLE_SIZE;
	}

	if(ucCmd != 0x21 && ucCmd != 0x22 && ucCmd != 0x74)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmd << ") " << GetErrorText(FEDM_ERROR_UNKNOWN_COMMAND));
		return FEDM_ERROR_UNKNOWN_COMMAND;
	}

	unsigned char	ucTrData[2];
	unsigned int	uiSets	= 0;
	int		iErr	= 0;
	unsigned int	uiByteCnt = 0;

	memset(ucTrData, 0, 2);

	switch(ucCmd)
	{
	case 0x21:
		FEDM_CHK1(iErr, GetData(FEDM_ISCLR_TMP_BRM_TRDATA,	&ucTrData[0]));
		FEDM_CHK1(iErr, GetData(FEDM_ISCLR_TMP_BRM_RECSETS, &uiSets));
		break;

	case 0x22:
	case 0x74:
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_ADV_BRM_TRDATA1, &ucTrData[0]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_ADV_BRM_TRDATA2, &ucTrData[1]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_ADV_BRM_RECSETS, &uiSets));
		break;
	}


	// reset BRM-table
	m_uiBRMTableLength = 0;
	
	// write data in BrmTable
	for( unsigned int iCnt=0; iCnt<uiSets; iCnt++ )
	{
		m_BRMTable[iCnt]->Init();
		iErr = m_BRMTable[iCnt]->SetData(ucCmd, ucData, ucTrData, uiByteCnt);
		if(iErr < 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmd << ") " << GetErrorText(iErr));
			return iErr;
		}

		m_uiBRMTableLength++;

		if(m_uiBRMTableLength > (unsigned int)m_BRMTable.size())
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmd << ") " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
			return FEDM_ERROR_ARRAY_BOUNDARY;
		}
	}

	return FEDM_OK;
}



//////////////////////////////////////////////////////////////////////
// functions for [0xB0], [0xB1], [0xB2] ISO Host Protocol
//////////////////////////////////////////////////////////////////////

/***************************************************************************
  Begin        		:	08.06.2000 / M. Hultsch
  Version       	:	04.06.06 / 23.09.2013 / M. Hultsch
						- only for EPC Class1 Gen2:
						  bugfix for addressed Host commands, if Reader is configured for UID = EPC + TID

						04.06.01 / 07.03.2014 / M. Hultsch
						- support for [0x22] Lock Multiple Blocks with extended addressed mode
						- support for [0x2C] Get Multiple Block Security Status with extended addressed mode

						04.02.00 / 11.05.2012 / M. Hultsch
						- only for EPC Class1 Gen2:
						  if Reader is configured for UID = EPC + TID, use IDDT from last Inventory to evaluate UID

						03.03.04 / 21.06.2011 / M. Hultsch
						- only for EPC Class1 Gen2:
						  if Reader is configured for UID = EPC + TID, set TID as UID for next request in addressed mode

						03.00.12 / 01.09.2009 / M. Hultsch
						- new mode for Select command: request with manual selected transponder driver
						
						03.00.07 / 29.04.2008 / M. Hultsch
						- new mode for Inventory command: request with antenna select
						
						03.00.06 / 31.03.2008 / M. Hultsch
						- if m_bSelectWithCinf=true (preset only with classic-pro Reader)
						  execute always with CINF-flag
						
						02.04.01 / 22.02.2006 / M. Hultsch

  Function			:	collects the necessary data for the next [0xB0] iso protocol

  Parameters		:	unsigned char ucCmdByte	-	iso command byte
						unsigned char* ucData	-	pointer to buffer for data

  Return value		:	iCnt (>0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Get_0xB0_IsoTableData(unsigned char ucCmdByte, unsigned char* ucData)
{
	FEDM_CHK3(ucData);

	bool bUidLF = false;
	bool bExtAdr = false;
	bool bAccessPw = false;
	bool bNotify = false;
	bool bAntenna = false;
	int iCnt = 0;
	int iErr = 0;
	int iIdx = 0;
	unsigned char ucUidLen	= 0x00;
	unsigned char ucEpcLen	= 0x00;
	unsigned char ucLen		= 0x00;
	unsigned char ucUid[FEDM_ISC_MAX_UID_LENGTH];
	string sUid = "";

	memset(ucUid, 0, FEDM_ISC_MAX_UID_LENGTH);

	switch(ucCmdByte)
	{
	case 0x01:	// [0x01] Read Serial Number
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_CMD, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE_NOTIFY, &bNotify));
		if(bNotify)
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_NOTIFY_TIMEOUT, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE_ANT, &bAntenna));
		if(bAntenna)
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_ANT_SEL, &ucData[iCnt++]));

		break;

	case 0x02:	// [0x02] Stay Quiet
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_CMD, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE, &ucData[iCnt++]));
		// UID = unified identifier = serial number
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, &ucData[iCnt], 8));
		iCnt += 8;
		break;

	case 0x22:	// [0x22] Lock Multiple Blocks
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_CMD, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE_EXT_ADR, &bExtAdr));
		if(bExtAdr)
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_BANK, &ucData[iCnt++]));
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_DB_ADR_EXT, &ucData[iCnt], 2));
			iCnt += 2;
		}
		else
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_DB_ADR, &ucData[iCnt++]));
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_DBN, &ucData[iCnt++]));
		break;

	case 0x23:	// [0x23] Read Multiple Blocks
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_CMD, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE_UID_LF, &bUidLF));
			if(bUidLF)	// variable UID length
			{
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID_LEN, &ucUidLen));
			}
			else
			{
				ucUidLen = 8;
			}

			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, &ucUid[0], (int)ucUidLen));
			FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], (int)ucUidLen, sUid));
			
			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
			if(iIdx < 0)
				return iIdx;

			// only for UHF EPC Class1 Gen2
			if(	m_ISOTable[iIdx]->m_ucTrType == FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2 ||
				m_ISOTable[iIdx]->m_ucTrType == FEDM_ISC_TR_TYPE_ISO18000_3M3)
			{
				// check, if Reader is configured for UID = EPC + TID
				if(m_ISOTable[iIdx]->m_ucIDDT == 0x02)
				{
					// check, if response contains UID = EPC + TID
					ucEpcLen = ((m_ISOTable[iIdx]->m_ucClass1Gen2_PC[0] & 0xF8) >> 3) * 2;
					if(m_ISOTable[iIdx]->m_ucClass1Gen2_PC[0] & 0x02) // extended PC W1
						ucEpcLen -= 2;

					if( ucEpcLen < ucUidLen)
					{
						// take TID as UID for next request
						if(bUidLF || ((ucUidLen - ucEpcLen) != 8))
							ucData[iCnt++] = (ucUidLen - ucEpcLen);

						memcpy(&ucData[iCnt], &ucUid[ucEpcLen], ucUidLen - ucEpcLen);
						iCnt += (ucUidLen - ucEpcLen);
					}
					else
					{
						// error in the Reader response: TID is missing
						FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_INVENTORY_NO_TID_IN_UID));
						return FEDM_ERROR_INVENTORY_NO_TID_IN_UID;
					}
				}
				else
				{
					if(bUidLF || (ucUidLen != 8))
						ucData[iCnt++] = ucUidLen;
					
					// UID = unified identifier = serial number
					FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, &ucData[iCnt], ucUidLen));
					iCnt += ucUidLen;
				}
			}
			else
			{
				if(bUidLF)	// variable UID length
				{
					ucData[iCnt++] = ucUidLen;
					// UID = unified identifier = serial number
					FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, &ucData[iCnt], ucUidLen));
					iCnt += ucUidLen;
				}
				else
				{
					// UID = unified identifier = serial number
					FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, &ucData[iCnt], 8));
					iCnt += 8;
				}
			}
		}

		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE_EXT_ADR, &bExtAdr));
		if(bExtAdr)
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_BANK, &ucData[iCnt++]));
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_BANK_ACCESS_FLAG, &bAccessPw));
			if(bAccessPw)
			{
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_ACCESS_PW_LENGTH, &ucLen));
				ucData[iCnt++] = ucLen;
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_ACCESS_PW, &ucData[iCnt], ucLen));
				iCnt += ucLen;
			}
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_DB_ADR_EXT, &ucData[iCnt], 2));
			iCnt += 2;
		}
		else
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_DB_ADR, &ucData[iCnt++]));
		}

		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_DBN, &ucData[iCnt++]));
		break;

	case 0x24:	// [0x24] Write Multiple Blocks
		FEDM_CHK1(iErr, Get_0xB0_0x24_IsoTableData(ucData));
		iCnt += iErr;
		break;

	case 0x25:	// [0x25] Select
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_CMD, &ucData[iCnt++]));

		// new since V3.00.06 to request more tag information
		if( m_bSelectWithCinf && (!m_bDisableSelectWithCinf) )
		{
			FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B0_MODE_CINF, true));
		}

		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE, &ucData[iCnt++]));

		// new since V3.00.12
		if( ucData[1] & FEDM_ISC_ISO_MODE_DRV_SEL )	// manual selection of transponder driver
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_TR_DRIVER, &ucData[iCnt++]));
		}

		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE_UID_LF, &bUidLF));
			if(bUidLF)	// variable UID length
			{
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID_LEN, &ucUidLen));
				ucData[iCnt++] = ucUidLen;
				// UID = unified identifier = serial number
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, &ucData[iCnt], ucUidLen));
				iCnt += ucUidLen;
			}
			else
			{
				// UID = unified identifier = serial number
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, &ucData[iCnt], 8));
				iCnt += 8;
			}
		}
		break;

	case 0x26:	// [0x26] Reset to Ready
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_CMD, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		break;

	case 0x27:	// [0x27] Write AFI
		FEDM_CHK1(iErr, Get_0xB0_0x27_IsoTableData(ucData));
		iCnt += iErr;
		break;

	case 0x28:	// [0x28] Lock AFI
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_CMD, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		break;

	case 0x29:	// [0x29] Write DSFID
		FEDM_CHK1(iErr, Get_0xB0_0x29_IsoTableData(ucData));
		iCnt += iErr;
		break;

	case 0x2A:	// [0x2A] Lock DSFID
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_CMD, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == 0x01)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		break;

	case 0x2B:	// [0x2B] Get System Information
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_CMD, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		break;

	case 0x2C:	// [0x2C] Get Multiple Block Security Status
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_CMD, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE_EXT_ADR, &bExtAdr));
		if(bExtAdr)
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_BANK, &ucData[iCnt++]));
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_DB_ADR_EXT, &ucData[iCnt], 2));
			iCnt += 2;
		}
		else
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_DB_ADR, &ucData[iCnt++]));
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_DBN, &ucData[iCnt++]));
		break;

	case 0xA0:	// [0xA0] Read Config Block
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_CMD, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_CB_ADR, &ucData[iCnt++]));
		break;
	
	case 0xA1:	// [0xA1] Write Config Block
		FEDM_CHK1(iErr, Get_0xB0_0xA1_IsoTableData(ucData));
		iCnt += iErr;
		break;

	case 0xC0:	// [0xC0] Halt
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_CMD, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE, &ucData[iCnt++]));
		break;
	}

	return iCnt;
}

/***************************************************************************
  Begin        		:	28.07.2005 / M. Hultsch
  Version       	:	03.03.01 / 31.03.2011 / M. Hultsch

  Function			:	collects the necessary data for the next [0xB1] iso protocol

  Parameters		:	unsigned char ucCmdByte	-	iso command byte
						unsigned char* ucData	-	pointer to buffer for data

  Return value		:	iCnt (>0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Get_0xB1_IsoTableData(unsigned char ucCmdByte, unsigned char* ucData)
{
	FEDM_CHK3(ucData);

	int iErr = 0;
	unsigned char ucMfr		= 0x00;

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MFR, &ucMfr));

	switch(ucMfr)
	{
	case FEDM_ISC_ISO_MFR_NXP:
		return Get_0xB1_IsoTableData_NXP(ucCmdByte, ucData);
		
	case FEDM_ISC_ISO_MFR_TI:
		return Get_0xB1_IsoTableData_TI(ucCmdByte, ucData);

	case FEDM_ISC_ISO_MFR_STM:
		return Get_0xB1_IsoTableData_STM(ucCmdByte, ucData);

	case FEDM_ISC_ISO_MFR_EM:
		return Get_0xB1_IsoTableData_EM(ucCmdByte, ucData);

	case FEDM_ISC_ISO_MFR_INFINEON:
		return Get_0xB1_IsoTableData_Infineon(ucCmdByte, ucData);

	case FEDM_ISC_ISO_MFR_FUJITSU:
		return Get_0xB1_IsoTableData_Fujitsu(ucCmdByte, ucData);

	case FEDM_ISC_ISO_MFR_KSW:
		return Get_0xB1_IsoTableData_KSW(ucCmdByte, ucData);

	case FEDM_ISC_ISO_MFR_IDS:
		return Get_0xB1_IsoTableData_IDS(ucCmdByte, ucData);

	}

	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ", Manufacturer=0x" << std::hex << std::uppercase << (int)ucMfr << ") - is not supported");
	FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
}

/***************************************************************************
  Begin        		:	07.10.2009 / M. Hultsch
  Version       	:	03.00.13 / 07.10.2009 / M. Hultsch

  Function			:	collects the necessary data for the next [] iso protocol
						manufacturer NXP

  Parameters		:	unsigned char ucCmdByte	-	iso command byte
						unsigned char* ucData	-	pointer to buffer for data

  Return value		:	iCnt (>0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Get_0xB1_IsoTableData_NXP(unsigned char ucCmdByte, unsigned char* ucData)
{
	int iCnt = 0;
	int iErr = 0;
	int iIdx = 0;
	unsigned int uiDBAdr=0;

	switch(ucCmdByte)
	{
	case 0xA0:  // [0xA0] Inventory Read
	case 0xA1:  // [0xA1] Fast Inventory Read
	case 0xB0:  // [0xB0] Inventory Page Read 
	case 0xB1:  // [0xB1] Fast Page Inventory Read 
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_NXP_DB_ADR, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_NXP_DB_N, &ucData[iCnt++]));
		break;

	case 0xA2:	// [0xA2] Set EAS
	case 0xA3:	// [0xA3] Reset EAS
	case 0xA4:	// [0xA4] Lock EAS
	case 0xA5:	// [0xA5] EAS Alarm
	case 0xA6:	// [0xA6] Password Protect EAS
	case 0xA8:	// [0xA8] Read EPC
	case 0xB2:	// [0xB2] Get Random Number
	case 0xB9:	// [0xB9] Destroy SLI-S
	case 0xBA:	// [0xBA] Enable Privacy
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		break;

	case 0xA7:	// [0xA7] Write EAS ID
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_NXP_EAS_ID, &ucData[iCnt], 2));
		iCnt += 2;
		break;

	case 0xB3:	// [0xB3] Set Password
	case 0xB4:	// [0xB4] Write Password
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_NXP_PASSWORD_ID, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_NXP_PASSWORD, &ucData[iCnt], 4));
		iCnt += 4;
		break;

	case 0xB5:	// [0xB5] Lock Password
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_NXP_PASSWORD_ID, &ucData[iCnt++]));
		break;

	case 0xB6:	// [0xB6] Protect Page
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_NXP_PAGE_NO, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_NXP_PROTECTION_STATUS, &ucData[iCnt++]));
		break;

	case 0xB7:	// [0xB7] Lock Page Protection Condition
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_NXP_PAGE_NO, &ucData[iCnt++]));
		break;

	case 0xB8:	// [0xB8] Get Multiple Bloock Protection Status
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_DB_ADR, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_DBN, &ucData[iCnt++]));
		break;

	case 0xBB:	// [0xBB] 64 Bit Password Protection
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_NXP_64BIT_PWD_PARAMETER, &ucData[iCnt++]));
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") - is not supported");
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	}

	return iCnt;
}

/***************************************************************************
  Begin        		:	07.10.2009 / M. Hultsch
  Version       	:	03.00.13 / 07.10.2009 / M. Hultsch

  Function			:	collects the necessary data for the next [0xB1] iso protocol
						manufacturer TI

  Parameters		:	unsigned char ucCmdByte	-	iso command byte
						unsigned char* ucData	-	pointer to buffer for data

  Return value		:	iCnt (>0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Get_0xB1_IsoTableData_TI(unsigned char ucCmdByte, unsigned char* ucData)
{
	int iCnt = 0;
	int iErr = 0;
	int iIdx = 0;
	unsigned char ucUid[FEDM_ISC_MAX_UID_LENGTH];
	string sUid = "";
	unsigned int uiDBAdr=0;
	
	switch(ucCmdByte)
	{
	case 0xA2:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucUid[0], 8));
			FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], 8, sUid));
			

			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
			if(iIdx < 0)
				return iIdx;

			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_TI_DB_ADR, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_TI_DB_SIZE, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_TI_EVEN_DB, &ucData[iCnt], 4));
		iCnt += 4;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_TI_ODD_DB, &ucData[iCnt], 4));
		iCnt += 4;
		break;

	case 0xA3:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucUid[0], 8));
			FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], 8, sUid));
			

			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
			if(iIdx < 0)
				return iIdx;

			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_TI_DB_ADR, &ucData[iCnt++]));
		break;
	
	case 0xA4:	// [0xA4] Kill (Tag it HF-I Light)
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_TI_PASSWORD, &ucData[iCnt], 4));
		iCnt += 4;
		break;

	case 0xA5:	// [0xA5] Write Single Block Pwd (Tag it HF-I Light)
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucUid[0], 8));
			FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], 8, sUid));
			

			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
			if(iIdx < 0)
				return iIdx;

			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_TI_PASSWORD, &ucData[iCnt], 4));
		iCnt += 4;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_DB_ADR, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_DB_ADR, &uiDBAdr));
		FEDM_CHK1(iErr, m_ISOTable[iIdx]->GetData(0x24, ucData, iCnt, FEDM_ISC_ISO_BANK_USER_MEMORY, uiDBAdr));
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") - is not supported");
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	}

	return iCnt;
}	

/***************************************************************************
  Begin        		:	07.10.2009 / M. Hultsch
  Version       	:	03.00.17 / 24.11.2009 / M. Hultsch

  Function			:	collects the necessary data for the next [0xB1] iso protocol
						manufacturer STM

  Parameters		:	unsigned char ucCmdByte	-	iso command byte
						unsigned char* ucData	-	pointer to buffer for data

  Return value		:	iCnt (>0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Get_0xB1_IsoTableData_STM(unsigned char ucCmdByte, unsigned char* ucData)
{
	int iCnt = 0;
	int iErr = 0;
	int iIdx = 0;
	unsigned char ucUid[FEDM_ISC_MAX_UID_LENGTH];
	string sUid = "";

	switch(ucCmdByte)
	{
	case 0xA0: // since V4.05.00: dual support for old LRI512 and new M24LRxxE-R
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucUid[0], 8));
			FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], 8, sUid));
			

			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
			if(iIdx < 0)
				return iIdx;

			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		break;

	case 0xA1: // since V4.05.00: removed support for old LRI512, new support for M24LRxxE-R
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucUid[0], 8));
			FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], 8, sUid));
			

			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
			if(iIdx < 0)
				return iIdx;

			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_STM_DATA, &ucData[iCnt++]));
		break;

	case 0xA2: // since V4.05.00: removed support for old LRI512, new support for M24LRxxE-R
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucUid[0], 8));
			FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], 8, sUid));
			

			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
			if(iIdx < 0)
				return iIdx;

			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_STM_DATA, &ucData[iCnt++]));
		break;

	case 0xA3:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucUid[0], 8));
			FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], 8, sUid));
			

			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
			if(iIdx < 0)
				return iIdx;

			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		break;

	case 0xA4:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucUid[0], 8));
			FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], 8, sUid));
			

			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
			if(iIdx < 0)
				return iIdx;

			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_STM_DATA, &ucData[iCnt++]));
		break;

	case 0xA6:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucUid[0], 8));
			FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], 8, sUid));
			

			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
			if(iIdx < 0)
				return iIdx;

			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_STM_KILL_CODE, &ucData[iCnt], 4));
		iCnt += 4;
		break;

	case 0xB1:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucUid[0], 8));
			FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], 8, sUid));
			

			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
			if(iIdx < 0)
				return iIdx;

			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_STM_PASSWORD_NO, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_STM_PASSWORD, &ucData[iCnt], 4));
		iCnt += 4;
		break;

	case 0xB2:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucUid[0], 8));
			FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], 8, sUid));
			

			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
			if(iIdx < 0)
				return iIdx;

			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_STM_INFO_BYTE, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_STM_BLOCK_NO, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_STM_PROTECT_STATUS, &ucData[iCnt++]));
		break ;

	case 0xB3:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucUid[0], 8));
			FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], 8, sUid));
			

			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
			if(iIdx < 0)
				return iIdx;

			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_STM_PASSWORD_NO, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_STM_PASSWORD, &ucData[iCnt], 4));
		iCnt += 4;
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") - is not supported");
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	}

	return iCnt;
}

/***************************************************************************
  Begin        		:	07.10.2009 / M. Hultsch
  Version       	:	03.00.13 / 07.10.2009 / M. Hultsch

  Function			:	collects the necessary data for the next [0xB1] iso protocol
						manufacturer Infineon

  Parameters		:	unsigned char ucCmdByte	-	iso command byte
						unsigned char* ucData	-	pointer to buffer for data

  Return value		:	iCnt (>0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Get_0xB1_IsoTableData_Infineon(unsigned char ucCmdByte, unsigned char* ucData)
{
	int iCnt = 0;
	int iErr = 0;
	int iIdx = 0;
	unsigned char ucBlockCnt = 0;
	unsigned char ucDBAdr = 0;
	unsigned char ucDBSize = 0;
	unsigned char ucDBN		= 0x00;
	unsigned char ucUid[FEDM_ISC_MAX_UID_LENGTH];
	string sUid = "";

	switch(ucCmdByte)
	{
	case 0x90:
	case 0x10:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucUid[0], 8));
			FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], 8, sUid));

			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
			if(iIdx < 0)
				return iIdx;

			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_INFINEON_DB_ADR, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_INFINEON_DB_N, &ucData[iCnt++]));
		break;

	case 0x30:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucUid[0], 8));
			FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], 8, sUid));

			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
			if(iIdx < 0)
				return iIdx;

			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_INFINEON_DB_ADR, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_INFINEON_DB_ADR, &ucDBAdr));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_INFINEON_DB_N, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_INFINEON_DB_N, &ucDBN));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_INFINEON_DB_SIZE, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_INFINEON_DB_SIZE, &ucDBSize));

		memcpy(&ucData[iCnt], &m_ISOTable[iIdx]->m_ucTxDB[ucDBAdr * ucDBSize], ucDBN * ucDBSize);
		iCnt += (unsigned int)(ucDBN * ucDBSize);
		break;

	case 0xAB:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucUid[0], 8));
			FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], 8, sUid));

			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
			if(iIdx < 0)
				return iIdx;

			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_INFINEON_AQM, &ucData[iCnt++]));
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") - is not supported");
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	}

	return iCnt;
}
/***************************************************************************
  Begin        		:	07.10.2009 / M. Hultsch
  Version       	:	03.00.13 / 08.10.2009 / M. Hultsch

  Function			:	collects the necessary data for the next [0xB1] iso protocol
						manufacturer EM

  Parameters		:	unsigned char ucCmdByte	-	iso command byte
						unsigned char* ucData	-	pointer to buffer for data

  Return value		:	iCnt (>0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Get_0xB1_IsoTableData_EM(unsigned char ucCmdByte, unsigned char* ucData)
{
	int iCnt = 0;
	int iErr = 0;
	int iIdx = 0;
	unsigned char ucUid[FEDM_ISC_MAX_UID_LENGTH];
	string sUid = "";
	switch(ucCmdByte)
	{
	case 0xA0:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_EM_PASSWORD, &ucData[iCnt], 4));
		iCnt += 4;
		break;

	case 0xE4:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucUid[0], 8));
			FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], 8, sUid));

			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
			if(iIdx < 0)
				return iIdx;

			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_EM_PASSWORD, &ucData[iCnt], 4));
		iCnt += 4;
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") - is not supported");
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	}

	return iCnt;
}
/***************************************************************************
  Begin        		:	07.10.2009 / M. Hultsch
  Version       	:	03.00.13 / 08.10.2009 / M. Hultsch

  Function			:	collects the necessary data for the next [0xB1] iso protocol
						manufacturer FUJITSU

  Parameters		:	unsigned char ucCmdByte	-	iso command byte
						unsigned char* ucData	-	pointer to buffer for data

  Return value		:	iCnt (>0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Get_0xB1_IsoTableData_Fujitsu(unsigned char ucCmdByte, unsigned char* ucData)
{
	int iCnt = 0;
	int iErr = 0;
	int iIdx = 0;
	unsigned char ucUid[FEDM_ISC_MAX_UID_LENGTH];
	string sUid = "";
	unsigned int uiDBAdr=0;
	
	switch(ucCmdByte)
	{
	case 0xA0:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		break;

	case 0xA1:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucUid[0], 8));
			FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], 8, sUid));

			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
			if(iIdx < 0) 
				return iIdx;

			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_FUJITSU_DATA, &ucData[iCnt++]));
		break;

	case 0xA2:
	case 0xA6:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucUid[0], 8));
			FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], 8, sUid));

			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
			if(iIdx < 0)
				return iIdx;

			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") - is not supported");
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	}

	return iCnt;
}

/***************************************************************************
  Begin        		:	07.10.2009 / M. Hultsch
  Version       	:	03.00.13 / 08.10.2009 / M. Hultsch

  Function			:	collects the necessary data for the next [0xB1] iso protocol
						manufacturer KSW

  Parameters		:	unsigned char ucCmdByte	-	iso command byte
						unsigned char* ucData	-	pointer to buffer for data

  Return value		:	iCnt (>0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Get_0xB1_IsoTableData_KSW(unsigned char ucCmdByte, unsigned char* ucData)
{
	int iCnt = 0;
	int iErr = 0;
	int iIdx = 0;
	unsigned char ucUid[FEDM_ISC_MAX_UID_LENGTH];
	string sUid = "";
	unsigned int uiDBAdr=0;
	
	switch(ucCmdByte)
	{
	case 0xA2:
	case 0xA3:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucUid[0], 8));
			FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], 8, sUid));

			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
			if(iIdx < 0)
				return iIdx;

			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		break;

	case 0xA4:
	case 0xA5:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucUid[0], 8));
			FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], 8, sUid));

			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
			if(iIdx < 0)
				return iIdx;

			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_KSW_PASSWORD, &ucData[iCnt], 6));
		iCnt += 6;
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") - is not supported");
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	}

	return iCnt;
}

/***************************************************************************
  Begin        		:	31.03.2011 / M. Hultsch
  Version       	:	03.03.01 / 31.03.2011 / M. Hultsch

  Function			:	collects the necessary data for the next [0xB1] iso protocol
						manufacturer IDS Microchip AG

  Parameters		:	unsigned char ucCmdByte	-	iso command byte
						unsigned char* ucData	-	pointer to buffer for data

  Return value		:	iCnt (>0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Get_0xB1_IsoTableData_IDS(unsigned char ucCmdByte, unsigned char* ucData)
{
	int iCnt = 0;
	int iErr = 0;
	
	switch(ucCmdByte)
	{
	case 0xA0:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_0xB1_REQ_IDS_PASSWORD_LEVEL, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_0xB1_REQ_IDS_PASSWORD, &ucData[iCnt], 4));
		iCnt += 4;
		break;

	case 0xA1:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_0xB1_REQ_IDS_LOG_MODE, &ucData[iCnt], 4));
		iCnt += 4;
		break;

	case 0xA2:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_0xB1_REQ_IDS_LOG_LIMITS, &ucData[iCnt], 4));
		iCnt += 4;
		break;

	case 0xA3:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		break;

	case 0xA4:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_0xB1_REQ_IDS_EXT_CAL_DATA, &ucData[iCnt], 4));
		iCnt += 4;
		break;

	case 0xA5:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_0xB1_REQ_IDS_INT_CAL_DATA, &ucData[iCnt], 4));
		iCnt += 4;
		break;

	case 0xA6:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		break;

	case 0xA7:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_0xB1_REQ_IDS_START_TIME, &ucData[iCnt], 4));
		iCnt += 4;
		break;

	case 0xA8:
	case 0xA9:
	case 0xAA:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		break;

	case 0xAB:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_0xB1_REQ_IDS_PASSWORD_LEVEL, &ucData[iCnt++]));
		break;

	case 0xAC:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_0xB1_REQ_IDS_INIT_DATA, &ucData[iCnt], 4));
		iCnt += 4;
		break;

	case 0xAD:
		ucData[iCnt++] = ucCmdByte;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") - is not supported");
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	}

	return iCnt;
}

/***************************************************************************
  Begin        		:	28.07.2005 / M. Hultsch
  Version       	:	03.00.07 / 05.05.2009 / M. Hultsch

  Function			:	collects the necessary data for the next [0xB2] iso protocol

  Parameters		:	unsigned char ucCmdByte	-	iso command byte
						unsigned char* ucData	-	pointer to buffer for data

  Return value		:	iCnt (>0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Get_0xB2_IsoTableData(unsigned char ucCmdByte, unsigned char* ucData)
{
	FEDM_CHK3(ucData);

	bool bKL = false;
	int iCnt = 0;
	int iErr;

	switch(ucCmdByte)
	{
	case 0x2B:	// [0x2B] ISO14443-4 Transponder Info
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B2_CMD, &ucData[iCnt++]));
		break;

	case 0x30:	// [0x30] Mifare Value Commands
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B2_CMD, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B2_MODE, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B2_REQ_MF_CMD, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B2_REQ_DB_ADR, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B2_REQ_OP_VALUE, &ucData[iCnt], 4));
		iCnt += 4;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B2_REQ_DEST_ADR, &ucData[iCnt++]));
		break;

	case 0xB0:	// [0xB0] Authent Mifare
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B2_MODE_KL, &bKL));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B2_CMD, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B2_MODE, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B2_REQ_DB_ADR, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B2_REQ_KEY_TYPE, &ucData[iCnt++]));
		if(bKL)
		{
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_ISO14443A_KEY, &ucData[iCnt], 6));
			iCnt += 6;
		}
		else
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B2_REQ_KEY_ADR, &ucData[iCnt++]));
		}
		break;

	case 0xB1:	// [0xB1] Authent my-d
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B2_CMD, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B2_MODE, &ucData[iCnt++]));
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B2_REQ_UID, &ucData[iCnt], 8));
			iCnt += 8;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B2_REQ_KEY_ADR_TAG, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B2_REQ_AUTH_COUNTER_ADR, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B2_REQ_KEY_AUTH_SEQUENCE, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B2_REQ_KEY_ADR_SAM, &ucData[iCnt++]));
		// reserved bytes
		ucData[iCnt++] = 0;
		ucData[iCnt++] = 0;
		ucData[iCnt++] = 0;
		ucData[iCnt++] = 0;
		ucData[iCnt++] = 0;
		ucData[iCnt++] = 0;
		break;

	case 0xB2:	// [0xB2] Authent Mifare Ultralight C
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B2_CMD, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B2_MODE, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B2_REQ_KEY_INDEX, &ucData[iCnt++]));
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") - is not supported");
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	}

	return iCnt;
}

/***************************************************************************
  Begin        		:	08.03.2001 / M. Hultsch
  Version       	:	04.06.06 / 23.09.2013 / M. Hultsch
						- only for EPC Class1 Gen2:
						  bugfix for addressed Host commands, if Reader is configured for UID = EPC + TID

						04.02.00 / 11.05.2012 / M. Hultsch
						- only for EPC Class1 Gen2:
						  if Reader is configured for UID = EPC + TID, use IDDT from last Inventory to evaluate UID

						03.03.04 / 21.06.2011 / M. Hultsch
						- only for EPC Class1 Gen2:
						  if Reader is configured for UID = EPC + TID, set TID as UID for next request in addressed mode

						03.00.13 / 07.10.2009 / M. Hultsch

  Function			:	collects the necessary data for the next [0xB0][0x24] iso protocol

  Parameters		:	unsigned char* ucData	-	pointer to buffer for data

  Return value		:	iCnt (>0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::Get_0xB0_0x24_IsoTableData(unsigned char* ucData)
{
	FEDM_CHK3(ucData);

	bool bUidLF = false;
	bool bExtAdr = false;
	bool bAccessPw = false;
	int iCnt = 0;
	int iIdx, iErr, iBlockCnt;
	unsigned char ucBank	= 0x03;	// User Memory Bank
	unsigned char ucDBN		= 0x00;
	unsigned char ucBlockSize = 0x04;
	unsigned char ucUidLen	= 0x00;
	unsigned char ucEpcLen	= 0x00;
	unsigned char ucLen		= 0x00;
	unsigned char ucUid[FEDM_ISC_MAX_UID_LENGTH];
	unsigned int uiDBAdr	= 0;
	string sUid = "";

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_CMD, &ucData[iCnt++]));
	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE, &ucData[iCnt++]));

	if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_NONADR)	// non addressed
	{
		iIdx = m_uiNonAddressedTabIndex;	// take data from entry with index >= 0
	}
	else if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
	{
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE_UID_LF, &bUidLF));
		if(bUidLF)
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID_LEN, &ucUidLen));
		}
		else
		{
			ucUidLen = 8;
		}

		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, &ucUid[0], (int)ucUidLen));
		FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], (int)ucUidLen, sUid));
		
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
		if(iIdx < 0)
			return iIdx;

		// only for UHF EPC Class1 Gen2
		if(	m_ISOTable[iIdx]->m_ucTrType == FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2 ||
			m_ISOTable[iIdx]->m_ucTrType == FEDM_ISC_TR_TYPE_ISO18000_3M3)
		{
			// check, if Reader is configured for UID = EPC + TID
			if(m_ISOTable[iIdx]->m_ucIDDT == 0x02)
			{
				// check, if response contains UID = EPC + TID
				ucEpcLen = ((m_ISOTable[iIdx]->m_ucClass1Gen2_PC[0] & 0xF8) >> 3) * 2;
				if(m_ISOTable[iIdx]->m_ucClass1Gen2_PC[0] & 0x02) // extended PC W1
					ucEpcLen -= 2;

				if( ucEpcLen < ucUidLen)
				{
					// take TID as UID for next request
					if(bUidLF || ((ucUidLen - ucEpcLen) != 8))
						ucData[iCnt++] = (ucUidLen - ucEpcLen);
					
					memcpy(&ucData[iCnt], &ucUid[ucEpcLen], ucUidLen - ucEpcLen);
					iCnt += (ucUidLen - ucEpcLen);
				}
				else
				{
					// error in the Reader response: TID is missing
					FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_INVENTORY_NO_TID_IN_UID));
					return FEDM_ERROR_INVENTORY_NO_TID_IN_UID;
				}
			}
			else
			{
				if(bUidLF || (ucUidLen != 8))
					ucData[iCnt++] = ucUidLen;

				// UID = unified identifier = serial number
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, &ucData[iCnt], ucUidLen));
				iCnt += ucUidLen;
			}
		}
		else
		{
			if(bUidLF)	// variable UID length
			{
				ucData[iCnt++] = ucUidLen;
				// UID = unified identifier = serial number
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, &ucData[iCnt], ucUidLen));
				iCnt += ucUidLen;
			}
			else
			{
				// UID = unified identifier = serial number
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, &ucData[iCnt], 8));
				iCnt += 8;
			}
		}
	}
	else if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_SEL)	// selected
	{
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
		if(iIdx < 0)
		{
			// exception for ISO 14443 Transponder, when Reader is configuered with Anticollision = off
			if(  m_uiISOTableLength == 1 && (m_ISOTable[0]->m_ucTrType == FEDM_ISC_TR_TYPE_ISO14443A || m_ISOTable[0]->m_ucTrType == FEDM_ISC_TR_TYPE_ISO14443B) )
				iIdx = 0;
			else
				return iIdx;
		}
	}
	else
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNKNOWN_ADDRESS_MODE));
		return FEDM_ERROR_UNKNOWN_ADDRESS_MODE;
	}

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE_EXT_ADR, &bExtAdr));
	if(bExtAdr)
	{
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_BANK, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_BANK_BANK_NR, &ucBank));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_BANK_ACCESS_FLAG, &bAccessPw));
		if(bAccessPw)
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_ACCESS_PW_LENGTH, &ucLen));
			ucData[iCnt++] = ucLen;
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_ACCESS_PW, &ucData[iCnt], ucLen));
			iCnt += ucLen;
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_DB_ADR_EXT, &ucData[iCnt], 2));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_DB_ADR_EXT, &uiDBAdr));
		iCnt += 2;
	}
	else
	{
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_DB_ADR, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_DB_ADR, &uiDBAdr));
	}

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_DBN, &ucData[iCnt++]));
	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_DBN, &ucDBN));
	

	// take the blocksize from table
	ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
	FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B0_REQ_DB_SIZE, ucBlockSize));
	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_DB_SIZE, &ucData[iCnt++]));

	// avoid buffer overflow
	if( (5+iCnt+ucDBN*ucBlockSize) > FEDM_MAX_PROTOCOL_SIZE)	//    5 : general protocol frame
																// iCnt : [0x24] protocol frame
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_SENDBUFFER_OVERFLOW));
		return FEDM_ERROR_SENDBUFFER_OVERFLOW;
	}

	for(iBlockCnt=0; iBlockCnt<ucDBN; iBlockCnt++)
	{
		FEDM_CHK1(iErr, m_ISOTable[iIdx]->GetData(0x24, ucData, iCnt, ucBank, uiDBAdr));
		uiDBAdr++;
	}

	return iCnt;
}

/***************************************************************************
  Begin        		:	19.03.2001 / M. Hultsch
  Version       	:	03.00.13 / 07.10.2009 / M. Hultsch

  Function			:	collects the necessary data for the next [0xB0][0x27] iso protocol

  Parameters		:	unsigned char* ucData	-	pointer to buffer for data

  Return value		:	iCnt (>0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::Get_0xB0_0x27_IsoTableData(unsigned char* ucData)
{
	FEDM_CHK3(ucData);

	int iCnt = 0;
	int iIdx = 0;
	int iErr = 0;
	string sSnr, tmp;

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_CMD, &ucData[iCnt++]));
	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE, &ucData[iCnt++]));

	if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_NONADR)	// non addressed
	{
		iIdx = m_uiNonAddressedTabIndex;	// take data from entry with index >= 0
	}
	else if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
	{
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, tmp));
		sSnr = tmp.substr(0, 16);
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sSnr);
		if(iIdx < 0)
			return iIdx;

		// UID = unified identifier = serial number
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, &ucData[iCnt], 8));
		iCnt += 8;
	}
	else if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_SEL)	// selected
	{
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
		if(iIdx < 0)
		{
			// exception for ISO 14443 Transponder, when Reader is configuered with Anticollision = off
			if(  m_uiISOTableLength == 1 && (m_ISOTable[0]->m_ucTrType == FEDM_ISC_TR_TYPE_ISO14443A || m_ISOTable[0]->m_ucTrType == FEDM_ISC_TR_TYPE_ISO14443B) )
				iIdx = 0;
			else
				return iIdx;
		}
	}
	else
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNKNOWN_ADDRESS_MODE));
		return FEDM_ERROR_UNKNOWN_ADDRESS_MODE;
	}

	ucData[iCnt++] = m_ISOTable[iIdx]->m_ucAFI;

	return iCnt;
}

/***************************************************************************
  Begin        		:	19.03.2001 / M. Hultsch
  Version       	:	03.00.13 / 07.10.2009 / M. Hultsch

  Function			:	collects the necessary data for the next [0xB0][0x29] iso protocol

  Parameters		:	unsigned char* ucData	-	pointer to buffer for data

  Return value		:	iCnt (>0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::Get_0xB0_0x29_IsoTableData(unsigned char* ucData)
{
	FEDM_CHK3(ucData);

	int iCnt = 0;
	int iIdx = 0;
	int iErr = 0;
	string sSnr, tmp;

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_CMD, &ucData[iCnt++]));
	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE, &ucData[iCnt++]));

	if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_NONADR)	// non addressed
	{
		iIdx = m_uiNonAddressedTabIndex;	// take data from entry with index >= 0
	}
	else if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
	{
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, tmp));
		sSnr = tmp.substr(0, 16);
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sSnr);
		if(iIdx < 0)
			return iIdx;

		// UID = unified identifier = serial number
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, &ucData[iCnt], 8));
		iCnt += 8;
	}
	else if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_SEL)	// selected
	{
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
		if(iIdx < 0)
		{
			// exception for ISO 14443 Transponder, when Reader is configuered with Anticollision = off
			if(  m_uiISOTableLength == 1 && (m_ISOTable[0]->m_ucTrType == FEDM_ISC_TR_TYPE_ISO14443A || m_ISOTable[0]->m_ucTrType == FEDM_ISC_TR_TYPE_ISO14443B) )
				iIdx = 0;
			else
				return iIdx;
		}
	}
	else
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNKNOWN_ADDRESS_MODE));
		return FEDM_ERROR_UNKNOWN_ADDRESS_MODE;
	}

	ucData[iCnt++] = m_ISOTable[iIdx]->m_ucDsfID;

	return iCnt;
}

/***************************************************************************
  Begin        		:	17.07.2001 / M. Hultsch
  Version       	:	02.03.01 / 23.08.2005 / M. Hultsch

  Function			:	collects the necessary data for the next [0xB0][0xA1] iso protocol

  Parameters		:	unsigned char* ucData	-	pointer to buffer for data

  Return value		:	iCnt (>0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::Get_0xB0_0xA1_IsoTableData(unsigned char* ucData)
{
	FEDM_CHK3(ucData);

	int iCnt = 0;
	int iIdx = 0;
	int iErr = 0;
	unsigned char ucCBAdr	= 0x00;
	unsigned char ucMode	= 0x00;
	string sSnr, tmp;


	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE, &ucMode));
	// only addressed supported
	if((unsigned char)(ucMode&0x07) != FEDM_ISC_ISO_MODE_ADR)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	}

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_CMD, &ucData[iCnt++]));
	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE, &ucData[iCnt++]));
	if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
	{
		// UID = unified identifier = serial number

		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, &ucData[iCnt], 8));
		iCnt += 8;
	}
	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_CB_ADR, &ucData[iCnt++]));
	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_CB_ADR, &ucCBAdr));

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, tmp));
	sSnr = tmp.substr(0, 16);
	iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sSnr);
	if(iIdx < 0)
		return iIdx;
	
	FEDM_CHK1(iErr, m_ISOTable[iIdx]->GetData(0xA1, ucData, iCnt, 0, (unsigned int)ucCBAdr ));

	return iCnt;
}


/***************************************************************************
  Begin        		:	08.06.2000 / M. Hultsch
  Version       	:	03.00.00 / 03.09.2007 / M. Hultsch

  Function			:	sets the received data of last [0xB0] iso protocol
						into iso-table

  Parameters		:	unsigned char* ucData	-	pointer to buffer with data
						int iLen		-	length of data
						int iStatusByte	-	status byte of receive protocol

  Return value		:	OK (=0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::Set_0xB0_IsoTableData(unsigned char* ucData, int iLen, int iStatusByte)
{
	FEDM_CHK3(ucData);

	if(m_ISOTable.size() <= 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
		return FEDM_ERROR_NO_TABLE_SIZE;
	}

	bool bCardInfo = false;
	unsigned char	ucCmd	= 0x00;
	int iByteCnt = 0;
	int	iCnt = 0;
	int iErr = 0;
	int iIdx = 0;
	string sSnr, tmp;

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_CMD, &ucCmd));

	switch(ucCmd)
	{
	case 0x01:	// [0x01] Read Serial Number
		if(iLen <= 0)
		{
			FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B0_RSP_DATASETS, (unsigned char)0));
			if(	iStatusByte == 0x83 || 
				iStatusByte == 0x84)
			{
				return iStatusByte;
			}
			else
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_NO_DATA));
				return FEDM_ERROR_NO_DATA;
			}
		}

		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B0_RSP_DATASETS, ucData[iByteCnt++]));

		// add new items to table
		for(iCnt=0; iCnt<ucData[0]; iCnt++)
		{
			if(m_uiISOTableLength >= m_ISOTable.size())
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmd << ") " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				return FEDM_ERROR_ARRAY_BOUNDARY;
			}

			FEDM_CHK1(iErr, m_ISOTable[m_uiISOTableLength]->SetData(0x01, &ucData[0], iByteCnt));
			m_uiISOTableLength++;
		}

		// reset selected flags in the table
		for(iIdx=0; iIdx<(int)m_uiISOTableLength; iIdx++)
			m_ISOTable[iIdx]->m_bIsSelected = false;

		break;

	case 0x02:	// [0x02] Stay Quiet
		// search serial number in iso-table
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, tmp));
		sSnr = tmp.substr(0, 16);
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sSnr);
		if(iIdx >= 0)
			m_ISOTable[iIdx]->m_bIsSelected = false; // reset selection

		break;

	case 0x22:	// [0x22] Lock Multiple Blocks
		break;
	
	case 0x23:	// [0x23] Read Multiple Blocks
		FEDM_CHK1(iErr, Set_0xB0_0x23_IsoTableData(ucData, iLen));
		break;

	case 0x24:	// [0x24] Write Multiple Blocks
		break;

	case 0x25:	// [0x25] Select
		FEDM_CHK1(iErr, Set_0xB0_0x25_IsoTableData(ucData, iLen));
		break;

	case 0x26:	// [0x26] Reset to Ready
		FEDM_CHK1(iErr, Set_0xB0_0x26_IsoTableData(ucData, iLen));
		break;

	case 0x27:	// [0x27] Write AFI
	case 0x28:	// [0x28] Lock AFI
	case 0x29:	// [0x29] Write DSFID
	case 0x2A:	// [0x2A] Lock DSFID
		break;
	
	case 0x2B:	// [0x2B] Get System Information
		FEDM_CHK1(iErr, Set_0xB0_0x2B_IsoTableData(ucData, iLen, iStatusByte));
		break;
	
	case 0x2C:	// [0x2C] Get Multiple Block Security Status
		if(iLen == 0)
			return iStatusByte;

		FEDM_CHK1(iErr, Set_0xB0_0x2C_IsoTableData(ucData, iLen));
		break;
	
	case 0xA0:	// [0xA0] Read Config Block
		FEDM_CHK1(iErr, Set_0xB0_0xA0_IsoTableData(ucData, iLen));
		break;

	case 0xA1:	// [0xA1] Write Config Block
		break;
	
	case 0xC0:	// [0xC0] Halt
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmd << ") " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		return FEDM_ERROR_UNSUPPORTED;
	}

	return FEDM_OK;
}


/***************************************************************************
  Begin        		:	06.11.2006 / M. Hultsch
  Version       	:	03.03.01 / 31.03.2011 / M. Hultsch

  Function			:	sets the received data of last [0xB1] iso protocol
						into iso-table

  Parameters		:	unsigned char ucCmdByte	- command byte
						unsigned char* ucData	-	pointer to buffer with data
						int iLen		-	length of data

  Return value		:	OK (=0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Set_0xB1_IsoTableData(unsigned char ucCmdByte, unsigned char* ucData, int iLen)
{
	FEDM_CHK3(ucData);

	int iErr = 0;
	unsigned char ucMfr	= 0x00;

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MFR, &ucMfr));

	switch(ucMfr)
	{
	case FEDM_ISC_ISO_MFR_NXP:
		return Set_0xB1_IsoTableData_NXP(ucCmdByte, ucData, iLen);
		
	case FEDM_ISC_ISO_MFR_TI:
		return Set_0xB1_IsoTableData_TI(ucCmdByte, ucData, iLen);

	case FEDM_ISC_ISO_MFR_STM:
		return Set_0xB1_IsoTableData_STM(ucCmdByte, ucData, iLen);

	case FEDM_ISC_ISO_MFR_EM:
		return Set_0xB1_IsoTableData_EM(ucCmdByte, ucData, iLen);

	case FEDM_ISC_ISO_MFR_INFINEON:
		return Set_0xB1_IsoTableData_Infineon(ucCmdByte, ucData, iLen);

	case FEDM_ISC_ISO_MFR_FUJITSU:
		return Set_0xB1_IsoTableData_Fujitsu(ucCmdByte, ucData, iLen);

	case FEDM_ISC_ISO_MFR_KSW:
		return Set_0xB1_IsoTableData_KSW(ucCmdByte, ucData, iLen);

	case FEDM_ISC_ISO_MFR_IDS:
		return Set_0xB1_IsoTableData_IDS(ucCmdByte, ucData, iLen);
	}

	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ", Manufacturer=0x" << std::hex << std::uppercase << (int)ucMfr << ")" << GetErrorText(FEDM_ERROR_UNSUPPORTED));
	return FEDM_ERROR_UNSUPPORTED;
}

/***************************************************************************
  Begin        		:	07.10.2009 / M. Hultsch
  Version       	:	03.00.14 / 15.10.2009 / M. Hultsch

  Function			:	sets the received data of last [0xB1] iso protocol
						into iso-table
						MFR : NXP

  Parameters		:	unsigned char ucCmdByte	- command byte
						unsigned char* ucData	-	pointer to buffer with data
						int iLen		-	length of data

  Return value		:	OK (=0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Set_0xB1_IsoTableData_NXP(unsigned char ucCmdByte, unsigned char* ucData, int iLen)
{
	FEDM_CHK3(ucData);

	if(m_ISOTable.size() <= 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
		return FEDM_ERROR_NO_TABLE_SIZE;
	}

	unsigned char	ucMode	= 0x00;
	unsigned char	ucDBAdr = 0;
	unsigned char	ucUid[FEDM_ISC_MAX_UID_LENGTH];
	int		iErr = 0;
	int		iByteCnt = 0;
	int		iIdx = 0;
	int		iBlockCnt = 0;
	int		iCnt = 0;
	string	sUid;


	switch(ucCmdByte)
	{
	case 0xA0:
		break;

	case 0xA1:
		break;

	case 0xA2:
		break;

	case 0xA3:
		break;

	case 0xA4:
		break;

	case 0xA5:
		break;

	case 0xB0:
		break;

	case 0xB1:
		break;

	case 0xA6:
		break;

	case 0xA7:
		break;

	case 0xA8:	// [0xA8] Read EPC
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B1_RSP_NXP_EPC_DATA, ucData, 12));
		break;

	case 0xB2:	// [0xB2] Get Random Number
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B1_RSP_NXP_RANDOM_NUMBER, ucData, 2));
		break;

	case 0xB3:
		break;

	case 0xB4:
		break;

	case 0xB5:
		break;

	case 0xB6:
		break;

	case 0xB7:
		break;

	case 0xB8:	// [0xB8] Get Multiple Block Protection Status
		// save number of received data blocks
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B1_RSP_DBN, ucData[iByteCnt++]));
		if(ucData[0] == 0)
			return FEDM_OK;

		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucMode));

		if((unsigned char)(ucMode&0x07) == FEDM_ISC_ISO_MODE_NONADR)	// non addressed
		{
			iIdx = m_uiNonAddressedTabIndex;	// use entry with index >= 0
		}		
		else if((unsigned char)(ucMode&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// search serial number in iso-table
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, sUid));
			if(sUid.size() > 16)
				sUid = sUid.substr(0, 16);
			
			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
			if(iIdx < 0)
			{
				// new entry in table
				iIdx = m_uiISOTableLength;
				m_uiISOTableLength++;

				// write new serial number (UID) into table
				FEDM_CHK1(iErr, FEDM_ConvHexStrToHexUChar(sUid, &ucUid[0], 8));
				for(iCnt=0; iCnt<8; iCnt++)
					m_ISOTable[iIdx]->m_ucSnr[iCnt] = ucUid[iCnt];
			}
		}
		else if((unsigned char)(ucMode&0x07) == FEDM_ISC_ISO_MODE_SEL)	// selected
		{
			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
			if(iIdx < 0)
				return iIdx;
		}
		else
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_UNKNOWN_ADDRESS_MODE));
			return FEDM_ERROR_UNKNOWN_ADDRESS_MODE;
		}

		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_DB_ADR, &ucDBAdr));
		// write the received block protection status into table
		for(iBlockCnt=0; iBlockCnt<ucData[0]; iBlockCnt++)
		{
			m_ISOTable[iIdx]->m_ucSecStatus[ucDBAdr + iBlockCnt];
		}
		
		break;

	case 0xB9:
		break;

	case 0xBA:
		break;

	case 0xBB:
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		return FEDM_ERROR_UNSUPPORTED;
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        		:	07.10.2009 / M. Hultsch
  Version       	:	04.03.01 / 26.10.2012 / M. Hultsch

  Function			:	sets the received data of last [0xB1] iso protocol
						into iso-table
						MFR : STM

  Parameters		:	unsigned char ucCmdByte	- command byte
						unsigned char* ucData	-	pointer to buffer with data
						int iLen		-	length of data

  Return value		:	OK (=0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Set_0xB1_IsoTableData_STM(unsigned char ucCmdByte, unsigned char* ucData, int iLen)
{
	FEDM_CHK3(ucData);

	int	iErr = 0;

	if(m_ISOTable.size() <= 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
		return FEDM_ERROR_NO_TABLE_SIZE;
	}

	switch(ucCmdByte)
	{
	case 0xA1:
	case 0xA2:
	case 0xA4:
	case 0xA6:
	case 0xB1:
	case 0xB2:
	case 0xB3:
		break;

	case 0xA0: // ReadCfg
	case 0xA3: // CheckEHEn
		if(iLen < 1)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_NO_DATA));
			return FEDM_ERROR_NO_DATA;
		}

		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B1_RSP_STM_DATA, ucData[0]));
		break;

	default:
		return FEDM_ERROR_UNSUPPORTED;
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        		:	07.10.2009 / M. Hultsch
  Version       	:	03.00.14 / 15.10.2009 / M. Hultsch

  Function			:	sets the received data of last [0xB1] iso protocol
						into iso-table
						MFR : INFINEON

  Parameters		:	unsigned char* ucData	-	pointer to buffer with data
						int iLen		-	length of data

  Return value		:	OK (=0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Set_0xB1_IsoTableData_Infineon(unsigned char ucCmdByte, unsigned char* ucData, int iLen)
{
	FEDM_CHK3(ucData);

	if(m_ISOTable.size() <= 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
		return FEDM_ERROR_NO_TABLE_SIZE;
	}

	unsigned char ucCnt=0;
	unsigned char ucMode = 0;
	unsigned char ucDBAdr = 0;
	unsigned char ucDBN = 0;
	unsigned char ucDBSize = 0;
	unsigned char ucSnr[FEDM_ISC_MAX_UID_LENGTH];
	int	iErr = 0;
	int iIdx = 0;
	int iCnt = 0;
	string	sSnr;

	switch(ucCmdByte)
	{
	case 0x10:
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_MODE, &ucMode));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_INFINEON_DB_ADR, &ucDBAdr));
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B1_RSP_INFINEON_DB_N, ucData[iCnt]));
		ucDBN = ucData[iCnt++];
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B1_RSP_INFINEON_DB_SIZE, ucData[iCnt]));
		ucDBSize = ucData[iCnt++];

		if((unsigned char)(ucMode&0x07) == FEDM_ISC_ISO_MODE_NONADR)	// non addressed
		{
			iIdx = m_uiNonAddressedTabIndex;	// use entry with index >= 0
		}		
		else if((unsigned char)(ucMode&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// search serial number in iso-table
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B1_REQ_UID, sSnr));
			if(sSnr.size() > 16)
				sSnr = sSnr.substr(0, 16);
			
			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sSnr);
			if(iIdx < 0)
			{
				// new entry in table
				iIdx = m_uiISOTableLength;
				m_uiISOTableLength++;

				// write new serial number (UID) into table
				FEDM_CHK1(iErr, FEDM_ConvHexStrToHexUChar(sSnr, &ucSnr[0], 8));
				for(iCnt=0; iCnt<8; iCnt++)
					m_ISOTable[iIdx]->m_ucSnr[iCnt] = ucSnr[iCnt];
			}
		}
		else if((unsigned char)(ucMode&0x07) == FEDM_ISC_ISO_MODE_SEL)	// selected
		{
			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
			if(iIdx < 0)
				return iIdx;
		}
		else
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_UNKNOWN_ADDRESS_MODE));
			return FEDM_ERROR_UNKNOWN_ADDRESS_MODE;
		}

		m_ISOTable[iIdx]->m_ucBlockSize = ucDBSize;
		for(ucCnt=0; ucCnt<ucDBN; ucCnt++)
		{
			m_ISOTable[iIdx]->m_ucSecStatus[ucDBAdr + ucCnt] = ucData[iCnt++];
			memcpy(&m_ISOTable[iIdx]->m_ucRxDB[(ucDBAdr + ucCnt) * ucDBSize], &ucData[iCnt], ucDBSize);
			iCnt += (unsigned int)ucDBSize;
		}
		break;

	case 0x30:
	case 0x90:
	case 0xAB:
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		return FEDM_ERROR_UNSUPPORTED;
	}
	
	return FEDM_OK;
}

/***************************************************************************
  Begin        		:	07.10.2009 / M. Hultsch
  Version       	:	03.00.14 / 15.10.2009 / M. Hultsch

  Function			:	sets the received data of last [0xB1] iso protocol
						into iso-table
						MFR : TI

  Parameters		:	unsigned char ucCmdByte	- command byte
						unsigned char* ucData	-	pointer to buffer with data
						int iLen		-	length of data

  Return value		:	OK (=0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Set_0xB1_IsoTableData_TI(unsigned char ucCmdByte, unsigned char* ucData, int iLen)
{
	FEDM_CHK3(ucData);

	if(m_ISOTable.size() <= 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
		return FEDM_ERROR_NO_TABLE_SIZE;
	}

	int	iErr = 0;
	
	switch(ucCmdByte)
	{
	case 0xA2:
	case 0xA3:
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B1_RSP_TI_DB_ADR_E, ucData, 1));
		break;
	
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		return FEDM_ERROR_UNSUPPORTED;
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        		:	07.10.2009 / M. Hultsch
  Version       	:	03.00.14 / 15.10.2009 / M. Hultsch

  Function			:	sets the received data of last [0xB1] iso protocol
						into iso-table
						MFR : Fujitsu

  Parameters		:	unsigned char ucCmdByte	- command byte
						unsigned char* ucData	-	pointer to buffer with data
						int iLen		-	length of data

  Return value		:	OK (=0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Set_0xB1_IsoTableData_Fujitsu(unsigned char ucCmdByte, unsigned char* ucData, int iLen)
{
	FEDM_CHK3(ucData);

	if(m_ISOTable.size() <= 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
		return FEDM_ERROR_NO_TABLE_SIZE;
	}

/*	switch(ucCmdByte)
	{
	default:
		return FEDM_ERROR_UNSUPPORTED;
	}*/
	
	return FEDM_OK;
}

/***************************************************************************
  Begin        		:	07.10.2009 / M. Hultsch
  Version       	:	03.00.14 / 15.10.2009 / M. Hultsch

  Function			:	sets the received data of last [0xB1] iso protocol
						into iso-table
						MFR : KSW

  Parameters		:	unsigned char ucCmdByte	- command byte
						unsigned char* ucData	-	pointer to buffer with data
						int iLen		-	length of data

  Return value		:	OK (=0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Set_0xB1_IsoTableData_KSW(unsigned char ucCmdByte, unsigned char* ucData, int iLen)
{
	FEDM_CHK3(ucData);

	if(m_ISOTable.size() <= 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
		return FEDM_ERROR_NO_TABLE_SIZE;
	}

	int		iErr = 0;
	
	switch(ucCmdByte)
	{
	case 0xA2:
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B1_RSP_KSW_LOGFLAGS, ucData, 1));
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B1_RSP_KSW_LOGPOINTER, ucData, 1));
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B1_RSP_KSW_LOGLIMITLO, ucData, 2));
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B1_RSP_KSW_LOGLIMITHI, ucData, 2));
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B1_RSP_KSW_LOGPERIODE, ucData, 2));
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B1_RSP_KSW_TIMERTICKS, ucData, 2));					
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		return FEDM_ERROR_UNSUPPORTED;
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        		:	31.03.2011 / M. Hultsch
  Version       	:	03.03.01 / 31.03.2011 / M. Hultsch

  Function			:	sets the received data of last [0xB1] iso protocol
						into iso-table
						MFR : IDS Microchip AG

  Parameters		:	unsigned char ucCmdByte	- command byte
						unsigned char* ucData	-	pointer to buffer with data
						int iLen				-	length of data

  Return value		:	OK (=0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Set_0xB1_IsoTableData_IDS(unsigned char ucCmdByte, unsigned char* ucData, int iLen)
{
	FEDM_CHK3(ucData);

	if(m_ISOTable.size() <= 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
		return FEDM_ERROR_NO_TABLE_SIZE;
	}

	int	iErr = 0;
	int iCnt = 0;
	
	switch(ucCmdByte)
	{
	case 0xA0:
	case 0xA1:
	case 0xA2:
		break;

	case 0xA3:
		if(iLen < 16)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
			return FEDM_ERROR_BUFFER_LENGTH;
		}

		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB1_RSP_IDS_START_TIME, &ucData[iCnt], 4));
		iCnt += 4;
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB1_RSP_IDS_LOG_LIMITS, &ucData[iCnt], 4));
		iCnt += 4;
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB1_RSP_IDS_LOG_MODE, &ucData[iCnt], 4));
		iCnt += 4;
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB1_RSP_IDS_DELAY_TIME, &ucData[iCnt], 4));
		break;

	case 0xA4:
	case 0xA5:
	case 0xA6:
	case 0xA7:
		break;

	case 0xA8:
		if(iLen < 8)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
			return FEDM_ERROR_BUFFER_LENGTH;
		}

		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB1_RSP_IDS_MEAS_STATE, &ucData[iCnt], 4));
		iCnt += 4;
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB1_RSP_IDS_LIMITS_COUNTER, &ucData[iCnt], 4));
		break;

	case 0xA9:
		if(iLen < 8)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
			return FEDM_ERROR_BUFFER_LENGTH;
		}

		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB1_RSP_IDS_INT_CAL_DATA, &ucData[iCnt], 4));
		iCnt += 4;
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB1_RSP_IDS_EXT_CAL_DATA, &ucData[iCnt], 4));
		break;

	case 0xAA:
		if(iLen < 1)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
			return FEDM_ERROR_BUFFER_LENGTH;
		}

		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB1_RSP_IDS_BATTERY_LEVEL, ucData, 1));
		break;

	case 0xAB:
	case 0xAC:
		break;

	case 0xAD:
		if(iLen < 2)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
			return FEDM_ERROR_BUFFER_LENGTH;
		}

		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB1_RSP_IDS_MEAS_TEMP, ucData, 2));
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		return FEDM_ERROR_UNSUPPORTED;
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        		:	07.10.2009 / M. Hultsch
  Version       	:	03.00.14 / 15.10.2009 / M. Hultsch

  Function			:	sets the received data of last [0xB1] iso protocol
						into iso-table
						MFR : EM

  Parameters		:	unsigned char ucCmdByte	- command byte
						unsigned char* ucData	-	pointer to buffer with data
						int iLen		-	length of data

  Return value		:	OK (=0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Set_0xB1_IsoTableData_EM(unsigned char ucCmdByte, unsigned char* ucData, int iLen)
{
	FEDM_CHK3(ucData);

	if(m_ISOTable.size() <= 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
		return FEDM_ERROR_NO_TABLE_SIZE;
	}

	int iErr=0;

	switch(ucCmdByte)
	{
	case 0xA0:
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B1_RSP_EM_EAS_BIT, ucData, 1));
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		return FEDM_ERROR_UNSUPPORTED;
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        		:	08.06.2000 / M. Hultsch
  Version       	:	02.03.00 / 28.07.2005 / M. Hultsch

  Function			:	sets the received data of last [0xB2] iso protocol
						into iso-table

  Parameters		:	unsigned char* ucData	-	pointer to buffer with data
						int iLen		-	length of data
						int iStatusByte	-	status byte of receive protocol

  Return value		:	OK (=0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Set_0xB2_IsoTableData(unsigned char* ucData, int iLen, int iStatusByte)
{
	FEDM_CHK3(ucData);

	if(m_ISOTable.size() <= 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_NO_TABLE_SIZE));
		return FEDM_ERROR_NO_TABLE_SIZE;
	}

	unsigned char	ucCmdByte = 0x00;
	int iErr = 0;

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B2_CMD, &ucCmdByte));

	switch(ucCmdByte)
	{
	case 0x30: // MIFARE Value Commands
		return FEDM_OK;

	case 0x2B:	// ISO14443-4 Transponder Info
		FEDM_CHK1(iErr, Set_0xB2_0x2B_IsoTableData(ucData, iLen));
		break;

	case 0xB0: // Authent MIFARE
		return FEDM_OK;

	case 0xB2: // Authent MIFARE Ultralight C
		return FEDM_OK;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        		:	22.04.2010 / M. Hultsch
  Version       	:	03.03.01 / 08.04.2011 / M. Hultsch

  Function			:	sets the received data of last [0xB4] iso protocol
						into tmpdata field (and not into iso-table!!)

  Parameters		:	unsigned char ucCmdByte	- command byte
						unsigned char* ucData	-	pointer to buffer with data
						int iLen		-	length of data

  Return value		:	OK (=0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Set_0xB4_IsoTableData(unsigned char ucCmdByte, unsigned char* ucData, int iLen)
{
	FEDM_CHK3(ucData);

	int iErr = 0;
	unsigned char ucMfr	= 0x00;

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_MFR, &ucMfr));

	switch(ucMfr)
	{
	case FEDM_ISC_ISO_MFR_NXP:
		return Set_0xB4_IsoTableData_NXP(ucCmdByte, ucData, iLen);

	case FEDM_ISC_ISO_MFR_EM:
		return Set_0xB4_IsoTableData_EM(ucCmdByte, ucData, iLen);

	case FEDM_ISC_ISO_MFR_IDS:
		return Set_0xB4_IsoTableData_IDS(ucCmdByte, ucData, iLen);

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ", Manufacturer=0x" << std::hex << std::uppercase << (int)ucMfr << ") " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		return FEDM_ERROR_UNSUPPORTED;
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        		:	22.04.2010 / M. Hultsch
  Version       	:	04.06.10 / 02.12.2014 / M. Hultsch
						- new [0xB4] NXP Custom Commands: 0x10, 0x11, 0x12

						03.01.05 / 22.04.2010 / M. Hultsch

  Function			:	sets the received data of last [0xB4] iso protocol
						into tmpdata field (and not into iso-table!!)
						MFR : NXP

  Parameters		:	unsigned char ucCmdByte	- command byte
						unsigned char* ucData	-	pointer to buffer with data
						int iLen		-	length of data

  Return value		:	OK (=0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Set_0xB4_IsoTableData_NXP(unsigned char ucCmdByte, unsigned char* ucData, int iLen)
{
	FEDM_CHK3(ucData);

	int	iErr = 0;
	int iByteCnt = 0;
	unsigned char ucProfile = 0;
	unsigned int uiOffset = 0;
	unsigned char ucRspDataLen = 0;
	bool bUidLF = false;
	unsigned char	ucMode	= 0x00;
	unsigned char	ucUidLen = 0x00;
	unsigned char	ucUid[FEDM_ISC_MAX_UID_LENGTH];
	int		iCnt, iIdx;
	string	sUid, tmp;
	FEDM_BYTE_ARRAY_ITOR itor;

	switch(ucCmdByte)
	{
	case 0x00:
	case 0x01:
	case 0x03:
	case 0x04:
	case 0x05:
		break;

	case 0x06:
		if(iLen < 2)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_NO_DATA));
			return FEDM_ERROR_NO_DATA;
		}

		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B4_RSP_NXP_CONFIG_WORD, ucData, 2));
		break;

	case 0x07:
	case 0x08:
		break;

/*	case 0x10:	// [0x10] Untraceable
		break;

	case 0x11:	// [0x11] Authenticate
		if(iLen <= 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_NO_DATA));
			return FEDM_ERROR_NO_DATA;
		}

		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_MODE, &ucMode));

		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_NXP_PROFILE, &ucProfile));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_NXP_OFFSET, &uiOffset));

		if((unsigned char)(ucMode&0x07) == FEDM_ISC_ISO_MODE_NONADR)	// non addressed
		{
			iIdx = m_uiNonAddressedTabIndex;	// use entry with index >= 0
		}		
		else if((unsigned char)(ucMode&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			// search serial number in iso-table
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_EPC, tmp));
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_MODE_EPC_LF, &bUidLF));
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_EPC_LEN, &ucUidLen));
			if(bUidLF)
			{
				sUid = tmp.substr(0, (ucUidLen<<1));
			}
			else
			{
				sUid = tmp.substr(0, 16);
			}
		
			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
			if(iIdx < 0)
			{
				// new entry in table
				iIdx = m_uiISOTableLength;
				m_uiISOTableLength++;

				// write new serial number (UID) into table
				FEDM_CHK1(iErr, FEDM_ConvHexStrToHexUChar(sUid, &ucUid[0], ucUidLen));
				for(iCnt=0; iCnt<ucUidLen; iCnt++)
					m_ISOTable[iIdx]->m_ucSnr[iCnt] = ucUid[iCnt];
			}
		}
		else if((unsigned char)(ucMode&0x07) == FEDM_ISC_ISO_MODE_SEL)	// selected
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << "; selected mode) " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			return FEDM_ERROR_UNSUPPORTED;
		}
		else
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNKNOWN_ADDRESS_MODE));
			return FEDM_ERROR_UNKNOWN_ADDRESS_MODE;
		}

		// first responded byte contains the number of following data bytes
		ucRspDataLen = ucData[iByteCnt++];
		
		if(iLen < (ucRspDataLen+1))
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_NO_DATA));
			return FEDM_ERROR_NO_DATA;
		}

		switch(ucProfile)
		{
		case 0: // EPC memory bank
			if( ((uiOffset+1)*8 + ucRspDataLen) > m_ISOTable[iIdx]->m_ucRxDB_EpcBank.size() )
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << "; Offset too large for EPC memory bank) " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				return FEDM_ERROR_ARRAY_BOUNDARY;
			}

			itor = m_ISOTable[iIdx]->m_ucRxDB_EpcBank.begin() + (uiOffset+1)*8;
			break;
		case 1: // TID memory bank
			if( ((uiOffset+1)*8 + ucRspDataLen) > m_ISOTable[iIdx]->m_ucRxDB_TidBank.size() )
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << "; Offset too large for EPC memory bank) " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				return FEDM_ERROR_ARRAY_BOUNDARY;
			}

			itor = m_ISOTable[iIdx]->m_ucRxDB_TidBank.begin() + (uiOffset+1)*8;
			break;
		case 2: // USER memory bank
			if( ((uiOffset+1)*8 + ucRspDataLen) > m_ISOTable[iIdx]->m_ucRxDB.size() )
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << "; Offset too large for EPC memory bank) " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				return FEDM_ERROR_ARRAY_BOUNDARY;
			}

			itor = m_ISOTable[iIdx]->m_ucRxDB.begin() + (uiOffset+1)*8;
			break;
		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << "; Profile=0x" << std::hex << std::uppercase << (int)ucProfile << ") " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			return FEDM_ERROR_UNSUPPORTED;
			break;
		}

		// write the received public data into table
		for(iCnt=0; iCnt<ucRspDataLen; iCnt++)
		{
			*itor = ucData[iByteCnt++];
			itor++;
		}
		break;*/

	case 0x12:	// [0x12] Write Key
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		return FEDM_ERROR_UNSUPPORTED;
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        		:	09.12.2014 / M. Hultsch
  Version       	:	04.06.10 / 09.12.2014 / M. Hultsch

  Function			:	sets the received data of last [0xB4] iso protocol
						into tmpdata field (and not into iso-table!!)
						MFR : EM Microelectronic-Marin SA

  Parameters		:	unsigned char ucCmdByte	- command byte
						unsigned char* ucData	-	pointer to buffer with data
						int iLen		-	length of data

  Return value		:	OK (=0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Set_0xB4_IsoTableData_EM(unsigned char ucCmdByte, unsigned char* ucData, int iLen)
{
	FEDM_CHK3(ucData);

	int	iErr = 0;
	int iAdr = 0;
	unsigned int uiByteCnt = 0;
	unsigned int uiRspDataLen = 0;
	unsigned int uiCnt = 0;
	unsigned char ucRespSize = 0;
	unsigned char ucBuffer[20];

	switch(ucCmdByte)
	{
	case 0x00:
		if(iLen < 8)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_NO_DATA));
			return FEDM_ERROR_NO_DATA;
		}

		iAdr = FEDM_GetAdrOfID(FEDM_ISC_TMP_B4_RSP_EM_UID, 32);
		if(iAdr < 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(iAdr));
			return iAdr;
		}
		
		memset(ucBuffer, 0, 20);

		// copy responded UID with length information coded in first byte
		switch(ucData[0])
		{
		case 0xE0: // ISO/IEC 7816-6 (64 bit)
			uiRspDataLen = 8;
			break;
		case 0xE2: // EPC global Class1 Gen2 (96 bit)
			uiRspDataLen = 12;
			break;
		case 0xE3: // ISO/IEC 7816-6 (80 bit)
			uiRspDataLen = 10;
			break;
		case 0x44: // legacy TOTAL (64 bit)
		case 0x45:
		case 0x46:
		case 0x47:
			uiRspDataLen = 8;
			break;
		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << "; Class-ID=0x" << std::hex << std::uppercase << (int)ucData[0] << ") " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			return FEDM_ERROR_UNSUPPORTED;
		}

		memcpy(ucBuffer, ucData, uiRspDataLen);
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B4_RSP_EM_UID, ucBuffer, 12));
		break;

	case 0x01:
		if(iLen < 16)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_NO_DATA));
			return FEDM_ERROR_NO_DATA;
		}

		iAdr = FEDM_GetAdrOfID(FEDM_ISC_TMP_B4_RSP_EM_UID, 32);
		if(iAdr < 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(iAdr));
			return iAdr;
		}
		
		memset(ucBuffer, 0, 20);

		// copy responded UID with length information coded in first byte
		switch(ucData[0])
		{
		case 0xE0: // ISO/IEC 7816-6 (64 bit)
			uiRspDataLen = 8;
			break;
		case 0xE2: // EPC global Class1 Gen2 (96 bit)
			uiRspDataLen = 12;
			break;
		case 0xE3: // ISO/IEC 7816-6 (80 bit)
			uiRspDataLen = 10;
			break;
		case 0x44: // legacy TOTAL (64 bit)
		case 0x45:
		case 0x46:
		case 0x47:
			uiRspDataLen = 8;
			break;
		default:
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << "; Class-ID=0x" << std::hex << std::uppercase << (int)ucData[0] << ") " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
			return FEDM_ERROR_UNSUPPORTED;
		}

		memcpy(ucBuffer, ucData, uiRspDataLen);
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B4_RSP_EM_UID, ucBuffer, 12));
		uiByteCnt += uiRspDataLen;
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B4_RSP_EM_SENSOR_DATA, &ucData[uiByteCnt], 4));
		uiByteCnt += 4;
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B4_RSP_EM_UTC_TIMESTAMP, &ucData[uiByteCnt], 4));
		uiByteCnt += 4;
		break;

	case 0x02:
		if(iLen > 0)
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_EM_SPI_RESP_SIZE, &ucRespSize));
			if(ucRespSize != (unsigned int)iLen)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
				return FEDM_ERROR_BUFFER_LENGTH;
			}

			memset(ucBuffer, 0, 20);
			memcpy(ucBuffer, ucData, iLen);
			FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B4_RSP_EM_SPI_DATA, ucBuffer, 7));
		}
		break;

	case 0x04:
		break;

	case 0xD1:
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		return FEDM_ERROR_UNSUPPORTED;
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        		:	03.03.01 / 08.04.2011 / M. Hultsch
  Version       	:	03.03.01 / 08.04.2011 / M. Hultsch

  Function			:	sets the received data of last [0xB4] iso protocol
						into tmpdata field (and not into iso-table!!)
						MFR : IDS

  Parameters		:	unsigned char ucCmdByte	- command byte
						unsigned char* ucData	-	pointer to buffer with data
						int iLen		-	length of data

  Return value		:	OK (=0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Set_0xB4_IsoTableData_IDS(unsigned char ucCmdByte, unsigned char* ucData, int iLen)
{
	FEDM_CHK3(ucData);

	int	iErr = 0;
	int iCnt = 0;

	switch(ucCmdByte)
	{
	case 0xA0:
	case 0xA1:
	case 0xA2:
		break;

	case 0xA3:
		if(iLen < 16)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_NO_DATA));
			return FEDM_ERROR_NO_DATA;
		}

		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB4_RSP_IDS_START_TIME, &ucData[iCnt], 4));
		iCnt += 4;
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB4_RSP_IDS_LOG_LIMITS, &ucData[iCnt], 5));
		iCnt += 5;
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB4_RSP_IDS_LOG_MODE, &ucData[iCnt], 1));
		iCnt += 1;
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB4_RSP_IDS_LOG_INTERVAL, &ucData[iCnt], 2));
		iCnt += 2;
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB4_RSP_IDS_DELAY_TIME, &ucData[iCnt], 2));
		iCnt += 2;
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB4_RSP_IDS_APP_DATA, &ucData[iCnt], 2));
		break;

	case 0xA4:
	case 0xA5:
	case 0xA6:
	case 0xA7:
		break;

	case 0xA8:
		if(iLen == 9)
		{
			FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB4_RSP_IDS_LIMITS_COUNTER, &ucData[iCnt], 4));
			iCnt += 4;
			FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB4_RSP_IDS_SYSTEM_STATUS, &ucData[iCnt], 4));
			iCnt += 4;
			FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB4_RSP_IDS_SL_BLOCK_0, (std::string)"00000000"));
			FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB4_RSP_IDS_SL_BLOCK_1, (std::string)"00000000"));
			FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB4_RSP_IDS_CURRENT_SHELF_LIFE, (std::string)"000000"));
			FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB4_RSP_IDS_STATUS_FLAG, &ucData[iCnt], 1));
		}
		else if(iLen == 20)
		{
			FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB4_RSP_IDS_LIMITS_COUNTER, &ucData[iCnt], 4));
			iCnt += 4;
			FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB4_RSP_IDS_SYSTEM_STATUS, &ucData[iCnt], 4));
			iCnt += 4;
			FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB4_RSP_IDS_SL_BLOCK_0, &ucData[iCnt], 4));
			iCnt += 4;
			FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB4_RSP_IDS_SL_BLOCK_1, &ucData[iCnt], 4));
			iCnt += 4;
			FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB4_RSP_IDS_CURRENT_SHELF_LIFE, &ucData[iCnt], 3));
			iCnt += 3;
			FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB4_RSP_IDS_STATUS_FLAG, &ucData[iCnt], 1));
		}
		else
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_NO_DATA));
			return FEDM_ERROR_NO_DATA;
		}
		break;

	case 0xA9:
		if(iLen < 9)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_NO_DATA));
			return FEDM_ERROR_NO_DATA;
		}

		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB4_RSP_IDS_CAL_DATA, &ucData[iCnt], 7));
		iCnt += 7;
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB4_RSP_IDS_SFE_PARA, &ucData[iCnt], 2));
		break;

	case 0xAA:
		if(iLen < 2)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_NO_DATA));
			return FEDM_ERROR_NO_DATA;
		}

		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB4_RSP_IDS_BATTERY_LEVEL, &ucData[iCnt], 2));
		break;

	case 0xAB:
	case 0xAC:
		break;

	case 0xAD:
		if(iLen < 2)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_NO_DATA));
			return FEDM_ERROR_NO_DATA;
		}

		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB4_RSP_IDS_SENSOR_INFO, &ucData[iCnt], 2));
		break;

	case 0xAE:
		break;

	case 0xAF:
		if(iLen > 0)
		{
			FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_0xB4_RSP_IDS_PAYLOAD, &ucData[iCnt], iLen));
		}
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		return FEDM_ERROR_UNSUPPORTED;
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        		:	09.02.2009 / M. Hultsch
  Version       	:	03.00.06 / 02.04.2009 / M. Hultsch

  Function			:	sets the received data of last [0xC1] iso protocol
						into iso-table

  Parameters		:	unsigned char  ucDESFireCmd	-	sub command
						unsigned char* ucData		-	pointer to buffer with data
						int iLen			-	length of data

  Return value		:	OK (=0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::Set_0xC1_IsoTableData(unsigned char ucDESFireCmd, unsigned char* ucData, int iLen)
{
	FEDM_CHK3(ucData);

	int iErr = 0;
	int iCnt = 0;

	unsigned int uiFileOffset = 0;
	unsigned int uiFileLen = 0;
	unsigned int uiNoOfRecords = 0;
	unsigned int uiRecordSize = 0;

	int iIdx = 0;

	switch(ucDESFireCmd)
	{
	case 0xFA:	// DESFire Authent
		break;

	case 0xBD:	// DESFire Read Standard Data
		// command only in selected mode
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
		if(iIdx < 0)
			return iIdx;

		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_OFFSET, &uiFileOffset));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_DATA_LEN, &uiFileLen));

		if(uiFileOffset + uiFileLen > FEDM_ISC_ISO_TABLE_RxDB_SIZE)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(DESFireCmd=0x" << std::hex << std::uppercase << (int)ucDESFireCmd << ") " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
			return FEDM_ERROR_ARRAY_BOUNDARY;
		}

		memcpy(&m_ISOTable[iIdx]->m_ucRxDB[uiFileOffset], &ucData[0], uiFileLen);
		break;

	case 0x3D:	// DESFire Write Standard Data
		break;

	case 0x6C:	// DESFire Get Value
		// command only in selected mode
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
		if(iIdx < 0)
			return iIdx;

		m_ISOTable[iIdx]->m_uiValue  = ucData[iCnt++];
		m_ISOTable[iIdx]->m_uiValue += (((unsigned int)ucData[iCnt++]) <<  8);
		m_ISOTable[iIdx]->m_uiValue += (((unsigned int)ucData[iCnt++]) << 16);
		m_ISOTable[iIdx]->m_uiValue += (((unsigned int)ucData[iCnt++]) << 24);

		break;

	case 0x0C:	// DESFire Credit
		break;

	case 0xDC:	// DESFire Debit
		break;

	case 0x1C:	// DESFire Limited Credit
		break;

	case 0x3B:	// DESFire Write Record
		break;

	case 0xBB:	// DESFire Read Records
		// command only in selected mode
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
		if(iIdx < 0)
			return iIdx;

		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_NO_OF_RECORDS, &uiNoOfRecords));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_RECORD_SIZE, &uiRecordSize));

		if(uiNoOfRecords * uiRecordSize > FEDM_ISC_ISO_TABLE_RxDB_SIZE)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(DESFireCmd=0x" << std::hex << std::uppercase << (int)ucDESFireCmd << ") " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
			return FEDM_ERROR_ARRAY_BOUNDARY;
		}

		memcpy(&m_ISOTable[iIdx]->m_ucRxDB[0], &ucData[0], uiNoOfRecords*uiRecordSize);
		break;

	case 0xEB:	// DESFire Clear Record File
		break;

	case 0x5F:	// DESFire Change File Settings
		break;

	case 0x54:	// DESFire Change Key Settings
		break;

	case 0xC4:	// DESFire Change Key
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(DESFireCmd=0x" << std::hex << std::uppercase << (int)ucDESFireCmd << ") " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        		:	08.09.2010 / M. Hultsch
  Version       	:	03.02.05 / 08.09.2010 / M. Hultsch

  Function			:	sets the received data of last [0xC3] DESFire Command
						into iso-table

  Parameters		:	unsigned char  ucDESFireCmd	-	sub command
						unsigned char* ucData		-	pointer to buffer with data
						int iLen					-	length of data

  Return value		:	OK (=0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::Set_0xC3_IsoTableData(unsigned char ucDESFireCmd, unsigned char* ucData, int iLen)
{
	FEDM_CHK3(ucData);

	int iErr = 0;
	int iCnt = 0;
	int iAdr = 0;

	unsigned int uiFileOffset = 0;
	unsigned int uiFileLen = 0;
	unsigned int uiNoOfRecords = 0;
	unsigned int uiRecordSize = 0;
	unsigned char* pData = NULL;

	int iIdx = 0;

	switch(ucDESFireCmd)
	{
	// all command with no response data
	case 0xFA: // DESFire Authent
	case 0xFC: // DESFire Format PICC
	case 0xCA: // DESFire Create Application
	case 0xDA: // DESFire Delete Application
	case 0x0C: // DESFire Credit
	case 0xDC: // DESFire Debit
	case 0x1C: // DESFire Limited Credit
	case 0x3B: // DESFire Write Record
	case 0x3D: // DESFire Write Standard Data
	case 0xEB: // DESFire Clear Record File
	case 0x5F: // DESFire Change File Settings
	case 0x54: // DESFire Change Key Settings
	case 0xC4: // DESFire Change Key
	case 0x5A: // DESFire Select Application
	case 0x5C: // DESFire Set Configuration
	case 0xCD: // DESFire Create Standard Data File
	case 0xCB: // DESFire Create Backup Data File
	case 0xCC: // DESFire Create Value File
	case 0xC1: // DESFire Create Linear Record File
	case 0xC0: // DESFire Create Cyclic Record File
	case 0xDF: // DESFire Delete File
	case 0xC7: // DESFire Commit Transaction
	case 0xA7: // DESFire Abort Transaction
		break;

	// all commands with response data which are not stored in the table
	case 0x45: // DESFire Get Key Settings
	case 0x64: // DESFire Get Key Version
	case 0x6A: // DESFire Get Application IDs
	case 0x6D: // DESFire Get DF Names
	case 0x60: // DESFire Get Version
	case 0x51: // DESFire Get Card UID
	case 0x6F: // DESFire Get File IDs
	case 0xF5: // DESFire Get File Settings
	case 0x61: // DESFire Get ISO File IDs
	case 0x6E: // DESFire Free Mem
		iAdr = FEDM_GetAdrOfID(FEDM_ISC_TMP_0xC3_RSP_DATA, 32);
		if(iAdr < 0)
			return iAdr;
		pData = &m_TmpData[iAdr];
		memcpy(pData, ucData, iLen);
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0xC3_RSP_DATA_LEN, (unsigned int)iLen));
		break;

	case 0xBD:	// DESFire Read Standard Data
		// command only in selected mode
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
		if(iIdx < 0)
			return iIdx;

		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_OFFSET, &uiFileOffset));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_DATA_LEN, &uiFileLen));

		if(uiFileOffset + uiFileLen > FEDM_ISC_ISO_TABLE_RxDB_SIZE)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(DESFireCmd=0x" << std::hex << std::uppercase << (int)ucDESFireCmd << ") " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
			return FEDM_ERROR_ARRAY_BOUNDARY;
		}

		memcpy(&m_ISOTable[iIdx]->m_ucRxDB[uiFileOffset], &ucData[0], uiFileLen);
		break;

	case 0x6C:	// DESFire Get Value
		// command only in selected mode
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
		if(iIdx < 0)
			return iIdx;

		m_ISOTable[iIdx]->m_uiValue  = ucData[iCnt++];
		m_ISOTable[iIdx]->m_uiValue += (((unsigned int)ucData[iCnt++]) <<  8);
		m_ISOTable[iIdx]->m_uiValue += (((unsigned int)ucData[iCnt++]) << 16);
		m_ISOTable[iIdx]->m_uiValue += (((unsigned int)ucData[iCnt++]) << 24);

		break;

	case 0xBB:	// DESFire Read Records
		// command only in selected mode
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
		if(iIdx < 0)
			return iIdx;

		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_NO_OF_RECORDS, &uiNoOfRecords));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_RECORD_SIZE, &uiRecordSize));

		if(uiNoOfRecords * uiRecordSize > FEDM_ISC_ISO_TABLE_RxDB_SIZE)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(DESFireCmd=0x" << std::hex << std::uppercase << (int)ucDESFireCmd << ") " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
			return FEDM_ERROR_ARRAY_BOUNDARY;
		}

		memcpy(&m_ISOTable[iIdx]->m_ucRxDB[0], &ucData[0], uiNoOfRecords*uiRecordSize);
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(DESFireCmd=0x" << std::hex << std::uppercase << (int)ucDESFireCmd << ") " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	}

	return FEDM_OK;
}


/***************************************************************************
  Begin        		:	24.08.2009 / M. Hultsch
  Version       	:	03.00.13 / 01.10.2009 / M. Hultsch

  Function			:	sets the received data of last [0xC2] iso protocol
						into iso-table

  Parameters		:	unsigned char  ucMifarePlusCmd	-	sub command
						unsigned char* ucData			-	pointer to buffer with data
						int iLen				-	length of data

  Return value		:	OK (=0) or error code (<0)
***************************************************************************/
int FEDM_ISCReader::Set_0xC2_IsoTableData(unsigned char ucMifarePlusCmd, unsigned char* ucData, int iLen)
{
	FEDM_CHK3(ucData);

	int iErr = 0;
	int iCnt = 0;
	int iIdx = 0;
	unsigned int uiBlockNo = 0;
	unsigned int uiLength = 0;

	switch(ucMifarePlusCmd)
	{
	// commands for all security levels
	case 0x70:	// First Authent
		break;

	case 0x76:	// Following Authent
		break;

	// Security Level 1 commands
	case 0x77:	// SL1 AES Authent
		break;

	// Security Level 2 commands
	case 0x75:	// SL2 AES and Crypto Authent
		break;

	case 0x38:	// SL2 Multiblock Read
		// command only in selected mode
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
		if(iIdx < 0)
			return iIdx;

		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_SRC_BLOCK_NO, &uiBlockNo));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_EXT, &uiLength));

		if( (uiBlockNo + uiLength)*16 > FEDM_ISC_ISO_TABLE_RxDB_SIZE)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(MifarePlusCmd=0x" << std::hex << std::uppercase << (int)ucMifarePlusCmd << ") " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
			return FEDM_ERROR_ARRAY_BOUNDARY;
		}

		m_ISOTable[iIdx]->m_ucBlockSize = 16;
		m_ISOTable[iIdx]->m_bIsBlockSizeSet = true;
		memcpy(&m_ISOTable[iIdx]->m_ucRxDB[uiBlockNo*16], &ucData[0], uiLength*16);
		break;

	case 0xA8:	// SL2 Multiblock Write
		break;

	// Security Level 3 commands
	case 0x78:	// SL3 Reset Authent
		break;

	case 0x30:	// SL3 Read Encrypted
	case 0x31:	// SL3 Read Encrypted Maced
	case 0x32:	// SL3 Read Plain
	case 0x33:	// SL3 Read Plain Maced
	case 0x34:	// SL3 Read Encrypted Unmaced
	case 0x35:	// SL3 Read Encrypted Unmaced Respond Maced
	case 0x36:	// SL3 Read Plain Unmaced
	case 0x37:	// SL3 Read Plain Unmaced Respond Maced
		// command only in selected mode
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
		if(iIdx < 0)
			return iIdx;

		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_BLOCK_NO, &uiBlockNo));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_EXT, &uiLength));

		if( (uiBlockNo + uiLength)*16 > FEDM_ISC_ISO_TABLE_RxDB_SIZE)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(MifarePlusCmd=0x" << std::hex << std::uppercase << (int)ucMifarePlusCmd << ") " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
			return FEDM_ERROR_ARRAY_BOUNDARY;
		}

		m_ISOTable[iIdx]->m_ucBlockSize = 16;
		m_ISOTable[iIdx]->m_bIsBlockSizeSet = true;
		memcpy(&m_ISOTable[iIdx]->m_ucRxDB[uiBlockNo*16], &ucData[0], uiLength*16);
		break;

	case 0xA0:	// SL3 Write Encrypted
		break;

	case 0xA1:	// SL3 Write Encrypted Maced
		break;

	case 0xA2:	// SL3 Write Plain
		break;

	case 0xA3:	// SL3 Write Plain Maced
		break;

	case 0xB0:	// SL3 Increment Encrypted
		break;

	case 0xB1:	// SL3 Increment Encrypted Maced
		break;

	case 0xB2:	// SL3 Decrement Encrypted
		break;

	case 0xB3:	// SL3 Decrement Encrypted Maced
		break;

	case 0xB4:	// SL3 Transfer
		break;

	case 0xB5:	// SL3 Transfer Maced
		break;

	case 0xB6:	// SL3 Increment Transfer Encrypted
		break;

	case 0xB7:	// SL3 Increment Transfer Encrypted Maced
		break;

	case 0xB8:	// SL3 Decrement Transfer Encrypted
		break;

	case 0xB9:	// SL3 Decrement Transfer Encrypted Maced
		break;

	case 0xC2:	// SL3 Restore
		break;

	case 0xC3:	// SL3 Restore Maced
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(MifarePlusCmd=0x" << std::hex << std::uppercase << (int)ucMifarePlusCmd << ") " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	}

	return FEDM_OK;
}


/***************************************************************************
  Begin        		:	08.03.2001 / M. Hultsch
  Version       	:	03.00.13 / 07.10.2009 / M. Hultsch

  Function			:	sets the received data of last [0xB0][0x23] iso protocol
						into iso-table

  Parameters		:	unsigned char* ucData	-	pointer to buffer with data
						int iLen		-	length of data

  Return value		:	OK (=0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::Set_0xB0_0x23_IsoTableData(unsigned char* ucData, int iLen)
{
	FEDM_CHK3(ucData);

	if(iLen <= 0)
		return FEDM_ERROR_NO_DATA;

	bool bUidLF = false;
	bool bExtAdr = false;
	unsigned char	ucMode	= 0x00;
	unsigned char	ucBank	= 0x03;	// User Memory Bank
	unsigned char	ucBlockSize = 0x04;
	unsigned char	ucUidLen = 0x00;
	unsigned int	uiDBAdr	= 0x00;

	unsigned char	ucSnr[FEDM_ISC_MAX_UID_LENGTH];
	int		iByteCnt = 0;
	int		iCnt, iIdx, iBlockCnt, iErr;
	string	sSnr, tmp;

	// save number of received data blocks
	FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B0_RSP_DBN, ucData[iByteCnt++]));
	if(ucData[0] == 0)
		return FEDM_OK;

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE, &ucMode));
	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE_EXT_ADR, &bExtAdr));
	if(bExtAdr)
	{
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_DB_ADR_EXT, &uiDBAdr));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_BANK_BANK_NR, &ucBank));
	}
	else
	{
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_DB_ADR, &uiDBAdr));
	}

	if((unsigned char)(ucMode&0x07) == FEDM_ISC_ISO_MODE_NONADR)	// non addressed
	{
		iIdx = m_uiNonAddressedTabIndex;	// use entry with index >= 0
	}		
	else if((unsigned char)(ucMode&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
	{
		// search serial number in iso-table
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, tmp));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE_UID_LF, &bUidLF));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID_LEN, &ucUidLen));
		if(bUidLF)
		{
			sSnr = tmp.substr(0, (ucUidLen<<1));
		}
		else
		{
			sSnr = tmp.substr(0, 16);
		}
		
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sSnr);
		if(iIdx < 0)
		{
			// new entry in table
			iIdx = m_uiISOTableLength;
			m_uiISOTableLength++;

			// write new serial number (UID) into table
			FEDM_CHK1(iErr, FEDM_ConvHexStrToHexUChar(sSnr, &ucSnr[0], ucUidLen));
			for(iCnt=0; iCnt<ucUidLen; iCnt++)
				m_ISOTable[iIdx]->m_ucSnr[iCnt] = ucSnr[iCnt];
		}
	}
	else if((unsigned char)(ucMode&0x07) == FEDM_ISC_ISO_MODE_SEL)	// selected
	{
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
		if(iIdx < 0)
		{
			// exception for ISO 14443 Transponder, when Reader is configuered with Anticollision = off
			if(  m_uiISOTableLength == 1 && (m_ISOTable[0]->m_ucTrType == FEDM_ISC_TR_TYPE_ISO14443A || m_ISOTable[0]->m_ucTrType == FEDM_ISC_TR_TYPE_ISO14443B) )
				iIdx = 0;
			else
				return iIdx;
		}
	}
	else
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNKNOWN_ADDRESS_MODE));
		return FEDM_ERROR_UNKNOWN_ADDRESS_MODE;
	}

	FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B0_RSP_DB_SIZE, ucData[1]));

	ucBlockSize	= ucData[iByteCnt++];

	// save the blocksize in table and set the flag, that blocksize is guilty
	m_ISOTable[iIdx]->m_ucBlockSize = ucBlockSize;
	m_ISOTable[iIdx]->m_bIsBlockSizeSet = true;

	// write the received public data into table
	for(iBlockCnt=0; iBlockCnt<ucData[0]; iBlockCnt++)
	{
		FEDM_CHK1(iErr, m_ISOTable[iIdx]->SetData(0x23, &ucData[0], iByteCnt, ucBank, uiDBAdr));
		uiDBAdr++;
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        		:	09.03.2001 / M. Hultsch
  Version       	:	04.02.01 / 31.05.2012 / M. Hultsch
						- support for Innovatron

						03.00.16 / 20.11.2009 / M. Hultsch
						- clear of Card Info Format

						02.03.05 / 07.12.2005 / M. Hultsch

  Function			:	sets the received data of last [oxB0][0x25] iso protocol
						into iso-table

  Parameters		:	unsigned char* ucData	-	pointer to buffer with data

						int iLen		-	length of data

  Return value		:	OK (=0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::Set_0xB0_0x25_IsoTableData(unsigned char* ucData, int iLen)
{
	FEDM_CHK3(ucData);

	bool bUidLF = false;
	unsigned char	ucMode	= 0x00;
	unsigned char	ucUidLen = 0x00;
	unsigned char	ucFormat = 0x00;
	int		iIdx, iErr;
	string	sSnr, tmp;

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE, &ucMode));
	// only addressed supported
	if((unsigned char)(ucMode&0x07) != FEDM_ISC_ISO_MODE_ADR)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	}

	// clear card info format
	FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B0_RSP_FORMAT, (unsigned char)0x00));

	// save card information, comming with ISO14443 Transponder, if CINF flag is set in MODE
	// memory space in m_TmpData is used, beginning with offset 2048
	if(ucMode & FEDM_ISC_ISO_MODE_CINF)
	{
		ucFormat = ucData[0];
		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B0_RSP_FORMAT, ucFormat));

		switch(ucFormat)
		{
		case 0x00: // no card information
			break;

		case 0x01: // card information of an ISO14443-4 Type-A Transponder
			memcpy(&m_TmpData[2048], &ucData[1], 4);
			memcpy(&m_TmpData[2052], &ucData[5], ucData[4]); // ucData[4] is TL
			break;

		case 0x02: // card information of an ISO14443-4 Type-B Transponder
			memcpy(&m_TmpData[2048], &ucData[1], 4);
			break;

		case 0x03: // card information of an ISO14443-3 Type-A Transponder
			memcpy(&m_TmpData[2048], &ucData[1], 3);
			break;

		case 0x04: // card information of an Innovatron (ISO 14443B') Transponder
			memcpy(&m_TmpData[2048], &ucData[1], 1);
			break;
		}

	}

	// reset selected flags in the table
	for(iIdx=0; iIdx<(int)m_uiISOTableLength; iIdx++)
		m_ISOTable[iIdx]->m_bIsSelected = false;

	// search serial number in iso-table
	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, tmp));
	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE_UID_LF, &bUidLF));
	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID_LEN, &ucUidLen));
	if(bUidLF)
	{
		sSnr = tmp.substr(0, (ucUidLen<<1));
	}
	else
	{
		sSnr = tmp.substr(0, 16);
	}

	iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sSnr);
	if(iIdx < 0)
		return iIdx;

	// select new tag
	m_ISOTable[iIdx]->m_bIsSelected = true;

	// create command handler for ISO14443 transponder, if not previously done
	if(	m_ISOTable[iIdx]->m_ucTrType == FEDM_ISC_TR_TYPE_ISO14443A ||
		m_ISOTable[iIdx]->m_ucTrType == FEDM_ISC_TR_TYPE_ISO14443B )
	{
		if(! m_ISOTable[iIdx]->HasTagHandler())
		{
			if(ucMode & FEDM_ISC_ISO_MODE_CINF)
			{
				// identify transponder chip and create corresponding class
			}
		}
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        		:	19.03.2001 / M. Hultsch
  Version       	:	02.03.00 / 28.07.2005 / M. Hultsch

  Function			:	sets the received data of last [0xB0][0x26] iso protocol
						into iso-table

  Parameters		:	unsigned char* ucData	-	pointer to buffer with data
						int iLen		-	length of data

  Return value		:	OK (=0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::Set_0xB0_0x26_IsoTableData(unsigned char* ucData, int iLen)
{
	FEDM_CHK3(ucData);

	// search selected entry and deselect it
	int iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
	if(iIdx >= 0)
		m_ISOTable[iIdx]->m_bIsSelected = false;

	return FEDM_OK;
}

/***************************************************************************
  Begin        		:	09.03.2001 / M. Hultsch
  Version       	:	03.00.13 / 07.10.2009 / M. Hultsch

  Function			:	sets the received data of last [0xB0][0x2B] iso protocol
						into iso-table

  Parameters		:	unsigned char* ucData	-	pointer to buffer with data
						int iLen		-	length of data
						int iStatusByte	-	status byte of response protocol

  Return value		:	OK (=0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::Set_0xB0_0x2B_IsoTableData(unsigned char* ucData, int iLen, int iStatusByte)
{
	FEDM_CHK3(ucData);

	unsigned char	ucMode	= 0x00;
	unsigned char	ucSnr[FEDM_ISC_MAX_UID_LENGTH];
	int		iCnt, iIdx, iErr;
	string	sSnr, tmp;

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE, &ucMode));

	if((unsigned char)(ucMode&0x07) == FEDM_ISC_ISO_MODE_NONADR)	// non addressed
	{
		if(iLen == 0)
			return iStatusByte;

		iIdx = m_uiNonAddressedTabIndex;	// use entry with index >= 0

		// write new serial number (UID) into table
		for(iCnt=0; iCnt<8; iCnt++)
			m_ISOTable[0]->m_ucSnr[iCnt] = ucData[1+iCnt];
	}		
	else if((unsigned char)(ucMode&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
	{
		if(iLen == 0)
			return iStatusByte;

		// search serial number in iso-table
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, tmp));
		sSnr = tmp.substr(0, 16);
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sSnr);
		if(iIdx < 0)
		{
			// new entry in table
			iIdx = m_uiISOTableLength;
			m_uiISOTableLength++;

			// write new serial number (UID) into table
			FEDM_CHK1(iErr, FEDM_ConvHexStrToHexUChar(sSnr, &ucSnr[0], 8));
			for(iCnt=0; iCnt<8; iCnt++)
				m_ISOTable[iIdx]->m_ucSnr[iCnt] = ucSnr[iCnt];
		}
	}
	else if((unsigned char)(ucMode&0x07) == FEDM_ISC_ISO_MODE_SEL)	// selected
	{
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
		if(iIdx < 0)
			return iIdx;
	}
	else
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNKNOWN_ADDRESS_MODE));
		return FEDM_ERROR_UNKNOWN_ADDRESS_MODE;
	}

	// the reader sends no data, the tag is probably lost: 
	// we deselect the tag and return with the status byte of the response protocol
	if(iLen == 0)
	{
		m_ISOTable[iIdx]->m_bIsSelected = false;
		return iStatusByte;
	}

	// save the response data (except the UID)
	m_ISOTable[iIdx]->m_ucDsfID			= ucData[ 0];
	m_ISOTable[iIdx]->m_ucAFI			= ucData[ 9];
	m_ISOTable[iIdx]->m_ucMemSize[0]	= ucData[10];	// contains block size
	m_ISOTable[iIdx]->m_ucMemSize[1]	= ucData[11];	// block length (single or high byte)
	if(iLen == 14)
	{
		m_ISOTable[iIdx]->m_ucMemSize[2]	= ucData[12];	// block length (low byte)
		m_ISOTable[iIdx]->m_ucICRef			= ucData[13];
		m_ISOTable[iIdx]->m_ucMemSizeLen	= 3;
	}
	else
	{
		m_ISOTable[iIdx]->m_ucICRef			= ucData[12];
		m_ISOTable[iIdx]->m_ucMemSizeLen	= 2;
	}

	// if valid data in mem size, save block size information
	if(ucData[10] > 0)
		m_ISOTable[iIdx]->m_ucBlockSize = (ucData[10] & 0x1F) + (unsigned char)0x01;
		
	m_ISOTable[iIdx]->m_bIsSnr			= true;
	m_ISOTable[iIdx]->m_bIsAFI			= true;
	m_ISOTable[iIdx]->m_bIsSysInfo		= true;
	m_ISOTable[iIdx]->m_bIsBlockSizeSet	= true;

	return FEDM_OK;
}

/***************************************************************************
  Begin        		:	09.03.2001 / M. Hultsch
  Version       	:	04.06.03 / 26.06.2014 / M. Hultsch
						- support for extended address mode

						03.00.13 / 07.10.2009 / M. Hultsch

  Function			:	sets the received data of last [0xB0][0x2C] iso protocol
						into iso-table

  Parameters		:	unsigned char* ucData	-	pointer to buffer with data
						int iLen		-	length of data

  Return value		:	OK (=0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::Set_0xB0_0x2C_IsoTableData(unsigned char* ucData, int iLen)
{
	FEDM_CHK3(ucData);

	if(iLen <= 0)
		return FEDM_ERROR_NO_DATA;

	bool bExtAddr = false;
	unsigned int	uiDBAdr	= 0x00;
	unsigned char	ucMode	= 0x00;
	unsigned char	ucSnr[FEDM_ISC_MAX_UID_LENGTH];
	int		iCnt, iIdx, iBlockCnt, iErr;
	string	sSnr, tmp;

	// save number of received data blocks
	FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B0_RSP_DBN, ucData[0]));
	if(ucData[0] == 0)
		return FEDM_OK;

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE_EXT_ADR, &bExtAddr));
	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE, &ucMode));

	if(bExtAddr)
	{
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_DB_ADR_EXT, &uiDBAdr));
	}
	else
	{
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_DB_ADR, &uiDBAdr));
	}
	
	if((unsigned char)(ucMode&0x07) == FEDM_ISC_ISO_MODE_NONADR)	// non addressed
	{
		iIdx = m_uiNonAddressedTabIndex;	// use entry with index >= 0
	}		
	else if((unsigned char)(ucMode&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
	{
		// search serial number in iso-table
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, tmp));
		sSnr = tmp.substr(0, 16);
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sSnr);
		if(iIdx < 0)
		{
			// new entry in table
			iIdx = m_uiISOTableLength;
			m_uiISOTableLength++;

			// write new serial number (UID) into table
			FEDM_CHK1(iErr, FEDM_ConvHexStrToHexUChar(sSnr, &ucSnr[0], 8));
			for(iCnt=0; iCnt<8; iCnt++)
				m_ISOTable[iIdx]->m_ucSnr[iCnt] = ucSnr[iCnt];
		
			m_ISOTable[iIdx]->m_bIsSnr = true;
		}
	}
	else if((unsigned char)(ucMode&0x07) == FEDM_ISC_ISO_MODE_SEL)	// selected
	{
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
		if(iIdx < 0)
			return iIdx;
	}
	else
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNKNOWN_ADDRESS_MODE));
		return FEDM_ERROR_UNKNOWN_ADDRESS_MODE;
	}

	if((unsigned int)(uiDBAdr+ucData[0]) >= m_ISOTable[iIdx]->m_ucSecStatus.size())
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_BUFFER_LENGTH));
		return FEDM_ERROR_BUFFER_LENGTH;
	}

	// write the received security status into table
	for(iBlockCnt=1; iBlockCnt<=ucData[0]; iBlockCnt++)
		m_ISOTable[iIdx]->m_ucSecStatus[uiDBAdr++] = ucData[iBlockCnt];

	return FEDM_OK;
}

/***************************************************************************
  Begin        		:	09.03.2001 / M. Hultsch
  Version       	:	02.04.01 / 28.03.2006 / M. Hultsch

  Function			:	sets the received data of last [0xB0][0xA0] iso protocol
						into iso-table

  Parameters		:	unsigned char* ucData	-	pointer to buffer with data
						int iLen		-	length of data

  Return value		:	OK (=0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::Set_0xB0_0xA0_IsoTableData(unsigned char* ucData, int iLen)
{
	FEDM_CHK3(ucData);

	if(iLen <= 0)
		return FEDM_ERROR_NO_DATA;

	unsigned char	ucCBAdr	= 0x00;
	unsigned char	ucMode	= 0x00;
	unsigned char	ucSnr[FEDM_ISC_MAX_UID_LENGTH];
	int		iByteCnt = 0;
	int		iCnt, iIdx, iErr;
	string	sSnr, tmp;

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_MODE, &ucMode));
	// only addressed supported
	if((unsigned char)(ucMode&0x07) != FEDM_ISC_ISO_MODE_ADR)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	}

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_CB_ADR, &ucCBAdr));
	
	// search serial number in iso-table
	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B0_REQ_UID, sSnr));
	sSnr = tmp.substr(0, 16);
	iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sSnr);
	if(iIdx < 0)
	{
		// new entry in table
		iIdx = m_uiISOTableLength;
		m_uiISOTableLength++;

		// write new serial number (UID) into table
		FEDM_CHK1(iErr, FEDM_ConvHexStrToHexUChar(sSnr, &ucSnr[0], 8));
		for(iCnt=0; iCnt<8; iCnt++)
			m_ISOTable[iIdx]->m_ucSnr[iCnt] = ucSnr[iCnt];
	}

	// write config data into table
	FEDM_CHK1(iErr, m_ISOTable[iIdx]->SetData(0xA0, &ucData[0], iByteCnt, 0, (int)ucCBAdr));

	return FEDM_OK;
}


/***************************************************************************
  Begin        		:	28.07.2005 / M. Hultsch
  Version       	:	02.03.00 / 28.07.2005 / M. Hultsch

  Function			:	sets the received data of last [0xB2][0x2B] iso protocol
						into iso-table

  Parameters		:	unsigned char* ucData	-	pointer to buffer with data
						int iLen		-	length of data

  Return value		:	OK (=0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::Set_0xB2_0x2B_IsoTableData(unsigned char* ucData, int iLen)
{
	FEDM_CHK3(ucData);

	if(iLen <= 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_NO_DATA));
		return FEDM_ERROR_NO_DATA;
	}

	int		iIdx;

	// command only in selected mode
	iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
	if(iIdx < 0)
	{
		// exception for ISO 14443 Transponder, when Reader is configuered with Anticollision = off
		if(  m_uiISOTableLength == 1 && (m_ISOTable[0]->m_ucTrType == FEDM_ISC_TR_TYPE_ISO14443A || m_ISOTable[0]->m_ucTrType == FEDM_ISC_TR_TYPE_ISO14443B) )
			iIdx = 0;
		else
			return iIdx;
	}

	// save the response data
	m_ISOTable[iIdx]->m_ucFSCI	= ucData[0];
	m_ISOTable[iIdx]->m_ucFWI	= ucData[1];
	m_ISOTable[iIdx]->m_ucDSI	= ucData[2];
	m_ISOTable[iIdx]->m_ucDRI	= ucData[3];
	m_ISOTable[iIdx]->m_ucNad	= ucData[4];
	m_ISOTable[iIdx]->m_ucCid	= ucData[5];

	m_ISOTable[iIdx]->m_bIsISO14443_4Info	= true;

	return FEDM_OK;
}

//////////////////////////////////////////////////////////////////////
// functions for [0xB3] EPC Host Protocol
//////////////////////////////////////////////////////////////////////

/***************************************************************************
  Begin        		:	30.06.2005 / M. Hultsch
  Version       	:	04.06.06 / 23.09.2013 / M. Hultsch
						- only for EPC Class1 Gen2:
						  bugfix for addressed Host commands, if Reader is configured for UID = EPC + TID

						04.03.01 / 01.11.2012 / M. Hultsch
						- support for Recommissioning Bits in [0xB3][0x18] Kill

						04.02.04 / 21.06.2012 / M. Hultsch
						- UHF Class1 Gen2: support for [0xB3][0x25] Block Permalock and [0xB3][0x26] Read Permalock Status

						04.02.00 / 11.05.2012 / M. Hultsch
						- only for EPC Class1 Gen2:
						  if Reader is configured for UID = EPC + TID, use IDDT from last Inventory to evaluate UID

						03.03.04 / 21.06.2011 / M. Hultsch
						- only for EPC Class1 Gen2:
						  if Reader is configured for UID = EPC + TID, set TID as UID for next request in addressed mode

						02.05.05 / 24.10.2006 / M. Hultsch

  Function			:	collects the necessary data for the next [0xB3] epc protocol

  Parameters		:	unsigned char ucCmdByte	-	epc command byte
						unsigned char* ucData	-	pointer to buffer for data

  Return value		:	iCnt (>0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::Get_0xB3_IsoTableData(unsigned char ucCmdByte, unsigned char* ucData)
{
	FEDM_CHK3(ucData);

	bool bUidLF = false;
	bool bRecomBits = false;
	int iCnt = 0;
	int iErr = 0;
	int iIdx = 0;
	unsigned char ucUidLen	= 0x00;
	unsigned char ucEpcLen	= 0x00;
	unsigned char ucTrType	= 0x00;
	unsigned char ucLen		= 0x00;
	unsigned char ucUid[FEDM_ISC_MAX_UID_LENGTH];
	string sUid;

	switch(ucCmdByte)
	{
	case 0x18:	// [0x18] Kill
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_CMD, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_MODE, &ucData[iCnt++]));
		
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_MODE_EPC_LF, &bUidLF));
			if(bUidLF)	// variable UID length
			{
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_EPC_LEN, &ucUidLen));
			}
			else
			{
				ucUidLen = 8;
			}

			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_EPC, &ucUid[0], (int)ucUidLen));
			FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], (int)ucUidLen, sUid));
			
			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
			if(iIdx < 0)
				return iIdx;

			// only for UHF EPC Class1 Gen2
			if(	m_ISOTable[iIdx]->m_ucTrType == FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2 ||
				m_ISOTable[iIdx]->m_ucTrType == FEDM_ISC_TR_TYPE_ISO18000_3M3)
			{
				// check, if Reader is configured for UID = EPC + TID
				if(m_ISOTable[iIdx]->m_ucIDDT == 0x02)
				{
					// check, if response contains UID = EPC + TID
					ucEpcLen = ((m_ISOTable[iIdx]->m_ucClass1Gen2_PC[0] & 0xF8) >> 3) * 2;
					if(m_ISOTable[iIdx]->m_ucClass1Gen2_PC[0] & 0x02) // extended PC W1
						ucEpcLen -= 2;

					if( ucEpcLen < ucUidLen)
					{
						// take TID as UID for next request
						if(bUidLF || ((ucUidLen - ucEpcLen) != 8))
							ucData[iCnt++] = (ucUidLen - ucEpcLen);
						
						memcpy(&ucData[iCnt], &ucUid[ucEpcLen], ucUidLen - ucEpcLen);
						iCnt += (ucUidLen - ucEpcLen);
					}
					else
					{
						// error in the Reader response: TID is missing
						FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_INVENTORY_NO_TID_IN_UID));
						return FEDM_ERROR_INVENTORY_NO_TID_IN_UID;
					}
				}
				else
				{
					if(bUidLF || (ucUidLen != 8))
						ucData[iCnt++] = ucUidLen;
					
					// UID = unified identifier = serial number
					FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_EPC, &ucData[iCnt], ucUidLen));
					iCnt += ucUidLen;
				}
			}
			else
			{
				if(bUidLF)	// variable UID length
				{
					ucData[iCnt++] = ucUidLen;
					// UID = unified identifier = serial number
					FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_EPC, &ucData[iCnt], ucUidLen));
					iCnt += ucUidLen;
				}
				else
				{
					// UID = unified identifier = serial number
					FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_EPC, &ucData[iCnt], 8));
					iCnt += 8;
				}
			}
		}

		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_KILL_PW_LENGTH, &ucLen));
		ucData[iCnt++] = ucLen;
		if(ucLen > 0)
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_KILL_PW, &ucData[iCnt], ucLen));
			iCnt += ucLen;
		}
		
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_MODE_RECOM, &bRecomBits));
		if(bRecomBits)
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_RECOM_BITS, &ucData[iCnt++]));
		}

		break;

	case 0x22:	// [0x22] Lock
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_CMD, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_MODE, &ucData[iCnt++]));
		
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_MODE_EPC_LF, &bUidLF));
			if(bUidLF)	// variable UID length
			{
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_EPC_LEN, &ucUidLen));
			}
			else
			{
				ucUidLen = 8;
			}

			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_EPC, &ucUid[0], (int)ucUidLen));
			FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], (int)ucUidLen, sUid));
			
			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
			if(iIdx < 0)
				return iIdx;

			// only for UHF EPC Class1 Gen2
			if(	m_ISOTable[iIdx]->m_ucTrType == FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2 ||
				m_ISOTable[iIdx]->m_ucTrType == FEDM_ISC_TR_TYPE_ISO18000_3M3)
			{
				// check, if Reader is configured for UID = EPC + TID
				if(m_ISOTable[iIdx]->m_ucIDDT == 0x02)
				{
					// check, if response contains UID = EPC + TID
					ucEpcLen = ((m_ISOTable[iIdx]->m_ucClass1Gen2_PC[0] & 0xF8) >> 3) * 2;
					if(m_ISOTable[iIdx]->m_ucClass1Gen2_PC[0] & 0x02) // extended PC W1
						ucEpcLen -= 2;

					if( ucEpcLen < ucUidLen)
					{
						// take TID as UID for next request
						if(bUidLF || ((ucUidLen - ucEpcLen) != 8))
							ucData[iCnt++] = (ucUidLen - ucEpcLen);
						
						memcpy(&ucData[iCnt], &ucUid[ucEpcLen], ucUidLen - ucEpcLen);
						iCnt += (ucUidLen - ucEpcLen);
					}
					else
					{
						// error in the Reader response: TID is missing
						FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_INVENTORY_NO_TID_IN_UID));
						return FEDM_ERROR_INVENTORY_NO_TID_IN_UID;
					}
				}
				else
				{
					if(bUidLF || (ucUidLen != 8))
						ucData[iCnt++] = ucUidLen;
					
					// UID = unified identifier = serial number
					FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_EPC, &ucData[iCnt], ucUidLen));
					iCnt += ucUidLen;
				}
			}
			else
			{
				if(bUidLF)	// variable UID length
				{
					ucData[iCnt++] = ucUidLen;
					// UID = unified identifier = serial number
					FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_EPC, &ucData[iCnt], ucUidLen));
					iCnt += ucUidLen;
				}
				else
				{
					// UID = unified identifier = serial number
					FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_EPC, &ucData[iCnt], 8));
					iCnt += 8;
				}
			}
		}
			
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_TR_TYPE, &ucTrType));
		ucData[iCnt++] = ucTrType;

		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_LOCK_DATA_LENGTH, &ucLen));
		ucData[iCnt++] = ucLen;

		switch(ucTrType)
		{
		case FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN1:
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_LOCK_DATA, &ucData[iCnt], ucLen));
			iCnt += ucLen;
			ucData[iCnt++] = 0x00; // PW-Length
			break;

		case FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2:
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_LOCK_DATA, &ucData[iCnt], ucLen));
			iCnt += ucLen;
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_ACCESS_PW_LENGTH, &ucLen));
			ucData[iCnt++] = ucLen;
			if(ucLen > 0)
			{
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_ACCESS_PW, &ucData[iCnt], ucLen));
				iCnt += ucLen;
			}
			break;
		}

		break;

	case 0x24:	// [0x24] Write Multiple Blocks
		FEDM_CHK1(iErr, Get_0xB3_0x24_IsoTableData(ucData));
		iCnt += iErr;
		break;

	case 0x25:	// [0x25] Block Permalock
		FEDM_CHK1(iErr, Get_0xB3_0x25_IsoTableData(ucData));
		iCnt += iErr;
		break;

	case 0x26:	// [0x26] Read Permalock Status
		FEDM_CHK1(iErr, Get_0xB3_0x26_IsoTableData(ucData));
		iCnt += iErr;
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		return FEDM_ERROR_UNSUPPORTED;
	}

	return iCnt;
}

/***************************************************************************
  Begin        		:	30.06.2005 / M. Hultsch
  Version       	:	04.06.06 / 23.09.2013 / M. Hultsch
						- only for EPC Class1 Gen2:
						  bugfix for addressed Host commands, if Reader is configured for UID = EPC + TID

						04.02.00 / 11.05.2012 / M. Hultsch
						- only for EPC Class1 Gen2:
						  if Reader is configured for UID = EPC + TID, use IDDT from last Inventory to evaluate UID

						04.00.99 / 29.07.2011 / M. Hultsch

  Function			:	collects the necessary data for the next [0xB3][0x24] epc protocol

  Parameters		:	unsigned char* ucData	-	pointer to buffer for data

  Return value		:	iCnt (>0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::Get_0xB3_0x24_IsoTableData(unsigned char* ucData)
{
	FEDM_CHK3(ucData);

	bool bUidLF = false;
	bool bExtAdr = false;
	bool bAccessPw = false;
	int iCnt = 0;
	int iIdx = 0;
	int iErr = 0;
	int iBlockCnt = 0;
	unsigned char ucBank	= 0x03;	// User Memory Bank
	unsigned char ucDBN		= 0x00;
	unsigned char ucBlockSize = 0x04;
	unsigned char ucUidLen	= 0x00;
	unsigned char ucEpcLen	= 0x00;
	unsigned char ucLen		= 0x00;
	unsigned int uiDBAdr	= 0;
	unsigned char ucUid[FEDM_ISC_MAX_UID_LENGTH];
	string sUid;

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_CMD, &ucData[iCnt++]));
	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_MODE, &ucData[iCnt++]));

	if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_NONADR)	// non addressed
	{
		iIdx = m_uiNonAddressedTabIndex;	// take data from entry with index >= 0
	}
	else if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
	{
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_MODE_EPC_LF, &bUidLF));
		if(bUidLF)	// variable UID length
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_EPC_LEN, &ucUidLen));
		}
		else
		{
			ucUidLen = 8;
		}

		// UID = unified identifier = serial number
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_EPC, &ucUid[0], (int)ucUidLen));
		FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], (int)ucUidLen, sUid));
		
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
		if(iIdx < 0)
			return iIdx;

		// only for UHF EPC Class1 Gen2
		if(	m_ISOTable[iIdx]->m_ucTrType == FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2 ||
			m_ISOTable[iIdx]->m_ucTrType == FEDM_ISC_TR_TYPE_ISO18000_3M3)
		{
			// check, if Reader is configured for UID = EPC + TID
			if(m_ISOTable[iIdx]->m_ucIDDT == 0x02)
			{
				// check, if response contains UID = EPC + TID
				ucEpcLen = ((m_ISOTable[iIdx]->m_ucClass1Gen2_PC[0] & 0xF8) >> 3) * 2;
				if(m_ISOTable[iIdx]->m_ucClass1Gen2_PC[0] & 0x02) // extended PC W1
					ucEpcLen -= 2;

				if( ucEpcLen < ucUidLen)
				{
					// take TID as UID for next request
					if(bUidLF || ((ucUidLen - ucEpcLen) != 8))
						ucData[iCnt++] = (ucUidLen - ucEpcLen);
					
					memcpy(&ucData[iCnt], &ucUid[ucEpcLen], ucUidLen - ucEpcLen);
					iCnt += (ucUidLen - ucEpcLen);
				}
				else
				{
					// error in the Reader response: TID is missing
					FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_INVENTORY_NO_TID_IN_UID));
					return FEDM_ERROR_INVENTORY_NO_TID_IN_UID;
				}
			}
			else
			{
				if(bUidLF || (ucUidLen != 8))
					ucData[iCnt++] = ucUidLen;
				
				// UID = unified identifier = serial number
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_EPC, &ucData[iCnt], ucUidLen));
				iCnt += ucUidLen;
			}
		}
		else
		{
			if(bUidLF)	// variable UID length
			{
				ucData[iCnt++] = ucUidLen;
				// UID = unified identifier = serial number
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_EPC, &ucData[iCnt], ucUidLen));
				iCnt += ucUidLen;
			}
			else
			{
				// UID = unified identifier = serial number
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_EPC, &ucData[iCnt], 8));
				iCnt += 8;
			}
		}
	}
	else
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNKNOWN_ADDRESS_MODE));
		return FEDM_ERROR_UNKNOWN_ADDRESS_MODE;
	}

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_MODE_EXT_ADR, &bExtAdr));
	if(bExtAdr)
	{
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_BANK, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_BANK_BANK_NR, &ucBank));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_BANK_ACCESS_FLAG, &bAccessPw));
		if(bAccessPw)
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_ACCESS_PW_LENGTH, &ucLen));
			ucData[iCnt++] = ucLen;
			if(ucLen > 0)
			{
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_ACCESS_PW, &ucData[iCnt], ucLen));
				iCnt += ucLen;
			}
		}
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_DB_ADR_EXT, &ucData[iCnt], 2));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_DB_ADR_EXT, &uiDBAdr));
		iCnt += 2;
	}
	else
	{
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_DB_ADR, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_DB_ADR, &uiDBAdr));
	}

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_DBN, &ucData[iCnt++]));
	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_DBN, &ucDBN));
	

	// take the blocksize from table
	ucBlockSize = m_ISOTable[iIdx]->m_ucBlockSize;
	FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B3_REQ_DB_SIZE, ucBlockSize));
	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_DB_SIZE, &ucData[iCnt++]));

	// avoid buffer overflow
	if( (5+iCnt+ucDBN*ucBlockSize) > FEDM_MAX_PROTOCOL_SIZE)	//    5 : general protocol frame
																// iCnt : [0x24] protocol frame
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_SENDBUFFER_OVERFLOW));
		return FEDM_ERROR_SENDBUFFER_OVERFLOW;
	}

	for(iBlockCnt=0; iBlockCnt<ucDBN; iBlockCnt++)
	{
		FEDM_CHK1(iErr, m_ISOTable[iIdx]->GetData(0x24, ucData, iCnt, ucBank, uiDBAdr));
		uiDBAdr++;
	}

	return iCnt;
}

/***************************************************************************
  Begin        		:	21.06.2012 / M. Hultsch
  Version       	:	04.06.06 / 23.09.2013 / M. Hultsch
						- only for EPC Class1 Gen2:
						  bugfix for addressed Host commands, if Reader is configured for UID = EPC + TID

						04.02.04 / 21.06.2012 / M. Hultsch

  Function			:	collects the necessary data for the next [0xB3][0x25] Block Permalock protocol

  Parameters		:	unsigned char* ucData	-	pointer to buffer for data

  Return value		:	iCnt (>0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::Get_0xB3_0x25_IsoTableData(unsigned char* ucData)
{
	FEDM_CHK3(ucData);

	bool bUidLF = false;
	bool bExtAdr = false;
	bool bAccessPw = false;
	int iCnt = 0;
	int iIdx = 0;
	int iErr = 0;
	unsigned char ucBank	= 0x03;	// User Memory Bank
	unsigned char ucUidLen	= 0x00;
	unsigned char ucEpcLen	= 0x00;
	unsigned char ucLen		= 0x00;
	unsigned char ucBlockRange	= 0;
	unsigned char ucUid[FEDM_ISC_MAX_UID_LENGTH];
	string sUid;

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_CMD, &ucData[iCnt++]));
	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_MODE, &ucData[iCnt++]));

	if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_NONADR)	// non addressed
	{
		iIdx = m_uiNonAddressedTabIndex;	// take data from entry with index >= 0
	}
	else if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
	{
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_MODE_EPC_LF, &bUidLF));
		if(bUidLF)	// variable UID length
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_EPC_LEN, &ucUidLen));
		}
		else
		{
			// error: EPC_LF is missing
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " - mode flag EPC_LF is missing");
			return FEDM_ERROR_PARAMETER;
		}

		// UID = unified identifier = serial number
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_EPC, &ucUid[0], (int)ucUidLen));
		FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], (int)ucUidLen, sUid));
		
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
		if(iIdx < 0)
			return iIdx;

		// only for UHF EPC Class1 Gen2
		if(	m_ISOTable[iIdx]->m_ucTrType == FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2 ||
			m_ISOTable[iIdx]->m_ucTrType == FEDM_ISC_TR_TYPE_ISO18000_3M3)
		{
			// check, if Reader is configured for UID = EPC + TID
			if(m_ISOTable[iIdx]->m_ucIDDT == 0x02)
			{
				// check, if response contains UID = EPC + TID
				ucEpcLen = ((m_ISOTable[iIdx]->m_ucClass1Gen2_PC[0] & 0xF8) >> 3) * 2;
				if(m_ISOTable[iIdx]->m_ucClass1Gen2_PC[0] & 0x02) // extended PC W1
					ucEpcLen -= 2;

				if( ucEpcLen < ucUidLen)
				{
					// take TID as UID for next request
					if(bUidLF || ((ucUidLen - ucEpcLen) != 8))
						ucData[iCnt++] = (ucUidLen - ucEpcLen);
					
					memcpy(&ucData[iCnt], &ucUid[ucEpcLen], ucUidLen - ucEpcLen);
					iCnt += (ucUidLen - ucEpcLen);
				}
				else
				{
					// error in the Reader response: TID is missing
					FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_INVENTORY_NO_TID_IN_UID));
					return FEDM_ERROR_INVENTORY_NO_TID_IN_UID;
				}
			}
			else
			{
				if(bUidLF || (ucUidLen != 8))
					ucData[iCnt++] = ucUidLen;
				
				// UID = unified identifier = serial number
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_EPC, &ucData[iCnt], ucUidLen));
				iCnt += ucUidLen;
			}
		}
		else
		{
			if(bUidLF)	// variable UID length
			{
				ucData[iCnt++] = ucUidLen;
				// UID = unified identifier = serial number
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_EPC, &ucData[iCnt], ucUidLen));
				iCnt += ucUidLen;
			}
			else
			{
				// UID = unified identifier = serial number
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_EPC, &ucData[iCnt], 8));
				iCnt += 8;
			}
		}
	}
	else
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNKNOWN_ADDRESS_MODE));
		return FEDM_ERROR_UNKNOWN_ADDRESS_MODE;
	}

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_MODE_EXT_ADR, &bExtAdr));
	if(bExtAdr)
	{
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_BANK, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_BANK_BANK_NR, &ucBank));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_BANK_ACCESS_FLAG, &bAccessPw));
		if(bAccessPw)
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_ACCESS_PW_LENGTH, &ucLen));
			ucData[iCnt++] = ucLen;
			if(ucLen > 0)
			{
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_ACCESS_PW, &ucData[iCnt], ucLen));
				iCnt += ucLen;
			}
		}
	}
	else
	{
		// error: flag EXT_ADR is missing
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " - mode flag EXT_ADR is missing");
		return FEDM_ERROR_PARAMETER;
	}

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_BLOCK_PTR, &ucData[iCnt], 2));
	iCnt += 2;
	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_BLOCK_RANGE, &ucData[iCnt]));
	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_BLOCK_RANGE, &ucBlockRange));
	iCnt++;

	// avoid mask buffer overflow
	if( (2*ucBlockRange) > 8) // FEDM_ISC_TMP_B3_REQ_MASK is limited to 8 bytes
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " Block-Range is greater than 4");
		return FEDM_ERROR_BUFFER_LENGTH;
	}

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_MASK, &ucData[iCnt], 2*ucBlockRange));
	iCnt += 2*ucBlockRange;

	return iCnt;
}


/***************************************************************************
  Begin        		:	21.06.2012 / M. Hultsch
  Version       	:	04.06.06 / 23.09.2013 / M. Hultsch
						- only for EPC Class1 Gen2:
						  bugfix for addressed Host commands, if Reader is configured for UID = EPC + TID

						04.02.04 / 21.06.2012 / M. Hultsch

  Function			:	collects the necessary data for the next [0xB3][0x26] Read Permalock Status protocol

  Parameters		:	unsigned char* ucData	-	pointer to buffer for data

  Return value		:	iCnt (>0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::Get_0xB3_0x26_IsoTableData(unsigned char* ucData)
{
	FEDM_CHK3(ucData);

	bool bUidLF = false;
	bool bExtAdr = false;
	bool bAccessPw = false;
	int iCnt = 0;
	int iIdx = 0;
	int iErr = 0;
	unsigned char ucBank	= 0x03;	// User Memory Bank
	unsigned char ucUidLen	= 0x00;
	unsigned char ucEpcLen	= 0x00;
	unsigned char ucLen		= 0x00;
	unsigned char ucBlockRange	= 0;
	unsigned char ucUid[FEDM_ISC_MAX_UID_LENGTH];
	string sUid;

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_CMD, &ucData[iCnt++]));
	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_MODE, &ucData[iCnt++]));

	if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_NONADR)	// non addressed
	{
		iIdx = m_uiNonAddressedTabIndex;	// take data from entry with index >= 0
	}
	else if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
	{
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_MODE_EPC_LF, &bUidLF));
		if(bUidLF)	// variable UID length
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_EPC_LEN, &ucUidLen));
		}
		else
		{
			// error: EPC_LF is missing
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " - mode flag EPC_LF is missing");
			return FEDM_ERROR_PARAMETER;
		}

		// UID = unified identifier = serial number
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_EPC, &ucUid[0], (int)ucUidLen));
		FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], (int)ucUidLen, sUid));
		
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
		if(iIdx < 0)
			return iIdx;

		// only for UHF EPC Class1 Gen2
		if(	m_ISOTable[iIdx]->m_ucTrType == FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2 ||
			m_ISOTable[iIdx]->m_ucTrType == FEDM_ISC_TR_TYPE_ISO18000_3M3)
		{
			// check, if Reader is configured for UID = EPC + TID
			if(m_ISOTable[iIdx]->m_ucIDDT == 0x02)
			{
				// check, if response contains UID = EPC + TID
				ucEpcLen = ((m_ISOTable[iIdx]->m_ucClass1Gen2_PC[0] & 0xF8) >> 3) * 2;
				if(m_ISOTable[iIdx]->m_ucClass1Gen2_PC[0] & 0x02) // extended PC W1
					ucEpcLen -= 2;

				if( ucEpcLen < ucUidLen)
				{
					// take TID as UID for next request
					if(bUidLF || ((ucUidLen - ucEpcLen) != 8))
						ucData[iCnt++] = (ucUidLen - ucEpcLen);
					
					memcpy(&ucData[iCnt], &ucUid[ucEpcLen], ucUidLen - ucEpcLen);
					iCnt += (ucUidLen - ucEpcLen);
				}
				else
				{
					// error in the Reader response: TID is missing
					FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_INVENTORY_NO_TID_IN_UID));
					return FEDM_ERROR_INVENTORY_NO_TID_IN_UID;
				}
			}
			else
			{
				if(bUidLF || (ucUidLen != 8))
					ucData[iCnt++] = ucUidLen;
				
				// UID = unified identifier = serial number
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_EPC, &ucData[iCnt], ucUidLen));
				iCnt += ucUidLen;
			}
		}
		else
		{
			if(bUidLF)	// variable UID length
			{
				ucData[iCnt++] = ucUidLen;
				// UID = unified identifier = serial number
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_EPC, &ucData[iCnt], ucUidLen));
				iCnt += ucUidLen;
			}
			else
			{
				// UID = unified identifier = serial number
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_EPC, &ucData[iCnt], 8));
				iCnt += 8;
			}
		}
	}
	else
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNKNOWN_ADDRESS_MODE));
		return FEDM_ERROR_UNKNOWN_ADDRESS_MODE;
	}

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_MODE_EXT_ADR, &bExtAdr));
	if(bExtAdr)
	{
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_BANK, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_BANK_BANK_NR, &ucBank));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_BANK_ACCESS_FLAG, &bAccessPw));
		if(bAccessPw)
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_ACCESS_PW_LENGTH, &ucLen));
			ucData[iCnt++] = ucLen;
			if(ucLen > 0)
			{
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_ACCESS_PW, &ucData[iCnt], ucLen));
				iCnt += ucLen;
			}
		}
	}
	else
	{
		// error: flag EXT_ADR is missing
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " - mode flag EXT_ADR is missing");
		return FEDM_ERROR_PARAMETER;
	}

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_BLOCK_PTR, &ucData[iCnt], 2));
	iCnt += 2;
	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_BLOCK_RANGE, &ucData[iCnt]));
	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_BLOCK_RANGE, &ucBlockRange));
	iCnt++;

	// avoid mask buffer overflow
	if( (2*ucBlockRange) > 8) // FEDM_ISC_TMP_B3_REQ_MASK is limited to 8 bytes
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " Block-Range is greater than 4");
		return FEDM_ERROR_BUFFER_LENGTH;
	}

	return iCnt;
}


/***************************************************************************
  Begin        		:	21.06.2012 / M. Hultsch
  Version       	:	04.02.06 / 03.08.2012 / M. Hultsch

  Function			:	sets the received data of last [0xB3] epc protocol

  Parameters		:	unsigned char* ucData	-	pointer to buffer with data
						int iLen				-	length of data
						int iStatusByte			-	status byte of receive protocol

  Return value		:	OK (=0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::Set_0xB3_IsoTableData(unsigned char* ucData, int iLen, int iStatusByte)
{
	FEDM_CHK3(ucData);

	unsigned char	ucCmd	= 0x00;
	unsigned char ucBlockRange	= 0;
	int iByteCnt = 0;
	int iErr = 0;

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_CMD, &ucCmd));

	switch(ucCmd)
	{
	case 0x18:	// [0xB3][0x18] Kill
	case 0x22:	// [0xB3][0x22] Lock
	case 0x24:	// [0xB3][0x24] Write Multiple Blocks
	case 0x25:	// [0xB3][0x25] Block Permalock
		break;

	case 0x26:	// [0xB3][0x26] Read Permalock Status
		if(iLen <= 0)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_NO_DATA));
			return FEDM_ERROR_NO_DATA;
		}

		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B3_REQ_BLOCK_RANGE, &ucBlockRange));

		// avoid mask buffer overflow
		if( (2*ucBlockRange) > 8) // FEDM_ISC_TMP_B3_REQ_MASK is limited to 8 bytes
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " Block-Range is greater than 4");
			return FEDM_ERROR_BUFFER_LENGTH;
		}

		FEDM_CHK1(iErr, SetData(FEDM_ISC_TMP_B3_RSP_MASK, &ucData[iByteCnt++], 2*ucBlockRange));

		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmd << ") " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		return FEDM_ERROR_UNSUPPORTED;
	}

	return FEDM_OK;
}


//////////////////////////////////////////////////////////////////////
// functions for [0xB4] EPC UHF Host Protocol
//////////////////////////////////////////////////////////////////////

/***************************************************************************
  Begin        		:	23.10.2006 / M. Hultsch
  Version       	:	03.01.05 / 22.04.2010 / M. Hultsch

  Function			:	collects the necessary data for the next [0xB4] epc uhf protocol

  Parameters		:	unsigned char ucCmdByte	-	epc command byte
						unsigned char* ucData	-	pointer to buffer for data

  Return value		:	iCnt (>0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::Get_0xB4_IsoTableData(unsigned char ucCmdByte, unsigned char* ucData)
{
	FEDM_CHK3(ucData);

	int iErr = 0;
	int iCnt = 0;
	unsigned char ucMfr	= 0x00;

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_MFR, &ucMfr));

	switch(ucMfr)
	{
	case FEDM_ISC_ISO_MFR_NXP:
		return Get_0xB4_IsoTableData_NXP(ucCmdByte, ucData);

	case FEDM_ISC_ISO_MFR_EM:
		return Get_0xB4_IsoTableData_EM(ucCmdByte, ucData);

	case FEDM_ISC_ISO_MFR_IDS:
		return Get_0xB4_IsoTableData_IDS(ucCmdByte, ucData);

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ", Manufacturer=0x" << std::hex << std::uppercase << (int)ucMfr << ") " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		return FEDM_ERROR_UNSUPPORTED;
	}

	return iCnt;
}

/***************************************************************************
  Begin        		:	22.04.2010 / M. Hultsch
  Version       	:	04.06.10 / 02.12.2014 / M. Hultsch
						- new [0xB4] NXP Custom Commands: 0x10, 0x11, 0x12

						04.06.06 / 23.09.2013 / M. Hultsch
						- only for EPC Class1 Gen2:
						  bugfix for addressed Host commands, if Reader is configured for UID = EPC + TID

						04.02.00 / 11.05.2012 / M. Hultsch
						- if Reader is configured for UID = EPC + TID, use IDDT from last Inventory to evaluate UID

						04.00.00 / 29.07.2011 / M. Hultsch

  Function			:	collects the necessary data for the next [0xB4] epc uhf protocol

  Parameters		:	unsigned char ucCmdByte	-	epc command byte
						unsigned char* ucData	-	pointer to buffer for data

  Return value		:	iCnt (>0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::Get_0xB4_IsoTableData_NXP(unsigned char ucCmdByte, unsigned char* ucData)
{
	FEDM_CHK3(ucData);

	bool bUidLF = false;
	bool bExtAdr = false;
	bool bAccessPw = false;
	int iErr = 0;
	int iCnt = 0;
	int iIdx = 0;
	unsigned char ucUidLen	= 0x00;
	unsigned char ucEpcLen	= 0x00;
	unsigned char ucLen		= 0x00;
	unsigned char ucUid[FEDM_ISC_MAX_UID_LENGTH];
	string sUid;

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_CMD, &ucData[iCnt++]));
	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_MODE, &ucData[iCnt++]));

	switch(ucCmdByte)
	{
	case 0x04:	// [0x04] EAS Alarm
		break; // only non-addressed

	case 0x00:	// [0x00] Lock Block
	case 0x01:	// [0x01] Change Quiet Bit
	case 0x03:	// [0x03] Change EAS
	case 0x05:	// [0x05] Calibrate
	case 0x06:	// [0x06] Change Config
	case 0x07:	// [0x07] Read Protect
	case 0x08:	// [0x08] Reset Read Protect
	//case 0x10:	// [0x10] Untraceable
	//case 0x11:	// [0x11] Authenticate
	case 0x12:	// [0x12] Write Key
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_MODE_EPC_LF, &bUidLF));
			if(bUidLF)	// variable UID length
			{
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_EPC_LEN, &ucUidLen));
			}
			else
			{
				ucUidLen = 8;
			}

			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_EPC, &ucUid[0], (int)ucUidLen));
			FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], (int)ucUidLen, sUid));
			
			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
			if(iIdx < 0)
				return iIdx;

			// only for UHF EPC Class1 Gen2
			if(	m_ISOTable[iIdx]->m_ucTrType == FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2 ||
				m_ISOTable[iIdx]->m_ucTrType == FEDM_ISC_TR_TYPE_ISO18000_3M3)
			{
				// check, if Reader is configured for UID = EPC + TID
				if(m_ISOTable[iIdx]->m_ucIDDT == 0x02)
				{
					// check, if response contains UID = EPC + TID
					ucEpcLen = ((m_ISOTable[iIdx]->m_ucClass1Gen2_PC[0] & 0xF8) >> 3) * 2;
					if(m_ISOTable[iIdx]->m_ucClass1Gen2_PC[0] & 0x02) // extended PC W1
						ucEpcLen -= 2;

					if( ucEpcLen < ucUidLen)
					{
						// take TID as UID for next request
						if(bUidLF || ((ucUidLen - ucEpcLen) != 8))
							ucData[iCnt++] = (ucUidLen - ucEpcLen);
						
						memcpy(&ucData[iCnt], &ucUid[ucEpcLen], ucUidLen - ucEpcLen);
						iCnt += (ucUidLen - ucEpcLen);
					}
					else
					{
						// error in the Reader response: TID is missing
						FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_INVENTORY_NO_TID_IN_UID));
						return FEDM_ERROR_INVENTORY_NO_TID_IN_UID;
					}
				}
				else
				{
					if(bUidLF || (ucUidLen != 8))
						ucData[iCnt++] = ucUidLen;
					
					// UID = unified identifier = serial number
					FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_EPC, &ucData[iCnt], ucUidLen));
					iCnt += ucUidLen;
				}
			}
			else
			{
				if(bUidLF)	// variable UID length
				{
					ucData[iCnt++] = ucUidLen;
					// UID = unified identifier = serial number
					FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_EPC, &ucData[iCnt], ucUidLen));
					iCnt += ucUidLen;
				}
				else
				{
					// UID = unified identifier = serial number
					FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_EPC, &ucData[iCnt], 8));
					iCnt += 8;
				}
			}
		}
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_PARAMETER));
		return FEDM_ERROR_PARAMETER;
	}

	switch(ucCmdByte)
	{
	case 0x00:	// [0x00] Lock Block
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, &bExtAdr));
		if(bExtAdr)
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_BANK, &ucData[iCnt++]));
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_BANK_ACCESS_FLAG, &bAccessPw));
			if(bAccessPw)
			{
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_ACCESS_PW_LENGTH, &ucLen));
				ucData[iCnt++] = ucLen;
				if(ucLen > 0)
				{
					FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_ACCESS_PW, &ucData[iCnt], ucLen));
					iCnt += ucLen;
				}
			}
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_DB_ADR_EXT, &ucData[iCnt], 2));
			iCnt += 2;
		}
		else
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_DB_ADR, &ucData[iCnt++]));
		}

		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_DBN, &ucData[iCnt++]));
		break;

	case 0x01:	// [0x01] Change Quiet Bit
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, &bExtAdr));
		if(bExtAdr)
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_BANK, &ucData[iCnt++]));
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_BANK_ACCESS_FLAG, &bAccessPw));
			if(bAccessPw)
			{
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_ACCESS_PW_LENGTH, &ucLen));
				ucData[iCnt++] = ucLen;
				if(ucLen > 0)
				{
					FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_ACCESS_PW, &ucData[iCnt], ucLen));
					iCnt += ucLen;
				}
			}
		}
		else
		{
		}

		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_NXP_QUIET_BIT, &ucData[iCnt++]));
		break;

	case 0x03:	// [0x03] Change EAS
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, &bExtAdr));
		if(bExtAdr)
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_BANK, &ucData[iCnt++]));
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_BANK_ACCESS_FLAG, &bAccessPw));
			if(bAccessPw)
			{
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_ACCESS_PW_LENGTH, &ucLen));
				ucData[iCnt++] = ucLen;
				if(ucLen > 0)
				{
					FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_ACCESS_PW, &ucData[iCnt], ucLen));
					iCnt += ucLen;
				}
			}
		}
		else
		{
		}

		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_NXP_EAS, &ucData[iCnt++]));
		break;

	case 0x04:	// [0x04] EAS Alarm
		break;

	case 0x05:	// [0x05] Calibrate
	case 0x07:	// [0x07] Read Protect
	case 0x08:	// [0x08] Reset Read Protect
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, &bExtAdr));
		if(bExtAdr)
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_BANK, &ucData[iCnt++]));
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_BANK_ACCESS_FLAG, &bAccessPw));
			if(bAccessPw)
			{
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_ACCESS_PW_LENGTH, &ucLen));
				ucData[iCnt++] = ucLen;
				if(ucLen > 0)
				{
					FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_ACCESS_PW, &ucData[iCnt], ucLen));
					iCnt += ucLen;
				}
			}
		}
		break;

	case 0x06:	// [0x06] Change Config
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, &bExtAdr));
		if(bExtAdr)
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_BANK, &ucData[iCnt++]));
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_BANK_ACCESS_FLAG, &bAccessPw));
			if(bAccessPw)
			{
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_ACCESS_PW_LENGTH, &ucLen));
				ucData[iCnt++] = ucLen;
				if(ucLen > 0)
				{
					FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_ACCESS_PW, &ucData[iCnt], ucLen));
					iCnt += ucLen;
				}
			}
		}

		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_NXP_CONFIG_WORD, &ucData[iCnt], 2));
		iCnt += 2;
		break;

/*	case 0x10:	// [0x10] Untraceable
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, &bExtAdr));
		if(bExtAdr)
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_BANK, &ucData[iCnt++]));
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_BANK_ACCESS_FLAG, &bAccessPw));
			if(bAccessPw)
			{
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_ACCESS_PW_LENGTH, &ucLen));
				ucData[iCnt++] = ucLen;
				if(ucLen > 0)
				{
					FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_ACCESS_PW, &ucData[iCnt], ucLen));
					iCnt += ucLen;
				}
			}
		}

		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_NXP_UFLAG, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_NXP_HIDE, &ucData[iCnt], 2));
		iCnt += 2;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_NXP_RANGE, &ucData[iCnt++]));
		break;

	case 0x11:	// [0x11] Authenticate
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, &bExtAdr));
		if(bExtAdr)
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_BANK, &ucData[iCnt++]));
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_BANK_ACCESS_FLAG, &bAccessPw));
			if(bAccessPw)
			{
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_ACCESS_PW_LENGTH, &ucLen));
				ucData[iCnt++] = ucLen;
				if(ucLen > 0)
				{
					FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_ACCESS_PW, &ucData[iCnt], ucLen));
					iCnt += ucLen;
				}
			}
		}

		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_NXP_RESP_FLAG, &ucData[iCnt++]));
		iCnt++; // reserved byte
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_NXP_TAM, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_NXP_KEY_ID, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_NXP_PROFILE, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_NXP_OFFSET, &ucData[iCnt], 2));
		iCnt += 2;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_NXP_BLOCK_COUNT, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_NXP_PROT_MODE, &ucData[iCnt++]));
		break;*/

	case 0x12:	// [0x12] Write Key
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, &bExtAdr));
		if(bExtAdr)
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_BANK, &ucData[iCnt++]));
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_BANK_ACCESS_FLAG, &bAccessPw));
			if(bAccessPw)
			{
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_ACCESS_PW_LENGTH, &ucLen));
				ucData[iCnt++] = ucLen;
				if(ucLen > 0)
				{
					FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_ACCESS_PW, &ucData[iCnt], ucLen));
					iCnt += ucLen;
				}
			}
		}

		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_NXP_KEY_TYPE, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_NXP_KEY, &ucData[iCnt], 16));
		iCnt += 16;
		break;
	}

	return iCnt;
}

/***************************************************************************
  Begin        		:	09.12.2014 / M. Hultsch
  Version       	:	04.06.10 / 09.12.2014 / M. Hultsch

  Function			:	collects the necessary data for the next [0xB4] epc uhf protocol

  Parameters		:	unsigned char ucCmdByte	-	epc command byte
						unsigned char* ucData	-	pointer to buffer for data

  Return value		:	iCnt (>0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::Get_0xB4_IsoTableData_EM(unsigned char ucCmdByte, unsigned char* ucData)
{
	FEDM_CHK3(ucData);

	bool bUidLF = false;
	int iErr = 0;
	int iCnt = 0;
	int iIdx = 0;
	unsigned char ucUidLen	= 0x00;
	unsigned char ucEpcLen	= 0x00;
	unsigned char ucCmdSize	= 0x00;
	unsigned char ucUid[FEDM_ISC_MAX_UID_LENGTH];
	string sUid;

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_CMD, &ucData[iCnt++]));
	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_MODE, &ucData[iCnt++]));

	switch(ucCmdByte)
	{
	case 0x00:	// [0x00] GetUid
	case 0x01:	// [0x01] Get Sensor Data
	case 0x02:	// [0x02] Send SPI Command
	case 0x04:	// [0x04] Reset Alarms
	case 0xD1:	// [0xD1] Broadcast Sync
		if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_MODE_EPC_LF, &bUidLF));
			if(bUidLF)	// variable UID length
			{
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_EPC_LEN, &ucUidLen));
			}
			else
			{
				ucUidLen = 8;
			}

			// UID = unified identifier = serial number
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_EPC, &ucUid[0], (int)ucUidLen));
			FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], (int)ucUidLen, sUid));
			
			iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
			if(iIdx < 0)
				return iIdx;

			// only for UHF EPC Class1 Gen2
			if(	m_ISOTable[iIdx]->m_ucTrType == FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2 ||
				m_ISOTable[iIdx]->m_ucTrType == FEDM_ISC_TR_TYPE_ISO18000_3M3)
			{
				// check, if Reader is configured for UID = EPC + TID
				if(m_ISOTable[iIdx]->m_ucIDDT == 0x02)
				{
					// check, if response contains UID = EPC + TID
					ucEpcLen = ((m_ISOTable[iIdx]->m_ucClass1Gen2_PC[0] & 0xF8) >> 3) * 2;
					if(m_ISOTable[iIdx]->m_ucClass1Gen2_PC[0] & 0x02) // extended PC W1
						ucEpcLen -= 2;

					if( ucEpcLen < ucUidLen)
					{
						// take TID as UID for next request
						if(bUidLF || ((ucUidLen - ucEpcLen) != 8))
							ucData[iCnt++] = (ucUidLen - ucEpcLen);
						
						memcpy(&ucData[iCnt], &ucUid[ucEpcLen], ucUidLen - ucEpcLen);
						iCnt += (ucUidLen - ucEpcLen);
					}
					else
					{
						// error in the Reader response: TID is missing
						FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_INVENTORY_NO_TID_IN_UID));
						return FEDM_ERROR_INVENTORY_NO_TID_IN_UID;
					}
				}
				else
				{
					if(bUidLF || (ucUidLen != 8))
						ucData[iCnt++] = ucUidLen;
					
					// UID = unified identifier = serial number
					FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_EPC, &ucData[iCnt], ucUidLen));
					iCnt += ucUidLen;
				}
			}
			else
			{
				if(bUidLF)	// variable UID length
				{
					ucData[iCnt++] = ucUidLen;
					// UID = unified identifier = serial number
					FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_EPC, &ucData[iCnt], ucUidLen));
					iCnt += ucUidLen;
				}
				else
				{
					// UID = unified identifier = serial number
					FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_EPC, &ucData[iCnt], 8));
					iCnt += 8;
				}
			}
		}
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_PARAMETER));
		return FEDM_ERROR_PARAMETER;
	}

	switch(ucCmdByte)
	{
	case 0x00:	// [0x00] GetUID
		break;
	case 0x01:	// [0x01] Get Sensor Data
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_EM_OPTION, &ucData[iCnt++]));
		break;
	case 0x02:	// [0x02] Send SPI Command
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_EM_SPI_CMD_SIZE, &ucCmdSize));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_EM_SPI_CMD_SIZE, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_EM_SPI_RESP_SIZE, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_EM_SPI_SCLK, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_EM_SPI_DELAY_TIME, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_EM_SPI_CMD, &ucData[iCnt], 8)); // get 8 bytes
		if(ucCmdSize == 0)
		{
			iCnt += 8;
		}
		else
		{
			memset(&ucData[iCnt+ucCmdSize], 8-ucCmdSize, 0); // clear unused bytes
			iCnt += (int)ucCmdSize;
		}
		break;
	case 0x04:	// [0x04] Reset Alarms
		break;
	case 0xD1:	// [0xD1] Broadcast Sync
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_EM_UTC_TIME, &ucData[iCnt], 4));
		iCnt += 4;
		break;
	}

	return iCnt;
}

/***************************************************************************
  Begin        		:	03.03.01 / 08.04.2011 / M. Hultsch
  Version       	:	04.06.06 / 23.09.2013 / M. Hultsch
						- only for EPC Class1 Gen2:
						  bugfix for addressed Host commands, if Reader is configured for UID = EPC + TID

						04.02.00 / 11.05.2012 / M. Hultsch
						- if Reader is configured for UID = EPC + TID, use IDDT from last Inventory to evaluate UID

						04.00.00 / 29.07.2011 / M. Hultsch

  Function			:	collects the necessary data for the next [0xB4] epc uhf protocol

  Parameters		:	unsigned char ucCmdByte	-	epc command byte
						unsigned char* ucData	-	pointer to buffer for data

  Return value		:	iCnt (>0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::Get_0xB4_IsoTableData_IDS(unsigned char ucCmdByte, unsigned char* ucData)
{
	FEDM_CHK3(ucData);

	bool bUidLF = false;
	bool bExtAdr = false;
	bool bAccessPw = false;
	int iErr = 0;
	int iCnt = 0;
	int iIdx = 0;
	unsigned char ucUidLen	= 0x00;
	unsigned char ucEpcLen	= 0x00;
	unsigned char ucLen		= 0x00;
	unsigned char ucSubCommand = 0x00;
	unsigned char ucNoOfBytes = 0x00;
	unsigned char ucUid[FEDM_ISC_MAX_UID_LENGTH];
	string sUid;

	switch(ucCmdByte)
	{
	case 0xA0:
	case 0xA1:
	case 0xA2:
	case 0xA3:
	case 0xA4:
	case 0xA5:
	case 0xA6:
	case 0xA7:
	case 0xA8:
	case 0xA9:
	case 0xAA:
	case 0xAB:
	case 0xAC:
	case 0xAD:
	case 0xAE:
	case 0xAF:
		break;
	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_PARAMETER));
		return FEDM_ERROR_PARAMETER;
	}

	ucData[iCnt++] = ucCmdByte;
	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_MODE, &ucData[iCnt++]));
	if((unsigned char)(ucData[1]&0x07) == FEDM_ISC_ISO_MODE_ADR)	// addressed
	{
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_MODE_EPC_LF, &bUidLF));
		if(bUidLF)	// variable UID length
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_EPC_LEN, &ucUidLen));
		}
		else
		{
			ucUidLen = 8;
		}

		// UID = unified identifier = serial number
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_EPC, &ucUid[0], (int)ucUidLen));
		FEDM_CHK1(iErr, FEDM_ConvHexUCharToHexStr(&ucUid[0], (int)ucUidLen, sUid));
		
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_SNR, sUid);
		if(iIdx < 0)
			return iIdx;

		// only for UHF EPC Class1 Gen2
		if(	m_ISOTable[iIdx]->m_ucTrType == FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2 ||
			m_ISOTable[iIdx]->m_ucTrType == FEDM_ISC_TR_TYPE_ISO18000_3M3)
		{
			// check, if Reader is configured for UID = EPC + TID
			if(m_ISOTable[iIdx]->m_ucIDDT == 0x02)
			{
				// check, if response contains UID = EPC + TID
				ucEpcLen = ((m_ISOTable[iIdx]->m_ucClass1Gen2_PC[0] & 0xF8) >> 3) * 2;
				if(m_ISOTable[iIdx]->m_ucClass1Gen2_PC[0] & 0x02) // extended PC W1
					ucEpcLen -= 2;

				if( ucEpcLen < ucUidLen)
				{
					// take TID as UID for next request
					if(bUidLF || ((ucUidLen - ucEpcLen) != 8))
						ucData[iCnt++] = (ucUidLen - ucEpcLen);
					
					memcpy(&ucData[iCnt], &ucUid[ucEpcLen], ucUidLen - ucEpcLen);
					iCnt += (ucUidLen - ucEpcLen);
				}
				else
				{
					// error in the Reader response: TID is missing
					FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_INVENTORY_NO_TID_IN_UID));
					return FEDM_ERROR_INVENTORY_NO_TID_IN_UID;
				}
			}
			else
			{
				if(bUidLF || (ucUidLen != 8))
					ucData[iCnt++] = ucUidLen;
				
				// UID = unified identifier = serial number
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_EPC, &ucData[iCnt], ucUidLen));
				iCnt += ucUidLen;
			}
		}
		else
		{
			if(bUidLF)	// variable UID length
			{
				ucData[iCnt++] = ucUidLen;
				// UID = unified identifier = serial number
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_EPC, &ucData[iCnt], ucUidLen));
				iCnt += ucUidLen;
			}
			else
			{
				// UID = unified identifier = serial number
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_EPC, &ucData[iCnt], 8));
				iCnt += 8;
			}
		}
	}

	FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_MODE_EXT_ADR, &bExtAdr));
	if(bExtAdr)
	{
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_BANK, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_REQ_BANK_ACCESS_FLAG, &bAccessPw));
		if(bAccessPw)
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_ACCESS_PW_LENGTH, &ucLen));
			ucData[iCnt++] = ucLen;
			if(ucLen > 0)
			{
				FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_B4_ACCESS_PW, &ucData[iCnt], ucLen));
				iCnt += ucLen;
			}
		}
	}


	switch(ucCmdByte)
	{
	case 0xA0:
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_0xB4_REQ_IDS_PASSWORD_LEVEL, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_0xB4_REQ_IDS_PASSWORD, &ucData[iCnt], 4));
		iCnt += 4;
		break;

	case 0xA1:
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_0xB4_REQ_IDS_LOG_MODE, &ucData[iCnt], 3));
		iCnt += 3;
		break;

	case 0xA2:
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_0xB4_REQ_IDS_LOG_LIMITS, &ucData[iCnt], 5));
		iCnt += 5;
		break;

	case 0xA3:
		break;

	case 0xA4:
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_0xB4_REQ_IDS_SFE_PARA, &ucData[iCnt], 2));
		iCnt += 2;
		break;

	case 0xA5:
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_0xB4_REQ_IDS_CAL_DATA, &ucData[iCnt], 7));
		iCnt += 7;
		break;

	case 0xA6:
		break;

	case 0xA7:
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_0xB4_REQ_IDS_START_TIME, &ucData[iCnt], 4));
		iCnt += 4;
		break;

	case 0xA8:
	case 0xA9:
		break;

	case 0xAA:
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_0xB4_REQ_IDS_BATTERY_RETRIGGER, &ucData[iCnt++]));
		break;

	case 0xAB:
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_0xB4_REQ_IDS_SL_BLOCK_0, &ucData[iCnt], 4));
		iCnt += 4;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_0xB4_REQ_IDS_SL_BLOCK_1, &ucData[iCnt], 4));
		iCnt += 4;
		break;

	case 0xAC:
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_0xB4_REQ_IDS_DELAY_TIME, &ucData[iCnt], 2));
		iCnt += 2;
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_0xB4_REQ_IDS_APP_DATA, &ucData[iCnt], 2));
		iCnt += 2;
		break;

	case 0xAD:
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_0xB4_REQ_IDS_SENSOR_VALUE, &ucData[iCnt++]));
		break;

	case 0xAE:
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_0xB4_REQ_IDS_PASSWORD_LEVEL, &ucData[iCnt++]));
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_0xB4_REQ_IDS_PASSWORD, &ucData[iCnt], 4));
		iCnt += 4;
		break;

	case 0xAF:
		FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_0xB4_REQ_IDS_SUB_COMMAND, &ucData[iCnt]));
		ucSubCommand = ((ucData[iCnt] & 0xE0) >> 5);
		ucNoOfBytes = (ucData[iCnt] & 0x07);
		iCnt++;
		if(ucSubCommand == 0x05) // write to FIFO
		{
			FEDM_CHK1(iErr, GetData(FEDM_ISC_TMP_0xB4_REQ_IDS_PAYLOAD, &ucData[iCnt], ucNoOfBytes));
			iCnt += ucNoOfBytes;
		}
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(CmdByte=0x" << std::hex << std::uppercase << (int)ucCmdByte << ") " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	}

	return iCnt;
}

/***************************************************************************
  Begin        		:	09.02.2009 / M. Hultsch
  Version       	:	03.00.06 / 02.04.2009 / M. Hultsch

  Function			:	collects the necessary data for the next  [0xC1] iso protocol

  Parameters		:	unsigned char  ucDESFireCmd	-	sub command
						unsigned char* ucData		-	pointer to buffer with data

  Return value		:	iCnt (>0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::Get_0xC1_IsoTableData(unsigned char ucDESFireCmd, unsigned char* ucData)
{
	FEDM_CHK3(ucData);

	int iErr = 0;
	int iCnt = 0;
	int	iIdx = 0;

	unsigned char ucKeyLen = 0;
	unsigned int uiFileOffset = 0;
	unsigned int uiFileLen = 0;
	unsigned int uiRecordOffset = 0;
	unsigned int uiNoOfRecords = 0;
	unsigned int uiRecordSize = 0;
	unsigned int uiValue = 0;

	switch(ucDESFireCmd)
	{
	case 0xFA:	// DESFire Authent
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_KEY_NO, &ucData[iCnt++]));
		break;

	case 0xBD:	// DESFire Read Standard Data
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_KEY_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_COMM_SETTINGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_OFFSET, &uiFileOffset));
		ucData[iCnt++] = (unsigned char) (uiFileOffset & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiFileOffset & 0x0000FF00) >> 8);
		ucData[iCnt++] = (unsigned char)((uiFileOffset & 0x00FF0000) >> 16);
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_DATA_LEN, &uiFileLen));
		ucData[iCnt++] = (unsigned char) (uiFileLen & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiFileLen & 0x0000FF00) >> 8);
		ucData[iCnt++] = (unsigned char)((uiFileLen & 0x00FF0000) >> 16);
		break;

	case 0x3D:	// DESFire Write Standard Data
		// command only in selected mode
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
		if(iIdx < 0)
			return iIdx;

		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_KEY_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_COMM_SETTINGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_OFFSET, &uiFileOffset));
		ucData[iCnt++] = (unsigned char) (uiFileOffset & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiFileOffset & 0x0000FF00) >> 8);
		ucData[iCnt++] = (unsigned char)((uiFileOffset & 0x00FF0000) >> 16);
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_DATA_LEN, &uiFileLen));
		ucData[iCnt++] = (unsigned char) (uiFileLen & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiFileLen & 0x0000FF00) >> 8);
		ucData[iCnt++] = (unsigned char)((uiFileLen & 0x00FF0000) >> 16);

		if(uiFileOffset + uiFileLen > FEDM_ISC_ISO_TABLE_RxDB_SIZE)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(DESFireCmd=0x" << std::hex << std::uppercase << (int)ucDESFireCmd << ") " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
			return FEDM_ERROR_ARRAY_BOUNDARY;
		}

		memcpy(&ucData[iCnt], &m_ISOTable[iIdx]->m_ucTxDB[uiFileOffset], uiFileLen);
		iCnt += uiFileLen;

		break;

	case 0x6C:	// DESFire Get Value
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_KEY_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_COMM_SETTINGS, &ucData[iCnt++]));
		break;

	case 0x0C:	// DESFire Credit
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_KEY_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_COMM_SETTINGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_CREDIT_VALUE, &uiValue));
		ucData[iCnt++] = (unsigned char) (uiValue & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiValue & 0x0000FF00) >> 8);
		ucData[iCnt++] = (unsigned char)((uiValue & 0x00FF0000) >> 16);
		ucData[iCnt++] = (unsigned char)((uiValue & 0xFF000000) >> 24);
		break;

	case 0xDC:	// DESFire Debit
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_KEY_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_COMM_SETTINGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_DEBIT_VALUE, &uiValue));
		ucData[iCnt++] = (unsigned char) (uiValue & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiValue & 0x0000FF00) >> 8);
		ucData[iCnt++] = (unsigned char)((uiValue & 0x00FF0000) >> 16);
		ucData[iCnt++] = (unsigned char)((uiValue & 0xFF000000) >> 24);
		break;

	case 0x1C:	// DESFire Limited Credit
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_KEY_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_COMM_SETTINGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_CREDIT_VALUE, &uiValue));
		ucData[iCnt++] = (unsigned char) (uiValue & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiValue & 0x0000FF00) >> 8);
		ucData[iCnt++] = (unsigned char)((uiValue & 0x00FF0000) >> 16);
		ucData[iCnt++] = (unsigned char)((uiValue & 0xFF000000) >> 24);
		break;

	case 0x3B:	// DESFire Write Record
		// command only in selected mode
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
		if(iIdx < 0)
			return iIdx;

		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_KEY_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_COMM_SETTINGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_OFFSET, &uiFileOffset));
		ucData[iCnt++] = (unsigned char) (uiFileOffset & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiFileOffset & 0x0000FF00) >> 8);
		ucData[iCnt++] = (unsigned char)((uiFileOffset & 0x00FF0000) >> 16);
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_DATA_LEN, &uiFileLen));
		ucData[iCnt++] = (unsigned char) (uiFileLen & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiFileLen & 0x0000FF00) >> 8);
		ucData[iCnt++] = (unsigned char)((uiFileLen & 0x00FF0000) >> 16);

		if(uiFileOffset + uiFileLen > FEDM_ISC_ISO_TABLE_RxDB_SIZE)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(DESFireCmd=0x" << std::hex << std::uppercase << (int)ucDESFireCmd << ") " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
			return FEDM_ERROR_ARRAY_BOUNDARY;
		}

		memcpy(&ucData[iCnt], &m_ISOTable[iIdx]->m_ucTxDB[uiFileOffset], uiFileLen);
		iCnt += uiFileLen;

		break;

	case 0xBB:	// DESFire Read Records
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_KEY_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_COMM_SETTINGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_RECORD_OFFSET, &uiRecordOffset));
		ucData[iCnt++] = (unsigned char) (uiRecordOffset & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiRecordOffset & 0x0000FF00) >> 8);
		ucData[iCnt++] = (unsigned char)((uiRecordOffset & 0x00FF0000) >> 16);
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_NO_OF_RECORDS, &uiNoOfRecords));
		ucData[iCnt++] = (unsigned char) (uiNoOfRecords & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiNoOfRecords & 0x0000FF00) >> 8);
		ucData[iCnt++] = (unsigned char)((uiNoOfRecords & 0x00FF0000) >> 16);
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_RECORD_SIZE, &uiRecordSize));
		ucData[iCnt++] = (unsigned char) (uiRecordSize & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiRecordSize & 0x0000FF00) >> 8);
		ucData[iCnt++] = (unsigned char)((uiRecordSize & 0x00FF0000) >> 16);
		break;

	case 0xEB:	// DESFire Clear Record File
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_KEY_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_COMM_SETTINGS, &ucData[iCnt++]));
		break;

	case 0x5F:	// DESFire Change File Settings
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_KEY_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_COMM_SETTINGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_FILE_ACCESS_RIGHTS, &ucData[iCnt], 2));
		iCnt += 2;
		break;

	case 0x54:	// DESFire Change Key Settings
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_NEW_KEY_SETTINGS, &ucData[iCnt++]));
		break;

	case 0xC4:	// DESFire Change Key
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_KEY_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_KEY_NO_TB_CHANGED, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_KEY_LEN, &ucKeyLen));
		if(ucKeyLen > 24)
		{
			return FEDM_ERROR_ARRAY_BOUNDARY;
		}
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_KEY_LEN, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_OLD_KEY, &ucData[iCnt], 24));
		iCnt += 24;
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_NEW_KEY, &ucData[iCnt], 24));
		iCnt += 24;
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC1_NEW_KEY_VERSION, &ucData[iCnt++]));
		break;
	}

	return iCnt;
}


/***************************************************************************
  Begin        		:	08.09.2010 / M. Hultsch
  Version       	:	03.02.05 / 08.09.2010 / M. Hultsch

  Function			:	collects the necessary data for the next  [0xC3] DESFire Command

  Parameters		:	unsigned char  ucDESFireCmd	-	sub command
						unsigned char* ucData		-	pointer to buffer with data

  Return value		:	iCnt (>0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::Get_0xC3_IsoTableData(unsigned char ucDESFireCmd, unsigned char ucMode, unsigned char* ucData)
{
	FEDM_CHK3(ucData);

	int iErr = 0;
	int iCnt = 0;
	int	iIdx = 0;

	unsigned char ucBuffer[32];
	unsigned char ucFlags = 0;
	unsigned char ucLen = 0;
	unsigned char ucKeyLen = 0;
	unsigned int uiFileOffset = 0;
	unsigned int uiFileLen = 0;
	unsigned int uiRecordOffset = 0;
	unsigned int uiNoOfRecords = 0;
	unsigned int uiRecordSize = 0;
	unsigned int uiValue = 0;

	// command only in selected mode
	iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
	if(iIdx < 0)
		return iIdx;

	if(ucMode & 0x01)
	{
		// SAM flag is set
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_SLOT_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_TIMEOUT, &ucData[iCnt++]));
	}


	switch(ucDESFireCmd)
	{
	case 0xFA:	// DESFire Authent
		if( (ucMode & 0x01) == 0)
		{
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_KEY_INDEX, &ucData[iCnt++]));
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_KEY_NO, &ucData[iCnt++]));
		}
		else
		{
			// SAM flag is set
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_DESFIRE_AUTH_MODE, &ucData[iCnt++]));
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_KEY_NO, &ucData[iCnt++]));
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_SAM_AUTH_MODE, &ucData[iCnt++]));
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_SAM_KEY_NO, &ucData[iCnt++]));
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_SAM_KEY_VERSION, &ucData[iCnt++]));
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_SAM_DIV_INPUT_LEN, &ucData[iCnt++]));
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_SAM_DIV_INPUT_LEN, &ucLen));
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_SAM_DIV_INPUT, ucBuffer, 16));
			memcpy(&ucData[iCnt], ucBuffer, ucLen);
			iCnt += ucLen;
		}
		break;

	case 0x54:	// DESFire Change Key Settings
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_NEW_KEY_SETTINGS, &ucData[iCnt++]));
		break;

	case 0x45:	// DESFire Get Key Settings
		break;

	case 0xC4:	// DESFire Change Key
		if( (ucMode & 0x01) == 0)
		{
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_KEY_NO_TB_CHANGED, &ucData[iCnt++]));
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_KEY_LEN, &ucKeyLen));
			if(ucKeyLen > 24)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(DESFireCmd=0x" << std::hex << std::uppercase << (int)ucDESFireCmd << ", Mode=0x" << std::hex << std::uppercase << (int)ucMode << ") " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
				return FEDM_ERROR_ARRAY_BOUNDARY;
			}
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_KEY_LEN, &ucData[iCnt++]));
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_OLD_KEY, &ucData[iCnt], 24));
			iCnt += 24;
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_NEW_KEY, &ucData[iCnt], 24));
			iCnt += 24;
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_NEW_KEY_VERSION, &ucData[iCnt++]));
		}
		else
		{
			// SAM flag is set
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_SAM_KEY_COMP_METHOD, &ucData[iCnt++]));
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_SAM_CFG, &ucData[iCnt++]));
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_SAM_KEY_NO_CURRENT_KEY, &ucData[iCnt++]));
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_SAM_KEY_VERSION_CURRENT_KEY, &ucData[iCnt++]));
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_SAM_KEY_NO_NEW_KEY, &ucData[iCnt++]));
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_SAM_KEY_VERSION_NEW_KEY, &ucData[iCnt++]));
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_SAM_DIV_INPUT_LEN, &ucData[iCnt++]));
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_SAM_DIV_INPUT_LEN, &ucLen));
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_SAM_DIV_INPUT, ucBuffer, 16));
			memcpy(&ucData[iCnt], ucBuffer, ucLen);
			iCnt += ucLen;
		}
		break;

	case 0x64: // Get Key Version
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_KEY_NO, &ucData[iCnt++]));
		break;

	case 0xCA: // Create Application
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_PICC_LEVEL_FLAGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_PICC_LEVEL_FLAGS, &ucFlags));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_APP_ID, &ucData[iCnt], 3));
		iCnt += 3;
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_KEY_SETTINGS1, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_KEY_SETTINGS2, &ucData[iCnt++]));
/*		if( (ucFlags & 0x03) == 0x03)
		{
			return FEDM_ERROR_PARAMETER;
		}
		else*/ if( (ucFlags & 0x01) )
		{
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_ISO7816_FILE_ID, &ucData[iCnt], 2));
			iCnt += 2;
		}
		/*else*/ if( (ucFlags & 0x02) )
		{
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_ISO7816_DF_NAME_LEN, &ucData[iCnt++]));
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_ISO7816_DF_NAME_LEN, &ucLen));
			if(ucLen > 16)
			{
				FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(DESFireCmd=0x" << std::hex << std::uppercase << (int)ucDESFireCmd << ", Mode=0x" << std::hex << std::uppercase << (int)ucMode << ") " << GetErrorText(FEDM_ERROR_PARAMETER));
				return FEDM_ERROR_PARAMETER;
			}
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_ISO7816_DF_NAME, &ucData[iCnt], ucLen));
			iCnt += ucLen;
		}
		break;

	case 0xDA: // Delete Application
	case 0x5A: // Select Application
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_PICC_LEVEL_FLAGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_APP_ID, &ucData[iCnt], 3));
		iCnt += 3;
		break;

	case 0x6A: // Get Application IDs
	case 0x6D: // Get DF Names
	case 0xFC: // Format PICC
	case 0x60: // Get Version
	case 0x6E: // Free Mem
	case 0x51: // Get Card UID
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_PICC_LEVEL_FLAGS, &ucData[iCnt++]));
		break;

	case 0x5C: // Set Configuration
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_PICC_LEVEL_FLAGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_0x6E_OPTION, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_0x6E_OPTION, &uiValue));
		if(uiValue == 0x00)
		{
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_0x6E_CONFIG, &ucData[iCnt++]));
		}
		else if(uiValue == 0x01)
		{
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_0x6E_DEFAULT_KEY_VERSION, &ucData[iCnt++]));
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_0x6E_DEFAULT_KEY, &ucData[iCnt], 24));
			iCnt += 24;
		}
		else if(uiValue == 0x02)
		{
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_0x6E_USER_DEF_ATS, ucBuffer, 20));
			memcpy(&ucData[iCnt], ucBuffer, ucBuffer[0]); // first byte of ATS contains length information TL
			iCnt += ucBuffer[0];
		}
		else
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(DESFireCmd=0x" << std::hex << std::uppercase << (int)ucDESFireCmd << ", Mode=0x" << std::hex << std::uppercase << (int)ucMode << ") " << GetErrorText(FEDM_ERROR_PARAMETER));
			return FEDM_ERROR_PARAMETER;
		}

		break;

	case 0x6F: // Get File IDs
	case 0x61: // Get ISO File IDs
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_APPLICATION_LEVEL_FLAGS, &ucData[iCnt++]));
		break;

	case 0xF5: // Get File Settings
	case 0xDF: // Delete File
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_APPLICATION_LEVEL_FLAGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_NO, &ucData[iCnt++]));
		break;

	case 0x5F:	// DESFire Change File Settings
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_APPLICATION_LEVEL_FLAGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_ACCESS_RIGHTS, &ucData[iCnt], 2));
		iCnt += 2;
		break;

	case 0xCD:	// DESFire Create Standard Data File
	case 0xCB:	// DESFire Create Backup Data File
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_APPLICATION_LEVEL_FLAGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_APPLICATION_LEVEL_FLAGS, &ucFlags));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_NO, &ucData[iCnt++]));
		if( (ucFlags & 0x01) )
		{
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_ISO7816_FILE_ID, &ucData[iCnt], 2));
			iCnt += 2;
		}
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_ACCESS_RIGHTS, &ucData[iCnt], 2));
		iCnt += 2;
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_SIZE, &ucData[iCnt], 3));
		iCnt += 3;
		break;

	case 0xCC:	// DESFire Create Value File
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_APPLICATION_LEVEL_FLAGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_ACCESS_RIGHTS, &ucData[iCnt], 2));
		iCnt += 2;
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_LOWER_LIMIT, &ucData[iCnt], 4));
		iCnt += 4;
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_UPPER_LIMIT, &ucData[iCnt], 4));
		iCnt += 4;
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_VALUE, &ucData[iCnt], 4));
		iCnt += 4;
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_LIMITED_CREDIT_ENABLED, &ucData[iCnt++]));
		break;

	case 0xC1:	// DESFire Create Linear Record File
	case 0xC0:	// DESFire Create Cyclic Record File
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_APPLICATION_LEVEL_FLAGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_APPLICATION_LEVEL_FLAGS, &ucFlags));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_NO, &ucData[iCnt++]));
		if( (ucFlags & 0x01) )
		{
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_ISO7816_FILE_ID, &ucData[iCnt], 2));
			iCnt += 2;
		}
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_ACCESS_RIGHTS, &ucData[iCnt], 2));
		iCnt += 2;
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_RECORD_SIZE, &ucData[iCnt], 3));
		iCnt += 3;
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_NO_OF_RECORDS, &ucData[iCnt], 3));
		iCnt += 3;
		break;

	case 0xBD:	// DESFire Read Standard Data
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_DATA_MANIPULATION_FLAGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_OFFSET, &uiFileOffset));
		ucData[iCnt++] = (unsigned char) (uiFileOffset & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiFileOffset & 0x0000FF00) >> 8);
		ucData[iCnt++] = (unsigned char)((uiFileOffset & 0x00FF0000) >> 16);
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_DATA_LEN, &uiFileLen));
		ucData[iCnt++] = (unsigned char) (uiFileLen & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiFileLen & 0x0000FF00) >> 8);
		ucData[iCnt++] = (unsigned char)((uiFileLen & 0x00FF0000) >> 16);
		break;

	case 0x3D:	// DESFire Write Standard Data
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_DATA_MANIPULATION_FLAGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_OFFSET, &uiFileOffset));
		ucData[iCnt++] = (unsigned char) (uiFileOffset & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiFileOffset & 0x0000FF00) >> 8);
		ucData[iCnt++] = (unsigned char)((uiFileOffset & 0x00FF0000) >> 16);
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_DATA_LEN, &uiFileLen));
		ucData[iCnt++] = (unsigned char) (uiFileLen & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiFileLen & 0x0000FF00) >> 8);
		ucData[iCnt++] = (unsigned char)((uiFileLen & 0x00FF0000) >> 16);

		if(uiFileOffset + uiFileLen > FEDM_ISC_ISO_TABLE_RxDB_SIZE)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(DESFireCmd=0x" << std::hex << std::uppercase << (int)ucDESFireCmd << ", Mode=0x" << std::hex << std::uppercase << (int)ucMode << ") " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
			return FEDM_ERROR_ARRAY_BOUNDARY;
		}

		memcpy(&ucData[iCnt], &m_ISOTable[iIdx]->m_ucTxDB[uiFileOffset], uiFileLen);
		iCnt += uiFileLen;

		break;

	case 0x6C:	// DESFire Get Value
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_DATA_MANIPULATION_FLAGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, &ucData[iCnt++]));
		break;

	case 0x0C:	// DESFire Credit
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_DATA_MANIPULATION_FLAGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_CREDIT_VALUE, &uiValue));
		ucData[iCnt++] = (unsigned char) (uiValue & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiValue & 0x0000FF00) >> 8);
		ucData[iCnt++] = (unsigned char)((uiValue & 0x00FF0000) >> 16);
		ucData[iCnt++] = (unsigned char)((uiValue & 0xFF000000) >> 24);
		break;

	case 0xDC:	// DESFire Debit
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_DATA_MANIPULATION_FLAGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_DEBIT_VALUE, &uiValue));
		ucData[iCnt++] = (unsigned char) (uiValue & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiValue & 0x0000FF00) >> 8);
		ucData[iCnt++] = (unsigned char)((uiValue & 0x00FF0000) >> 16);
		ucData[iCnt++] = (unsigned char)((uiValue & 0xFF000000) >> 24);
		break;

	case 0x1C:	// DESFire Limited Credit
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_DATA_MANIPULATION_FLAGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_CREDIT_VALUE, &uiValue));
		ucData[iCnt++] = (unsigned char) (uiValue & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiValue & 0x0000FF00) >> 8);
		ucData[iCnt++] = (unsigned char)((uiValue & 0x00FF0000) >> 16);
		ucData[iCnt++] = (unsigned char)((uiValue & 0xFF000000) >> 24);
		break;

	case 0x3B:	// DESFire Write Record
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_DATA_MANIPULATION_FLAGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_OFFSET, &uiFileOffset));
		ucData[iCnt++] = (unsigned char) (uiFileOffset & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiFileOffset & 0x0000FF00) >> 8);
		ucData[iCnt++] = (unsigned char)((uiFileOffset & 0x00FF0000) >> 16);
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_DATA_LEN, &uiFileLen));
		ucData[iCnt++] = (unsigned char) (uiFileLen & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiFileLen & 0x0000FF00) >> 8);
		ucData[iCnt++] = (unsigned char)((uiFileLen & 0x00FF0000) >> 16);

		if(uiFileOffset + uiFileLen > FEDM_ISC_ISO_TABLE_RxDB_SIZE)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(DESFireCmd=0x" << std::hex << std::uppercase << (int)ucDESFireCmd << ", Mode=0x" << std::hex << std::uppercase << (int)ucMode << ") " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
			return FEDM_ERROR_ARRAY_BOUNDARY;
		}

		memcpy(&ucData[iCnt], &m_ISOTable[iIdx]->m_ucTxDB[uiFileOffset], uiFileLen);
		iCnt += uiFileLen;

		break;

	case 0xBB:	// DESFire Read Records
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_DATA_MANIPULATION_FLAGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_RECORD_OFFSET, &uiRecordOffset));
		ucData[iCnt++] = (unsigned char) (uiRecordOffset & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiRecordOffset & 0x0000FF00) >> 8);
		ucData[iCnt++] = (unsigned char)((uiRecordOffset & 0x00FF0000) >> 16);
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_NO_OF_RECORDS, &uiNoOfRecords));
		ucData[iCnt++] = (unsigned char) (uiNoOfRecords & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiNoOfRecords & 0x0000FF00) >> 8);
		ucData[iCnt++] = (unsigned char)((uiNoOfRecords & 0x00FF0000) >> 16);
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_RECORD_SIZE, &uiRecordSize));
		ucData[iCnt++] = (unsigned char) (uiRecordSize & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiRecordSize & 0x0000FF00) >> 8);
		ucData[iCnt++] = (unsigned char)((uiRecordSize & 0x00FF0000) >> 16);
		break;

	case 0xEB:	// DESFire Clear Record File
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_DATA_MANIPULATION_FLAGS, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS, &ucData[iCnt++]));
		break;

	case 0xC7:	// DESFire Commit Transaction
	case 0xA7:	// DESFire Abort Transaction
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC3_DATA_MANIPULATION_FLAGS, &ucData[iCnt++]));
		break;
	}

	return iCnt;
}


/***************************************************************************
  Begin        		:	24.08.2009 / M. Hultsch
  Version       	:	03.00.13 / 30.09.2009 / M. Hultsch

  Function			:	collects the necessary data for the next  [0xC2] iso protocol

  Parameters		:	unsigned char  ucMifarePlusCmd	-	sub command
						unsigned char* ucData			-	pointer to buffer with data

  Return value		:	iCnt (>0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::Get_0xC2_IsoTableData(unsigned char ucMifarePlusCmd, unsigned char* ucData)
{
	FEDM_CHK3(ucData);

	unsigned char ucTmp = 0;
	int iErr = 0;
	int iCnt = 0;
	int iIdx = 0;
	unsigned int uiLength = 0;
	unsigned int uiBlockNo = 0;

	switch(ucMifarePlusCmd)
	{
	// commands for all security levels
	case 0x70:	// First Authent
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_AES_KEY_BLOCK_NO, &uiBlockNo));
		ucData[iCnt++] = (unsigned char) (uiBlockNo & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiBlockNo & 0x0000FF00) >> 8);
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_PCD_CAP_LEN, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_PCD_CAP_LEN, &ucTmp));
		if(ucTmp > 0)
		{
			FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_PCD_CAP, &ucData[iCnt], ucTmp));
			iCnt += ucTmp;
		}
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_KEY_INDEX, &ucData[iCnt++]));
		break;

	case 0x76:	// Following Authent
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_AES_KEY_BLOCK_NO, &uiBlockNo));
		ucData[iCnt++] = (unsigned char) (uiBlockNo & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiBlockNo & 0x0000FF00) >> 8);
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_KEY_INDEX, &ucData[iCnt++]));
		break;

	// Security Level 1 commands
	case 0x77:	// SL1 AES Authent
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_KEY_INDEX, &ucData[iCnt++]));
		break;

	// Security Level 2 commands
	case 0x75:	// SL2 AES and Crypto Authent
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_DB_ADR, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_KEY_TYPE, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_KEY_NO, &ucData[iCnt++]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_KEY_INDEX, &ucData[iCnt++]));
		break;

	case 0x38:	// SL2 Multiblock Read
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_SRC_BLOCK_NO, &uiBlockNo));
		ucData[iCnt++] = (unsigned char) (uiBlockNo & 0x000000FF); // only LSB !!
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_EXT, &ucData[iCnt++]));
		break;

	case 0xA8:	// SL2 Multiblock Write
		// commands only in selected mode
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
		if(iIdx < 0)
			return iIdx;

		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_BLOCK_NO, &uiBlockNo));
		ucData[iCnt++] = (unsigned char) (uiBlockNo & 0x000000FF); // only LSB !!
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_EXT, &uiLength));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_EXT, &ucData[iCnt++]));

		if( ((uiBlockNo + uiLength)*16) > FEDM_ISC_ISO_TABLE_TxDB_SIZE)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(MifarePlusCmd=0x" << std::hex << std::uppercase << (int)ucMifarePlusCmd << ") " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
			return FEDM_ERROR_ARRAY_BOUNDARY;
		}

		memcpy(&ucData[iCnt], &m_ISOTable[iIdx]->m_ucTxDB[uiBlockNo*16], uiLength*16);
		iCnt += uiLength*16;
		break;

	// Security Level 3 commands
	case 0x78:	// SL3 Reset Authent
		break;

	case 0x30:	// SL3 Read Encrypted
	case 0x31:	// SL3 Read Encrypted Maced
	case 0x32:	// SL3 Read Plain
	case 0x33:	// SL3 Read Plain Maced
	case 0x34:	// SL3 Read Encrypted Unmaced
	case 0x35:	// SL3 Read Encrypted Unmaced Respond Maced
	case 0x36:	// SL3 Read Plain Unmaced
	case 0x37:	// SL3 Read Plain Unmaced Respond Maced
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_BLOCK_NO, &uiBlockNo));
		ucData[iCnt++] = (unsigned char) (uiBlockNo & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiBlockNo & 0x0000FF00) >> 8);
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_EXT, &ucData[iCnt++]));
		break;

	case 0xA0:	// SL3 Write Encrypted
	case 0xA1:	// SL3 Write Encrypted Maced
	case 0xA2:	// SL3 Write Plain
	case 0xA3:	// SL3 Write Plain Maced
		// commands only in selected mode
		iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
		if(iIdx < 0)
			return iIdx;

		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_BLOCK_NO, &uiBlockNo));
		ucData[iCnt++] = (unsigned char) (uiBlockNo & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiBlockNo & 0x0000FF00) >> 8);
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_EXT, &uiLength));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_EXT, &ucData[iCnt++]));

		if( (((uiBlockNo & 0x000000FF) + uiLength)*16) > FEDM_ISC_ISO_TABLE_TxDB_SIZE)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(MifarePlusCmd=0x" << std::hex << std::uppercase << (int)ucMifarePlusCmd << ") " << GetErrorText(FEDM_ERROR_ARRAY_BOUNDARY));
			return FEDM_ERROR_ARRAY_BOUNDARY;
		}

		memcpy(&ucData[iCnt], &m_ISOTable[iIdx]->m_ucTxDB[(uiBlockNo & 0x000000FF)*16], uiLength*16);
		iCnt += uiLength*16;

		break;

	case 0xB0:	// SL3 Increment Encrypted
	case 0xB1:	// SL3 Increment Encrypted Maced
	case 0xB2:	// SL3 Decrement Encrypted
	case 0xB3:	// SL3 Decrement Encrypted Maced
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_SRC_BLOCK_NO, &uiBlockNo));
		ucData[iCnt++] = (unsigned char) (uiBlockNo & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiBlockNo & 0x0000FF00) >> 8);
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_VALUE, &ucData[iCnt], 4));
		iCnt += 4;
		break;

	case 0xB4:	// SL3 Transfer
	case 0xB5:	// SL3 Transfer Maced
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_DST_BLOCK_NO, &uiBlockNo));
		ucData[iCnt++] = (unsigned char) (uiBlockNo & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiBlockNo & 0x0000FF00) >> 8);
		break;

	case 0xB6:	// SL3 Increment Transfer Encrypted
	case 0xB7:	// SL3 Increment Transfer Encrypted Maced
	case 0xB8:	// SL3 Decrement Transfer Encrypted
	case 0xB9:	// SL3 Decrement Transfer Encrypted Maced
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_SRC_BLOCK_NO, &uiBlockNo));
		ucData[iCnt++] = (unsigned char) (uiBlockNo & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiBlockNo & 0x0000FF00) >> 8);
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_DST_BLOCK_NO, &uiBlockNo));
		ucData[iCnt++] = (unsigned char) (uiBlockNo & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiBlockNo & 0x0000FF00) >> 8);
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_VALUE, &ucData[iCnt], 4));
		iCnt += 4;
		break;

	case 0xC2:	// SL3 Restore
	case 0xC3:	// SL3 Restore Maced
		FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_0xC2_SRC_BLOCK_NO, &uiBlockNo));
		ucData[iCnt++] = (unsigned char) (uiBlockNo & 0x000000FF);
		ucData[iCnt++] = (unsigned char)((uiBlockNo & 0x0000FF00) >> 8);
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(MifarePlusCmd=0x" << std::hex << std::uppercase << (int)ucMifarePlusCmd << ") " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		return FEDM_ERROR_UNSUPPORTED;
	}

	return iCnt;
}


//////////////////////////////////////////////////////////////////////
// functions for Channel Analyze Data table
//////////////////////////////////////////////////////////////////////

#if !defined(__BORLANDC__)
int FEDM_ISCReader::SetCADTableData(unsigned char ucMode, long* nData)
{
	int iCnt = 0;
	int iByteCnt = 0;
	int iDataSets = 0;
	FEDM_ISC_STRUCT_CAD_HISTORY*	pData = NULL;

	if(ucMode & 0x01)			// mode TDM
	{
		// initializations
		m_CADTable.Init();
		m_CADTable.m_ucMode = 0x01;
		m_CADTable.m_bValid = true;

		m_CADTable.m_nData[0] = nData[0];
		m_CADTable.m_nData[1] = nData[1];
		m_CADTable.m_nData[2] = nData[2];
		m_CADTable.m_nData[3] = nData[3];
	}
	else if(ucMode & 0x04)			// mode CH_HISTORY
	{
		// build number of records
		iDataSets  = ((nData[iByteCnt++] & 0x000000FF) << 8); 
		iDataSets +=  (nData[iByteCnt++] & 0x000000FF);
		
		if(iDataSets > 516)
			FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);

		// initializations
		if(m_CADTable.m_bValid && (m_CADTable.m_ucMode != 0x04)) 
			m_CADTable.Init();

		m_CADTable.m_ucMode = 0x04;

		// allocate memory
//		m_CADTable.m_History.reserve(iDataSets);

		for(iCnt=0; iCnt<iDataSets; ++iCnt)
		{
			pData = new FEDM_ISC_STRUCT_CAD_HISTORY;
			if(pData == NULL)
				FEDM_RETURN(FEDM_ERROR_NO_MORE_MEM);

			pData->uiAlloc  = ((nData[iByteCnt++] & 0x000000FF) << 8); 
			pData->uiAlloc +=  (nData[iByteCnt++] & 0x000000FF);
			pData->ucTxChannel = (unsigned char)(nData[iByteCnt++] & 0x000000FF);
			pData->ucRxChannel = (unsigned char)(nData[iByteCnt++] & 0x000000FF);

			m_CADTable.m_History.push_back(pData);
		}

		m_CADTable.m_bValid = true;
	}
	else if(ucMode & 0x08)		// mode CH_POWER
	{
		// initializations
		m_CADTable.Init();
		m_CADTable.m_ucMode = 0x08;
		m_CADTable.m_bValid = true;

		for(iCnt=0; iCnt<15; ++iCnt)
			m_CADTable.m_ucPowerPeak[iCnt] = (unsigned char)(nData[iByteCnt++] & 0x000000FF); 

		for(iCnt=0; iCnt<15; ++iCnt)
			m_CADTable.m_ucPowerAvg[iCnt] = (unsigned char)(nData[iByteCnt++] & 0x000000FF); 
	}
	else
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	FEDM_RETURN(FEDM_OK);
}
#endif

//////////////////////////////////////////////////////////////////////
// miscellaneous functions
//////////////////////////////////////////////////////////////////////


void FEDM_ISCReader::SetLastError(int iError)
{
	if(iError == 0)
	{
		SetData(FEDM_ISC_TMP_LAST_STATE, (unsigned char)0);
		SetData(FEDM_ISC_TMP_LAST_ERROR, (unsigned int)0);
		m_iLastError = 0;
	}
	else if(iError > 0)
	{
		SetData(FEDM_ISC_TMP_LAST_STATE, (unsigned char)iError);
		SetData(FEDM_ISC_TMP_LAST_ERROR, (unsigned int)0);
		m_iLastError = 0;
	}
	else
	{
		SetData(FEDM_ISC_TMP_LAST_ERROR, (unsigned int)iError);
		SetData(FEDM_ISC_TMP_LAST_STATE, (unsigned char)0);
		m_iLastError = iError;
	}
}


/***************************************************************************
  Begin        		:	15.04.2003 / M. Hultsch
  
  Version       	:	04.05.05 / 04.11.2013 / M. Hultsch
						- new reader: ID ISC.PRH200

						04.05.03 / 30.08.2013 / M. Hultsch
						- new reader: OBID myAXXESS onTop
						- bugfix for LRU3x00: max number of CFG-pages increased from 86 to 88

						04.03.01 / 30.10.2012 / M. Hultsch
						- new reader: ID ISC.MU95

						04.02.06 / 23.07.2012 / M. Hultsch
						- new reader: ID CPR46

						04.02.05 / 05.07.2012 / M. Hultsch
						- new reader: ID CPR20

						04.01.01 / 29.03.2012 / M. Hultsch
						- set default blocksize for RFC configuration

						04.00.09 / 23.01.2012 / M. Hultsch
						- new reader: ID ISC.LR1002

						04.00.06 / 22.11.2011 / M. Hultsch
						- new nPA reader: OBID myAXXESS standard, OBID myAXXESS comfort

						04.00.02 / 09.08.2011 / M. Hultsch
						- support for OEM Reader names

						04.00.01 / 04.08.2011 / M. Hultsch
						- bugfix for LR2500-A/B and LRU3000 concerning m_iUsedEEDataBlocks, m_iUsedRAMDataBlocks

						03.03.03 / 09.05.2011 / M. Hultsch
						- parameter ReaderType moved from unsigned char to unsigned int

						03.03.01 / 07.03.2011 / M. Hultsch
						- ID ISC.MRU102 added

						03.03.00 / 01.02.2011 / M. Hultsch
						- ID ISC.LR2500-A added

						03.01.05 / 23.04.2010 / M. Hultsch
						- ID ISC.MR102 and ID ISC.LR2500-B added

						03.00.19 / 22.01.2010 / M. Hultsch
						- new FU-Type: ID ISC.ANT.GPC
						- full support for CPR50.XX

						03.00.18 / 02.12.2009 / M. Hultsch
						- ID MLC.COM and ID MLC.EMPS added

						03.00.13 / 29.09.2009 / M. Hultsch
						- ID MAX50.xx added
						- used CFG Pages increased for PR101, MR101, MR200

						03.00.08 / 09.06.2009 / M. Hultsch
						- new reader type CPR44.xx

						03.00.07 / 29.04.2009 / M. Hultsch
						- new reader type ISC.M02.M8

						03.00.06 / 15.04.2009 / M. Hultsch
						- new reader type ISC.UM02

						03.00.00 / 10.10.2008 / M. Hultsch

  Function			:	sets the reader type and reader name

  Parameters		:	unsigned char uiReaderType	-	reader type from system manual

  Return value		:	OK (=0) or error code (<0)

***************************************************************************/
int FEDM_ISCReader::SetReaderType(unsigned int uiReaderType)
{
	unsigned char ucHWType = 0;
	char cReaderType[8];
	map<unsigned int, string>::iterator itor;

	FEDM_SETPROP2(m_pLogger, FELOGMNG_PROP_READER_TYPE, uiReaderType);
	sprintf(cReaderType, "%u", uiReaderType);
	FEISC_SetReaderPara(m_iReaderHnd, "ReaderType", cReaderType);

	// set blocksize for RFC configuration to default value
	// some Readers can change this blocksize
	m_iRFC_DataBlockSize	= 16;

	if(uiReaderType & 0x00000080)
	{
		// reader class defined
		// rfu
#if defined(_FEDM_SUPPORT_READER_CLASS)
		m_uiReaderType = uiReaderType;

		switch(uiReaderType)
		{
		case FEDM_ISC_TYPE_ISCMR10X:
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_ISCMR10X);
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
			BuildAccessIdMap_Class_ISC_HF_MR1();
			break;
		}
#endif
	}
	else
	{
		// no reader class defined

		m_uiReaderType = uiReaderType;

		switch(uiReaderType)
		{
		// HF function units
		case 11: // ID ISC.DAT
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, "ID ISC.DAT");
			m_iUsedEEDataBlocks = 0;
			m_iUsedRAMDataBlocks = 0;
			break;
		case 12: // ID ISC.UMUX
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, "ID ISC.UMUX");
			m_iUsedEEDataBlocks = 0;
			m_iUsedRAMDataBlocks = 0;
			break;
		case FEDM_ISC_TYPE_ANT_GPC: // ID ISC.ANT.GPC (Gate People Counter)
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, "ID ISC.ANT.GPC");
			m_iUsedEEDataBlocks = 2;
			m_iUsedRAMDataBlocks = 2;
			break;

		// LF classic types
		case 20: // ID RW40.30
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, "ID RW40.30");
			m_iUsedEEDataBlocks = 0;
			m_iUsedRAMDataBlocks = 0;
			break;
		case FEDM_ISC_TYPE_ISCM01:
			m_bSelectWithCinf = false;
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED_READER_TYPE);
		case FEDM_ISC_TYPE_ISCM02:
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_ISCM02);
			m_iUsedEEDataBlocks = 16;
			m_iUsedRAMDataBlocks = 16;
			BuildAccessIdMap_M02();
			break;
		case 32: // ID ISC.M02.M8-B-DM
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, "ID ISC.M02.M8-B-DM");
			m_iUsedEEDataBlocks = 0;
			m_iUsedRAMDataBlocks = 0;
			break;
		case FEDM_ISC_TYPE_ISCM02_M8:
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_ISCM02_M8);
			m_iUsedEEDataBlocks = 8;
			m_iUsedRAMDataBlocks = 8;
			BuildAccessIdMap_M02();
			break;
		case FEDM_ISC_TYPE_ISCLR100:
			m_bSelectWithCinf = false;
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED_READER_TYPE);
		case FEDM_ISC_TYPE_ISCLR200:
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_ISCLR200);
			m_iUsedEEDataBlocks = 16;
			m_iUsedRAMDataBlocks = 16;
			BuildAccessIdMap_LR200();
			break;
		case FEDM_ISC_TYPE_ISCLR1002:
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_ISCLR1002);
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
			BuildAccessIdMap_LR1002();
			break;
		case FEDM_ISC_TYPE_ISCLR2000:
			m_bSelectWithCinf = false;
			GetData(FEDM_ISC_TMP_SOFTVER_HW_TYPE, &ucHWType);
			if(ucHWType & 0xC0)
			{
				unsigned int uiType = ((unsigned int)ucHWType << 24) + uiReaderType;
				itor = m_mapOemNames4ReaderVariants.find(uiType);
				if(itor != m_mapOemNames4ReaderVariants.end())
					strcpy(m_sReaderName, itor->second.c_str());
				else
					strcpy(m_sReaderName, FEDM_ISC_NAME_ISCLRM2000);
			}
			else
			{
				itor = m_mapOemNames.find(uiReaderType);
				if(itor != m_mapOemNames.end())
					strcpy(m_sReaderName, itor->second.c_str());
				else
					strcpy(m_sReaderName, FEDM_ISC_NAME_ISCLR2000);
			}
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
			BuildAccessIdMap_LR2000();
			break;
		case FEDM_ISC_TYPE_ISCLR2500_A:
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_ISCLR2500_A);
			m_iUsedEEDataBlocks = 87;
			m_iUsedRAMDataBlocks = 87;
			m_iRFC_DataBlockSize = 32;
			BuildAccessIdMap_LR2500_A();
			break;
		case FEDM_ISC_TYPE_ISCLR2500_B:
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_ISCLR2500_B);
			m_iUsedEEDataBlocks = 87;
			m_iUsedRAMDataBlocks = 87;
			m_iRFC_DataBlockSize = 32;
			BuildAccessIdMap_LR2500_B();
			break;
		case FEDM_ISC_TYPE_ISCPRH100_U:
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_ISCPRH100_U);
			m_iUsedEEDataBlocks = 16;
			m_iUsedRAMDataBlocks = 16;
			BuildAccessIdMap_MR101();
			break;
		case FEDM_ISC_TYPE_ISCPRH100:
			m_bSelectWithCinf = false;
			strcpy(m_sReaderName, FEDM_ISC_NAME_ISCPRH100);
			m_iUsedEEDataBlocks = 16;
			m_iUsedRAMDataBlocks = 16;
			BuildAccessIdMap_MR101();
			break;
		case FEDM_ISC_TYPE_ISCPRH101:
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_ISCPRH101);
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
			BuildAccessIdMap_MR101();
			break;
		case FEDM_ISC_TYPE_ISCPRH101_U:
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_ISCPRH101_U);
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
			BuildAccessIdMap_MR101();
			break;
		case FEDM_ISC_TYPE_ISCPRH102:
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_ISCPRH102);
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
			BuildAccessIdMap_PRH102();
			break;
		case FEDM_ISC_TYPE_ISCPRH200:
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_ISCPRH200);
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
			BuildAccessIdMap_PRH200();
			break;
		case FEDM_ISC_TYPE_ISCMR100_U:
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_ISCMR100_U);
			m_iUsedEEDataBlocks = 16;
			m_iUsedRAMDataBlocks = 16;
			BuildAccessIdMap_MR101();
			break;
		case FEDM_ISC_TYPE_ISCMR100:
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_ISCMR100);
			m_iUsedEEDataBlocks = 16;
			m_iUsedRAMDataBlocks = 16;
			BuildAccessIdMap_MR101();
			break;
		case FEDM_ISC_TYPE_ISCMR102:
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_ISCMR102);
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
			BuildAccessIdMap_MR102();
			break;
		case FEDM_ISC_TYPE_ISCMR200:
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_ISCMR200);
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
			BuildAccessIdMap_MR200();
			break;
		case FEDM_ISC_TYPE_ISCMR101:
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_ISCMR101);
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
			BuildAccessIdMap_MR101();
			break;
		case FEDM_ISC_TYPE_ISCMR101_U:
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_ISCMR101_U);
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
			BuildAccessIdMap_MR101();
			break;
		case FEDM_ISC_TYPE_CPRM02:
			m_bSelectWithCinf = true;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_CPRM02);
			m_iUsedEEDataBlocks = 16;
			m_iUsedRAMDataBlocks = 16;
			BuildAccessIdMap_CPR();
			break;
		case FEDM_ISC_TYPE_CPR02:
			m_bSelectWithCinf = true;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_CPR02);
			m_iUsedEEDataBlocks = 16;
			m_iUsedRAMDataBlocks = 16;
			BuildAccessIdMap_CPR();
			break;
		case FEDM_ISC_TYPE_CPR04_U:
			m_bSelectWithCinf = true;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_CPR04_U);
			m_iUsedEEDataBlocks = 16;
			m_iUsedRAMDataBlocks = 16;
			BuildAccessIdMap_CPR();
			break;
		case FEDM_ISC_TYPE_CPR20_XX:
			m_bSelectWithCinf = true;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_CPR20_XX);
			m_iUsedEEDataBlocks = 16;
			m_iUsedRAMDataBlocks = 16;
			BuildAccessIdMap_CPR20();
			break;
		case FEDM_ISC_TYPE_CPR30_XX:
			m_bSelectWithCinf = true;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_CPR30_XX);
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
			BuildAccessIdMap_CPR30();
			break;
		case FEDM_ISC_TYPE_CPR40_XX_U:
			m_bSelectWithCinf = true;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_CPR40_XX_U);
			m_iUsedEEDataBlocks = 16;
			m_iUsedRAMDataBlocks = 16;
			BuildAccessIdMap_CPR40();
			break;
		case FEDM_ISC_TYPE_CPR40_XX:
			m_bSelectWithCinf = true;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_CPR40_XX);
			m_iUsedEEDataBlocks = 16;
			m_iUsedRAMDataBlocks = 16;
			BuildAccessIdMap_CPR40();
			break;
		case FEDM_ISC_TYPE_CPR44_XX:
			m_bSelectWithCinf = true;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_CPR44_XX);
			m_iUsedEEDataBlocks = 16;
			m_iUsedRAMDataBlocks = 16;
			BuildAccessIdMap_CPR44();
			break;
		case FEDM_ISC_TYPE_CPR46_XX:
			m_bSelectWithCinf = true;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_CPR46_XX);
			m_iUsedEEDataBlocks = 16;
			m_iUsedRAMDataBlocks = 16;
			BuildAccessIdMap_CPR46();
			break;
		case FEDM_ISC_TYPE_CPR47_XX:
			m_bSelectWithCinf = true;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_CPR47_XX);
			m_iUsedEEDataBlocks = 16;
			m_iUsedRAMDataBlocks = 16;
			BuildAccessIdMap_CPR47();
			break;
		case FEDM_ISC_TYPE_CPR50_XX:
			m_bSelectWithCinf = true;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_CPR50_XX);
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
			BuildAccessIdMap_CPR50();
			break;
		case FEDM_ISC_TYPE_CPR52_XX:
			m_bSelectWithCinf = true;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_CPR52_XX);
			m_iUsedEEDataBlocks = 16;
			m_iUsedRAMDataBlocks = 16;
			BuildAccessIdMap_CPR52();
			break;
		case FEDM_ISC_TYPE_CPR60_XX:
			m_bSelectWithCinf = true;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_CPR60_XX);
			m_iUsedEEDataBlocks = 16;
			m_iUsedRAMDataBlocks = 16;
			BuildAccessIdMap_CPR60();
			break;

		// OBID myAXXESS reader types
		case FEDM_ISC_TYPE_MAX50_XX:
			m_bSelectWithCinf = true;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_MAX50_XX);
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
			BuildAccessIdMap_MAX50();
			break;
		case FEDM_ISC_TYPE_MAX_STANDARD:
			m_bSelectWithCinf = true;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_MAX_STANDARD);
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
			BuildAccessIdMap_CPR30();
			break;
		case FEDM_ISC_TYPE_MAX_COMFORT:
			m_bSelectWithCinf = true;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_MAX_COMFORT);
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
			BuildAccessIdMap_CPR30();
			break;
		case FEDM_ISC_TYPE_MAX_ONTOP:
			m_bSelectWithCinf = true;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_MAX_ONTOP);
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
			BuildAccessIdMap_MAX_ONTOP();
			break;
		case FEDM_ISC_TYPE_MAXU1002:
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_MAXU1002);
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
			BuildAccessIdMap_MAXU1002();
			break;
		
		// UHF reader types
		case FEDM_ISC_TYPE_ISCMU02:
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_ISCMU02);
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
			BuildAccessIdMap_ISCMU02();
			break;
		case FEDM_ISC_TYPE_ISCMU95:
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_ISCMU95);
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
			BuildAccessIdMap_ISCMU02();
			break;
		case FEDM_ISC_TYPE_ISCMRU102:
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_ISCMRU102);
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
			BuildAccessIdMap_MRU102();
			break;
		case FEDM_ISC_TYPE_ISCMRU200:
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_ISCMRU200);
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
			BuildAccessIdMap_MRU200();
			break;
		case FEDM_ISC_TYPE_ISCLRU1000:
			m_bSelectWithCinf = false;
			GetData(FEDM_ISC_TMP_SOFTVER_HW_TYPE, &ucHWType);
			if(ucHWType & 0xC0)
			{
				unsigned int uiType = ((unsigned int)ucHWType << 24) + uiReaderType;
				itor = m_mapOemNames4ReaderVariants.find(uiType);
				if(itor != m_mapOemNames4ReaderVariants.end())
					strcpy(m_sReaderName, itor->second.c_str());
				else
					strcpy(m_sReaderName, FEDM_ISC_NAME_ISCLRMU1000);
			}
			else
			{
				itor = m_mapOemNames.find(uiReaderType);
				if(itor != m_mapOemNames.end())
					strcpy(m_sReaderName, itor->second.c_str());
				else
					strcpy(m_sReaderName, FEDM_ISC_NAME_ISCLRU1000);
			}
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
			BuildAccessIdMap_LRU1000();
			break;
		case FEDM_ISC_TYPE_ISCLRU1002:
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_ISCLRU1002);
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
			BuildAccessIdMap_LRU1002();
			break;
		case FEDM_ISC_TYPE_ISCLRU2000:
			m_bSelectWithCinf = false;
			GetData(FEDM_ISC_TMP_SOFTVER_HW_TYPE, &ucHWType);
			if(ucHWType & 0xC0)
			{
				unsigned int uiType = ((unsigned int)ucHWType << 24) + uiReaderType;
				itor = m_mapOemNames4ReaderVariants.find(uiType);
				if(itor != m_mapOemNames4ReaderVariants.end())
					strcpy(m_sReaderName, itor->second.c_str());
				else
					strcpy(m_sReaderName, FEDM_ISC_NAME_ISCLRMU2000);
			}
			else
			{
				itor = m_mapOemNames.find(uiReaderType);
				if(itor != m_mapOemNames.end())
					strcpy(m_sReaderName, itor->second.c_str());
				else
					strcpy(m_sReaderName, FEDM_ISC_NAME_ISCLRU2000);
			}
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
			BuildAccessIdMap_LRU2000();
			break;
		case FEDM_ISC_TYPE_ISCLRU3000:
			m_bSelectWithCinf = false;
			GetData(FEDM_ISC_TMP_SOFTVER_HW_TYPE, &ucHWType);
			if(ucHWType & 0x01)
			{
				unsigned int uiType = ((unsigned int)ucHWType << 24) + uiReaderType;
				itor = m_mapOemNames4ReaderVariants.find(uiType);
				if(itor != m_mapOemNames4ReaderVariants.end())
					strcpy(m_sReaderName, itor->second.c_str());
				else
					strcpy(m_sReaderName, FEDM_ISC_NAME_ISCLRU3500);
			}
			else
			{
				itor = m_mapOemNames.find(uiReaderType);
				if(itor != m_mapOemNames.end())
					strcpy(m_sReaderName, itor->second.c_str());
				else
					strcpy(m_sReaderName, FEDM_ISC_NAME_ISCLRU3000);
			}
#ifndef FEDM_NEW_LRU3000_ACC_CFG
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
#else
			m_iUsedEEDataBlocks = 88;
			m_iUsedRAMDataBlocks = 88;
			m_iRFC_DataBlockSize = 32;
#endif
			BuildAccessIdMap_LRU3000();
			break;

		// HF/UHF dual frequency reader types
		case FEDM_ISC_TYPE_ISCPRHD102:
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_ISCPRHD102);
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
			BuildAccessIdMap_PRHD102();
			break;

		// OBID megalock C reader types
		case FEDM_ISC_TYPE_MLC_COM:	// communicator
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_MLC_COM);
			m_iUsedEEDataBlocks = 8;
			m_iUsedRAMDataBlocks = 8;
			BuildAccessIdMap_CPR();
	//		m_mapAccessID.clear(); // no configuration
			break;
		case FEDM_ISC_TYPE_MLC_EMPS: // locker
			m_bSelectWithCinf = false;
			itor = m_mapOemNames.find(uiReaderType);
			if(itor != m_mapOemNames.end())
				strcpy(m_sReaderName, itor->second.c_str());
			else
				strcpy(m_sReaderName, FEDM_ISC_NAME_MLC_EMPS);
			m_iUsedEEDataBlocks = 0;
			m_iUsedRAMDataBlocks = 0;
			m_mapAccessID.clear(); // no configuration
			break;

		default:
			m_bSelectWithCinf = false;
			m_sReaderName[0] = '\0';
			m_iUsedEEDataBlocks = 64;
			m_iUsedRAMDataBlocks = 64;
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(ReaderType=" << uiReaderType << ") " << GetErrorText(FEDM_ERROR_UNKNOWN_READER_TYPE));
			FEDM_RETURN(FEDM_ERROR_UNKNOWN_READER_TYPE);
		}
	}

	FEDM_RETURN(FEDM_OK);
}

char* FEDM_ISCReader::GetReaderName()
{
	return FEDM_DataBase::GetReaderName();
}

char* FEDM_ISCReader::GetReaderName(unsigned int uiReaderType)
{
	map<unsigned int, string>::iterator itor;

	itor = m_mapOemNames.find(uiReaderType);
	if(itor != m_mapOemNames.end())
		return (char*)itor->second.c_str();

	switch(uiReaderType)
	{
	// HF Function Units
	case 11: // ID ISC.DAT
		return "ID ISC.DAT";
	case 12: // ID ISC.UMUX
		return "ID ISC.UMUX";
	case FEDM_ISC_TYPE_ANT_GPC: // ID ISC.ANT.GPC (Gate People Counter)
		return "ID ISC.ANT.GPC";
	// LF classic reader types
	case 20:
		return "ID RW40.30-U";
	// HF reader types
	case FEDM_ISC_TYPE_ISCM01:
		break; // no more support
	case FEDM_ISC_TYPE_ISCM02:
		return FEDM_ISC_NAME_ISCM02;
	case 32: // ID ISC.M02.M8-B-DM
		return "ID ISC.M02.M8-B-DM";
	case FEDM_ISC_TYPE_ISCM02_M8:
		return FEDM_ISC_NAME_ISCM02_M8;
	case FEDM_ISC_TYPE_ISCLR100:
		break; // no more support
	case FEDM_ISC_TYPE_ISCLR200:
		return FEDM_ISC_NAME_ISCLR200;
	case FEDM_ISC_TYPE_ISCLR1002:
		return FEDM_ISC_NAME_ISCLR1002;
	case FEDM_ISC_TYPE_ISCLR2000:
		return FEDM_ISC_NAME_ISCLR2000;
	case FEDM_ISC_TYPE_ISCLR2500_A:
		return FEDM_ISC_NAME_ISCLR2500_A;
	case FEDM_ISC_TYPE_ISCLR2500_B:
		return FEDM_ISC_NAME_ISCLR2500_B;
	case FEDM_ISC_TYPE_ISCPRH100_U:
		return FEDM_ISC_NAME_ISCPRH100_U;
	case FEDM_ISC_TYPE_ISCPRH100:
		return FEDM_ISC_NAME_ISCPRH100;
	case FEDM_ISC_TYPE_ISCPRH101:
		return FEDM_ISC_NAME_ISCPRH101;
	case FEDM_ISC_TYPE_ISCPRH101_U:
		return FEDM_ISC_NAME_ISCPRH101_U;
	case FEDM_ISC_TYPE_ISCPRH102:
		return FEDM_ISC_NAME_ISCPRH102;
	case FEDM_ISC_TYPE_ISCPRH200:
		return FEDM_ISC_NAME_ISCPRH200;
	case FEDM_ISC_TYPE_ISCMR100_U:
		return FEDM_ISC_NAME_ISCMR100_U;
	case FEDM_ISC_TYPE_ISCMR100:
		return FEDM_ISC_NAME_ISCMR100;
	case FEDM_ISC_TYPE_ISCMR200:
		return FEDM_ISC_NAME_ISCMR200;
	case FEDM_ISC_TYPE_ISCMR101:
		return FEDM_ISC_NAME_ISCMR101;
	case FEDM_ISC_TYPE_ISCMR102:
		return FEDM_ISC_NAME_ISCMR102;
	case FEDM_ISC_TYPE_ISCMR10X:
		return FEDM_ISC_NAME_ISCMR10X;
	case FEDM_ISC_TYPE_ISCMR101_U:
		return FEDM_ISC_NAME_ISCMR101_U;
	case FEDM_ISC_TYPE_CPRM02:
		return FEDM_ISC_NAME_CPRM02;
	case FEDM_ISC_TYPE_CPR02:
		return FEDM_ISC_NAME_CPR02;
	case FEDM_ISC_TYPE_CPR04_U:
		return FEDM_ISC_NAME_CPR04_U;
	case FEDM_ISC_TYPE_CPR20_XX:
		return FEDM_ISC_NAME_CPR20_XX;
	case FEDM_ISC_TYPE_CPR30_XX:
		return FEDM_ISC_NAME_CPR30_XX;
	case FEDM_ISC_TYPE_CPR40_XX_U:
		return FEDM_ISC_NAME_CPR40_XX_U;
	case FEDM_ISC_TYPE_CPR40_XX:
		return FEDM_ISC_NAME_CPR40_XX;
	case FEDM_ISC_TYPE_CPR44_XX:
		return FEDM_ISC_NAME_CPR44_XX;
	case FEDM_ISC_TYPE_CPR46_XX:
		return FEDM_ISC_NAME_CPR46_XX;
	case FEDM_ISC_TYPE_CPR47_XX:
		return FEDM_ISC_NAME_CPR47_XX;
	case FEDM_ISC_TYPE_CPR50_XX:
		return FEDM_ISC_NAME_CPR50_XX;
	
	// OBID myAXXESS reader types
	case FEDM_ISC_TYPE_MAX50_XX:
		return FEDM_ISC_NAME_MAX50_XX;
	case FEDM_ISC_TYPE_MAX_STANDARD:
		return FEDM_ISC_NAME_MAX_STANDARD;
	case FEDM_ISC_TYPE_MAX_COMFORT:
		return FEDM_ISC_NAME_MAX_COMFORT;
	case FEDM_ISC_TYPE_MAX_ONTOP:
		return FEDM_ISC_NAME_MAX_ONTOP;
	case FEDM_ISC_TYPE_MAXU1002:
		return FEDM_ISC_NAME_MAXU1002;
	
	// UHF reader types
	case FEDM_ISC_TYPE_ISCMU02:
		return FEDM_ISC_NAME_ISCMU02;
	case FEDM_ISC_TYPE_ISCMRU102:
		return FEDM_ISC_NAME_ISCMRU102;
	case FEDM_ISC_TYPE_ISCMRU200:
		return FEDM_ISC_NAME_ISCMRU200;
	case FEDM_ISC_TYPE_ISCLRU1000:
		return FEDM_ISC_NAME_ISCLRU1000;
	case FEDM_ISC_TYPE_ISCLRU1002:
		return FEDM_ISC_NAME_ISCLRU1002;
	case FEDM_ISC_TYPE_ISCLRU2000:
		return FEDM_ISC_NAME_ISCLRU2000;
	case FEDM_ISC_TYPE_ISCLRU3000:
		return FEDM_ISC_NAME_ISCLRU3000;

	// HF/UHF dual frequency reader types
	case FEDM_ISC_TYPE_ISCPRHD102:
		return FEDM_ISC_NAME_ISCPRHD102;

	// OBID megalock C reader types
	case FEDM_ISC_TYPE_MLC_COM:	// communicator
		return FEDM_ISC_NAME_MLC_COM;

	case FEDM_ISC_TYPE_MLC_EMPS: // locker
		return FEDM_ISC_NAME_MLC_EMPS;
	}

	return "";
}

/***************************************************************************
  Begin        		:	09.08.2011 / M. Hultsch
  Version       	:	04.00.02 / 09.08.2011 / M. Hultsch

  Function			:	set OEM names for Readers

  Parameters		:	two maps with OEM names for Readers

  Return value		:	-
***************************************************************************/
void FEDM_ISCReader::SetOemReaderNames(map<unsigned int, string> mapOemNames,
									   map<unsigned int, string> mapOemNames4ReaderVariants)
{
	m_mapOemNames.swap(mapOemNames);
	m_mapOemNames4ReaderVariants.swap(mapOemNames4ReaderVariants);
}

/***************************************************************************
  Begin        		:	06.06.2005 / M. Hultsch
  Version       	:	03.00.14 / 29.10.2009 / M. Hultsch

  Function			:	gets the tag name of the tag type

  Parameters		:	unsigned char ucTagType	-	tag type from system manual

  Return value		:	C-string

***************************************************************************/
char* FEDM_ISCReader::GetTagName(unsigned char ucTagType)
{
	switch(ucTagType)
	{
	case FEDM_ISC_TR_TYPE_ICODE1:
		return FEDM_ISC_TR_NAME_ICODE1;

	case FEDM_ISC_TR_TYPE_TAGIT:
		return FEDM_ISC_TR_NAME_TAGIT;

	case FEDM_ISC_TR_TYPE_ISO15693:
		return FEDM_ISC_TR_NAME_ISO15693;

	case FEDM_ISC_TR_TYPE_ISO14443A:
		return FEDM_ISC_TR_NAME_ISO14443A;

	case FEDM_ISC_TR_TYPE_ISO14443B:
		return FEDM_ISC_TR_NAME_ISO14443B;

	case FEDM_ISC_TR_TYPE_EPC:
		return FEDM_ISC_TR_NAME_EPC;

	case FEDM_ISC_TR_TYPE_ICODE_UID:
		return FEDM_ISC_TR_NAME_ICODE_UID;

	case FEDM_ISC_TR_TYPE_JEWEL:
		return FEDM_ISC_TR_NAME_JEWEL;

	case FEDM_ISC_TR_TYPE_ISO18000_3M3:
		return FEDM_ISC_TR_NAME_ISO18000_3M3;

	case FEDM_ISC_TR_TYPE_STM_SR176:
		return FEDM_ISC_TR_NAME_STM_SR176;

	case FEDM_ISC_TR_TYPE_STM_SRIxx:
		return FEDM_ISC_TR_NAME_STM_SRIxx;

	case FEDM_ISC_TR_TYPE_MCRFxxx:
		return FEDM_ISC_TR_NAME_MCRFxxx;

	case FEDM_ISC_TR_TYPE_INNOVATRON:
		return FEDM_ISC_TR_NAME_INNOVATRON;

	case FEDM_ISC_TR_TYPE_ASK_CTx:
		return FEDM_ISC_TR_NAME_ASK_CTx;

	/*case FEDM_ISC_TR_TYPE_0x64:
		return FEDM_ISC_TR_NAME_0x64;*/

	case FEDM_ISC_TR_TYPE_ISO18000_6_A:
		return FEDM_ISC_TR_NAME_ISO18000_6_A;

	case FEDM_ISC_TR_TYPE_ISO18000_6_B:
		return FEDM_ISC_TR_NAME_ISO18000_6_B;

	case FEDM_ISC_TR_TYPE_EM4222:
		return FEDM_ISC_TR_NAME_EM4222;

	case FEDM_ISC_TR_TYPE_EPC_CLASS0:
		return FEDM_ISC_TR_NAME_EPC_CLASS0;

	case FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2:
		return FEDM_ISC_TR_NAME_EPC_CLASS1_GEN2;

	case FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN1:
		return FEDM_ISC_TR_NAME_EPC_CLASS1_GEN1;

	case 0x7F:
		return "Generic Tag";

	default:
		return "";
	}
}
