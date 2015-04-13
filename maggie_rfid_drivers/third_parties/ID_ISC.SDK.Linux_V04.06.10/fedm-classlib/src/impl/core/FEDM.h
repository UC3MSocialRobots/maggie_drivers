/*-------------------------------------------------------
|                                                       |
|                       FEDM.h                          |
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
Begin        		:	16.05.2000
Version       		:	04.06.10 / 09.12.2014 / M. Hultsch

Operation Systems	:	independent


This file contains the defines for the component FEDM


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH

Linux® is a registered trademark of Linus Torvalds.
Windows is a registered trademark of Microsoft Corporation in the United States and other countries.
Apple, Mac, Mac OS, OS X, Cocoa and Xcode are trademarks of Apple Inc., registered in the U.S. and other countries.
Android is a trademark of Google Inc.

Electronic Product Code (TM) is a Trademark of EPCglobal Inc.
I-CODE® and Mifare® are registered Trademarks of Philips Electronics N.V.
Tag-it (TM) is a registered Trademark of Texas Instruments Inc.
my-d® is a registered trademark of Infineon Technologies AG
Jewel (TM) is a trademark of Innovision Research & Technology plc.
*/


#if !defined(_FEDM_H_INCLUDED_)
#define _FEDM_H_INCLUDED_


#if defined(_MSC_VER) || defined(__BORLANDC__)

		#if defined(__BORLANDC__)
				#define _FEDM_ISC_CORE_CALL_CONV __stdcall
		#else
				#define _FEDM_ISC_CORE_CALL_CONV
		#endif

		#define FEDM_CALL __cdecl

	#ifdef _FEDM_ISC_CORE_DLL
		#define _FEDM_ISC_CORE_EXT_CLASS __declspec(dllexport) _FEDM_ISC_CORE_CALL_CONV
		#define _FEDM_ISC_CORE_EXT_FUNCT __declspec(dllexport)
	#else
		#if !defined(_FEDM_DLL)
			#define	_FEDM_ISC_CORE_EXT_CLASS
			#define _FEDM_ISC_CORE_EXT_FUNCT
		#else
			#define _FEDM_ISC_CORE_EXT_CLASS __declspec(dllimport) _FEDM_ISC_CORE_CALL_CONV
			#define _FEDM_ISC_CORE_EXT_FUNCT
		#endif
	#endif

	#ifdef _FEDM_SERVICE_DLL
		#define _FEDM_SERVICE_EXT_CLASS __declspec(dllexport)
	#else
		#if !defined(_FEDM_DLL)
			#define	_FEDM_SERVICE_EXT_CLASS
		#else
			#define _FEDM_SERVICE_EXT_CLASS __declspec(dllimport)
		#endif
	#endif

	#ifdef _FEDM_ISC_TAG_DIALOG_DLL
		#define _FEDM_ISC_TAG_DIALOG_EXT_CLASS __declspec(dllexport)
	#else
		#if !defined(_FEDM_DLL)
			#define	_FEDM_ISC_TAG_DIALOG_EXT_CLASS
		#else
			#define _FEDM_ISC_TAG_DIALOG_EXT_CLASS __declspec(dllimport)
		#endif
	#endif
#else
	#define _FEDM_ISC_CORE_EXT_CLASS
	#define _FEDM_ISC_CORE_EXT_FUNCT
	#define _FEDM_SERVICE_EXT_CLASS
	#define FEDM_CALL  
#endif



// for MFC (Microsoft Foundation Class)
#if _MSC_VER
	#ifdef _FEDM_MFC_SUPPORT
		#include <afxwin.h>
	#else
		#include <windows.h>
		#include <tchar.h>
	#endif
//	#define _FEDM_TRACE
#endif

// for Linux
#ifdef _FEDM_LINUX
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <unistd.h>
#endif

// for Borland C++ Builder
#ifdef __BORLANDC__
	//#include <vcl.h>
	#include <windows.h>
	#include <sys\types.h>
	#include <time.h>
	//#define CString		AnsiString
