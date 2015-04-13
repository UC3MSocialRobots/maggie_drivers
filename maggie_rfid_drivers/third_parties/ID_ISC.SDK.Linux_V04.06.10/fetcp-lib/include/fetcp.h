/*-------------------------------------------------------
|                                                       |
|                       fetcp.h                         |
|                                                       |
---------------------------------------------------------

Copyright (C) 2003-2012	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Version       		:	02.02.00 / 28.03.2012 / M. Hultsch

Operating Systems	:	Windows XP / Vista / 7 / 8
						Windows CE
						Linux
						Mac OS X V10.7.3 or higher
						Android



This file contains the constants, datatypes and function declartions of FETCP library
*/

#ifndef _FETCP_INCLUDE_H
#define _FETCP_INCLUDE_H


#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(_CVI_)
	#ifdef FETCPDLL
		#define DLL_EXT_FUNC __declspec(dllexport) __stdcall
	#else
		#define DLL_EXT_FUNC __declspec(dllimport) __stdcall
	#endif
	#define FETCP_EXT_CALL __stdcall
	#define FETCP_CALL __cdecl
#else
	#define	DLL_EXT_FUNC
	#define	CALLBACK
	#define FETCP_EXT_CALL
	#define FETCP_CALL
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
// FETCP constants
// #####################################################

// FETCP error codes

// common errors
#define	FETCP_ERR_NEWSOCKET_FAILURE			-1200
#define	FETCP_ERR_EMPTY_LIST				-1201
#define FETCP_ERR_POINTER_IS_NULL			-1202
#define FETCP_ERR_NO_MEMORY					-1203
#define FETCP_ERR_SERVER_NOT_FOUND			-1205

// error while connect/disconnect the server
#define	FETCP_ERR_NO_CONNECTION				-1211
#define	FETCP_ERR_SERVER_ADDR				-1212	// Wenn Hostname nicht aufgel�st werden kann
#define	FETCP_ERR_CLOSE_CONNECTION			-1213

// handle errors
#define	FETCP_ERR_UNKNOWN_HND				-1220
#define	FETCP_ERR_HND_IS_NULL				-1221
#define	FETCP_ERR_HND_IS_NEGATIVE			-1222
#define	FETCP_ERR_NO_HND_FOUND				-1223

// communication errors
#define	FETCP_ERR_TIMEOUT					-1230
#define	FETCP_ERR_RECEIVE_PROCESS			-1232
#define	FETCP_ERR_CHANGE_SOCKET_PARA		-1236
#define	FETCP_ERR_TRANSMIT_PROCESS			-1237
#define	FETCP_ERR_GET_CONNECTION_STATE		-1238

// parameter errors
#define	FETCP_ERR_UNKNOWN_PARAMETER			-1250
#define	FETCP_ERR_PARAMETER_OUT_OF_RANGE	-1251
#define	FETCP_ERR_ODD_PARAMETERSTRING		-1252
#define	FETCP_ERR_UNKNOWN_ERRORCODE			-1254

// communication data flow errors
#define	FETCP_ERR_BUFFER_OVERFLOW			-1270



// constants for dynamic load of library

// miscellaneous functions
#define FETCP_GET_DLL_VERSION			1200
#define FETCP_GET_ERROR_TEXT			1201
#define FETCP_GET_LAST_ERROR			1202

// functions for event notification
#define FETCP_ADD_EVENT_HANDLER			1210
#define FETCP_DEL_EVENT_HANDLER			1211

// port functions
#define FETCP_DETECT					1220
#define FETCP_CONNECT					1221
#define FETCP_DISCONNECT				1222
#define FETCP_GET_SOCKET_LIST			1223
#define FETCP_GET_SOCKET_PARA			1224
#define FETCP_SET_SOCKET_PARA			1225
#define FETCP_GET_SOCKET_HND			1226
#define FETCP_GET_SOCKET_STATE			1227

// communication function
#define FETCP_TRANSCEIVE				1230
#define FETCP_TRANSMIT					1231
#define FETCP_RECEIVE					1232


// TCP states, returned by					FETCP_GetSocketStatus | FETCP_DisConnect
#define FETCP_STATE_CLOSED     			 1	//		X					X
#define FETCP_STATE_LISTEN     			 2	//		X					X
#define FETCP_STATE_SYN_SENT   			 3	//		X					X
#define FETCP_STATE_SYN_RCVD   			 4	//		X					X
#define FETCP_STATE_ESTABLISHED			 5	//		X					X
#define FETCP_STATE_FIN_WAIT1  			 6	//		X					X
#define FETCP_STATE_FIN_WAIT2  			 7	//		X					X
#define FETCP_STATE_CLOSE_WAIT 			 8	//		X					X
#define FETCP_STATE_CLOSING    			 9	//		X					X
#define FETCP_STATE_LAST_ACK   			10	//		X					X
#define FETCP_STATE_TIME_WAIT  			11	//		X					-
#define FETCP_STATE_DELETE_TCB 			12	//		X					-



// defines for uiFlag in FETCP_EVENT_INIT
#define FETCP_THREAD_ID		1
#define FETCP_WND_HWND		2
#define FETCP_CALLBACK		3

// defines for uiUse in FETCP_EVENT_INIT
#define FETCP_DEV_DISCONNECT_EVENT		   2



// #####################################################
// FETCP structures
// #####################################################

