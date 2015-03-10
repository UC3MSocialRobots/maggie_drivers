/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO14443_4.cpp                 |
|                                                       |
---------------------------------------------------------

Copyright © 2009-2011	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	01.04.2009

Version       		:	03.03.02 / 29.04.2011 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID® classic-pro transponder ISO14443-4

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "FedmIscTagHandler_ISO14443_4.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"

//#ifdef _FEDM_WINDOWS
	#include "fetcl.h"
//#endif


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscTagHandler_ISO14443_4
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_ISO14443_4::FedmIscTagHandler_ISO14443_4(
	FEDM_ISCReader* pReader, 
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_3(pReader, FedmIscTagHandler::TYPE_ISO14443_4, pTabItem)
{
	m_iFetclHnd = 0;

	m_cbFct	= NULL;
	m_pAny	= NULL;

	if(m_pTabItem != NULL)
	{
		if(m_pTabItem->m_ucTrType == FEDM_ISC_TR_TYPE_ISO14443A)
			m_sTagName = "ISO 14443-4 Type A";
		else if(m_pTabItem->m_ucTrType == FEDM_ISC_TR_TYPE_ISO14443B)
			m_sTagName = "ISO 14443-4 Type B";

		if(m_pReader != NULL)
		{
			// create default transponder without CID and without NAD
			m_iFetclHnd = FETCL_NewTransponder(	m_pReader->GetReaderHnd(),
												m_pReader->GetBusAddress(),
												0,
												0,
												false,
												false );
		}
	}

	Init();
}

FedmIscTagHandler_ISO14443_4::FedmIscTagHandler_ISO14443_4(
	FEDM_ISCReader* pReader, 
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_3(pReader, uiTagHandlerType, pTabItem)
{
	m_iFetclHnd = 0;

	m_cbFct	= NULL;
	m_pAny	= NULL;

	if(m_pTabItem != NULL)
	{
		if(m_pTabItem->m_ucTrType == FEDM_ISC_TR_TYPE_ISO14443A)
			m_sTagName = "ISO 14443-4 Type A";
		else if(m_pTabItem->m_ucTrType == FEDM_ISC_TR_TYPE_ISO14443B)
			m_sTagName = "ISO 14443-4 Type B";

		if(m_pReader != NULL)
		{
			// create default transponder without CID and without NAD
			m_iFetclHnd = FETCL_NewTransponder(	m_pReader->GetReaderHnd(),
												m_pReader->GetBusAddress(),
												0,
												0,
												false,
												false );
		}
	}

	Init();
}

FedmIscTagHandler_ISO14443_4::~FedmIscTagHandler_ISO14443_4()
{
	if(m_iFetclHnd > 0)
		FETCL_DeleteTransponder(m_iFetclHnd);
}

// set all data members to 0
void FedmIscTagHandler_ISO14443_4::Init()
{
	FedmIscTagHandler::Init();

	m_ucCID		= 0;
	m_ucNAD		= 0;
	m_bUseCID	= false;
	m_bUseNAD	= false;

	m_uiResponseLen = 0;

	m_ucISO14443ErrorCode = 0;
}

unsigned char FedmIscTagHandler_ISO14443_4::GetLastISOErrorCode()
{
	return m_ucISO14443ErrorCode;
}

void FedmIscTagHandler_ISO14443_4::SetCID(unsigned char ucCID)
{
	m_ucCID = ucCID;
}

int FedmIscTagHandler_ISO14443_4::EnableCID(bool bEnable)
{
	m_bUseCID = bEnable;

	if(m_iFetclHnd > 0)
		FETCL_DeleteTransponder(m_iFetclHnd);

	// create transponder with or without CID and with or without NAD
	m_iFetclHnd = FETCL_NewTransponder(	m_pReader->GetReaderHnd(),
										m_pReader->GetBusAddress(),
										m_ucCID,
										m_ucNAD,
										m_bUseCID,
										m_bUseNAD );

	if(m_iFetclHnd < 0)
		return m_iFetclHnd;

	return 0;
}

unsigned char FedmIscTagHandler_ISO14443_4::GetCID()
{
	return m_ucCID;
}

bool FedmIscTagHandler_ISO14443_4::IsCIDEnabled()
{
	return m_bUseCID;
}


void FedmIscTagHandler_ISO14443_4::SetNAD(unsigned char ucNAD)
{
	m_ucNAD = ucNAD;
}

int FedmIscTagHandler_ISO14443_4::EnableNAD(bool bEnable)
{
	m_bUseNAD = bEnable;

	if(m_iFetclHnd > 0)
		FETCL_DeleteTransponder(m_iFetclHnd);

	// create transponder with or without CID and with or without NAD
	m_iFetclHnd = FETCL_NewTransponder(	m_pReader->GetReaderHnd(),
										m_pReader->GetBusAddress(),
										m_ucCID,
										m_ucNAD,
										m_bUseCID,
										m_bUseNAD );

	if(m_iFetclHnd < 0)
		return m_iFetclHnd;

	return 0;
}

unsigned char FedmIscTagHandler_ISO14443_4::GetNAD()
{
	return m_ucNAD;
}

bool FedmIscTagHandler_ISO14443_4::IsNADEnabled()
{
	return m_bUseNAD;
}



/***************************************************************************
  Begin        		:	31.03.2009 / M. Hultsch

  Version       	:	03.01.00 / 11.05.2009 / M. Hultsch

  Function			:	[0xB2][0x2B] Transponder Info

						NOTE: only for selected tags

  Parameters		:	unsigned char ucFSCI		- [out]
						unsigned char ucFWI			- [out]
						unsigned char ucDSI			- [out]
						unsigned char ucDRI			- [out]
						unsigned char ucNADSupport	- [out]
						unsigned char ucCIDSupport	- [out]

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4::GetTransponderInfo(
	unsigned char& ucFSCI,
	unsigned char& ucFWI,
	unsigned char& ucDSI,
	unsigned char& ucDRI,
	unsigned char& ucNADSupport,
	unsigned char& ucCIDSupport )
{
	FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__);

	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B2_CMD, (unsigned char)0x2B));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B2_MODE, (unsigned char)FEDM_ISC_ISO_MODE_SEL));

	m_ucISO14443ErrorCode = 0;

	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB2);
	if(iBack == 0)
	{
		int iIdx = m_pReader->FindTableIndex(0, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_IS_SELECTED, true);
		if(iIdx < 0)
		{
			// exception for ISO 14443 Transponder, when Reader is configuered with Anticollision = off
			if(  m_pReader->m_uiISOTableLength == 1 && (m_pReader->m_ISOTable[0]->m_ucTrType == FEDM_ISC_TR_TYPE_ISO14443A || m_pReader->m_ISOTable[0]->m_ucTrType == FEDM_ISC_TR_TYPE_ISO14443B) )
				iIdx = 0;
			else
				return FEDM_ERROR_NO_TABLE_DATA;
		}

		FEDM_CHK1(iErr, m_pReader->GetTableData(iIdx, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_FSCI, &ucFSCI));
		FEDM_CHK1(iErr, m_pReader->GetTableData(iIdx, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_FWI, &ucFWI));
		FEDM_CHK1(iErr, m_pReader->GetTableData(iIdx, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_DSI, &ucDSI));
		FEDM_CHK1(iErr, m_pReader->GetTableData(iIdx, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_DRI, &ucDRI));
		FEDM_CHK1(iErr, m_pReader->GetTableData(iIdx, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_NAD, &ucNADSupport));
		FEDM_CHK1(iErr, m_pReader->GetTableData(iIdx, FEDM_ISC_ISO_TABLE, FEDM_ISC_DATA_CID, &ucCIDSupport));
	}
	else if(iBack == 0x96)
	{
		// ISO 14443 Error
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B2_ISO_ERROR, &m_ucISO14443ErrorCode));
	}

	return iBack;
}


/***************************************************************************
  Begin        		:	31.03.2009 / M. Hultsch

  Version       	:	03.03.02 / 28.04.2011 / M. Hultsch

  Function			:	[0xB2][0xBE] T=CL APDU - synchronous call

						NOTE: only for selected tags

  Parameters		:	unsigned char*	pucRequestData		- [in] buffer with APDU request
						unsigned int	uiRequestLen		- [in] length of APDU (number of bytes)
						unsigned char*	pucResponseData		- [out] for synchronous mode: buffer for APDU response
						unsigned int	uiResponseBufferLen	- [in]  for synchronous mode: length of response buffer
						unsigned int&	uiResponseLen		- [out] length of APDU response (number of bytes)

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4::Apdu(
	unsigned char*	pucRequestData,
	unsigned int	uiRequestLen,
	unsigned char*	pucResponseData,
	unsigned int	uiResponseBufferLen,
	unsigned int&	uiResponseLen )
{
	FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__ << " sync call");

	FEDM_CHK3(pucRequestData);
	FEDM_CHK3(pucResponseData);

	if(m_iFetclHnd < 0)
		return m_iFetclHnd;

	int iBack = 0;
	int iErr = 0;

	m_ucISO14443ErrorCode = 0;

	// sychronous invoke
	iBack = FETCL_Apdu(m_iFetclHnd, pucRequestData, uiRequestLen, NULL);
	m_pReader->SetLastError(iBack);
	if(iBack != 0 && iBack != 0x96)
		return iBack;

	int iResponseLen = FETCL_GetResponseData(m_iFetclHnd, pucResponseData, uiResponseBufferLen);
	if(iResponseLen < 0)
	{
		m_pReader->SetLastError(iResponseLen);
		return iResponseLen;
	}
	else if(iResponseLen == 0)
	{
		m_pReader->SetLastError(FEDM_ERROR_NO_DATA);
		return FEDM_ERROR_NO_DATA;
	}

	uiResponseLen = (unsigned int)iResponseLen;

	if(iBack == 0x96)
	{
		// ISO 14443 Error
		m_ucISO14443ErrorCode = pucResponseData[0];
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B2_ISO_ERROR, m_ucISO14443ErrorCode));
	}

	return iBack;
}


/***************************************************************************
  Begin        		:	31.03.2009 / M. Hultsch

  Version       	:	03.03.02 / 28.04.2011 / M. Hultsch

  Function			:	[0xB2][0xBE] T=CL APDU - asynchronous call

						NOTE: only for selected tags

  Parameters		:	unsigned char*	pucRequestData		- [in] buffer with APDU request
						unsigned int	uiRequestLen		- [in] length of APDU (number of bytes)
						void* pAny							- [in] pojnter to anything reflected with the first parameter of callback
						void (*cbFct)(void*, int, int, int)	- [in] callback function

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4::Apdu(
	unsigned char*	pucRequestData,
	unsigned int	uiRequestLen,
	void* pAny,
	void (*cbFct)(void*, int, int, int) )
{
	FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__ << " async call");

	FEDM_CHK3(pucRequestData);

	if(m_iFetclHnd < 0)
		return m_iFetclHnd;

	int iBack = 0;
	int iErr = 0;
	int iResponseLen = 0;
	unsigned char ucResponseData = 0;
	FETCL_EVENT_INIT Init;

	// asychronous invoke
	m_cbFct = cbFct;
	m_pAny	= pAny;

	Init.pAny	= this;
	Init.uiFlag = FETCL_CALLBACK2;
#if defined(__BORLANDC__)
	Init.cbFct2	= (void(*)(void*,int,int,int))ApduCallback;
#else
	Init.cbFct2	= ApduCallback;
#endif

	m_ucISO14443ErrorCode = 0;

	iBack = FETCL_Apdu(m_iFetclHnd, pucRequestData, uiRequestLen, &Init);
	m_pReader->SetLastError(iBack);
	if(iBack == 0x96)
	{
		iResponseLen = FETCL_GetResponseData(m_iFetclHnd, &ucResponseData, 1);
		if(iResponseLen < 0)
		{
			m_pReader->SetLastError(iResponseLen);
			return iResponseLen;
		}
		else if(iResponseLen == 0)
		{
			m_pReader->SetLastError(FEDM_ERROR_NO_DATA);
			return FEDM_ERROR_NO_DATA;
		}

		// ISO 14443 Error
		m_ucISO14443ErrorCode = ucResponseData;
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B2_ISO_ERROR, m_ucISO14443ErrorCode));
	}

	return iBack;
}

// [0xB2][0xBE] T=CL Ping
int FedmIscTagHandler_ISO14443_4::Ping()
{
	FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__);

	if(m_iFetclHnd < 0)
		return m_iFetclHnd;

	int iBack = 0;
	int iErr = 0;
	int iResponseLen = 0;
	unsigned char ucResponseData = 0;

	m_ucISO14443ErrorCode = 0;

	iBack = FETCL_Ping(m_iFetclHnd);
	m_pReader->SetLastError(iBack);
	if(iBack == 0x96)
	{
		iResponseLen = FETCL_GetResponseData(m_iFetclHnd, &ucResponseData, 1);
		if(iResponseLen < 0)
		{
			m_pReader->SetLastError(iResponseLen);
			return iResponseLen;
		}
		else if(iResponseLen == 0)
		{
			m_pReader->SetLastError(FEDM_ERROR_NO_DATA);
			return FEDM_ERROR_NO_DATA;
		}

		// ISO 14443 Error
		m_ucISO14443ErrorCode = ucResponseData;
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B2_ISO_ERROR, m_ucISO14443ErrorCode));
	}

	return iBack;
}

// [0xB2][0xBE] T=CL Deselect
int FedmIscTagHandler_ISO14443_4::Deselect()
{
	FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__);

	if(m_iFetclHnd < 0)
		return m_iFetclHnd;

	int iBack = 0;
	int iErr = 0;
	int iResponseLen = 0;
	unsigned char ucResponseData = 0;

	m_ucISO14443ErrorCode = 0;

	iBack = FETCL_Deselect(m_iFetclHnd);
	m_pReader->SetLastError(iBack);
	if(iBack == 0x96)
	{
		iResponseLen = FETCL_GetResponseData(m_iFetclHnd, &ucResponseData, 1);
		if(iResponseLen < 0)
		{
			m_pReader->SetLastError(iResponseLen);
			return iResponseLen;
		}
		else if(iResponseLen == 0)
		{
			m_pReader->SetLastError(FEDM_ERROR_NO_DATA);
			return FEDM_ERROR_NO_DATA;
		}

		// ISO 14443 Error
		m_ucISO14443ErrorCode = ucResponseData;
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B2_ISO_ERROR, m_ucISO14443ErrorCode));
	}

	return iBack;
}


/***************************************************************************
  Begin        		:	31.03.2009 / M. Hultsch

  Version       	:	03.01.00 / 11.05.2009 / M. Hultsch

  Function			:	query of APDU response after asynchronous call

  Parameters		:	unsigned char*	pucResponseData		- [out] for synchronous mode: buffer for APDU response
						unsigned int	uiResponseBufferLen	- [in]  for synchronous mode: legth of response bufer
						unsigned int&	uiResponseLen		- [out] length of APDU response (number of bytes)

  Return value		:	0				- if OK
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4::GetResponseData(
	unsigned char*	pucResponseData,
	unsigned int	uiResponseBufferLen,
	unsigned int&	uiResponseLen )
{
	FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__);

	FEDM_CHK3(pucResponseData);

	if(uiResponseBufferLen < m_uiResponseLen)
	{
		m_pReader->SetLastError(FEDM_ERROR_BUFFER_LENGTH);
		return FEDM_ERROR_BUFFER_LENGTH;
	}

	memcpy(pucResponseData, m_ucResponseData, m_uiResponseLen);
	uiResponseLen = m_uiResponseLen;
	return 0;
}


/***************************************************************************
  Begin        		:	31.03.2009 / M. Hultsch

  Version       	:	03.03.02 / 29.04.2011 / M. Hultsch

  Function			:	APDU callback for asynchronous call

  Parameters		:	void* pAny			- [in] pojnter to this
						int iApduHnd		- [in] handle of FETCL library
						int iError			- [in] error code
						int iDataLength		- [in] length of APDU response

  Return value		:	-
***************************************************************************/
void FedmIscTagHandler_ISO14443_4::ApduCallback(void* pAny, int iApduHnd, int iError, int iDataLength)
{
	if(pAny == NULL)
		return;

	FEDM_TRACE(((FedmIscTagHandler_ISO14443_4*)pAny)->m_pReader->GetLogger(), FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__);

	if(iApduHnd != ((FedmIscTagHandler_ISO14443_4*)pAny)->m_iFetclHnd)
		return;

	int iResponseLen = 0;

	((FedmIscTagHandler_ISO14443_4*)pAny)->m_pReader->SetLastError(iError);

	if(iError < 0)
	{
		((FedmIscTagHandler_ISO14443_4*)pAny)->m_cbFct(((FedmIscTagHandler_ISO14443_4*)pAny)->m_pAny, iApduHnd, iError, iDataLength);
	}
	else if(iError == 0x96)
	{
		iResponseLen = FETCL_GetResponseData(iApduHnd, ((FedmIscTagHandler_ISO14443_4*)pAny)->m_ucResponseData, FEDM_ISO14443_4_APDU_BUFFER_SIZE);
		if(iResponseLen < 0)
		{
			((FedmIscTagHandler_ISO14443_4*)pAny)->m_pReader->SetLastError(iResponseLen);
			((FedmIscTagHandler_ISO14443_4*)pAny)->m_cbFct(((FedmIscTagHandler_ISO14443_4*)pAny)->m_pAny, iApduHnd, iResponseLen, 0);
		}
		else if(iResponseLen == 0)
		{
			((FedmIscTagHandler_ISO14443_4*)pAny)->m_pReader->SetLastError(FEDM_ERROR_NO_DATA);
			((FedmIscTagHandler_ISO14443_4*)pAny)->m_cbFct(((FedmIscTagHandler_ISO14443_4*)pAny)->m_pAny, iApduHnd, FEDM_ERROR_NO_DATA, 0);
		}
		else
		{
			// ISO 14443 Error
			((FedmIscTagHandler_ISO14443_4*)pAny)->m_ucISO14443ErrorCode = ((FedmIscTagHandler_ISO14443_4*)pAny)->m_ucResponseData[0];
			((FedmIscTagHandler_ISO14443_4*)pAny)->m_pReader->SetData(FEDM_ISC_TMP_B2_ISO_ERROR, ((FedmIscTagHandler_ISO14443_4*)pAny)->m_ucISO14443ErrorCode);
			((FedmIscTagHandler_ISO14443_4*)pAny)->m_cbFct(((FedmIscTagHandler_ISO14443_4*)pAny)->m_pAny, iApduHnd, iError, iDataLength);
		}
	}
	else if(iError == 0)
	{
		iResponseLen = FETCL_GetResponseData(iApduHnd, ((FedmIscTagHandler_ISO14443_4*)pAny)->m_ucResponseData, FEDM_ISO14443_4_APDU_BUFFER_SIZE);
		if(iResponseLen < 0)
		{
			((FedmIscTagHandler_ISO14443_4*)pAny)->m_cbFct(((FedmIscTagHandler_ISO14443_4*)pAny)->m_pAny, iApduHnd, iResponseLen, 0);
		}
		else if(iResponseLen == 0)
		{
			((FedmIscTagHandler_ISO14443_4*)pAny)->m_cbFct(((FedmIscTagHandler_ISO14443_4*)pAny)->m_pAny, iApduHnd, FEDM_ERROR_NO_DATA, 0);
		}
		else
		{
			((FedmIscTagHandler_ISO14443_4*)pAny)->m_cbFct(((FedmIscTagHandler_ISO14443_4*)pAny)->m_pAny, iApduHnd, iError, iDataLength);
		}
	}
	else
	{
		((FedmIscTagHandler_ISO14443_4*)pAny)->m_cbFct(((FedmIscTagHandler_ISO14443_4*)pAny)->m_pAny, iApduHnd, iError, iDataLength);
	}
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
