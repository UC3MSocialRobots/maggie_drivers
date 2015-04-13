/*-------------------------------------------------------
|                                                       |
|                 FEDM_ISCFunctionUnit.cpp              |
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

Version       		:	03.00.00 / 04.04.2008 / M. Hultsch

Operation Systems	:	Windows 9x/ME/NT/2000/XP/Linux/WindowsCE

Function			:	classes for OBID i-scan® function units


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/



#if !defined(_FEDM_NO_FU_SUPPORT)

#include "FEDM_ISCFunctionUnit.h"
#include "FEDM_ISCFunctionUnitID.h"
#include "../FEDM_ISCReader.h"
#include "../../FEDM_Functions.h"
#include "feisc.h"
#include "fefu.h"




//####################################################################
// class FEDM_ISCFunctionUnit
//####################################################################

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FEDM_ISCFunctionUnit::FEDM_ISCFunctionUnit(FEDM_ISCReader* pReader, int iFUType)
{
	// memory for temporary data
	TmpData.reserve(FEDM_ISC_FU_MAX_TMPDATA_MEM);
	TmpData.resize(FEDM_ISC_FU_MAX_TMPDATA_MEM);

	// initialize memories with 0x00
	for( int i=0; i<FEDM_ISC_FU_MAX_TMPDATA_MEM; i++ )	{ TmpData[i] = 0x00; }

	m_iReaderHnd	= 0;
	m_ucBusAdr		= 0;
	m_pReader		= pReader;

	m_iLastError	= 0;
	m_ucLastStatus	= 0;

	m_bManageChilds	= true;

	m_iFUType		= iFUType;

	m_uiAntNo		= 0;

	m_iModuleHandle	= 0;

	if( (m_iFUType == FEDM_ISC_FU_TYPE_MUX) || (m_iFUType == FEDM_ISC_FU_TYPE_UMUX) )
	{
		m_ChildList.resize(8);
		for(int i=0; i<8; ++i)	{ m_ChildList[i] = NULL; }
	}
}


FEDM_ISCFunctionUnit::~FEDM_ISCFunctionUnit()
{
	TmpData.clear();
	DelChildList();
	m_ChildList.clear();
}

/***************************************************************************
  Begin        		:	27.07.2005 / M. Hultsch
  
  Version       	:	02.03.00 / 27.07.2005 / M. Hultsch

  Function			:	function to control the child management, whether the child(s) are physically
						deleted internally under control of the FU or the child(s) must be physically 
						deleted by the application.

  Parameters		:	bool bDeleteInternal
						- true (default): the parent FU can physically delete the child FUs
						- false			: the application must physically delete the child FUs

  Return value		:	-
***************************************************************************/
void FEDM_ISCFunctionUnit::SetManageChildMode(bool bDeleteInternal)
{
	m_bManageChilds = bDeleteInternal;
}

int FEDM_ISCFunctionUnit::GetFUType()
{
	return m_iFUType;
}

