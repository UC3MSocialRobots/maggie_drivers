/*-------------------------------------------------------
|                                                       |
|                       feusb.h                         |
|                                                       |
---------------------------------------------------------

Copyright (C) 2000-2013	FEIG ELECTRONIC GmbH, All Rights Reserved.
                        Lange Strasse 4
                        D-35781 Weilburg
                        Federal Republic of Germany
                        phone    : +49 6471 31090
                        fax      : +49 6471 310999
                        e-mail   : obid-support@feig.de
                        Internet : http://www.feig.de

Author         		:	Markus Hultsch

Version       		:	Linux:			05.00.00 / 01.12.2014 / A. Bierschenk
Version       		:	Windows:		04.02.06 / 23.01.2013 / M. Hultsch
Version       		:	Linux:			04.02.01 / 12.02.2014 / M. Sahm
Version       		:	Windows CE:		04.02.06 / 23.01.2013 / M. Hultsch

Operation Systems	:	Windows XP / Vista / 7 / 8 (based on usbio V2.50)
                        WindowsCE (based on usbio_ce V1.16)
                        Linux (based on libusb V0.1.12)
						Mac OS X V10.7.3 or higher
						Android


This file contains the constants, datatypes and function declartions of FEUSB library
*/

#ifndef _FEUSB_INCLUDE_H
#define _FEUSB_INCLUDE_H



#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(_CVI_)
	#ifdef FEUSBDLL
		#define DLL_EXT_FUNC __declspec(dllexport) __stdcall
	#else
		#define DLL_EXT_FUNC __declspec(dllimport) __stdcall
	#endif
	#define FEUSB_EXT_CALL __stdcall
	#define FEUSB_CALL __cdecl
#else
	#define	DLL_EXT_FUNC
	#define	CALLBACK
	#define FEUSB_EXT_CALL
	#define FEUSB_CALL
#endif


// type defines
#ifdef __GNUC__
	#ifndef __int64
		#define __int64 long long
	#endif
#endif



