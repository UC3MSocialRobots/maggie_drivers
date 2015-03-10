/*-------------------------------------------------------
|                                                       |
|                FEDM_ISCReaderModule.cpp               |
|                                                       |
---------------------------------------------------------

Copyright  2000-2014	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	06.11.2003

Version       		:	04.06.06 / 23.09.2014 / M. Hultsch
						- TagHandler support for ISO 14443-4 Tags with 10 byte UID

						04.06.01 / 13.03.2014 / M. Hultsch
						- [0x6E] Reader Diagnostic: support for new mode 0x07
						- modification in ApplyConfiguration for Paerameter Range Error (status 0x11)

						04.05.05 / 29.11.2013 / M. Hultsch
						- new reader: ID ISC.PRH200
						- [0x66] Reader Info: new mode 0x09 for WLAN-Firmware

						04.05.04 / 04.09.2013 / M. Hultsch
						- new reader: ID ISC.LRU1002

						04.05.03 / 15.07.2013 / M. Hultsch
						- new reader: OBID myAXXESS onTop
						- TagSelect: bugfix for UID-Length != 8

						04.05.02 / 02.07.2013 / M. Hultsch
						- support for new TagHandler for ISO 15693: NXP SLIX, SLIX-L, SLIX-S

						04.04.02 / 18.01.2013 / M. Hultsch
						- support for new reader ID CPR47

						04.04.00 / 06.12.2012 / M. Hultsch
						- [0x6E] Reader Diagnostic: support for new mode 0x21
		 
						04.02.08 / 13.08.2012 / M. Hultsch
						- support for events from People Counter
		 
						04.02.06 / 23.07.2012 / M. Hultsch
						- bugfix in cbsTaskRsp1 for FEISC_TASKID_FIRST_NEW_TAG
						- [0x66] Read Reader Info: new mode 0x08 (with firmware supported tag drivers)

						04.02.01 / 31.05.2012 / M. Hultsch
						- support for new tag types: Innovatron and ASK CTx

						04.02.00 / 04.04.2012 / M. Hultsch
						- renamed method: GetNonAddressedTagHandler to CreateNonAddressedTagHandler
		 
						04.00.10 / 10.02.2012 / M. Hultsch
						- support for external logmanager (FELOGMNG.dll), when #define _FELOGMNG_SUPPORT is set
						- non-addressed EPC Class1 Gen2 supported

						04.00.08 / 05.01.2012 / M. Hultsch
						- creation of ISO 14443 TagHandler after Select command only, if card type is changed

						04.00.02 / 11.08.2011 / M. Hultsch
						- check of identical UID in TagInventory

						04.00.00 / 21.07.2011 / M. Hultsch
						- LRU3000: ACC configuration moved to device RFC
						- support for [0x74] Input Event from CPR50
						- multiple TCP connections with different Reader Objects causes error
						
						03.03.01 / 31.03.2011 / M. Hultsch
						- new TagHandler class for ISO 15693 IDS SL13A
						- new TagHandler class for EPC Class1 Gen2 IDS SL900A

						03.03.00 / 01.02.2011 / M. Hultsch
						- Updated ReadReaderInfo()
						- ID ISC.LR2500-A added

						03.02.07 / 29.10.2010 / M. Hultsch
						- Updated ReadReaderInfo()
						- Updated TagSelect() for Maxim Tags
						- new method: GetNonAddressedTagHandler()

						03.02.03 / 29.07.2010 / M. Hultsch
						- PortPrefix for FECOM under Linux to support Bluetooth

						03.02.00 / 22.06.2010 / M. Hultsch

						03.01.06 / 31.05.2010 / M. Hultsch
						- ExclusiveAccess fï¿½r FEUSB mit SetPortPara

						03.01.05 / 23.04.2010 / M. Hultsch
						- ID ISC.MR102 and ID ISC.LR2500-B added

						03.01.01 / 16.03.2010 / M. Hultsch
						- Reader-Info for external peripheral devices added
						- ResetCompleteConfiguration with final CPU-Reset

						03.01.00 / 17.02.2010 / M. Hultsch
						- new Reader-Info for ID CPR50.xx/MAX50.xx added
						- support for Reader Authentification

						03.00.18 / 16.12.2009 / M. Hultsch
						- ID MAX50.xx added

Operation Systems	:	independent

Function			:	classes for OBID i-scan and OBID classic-pro reader family


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#include "FEDM_ISCReaderModule.h"
#include "FEDM_ISOTabItem.h"
#include "FEDM_ISCReaderID.h"
//#include "FEDM_ISCReaderConfig.h"
#include "FEDM_ISCReaderConfig.h"
#include "../FEDM_Functions.h"
#include "feisc.h"	// declaration of constants, datatypes and DLL-functions
#ifdef _FEDM_TAG_HANDLER
	#include "./tag_handler/FedmIscTagHandler.h"
#if !defined(_FEDM_NO_TAG_HANDLER_EPC_C1_G2)
	#include "./tag_handler/FedmIscTagHandler_EPC_Class1_Gen2.h"
	#include "./tag_handler/FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A.h"
#endif
#if !defined(_FEDM_NO_TAG_HANDLER_ISO18000_3M3)
	#include "./tag_handler/FedmIscTagHandler_ISO18000_3M3.h"
#endif
#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
	#include "fetcl.h"
	#include "./tag_handler/FedmIscTagHandler_ISO14443.h"
	#include "./tag_handler/FedmIscTagHandler_ISO14443_2.h"
	#include "./tag_handler/FedmIscTagHandler_ISO14443_2_Innovision_Jewel.h"
	#include "./tag_handler/FedmIscTagHandler_ISO14443_2_STM_SR176.h"
	#include "./tag_handler/FedmIscTagHandler_ISO14443_2_STM_SRIxxx.h"
	#include "./tag_handler/FedmIscTagHandler_ISO14443_3.h"
	#include "./tag_handler/FedmIscTagHandler_ISO14443_3_Infineon_my_d.h"
	#include "./tag_handler/FedmIscTagHandler_ISO14443_3_Infineon_my_d_move.h"
	#include "./tag_handler/FedmIscTagHandler_ISO14443_3_MIFARE_Classic.h"
	#include "./tag_handler/FedmIscTagHandler_ISO14443_3_MIFARE_Ultralight.h"
	#include "./tag_handler/FedmIscTagHandler_ISO14443_3_MIFARE_Plus.h"
	#include "./tag_handler/FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL1.h"
	#include "./tag_handler/FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL2.h"
	#include "./tag_handler/FedmIscTagHandler_ISO14443_4.h"
	#include "./tag_handler/FedmIscTagHandler_ISO14443_4_Maxim.h"
	#include "./tag_handler/FedmIscTagHandler_ISO14443_4_Maxim_MAX66000.h"
	#include "./tag_handler/FedmIscTagHandler_ISO14443_4_Maxim_MAX66020.h"
	#include "./tag_handler/FedmIscTagHandler_ISO14443_4_Maxim_MAX66040.h"
	#include "./tag_handler/FedmIscTagHandler_ISO14443_4_MIFARE_DESFire.h"
	#include "./tag_handler/FedmIscTagHandler_ISO14443_4_MIFARE_Plus.h"
	#include "./tag_handler/FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL1.h"
	#include "./tag_handler/FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL2.h"
	#include "./tag_handler/FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3.h"
#endif
#if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)
	#include "./tag_handler/FedmIscTagHandler_ISO15693.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_STM.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_STM_LRI2K.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_STM_LRIS2K.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_STM_LRIS64K.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_STM_M24LR64R.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_STM_M24LRxxER.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_NXP.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_NXP_ICODE_SLI.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_NXP_ICODE_SLIX.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_L.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_NXP_ICODE_SLIX_L.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_S.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_NXP_ICODE_SLIX_S.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_Infineon.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_Infineon_my_d.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_TI.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_TI_Tag_it_HFI_Pro.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_TI_Tag_it_HFI_Plus.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_Fujitsu.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_EM.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_EM_4034.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_KSW.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_Maxim.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_Maxim_MAX66100.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_Maxim_MAX66120.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_Maxim_MAX66140.h"
	#include "./tag_handler/FedmIscTagHandler_ISO15693_IDS_SL13A.h"
#endif
#endif

//#if defined(_FEDM_WITH_NAMESPACE)
//using namespace OBID::FEDM::Core;
//#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FEDM_ISCReaderModule::FEDM_ISCReaderModule()
{
	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_ADMIN, __FUNCTION__);

	m_iCommMode = FEDM_COMM_MODE_UNDEF;	// see constants in fedm.h
	m_bActiveAsyncTask = false;

	// clear flag filed for modified config pages
	memset(m_ucModifiedCfgPages, 0, 16);

	memset(&m_TaskInit, 0, sizeof(FEDM_TASK_INIT));

	// create new reader object in FEISC
	int iReaderHnd = FEISC_NewReader(0);
	if(iReaderHnd > 0)
	{
		// save reader handle (see FEDM_DataBase)
		m_iReaderHnd = iReaderHnd;
		FEISC_SetReaderPara(m_iReaderHnd, "LOGPROT", "1");
	}
}

FEDM_ISCReaderModule::~FEDM_ISCReaderModule()
{
	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_ADMIN, __FUNCTION__);

	FEISC_SetReaderPara(m_iReaderHnd, "LOGPROT", "0");
	if(m_iReaderHnd > 0)
		FEISC_DeleteReader(m_iReaderHnd);
}


//////////////////////////////////////////////////////////////////////
// entry functions
//////////////////////////////////////////////////////////////////////

/***************************************************************************
  Begin        		:	06.11.2003 / M. Hultsch

  Version       	:	03.00.00 / 22.08.2008 / M. Hultsch

  Function			:	opens serial port with FECOM and if successful,
						create a reader	object in FEISC

  Parameters		:	int iPortNr		-	number of port (1..256)

  Return value		:	FEDM_OK or error code (<0)
***************************************************************************/
int FEDM_ISCReaderModule::ConnectCOMM(int iPortNr)
{
	FEDM_SETPROP2(m_pLogger, FELOGMNG_PROP_PORT_NR, (unsigned int)iPortNr);
	FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_ADMIN, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "Port-Number=" << iPortNr);

#ifdef _FEDM_COM_SUPPORT
	if(m_iCommMode != FEDM_COMM_MODE_UNDEF)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_ALREADY_CONNECTED));
		FEDM_RETURN(FEDM_ERROR_ALREADY_CONNECTED);
	}

	if(iPortNr <= 0 || iPortNr > 256)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_PARAMETER));
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	bool bOpen = false;
	char cPortNr[5];
	int iErr;
	int iPortHnd = 0;
	int iReaderHnd = 0;

#if !defined(_FEDM_SUPPORT_SLINK)
	LPFN_FECOM_GET_PORT_HND lpfnGetPortHnd = (LPFN_FECOM_GET_PORT_HND)GetFeComFunction(FECOM_GET_PORT_HND);
	LPFN_FECOM_OPEN_PORT lpfnOpenPort = (LPFN_FECOM_OPEN_PORT)GetFeComFunction(FECOM_OPEN_PORT);
	LPFN_FECOM_CLOSE_PORT lpfnClosePort = (LPFN_FECOM_CLOSE_PORT)GetFeComFunction(FECOM_CLOSE_PORT);
	LPFN_FECOM_SET_PORT_PARA lpfnSetPortPara = (LPFN_FECOM_SET_PORT_PARA)GetFeComFunction(FECOM_SET_PORT_PARA);

	FEDM_CHK5(lpfnGetPortHnd);
	FEDM_CHK5(lpfnOpenPort);
	FEDM_CHK5(lpfnClosePort);
	FEDM_CHK5(lpfnSetPortPara);
#endif

	sprintf(cPortNr, "%d", iPortNr);

	// is port still open?
#if !defined(_FEDM_SUPPORT_SLINK)
	iPortHnd = lpfnGetPortHnd(cPortNr);
#else
	iPortHnd = FECOM_GetPortHnd(cPortNr);
#endif
	if(iPortHnd <= 0)
	{
		// open port
#if !defined(_FEDM_SUPPORT_SLINK)
		iPortHnd = lpfnOpenPort(cPortNr);
#else
		iPortHnd = FECOM_OpenPort(cPortNr);
#endif
		if(iPortHnd < 0)
		{
			FEDM_RETURN(iPortHnd);
		}
		bOpen = true;
	}

	if(m_iReaderHnd == 0)
	{
		// create new reader object in FEISC
		iReaderHnd = FEISC_NewReader(0);
		if(iReaderHnd < 0)
		{
			if(bOpen)
			{
#if !defined(_FEDM_SUPPORT_SLINK)
				lpfnClosePort(iPortHnd);	// close port, if this has open it
#else
				FECOM_ClosePort(iPortHnd);	// close port, if this has open it
#endif
			}
			FEDM_RETURN(iReaderHnd);
		}

		// save reader handle (see FEDM_DataBase)
		m_iReaderHnd = iReaderHnd;
		FEISC_SetReaderPara(m_iReaderHnd, "LOGPROT", "1");
	}

	// save port handle
	FEDM_CHK2(iErr, FEDM_ISCReaderModule::SetPortHnd(iPortHnd));
	// set communication mode
	m_iCommMode = FEDM_COMM_MODE_SERIAL;

	// set port parameters
#if !defined(_FEDM_SUPPORT_SLINK)
	FEDM_CHK2(iErr, lpfnSetPortPara(iPortHnd, "BAUD", "38400"));
	FEDM_CHK2(iErr, lpfnSetPortPara(iPortHnd, "FRAME", "8E1"));
	FEDM_CHK2(iErr, lpfnSetPortPara(iPortHnd, "TIMEOUT", "3000"));
#else
	FEDM_CHK2(iErr, FECOM_SetPortPara(iPortHnd, "BAUD", "38400"));
	FEDM_CHK2(iErr, FECOM_SetPortPara(iPortHnd, "FRAME", "8E1"));
	FEDM_CHK2(iErr, FECOM_SetPortPara(iPortHnd, "TIMEOUT", "3000"));
#endif

	FEDM_RETURN(FEDM_OK);
#else
	FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
#endif
}

/***************************************************************************
  Begin        		:	06.11.2003 / M. Hultsch

  Version       	:	04.00.00 / 21.07.2011 / M. Hultsch
						- multiple TCP connections with different Reader Objects causes error
						
						03.00.00 / 22.08.2008 / M. Hultsch

  Function			:	opens socket with FETCP and if successful,
						create a reader	object in FEISC

  Parameters		:	char* cHostAdr	-	pointer to host address
						int iPortNr		-	port number (1..65535)

  Return value		:	FEDM_OK or error code (<0)
***************************************************************************/
int FEDM_ISCReaderModule::ConnectTCP(char* cHostAdr, int iPortNr)
{
	FEDM_SETPROP1(m_pLogger, (unsigned int)iPortNr, cHostAdr);
	FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_ADMIN, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "IP-Address=" << cHostAdr << ", Port-Number=" << iPortNr);

#ifdef _FEDM_TCP_SUPPORT
	FEDM_CHK5(cHostAdr);

	if(m_iCommMode != FEDM_COMM_MODE_UNDEF)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_ALREADY_CONNECTED));
		FEDM_RETURN(FEDM_ERROR_ALREADY_CONNECTED);
	}

	if(iPortNr <= 0 || iPortNr > 65535)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_PARAMETER));
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	bool bOpen = false;
	int iPortHnd = 0;
	int iReaderHnd = 0;

#if !defined(_FEDM_SUPPORT_SLINK)
	LPFN_FETCP_GET_SOCKET_HND lpfnGetSocketHnd = (LPFN_FETCP_GET_SOCKET_HND)GetFeTcpFunction(FETCP_GET_SOCKET_HND);
	LPFN_FETCP_CONNECT lpfnConnect = (LPFN_FETCP_CONNECT)GetFeTcpFunction(FETCP_CONNECT);
	LPFN_FETCP_DISCONNECT lpfnDisConnect = (LPFN_FETCP_DISCONNECT)GetFeTcpFunction(FETCP_DISCONNECT);

	FEDM_CHK5(lpfnGetSocketHnd);
	FEDM_CHK5(lpfnConnect);
	FEDM_CHK5(lpfnDisConnect);
#endif

	// is port still open?
#if !defined(_FEDM_SUPPORT_SLINK)
	iPortHnd = lpfnGetSocketHnd(cHostAdr, iPortNr);
#else
	iPortHnd = FETCP_GetSocketHnd(cHostAdr, iPortNr);
#endif
	if(iPortHnd > 0)
	{
		// prevent multiple TCP connections from one app to the same Reader
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_CONNECTED_WITH_OTHER_MODULE));
		FEDM_RETURN(FEDM_ERROR_CONNECTED_WITH_OTHER_MODULE);
	}

	// open port
#if !defined(_FEDM_SUPPORT_SLINK)
	iPortHnd = lpfnConnect(cHostAdr, iPortNr);
#else
	iPortHnd = FETCP_Connect(cHostAdr, iPortNr);
#endif
	if(iPortHnd < 0)
	{
		FEDM_RETURN(iPortHnd);
	}
	bOpen = true;

	if(m_iReaderHnd == 0)
	{
		// create new reader object in FEISC
		iReaderHnd = FEISC_NewReader(0);
		if(iReaderHnd < 0)
		{
			if(bOpen)
			{
#if !defined(_FEDM_SUPPORT_SLINK)
				lpfnDisConnect(iPortHnd);	// close port, if this has open it
#else
				FETCP_DisConnect(iPortHnd);	// close port, if this has open it
#endif
			}
			FEDM_RETURN(iReaderHnd);
		}

		// save reader handle (see FEDM_DataBase)
		m_iReaderHnd = iReaderHnd;
		FEISC_SetReaderPara(m_iReaderHnd, "LOGPROT", "1");
	}

	// save port handle
	FEDM_ISCReaderModule::SetPortHnd(iPortHnd);
	// set communication mode
	m_iCommMode = FEDM_COMM_MODE_TCP;

	FEDM_RETURN(FEDM_OK);
#else
	FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
#endif
}

/***************************************************************************
  Begin        		:	06.11.2003 / M. Hultsch

  Version       	:	03.00.00 / 22.08.2008 / M. Hultsch

  Function			:	opens USB device and if successful,
						create a reader	object in FEISC

  Parameters		:	long nDeviceID	-	device id

  Return value		:	FEDM_OK or error code (<0)
***************************************************************************/
int FEDM_ISCReaderModule::ConnectUSB(unsigned long dwDeviceID)
{
	FEDM_SETPROP2(m_pLogger, FELOGMNG_PROP_DEVICED_ID, (unsigned int)dwDeviceID);
	FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_ADMIN, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "Device-ID=" << dwDeviceID);

#ifdef _FEDM_USB_SUPPORT
	if(m_iCommMode != FEDM_COMM_MODE_UNDEF)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_ALREADY_CONNECTED));
		FEDM_RETURN(FEDM_ERROR_ALREADY_CONNECTED);
	}

	bool bOpen = false;
	int iBack = 0;
	int iPortHnd = 0;
	int iReaderHnd = 0;
	char cDeviceID[32];

#if !defined(_FEDM_SUPPORT_SLINK)
	LPFN_FEUSB_GET_DEVICE_HND	lpfnGetDeviceHnd	= (LPFN_FEUSB_GET_DEVICE_HND)GetFeUsbFunction(FEUSB_GET_DEVICE_HND);
	LPFN_FEUSB_OPEN_DEVICE		lpfnOpenDevice		= (LPFN_FEUSB_OPEN_DEVICE)GetFeUsbFunction(FEUSB_OPEN_DEVICE);
	LPFN_FEUSB_CLOSE_DEVICE		lpfnCloseDevice		= (LPFN_FEUSB_CLOSE_DEVICE)GetFeUsbFunction(FEUSB_CLOSE_DEVICE);
	LPFN_FEUSB_SCAN_AND_OPEN	lpfnScanAndOpen		= (LPFN_FEUSB_SCAN_AND_OPEN)GetFeUsbFunction(FEUSB_SCAN_AND_OPEN);
	LPFN_FEUSB_SCAN				lpfnScan			= (LPFN_FEUSB_SCAN)GetFeUsbFunction(FEUSB_SCAN);
	LPFN_FEUSB_GET_SCAN_LIST_SIZE	lpfnGetScanListSize	= (LPFN_FEUSB_GET_SCAN_LIST_SIZE)GetFeUsbFunction(FEUSB_GET_SCAN_LIST_SIZE);
	LPFN_FEUSB_GET_SCAN_LIST_PARA	lpfnGetScanListPara	= (LPFN_FEUSB_GET_SCAN_LIST_PARA)GetFeUsbFunction(FEUSB_GET_SCAN_LIST_PARA);
	LPFN_FEUSB_CLEAR_SCAN_LIST	lpfnClearScanList	= (LPFN_FEUSB_CLEAR_SCAN_LIST)GetFeUsbFunction(FEUSB_CLEAR_SCAN_LIST);
	LPFN_FEUSB_GET_DEVICE_PARA	lpfnGetDevicePara	= (LPFN_FEUSB_GET_DEVICE_PARA)GetFeUsbFunction(FEUSB_GET_DEVICE_PARA);

	FEDM_CHK5(lpfnGetDeviceHnd);
	FEDM_CHK5(lpfnOpenDevice);
	FEDM_CHK5(lpfnCloseDevice);
	FEDM_CHK5(lpfnScanAndOpen);
	FEDM_CHK5(lpfnScan);
	FEDM_CHK5(lpfnGetScanListSize);
	FEDM_CHK5(lpfnGetScanListPara);
	FEDM_CHK5(lpfnClearScanList);
	FEDM_CHK5(lpfnGetDevicePara);
#endif

	if(dwDeviceID == 0)
	{
#if !defined(_FEDM_SUPPORT_SLINK)
		lpfnClearScanList();
//		iPortHnd = lpfnScanAndOpen(FEUSB_SCAN_FIRST, NULL);
		FEDM_CHK2(iBack, lpfnScan(FEUSB_SCAN_ALL, NULL));
		iBack = lpfnGetScanListSize();
		if(iBack > 0)
		{
			// take the first device
			lpfnGetScanListPara(0, "Device-ID", cDeviceID);
			sscanf((const char*)cDeviceID, "%lx", &dwDeviceID);
			iPortHnd = lpfnOpenDevice(dwDeviceID);
		}
#else
		FEUSB_ClearScanList();
//		iPortHnd = FEUSB_ScanAndOpen(FEUSB_SCAN_FIRST, NULL);
		FEDM_CHK2(iBack, FEUSB_Scan(FEUSB_SCAN_ALL, NULL));
		iBack = FEUSB_GetScanListSize();
		if(iBack > 0)
		{
			// take the first device
			FEUSB_GetScanListPara(0, "Device-ID", cDeviceID);
			sscanf((const char*)cDeviceID, "%lx", &dwDeviceID);
			iPortHnd = FEUSB_OpenDevice(dwDeviceID);
		}
#endif
		if(iPortHnd < 0)
		{
			FEDM_RETURN(iPortHnd);
		}
		bOpen = true;
	}
	else
	{
		// is port still open?
#if !defined(_FEDM_SUPPORT_SLINK)
		iPortHnd = lpfnGetDeviceHnd(dwDeviceID);
#else
		iPortHnd = FEUSB_GetDeviceHnd(dwDeviceID);
#endif
		if(iPortHnd <= 0)
		{
			// open port
#if !defined(_FEDM_SUPPORT_SLINK)
//			iBack = lpfnScan(FEUSB_SCAN_NEW, NULL);
			iBack = lpfnScan(FEUSB_SCAN_ALL, NULL);
			iPortHnd = lpfnOpenDevice(dwDeviceID);
#else
//			iBack = FEUSB_Scan(FEUSB_SCAN_NEW, NULL);
			iBack = FEUSB_Scan(FEUSB_SCAN_ALL, NULL);
			iPortHnd = FEUSB_OpenDevice(dwDeviceID);
#endif
			if(iPortHnd < 0)
			{
				FEDM_RETURN(iPortHnd);
			}
			bOpen = true;
		}
	}

	if(m_iReaderHnd == 0)
	{
		// create new reader object in FEISC
		iReaderHnd = FEISC_NewReader(iPortHnd);
		if(iReaderHnd < 0)
		{
			if(bOpen)
			{
#if !defined(_FEDM_SUPPORT_SLINK)
				lpfnCloseDevice(iPortHnd);	// close port, if this has open it
#else
				FEUSB_CloseDevice(iPortHnd);	// close port, if this has open it
#endif
			}
			FEDM_RETURN(iReaderHnd);
		}

		// save reader handle (see FEDM_DataBase)
		m_iReaderHnd = iReaderHnd;
		FEISC_SetReaderPara(m_iReaderHnd, "LOGPROT", "1");
	}

	// save port handle
	SetPortHnd(iPortHnd);
	// set communication mode
	m_iCommMode = FEDM_COMM_MODE_USB;


	// get TR-RESPONSE-TIME from reader and initialize USB-Timeout
	// for future use
	/*char cTmp[128];
	int iMultiplier = 5;
	unsigned int uiTrResponseTime = 0;
	GetPortPara("DeviceName", cTmp);
	if(strcmp(cTmp, "ID CPR.04-USB") == 0)
		SetProtocolFrameSupport(FEDM_PRT_FRAME_STANDARD);
	else
		SetProtocolFrameSupport(FEDM_PRT_FRAME_ADVANCED);

	SetData(FEDM_ISC_TMP_READ_CFG,		(unsigned char)0);
	SetData(FEDM_ISC_TMP_READ_CFG_ADR,	(unsigned char)1);
	SetData(FEDM_ISC_TMP_READ_CFG_LOC,	false);

	iBack = SendProtocol(0x80);
	if(iBack == 0x00)
	{
		GetPortPara("FamilyName", cTmp);
		if(strcmp(cTmp, "OBID classic-pro") == 0 || strcmp(cTmp, "OBID myAXXESS") == 0)
		{
			iMultiplier = 100;
		}
		else if(strcmp(cTmp, "OBID i-scan Midrange") == 0)
		{
			GetPortPara("DeviceName", cTmp);
			if(strcmp(cTmp, "ID ISC.MR101-U") == 0 || strcmp(cTmp, "ID ISC.PR101-U") == 0 || strcmp(cTmp, "ID ISC.PRH101-U") == 0)
				iMultiplier = 100;
		}

		GetConfigPara(ReaderConfig::AirInterface::TimeLimit, &uiTrResponseTime, false);
		if((uiTrResponseTime*iMultiplier) < 3000)
			iBack = SetPortPara("Timeout", (int)3000);
		else
			iBack = SetPortPara("Timeout", (int)((uiTrResponseTime*iMultiplier) + 100));
	}*/


	FEDM_RETURN(FEDM_OK);
#else
	FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
#endif
}

/***************************************************************************
  Begin        		:	06.11.2003 / M. Hultsch

  Version       	:	04.00.00 / 21.07.2011 / M. Hultsch
						- TCP: return of last state, if not TIME_WAIT

						03.00.05 / 26.02.2009 / M. Hultsch

  Function			:	- deletes reader object in FEISC
						- close serial port in FECOM or FETCP or FEUSB,
						  if no other FEDM_ISCReaderModule object use this port

  Parameters		:	-

  Return value		:	 0	- FEDM_OK
						>0	- last TCP state
						<0	- error code
***************************************************************************/
int FEDM_ISCReaderModule::DisConnect()
{
	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_ADMIN, __FUNCTION__);

	if(m_iCommMode == FEDM_COMM_MODE_UNDEF)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_NOT_CONNECTED));
		FEDM_RETURN(FEDM_ERROR_NOT_CONNECTED);
	}

	bool bCanClose = true;
	int iNextHnd = 0;
	int iPortHnd = 0;
	int iHnd;
	int iErr;
	char cPortHnd[11];


	// query the reader object in FEISC
	if(m_iReaderHnd > 0)
	{
		// query the port handle before deletion of reader object
		cPortHnd[0] = '\0';
		FEDM_CHK2(iErr, FEISC_GetReaderPara(m_iReaderHnd, "PortHnd", cPortHnd));
		sscanf(cPortHnd, "%d", &iPortHnd);

		if(iPortHnd <= 0)
		{
			m_iCommMode = FEDM_COMM_MODE_UNDEF;
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_NOT_CONNECTED));
			FEDM_RETURN(FEDM_ERROR_NOT_CONNECTED);
		}
	}


	// check, if port can be closed
	// port will only been closed, if no other reader object uses it
	iNextHnd = FEISC_GetReaderList(0);
	while(iNextHnd > 0)
	{
		cPortHnd[0] = '\0';
		iErr = FEISC_GetReaderPara(iNextHnd, "PortHnd", cPortHnd);
		if(iErr < 0)
			break;

		sscanf(cPortHnd, "%d", &iHnd);
		if((m_iReaderHnd != iNextHnd) && (iHnd == iPortHnd))
		{
			bCanClose = false;	// another FEDM_ISCReaderModule object uses this port
			break;
		}

		iNextHnd = FEISC_GetReaderList(iNextHnd);
	}
	

	// close port
	if(bCanClose)
	{
		if(m_iCommMode == FEDM_COMM_MODE_SERIAL)
		{
#ifdef _FEDM_COM_SUPPORT
	#if !defined(_FEDM_SUPPORT_SLINK)
			LPFN_FECOM_CLOSE_PORT lpfnClosePort = (LPFN_FECOM_CLOSE_PORT)GetFeComFunction(FECOM_CLOSE_PORT);
			FEDM_CHK5(lpfnClosePort);
			iErr = lpfnClosePort(iPortHnd);
			if(iErr < 0)
			{
				m_iCommMode = FEDM_COMM_MODE_UNDEF;
				SetPortHnd(0);
			}
	#else
			iErr = FECOM_ClosePort(iPortHnd);
			if(iErr < 0)
			{
				m_iCommMode = FEDM_COMM_MODE_UNDEF;
				SetPortHnd(0);
			}
	#endif
#endif
		}
		else if(m_iCommMode == FEDM_COMM_MODE_TCP)
		{
#ifdef _FEDM_TCP_SUPPORT
	#if !defined(_FEDM_SUPPORT_SLINK)
			LPFN_FETCP_DISCONNECT lpfnDisConnect = (LPFN_FETCP_DISCONNECT)GetFeTcpFunction(FETCP_DISCONNECT);
			FEDM_CHK5(lpfnDisConnect);
			iErr = lpfnDisConnect(iPortHnd);
			if(iErr < 0)
			{
				m_iCommMode = FEDM_COMM_MODE_UNDEF;
				SetPortHnd(0);
			}
	#else
			iErr = FETCP_DisConnect(iPortHnd);
			if(iErr < 0)
			{
				m_iCommMode = FEDM_COMM_MODE_UNDEF;
				SetPortHnd(0);
			}
	#endif
			if(iErr > 0)
			{
				m_iCommMode = FEDM_COMM_MODE_UNDEF;
				SetPortHnd(0);
				SetLastError(0);
				return iErr; // return of last state
			}
#endif
		}
		else if(m_iCommMode == FEDM_COMM_MODE_USB)
		{
#ifdef _FEDM_USB_SUPPORT
	#if !defined(_FEDM_SUPPORT_SLINK)
			LPFN_FEUSB_CLOSE_DEVICE lpfnCloseDevice = (LPFN_FEUSB_CLOSE_DEVICE)GetFeUsbFunction(FEUSB_CLOSE_DEVICE);
			LPFN_FEUSB_SCAN lpfnScan = (LPFN_FEUSB_SCAN)GetFeUsbFunction(FEUSB_SCAN);
			FEDM_CHK5(lpfnCloseDevice);
			FEDM_CHK5(lpfnScan);
			iErr = lpfnCloseDevice(iPortHnd);
			if(iErr < 0)
			{
				m_iCommMode = FEDM_COMM_MODE_UNDEF;
				SetPortHnd(0);
			}
			FEDM_CHK2(iErr, lpfnScan(FEUSB_SCAN_PACK, NULL));
	#else
			iErr = FEUSB_CloseDevice(iPortHnd);
			if(iErr < 0)
			{
				m_iCommMode = FEDM_COMM_MODE_UNDEF;
				SetPortHnd(0);
			}
			FEDM_CHK2(iErr, FEUSB_Scan(FEUSB_SCAN_PACK, NULL));
	#endif
#endif
		}
	}

	// reset communication mode
	m_iCommMode = FEDM_COMM_MODE_UNDEF;
	SetPortHnd(0);

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        		:	07.11.2003 / M. Hultsch

  Version       	:	01.09.00 / 07.11.2003 / M. Hultsch

  Function			:	- checks, if reader module is connected to a port

  Parameters		:	-

  Return value		:	true/false
***************************************************************************/
bool FEDM_ISCReaderModule::IsConnected()
{
	if(m_iCommMode == FEDM_COMM_MODE_UNDEF)
	{
		if(FEDM_IS_PIPORT(GetPortHnd()))
		{
			// communication with plug-in port module
			return true;
		}

		return false;
	}

	return true;
}

/***************************************************************************
  Begin        		:	18.07.2011 / M. Hultsch

  Version       	:	04.00.00 / 21.07.2011 / M. Hultsch

  Function			:	returns the actual state of an open socket

  Parameters		:	-

  Return value		:	<0 error code
						>0 TCP state:
						FETCP_STATE_CLOSED     			 1
						FETCP_STATE_LISTEN     			 2
						FETCP_STATE_SYN_SENT   			 3
						FETCP_STATE_SYN_RCVD   			 4
						FETCP_STATE_ESTABLISHED			 5
						FETCP_STATE_FIN_WAIT1  			 6
						FETCP_STATE_FIN_WAIT2  			 7
						FETCP_STATE_CLOSE_WAIT 			 8
						FETCP_STATE_CLOSING    			 9
						FETCP_STATE_LAST_ACK   			10
						FETCP_STATE_TIME_WAIT  			11

***************************************************************************/
int FEDM_ISCReaderModule::GetTcpConnectionState()
{
#ifdef _FEDM_TCP_SUPPORT
	if(m_iCommMode != FEDM_COMM_MODE_TCP)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	}

	int iPortNr = 0;
	char cHostAdr[32];

	GetPortPara("HOSTADR", cHostAdr);
	GetPortPara("PORTNR", &iPortNr);

	#if !defined(_FEDM_SUPPORT_SLINK)
		LPFN_FETCP_GET_SOCKET_STATE lpfnGetSocketState = (LPFN_FETCP_GET_SOCKET_STATE)GetFeTcpFunction(FETCP_GET_SOCKET_STATE);
		FEDM_CHK5(lpfnGetSocketState);
		SetLastError(0);
		return lpfnGetSocketState(cHostAdr, iPortNr);
	#else
		SetLastError(0);
		return FETCP_GetSocketState(cHostAdr, iPortNr);
	#endif
