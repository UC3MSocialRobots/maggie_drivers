/*-------------------------------------------------------
|                                                       |
|                 FedmIscPeripheralDevice.h             |
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

Version       		:	03.02.00 / 18.05.2010 / M. Sahm, M. Hultsch

Operation Systems	:	independent

Function			:	classes for OBID i-scan® peripheral devices


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_PERIPHERAL_DEVICE_H_INCLUDED_)
#define _FEDM_ISC_PERIPHERAL_DEVICE_H_INCLUDED_

#ifdef _MSC_VER
	// The (microsoft) debugger can't handle symbols more than 255 characters long.
	// STL often creates symbols longer than that.
	// When symbols are longer than 255 characters, the warning is disabled.
	#pragma warning(disable:4786)	// for MFC
#endif

#if !defined(_FEDM_NO_PD_SUPPORT)

#include "../../FEDM_Base.h"
#include <vector>
#ifdef _FEDM_LINUX
#include <string.h>
#endif

class FEDM_ISCReader;
class FedmIscPeripheralDevice;


// STL - Standard Template Library
using namespace std;


//#####################################################################################
// constants
//#####################################################################################
		

//Peripheral Device Ports
#define FEDM_ISC_PD_PORT_TYPE_NONE			0x00
#define FEDM_ISC_PD_PORT_TYPE_RS4XX			0x02
	
// Peripheral Device types
#define FEDM_ISC_PD_TYPE_NONE				0x00
#define FEDM_ISC_PD_TYPE_PEOPLE_COUNTER		0x01
#define FEDM_ISC_PD_TYPE_EXTERNAL_IO		0x02

// Peripheral Device Software-Types
// see FEDM_ISC.h at location HF/UHF function unit types



//#####################################################################################
// type definitions
//#####################################################################################
		
typedef struct _FEISC_GPC_OUTPUT
{
	unsigned char	ucNumber;		//OUT-Number
	unsigned char	ucType;			//OUT-Type
	unsigned char	ucMode;			//OUTx-mode
	unsigned char	ucFrequency;	//OUTx-frq
	unsigned int	uiHoldTime;		//OUT-TIME

	_FEISC_GPC_OUTPUT() :	ucNumber(0),
							ucType(0),
							ucMode(0),
							ucFrequency(0),
							uiHoldTime(0)
							{}

} FEISC_GPC_OUTPUT;


typedef struct _FEDM_ISC_PERIPHERAL_DEVICE_INFO
{
	// software information
	// [0x66] and Mode=0x00
	unsigned char	ucSwVer[2];				// major and minor version number
	unsigned char	ucDevVer;					// developer version number
	unsigned char	ucHwType;					// flag field with hardware options
	unsigned char	ucDeviceType;				// software type == type of peripheral device
	unsigned char	Reserved[2];				// Reserved
	unsigned char	ucRxBufferSize[2];		// size of reader's receive buffer
	unsigned char	ucTxBufferSize[2];		// size of reader's transmit buffer

	// hardware information
	// [0x66] and Mode=0x10
	unsigned char	ucHwInfo[2];				// flag field with hardware info
	unsigned char	ucDHw[2];					// for internal use
	unsigned char	ucAHw[2];					// for internal use
	unsigned char	ucFrequency;				// frequency information (HF, UHF, .., EU, FCC)
	unsigned char	ucPortTypes;				// flag field with supported port types
	unsigned char	ucReserved1;				// reserved byte
	unsigned char	ucReserved2;				// reserved byte
	unsigned char	ucReserved3;				// reserved byte

	// CFG information with read and write permission
	// [0x66] and Mode=0x40 and Mode=0x41
	unsigned char	ucNoOfCfgPages[2];			// number of configuration pages
	unsigned char	ucCfgReadPermissions[16];	// flag field with readable CFG pages
	unsigned char	ucCfgWritePermissions[16];	// flag field with writeable CFG pages

	// input/output capabilities information
	// [0x66] and Mode=0x60
	unsigned char	ucNoOfInputs;				// number of inputs
	unsigned char	ucNoOfOutputs;				// number of outputs
	unsigned char	ucNoOfRelays;				// number of relays

	bool	bIsMode0x00Read;
	bool	bIsMode0x10Read;
	bool	bIsMode0x40Read;
	bool	bIsMode0x41Read;
	bool	bIsMode0x60Read;

	// initialization
	void Init()
	{
		memset(&ucSwVer[0], 0, sizeof(_FEDM_ISC_PERIPHERAL_DEVICE_INFO));
	}

} FEDM_ISC_PERIPHERAL_DEVICE_INFO;


//####################################################################
// class FEDM_ISCPeripheralDevice
//####################################################################

class _FEDM_ISC_CORE_EXT_CLASS FedmIscPeripheralDevice
{
// public functions
public:

	// constructor
	FedmIscPeripheralDevice(FEDM_ISCReader* pReader, 
							unsigned char ucBusAdr, 
							unsigned char ucPort, 
							int iFUType = FEDM_ISC_PD_TYPE_NONE);

	// destructor
	virtual ~FedmIscPeripheralDevice();

	// get/set function unit type, which controlls internal operations
	int GetPDType();

	unsigned char GetBusAddress();

	FEDM_ISCReader* GetReaderObject();

	// get the last error code
	int GetLastError();
	// get the last status byte
	unsigned char GetLastStatus();
	
	// get error text
	const char* GetErrorText(int iErrorCode);
	// get status text
	const char* GetStatusText(unsigned char ucStatus);

	int Reset();
	FEDM_ISC_PERIPHERAL_DEVICE_INFO* ReadInfo();
	FEDM_ISC_PERIPHERAL_DEVICE_INFO* GetInfo()		{ return &m_DeviceInfo; }
	void BuildInfoReport(string& sReport);

// protected functions
protected:
	void SetLastError(int iError);

	void Add0x00InfoText(string& sReport);
	void Add0x10InfoText(string& sReport);
	void Add0x60InfoText(string& sReport);

// public data
public:

// protected data
protected:
	// reader for communication
	FEDM_ISCReader* m_pReader;

	// Reader Handle
	int m_iReaderHnd;

	//Bus Address of Peripheral Device
	unsigned char m_ucBusAdr;
	//Port of the Peripheral Device
	unsigned char m_ucPort;

	int m_iLastError;
	unsigned char m_ucLastStatus;

	int m_iPDType;

	// Buffer for Data Transfer
	unsigned char	m_ucSendData[128];
	int				m_iSendDataLen;
	unsigned char	m_ucSendProt[128];
	int				m_iSendProtLen;
	unsigned char	m_ucRspProt[128];
	int				m_iRspProtLen;
	unsigned char	m_ucRspData[128];
	int				m_iRspDataLen;

	// Info struture for peripheral Devices
	FEDM_ISC_PERIPHERAL_DEVICE_INFO	m_DeviceInfo;
};

#endif // #if !defined(_FEDM_NO_PD_SUPPORT)

#endif // !defined(_FEDM_ISC_PERIPHERAL_DEVICE_H_INCLUDED_)