int FEDM_ISCFunctionUnit::SetFUType(int iFUType)
{
	switch(m_iFUType)
	{
	case FEDM_ISC_FU_TYPE_NONE:
		m_iFUType = iFUType;
		break;

	case FEDM_ISC_FU_TYPE_ANT:
		m_iFUType = iFUType;
		break;
	
	case FEDM_ISC_FU_TYPE_MUX:
	case FEDM_ISC_FU_TYPE_UMUX:
		if(m_iFUType == iFUType)
		{
			DelChildList();
			FEDM_RETURN(FEDM_OK);
		}

		DelChildList();
		m_iFUType = iFUType;
		break;
	
	case FEDM_ISC_FU_TYPE_DAT:
		m_iFUType = iFUType;
		break;

	default:
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	if( (m_iFUType == FEDM_ISC_FU_TYPE_MUX)||(m_iFUType == FEDM_ISC_FU_TYPE_UMUX) )
	{
		m_ChildList.clear();
		m_ChildList.resize(8);
		for(int i=0; i<8; ++i)	{ m_ChildList[i]  = NULL; }
	}

	FEDM_RETURN(FEDM_OK);
}

unsigned int FEDM_ISCFunctionUnit::GetAntNo()
{
	return m_uiAntNo;
}

void FEDM_ISCFunctionUnit::SetAntNo(unsigned int uiAntNo)
{
	if(uiAntNo > 0)
		m_uiAntNo = uiAntNo;
}


int FEDM_ISCFunctionUnit::SetModuleHnd(int iHandle)
{
	if(iHandle <= 0)
		FEDM_RETURN(FEDM_ERROR_PARAMETER);

	m_iModuleHandle = iHandle;
	
	FEDM_RETURN(FEDM_OK);
}

int FEDM_ISCFunctionUnit::GetModuleHnd()
{
	return m_iModuleHandle;
}

FEDM_ISCReader* FEDM_ISCFunctionUnit::GetReader()
{
	return m_pReader;
}


//////////////////////////////////////////////////////////////////////
// Communication
//////////////////////////////////////////////////////////////////////

/***************************************************************************
  Begin        		:	05.01.2005 / M. Hultsch
  
  Version			:	02.06.00 / 28.03.2008 / M. Hultsch

  Function			:	main function for communication with i-scan function unint

  Parameters		:	unsigned char ucCmdByte		-	command byte

  Return value		:	status byte (>=0) or error code (<0)
***************************************************************************/
int FEDM_ISCFunctionUnit::SendProtocol(unsigned char ucCmdByte)
{
	int		iErr = 0;
	int		iBack = 0;
	unsigned char	ucFUAdr = 0;	// function unit address
	unsigned char	ucValue[32];	memset( ucValue, '\0', 32);
	unsigned char	ucUHFFlags;

	if(m_pReader != NULL)
	{
		m_iReaderHnd	= m_pReader->GetReaderHnd();
		m_ucBusAdr		= m_pReader->GetBusAddress();
	}
	else
	{
		FEDM_RETURN(FEDM_ERROR_NULL_POINTER);
	}

	// no reader handle -> no communication
	if( m_iReaderHnd == 0)
		FEDM_RETURN(FEDM_ERROR_NO_READER_HANDLE);

	switch(ucCmdByte)
	{
	case 0xC0: // DAT: Get Firmware Version
		FEDM_CHK2(iErr, GetData(FEDM_ISC_FU_TMP_DAT_ADR, &ucFUAdr));
		iBack = FEFU_DAT_SoftVersion(m_iReaderHnd, m_ucBusAdr, ucFUAdr, &ucValue[0]);
		if(iBack != 0)
			FEDM_RETURN(iBack);

		FEDM_CHK2(iErr, SetData(FEDM_ISC_FU_TMP_SOFTVER, &ucValue[0], 7));
		break;

	case 0xC1: // DAT: CPU Reset
		FEDM_CHK2(iErr, GetData(FEDM_ISC_FU_TMP_DAT_ADR, &ucFUAdr));
		iBack = FEFU_DAT_CPUReset(m_iReaderHnd, m_ucBusAdr, ucFUAdr);
		break;

	case 0xC2: // DAT: Set Capacities
		FEDM_CHK2(iErr, GetData(FEDM_ISC_FU_TMP_DAT_ADR, &ucFUAdr));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_FU_TMP_DAT_ANT_VAL_C1, &ucValue[0]));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_FU_TMP_DAT_ANT_VAL_C2, &ucValue[1]));
		iBack = FEFU_DAT_SetCapacities(m_iReaderHnd, m_ucBusAdr, ucFUAdr, ucValue[0], ucValue[1]);
		break;

	case 0xC3: // DAT: Get Antenna Values
		FEDM_CHK2(iErr, GetData(FEDM_ISC_FU_TMP_DAT_ADR, &ucFUAdr));
		iBack = FEFU_DAT_GetValues(m_iReaderHnd, m_ucBusAdr, ucFUAdr, &ucValue[0]);
		if(iBack != 0)
			FEDM_RETURN(iBack);

		FEDM_CHK2(iErr, SetData(FEDM_ISC_FU_TMP_DAT_ANT_VAL, &ucValue[0], 6));
		break;

	case 0xC4: // DAT: Set Outputs
		FEDM_CHK2(iErr, GetData(FEDM_ISC_FU_TMP_DAT_ADR, &ucFUAdr));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_FU_TMP_DAT_OUT, &ucValue[0]));
		iBack = FEFU_DAT_SetOutput(m_iReaderHnd, m_ucBusAdr, ucFUAdr, ucValue[0]);
		break;

	case 0xC5: // DAT: Re-Tuning
		FEDM_CHK2(iErr, GetData(FEDM_ISC_FU_TMP_DAT_ADR, &ucFUAdr));
		iBack = FEFU_DAT_ReTuning(m_iReaderHnd, m_ucBusAdr, ucFUAdr);
		break;

	case 0xC6: // DAT: Start Tuning
		FEDM_CHK2(iErr, GetData(FEDM_ISC_FU_TMP_DAT_ADR, &ucFUAdr));
		iBack = FEFU_DAT_StartTuning(m_iReaderHnd, m_ucBusAdr, ucFUAdr);
		break;

	case 0xC8: // DAT: Store Settings
		FEDM_CHK2(iErr, GetData(FEDM_ISC_FU_TMP_DAT_ADR, &ucFUAdr));
		iBack = FEFU_DAT_StoreSettings(m_iReaderHnd, m_ucBusAdr, ucFUAdr);
		break;

	case 0xC9: // DAT: Detect
		FEDM_CHK2(iErr, GetData(FEDM_ISC_FU_TMP_DAT_ADR, &ucFUAdr));
		iBack = FEFU_DAT_Detect(m_iReaderHnd, m_ucBusAdr, ucFUAdr);
		break;

	case 0xCA: // DAT: Set Address
		FEDM_CHK2(iErr, GetData(FEDM_ISC_FU_TMP_DAT_ADR, &ucFUAdr));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_FU_TMP_DAT_NEW_ADR, &ucValue[0]));
		iBack = FEFU_DAT_SetAddress(m_iReaderHnd, m_ucBusAdr, ucFUAdr, ucValue[0]);
		break;

	case 0xCB: // DAT: Set Mode
		FEDM_CHK2(iErr, GetData(FEDM_ISC_FU_TMP_DAT_ADR, &ucFUAdr));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_FU_TMP_DAT_MODE, &ucValue[0]));
		iBack = FEFU_DAT_SetMode(m_iReaderHnd, m_ucBusAdr, ucFUAdr, ucValue[0]);
		break;

	case 0xDC: // MUX or UMUX: Detect
		FEDM_CHK2(iErr, GetData(FEDM_ISC_FU_TMP_MUX_ADR, &ucFUAdr));

		if ( m_iFUType == FEDM_ISC_FU_TYPE_MUX )
		{
			iBack = FEFU_MUX_Detect(m_iReaderHnd, m_ucBusAdr, ucFUAdr);
		}
		else if ( m_iFUType == FEDM_ISC_FU_TYPE_UMUX )
		{
			FEDM_CHK2(iErr, GetData(FEDM_ISC_FU_TMP_FLAGS, &ucUHFFlags));
			iBack = FEFU_UMUX_Detect_GetPower(m_iReaderHnd, m_ucBusAdr, ucFUAdr, ucUHFFlags, &m_ucLastStatus, &ucValue[0]);
			FEDM_CHK2(iErr, SetData(FEDM_ISC_FU_TMP_UMUX_POWER, &ucValue[0], 5));
			FEDM_CHK2(iErr, SetData(FEDM_ISC_FU_TMP_UMUX_LAST_STATE, m_ucLastStatus));
		}
		break;

	case 0xDD: // MUX or UMUX: Select Channel
		FEDM_CHK2(iErr, GetData(FEDM_ISC_FU_TMP_MUX_ADR, &ucFUAdr));
		FEDM_CHK2(iErr, GetData(FEDM_ISC_FU_TMP_MUX_OUT_CH1, &ucValue[0]));

		if ( m_iFUType == FEDM_ISC_FU_TYPE_MUX )
		{
			FEDM_CHK2(iErr, GetData(FEDM_ISC_FU_TMP_MUX_OUT_CH2, &ucValue[1]));
			iBack = FEFU_MUX_SelectChannel(m_iReaderHnd, m_ucBusAdr, ucFUAdr, ucValue[0], ucValue[1]);
		}
		else if ( m_iFUType == FEDM_ISC_FU_TYPE_UMUX )
		{
			FEDM_CHK2(iErr, GetData(FEDM_ISC_FU_TMP_FLAGS, &ucUHFFlags));
			iBack = FEFU_UMUX_SelectChannel(m_iReaderHnd, m_ucBusAdr, ucFUAdr, ucUHFFlags, ucValue[0], &m_ucLastStatus);
			FEDM_CHK2(iErr, SetData(FEDM_ISC_FU_TMP_UMUX_LAST_STATE, m_ucLastStatus));
		}
		break;

	case 0xDE: // MUX or UMUX: CPU Reset
		FEDM_CHK2(iErr, GetData(FEDM_ISC_FU_TMP_MUX_ADR, &ucFUAdr));

		if ( m_iFUType == FEDM_ISC_FU_TYPE_MUX )
		{
			iBack = FEFU_MUX_CPUReset(m_iReaderHnd, m_ucBusAdr, ucFUAdr);
		}
		else if ( m_iFUType == FEDM_ISC_FU_TYPE_UMUX )
		{
			FEDM_CHK2(iErr, GetData(FEDM_ISC_FU_TMP_FLAGS, &ucUHFFlags));
			iBack = FEFU_UMUX_CPUReset(m_iReaderHnd, m_ucBusAdr, ucFUAdr, ucUHFFlags, &m_ucLastStatus);
			FEDM_CHK2(iErr, SetData(FEDM_ISC_FU_TMP_UMUX_LAST_STATE, m_ucLastStatus));
		}
		break;

	case 0xDF: // MUX or UMUX: Get Firmware Version
		FEDM_CHK2(iErr, GetData(FEDM_ISC_FU_TMP_MUX_ADR, &ucFUAdr));

		if ( m_iFUType == FEDM_ISC_FU_TYPE_MUX )
		{
			iBack = FEFU_MUX_SoftVersion(m_iReaderHnd, m_ucBusAdr, ucFUAdr, &ucValue[0]);
		}
		else if ( m_iFUType == FEDM_ISC_FU_TYPE_UMUX )
		{
			FEDM_CHK2(iErr, GetData(FEDM_ISC_FU_TMP_FLAGS, &ucUHFFlags));
			iBack = FEFU_UMUX_SoftVersion(m_iReaderHnd, m_ucBusAdr, ucFUAdr, ucUHFFlags, &m_ucLastStatus, &ucValue[0]);
			FEDM_CHK2(iErr, SetData(FEDM_ISC_FU_TMP_UMUX_LAST_STATE, m_ucLastStatus));
		}

		if(iBack != 0)
			FEDM_RETURN(iBack);

		FEDM_CHK2(iErr, SetData(FEDM_ISC_FU_TMP_SOFTVER, &ucValue[0], 7));
		break;

	default:
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_CONTROL_BYTE);
	}

	FEDM_RETURN(iBack);
}