#else
	FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
#endif
}


/***************************************************************************
  Begin        		:	29.01.2010 / M. Hultsch

  Version       	:	03.00.19 / 29.10.2010 / M. Hultsch

  Function			:	start of Authentication process

  Parameters		:	unsigned char ucAuthentType	- authentication type (e.g. AES256)
													  0 : AES 128
													  1 : AES 192
													  2 : AES 256
						string sAuthentKey			- authentication key

  Return value		:	FEDM_OK (0) or error code (<0) or status byte (>0)
***************************************************************************/
int FEDM_ISCReaderModule::ReaderAuthentication(unsigned char ucAuthentType, string sAuthentKey)
{
	FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_HIGH_LEVEL, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "AuthentType=" << (int)ucAuthentType);

	switch(ucAuthentType)
	{
	case 0: // AES 128 Bit
		if(sAuthentKey.length() != 32)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
			FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
		}
		break;

	case 1: // AES 192 Bit
		if(sAuthentKey.length() != 48)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
			FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
		}
		break;

	case 2: // AES 256 Bit
		if(sAuthentKey.length() != 64)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
			FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
		}
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_PARAMETER));
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	return FEISC_0xAE_ReaderAuthent(m_iReaderHnd, 
									m_ucBusAdr,
									0,
									ucAuthentType,
									(unsigned char)sAuthentKey.length(),
									(unsigned char*)sAuthentKey.c_str(),
									1);
}


/***************************************************************************
  Begin        		:	29.01.2010 / M. Hultsch

  Version       	:	03.00.19 / 29.10.2010 / M. Hultsch

  Function			:	start of Authentication process

  Parameters		:	unsigned char ucAuthentType	- authentication type (e.g. AES256)
													  0 : AES 128
													  1 : AES 192
													  2 : AES 256
						unsigned int uiAuthentKeyLength	- length in bytes of AuthentKey
						unsigned char* ucAuthentKey	- buffer with authentication key

  Return value		:	FEDM_OK (0) or error code (<0) or status byte (>0)
***************************************************************************/
int FEDM_ISCReaderModule::ReaderAuthentication(unsigned char ucAuthentType, unsigned int uiAuthentKeyLength, unsigned char* ucAuthentKey)
{
	FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_HIGH_LEVEL, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "AuthentType=" << (int)ucAuthentType);

	FEDM_CHK5(ucAuthentKey);

	switch(ucAuthentType)
	{
	case 0: // AES 128 Bit
		if(uiAuthentKeyLength != 16)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
			FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
		}
		break;

	case 1: // AES 192 Bit
		if(uiAuthentKeyLength != 24)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
			FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
		}
		break;

	case 2: // AES 256 Bit
		if(uiAuthentKeyLength != 32)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_STRING_LENGTH));
			FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
		}
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_PARAMETER));
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	return FEISC_0xAE_ReaderAuthent(m_iReaderHnd, 
									m_ucBusAdr,
									0,
									ucAuthentType,
									(unsigned char)uiAuthentKeyLength,
									ucAuthentKey,
									0);
}


/***************************************************************************
  Begin        		:	04.01.2007 / M. Hultsch

  Version       	:	03.00.00 / 25.08.2008 / M. Hultsch

  Function			:	- set of communication handle, if connection is established
						  with communication library FECOM, FEUSB or FETCP

  Parameters		:	-

  Return value		:	true/false
***************************************************************************/
int FEDM_ISCReaderModule::SetPortHnd(int iPortHnd)
{
	int iErr = 0;

	FEDM_CHK2(iErr, FEDM_ISCReader::SetPortHnd(iPortHnd));

	if(iPortHnd > 0)
	{
		if(FEDM_IS_COMPORT(iPortHnd))
			m_iCommMode = FEDM_COMM_MODE_SERIAL;
		else if(FEDM_IS_USBPORT(iPortHnd))
			m_iCommMode = FEDM_COMM_MODE_USB;
		else if(FEDM_IS_TCPPORT(iPortHnd))
			m_iCommMode = FEDM_COMM_MODE_TCP;
		else
			m_iCommMode = FEDM_COMM_MODE_UNDEF;
	}
	else
	{
		m_iCommMode = FEDM_COMM_MODE_UNDEF;
	}

	return FEDM_OK;
}


//////////////////////////////////////////////////////////////////////
// methods for tag communication
//////////////////////////////////////////////////////////////////////

/***************************************************************************
  Begin        		:	01.04.2009 / M. Hultsch
  
  Version       	:	04.00.02 / 11.08.2011 / M. Hultsch
						- check of identical UID

						03.00.12 / 23.09.2009 / M. Hultsch

  Function			:	method executes synchronous Inventory (for all i-scan and classic-pro Readers)
						Inventory with More-Data handling (preset by bAll)
						NOTE: all MODE flags must be set before invoke of TagInventory
							  and will be respected, except MORE

  Parameters		:	bool bAll (=true)				- automatic mode with internal ResetTable and MORE-loop
						unsigned char ucMode (=0x00)	- manual control of inventory (if bAll=true, MORE-Flag will be ignored)
						unsigned char ucAntennas (=1)	- flag field with antennas (only for i-scan Readers)

  Return value		:	FEDM_ISC_TAG_LIST*
***************************************************************************/
#ifdef _FEDM_TAG_HANDLER
FEDM_ISC_TAG_LIST* FEDM_ISCReaderModule::TagInventory(	bool bAll,
														unsigned char ucMode,
														unsigned char ucAntennas )
{
	FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "bAll=" << (int)bAll << ", Mode=0x" << std::hex << std::uppercase << (int)ucMode << ", Antennas=0x" << std::hex << std::uppercase << (int)ucAntennas);

	string sSnr;

	m_mapTagHandler.clear();
	SetLastError(FEDM_OK);

	// command byte for inventory
	SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char)0x01);

	if(bAll)
	{
		SetData(FEDM_ISC_TMP_B0_MODE, ucMode);
		SetData(FEDM_ISC_TMP_B0_MODE_MORE, false);
		ResetTable(FEDM_ISC_ISO_TABLE);
	}
	else
	{
		SetData(FEDM_ISC_TMP_B0_MODE, ucMode);
	}

	if(ucMode & FEDM_ISC_ISO_MODE_ANT)
		SetData(FEDM_ISC_TMP_B0_REQ_ANT_SEL, ucAntennas);

	// synchronous execution of inventory command
	int iBack = SendProtocol(0xB0);
	if( ! (	iBack==0x00 || 
			iBack==0x01 || 
			iBack==0x02 || 
			iBack==0x03 || 
			iBack==0x83 || 
			iBack==0x84 || 
			iBack==0x90 || 
			iBack==0x93 || 
			iBack==0x94 || 
			iBack==0x95 ) )
	{
		SetLastError(iBack);
		return &m_mapTagHandler;
	}

	if( bAll )
	{
		while(iBack == 0x94) // more data
		{
			SetData(FEDM_ISC_TMP_B0_MODE_MORE, true);
			iBack = SendProtocol(0xB0);
		}
	}

	if( m_bEnableTagHandler )
	{
		// build new tag handler list
		for(unsigned int uiCnt=0; uiCnt<m_uiISOTableLength; uiCnt++)
		{
			if(m_ISOTable[uiCnt]->m_pTagHandler != NULL)
			{
				if(m_ISOTable[uiCnt]->GetSnr(sSnr) != FEDM_OK)
					continue;

				// check for double UID; identical UID is ignored
				if( m_mapTagHandler.find(sSnr) != m_mapTagHandler.end())
					continue;

				m_mapTagHandler.insert(make_pair(sSnr, m_ISOTable[uiCnt]->m_pTagHandler));
			}
		}
	}

	return &m_mapTagHandler;
}
#endif


/***************************************************************************
  Begin        		:	01.04.2009 / M. Hultsch
  
  Version       	:	04.05.03 / 30.08.2013 / M. Hultsch
						- bugfix for UID-Length != 8

						04.02.01 / 31.05.2012 / M. Hultsch
						- support for Innovatron and ASK CTx

						03.00.12 / 01.09.2009 / M. Hultsch

  Function			:	Select of transponder
						- with automatic CINF-flag for classic-pro Reader

  Parameters		:	FedmIscTagHandler*			- pointer to tag handler base class
						unsigned int uiTagDriver	- optional: for explizit tag driver selection  in reader

  Return value		:	FedmIscTagHandler*	- pointer to new tag handler (base class) of selected tag
											  or NULL	-	in error case
														-	if no tag could be selected
														-	if transponder type could not be identified
***************************************************************************/
#ifdef _FEDM_TAG_HANDLER
FedmIscTagHandler* FEDM_ISCReaderModule::TagSelect(	FedmIscTagHandler* pTagHandler,
													unsigned int uiTagDriver )
{
	FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "TagDriver=" << uiTagDriver);
	
	SetLastError(FEDM_OK);

	if( !m_bEnableTagHandler )
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " Support for TagHandler disabled");
		SetLastError(FEDM_ERROR_UNSUPPORTED);
		return NULL;
	}

	if(pTagHandler == NULL)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " TagHandler is NULL");
		SetLastError(FEDM_ERROR_NULL_POINTER);
		return NULL;
	}

	int iBack = 0;
	int iErr = 0;

	// prepare select command
	FEDM_CHK7(iErr, SetData(FEDM_ISC_TMP_B0_MODE, (unsigned char)0));
	FEDM_CHK7(iErr, SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
	if( m_bSelectWithCinf && (!m_bDisableSelectWithCinf) )
	{
		FEDM_CHK7(iErr, SetData(FEDM_ISC_TMP_B0_MODE_CINF, true));
	}

	// manual selection of special transponder driver
	if(uiTagDriver > 0)
	{
		FEDM_CHK7(iErr, SetData(FEDM_ISC_TMP_B0_MODE_DRV_SEL, true));
		FEDM_CHK7(iErr, SetData(FEDM_ISC_TMP_B0_REQ_TR_DRIVER, uiTagDriver));
	}
	else
	{
		FEDM_CHK7(iErr, SetData(FEDM_ISC_TMP_B0_MODE_DRV_SEL, false));
	}

	if(strlen(pTagHandler->GetUID()) != 16)
	{
		FEDM_CHK7(iErr, SetData(FEDM_ISC_TMP_B0_MODE_UID_LF, true));
		FEDM_CHK7(iErr, SetData(FEDM_ISC_TMP_B0_REQ_UID_LEN, (unsigned int)strlen(pTagHandler->GetUID()) >> 1 ));
	}
	FEDM_CHK7(iErr, SetData(FEDM_ISC_TMP_B0_REQ_UID, (char*)pTagHandler->GetUID(), (int)strlen(pTagHandler->GetUID())));
	
	FEDM_CHK7(iErr, SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char)0x25));

	// execute select command
	iBack = SendProtocol(0xB0);
	if(iBack)
	{
		SetLastError(iBack);
		return NULL;
	}

	int iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
	if(iIdx < 0)
		return NULL;

	// create concrete transponder handler class
	switch(pTagHandler->m_pTabItem->m_ucTrType)
	{
	// HF transponder types
	case FEDM_ISC_TR_TYPE_ICODE1:
	case FEDM_ISC_TR_TYPE_TAGIT:
	case FEDM_ISC_TR_TYPE_ISO15693:
		pTagHandler->m_bNonAddressedMode = false;
		return pTagHandler;

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
	case FEDM_ISC_TR_TYPE_ISO14443A:
	case FEDM_ISC_TR_TYPE_ISO14443B:
		{
			if(uiTagDriver > 0)
			{
				return TagSelect_CreateNewTagHandler(iIdx, uiTagDriver, pTagHandler);
			}
			else
			{
				unsigned char ucFormat = 0;

				FEDM_CHK7(iErr, GetData(FEDM_ISC_TMP_B0_RSP_FORMAT, &ucFormat));
				switch(ucFormat)
				{
				case 0: // no card info available
					return pTagHandler;

				case 1:	// card info for ISO14443-4 type A transponder
					return TagSelect_EvaFormat1(iIdx, pTagHandler);

				case 2:	// card info for ISO14443-3 or -4 type B transponder
					return TagSelect_EvaFormat2(iIdx, pTagHandler);

				case 3:	// card info for ISO14443-3 type A transponder
					return TagSelect_EvaFormat3(iIdx, pTagHandler);
				}
			}
		}
		break;
#endif

	case FEDM_ISC_TR_TYPE_EPC:
	case FEDM_ISC_TR_TYPE_ICODE_UID:
	case FEDM_ISC_TR_TYPE_JEWEL:
	case FEDM_ISC_TR_TYPE_ISO18000_3M3:
	case FEDM_ISC_TR_TYPE_STM_SR176:
	case FEDM_ISC_TR_TYPE_STM_SRIxx:
	case FEDM_ISC_TR_TYPE_MCRFxxx:
	case FEDM_ISC_TR_TYPE_INNOVATRON:
	case FEDM_ISC_TR_TYPE_ASK_CTx:
	//case FEDM_ISC_TR_TYPE_0x64:
		pTagHandler->m_bNonAddressedMode = false;
		return pTagHandler;

	// UHF transponder types
	case FEDM_ISC_TR_TYPE_ISO18000_6_A:
	case FEDM_ISC_TR_TYPE_ISO18000_6_B:
	case FEDM_ISC_TR_TYPE_EM4222:
	case FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2:
	case FEDM_ISC_TR_TYPE_EPC_CLASS0:
	case FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN1:
		pTagHandler->m_bNonAddressedMode = false;
		return pTagHandler;
	}

	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " Unknown Transponder-Type 0x" << std::hex << std::uppercase << (int)pTagHandler->m_pTabItem->m_ucTrType);
	return NULL;
}
#endif




//////////////////////////////////////////////////////////////////////
// protected functions
//////////////////////////////////////////////////////////////////////

/***************************************************************************
  Begin        		:	01.09.2009 / M. Hultsch
  
  Version       	:	04.00.08 / 05.01.2012 / M. Hultsch
						- creation of TagHandler only if card type is changed

						03.01.00 / 17.02.2010 / M. Hultsch

  Function			:	create tag handler class identified by uiTagDriver

  Parameters		:	int iIdx					- index in FEDM_ISOTabItem
						unsigned int uiTagDriver	- manual selected special tag driver
													  value according system manual of Reader
						FedmIscTagHandler*			- pointer to tag handler base class

  Return value		:	FedmIscTagHandler*	- pointer to new tag handler (base class) of selected tag
											  or NULL	-	in error case
														-	if transponder type could not be identified
***************************************************************************/
#ifdef _FEDM_TAG_HANDLER
#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
FedmIscTagHandler* FEDM_ISCReaderModule::TagSelect_CreateNewTagHandler(int iIdx, unsigned int uiTagDriver, FedmIscTagHandler* pTagHandler)
{
	FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "TagDriver=" << uiTagDriver);
	
	if(dynamic_cast<FedmIscTagHandler_ISO14443*>(pTagHandler) == NULL)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " TagHandler is not of type ISO 14443");
		return NULL;
	}

	if(uiTagDriver == 1)
	{
		if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_4)
		{
			FedmIscTagHandler_ISO14443_4* pNewTagHandler = NULL;
			pNewTagHandler = new FedmIscTagHandler_ISO14443_4(this, pTagHandler->m_pTabItem);
			if(pNewTagHandler != NULL)
			{
				delete pTagHandler;
				m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
				return (FedmIscTagHandler*)pNewTagHandler;
			}
		}
		else
		{
			pTagHandler->Init();
			return pTagHandler;
		}
	}
	else if(uiTagDriver == 2)
	{
		if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_3_MIFARE_CLASSIC)
		{
			FedmIscTagHandler_ISO14443_3_MIFARE_Classic* pNewTagHandler = NULL;
			pNewTagHandler = new FedmIscTagHandler_ISO14443_3_MIFARE_Classic(this, pTagHandler->m_pTabItem);
			if(pNewTagHandler != NULL)
			{
				delete pTagHandler;
				m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
				return (FedmIscTagHandler*)pNewTagHandler;
			}
		}
		else
		{
			pTagHandler->Init();
			return pTagHandler;
		}
	}
	else if(uiTagDriver == 3)
	{
		if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_3_MIFARE_ULTRALIGHT)
		{
			FedmIscTagHandler_ISO14443_3_MIFARE_Ultralight* pNewTagHandler = NULL;
			pNewTagHandler = new FedmIscTagHandler_ISO14443_3_MIFARE_Ultralight(this, pTagHandler->m_pTabItem);
			if(pNewTagHandler != NULL)
			{
				delete pTagHandler;
				m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
				return (FedmIscTagHandler*)pNewTagHandler;
			}
		}
		else
		{
			pTagHandler->Init();
			return pTagHandler;
		}
	}
	else if(uiTagDriver == 4)
	{
		if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_3_MIFARE_PLUS_SL1)
		{
			FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL1* pNewTagHandler = NULL;
			pNewTagHandler = new FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL1(this, pTagHandler->m_pTabItem);
			if(pNewTagHandler != NULL)
			{
				delete pTagHandler;
				m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
				return (FedmIscTagHandler*)pNewTagHandler;
			}
		}
		else
		{
			pTagHandler->Init();
			return pTagHandler;
		}
	}
	else if(uiTagDriver == 5)
	{
		if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_4_MIFARE_PLUS_SL1)
		{
			FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL1* pNewTagHandler = NULL;
			pNewTagHandler = new FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL1(this, pTagHandler->m_pTabItem);
			if(pNewTagHandler != NULL)
			{
				delete pTagHandler;
				m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
				return (FedmIscTagHandler*)pNewTagHandler;
			}
		}
		else
		{
			pTagHandler->Init();
			return pTagHandler;
		}
	}
	else if(uiTagDriver == 6)
	{
		if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_3_MIFARE_PLUS_SL2)
		{
			FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL2* pNewTagHandler = NULL;
			pNewTagHandler = new FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL2(this, pTagHandler->m_pTabItem);
			if(pNewTagHandler != NULL)
			{
				delete pTagHandler;
				m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
				return (FedmIscTagHandler*)pNewTagHandler;
			}
		}
		else
		{
			pTagHandler->Init();
			return pTagHandler;
		}
	}
	else if(uiTagDriver == 7)
	{
		if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_4_MIFARE_PLUS_SL2)
		{
			FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL2* pNewTagHandler = NULL;
			pNewTagHandler = new FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL2(this, pTagHandler->m_pTabItem);
			if(pNewTagHandler != NULL)
			{
				delete pTagHandler;
				m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
				return (FedmIscTagHandler*)pNewTagHandler;
			}
		}
		else
		{
			pTagHandler->Init();
			return pTagHandler;
		}
	}
	else if(uiTagDriver == 8)
	{
		if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_4_MIFARE_PLUS_SL3)
		{
			FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3* pNewTagHandler = NULL;
			pNewTagHandler = new FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3(this, pTagHandler->m_pTabItem);
			if(pNewTagHandler != NULL)
			{
				delete pTagHandler;
				m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
				return (FedmIscTagHandler*)pNewTagHandler;
			}
		}
		else
		{
			pTagHandler->Init();
			return pTagHandler;
		}
	}
	else if(uiTagDriver == 9)
	{
		if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_4_MIFARE_DESFIRE)
		{
			FedmIscTagHandler_ISO14443_4_MIFARE_DESFire* pNewTagHandler = NULL;
			pNewTagHandler = new FedmIscTagHandler_ISO14443_4_MIFARE_DESFire(	this, pTagHandler->m_pTabItem);
			if(pNewTagHandler != NULL)
			{
				pNewTagHandler->Init();
				delete pTagHandler;
				m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
				return (FedmIscTagHandler*)pNewTagHandler;
			}
		}
		else
		{
			pTagHandler->Init();
			return pTagHandler;
		}
	}
	else if(uiTagDriver == 10)
	{
		if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_3_INFINEON_MY_D)
		{
			FedmIscTagHandler_ISO14443_3_Infineon_my_d* pNewTagHandler = NULL;
			pNewTagHandler = new FedmIscTagHandler_ISO14443_3_Infineon_my_d(this, pTagHandler->m_pTabItem);
			if(pNewTagHandler != NULL)
			{
				delete pTagHandler;
				m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
				return (FedmIscTagHandler*)pNewTagHandler;
			}
		}
		else
		{
			pTagHandler->Init();
			return pTagHandler;
		}
	}
	else if(uiTagDriver == 11)
	{
		if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_3_INFINEON_MY_D_MOVE)
		{
			FedmIscTagHandler_ISO14443_3_Infineon_my_d_move* pNewTagHandler = NULL;
			pNewTagHandler = new FedmIscTagHandler_ISO14443_3_Infineon_my_d_move(this, pTagHandler->m_pTabItem);
			if(pNewTagHandler != NULL)
			{
				delete pTagHandler;
				m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
				return (FedmIscTagHandler*)pNewTagHandler;
			}
		}
		else
		{
			pTagHandler->Init();
			return pTagHandler;
		}
	}
	else
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " Unvalid TagDriver parameter");
	}

	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " Creation of TagHandler failed - no more mem?");
	return NULL;
}
#endif
#endif

/***************************************************************************
  Begin        		:	20.04.2009 / M. Hultsch
  
  Version       	:	04.06.06 / 23.09.2014 / M. Hultsch
						- support for ISO 14443-4 Tags with 10 byte UID

						04.00.08 / 05.01.2012 / M. Hultsch
						- creation of TagHandler only if card type is changed

						03.00.07 / 06.05.2009 / M. Hultsch

  Function			:	evaluate the returned SAK

  Parameters		:	int iIdx			- index in FEDM_ISOTabItem
						FedmIscTagHandler*	- pointer to tag handler base class

  Return value		:	FedmIscTagHandler*	- pointer to new tag handler (base class) of selected tag
											  or NULL	-	in error case
														-	if transponder type could not be identified
***************************************************************************/
#ifdef _FEDM_TAG_HANDLER
#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
FedmIscTagHandler* FEDM_ISCReaderModule::TagSelect_EvaFormat1(int iIdx, FedmIscTagHandler* pTagHandler)
{
	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__);

	// evaluate ATQA
	unsigned int uiAtqA = ((unsigned int)m_TmpData[2048] << 8) + m_TmpData[2049];

	// evaluate SAK
	unsigned char ucSAK = m_TmpData[2050];

	if( (m_ISOTable[iIdx]->m_ucOptInfo & 0x03) == 0x00 )
	{
		// cascade level 1 == 4 byte UID

		if(ucSAK & 0x20)
		{
			// any ISO14443-4 compatible tag types
			if(dynamic_cast<FedmIscTagHandler_ISO14443*>(pTagHandler) == NULL)
				return NULL;

			switch(ucSAK)
			{
			case 0x20:
				if(uiAtqA == 0x0002 || uiAtqA == 0x0004)
				{
					unsigned char ucTL = m_TmpData[2051];
					unsigned char ucT0 = m_TmpData[2052];
					unsigned char* pucAts = &m_TmpData[2052];
					unsigned char ucMfPx[] = {0xC1, 0x05, 0x2F, 0x2F, 0x01, 0xBC, 0xD6};
					unsigned char ucMfPs[] = {0xC1, 0x05, 0x2F, 0x2F, 0x00, 0x35, 0xC7};

					if(ucTL >= 9)
					{
						if(ucT0 & 0x10)
						{
							pucAts++;
						}
						if(ucT0 & 0x20)
						{
							pucAts++;
						}
						if(ucT0 & 0x40)
						{
							pucAts++;
						}
						pucAts++;
						
						if( memcmp(pucAts, ucMfPx, 7) == 0 || memcmp(pucAts, ucMfPs, 7) == 0)
						{
							// MIFARE Plus Security Level 0 or 3 with 4 byte fix UID or 4 byte random UID

							if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_4_MIFARE_PLUS_SL3)
							{
								FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3* pNewTagHandler = NULL;
								pNewTagHandler = new FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3(this, pTagHandler->m_pTabItem);
								if(pNewTagHandler != NULL)
								{
									delete pTagHandler;
									m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
									return (FedmIscTagHandler*)pNewTagHandler;
								}
							}
							else
							{
								pTagHandler->Init();
								return pTagHandler;
							}
						}
						else
						{
							if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_4)
							{
								FedmIscTagHandler_ISO14443_4* pNewTagHandler = NULL;
								pNewTagHandler = new FedmIscTagHandler_ISO14443_4(this, pTagHandler->m_pTabItem);
								if(pNewTagHandler != NULL)
								{
									delete pTagHandler;
									m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
									return (FedmIscTagHandler*)pNewTagHandler;
								}
							}
							else
							{
								pTagHandler->Init();
								return pTagHandler;
							}
						}
					} // if(ucTL >= 9)
					else
					{
						if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_4)
						{
							FedmIscTagHandler_ISO14443_4* pNewTagHandler = NULL;
							pNewTagHandler = new FedmIscTagHandler_ISO14443_4(this, pTagHandler->m_pTabItem);
							if(pNewTagHandler != NULL)
							{
								delete pTagHandler;
								m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
								return (FedmIscTagHandler*)pNewTagHandler;
							}
						}
						else
						{
							pTagHandler->Init();
							return pTagHandler;
						}
					}
				}
				else if(uiAtqA == 0x0304 && m_ISOTable[iIdx]->m_ucSnr[7] == 0x80)
				{
					// MIFARE DESFire with 4 byte random UID (filled with leading/following zero to complete 8 byte UID)
					if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_4_MIFARE_DESFIRE)
					{
						//MIFARE DESFire CL2, MIFARE DESFire EV1 CL2
						FedmIscTagHandler_ISO14443_4_MIFARE_DESFire* pNewTagHandler = NULL;
						pNewTagHandler = new FedmIscTagHandler_ISO14443_4_MIFARE_DESFire(this, pTagHandler->m_pTabItem);
						if(pNewTagHandler != NULL)
						{
							delete pTagHandler;
							m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
							return (FedmIscTagHandler*)pNewTagHandler;
						}
					}
					else
					{
						pTagHandler->Init();
						return pTagHandler;
					}
				}
				else
				{
					if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_4)
					{
						FedmIscTagHandler_ISO14443_4* pNewTagHandler = NULL;
						pNewTagHandler = new FedmIscTagHandler_ISO14443_4(this, pTagHandler->m_pTabItem);
						if(pNewTagHandler != NULL)
						{
							delete pTagHandler;
							m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
							return (FedmIscTagHandler*)pNewTagHandler;
						}
					}
					else
					{
						pTagHandler->Init();
						return pTagHandler;
					}
				}
				break;

			default:
				if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_4)
				{
					FedmIscTagHandler_ISO14443_4* pNewTagHandler = NULL;
					pNewTagHandler = new FedmIscTagHandler_ISO14443_4(this, pTagHandler->m_pTabItem);
					if(pNewTagHandler != NULL)
					{
						delete pTagHandler;
						m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
						return (FedmIscTagHandler*)pNewTagHandler;
					}
				}
				else
				{
					pTagHandler->Init();
					return pTagHandler;
				}
				break;
			
			} // switch(ucSAK)
		
		} // if(ucSAK & 0x20)
	}
	else if( (m_ISOTable[iIdx]->m_ucOptInfo & 0x03) == 0x01 )
	{
		// cascade level 2 == 7 byte UID

		if(m_ISOTable[iIdx]->m_ucSnr[7] == FEDM_ISC_ISO_MFR_NXP)
		{
			if(ucSAK & 0x20)
			{
				// any ISO14443-4 compatible tag types
				if(dynamic_cast<FedmIscTagHandler_ISO14443*>(pTagHandler) == NULL)
					return NULL;

				switch(ucSAK)
				{
				case 0x20:	// MIFARE DESFire CL2, MIFARE DESFire EV1 CL2, MIFARE Plus CL2 (Security Level 0 oder 3)
					if(uiAtqA == 0x0344)
					{
						if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_4_MIFARE_DESFIRE)
						{
							//MIFARE DESFire CL2, MIFARE DESFire EV1 CL2
							FedmIscTagHandler_ISO14443_4_MIFARE_DESFire* pNewTagHandler = NULL;
							pNewTagHandler = new FedmIscTagHandler_ISO14443_4_MIFARE_DESFire(this, pTagHandler->m_pTabItem);
							if(pNewTagHandler != NULL)
							{
								delete pTagHandler;
								m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
								return (FedmIscTagHandler*)pNewTagHandler;
							}
						}
						else
						{
							pTagHandler->Init();
							return pTagHandler;
						}
					}
					else if(uiAtqA == 0x0042 || uiAtqA == 0x0044)
					{
						unsigned char ucTL = m_TmpData[2051];
						unsigned char ucT0 = m_TmpData[2052];
						unsigned char* pucAts = &m_TmpData[2052];
						unsigned char ucMfPx[] = {0xC1, 0x05, 0x2F, 0x2F, 0x01, 0xBC, 0xD6};
						unsigned char ucMfPs[] = {0xC1, 0x05, 0x2F, 0x2F, 0x00, 0x35, 0xC7};

						if(ucTL >= 9)
						{
							if(ucT0 & 0x10)
							{
								pucAts++;
							}
							if(ucT0 & 0x20)
							{
								pucAts++;
							}
							if(ucT0 & 0x40)
							{
								pucAts++;
							}
							pucAts++;
							
							if( memcmp(pucAts, ucMfPx, 7) == 0 || memcmp(pucAts, ucMfPs, 7) == 0)
							{
								if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_4_MIFARE_PLUS_SL3)
								{
									FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3* pNewTagHandler = NULL;
									pNewTagHandler = new FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3(this, pTagHandler->m_pTabItem);
									if(pNewTagHandler != NULL)
									{
										delete pTagHandler;
										m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
										return (FedmIscTagHandler*)pNewTagHandler;
									}
								}
								else
								{
									pTagHandler->Init();
									return pTagHandler;
								}
							}
							else
							{
								if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_4)
								{
									FedmIscTagHandler_ISO14443_4* pNewTagHandler = NULL;
									pNewTagHandler = new FedmIscTagHandler_ISO14443_4(this, pTagHandler->m_pTabItem);
									if(pNewTagHandler != NULL)
									{
										delete pTagHandler;
										m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
										return (FedmIscTagHandler*)pNewTagHandler;
									}
								}
								else
								{
									pTagHandler->Init();
									return pTagHandler;
								}
							}
						} // if(ucTL >= 9)
						else
						{
							if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_4)
							{
								FedmIscTagHandler_ISO14443_4* pNewTagHandler = NULL;
								pNewTagHandler = new FedmIscTagHandler_ISO14443_4(this, pTagHandler->m_pTabItem);
								if(pNewTagHandler != NULL)
								{
									delete pTagHandler;
									m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
									return (FedmIscTagHandler*)pNewTagHandler;
								}
							}
							else
							{
								pTagHandler->Init();
								return pTagHandler;
							}
						}
					}
					else
					{
						if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_4)
						{
							FedmIscTagHandler_ISO14443_4* pNewTagHandler = NULL;
							pNewTagHandler = new FedmIscTagHandler_ISO14443_4(this, pTagHandler->m_pTabItem);
							if(pNewTagHandler != NULL)
							{
								delete pTagHandler;
								m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
								return (FedmIscTagHandler*)pNewTagHandler;
							}
						}
						else
						{
							pTagHandler->Init();
							return pTagHandler;
						}
					}
					break;

				default:
					if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_4)
					{
						FedmIscTagHandler_ISO14443_4* pNewTagHandler = NULL;
						pNewTagHandler = new FedmIscTagHandler_ISO14443_4(this, pTagHandler->m_pTabItem);
						if(pNewTagHandler != NULL)
						{
							delete pTagHandler;
							m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
							return (FedmIscTagHandler*)pNewTagHandler;
						}
					}
					else
					{
						pTagHandler->Init();
						return pTagHandler;
					}
					break;
				
				} // switch(ucSAK)
			
			} // if(ucSAK & 0x20)
		}
		else
		{	
			// non NXP tag

			if(ucSAK & 0x20)
			{
				// any ISO14443-4 compatible tag types
				if(dynamic_cast<FedmIscTagHandler_ISO14443*>(pTagHandler) == NULL)
					return NULL;

				if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_4)
				{
					FedmIscTagHandler_ISO14443_4* pNewTagHandler = NULL;
					pNewTagHandler = new FedmIscTagHandler_ISO14443_4(this, pTagHandler->m_pTabItem);
					if(pNewTagHandler != NULL)
					{
						delete pTagHandler;
						m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
						return (FedmIscTagHandler*)pNewTagHandler;
					}
				}
				else
				{
					pTagHandler->Init();
					return pTagHandler;
				}
			}
		}
	}
	else
	{
		// cascade level 3 == 10 byte UID

		// non NXP tag

		if(ucSAK & 0x20)
		{
			// any ISO14443-4 compatible tag types
			if(dynamic_cast<FedmIscTagHandler_ISO14443*>(pTagHandler) == NULL)
				return NULL;

			if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_4)
			{
				FedmIscTagHandler_ISO14443_4* pNewTagHandler = NULL;
				pNewTagHandler = new FedmIscTagHandler_ISO14443_4(this, pTagHandler->m_pTabItem);
				if(pNewTagHandler != NULL)
				{
					delete pTagHandler;
					m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
					return (FedmIscTagHandler*)pNewTagHandler;
				}
			}
			else
			{
				pTagHandler->Init();
				return pTagHandler;
			}
		}
	}

	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " Creation of TagHandler failed");
	SetLastError(FEDM_ERROR_UNKNOWN_TRANSPONDER_TYPE);
	return NULL;
}
#endif
#endif

