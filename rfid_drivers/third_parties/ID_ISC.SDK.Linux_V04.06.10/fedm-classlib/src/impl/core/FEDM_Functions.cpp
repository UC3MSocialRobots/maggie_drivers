/*-------------------------------------------------------
|                                                       |
|                    FEDM_Functions.cpp                 |
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
						- new functions: FEDM_ConvBcdCharToHexUChar, FEDM_ConvHexUCharToBcdChar

						03.02.04 / 10.08.2010 / M. Hultsch

Operation Systems	:	independent

Function			:	collects all global functions for FEDM class library


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#include <math.h>
#include <stdio.h>
#if !defined(_WIN32_WCE)
#include <sys/timeb.h>
#endif
#include <time.h>
#include "FEDM.h"
#include "FEDM_Functions.h"

#if defined(_FEDM_LINUX)
	#include <sys/time.h>
	#include <string.h>
#endif

#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//#####################################################################################
// calculation functions
//#####################################################################################


/***************************************************************************
  Begin        	:	18.05.2000 / M. Hultsch
  Version       :	03.00.00 / 27.11.2007 / M. Hultsch

  Function		:	extractes the memory-ID from ID

  Parameters	:	const char* ID		-	data identifier

  Return Value	:	int					-	memory-ID

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int FEDM_GetMemIDOfID(const char* ID)
{
	FEDM_CHK3(ID);

	int FamID=0, MemID=0, Block=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	
	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &Block, &BytePos, &ByteCnt, &BitPos, &BitCnt );
	
	return MemID;
}


/***************************************************************************
  Begin        	:	10.08.2010 / M. Hultsch
  Version       :	03.02.04 / 10.08.2010 / M. Hultsch

  Function		:	extractes the start block number from ID

  Parameters	:	const char* ID		-	data identifier

  Return Value	:	int					-	block number

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int FEDM_GetBlockNoOfID(const char* ID)
{
	FEDM_CHK3(ID);

	int FamID=0, MemID=0, Block=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	
	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &Block, &BytePos, &ByteCnt, &BitPos, &BitCnt );
	
	return Block;
}

/***************************************************************************
  Begin        	:	18.05.2000 / M. Hultsch
  Version       :	03.00.00 / 27.11.2007 / M. Hultsch

  Function		:	calculates the start address from ID

  Parameters	:	const char* ID		-	data identifier

  Return Value	:	int					-	address or error code

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int FEDM_GetAdrOfID(const char* ID, int BlockSize)
{
	FEDM_CHK3(ID);

	int FamID=0, MemID=0, Block=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	
	// security check
	if( (BlockSize!=4) && (BlockSize!=16) && (BlockSize!=32) )
		return FEDM_ERROR_BLOCK_SIZE;

	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &Block, &BytePos, &ByteCnt, &BitPos, &BitCnt );
	
	return Block * BlockSize + BytePos;
}

/***************************************************************************
  Begin        	:	18.05.2000 / M. Hultsch
  Version       :	03.00.00 / 27.11.2007 / M. Hultsch

  Function		:	extracts the count of bytes from ID

  Parameters	:	const char* ID		-	data identifier

  Return Value	:	int					-	count of bytes or error code

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int FEDM_GetByteCntOfID(const char* ID)
{
	FEDM_CHK3(ID);

	int FamID=0, MemID=0, Block=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;

	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &Block, &BytePos, &ByteCnt, &BitPos, &BitCnt );

	return ByteCnt;
}



//#####################################################################################
// MemID modification functions
//#####################################################################################


/***************************************************************************
  Begin        	:	30.03.2001 / M. Hultsch
  Version       :	03.00.00 / 20.08.2008 / M. Hultsch

  Function		:	changes MemID in ID from FEDM_xxx_EEDATA_MEM to FEDM_xxx_RAMDATA_MEM

  Parameters	:	const char* ID		-	data identifier

  Return Value	:	char* ID			-	Pointer to new ID or "00 00 00 00 00 00 00"

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT char* FEDM_ToRAM(const char* ID)
{
	static char cID[32];

	if(ID==NULL)
	{
		strcpy(cID, "00 00 00 00 00 00 00");
		return cID;
	}

	int FamID=0, MemID=0, Block=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;

	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &Block, &BytePos, &ByteCnt, &BitPos, &BitCnt );

	switch(MemID)
	{
	case FEDM_RFC_EEDATA_MEM:
		sprintf(cID,
				"%02d %02d %02d %02d %02d %02d %02d",
				FamID, FEDM_RFC_RAMDATA_MEM, Block, BytePos, ByteCnt, BitPos, BitCnt);
		break;

	case FEDM_ACC_EEDATA_MEM:
		sprintf(cID,
				"%02d %02d %02d %02d %02d %02d %02d",
				FamID, FEDM_ACC_RAMDATA_MEM, Block, BytePos, ByteCnt, BitPos, BitCnt);
		break;
	}

	return cID;
}


/***************************************************************************
  Begin        	:	16.08.2007 / M. Hultsch
  Version       :	03.00.00 / 20.08.2008 / M. Hultsch

  Function		:	changes MemID in ID from FEDM_xxx_EEDATA_MEM to FEDM_xxx_DATA_MASK_MEM

  Parameters	:	const char* ID		-	data identifier

  Return Value	:	char* ID			-	Pointer to new ID or "00 00 00 00 00 00 00"

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT char* FEDM_ToMask(const char* ID)
{
	static char cID[32];

	if(ID==NULL)
	{
		strcpy(cID, "00 00 00 00 00 00 00");
		return cID;
	}

	int FamID=0, MemID=0, Block=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;

	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &Block, &BytePos, &ByteCnt, &BitPos, &BitCnt );


	switch(MemID)
	{
	case FEDM_RFC_EEDATA_MEM:
		sprintf(cID,
				"%02d %02d %02d %02d %02d %02d %02d",
				FamID, FEDM_RFC_DATA_MASK_MEM, Block, BytePos, ByteCnt, BitPos, BitCnt);
		break;

	case FEDM_ACC_EEDATA_MEM:
		sprintf(cID,
				"%02d %02d %02d %02d %02d %02d %02d",
				FamID, FEDM_ACC_DATA_MASK_MEM, Block, BytePos, ByteCnt, BitPos, BitCnt);
		break;
	}

	return cID;
}


/***************************************************************************
  Begin        	:	12.07.2001 / M. Hultsch
  Version       :	03.00.00 / 27.11.2007 / M. Hultsch

  Function		:	changes MemID in ID

  Parameters	:	const char* ID		-	data identifier
					int iNewMemID		-	new memory identifier
					
  Return Value	:	char* ID			-	Pointer to new ID or "00 00 00 00 00 00 00"

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT char* FEDM_MdfyMemID(const char* ID, int iNewMemID)
{
	static char cID[32];

	if(ID==NULL)
	{
		strcpy(cID, "00 00 00 00 00 00 00");
		return cID;
	}

	int FamID=0, MemID=0, Block=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;

	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &Block, &BytePos, &ByteCnt, &BitPos, &BitCnt );

	sprintf(cID, "%02d %02d %02d %02d %02d %02d %02d",
			FamID, iNewMemID, Block, BytePos, ByteCnt, BitPos, BitCnt);

	return cID;
}

/***************************************************************************
  Begin        	:	20.04.2001 / M. Hultsch
  Version       :	03.00.00 / 27.11.2007 / M. Hultsch

  Function		:	changes block number in ID

  Parameters	:	const char* ID		-	data identifier

  Return Value	:	char* ID			-	Pointer to new ID or "00 00 00 00 00 00 00"

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT char* FEDM_MdfyBlockNr(const char* ID, int iBlockNr)
{
	static char cID[32];

	if(ID==NULL)
	{
		strcpy(cID, "00 00 00 00 00 00 00");
		return cID;
	}

	int FamID=0, MemID=0, Block=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;

	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &Block, &BytePos, &ByteCnt, &BitPos, &BitCnt );

	sprintf(cID,
			"%02d %02d %02d %02d %02d %02d %02d",
			FamID, MemID, iBlockNr, BytePos, ByteCnt, BitPos, BitCnt);

	return cID;
}

/***************************************************************************
  Begin        	:	19.04.2002 / M. Hultsch
  Version       :	03.00.00 / 27.11.2007 / M. Hultsch

  Function		:	adds offset to block number in ID

  Parameters	:	const char* ID		-	data identifier

  Return Value	:	char* ID			-	Pointer to new ID or "00 00 00 00 00 00 00"

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT char* FEDM_AddOff2BlockNr(const char* ID, int iOffset)
{
	static char cID[32];

	if(ID==NULL)
	{
		strcpy(cID, "00 00 00 00 00 00 00");
		return cID;
	}

	int FamID=0, MemID=0, Block=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;

	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &Block, &BytePos, &ByteCnt, &BitPos, &BitCnt );

	sprintf(cID,
			"%02d %02d %02d %02d %02d %02d %02d",
			FamID, MemID, Block+iOffset, BytePos, ByteCnt, BitPos, BitCnt);

	return cID;
}


/***************************************************************************
  Begin        	:	31.05.2012 / M. Hultsch
  Version       :	04.02.01 / 31.05.2012 / M. Hultsch

  Function		:	swaps byte order

  Parameters	:	unsigned int value

  Return Value	:	unsigned int - value with swapped byte order
***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT unsigned int FEDM_SwapByteOrder(unsigned int uiValue)
{
	unsigned int uiTmp = ((uiValue & 0xFF000000) >> 24);
	uiTmp |= ((uiValue & 0x00FF0000) >>  8);
	uiTmp |= ((uiValue & 0x0000FF00) <<  8);
	uiTmp |= ((uiValue & 0x000000FF) << 24);

	return uiTmp;
}

/***************************************************************************
  Begin        	:	31.05.2012 / M. Hultsch
  Version       :	04.02.01 / 31.05.2012 / M. Hultsch

  Function		:	swaps byte order

  Parameters	:	__int64 value

  Return Value	:	__int64 - value with swapped byte order
***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT __int64 FEDM_SwapByteOrder(__int64 i64Value)
{
	__int64 i64Tmp = 0;
#if _MSC_VER <= 1200 && !defined(__GNUC__)
	i64Tmp  = ((i64Value & 0xFF00000000000000) >> 56);
	i64Tmp |= ((i64Value & 0x00FF000000000000) >> 40);
	i64Tmp |= ((i64Value & 0x0000FF0000000000) >> 24);
	i64Tmp |= ((i64Value & 0x000000FF00000000) >>  8);
	i64Tmp |= ((i64Value & 0x00000000FF000000) <<  8);
	i64Tmp |= ((i64Value & 0x0000000000FF0000) << 24);
	i64Tmp |= ((i64Value & 0x000000000000FF00) << 40);
	i64Tmp |= ((i64Value & 0x00000000000000FF) << 56);
#else
	i64Tmp  = ((i64Value & 0xFF00000000000000ULL) >> 56);
	i64Tmp |= ((i64Value & 0x00FF000000000000ULL) >> 40);
	i64Tmp |= ((i64Value & 0x0000FF0000000000ULL) >> 24);
	i64Tmp |= ((i64Value & 0x000000FF00000000ULL) >>  8);
	i64Tmp |= ((i64Value & 0x00000000FF000000ULL) <<  8);
	i64Tmp |= ((i64Value & 0x0000000000FF0000ULL) << 24);
	i64Tmp |= ((i64Value & 0x000000000000FF00ULL) << 40);
	i64Tmp |= ((i64Value & 0x00000000000000FFULL) << 56);
#endif
	return i64Tmp;
}


//#####################################################################################
// data type conversion functions
// 
// HexStr -> different types
//#####################################################################################


/***************************************************************************
  Begin        	:	20.11.2000 / M. Hultsch
  Version       :	01.03.00 / 19.06.2001 / M. Hultsch

  Function		:	converts hex-string (CString) in int
					all chars except "0..9, a..f, A..F" will be removed

					attention: max. 8 chars in sIn

					example:
					sIn  = "1122F05E"
					iOut = 287502430

  Parameters	:	CString sIn		-	input string
					int* iOut		-	pointer to data member

  Return Value	:	int				-	OK (0) or error (<0)

***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvHexStrToInt(CString sIn, int* iOut)
{
	FEDM_CHK3(iOut);

	int		iByteCnt;
#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
	int		iTmpBufLen	= sIn.GetLength();
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
	int		iTmpBufLen	= sIn.Length();
#endif
	unsigned char	*ucTmpBuf;

	if (iTmpBufLen == 0)
		return FEDM_ERROR_STRING_LENGTH;

	if ((iTmpBufLen%2) != 0)
		return FEDM_ERROR_ODD_STRING_LENGTH;

	ucTmpBuf = new unsigned char[iTmpBufLen/2];

	if(ucTmpBuf == NULL)
		return FEDM_ERROR_NO_MORE_MEM;

	int	iLen = FEDM_ConvHexStrToHexUChar(sIn, ucTmpBuf, iTmpBufLen/2);

	if(iLen <= 0 || iLen > 4)
	{
		delete[] ucTmpBuf;
		return FEDM_ERROR_BUFFER_LENGTH;
	}

	*iOut = 0;

	for(iByteCnt=0; iByteCnt<iLen; iByteCnt++)
		// hier wird Motorola zu Intel geschoben
		*iOut += ( (int)(*(ucTmpBuf+(iLen-1)-iByteCnt)) << (8*iByteCnt) );

	delete[] ucTmpBuf;

	return FEDM_OK;
}
#endif


/***************************************************************************
  Begin        	:	30.04.2002 / M. Hultsch
  Version       :	01.05.00 / 30.04.2002 / M. Hultsch

  Function		:	converts hex-string (STL-string) in int
					all chars except "0..9, a..f, A..F" will be removed

					attention: max. 8 chars in sIn

					example:
					sIn  = "1122F05E"
					iOut = 287502430

  Parameters	:	string sIn		-	input string
					int* iOut		-	pointer to data member

  Return Value	:	int				-	OK (0) or error (<0)

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvHexStrToInt(string sIn, int* iOut)
{
	FEDM_CHK3(iOut);

	int		iByteCnt;
	size_t	iTmpBufLen	= sIn.length();
	unsigned char	*ucTmpBuf;

	if (iTmpBufLen == 0)
		return FEDM_ERROR_STRING_LENGTH;

	if ((iTmpBufLen%2) != 0)
		return FEDM_ERROR_ODD_STRING_LENGTH;

	ucTmpBuf = new unsigned char[iTmpBufLen/2];

	if(ucTmpBuf == NULL)
		return FEDM_ERROR_NO_MORE_MEM;

	int	iLen = FEDM_ConvHexStrToHexUChar(sIn, ucTmpBuf, (int)iTmpBufLen/2);

	if(iLen <= 0 || iLen > 4)
	{
		delete[] ucTmpBuf;
		return FEDM_ERROR_BUFFER_LENGTH;
	}

	*iOut = 0;

	for(iByteCnt=0; iByteCnt<iLen; iByteCnt++)
		// hier wird Motorola zu Intel geschoben
		*iOut += ( (int)(*(ucTmpBuf+(iLen-1)-iByteCnt)) << (8*iByteCnt) );

	delete[] ucTmpBuf;

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	20.11.2000 / M. Hultsch
  Version       :	01.00.01 / 10.12.2000 / M. Hultsch

  Function		:	converts hex-string (CString) in unsigned int
					all chars except "0..9, a..f, A..F" will be removed

					attention: max. 8 chars in sIn

					example:
					sIn   = "1122F05E"
					uiOut = 287502430

  Parameters	:	CString sIn		-	input string
					unsigned int* uiOut		-	pointer to data member

  Return Value	:	int				-	OK (0) or error (<0)

***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvHexStrToUInt(CString sIn, unsigned int* uiOut)
{
	FEDM_CHK3(uiOut);

	int iOut = 0;

	int	iBack = FEDM_ConvHexStrToInt(sIn, &iOut);
	
	if(iBack < 0)
		return iBack;

	*uiOut = (unsigned int)iOut;

	return FEDM_OK;
}
#endif

/***************************************************************************
  Begin        	:	30.04.2002 / M. Hultsch
  Version       :	01.05.00 / 30.04.2004 / M. Hultsch

  Function		:	converts hex-string (STL-string) in unsigned int
					all chars except "0..9, a..f, A..F" will be removed

					attention: max. 8 chars in sIn

					example:
					sIn   = "1122F05E"
					uiOut = 287502430

  Parameters	:	string sIn		-	input string
					unsigned int* uiOut		-	pointer to data member

  Return Value	:	int				-	OK (0) or error (<0)

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvHexStrToUInt(string sIn, unsigned int* uiOut)
{
	FEDM_CHK3(uiOut);

	int iOut = 0;

	int	iBack = FEDM_ConvHexStrToInt(sIn, &iOut);
	
	if(iBack < 0)
		return iBack;

	*uiOut = (unsigned int)iOut;

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	20.11.2000 / M. Hultsch
  Version       :	01.03.00 / 19.06.2001 / M. Hultsch

  Function		:	converts hex-string (CString) in __int64
					all chars except "0..9, a..f, A..F" will be removed

					attention: max. 16 chars in sIn

					example:
					sIn		= "1122F05E1122F05E"
					i64Out	= 1234813534658031710

  Parameters	:	CString sIn		-	input string
					__int64* i64Out	-	pointer to data member

  Return Value	:	int				-	OK (0) or error (<0)

***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvHexStrToInt64(CString sIn, __int64* i64Out)
{
	FEDM_CHK3(i64Out);

	int		iByteCnt;
#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
	int		iTmpBufLen	= sIn.GetLength();
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
	int		iTmpBufLen	= sIn.Length();
#endif
	unsigned char	*ucTmpBuf;

	if (iTmpBufLen == 0)
		return FEDM_ERROR_STRING_LENGTH;

	if ((iTmpBufLen%2) != 0)
		return FEDM_ERROR_ODD_STRING_LENGTH;

	ucTmpBuf = new unsigned char[iTmpBufLen/2];

	if(ucTmpBuf == NULL)
		return FEDM_ERROR_NO_MORE_MEM;

	int	iLen = FEDM_ConvHexStrToHexUChar(sIn, ucTmpBuf, iTmpBufLen/2);

	if(iLen <= 0 || iLen > 8)
	{
		delete[] ucTmpBuf;
		return FEDM_ERROR_BUFFER_LENGTH;
	}

	*i64Out = 0;

	for(iByteCnt=0; iByteCnt<iLen; iByteCnt++)
		// hier wird Motorola zu Intel geschoben
		*i64Out += ( (__int64)(*(ucTmpBuf+(iLen-1)-iByteCnt)) << (8*iByteCnt) );

	delete[] ucTmpBuf;

	return FEDM_OK;
}
#endif

/***************************************************************************
  Begin        	:	17.03.2010 / M. Hultsch
  Version       :	03.01.01 / 17.03.2010 / M. Hultsch

  Function		:	converts hex-string (CString) in unsigned __int64
					all chars except "0..9, a..f, A..F" will be removed

					attention: max. 16 chars in sIn

					example:
					sIn		= "1122F05E1122F05E"
					ui64Out	= 1234813534658031710

  Parameters	:	CString sIn					-	input string
					unsigned __int64* ui64Out	-	pointer to data member

  Return Value	:	int				-	OK (0) or error (<0)

***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvHexStrToUInt64(CString sIn, unsigned __int64* ui64Out)
{
	FEDM_CHK3(ui64Out);

	int		iByteCnt;
#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
	int		iTmpBufLen	= sIn.GetLength();
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
	int		iTmpBufLen	= sIn.Length();
#endif
	unsigned char	*ucTmpBuf;

	if (iTmpBufLen == 0)
		return FEDM_ERROR_STRING_LENGTH;

	if ((iTmpBufLen%2) != 0)
		return FEDM_ERROR_ODD_STRING_LENGTH;

	ucTmpBuf = new unsigned char[iTmpBufLen/2];

	if(ucTmpBuf == NULL)
		return FEDM_ERROR_NO_MORE_MEM;

	int	iLen = FEDM_ConvHexStrToHexUChar(sIn, ucTmpBuf, iTmpBufLen/2);

	if(iLen <= 0 || iLen > 8)
	{
		delete[] ucTmpBuf;
		return FEDM_ERROR_BUFFER_LENGTH;
	}

	*ui64Out = 0;

	for(iByteCnt=0; iByteCnt<iLen; iByteCnt++)
		// hier wird Motorola zu Intel geschoben
		*ui64Out += ( (unsigned __int64)(*(ucTmpBuf+(iLen-1)-iByteCnt)) << (8*iByteCnt) );

	delete[] ucTmpBuf;

	return FEDM_OK;
}
#endif

/***************************************************************************
  Begin        	:	30.04.2002 / M. Hultsch
  Version       :	01.05.00 / 30.04.2002 / M. Hultsch

  Function		:	converts hex-string (STL-string) in __int64
					all chars except "0..9, a..f, A..F" will be removed

					attention: max. 16 chars in sIn

					example:
					sIn		= "1122F05E1122F05E"
					i64Out	= 1234813534658031710

  Parameters	:	string sIn		-	input string
					__int64* i64Out	-	pointer to data member

  Return Value	:	int				-	OK (0) or error (<0)

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvHexStrToInt64(string sIn, __int64* i64Out)
{
	FEDM_CHK3(i64Out);

	int		iByteCnt;
	size_t	iTmpBufLen	= sIn.length();
	unsigned char	*ucTmpBuf;

	if (iTmpBufLen == 0)
		return FEDM_ERROR_STRING_LENGTH;

	if ((iTmpBufLen%2) != 0)
		return FEDM_ERROR_ODD_STRING_LENGTH;

	ucTmpBuf = new unsigned char[iTmpBufLen/2];

	if(ucTmpBuf == NULL)
		return FEDM_ERROR_NO_MORE_MEM;

	int	iLen = FEDM_ConvHexStrToHexUChar(sIn, ucTmpBuf, (int)iTmpBufLen/2);

	if(iLen <= 0 || iLen > 8)
	{
		delete[] ucTmpBuf;
		return FEDM_ERROR_BUFFER_LENGTH;
	}

	*i64Out = 0;

	for(iByteCnt=0; iByteCnt<iLen; iByteCnt++)
		// hier wird Motorola zu Intel geschoben
		*i64Out += ( (__int64)(*(ucTmpBuf+(iLen-1)-iByteCnt)) << (8*iByteCnt) );

	delete[] ucTmpBuf;

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	17.03.2010 / M. Hultsch
  Version       :	03.01.01 / 17.03.2010 / M. Hultsch

  Function		:	converts hex-string (STL-string) in unsigned __int64
					all chars except "0..9, a..f, A..F" will be removed

					attention: max. 16 chars in sIn

					example:
					sIn		= "1122F05E1122F05E"
					ui64Out	= 1234813534658031710

  Parameters	:	string sIn					-	input string
					unsigned __int64* ui64Out	-	pointer to data member

  Return Value	:	int				-	OK (0) or error (<0)

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvHexStrToUInt64(string sIn, unsigned __int64* ui64Out)
{
	FEDM_CHK3(ui64Out);

	int		iByteCnt;
	size_t	iTmpBufLen	= sIn.length();
	unsigned char	*ucTmpBuf;

	if (iTmpBufLen == 0)
		return FEDM_ERROR_STRING_LENGTH;

	if ((iTmpBufLen%2) != 0)
		return FEDM_ERROR_ODD_STRING_LENGTH;

	ucTmpBuf = new unsigned char[iTmpBufLen/2];

	if(ucTmpBuf == NULL)
		return FEDM_ERROR_NO_MORE_MEM;

	int	iLen = FEDM_ConvHexStrToHexUChar(sIn, ucTmpBuf, (int)iTmpBufLen/2);

	if(iLen <= 0 || iLen > 8)
	{
		delete[] ucTmpBuf;
		return FEDM_ERROR_BUFFER_LENGTH;
	}

	*ui64Out = 0;

	for(iByteCnt=0; iByteCnt<iLen; iByteCnt++)
		// hier wird Motorola zu Intel geschoben
		*ui64Out += ( (unsigned __int64)(*(ucTmpBuf+(iLen-1)-iByteCnt)) << (8*iByteCnt) );

	delete[] ucTmpBuf;

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	18.05.2000 / M. Hultsch
  Version       :	01.00.01 / 10.12.2000 / M. Hultsch

  Function		:	converts hex-string (CString) in hex-string (unsigned char)
					example: sIn = "1122F05E" --> ucOutBuf = {0x11, 0x22, 0xF0, 0x5E}
					all chars except "0..9, a..f, A..F" will be removed
					
					attention: no null-termination

  Parameters	:	CString sIn			-	input string
					unsigned char* ucOutBuf		-	pointer to output buffer
					int iOutBufLen		-	size of output buffer

  Return Value	:	int					-	count of bytes in output buffer or error code

***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvHexStrToHexUChar(CString sIn, unsigned char* ucOutBuf, int iOutBufLen)
{
	FEDM_CHK3(ucOutBuf);

	int		i;
	int		iCnt	= 0;
#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
	int		iInLen	= sIn.GetLength();
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
	int		iInLen	= sIn.Length();
#endif
	int		iOutLen = 0;
	unsigned char	*ucTmpBuf;


	if (iInLen == 0)
		return FEDM_ERROR_STRING_LENGTH;

	ucTmpBuf = new unsigned char[iInLen];

	if(ucTmpBuf == NULL)
		return FEDM_ERROR_NO_MORE_MEM;

	// remove all non-hex chars, copy string (CString) in temp. string (unsigned char)
	iCnt = FEDM_RemNoHexChar(sIn, ucTmpBuf, iInLen);
	
	if ((iCnt%2) != 0)
	{
		delete[] ucTmpBuf;
		return FEDM_ERROR_ODD_STRING_LENGTH;
	}
	
	if ((iCnt / 2) > iOutBufLen)
	{
		delete[] ucTmpBuf;
		return FEDM_ERROR_BUFFER_LENGTH;
	}

	// convert every two chars to one hex-char
	for (i = 0; i < iCnt; i += 2)
	{
		ucOutBuf[i / 2] = FEDM_ConvTwoAsciiToUChar(ucTmpBuf[i], ucTmpBuf[i+1]);
		iOutLen++;
	}

	delete[] ucTmpBuf;

	return iOutLen;
}
#endif

/***************************************************************************
  Begin        	:	30.04.2002 / M. Hultsch
  Version       :	01.05.00 / 30.04.2002 / M. Hultsch

  Function		:	converts hex-string (STL-string) in hex-string (unsigned char)
					example: sIn = "1122F05E" --> ucOutBuf = {0x11, 0x22, 0xF0, 0x5E}
					all chars except "0..9, a..f, A..F" will be removed
					
					attention: no null-termination

  Parameters	:	string sIn			-	input string
					unsigned char* ucOutBuf		-	pointer to output buffer
					int iOutBufLen		-	size of output buffer

  Return Value	:	int					-	count of bytes in output buffer or error code

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvHexStrToHexUChar(string sIn, unsigned char* ucOutBuf, int iOutBufLen)
{
	FEDM_CHK3(ucOutBuf);

	int		i;
	int		iCnt	= 0;
	int		iInLen	= (int)sIn.length();
	int		iOutLen = 0;
	unsigned char	*ucTmpBuf;


	if (iInLen == 0)
		return FEDM_ERROR_STRING_LENGTH;

	ucTmpBuf = new unsigned char[iInLen];

	if(ucTmpBuf == NULL)
		return FEDM_ERROR_NO_MORE_MEM;

	// remove all non-hex chars, copy string (STL-string) in temp. string (unsigned char)
	iCnt = FEDM_RemNoHexChar(sIn, ucTmpBuf, iInLen);
	
	if ((iCnt%2) != 0)
	{
		delete[] ucTmpBuf;
		return FEDM_ERROR_ODD_STRING_LENGTH;
	}
	
	if ((iCnt / 2) > iOutBufLen)
	{
		delete[] ucTmpBuf;
		return FEDM_ERROR_BUFFER_LENGTH;
	}

	// convert every two chars to one hex-char
	for (i = 0; i < iCnt; i += 2)
	{
		ucOutBuf[i / 2] = FEDM_ConvTwoAsciiToUChar(ucTmpBuf[i], ucTmpBuf[i+1]);
		iOutLen++;
	}

	delete[] ucTmpBuf;

	return iOutLen;
}


//#####################################################################################
// data type conversion functions
// 
// HexUChar -> different types
//#####################################################################################

/***************************************************************************
  Begin        	:	20.11.2000 / M. Hultsch
  Version       :	01.00.01 / 10.12.2000 / M. Hultsch

  Function		:	converts hex-string (unsigned char) in int

					attention: max. 4 bytes

					example:
					ucInBuf		= {0x11, 0x22, 0xF0, 0x5E}
					iInBufLen	= 4
					iOut		= 287502430

  Parameters	:	unsigned char* ucInBuf		-	pointer to input buffer
					int iInBufLen		-	size of input buffer
					int* iOut			-	pointer to data member

  Return Value	:	int					-	OK (0) or error (<0)

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int FEDM_ConvHexUCharToInt(unsigned char* ucInBuf, int iInBufLen, int* iOut)
{
	FEDM_CHK3(ucInBuf);
	FEDM_CHK3(iOut);

	if(iInBufLen<0 || iInBufLen>4)
		return FEDM_ERROR_BUFFER_LENGTH;

	int	iByteCnt;
	*iOut = 0;

	// change Motorola format into Intel format
	for ( iByteCnt=0; iByteCnt<iInBufLen; iByteCnt++ )
		*iOut += ( (int)ucInBuf[iInBufLen-1-iByteCnt] << (8*iByteCnt) );

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	20.11.2000 / M. Hultsch
  Version       :	01.00.01 / 10.12.2000 / M. Hultsch

  Function		:	converts hex-string (unsigned char) in unsigned int

					attention: max. 4 bytes

					example:
					ucInBuf		= {0x11, 0x22, 0xF0, 0x5E}
					iInBufLen	= 4
					iOut		= 287502430

  Parameters	:	unsigned char* ucInBuf		-	pointer to input buffer
					int iInBufLen		-	size of input buffer
					unsigned int* uiOut			-	pointer to data member

  Return Value	:	int					-	OK (0) or error (<0)

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int FEDM_ConvHexUCharToUInt(unsigned char* ucInBuf, int iInBufLen, unsigned int* uiOut)
{
	FEDM_CHK3(ucInBuf);
	FEDM_CHK3(uiOut);

	int iOut = 0;

	int iBack = FEDM_ConvHexUCharToInt(ucInBuf, iInBufLen, &iOut);

	if(iBack)
		return iBack;

	*uiOut = (unsigned int)iOut;

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	20.11.2000 / M. Hultsch
  Version       :	01.00.01 / 10.12.2000 / M. Hultsch


  Function		:	converts hex-string (unsigned char) in __int64
					attention: max. 8 bytes

					example:
					ucInBuf		= {0x11, 0x22, 0xF0, 0x5E, 0x11, 0x22, 0xF0, 0x5E}
					iInBufLen	= 8
					i64Out		= 1234813534658031710

  Parameters	:	unsigned char* ucInBuf		-	pointer to input buffer
					int iBufLen			-	size of input buffer
					__int64* i64Out		-	pointer to data member

  Return Value	:	int					-	OK (0) or error (<0)

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int FEDM_ConvHexUCharToInt64(unsigned char* ucInBuf, int iInBufLen, __int64* i64Out)
{
	FEDM_CHK3(ucInBuf);
	FEDM_CHK3(i64Out);

	if(iInBufLen<0 || iInBufLen>8)
		return FEDM_ERROR_BUFFER_LENGTH;

	int	iByteCnt;
	*i64Out = 0;

	// change Motorola format into Intel format
	for ( iByteCnt=0; iByteCnt<iInBufLen; iByteCnt++ )
		*i64Out += ( (__int64)ucInBuf[iInBufLen-1-iByteCnt] << (8*iByteCnt) );

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	17.03.2010 / M. Hultsch
  Version       :	03.01.01 / 17.03.2010 / M. Hultsch


  Function		:	converts hex-string (unsigned char) in unsigned __int64
					attention: max. 8 bytes

					example:
					ucInBuf		= {0x11, 0x22, 0xF0, 0x5E, 0x11, 0x22, 0xF0, 0x5E}
					iInBufLen	= 8
					ui64Out		= 1234813534658031710

  Parameters	:	unsigned char* ucInBuf				-	pointer to input buffer
					int iBufLen					-	size of input buffer
					unsigned __int64* i64Out	-	pointer to data member

  Return Value	:	int					-	OK (0) or error (<0)

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int FEDM_ConvHexUCharToUInt64(unsigned char* ucInBuf, int iInBufLen, unsigned __int64* ui64Out)
{
	FEDM_CHK3(ucInBuf);
	FEDM_CHK3(ui64Out);

	if(iInBufLen<0 || iInBufLen>8)
		return FEDM_ERROR_BUFFER_LENGTH;

	int	iByteCnt;
	*ui64Out = 0;

	// change Motorola format into Intel format
	for ( iByteCnt=0; iByteCnt<iInBufLen; iByteCnt++ )
		*ui64Out += ( (unsigned __int64)ucInBuf[iInBufLen-1-iByteCnt] << (8*iByteCnt) );

	return FEDM_OK;
}


/***************************************************************************
  Begin        	:	20.02.2006 / M. Hultsch
  Version       :	02.04.01 / 20.02.2006 / M. Hultsch

  Function		:	converts hex-string (unsigned char) in char string
					this function is used for transponders serial numbers
					and other ultra long values

					example:
					ucInBuf		= {0x11, 0x22, 0xF0, 0x5E}
					iInBufLen	= 4
					cOut		= "1122F05E"

  Parameters	:	unsigned char* ucInBuf		-	pointer to input buffer
					int iInBufLen		-	size of input buffer
					char* cOut			-	pointer to data member
					int iOutBufLen		-	size of output buffer (includes additional '\0')

  Return Value	:	int					-	OK (0) or error (<0)

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvHexUCharToHexChar(unsigned char* ucInBuf, int iInBufLen, char* cOut, int iOutBufLen)
{
	FEDM_CHK3(ucInBuf);
	FEDM_CHK3(cOut);

	if(iInBufLen<0 || iOutBufLen<0)
		return FEDM_ERROR_BUFFER_LENGTH;

	if(iOutBufLen < (2*iInBufLen))
		return FEDM_ERROR_BUFFER_LENGTH;

	int	 iByteCnt;
	char cBuffer[3];
	unsigned char ucTmp;

	for(int i=0; i<iOutBufLen; ++i)
		cOut[i] = 0;

	for ( iByteCnt=0; iByteCnt<iInBufLen; iByteCnt++ )
	{
		ucTmp = ucInBuf[iByteCnt];
		cBuffer[0] = '\0';
		sprintf( cBuffer, "%02X", ucTmp );
		strcat(cOut, cBuffer);
	}

	return FEDM_OK;
}


/***************************************************************************
  Begin        	:	19.08.2003 / M. Hultsch
  Version       :	01.08.00 / 19.08.2003 / M. Hultsch

  Function		:	converts hex-string (unsigned char) in STL-string
					this function is used for transponders serial numbers
					and other ultra long values

					example:
					ucInBuf		= {0x11, 0x22, 0xF0, 0x5E}
					iInBufLen	= 4
					sOut		= "1122F05E"

  Parameters	:	unsigned char* ucInBuf		-	pointer to input buffer
					int iBufLen			-	size of input buffer
					string& sOut		-	pointer to data member

  Return Value	:	int					-	OK (0) or error (<0)

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int FEDM_ConvHexUCharToHexStr(unsigned char* ucInBuf, int iInBufLen, string& sOut)
{
	FEDM_CHK3(ucInBuf);
	FEDM_CHK3(&sOut);

	if(iInBufLen<0)
		return FEDM_ERROR_BUFFER_LENGTH;

	int	 iByteCnt;
	char cBuffer[3];
	unsigned char ucTmp;

	sOut = "";

	for ( iByteCnt=0; iByteCnt<iInBufLen; iByteCnt++ )
	{
		ucTmp = ucInBuf[iByteCnt];
		cBuffer[0] = '\0';
		sprintf( cBuffer, "%02X", ucTmp );
		sOut += cBuffer;
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	18.05.2000 / M. Hultsch
  Version       :	01.00.01 / 10.12.2000 / M. Hultsch

  Function		:	converts hex-string (unsigned char) in CString
					this function is used for transponders serial numbers
					and other ultra long values

					example:
					ucInBuf		= {0x11, 0x22, 0xF0, 0x5E}
					iInBufLen	= 4
					sOut		= "1122F05E"

  Parameters	:	unsigned char* ucInBuf		-	pointer to input buffer
					int iBufLen			-	size of input buffer
					CString& sOut		-	pointer to data member

  Return Value	:	int					-	OK (0) or error (<0)

***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
_FEDM_ISC_CORE_EXT_FUNCT int FEDM_ConvHexUCharToHexStr(unsigned char* ucInBuf, int iInBufLen, CString& sOut)
{
	FEDM_CHK3(ucInBuf);
	FEDM_CHK3(&sOut);

	if(iInBufLen<0)
		return FEDM_ERROR_BUFFER_LENGTH;

	int	 iByteCnt;
	char cBuffer[3];
	unsigned char ucTmp;

	sOut = "";

	for ( iByteCnt=0; iByteCnt<iInBufLen; iByteCnt++ )
	{
		ucTmp = ucInBuf[iByteCnt];
		cBuffer[0] = '\0';
		sprintf( cBuffer, "%02X", ucTmp );
		sOut += cBuffer;
	}

	return FEDM_OK;
}
#endif



//#####################################################################################
// data type conversion functions
// 
// HexChar -> different types
//#####################################################################################

/***************************************************************************
  Begin        	:	21.02.2006 / M. Hultsch
  Version       :	02.04.01 / 21.02.2006 / M. Hultsch

  Function		:	converts hex char in int

					attention: max. 8 bytes and even number of bytes

					example:
					cInBuf		= "1122F05E"
					iInBufLen	= 8
					iOut		= 287502430

  Parameters	:	char* cInBuf		-	pointer to input buffer
					int iInBufLen		-	size of input buffer
					int* iOut			-	pointer to data member

  Return Value	:	int					-	OK (0) or error (<0)

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvHexCharToInt(char* cInBuf, int iBufLen, int* iOut)
{
	FEDM_CHK3(cInBuf);
	FEDM_CHK3(iOut);

	int		iByteCnt;
	unsigned char	*ucTmpBuf;

	if (iBufLen == 0)
		return FEDM_ERROR_STRING_LENGTH;

	if ((iBufLen%2) != 0)
		return FEDM_ERROR_ODD_STRING_LENGTH;

	ucTmpBuf = new unsigned char[iBufLen/2];

	if(ucTmpBuf == NULL)
		return FEDM_ERROR_NO_MORE_MEM;

	int	iLen = FEDM_ConvHexCharToHexUChar(cInBuf, iBufLen, ucTmpBuf, (int)iBufLen/2);

	if(iLen <= 0 || iLen > 4)
	{
		delete[] ucTmpBuf;
		return FEDM_ERROR_BUFFER_LENGTH;
	}

	*iOut = 0;

	for(iByteCnt=0; iByteCnt<iLen; iByteCnt++)
		// hier wird Motorola zu Intel geschoben
		*iOut += ( (int)(*(ucTmpBuf+(iLen-1)-iByteCnt)) << (8*iByteCnt) );

	delete[] ucTmpBuf;

	return FEDM_OK;
}


/***************************************************************************
  Begin        	:	21.02.2006 / M. Hultsch
  Version       :	02.04.01 / 21.02.2006 / M. Hultsch

  Function		:	converts hex char in unsigned int

					attention: max. 8 bytes and even number of bytes

					example:
					cInBuf		= "1122F05E"
					iInBufLen	= 8
					iOut		= 287502430

  Parameters	:	char* cInBuf		-	pointer to input buffer
					int iInBufLen		-	size of input buffer
					unsigned int* uiOut			-	pointer to data member

  Return Value	:	int					-	OK (0) or error (<0)

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvHexCharToUInt(char* cInBuf, int iBufLen, unsigned int* uiOut)
{
	FEDM_CHK3(cInBuf);
	FEDM_CHK3(uiOut);

	int iOut = 0;

	int	iBack = FEDM_ConvHexCharToInt(cInBuf, iBufLen, &iOut);
	
	if(iBack < 0)
		return iBack;

	*uiOut = (unsigned int)iOut;

	return FEDM_OK;
}


/***************************************************************************
  Begin        	:	21.02.2006 / M. Hultsch
  Version       :	02.04.01 / 21.02.2006 / M. Hultsch

  Function		:	converts hex char in __int64

					attention: max. 16 chars and even number of bytes in cIn

					example:
					cInBuf		= "1122F05E1122F05E"
					iInBufLen	= 16
					i64Out		= 1234813534658031710

  Parameters	:	char* cInBuf		-	pointer to input buffer
					int iInBufLen		-	size of input buffer
					__int64* i64Out		-	pointer to data member

  Return Value	:	int					-	OK (0) or error (<0)

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvHexCharToInt64(char* cInBuf, int iBufLen, __int64* i64Out)
{
	FEDM_CHK3(cInBuf);
	FEDM_CHK3(i64Out);

	int		iByteCnt;
	unsigned char	*ucTmpBuf;

	if (iBufLen == 0)
		return FEDM_ERROR_STRING_LENGTH;

	if ((iBufLen%2) != 0)
		return FEDM_ERROR_ODD_STRING_LENGTH;

	ucTmpBuf = new unsigned char[iBufLen/2];

	if(ucTmpBuf == NULL)
		return FEDM_ERROR_NO_MORE_MEM;

	int	iLen = FEDM_ConvHexCharToHexUChar(cInBuf, iBufLen, ucTmpBuf, (int)iBufLen/2);

	if(iLen <= 0 || iLen > 8)
	{
		delete[] ucTmpBuf;
		return FEDM_ERROR_BUFFER_LENGTH;
	}

	*i64Out = 0;

	for(iByteCnt=0; iByteCnt<iLen; iByteCnt++)
		// hier wird Motorola zu Intel geschoben
		*i64Out += ( (__int64)(*(ucTmpBuf+(iLen-1)-iByteCnt)) << (8*iByteCnt) );

	delete[] ucTmpBuf;

	return FEDM_OK;
}


/***************************************************************************
  Begin        	:	17.03.2010 / M. Hultsch
  Version       :	03.01.01 / 17.03.2010 / M. Hultsch

  Function		:	converts hex char in unsigned __int64

					attention: max. 16 chars and even number of bytes in cIn

					example:
					cInBuf		= "1122F05E1122F05E"
					iInBufLen	= 16
					ui64Out		= 1234813534658031710

  Parameters	:	char* cInBuf				-	pointer to input buffer
					int iInBufLen				-	size of input buffer
					unsigned __int64* ui64Out	-	pointer to data member

  Return Value	:	int					-	OK (0) or error (<0)

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvHexCharToUInt64(char* cInBuf, int iBufLen, unsigned __int64* ui64Out)
{
	FEDM_CHK3(cInBuf);
	FEDM_CHK3(ui64Out);

	int		iByteCnt;
	unsigned char	*ucTmpBuf;

	if (iBufLen == 0)
		return FEDM_ERROR_STRING_LENGTH;

	if ((iBufLen%2) != 0)
		return FEDM_ERROR_ODD_STRING_LENGTH;

	ucTmpBuf = new unsigned char[iBufLen/2];

	if(ucTmpBuf == NULL)
		return FEDM_ERROR_NO_MORE_MEM;

	int	iLen = FEDM_ConvHexCharToHexUChar(cInBuf, iBufLen, ucTmpBuf, (int)iBufLen/2);

	if(iLen <= 0 || iLen > 8)
	{
		delete[] ucTmpBuf;
		return FEDM_ERROR_BUFFER_LENGTH;
	}

	*ui64Out = 0;

	for(iByteCnt=0; iByteCnt<iLen; iByteCnt++)
		// hier wird Motorola zu Intel geschoben
		*ui64Out += ( (unsigned __int64)(*(ucTmpBuf+(iLen-1)-iByteCnt)) << (8*iByteCnt) );

	delete[] ucTmpBuf;

	return FEDM_OK;
}


/***************************************************************************
  Begin        	:	21.02.2006 / M. Hultsch
  Version       :	02.04.01 / 21.02.2006 / M. Hultsch

  Function		:	converts hex char in hex-string (unsigned char)
					example: cInBuf = "1122F05E" --> ucOutBuf = {0x11, 0x22, 0xF0, 0x5E}
					
					attention: no null-termination

  Parameters	:	char* cInBuf		-	pointer to input buffer
					int iInBufLen		-	size of input buffer
					unsigned char* ucOutBuf		-	pointer to output buffer
					int iOutBufLen		-	size of output buffer

  Return Value	:	int					-	count of bytes in output buffer or error code

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvHexCharToHexUChar(char* cInBuf, int iBufLen, unsigned char* ucOutBuf, int iOutBufLen)
{
	FEDM_CHK3(cInBuf);
	FEDM_CHK3(ucOutBuf);

	int		i;
	int		iOutLen = 0;


	if (iBufLen == 0)
		return FEDM_ERROR_STRING_LENGTH;

	if ((iBufLen%2) != 0)
		return FEDM_ERROR_ODD_STRING_LENGTH;
	
	if ((iBufLen / 2) > iOutBufLen)
		return FEDM_ERROR_BUFFER_LENGTH;


	// convert every two chars to one hex-char
	for (i = 0; i < iBufLen; i += 2)
	{
		ucOutBuf[i / 2] = FEDM_ConvTwoAsciiToUChar((unsigned char)cInBuf[i], (unsigned char)cInBuf[i+1]);
		iOutLen++;
	}

	return iOutLen;
}




//#####################################################################################
// data type conversion functions
// 
// int -> HexUChar and HexStr
//#####################################################################################

/***************************************************************************
  Begin        	:	20.11.2000 / M. Hultsch
  Version       :	01.00.00 / 21.11.2000 / M. Hultsch

  Function		:	converts int in hex-string (CString)

					example:
					iIn  = 287502430
					sOut = "1122F05E"

  Parameters	:	int iIn			-	input data
					CString& sOut	-	reference to output string

  Return Value	:	int				-	OK (0) or error (<0)

***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvIntToHexStr(int iIn, CString& sOut)
{
	int iErr;

	FEDM_CHK1(iErr, FEDM_ConvUIntToHexStr((unsigned int)iIn, sOut));

	return FEDM_OK;
}
#endif

/***************************************************************************
  Begin        	:	30.04.2002 / M. Hultsch
  Version       :	01.05.00 / 30.11.2002 / M. Hultsch

  Function		:	converts int in hex-string (STL-string)

					example:
					iIn  = 287502430
					sOut = "1122F05E"

  Parameters	:	int iIn			-	input data
					string& sOut	-	reference to output string

  Return Value	:	int				-	OK (0) or error (<0)

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvIntToHexStr(int iIn, string& sOut)
{
	int iErr;

	FEDM_CHK1(iErr, FEDM_ConvUIntToHexStr((unsigned int)iIn, sOut));

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	20.02.2006 / M. Hultsch
  Version       :	02.04.01 / 20.02.2006 / M. Hultsch

  Function		:	converts int in char string

					example:
					iIn  = 287502430
					cOut = "1122F05E"

  Parameters	:	int iIn			-	input data
					char* cOut		-	pointer to output string
					int iOutBufLen	-	size of output buffer (includes additional '\0')

  Return Value	:	int				-	OK (0) or error (<0)

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvIntToHexChar(int iIn, char* cOut, int iOutBufLen)
{
	int iErr;

	FEDM_CHK1(iErr, FEDM_ConvUIntToHexChar((unsigned int)iIn, cOut, iOutBufLen));

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	20.11.2000 / M. Hultsch
  Version       :	01.00.00 / 22.11.2000 / M. Hultsch

  Function		:	converts int in hex-string (unsigned char)
					
					attention: no null-termination

					example:
					iIn			= 287502430
					ucOutBuf	= {0x11, 0x22, 0xF0, 0x5E}
					iOutBufLen	= 4

  Parameters	:	int iIn			-	input data
					unsigned char* ucOutBuf	-	pointer to output buffer
					int iOutBufLen	-	size of output buffer

  Return Value	:	int				-	OK (0) or error (<0)

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvIntToHexUChar(int iIn, unsigned char* ucOutBuf, int iOutBufLen)
{
	int iErr;

	FEDM_CHK1(iErr, FEDM_ConvUIntToHexUChar((unsigned int)iIn, ucOutBuf, iOutBufLen));

	return FEDM_OK;
}



//#####################################################################################
// data type conversion functions
// 
// unsigned int -> HexUChar and HexStr
//#####################################################################################

/***************************************************************************
  Begin        	:	20.11.2000 / M. Hultsch
  Version       :	01.00.01 / 10.12.2000 / M. Hultsch

  Function		:	converts unsigned int in hex-string (CString)

					example:
					uiIn = 287502430
					sOut = "1122F05E"

  Parameters	:	unsigned int uiIn		-	input data
					CString& sOut	-	reference to output string

  Return Value	:	int				-	OK (0) or error (<0)

***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvUIntToHexStr(unsigned int uiIn, CString& sOut)
{
	FEDM_CHK3(&sOut);

	unsigned char ucTmpBuf[4];
	unsigned char ucTmp;
	char  cBuffer[3];
	int   iErr;

	FEDM_CHK1(iErr, FEDM_ConvUIntToHexUChar(uiIn, ucTmpBuf, 4));

	sOut = "";
	for(int iCnt=0; iCnt<4; iCnt++)
	{
		ucTmp = ucTmpBuf[iCnt];
		cBuffer[0] = '\0';
		sprintf( cBuffer, "%02X", ucTmp );
		sOut += cBuffer;
	}

	return FEDM_OK;
}
#endif

/***************************************************************************
  Begin        	:	30.04.2002 / M. Hultsch
  Version       :	01.05.00 / 30.04.2002 / M. Hultsch

  Function		:	converts unsigned int in hex-string (STL-string)

					example:
					uiIn = 287502430
					sOut = "1122F05E"

  Parameters	:	unsigned int uiIn		-	input data
					string& sOut	-	reference to output string

  Return Value	:	int				-	OK (0) or error (<0)

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvUIntToHexStr(unsigned int uiIn, string& sOut)
{
	FEDM_CHK3(&sOut);

	unsigned char ucTmpBuf[4];
	unsigned char ucTmp;
	char  cBuffer[3];
	int   iErr;

	FEDM_CHK1(iErr, FEDM_ConvUIntToHexUChar(uiIn, ucTmpBuf, 4));

	sOut = "";
	for(int iCnt=0; iCnt<4; iCnt++)
	{
		ucTmp = ucTmpBuf[iCnt];
		cBuffer[0] = '\0';
		sprintf( cBuffer, "%02X", ucTmp );
		sOut += cBuffer;
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	30.04.2002 / M. Hultsch
  Version       :	01.05.00 / 30.04.2002 / M. Hultsch

  Function		:	converts unsigned int in hex-string (STL-string)

					example:
					uiIn = 287502430
					cOut = "1122F05E"

  Parameters	:	unsigned int uiIn		-	input data
					char* cOut		-	pointer to output string
					int iOutBufLen	-	size of output buffer (includes additional '\0')

  Return Value	:	int				-	OK (0) or error (<0)

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvUIntToHexChar(unsigned int uiIn, char* cOut, int iOutBufLen)
{
	FEDM_CHK3(cOut);

	if(iOutBufLen < 9)
		return FEDM_ERROR_BUFFER_LENGTH;

	unsigned char ucTmpBuf[4];
	unsigned char ucTmp;
	char  cBuffer[3];
	int   iErr;

	FEDM_CHK1(iErr, FEDM_ConvUIntToHexUChar(uiIn, ucTmpBuf, 4));

	for(int i=0; i<iOutBufLen; ++i)
		cOut[i] = 0;

	for(int iCnt=0; iCnt<4; iCnt++)
	{
		ucTmp = ucTmpBuf[iCnt];
		cBuffer[0] = '\0';
		sprintf( cBuffer, "%02X", ucTmp );
		strcat(cOut, cBuffer);
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	20.11.2000 / M. Hultsch
  Version       :	01.00.01 / 10.12.2000 / M. Hultsch

  Function		:	converts unsigned int in hex-string (unsigned char)
					
					attention: no null-termination

					example:
					uiIn		= 287502430
					ucOutBuf	= {0x11, 0x22, 0xF0, 0x5E}
					iOutBufLen	= 4

  Parameters	:	unsigned int uiIn		-	input data
					unsigned char* ucOutBuf	-	pointer to output buffer
					int iOutBufLen	-	size of output buffer

  Return Value	:	int				-	OK (0) or error (<0)

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvUIntToHexUChar(unsigned int uiIn, unsigned char* ucOutBuf, int iOutBufLen)
{
	FEDM_CHK3(ucOutBuf);

	if(iOutBufLen < 4)
		return FEDM_ERROR_BUFFER_LENGTH;
	
	for(int iCnt=0; iCnt<4; iCnt++ )
	{
		// set data in Motorola format
		ucOutBuf[4-1-iCnt] = *( ((unsigned char*)&uiIn) + iCnt );
	}

	return FEDM_OK;
}



//#####################################################################################
// data type conversion functions
// 
// __int64 -> HexUChar and HexStr
//#####################################################################################

/***************************************************************************
  Begin        	:	20.11.2000 / M. Hultsch
  Version       :	01.00.01 / 10.12.2000 / M. Hultsch

  Function		:	converts __int64 in hex-string (CString)

					example:
					i64In	= 1234813534658031710
					sOut	= "1122F05E1122F05E"

  Parameters	:	__int64 i64In	-	input data
					CString& sOut	-	reference to output string

  Return Value	:	int				-	OK (0) or error (<0)

***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvInt64ToHexStr(__int64 i64In, CString& sOut)
{
	FEDM_CHK3(&sOut);

	unsigned char ucTmpBuf[8];
	unsigned char ucTmp;
	char  cBuffer[3];
	int   iErr;

	FEDM_CHK1(iErr, FEDM_ConvInt64ToHexUChar(i64In, ucTmpBuf, 8));

	sOut = "";
	for(int iCnt=0; iCnt<8; iCnt++)
	{
		ucTmp = ucTmpBuf[iCnt];
		cBuffer[0] = '\0';
		sprintf( cBuffer, "%02X", ucTmp );
		sOut += cBuffer;
	}
	
	return FEDM_OK;
}
#endif

/***************************************************************************
  Begin        	:	17.03.2010 / M. Hultsch
  Version       :	03.01.01 / 17.03.2010 / M. Hultsch

  Function		:	converts unsigned __int64 in hex-string (CString)

					example:
					ui64In	= 1234813534658031710
					sOut	= "1122F05E1122F05E"

  Parameters	:	unsigned __int64 ui64In	-	input data
					CString& sOut			-	reference to output string

  Return Value	:	int				-	OK (0) or error (<0)

***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvUInt64ToHexStr(unsigned __int64 ui64In, CString& sOut)
{
	FEDM_CHK3(&sOut);

	unsigned char ucTmpBuf[8];
	unsigned char ucTmp;
	char  cBuffer[3];
	int   iErr;

	FEDM_CHK1(iErr, FEDM_ConvUInt64ToHexUChar(ui64In, ucTmpBuf, 8));

	sOut = "";
	for(int iCnt=0; iCnt<8; iCnt++)
	{
		ucTmp = ucTmpBuf[iCnt];
		cBuffer[0] = '\0';
		sprintf( cBuffer, "%02X", ucTmp );
		sOut += cBuffer;
	}
	
	return FEDM_OK;
}
#endif

/***************************************************************************
  Begin        	:	30.04.2002 / M. Hultsch
  Version       :	01.05.00 / 30.04.2002 / M. Hultsch

  Function		:	converts __int64 in hex-string (STL-string)

					example:
					i64In	= 1234813534658031710
					sOut	= "1122F05E1122F05E"

  Parameters	:	__int64 i64In	-	input data
					string& sOut	-	reference to output string

  Return Value	:	int				-	OK (0) or error (<0)

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvInt64ToHexStr(__int64 i64In, string& sOut)
{
	FEDM_CHK3(&sOut);

	unsigned char ucTmpBuf[8];
	unsigned char ucTmp;
	char  cBuffer[3];
	int   iErr;

	FEDM_CHK1(iErr, FEDM_ConvInt64ToHexUChar(i64In, ucTmpBuf, 8));

	sOut = "";
	for(int iCnt=0; iCnt<8; iCnt++)
	{
		ucTmp = ucTmpBuf[iCnt];
		cBuffer[0] = '\0';
		sprintf( cBuffer, "%02X", ucTmp );
		sOut += cBuffer;
	}
	
	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	17.03.2010 / M. Hultsch
  Version       :	03.01.01 / 17.03.2010 / M. Hultsch

  Function		:	converts unsigned __int64 in hex-string (STL-string)

					example:
					ui64In	= 1234813534658031710
					sOut	= "1122F05E1122F05E"

  Parameters	:	unsigned __int64 ui64In	-	input data
					string& sOut			-	reference to output string

  Return Value	:	int				-	OK (0) or error (<0)

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvUInt64ToHexStr(unsigned __int64 ui64In, string& sOut)
{
	FEDM_CHK3(&sOut);

	unsigned char ucTmpBuf[8];
	unsigned char ucTmp;
	char  cBuffer[3];
	int   iErr;

	FEDM_CHK1(iErr, FEDM_ConvUInt64ToHexUChar(ui64In, ucTmpBuf, 8));

	sOut = "";
	for(int iCnt=0; iCnt<8; iCnt++)
	{
		ucTmp = ucTmpBuf[iCnt];
		cBuffer[0] = '\0';
		sprintf( cBuffer, "%02X", ucTmp );
		sOut += cBuffer;
	}
	
	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	20.02.2006 / M. Hultsch
  Version       :	02.04.01 / 20.02.2006 / M. Hultsch

  Function		:	converts __int64 in char string

					example:
					i64In	= 1234813534658031710
					cOut	= "1122F05E1122F05E"
					int iOutBufLen	-	size of output buffer (includes additional '\0')

  Parameters	:	__int64 i64In	-	input data
					char* cOut		-	pointer to output string
					int iOutBufLen	-	size of output buffer (includes additional '\0')

  Return Value	:	int				-	OK (0) or error (<0)

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvInt64ToHexChar(__int64 i64In, char* cOut, int iOutBufLen)
{
	FEDM_CHK3(cOut);

	if(iOutBufLen < 8)
		return FEDM_ERROR_BUFFER_LENGTH;

	unsigned char ucTmpBuf[8];
	unsigned char ucTmp;
	char  cBuffer[3];
	int   iErr;

	FEDM_CHK1(iErr, FEDM_ConvInt64ToHexUChar(i64In, ucTmpBuf, 8));

	for(int i=0; i<iOutBufLen; ++i)
		cOut[i] = 0;

	for(int iCnt=0; iCnt<8; iCnt++)
	{
		ucTmp = ucTmpBuf[iCnt];
		cBuffer[0] = '\0';
		sprintf( cBuffer, "%02X", ucTmp );
		strcat(cOut, cBuffer);
	}
	
	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	17.03.2010 / M. Hultsch
  Version       :	03.01.01 / 17.03.2010 / M. Hultsch

  Function		:	converts unsigned __int64 in char string

					example:
					ui64In	= 1234813534658031710
					cOut	= "1122F05E1122F05E"
					int iOutBufLen	-	size of output buffer (includes additional '\0')

  Parameters	:	unsigned __int64 ui64In	-	input data
					char* cOut				-	pointer to output string
					int iOutBufLen			-	size of output buffer (includes additional '\0')

  Return Value	:	int				-	OK (0) or error (<0)

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvUInt64ToHexChar(unsigned __int64 ui64In, char* cOut, int iOutBufLen)
{
	FEDM_CHK3(cOut);

	if(iOutBufLen < 8)
		return FEDM_ERROR_BUFFER_LENGTH;

	unsigned char ucTmpBuf[8];
	unsigned char ucTmp;
	char  cBuffer[3];
	int   iErr;

	FEDM_CHK1(iErr, FEDM_ConvUInt64ToHexUChar(ui64In, ucTmpBuf, 8));

	for(int i=0; i<iOutBufLen; ++i)
		cOut[i] = 0;

	for(int iCnt=0; iCnt<8; iCnt++)
	{
		ucTmp = ucTmpBuf[iCnt];
		cBuffer[0] = '\0';
		sprintf( cBuffer, "%02X", ucTmp );
		strcat(cOut, cBuffer);
	}
	
	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	20.11.2000 / M. Hultsch
  Version       :	01.00.01 / 10.12.2000 / M. Hultsch

  Function		:	converts __int64 in hex-string (unsigned char)
					
					attention: no null-termination

					example:
					i64In		= 1234813534658031710
					ucOutBuf	= {0x11, 0x22, 0xF0, 0x5E, 0x11, 0x22, 0xF0, 0x5E}
					iOutBufLen	= 8

  Parameters	:	__int64 i64In	-	input data
					unsigned char* ucOutBuf	-	pointer to output buffer
					int iOutBufLen	-	size of output buffer

  Return Value	:	int				-	OK (0) or error (<0)

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvInt64ToHexUChar(__int64 i64In, unsigned char* ucOutBuf, int iOutBufLen)
{
	FEDM_CHK3(ucOutBuf);

	if(iOutBufLen < 8)
		return FEDM_ERROR_BUFFER_LENGTH;
	
	for(int iCnt=0; iCnt<8; iCnt++ )
	{
		// set data in Motorola format
		ucOutBuf[8-1-iCnt] = *( ((unsigned char*)&i64In) + iCnt );
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	17.03.2010 / M. Hultsch
  Version       :	03.01.01 / 17.03.2010 / M. Hultsch

  Function		:	converts unsigned __int64 in hex-string (unsigned char)
					
					attention: no null-termination

					example:
					ui64In		= 1234813534658031710
					ucOutBuf	= {0x11, 0x22, 0xF0, 0x5E, 0x11, 0x22, 0xF0, 0x5E}
					iOutBufLen	= 8

  Parameters	:	unsigned __int64 ui64In	-	input data
					unsigned char* ucOutBuf			-	pointer to output buffer
					int iOutBufLen			-	size of output buffer

  Return Value	:	int				-	OK (0) or error (<0)
***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int	FEDM_ConvUInt64ToHexUChar(unsigned __int64 ui64In, unsigned char* ucOutBuf, int iOutBufLen)
{
	FEDM_CHK3(ucOutBuf);

	if(iOutBufLen < 8)
		return FEDM_ERROR_BUFFER_LENGTH;
	
	for(int iCnt=0; iCnt<8; iCnt++ )
	{
		// set data in Motorola format
		ucOutBuf[8-1-iCnt] = *( ((unsigned char*)&ui64In) + iCnt );
	}

	return FEDM_OK;
}




//#####################################################################################
// very helpful functions
//#####################################################################################

/***************************************************************************
  Begin        	:	18.05.2000 / M. Hultsch
  Version       :	01.00.01 / 10.12.2000 / M. Hultsch

  Function		:	all chars except "0..9, a..f, A..F" will be removed
					copy CString in unsigned char-String

  Parameters	:	CString sIn			-	input string
					unsigned char* ucOutBuf		-	pointer to output buffer
					int iBufLen			-	size of output buffer

  Return Value	:	int					-	count of bytes in output buffer

***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
_FEDM_ISC_CORE_EXT_FUNCT int FEDM_RemNoHexChar(CString sIn, unsigned char* ucOutBuf, int iOutBufLen)
{
	FEDM_CHK3(ucOutBuf);

#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
	int	iInLen	= sIn.GetLength();
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
	int	iInLen	= sIn.Length();
#endif

	if(iOutBufLen == 0 || iOutBufLen < iInLen)
		return FEDM_ERROR_BUFFER_LENGTH;

	int iOutLen = 0;
	
#ifdef __BORLANDC__	// Borland C++ Builder: AnsiStrings index starts with 1 !!
	for (int i = 1; i<=iInLen; i++)
#else
	for (int i = 0; i<iInLen; i++)
#endif
	{
		switch (sIn[i])
		{
		case '0' :;	case '1' :;	case '2' :;	case '3' :;	case '4' :;	case '5' :;
		case '6' :;	case '7' :;	case '8' :;	case '9' :;
		case 'a' :;	case 'b' :;	case 'c' :;	case 'd' :;	case 'e' :;	case 'f' :;
		case 'A' :;	case 'B' :;	case 'C' :;	case 'D' :;	case 'E' :;	case 'F' :;
			ucOutBuf[iOutLen++] = (unsigned char) sIn[i];
			break;
		default:
			break;
		}
	}

	return iOutLen;
}
#endif

/***************************************************************************
  Begin        	:	30.04.2002 / M. Hultsch
  Version       :	01.05.00 / 30.04.2002 / M. Hultsch

  Function		:	all chars except "0..9, a..f, A..F" will be removed
					copy STL-string in unsigned char-String

  Parameters	:	string sIn			-	input string
					unsigned char* ucOutBuf		-	pointer to output buffer
					int iBufLen			-	size of output buffer

  Return Value	:	int					-	count of bytes in output buffer

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int FEDM_RemNoHexChar(string sIn, unsigned char* ucOutBuf, int iOutBufLen)
{
	FEDM_CHK3(ucOutBuf);

	int	iInLen	= (int)sIn.length();

	if(iOutBufLen == 0 || iOutBufLen < iInLen)
		return FEDM_ERROR_BUFFER_LENGTH;

	int iOutLen = 0;
	
	for (int i = 0; i<iInLen; i++)
	{
		switch (sIn[i])
		{
		case '0' :;	case '1' :;	case '2' :;	case '3' :;	case '4' :;	case '5' :;
		case '6' :;	case '7' :;	case '8' :;	case '9' :;
		case 'a' :;	case 'b' :;	case 'c' :;	case 'd' :;	case 'e' :;	case 'f' :;
		case 'A' :;	case 'B' :;	case 'C' :;	case 'D' :;	case 'E' :;	case 'F' :;
			ucOutBuf[iOutLen++] = (unsigned char) sIn[i];
			break;
		default:
			break;
		}
	}

	return iOutLen;
}

/***************************************************************************
  Begin        	:	07.02.2005 / M. Hultsch
  Version       :	02.01.00 / 07.02.2005 / M. Hultsch

  Function		:	all chars except "0..9, a..f, A..F" will be removed
					copy CString in CString

  Parameters	:	CString sIn			-	input string
					CString& sOut		-	output string

  Return Value	:	int					-	number of bytes in output string

***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
_FEDM_ISC_CORE_EXT_FUNCT int FEDM_RemNoHexChar(CString sIn, CString& sOut)
{
#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
	int	iInLen	= sIn.GetLength();
	sOut.Empty();
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
	int	iInLen	= sIn.Length();
#endif

	int iOutLen = 0;

	
#ifdef __BORLANDC__	// Borland C++ Builder: AnsiStrings index starts with 1 !!
	for (int i = 1; i<=iInLen; i++)
#else
	for (int i = 0; i<iInLen; i++)
#endif
	{
		switch (sIn[i])
		{
		case '0' :;	case '1' :;	case '2' :;	case '3' :;	case '4' :;	case '5' :;
		case '6' :;	case '7' :;	case '8' :;	case '9' :;
		case 'a' :;	case 'b' :;	case 'c' :;	case 'd' :;	case 'e' :;	case 'f' :;
		case 'A' :;	case 'B' :;	case 'C' :;	case 'D' :;	case 'E' :;	case 'F' :;
			sOut += sIn[i];
			iOutLen++;
			break;
		default:
			break;
		}
	}

	return iOutLen;
}
#endif

/***************************************************************************
  Begin        	:	16.04.2007 / M. Hultsch
  Version       :	03.00.00 / 16.04.2007 / M. Hultsch

  Function		:	all chars except "0..9, a..f, A..F" will be removed
					copy CString in CString

  Parameters	:	CString sIn			-	input string

  Return Value	:	CString sOut		-	output string

***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
_FEDM_ISC_CORE_EXT_FUNCT CString FEDM_RemNoHexChar(CString sIn)
{
#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
	int	iInLen	= sIn.GetLength();
	CString sOut;
#endif
#ifdef __BORLANDC__	// Borland C++ Builder
	int	iInLen	= sIn.Length();
	CString sOut;
#endif

	
#ifdef __BORLANDC__	// Borland C++ Builder: AnsiStrings index starts with 1 !!
	for (int i = 1; i<=iInLen; i++)
#else
	for (int i = 0; i<iInLen; i++)
#endif
	{
		switch (sIn[i])
		{
		case '0' :;	case '1' :;	case '2' :;	case '3' :;	case '4' :;	case '5' :;
		case '6' :;	case '7' :;	case '8' :;	case '9' :;
		case 'a' :;	case 'b' :;	case 'c' :;	case 'd' :;	case 'e' :;	case 'f' :;
		case 'A' :;	case 'B' :;	case 'C' :;	case 'D' :;	case 'E' :;	case 'F' :;
			sOut += sIn[i];
			break;
		default:
			break;
		}
	}

	return sOut;
}
#endif

/***************************************************************************
  Begin        	:	07.02.2005 / M. Hultsch
  Version       :	02.01.00 / 07.02.2005 / M. Hultsch

  Function		:	all chars except "0..9, a..f, A..F" will be removed
					copy STL-string in STL-string

  Parameters	:	string sIn			-	input string
					string sOut			-	output string

  Return Value	:	int					-	number of bytes in output string

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int FEDM_RemNoHexChar(string sIn, string& sOut)
{
	int	iInLen	= (int)sIn.length();

	int iOutLen = 0;
	
	for (int i = 0; i<iInLen; i++)
	{
		switch (sIn[i])
		{
		case '0' :;	case '1' :;	case '2' :;	case '3' :;	case '4' :;	case '5' :;
		case '6' :;	case '7' :;	case '8' :;	case '9' :;
		case 'a' :;	case 'b' :;	case 'c' :;	case 'd' :;	case 'e' :;	case 'f' :;
		case 'A' :;	case 'B' :;	case 'C' :;	case 'D' :;	case 'E' :;	case 'F' :;
			sOut += sIn[i];
			iOutLen++;
			break;
		default:
			break;
		}
	}

	return iOutLen;
}

/***************************************************************************
  Begin        	:	18.05.2000 / M. Hultsch
  Version       :	01.00.00 / 18.05.2000 / M. Hultsch

  Function		:	converts two ascii-chars into one byte
					example: c0='1'; c1='2' --> return value = 0x12

  Parameters	:	unsigned char ucIn1			-	first ascii char
					unsigned char ucIn2			-	second ascii char

  Return Value	:	unsigned char				-	byte

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT unsigned char FEDM_ConvTwoAsciiToUChar(unsigned char ucIn1, unsigned char ucIn2)
{
	unsigned char	uc1, uc2;
	int		iValue	= 0;

	uc1 = (unsigned char) toupper((int) ucIn1);
	uc2 = (unsigned char) toupper((int) ucIn2);


	if ((uc1 >= '0') && (uc1 <= '9'))	{ iValue  = (uc1 - 48) << 4; }
	if ((uc1 >= 'A') && (uc1 <= 'F'))	{ iValue  = (uc1 - 55) << 4; }
	if ((uc2 >= '0') && (uc2 <= '9'))	{ iValue += (uc2 - 48);	}
	if ((uc2 >= 'A') && (uc2 <= 'F'))	{ iValue += (uc2 - 55);	}

	return (unsigned char)iValue;
}

/***************************************************************************
  Begin        	:	27.10.2005 / M. Hultsch
  Version       :	02.03.06 / 27.10.2005 / M. Hultsch

  Function		:	converts one hex character into two ascii-chars
					example: unIn = 0x12 --> return values:  ucOut1='1' (0x31); ucOut2='2' (0x32)

  Parameters	:	unsigned char  ucIn			-	input hex char
					unsigned char& ucOut		-	reference to first ascii char
					unsigned char& ucOut2		-	reference to second ascii char

  Return Value	:	-
***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT void FEDM_ConvHexUCharToTwoAscii(unsigned char ucIn, unsigned char& ucOut1, unsigned char& ucOut2)
{
	if( ((ucIn&0xF0) >> 4 ) >= 0 && ((ucIn&0xF0) >> 4 ) <= 9)
		ucOut1 = ((ucIn&0xF0) >> 4 ) + 48;
	else if( ((ucIn&0xF0) >> 4 ) >= 10 && ((ucIn&0xF0) >> 4 ) <= 15)
		ucOut1 = ((ucIn&0xF0) >> 4 ) + 55;

	if( (ucIn&0x0F) >= 0 && (ucIn&0x0F) <= 9)
		ucOut2 = (ucIn&0x0F) + 48;
	else if( (ucIn&0x0F) >= 10 && (ucIn&0x0F) <= 15)
		ucOut2 = (ucIn&0x0F) + 55;
}

/***************************************************************************
  Begin        	:	24.04.2013 / M. Hultsch
  Version       :	04.04.04 / 24.04.2013 / M. Hultsch

  Function		:	Converts each ASCII character into hex character.
					
					attention: no null-termination

					example:
					ucInBuf		= {'9', '3', '0'}
					iInBufLen	= 3
					ucOutBuf	= {0x39, 0x33, 0x30}
					iOutBufLen	= 3

  Parameters	:	unsigned char* ucInBuf	-	pointer to input buffer
					int iInBufLen			-	size of input buffer
					unsigned char* ucOutBuf	-	pointer to output buffer
					int iOutBufLen			-	size of output buffer

  Return Value	:	int	-	OK (0) or error (<0)
***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int FEDM_ConvAsciiCharToHexUChar(unsigned char* ucInBuf, int iInBufLen, unsigned char* ucOutBuf, int iOutBufLen)
{
	int	iByteCnt = 0;

	for( iByteCnt=0; iByteCnt<iInBufLen; iByteCnt++ )
	{
		sscanf((const char*)&ucInBuf[iByteCnt], "%c", &ucOutBuf[iByteCnt]);
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	24.04.2013 / M. Hultsch
  Version       :	04.04.04 / 24.04.2013 / M. Hultsch

  Function		:	Converts BCD characters into hex array.
					Each input byte is expanded into two output bytes.
					
					attention: no null-termination

					example:
					ucInBuf		= {0x21, 0x00, 0x14}
					iInBufLen	= 3
					ucOutBuf	= {0x02, 0x01, 0x00, 0x00, 0x01, 0x04}
					iOutBufLen	= 6

  Parameters	:	unsigned char* ucInBuf	-	pointer to input buffer
					int iInBufLen			-	size of input buffer
					unsigned char* ucOutBuf	-	pointer to output buffer
					int iOutBufLen			-	size of output buffer

  Return Value	:	int	-	Number of bytes in ucOutBuf (>0) or error (<0)
***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int FEDM_ConvBcdCharToHexUChar(unsigned char* ucInBuf, int iInBufLen, unsigned char* ucOutBuf, int iOutBufLen)
{
	int		iBcdCnt;
	int		iByteCnt = 0;

	FEDM_CHK3(ucInBuf);
	FEDM_CHK3(ucOutBuf);

	if(iOutBufLen < (iInBufLen<<1) )
	{
		return FEDM_ERROR_BUFFER_LENGTH;
	}


	for( iBcdCnt=0; iBcdCnt<iInBufLen*2; iBcdCnt++ )
	{
		if(iBcdCnt%2 == 0)
			ucOutBuf[iBcdCnt] = (unsigned char) ( ((ucInBuf[iByteCnt] & 0xF0) >> 4) + 0x30 );
		else
			ucOutBuf[iBcdCnt] = (unsigned char)( (ucInBuf[iByteCnt++] & 0x0F) + 0x30 );
	}

	return iByteCnt;
}

/***************************************************************************
  Begin        	:	24.04.2013 / M. Hultsch
  Version       :	04.04.04 / 24.04.2013 / M. Hultsch

  Function		:	Converts hex array into BCD characters .
					Two input bytes are packed into one output byte.
					
					attention: no null-termination

					example:
					ucInBuf		= {0x02, 0x01, 0x00, 0x00, 0x01, 0x04}
					iInBufLen	= 6
					ucOutBuf	= {0x21, 0x00, 0x14}
					iOutBufLen	= 3

  Parameters	:	unsigned char* ucInBuf	-	pointer to input buffer
					int iInBufLen			-	size of input buffer
					unsigned char* ucOutBuf	-	pointer to output buffer
					int iOutBufLen			-	size of output buffer

  Return Value	:	int	-	Number of bytes in ucOutBuf (>0) or error (<0)
***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT int FEDM_ConvHexUCharToBcdChar(unsigned char* ucInBuf, int iInBufLen, unsigned char* ucOutBuf, int iOutBufLen)
{
	int		iBcdCnt;
	int		iByteCnt = 0;
	
	FEDM_CHK3(ucInBuf);
	FEDM_CHK3(ucOutBuf);

	if(iInBufLen % 2)
	{
		return FEDM_ERROR_PARAMETER;
	}

	if(iOutBufLen < (iInBufLen>>1) )
	{
		return FEDM_ERROR_BUFFER_LENGTH;
	}

	for( iBcdCnt=0; iBcdCnt<iInBufLen; iBcdCnt+=2 )
	{
		ucOutBuf[iByteCnt  ]  = (unsigned char)( ( (ucInBuf[iBcdCnt  ]-0x30) & 0x0F ) << 4 );
		ucOutBuf[iByteCnt++] += (unsigned char)(   (ucInBuf[iBcdCnt+1]-0x30) & 0x0F );
	}

	return iByteCnt;
}

/***************************************************************************
  Begin        	:	20.11.2000 / M. Hultsch
  Version       :	01.00.01 / 06.12.2000 / M. Hultsch

  Function		:	test of hex char inside sIn

  Parameters	:	CString sIn	- input string

  Return Value	:	true		- if all char inside sIn are hex chars
					false		- if one char inside sIn is not a hex char

***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
_FEDM_ISC_CORE_EXT_FUNCT bool FEDM_IsHex(CString sIn)
{
#ifdef _FEDM_MFC_SUPPORT		// Microsoft Compiler
	for( int iCnt=0; iCnt<sIn.GetLength(); iCnt++)
#endif
#ifdef __BORLANDC__	// Borland C++ Builder: AnsiStrings index starts with 1 !!
	for( int iCnt=1; iCnt<=sIn.Length(); iCnt++)
#endif
	{
		if(! ((sIn[iCnt] >= '0' && sIn[iCnt] <= '9') ||
			  (sIn[iCnt] >= 'a' && sIn[iCnt] <= 'f') ||
			  (sIn[iCnt] >= 'A' && sIn[iCnt] <= 'F') )
		  )
		{
			return false;
		}
	}

	return true;
}
#endif

/***************************************************************************
  Begin        	:	30.04.2002 / M. Hultsch
  Version       :	01.05.00 / 30.04.2002 / M. Hultsch

  Function		:	test of hex char inside sIn

  Parameters	:	string sIn	- input string

  Return Value	:	true		- if all char inside sIn are hex chars
					false		- if one char inside sIn is not a hex char

***************************************************************************/
_FEDM_ISC_CORE_EXT_FUNCT bool FEDM_IsHex(string sIn)
{
	for( unsigned int iCnt=0; iCnt<sIn.length(); iCnt++)
	{
		if(! ((sIn[iCnt] >= '0' && sIn[iCnt] <= '9') ||
			  (sIn[iCnt] >= 'a' && sIn[iCnt] <= 'f') ||
			  (sIn[iCnt] >= 'A' && sIn[iCnt] <= 'F') )
		  )
		{
			return false;
		}
	}

	return true;
}