#endif

#ifdef __GNUC__
	#include <ctype.h>
	#define _GNU_SOURCE 1
	#ifndef __int64
		#define __int64 long long
	#endif
#endif

// VxWorks
#if defined(_FEDM_VXWORKS)
	#ifndef __int64
		#define __int64 long long
	#endif
#endif


// FEDM version string
#define FEDM_VERSION			 "04.06.10"		// in ASCII
#define FEDM_VERSION_UC			L"04.06.10"		// in UNICODE
#define FEDM_VERSION_RC1		 4,6,10,0		// for VS recource files
#define FEDM_VERSION_RC2		 "4.6.10.0"		// for VS recource files


#define FEDM_NEW_LRU3000_ACC_CFG

// return values
#define FEDM_MODIFIED								 1
#define FEDM_OK										 0
#define FEDM_ERROR_BLOCK_SIZE						-101
#define FEDM_ERROR_BIT_BOUNDARY						-102
#define FEDM_ERROR_BYTE_BOUNDARY					-103
#define FEDM_ERROR_ARRAY_BOUNDARY					-104
#define FEDM_ERROR_BUFFER_LENGTH					-105
#define FEDM_ERROR_PARAMETER						-106
#define FEDM_ERROR_STRING_LENGTH					-107
#define FEDM_ERROR_ODD_STRING_LENGTH				-108
#define FEDM_ERROR_NO_DATA							-109
#define FEDM_ERROR_NO_READER_HANDLE					-110
#define FEDM_ERROR_NO_PORT_HANDLE					-111
#define FEDM_ERROR_UNKNOWN_CONTROL_BYTE				-112
#define FEDM_ERROR_UNKNOWN_MEM_ID					-113
#define FEDM_ERROR_UNKNOWN_POLL_MODE				-114
#define FEDM_ERROR_NO_TABLE_DATA					-115
#define FEDM_ERROR_UNKNOWN_ERROR_CODE				-116
#define FEDM_ERROR_UNKNOWN_COMMAND					-117
#define FEDM_ERROR_UNSUPPORTED						-118
#define FEDM_ERROR_NO_MORE_MEM						-119
#define FEDM_ERROR_NO_READER_FOUND					-120
#define FEDM_ERROR_NULL_POINTER						-121
#define FEDM_ERROR_UNKNOWN_READER_TYPE				-122
#define FEDM_ERROR_UNSUPPORTED_READER_TYPE			-123
#define FEDM_ERROR_UNKNOWN_TABLE_ID					-124
#define FEDM_ERROR_UNKNOWN_LANGUAGE					-125
#define FEDM_ERROR_NO_TABLE_SIZE					-126
#define FEDM_ERROR_SENDBUFFER_OVERFLOW				-127
#define FEDM_ERROR_VERIFY							-128
#define FEDM_ERROR_OPEN_FILE						-129
#define FEDM_ERROR_SAVE_FILE						-130
#define FEDM_ERROR_UNKNOWN_TRANSPONDER_TYPE			-131
#define FEDM_ERROR_READ_FILE						-132
#define FEDM_ERROR_WRITE_FILE						-133
#define FEDM_ERROR_UNKNOWN_EPC_TYPE					-134
#define FEDM_ERROR_UNSUPPORTED_PORT_DRIVER			-135
#define FEDM_ERROR_UNKNOWN_ADDRESS_MODE				-136
#define FEDM_ERROR_ALREADY_CONNECTED				-137
#define FEDM_ERROR_NOT_CONNECTED					-138
#define FEDM_ERROR_NO_MODULE_HANDLE					-139
#define FEDM_ERROR_EMPTY_MODULE_LIST				-140
#define FEDM_ERROR_MODULE_NOT_FOUND					-141
#define FEDM_ERROR_DIFFERENT_OBJECTS				-142
#define FEDM_ERROR_NOT_AN_EPC						-143
#define FEDM_ERROR_OLD_LIB_VERSION					-144
#define FEDM_ERROR_WRONG_READER_TYPE				-145
#define FEDM_ERROR_CRC								-146
#define FEDM_ERROR_CFG_BLOCK_PREVIOUSLY_NOT_READ	-147
#define FEDM_ERROR_UNSUPPORTED_CONTROLLER_TYPE		-148
#define FEDM_ERROR_VERSION_CONFLICT					-149
#define FEDM_ERROR_UNSUPPORTED_NAMESPACE			-150
#define	FEDM_ERROR_TASK_STILL_RUNNING				-151
#define	FEDM_ERROR_TAG_HANDLER_NOT_IDENTIFIED		-152
#define FEDM_ERROR_UNVALID_IDD_LENGTH				-153
#define FEDM_ERROR_UNVALID_IDD_FORMAT				-154
#define	FEDM_ERROR_UNKNOWN_TAG_HANDLER_TYPE			-155
#define FEDM_ERROR_UNSUPPORTED_TRANSPONDER_TYPE		-156
#define FEDM_ERROR_CONNECTED_WITH_OTHER_MODULE		-157
#define FEDM_ERROR_INVENTORY_NO_TID_IN_UID			-158