/***************************************************************************
  Begin        		:	20.04.2009 / M. Hultsch
  
  Version       	:	04.00.08 / 05.01.2012 / M. Hultsch
						- creation of TagHandler only if card type is changed

						03.00.07 / 06.05.2009 / M. Hultsch

  Function			:	evaluate the returned protocol type

  Parameters		:	int iIdx			- index in FEDM_ISOTabItem
						FedmIscTagHandler*	- pointer to tag handler base class

  Return value		:	FedmIscTagHandler*	- pointer to new tag handler (base class) of selected tag
											  or NULL	-	in error case
														-	if transponder type could not be identified
***************************************************************************/
#ifdef _FEDM_TAG_HANDLER
#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
FedmIscTagHandler* FEDM_ISCReaderModule::TagSelect_EvaFormat2(int iIdx, FedmIscTagHandler* pTagHandler)
{
	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__);

	// evaluate protocol type
	unsigned char ucProtocolType = m_TmpData[2049] & 0x0F;

	switch(ucProtocolType&0x01)
	{
	case 0:	// ISO 14443-3 Type B transponder
		{
			if(dynamic_cast<FedmIscTagHandler_ISO14443*>(pTagHandler) == NULL)
				return NULL;

			if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_3)
			{
				FedmIscTagHandler_ISO14443_3* pNewTagHandler = NULL;
				pNewTagHandler = new FedmIscTagHandler_ISO14443_3(this, pTagHandler->m_pTabItem);
				if(pNewTagHandler != NULL)
				{
					delete pTagHandler;
					m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
					return (FedmIscTagHandler*)pNewTagHandler;
				}
			}
			else
			{
				pTagHandler->Init();
				return pTagHandler;
			}
		}
		break;

	case 1:	// ISO 14443-4 Type B transponder
		{
			if(dynamic_cast<FedmIscTagHandler_ISO14443*>(pTagHandler) == NULL)
				return NULL;

			unsigned int uiTmpAppData = 0;
			uiTmpAppData = ((pTagHandler->m_pTabItem->m_ucSnr[0] << 24) | (pTagHandler->m_pTabItem->m_ucSnr[1] << 16) | (pTagHandler->m_pTabItem->m_ucSnr[2] << 8) | (pTagHandler->m_pTabItem->m_ucSnr[3]));

			if(uiTmpAppData == 0xe02b0010)
			{
				if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_4_MAXIM_MAX66000)
				{
					FedmIscTagHandler_ISO14443_4_Maxim_MAX66000* pNewTagHandler = NULL;
					pNewTagHandler = new FedmIscTagHandler_ISO14443_4_Maxim_MAX66000(this, pTagHandler->m_pTabItem);
					if(pNewTagHandler != NULL)
					{
						delete pTagHandler;
						m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
						return (FedmIscTagHandler*)pNewTagHandler;
					}
				}
				else
				{
					pTagHandler->Init();
					return pTagHandler;
				}
			}
			else if(uiTmpAppData == 0xe02b0020)
			{
				if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_4_MAXIM_MAX66020)
				{
					FedmIscTagHandler_ISO14443_4_Maxim_MAX66020* pNewTagHandler = NULL;
					pNewTagHandler = new FedmIscTagHandler_ISO14443_4_Maxim_MAX66020(this, pTagHandler->m_pTabItem);
					if(pNewTagHandler != NULL)
					{
						delete pTagHandler;
						m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
						return (FedmIscTagHandler*)pNewTagHandler;
					}
				}
				else
				{
					pTagHandler->Init();
					return pTagHandler;
				}
			}
			else if(uiTmpAppData == 0xe02b0030)
			{
				if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_4_MAXIM_MAX66040)
				{
					FedmIscTagHandler_ISO14443_4_Maxim_MAX66040* pNewTagHandler = NULL;
					pNewTagHandler = new FedmIscTagHandler_ISO14443_4_Maxim_MAX66040(this, pTagHandler->m_pTabItem);
					if(pNewTagHandler != NULL)
					{
						delete pTagHandler;
						m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
						return (FedmIscTagHandler*)pNewTagHandler;
					}
				}
				else
				{
					pTagHandler->Init();
					return pTagHandler;
				}
			}
			else
			{
				if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_4)
				{
					FedmIscTagHandler_ISO14443_4* pNewTagHandler = NULL;
					pNewTagHandler = new FedmIscTagHandler_ISO14443_4(this, pTagHandler->m_pTabItem);
					if(pNewTagHandler != NULL)
					{
						delete pTagHandler;
						m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
						return (FedmIscTagHandler*)pNewTagHandler;
					}
				}
				else
				{
					pTagHandler->Init();
					return pTagHandler;
				}
			}
		}
		break;
	}

	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " Creation of TagHandler failed");
	SetLastError(FEDM_ERROR_UNKNOWN_TRANSPONDER_TYPE);
	return NULL;
}
#endif
#endif

/***************************************************************************
  Begin        		:	20.04.2009 / M. Hultsch
  
  Version       	:	04.00.08 / 05.01.2012 / M. Hultsch
						- creation of TagHandler only if card type is changed

						03.00.07 / 06.05.2009 / M. Hultsch

  Function			:	evaluate the returned SAK

  Parameters		:	int iIdx			- index in FEDM_ISOTabItem
						FedmIscTagHandler*	- pointer to tag handler base class

  Return value		:	FedmIscTagHandler*	- pointer to new tag handler (base class) of selected tag
											  or NULL	-	in error case
														-	if transponder type could not be identified
***************************************************************************/
#ifdef _FEDM_TAG_HANDLER
#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
FedmIscTagHandler* FEDM_ISCReaderModule::TagSelect_EvaFormat3(int iIdx, FedmIscTagHandler* pTagHandler)
{
	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__);

	// evaluate SAK
	unsigned char ucSAK = m_TmpData[2050];

	if( (m_ISOTable[iIdx]->m_ucOptInfo & 0x03) == 0x00 )
	{
		// cascade level 1 == 4 byte UID
		switch(ucSAK)
		{
		case 0x00:	// Infineon my-d (SLE55Rxx
			{
				if(dynamic_cast<FedmIscTagHandler_ISO14443*>(pTagHandler) == NULL)
					return NULL;

				if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_3_INFINEON_MY_D)
				{
					FedmIscTagHandler_ISO14443_3_Infineon_my_d* pNewTagHandler = NULL;
					pNewTagHandler = new FedmIscTagHandler_ISO14443_3_Infineon_my_d(this, pTagHandler->m_pTabItem);
					if(pNewTagHandler != NULL)
					{
						delete pTagHandler;
						m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
						return (FedmIscTagHandler*)pNewTagHandler;
					}
				}
				else
				{
					pTagHandler->Init();
					return pTagHandler;
				}
			}
			break;

		case 0x08:	// MIFARE 1K
					// MIFARE Plus with 2KB memory in security level 1
		case 0x09:	// MIFARE Mini
		case 0x18:	// MIFARE 4K
					// MIFARE Plus with 4KB memory in security level 1
		case 0x88:	// Infineon MIFARE 1K
			{
				if(dynamic_cast<FedmIscTagHandler_ISO14443*>(pTagHandler) == NULL)
					return NULL;

				if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_3_MIFARE_CLASSIC)
				{
					FedmIscTagHandler_ISO14443_3_MIFARE_Classic* pNewTagHandler = NULL;
					pNewTagHandler = new FedmIscTagHandler_ISO14443_3_MIFARE_Classic(this, pTagHandler->m_pTabItem);
					if(pNewTagHandler != NULL)
					{
						delete pTagHandler;
						m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
						return (FedmIscTagHandler*)pNewTagHandler;
					}
				}
				else
				{
					pTagHandler->Init();
					return pTagHandler;
				}
			}
			break;

		case 0x10: 	// MIFARE Plus CL2 with 2KB memory in security level 2
		case 0x11: 	// MIFARE Plus CL2 with 4KB memory in security level 2
			{
				if(dynamic_cast<FedmIscTagHandler_ISO14443*>(pTagHandler) == NULL)
					return NULL;

				if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_3_MIFARE_PLUS_SL2)
				{
					FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL2* pNewTagHandler = NULL;
					pNewTagHandler = new FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL2(this, pTagHandler->m_pTabItem);
					if(pNewTagHandler != NULL)
					{
						delete pTagHandler;
						m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
						return (FedmIscTagHandler*)pNewTagHandler;
					}
				}
				else
				{
					pTagHandler->Init();
					return pTagHandler;
				}
			}
			break;
		}
	}
	else if( (m_ISOTable[iIdx]->m_ucOptInfo & 0x03) == 0x01 )
	{
		// cascade level 2 == 7 byte UID
		if(m_ISOTable[iIdx]->m_ucSnr[7] == FEDM_ISC_ISO_MFR_NXP)
		{
			switch(ucSAK)
			{
			case 0x00:	// MIFARE Ultralight CL2
				{
					if(dynamic_cast<FedmIscTagHandler_ISO14443*>(pTagHandler) == NULL)
						return NULL;

					if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_3_MIFARE_ULTRALIGHT)
					{
						FedmIscTagHandler_ISO14443_3_MIFARE_Ultralight* pNewTagHandler = NULL;
						pNewTagHandler = new FedmIscTagHandler_ISO14443_3_MIFARE_Ultralight(this, pTagHandler->m_pTabItem);
						if(pNewTagHandler != NULL)
						{
							delete pTagHandler;
							m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
							return (FedmIscTagHandler*)pNewTagHandler;
						}
					}
					else
					{
						pTagHandler->Init();
						return pTagHandler;
					}
				}
				break;

			case 0x08:	// MIFARE Plus CL2 with 2KB memory in security level 1
			case 0x18:	// MIFARE Plus CL2 with 4KB memory in security level 1
				{
					if(dynamic_cast<FedmIscTagHandler_ISO14443*>(pTagHandler) == NULL)
						return NULL;

					if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_3_MIFARE_CLASSIC)
					{
						FedmIscTagHandler_ISO14443_3_MIFARE_Classic* pNewTagHandler = NULL;
						pNewTagHandler = new FedmIscTagHandler_ISO14443_3_MIFARE_Classic(this, pTagHandler->m_pTabItem);
						if(pNewTagHandler != NULL)
						{
							delete pTagHandler;
							m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
							return (FedmIscTagHandler*)pNewTagHandler;
						}
					}
					else
					{
						pTagHandler->Init();
						return pTagHandler;
					}
				}
				break;

			case 0x10: 	// MIFARE Plus CL2 with 2KB memory in security level 2
			case 0x11: 	// MIFARE Plus CL2 with 4KB memory in security level 2
				{
					if(dynamic_cast<FedmIscTagHandler_ISO14443*>(pTagHandler) == NULL)
						return NULL;

					if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_3_MIFARE_PLUS_SL2)
					{
						FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL2* pNewTagHandler = NULL;
						pNewTagHandler = new FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL2(this, pTagHandler->m_pTabItem);
						if(pNewTagHandler != NULL)
						{
							delete pTagHandler;
							m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
							return (FedmIscTagHandler*)pNewTagHandler;
						}
					}
					else
					{
						pTagHandler->Init();
						return pTagHandler;
					}
				}
				break;
			}
		}
		else if(m_ISOTable[iIdx]->m_ucSnr[7] == FEDM_ISC_ISO_MFR_INFINEON)
		{
			if((m_ISOTable[iIdx]->m_ucSnr[6] & 0x30) == 0x30)
			{
				if(dynamic_cast<FedmIscTagHandler_ISO14443*>(pTagHandler) == NULL)
					return NULL;

				if(pTagHandler->GetTagHandlerType() != FedmIscTagHandler::TYPE_ISO14443_3_INFINEON_MY_D_MOVE)
				{
					FedmIscTagHandler_ISO14443_3_Infineon_my_d_move* pNewTagHandler = NULL;
					pNewTagHandler = new FedmIscTagHandler_ISO14443_3_Infineon_my_d_move(this, pTagHandler->m_pTabItem);
					if(pNewTagHandler != NULL)
					{
						delete pTagHandler;
						m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
						return (FedmIscTagHandler*)pNewTagHandler;
					}
				}
				else
				{
					pTagHandler->Init();
					return pTagHandler;
				}
			}
			else
			{
				//No Valid INFINEON Tag found
				return pTagHandler;			
			}
		}
		else
		{	
			//No Valid Manufacturer found
			return pTagHandler;
		}
	}
	else if( (m_ISOTable[iIdx]->m_ucOptInfo & 0x03) == 0x02 )
	{
		// cascade level 3 == 10 byte UID
	}


	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " Creation of TagHandler failed");
	SetLastError(FEDM_ERROR_UNKNOWN_TRANSPONDER_TYPE);
	return NULL;
}
#endif
#endif



//////////////////////////////////////////////////////////////////////
// public functions
//////////////////////////////////////////////////////////////////////

/***************************************************************************
  Begin        		:	01.04.2009 / M. Hultsch
  
  Version       	:	03.00.12 / 23.09.2009 / M. Hultsch

  Function			:	returns the list with all tag handlers
						- tag handlers can be invalid due to its RF-Status

  Parameters		:	-

  Return value		:	map<string, FedmIscTagHandler*>*
***************************************************************************/
#ifdef _FEDM_TAG_HANDLER
FEDM_ISC_TAG_LIST* FEDM_ISCReaderModule::GetTagList()
{
	string sSnr;

	if( m_bEnableTagHandler )
	{
		m_mapTagHandler.clear();

		// build new tag handler list
		for(unsigned int uiCnt=0; uiCnt<m_uiISOTableLength; uiCnt++)
		{
			if(m_ISOTable[uiCnt]->m_pTagHandler != NULL)
			{
				if(m_ISOTable[uiCnt]->GetSnr(sSnr) != FEDM_OK)
					continue;
				
				m_mapTagHandler.insert(make_pair(sSnr, m_ISOTable[uiCnt]->m_pTagHandler));
			}
		}
	}

	return &m_mapTagHandler;
}
#endif

/***************************************************************************
  Begin        		:	24.08.2009 / M. Hultsch
  
  Version       	:	03.00.12 / 24.08.2009 / M. Hultsch

  Function			:	returns the tag handler from list

  Parameters		:	string sSnr	- serial number or UID

  Return value		:	FedmIscTagHandler* or NULL
***************************************************************************/
#ifdef _FEDM_TAG_HANDLER
#ifdef _WIN32_WCE
FedmIscTagHandler*  FEDM_ISCReaderModule::GetTagHandler(char* sSnr)
{
	string stlSnr;

	// rebuild the internal tag list
	GetTagList();

	stlSnr = sSnr;
	FEDM_ISC_TAG_LIST_ITOR itor = m_mapTagHandler.find(stlSnr);
	if(itor == m_mapTagHandler.end())
		return NULL;

	return itor->second;
}
#else
FedmIscTagHandler*  FEDM_ISCReaderModule::GetTagHandler(string sSnr)
{
	// rebuild the internal tag list
	GetTagList();

	FEDM_ISC_TAG_LIST_ITOR itor = m_mapTagHandler.find(sSnr);
	if(itor == m_mapTagHandler.end())
		return NULL;

	itor->second->m_bNonAddressedMode = false;
	return itor->second;
}
#endif
#endif


/***************************************************************************
  Begin        		:	24.08.2009 / M. Hultsch
  
  Version       	:	03.00.12 / 24.08.2009 / M. Hultsch

  Function			:	returns the tag handler with selected state from list

  Parameters		:	-

  Return value		:	FedmIscTagHandler* or NULL
***************************************************************************/
#ifdef _FEDM_TAG_HANDLER
FedmIscTagHandler*  FEDM_ISCReaderModule::GetSelectedTagHandler()
{
	FEDM_ISC_TAG_LIST_ITOR itor;

	// rebuild the internal tag list
	GetTagList();

	for(itor  = m_mapTagHandler.begin();
		itor != m_mapTagHandler.end();
		itor++ )
	{
		if(itor->second->m_pTabItem->m_bIsSelected)
			return itor->second;
	}

	return NULL;
}
#endif


/***************************************************************************
  Begin        		:	25.10.2010 / M. Hultsch
  Version       	:	04.05.02 / 02.07.2013 / M. Hultsch
						- support for new tag types: ISO 15693 NXP SLIX, SLIX-L, SLIX-S

						04.03.01 / 25.09.2012 / M. Hultsch
						- support for new tag types: ISO 15693 STM M24LRxxE-R, STM LRIS64K

						04.02.00 / 04.04.2012 / M. Hultsch
						- renamed from GetNonAddressedTagHandler to CreateNonAddressedTagHandler
		 
						04.00.10 / 13.02.2012 / M. Hultsch
						- EPC Class1 Gen2 supported
		 
						03.03.02 / 03.05.2011 / M. Hultsch

  Function			:	creates a new taghandler in tagitem with index 0

  Parameters		:	unsigned int uiTagHandlerType - one of a type from TagHandler

  Return value		:	FedmIscTagHandler or null
***************************************************************************/
#ifdef _FEDM_TAG_HANDLER
FedmIscTagHandler* FEDM_ISCReaderModule::CreateNonAddressedTagHandler(unsigned int uiTagHandlerType)
{
	FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "TagHandlerType=0x" << std::hex << std::uppercase << uiTagHandlerType);
	
	FedmIscTagHandler* pNewTagHandler = NULL;
	FEDM_ISOTabItem* tabItem = GetISOTableItem(0);

	if( !m_bEnableTagHandler )
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " Support for TagHandler disabled");
		SetLastError(FEDM_ERROR_UNSUPPORTED);
		return NULL;
	}

	if(uiTagHandlerType == FedmIscTagHandler::TYPE_BASIC)
	{
		if(tabItem->m_pTagHandler != NULL)
		{
			pNewTagHandler = tabItem->m_pTagHandler; // all taghandler are type of BASIC
		}
		else
		{
			pNewTagHandler = new FedmIscTagHandler(this, FedmIscTagHandler::TYPE_BASIC, tabItem);
		}
	}
#if !defined(_FEDM_NO_TAG_HANDLER_EPC_C1_G2)
	else if(uiTagHandlerType == FedmIscTagHandler::TYPE_EPC_CLASS1_GEN2)
	{
		if(tabItem->m_pTagHandler != NULL)
		{
			if(dynamic_cast<FedmIscTagHandler_EPC_Class1_Gen2*>(tabItem->m_pTagHandler) == NULL)
			{
				delete tabItem->m_pTagHandler;
				tabItem->m_pTagHandler = NULL;
				pNewTagHandler = new FedmIscTagHandler_EPC_Class1_Gen2(this, tabItem);
			}
			else
			{
				pNewTagHandler = tabItem->m_pTagHandler;
			}
		}
		else
		{
			pNewTagHandler = new FedmIscTagHandler_EPC_Class1_Gen2(this, tabItem);
		}
	}
	else if(uiTagHandlerType == FedmIscTagHandler::TYPE_EPC_CLASS1_GEN2_IDS_SL900A)
	{
		if(tabItem->m_pTagHandler != NULL)
		{
			if(dynamic_cast<FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A*>(tabItem->m_pTagHandler) == NULL)
			{
				delete tabItem->m_pTagHandler;
				tabItem->m_pTagHandler = NULL;
				pNewTagHandler = new FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A(this, tabItem);
			}
			else
			{
				pNewTagHandler = tabItem->m_pTagHandler;
			}
		}
		else
		{
			pNewTagHandler = new FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A(this, tabItem);
		}
	}
#endif
#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
	else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO14443 ||
			uiTagHandlerType == FedmIscTagHandler::TYPE_ISO14443_2 ||
			uiTagHandlerType == FedmIscTagHandler::TYPE_ISO14443_2_INNOVISION_JEWEL ||
			uiTagHandlerType == FedmIscTagHandler::TYPE_ISO14443_2_STM_SR176 ||
			uiTagHandlerType == FedmIscTagHandler::TYPE_ISO14443_2_STM_SRIxxx ||
			uiTagHandlerType == FedmIscTagHandler::TYPE_ISO14443_3 ||
			uiTagHandlerType == FedmIscTagHandler::TYPE_ISO14443_3_INFINEON_MY_D ||
			uiTagHandlerType == FedmIscTagHandler::TYPE_ISO14443_3_INFINEON_MY_D_MOVE ||
			uiTagHandlerType == FedmIscTagHandler::TYPE_ISO14443_3_MIFARE_CLASSIC ||
			uiTagHandlerType == FedmIscTagHandler::TYPE_ISO14443_3_MIFARE_PLUS_SL1 ||
			uiTagHandlerType == FedmIscTagHandler::TYPE_ISO14443_3_MIFARE_PLUS_SL2 ||
			uiTagHandlerType == FedmIscTagHandler::TYPE_ISO14443_3_MIFARE_ULTRALIGHT ||
			uiTagHandlerType == FedmIscTagHandler::TYPE_ISO14443_4 ||
			uiTagHandlerType == FedmIscTagHandler::TYPE_ISO14443_4_MAXIM ||
			uiTagHandlerType == FedmIscTagHandler::TYPE_ISO14443_4_MAXIM_MAX66000 ||
			uiTagHandlerType == FedmIscTagHandler::TYPE_ISO14443_4_MAXIM_MAX66020 ||
			uiTagHandlerType == FedmIscTagHandler::TYPE_ISO14443_4_MAXIM_MAX66040 ||
			uiTagHandlerType == FedmIscTagHandler::TYPE_ISO14443_4_MIFARE_DESFIRE ||
			uiTagHandlerType == FedmIscTagHandler::TYPE_ISO14443_4_MIFARE_PLUS_SL1 ||
			uiTagHandlerType == FedmIscTagHandler::TYPE_ISO14443_4_MIFARE_PLUS_SL2 ||
			uiTagHandlerType == FedmIscTagHandler::TYPE_ISO14443_4_MIFARE_PLUS_SL3 )
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " Not applicable for ISO 14443 and EPC C1G2 TagHandler ");
		SetLastError(FEDM_ERROR_UNSUPPORTED);
		return NULL;
	}
#endif

#if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)
	else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693)
	{
		if(tabItem->m_pTagHandler != NULL)
		{
			if(dynamic_cast<FedmIscTagHandler_ISO15693*>(tabItem->m_pTagHandler) == NULL)
			{
				delete tabItem->m_pTagHandler;
				tabItem->m_pTagHandler = NULL;
				pNewTagHandler = new FedmIscTagHandler_ISO15693(this, tabItem);
			}
			else
			{
				pNewTagHandler = tabItem->m_pTagHandler;
			}
		}
		else
		{
			pNewTagHandler = new FedmIscTagHandler_ISO15693(this, tabItem);
		}
	}
	else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_EM ||
			uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_Fujitsu ||
			uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_MAXIM ||
			uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_NXP ||
			uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_STM ||
			uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_TI ||
			uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_TI_Tag_it_HFI_Pro )
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " Not applicable for this ISO 15693 TagHandler: " << std::hex << std::uppercase << uiTagHandlerType);
		SetLastError(FEDM_ERROR_UNSUPPORTED);
		return NULL;
	}
	
	
	else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_EM_4034)
	{
		if(tabItem->m_pTagHandler != NULL)
		{
			if(dynamic_cast<FedmIscTagHandler_ISO15693_EM_4034*>(tabItem->m_pTagHandler) == NULL)
			{
				delete tabItem->m_pTagHandler;
				tabItem->m_pTagHandler = NULL;
				pNewTagHandler = new FedmIscTagHandler_ISO15693_EM_4034(this, tabItem);
			}
			else
			{
				pNewTagHandler = tabItem->m_pTagHandler;
			}
		}
		else
		{
			pNewTagHandler = new FedmIscTagHandler_ISO15693_EM_4034(this, tabItem);
		}
	}
	else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_Fujitsu_MB89R1xx)
	{
		if(tabItem->m_pTagHandler != NULL)
		{
			if(dynamic_cast<FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx*>(tabItem->m_pTagHandler) == NULL)
			{
				delete tabItem->m_pTagHandler;
				tabItem->m_pTagHandler = NULL;
				pNewTagHandler = new FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx(this, tabItem);
			}
			else
			{
				pNewTagHandler = tabItem->m_pTagHandler;
			}
		}
		else
		{
			pNewTagHandler = new FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx(this, tabItem);
		}
	}
	else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_IDS_SL13A)
	{
		if(tabItem->m_pTagHandler != NULL)
		{
			if(dynamic_cast<FedmIscTagHandler_ISO15693_IDS_SL13A*>(tabItem->m_pTagHandler) == NULL)
			{
				delete tabItem->m_pTagHandler;
				tabItem->m_pTagHandler = NULL;
				pNewTagHandler = new FedmIscTagHandler_ISO15693_IDS_SL13A(this, tabItem);
			}
			else
			{
				pNewTagHandler = tabItem->m_pTagHandler;
			}
		}
		else
		{
			pNewTagHandler = new FedmIscTagHandler_ISO15693_IDS_SL13A(this, tabItem);
		}
	}
	else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_Infineon)
	{
		if(tabItem->m_pTagHandler != NULL)
		{
			if(dynamic_cast<FedmIscTagHandler_ISO15693_Infineon*>(tabItem->m_pTagHandler) == NULL)
			{
				delete tabItem->m_pTagHandler;
				tabItem->m_pTagHandler = NULL;
				pNewTagHandler = new FedmIscTagHandler_ISO15693_Infineon(this, tabItem);
			}
			else
			{
				pNewTagHandler = tabItem->m_pTagHandler;
			}
		}
		else
		{
			pNewTagHandler = new FedmIscTagHandler_ISO15693_Infineon(this, tabItem);
		}
	}
	else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_Infineon_my_d)
	{
		if(tabItem->m_pTagHandler != NULL)
		{
			if(dynamic_cast<FedmIscTagHandler_ISO15693_Infineon_my_d*>(tabItem->m_pTagHandler) == NULL)
			{
				delete tabItem->m_pTagHandler;
				tabItem->m_pTagHandler = NULL;
				pNewTagHandler = new FedmIscTagHandler_ISO15693_Infineon_my_d(this, tabItem);
			}
			else
			{
				pNewTagHandler = tabItem->m_pTagHandler;
			}
		}
		else
		{
			pNewTagHandler = new FedmIscTagHandler_ISO15693_Infineon_my_d(this, tabItem);
		}
	}
	else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_KSW)
	{
		if(tabItem->m_pTagHandler != NULL)
		{
			if(dynamic_cast<FedmIscTagHandler_ISO15693_KSW*>(tabItem->m_pTagHandler) == NULL)
			{
				delete tabItem->m_pTagHandler;
				tabItem->m_pTagHandler = NULL;
				pNewTagHandler = new FedmIscTagHandler_ISO15693_KSW(this, tabItem);
			}
			else
			{
				pNewTagHandler = tabItem->m_pTagHandler;
			}
		}
		else
		{
			pNewTagHandler = new FedmIscTagHandler_ISO15693_KSW(this, tabItem);
		}
	}
	else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_MAXIM_MAX66100)
	{
		if(tabItem->m_pTagHandler != NULL)
		{
			if(dynamic_cast<FedmIscTagHandler_ISO15693_Maxim_MAX66100*>(tabItem->m_pTagHandler) == NULL)
			{
				delete tabItem->m_pTagHandler;
				tabItem->m_pTagHandler = NULL;
				pNewTagHandler = new FedmIscTagHandler_ISO15693_Maxim_MAX66100(this, tabItem);
			}
			else
			{
				pNewTagHandler = tabItem->m_pTagHandler;
			}
		}
		else
		{
			pNewTagHandler = new FedmIscTagHandler_ISO15693_Maxim_MAX66100(this, tabItem);
		}
	}
	else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_MAXIM_MAX66120)
	{
		if(tabItem->m_pTagHandler != NULL)
		{
			if(dynamic_cast<FedmIscTagHandler_ISO15693_Maxim_MAX66120*>(tabItem->m_pTagHandler) == NULL)
			{
				delete tabItem->m_pTagHandler;
				tabItem->m_pTagHandler = NULL;
				pNewTagHandler = new FedmIscTagHandler_ISO15693_Maxim_MAX66120(this, tabItem);
			}
			else
			{
				pNewTagHandler = tabItem->m_pTagHandler;
			}
		}
		else
		{
			pNewTagHandler = new FedmIscTagHandler_ISO15693_Maxim_MAX66120(this, tabItem);
		}
	}
	else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_MAXIM_MAX66140)
	{
		if(tabItem->m_pTagHandler != NULL)
		{
			if(dynamic_cast<FedmIscTagHandler_ISO15693_Maxim_MAX66140*>(tabItem->m_pTagHandler) == NULL)
			{
				delete tabItem->m_pTagHandler;
				tabItem->m_pTagHandler = NULL;
				pNewTagHandler = new FedmIscTagHandler_ISO15693_Maxim_MAX66140(this, tabItem);
			}
			else
			{
				pNewTagHandler = tabItem->m_pTagHandler;
			}
		}
		else
		{
			pNewTagHandler = new FedmIscTagHandler_ISO15693_Maxim_MAX66140(this, tabItem);
		}
	}
	else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_NXP_ICODE_SLI)
	{
		if(tabItem->m_pTagHandler != NULL)
		{
			if(dynamic_cast<FedmIscTagHandler_ISO15693_NXP_ICODE_SLI*>(tabItem->m_pTagHandler) == NULL)
			{
				delete tabItem->m_pTagHandler;
				tabItem->m_pTagHandler = NULL;
				pNewTagHandler = new FedmIscTagHandler_ISO15693_NXP_ICODE_SLI(this, tabItem);
			}
			else
			{
				pNewTagHandler = tabItem->m_pTagHandler;
			}
		}
		else
		{
			pNewTagHandler = new FedmIscTagHandler_ISO15693_NXP_ICODE_SLI(this, tabItem);
		}
	}
	else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_NXP_ICODE_SLI_L)
	{
		if(tabItem->m_pTagHandler != NULL)
		{
			if(dynamic_cast<FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_L*>(tabItem->m_pTagHandler) == NULL)
			{
				delete tabItem->m_pTagHandler;
				tabItem->m_pTagHandler = NULL;
				pNewTagHandler = new FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_L(this, tabItem);
			}
			else
			{
				pNewTagHandler = tabItem->m_pTagHandler;
			}
		}
		else
		{
			pNewTagHandler = new FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_L(this, tabItem);
		}
	}
	else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_NXP_ICODE_SLI_S)
	{
		if(tabItem->m_pTagHandler != NULL)
		{
			if(dynamic_cast<FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_S*>(tabItem->m_pTagHandler) == NULL)
			{
				delete tabItem->m_pTagHandler;
				tabItem->m_pTagHandler = NULL;
				pNewTagHandler = new FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_S(this, tabItem);
			}
			else
			{
				pNewTagHandler = tabItem->m_pTagHandler;
			}
		}
		else
		{
			pNewTagHandler = new FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_S(this, tabItem);
		}
	}
	else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_NXP_ICODE_SLIX)
	{
		if(tabItem->m_pTagHandler != NULL)
		{
			if(dynamic_cast<FedmIscTagHandler_ISO15693_NXP_ICODE_SLIX*>(tabItem->m_pTagHandler) == NULL)
			{
				delete tabItem->m_pTagHandler;
				tabItem->m_pTagHandler = NULL;
				pNewTagHandler = new FedmIscTagHandler_ISO15693_NXP_ICODE_SLIX(this, tabItem);
			}
			else
			{
				pNewTagHandler = tabItem->m_pTagHandler;
			}
		}
		else
		{
			pNewTagHandler = new FedmIscTagHandler_ISO15693_NXP_ICODE_SLIX(this, tabItem);
		}
	}
	else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_NXP_ICODE_SLIX_L)
	{
		if(tabItem->m_pTagHandler != NULL)
		{
			if(dynamic_cast<FedmIscTagHandler_ISO15693_NXP_ICODE_SLIX_L*>(tabItem->m_pTagHandler) == NULL)
			{
				delete tabItem->m_pTagHandler;
				tabItem->m_pTagHandler = NULL;
				pNewTagHandler = new FedmIscTagHandler_ISO15693_NXP_ICODE_SLIX_L(this, tabItem);
			}
			else
			{
				pNewTagHandler = tabItem->m_pTagHandler;
			}
		}
		else
		{
			pNewTagHandler = new FedmIscTagHandler_ISO15693_NXP_ICODE_SLIX_L(this, tabItem);
		}
	}
	else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_NXP_ICODE_SLIX_S)
	{
		if(tabItem->m_pTagHandler != NULL)
		{
			if(dynamic_cast<FedmIscTagHandler_ISO15693_NXP_ICODE_SLIX_S*>(tabItem->m_pTagHandler) == NULL)
			{
				delete tabItem->m_pTagHandler;
				tabItem->m_pTagHandler = NULL;
				pNewTagHandler = new FedmIscTagHandler_ISO15693_NXP_ICODE_SLIX_S(this, tabItem);
			}
			else
			{
				pNewTagHandler = tabItem->m_pTagHandler;
			}
		}
		else
		{
			pNewTagHandler = new FedmIscTagHandler_ISO15693_NXP_ICODE_SLIX_S(this, tabItem);
		}
	}
	else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_STM_LRI2K)
	{
		if(tabItem->m_pTagHandler != NULL)
		{
			if(dynamic_cast<FedmIscTagHandler_ISO15693_STM_LRI2K*>(tabItem->m_pTagHandler) == NULL)
			{
				delete tabItem->m_pTagHandler;
				tabItem->m_pTagHandler = NULL;
				pNewTagHandler = new FedmIscTagHandler_ISO15693_STM_LRI2K(this, tabItem);
			}
			else
			{
				pNewTagHandler = tabItem->m_pTagHandler;
			}
		}
		else
		{
			pNewTagHandler = new FedmIscTagHandler_ISO15693_STM_LRI2K(this, tabItem);
		}
	}
	else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_STM_LRIS2K)
	{
		if(tabItem->m_pTagHandler != NULL)
		{
			if(dynamic_cast<FedmIscTagHandler_ISO15693_STM_LRIS2K*>(tabItem->m_pTagHandler) == NULL)
			{
				delete tabItem->m_pTagHandler;
				tabItem->m_pTagHandler = NULL;
				pNewTagHandler = new FedmIscTagHandler_ISO15693_STM_LRIS2K(this, tabItem);
			}
			else
			{
				pNewTagHandler = tabItem->m_pTagHandler;
			}
		}
		else
		{
			pNewTagHandler = new FedmIscTagHandler_ISO15693_STM_LRIS2K(this, tabItem);
		}
	}
	else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_STM_LRIS64K)
	{
		if(tabItem->m_pTagHandler != NULL)
		{
			if(dynamic_cast<FedmIscTagHandler_ISO15693_STM_LRIS64K*>(tabItem->m_pTagHandler) == NULL)
			{
				delete tabItem->m_pTagHandler;
				tabItem->m_pTagHandler = NULL;
				pNewTagHandler = new FedmIscTagHandler_ISO15693_STM_LRIS64K(this, tabItem);
			}
			else
			{
				pNewTagHandler = tabItem->m_pTagHandler;
			}
		}
		else
		{
			pNewTagHandler = new FedmIscTagHandler_ISO15693_STM_LRIS64K(this, tabItem);
		}
	}
	else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_STM_M24LR64R)
	{
		if(tabItem->m_pTagHandler != NULL)
		{
			if(dynamic_cast<FedmIscTagHandler_ISO15693_STM_M24LR64R*>(tabItem->m_pTagHandler) == NULL)
			{
				delete tabItem->m_pTagHandler;
				tabItem->m_pTagHandler = NULL;
				pNewTagHandler = new FedmIscTagHandler_ISO15693_STM_M24LR64R(this, tabItem);
			}
			else
			{
				pNewTagHandler = tabItem->m_pTagHandler;
			}
		}
		else
		{
			pNewTagHandler = new FedmIscTagHandler_ISO15693_STM_M24LR64R(this, tabItem);
		}
	}
	else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_STM_M24LRxxER)
	{
		if(tabItem->m_pTagHandler != NULL)
		{
			if(dynamic_cast<FedmIscTagHandler_ISO15693_STM_M24LRxxER*>(tabItem->m_pTagHandler) == NULL)
			{
				delete tabItem->m_pTagHandler;
				tabItem->m_pTagHandler = NULL;
				pNewTagHandler = new FedmIscTagHandler_ISO15693_STM_M24LRxxER(this, tabItem);
			}
			else
			{
				pNewTagHandler = tabItem->m_pTagHandler;
			}
		}
		else
		{
			pNewTagHandler = new FedmIscTagHandler_ISO15693_STM_M24LRxxER(this, tabItem);
		}
	}
	else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_TI_Tag_it_HFI_Plus)
	{
		if(tabItem->m_pTagHandler != NULL)
		{
			if(dynamic_cast<FedmIscTagHandler_ISO15693_TI_Tag_it_HFI_Plus*>(tabItem->m_pTagHandler) == NULL)
			{
				delete tabItem->m_pTagHandler;
				tabItem->m_pTagHandler = NULL;
				pNewTagHandler = new FedmIscTagHandler_ISO15693_TI_Tag_it_HFI_Plus(this, tabItem);
			}
			else
			{
				pNewTagHandler = tabItem->m_pTagHandler;
			}
		}
		else
		{
			pNewTagHandler = new FedmIscTagHandler_ISO15693_TI_Tag_it_HFI_Plus(this, tabItem);
		}
	}