#ifdef __cplusplus
extern "C" {
#endif




// #####################################################
// FEUSB constants
// #####################################################

	
// FEUSB error codes

// common errors
#define	FEUSB_ERR_EMPTY_DEVICELIST			-1100
#define	FEUSB_ERR_EMPTY_SCANLIST			-1101
#define FEUSB_ERR_POINTER_IS_NULL			-1102
#define	FEUSB_ERR_NO_MORE_MEM				-1103
#define	FEUSB_ERR_SET_CONFIGURATION			-1104
#define	FEUSB_ERR_KERNEL					-1105
#define	FEUSB_ERR_UNSUPPORTED_OPTION		-1106
#define FEUSB_ERR_UNSUPPORTED_FUNCTION		-1107
#define FEUSB_ERR_NO_CONFIGURATIONS         -1108	// new since linux-version 05.00.00 (01.12.14)
#define FEUSB_ERR_NOT_HAS_CAPABILITY        -1109	// new since linux-version 05.00.00 (01.12.14)

// error while scanning
#define	FEUSB_ERR_NO_FEIG_DEVICE			-1110
#define	FEUSB_ERR_SEARCH_MISMATCH			-1111
#define	FEUSB_ERR_NO_DEVICE_FOUND			-1112
#define	FEUSB_ERR_DEVICE_IS_SCANNED			-1113
#define	FEUSB_ERR_SCANLIST_OVERFLOW			-1114
#define FEUSB_ERR_NO_SCAN_FIRST             -1115

// handle errors
#define	FEUSB_ERR_UNKNOWN_HND				-1120
#define	FEUSB_ERR_HND_IS_NULL				-1121
#define	FEUSB_ERR_HND_IS_NEGATIVE			-1122
#define	FEUSB_ERR_NO_HND_FOUND				-1123	// means: handle not found

// communication errors (part1))
#define	FEUSB_ERR_TIMEOUT					-1130
#define	FEUSB_ERR_NO_SENDDATA				-1131
#define	FEUSB_ERR_UNKNOWN_INTERFACE			-1132
#define	FEUSB_ERR_UNKNOWN_DIRECTION			-1133
#define	FEUSB_ERR_RECBUF_TOO_SMALL			-1134
#define	FEUSB_ERR_SENDDATA_LEN				-1135
#define	FEUSB_ERR_UNKNOWN_DESCRIPTOR_TYPE	-1136
#define	FEUSB_ERR_DEVICE_NOT_PRESENT		-1137
#define	FEUSB_ERR_TRANSMIT_PROCESS			-1138
#define	FEUSB_ERR_BULK_TRANSFER 			-1139	// new since linux-version 05.00.00 (01.12.14)

// open/close device errors
#define	FEUSB_ERR_DEVICE_NOT_SCANNED		-1140
#define	FEUSB_ERR_DEVHND_NOT_IN_SCANLIST	-1141
#define	FEUSB_ERR_DRIVERLIST				-1142
#define FEUSB_ERR_OPEN_DEVICE               -1143
#define FEUSB_ERR_DEVICE_NOT_OPENED         -1144	// new since linux-version 05.00.00 (01.12.14)
#define FEUSB_ERR_CLAIM_INTERFACE           -1145	// new since linux-version 05.00.00 (01.12.14)

// parameter errors
#define	FEUSB_ERR_UNKNOWN_PARAMETER			-1150
#define	FEUSB_ERR_PARAMETER_OUT_OF_RANGE	-1151
#define	FEUSB_ERR_ODD_PARAMETERSTRING		-1152
#define	FEUSB_ERR_INDEX_OUT_OF_RANGE		-1153
#define	FEUSB_ERR_UNKNOWN_SCANOPTION		-1154
#define	FEUSB_ERR_UNKNOWN_ERRORCODE			-1155

// identification errors
#define FEUSB_ERR_DEV_DESC_LENGTH			-1160
#define FEUSB_ERR_CFG_DESC_LENGTH			-1161
#define FEUSB_ERR_INTF_DESC_LENGTH			-1162
#define FEUSB_ERR_ENDP_DESC_LENGTH			-1163
#define FEUSB_ERR_HID_DESC_LENGTH			-1164
#define FEUSB_ERR_STRG_DESC_LENGTH			-1165
#define	FEUSB_ERR_READ_DEV_DESCRIPTOR		-1166
#define	FEUSB_ERR_READ_CFG_DESCRIPTOR		-1167
#define	FEUSB_ERR_READ_STRG_DESCRIPTOR		-1168

#define FEUSB_ERR_MAX_INTERFACES			-1170
#define FEUSB_ERR_MAX_ENDPOINTS				-1171
#define FEUSB_ERR_MAX_STRINGS				-1172

// communication errors (part2)
#define	FEUSB_ERR_CONTROL_TRANSFER 			-1180	// new since linux-version 05.00.00 (01.12.14)
	


// constants for dynamic load of library

// miscellaneous functions
#define FEUSB_GET_DLL_VERSION				1100
#define FEUSB_GET_DRV_VERSION				1101
#define FEUSB_GET_ERROR_TEXT				1102
#define FEUSB_GET_LAST_ERROR				1103
											
// function for scan-list					
#define FEUSB_SCAN							1110
#define FEUSB_SCAN_AND_OPEN					1111
#define FEUSB_GET_SCAN_LIST_PARA			1112
#define FEUSB_GET_SCAN_LIST_SIZE			1113
#define FEUSB_CLEAR_SCAN_LIST				1114
#define FEUSB_GET_SCAN_LIST_INDEX           1115
											
// functions for event notification			
#define FEUSB_ADD_EVENT_HANDLER				1120
#define FEUSB_DEL_EVENT_HANDLER				1121
											
// device functions							
#define FEUSB_OPEN_DEVICE					1130
#define FEUSB_CLOSE_DEVICE					1131
#define FEUSB_IS_DEVICE_PRESENT				1132
#define FEUSB_GET_DEVICE_LIST				1133
#define FEUSB_GET_DEVICE_PARA				1134
#define FEUSB_SET_DEVICE_PARA				1135
#define FEUSB_GET_DEVICE_HND				1136
											
// communication function					
#define FEUSB_TRANSCEIVE					1140
#define FEUSB_TRANSMIT						1141
#define FEUSB_RECEIVE						1142



// constants for structure _FEUSB_EVENT_INIT

// defines for uiFlag in FEUSB_EVENT_INIT
#define FEUSB_THREAD_ID					1
#define FEUSB_WND_HWND					2
#define FEUSB_CALLBACK					3
#define FEUSB_CALLBACK_2				4

// defines for uiUse in FEUSB_EVENT_INIT
#define FEUSB_DEV_CONNECT_EVENT			1
#define FEUSB_DEV_DISCONNECT_EVENT		2


// constants for function options

// scan options
#define	FEUSB_SCAN_FIRST				0x00000001
#define	FEUSB_SCAN_NEXT					0x00000002
#define	FEUSB_SCAN_NEW					0x00000003
#define	FEUSB_SCAN_ALL					0x0000000F
#define	FEUSB_SCAN_SEARCH				0x00010000
#define	FEUSB_SCAN_PACK					0x00020000

// search options
#define	FEUSB_SEARCH_FAMILY				0x00000001
#define	FEUSB_SEARCH_PRODUCT			0x00000002
#define	FEUSB_SEARCH_DEVICEID			0x00000004

// communication directions
#define	FEUSB_DIR_IN					0x00000001
#define	FEUSB_DIR_OUT					0x00000002

// Interface names
#define FEUSB_INTERFACE_OBID_RCI		"OBID-RCI"
#define FEUSB_INTERFACE_OBID_RCI_V2		"OBID-RCI2"
	
// special handle values for FEUSB_SetDevicePara-function - new since linux-version 05.00.00 (01.12.14)
#define FEUSB_PARAHND_ALL_DEVICES		0
#define FEUSB_PARAHND_DLL				-1



// #####################################################
// FEUSB structures
// #####################################################

// structure for message-handling
typedef struct _FEUSB_EVENT_INIT
{
	unsigned int uiUse;		// specifies the event (connect or disconnect)
	unsigned int uiMsg;		// message code used with dwThreadID and hwndWnd (e.g. WM_USER_xyz)
	unsigned int uiFlag;	// specifies the use of the union (e.g. FEUSB_WND_HWND)
	void* pAny;				// pointer to anything, which is reflected as the first parameter of cbFct2
							// in the callback function (e.g. can be used to pass the object pointer)

	union
	{
#if defined(_MSC_VER) || defined(__BORLANDC__)
		DWORD	dwThreadID;				// for thread-ID
		HWND	hwndWnd;				// for window-handle
#endif
		void	(FEUSB_CALL* cbFct)(int, unsigned int);			// for 1st callback-function
		void	(FEUSB_CALL* cbFct2)(void*, int, unsigned int);	// for 2nd callback-function
#ifdef __cplusplus
	};
#else
	}Method;
#endif

} FEUSB_EVENT_INIT;


