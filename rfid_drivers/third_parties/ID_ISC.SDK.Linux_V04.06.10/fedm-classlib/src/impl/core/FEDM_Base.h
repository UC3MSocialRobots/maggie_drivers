/*-------------------------------------------------------
|                                                       |
|                      FEDM_Base.h                      |
|                                                       |
---------------------------------------------------------

Copyright © 2000-2011	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de

Author         		:	Markus Hultsch
Begin        		:	13.05.2000
Version       		:	04.00.00 / 21.07.2011 / M. Hultsch

Operation Systems	:	Windows, WindowsCE
						Linux, µClinux

Function			:	base class for FEDM class library
						collects all access functions for data memories


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_BASE_H_INCLUDED_)
#define _FEDM_BASE_H_INCLUDED_




#ifdef _MSC_VER
	// The (microsoft) debugger can't handle symbols more than 255 characters long.
	// STL often creates symbols longer than that.
	// When symbols are longer than 255 characters, the warning is disabled.
	#pragma warning(disable:4786)	// for MFC
#endif



#include <string>
#include <vector>
#include <map>
#include "FEDM.h"

class FEDM_DataBase;

// the following include is for serial port support
// please define _FEDM_COM_SUPPORT with compiler option
// additionally, you must define _FEDM_WINDOWS or _FEDM_LINUX with compiler option
#ifdef _FEDM_COM_SUPPORT
	#include "fecom.h"
#endif

// the following include is for USB support
// please define _FEDM_USB_SUPPORT with compiler option
// additionally, you must define _FEDM_WINDOWS or _FEDM_LINUX with compiler option
#ifdef _FEDM_USB_SUPPORT
	#include "feusb.h"
#endif

// the following include is for TCP support
// please define _FEDM_TCP_SUPPORT with compiler option
// additionally, you must define _FEDM_WINDOWS or _FEDM_LINUX with compiler option
#ifdef _FEDM_TCP_SUPPORT
	#include "fetcp.h"
#endif


// STL - Standard Template Library
using namespace std;


// type declarations
typedef vector<unsigned char>				FEDM_BYTE_ARRAY;
typedef vector<unsigned char>::iterator		FEDM_BYTE_ARRAY_ITOR;

typedef vector<int>							FEDM_INT_ARRAY;
typedef vector<int>::iterator				FEDM_INT_ARRAY_ITOR;

typedef vector<string>						FEDM_STRING_ARRAY;
typedef vector<string>::iterator			FEDM_STRING_ARRAY_ITOR;

typedef vector<FEDM_DataBase*>				FEDM_READER_ARRAY;
typedef vector<FEDM_DataBase*>::iterator	FEDM_READER_ARRAY_ITOR;

// map with namespace as key and access-id as value
typedef map<string, string>					FEDM_MAP_ACCESS_ID;
typedef map<string, string>::iterator		FEDM_MAP_ACCESS_ID_ITOR;


class _FEDM_ISC_CORE_EXT_CLASS FEDM_Base
{
public:
	FEDM_Base();
	virtual ~FEDM_Base();

	char* GetLibVersion();

	// language setting for error text
	int SetLanguage(int iLanguage);
	static int GetLanguage();

	// for access with ID
	static int GetData(const char* ID, bool* Data, FEDM_BYTE_ARRAY& cArray, int BlockSize);
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	static int GetData(const char* ID, BOOL* Data, FEDM_BYTE_ARRAY& cArray, int BlockSize);
#endif
	static int GetData(const char* ID, unsigned char* Data, FEDM_BYTE_ARRAY& cArray, int BlockSize);
	static int GetData(const char* ID, unsigned char* Data, int Cnt, FEDM_BYTE_ARRAY& cArray, int BlockSize);
	static int GetData(const char* ID, unsigned int* Data, FEDM_BYTE_ARRAY& cArray, int BlockSize);
	static int GetData(const char* ID, __int64* Data, FEDM_BYTE_ARRAY& cArray, int BlockSize);
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	static int GetData(const char* ID, CString& Data, FEDM_BYTE_ARRAY& cArray, int BlockSize);
#endif
	static int GetData(const char* ID, string& Data, FEDM_BYTE_ARRAY& cArray, int BlockSize);
	static int GetData(const char* ID, char* Data, int DataLen, FEDM_BYTE_ARRAY& cArray, int BlockSize);

	static int SetData(const char* ID, bool Data, FEDM_BYTE_ARRAY& cArray, int BlockSize);
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	static int SetData(const char* ID, BOOL Data, FEDM_BYTE_ARRAY& cArray, int BlockSize);
#endif
	static int SetData(const char* ID, unsigned char Data, FEDM_BYTE_ARRAY& cArray, int BlockSize);
	static int SetData(const char* ID, unsigned char* Data, int Cnt, FEDM_BYTE_ARRAY& cArray, int BlockSize);
	static int SetData(const char* ID, unsigned int Data, FEDM_BYTE_ARRAY& cArray, int BlockSize);
	static int SetData(const char* ID, __int64 Data, FEDM_BYTE_ARRAY& cArray, int BlockSize);
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	static int SetData(const char* ID, CString Data, FEDM_BYTE_ARRAY& cArray, int BlockSize);
#endif
	static int SetData(const char* ID, string Data, FEDM_BYTE_ARRAY& cArray, int BlockSize);
	static int SetData(const char* ID, char* Data, int DataLen, FEDM_BYTE_ARRAY& cArray, int BlockSize);

	// for direct access with address
	static int GetData(int Adr, unsigned char* Data, FEDM_BYTE_ARRAY& cArray);
	static int GetData(int Adr, unsigned char* Data, int Cnt, FEDM_BYTE_ARRAY& cArray);
	static int GetData(int Adr, unsigned int* Data, FEDM_BYTE_ARRAY& cArray);
	static int GetData(int Adr, __int64* Data, FEDM_BYTE_ARRAY& cArray);
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	static int GetData(int Adr, CString& Data, int Cnt, FEDM_BYTE_ARRAY& cArray);
#endif

	static int SetData(int Adr, unsigned char Data, FEDM_BYTE_ARRAY& cArray);
	static int SetData(int Adr, unsigned char* Data, int Cnt, FEDM_BYTE_ARRAY& cArray);
	static int SetData(int Adr, unsigned int Data, FEDM_BYTE_ARRAY& cArray);
	static int SetData(int Adr, __int64 Data, FEDM_BYTE_ARRAY& cArray);
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	static int SetData(int Adr, CString Data, FEDM_BYTE_ARRAY& cArray);
#endif

	// return an error text for FEDM-error code
	static int GetErrorText(char* sText, int iErrCode);

#ifdef _FEDM_COM_SUPPORT
	void SetFecomLibName(char* pszLibName);
#endif
#ifdef _FEDM_USB_SUPPORT
	void SetFeusbLibName(char* pszLibName);
#endif
#ifdef _FEDM_TCP_SUPPORT
	void SetFetcpLibName(char* pszLibName);
#endif

protected:
	int m_iLastError;
	static int m_iLanguage;

	static int GetErrorText(int iLanguage, char* sText, int iErrCode);
	void SetLastError(int iErrorCode);

	
#ifdef _FEDM_WINDOWS
	// function detects the Windows operation version
	void GetOS();
	// Windows version
	int m_iWinVer;
#endif

	//void* LoadLib(const char* pszLibName);
	//void  CloseLib(void* pLib);

#if !defined(_FEDM_SUPPORT_SLINK)
	void* GetFunctionPtr(void* pLib, const char* szFctName);
#endif

// the following codelines are for serial port support
// please define _FEDM_COM_SUPPORT with compiler option
#ifdef _FEDM_COM_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)

public:
	void* GetFeComFunction(unsigned int uiFct);

protected:
	void* LoadFeCom();

protected:
	struct _FEDM_FECOM_PTR
	{
		// function pointers for fecom library
		LPFN_FECOM_GET_DLL_VERSION		lpfnGetDLLVersion;
		LPFN_FECOM_GET_ERROR_TEXT		lpfnGetErrorText;
		LPFN_FECOM_GET_LAST_ERROR		lpfnGetLastError;
		LPFN_FECOM_DETECT_PORT			lpfnDetectPort;
		LPFN_FECOM_ADD_EVENT_HANDLER	lpfnAddEventHandler;
		LPFN_FECOM_DEL_EVENT_HANDLER	lpfnDelEventHandler;
		LPFN_FECOM_OPEN_PORT			lpfnOpenPort;
		LPFN_FECOM_CLOSE_PORT			lpfnClosePort;
		LPFN_FECOM_GET_PORT_LIST		lpfnGetPortList;
		LPFN_FECOM_GET_PORT_PARA		lpfnGetPortPara;
		LPFN_FECOM_SET_PORT_PARA		lpfnSetPortPara;
		LPFN_FECOM_DO_PORT_CMD			lpfnDoPortCmd;
		LPFN_FECOM_GET_PORT_HND			lpfnGetPortHnd;
		LPFN_FECOM_TRANSCEIVE			lpfnTransceive;
		LPFN_FECOM_TRANSMIT				lpfnTransmit;
		LPFN_FECOM_RECEIVE				lpfnReceive;

		// initialization
		_FEDM_FECOM_PTR() :	lpfnGetDLLVersion(NULL),
							lpfnGetErrorText(NULL),
							lpfnGetLastError(NULL),
							lpfnDetectPort(NULL),
							lpfnAddEventHandler(NULL),
							lpfnDelEventHandler(NULL),
							lpfnOpenPort(NULL),
							lpfnClosePort(NULL),
							lpfnGetPortList(NULL),
							lpfnGetPortPara(NULL),
							lpfnSetPortPara(NULL),
							lpfnDoPortCmd(NULL),
							lpfnGetPortHnd(NULL),
							lpfnTransceive(NULL),
							lpfnTransmit(NULL),
							lpfnReceive(NULL)
							{}
	};
#endif // #if !defined(_FEDM_SUPPORT_SLINK)
#endif // #ifdef _FEDM_COM_SUPPORT


// the following codelines are for USB support
// please define _FEDM_USB_SUPPORT with compiler option
#ifdef _FEDM_USB_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)

public:
	void* GetFeUsbFunction(unsigned int uiFct);

protected:
	void* LoadFeUsb();

protected:
	struct _FEDM_FEUSB_PTR
	{
		// function pointers for feusb library
		LPFN_FEUSB_GET_DLL_VERSION		lpfnGetDLLVersion;
		LPFN_FEUSB_GET_DRV_VERSION		lpfnGetDrvVersion;
		LPFN_FEUSB_GET_ERROR_TEXT		lpfnGetErrorText;
		LPFN_FEUSB_GET_LAST_ERROR		lpfnGetLastError;
		LPFN_FEUSB_SCAN					lpfnScan;
		LPFN_FEUSB_SCAN_AND_OPEN		lpfnScanAndOpen;
		LPFN_FEUSB_GET_SCAN_LIST_PARA	lpfnGetScanListPara;
		LPFN_FEUSB_GET_SCAN_LIST_SIZE	lpfnGetScanListSize;
		LPFN_FEUSB_CLEAR_SCAN_LIST		lpfnClearScanList;
		LPFN_FEUSB_ADD_EVENT_HANDLER	lpfnAddEventHandler;
		LPFN_FEUSB_DEL_EVENT_HANDLER	lpfnDelEventHandler;
		LPFN_FEUSB_OPEN_DEVICE			lpfnOpenDevice;
		LPFN_FEUSB_CLOSE_DEVICE			lpfnCloseDevice;
		LPFN_FEUSB_IS_DEVICE_PRESENT	lpfnIsDevicePresent;
		LPFN_FEUSB_GET_DEVICE_LIST		lpfnGetDeviceList;
		LPFN_FEUSB_GET_DEVICE_PARA		lpfnGetDevicePara;
		LPFN_FEUSB_SET_DEVICE_PARA		lpfnSetDevicePara;
		LPFN_FEUSB_GET_DEVICE_HND		lpfnGetDeviceHnd;
		LPFN_FEUSB_TRANSCEIVE			lpfnTransceive;
		LPFN_FEUSB_TRANSMIT				lpfnTransmit;
		LPFN_FEUSB_RECEIVE				lpfnReceive;

		// initialization
		_FEDM_FEUSB_PTR() :	lpfnGetDLLVersion(NULL),
							lpfnGetDrvVersion(NULL),
							lpfnGetErrorText(NULL),
							lpfnGetLastError(NULL),
							lpfnScan(NULL),
							lpfnScanAndOpen(NULL),
							lpfnGetScanListPara(NULL),
							lpfnGetScanListSize(NULL),
							lpfnClearScanList(NULL),
							lpfnAddEventHandler(NULL),
							lpfnDelEventHandler(NULL),
							lpfnOpenDevice(NULL),
							lpfnCloseDevice(NULL),
							lpfnIsDevicePresent(NULL),
							lpfnGetDeviceList(NULL),
							lpfnGetDevicePara(NULL),
							lpfnSetDevicePara(NULL),
							lpfnGetDeviceHnd(NULL),
							lpfnTransceive(NULL),
							lpfnTransmit(NULL),
							lpfnReceive(NULL)
							{}
	};
#endif // #if !defined(_FEDM_SUPPORT_SLINK)
#endif


// the following codelines are for TCP support
// please define _FEDM_TCP_SUPPORT with compiler option
#ifdef _FEDM_TCP_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)

public:
	void* GetFeTcpFunction(unsigned int uiFct);

protected:
	void* LoadFeTcp();

protected:
	struct _FEDM_FETCP_PTR
	{
		// function pointers for fetcp library
		LPFN_FETCP_GET_DLL_VERSION		lpfnGetDLLVersion;
		LPFN_FETCP_GET_ERROR_TEXT		lpfnGetErrorText;
		LPFN_FETCP_GET_LAST_ERROR		lpfnGetLastError;
		LPFN_FETCP_ADD_EVENT_HANDLER	lpfnAddEventHandler;
		LPFN_FETCP_DEL_EVENT_HANDLER	lpfnDelEventHandler;
		LPFN_FETCP_DETECT				lpfnDetect;
		LPFN_FETCP_CONNECT				lpfnConnect;
		LPFN_FETCP_DISCONNECT			lpfnDisConnect;
		LPFN_FETCP_GET_SOCKET_LIST		lpfnGetSocketList;
		LPFN_FETCP_GET_SOCKET_PARA		lpfnGetSocketPara;
		LPFN_FETCP_SET_SOCKET_PARA		lpfnSetSocketPara;
		LPFN_FETCP_GET_SOCKET_HND		lpfnGetSocketHnd;
		LPFN_FETCP_GET_SOCKET_STATE		lpfnGetSocketState;
		LPFN_FETCP_TRANSCEIVE			lpfnTransceive;
		LPFN_FETCP_TRANSMIT				lpfnTransmit;
		LPFN_FETCP_RECEIVE				lpfnReceive;

		// initialization
		_FEDM_FETCP_PTR() :	lpfnGetDLLVersion(NULL),
							lpfnGetErrorText(NULL),
							lpfnGetLastError(NULL),
							lpfnAddEventHandler(NULL),
							lpfnDelEventHandler(NULL),
							lpfnDetect(NULL),
							lpfnConnect(NULL),
							lpfnDisConnect(NULL),
							lpfnGetSocketList(NULL),
							lpfnGetSocketPara(NULL),
							lpfnSetSocketPara(NULL),
							lpfnGetSocketHnd(NULL),
							lpfnGetSocketState(NULL),
							lpfnTransceive(NULL),
							lpfnTransmit(NULL),
							lpfnReceive(NULL)
							{}
	};
#endif // #if !defined(_FEDM_SUPPORT_SLINK)
#endif // #ifdef _FEDM_TCP_SUPPORT

protected:
#ifdef _FEDM_COM_SUPPORT
	void* m_pFeComLib;		// pointer or handle to fecom
	void* m_pFeComPtrList;	// pointer to structure with function pointers of FECOM
	char* m_pFecomLibName;	// pointer to alternative fecom library name
#endif

#ifdef _FEDM_USB_SUPPORT
	void* m_pFeUsbLib;		// pointer or handle to feusb
	void* m_pFeUsbPtrList;	// pointer to structure with function pointers of FEUSB
	char* m_pFeusbLibName;	// pointer to alternative feusb library name
#endif

#ifdef _FEDM_TCP_SUPPORT
	void* m_pFeTcpLib;		// pointer or handle to feusb
	void* m_pFeTcpPtrList;	// pointer to structure with function pointers of FETCP
	char* m_pFetcpLibName;	// pointer to alternative fetcp library name
#endif
};

#endif // !defined(_FEDM_BASE_H_INCLUDED_)