#endif
#if !defined(_FEDM_NO_TAG_HANDLER_ISO18000_3M3)
	else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO18000_3M3)
	{
		if(tabItem->m_pTagHandler != NULL)
		{
			if(dynamic_cast<FedmIscTagHandler_ISO18000_3M3*>(tabItem->m_pTagHandler) == NULL)
			{
				delete tabItem->m_pTagHandler;
				tabItem->m_pTagHandler = NULL;
				pNewTagHandler = new FedmIscTagHandler_ISO18000_3M3(this, tabItem);
			}
			else
			{
				pNewTagHandler = tabItem->m_pTagHandler;
			}
		}
		else
		{
			pNewTagHandler = new FedmIscTagHandler_ISO18000_3M3(this, tabItem);
		}
	}
#endif
	else
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " Unknown TagHandler-Type " << std::hex << std::uppercase << uiTagHandlerType);
		SetLastError(FEDM_ERROR_UNKNOWN_TAG_HANDLER_TYPE);
		return NULL;
	}

	if (pNewTagHandler != NULL)
	{
		// insert new taghandler and mark it for use in non-addressed mode
		tabItem->m_pTagHandler = pNewTagHandler;
		pNewTagHandler->m_bNonAddressedMode = true;
	}
	else
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " Creation of TagHandler failed - no more mem");
		SetLastError(FEDM_ERROR_NO_MORE_MEM);
	}

	return pNewTagHandler;
}
#endif

/***************************************************************************
  Begin        		:	25.10.2010 / M. Hultsch
  Version       	:	03.02.07 / 25.10.2010 / M. Hultsch

  Function			:	convert a taghandler to the specified type

  Parameters		:	FedmIscTagHandler*	pTagHandler			pointer to the source tag handler
                        unsigned char ucMfrCode					Manufacturer Code
						uiTagHandlerType						target type

  Return value		:	FedmIscTagHandler or null
***************************************************************************/
#ifdef _FEDM_TAG_HANDLER
#if !defined(_FEDM_NO_TAG_HANDLER_EPC_C1_G2)
FedmIscTagHandler* FEDM_ISCReaderModule::Convert_EPC_C1_G2_TagHandler(FedmIscTagHandler* pTagHandler, unsigned char ucMfrCode, unsigned int uiTagHandlerType)
{
	FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "MfrCode=0x" << std::hex <<::std::uppercase << (int)ucMfrCode << "TagHandlerType=0x" << std::hex <<::std::uppercase << uiTagHandlerType);
	
	if( !m_bEnableTagHandler )
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " Support for TagHandler disabled");
		SetLastError(FEDM_ERROR_UNSUPPORTED);
		return NULL;
	}

	if(pTagHandler == NULL)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " TagHandler is NULL");
		SetLastError(FEDM_ERROR_NULL_POINTER);
		return NULL;
	}

	if(dynamic_cast<FedmIscTagHandler_EPC_Class1_Gen2*>(pTagHandler) == NULL)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " TagHandler is not of type EPC C1G2");
		SetLastError(FEDM_ERROR_UNSUPPORTED_TRANSPONDER_TYPE);
		return NULL;
	}

	int iIdx = pTagHandler->m_pTabItem->m_uiTabIndex;

	switch(ucMfrCode)
	{
	case FEDM_ISC_ISO_MFR_IDS:
		if(uiTagHandlerType == FedmIscTagHandler::TYPE_EPC_CLASS1_GEN2_IDS_SL900A)
		{
			FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A* pNewTagHandler = NULL;
			pNewTagHandler = new FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A(this, pTagHandler->m_pTabItem);
			if(pNewTagHandler != NULL)
			{
				delete pTagHandler;
				m_ISOTable[iIdx]->m_pTagHandler = (FedmIscTagHandler*)pNewTagHandler;
				return (FedmIscTagHandler*)pNewTagHandler;
			}
		}
		break;

	default:
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " Unsupported manufacturer code");
		break;
	}

	SetLastError(FEDM_ERROR_PARAMETER);
	return NULL;
}
#endif
#endif


//////////////////////////////////////////////////////////////////////
// methods for executing asynchronous tasks
//////////////////////////////////////////////////////////////////////

/***************************************************************************
  Begin        		:	20.02.2006 / M. Hultsch

  Version       	:	04.02.08 / 13.08.2012 / M. Hultsch
						- support for events from People Counter

						03.00.13 / 24.09.2009 / M. Hultsch
						- add of MAX event handling

						02.04.04 / 25.08.2006 / M. Hultsch

  Function			:	function to execute an asynchronous task

  Parameters		:	initialization structure

  Return value		:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCReaderModule::StartAsyncTask(FEDM_TASK_INIT* pInit)
{
	int iBack = 0;
	string sDbgMsg;
	FEISC_TASK_INIT		TI;

	if(pInit == NULL)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ <<  " pInit=NULL");		
		FEDM_RETURN(FEDM_ERROR_NULL_POINTER);
	}

	// filter unsupported flags
	switch(pInit->uiFlag)
	{
	case FEDM_TASKCB1:
		sDbgMsg = "Flag=CB1";
		if(pInit->cbFct1 == NULL)
			iBack = FEDM_ERROR_NULL_POINTER;
		break;
	case FEDM_TASKCB2:
		sDbgMsg = "Flag=CB2";
		if(pInit->cbFct2 == NULL)
			iBack = FEDM_ERROR_NULL_POINTER;
		break;
#ifdef _FEDM_WINDOWS
	case FEDM_TASKCB_NET1:
		sDbgMsg = "Flag=CBNet1";
		if(pInit->cbFctNET1 == NULL)
			iBack = FEDM_ERROR_NULL_POINTER;
		break;
#endif
	default:
		iBack = FEDM_ERROR_UNSUPPORTED;
	}

	// filter unsupported events
	switch(pInit->uiUse)
	{
	case FEDM_TASKID_FIRST_NEW_TAG:
		sDbgMsg += ", Use=FirstNewTag";
		switch(pInit->uiFlag)
		{
		case FEDM_TASKCB1:
		case FEDM_TASKCB_NET1:
			break;
		default:
			iBack = FEDM_ERROR_UNSUPPORTED;
		}
		break;

	case FEDM_TASKID_EVERY_NEW_TAG:
		sDbgMsg += ", Use=EveryNewTag";
		switch(pInit->uiFlag)
		{
		case FEDM_TASKCB1:
		case FEDM_TASKCB_NET1:
			break;
		default:
			iBack = FEDM_ERROR_UNSUPPORTED;
		}
		break;

	case FEDM_TASKID_NOTIFICATION:
		sDbgMsg += ", Use=Notification";
		switch(pInit->uiFlag)
		{
		case FEDM_TASKCB1:
		case FEDM_TASKCB2:
		case FEDM_TASKCB_NET1: // TODO - verify
			break;
		default:
			iBack = FEDM_ERROR_UNSUPPORTED;
		}
		break;

/*	case FEDM_TASKID_PEOPLE_COUNTER_EVENT:
		sDbgMsg += ", Use=PeopleCounterEvent";
		switch(pInit->uiFlag)
		{
		case FEDM_TASKCB1:
		case FEDM_TASKCB2:
		case FEDM_TASKCB_NET1: // TODO - verify
			break;
		default:
			iBack = FEDM_ERROR_UNSUPPORTED;
		}
		break;*/

	default:
		iBack = FEDM_ERROR_UNSUPPORTED;
	}

	if(pInit->bCryptoMode)
		sDbgMsg += ", Crypto=On";
	else
		sDbgMsg += ", Crypto=Off";

	if(pInit->bKeepAlive)
		sDbgMsg += ", KeepAlive=On";
	else
		sDbgMsg += ", KeepAlive=Off";

	FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_EVENTING, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, sDbgMsg);

	if(iBack < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(iBack));
		FEDM_RETURN(iBack);
	}

	memset(&m_TaskInit, 0, sizeof(FEDM_TASK_INIT));
	m_TaskInit.uiFlag			= pInit->uiFlag;
	m_TaskInit.uiUse			= pInit->uiUse;
	m_TaskInit.pAny				= pInit->pAny;
	m_TaskInit.iConnectByHost	= pInit->iConnectByHost;
	m_TaskInit.iNotifyWithAck	= pInit->iNotifyWithAck;
	m_TaskInit.uiTimeout		= pInit->uiTimeout;
	if(pInit->bCryptoMode)
	{
		m_TaskInit.bCryptoMode = true;
		if(	pInit->uiAuthentKeyLength != 16 &&
			pInit->uiAuthentKeyLength != 24 &&
			pInit->uiAuthentKeyLength != 32)
		{
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_PARAMETER));
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		m_TaskInit.uiAuthentKeyLength	= pInit->uiAuthentKeyLength;
		memcpy(m_TaskInit.ucAuthentKey, pInit->ucAuthentKey, pInit->uiAuthentKeyLength);
	}
	if(pInit->bKeepAlive)
	{
		m_TaskInit.bKeepAlive = true;
		m_TaskInit.uiKeepAliveIdleTime = pInit->uiKeepAliveIdleTime;
		m_TaskInit.uiKeepAliveProbeCount = pInit->uiKeepAliveProbeCount;
		m_TaskInit.uiKeepAliveIntervalTime = pInit->uiKeepAliveIntervalTime;
	}

	// flag dependent initialization
	switch(pInit->uiFlag)
	{
	case FEDM_TASKCB1:
		m_TaskInit.cbFct1	= pInit->cbFct1;
		break;
	case FEDM_TASKCB2:
		m_TaskInit.cbFct2	= pInit->cbFct2;
		break;
#ifdef _FEDM_WINDOWS
	case FEDM_TASKCB_NET1:
		m_TaskInit.cbFctNET1	= pInit->cbFctNET1;
		break;
#endif
	}

	memset(&TI, 0, sizeof(FEISC_TASK_INIT));

	// event dependent initialization
	switch(pInit->uiUse)
	{
	case FEDM_TASKID_FIRST_NEW_TAG:
		TI.uiFlag		= FEISC_TASKCB_1;
#if defined(__BORLANDC__)
		TI.cbFct1		= (void(*)(void*, int, int, int, unsigned char, unsigned char*, int))cbsTaskRsp1;		// static member function
#else
		TI.cbFct1		= cbsTaskRsp1;		// static member function
#endif
		TI.uiChannelType = FEISC_TASK_CHANNEL_TYPE_AS_OPEN;
		TI.ucBusAdr		= GetBusAddress();
		TI.iConnectByHost = 0; // unused
		memset(TI.cIPAdr, 0, 16);
		TI.iPortNr		= 0;
		TI.pAny			= (void*)this;		// backpointer to caller object
		TI.uiTimeout	= m_TaskInit.uiTimeout;
		if(m_TaskInit.bCryptoMode)
		{
			TI.bCryptoMode = true;
			TI.uiAuthentKeyLength	= m_TaskInit.uiAuthentKeyLength;
			memcpy(TI.ucAuthentKey, m_TaskInit.ucAuthentKey, m_TaskInit.uiAuthentKeyLength);
		}
		FEDM_CHK2(iBack, SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char)0x01));

		FEDM_CHK2(iBack, FEISC_StartAsyncTask(GetReaderHnd(), FEISC_TASKID_FIRST_NEW_TAG, &TI, NULL));
		break;

	case FEDM_TASKID_EVERY_NEW_TAG:
		TI.uiFlag		= FEISC_TASKCB_1;
#if defined(__BORLANDC__)
		TI.cbFct1		= (void(*)(void*, int, int, int, unsigned char, unsigned char*, int))cbsTaskRsp1;		// static member function
#else
		TI.cbFct1		= cbsTaskRsp1;		// static member function
#endif
		TI.uiChannelType = FEISC_TASK_CHANNEL_TYPE_AS_OPEN;
		TI.ucBusAdr		= GetBusAddress();
		TI.iConnectByHost = 0; // unused
		memset(TI.cIPAdr, 0, 16);
		TI.iPortNr		= 0;
		TI.pAny			= (void*)this;		// backpointer to caller object
		TI.uiTimeout	= m_TaskInit.uiTimeout;
		if(m_TaskInit.bCryptoMode)
		{
			TI.bCryptoMode = true;
			TI.uiAuthentKeyLength	= m_TaskInit.uiAuthentKeyLength;
			memcpy(TI.ucAuthentKey, m_TaskInit.ucAuthentKey, m_TaskInit.uiAuthentKeyLength);
		}
		FEDM_CHK2(iBack, SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char)0x01));

		FEDM_CHK2(iBack, FEISC_StartAsyncTask(GetReaderHnd(), FEISC_TASKID_EVERY_NEW_TAG, &TI, NULL));
		break;

	case FEDM_TASKID_NOTIFICATION:
	//case FEDM_TASKID_PEOPLE_COUNTER_EVENT:
		TI.uiFlag		= FEISC_TASKCB_2;
#if defined(__BORLANDC__)
		TI.cbFct2		= (void(*)(void*, int, int, int, unsigned char, unsigned char*, int, char*, int))cbsTaskRsp2;		// static member function
#else
		TI.cbFct2		= cbsTaskRsp2;		// static member function
#endif
		TI.uiChannelType = FEISC_TASK_CHANNEL_TYPE_NEW_TCP;
		TI.ucBusAdr		= GetBusAddress();
		TI.iConnectByHost = pInit->iConnectByHost;
		strcpy(TI.cIPAdr, pInit->cIPAdr);
		TI.iPortNr		= pInit->iPortNr;
		TI.pAny			= (void*)this;		// backpointer to caller object
		TI.uiTimeout	= m_TaskInit.uiTimeout;
		TI.iNotifyWithAck = pInit->iNotifyWithAck;
		if(m_TaskInit.bCryptoMode)
		{
			TI.bCryptoMode = true;
			TI.uiAuthentKeyLength	= m_TaskInit.uiAuthentKeyLength;
			memcpy(TI.ucAuthentKey, m_TaskInit.ucAuthentKey, m_TaskInit.uiAuthentKeyLength);
		}
		if(m_TaskInit.bKeepAlive)
		{
			TI.bKeepAlive = true;
			TI.uiKeepAliveIdleTime = m_TaskInit.uiKeepAliveIdleTime;
			TI.uiKeepAliveProbeCount = m_TaskInit.uiKeepAliveProbeCount;
			TI.uiKeepAliveIntervalTime = m_TaskInit.uiKeepAliveIntervalTime;
		}

		if(pInit->uiUse == FEDM_TASKID_NOTIFICATION)
		{
			FEDM_CHK2(iBack, FEISC_StartAsyncTask(GetReaderHnd(), FEISC_TASKID_NOTIFICATION, &TI, NULL));
		}
		/*else if(pInit->uiUse == FEDM_TASKID_PEOPLE_COUNTER_EVENT)
		{
			FEDM_CHK2(iBack, FEISC_StartAsyncTask(GetReaderHnd(), FEISC_TASKID_PEOPLE_COUNTER_EVENT, &TI, NULL));
		}*/
		break;
	}

	m_bActiveAsyncTask = true; // asynchronous task is started successfully

	FEDM_RETURN(FEDM_OK);
}

int FEDM_ISCReaderModule::CancelAsyncTask()
{
	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_EVENTING, __FUNCTION__);

	int iBack = 0;

	if(m_bActiveAsyncTask)
	{
		FEDM_CHK2(iBack, FEISC_CancelAsyncTask(GetReaderHnd()));
		m_bActiveAsyncTask = false;
	}

	FEDM_RETURN(FEDM_OK);
}

int FEDM_ISCReaderModule::TriggerAsyncTask()
{
	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_EVENTING, __FUNCTION__);

	int iBack = 0;

	if(m_bActiveAsyncTask)
	{
		FEDM_CHK2(iBack, FEISC_TriggerAsyncTask(GetReaderHnd()));
	}

	FEDM_RETURN(FEDM_OK);
}




//////////////////////////////////////////////////////////////////////
// parameter exchange with OBID libraries
//////////////////////////////////////////////////////////////////////

/***************************************************************************
  Begin        		:	06.11.2003 / M. Hultsch

  Version       	:	03.02.03 / 29.07.2010 / M. Hultsch
						- PortPrefix for FECOM under Linux to support Bluetooth

						03.01.06 / 31.05.2010 / M. Hultsch
						- ExclusiveAccess for FEUSB

						02.03.05 / 14.11.2005 / M. Hultsch
						- Modification for uClinux

						02.03.00 / 04.08.2004 / M. Hultsch
						- Extension for USB
						- SetRTS, SetDTR for serial port

						01.09.00 / 13.11.2003 / M. Hultsch

  Function			:	sets one port parameter in port library

  Parameters		:	char* cPara		-	pointer to parameter string (see port manual)
						char* cValue	-	pointer to value string (see port manual)

  Return value		:	FEDM_OK or error code (<0)
***************************************************************************/
int FEDM_ISCReaderModule::SetPortPara(char* cPara, char* cValue)
{
	FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_ADMIN_SET_GET, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "Para=" << cPara << ", Value=" << cValue);
	
	int iBack = 0;

#if defined(_FEDM_COM_SUPPORT)
	// it's a global setting
	if(strcmp(cPara, "PortPrefix")==0)
	{
#if !defined(_FEDM_LINUX)
		return 0;
#else
#if !defined(_FEDM_SUPPORT_SLINK)
		LPFN_FECOM_SET_PORT_PARA lpfnSetPortPara = (LPFN_FECOM_SET_PORT_PARA)GetFeComFunction(FECOM_SET_PORT_PARA);
		FEDM_CHK5(lpfnSetPortPara);
		iBack = lpfnSetPortPara(0, cPara, cValue);
		FEDM_RETURN(iBack);
#else
		iBack = FECOM_SetPortPara(0, cPara, cValue);
		FEDM_RETURN(iBack);
#endif
#endif // #if !defined(_FEDM_LINUX)
	}
#endif // #if defined(_FEDM_COM_SUPPORT)


#ifdef _FEDM_USB_SUPPORT
	// it's a global setting
	if(strcmp(cPara, "ExclusiveAccess")==0)
	{
#if !defined(_FEDM_SUPPORT_SLINK)
		LPFN_FEUSB_SET_DEVICE_PARA lpfnSetDevicePara = (LPFN_FEUSB_SET_DEVICE_PARA)GetFeUsbFunction(FEUSB_SET_DEVICE_PARA);
		FEDM_CHK5(lpfnSetDevicePara);
		iBack = lpfnSetDevicePara(0, cPara, cValue);
		FEDM_RETURN(iBack);
#else
		iBack = FEUSB_SetDevicePara(0, cPara, cValue);
		FEDM_RETURN(iBack);
#endif
	}
#endif // #ifdef _FEDM_USB_SUPPORT


	if(m_iCommMode == FEDM_COMM_MODE_UNDEF)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_NOT_CONNECTED));
		FEDM_RETURN(FEDM_ERROR_NOT_CONNECTED);
	}

	int iPortHnd = GetPortHnd();
	if(iPortHnd <= 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_NOT_CONNECTED));
		FEDM_RETURN(FEDM_ERROR_NOT_CONNECTED);
	}


#ifdef _FEDM_COM_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
	LPFN_FECOM_SET_PORT_PARA lpfnSetPortPara = (LPFN_FECOM_SET_PORT_PARA)GetFeComFunction(FECOM_SET_PORT_PARA);
	LPFN_FECOM_DO_PORT_CMD lpfnDoPortCmd = (LPFN_FECOM_DO_PORT_CMD)GetFeComFunction(FECOM_DO_PORT_CMD);
	if(m_iCommMode == FEDM_COMM_MODE_SERIAL)
	{
		FEDM_CHK5(lpfnSetPortPara);
		FEDM_CHK5(lpfnDoPortCmd);
	}
#endif
#endif

#ifdef _FEDM_USB_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
	LPFN_FEUSB_SET_DEVICE_PARA lpfnSetDevicePara = (LPFN_FEUSB_SET_DEVICE_PARA)GetFeUsbFunction(FEUSB_SET_DEVICE_PARA);
	if(m_iCommMode == FEDM_COMM_MODE_USB)
	{
		FEDM_CHK5(lpfnSetDevicePara);
	}
#endif
#endif

#ifdef _FEDM_TCP_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
	LPFN_FETCP_SET_SOCKET_PARA lpfnSetSocketPara = (LPFN_FETCP_SET_SOCKET_PARA)GetFeTcpFunction(FETCP_SET_SOCKET_PARA);
	if(m_iCommMode == FEDM_COMM_MODE_TCP)
	{
		FEDM_CHK5(lpfnSetSocketPara);
	}
#endif
#endif


	if(m_iCommMode == FEDM_COMM_MODE_SERIAL)
	{
#ifdef _FEDM_COM_SUPPORT
		if(strcmp(cPara, "SetRTS")==0)
		{
#if !defined(_FEDM_SUPPORT_SLINK)
			iBack = lpfnDoPortCmd(iPortHnd, cPara, cValue);
			FEDM_RETURN(iBack);
#else
			iBack = FECOM_DoPortCmd(iPortHnd, cPara, cValue);
			FEDM_RETURN(iBack);
#endif
		}
		else if(strcmp(cPara, "SetDTR")==0)
		{
#if !defined(_FEDM_SUPPORT_SLINK)
			iBack = lpfnDoPortCmd(iPortHnd, cPara, cValue);
			FEDM_RETURN(iBack);
#else
			iBack = FECOM_DoPortCmd(iPortHnd, cPara, cValue);
			FEDM_RETURN(iBack);
#endif
		}
		else
		{
#if !defined(_FEDM_SUPPORT_SLINK)
			iBack = lpfnSetPortPara(iPortHnd, cPara, cValue);
			FEDM_RETURN(iBack);
#else
			iBack = FECOM_SetPortPara(iPortHnd, cPara, cValue);
			FEDM_RETURN(iBack);
#endif
		}
#else
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
#endif
	}
	else if(m_iCommMode == FEDM_COMM_MODE_TCP)
	{
#ifdef _FEDM_TCP_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
		iBack = lpfnSetSocketPara(iPortHnd, cPara, cValue);
		FEDM_RETURN(iBack);
#else
		iBack = FETCP_SetSocketPara(iPortHnd, cPara, cValue);
		FEDM_RETURN(iBack);
#endif
#else
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
#endif
	}
	else if(m_iCommMode == FEDM_COMM_MODE_USB)
	{
#ifdef _FEDM_USB_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
		iBack = lpfnSetDevicePara(iPortHnd, cPara, cValue);
		FEDM_RETURN(iBack);
#else
		iBack = FEUSB_SetDevicePara(iPortHnd, cPara, cValue);
		FEDM_RETURN(iBack);
#endif
#else
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
#endif
	}

	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
	FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
}

/***************************************************************************
  Begin        		:	06.11.2003 / M. Hultsch

  Version       	:	03.01.06 / 05.05.2010 / M. Hultsch
						- ExclusiveAccess fï¿½r FEUSB

						02.03.05 / 14.11.2005 / M. Hultsch
						- Modifikation fï¿½r uClinux

						02.03.00 / 04.05.2004 / M. Hultsch
						- Erweiterung fï¿½r USB, TCP

						01.09.00 / 06.11.2003 / M. Hultsch

  Function			:	sets one port parameter in port library

  Parameters		:	char* cPara		-	pointer to parameter string (see port manual)
						int	  iValue	-	value (see port manual)

  Return value		:	FEDM_OK or error code (<0)
***************************************************************************/
int FEDM_ISCReaderModule::SetPortPara(char* cPara, int iValue)
{
	FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_ADMIN_SET_GET, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "Para=" << cPara << ", Value=" << iValue);

	int iBack = 0;
	char cValue[16];

#ifdef _FEDM_USB_SUPPORT
	// it's a global setting
	if(strcmp(cPara, "ExclusiveAccess")==0)
	{
#if !defined(_FEDM_SUPPORT_SLINK)
		LPFN_FEUSB_SET_DEVICE_PARA lpfnSetDevicePara = (LPFN_FEUSB_SET_DEVICE_PARA)GetFeUsbFunction(FEUSB_SET_DEVICE_PARA);
		FEDM_CHK5(lpfnSetDevicePara);
		sprintf(cValue,"%d", iValue);
		iBack = lpfnSetDevicePara(0, cPara, cValue);
		FEDM_RETURN(iBack);
#else
		iBack = FEUSB_SetDevicePara(0, cPara, cValue);
		FEDM_RETURN(iBack);
#endif
	}
#endif

	if(m_iCommMode == FEDM_COMM_MODE_UNDEF)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_NOT_CONNECTED));
		FEDM_RETURN(FEDM_ERROR_NOT_CONNECTED);
	}

	int iPortHnd = GetPortHnd();
	if(iPortHnd < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_NOT_CONNECTED));
		FEDM_RETURN(FEDM_ERROR_NOT_CONNECTED);
	}


#ifdef _FEDM_COM_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
	LPFN_FECOM_SET_PORT_PARA lpfnSetPortPara = (LPFN_FECOM_SET_PORT_PARA)GetFeComFunction(FECOM_SET_PORT_PARA);
	if(m_iCommMode == FEDM_COMM_MODE_SERIAL)
	{
		FEDM_CHK5(lpfnSetPortPara);
	}
#endif
#endif

#ifdef _FEDM_USB_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
	LPFN_FEUSB_SET_DEVICE_PARA lpfnSetDevicePara = (LPFN_FEUSB_SET_DEVICE_PARA)GetFeUsbFunction(FEUSB_SET_DEVICE_PARA);
	if(m_iCommMode == FEDM_COMM_MODE_USB)
	{
		FEDM_CHK5(lpfnSetDevicePara);
	}
#endif
#endif

#ifdef _FEDM_TCP_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
	LPFN_FETCP_SET_SOCKET_PARA lpfnSetSocketPara = (LPFN_FETCP_SET_SOCKET_PARA)GetFeTcpFunction(FETCP_SET_SOCKET_PARA);
	if(m_iCommMode == FEDM_COMM_MODE_TCP)
	{
		FEDM_CHK5(lpfnSetSocketPara);
	}
#endif
#endif

	if(m_iCommMode == FEDM_COMM_MODE_SERIAL)
	{
#ifdef _FEDM_COM_SUPPORT
		sprintf(cValue,"%d", iValue);
#if !defined(_FEDM_SUPPORT_SLINK)
		iBack = lpfnSetPortPara(iPortHnd, cPara, cValue);
		FEDM_RETURN(iBack);
#else
		iBack = FECOM_SetPortPara(iPortHnd, cPara, cValue);
		FEDM_RETURN(iBack);
#endif
#else
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
#endif
	}
	else if(m_iCommMode == FEDM_COMM_MODE_TCP)
	{
#ifdef _FEDM_TCP_SUPPORT
		sprintf(cValue,"%d", iValue);
#if !defined(_FEDM_SUPPORT_SLINK)
		iBack = lpfnSetSocketPara(iPortHnd, cPara, cValue);
		FEDM_RETURN(iBack);
#else
		iBack = FETCP_SetSocketPara(iPortHnd, cPara, cValue);
		FEDM_RETURN(iBack);
#endif
#else
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
#endif
	}
	else if(m_iCommMode == FEDM_COMM_MODE_USB)
	{
#ifdef _FEDM_USB_SUPPORT
		sprintf(cValue,"%d", iValue);
#if !defined(_FEDM_SUPPORT_SLINK)
		iBack = lpfnSetDevicePara(iPortHnd, cPara, cValue);
		FEDM_RETURN(iBack);
#else
		iBack = FEUSB_SetDevicePara(iPortHnd, cPara, cValue);
		FEDM_RETURN(iBack);
#endif
#else
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
#endif
	}

	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
	FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
}

/***************************************************************************
  Begin        		:	06.11.2003 / M. Hultsch

  Version       	:	03.01.06 / 05.05.2010 / M. Hultsch
						- ExclusiveAccess fï¿½r FEUSB

						02.03.05 / 14.11.2005 / M. Hultsch
						- Modifikation fï¿½r uClinux

						01.09.05 / 18.02.2004 / M. Hultsch

  Function			:	gets one port parameter from port library

  Parameters		:	char* cPara		-	pointer to parameter string (see port manual)
						char* cValue	-	pointer to value string (see port manual)

  Return value		:	FEDM_OK or error code (<0)
***************************************************************************/
int FEDM_ISCReaderModule::GetPortPara(char* cPara, char* cValue)
{
	if(m_iCommMode == FEDM_COMM_MODE_UNDEF)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_NOT_CONNECTED));
		FEDM_RETURN(FEDM_ERROR_NOT_CONNECTED);
	}

	int iBack = 0;
	int iPortHnd = GetPortHnd();
	if(iPortHnd < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_NOT_CONNECTED));
		FEDM_RETURN(FEDM_ERROR_NOT_CONNECTED);
	}


#ifdef _FEDM_COM_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
	LPFN_FECOM_GET_PORT_PARA lpfnGetPortPara = (LPFN_FECOM_GET_PORT_PARA)GetFeComFunction(FECOM_GET_PORT_PARA);
	FEDM_CHK5(lpfnGetPortPara);
#endif
#endif

#ifdef _FEDM_USB_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
	LPFN_FEUSB_GET_DEVICE_PARA lpfnGetDevicePara = (LPFN_FEUSB_GET_DEVICE_PARA)GetFeUsbFunction(FEUSB_GET_DEVICE_PARA);
	FEDM_CHK5(lpfnGetDevicePara);
#endif
#endif

#ifdef _FEDM_TCP_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
	LPFN_FETCP_GET_SOCKET_PARA lpfnGetSocketPara = (LPFN_FETCP_GET_SOCKET_PARA)GetFeTcpFunction(FETCP_GET_SOCKET_PARA);
	FEDM_CHK5(lpfnGetSocketPara);
#endif
#endif

	if(m_iCommMode == FEDM_COMM_MODE_SERIAL)
	{
#ifdef _FEDM_COM_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
		iBack = lpfnGetPortPara(iPortHnd, cPara, cValue);
		FEDM_RETURN(iBack);
#else
		iBack = FECOM_GetPortPara(iPortHnd, cPara, cValue);
		FEDM_RETURN(iBack);
#endif
#else
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
#endif
	}
	else if(m_iCommMode == FEDM_COMM_MODE_USB)
	{
#ifdef _FEDM_USB_SUPPORT
		if(strcmp(cPara, "ExclusiveAccess")==0)
		{
			iPortHnd = 0; // it's a global setting
		}
#if !defined(_FEDM_SUPPORT_SLINK)
		iBack = lpfnGetDevicePara(iPortHnd, cPara, cValue);
		FEDM_RETURN(iBack);
#else
		iBack = FEUSB_GetDevicePara(iPortHnd, cPara, cValue);
		FEDM_RETURN(iBack);
#endif
#else
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
#endif
	}
	else if(m_iCommMode == FEDM_COMM_MODE_TCP)
	{
#ifdef _FEDM_TCP_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
		iBack = lpfnGetSocketPara(iPortHnd, cPara, cValue);
		FEDM_RETURN(iBack);
#else
		iBack = FETCP_GetSocketPara(iPortHnd, cPara, cValue);
		FEDM_RETURN(iBack);
#endif
#else
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
#endif
	}

	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
	FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
}

