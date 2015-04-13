/*-------------------------------------------------------
|                                                       |
|                    FEDM_Functions.h                   |
|                                                       |
---------------------------------------------------------

Copyright  2000-2013	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	18.05.2000
Version       		:	04.05.00 / 26.04.2013 / M. Hultsch

Operation Systems	:	independent

Function			:	collects all global functions for FEDM class library


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#if !defined(_FEDM_FUNCTIONS_H_INCLUDED_)
#define _FEDM_FUNCTIONS_H_INCLUDED_


#ifdef _MSC_VER
	// The (microsoft) debugger can't handle symbols more than 255 characters long.
	// STL often creates symbols longer than that.
	// When symbols are longer than 255 characters, the warning is disabled.
	#pragma warning(disable:4786)	// for MFC
#endif



#include <string>
#include "FEDM.h"

// STL - Standard Template Library
using namespace std;


// calculation functions
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_GetMemIDOfID(const char* ID);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_GetAdrOfID(const char* ID, int BlockSize);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_GetBlockNoOfID(const char* ID);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_GetByteCntOfID(const char* ID);

// modification functions
_FEDM_ISC_CORE_EXT_FUNCT char*		FEDM_ToRAM(const char* ID);
_FEDM_ISC_CORE_EXT_FUNCT char*		FEDM_ToMask(const char* ID);
_FEDM_ISC_CORE_EXT_FUNCT char*		FEDM_MdfyMemID(const char* ID, int iNewMemID);
_FEDM_ISC_CORE_EXT_FUNCT char*		FEDM_MdfyBlockNr(const char* ID, int iBlockNr);
_FEDM_ISC_CORE_EXT_FUNCT char*		FEDM_AddOff2BlockNr(const char* ID, int iOffset);

_FEDM_ISC_CORE_EXT_FUNCT unsigned int	FEDM_SwapByteOrder(unsigned int uiValue);
_FEDM_ISC_CORE_EXT_FUNCT __int64		FEDM_SwapByteOrder(__int64 i64Value);

// data type conversion functions
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvHexStrToInt(CString sInStr, int* iOut);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvHexStrToUInt(CString sInStr, unsigned int* uiOut);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvHexStrToInt64(CString sInStr, __int64* i64Out);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvHexStrToUInt64(CString sInStr, unsigned __int64* ui64Out);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvHexStrToHexUChar(CString sInStr, unsigned char* ucOutBuf, int iOutBufLen);
#endif

_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvHexStrToInt(string sInStr, int* iOut);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvHexStrToUInt(string sInStr, unsigned int* uiOut);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvHexStrToInt64(string sInStr, __int64* i64Out);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvHexStrToUInt64(string sInStr, unsigned __int64* ui64Out);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvHexStrToHexUChar(string sInStr, unsigned char* ucOutBuf, int iOutBufLen);

_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvHexUCharToInt(unsigned char* ucInBuf, int iBufLen, int* iOut);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvHexUCharToUInt(unsigned char* ucInBuf, int iBufLen, unsigned int* uiOut);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvHexUCharToInt64(unsigned char* ucInBuf, int iBufLen, __int64* i64Out);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvHexUCharToUInt64(unsigned char* ucInBuf, int iBufLen, unsigned __int64* ui64Out);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvHexUCharToHexChar(unsigned char* ucInBuf, int iBufLen, char* cOut, int iOutBufLen);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvHexUCharToHexStr(unsigned char* ucInBuf, int iBufLen, string& sOut);
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvHexUCharToHexStr(unsigned char* ucInBuf, int iBufLen, CString& sOut);
#endif

_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvHexCharToInt(char* cInBuf, int iBufLen, int* iOut);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvHexCharToUInt(char* cInBuf, int iBufLen, unsigned int* uiOut);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvHexCharToInt64(char* cInBuf, int iBufLen, __int64* i64Out);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvHexCharToUInt64(char* cInBuf, int iBufLen, unsigned __int64* ui64Out);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvHexCharToHexUChar(char* cInBuf, int iBufLen, unsigned char* ucOutBuf, int iOutBufLen);

#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvIntToHexStr(int iIn, CString& sOut);
#endif
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvIntToHexStr(int iIn, string& sOut);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvIntToHexChar(int iIn, char* cOutBuf, int iOutBufLen);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvIntToHexUChar(int iIn, unsigned char* ucOutBuf, int iOutBufLen);

#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvUIntToHexStr(unsigned int uiIn, CString& sOut);
#endif
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvUIntToHexStr(unsigned int uiIn, string& sOut);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvUIntToHexChar(unsigned int uiIn, char* uOutBuf, int iOutBufLen);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvUIntToHexUChar(unsigned int uiIn, unsigned char* ucOutBuf, int iOutBufLen);

#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvInt64ToHexStr(__int64 i64In, CString& sOut);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvUInt64ToHexStr(unsigned __int64 ui64In, CString& sOut);
#endif
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvInt64ToHexStr(__int64 i64In, string& sOut);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvUInt64ToHexStr(unsigned __int64 ui64In, string& sOut);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvInt64ToHexChar(__int64 i64In, char* cOutBuf, int iOutBufLen);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvUInt64ToHexChar(unsigned __int64 ui64In, char* cOutBuf, int iOutBufLen);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvInt64ToHexUChar(__int64 i64In, unsigned char* ucOutBuf, int iOutBufLen);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvUInt64ToHexUChar(unsigned __int64 ui64In, unsigned char* ucOutBuf, int iOutBufLen);

_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvAsciiCharToHexUChar(unsigned char* ucInBuf, int iInBufLen, unsigned char* ucOutBuf, int iOutBufLen);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvBcdCharToHexUChar(unsigned char* ucInBuf, int iInBufLen, unsigned char* ucOutBuf, int iOutBufLen);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_ConvHexUCharToBcdChar(unsigned char* ucInBuf, int iInBufLen, unsigned char* ucOutBuf, int iOutBufLen);


// very helpful functions
_FEDM_ISC_CORE_EXT_FUNCT unsigned char FEDM_ConvTwoAsciiToUChar(unsigned char uc0, unsigned char uc1);
_FEDM_ISC_CORE_EXT_FUNCT void		FEDM_ConvHexUCharToTwoAscii(unsigned char ucIn, unsigned char& ucOut1, unsigned char& ucOut2);

#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_RemNoHexChar(CString sIn, unsigned char* ucOutBuf, int iOutBufLen);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_RemNoHexChar(CString sIn, CString& sOut);
_FEDM_ISC_CORE_EXT_FUNCT CString	FEDM_RemNoHexChar(CString sIn);
_FEDM_ISC_CORE_EXT_FUNCT bool		FEDM_IsHex(CString sIn);
#endif

_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_RemNoHexChar(string sIn, unsigned char* ucOutBuf, int iOutBufLen);
_FEDM_ISC_CORE_EXT_FUNCT int		FEDM_RemNoHexChar(string sIn, string& sOut);
_FEDM_ISC_CORE_EXT_FUNCT bool		FEDM_IsHex(string sIn);

_FEDM_ISC_CORE_EXT_FUNCT __int64	FEDM_GetUTCTimestamp();

#undef DLL_EXT_FUNC

#endif // #if !defined(_FEDM_FUNCTIONS_H_INCLUDED_)