// structure for transfering thread-IDs, message-handles or callbacks
typedef struct _FETCP_EVENT_INIT
{
	void* pAny;		// pointer to anything, which is reflected as the first parameter 
					// in the callback function (e.g. can be used to pass the object pointer)

	unsigned int uiUse;		// defines the event (e.g. FETCP_xyz_EVENT)
	unsigned int uiMsg;		// message code used with dwThreadID and hwndWnd (e.g. WM_USER_xyz)
	unsigned int uiFlag;	// specifies the use of the union (e.g. FETCP_WND_HWND)
	union
	{
#if defined(_MSC_VER) || defined(__BORLANDC__)
		DWORD	dwThreadID;			// for thread-ID
		HWND	hwndWnd;			// for window-handle
#endif
		// callback-function
		void (FETCP_CALL* cbFct1)(	void* pAny,					// [in] pointer to anything (from struct _FETCP_EVENT_INIT)
									int iSocketHnd,				// [in] socket handle of FETCP
									unsigned int uiUse,			// [in] reflects the event (e.g. FETCP_xyz_EVENT)
									int iValue );				// [in] a value
#ifdef __cplusplus
	};
#else
	}Method;
#endif

} FETCP_EVENT_INIT;



// #####################################################
// FETCP functions
// #####################################################

// miscellaneous functions
void DLL_EXT_FUNC FETCP_GetDLLVersion( char* cVersion );
int  DLL_EXT_FUNC FETCP_GetErrorText( int iErrorCode, char* cErrorText );
int  DLL_EXT_FUNC FETCP_GetLastError( int iSocketHnd, int* iErrorCode, char* cErrorText );
int  DLL_EXT_FUNC FETCP_Detect( char* cHostAdr, int iPortNr );
int  DLL_EXT_FUNC FETCP_GetSocketState( char* cHostAdr, int iPortNr );

// functions for event notification
int  DLL_EXT_FUNC FETCP_AddEventHandler(int iSocketHnd, FETCP_EVENT_INIT* pInit);
int  DLL_EXT_FUNC FETCP_DelEventHandler(int iSocketHnd, FETCP_EVENT_INIT* pInit);

// socket functions for normal applications
int  DLL_EXT_FUNC FETCP_Connect( char* cHostAdr, int iPortNr );
int  DLL_EXT_FUNC FETCP_DisConnect( int iSocketHnd );

// port administration functions
int  DLL_EXT_FUNC FETCP_GetSocketList( int iNext );
int  DLL_EXT_FUNC FETCP_GetSocketPara( int iSocketHnd, char* cPara, char* cValue );
int  DLL_EXT_FUNC FETCP_SetSocketPara( int iSocketHnd, char* cPara, char* cValue );
int  DLL_EXT_FUNC FETCP_GetSocketHnd( char* cHostAdr, int iPortNr );

// communication function
int  DLL_EXT_FUNC FETCP_Transceive( int iSocketHnd, unsigned char* cSendProt, int iSendLen, unsigned char* cRecProt, int iRecLen );
int  DLL_EXT_FUNC FETCP_Transmit( int iSocketHnd, unsigned char* cSendProt, int iSendLen );
int  DLL_EXT_FUNC FETCP_Receive( int iSocketHnd, unsigned char* cRecProt, int iRecLen );


#undef DLL_EXT_FUNC

#ifdef __cplusplus
}
#endif


// #####################################################
// typedefs of DLL-functions for explicite loading
// #####################################################

// miscellaneous functions
typedef void (CALLBACK* LPFN_FETCP_GET_DLL_VERSION)(char*);
typedef int  (CALLBACK* LPFN_FETCP_GET_ERROR_TEXT)(int, char*);
typedef int  (CALLBACK* LPFN_FETCP_GET_LAST_ERROR)(int, int*, char*);

// functions for event notification
typedef int  (CALLBACK* LPFN_FETCP_ADD_EVENT_HANDLER)(int, FETCP_EVENT_INIT*);
typedef int  (CALLBACK* LPFN_FETCP_DEL_EVENT_HANDLER)(int, FETCP_EVENT_INIT*);

// port functions
typedef int  (CALLBACK* LPFN_FETCP_DETECT)(char*, int);
typedef int  (CALLBACK* LPFN_FETCP_CONNECT)(char*, int);
typedef int  (CALLBACK* LPFN_FETCP_DISCONNECT)(int);
typedef int  (CALLBACK* LPFN_FETCP_GET_SOCKET_STATE)(char*, int);
typedef int  (CALLBACK* LPFN_FETCP_GET_SOCKET_LIST)(int);
typedef int  (CALLBACK* LPFN_FETCP_GET_SOCKET_PARA)(int, char*, char*);
typedef int  (CALLBACK* LPFN_FETCP_SET_SOCKET_PARA)(int, char*, char*);
typedef int  (CALLBACK* LPFN_FETCP_GET_SOCKET_HND)(char*, int);

// communication function
typedef int  (CALLBACK* LPFN_FETCP_TRANSCEIVE)(int, unsigned char*, int, unsigned char*, int);
typedef int  (CALLBACK* LPFN_FETCP_TRANSMIT)(int, unsigned char*, int);
typedef int  (CALLBACK* LPFN_FETCP_RECEIVE)(int, unsigned char*, int);

#endif // _FETCP_INCLUDE_H