/***************************************************************************
  Begin        		:	06.11.2003 / M. Hultsch

  Version       	:	03.01.06 / 05.05.2010 / M. Hultsch
						- ExclusiveAccess fï¿½r FEUSB

						02.03.05 / 14.11.2005 / M. Hultsch
						- Modifikation fï¿½r uClinux

						02.03.00 / 04.05.2004 / M. Hultsch
						- Erweiterung fï¿½r USB, TCP

						01.09.00 / 06.11.2003 / M. Hultsch

  Function			:	gets one port parameter from port library

  Parameters		:	char* cPara		-	pointer to parameter string (see port manual)
						int*  iValue	-	pointer to value (see port manual)

  Return value		:	FEDM_OK or error code (<0)
***************************************************************************/
int FEDM_ISCReaderModule::GetPortPara(char* cPara, int* iValue)
{
	if(m_iCommMode == FEDM_COMM_MODE_UNDEF)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_NOT_CONNECTED));
		FEDM_RETURN(FEDM_ERROR_NOT_CONNECTED);
	}

	int iPortHnd = GetPortHnd();
	if(iPortHnd < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_NOT_CONNECTED));
		FEDM_RETURN(FEDM_ERROR_NOT_CONNECTED);
	}


	char cValue[16];
	int iErr;

#ifdef _FEDM_COM_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
	LPFN_FECOM_GET_PORT_PARA lpfnGetPortPara = (LPFN_FECOM_GET_PORT_PARA)GetFeComFunction(FECOM_GET_PORT_PARA);
	FEDM_CHK5(lpfnGetPortPara);
#endif
#endif

#ifdef _FEDM_USB_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
	LPFN_FEUSB_GET_DEVICE_PARA lpfnGetDevicePara = (LPFN_FEUSB_GET_DEVICE_PARA)GetFeUsbFunction(FEUSB_GET_DEVICE_PARA);
	FEDM_CHK5(lpfnGetDevicePara);
#endif
#endif

#ifdef _FEDM_TCP_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
	LPFN_FETCP_GET_SOCKET_PARA lpfnGetSocketPara = (LPFN_FETCP_GET_SOCKET_PARA)GetFeTcpFunction(FETCP_GET_SOCKET_PARA);
	FEDM_CHK5(lpfnGetSocketPara);
#endif
#endif

	if(m_iCommMode == FEDM_COMM_MODE_SERIAL)
	{
#ifdef _FEDM_COM_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
		FEDM_CHK2(iErr, lpfnGetPortPara(iPortHnd, cPara, cValue));
#else
		FEDM_CHK2(iErr, FECOM_GetPortPara(iPortHnd, cPara, cValue));
#endif
		sscanf(cValue,"%d", iValue);
#else
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
#endif
	}
	else if(m_iCommMode == FEDM_COMM_MODE_TCP)
	{
#ifdef _FEDM_TCP_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
		FEDM_CHK2(iErr, lpfnGetSocketPara(iPortHnd, cPara, cValue));
#else
		FEDM_CHK2(iErr, FETCP_GetSocketPara(iPortHnd, cPara, cValue));
#endif
		sscanf(cValue,"%d", iValue);
#else
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
#endif
	}
	else if(m_iCommMode == FEDM_COMM_MODE_USB)
	{
#ifdef _FEDM_USB_SUPPORT
		if(strcmp(cPara, "ExclusiveAccess")==0)
		{
			iPortHnd = 0; // it's a global setting
		}
#if !defined(_FEDM_SUPPORT_SLINK)
		FEDM_CHK2(iErr, lpfnGetDevicePara(iPortHnd, cPara, cValue));
#else
		FEDM_CHK2(iErr, FEUSB_GetDevicePara(iPortHnd, cPara, cValue));
#endif
		sscanf(cValue,"%d", iValue);
#else
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
#endif
	}
	else
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED_PORT_DRIVER));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED_PORT_DRIVER);
	}

	FEDM_RETURN(FEDM_OK);
}



//////////////////////////////////////////////////////////////////////
// complex communication functions
//////////////////////////////////////////////////////////////////////

/***************************************************************************
  Begin        		:	11.11.2003 / M. Hultsch

  Version       	:	04.05.04 / 03.09.2013 / M. Hultsch
						- new reader: ID ISC.LRU1002

						03.03.00 / 01.02.2011 / M. Hultsch
						- ID LR2500-A and -B added
  
						03.01.00 / 12.02.2010 / M. Hultsch
						- support for crypto mode
  
						03.00.13 / 28.09.2009 / M. Hultsch
						- ID MAX50.xx added
  
						03.00.08 / 09.06.2009 / M. Hultsch
						- ID CPR44.xx added
  
						03.00.07 / 29.04.2009 / M. Hultsch
						- ID ISC.M02.M8 added
  
						03.00.06 / 16.04.2009 / M. Hultsch
						- ID ISC.UM02 added
  
						03.00.05 / 12.03.2009 / M. Hultsch

  Function			:	reads complete reader configuration

  Parameters		:	bool bEEPROM	- true, if location is EEPROM

  Return value		:	FEDM_OK or error code (<0) or status byte (>0)
***************************************************************************/
int FEDM_ISCReaderModule::ReadCompleteConfiguration(bool bEEPROM)
{
	if(bEEPROM)
	{
		FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_HIGH_LEVEL, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "EEPROM");
	}
	else
	{
		FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_HIGH_LEVEL, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "RAM");
	}

	int iErr = 0;
	int iBitNr = 0;
	int iBitPos = 0;
	int iIdx = 0;
	unsigned char ucPage	 = 0;
	unsigned int uiMaxPages = 0;
	unsigned __int64 ui64CfgReadPermissions = 0; // read permissions, based on system manuals
	unsigned __int64 ui64BitNr = 0;

	if(! IsConnected())
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_NOT_CONNECTED));
		FEDM_RETURN(FEDM_ERROR_NOT_CONNECTED);
	}

	switch(m_uiReaderType)
	{
	case FEDM_ISC_TYPE_ISCM02:
	case FEDM_ISC_TYPE_ISCM02_M8:
	case FEDM_ISC_TYPE_ISCPRH100:
	case FEDM_ISC_TYPE_ISCPRH100_U:
	case FEDM_ISC_TYPE_ISCMR100:	// also FEDM_ISC_TYPE_ISCPR100
	case FEDM_ISC_TYPE_ISCMR100_U:
	case FEDM_ISC_TYPE_CPRM02:
	case FEDM_ISC_TYPE_CPR02:
	case FEDM_ISC_TYPE_CPR04_U:
	case FEDM_ISC_TYPE_CPR40_XX_U:
	case FEDM_ISC_TYPE_CPR40_XX:
	case FEDM_ISC_TYPE_CPR44_XX:
		if(m_ReaderInfo.bIsMode0x40Read)
		{
			uiMaxPages = m_ReaderInfo.GetNoOfCfgPages();
		}
		else
		{
			uiMaxPages = 8;
#ifdef __GNUC__
			ui64CfgReadPermissions = 0xFF00000000000000LL; // 0-7
#else
			ui64CfgReadPermissions = 0xFF00000000000000; // 0-7
#endif
		}
		break;

	case FEDM_ISC_TYPE_CPR50_XX:
		if(m_ReaderInfo.bIsMode0x40Read)
		{
			uiMaxPages = m_ReaderInfo.GetNoOfCfgPages();
		}
		else
		{
			uiMaxPages = 64;
#ifdef __GNUC__
			ui64CfgReadPermissions = 0xFFC0800000C00001LL; // 0-9,16,40-41,63
#else
			ui64CfgReadPermissions = 0xFFC0800000C00001; // 0-9,16,40-41,63
#endif
		}
		break;

	case FEDM_ISC_TYPE_ISCPRH101:
		if(m_ReaderInfo.bIsMode0x40Read)
		{
			uiMaxPages = m_ReaderInfo.GetNoOfCfgPages();
		}
		else
		{
			uiMaxPages = 64;
#ifdef __GNUC__
			ui64CfgReadPermissions = 0xFFC0800000008001LL; // 0-9,16,48,63
#else
			ui64CfgReadPermissions = 0xFFC0800000008001; // 0-9,16,48,63
#endif
		}
		break;

	case FEDM_ISC_TYPE_ISCPRH101_U:
		if(m_ReaderInfo.bIsMode0x40Read)
		{
			uiMaxPages = m_ReaderInfo.GetNoOfCfgPages();
		}
		else
		{
			uiMaxPages = 64;
#ifdef __GNUC__
			ui64CfgReadPermissions = 0xFFC0800000000001LL; // 0-9,16,63
#else
			ui64CfgReadPermissions = 0xFFC0800000000001; // 0-9,16,63
#endif
		}
		break;

	case FEDM_ISC_TYPE_ISCMR101:	// also FEDM_ISC_TYPE_ISCPR101
	case FEDM_ISC_TYPE_ISCMR101_U:
		if(m_ReaderInfo.bIsMode0x40Read)
		{
			uiMaxPages = m_ReaderInfo.GetNoOfCfgPages();
		}
		else
		{
			uiMaxPages = 64;
#ifdef __GNUC__
			ui64CfgReadPermissions = 0xFFE0800000000001LL; // 0-10,16,63
#else
			ui64CfgReadPermissions = 0xFFE0800000000001; // 0-10,16,63
#endif
		}
		break;

	case FEDM_ISC_TYPE_ISCLR200:
		if(m_ReaderInfo.bIsMode0x40Read)
		{
			uiMaxPages = m_ReaderInfo.GetNoOfCfgPages();
		}
		else
		{
			uiMaxPages = 16;
#ifdef __GNUC__
			ui64CfgReadPermissions = 0xFFFF000000000000LL; // 0-15
#else
			ui64CfgReadPermissions = 0xFFFF000000000000; // 0-15
#endif
		}
		break;

	case FEDM_ISC_TYPE_ISCMU02:
	case FEDM_ISC_TYPE_ISCPRH102:
	case FEDM_ISC_TYPE_ISCPRHD102:
	case FEDM_ISC_TYPE_ISCMR200:
	case FEDM_ISC_TYPE_ISCMRU200:
	case FEDM_ISC_TYPE_ISCLRU1000:
	case FEDM_ISC_TYPE_ISCLRU2000:
	case FEDM_ISC_TYPE_ISCLR2000:
	case FEDM_ISC_TYPE_MAX50_XX:
		if(m_ReaderInfo.bIsMode0x40Read)
		{
			uiMaxPages = m_ReaderInfo.GetNoOfCfgPages();
		}
		else
		{
			uiMaxPages = 64;
#ifdef __GNUC__
			ui64CfgReadPermissions = 0xFFFFFFFFFFFFFFFFLL; // 0-63
#else
			ui64CfgReadPermissions = 0xFFFFFFFFFFFFFFFF; // 0-63
#endif
		}
		break;

#ifndef FEDM_NEW_LRU3000_ACC_CFG
	case FEDM_ISC_TYPE_ISCLRU3000:
		if(bEEPROM)
			SetData(FEDM_ISC_TMP_0x8A_READ_MODE,		(unsigned char)0x41);	// mode: read all from EEPROM
		else
			SetData(FEDM_ISC_TMP_0x8A_READ_MODE,		(unsigned char)0x40);	// mode: read all from RAM

		SetData(FEDM_ISC_TMP_0x8A_READ_DEVICE,		(unsigned char)FEDM_CNTRL_TYPE_ACC);		// device: ACC
		SetData(FEDM_ISC_TMP_0x8A_READ_BANK,		(unsigned char)FEDM_MEM_BANK_TYPE_MAIN);	// bank: main
		SetData(FEDM_ISC_TMP_0x8A_READ_REQ_CFG_ADR,	(unsigned int)0);	// first block address not necessary
		SetData(FEDM_ISC_TMP_0x8A_READ_REQ_CFG_N,	(unsigned char)0);	// number of blocks not necessary
		FEDM_CHK2(iErr, SendProtocol(0x8A));
		if( iErr == 0x13 || // Login-Request
			iErr == 0x19 ) // crypto processing error
		{
			FEDM_RETURN(iErr);
		}
		FEDM_RETURN(FEDM_OK);
#endif

	case FEDM_ISC_TYPE_ISCLR2500_A:
#ifdef FEDM_NEW_LRU3000_ACC_CFG
	case FEDM_ISC_TYPE_ISCLRU3000:
#endif
		if(bEEPROM)
			SetData(FEDM_ISC_TMP_0x8A_READ_MODE,		(unsigned char)0x41);	// mode: read all from EEPROM
		else
			SetData(FEDM_ISC_TMP_0x8A_READ_MODE,		(unsigned char)0x40);	// mode: read all from RAM

		SetData(FEDM_ISC_TMP_0x8A_READ_DEVICE,		(unsigned char)FEDM_CNTRL_TYPE_RFC);		// device: RFC
		SetData(FEDM_ISC_TMP_0x8A_READ_BANK,		(unsigned char)FEDM_MEM_BANK_TYPE_MAIN);	// bank: main
		SetData(FEDM_ISC_TMP_0x8A_READ_REQ_CFG_ADR,	(unsigned int)0);	// first block address not necessary
		SetData(FEDM_ISC_TMP_0x8A_READ_REQ_CFG_N,	(unsigned char)0);	// number of blocks not necessary
		FEDM_CHK2(iErr, SendProtocol(0x8A));
		if( iErr == 0x13 || // Login-Request
			iErr == 0x19 ) // crypto processing error
		{
			FEDM_RETURN(iErr);
		}
		FEDM_RETURN(FEDM_OK);

	default:
		if(m_ReaderInfo.bIsMode0x40Read)
		{
			uiMaxPages = m_ReaderInfo.GetNoOfCfgPages();
		}
		else
		{
			FEDM_RETURN(FEDM_ERROR_UNKNOWN_READER_TYPE);
		}
	}


	// read configuration from RAM or EEPROM
	FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_READ_CFG, (unsigned char)0));
	FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_READ_CFG_LOC, bEEPROM));

	FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8A_READ_MODE, (unsigned char)0));
	FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8A_READ_MODE_LOC, bEEPROM));	// memory location

	if(m_ReaderInfo.bIsMode0x40Read)
	{
		// read with knowledge about read permissions
		ucPage = 0;
		for(iIdx=0; iIdx<16; iIdx++) // max 16 permission bytes
		{
			iBitNr = 0x80;
			for(iBitPos=0; iBitPos<8; iBitPos++)
			{
				if(m_ReaderInfo.ucCfgReadPermissions[iIdx] & iBitNr)
				{
					switch(m_uiReaderType)
					{
					case FEDM_ISC_TYPE_ISCLR2500_A:
					case FEDM_ISC_TYPE_ISCLR2500_B:
#ifdef FEDM_NEW_LRU3000_ACC_CFG
					case FEDM_ISC_TYPE_ISCLRU3000:
#endif
						SetData(FEDM_ISC_TMP_0x8A_READ_DEVICE,		(unsigned char)FEDM_CNTRL_TYPE_RFC);		// device: RFC
						SetData(FEDM_ISC_TMP_0x8A_READ_BANK,		(unsigned char)FEDM_MEM_BANK_TYPE_MAIN);	// bank: main
						SetData(FEDM_ISC_TMP_0x8A_READ_REQ_CFG_ADR,	(unsigned int)ucPage);	// first block address
						SetData(FEDM_ISC_TMP_0x8A_READ_REQ_CFG_N,	(unsigned char)1);		// number of blocks
						FEDM_CHK2(iErr, SendProtocol(0x8A));
						if( iErr == 0x13 || // Login-Request
							iErr == 0x19 ) // crypto processing error
						{
							FEDM_RETURN(iErr);
						}
						break;

					default:
						FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_READ_CFG_ADR, ucPage));
						FEDM_CHK2(iErr, SendProtocol(0x80));
						if(	iErr == 0x13 || // Login-Request
							iErr == 0x19 ) // crypto processing error
						{
							FEDM_RETURN(iErr);
						}
					}
				}
				
				iBitNr >>= 1;
				ucPage++;
			}
		}
	}
	else
	{
		// read with explicite knowledge about read permissions from system manuals
		ucPage = 0;
#ifdef __GNUC__
		ui64BitNr = 0x8000000000000000LL;
#else
		ui64BitNr = 0x8000000000000000;
#endif
		for(iBitPos=0; iBitPos<64; iBitPos++)
		{
			if(ui64CfgReadPermissions & ui64BitNr)
			{
				FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_READ_CFG_ADR, ucPage));
				FEDM_CHK2(iErr, SendProtocol(0x80));
				if(	iErr == 0x13 || // Login-Request
					iErr == 0x19 ) // crypto processing error
				{
					FEDM_RETURN(iErr);
				}
			}
			
			ui64BitNr >>= 1;
			ucPage++;
		}
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        		:	11.11.2003 / M. Hultsch

  Version       	:	04.05.04 / 03.09.2013 / M. Hultsch
						- new reader: ID ISC.LRU1002

						03.03.00 / 01.02.2011 / M. Hultsch
						- ID LR2500-A and -B added
  
						03.01.00 / 12.02.2010 / M. Hultsch
						- support for crypto mode
  
						03.00.13 / 28.09.2009 / M. Hultsch
						- ID MAX50.xx added
  
						03.00.08 / 09.06.2009 / M. Hultsch
						- ID CPR44.xx added
  
						03.00.07 / 29.04.2009 / M. Hultsch
						- ID ISC.M02.M8 added
  
						03.00.06 / 16.04.2009 / M. Hultsch
						- ID ISC.UM02 added
  
						03.00.05 / 12.03.2009 / M. Hultsch

  Function			:	writes complete reader configuration

  Parameters		:	bool bEEPROM	- true, if location is EEPROM

  Return value		:	FEDM_OK or error code (<0) or status byte (>0)
***************************************************************************/
int FEDM_ISCReaderModule::WriteCompleteConfiguration(bool bEEPROM)
{
	if(bEEPROM)
	{
		FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_HIGH_LEVEL, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "EEPROM");
	}
	else
	{
		FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_HIGH_LEVEL, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "RAM");
	}

	int iErr = 0;
	int iBitNr = 0;
	int iBitPos = 0;
	int iIdx = 0;
	int iTmpMemAdr = 0;
	unsigned char ucPage	 = 0;
	unsigned int uiMaxPages = 0;
	unsigned __int64 ui64CfgWritePermissions = 0; // read permissions, based on system manuals
	unsigned __int64 ui64BitNr = 0;

	if(! IsConnected())
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_NOT_CONNECTED));
		FEDM_RETURN(FEDM_ERROR_NOT_CONNECTED);
	}


	switch(m_uiReaderType)
	{
	case FEDM_ISC_TYPE_ISCM02:
	case FEDM_ISC_TYPE_ISCM02_M8:
	case FEDM_ISC_TYPE_ISCPRH100:
	case FEDM_ISC_TYPE_ISCPRH100_U:
	case FEDM_ISC_TYPE_ISCMR100:	// also FEDM_ISC_TYPE_ISCPR100
	case FEDM_ISC_TYPE_ISCMR100_U:
	case FEDM_ISC_TYPE_CPRM02:
	case FEDM_ISC_TYPE_CPR02:
	case FEDM_ISC_TYPE_CPR04_U:
	case FEDM_ISC_TYPE_CPR40_XX_U:
	case FEDM_ISC_TYPE_CPR40_XX:
	case FEDM_ISC_TYPE_CPR44_XX:
		if(m_ReaderInfo.bIsMode0x41Read)
		{
			uiMaxPages = m_ReaderInfo.GetNoOfCfgPages();
		}
		else
		{
			uiMaxPages = 8;
#ifdef __GNUC__
			ui64CfgWritePermissions = 0xFF00000000000000LL; // 0-7
#else
			ui64CfgWritePermissions = 0xFF00000000000000; // 0-7
#endif
		}
		break;

	case FEDM_ISC_TYPE_CPR50_XX:
		if(m_ReaderInfo.bIsMode0x41Read)
		{
			uiMaxPages = m_ReaderInfo.GetNoOfCfgPages();
		}
		else
		{
			uiMaxPages = 64;
#ifdef __GNUC__
			ui64CfgWritePermissions = 0xFFC0800000C00001LL; // 0-9,16,40-41,63
#else
			ui64CfgWritePermissions = 0xFFC0800000C00001; // 0-9,16,40-41,63
#endif
		}
		break;

	case FEDM_ISC_TYPE_ISCPRH101:
		if(m_ReaderInfo.bIsMode0x41Read)
		{
			uiMaxPages = m_ReaderInfo.GetNoOfCfgPages();
		}
		else
		{
			uiMaxPages = 64;
#ifdef __GNUC__
			ui64CfgWritePermissions = 0xFFC0800000008001LL; // 0-9,16,48,63
#else
			ui64CfgWritePermissions = 0xFFC0800000008001; // 0-9,16,48,63
#endif
		}
		break;

	case FEDM_ISC_TYPE_ISCPRH101_U:
		if(m_ReaderInfo.bIsMode0x41Read)
		{
			uiMaxPages = m_ReaderInfo.GetNoOfCfgPages();
		}
		else
		{
			uiMaxPages = 64;
#ifdef __GNUC__
			ui64CfgWritePermissions = 0xFFC0800000000001LL; // 0-9,16,63
#else
			ui64CfgWritePermissions = 0xFFC0800000000001; // 0-9,16,63
#endif
		}
		break;

	case FEDM_ISC_TYPE_ISCMR101:	// also FEDM_ISC_TYPE_ISCPR101
	case FEDM_ISC_TYPE_ISCMR101_U:
		if(m_ReaderInfo.bIsMode0x41Read)
		{
			uiMaxPages = m_ReaderInfo.GetNoOfCfgPages();
		}
		else
		{
			uiMaxPages = 64;
#ifdef __GNUC__
			ui64CfgWritePermissions = 0xFFE0800000000001LL; // 0-10,16,63
#else
			ui64CfgWritePermissions = 0xFFE0800000000001; // 0-10,16,63
#endif
		}
		break;

	case FEDM_ISC_TYPE_ISCLR200:
		if(m_ReaderInfo.bIsMode0x41Read)
		{
			uiMaxPages = m_ReaderInfo.GetNoOfCfgPages();
		}
		else
		{
			uiMaxPages = 16;
#ifdef __GNUC__
			ui64CfgWritePermissions = 0xFFFF000000000000LL; // 0-15
#else
			ui64CfgWritePermissions = 0xFFFF000000000000; // 0-15
#endif
		}
		break;

	case FEDM_ISC_TYPE_ISCMU02:
	case FEDM_ISC_TYPE_ISCPRHD102:
	case FEDM_ISC_TYPE_ISCPRH102:
	case FEDM_ISC_TYPE_ISCMR200:
	case FEDM_ISC_TYPE_ISCMRU200:
	case FEDM_ISC_TYPE_ISCLRU1000:
	case FEDM_ISC_TYPE_ISCLRU2000:
	case FEDM_ISC_TYPE_ISCLR2000:
	case FEDM_ISC_TYPE_MAX50_XX:
		if(m_ReaderInfo.bIsMode0x41Read)
		{
			uiMaxPages = m_ReaderInfo.GetNoOfCfgPages();
		}
		else
		{
			uiMaxPages = 64;
#ifdef __GNUC__
			ui64CfgWritePermissions = 0xFFFFFFFFFFFFFFFFLL; // 0-63
#else
			ui64CfgWritePermissions = 0xFFFFFFFFFFFFFFFF; // 0-63
#endif
		}
		break;

#ifndef FEDM_NEW_LRU3000_ACC_CFG
	case FEDM_ISC_TYPE_ISCLRU3000:
		if(bEEPROM)
			SetData(FEDM_ISC_TMP_0x8B_WRITE_MODE,		(unsigned char)0x01);	// mode: write to EEPROM
		else
			SetData(FEDM_ISC_TMP_0x8B_WRITE_MODE,		(unsigned char)0x00);	// mode: write to RAM

		SetData(FEDM_ISC_TMP_0x8B_WRITE_DEVICE,		(unsigned char)FEDM_CNTRL_TYPE_ACC);		// device: ACC
		SetData(FEDM_ISC_TMP_0x8B_WRITE_BANK,		(unsigned char)FEDM_MEM_BANK_TYPE_MAIN);	// bank: main
		SetData(FEDM_ISC_TMP_0x8B_WRITE_CFG_SIZE,	(unsigned char)30);	// blocksize
		iTmpMemAdr = FEDM_GetAdrOfID(FEDM_ISC_TMP_0x8B_WRITE_CFG_ADR, 32);	// start address in memory

		uiMaxPages = 0;

		if(m_ReaderInfo.bIsMode0x41Read)
		{
			// write with knowledge about write permissions
			ucPage = 0;
			for(iIdx=0; iIdx<16; iIdx++) // max 16 permission bytes
			{
				iBitNr = 0x80;
				for(iBitPos=0; iBitPos<8; iBitPos++)
				{
					if(m_ReaderInfo.ucCfgWritePermissions[iIdx] & iBitNr)
					{
						m_TmpData[iTmpMemAdr]	= 0x00;
						m_TmpData[iTmpMemAdr+1]	= ucPage;
						iTmpMemAdr += 2;

						uiMaxPages++;
					}
					
					iBitNr >>= 1;
					ucPage++;
				}
			}
		}
		else
		{
			// write without knowledge
			for(ucPage=0; ucPage<=85; ucPage++)
			{
				m_TmpData[iTmpMemAdr]	= 0x00;
				m_TmpData[iTmpMemAdr+1]	= ucPage;
				iTmpMemAdr += 2;

				uiMaxPages++;
			}
		}

		SetData(FEDM_ISC_TMP_0x8B_WRITE_CFG_N,		(unsigned char)ucMaxPages);	// read one or multiple blocks
		FEDM_CHK2(iErr, SendProtocol(0x8B));
		if(	iErr == 0x13 || // Login-Request
			iErr == 0x19 ) // crypto processing error
		{
			FEDM_RETURN(iErr);
		}
		FEDM_RETURN(FEDM_OK);
		break;
#endif

	case FEDM_ISC_TYPE_ISCLR2500_A:
#ifdef FEDM_NEW_LRU3000_ACC_CFG
	case FEDM_ISC_TYPE_ISCLRU3000:
#endif
		if(bEEPROM)
			SetData(FEDM_ISC_TMP_0x8B_WRITE_MODE,		(unsigned char)0x01);	// mode: write to EEPROM
		else
			SetData(FEDM_ISC_TMP_0x8B_WRITE_MODE,		(unsigned char)0x00);	// mode: write to RAM

		SetData(FEDM_ISC_TMP_0x8B_WRITE_DEVICE,		(unsigned char)FEDM_CNTRL_TYPE_RFC);		// device: RFC
		SetData(FEDM_ISC_TMP_0x8B_WRITE_BANK,		(unsigned char)FEDM_MEM_BANK_TYPE_MAIN);	// bank: main
		SetData(FEDM_ISC_TMP_0x8B_WRITE_CFG_SIZE,	(unsigned char)30);	// blocksize
		iTmpMemAdr = FEDM_GetAdrOfID(FEDM_ISC_TMP_0x8B_WRITE_CFG_ADR, 32);	// start address in memory

		uiMaxPages = 0;

		if(m_ReaderInfo.bIsMode0x41Read)
		{
			// write with knowledge about write permissions
			ucPage = 0;
			for(iIdx=0; iIdx<16; iIdx++) // max 16 permission bytes
			{
				iBitNr = 0x80;
				for(iBitPos=0; iBitPos<8; iBitPos++)
				{
					if(m_ReaderInfo.ucCfgWritePermissions[iIdx] & iBitNr)
					{
						m_TmpData[iTmpMemAdr]	= 0x00;
						m_TmpData[iTmpMemAdr+1]	= ucPage;
						iTmpMemAdr += 2;

						uiMaxPages++;
					}
					
					iBitNr >>= 1;
					ucPage++;
				}
			}
		}
		else
		{
			// write without knowledge
			for(ucPage=0; ucPage<=85; ucPage++)
			{
				m_TmpData[iTmpMemAdr]	= 0x00;
				m_TmpData[iTmpMemAdr+1]	= ucPage;
				iTmpMemAdr += 2;

				uiMaxPages++;
			}
		}

		SetData(FEDM_ISC_TMP_0x8B_WRITE_CFG_N,		(unsigned char)uiMaxPages);	// read one or multiple blocks
		FEDM_CHK2(iErr, SendProtocol(0x8B));
		if(	iErr == 0x13 || // Login-Request
			iErr == 0x19 ) // crypto processing error
		{
			FEDM_RETURN(iErr);
		}
		FEDM_RETURN(FEDM_OK);
		break;

	default:
		if(m_ReaderInfo.bIsMode0x41Read)
		{
			uiMaxPages = m_ReaderInfo.GetNoOfCfgPages();
		}
		else
		{
			FEDM_RETURN(FEDM_ERROR_UNKNOWN_READER_TYPE);
		}
	}


	// write configuration to RAM or EEPROM
	FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_WRITE_CFG, (unsigned char)0));
	FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_WRITE_CFG_LOC, bEEPROM));

	FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8B_WRITE_MODE, (unsigned char)0));	// mode: write to EEPROM
	FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8B_WRITE_MODE_LOC, bEEPROM));	// mode: write to EEPROM

	if(m_ReaderInfo.bIsMode0x41Read)
	{
		// write with knowledge about write permissions
		ucPage = 0;
		for(iIdx=0; iIdx<16; iIdx++) // max 16 permission bytes
		{
			iBitNr = 0x80;
			for(iBitPos=0; iBitPos<8; iBitPos++)
			{
				if(m_ReaderInfo.ucCfgWritePermissions[iIdx] & iBitNr)
				{
					switch(m_uiReaderType)
					{
					case FEDM_ISC_TYPE_ISCLR2500_A:
					case FEDM_ISC_TYPE_ISCLR2500_B:
#ifdef FEDM_NEW_LRU3000_ACC_CFG
					case FEDM_ISC_TYPE_ISCLRU3000:
#endif
						FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8B_WRITE_DEVICE,		(unsigned char)FEDM_CNTRL_TYPE_RFC));		// device: RFC
						FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8B_WRITE_BANK,		(unsigned char)FEDM_MEM_BANK_TYPE_MAIN));	// bank: main
						FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8B_WRITE_CFG_SIZE,	(unsigned char)(m_iRFC_DataBlockSize-2)));	// blocksize
						FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8B_WRITE_CFG_ADR,	(unsigned int)ucPage));	// start address in memory
						FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8B_WRITE_CFG_N,		(unsigned char)0x01));	// number of blocks
						FEDM_CHK2(iErr, SendProtocol(0x8B));
						if(	iErr == 0x13 || // Login-Request
							iErr == 0x19 ) // crypto processing error
						{
							FEDM_RETURN(iErr);
						}
						break;

					default:
						FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_WRITE_CFG_ADR, ucPage));
						FEDM_CHK2(iErr, SendProtocol(0x81));
						if(	iErr == 0x13 || // Login-Request
							iErr == 0x19 ) // crypto processing error
						{
							FEDM_RETURN(iErr);
						}
					}
				}
				
				iBitNr >>= 1;
				ucPage++;
			}
		}
	}
	else
	{
		// write with explicite knowledge about write permissions from system manuals
		ucPage = 0;
#ifdef __GNUC__
		ui64BitNr = 0x8000000000000000LL;
#else
		ui64BitNr = 0x8000000000000000;
#endif
		for(iBitPos=0; iBitPos<64; iBitPos++)
		{
			if(ui64CfgWritePermissions & ui64BitNr)
			{
				FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_WRITE_CFG_ADR, ucPage));
				FEDM_CHK2(iErr, SendProtocol(0x81));
				if(	iErr == 0x13 || // Login-Request
					iErr == 0x19 ) // crypto processing error
				{
					FEDM_RETURN(iErr);
				}
			}
			
			ui64BitNr >>= 1;
			ucPage++;
		}
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        		:	29.07.2008 / M. Hultsch

  Version       	:	04.06.01 / 13.03.2014 / M. Hultsch
						- return of status 0x11 (Parameter Range Error), when the Reader signals this error

						04.05.04 / 03.09.2013 / M. Hultsch
						- new reader: ID ISC.LRU1002

						03.03.00 / 01.02.2011 / M. Hultsch
						- ID LR2500-A and -B added
  
						03.01.00 / 12.02.2010 / M. Hultsch
						- support for crypto mode
  
						03.00.13 / 28.09.2009 / M. Hultsch
						- ID MAX50.xx added
						- read page back from Reader after status 0x11 (Parameter Range Error)
  
						03.00.12 / 25.08.2009 / M. Hultsch
						- bugfix for ID LRU3000
  
						03.00.08 / 10.06.2009 / M. Hultsch
						- ID CPR44.xx added
						- with knowledge of write permission
  
						03.00.07 / 29.04.2009 / M. Hultsch
						- ID ISC.M02.M8 added
  
						03.00.06 / 16.04.2009 / M. Hultsch
						- ID ISC.UM02 added
  
						03.00.05 / 11.03.2009 / M. Hultsch

  Function			:	writes modified configuration pages to reader

  Parameters		:	bool bEEPROM	- true, if location is EEPROM

  Return value		:	FEDM_OK			- if nothing to be applied to
						FEDM_MODIFIED	- if at least one page is successfully written
						error code (<0)	- if something goes wrong
						status byte (>1)- if the reader signals a problem