#define FEDM_XML_ERROR_NO_XML_FILE					-200
#define FEDM_XML_ERROR_NO_OBID_TAG					-201
#define FEDM_XML_ERROR_NO_CHILD_TAG					-202
#define FEDM_XML_ERROR_TAG_NOT_FOUND				-203
#define FEDM_XML_ERROR_DOC_NOT_WELL_FORMED			-204
#define FEDM_XML_ERROR_NO_TAG_VALUE					-205
#define FEDM_XML_ERROR_NO_TAG_ATTRIBUTE				-206
#define FEDM_XML_ERROR_DOC_FILE_VERSION				-207
#define FEDM_XML_ERROR_DOC_FILE_FAMILY				-208
#define FEDM_XML_ERROR_DOC_FILE_TYPE				-209
#define FEDM_XML_ERROR_WRONG_CONTROLLER_TYPE		-210
#define FEDM_XML_ERROR_WRONG_MEM_BANK_TYPE			-211

// Constants for Windows operation system
#define FEDM_OS_W95		1
#define FEDM_OS_W98		2
#define FEDM_OS_WME		3
#define FEDM_OS_WNT4	4
#define FEDM_OS_W2K		5
#define FEDM_OS_WCE		6


// defines for language support
#define FEDM_LANG_GERMAN	7
#define FEDM_LANG_ENGLISH	9


// defines for communication mode
#define FEDM_COMM_MODE_SERIAL		0x0000
#define FEDM_COMM_MODE_USB			0x0001
#define FEDM_COMM_MODE_TCP			0x0002
#define FEDM_COMM_MODE_BLUETOOTH	0x0003
#define FEDM_COMM_MODE_UNDEF		0x00FF


// defines for reader families
#define FEDM_RW_FAMILY		1
#define FEDM_RWA_FAMILY		2
#define FEDM_ISC_FAMILY		3
#define FEDM_TRI_FAMILY		4
#define FEDM_RO_FAMILY		5

#define FEDM_STR_RW_FAMILY			 "RW"
#define FEDM_STR_RWA_FAMILY			 "RWA"
#define FEDM_STR_ISC_FAMILY			 "ISC"
#define FEDM_STR_TRI_FAMILY			 "TRI"
#define FEDM_STR_RO_FAMILY			 "RO"

#define FEDM_STR_RW_FAMILY_UC		L"RW"
#define FEDM_STR_RWA_FAMILY_UC		L"RWA"
#define FEDM_STR_ISC_FAMILY_UC		L"ISC"
#define FEDM_STR_TRI_FAMILY_UC		L"TRI"
#define FEDM_STR_RO_FAMILY_UC		L"RO"



// defines for protocol frame support
//(SetProtocolFrameSupport)
#define FEDM_PRT_FRAME_STANDARD		    1
#define FEDM_PRT_FRAME_ADVANCED		    2

