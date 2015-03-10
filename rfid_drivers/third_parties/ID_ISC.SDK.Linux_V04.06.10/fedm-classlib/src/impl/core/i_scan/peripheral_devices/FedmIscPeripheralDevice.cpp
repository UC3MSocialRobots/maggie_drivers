/*-------------------------------------------------------
|                                                       |
|                 FedmIscPeripheralDevice.cpp           |
|                                                       |
---------------------------------------------------------

Copyright © 2010		FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Manuel Sahm
Begin        		:	18.02.2010

Version       		:	03.02.00 / 15.06.2010 / M. Sahm, M. Hultsch

Operation Systems	:	independent

Function			:	classes for OBID i-scan® peripheral devices


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_NO_PD_SUPPORT)

#include "FedmIscPeripheralDevice.h"
#include "../FEDM_ISCReader.h"
#include "feisc.h"


//####################################################################
// class FEDM_ISCPeripheralDevice
//####################################################################

//////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////

FedmIscPeripheralDevice::FedmIscPeripheralDevice(FEDM_ISCReader* pReader, 
												 unsigned char ucBusAdr, 
												 unsigned char ucPort, 
												 int iPDType)
{
	m_DeviceInfo.Init();

	// Bus Address of Peripheral Device
	m_ucBusAdr		= ucBusAdr;
	m_ucPort		= ucPort;

	// reader object for communication
	m_pReader		= pReader;

	m_iLastError	= 0;
	m_ucLastStatus	= 0;

	m_iPDType		= iPDType;

	
	// Reader Handle
	m_iReaderHnd	= 0;


	// Buffer for Data Transfer
	memset(&m_ucSendData[0], 0x00, sizeof(m_ucSendData));
	m_iSendDataLen	= 0;
	memset(&m_ucSendProt[0], 0x00, sizeof(m_ucSendProt));
	m_iSendProtLen	= 0 ;
	memset(&m_ucRspProt[0], 0x00, sizeof(m_ucRspProt));
	m_iRspProtLen	= 0;
	memset(&m_ucRspData[0], 0x00, sizeof(m_ucRspData));
	m_iRspDataLen	= 0;

	m_iReaderHnd = FEISC_NewReader(0);
	FEISC_SetReaderPara(m_iReaderHnd, "FrameSupport","Advanced");
}

//////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////

FedmIscPeripheralDevice::~FedmIscPeripheralDevice()
{
	FEISC_DeleteReader(m_iReaderHnd);
}


/***************************************************************************
  Begin        		:	23.09.2010 / M. Hultsch
  
  Version       	:	03.02.05 / 23.09.2010 / M. Hultsch

  Function			:	controller reset

  Parameters		:	-

  Return value		:	0:		FEDM_OK
						<0:		Error Code
***************************************************************************/
int FedmIscPeripheralDevice::Reset()
{
	int iBack = 0;
	unsigned char ucRecBusAddr = 0x00;
	unsigned char ucCmd = 0x00;


	// Build Protocol : CPU-Reset (0x63)
	m_iSendProtLen = FEISC_BuildSendProtocol(m_iReaderHnd, m_ucBusAdr, FEISC_CPU_RESET, m_ucSendData , 0, m_ucSendProt, 0);
	if(m_iSendProtLen <= 0)
	{
		FEDM_RETURN(m_iSendProtLen);
	}

	// Send Protocol within Piggyback frame
	iBack = FEISC_0x9F_Piggyback_Command(m_pReader->GetReaderHnd(),m_pReader->GetBusAddress(), 0x00, FEDM_ISC_TYPE_ANT_GPC, FEDM_ISC_PD_PORT_TYPE_RS4XX, m_ucSendProt, m_iSendProtLen, m_ucRspProt, &m_iRspProtLen);
	if(iBack < 0)
	{
		FEDM_RETURN(iBack);
	}
	else if(iBack == 0x20) // Reader: External Device communication error
	{
		FEDM_RETURN(iBack);
	}

	// Get Response out of Answer from Protocol [0x63]
	iBack = FEISC_SplitRecProtocol(m_iReaderHnd, m_ucRspProt, m_iRspProtLen, &ucRecBusAddr, &ucCmd, m_ucRspData, &m_iRspDataLen, 0);
	if(iBack != 0)
	{
		FEDM_RETURN(iBack);
	}

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        		:	18.02.2010 / M. Sahm
  
  Version       	:	03.01.01 / 18.02.2010 / M. Sahm

  Function			:	Read information out of the Device
						[GetReaderInfo - Mode 0x00]
						[GetReaderInfo - Mode 0x10]
						[GetReaderInfo - Mode 0x40]
						[GetReaderInfo - Mode 0x41]
						[GetReaderInfo - Mode 0x60]

  Parameters		:	-

  Return value		:	Pointer to FEDM_ISC_PEOPLECOUNTER_INFO structure in case of success
						NULL-Pointer in case of error
***************************************************************************/
FEDM_ISC_PERIPHERAL_DEVICE_INFO* FedmIscPeripheralDevice::ReadInfo()
{
	int iBack = 0;
	unsigned char ucRecBusAddr = 0x00;
	unsigned char ucCmd = 0x00;
	unsigned char ucDevice = 0x00;

	m_DeviceInfo.Init();

	switch(m_iPDType)
	{
		case FEDM_ISC_PD_TYPE_PEOPLE_COUNTER:
		case FEDM_ISC_PD_TYPE_EXTERNAL_IO:
			ucDevice = FEDM_ISC_TYPE_ANT_GPC;
		break;
			SetLastError(FEDM_ERROR_UNKNOWN_READER_TYPE);
			return NULL;
		default:

		break;
	}

	// ####################################################################################
	// new standard protocol - must have all newer readers implemented
	// mode: 0x00 - RFC firmware version
	// ####################################################################################
	//Build the "internal" protocol
	m_ucSendData[0] = 0x00;				//Mode-Byte
	// Build Protocol : GetReaderInfo (0x66) and send Protocol within Piggyback frame
	FEDM_CHK7(m_iSendProtLen,FEISC_BuildSendProtocol(m_pReader->GetReaderHnd(), m_ucBusAdr, FEISC_READER_INFO, m_ucSendData , 1, m_ucSendProt, 0));
	FEDM_CHK7(iBack,FEISC_0x9F_Piggyback_Command(m_pReader->GetReaderHnd(),m_pReader->GetBusAddress(), 0x00, ucDevice, m_ucPort, m_ucSendProt, m_iSendProtLen, m_ucRspProt, &m_iRspProtLen));
	if(iBack==0x00 || iBack==0x84 || iBack==0xF1)
	{
		// Get Response out of Piggyback frame and then Get Response out of Answer from Protocol 0x66 (GetReaderInfo)
		FEDM_CHK7(iBack, FEISC_SplitRecProtocol(m_pReader->GetReaderHnd(), m_ucRspProt, m_iRspProtLen, &ucRecBusAddr, &ucCmd, m_ucRspData, &m_iRspDataLen, 0));
		if(m_iRspDataLen > 0)
		{
			memcpy(m_DeviceInfo.ucSwVer, m_ucRspData, 13);
			m_DeviceInfo.bIsMode0x00Read = true;
		}
	}
	else if(iBack==0x19) // crypto processing error
	{
		this->SetLastError(iBack);
		return NULL;
	}
	else if(iBack == 0x20) // Reader: External Device communication error
	{
		this->SetLastError(iBack);
		return NULL;
	}

	// ####################################################################################
	// mode: 0x10 - additional hardware information
	// ####################################################################################
	//Build the "internal" protocol
	m_ucSendData[0] = 0x10;				//Mode-Byte
	// Build Protocol : GetReaderInfo (0x66) and send Protocol within Piggyback frame
	FEDM_CHK7(m_iSendProtLen,FEISC_BuildSendProtocol(m_pReader->GetReaderHnd(), m_ucBusAdr, FEISC_READER_INFO, m_ucSendData , 1, m_ucSendProt, 0));
	FEDM_CHK7(iBack,FEISC_0x9F_Piggyback_Command(m_pReader->GetReaderHnd(),m_pReader->GetBusAddress(), 0x00, ucDevice, m_ucPort, m_ucSendProt, m_iSendProtLen, m_ucRspProt, &m_iRspProtLen));
	if(iBack==0x00 || iBack==0x84 || iBack==0xF1)
	{
		// Get Response out of Piggyback frame and then Get Response out of Answer from Protocol 0x66 (GetReaderInfo)
		FEDM_CHK7(iBack, FEISC_SplitRecProtocol(m_pReader->GetReaderHnd(), m_ucRspProt, m_iRspProtLen, &ucRecBusAddr, &ucCmd, m_ucRspData, &m_iRspDataLen, 0));
		if(m_iRspDataLen > 0)
		{
			memcpy(m_DeviceInfo.ucHwInfo, m_ucRspData, 11);
			m_DeviceInfo.bIsMode0x10Read = true;
		}
	}


	// ####################################################################################
	// mode: 0x40 - CFG information with read permission (RFU)
	// ####################################################################################
	//Build the "internal" protocol
/*	m_ucSendData[0] = 0x40;				//Mode-Byte
	// Build Protocol : GetReaderInfo (0x66) and send Protocol within Piggyback frame
	FEDM_CHK7(m_iSendProtLen,FEISC_BuildSendProtocol(m_pReader->GetReaderHnd(), m_ucBusAdr, FEISC_READER_INFO, m_ucSendData , 1, m_ucSendProt, 0));
	FEDM_CHK7(iBack,FEISC_0x9F_Piggyback_Command(m_pReader->GetReaderHnd(),m_pReader->GetBusAddress(), 0x00, ucDevice, m_ucPort, m_ucSendProt, m_iSendProtLen, m_ucRspProt, &m_iRspProtLen));
	if(iBack==0x00 || iBack==0xF1)
	{
		// Get Response out of Piggyback frame and then Get Response out of Answer from Protocol 0x66 (GetReaderInfo)
		FEDM_CHK7(iBack, FEISC_SplitRecProtocol(m_pReader->GetReaderHnd(), m_ucRspProt, m_iRspProtLen, &ucRecBusAddr, &ucCmd, m_ucRspData, &m_iRspDataLen, 0));
		if(m_iRspDataLen > 0)
		{
			memcpy(m_DeviceInfo.ucNoOfCfgPages, m_ucRspData, 2);
			unsigned int uiPageCount = ((m_DeviceInfo.ucNoOfCfgPages[0]<<8) + m_DeviceInfo.ucNoOfCfgPages[1]) / 8;
			if( ((m_DeviceInfo.ucNoOfCfgPages[0]<<8) + m_DeviceInfo.ucNoOfCfgPages[1]) % 8 )
				uiPageCount++;
			memcpy(m_DeviceInfo.ucCfgReadPermissions, &m_ucRspData[2], uiPageCount);
			m_DeviceInfo.bIsMode0x40Read = true;
			}
	}*/


	// ####################################################################################
	// mode: 0x41 - CFG information with write permission (RFU)
	// ####################################################################################
	//Build the "internal" protocol
/*	m_ucSendData[0] = 0x41;				//Mode-Byte
	// Build Protocol : GetReaderInfo (0x66) and send Protocol within Piggyback frame
	FEDM_CHK7(m_iSendProtLen,FEISC_BuildSendProtocol(m_pReader->GetReaderHnd(), m_ucBusAdr, FEISC_READER_INFO, m_ucSendData , 1, m_ucSendProt, 0));
	FEDM_CHK7(iBack,FEISC_0x9F_Piggyback_Command(m_pReader->GetReaderHnd(),m_pReader->GetBusAddress(), 0x00, ucDevice, m_ucPort, m_ucSendProt, m_iSendProtLen, m_ucRspProt, &m_iRspProtLen));
	if(iBack==0x00 || iBack==0xF1)
	{
		// Get Response out of Piggyback frame and then Get Response out of Answer from Protocol 0x66 (GetReaderInfo)
		FEDM_CHK7(iBack, FEISC_SplitRecProtocol(m_pReader->GetReaderHnd(), m_ucRspProt, m_iRspProtLen, &ucRecBusAddr, &ucCmd, m_ucRspData, &m_iRspDataLen, 0));
		if(m_iRspDataLen > 0)
		{
			memcpy(m_DeviceInfo.ucNoOfCfgPages, m_ucRspData, 2);
			unsigned int uiPageCount = ((m_DeviceInfo.ucNoOfCfgPages[0]<<8) + m_DeviceInfo.ucNoOfCfgPages[1]) / 8;
			if( ((m_DeviceInfo.ucNoOfCfgPages[0]<<8) + m_DeviceInfo.ucNoOfCfgPages[1]) % 8 )
				uiPageCount++;
			memcpy(m_DeviceInfo.ucCfgWritePermissions, &m_ucRspData[2], uiPageCount);
			m_DeviceInfo.bIsMode0x41Read = true;
		}
	}*/


	// ####################################################################################
	// mode: 0x60 - input/output capabilities information
	// ####################################################################################
	//Build the "internal" protocol
	m_ucSendData[0] = 0x60;				//Mode-Byte
	// Build Protocol : GetReaderInfo (0x66) and send Protocol within Piggyback frame
	FEDM_CHK7(m_iSendProtLen,FEISC_BuildSendProtocol(m_pReader->GetReaderHnd(), m_ucBusAdr, FEISC_READER_INFO, m_ucSendData , 1, m_ucSendProt, 0));
	FEDM_CHK7(iBack,FEISC_0x9F_Piggyback_Command(m_pReader->GetReaderHnd(),m_pReader->GetBusAddress(), 0x00, ucDevice, m_ucPort, m_ucSendProt, m_iSendProtLen, m_ucRspProt, &m_iRspProtLen));
	if(iBack==0x00 || iBack==0xF1)
	{
		// Get Response out of Piggyback frame and then Get Response out of Answer from Protocol 0x66 (GetReaderInfo)
		FEDM_CHK7(iBack, FEISC_SplitRecProtocol(m_pReader->GetReaderHnd(), m_ucRspProt, m_iRspProtLen, &ucRecBusAddr, &ucCmd, m_ucRspData, &m_iRspDataLen, 0));
		if(m_iRspDataLen > 0)
		{
			memcpy(&m_DeviceInfo.ucNoOfInputs, m_ucRspData, 3);
			m_DeviceInfo.bIsMode0x60Read = true;
		}
	}

	SetLastError(FEDM_OK);
	return &m_DeviceInfo;
}

/***************************************************************************
  Begin        		:	18.02.2010 / M. Sahm
  
  Version       	:	03.01.01 / 18.02.2010 / M. Sahm

  Function			:	Return the type of the Peripheral Device

  Parameters		:	-

  Return value		:	<int> m_iPDType
***************************************************************************/
int FedmIscPeripheralDevice::GetPDType()
{
	return m_iPDType;
}

unsigned char FedmIscPeripheralDevice::GetBusAddress()
{
	return m_ucBusAdr;
}

/***************************************************************************
  Begin        		:	18.02.2010 / M. Sahm
  
  Version       	:	03.01.01 / 18.02.2010 / M. Sahm

  Function			:	Get the Reader Object

  Parameters		:	-

  Return value		:	<FEDM_ISCReader> m_pReader
***************************************************************************/
FEDM_ISCReader* FedmIscPeripheralDevice::GetReaderObject()
{
	return m_pReader;
}


//////////////////////////////////////////////////////////////////////
// miscellaneous functions
//////////////////////////////////////////////////////////////////////

/***************************************************************************
  Begin        		:	18.02.2010 / M. Sahm
  
  Version       	:	03.01.01 / 18.02.2010 / M. Sahm

  Function			:	Get the last error

  Parameters		:	-

  Return value		:	<int> m_iLastError
***************************************************************************/
int FedmIscPeripheralDevice::GetLastError()
{
	return m_iLastError;
}


/***************************************************************************
  Begin        		:	18.02.2010 / M. Sahm
  
  Version       	:	03.01.01 / 18.02.2010 / M. Sahm

  Function			:	Get the last status

  Parameters		:	-

  Return value		:	<unsigned char> m_ucLastStatus
***************************************************************************/
unsigned char FedmIscPeripheralDevice::GetLastStatus()
{
	return m_ucLastStatus;
}


/***************************************************************************
  Begin        		:	18.02.2010 / M. Sahm
  
  Version       	:	03.01.01 / 18.02.2010 / M. Sahm

  Function			:	Get Error Text from Error Code

  Parameters		:	<int> iErrorCode			:	Error Code

  Return value		:	<const char*> "Error Text"
***************************************************************************/
const char* FedmIscPeripheralDevice::GetErrorText(int iErrorCode)
{
	if(iErrorCode == 0)
	{
		return "OK";
	}
	else 
	{
		// errors from reader controlled libraries
		if(m_pReader != NULL)
		{
			return m_pReader->GetErrorText(iErrorCode);
		}
	}

	return "";
}


/***************************************************************************
  Begin        		:	18.02.2010 / M. Sahm
  
  Version       	:	03.01.01 / 18.02.2010 / M. Sahm

  Function			:	Get Status Text from Status Code

  Parameters		:	<unsigned char> ucStatus	:	Status Value

  Return value		:	<const char*> "Status Text"
***************************************************************************/
const char* FedmIscPeripheralDevice::GetStatusText(unsigned char ucStatus)
{
	int iBack = 0 ;
	static char cStatusText[256];

	// status text from feisc library
	iBack = FEISC_GetStatusText(ucStatus, cStatusText);
	if(iBack == 0)
	{
		return cStatusText;
	}
	else
	{
		return "";
	}
}


//////////////////////////////////////////////////////////////////////
// protected functions
//////////////////////////////////////////////////////////////////////
/***************************************************************************
  Begin        		:	18.02.2010 / M. Sahm
  
  Version       	:	03.01.01 / 18.02.2010 / M. Sahm

  Function			:	Set the last error

  Parameters		:	<int> iError			:	Error Value

  Return value		:	-
***************************************************************************/
void FedmIscPeripheralDevice::SetLastError(int iError)
{
	if(iError < 0)
		m_iLastError = iError;
	else
		m_ucLastStatus = (unsigned char)iError;
}

/***************************************************************************
  Begin        		:	29.03.2010 / M. Hultsch
  
  Version       	:	03.01.04 / 29.03.2010 / M. Hultsch

  Function			:	build the report string after ReadInfo

  Parameters		:	<string&> sReport	:	reference to a string variable

  Return value		:	-
***************************************************************************/
void FedmIscPeripheralDevice::BuildInfoReport(string& sReport)
{
	if(m_DeviceInfo.bIsMode0x00Read)
		Add0x00InfoText(sReport);

	if(m_DeviceInfo.bIsMode0x10Read)
		Add0x10InfoText(sReport);

	if(m_DeviceInfo.bIsMode0x60Read)
		Add0x60InfoText(sReport);
}


// version info like [0x65] Software Version
void FedmIscPeripheralDevice::Add0x00InfoText(string& sReport)
{
	char cBuffer[64];
	unsigned int uiRxBufLen = 0;
	unsigned int uiTxBufLen = 0;

	uiRxBufLen		= (m_DeviceInfo.ucRxBufferSize[0]<<8) + m_DeviceInfo.ucRxBufferSize[1];
	uiTxBufLen		= (m_DeviceInfo.ucTxBufferSize[0]<<8) + m_DeviceInfo.ucTxBufferSize[1];


	sReport += "Firmware Version of Peripheral Device:";
	sReport += "\r\n";
	sprintf(cBuffer, "%02d.%02d.%02d", m_DeviceInfo.ucSwVer[0], m_DeviceInfo.ucSwVer[1], m_DeviceInfo.ucDevVer);

	sReport += "\tSoftware Revision:.....";
	sReport += cBuffer;
	sReport += "\r\n";
	sprintf(cBuffer, "0x%02X", m_DeviceInfo.ucHwType);
	sReport += "\tHardware Type:.........";
	sReport += cBuffer;
	sReport += "\r\n";

	sprintf(cBuffer, "%02d", m_DeviceInfo.ucDeviceType);
	sReport += "\tDevice Type:...........";
	sReport += cBuffer;
	sReport += " (";
	switch(m_DeviceInfo.ucDeviceType)
	{
	case FEDM_ISC_TYPE_ANT_GPC:
		sReport += "People Counter";
		break;
//	case :
//		sReport += "External I/O";
//		break;
	}
	sReport += ")";
	sReport += "\r\n";

	sprintf(cBuffer, "%05d", uiRxBufLen);
	sReport += "\tMax. Rx buffer size:...";
	sReport += cBuffer;
	sReport += "\r\n";
	sprintf(cBuffer, "%05d", uiTxBufLen);
	sReport += "\tMax. Tx buffer size:...";
	sReport += cBuffer;
	sReport += "\r\n";
}

void FedmIscPeripheralDevice::Add0x10InfoText(string& sReport)
{
	char cBuffer[64];

	sReport += "\r\n";
	sReport += "Hardware Information:";
	sReport += "\r\n";
	sprintf(cBuffer, "\t0x%02X%02X - 0x%02X%02X - 0x%02X%02X",
				m_DeviceInfo.ucHwInfo[0],
				m_DeviceInfo.ucHwInfo[1],
				m_DeviceInfo.ucDHw[0],
				m_DeviceInfo.ucDHw[1],
				m_DeviceInfo.ucAHw[0],
				m_DeviceInfo.ucAHw[1]);

	sReport += cBuffer;

	sReport += "\r\n";
	sReport += "\tSupported Port Types:..";
	if(m_DeviceInfo.ucPortTypes & 0x01)
	{
		sReport += "Serial";
		if(m_DeviceInfo.ucPortTypes & 0xFE)
			sReport += ", ";
	}
	if(m_DeviceInfo.ucPortTypes & 0x02)
	{
		sReport += "RS4xx";
		if(m_DeviceInfo.ucPortTypes & 0xFC)
		sReport += ", ";
	}
	if(m_DeviceInfo.ucPortTypes & 0x04)
	{
		sReport += "LAN";
		if(m_DeviceInfo.ucPortTypes & 0xF8)
			sReport += ", ";
	}
	if(m_DeviceInfo.ucPortTypes & 0x08)
	{
		sReport += "WLAN";
		if(m_DeviceInfo.ucPortTypes & 0xF0)
			sReport += ", ";
	}
	if(m_DeviceInfo.ucPortTypes & 0x10)
	{
		sReport += "USB";
		if(m_DeviceInfo.ucPortTypes & 0xE0)
			sReport += ", ";
	}
	if(m_DeviceInfo.ucPortTypes & 0x20)
	{
		sReport += "Bluetooth";
		if(m_DeviceInfo.ucPortTypes & 0xC0)
			sReport += ", ";
	}
	if(m_DeviceInfo.ucPortTypes & 0x80)
	{
		sReport += "Discovery";
	}

	sReport += "\r\n";
}

void FedmIscPeripheralDevice::Add0x60InfoText(string& sReport)
{
	char cBuffer[64];

	sReport += "\r\n";
	sReport += "Input/Output capabilities";
	sReport += "\r\n";

	sReport += "\tNumber of Inputs:......";
	sprintf(cBuffer, "%d", m_DeviceInfo.ucNoOfInputs);
	sReport += cBuffer;
	sReport += "\r\n";

	sReport += "\tNumber of Outputs:.....";
	sprintf(cBuffer, "%d", m_DeviceInfo.ucNoOfOutputs);
	sReport += cBuffer;
	sReport += "\r\n";

	sReport += "\tNumber of Relays:......";
	sprintf(cBuffer, "%d", m_DeviceInfo.ucNoOfRelays);
	sReport += cBuffer;
	sReport += "\r\n";
}

#endif // #if !defined(_FEDM_NO_PD_SUPPORT)