***************************************************************************/
int FEDM_ISCReaderModule::ApplyConfiguration(bool bEEPROM)
{
	if(bEEPROM)
	{
		FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_HIGH_LEVEL, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "EEPROM");
	}
	else
	{
		FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_HIGH_LEVEL, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "RAM");
	}

	bool bModified = false;
	bool bIsMode0x41Read = false;
	int iErr = 0;
	int iBitNr = 0;
	int iBitPos = 0;
	int iIdx = 0;
	int iReturnStatus = FEDM_OK;
	unsigned int uiPage	 = 0;
	unsigned int uiMaxPages = 0;
	unsigned char ucData[32];
	unsigned char ucCfgWritePermissions[16];

	if(! IsConnected())
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_NOT_CONNECTED));
		FEDM_RETURN(FEDM_ERROR_NOT_CONNECTED);
	}

	// clear flag field for modified config pages
	memset(m_ucModifiedCfgPages, 0, 16);
	memset(ucCfgWritePermissions, 0, 16);

	// query write permissions
	if(m_ReaderInfo.bIsMode0x41Read)
	{
		uiMaxPages = m_ReaderInfo.GetNoOfCfgPages();
		memcpy(ucCfgWritePermissions, m_ReaderInfo.ucCfgWritePermissions, 16);
		bIsMode0x41Read = true;
	}
	else
	{
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x41));
		FEDM_CHK2(iErr, SendProtocol(0x66));
		if(	iErr == 0x19 ) // crypto processing error
		{
			FEDM_RETURN(iErr);
		}
		else if(iErr == 0)
		{
			FEDM_CHK2(iErr, GetData(FEDM_GetAdrOfID(FEDM_ISC_TMP_READER_INFO, 32), &ucData[0], 31, FEDM_TMPDATA_MEM));
			uiMaxPages = ((unsigned int)ucData[0]<<8) + (unsigned int)ucData[1];
			memcpy(ucCfgWritePermissions, &ucData[2], 16);
			bIsMode0x41Read = true;
		}
	}

	// write configuration to RAM and/or EEPROM

	if(uiMaxPages == 0)
	{
		switch(m_uiReaderType)
		{
		case FEDM_ISC_TYPE_ISCM02:
		case FEDM_ISC_TYPE_ISCM02_M8:
		case FEDM_ISC_TYPE_ISCPRH100:
		case FEDM_ISC_TYPE_ISCPRH100_U:
		case FEDM_ISC_TYPE_ISCMR100:	// also FEDM_ISC_TYPE_ISCPR100
		case FEDM_ISC_TYPE_ISCMR100_U:
		case FEDM_ISC_TYPE_CPRM02:
		case FEDM_ISC_TYPE_CPR02:
		case FEDM_ISC_TYPE_CPR04_U:
			uiMaxPages = 8;
			break;
		case FEDM_ISC_TYPE_CPR20_XX:
		case FEDM_ISC_TYPE_CPR40_XX_U:
		case FEDM_ISC_TYPE_CPR40_XX:
		case FEDM_ISC_TYPE_CPR44_XX:
			uiMaxPages = 16;
			break;
		case FEDM_ISC_TYPE_ISCLR200:
			uiMaxPages = 16;
			break;
		case FEDM_ISC_TYPE_ISCPRH101:
		case FEDM_ISC_TYPE_ISCPRH101_U:
		case FEDM_ISC_TYPE_ISCMR101:	// also FEDM_ISC_TYPE_ISCPR101
		case FEDM_ISC_TYPE_ISCMR101_U:
		case FEDM_ISC_TYPE_CPR50_XX:
		case FEDM_ISC_TYPE_ISCMU02:
		case FEDM_ISC_TYPE_ISCMR102:
		case FEDM_ISC_TYPE_ISCPRHD102:
		case FEDM_ISC_TYPE_ISCPRH102:
		case FEDM_ISC_TYPE_ISCMR200:
		case FEDM_ISC_TYPE_ISCMRU200:
		case FEDM_ISC_TYPE_ISCLRU1000:
		case FEDM_ISC_TYPE_ISCLRU2000:
		case FEDM_ISC_TYPE_ISCLR2000:
			uiMaxPages = 64;
			break;
		default:
			uiMaxPages = 64;
			break;
		}
	}

	switch(m_uiReaderType)
	{
	// classic-pro family
	case FEDM_ISC_TYPE_CPRM02:
	case FEDM_ISC_TYPE_CPR02:
	case FEDM_ISC_TYPE_CPR04_U:
	case FEDM_ISC_TYPE_CPR20_XX:
	case FEDM_ISC_TYPE_CPR40_XX_U:
	case FEDM_ISC_TYPE_CPR40_XX:
	case FEDM_ISC_TYPE_CPR44_XX:
	case FEDM_ISC_TYPE_CPR46_XX:
	case FEDM_ISC_TYPE_CPR50_XX:

	// OBID myAXXESS Reader
	case FEDM_ISC_TYPE_MAX50_XX:

	// i-scan HF short-range reader
	case FEDM_ISC_TYPE_ISCM02:
	case FEDM_ISC_TYPE_ISCM02_M8:

	// i-scan HF mid-range reader
	case FEDM_ISC_TYPE_ISCPRH100:
	case FEDM_ISC_TYPE_ISCPRH100_U:
	case FEDM_ISC_TYPE_ISCMR100:	// also FEDM_ISC_TYPE_ISCPR100
	case FEDM_ISC_TYPE_ISCMR100_U:
	case FEDM_ISC_TYPE_ISCPRH101:
	case FEDM_ISC_TYPE_ISCPRH101_U:
	case FEDM_ISC_TYPE_ISCMR101:	// also FEDM_ISC_TYPE_ISCPR101
	case FEDM_ISC_TYPE_ISCMR101_U:
	case FEDM_ISC_TYPE_ISCPRH102:
	case FEDM_ISC_TYPE_ISCMR200:
	case FEDM_ISC_TYPE_ISCMR10X:

	// i-scan HF long-range reader
	case FEDM_ISC_TYPE_ISCLR200:
	case FEDM_ISC_TYPE_ISCLR2000:

	// i-scan UHF short-range reader
	case FEDM_ISC_TYPE_ISCMU02:

	// i-scan UHF mid-range reader
	case FEDM_ISC_TYPE_ISCMRU200:

	// i-scan UHF long-range reader
	case FEDM_ISC_TYPE_ISCLRU1000:
	case FEDM_ISC_TYPE_ISCLRU2000:

	// i-scan HF/UHF mid-range reader
	case FEDM_ISC_TYPE_ISCPRHD102:
		FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_WRITE_CFG, (unsigned char)0));
		FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_WRITE_CFG_LOC, bEEPROM));
		if(bIsMode0x41Read)
		{
			// write with knowledge about write permissions
			uiPage = 0;
			for(iIdx=0; iIdx<16; iIdx++) // max 16 permission bytes
			{
				iBitNr = 0x80;
				for(iBitPos=0; iBitPos<8; iBitPos++)
				{
					if(ucCfgWritePermissions[iIdx] & iBitNr)
					{
						if(uiPage*16 + 15 >= FEDM_ISC_MAX_RFC_EEDATA_MEM)
							continue;

						if(	(  bEEPROM  && (m_RFC_EEData[uiPage*16+15] & 0x02)) ||
							((!bEEPROM) && (m_RFC_RAMData[uiPage*16+15] & 0x02)) )
						{
							FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_WRITE_CFG_ADR, uiPage));
							FEDM_CHK2(iErr, SendProtocol(0x81));
							if(iErr == 0x11) // Parameter Range Error
							{
								FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_READ_CFG_LOC, bEEPROM));
								FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_READ_CFG_ADR, uiPage));
								FEDM_CHK2(iErr, SendProtocol(0x80));
								iReturnStatus = 0x11;
							}
							else if(iErr == 0x13 || // Login-Request
									iErr == 0x19 ) // crypto processing error
							{
								FEDM_RETURN(iErr);
							}
							else
							{
								bModified = true;
							}

							// mark modified page in flag field
							m_ucModifiedCfgPages[iIdx] |= iBitNr;
						}
					}

					iBitNr >>= 1;
					uiPage++;
				}
			}
		}
		else
		{
			for(uiPage=0; uiPage<uiMaxPages; ++uiPage)
			{
				if(uiPage*16 + 15 >= FEDM_ISC_MAX_RFC_EEDATA_MEM)
					continue;

				if(	(  bEEPROM  && (m_RFC_EEData[uiPage*16+15] & 0x02)) ||
					((!bEEPROM) && (m_RFC_RAMData[uiPage*16+15] & 0x02)) )
				{
					FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_WRITE_CFG_ADR, uiPage));
					FEDM_CHK2(iErr, SendProtocol(0x81));
					if(iErr == 0x11) // Parameter Range Error
					{
						FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_READ_CFG_LOC, bEEPROM));
						FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_READ_CFG_ADR, uiPage));
						FEDM_CHK2(iErr, SendProtocol(0x80));
						iReturnStatus = 0x11;
					}
					else if(iErr == 0x13 || // Login-Request
							iErr == 0x19 ) // crypto processing error
					{
						FEDM_RETURN(iErr);
					}
					else
					{
						bModified = true;
					}

					// mark modified page in flag field
					iIdx = uiPage/8;
					iBitPos = uiPage % 8;
					iBitNr = 0x80;
					iBitNr >>= iBitPos;
					m_ucModifiedCfgPages[iIdx] |= iBitNr;
				}
			}
		}
		break;

	case FEDM_ISC_TYPE_ISCLR2500_A:
	case FEDM_ISC_TYPE_ISCLR2500_B:
#ifdef FEDM_NEW_LRU3000_ACC_CFG
	case FEDM_ISC_TYPE_ISCLRU3000:
#endif
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8B_WRITE_DEVICE,		(unsigned char)FEDM_CNTRL_TYPE_RFC));		// device: ACC
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8B_WRITE_BANK,		(unsigned char)FEDM_MEM_BANK_TYPE_MAIN));	// bank: main
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8B_WRITE_MODE,		(unsigned char)0));		// reset mode byte
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8B_WRITE_MODE_LOC,	bEEPROM));				// mode: write to EEPROM
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8B_WRITE_CFG_N,		(unsigned char)1));		// write one block
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8B_WRITE_CFG_SIZE,	(unsigned char)(m_iRFC_DataBlockSize-2)));	// blocksize

		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8A_READ_DEVICE,		(unsigned char)FEDM_CNTRL_TYPE_RFC));		// device: ACC
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8A_READ_BANK,		(unsigned char)FEDM_MEM_BANK_TYPE_MAIN));	// bank: main
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8A_READ_MODE,		(unsigned char)0));		// reset mode byte
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8A_READ_MODE_LOC,	bEEPROM));				// mode: write to EEPROM
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8A_READ_REQ_CFG_N,	(unsigned char)1));		// write one block

		if(bIsMode0x41Read)
		{
			// write with knowledge about write permissions
			uiPage = 0;
			for(iIdx=0; iIdx<16; iIdx++) // max 16 permission bytes
			{
				iBitNr = 0x80;
				for(iBitPos=0; iBitPos<8; iBitPos++)
				{
					if(ucCfgWritePermissions[iIdx] & iBitNr)
					{
						if( (  bEEPROM  && (m_RFC_EEData[uiPage*m_iRFC_DataBlockSize+m_iRFC_DataBlockSize-1] & 0x02))  ||
							((!bEEPROM) && (m_RFC_RAMData[uiPage*m_iRFC_DataBlockSize+m_iRFC_DataBlockSize-1] & 0x02)) )
						{
							FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_0x8B_WRITE_CFG_ADR, uiPage));
							FEDM_CHK2(iErr, SendProtocol(0x8B));
							if(iErr == 0x11) // Parameter Range Error
							{
								FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_0x8A_READ_REQ_CFG_ADR, uiPage));
								FEDM_CHK2(iErr, SendProtocol(0x8A));
								iReturnStatus = 0x11;
							}
							else if(iErr == 0x13 || // Login-Request
									iErr == 0x19 ) // crypto processing error
							{
								FEDM_RETURN(iErr);
							}
							else
							{
								bModified = true;
							}

							// mark modified page in flag field
							m_ucModifiedCfgPages[iIdx] |= iBitNr;
						}
					}

					iBitNr >>= 1;
					uiPage++;
				}
			}
		}
		else
		{
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
		}

		break;

#ifndef FEDM_NEW_LRU3000_ACC_CFG
	case FEDM_ISC_TYPE_ISCLRU3000:
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8B_WRITE_DEVICE,		(unsigned char)FEDM_CNTRL_TYPE_ACC));		// device: ACC
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8B_WRITE_BANK,		(unsigned char)FEDM_MEM_BANK_TYPE_MAIN));	// bank: main
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8B_WRITE_MODE,		(unsigned char)0));		// reset mode byte
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8B_WRITE_MODE_LOC,	bEEPROM));				// mode: write to EEPROM
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8B_WRITE_CFG_N,		(unsigned char)1));		// write one block
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8B_WRITE_CFG_SIZE,	(unsigned char)30));	// blocksize 30-Byte

		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8A_READ_DEVICE,		(unsigned char)FEDM_CNTRL_TYPE_ACC));		// device: ACC
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8A_READ_BANK,		(unsigned char)FEDM_MEM_BANK_TYPE_MAIN));	// bank: main
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8A_READ_MODE,		(unsigned char)0));		// reset mode byte
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8A_READ_MODE_LOC,	bEEPROM));				// mode: write to EEPROM
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8A_READ_REQ_CFG_N,	(unsigned char)1));		// write one block

		if(bIsMode0x41Read)
		{
			// write with knowledge about write permissions
			uiPage = 0;
			for(iIdx=0; iIdx<16; iIdx++) // max 16 permission bytes
			{
				iBitNr = 0x80;
				for(iBitPos=0; iBitPos<8; iBitPos++)
				{
					if(ucCfgWritePermissions[iIdx] & iBitNr)
					{
						if( (  bEEPROM  && (uiPage < 64 && (m_RFC_EEData[uiPage*16+15] & 0x02)))  ||
							(  bEEPROM  && (uiPage > 63 && (m_ACC_EEData[uiPage*32+31] & 0x02)))  ||
							((!bEEPROM) && (uiPage < 64 && (m_RFC_RAMData[uiPage*16+15] & 0x02))) ||
							((!bEEPROM) && (uiPage > 63 && (m_ACC_RAMData[uiPage*32+31] & 0x02))) )
						{
							FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_0x8B_WRITE_CFG_ADR, uiPage));
							FEDM_CHK2(iErr, SendProtocol(0x8B));
							if(iErr == 0x11) // Parameter Range Error
							{
								FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_0x8A_READ_REQ_CFG_ADR, uiPage));
								FEDM_CHK2(iErr, SendProtocol(0x8A));
								iReturnStatus = 0x11;
							}
							else if(iErr == 0x13 || // Login-Request
									iErr == 0x19 ) // crypto processing error
							{
								FEDM_RETURN(iErr);
							}
							else
							{
								bModified = true;
							}

							// mark modified page in flag field
							m_ucModifiedCfgPages[iIdx] |= iBitNr;
						}
					}

					iBitNr >>= 1;
					uiPage++;
				}
			}
		}
		else
		{
			for(uiPage=0; uiPage<63; ++uiPage)
			{
				if(	(  bEEPROM  && (m_RFC_EEData[uiPage*16+15] & 0x02)) ||
					((!bEEPROM) && (m_RFC_RAMData[uiPage*16+15] & 0x02)) )
				{
					FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8B_WRITE_CFG_ADR, uiPage));	// block address
					FEDM_CHK2(iErr, SendProtocol(0x8B));
					if(iErr == 0x11) // Parameter Range Error
					{
						FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_0x8A_READ_REQ_CFG_ADR, uiPage));
						FEDM_CHK2(iErr, SendProtocol(0x8A));
						iReturnStatus = 0x11;
					}
					else if(iErr == 0x13 || // Login-Request
							iErr == 0x19 ) // crypto processing error
					{
						FEDM_RETURN(iErr);
					}
					else
					{
						bModified = true;
					}

					// mark modified page in flag field
					iIdx = uiPage/8;
					iBitPos = uiPage % 8;
					iBitNr = 0x80;
					iBitNr >>= iBitPos;
					m_ucModifiedCfgPages[iIdx] |= iBitNr;
				}
			}

			for(uiPage=64; uiPage<84; ++uiPage)
			{
				if(	(  bEEPROM  && (m_ACC_EEData[uiPage*32+31] & 0x02)) ||
					((!bEEPROM) && (m_ACC_RAMData[uiPage*32+31] & 0x02)) )
				{
					FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8B_WRITE_CFG_ADR, uiPage));	// block address
					FEDM_CHK2(iErr, SendProtocol(0x8B));
					if(iErr == 0x11) // Parameter Range Error
					{
						FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_0x8A_READ_REQ_CFG_ADR, uiPage));
						FEDM_CHK2(iErr, SendProtocol(0x8A));
						iReturnStatus = 0x11;
					}
					else if(iErr == 0x13 || // Login-Request
							iErr == 0x19 ) // crypto processing error
					{
						FEDM_RETURN(iErr);
					}
					else
					{
						bModified = true;
					}

					// mark modified page in flag field
					iIdx = uiPage/8;
					iBitPos = uiPage % 8;
					iBitNr = 0x80;
					iBitNr >>= iBitPos;
					m_ucModifiedCfgPages[iIdx] |= iBitNr;
				}
			}
		}
		break;
#endif
	default:
		FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_WRITE_CFG, (unsigned char)0));
		FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_WRITE_CFG_LOC, bEEPROM));
		if(bIsMode0x41Read)
		{
			// write with knowledge about write permissions
			uiPage = 0;
			for(iIdx=0; iIdx<16; iIdx++) // max 16 permission bytes
			{
				iBitNr = 0x80;
				for(iBitPos=0; iBitPos<8; iBitPos++)
				{
					if(ucCfgWritePermissions[iIdx] & iBitNr)
					{
						if(uiPage*16 + 15 >= FEDM_ISC_MAX_RFC_EEDATA_MEM)
							continue;

						if(	(  bEEPROM  && (m_RFC_EEData[uiPage*16+15] & 0x02)) ||
							((!bEEPROM) && (m_RFC_RAMData[uiPage*16+15] & 0x02)) )
						{
							FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_WRITE_CFG_ADR, uiPage));
							FEDM_CHK2(iErr, SendProtocol(0x81));
							if(iErr == 0x11) // Parameter Range Error
							{
								FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_READ_CFG_ADR, uiPage));
								FEDM_CHK2(iErr, SendProtocol(0x80));
								iReturnStatus = 0x11;
							}
							else if(iErr == 0x13 || // Login-Request
									iErr == 0x19 ) // crypto processing error
							{
								FEDM_RETURN(iErr);
							}
							else
							{
								bModified = true;
							}

							// mark modified page in flag field
							m_ucModifiedCfgPages[iIdx] |= iBitNr;
						}
					}

					iBitNr >>= 1;
					uiPage++;
				}
			}
		}
		else
		{
			FEDM_RETURN(FEDM_ERROR_UNKNOWN_READER_TYPE);
		}
	}

	if(bModified)
	{
		if(iReturnStatus == FEDM_OK)
		{
			FEDM_RETURN(FEDM_MODIFIED);
		}
		else
		{
			FEDM_RETURN(iReturnStatus);
		}
	}
	else
	{
		FEDM_RETURN(iReturnStatus);
	}
}


unsigned char* FEDM_ISCReaderModule::GetModifiedCfgPageFlags()
{
	return m_ucModifiedCfgPages;
}


/***************************************************************************
  Begin        		:	05.08.2008 / M. Hultsch

  Version       	:	04.05.04 / 03.09.2013 / M. Hultsch
						- new reader: ID ISC.LRU1002

						03.03.00 / 31.01.2011 / M. Hultsch
						- with automatic detect, when busaddress, baudrate or frame are modified
  
						03.01.01 / 10.03.2010 / M. Hultsch
						- with final CPU-Reset
  
						03.01.00 / 12.02.2010 / M. Hultsch
						- support for crypto mode
  
						03.00.03 / 27.01.2009 / M. Hultsch

  Function			:	resets complete configuration to factory default

  Parameters		:	bool bEEPROM	- true, if location is EEPROM

  Return value		:	FEDM_OK or error code (<0) or status byte (>0)
***************************************************************************/
int FEDM_ISCReaderModule::ResetCompleteConfiguration(bool bEEPROM)
{
	if(bEEPROM)
	{
		FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_HIGH_LEVEL, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "EEPROM");
	}
	else
	{
		FEDM_TRACE_FNC(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_HIGH_LEVEL, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "RAM");
	}

	int iErr = 0;
	int iBack = 0;
	__int64 t1, t2;
	bool bDetect = false;
	unsigned char ucOldBusAdr = 0;
	unsigned char ucOldBaudrate = 0;
	unsigned char ucOldParity = 0;
	unsigned char ucNewBusAdr = 0;
	unsigned char ucNewBaudrate = 0;
	unsigned char ucNewParity = 0;

	if(! IsConnected())
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_NOT_CONNECTED));
		FEDM_RETURN(FEDM_ERROR_NOT_CONNECTED);
	}

	GetConfigPara(CoreImpl::ReaderConfig::HostInterface::Serial::BusAddress, &ucOldBusAdr);
	GetConfigPara(CoreImpl::ReaderConfig::HostInterface::Serial::Baudrate, &ucOldBaudrate);
	GetConfigPara(CoreImpl::ReaderConfig::HostInterface::Serial::Parity, &ucOldParity);

	switch(m_uiReaderType)
	{
	case FEDM_ISC_TYPE_ISCLR2500_A:
	case FEDM_ISC_TYPE_ISCLR2500_B:
#ifdef FEDM_NEW_LRU3000_ACC_CFG
	case FEDM_ISC_TYPE_ISCLRU3000:
#endif
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8C_RESET_DEVICE,		(unsigned char)FEDM_CNTRL_TYPE_RFC));		// device: ACC
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8C_RESET_BANK,		(unsigned char)FEDM_MEM_BANK_TYPE_MAIN));	// bank: main
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8C_RESET_MODE,		(unsigned char)0x00));	// reset mode byte
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8C_RESET_MODE_LOC,	bEEPROM));				// mode: reset in EEPROM and or RAM
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8C_RESET_MODE_ALL,	true));	// mode: reset all
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8C_RESET_CFG_N,		(unsigned char)0));		// number of block not necessary
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8C_RESET_CFG_ADR,	(unsigned char)0));		// first block address not necessary
		FEDM_CHK2(iErr, SendProtocol(0x8C));
		if(	iErr == 0x13 || // Login-Request
			iErr == 0x19 ) // crypto processing error
		{
			FEDM_RETURN(iErr);
		}
		break;

#ifndef FEDM_NEW_LRU3000_ACC_CFG
	case FEDM_ISC_TYPE_ISCLRU3000:
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8C_RESET_DEVICE,		(unsigned char)FEDM_CNTRL_TYPE_ACC));		// device: ACC
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8C_RESET_BANK,		(unsigned char)FEDM_MEM_BANK_TYPE_MAIN));	// bank: main
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8C_RESET_MODE,		(unsigned char)0x00));	// reset mode byte
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8C_RESET_MODE_LOC,	bEEPROM));				// mode: reset in EEPROM and or RAM
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8C_RESET_MODE_ALL,	true));	// mode: reset all
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8C_RESET_CFG_N,		(unsigned char)0));		// number of block not necessary
		FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8C_RESET_CFG_ADR,	(unsigned char)0));		// first block address not necessary
		FEDM_CHK2(iErr, SendProtocol(0x8C));
		if(	iErr == 0x13 || // Login-Request
			iErr == 0x19 ) // crypto processing error
		{
			FEDM_RETURN(iErr);
		}
		break;
#endif
	default:
		FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_RESET_CFG, (unsigned char)0));
		FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_RESET_CFG_LOC, bEEPROM));	// RAM and EEPROM
		FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_RESET_CFG_MODE, true));	// all configuration pages
		FEDM_CHK2(iErr, SendProtocol(0x83));
		if(	iErr == 0x13 || // Login-Request
			iErr == 0x19 ) // crypto processing error
		{
			FEDM_RETURN(iErr);
		}
		break;
	}

	// CPU-Reset
	if(bEEPROM)
	{
		if(FEDM_IS_COMPORT(GetPortHnd()))
		{
			switch(m_uiReaderType)
			{
			case FEDM_ISC_TYPE_ISCLR2500_B:
				FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8A_READ_DEVICE,		(unsigned char)FEDM_CNTRL_TYPE_RFC));		// device: ACC
				FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8A_READ_BANK,		(unsigned char)FEDM_MEM_BANK_TYPE_MAIN));	// bank: main
				FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8A_READ_MODE,		(unsigned char)0x00));	// reset mode byte
				FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8A_READ_MODE_LOC,	bEEPROM));				// mode: reset in EEPROM and or RAM
				FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8A_READ_REQ_CFG_N,	(unsigned char)1));		// 1 block
				FEDM_CHK2(iErr, SetData(FEDM_ISC_TMP_0x8A_READ_REQ_CFG_ADR,	(unsigned char)1));		// CFG1
				FEDM_CHK2(iErr, SendProtocol(0x8A));
				if(	iErr == 0x13 || // Login-Request
					iErr == 0x19 ) // crypto processing error
				{
					FEDM_RETURN(iErr);
				}
				break;

			default:
				FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_READ_CFG, (unsigned char)1));
				FEDM_CHK2(iErr, SetData( FEDM_ISC_TMP_READ_CFG_LOC, bEEPROM));	// RAM and EEPROM
				FEDM_CHK2(iErr, SendProtocol(0x80));
				if(	iErr == 0x13 || // Login-Request
					iErr == 0x19 ) // crypto processing error
				{
					FEDM_RETURN(iErr);
				}
				break;
			}

			// check for new detection
			GetConfigPara(CoreImpl::ReaderConfig::HostInterface::Serial::BusAddress, &ucNewBusAdr);
			GetConfigPara(CoreImpl::ReaderConfig::HostInterface::Serial::Baudrate, &ucNewBaudrate);
			GetConfigPara(CoreImpl::ReaderConfig::HostInterface::Serial::Parity, &ucNewParity);
			if(ucOldBusAdr != ucNewBusAdr)
			{
				SetBusAddress(ucNewBusAdr);
				bDetect = true;
			}

			if(ucOldBaudrate != ucNewBaudrate)
				bDetect = true;

			if(ucOldParity != ucNewParity)
				bDetect = true;
		}

		FEDM_CHK2(iErr, SendProtocol(0x63));

		// we must wait for Reader finishing CPU-Reset

		t1 = FEDM_GetUTCTimestamp() / 1000;	// time in µs
		t2 = t1 + (__int64)10000;

		if(FEDM_IS_COMPORT(GetPortHnd()))
		{
			FEDM_SLEEP(FE_1000MS);

			// detection method
			if(bDetect)
			{
				FindBaudRate();
			}
			else
			{
				int iTimeout = 0;
				GetPortPara("Timeout", &iTimeout);
				SetPortPara("Timeout", "200");
				while(SendProtocol(0x52))
				{
					t1 = FEDM_GetUTCTimestamp() / 1000;	// time in µs
					if(t1 > t2)
					{
						break;
					}
				}
				SetPortPara("Timeout", iTimeout);
			}
		}
		else if(FEDM_IS_USBPORT(GetPortHnd()))
		{
			// handling internally
		}
		else  if(FEDM_IS_TCPPORT(GetPortHnd()))
		{
			char cConIPAdr[16];
			int iConIPPort = 0;

			this->GetPortPara("HostAdr", cConIPAdr);
			this->GetPortPara("PortNr", &iConIPPort);

			DisConnect();
			FEDM_SLEEP(FE_1000MS);

			while(ConnectTCP(cConIPAdr, iConIPPort))
			{
				FEDM_SLEEP(FE_500MS);

				t1 = FEDM_GetUTCTimestamp() / 1000;	// time in µs
				if(t1 > t2)
				{
					// error
					break;
				}
			}
		}
	}

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        		:	13.08.2007 / M. Hultsch
  Version       	:	03.00.02 / 14.01.2009 / M. Hultsch

  Function			:	main-function for XML based reader profiling

  Parameters		:	bool bRead		-	if true : serialize from XML-File
											if false: serialize into XML-File
						char* sFileName	-	pointer to file name with directory

  Return value		:	OK or error code

***************************************************************************/
int FEDM_ISCReaderModule::SerializeProfile(bool bRead, char* sFileName)
{
#ifdef _FEDM_XML_SUPPORT
	int iBack = FEDM_DataBase::SerializeProfile(&m_ProfileModule, bRead, sFileName);

	FEDM_RETURN(iBack);
#else
	FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
#endif
}


/***************************************************************************
  Begin        		:	11.11.2003 / M. Hultsch

  Version       	:	03.03.01 / 31.03.2011 / M. Hultsch

  Function			:	transfer configuration from XML file to reader

  Parameters		:	char* sFileName	-	pointer to file name with directory

  Return value		:	FEDM_OK or error code (<0) or status byte (>0)
***************************************************************************/
int FEDM_ISCReaderModule::TransferXmlFileToReaderCfg(char* sFileName)
{
#ifdef _FEDM_XML_SUPPORT
	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_HIGH_LEVEL, __FUNCTION__);

	int iBack = 0;

	if(! IsConnected())
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_NOT_CONNECTED));
		FEDM_RETURN(FEDM_ERROR_NOT_CONNECTED);
	}

	// get all information from reader
	if(NULL == ReadReaderInfo())
		return this->GetLastError();

	// read from file
	FEDM_CHK2(iBack, Serialize(true, sFileName));

	// write to readers EEPROM and RAM
	FEDM_CHK4(iBack, WriteCompleteConfiguration(true));

	// execute CPU reset
	FEDM_CHK2(iBack, SendProtocol(0x63));

	// copy EEPROM content to RAM memory
	m_RFC_RAMData = m_RFC_EEData;

	if(m_ACC_EEData.size() > 0)
		m_ACC_RAMData = m_ACC_EEData;

	FEDM_RETURN(FEDM_OK);
#else
	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
	FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
#endif
}

/***************************************************************************
  Begin        		:	11.11.2003 / M. Hultsch

  Version       	:	03.00.00 / 28.10.2008 / M. Hultsch

  Function			:	transfer configuration from reader to XML file

  Parameters		:	char* sFileName	-	pointer to file name with directory

  Return value		:	FEDM_OK or error code (<0) or status byte (>0)
***************************************************************************/
int FEDM_ISCReaderModule::TransferReaderCfgToXmlFile(char* sFileName)
{
#ifdef _FEDM_XML_SUPPORT
	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_HIGH_LEVEL, __FUNCTION__);

	int iBack = 0;

	if(! IsConnected())
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_NOT_CONNECTED));
		FEDM_RETURN(FEDM_ERROR_NOT_CONNECTED);
	}

	// get all information from reader
	if(NULL == ReadReaderInfo())
		return this->GetLastError();

	// read from readers EEPROM and RAM
	FEDM_CHK4(iBack, ReadCompleteConfiguration(true));
	FEDM_CHK4(iBack, ReadCompleteConfiguration(false));

	// write to file
	FEDM_CHK2(iBack, Serialize(false, sFileName));

	FEDM_RETURN(FEDM_OK);
#else
	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " " << GetErrorText(FEDM_ERROR_UNSUPPORTED));
	FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
#endif
}

/***************************************************************************
  Begin        		:	06.11.2003 / M. Hultsch

  Version       	:	01.09.00 / 06.11.2003 / M. Hultsch

  Function			:	gets readers software version

  Parameters		:	unsigned char* ucSWRev	-	pointer to software version (2 bytes)
						unsigned char* ucDevRev	-	pointer to reader developer revision (1 byte)
						unsigned char* ucHWType	-	pointer to hardware identifier (1 byte)
						unsigned char* ucSWType	-	pointer to reader type (1 byte)
						unsigned char* ucTrp	-	poitner to transponder support (2 bytes)

  Return value		:	FEDM_OK or error code (<0) or status byte (>0)
***************************************************************************/
int FEDM_ISCReaderModule::ReadSoftVersion(	unsigned char* ucSWRev, 
											unsigned char* ucDevRev, 
											unsigned char* ucHWType, 
											unsigned char* ucSWType, 
											unsigned char* ucTrp )
{
	int iErr;
	FEDM_CHK3(ucSWRev);
	FEDM_CHK3(ucDevRev);
	FEDM_CHK3(ucHWType);
	FEDM_CHK3(ucSWType);
	FEDM_CHK3(ucTrp);

	FEDM_CHK4(iErr, SendProtocol(0x65));

	FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_SOFTVER_SW_REV, ucSWRev, 2));
	FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_SOFTVER_D_REV, ucDevRev));
	FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_SOFTVER_HW_TYPE, ucHWType));
	FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_SOFTVER_SW_TYPE, ucSWType));
	FEDM_CHK2(iErr, GetData(FEDM_ISC_TMP_SOFTVER_TR_TYPE, ucTrp, 2));

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        		:	19.06.2007 / M. Hultsch

  Version       	:	04.05.05 / 29.11.2013 / M. Hultsch
						- new mode: 0x09 for WLAN-Firmware

						04.05.04 / 04.09.2013 / M. Hultsch
						- new reader: ID ISC.LRU1002

						04.02.06 / 25.07.2012 / M. Hultsch
						- new mode 0x08 (with firmware supported tag drivers)

						03.03.00 / 10.02.2011 / M. Hultsch

  Function			:	gets readers complete information from reader
						>> partial read is possible <<

  Parameters		:	unsigned int uiProtocolFrame	- frame to be used for communication
														  (FEDM_PRT_FRAME_STANDARD, FEDM_PRT_FRAME_ADVANCED)

  Return value		:	FEDM_ISC_READER_INFO*	- pointer to info structure
						or
						NULL in error case