// controller types
#define FEDM_CNTRL_TYPE_FPGA			1	// RF-Decoder
#define FEDM_CNTRL_TYPE_RFC				2	// RF-Controller
#define FEDM_CNTRL_TYPE_ACC				3	// AC-Controller
#define FEDM_CNTRL_TYPE_BL				4	// Bootloader
#define FEDM_CNTRL_TYPE_KB				5	// Keyboard
#define FEDM_CNTRL_TYPE_PCC				6	// People Counter Controller
#define FEDM_CNTRL_TYPE_EXT_SCR			7	// ext. Smartcard-Reader
#define FEDM_CNTRL_TYPE_EXT_WLAN		8	// ext. WLAN-Modul
#define FEDM_CNTRL_TYPE_MLC_EMPS_LS200			0x00060011	// megalock C Lock
#define FEDM_CNTRL_TYPE_MLC_EMPS_LS300			0x00060020	// megalock C Lock
#define FEDM_CNTRL_TYPE_MLC_EMPS_LS400			0x00060021	// megalock C Lock
#define FEDM_CNTRL_TYPE_MLC_EMPS_DS200_KNOB		0x00060031	// megalock C Lock
#define FEDM_CNTRL_TYPE_MLC_EMPS_DS300_KNOB		0x00060032	// megalock C Lock
#define FEDM_CNTRL_TYPE_MLC_EMPS_DS400_KNOB		0x00060033	// megalock C Lock
#define FEDM_CNTRL_TYPE_MLC_EMPS_DS200_HANDLE	0x00060041	// megalock C Lock
#define FEDM_CNTRL_TYPE_MLC_EMPS_DS300_HANDLE	0x00060042	// megalock C Lock
#define FEDM_CNTRL_TYPE_MLC_EMPS_DS400_HANDLE	0x00060043	// megalock C Lock
#define FEDM_CNTRL_TYPE_CPR_SCRA_A_862			0x00000001	// ext. Smartcard-Reader ID CPR.SCRA-A-862

// controller sub types
#define FEDM_CNTRL_TYPE_ACC_CC7U		1
#define FEDM_CNTRL_TYPE_ACC_CC9U		2
#define FEDM_CNTRL_TYPE_ACC_FMB08		3

// memory bank types
#define FEDM_MEM_BANK_TYPE_UNDEFINED			 0x00
#define FEDM_MEM_BANK_TYPE_MAIN					 0x01
#define FEDM_MEM_BANK_TYPE_CALIBRATION_DATA		 0x20


#define FEDM_MAX_PROTOCOL_SIZE			 65536


// defines for memory types
#define FEDM_RFC_EEDATA_MEM	 3	// memory for parameters in readers EEPROM for RF-Controller
#define FEDM_RFC_RAMDATA_MEM 4	// memory for parameters in readers RAM for RF-Controller
#define FEDM_ACC_EEDATA_MEM	 5	// memory for parameters in readers EEPROM for AC-Controller
#define FEDM_ACC_RAMDATA_MEM 6	// memory for parameters in readers RAM for AC-Controller

#define FEDM_TMPDATA_MEM	10	// memory for temporary data
#define FEDM_MJPDATA_MEM	11	// memory for temporary multijob-poll data

#define FEDM_SN_MEM			20	// memory for transponders serial number and transponder type
#define FEDM_ID_MEM			21	// memory for transponders id number
#define FEDM_AC_MEM			22	// memory for transponders account data space
#define FEDM_PUB_MEM		23	// memory for transponders public data space
#define FEDM_SEC_MEM		24	// memory for transponders secret data space
#define FEDM_CONF_MEM		25	// memory for transponders configuration data space
#define FEDM_DATE_MEM		26	// memory for megaKey-s transponders date fields

#define FEDM_RFC_DATA_MASK_MEM	30	// memory for customization masks for readers EEPROM in RF-Controller
#define FEDM_ACC_DATA_MASK_MEM	31	// memory for customization masks for readers EEPROM in AC-Controller