// structure for scan with search options
typedef struct _FEUSB_SCANSEARCH
{
	int		iMask;
	char	cFamilyName[25];
	char	cDeviceName[25];
	char	cInterface[25];
	char	cDeviceID[16];

} FEUSB_SCANSEARCH;
	


// #####################################################
// FEUSB functions
// #####################################################

// miscellaneous functions
void DLL_EXT_FUNC FEUSB_GetDLLVersion(char* cVersion);
int  DLL_EXT_FUNC FEUSB_GetDrvVersion(char* cVersion);
int  DLL_EXT_FUNC FEUSB_GetErrorText(int iError, char* cText);
int	 DLL_EXT_FUNC FEUSB_GetLastError(int iDevHnd, int* iErrorCode, char* cErrorText);

// function for scan-list
int  DLL_EXT_FUNC FEUSB_Scan(int iScanOpt, FEUSB_SCANSEARCH* pSearchOpt);
int  DLL_EXT_FUNC FEUSB_ScanAndOpen(int iScanOpt, FEUSB_SCANSEARCH* pSearchOpt);
int  DLL_EXT_FUNC FEUSB_GetScanListPara(int iIndex, char* cParaID, char* cValue);
int  DLL_EXT_FUNC FEUSB_GetScanListSize();
void DLL_EXT_FUNC FEUSB_ClearScanList();

// functions for event notification
int  DLL_EXT_FUNC FEUSB_AddEventHandler(int iDevHnd, FEUSB_EVENT_INIT* pInit);
int  DLL_EXT_FUNC FEUSB_DelEventHandler(int iDevHnd, FEUSB_EVENT_INIT* pInit);