//////////////////////////////////////////////////////////////////////
// miscellaneous functions
//////////////////////////////////////////////////////////////////////

int FEDM_ISCFunctionUnit::GetLastError()
{
	return m_iLastError;
}

unsigned char FEDM_ISCFunctionUnit::GetLastStatus()
{
	return m_ucLastStatus;
}
	
int FEDM_ISCFunctionUnit::GetErrorText(char* cErrorText, int iErrorCode)
{
	FEDM_CHK5(cErrorText);

	if(iErrorCode == 0)
	{
		strcpy(cErrorText, "OK");
		return FEDM_OK;
	}
	else if(iErrorCode > -1000 && iErrorCode <= -100)
	{
		// errors from FEDM
		return FEDM_Base::GetErrorText(cErrorText, iErrorCode);
	}
	else if(iErrorCode > -4100 && iErrorCode < -1000)
	{
		// errors from reader controlled libraries
		if(m_pReader != NULL)
		{
			return m_pReader->GetErrorText(cErrorText, iErrorCode);
		}
		else
		{
			return FEDM_Base::GetErrorText(cErrorText, FEDM_ERROR_NULL_POINTER);
		}
	}
	else if(iErrorCode > -4200 && iErrorCode < -4100)
	{
		// errors from fefu library
		return FEFU_GetErrorText(iErrorCode, cErrorText);
	}
	else
	{
		return FEDM_Base::GetErrorText(cErrorText, iErrorCode);
	}

	return FEDM_ERROR_UNKNOWN_ERROR_CODE;
}