/*******************************************************************************
  Function		:	GetUTCTime

  Begin			:	15.02.2008 / M. Hultsch

  Version		:	03.00.05 / 11.03.2009 / M. Hultsch

  Description	:	get the utc timestamp in microseconds
					Windows CE: timestamp in seconds

  Parameters	:	-

  Return value	:	(__int64)utc timestamp in microseconds
*******************************************************************************/
#if defined(_FEDM_WINDOWS) && !defined(_WIN32_WCE) && !defined(__BORLANDC__)
_FEDM_ISC_CORE_EXT_FUNCT __int64 FEDM_GetUTCTimestamp()
{
	_timeb ts;
#if _MSC_VER == 1200
	_ftime(&ts);
#else
	_ftime_s(&ts);
#endif
	return ((__int64)ts.time * 1000000) + ((__int64)ts.millitm * 1000);
}
#endif

#if defined(_FEDM_WINDOWS) && !defined(_WIN32_WCE) && defined(__BORLANDC__)
_FEDM_ISC_CORE_EXT_FUNCT __int64 FEDM_GetUTCTimestamp()
{
	return 0;
}
#endif

#if defined(_FEDM_WINDOWS) && defined(_WIN32_WCE)
_FEDM_ISC_CORE_EXT_FUNCT __int64 FEDM_GetUTCTimestamp()
{
#ifdef _FEDM_EVC4
	SYSTEMTIME systime;
	FILETIME filetime;
	ULARGE_INTEGER ulTime;
	::GetSystemTime(&systime);
	SystemTimeToFileTime(&systime, &filetime);
	ulTime = *((ULARGE_INTEGER*)&filetime);
	return (__int64)ulTime.QuadPart;
#else
	__int64 ltime;
	_time64( &ltime );
	return ltime;
#endif
}
#endif

#ifdef _FEDM_LINUX
_FEDM_ISC_CORE_EXT_FUNCT __int64 FEDM_GetUTCTimestamp()
{
	struct timeval    ts;
	gettimeofday(&ts, NULL);
	return ((__int64)ts.tv_sec)*1000000LL + (__int64)ts.tv_usec;
}
#endif