***************************************************************************/
FEDM_ISC_READER_INFO* FEDM_ISCReaderModule::ReadReaderInfo(unsigned int uiProtocolFrame)
{
	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_HIGH_LEVEL, __FUNCTION__);

	unsigned char ucData[32];
	int iBack = 0;
	int iMemAdr = 0;
	unsigned int uiPageCount = 0;

	m_ReaderInfo.Init();

	// for support of all readers: use standard protocol frame
	switch(this->m_uiReaderType)
	{
	case FEDM_ISC_TYPE_ISCLR2500_A:
	case FEDM_ISC_TYPE_ISCLRU3000:
		SetProtocolFrameSupport(FEDM_PRT_FRAME_ADVANCED);
		break;
	default:
		SetProtocolFrameSupport(uiProtocolFrame);
		break;
	}

	// ####################################################################################
	// try to get all
	// mode: 0xFF - all
	// ####################################################################################
	FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0xFF));
	FEDM_CHK7(iBack, SendProtocol(0x66));
	if(iBack==0x19) // crypto processing error
	{
		this->SetLastError(iBack);
		return NULL;
	}
	
	if(iBack==0x00 || iBack==0x84 || iBack==0xF1)
	{
		// ####################################################################################
		// frequency information (EU, FCC) for older reader types
		// ####################################################################################
		switch(m_ReaderInfo.ucReaderType)	// reader type
		{
		// HF/UHF function unit types
		case FEDM_ISC_TYPE_ANT_DAT:
		case FEDM_ISC_TYPE_ANT_UMUX:
		// classic-pro Reader
		case FEDM_ISC_TYPE_CPRM02:
		case FEDM_ISC_TYPE_CPR02:
		case FEDM_ISC_TYPE_CPR04_U:
		// i-scan HF Reader
		case FEDM_ISC_TYPE_ISCLR200:
		case FEDM_ISC_TYPE_ISCM02:
			// standard frame
			break;

		// UHF-Reader
		case FEDM_ISC_TYPE_ISCLRU1000:
			FEDM_CHK7(iBack, FEISC_0x80_ReadConfBlock(	GetReaderHnd(),
														GetBusAddress(), 
														0x43,
														ucData,
														0 ));

			if(iBack==0x00 || iBack==0x18 || iBack==0x84)
			{
				m_ReaderInfo.ucRfcFrequency		= ucData[5];	// 1: EU; 2: FCC
				m_ReaderInfo.bIsRfcFrequency	= true;
			}
			SetProtocolFrameSupport(FEDM_PRT_FRAME_ADVANCED);
			break;

		case FEDM_ISC_TYPE_ISCLRU2000:
			if(m_ReaderInfo.ucRfcSwVer[0] >= 2)
			{
				m_ReaderInfo.bIsRfcFrequency = true; // beginning with Release 2.0.0
													 // LRU2000 is compatible with standard
			}
			else
			{
				FEDM_CHK7(iBack, FEISC_0x80_ReadConfBlock(	GetReaderHnd(),
															GetBusAddress(), 
															0x43,
															ucData,
															0 ));

				if(iBack==0x00 || iBack==0x18 || iBack==0x84)
				{
					m_ReaderInfo.ucRfcFrequency		= ucData[5];	// 1: EU; 2: FCC
					m_ReaderInfo.bIsRfcFrequency	= true;
				}
			}
			SetProtocolFrameSupport(FEDM_PRT_FRAME_ADVANCED);
			break;

		default:
			SetProtocolFrameSupport(FEDM_PRT_FRAME_ADVANCED);
			break;
		}

		SetLastError(FEDM_OK);
		return &m_ReaderInfo;
	}


	// ####################################################################################
	// new standard protocol - must have all newer readers implemented
	// mode: 0x00 - RFC firmware version
	// ####################################################################################
	FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x00));
	FEDM_CHK7(iBack, SendProtocol(0x66));
	if(iBack==0x00 || iBack==0x84 || iBack==0xF1)
	{
		FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_SOFTVER, &m_ReaderInfo.ucRfcSwVer[0], 16));
		FEDM_CHK7(iBack, SetReaderType(m_ReaderInfo.ucReaderType));
		m_ReaderInfo.bIsMode0x00Read = true;
	}
	else
	{
		// ####################################################################################
		// formerly standard protocol - must have all readers implemented
		// same as [0x66] with mode 0x00
		// ####################################################################################
		FEDM_CHK7(iBack, SendProtocol(0x65));
		if(iBack==0x00 || iBack==0x84 || iBack==0xF1)
		{
			FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_SOFTVER, &m_ReaderInfo.ucRfcSwVer[0], 11));
			FEDM_CHK7(iBack, SetReaderType(m_ReaderInfo.ucReaderType));
			m_ReaderInfo.bIsMode0x00Read = true;
		}
	}

	switch(m_ReaderInfo.ucReaderType)	// reader type
	{
	// exceptions for Readers without advanced protocol frame support
	// HF/UHF function unit types
	case FEDM_ISC_TYPE_ANT_DAT:
	case FEDM_ISC_TYPE_ANT_UMUX:
	// classic-pro Reader
	case FEDM_ISC_TYPE_CPRM02:
	case FEDM_ISC_TYPE_CPR02:
	case FEDM_ISC_TYPE_CPR04_U:
	// i-scan HF Reader
	case FEDM_ISC_TYPE_ISCLR200:
	case FEDM_ISC_TYPE_ISCM02:
		// standard frame
		break;

	// exceptions for Readers with only advanced protocol frame support
	default:
		SetProtocolFrameSupport(FEDM_PRT_FRAME_ADVANCED);
		break;
	}
	
	// ReaderInfo is partial or completely read
	m_ReaderInfo.bIsInfoRead = true;


	// ####################################################################################
	// mode: 0x01 - ACC firmware version
	// ####################################################################################
	switch(m_ReaderInfo.ucReaderType)	// reader type
	{
	// HF-Reader
	case FEDM_ISC_TYPE_ISCLR2000:
	case FEDM_ISC_TYPE_ISCLR2500_A:
	// UHF-Reader
	case FEDM_ISC_TYPE_ISCLRU1000:
	case FEDM_ISC_TYPE_ISCLRU2000:
	case FEDM_ISC_TYPE_ISCLRU3000:
		FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x01));
		FEDM_CHK7(iBack, SendProtocol(0x66));
		if(iBack==0x00 || iBack==0x84 || iBack==0xF1)
		{
			FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, &m_ReaderInfo.ucAccSwVer[0], 4));
			m_ReaderInfo.bIsMode0x01Read = true;
		}
		break;
	}


	// ####################################################################################
	// mode: 0x02 - USB-Controller firmware version
	// ####################################################################################
	switch(m_ReaderInfo.ucReaderType)	// reader type
	{
	case FEDM_ISC_TYPE_CPR04_U:
		FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x02));
		FEDM_CHK7(iBack, SendProtocol(0x66));
		if(iBack==0x00 || iBack==0x84 || iBack==0xF1)
		{
			FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, &m_ReaderInfo.ucUsbSwVer[0], 4));
			m_ReaderInfo.bIsMode0x02Read = true;
		}
		break;
	}


	// ####################################################################################
	// mode: 0x03 - FPGA firmware version
	// ####################################################################################
	switch(m_ReaderInfo.ucReaderType)	// reader type
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
	// myAXXESS
	case FEDM_ISC_TYPE_MAX_ONTOP:
	case FEDM_ISC_TYPE_MAXU1002:
	// HF-Reader
	case FEDM_ISC_TYPE_ISCLR2000:
	case FEDM_ISC_TYPE_ISCLR2500_A:
	case FEDM_ISC_TYPE_ISCLR2500_B:
	// UHF-Reader
	case FEDM_ISC_TYPE_ISCMRU200:
	case FEDM_ISC_TYPE_ISCLRU1002:
	case FEDM_ISC_TYPE_ISCLRU2000:
	case FEDM_ISC_TYPE_ISCLRU3000:
		FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x03));
		FEDM_CHK7(iBack, SendProtocol(0x66));
		if(iBack==0x00 || iBack==0x84 || iBack==0xF1)
		{
			switch(m_ReaderInfo.ucReaderType)	// reader type
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
			// myAXXESS
			//case FEDM_ISC_TYPE_MAX_ONTOP:
				FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, &m_ReaderInfo.ucDecoderType[0], 6));
				m_ReaderInfo.bIsMode0x03Read = true;
				break;

			// HF-Reader
			case FEDM_ISC_TYPE_ISCLR2000:
			// myAXXESS
			case FEDM_ISC_TYPE_MAXU1002:
			// UHF-Reader
			case FEDM_ISC_TYPE_ISCMRU200:
			case FEDM_ISC_TYPE_ISCLRU1002:
			case FEDM_ISC_TYPE_ISCLRU2000:
			case FEDM_ISC_TYPE_ISCLRU3000:
				FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, &m_ReaderInfo.ucFpgaSwVer[0], 4));
				m_ReaderInfo.bIsMode0x03Read = true;
				break;
			}
		}
		break;
	}


	// ####################################################################################
	// mode: 0x04 - special firmware information
	// ####################################################################################
	switch(m_ReaderInfo.ucReaderType)	// reader type
	{
	// LF classic types
	case 20: // ID RW40.30
	// megalock C
	case FEDM_ISC_TYPE_MLC_COM:
	// classic-pro
	case FEDM_ISC_TYPE_CPRM02:
	case FEDM_ISC_TYPE_CPR02:
	case FEDM_ISC_TYPE_CPR04_U:
		FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x04));
		FEDM_CHK7(iBack, SendProtocol(0x66));
		if(iBack==0x00 || iBack==0x84 || iBack==0xF1)
		{
			FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, &m_ReaderInfo.ucCprFctTemplate, 2));
			m_ReaderInfo.bIsMode0x04Read = true;
		}
		break;

	case FEDM_ISC_TYPE_CPR20_XX:
	case FEDM_ISC_TYPE_CPR30_XX:
	case FEDM_ISC_TYPE_CPR40_XX_U:
	case FEDM_ISC_TYPE_CPR40_XX:
	case FEDM_ISC_TYPE_CPR44_XX:
	case FEDM_ISC_TYPE_CPR46_XX:
	case FEDM_ISC_TYPE_CPR47_XX:
	case FEDM_ISC_TYPE_CPR50_XX:
	case FEDM_ISC_TYPE_CPR52_XX:
	case FEDM_ISC_TYPE_CPR60_XX:
	// myAXXESS
	case FEDM_ISC_TYPE_MAX50_XX:
	case FEDM_ISC_TYPE_MAX_STANDARD:
	case FEDM_ISC_TYPE_MAX_COMFORT:
	case FEDM_ISC_TYPE_MAX_ONTOP:
		FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x04));
		FEDM_CHK7(iBack, SendProtocol(0x66));
		if(iBack==0x00 || iBack==0x84 || iBack==0xF1)
		{
			FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, &m_ReaderInfo.ucCprFctTemplate, 11));
			m_ReaderInfo.bIsMode0x04Read = true;
		}
		break;

	// HF-Reader
	case FEDM_ISC_TYPE_ISCPRH200:
	case FEDM_ISC_TYPE_ISCLR2000:
	case FEDM_ISC_TYPE_ISCLR2500_A:
	case FEDM_ISC_TYPE_ISCLR2500_B:
	case FEDM_ISC_TYPE_ISCLR1002:
	// myAXXESS
	case FEDM_ISC_TYPE_MAXU1002:
	// UHF-Reader
	case FEDM_ISC_TYPE_ISCMU02:
	case FEDM_ISC_TYPE_ISCLRU1002:
	case FEDM_ISC_TYPE_ISCLRU2000:
	case FEDM_ISC_TYPE_ISCLRU3000:
		FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x04));
		FEDM_CHK7(iBack, SendProtocol(0x66));
		if(iBack==0x00 || iBack==0x84 || iBack==0xF1)
		{
			FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, &m_ReaderInfo.ucReserved1, 11));
			m_ReaderInfo.bIsMode0x04Read = true;
		}
		break;
	}


	// ####################################################################################
	// mode: 0x05 - Bootloader firmware version
	// ####################################################################################
	switch(m_ReaderInfo.ucReaderType)	// reader type
	{
	// LF classic types
	case 20: // ID RW40.30
	// megalock C
	case FEDM_ISC_TYPE_MLC_COM:
	// classic-pro
	case FEDM_ISC_TYPE_CPR20_XX:
	case FEDM_ISC_TYPE_CPR30_XX:
	case FEDM_ISC_TYPE_CPR40_XX_U:
	case FEDM_ISC_TYPE_CPR40_XX:
	case FEDM_ISC_TYPE_CPR44_XX:
	case FEDM_ISC_TYPE_CPR46_XX:
	case FEDM_ISC_TYPE_CPR47_XX:
	case FEDM_ISC_TYPE_CPR52_XX:
	case FEDM_ISC_TYPE_CPR60_XX:
	// myAXXESS
	case FEDM_ISC_TYPE_MAX50_XX:
	case FEDM_ISC_TYPE_MAX_STANDARD:
	case FEDM_ISC_TYPE_MAX_COMFORT:
	case FEDM_ISC_TYPE_MAX_ONTOP:
	case FEDM_ISC_TYPE_MAXU1002:
	// Dual-Frequency Reader
	case FEDM_ISC_TYPE_ISCPRHD102:
	// HF-Reader
	case FEDM_ISC_TYPE_ISCMR102:
	case FEDM_ISC_TYPE_ISCMR10X:
	case FEDM_ISC_TYPE_ISCPRH102:
	case FEDM_ISC_TYPE_ISCPRH200:
	case FEDM_ISC_TYPE_ISCLR1002:
	case FEDM_ISC_TYPE_ISCLR2000:
	case FEDM_ISC_TYPE_ISCLR2500_B:
	// UHF-Reader
	case FEDM_ISC_TYPE_ISCMRU200:
	case FEDM_ISC_TYPE_ISCLRU1002:
	case FEDM_ISC_TYPE_ISCLRU1000:
	case FEDM_ISC_TYPE_ISCLRU2000:
		FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x05));
		FEDM_CHK7(iBack, SendProtocol(0x66));
		if(iBack==0x00 || iBack==0x84 || iBack==0xF1)
		{
			FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, &m_ReaderInfo.ucBootSwVer[0], 2));
			m_ReaderInfo.bIsMode0x05Read = true;
		}
		break;
	}


	// ####################################################################################
	// mode: 0x06 - keyboard unit firmware version
	// ####################################################################################
	switch(m_ReaderInfo.ucReaderType)	// reader type
	{
	// classic-pro
	case FEDM_ISC_TYPE_CPR50_XX:
	// myAXXESS
	case FEDM_ISC_TYPE_MAX50_XX:
		FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x06));
		FEDM_CHK7(iBack, SendProtocol(0x66));
		if(iBack==0x00)
		{
			FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, &m_ReaderInfo.ucKeybStatus[0], 8));
			m_ReaderInfo.bIsMode0x06Read = true;
		}
		break;
	}


	// ####################################################################################
	// mode: 0x07 - CPRIO unit firmware version and IO information
	// ####################################################################################
	switch(m_ReaderInfo.ucReaderType)	// reader type
	{
	// classic-pro
	case FEDM_ISC_TYPE_CPR50_XX:
	// myAXXESS
	case FEDM_ISC_TYPE_MAX50_XX:
		FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x07));
		FEDM_CHK7(iBack, SendProtocol(0x66));
		if(iBack==0x00)
		{
			FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, &m_ReaderInfo.ucCprioSwVer[0], 7));
			m_ReaderInfo.bIsMode0x07Read = true;
		}
		break;
	}


	// ####################################################################################
	// mode: 0x08 - information with supported tag drivers
	// ####################################################################################
	switch(m_ReaderInfo.ucReaderType)	// reader type
	{
	// classic-pro
	case FEDM_ISC_TYPE_CPR46_XX:
	case FEDM_ISC_TYPE_CPR47_XX:
	// myAXXESS
	case FEDM_ISC_TYPE_MAX_ONTOP:
		FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x08));
		FEDM_CHK7(iBack, SendProtocol(0x66));
		if(iBack==0x00)
		{
			FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, &m_ReaderInfo.ucRfcSupportedTagDriver[0], 14));
			m_ReaderInfo.bIsMode0x08Read = true;
		}
		break;
	}


	// ####################################################################################
	// mode: 0x09 - information with WLAN-Modul firmware
	// ####################################################################################
	switch(m_ReaderInfo.ucReaderType)	// reader type
	{
	// i-scan HF
	case FEDM_ISC_TYPE_ISCPRH200:
		FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x09));
		FEDM_CHK7(iBack, SendProtocol(0x66));
		if(iBack==0x00)
		{
			FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, &m_ReaderInfo.ucWlanVer[0], 14));
			m_ReaderInfo.bIsMode0x09Read = true;
		}
		break;
	}


	// ####################################################################################
	// mode: 0x10 - additional hardware information
	// ####################################################################################
	switch(m_ReaderInfo.ucReaderType)	// reader type
	{
	// megalock C
	case FEDM_ISC_TYPE_MLC_COM:
	// classic-pro
	case FEDM_ISC_TYPE_CPR50_XX:
	case FEDM_ISC_TYPE_CPR52_XX:
	case FEDM_ISC_TYPE_CPR60_XX:
	// myAXXESS
	case FEDM_ISC_TYPE_MAX50_XX:
	case FEDM_ISC_TYPE_MAX_ONTOP:
	case FEDM_ISC_TYPE_MAXU1002:
	// UHF-Reader
	case FEDM_ISC_TYPE_ISCMU02:
	case FEDM_ISC_TYPE_ISCMRU102:
	case FEDM_ISC_TYPE_ISCLRU1002:
	case FEDM_ISC_TYPE_ISCLRU3000:
	// Dual-Frequency Reader
	case FEDM_ISC_TYPE_ISCPRHD102:
		FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x10));
		FEDM_CHK7(iBack, SendProtocol(0x66));
		if(iBack==0x00 || iBack==0x84 || iBack==0xF1)
		{
			FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, &m_ReaderInfo.ucRfcHwInfo[0], 11));
			m_ReaderInfo.bIsMode0x10Read	= true;

			m_ReaderInfo.bIsRfcFrequency	= true;
		}
		break;

	// HF-Reader
	case FEDM_ISC_TYPE_ISCMR102:
	case FEDM_ISC_TYPE_ISCPRH102:
	case FEDM_ISC_TYPE_ISCPRH200:
	case FEDM_ISC_TYPE_ISCMR10X:
	case FEDM_ISC_TYPE_ISCLR1002:
	case FEDM_ISC_TYPE_ISCLR2000:
	case FEDM_ISC_TYPE_ISCLR2500_A:
	case FEDM_ISC_TYPE_ISCLR2500_B:
		FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x10));
		FEDM_CHK7(iBack, SendProtocol(0x66));
		if(iBack==0x00 || iBack==0x84 || iBack==0xF1)
		{
			FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, &m_ReaderInfo.ucRfcHwInfo[0], 11));
			m_ReaderInfo.bIsMode0x10Read = true;
		}
		break;

	// UHF-Reader
	case FEDM_ISC_TYPE_ISCMRU200:
		FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x10));
		FEDM_CHK7(iBack, SendProtocol(0x66));
		if(iBack==0x00 || iBack==0x84 || iBack==0xF1)
		{
			FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, &m_ReaderInfo.ucRfcHwInfo[0], 11));
			m_ReaderInfo.bIsMode0x10Read = true;

			m_ReaderInfo.ucRfcFrequency	= (m_ReaderInfo.ucRfcHwInfo[1] & 0x01) + 1;	// 0: EU; 1: FCC
			m_ReaderInfo.bIsRfcFrequency	= true;
		}
		break;
	
	// UHF-Reader
	case FEDM_ISC_TYPE_ISCLRU1000:
		FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x10));
		FEDM_CHK7(iBack, SendProtocol(0x66));
		if(iBack==0x00 || iBack==0x84 || iBack==0xF1)
		{
			FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, &m_ReaderInfo.ucRfcHwInfo[0], 11));
			m_ReaderInfo.bIsMode0x10Read = true;
		}
		break;

	// UHF-Reader
	case FEDM_ISC_TYPE_ISCLRU2000:
		FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x10));
		FEDM_CHK7(iBack, SendProtocol(0x66));
		if(iBack==0x00 || iBack==0x84 || iBack==0xF1)
		{
			FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, &m_ReaderInfo.ucRfcHwInfo[0], 11));
			m_ReaderInfo.bIsMode0x10Read = true;

			if(m_ReaderInfo.ucRfcSwVer[0] >= 2)
				m_ReaderInfo.bIsRfcFrequency = true; // beginning with Release 2.0.0
													 // LRU2000 is compatible with standard
		}
		break;

	}


	// ####################################################################################
	// mode: 0x11 - SAM information
	// ####################################################################################
	switch(m_ReaderInfo.ucReaderType)	// reader type
	{
	// classic-pro
	case FEDM_ISC_TYPE_CPR40_XX_U:
	case FEDM_ISC_TYPE_CPR40_XX:
	case FEDM_ISC_TYPE_CPR44_XX:
	case FEDM_ISC_TYPE_CPR46_XX:
	case FEDM_ISC_TYPE_CPR47_XX:
	case FEDM_ISC_TYPE_CPR52_XX:
	case FEDM_ISC_TYPE_CPR60_XX:
	// myAXXESS
	case FEDM_ISC_TYPE_MAX_STANDARD:
	case FEDM_ISC_TYPE_MAX_COMFORT:
	case FEDM_ISC_TYPE_MAX_ONTOP:
		FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x11));
		FEDM_CHK7(iBack, SendProtocol(0x66));
		if(iBack==0x00 || iBack==0x84 || iBack==0xF1)
		{
			FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, &m_ReaderInfo.ucNoOfSlots, 5));
			m_ReaderInfo.bIsMode0x11Read = true;
		}
		break;
	}


	// ####################################################################################
	// mode: 0x12 - CPU information
	// ####################################################################################
	FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x12));
	FEDM_CHK7(iBack, SendProtocol(0x66));
	if(iBack==0x00 || iBack==0x84 || iBack==0xF1)
	{
		FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, &m_ReaderInfo.ucRfcCpuManufacturer, 8));
		m_ReaderInfo.bIsMode0x12Read = true;
	}


	// ####################################################################################
	// mode: 0x40 - CFG information with read permission
	// ####################################################################################
	FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x40));
	FEDM_CHK7(iBack, SendProtocol(0x66));
	if(iBack==0x00 || iBack==0xF1)
	{
		iMemAdr = FEDM_GetAdrOfID(FEDM_ISC_TMP_READER_INFO, 32);	// start address in memory
		FEDM_CHK7(iBack, GetData(iMemAdr, &m_ReaderInfo.ucNoOfCfgPages[0], 2, FEDM_TMPDATA_MEM));
		uiPageCount = ((m_ReaderInfo.ucNoOfCfgPages[0]<<8) + m_ReaderInfo.ucNoOfCfgPages[1]) / 8;
		if( ((m_ReaderInfo.ucNoOfCfgPages[0]<<8) + m_ReaderInfo.ucNoOfCfgPages[1]) % 8 )
			uiPageCount++;

		FEDM_CHK7(iBack, GetData(iMemAdr+2, &m_ReaderInfo.ucCfgReadPermissions[0], uiPageCount, FEDM_TMPDATA_MEM));
		m_ReaderInfo.bIsMode0x40Read = true;
	}


	// ####################################################################################
	// mode: 0x41 - CFG information with write permission
	// ####################################################################################
	FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x41));
	FEDM_CHK7(iBack, SendProtocol(0x66));
	if(iBack==0x00 || iBack==0xF1)
	{
		iMemAdr = FEDM_GetAdrOfID(FEDM_ISC_TMP_READER_INFO, 32);	// start address in memory
		FEDM_CHK7(iBack, GetData(iMemAdr, &m_ReaderInfo.ucNoOfCfgPages[0], 2, FEDM_TMPDATA_MEM));
		uiPageCount = (m_ReaderInfo.ucNoOfCfgPages[0]<<8) + m_ReaderInfo.ucNoOfCfgPages[1];
		if( ((m_ReaderInfo.ucNoOfCfgPages[0]<<8) + m_ReaderInfo.ucNoOfCfgPages[1]) % 8 )
			uiPageCount++;

		FEDM_CHK7(iBack, GetData(iMemAdr+2, &m_ReaderInfo.ucCfgWritePermissions[0], uiPageCount, FEDM_TMPDATA_MEM));
		m_ReaderInfo.bIsMode0x41Read = true;
	}


	// ####################################################################################
	// mode: 0x50 - information with MAC for LAN
	// ####################################################################################
	FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x50));
	FEDM_CHK7(iBack, SendProtocol(0x66));
	if(iBack==0x00 || iBack==0xF1)
	{
		FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, &m_ReaderInfo.ucLanFlags, 7));
		m_ReaderInfo.bIsMode0x50Read = true;
	}


	// ####################################################################################
	// mode: 0x51 - information with IP Address for LAN
	// ####################################################################################
	FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x51));
	FEDM_CHK7(iBack, SendProtocol(0x66));
	if(iBack==0x00 || iBack==0xF1)
	{
		FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, ucData, 30));
		m_ReaderInfo.ucLanFlags = ucData[0];
		memcpy(&m_ReaderInfo.ucLanIpV4[0], &ucData[1], 20);
		m_ReaderInfo.bIsMode0x51Read = true;
	}


	// ####################################################################################
	// mode: 0x52 - information with Netmask for LAN
	// ####################################################################################
	FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x52));
	FEDM_CHK7(iBack, SendProtocol(0x66));
	if(iBack==0x00 || iBack==0xF1)
	{
		FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, ucData, 30));
		m_ReaderInfo.ucLanFlags = ucData[0];
		memcpy(&m_ReaderInfo.ucLanNetmaskV4[0], &ucData[1], 20);
		m_ReaderInfo.bIsMode0x52Read = true;
	}


	// ####################################################################################
	// mode: 0x53 - information with Gateway Address for LAN
	// ####################################################################################
	FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x53));
	FEDM_CHK7(iBack, SendProtocol(0x66));
	if(iBack==0x00 || iBack==0xF1)
	{
		FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, ucData, 30));
		m_ReaderInfo.ucLanFlags = ucData[0];
		memcpy(&m_ReaderInfo.ucLanGatewayV4[0], &ucData[1], 20);
		m_ReaderInfo.bIsMode0x53Read = true;
	}


	// ####################################################################################
	// mode: 0x54 - information with MAC for WLAN
	// ####################################################################################
	FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x54));
	FEDM_CHK7(iBack, SendProtocol(0x66));
	if(iBack==0x00 || iBack==0xF1)
	{
		FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, &m_ReaderInfo.ucWlanFlags, 7));
		m_ReaderInfo.bIsMode0x54Read = true;
	}


	// ####################################################################################
	// mode: 0x55 - information with IP Address for WLAN
	// ####################################################################################
	FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x55));
	FEDM_CHK7(iBack, SendProtocol(0x66));
	if(iBack==0x00 || iBack==0xF1)
	{
		FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, ucData, 30));
		m_ReaderInfo.ucWlanFlags = ucData[0];
		memcpy(&m_ReaderInfo.ucWlanIpV4[0], &ucData[1], 20);
		m_ReaderInfo.bIsMode0x55Read = true;
	}


	// ####################################################################################
	// mode: 0x56 - information with Netmask for WLAN
	// ####################################################################################
	FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x56));
	FEDM_CHK7(iBack, SendProtocol(0x66));
	if(iBack==0x00 || iBack==0xF1)
	{
		FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, ucData, 30));
		m_ReaderInfo.ucWlanFlags = ucData[0];
		memcpy(&m_ReaderInfo.ucWlanNetmaskV4[0], &ucData[1], 20);
		m_ReaderInfo.bIsMode0x56Read = true;
	}


	// ####################################################################################
	// mode: 0x57 - information with Gateway Address for WLAN
	// ####################################################################################
	FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x57));
	FEDM_CHK7(iBack, SendProtocol(0x66));
	if(iBack==0x00 || iBack==0xF1)
	{
		FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, ucData, 30));
		m_ReaderInfo.ucWlanFlags = ucData[0];
		memcpy(&m_ReaderInfo.ucWlanGatewayV4[0], &ucData[1], 20);
		m_ReaderInfo.bIsMode0x57Read = true;
	}


	// ####################################################################################
	// mode: 0x60 - input/output capabilities information
	// ####################################################################################
	FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x60));
	FEDM_CHK7(iBack, SendProtocol(0x66));
	if(iBack==0x00 || iBack==0xF1)
	{
		FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, &m_ReaderInfo.ucNoOfInputs, 3));
		m_ReaderInfo.bIsMode0x60Read = true;
	}


	// ####################################################################################
	// mode: 0x61 - external peripheral devices information
	// ####################################################################################
	switch(m_ReaderInfo.ucReaderType)	// reader type
	{
	// classic-pro
	case FEDM_ISC_TYPE_ISCLR2000:
	case FEDM_ISC_TYPE_ISCLR2500_A:
	case FEDM_ISC_TYPE_ISCLR2500_B:
		FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x61));
		FEDM_CHK7(iBack, SendProtocol(0x66));
		if(iBack==0x00 || iBack==0xF1)
		{
			FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, &m_ReaderInfo.ucNoOfPeripheralDevices, 7));
			m_ReaderInfo.bIsMode0x61Read = true;
#if !defined(_FEDM_NO_PD_SUPPORT)
			FEDM_TRACE(m_pLogger, FELOGMNG_DBG_INFO, __FUNCTION__ << " - " << m_ReaderInfo.ucNoOfPeripheralDevices << " People-Counter detected");
			BuildPDMap();
#endif
		}
		break;
	}


	// ####################################################################################
	// mode: 0x80 - device information with serial number and licensed options
	// ####################################################################################
	FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_READER_INFO_MODE, (unsigned char)0x80));
	FEDM_CHK7(iBack, SendProtocol(0x66));
	if(iBack==0x00 || iBack==0x84 || iBack==0xF1)
	{
		FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_READER_INFO, &m_ReaderInfo.ucRfcDevID[0], 18));
		m_ReaderInfo.bIsMode0x80Read = true;
		FEDM_SETPROP2(m_pLogger, FELOGMNG_PROP_DEVICED_ID, m_ReaderInfo.GetDeviceID());
	}


	// ####################################################################################
	// frequency information (EU, FCC) for older reader types
	// ####################################################################################
	switch(m_ReaderInfo.ucReaderType)	// reader type
	{
	// UHF-Reader
	case FEDM_ISC_TYPE_ISCLRU1000:
	case FEDM_ISC_TYPE_ISCLRU2000:
		FEDM_CHK7(iBack, FEISC_0x80_ReadConfBlock(	GetReaderHnd(),
													GetBusAddress(), 
													0x43,
													ucData,
													0 ));

		if(iBack==0x00 || iBack==0x18 || iBack==0x84)
		{
			m_ReaderInfo.ucRfcFrequency		= ucData[5];	// 1: EU; 2: FCC
			m_ReaderInfo.bIsRfcFrequency	= true;
		}

		break;
	}

	SetLastError(FEDM_OK);
	return &m_ReaderInfo;
}