#ifdef _FEDM_WINDOWS
	#if defined(__BORLANDC__)
	#undef _FEDM_LINUX
	#endif
   #define         FEDM_SLEEP           Sleep
   // Windows: some time constants for Sleep()
   #define    FE_1MS               1
   #define   FE_10MS              10
   #define  FE_100MS             100
   #define  FE_500MS             500
   #define FE_1000MS            1000
   #define FE_2000MS            2000
   #define FE_5000MS            5000
#endif

#ifdef _FEDM_LINUX
   #define         FEDM_SLEEP          usleep
   // Linux: some time constants for usleep()
   #define    FE_1MS           1000LL
   #define   FE_10MS          10000LL
   #define  FE_100MS         100000LL
   #define  FE_500MS         500000LL
   #define FE_1000MS        1000000LL
   #define FE_2000MS        2000000LL
   #define FE_5000MS        5000000LL
#endif

// some macros (only for internal use in FEDM)
#define FEDM_CHK1(iErr, fkt)	{ if((iErr=fkt)<0) return iErr; }
#define FEDM_CHK2(iErr, fkt)	{ if((iErr=fkt)<0) {SetLastError(iErr); return iErr;} }
#define FEDM_CHK3(var)			{ if(var==NULL) return FEDM_ERROR_NULL_POINTER; }
#define FEDM_CHK4(iErr, fkt)	{ if((iErr=fkt)!=0) {SetLastError(iErr); return iErr;} }
#define FEDM_CHK5(var)			{ if(var==NULL) {SetLastError(FEDM_ERROR_NULL_POINTER); return FEDM_ERROR_NULL_POINTER;} }
#define FEDM_CHK6(iErr, fkt)	{ if((iErr=fkt)<0) return; }
#define FEDM_CHK7(iErr, fkt)	{ if((iErr=fkt)<0) {SetLastError(iErr); return NULL;} }
#define FEDM_CHK8(var)			{ if(var<=0) {SetLastError(FEDM_ERROR_STRING_LENGTH); return FEDM_ERROR_STRING_LENGTH;} }

#define FEDM_RETURN(iErr)		{ SetLastError(iErr); return iErr; }

#define FEDM_IS_COMPORT(iPortHnd)	(((iPortHnd&0xF0000000)==0)&&(iPortHnd>0))
#define FEDM_IS_USBPORT(iPortHnd)	((iPortHnd&0x10000000)&&(iPortHnd>0))
#define FEDM_IS_TCPPORT(iPortHnd)	((iPortHnd&0x20000000)&&(iPortHnd>0))
#define FEDM_IS_PIPORT(iPortHnd)	((iPortHnd&0x70000000)&&(iPortHnd>0))

#define FEDM_IS_RDR_MODULE(iHnd)	(iHnd&0x10000000)
#define FEDM_IS_FU_MODULE(iHnd)		(iHnd&0x20000000)

#if defined(_FEDM_SUPPORT_UCLINUX)
#define FEDM_SWSCANF(in, out)	{ for (int i = 0; i < sizeof(in); i++) out[i] = (char)(in)[i];}
#else
#define FEDM_SWSCANF(in, out)	{ int i; for (i = 0; i < (int)(in.length()); i++) {out[i] = (char)(in.c_str())[i];} out[i] = '\0';}
#endif

#define FEDM_SWPRINTF(in, out)	{ int i; for (i = 0; i < (int)(in.length()); i++) {out[i] = (wchar_t)(in.c_str())[i];} out[i] = '\0';}

#if defined(_FEDM_LINUX)
	//#define FEDM_STRUPR(str, len)	{strupr(str);}
	#define FEDM_STRUPR(str, len) {char* s = str; for(unsigned int i=0; i<len; ++i, s++) *s = toupper(*s);}
#else
	#if _MSC_VER <= 12
		#define FEDM_STRUPR(str, len)	{strupr(str);}
	#else
		#define FEDM_STRUPR(str, len)	{_strupr_s(str, len);}
	#endif
#endif

#endif // _FEDM_H_INCLUDED_
