/*-------------------------------------------------------
|                                                       |
|                      fescr.h                          |
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

Autor         	:		Manuel Sahm
Version       	:		01.02.01 / 31.07.2012 / B. Tschunko
						
Projekt       	:		API for ISO7816 APDUs
						
Betriebssystem	:		Windows XP / Vista / 7 / 8
						WindowsCE
						Linux

This file contains the constants, datatypes snd function declartions of FESCR library
*/


#ifndef _FESCR_INCLUDE_H
 #define _FESCR_INCLUDE_H


#if defined (_MSC_VER) || defined(__BORLANDC__) || defined(_CVI_)
	#ifdef FESCRDLL
		#define DLL_EXT_FUNC __declspec(dllexport) __stdcall
	#else
		#define DLL_EXT_FUNC __declspec(dllimport) __stdcall
	#endif
	#define FESCR_EXT_CALL __stdcall
	#define FESCR_CALL __cdecl
#else
	#define FESCR_EXT_CALL
	#define	DLL_EXT_FUNC
	#define FESCR_CALL
#endif


#ifdef __cplusplus
	extern "C" {
#endif


#define LANGUAGE		9


// error codes
//********************

// common errors
#define	FESCR_ERR_NEW_SMARTCARDSLOT_FAILURE			-4300
#define	FESCR_ERR_EMPTY_LIST						-4301
#define	FESCR_ERR_POINTER_IS_NULL					-4302
#define FESCR_ERR_NO_MORE_MEM						-4303
//#define FESCR_ERR_UNKNOWN_COMM_PORT				-4304
//#define FESCR_ERR_UNSUPPORTED_FUNCTION			-4305
#define FESCR_ERR_NO_USB_SUPPORT					-4306
//#define FESCR_ERR_OLD_FECOM						-4307
//#define FESCR_ERR_FILE_COULD_NOT_BE_OPENED		-4308
#define FESCR_ERR_APDU_CURRENTLY_RUNNING			-4309

// query errors
//#define FESCR_ERR_NO_VALUE		   				-4310

// handle errors
#define FESCR_ERR_UNKNOWN_HND						-4320
#define FESCR_ERR_HND_IS_NULL						-4321
#define FESCR_ERR_HND_IS_NEGATIVE					-4322
#define FESCR_ERR_NO_HND_FOUND						-4323
//#define FESCR_ERR_SMARTCARDSLOT_HND_IS_NEGATIVE	-4324
//#define FESCR_ERR_HND_UNVALID						-4325
#define FESCR_ERR_READER_HND_IS_NEGATIVE			-4326
#define FESCR_ERR_THREAD_NOT_CREATED				-4327

// parameter errors
#define FESCR_ERR_UNKNOWN_PARAMETER					-4350
#define FESCR_ERR_PARAMETER_OUT_OF_RANGE			-4351
//#define FESCR_ERR_ODD_PARAMETERSTRING				-4352
#define FESCR_ERR_UNKNOWN_ERRORCODE					-4353
#define	FESCR_ERR_UNDERSIZED_RESPONSE_BUFFER		-4357		// Responsebuffer < 64kB

// communication data flow errors
//#define FESCR_ERR_BUFFER_OVERFLOW					-4370
//#define	FESCR_ERR_OVERSIZED_RESPONSE			-4371		// Response > 64kB
//#define	FESCR_INVALID_ACKNOWLEDGEMENT			-4372
#define	FESCR_INVALID_ACKNOWLEDGEMENT_LENGTH		-4373
#define FESCR_LIST_COMPLETE_FAILURE					-4374		// Liste sollte komplett sein.
#define	FESCR_INCOMPLETE_RESPONSE					-4375		// Response zurueck, aber nicht	komplett.												//	gab aber was anderes 
#define FESCR_INVALID_PROTOCOL						-4376		
#define FESCR_INVALID_TRANSMISSION					-4377		


/*
// defines for uiFlag in FESCR_EVENT_INIT
#define FESCR_THREAD_ID			1
#define FESCR_WND_HWND			2
#define FESCR_CALLBACK			3
#define FESCR_CALLBACK2			4
#define FESCR_TASKCB_NET_1     10


// #################
// FESCR structures
// #################


typedef struct _FESCR_EVENT_INIT	// Structur for eventhandling; thread-ID, 
{									//   message-handle or callback

	void*	pAny;		// pointer to anything, which is reflected as the first parameter 
						// in the callback function cbFct2 (e.g. can be used to pass the object pointer)
	unsigned int	uiUse;		// unused
	unsigned int	uiMsg;		// message code used with dwThreadID and hwndWnd (e.g. WM_USER_xyz)
	unsigned int	uiFlag;		// specifies the use of the union (e.g. FESCR_WND_HWND)
	union
	{
#if defined(_MSC_VER) || defined(__BORLANDC__)
		DWORD	dwThreadID;					// for thread-ID
		HWND	hwndWnd;					// for window-handle
#endif
		void	(*cbFct)(int, int, int);                        // for callback-function
		void	(*cbFct2)(void*, int, int, int);                // for callback-function

#if defined(_MSC_VER)
		// for .NET delegates; for SAM, Queue and Container commands
		void	(CALLBACK* cbFctNET1)(	int iHandle,	// [out] handle of FESCR transponder object
										int iError,		// [out] OK (=0), error code (<0) or status byte from reader (>0)
										int iRspLen );	// [out] length of response data

#endif
#ifdef __cplusplus
	};
#else
	}Method;
#endif

} FESCR_EVENT_INIT;
*/



//##########################
// FESCR functions
//##########################
	

// miscellaneous functions
//********************

void DLL_EXT_FUNC FESCR_GetDLLVersion( char* cVersion );

int DLL_EXT_FUNC FESCR_GetErrorText( int iErrorCode, char* cErrorText );

int DLL_EXT_FUNC FESCR_GetLastError( int iSlotHnd, int* iErrorCode, char* cErrorText );



// ddministrative functions
//***************************

int  DLL_EXT_FUNC FESCR_NewScrSlot(	int iReaderHnd, 
									unsigned char ucBusAdr,
									int iSlotNo,
									unsigned char ucNad, 
									bool bUseNad );

int  DLL_EXT_FUNC FESCR_DeleteScrSlot( int iSlotHnd );

int  DLL_EXT_FUNC FESCR_GetScrSlotList( int iNext );


// communication functions
//**************************
int DLL_EXT_FUNC FESCR_Activate(	int iSlotHnd,
									unsigned char* ucReqData, 
									int iReqDataLen,
									unsigned char* ucRspData,
									unsigned int uiRspDataBufSize,
									int* iRspDataLen );

int DLL_EXT_FUNC FESCR_Deactivate( int iSlotHnd );

int DLL_EXT_FUNC FESCR_Apdu(		int iSlotHnd, 
									unsigned char* ucReqApdu, 
									int iReqApduLen, 
									unsigned char* ucRspApdu,
									unsigned int uiRspApduBufSize,
									int* iRspApduLen );
/*
int DLL_EXT_FUNC FESCR_Apdu_Async(	int iSlotHnd, 
									unsigned char* ucReqApdu, 
									int iReqApduLen, 
									FESCR_EVENT_INIT* pInit );
*/

/*
int DLL_EXT_FUNC FESCR_GetResponseData(	int iSlotHnd, 
										unsigned char* ucRspApdu, 
										int uiRspApduBufSize );
*/

#undef DLL_EXT_FUNC

#ifdef __cplusplus
	}
#endif


#endif	// _FESCR_INCLUDE_H
