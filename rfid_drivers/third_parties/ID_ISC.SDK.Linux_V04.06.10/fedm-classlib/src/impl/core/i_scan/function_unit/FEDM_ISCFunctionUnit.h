/*-------------------------------------------------------
|                                                       |
|                 FEDM_ISCFunctionUnit.h                |
|                                                       |
---------------------------------------------------------

Copyright © 2005-2008	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	05.01.2005

Version       		:	03.00.00 / 02.04.2008 / M. Hultsch

Operation Systems	:	Windows 9x/ME/NT/2000/XP/Linux/WindowsCE

Function			:	classes for OBID i-scan® function units


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_FUNCTION_UNIT_H_INCLUDED_)
#define _FEDM_ISC_FUNCTION_UNIT_H_INCLUDED_

#ifdef _MSC_VER
	// The (microsoft) debugger can't handle symbols more than 255 characters long.
	// STL often creates symbols longer than that.
	// When symbols are longer than 255 characters, the warning is disabled.
	#pragma warning(disable:4786)	// for MFC
#endif


#if !defined(_FEDM_NO_FU_SUPPORT)

#include "../../FEDM_Base.h"
#include <vector>

class FEDM_ISCReader;
class FEDM_ISCFunctionUnit;


// STL - Standard Template Library
using namespace std;


//#####################################################################################
// constants
//#####################################################################################
		
// size of memories in bytes
#define FEDM_ISC_FU_MAX_TMPDATA_MEM		128	// memory for temporary data

		
// function unit HF types
#define FEDM_ISC_FU_TYPE_NONE		0x00
#define FEDM_ISC_FU_TYPE_ANT		0x01
#define FEDM_ISC_FU_TYPE_MUX		0x0A
#define FEDM_ISC_FU_TYPE_DAT		0x0B

// function unit UHF types
#define FEDM_ISC_FU_TYPE_UMUX		0x0C



//#####################################################################################
// type definitions
//#####################################################################################
		
typedef vector<FEDM_ISCFunctionUnit*>				FEDM_FU_ARRAY;
typedef vector<FEDM_ISCFunctionUnit*>::iterator		FEDM_FU_ARRAY_ITOR;



//####################################################################
// class FEDM_ISCFunctionUnit
//####################################################################

class _FEDM_ISC_CORE_EXT_CLASS FEDM_ISCFunctionUnit
{
// public functions
public:

	// constructor
	FEDM_ISCFunctionUnit(FEDM_ISCReader* pReader, int iFUType = FEDM_ISC_FU_TYPE_NONE);

	// destructor
	virtual ~FEDM_ISCFunctionUnit();

	// get/set function unit type, which controlls internal operations
	int GetFUType();
	int SetFUType(int iFUType);

	// get/set antenna number
	unsigned int GetAntNo();
	void SetAntNo(unsigned int uiAntNo);

	// manage functions for wrapper classes
	int SetModuleHnd(int iHandle);
	int GetModuleHnd();

	// function to control the child management
	// if parameter is:
	// true (default)	- the parent FU can physically delete the child FUs
	// false			- the application must physically delete the child FUs
	void SetManageChildMode(bool bDeleteInternal);

	FEDM_ISCReader* GetReader();

	// main communication function
	int SendProtocol(unsigned char ucCmdByte);

	// get the last error code
	int GetLastError();
	// get the last status byte
	unsigned char GetLastStatus();
	
	// get error text
	int GetErrorText(char* cErrorText, int iErrorCode);
	// get status text
	int GetStatusText(char* cStatusText, unsigned char ucStatus);

	// functions for child list
	int AddChild(int iOutNo, FEDM_ISCFunctionUnit* pFU);
	int DelChild(int iOutNo);
	int DelChildList();
	int GetChildCount();
	FEDM_ISCFunctionUnit* GetChild(int iOutNo);


	// main access functions
	int GetData( char* ID, bool* Data );
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	int GetData( char* ID, BOOL* Data );
#endif
	int GetData( char* ID, unsigned char* Data );
	int GetData( char* ID, unsigned char* Data, int ParaAnz );
	int GetData( char* ID, unsigned int* Data );
	int GetData( char* ID, __int64* Data);
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	int GetData( char* ID, CString& Data );
#endif
	int GetData( char* ID, string& Data );
	int GetData( char* ID, char* Data, int DataLen );

	int SetData( char* ID, bool Data );
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	int SetData( char* ID, BOOL Data );
#endif
	int SetData( char* ID, unsigned char Data );
	int SetData( char* ID, unsigned char* Data, int ParaAnz );
	int SetData( char* ID, unsigned int Data );
	int SetData( char* ID, __int64 Data);
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	int SetData( char* ID, CString Data );
#endif
	int SetData( char* ID, string Data );
	int SetData( char* ID, char* Data, int DataLen );


// protected functions
protected:
	void SetLastError(int iError);


// public data
public:
	// memories (block-organized; data in Motorolas "Big Endian" format)
	// ### for temporary Data
	FEDM_BYTE_ARRAY TmpData;

// protected data
protected:
	// reader for communication
	FEDM_ISCReader* m_pReader;

	// alternatively: parameters for FEISC library
	int m_iReaderHnd;
	unsigned char m_ucBusAdr;

	int m_iLastError;
	unsigned char m_ucLastStatus;

	bool m_bManageChilds;

	int m_iFUType;
	FEDM_FU_ARRAY	m_ChildList;

	unsigned int m_uiAntNo;	// antenna number only for FU-Type FEDM_ISC_FU_TYPE_ANT

	// module handle for internal use
	int m_iModuleHandle;
};


#endif // #if !defined(_FEDM_NO_FU_SUPPORT)

#endif // !defined(_FEDM_ISC_FUNCTION_UNIT_H_INCLUDED_)