int FEDM_ISCFunctionUnit::GetStatusText(char* cStatusText, unsigned char ucStatus)
{
	FEDM_CHK5(cStatusText);

	// status text from fefu library
	return FEFU_GetStatusText(ucStatus, cStatusText);
}


//////////////////////////////////////////////////////////////////////
// main access functions
//////////////////////////////////////////////////////////////////////

/***************************************************************************
  Begin        	:	05.01.2005 / M. Hultsch
  Version       :	02.00.01 / 05.01.2005 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	char* ID	-	parameter identifier
					bool* Data	-	pointer to value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCFunctionUnit::GetData( char* ID, bool* Data )
{
	FEDM_CHK5(ID);
	FEDM_CHK5(Data);

	// extract MemID
	int MemID = FEDM_GetMemIDOfID(ID);
	
	switch( MemID )
	{
	case FEDM_TMPDATA_MEM:
		return FEDM_Base::GetData(ID, Data, TmpData, 16);
	case FEDM_RFC_EEDATA_MEM:
	case FEDM_RFC_RAMDATA_MEM:
	case FEDM_MJPDATA_MEM:
	case FEDM_SN_MEM:
	case FEDM_ID_MEM:
	case FEDM_AC_MEM:
	case FEDM_PUB_MEM:
	case FEDM_SEC_MEM:
	case FEDM_CONF_MEM:
	case FEDM_DATE_MEM:
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	default:
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_MEM_ID);
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	05.01.2005 / M. Hultsch
  Version       :	02.00.01 / 05.01.2005 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	char* ID	-	parameter identifier
					BOOL* Data	-	pointer to value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_ISCFunctionUnit::GetData( char* ID, BOOL* Data )
{
	FEDM_CHK5(ID);
	FEDM_CHK5(Data);

	// extract MemID
	int MemID = FEDM_GetMemIDOfID(ID);
	
	switch( MemID )
	{
	case FEDM_TMPDATA_MEM:
		return FEDM_Base::GetData(ID, Data, TmpData, 16);
	case FEDM_RFC_EEDATA_MEM:
	case FEDM_RFC_RAMDATA_MEM:
	case FEDM_MJPDATA_MEM:
	case FEDM_SN_MEM:
	case FEDM_ID_MEM:
	case FEDM_AC_MEM:
	case FEDM_PUB_MEM:
	case FEDM_SEC_MEM:
	case FEDM_CONF_MEM:
	case FEDM_DATE_MEM:
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	default:
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_MEM_ID);
	}

	FEDM_RETURN(FEDM_OK);
}
#endif

/***************************************************************************
  Begin        	:	05.01.2005 / M. Hultsch
  Version       :	02.00.01 / 05.01.2005 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	char* ID	-	parameter identifier
					unsigned char* Data	-	pointer to value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCFunctionUnit::GetData( char* ID, unsigned char* Data )
{
	FEDM_CHK5(ID);
	FEDM_CHK5(Data);

	// extract MemID
	int MemID = FEDM_GetMemIDOfID(ID);
	
	switch( MemID )
	{
	case FEDM_TMPDATA_MEM:
		return FEDM_Base::GetData(ID, Data, TmpData, 16);
	case FEDM_RFC_EEDATA_MEM:
	case FEDM_RFC_RAMDATA_MEM:
	case FEDM_MJPDATA_MEM:
	case FEDM_SN_MEM:
	case FEDM_ID_MEM:
	case FEDM_AC_MEM:
	case FEDM_PUB_MEM:
	case FEDM_SEC_MEM:
	case FEDM_CONF_MEM:
	case FEDM_DATE_MEM:
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	default:
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_MEM_ID);
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	05.01.2005 / M. Hultsch
  Version       :	02.00.01 / 05.01.2005 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	char* ID	-	parameter identifier
					unsigned char* Data	-	pointer to value
					int Cnt		-	length of Data

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCFunctionUnit::GetData( char* ID, unsigned char* Data, int Cnt )
{
	FEDM_CHK5(ID);
	FEDM_CHK5(Data);

	// extract MemID
	int MemID = FEDM_GetMemIDOfID(ID);
	
	switch( MemID )
	{
	case FEDM_TMPDATA_MEM:
		return FEDM_Base::GetData(ID, Data, Cnt, TmpData, 16);
	case FEDM_RFC_EEDATA_MEM:
	case FEDM_RFC_RAMDATA_MEM:
	case FEDM_MJPDATA_MEM:
	case FEDM_SN_MEM:
	case FEDM_ID_MEM:
	case FEDM_AC_MEM:
	case FEDM_PUB_MEM:
	case FEDM_SEC_MEM:
	case FEDM_CONF_MEM:
	case FEDM_DATE_MEM:
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	default:
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_MEM_ID);
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	05.01.2005 / M. Hultsch
  Version       :	02.00.01 / 05.01.2005 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	char* ID	-	parameter identifier
					unsigned int* Data	-	pointer to value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCFunctionUnit::GetData( char* ID, unsigned int* Data )
{
	FEDM_CHK5(ID);
	FEDM_CHK5(Data);

	// extract MemID
	int MemID = FEDM_GetMemIDOfID(ID);
	
	switch( MemID )
	{
	case FEDM_TMPDATA_MEM:
		return FEDM_Base::GetData(ID, Data, TmpData, 16);
	case FEDM_RFC_EEDATA_MEM:
	case FEDM_RFC_RAMDATA_MEM:
	case FEDM_MJPDATA_MEM:
	case FEDM_SN_MEM:
	case FEDM_ID_MEM:
	case FEDM_AC_MEM:
	case FEDM_PUB_MEM:
	case FEDM_SEC_MEM:
	case FEDM_CONF_MEM:
	case FEDM_DATE_MEM:
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	default:
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_MEM_ID);
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	05.01.2005 / M. Hultsch
  Version       :	02.00.01 / 05.01.2005 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	char* ID		-	parameter identifier
					__int64* Data	-	pointer to value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCFunctionUnit::GetData( char* ID, __int64* Data)
{
	FEDM_CHK5(ID);
	FEDM_CHK5(Data);

	// extract MemID
	int MemID = FEDM_GetMemIDOfID(ID);
	
	switch( MemID )
	{
	case FEDM_TMPDATA_MEM:
		return FEDM_Base::GetData(ID, Data, TmpData, 16);
	case FEDM_RFC_EEDATA_MEM:
	case FEDM_RFC_RAMDATA_MEM:
	case FEDM_MJPDATA_MEM:
	case FEDM_SN_MEM:
	case FEDM_ID_MEM:
	case FEDM_AC_MEM:
	case FEDM_PUB_MEM:
	case FEDM_SEC_MEM:
	case FEDM_CONF_MEM:
	case FEDM_DATE_MEM:
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	default:
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_MEM_ID);
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	05.01.2005 / M. Hultsch
  Version       :	02.00.01 / 05.01.2005 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	char* ID		-	parameter identifier
					CString& Data	-	reference of value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_ISCFunctionUnit::GetData( char* ID, CString& Data )
{
	FEDM_CHK5(ID);

	// extract MemID
	int MemID = FEDM_GetMemIDOfID(ID);
	
	switch( MemID )
	{
	case FEDM_TMPDATA_MEM:
		return FEDM_Base::GetData(ID, Data, TmpData, 16);
	case FEDM_RFC_EEDATA_MEM:
	case FEDM_RFC_RAMDATA_MEM:
	case FEDM_MJPDATA_MEM:
	case FEDM_SN_MEM:
	case FEDM_ID_MEM:
	case FEDM_AC_MEM:
	case FEDM_PUB_MEM:
	case FEDM_SEC_MEM:
	case FEDM_CONF_MEM:
	case FEDM_DATE_MEM:
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	default:
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_MEM_ID);
	}

	FEDM_RETURN(FEDM_OK);
}
#endif

/***************************************************************************
  Begin        	:	05.01.2005 / M. Hultsch
  Version       :	02.00.01 / 05.01.2005 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	char* ID		-	parameter identifier
					string& Data	-	reference of value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCFunctionUnit::GetData( char* ID, string& Data )
{
	FEDM_CHK5(ID);

	// extract MemID
	int MemID = FEDM_GetMemIDOfID(ID);
	
	switch( MemID )
	{
	case FEDM_TMPDATA_MEM:
		return FEDM_Base::GetData(ID, Data, TmpData, 16);
	case FEDM_RFC_EEDATA_MEM:
	case FEDM_RFC_RAMDATA_MEM:
	case FEDM_MJPDATA_MEM:
	case FEDM_SN_MEM:
	case FEDM_ID_MEM:
	case FEDM_AC_MEM:
	case FEDM_PUB_MEM:
	case FEDM_SEC_MEM:
	case FEDM_CONF_MEM:
	case FEDM_DATE_MEM:
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	default:
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_MEM_ID);
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	05.01.2005 / M. Hultsch
  Version       :	02.00.01 / 05.01.2005 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	char* ID	-	parameter identifier
					char* Data	-	pointer to value
					int DataLen	-	length of Data

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCFunctionUnit::GetData( char* ID, char* Data, int DataLen )
{
	FEDM_CHK5(ID);
	FEDM_CHK5(Data);

	// extract MemID
	int MemID = FEDM_GetMemIDOfID(ID);
	
	switch( MemID )
	{
	case FEDM_TMPDATA_MEM:
		return FEDM_Base::GetData(ID, Data, DataLen, TmpData, 16);
	case FEDM_RFC_EEDATA_MEM:
	case FEDM_RFC_RAMDATA_MEM:
	case FEDM_MJPDATA_MEM:
	case FEDM_SN_MEM:
	case FEDM_ID_MEM:
	case FEDM_AC_MEM:
	case FEDM_PUB_MEM:
	case FEDM_SEC_MEM:
	case FEDM_CONF_MEM:
	case FEDM_DATE_MEM:
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	default:
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_MEM_ID);
	}

	FEDM_RETURN(FEDM_OK);
}

	
/***************************************************************************
  Begin        	:	05.01.2005 / M. Hultsch
  Version       :	02.00.01 / 05.01.2005 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	char* ID	-	parameter identifier
					bool Data	-	value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCFunctionUnit::SetData( char* ID, bool Data )
{
	FEDM_CHK5(ID);

	// extract MemID
	int MemID = FEDM_GetMemIDOfID(ID);
	
	switch( MemID )
	{
	case FEDM_TMPDATA_MEM:
		return FEDM_Base::SetData(ID, Data, TmpData, 16);
	case FEDM_RFC_EEDATA_MEM:		
	case FEDM_RFC_RAMDATA_MEM:		
	case FEDM_MJPDATA_MEM:
	case FEDM_SN_MEM:
	case FEDM_ID_MEM:
	case FEDM_AC_MEM:			
	case FEDM_PUB_MEM:			
	case FEDM_SEC_MEM:			
	case FEDM_CONF_MEM:		
	case FEDM_DATE_MEM:
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	default:
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_MEM_ID);
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	05.01.2005 / M. Hultsch
  Version       :	02.00.01 / 05.01.2005 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	char* ID	-	parameter identifier
					BOOL Data	-	value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_ISCFunctionUnit::SetData( char* ID, BOOL Data )
{
	FEDM_CHK5(ID);

	// extract MemID
	int MemID = FEDM_GetMemIDOfID(ID);
	
	switch( MemID )
	{
	case FEDM_TMPDATA_MEM:
		return FEDM_Base::SetData(ID, Data, TmpData, 16);
	case FEDM_RFC_EEDATA_MEM:		
	case FEDM_RFC_RAMDATA_MEM:		
	case FEDM_MJPDATA_MEM:
	case FEDM_SN_MEM:
	case FEDM_ID_MEM:
	case FEDM_AC_MEM:			
	case FEDM_PUB_MEM:			
	case FEDM_SEC_MEM:			
	case FEDM_CONF_MEM:		
	case FEDM_DATE_MEM:
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	default:
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_MEM_ID);
	}

	FEDM_RETURN(FEDM_OK);
}
#endif

/***************************************************************************
  Begin        	:	05.01.2005 / M. Hultsch
  Version       :	02.00.01 / 05.01.2005 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	char* ID	-	parameter identifier
					unsigned char Data	-	value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCFunctionUnit::SetData( char* ID, unsigned char Data )
{
	FEDM_CHK5(ID);

	// extract MemID
	int MemID = FEDM_GetMemIDOfID(ID);
	
	switch( MemID )
	{
	case FEDM_TMPDATA_MEM:
		return FEDM_Base::SetData(ID, Data, TmpData, 16);
	case FEDM_RFC_EEDATA_MEM:		
	case FEDM_RFC_RAMDATA_MEM:		
	case FEDM_MJPDATA_MEM:
	case FEDM_SN_MEM:
	case FEDM_ID_MEM:
	case FEDM_AC_MEM:			
	case FEDM_PUB_MEM:			
	case FEDM_SEC_MEM:			
	case FEDM_CONF_MEM:		
	case FEDM_DATE_MEM:
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	default:
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_MEM_ID);
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	05.01.2005 / M. Hultsch
  Version       :	02.00.01 / 05.01.2005 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	char* ID	-	parameter identifier
					unsigned char* Data	-	pointer to value
					int Cnt		-	length of Data

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCFunctionUnit::SetData( char* ID, unsigned char* Data, int Cnt )
{
	FEDM_CHK5(ID);

	// extract MemID
	int MemID = FEDM_GetMemIDOfID(ID);
	
	switch( MemID )
	{
	case FEDM_TMPDATA_MEM:
		return FEDM_Base::SetData(ID, Data, Cnt, TmpData, 16);
	case FEDM_RFC_EEDATA_MEM:		
	case FEDM_RFC_RAMDATA_MEM:		
	case FEDM_MJPDATA_MEM:
	case FEDM_SN_MEM:
	case FEDM_ID_MEM:
	case FEDM_AC_MEM:			
	case FEDM_PUB_MEM:			
	case FEDM_SEC_MEM:			
	case FEDM_CONF_MEM:		
	case FEDM_DATE_MEM:
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	default:
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_MEM_ID);
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	05.01.2005 / M. Hultsch
  Version       :	02.00.01 / 05.01.2005 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	char* ID	-	parameter identifier
					unsigned int Data	-	value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCFunctionUnit::SetData( char* ID, unsigned int Data )
{
	FEDM_CHK5(ID);

	// extract MemID
	int MemID = FEDM_GetMemIDOfID(ID);
	
	switch( MemID )
	{
	case FEDM_TMPDATA_MEM:
		return FEDM_Base::SetData(ID, Data, TmpData, 16);
	case FEDM_RFC_EEDATA_MEM:		
	case FEDM_RFC_RAMDATA_MEM:		
	case FEDM_MJPDATA_MEM:
	case FEDM_SN_MEM:
	case FEDM_ID_MEM:
	case FEDM_AC_MEM:			
	case FEDM_PUB_MEM:			
	case FEDM_SEC_MEM:			
	case FEDM_CONF_MEM:		
	case FEDM_DATE_MEM:
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	default:
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_MEM_ID);
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	05.01.2005 / M. Hultsch
  Version       :	02.00.01 / 05.01.2005 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	char* ID		-	parameter identifier
					__int64 Data	-	value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCFunctionUnit::SetData( char* ID, __int64 Data)
{
	FEDM_CHK5(ID);

	// extract MemID
	int MemID = FEDM_GetMemIDOfID(ID);
	
	switch( MemID )
	{
	case FEDM_TMPDATA_MEM:
		return FEDM_Base::SetData(ID, Data, TmpData, 16);
	case FEDM_RFC_EEDATA_MEM:		
	case FEDM_RFC_RAMDATA_MEM:		
	case FEDM_MJPDATA_MEM:
	case FEDM_SN_MEM:
	case FEDM_ID_MEM:
	case FEDM_AC_MEM:			
	case FEDM_PUB_MEM:			
	case FEDM_SEC_MEM:			
	case FEDM_CONF_MEM:		
	case FEDM_DATE_MEM:
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	default:
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_MEM_ID);
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	05.01.2005 / M. Hultsch
  Version       :	02.00.01 / 05.01.2005 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	char* ID		-	parameter identifier
					CString Data	-	value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_ISCFunctionUnit::SetData( char* ID, CString Data )
{
	FEDM_CHK5(ID);

	// extract MemID
	int MemID = FEDM_GetMemIDOfID(ID);
	
	switch( MemID )
	{
	case FEDM_TMPDATA_MEM:
		return FEDM_Base::SetData(ID, Data, TmpData, 16);
	case FEDM_RFC_EEDATA_MEM:		
	case FEDM_RFC_RAMDATA_MEM:		
	case FEDM_MJPDATA_MEM:
	case FEDM_SN_MEM:
	case FEDM_ID_MEM:
	case FEDM_AC_MEM:			
	case FEDM_PUB_MEM:			
	case FEDM_SEC_MEM:			
	case FEDM_CONF_MEM:		
	case FEDM_DATE_MEM:
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	default:
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_MEM_ID);
	}

	FEDM_RETURN(FEDM_OK);
}
#endif

/***************************************************************************
  Begin        	:	05.01.2005 / M. Hultsch
  Version       :	02.00.01 / 05.01.2005 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	char* ID	-	parameter identifier
					string Data	-	value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCFunctionUnit::SetData( char* ID, string Data )
{
	FEDM_CHK5(ID);

	// extract MemID
	int MemID = FEDM_GetMemIDOfID(ID);
	
	switch( MemID )
	{
	case FEDM_TMPDATA_MEM:
		return FEDM_Base::SetData(ID, Data, TmpData, 16);
	case FEDM_RFC_EEDATA_MEM:		
	case FEDM_RFC_RAMDATA_MEM:		
	case FEDM_MJPDATA_MEM:
	case FEDM_SN_MEM:
	case FEDM_ID_MEM:
	case FEDM_AC_MEM:			
	case FEDM_PUB_MEM:			
	case FEDM_SEC_MEM:			
	case FEDM_CONF_MEM:		
	case FEDM_DATE_MEM:
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	default:
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_MEM_ID);
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	05.01.2005 / M. Hultsch
  Version       :	02.00.01 / 05.01.2005 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	char* ID	-	parameter identifier
					char* Data	-	pointer to value
					int DataLen	-	length of Data

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCFunctionUnit::SetData( char* ID, char* Data, int DataLen )
{
	FEDM_CHK5(ID);

	// extract MemID
	int MemID = FEDM_GetMemIDOfID(ID);
	
	switch( MemID )
	{
	case FEDM_TMPDATA_MEM:
		return FEDM_Base::SetData(ID, Data, DataLen, TmpData, 16);
	case FEDM_RFC_EEDATA_MEM:		
	case FEDM_RFC_RAMDATA_MEM:		
	case FEDM_MJPDATA_MEM:
	case FEDM_SN_MEM:
	case FEDM_ID_MEM:
	case FEDM_AC_MEM:			
	case FEDM_PUB_MEM:			
	case FEDM_SEC_MEM:			
	case FEDM_CONF_MEM:		
	case FEDM_DATE_MEM:
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	default:
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_MEM_ID);
	}

	FEDM_RETURN(FEDM_OK);
}


//////////////////////////////////////////////////////////////////////
// functions for child list
//////////////////////////////////////////////////////////////////////

/***************************************************************************
  Begin        	:	07.01.2005 / M. Hultsch
  Version       :	02.06.00 / 28.03.2008 / M. Hultsch

  Function		:	adds a new function unit object to the child list

  Parameter		:	int iOutNo					-	output number of function unit
					FEDM_ISCFunctionUnit* pFU	-	function unit object to be add

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCFunctionUnit::AddChild(int iOutNo, FEDM_ISCFunctionUnit* pFU)
{
	FEDM_CHK5(pFU);

	int iSize = 0;

	switch(m_iFUType)
	{
	case FEDM_ISC_FU_TYPE_NONE:
	case FEDM_ISC_FU_TYPE_ANT:
	case FEDM_ISC_FU_TYPE_DAT:
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
		
	case FEDM_ISC_FU_TYPE_MUX:
	case FEDM_ISC_FU_TYPE_UMUX:
		iSize = (int)m_ChildList.size();
		if(iSize != 8)
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);

		if(iOutNo<1 || iOutNo>8)
			FEDM_RETURN(FEDM_ERROR_PARAMETER);

		m_ChildList[iOutNo-1] = pFU;
		break;
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	07.01.2005 / M. Hultsch
  Version       :	02.06.00 / 28.03.2008 / M. Hultsch

  Function		:	removes a function unit object from the child list
					and deletes the object

  Parameter		:	int iOutNo					-	output number of function unit

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCFunctionUnit::DelChild(int iOutNo)
{
	switch(m_iFUType)
	{
	case FEDM_ISC_FU_TYPE_NONE:
	case FEDM_ISC_FU_TYPE_ANT:
	case FEDM_ISC_FU_TYPE_DAT:
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
		
	case FEDM_ISC_FU_TYPE_MUX:
	case FEDM_ISC_FU_TYPE_UMUX:
		if(m_ChildList.size() != 8)
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);

		if(iOutNo<1 || iOutNo>8)
			FEDM_RETURN(FEDM_ERROR_PARAMETER);

		if(m_ChildList[iOutNo-1] != NULL)
		{
			m_ChildList[iOutNo-1]->DelChildList();
			if(m_bManageChilds)
				delete m_ChildList[iOutNo-1];
			m_ChildList[iOutNo-1] = NULL;
		}

		break;
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	07.01.2005 / M. Hultsch
  Version       :	02.06.00 / 28.03.2008 / M. Hultsch

  Function		:	clears the child list and frees the memory
					## recursive function ##

  Parameter		:	-

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCFunctionUnit::DelChildList()
{
	int cnt;

	switch(m_iFUType)
	{
	case FEDM_ISC_FU_TYPE_NONE:
	case FEDM_ISC_FU_TYPE_ANT:
	case FEDM_ISC_FU_TYPE_DAT:
		break;
		
	case FEDM_ISC_FU_TYPE_MUX:
	case FEDM_ISC_FU_TYPE_UMUX:
		if(m_ChildList.size() != 8)
			FEDM_RETURN(FEDM_ERROR_NO_TABLE_SIZE);

		for(cnt=1; cnt<=8; ++cnt)
			DelChild(cnt);

		break;
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	07.01.2005 / M. Hultsch
  Version       :	02.06.00 / 28.03.2008 / M. Hultsch

  Function		:	counts the number of members in the child list

  Parameter		:	-

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_ISCFunctionUnit::GetChildCount()
{
	switch(m_iFUType)
	{
	case FEDM_ISC_FU_TYPE_NONE:
	case FEDM_ISC_FU_TYPE_ANT:
	case FEDM_ISC_FU_TYPE_DAT:
		break;
		
	case FEDM_ISC_FU_TYPE_MUX:
	case FEDM_ISC_FU_TYPE_UMUX:
		if(m_ChildList.size() == 8)
			return 8;

		break;
	}

	return 0;
}

/***************************************************************************
  Begin        	:	07.01.2005 / M. Hultsch
  Version       :	02.06.00 / 28.03.2008 / M. Hultsch

  Function		:	get a child from the child list

  Parameter		:	int iOutNo	-	output number of function unit

  Return Value	:	pointer to child or NULL
***************************************************************************/
FEDM_ISCFunctionUnit* FEDM_ISCFunctionUnit::GetChild(int iOutNo)
{
	FEDM_ISCFunctionUnit* pFU = NULL;

	switch(m_iFUType)
	{
	case FEDM_ISC_FU_TYPE_NONE:
	case FEDM_ISC_FU_TYPE_ANT:
	case FEDM_ISC_FU_TYPE_DAT:
		break;
	
	case FEDM_ISC_FU_TYPE_MUX:
	case FEDM_ISC_FU_TYPE_UMUX:
		if(iOutNo<1 || iOutNo>8)
			return NULL;

		if(m_ChildList.size() == 8)
			pFU = m_ChildList[iOutNo-1];

		break;
	}

	return pFU;
}



//////////////////////////////////////////////////////////////////////
// protected functions
//////////////////////////////////////////////////////////////////////

void FEDM_ISCFunctionUnit::SetLastError(int iError)
{
	m_iLastError = iError;
}


#endif // #if !defined(_FEDM_NO_FU_SUPPORT)