// device functions
int  DLL_EXT_FUNC FEUSB_OpenDevice(long nDeviceID);
int  DLL_EXT_FUNC FEUSB_CloseDevice(int iDevHnd);
int  DLL_EXT_FUNC FEUSB_IsDevicePresent(int iDevHnd);
int  DLL_EXT_FUNC FEUSB_GetDeviceList(int iDevHnd);   // FindFirstOrNextDeviceHandle
int  DLL_EXT_FUNC FEUSB_GetDevicePara(int iDevHnd, char* cPara, char* cValue);
int  DLL_EXT_FUNC FEUSB_SetDevicePara(int iDevHnd, char* cPara, char* cValue);
int  DLL_EXT_FUNC FEUSB_GetDeviceHnd(long nDeviceID);

// communication function
int  DLL_EXT_FUNC FEUSB_Transceive(int iDevHnd, char* cInterface, int iDir, unsigned char* cSendData, int iSendLen, unsigned char* cRecData, int iRecLen);
int  DLL_EXT_FUNC FEUSB_Transmit(int iDevHnd, char* cInterface, unsigned char* cSendData, int iSendLen);
int  DLL_EXT_FUNC FEUSB_Receive(int iDevHnd, char* cInterface, unsigned char* cRecData, int iRecLen);


#undef DLL_EXT_FUNC

#ifdef __cplusplus
}
#endif


// #####################################################
// typedefs of DLL-functions for explicite loading
// #####################################################

// miscellaneous functions
typedef void (CALLBACK* LPFN_FEUSB_GET_DLL_VERSION)(char*);
typedef int  (CALLBACK* LPFN_FEUSB_GET_DRV_VERSION)(char*);
typedef int  (CALLBACK* LPFN_FEUSB_GET_ERROR_TEXT)(int, char*);
typedef int  (CALLBACK* LPFN_FEUSB_GET_LAST_ERROR)(int, int*, char*);

// function for scan-list
typedef int  (CALLBACK* LPFN_FEUSB_SCAN)(int, FEUSB_SCANSEARCH*);
typedef int  (CALLBACK* LPFN_FEUSB_SCAN_AND_OPEN)(int, FEUSB_SCANSEARCH*);
typedef int  (CALLBACK* LPFN_FEUSB_GET_SCAN_LIST_PARA)(int, char*, char*);
typedef int  (CALLBACK* LPFN_FEUSB_GET_SCAN_LIST_SIZE)();
typedef void (CALLBACK* LPFN_FEUSB_CLEAR_SCAN_LIST)();

// functions for event notification
typedef int  (CALLBACK* LPFN_FEUSB_ADD_EVENT_HANDLER)(int, FEUSB_EVENT_INIT*);
typedef int  (CALLBACK* LPFN_FEUSB_DEL_EVENT_HANDLER)(int, FEUSB_EVENT_INIT*);

// device functions
typedef int  (CALLBACK* LPFN_FEUSB_OPEN_DEVICE)(long);
typedef int  (CALLBACK* LPFN_FEUSB_CLOSE_DEVICE)(int);
typedef int  (CALLBACK* LPFN_FEUSB_IS_DEVICE_PRESENT)(int);
typedef int  (CALLBACK* LPFN_FEUSB_GET_DEVICE_LIST)(int);
typedef int  (CALLBACK* LPFN_FEUSB_GET_DEVICE_PARA)(int, char*, char*);
typedef int  (CALLBACK* LPFN_FEUSB_SET_DEVICE_PARA)(int, char*, char*);
typedef int  (CALLBACK* LPFN_FEUSB_GET_DEVICE_HND)(long);

// communication function
typedef int (CALLBACK* LPFN_FEUSB_TRANSCEIVE)(int, char*, int, unsigned char*, int, unsigned char*, int);
typedef int (CALLBACK* LPFN_FEUSB_TRANSMIT)(int, char*, unsigned char*, int);
typedef int (CALLBACK* LPFN_FEUSB_RECEIVE)(int, char*, unsigned char*, int);

#endif // _FEUSB_INCLUDE_H