/***************************************************************************
  Begin        		:	09.12.2007 / M. Hultsch

  Version       	:	04.06.01 / 03.03.2014 / M. Hultsch
						- support for new mode 0x07

						04.04.00 / 06.12.2012 / M. Hultsch
						- support for new mode 0x21

						03.03.00 / 25.01.2011 / M. Hultsch

  Function			:	gets readers complete diagnostic information from reader
						>> partial read is possible <<

  Parameters		:	-

  Return value		:	FEDM_ISC_READER_DIAGNOSTIC*	- pointer to diagnostic structure
						or
						NULL in error case
***************************************************************************/
FEDM_ISC_READER_DIAGNOSTIC* FEDM_ISCReaderModule::ReadReaderDiagnostic()
{
	FEDM_TRACE(m_pLogger, FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_HIGH_LEVEL, __FUNCTION__);

	unsigned char ucTmp = 0;
	int iBack = 0;

	m_ReaderDiagnostic.Init();

	switch(this->m_uiReaderType)
	{
	// reader having not [0x6E] command
	case FEDM_ISC_TYPE_ANT_DAT:
	case FEDM_ISC_TYPE_ANT_UMUX:
	case FEDM_ISC_TYPE_CPRM02:
	case FEDM_ISC_TYPE_CPR02:
	case FEDM_ISC_TYPE_CPR20_XX:
	case FEDM_ISC_TYPE_CPR40_XX_U:
	case FEDM_ISC_TYPE_CPR40_XX:
	case FEDM_ISC_TYPE_CPR50_XX:
	case FEDM_ISC_TYPE_CPR44_XX:
	case FEDM_ISC_TYPE_CPR46_XX:
	case FEDM_ISC_TYPE_CPR47_XX:
	case FEDM_ISC_TYPE_CPR30_XX:
	case FEDM_ISC_TYPE_CPR52_XX:
	case FEDM_ISC_TYPE_CPR60_XX:
	case FEDM_ISC_TYPE_CPR04_U:
	case FEDM_ISC_TYPE_MAX_STANDARD:
	case FEDM_ISC_TYPE_MAX_COMFORT:
		SetLastError(FEDM_ERROR_UNSUPPORTED);
		return &m_ReaderDiagnostic;
	}


	// ####################################################################################
	// try to get all
	// mode: 0xFF - all
	// ####################################################################################
	FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_DIAG_MODE, (unsigned char)0xFF));
	FEDM_CHK7(iBack, SendProtocol(0x6E));
	if(iBack==0x00 || iBack==0x84 || iBack==0xF1)
	{
		if(m_uiReaderType == FEDM_ISC_TYPE_ISCLR2500_B)
		{
			if(!m_ReaderDiagnostic.bIsMode0x08Read)
			{
				// this case is applied, if the Reader returns NOT the diagnostic data for mode 0x08 when requested with mode 0xFF (all)
				SetData(FEDM_ISC_TMP_DIAG_MODE, (unsigned char)0x08);
				SendProtocol(0x6E);
			}
		}
		SetLastError(FEDM_OK);
		return &m_ReaderDiagnostic;
	}


	// ####################################################################################
	// mode: 0x01 - General RF Status
	// ####################################################################################
	FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_DIAG_MODE, (unsigned char)0x01));
	FEDM_CHK7(iBack, SendProtocol(0x6E));
	if(iBack==0x00 || iBack==0x84)
	{
		FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_DIAG_DATA, &m_ReaderDiagnostic.ucFlagsA, 2));
		m_ReaderDiagnostic.bIsMode0x01Read = true;
	}


	// ####################################################################################
	// mode: 0x02 - RF Power Amplifier Status
	// ####################################################################################
	FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_DIAG_MODE, (unsigned char)0x02));
	FEDM_CHK7(iBack, SendProtocol(0x6E));
	if(iBack==0x00 || iBack==0x84)
	{
		FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_DIAG_DATA, &m_ReaderDiagnostic.ucRfPower, 3));
		if(this->m_uiReaderType == FEDM_ISC_TYPE_ISCMR200)
		{
			// swap byte 0 and 1 to build Impedance value
			ucTmp = m_ReaderDiagnostic.ucRfPower;
			m_ReaderDiagnostic.ucRfPower = m_ReaderDiagnostic.ucRfModulation;
			m_ReaderDiagnostic.ucRfModulation = ucTmp;
		}
		m_ReaderDiagnostic.bIsMode0x02Read = true;
	}


	// ####################################################################################
	// mode: 0x03 - Multiplexer Status
	// ####################################################################################
	FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_DIAG_MODE, (unsigned char)0x03));
	FEDM_CHK7(iBack, SendProtocol(0x6E));
	if(iBack==0x00 || iBack==0x84)
	{
		FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_DIAG_DATA, &m_ReaderDiagnostic.ucMuxStatusCh1, 8));
		m_ReaderDiagnostic.bIsMode0x03Read = true;
	}


	// ####################################################################################
	// mode: 0x04 - Hardware and Configuration Status
	// ####################################################################################
	FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_DIAG_MODE, (unsigned char)0x04));
	FEDM_CHK7(iBack, SendProtocol(0x6E));
	if(iBack==0x00 || iBack==0x84)
	{
		FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_DIAG_DATA, &m_ReaderDiagnostic.ucIntErrorA, 2));
		m_ReaderDiagnostic.bIsMode0x04Read = true;
	}


	// ####################################################################################
	// mode: 0x05 - Multiplexer Inpedance Status
	// ####################################################################################
	FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_DIAG_MODE, (unsigned char)0x05));
	FEDM_CHK7(iBack, SendProtocol(0x6E));
	if(iBack==0x00 || iBack==0x84)
	{
		switch(m_uiReaderType)
		{
		case FEDM_ISC_TYPE_ISCLRU2000:
			FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_DIAG_DATA, (unsigned char*)&m_ReaderDiagnostic.CNTRL_OUT1.ucMux_PowerAmp, 12));
			m_ReaderDiagnostic.bIsMode0x05Read = true;
			break;

		case FEDM_ISC_TYPE_ISCLRU3000:
			FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_DIAG_DATA, (unsigned char*)&m_ReaderDiagnostic.CNTRL_OUT1.ucMux_PowerAmp, 12));
			m_ReaderDiagnostic.bIsMode0x05Read = true;
			break;

		default:
			FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_DIAG_DATA, (unsigned char*)&m_ReaderDiagnostic.CNTRL_OUT1.ucMux_PowerAmp, 3));
			m_ReaderDiagnostic.bIsMode0x05Read = true;
			break;
		}
	}


	// ####################################################################################
	// mode: 0x06 - MAX Status
	// ####################################################################################
	if(this->m_uiReaderType == FEDM_ISC_TYPE_MAX50_XX || 
		this->m_uiReaderType == FEDM_ISC_TYPE_MAXU1002)
	{
		FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_DIAG_MODE, (unsigned char)0x06));
		FEDM_CHK7(iBack, SendProtocol(0x6E));
		if(iBack==0x00 || iBack==0x84)
		{
			FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_DIAG_DATA, (unsigned char*)&m_ReaderDiagnostic.usErrorFlags, 6));
			m_ReaderDiagnostic.bIsMode0x06Read = true;
		}
	}


	// ####################################################################################
	// mode: 0x07 - LED Status
	// ####################################################################################
	if(this->m_uiReaderType == FEDM_ISC_TYPE_ISCPRH200)
	{
		FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_DIAG_MODE, (unsigned char)0x07));
		FEDM_CHK7(iBack, SendProtocol(0x6E));
		if(iBack==0x00 || iBack==0x84)
		{
			FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_DIAG_DATA, (unsigned char*)&m_ReaderDiagnostic.ucNoOfLEDs, 30));
			m_ReaderDiagnostic.bIsMode0x07Read = true;
		}
	}


	// ####################################################################################
	// mode: 0x08 - ID ISC.LR2500-B Antenna Tuning Status
	// ####################################################################################
	if(this->m_uiReaderType == FEDM_ISC_TYPE_ISCLR2500_B)
	{
		FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_DIAG_MODE, (unsigned char)0x08));
		FEDM_CHK7(iBack, SendProtocol(0x6E));
		if(iBack==0x00 || iBack==0x84)
		{
			FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_DIAG_DATA, (unsigned char*)&m_ReaderDiagnostic.usRes1, 15));
			m_ReaderDiagnostic.bIsMode0x08Read = true;
		}
	}


	// ####################################################################################
	// mode: 0x20 - Firmware Status
	// ####################################################################################
	FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_DIAG_MODE, (unsigned char)0x20));
	FEDM_CHK7(iBack, SendProtocol(0x6E));
	if(iBack==0x00 || iBack==0x84)
	{
		FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_DIAG_DATA, &m_ReaderDiagnostic.ucFirmwareStatus[0], 30));
		m_ReaderDiagnostic.bIsMode0x20Read = true;
	}

	// ####################################################################################
	// mode: 0x21 - Configuration change status after Firmware Update
	// ####################################################################################
	if(	this->m_uiReaderType == FEDM_ISC_TYPE_ISCLRU3000 || 
		this->m_uiReaderType == FEDM_ISC_TYPE_ISCLR2500_A)
	{
		FEDM_CHK7(iBack, SetData(FEDM_ISC_TMP_DIAG_MODE, (unsigned char)0x21));
		FEDM_CHK7(iBack, SendProtocol(0x6E));
		if(iBack==0x00 || iBack==0x84)
		{
			FEDM_CHK7(iBack, GetData(FEDM_ISC_TMP_DIAG_DATA, &m_ReaderDiagnostic.ucConfigParaChange, 1));
			m_ReaderDiagnostic.bIsMode0x21Read = true;
		}
	}

	if( !m_ReaderDiagnostic.bIsMode0x01Read &&
		!m_ReaderDiagnostic.bIsMode0x02Read &&
		!m_ReaderDiagnostic.bIsMode0x03Read &&
		!m_ReaderDiagnostic.bIsMode0x04Read &&
		!m_ReaderDiagnostic.bIsMode0x05Read &&
		!m_ReaderDiagnostic.bIsMode0x06Read &&
		!m_ReaderDiagnostic.bIsMode0x20Read &&
		!m_ReaderDiagnostic.bIsMode0x21Read )
	{
		SetLastError(FEDM_ERROR_NO_DATA);
	}
	else
	{
		SetLastError(FEDM_OK);
	}

	return &m_ReaderDiagnostic;
}

//////////////////////////////////////////////////////////////////////
// public callback functions
//////////////////////////////////////////////////////////////////////

/***************************************************************************
  Begin        		:	21.02.2006 / M. Hultsch

  Version       	:	04.02.08 / 13.08.2012 / M. Hultsch
						- support for events from People Counter

						04.02.06 / 23.07.2012 / M. Hultsch
						- for FEISC_TASKID_FIRST_NEW_TAG: set m_bActiveAsyncTask to false

						02.04.04 / 25.08.2006 / M. Hultsch

  Function			:	static callback function for invoke from DLL

  Parameters		:	void* pAny					- pointer to caller object
						int iReaderHnd				- reader handle (FEISC.DLL)
						int iTaskID					- task identifier from FEISC_StartAsyncTask(..)
						int iError					- error code
						unsigned char ucCmd			- reader command
						unsigned char* ucRspData	- pointer to response data
						int iRspLen					- length of response data

  Return value		:	-
***************************************************************************/
void FEDM_ISCReaderModule::cbsTaskRsp1(	void* pAny, 
										int iReaderHnd, 
										int iTaskID, 
										int iError, 
										unsigned char ucCmd,
										unsigned char* ucRspData, 
										int iRspLen )
{
	if((FEDM_ISCReaderModule*)pAny == NULL)
		return;

	switch(iTaskID)
	{
	case FEISC_TASKID_FIRST_NEW_TAG:
		((FEDM_ISCReaderModule*)pAny)->cbNewTag(iReaderHnd, iTaskID, iError, ucRspData, iRspLen);
		((FEDM_ISCReaderModule*)pAny)->m_bActiveAsyncTask = false;
		break;

	case FEISC_TASKID_EVERY_NEW_TAG:
		((FEDM_ISCReaderModule*)pAny)->cbNewTag(iReaderHnd, iTaskID, iError, ucRspData, iRspLen);
		break;

	case FEISC_TASKID_NOTIFICATION:
		switch(ucCmd)
		{
		case 0x22:
			((FEDM_ISCReaderModule*)pAny)->cbNewNotify(iReaderHnd, iTaskID, iError, ucRspData, iRspLen, "", 0);
			break;
		case 0x6E:
			((FEDM_ISCReaderModule*)pAny)->cbNewReaderDiagnostic(iReaderHnd, iTaskID, iError, ucRspData, iRspLen, "", 0);
			break;
		case 0x74:
			((FEDM_ISCReaderModule*)pAny)->cbNewInputEvent(iReaderHnd, iTaskID, iError, ucRspData, iRspLen, "", 0);
			break;
		case 0x77:
			((FEDM_ISCReaderModule*)pAny)->cbNewNotifyFromPeopleCounter(iReaderHnd, iTaskID, iError, ucRspData, iRspLen, "", 0);
			break;
		}
		break;
	}
}

/***************************************************************************
  Begin        		:	25.08.2006 / M. Hultsch

  Version       	:	04.02.08 / 13.08.2012 / M. Hultsch
						- support for events from People Counter

						02.04.04 / 25.08.2006 / M. Hultsch

  Function			:	static callback function for invoke from DLL

  Parameters		:	void* pAny					- pointer to caller object
						int iReaderHnd				- reader handle (FEISC.DLL)
						int iTaskID					- task identifier from FEISC_StartAsyncTask(..)
						int iError					- error code
						unsigned char ucCmd		- reader command
						unsigned char* ucRspData	- pointer to response data
						int iRspLen					- length of response data
						char* cIPAdr				- pointer to ip address of reader
						int iPortNr					- local or remote port number 

  Return value		:	-
***************************************************************************/
void FEDM_ISCReaderModule::cbsTaskRsp2(	void* pAny, 
										int iReaderHnd, 
										int iTaskID, 
										int iError, 
										unsigned char ucCmd,
										unsigned char* ucRspData, 
										int iRspLen,
										char* cIPAdr,
										int iPortNr )
{
	if((FEDM_ISCReaderModule*)pAny == NULL)
		return;

	switch(iTaskID)
	{
	case FEISC_TASKID_NOTIFICATION:
		switch(ucCmd)
		{
		case 0x22:
			((FEDM_ISCReaderModule*)pAny)->cbNewNotify(iReaderHnd, iTaskID, iError, ucRspData, iRspLen, cIPAdr, iPortNr);
			break;
		case 0x6E:
			((FEDM_ISCReaderModule*)pAny)->cbNewReaderDiagnostic(iReaderHnd, iTaskID, iError, ucRspData, iRspLen, cIPAdr, iPortNr);
			break;
		case 0x74:
			((FEDM_ISCReaderModule*)pAny)->cbNewInputEvent(iReaderHnd, iTaskID, iError, ucRspData, iRspLen, cIPAdr, iPortNr);
			break;
		case 0x77:
			((FEDM_ISCReaderModule*)pAny)->cbNewNotifyFromPeopleCounter(iReaderHnd, iTaskID, iError, ucRspData, iRspLen, cIPAdr, iPortNr);
			break;
		}
		break;
	}
}



//////////////////////////////////////////////////////////////////////
// protected callback functions
//////////////////////////////////////////////////////////////////////


/***************************************************************************
  Begin        		:	21.02.2006 / M. Hultsch

  Version       	:	03.09.12 / 24.08.2009 / M. Hultsch
						- ResetTable if TaskID = FEDM_TASKID_EVERY_NEW_TAG

						02.05.01 / 11.09.2006 / M. Hultsch

  Function			:	non-static callback function for invoke from DLL

  Parameters		:	int iReaderHnd				- reader handle (FEISC.DLL)
						int iTaskID					- task identifier from FEISC_StartAsyncTask(..)
						int iError					- error code
						unsigned char* ucRspData	- pointer to response data
						int iRspLen					- length of response data

  Return value		:	-
***************************************************************************/
void FEDM_ISCReaderModule::cbNewTag(	int iReaderHnd, 
										int iTaskID, 
										int iError, 
										unsigned char* ucRspData, 
										int iRspLen )
{
	unsigned char ucIsoMode = 0;
	int iIdx = 0;
	int iErr = 0;


	if(iReaderHnd != GetReaderHnd())
		return;

	SetLastError(iError);

	// first step: save inventory data in table
	if( iError==0x00 || 
		iError==0x01 || 
		iError==0x02 || 
		iError==0x03 || 
		iError==0x83 || 
		iError==0x84 || 
		iError==0x90 || 
		iError==0x93 || 
		iError==0x94 || 
		iError==0x95 )
	{	
		if(iRspLen > 0)
		{
			FEDM_CHK6(iErr, SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char)0x01));
	
			if(iError == 0x01)
			{
				// if command in selected mode and transponder doesn't respond
				// then remove selection flag
				FEDM_CHK6(iErr, GetData(FEDM_ISC_TMP_B0_MODE_ADR, &ucIsoMode));

				if(ucIsoMode == FEDM_ISC_ISO_MODE_SEL)
				{
					iIdx = FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
					if(iIdx >= 0)
						m_ISOTable[iIdx]->m_bIsSelected = false;
				}
			}
			else if(iError == 0x02) // exception for Inventory
			{
				if(iTaskID == FEDM_TASKID_EVERY_NEW_TAG)
					ResetTable(FEDM_ISC_ISO_TABLE);

				// save new transponder data in isc-table
				FEDM_CHK6(iErr, Set_0xB0_IsoTableData(ucRspData, iRspLen, iError));
			}
			else if(iError == 0x95)
			{
				FEDM_CHK6(iErr, SetData(FEDM_ISC_TMP_LAST_STATE, (unsigned char)iError));
				FEDM_CHK6(iErr, SetData(FEDM_ISC_TMP_B0_ISO_ERROR, ucRspData[0]));
			}
			else
			{
				if(iTaskID == FEDM_TASKID_EVERY_NEW_TAG)
					ResetTable(FEDM_ISC_ISO_TABLE);

				// save new transponder data in isc-table
				FEDM_CHK6(iErr, Set_0xB0_IsoTableData(ucRspData, iRspLen, iError));
			}
		}
	}


	// second step: invoke callback function of application
	switch(m_TaskInit.uiFlag)
	{
	case FEDM_TASKCB1:
		if(m_TaskInit.cbFct1 != NULL)
			m_TaskInit.cbFct1(m_TaskInit.pAny, iError, 0xB0);
		break;

#ifdef _FEDM_WINDOWS
	case FEDM_TASKCB_NET1:
		if(m_TaskInit.cbFctNET1 != NULL)
			m_TaskInit.cbFctNET1(iError, 0x0B);
		break;
#endif
	}
}


/***************************************************************************
  Begin        		:	24.03.2006 / M. Hultsch

  Version       	:	02.04.05 / 28.08.2006 / M. Hultsch

  Function			:	non-static callback function for invoke from DLL

  Parameters		:	int iReaderHnd				- reader handle (FEISC.DLL)
						int iTaskID					- task identifier from FEISC_StartAsyncTask(..)
						int iError					- error code
						unsigned char* ucRspData	- pointer to response data
						int iRspLen					- length of response data
						char* cIPAdr				- pointer to ip address of reader
						int iPortNr					- local or remote port number 

  Return value		:	-
***************************************************************************/
void FEDM_ISCReaderModule::cbNewNotify(	int iReaderHnd, 
										int iTaskID, 
										int iError, 
										unsigned char* ucRspData, 
										int iRspLen,
										char* cIPAdr,
										int iPortNr )
{
	int iErr = 0;
	unsigned int uiRecSets = 0;
	int iFirstByte = 0;
	unsigned int uiIP[4];
	unsigned char ucIP[4];

	memset(uiIP, 0, 4*sizeof(unsigned int));
	memset (ucIP, 0, 4*sizeof(unsigned char));

	if(iReaderHnd != GetReaderHnd())
		return;

	SetLastError(iError);

	// first step: save notification data in table
	if( iError==0x00 || 
		iError==0x83 || 
		iError==0x84 || 
		iError==0x85 || 
		iError==0x90 || 
		iError==0x93 || 
		iError==0x94 )
	{	

		if(iRspLen <= 0)
		{
			// reset BRM-table
			m_uiBRMTableLength = 0;
		}
		else
		{
			FEDM_CHK6(iErr, SetData(FEDM_ISC_TMP_ADV_BRM_TRDATA1,  ucRspData[0]));

			if(ucRspData[0] & 0x80)	// extension flag?
			{
				FEDM_CHK6(iErr, SetData(FEDM_ISC_TMP_ADV_BRM_TRDATA2,  ucRspData[1]));
				uiRecSets = (ucRspData[2] << 8) + ucRspData[3];
				iFirstByte = 4;
			}
			else
			{
				uiRecSets = (ucRspData[1] << 8) + ucRspData[2];
				iFirstByte = 3;
			}


			FEDM_CHK6(iErr, SetData(FEDM_ISC_TMP_ADV_BRM_RECSETS, uiRecSets));

			if(uiRecSets == 0)	// no more data
			{
				// reset BRM-table
				m_uiBRMTableLength = 0;

				return;
			}

			// save new buffered read mode data in brm-table
			FEDM_CHK6(iErr, SetBrmTableData(0x22, &ucRspData[iFirstByte]));
		}
	}
	else
	{
		// reset BRM-table
		m_uiBRMTableLength = 0;

		//return;
	}

	// second step: invoke callback function of application
	switch(m_TaskInit.uiFlag)
	{
	case FEDM_TASKCB1:
		if(m_TaskInit.cbFct1 != NULL)
			m_TaskInit.cbFct1(m_TaskInit.pAny, iError, 0x22);
		break;

	case FEDM_TASKCB2:
		if(m_TaskInit.cbFct2 != NULL)
		{
			sscanf(cIPAdr, "%u.%u.%u.%u", &uiIP[0], &uiIP[1], &uiIP[2], &uiIP[3]);
			ucIP[0] = (uiIP[0] & 0x000000FF);
			ucIP[1] = (uiIP[1] & 0x000000FF);
			ucIP[2] = (uiIP[2] & 0x000000FF);
			ucIP[3] = (uiIP[3] & 0x000000FF);
			SetData(FEDM_ISC_TMP_NTF_REMOTE_IP_ADR, ucIP, 4);
			SetData(FEDM_ISC_TMP_NTF_IP_PORT_NR, (unsigned int)iPortNr);
			m_TaskInit.cbFct2(m_TaskInit.pAny, iError, 0x22, cIPAdr, iPortNr);
		}
		break;

#ifdef _FEDM_WINDOWS
	case FEDM_TASKCB_NET1:
		if(m_TaskInit.cbFctNET1 != NULL)
		{
			sscanf(cIPAdr, "%u.%u.%u.%u", &uiIP[0], &uiIP[1], &uiIP[2], &uiIP[3]);
			ucIP[0] = (uiIP[0] & 0x000000FF);
			ucIP[1] = (uiIP[1] & 0x000000FF);
			ucIP[2] = (uiIP[2] & 0x000000FF);
			ucIP[3] = (uiIP[3] & 0x000000FF);
			SetData(FEDM_ISC_TMP_NTF_REMOTE_IP_ADR, ucIP, 4);
			SetData(FEDM_ISC_TMP_NTF_IP_PORT_NR, (unsigned int)iPortNr);
			m_TaskInit.cbFctNET1(iError, 0x22);
		}
		break;
#endif
	}
}


/***************************************************************************
  Begin        		:	28.03.2006 / M. Hultsch

  Version       	:	04.06.01 / 03.03.2014 / M. Hultsch
						- Extension for new response format: [rec_cnt][mode][30 byte data]

						02.04.05 / 28.08.2006 / M. Hultsch

  Function			:	non-static callback function for invoke from DLL

  Parameters		:	int iReaderHnd				- reader handle (FEISC.DLL)
						int iTaskID					- task identifier from FEISC_StartAsyncTask(..)
						int iError					- error code
						unsigned char* ucRspData	- pointer to response data
						int iRspLen					- length of response data
						char* cIPAdr				- pointer to ip address of reader
						int iPortNr					- local or remote port number 

  Return value		:	-
***************************************************************************/
void FEDM_ISCReaderModule::cbNewReaderDiagnostic(	int iReaderHnd, 
													int iTaskID, 
													int iError, 
													unsigned char* ucRspData, 
													int iRspLen,
													char* cIPAdr,
													int iPortNr )
{
	unsigned char ucRecCount = 0;
	unsigned char ucData[31];
	int iErr = 0;
	int iAddr = 0;
	int iToRead = 0;
	unsigned int uiReaderType = 0;
	unsigned int uiRecCount = 0;
	unsigned int uiIP[4];
	unsigned char ucIP[4];

	memset(uiIP, 0, 4*sizeof(unsigned int));
	memset (ucIP, 0, 4*sizeof(unsigned char));


	if(iReaderHnd != GetReaderHnd())
		return;

	SetLastError(iError);

	
	// first step: save reader diagnostic data in FEDM_TMPDATA_MEM
	if( iError==0x00 || 
		iError==0x84 )
	{	
		FEDM_CHK6(iErr, SetData(FEDM_ISC_TMP_0x6E_NTF_RSP_REC_COUNT, (unsigned char)0x00));
		FEDM_CHK6(iErr, SetData(FEDM_ISC_TMP_DIAG_MODE, (unsigned char)0x00));

		if(iRspLen > 0 && iRspLen < 32)
		{
			memset(ucData, 0, 31);

			if(iRspLen == 2)
			{
				// with mode 0x01, we receive 2 bytes
				memcpy(ucData, ucRspData, 2);

				// set reader diagnostic mode 0x01
				FEDM_CHK6(iErr, SetData(FEDM_ISC_TMP_DIAG_MODE, (unsigned char)0x01));
			}
			else if(iRspLen == 6)
			{
				// with mode 0x06, we receive 6 bytes
				memcpy(ucData, ucRspData, 6);

				// set reader diagnostic mode 0x06
				FEDM_CHK6(iErr, SetData(FEDM_ISC_TMP_DIAG_MODE, (unsigned char)0x06));
			}
			else
			{
				// all other modes

				memcpy(ucData, &ucRspData[1], iRspLen-1);

				// set reader diagnostic mode from receive protocol
				FEDM_CHK6(iErr, SetData(FEDM_ISC_TMP_DIAG_MODE, ucRspData[0]));
			}

			// FEDM_ISC_TMP_DIAG_DATA addresses always 31 bytes
			FEDM_CHK6(iErr, SetData(FEDM_ISC_TMP_DIAG_DATA, ucData, 31));
		}
		else if(iRspLen >= 36)
		{
			// new response format
			FEDM_CHK6(iErr, SetData(FEDM_ISC_TMP_0x6E_NTF_RSP_READER_TYPE, &ucRspData[0], 4));
			FEDM_CHK6(iErr, SetData(FEDM_ISC_TMP_DIAG_MODE, (unsigned char)0xFF));

			ucRecCount = ucRspData[4];

			iAddr = FEDM_GetAdrOfID(FEDM_ISC_TMP_0x6E_NTF_RSP_DATA, 32);
			iToRead = iRspLen;

			for(unsigned char ucCnt=0; ucCnt<ucRecCount; ucCnt++)
			{
				if(iToRead < 31)
					break;

				memset(ucData, 0, 31);
				memcpy(ucData, &ucRspData[5+ucCnt*31], 31);

				if(iAddr >= FEDM_ISC_MAX_TMPDATA_MEM-31)
					break;

				SetData( iAddr, ucData, 31, FEDM_TMPDATA_MEM );

				iAddr += 31;
				iToRead -= 31;
				uiRecCount++;
			}
			FEDM_CHK6(iErr, SetData(FEDM_ISC_TMP_0x6E_NTF_RSP_REC_COUNT, uiRecCount));
		}
	}

	// second step: invoke callback function of application
	switch(m_TaskInit.uiFlag)
	{
	case FEDM_TASKCB1:
		if(m_TaskInit.cbFct1 != NULL)
			m_TaskInit.cbFct1(m_TaskInit.pAny, iError, 0x6E);
		break;

	case FEDM_TASKCB2:
		if(m_TaskInit.cbFct2 != NULL)
		{
			sscanf(cIPAdr, "%u.%u.%u.%u", &uiIP[0], &uiIP[1], &uiIP[2], &uiIP[3]);
			ucIP[0] = (uiIP[0] & 0x000000FF);
			ucIP[1] = (uiIP[1] & 0x000000FF);
			ucIP[2] = (uiIP[2] & 0x000000FF);
			ucIP[3] = (uiIP[3] & 0x000000FF);
			SetData(FEDM_ISC_TMP_NTF_REMOTE_IP_ADR, ucIP, 4);
			SetData(FEDM_ISC_TMP_NTF_IP_PORT_NR, (unsigned int)iPortNr);
			m_TaskInit.cbFct2(m_TaskInit.pAny, iError, 0x6E, cIPAdr, iPortNr);
		}
		break;

#ifdef _FEDM_WINDOWS
	case FEDM_TASKCB_NET1:
		if(m_TaskInit.cbFctNET1 != NULL)
		{
			sscanf(cIPAdr, "%u.%u.%u.%u", &uiIP[0], &uiIP[1], &uiIP[2], &uiIP[3]);
			ucIP[0] = (uiIP[0] & 0x000000FF);
			ucIP[1] = (uiIP[1] & 0x000000FF);
			ucIP[2] = (uiIP[2] & 0x000000FF);
			ucIP[3] = (uiIP[3] & 0x000000FF);
			SetData(FEDM_ISC_TMP_NTF_REMOTE_IP_ADR, ucIP, 4);
			SetData(FEDM_ISC_TMP_NTF_IP_PORT_NR, (unsigned int)iPortNr);
			m_TaskInit.cbFctNET1(iError, 0x6E);
		}
		break;
#endif
	}
}


/***************************************************************************
  Begin        		:	04.07.2011 / M. Hultsch

  Version       	:	03.03.04 / 04.07.2011 / M. Hultsch

  Function			:	non-static callback function for invoke from DLL

  Parameters		:	int iReaderHnd				- reader handle (FEISC.DLL)
						int iTaskID					- task identifier from FEISC_StartAsyncTask(..)
						int iError					- error code
						unsigned char* ucRspData	- pointer to response data
						int iRspLen					- length of response data
						char* cIPAdr				- pointer to ip address of reader
						int iPortNr					- local or remote port number 

  Return value		:	-
***************************************************************************/
void FEDM_ISCReaderModule::cbNewInputEvent(	int iReaderHnd, 
											int iTaskID, 
											int iError, 
											unsigned char* ucRspData, 
											int iRspLen,
											char* cIPAdr,
											int iPortNr )
{
	int iErr = 0;
	unsigned int uiIP[4];
	unsigned char ucIP[4];

	memset(uiIP, 0, 4*sizeof(unsigned int));
	memset (ucIP, 0, 4*sizeof(unsigned char));

	if(iReaderHnd != GetReaderHnd())
		return;

	SetLastError(iError);

	// first step: save notification data in table
	if( iError==0x00 )
	{	

		if(iRspLen <= 0)
		{
			// reset BRM-table
			m_uiBRMTableLength = 0;
		}
		else
		{
			FEDM_CHK6(iErr, SetData(FEDM_ISC_TMP_ADV_BRM_TRDATA1,  (unsigned char)0x80));
			FEDM_CHK6(iErr, SetData(FEDM_ISC_TMP_ADV_BRM_TRDATA2,  (unsigned char)0x01));
			FEDM_CHK6(iErr, SetData(FEDM_ISC_TMP_ADV_BRM_RECSETS, (unsigned int)1));

			// save new buffered read mode data in brm-table
			FEDM_CHK6(iErr, SetBrmTableData(0x74, ucRspData));
		}
	}
	else
	{
		// reset BRM-table
		m_uiBRMTableLength = 0;

		//return;
	}

	// second step: invoke callback function of application
	switch(m_TaskInit.uiFlag)
	{
	case FEDM_TASKCB1:
		if(m_TaskInit.cbFct1 != NULL)
			m_TaskInit.cbFct1(m_TaskInit.pAny, iError, 0x22);
		break;

	case FEDM_TASKCB2:
		if(m_TaskInit.cbFct2 != NULL)
		{
			sscanf(cIPAdr, "%u.%u.%u.%u", &uiIP[0], &uiIP[1], &uiIP[2], &uiIP[3]);
			ucIP[0] = (uiIP[0] & 0x000000FF);
			ucIP[1] = (uiIP[1] & 0x000000FF);
			ucIP[2] = (uiIP[2] & 0x000000FF);
			ucIP[3] = (uiIP[3] & 0x000000FF);
			SetData(FEDM_ISC_TMP_NTF_REMOTE_IP_ADR, ucIP, 4);
			SetData(FEDM_ISC_TMP_NTF_IP_PORT_NR, (unsigned int)iPortNr);
			m_TaskInit.cbFct2(m_TaskInit.pAny, iError, 0x22, cIPAdr, iPortNr);
		}
		break;

#ifdef _FEDM_WINDOWS
	case FEDM_TASKCB_NET1:
		if(m_TaskInit.cbFctNET1 != NULL)
		{
			sscanf(cIPAdr, "%u.%u.%u.%u", &uiIP[0], &uiIP[1], &uiIP[2], &uiIP[3]);
			ucIP[0] = (uiIP[0] & 0x000000FF);
			ucIP[1] = (uiIP[1] & 0x000000FF);
			ucIP[2] = (uiIP[2] & 0x000000FF);
			ucIP[3] = (uiIP[3] & 0x000000FF);
			SetData(FEDM_ISC_TMP_NTF_REMOTE_IP_ADR, ucIP, 4);
			SetData(FEDM_ISC_TMP_NTF_IP_PORT_NR, (unsigned int)iPortNr);
			m_TaskInit.cbFctNET1(iError, 0x22);
		}
		break;
#endif
	}
}

/***************************************************************************
  Begin        		:	13.08.2012 / M. Hultsch

  Version       	:	04.02.08 / 13.08.2012 / M. Hultsch

  Function			:	non-static callback function for invoke from DLL

  Parameters		:	int iReaderHnd				- reader handle (FEISC.DLL)
						int iTaskID					- task identifier from FEISC_StartAsyncTask(..)
						int iError					- error code
						unsigned char* ucRspData	- pointer to response data
						int iRspLen					- length of response data
						char* cIPAdr				- pointer to ip address of reader
						int iPortNr					- local or remote port number 

  Return value		:	-
***************************************************************************/
void FEDM_ISCReaderModule::cbNewNotifyFromPeopleCounter(	int iReaderHnd, 
															int iTaskID, 
															int iError, 
															unsigned char* ucRspData, 
															int iRspLen,
															char* cIPAdr,
															int iPortNr )
{
	//unsigned char ucData[13];
	//int iErr = 0;
	unsigned int uiIP[4];
	unsigned char ucIP[4];

	memset(uiIP, 0, 4*sizeof(unsigned int));
	memset (ucIP, 0, 4*sizeof(unsigned char));


	if(iReaderHnd != GetReaderHnd())
		return;

	SetLastError(iError);

	if(iRspLen < 17)
		return;

	
	// first step: build counter values
	if(iError == 0)
	{
		unsigned int uiCnt1 = ucRspData[3] | ucRspData [2] << 8 | ucRspData[1] << 16 | ucRspData[0] << 24;
		unsigned int uiCnt2 = ucRspData[7] | ucRspData [6] << 8 | ucRspData[5] << 16 | ucRspData[4] << 24;
		unsigned int uiCnt3 = ucRspData[11] | ucRspData [10] << 8 | ucRspData[9] << 16 | ucRspData[8] << 24;
		unsigned int uiCnt4 = ucRspData[15] | ucRspData [14] << 8 | ucRspData[13] << 16 | ucRspData[12] << 24;

		SetData(FEDM_ISC_TMP_77_COUNTER1, uiCnt1);
		SetData(FEDM_ISC_TMP_77_COUNTER2, uiCnt2);
		SetData(FEDM_ISC_TMP_77_COUNTER3, uiCnt3);
		SetData(FEDM_ISC_TMP_77_COUNTER4, uiCnt4);
		SetData(FEDM_ISC_TMP_77_BUS_ADDR, ucRspData[16]);
	}

	// second step: invoke callback function of application
	switch(m_TaskInit.uiFlag)
	{
	case FEDM_TASKCB1:
		if(m_TaskInit.cbFct1 != NULL)
			m_TaskInit.cbFct1(m_TaskInit.pAny, iError, 0x77);
		break;

	case FEDM_TASKCB2:
		if(m_TaskInit.cbFct2 != NULL)
		{
			sscanf(cIPAdr, "%u.%u.%u.%u", &uiIP[0], &uiIP[1], &uiIP[2], &uiIP[3]);
			ucIP[0] = (uiIP[0] & 0x000000FF);
			ucIP[1] = (uiIP[1] & 0x000000FF);
			ucIP[2] = (uiIP[2] & 0x000000FF);
			ucIP[3] = (uiIP[3] & 0x000000FF);
			SetData(FEDM_ISC_TMP_NTF_REMOTE_IP_ADR, ucIP, 4);
			SetData(FEDM_ISC_TMP_NTF_IP_PORT_NR, (unsigned int)iPortNr);
			m_TaskInit.cbFct2(m_TaskInit.pAny, iError, 0x77, cIPAdr, iPortNr);
		}
		break;

#ifdef _FEDM_WINDOWS
	case FEDM_TASKCB_NET1:
		if(m_TaskInit.cbFctNET1 != NULL)
		{
			sscanf(cIPAdr, "%u.%u.%u.%u", &uiIP[0], &uiIP[1], &uiIP[2], &uiIP[3]);
			ucIP[0] = (uiIP[0] & 0x000000FF);
			ucIP[1] = (uiIP[1] & 0x000000FF);
			ucIP[2] = (uiIP[2] & 0x000000FF);
			ucIP[3] = (uiIP[3] & 0x000000FF);
			SetData(FEDM_ISC_TMP_NTF_REMOTE_IP_ADR, ucIP, 4);
			SetData(FEDM_ISC_TMP_NTF_IP_PORT_NR, (unsigned int)iPortNr);
			m_TaskInit.cbFctNET1(iError, 0x77);
		}
		break;
#endif
	}
}
