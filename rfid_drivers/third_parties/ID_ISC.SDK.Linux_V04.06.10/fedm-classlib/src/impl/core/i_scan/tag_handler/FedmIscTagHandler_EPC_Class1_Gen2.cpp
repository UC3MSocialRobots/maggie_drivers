/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_EPC_Class1_Gen2.cpp            |
|                                                       |
---------------------------------------------------------

Copyright © 2009-2013	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	30.03.2009

Version       		:	04.04.03 / 21.02.2013 / M. Hultsch
						- bugfix in WriteEPC: return of EPC-length as error code fixed

						04.03.01 / 14.11.2012 / M. Hultsch
						- Kill with extended parameter list for support of Recommissioning Bits
						- new methods: GetTagModelNumber()
									   GetMaskDesignerID()
									   GetMaskDesignerName()
									   IsUidWithTid()
									   IsExtendedPC_W1()
									   GetExtendedProtocolControlW1()

						04.02.04 / 19.06.2012 / M. Hultsch
						- new methods: BlockPermalock and ReadPermalockStatus

						04.02.00 / 11.05.2012 / M. Hultsch
						- accept a length of 0 for EPC in GetTidOfUid()

						04.00.10 / 14.02.2012 / M. Hultsch
						- check of string length of EPC and password in WriteEPC
						- check of string length of password in all other commands
						- support for external logmanager (FELOGMNG.dll), when #define _FELOGMNG_SUPPORT is set

						04.00.07 / 12.12.2011 / M. Hultsch
						- new method: simplified Lock

						04.00.06 / 01.12.2011 / M. Hultsch
						- bugfix in methods: ReadCompleteBank and ReadMultipleBlocks

						04.00.03 / 26.09.2011 / M. Hultsch
						- bugfix in method: ReadCompleteBank

						04.00.02 / 11.08.2011 / M. Hultsch
						- new method: ReadCompleteBank

						04.00.00 / 21.07.2011 / M. Hultsch
						- new methods: GetProtocolControl
									   GetEpcOfUid
									   GetTidOfUid

						03.03.04 / 16.06.2011 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID i-scan® HF/UHF transponder EPC Class 1 Generation 2

NOTE				:	this class supports only addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_NO_TAG_HANDLER_EPC_C1_G2)

#include "FedmIscTagHandler_EPC_Class1_Gen2.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"
#include "../../FEDM_Functions.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif


// constants for Bank
#if _MSC_VER <= 1200
const unsigned int FedmIscTagHandler_EPC_Class1_Gen2::BANK_RESERVED	= 0;
const unsigned int FedmIscTagHandler_EPC_Class1_Gen2::BANK_EPC		= 1;
const unsigned int FedmIscTagHandler_EPC_Class1_Gen2::BANK_TID		= 2;
const unsigned int FedmIscTagHandler_EPC_Class1_Gen2::BANK_USER		= 3;

	// constants for [0x22] Lock
const unsigned char FedmIscTagHandler_EPC_Class1_Gen2::UNCHANGED			= 0x00;
const unsigned char FedmIscTagHandler_EPC_Class1_Gen2::UNLOCK				= 0x20;
const unsigned char FedmIscTagHandler_EPC_Class1_Gen2::UNLOCK_PERMANENTLY	= 0x31;
const unsigned char FedmIscTagHandler_EPC_Class1_Gen2::LOCK					= 0x22;
const unsigned char FedmIscTagHandler_EPC_Class1_Gen2::LOCK_PERMANENTLY		= 0x33;
#endif

//####################################################################
// class FedmIscTagHandler_EPC_Class1_Gen2
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_EPC_Class1_Gen2::FedmIscTagHandler_EPC_Class1_Gen2(
	FEDM_ISCReader* pReader,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler(pReader, FedmIscTagHandler::TYPE_EPC_CLASS1_GEN2, pTabItem)
{
	m_sTagName = "EPC_Class1_Gen2";
}

// protected constructor for internal use
FedmIscTagHandler_EPC_Class1_Gen2::FedmIscTagHandler_EPC_Class1_Gen2(
	FEDM_ISCReader* pReader,
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem)
	: FedmIscTagHandler(pReader, uiTagHandlerType, pTabItem)
{
	m_sTagName = "EPC_Class1_Gen2";
}

FedmIscTagHandler_EPC_Class1_Gen2::~FedmIscTagHandler_EPC_Class1_Gen2()
{
}

// set all data members to 0
/*void FedmIscTagHandler_EPC_Class1_Gen2::Init()
{
}*/

bool FedmIscTagHandler_EPC_Class1_Gen2::IsUidWithTid()
{
	if(m_pTabItem->m_ucIDDT == 0x02)
		return true;

	return false;
}

bool FedmIscTagHandler_EPC_Class1_Gen2::IsExtendedPC_W1()
{
	return m_pTabItem->IsExtendedPC_W1();
}

/***************************************************************************
  Begin        		:	16.06.2011 / M. Hultsch

  Version       	:	03.03.04 / 16.06.2011 / M. Hultsch

  Function			:	return the Protocol Control

  Parameters		:	-

  Return value		:	Protocol Control as unsigned int
***************************************************************************/
unsigned int FedmIscTagHandler_EPC_Class1_Gen2::GetProtocolControl()
{
	return (unsigned int)m_pTabItem->m_ucClass1Gen2_PC[0] + ((unsigned int)(m_pTabItem->m_ucClass1Gen2_PC[1]) << 8);
}

/***************************************************************************
  Begin        		:	14.11.2012 / M. Hultsch

  Version       	:	04.03.01 / 14.11.2012 / M. Hultsch

  Function			:	return the Extended Protocol Control XPC_W1

  Parameters		:	-

  Return value		:	XPC_W1 as unsigned int
***************************************************************************/
unsigned int FedmIscTagHandler_EPC_Class1_Gen2::GetExtendedProtocolControlW1()
{
	return (unsigned int)m_pTabItem->m_ucClass1Gen2_XPC_W1[0] + ((unsigned int)(m_pTabItem->m_ucClass1Gen2_XPC_W1[1]) << 8);
}


/***************************************************************************
  Begin        		:	15.07.2011 / M. Hultsch

  Version       	:	04.00.00 / 21.07.2011 / M. Hultsch

  Function			:	return the EPC, extracted from UID

  Parameters		:	-

  Return value		:	EPC or "" in error case
***************************************************************************/
const char* FedmIscTagHandler_EPC_Class1_Gen2::GetEpcOfUid()
{
	int iBack = FEDM_ConvHexUCharToHexStr(m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen, m_sUid);
	if(iBack)
		return "";

	unsigned char ucEpcLen = ((m_pTabItem->m_ucClass1Gen2_PC[0] & 0xF8) >> 3) * 2;
	if(ucEpcLen == 0)
		return "";

	m_sEpc = m_sUid.substr(0, ucEpcLen*2);
	return m_sEpc.c_str();
}

/***************************************************************************
  Begin        		:	15.07.2011 / M. Hultsch

  Version       	:	04.02.00 / 11.05.2012 / M. Hultsch
						- accept a length of 0 for EPC

						04.00.00 / 21.07.2011 / M. Hultsch

  Function			:	return the TID, extracted from UID
						Note: returns "", if TID is not part of UID

  Parameters		:	-

  Return value		:	TID or "" in error case
***************************************************************************/
const char* FedmIscTagHandler_EPC_Class1_Gen2::GetTidOfUid()
{
	int iBack = FEDM_ConvHexUCharToHexStr(m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen, m_sUid);
	if(iBack)
		return "";

	unsigned char ucEpcLen = ((m_pTabItem->m_ucClass1Gen2_PC[0] & 0xF8) >> 3) * 2;

	if( size_t(ucEpcLen*2) >= m_sUid.size() )
		return "";

	m_sTid = m_sUid.substr(ucEpcLen*2, m_sUid.size() - ucEpcLen*2);
	return m_sTid.c_str();
}


/***************************************************************************
  Begin        		:	09.11.2012 / M. Hultsch

  Version       	:	04.03.01 / 09.11.2012 / M. Hultsch

  Function			:	return the Tag Model Number, based on information from TID

  Parameters		:	-

  Return value		:	Tag Model Number
***************************************************************************/
unsigned int FedmIscTagHandler_EPC_Class1_Gen2::GetTagModelNumber()
{
	return m_pTabItem->GetEpcC1G2TagModelNumber();
}


/***************************************************************************
  Begin        		:	09.11.2012 / M. Hultsch

  Version       	:	04.03.01 / 09.11.2012 / M. Hultsch

  Function			:	return the Mask Designer ID, based on information from TID

  Parameters		:	-

  Return value		:	Mask Designer ID
***************************************************************************/
unsigned int FedmIscTagHandler_EPC_Class1_Gen2::GetMaskDesignerID()
{
	return m_pTabItem->GetEpcC1G2MaskDesignerID();
}


/***************************************************************************
  Begin        		:	09.11.2012 / M. Hultsch

  Version       	:	04.03.01 / 09.11.2012 / M. Hultsch

  Function			:	return the Chip Manufacturer Name, based on Mask Designer ID from TID

  Parameters		:	-

  Return value		:	Chip Manufacturer Name
***************************************************************************/
const char* FedmIscTagHandler_EPC_Class1_Gen2::GetMaskDesignerName()
{
	return m_pTabItem->GetEpcC1G2MaskDesignerName();
}


/***************************************************************************
  Begin        		:	11.05.2009 / M. Hultsch

  Version       	:	04.00.10 / 13.02.2012 / M. Hultsch
						- check of string length of password

						04.00.06 / 01.12.2011 / M. Hultsch
						- bugfix for copy of data

						04.00.03 / 26.09.2011 / M. Hultsch

  Function			:	[0xB0][0x23] Read Multiple Blocks command
						with READ_COMPLETE_BANK option
						NOTE: block size is always 2

  Parameters		:	unsigned int uiBank						- [in] bank number
						string sPassword						- [in] optional password
						unsigned char* pucData					- [out] buffer with data blocks
						unsigned int uiDataBufferSize			- [in] size of buffer for data blocks (in Bytes)
						unsigned int& uiNoOfDataBlocksRead		- [out] number of data blocks read

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_EPC_Class1_Gen2::ReadCompleteBank(
		unsigned int uiBank,
		std::string sPassword,
		unsigned char* pucData,
		unsigned int uiDataBufferSize,
		unsigned int& uiNoOfDataBlocksRead )
{
	FEDM_TRACE_FNC(m_pReader->GetLogger(), FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "Bank=" << uiBank);

	FEDM_CHK3(pucData);

	int iErr = 0;
	unsigned int uiBlockAddress = 0;
	unsigned int uiNoOfDataBlocks = 0;

	uiNoOfDataBlocksRead = 0;

	// test buffer size
	if(uiDataBufferSize == 0)
	{
		FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - DataBufferSize is zero");
		return FEDM_ERROR_PARAMETER;
	}

	// string with password must be even
	if(sPassword.length() > 0 && sPassword.length() % 2)
	{
		FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - Invalid length of Password (=" << sPassword.length() << ")");
		return FEDM_ERROR_ODD_STRING_LENGTH;
	}

	// test bank number
	switch(uiBank)
	{
	case FEDM_ISC_ISO_BANK_RESERVED:
	case FEDM_ISC_ISO_BANK_EPC_MEMORY:
	case FEDM_ISC_ISO_BANK_TID_MEMORY:
	case FEDM_ISC_ISO_BANK_USER_MEMORY:
		break;
	default:
		FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - Invalid Bank Number");
		return FEDM_ERROR_PARAMETER;
	}

	// prepare ISO Host Command
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE, (unsigned char)0));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_EXT_ADR, true));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_READ_COMPLETE_BANK, true));

	if(m_bNonAddressedMode)
	{
//		m_pReader->SetNonAddressedTabIndex(m_pTabItem->m_uiTabIndex);
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		if(m_pTabItem->m_ucSnrLen != 8)
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_UID_LF, true));
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_UID_LEN, m_pTabItem->m_ucSnrLen));
		}
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_BANK, (unsigned char)0));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_BANK_BANK_NR, uiBank));

	if(sPassword.length() > 0)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_BANK_ACCESS_FLAG, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_ACCESS_PW_LENGTH, (unsigned char)(sPassword.length()/2)));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_ACCESS_PW, sPassword));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DB_ADR_EXT, (unsigned int)0)); // unused, but must be in protocol
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DBN, (unsigned char)0)); // unused, but must be in protocol
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char)0x23));
	
	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB0);
	while(	iBack == 0x00 ||	// OK
			iBack == 0x94)		// more data
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B0_RSP_DBN, &uiNoOfDataBlocks));

		if( (uiNoOfDataBlocks * 2) > uiDataBufferSize)
		{
			// copy received data blocks into buffer (up to buffer size)
			switch(uiBank)
			{
			case FEDM_ISC_ISO_BANK_RESERVED:
				memcpy( (pucData + uiBlockAddress*2), &m_pTabItem->m_ucRxDB_ResBank[uiBlockAddress*2], uiDataBufferSize);
				break;
			case FEDM_ISC_ISO_BANK_EPC_MEMORY:
				memcpy( (pucData + uiBlockAddress*2), &m_pTabItem->m_ucRxDB_EpcBank[uiBlockAddress*2], uiDataBufferSize);
				break;
			case FEDM_ISC_ISO_BANK_TID_MEMORY:
				memcpy( (pucData + uiBlockAddress*2), &m_pTabItem->m_ucRxDB_TidBank[uiBlockAddress*2], uiDataBufferSize);
				break;
			case FEDM_ISC_ISO_BANK_USER_MEMORY:
				memcpy( (pucData + uiBlockAddress*2), &m_pTabItem->m_ucRxDB[uiBlockAddress*2], uiDataBufferSize);
				break;
			}

			uiNoOfDataBlocksRead = (uiDataBufferSize >> 1);

			// return an error because of too small buffer
			FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - DataBuffer is too small");
			return FEDM_ERROR_BUFFER_LENGTH;
		}

		// copy received data blocks into buffer
		switch(uiBank)
		{
		case FEDM_ISC_ISO_BANK_RESERVED:
			memcpy( (pucData + uiBlockAddress*2), &m_pTabItem->m_ucRxDB_ResBank[uiBlockAddress*2], uiNoOfDataBlocks * 2);
			break;
		case FEDM_ISC_ISO_BANK_EPC_MEMORY:
			memcpy( (pucData + uiBlockAddress*2), &m_pTabItem->m_ucRxDB_EpcBank[uiBlockAddress*2], uiNoOfDataBlocks * 2);
			break;
		case FEDM_ISC_ISO_BANK_TID_MEMORY:
			memcpy( (pucData + uiBlockAddress*2), &m_pTabItem->m_ucRxDB_TidBank[uiBlockAddress*2], uiNoOfDataBlocks * 2);
			break;
		case FEDM_ISC_ISO_BANK_USER_MEMORY:
			memcpy( (pucData + uiBlockAddress*2), &m_pTabItem->m_ucRxDB[uiBlockAddress*2], uiNoOfDataBlocks * 2);
			break;
		}

		uiNoOfDataBlocksRead += uiNoOfDataBlocks;
		uiBlockAddress += uiNoOfDataBlocks;

		if(iBack == 0)
			break;

		// request next data blocks
		// NOTE: block address is unused by reader, but important to add returned data into table,
		//		 because the start address is not part of the response protocol !!
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DB_ADR_EXT, uiBlockAddress));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_MORE, true));
		iBack = m_pReader->SendProtocol(0xB0);
	}

	if (iBack == 0x95)
	{
		// ISO Error
		m_pReader->GetData(FEDM_ISC_TMP_B0_ISO_ERROR, &m_ucISOErrorCode);
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DB_ADR_EXT, (unsigned int)0));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_MORE, false));

	return iBack;
}


/***************************************************************************
  Begin        		:	11.05.2009 / M. Hultsch

  Version       	:	04.00.10 / 13.02.2012 / M. Hultsch
						- check of string length of password

						04.00.06 / 01.12.2011 / M. Hultsch
						- bugfix for copy of data

						03.01.00 / 11.05.2009 / M. Hultsch

  Function			:	[0xB0][0x23] Read Multiple Blocks command
						with support for extended address mode
						NOTE: block size is always 2

  Parameters		:	unsigned int uiBank					- [in] bank number
						unsigned int uiFirstDataBlock		- [in] first data block address
						unsigned int uiNoOfDataBlocks		- [in] number of requested data blocks
						string sPassword					- [in] optional password
						unsigned char* pucData				- [out] buffer with data blocks

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_EPC_Class1_Gen2::ReadMultipleBlocks(
		unsigned int uiBank,
		unsigned int uiFirstDataBlock,
		unsigned int uiNoOfDataBlocks,
		string sPassword,
		unsigned char* pucData )
{
	FEDM_TRACE_FNC(m_pReader->GetLogger(), FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "Bank=" << uiBank << ", FirstDataBlock=" << uiFirstDataBlock << ", NoOfDataBlocks" << uiNoOfDataBlocks);

	FEDM_CHK3(pucData);

	int iErr = 0;

	// std::string with password must be even
	if(sPassword.length() > 0 && sPassword.length() % 2)
	{
		FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - Invalid length of Password (=" << sPassword.length() << ")");
		return FEDM_ERROR_ODD_STRING_LENGTH;
	}

	// check of internal buffer capacity
	switch(uiBank)
	{
	case FEDM_ISC_ISO_BANK_RESERVED:
		if( (uiFirstDataBlock + uiNoOfDataBlocks) * 2 > m_pTabItem->m_ucRxDB_ResBank.size() )
		{
			FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - (uiFirstDataBlock + uiNoOfDataBlocks) * 2 > RxDB_ResBank.size()");
			return FEDM_ERROR_ARRAY_BOUNDARY;
		}
		break;
	case FEDM_ISC_ISO_BANK_EPC_MEMORY:
		if( (uiFirstDataBlock + uiNoOfDataBlocks) * 2 > m_pTabItem->m_ucRxDB_EpcBank.size() )
		{
			FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - (uiFirstDataBlock + uiNoOfDataBlocks) * 2 > RxDB_EpcBank.size()");
			return FEDM_ERROR_ARRAY_BOUNDARY;
		}
		break;
	case FEDM_ISC_ISO_BANK_TID_MEMORY:
		if( (uiFirstDataBlock + uiNoOfDataBlocks) * 2 > m_pTabItem->m_ucRxDB_TidBank.size() )
		{
			FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - (uiFirstDataBlock + uiNoOfDataBlocks) * 2 > RxDB_TidBank.size()");
			return FEDM_ERROR_ARRAY_BOUNDARY;
		}
		break;
	case FEDM_ISC_ISO_BANK_USER_MEMORY:
		if( (uiFirstDataBlock + uiNoOfDataBlocks) * 2 > m_pTabItem->m_ucRxDB.size() )
		{
			FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - (uiFirstDataBlock + uiNoOfDataBlocks) * 2 > RxDB.size()");
			return FEDM_ERROR_ARRAY_BOUNDARY;
		}
		break;
	default:
		FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - Invalid Bank Number");
		return FEDM_ERROR_PARAMETER;
	}

	// prepare ISO Host Command
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE, (unsigned char)0));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_EXT_ADR, true));

	if(m_bNonAddressedMode)
	{
//		m_pReader->SetNonAddressedTabIndex(m_pTabItem->m_uiTabIndex);
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		if(m_pTabItem->m_ucSnrLen != 8)
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_UID_LF, true));
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_UID_LEN, m_pTabItem->m_ucSnrLen));
		}
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_BANK, (unsigned char)0));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_BANK_BANK_NR, uiBank));

	if(sPassword.length() > 0)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_BANK_ACCESS_FLAG, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_ACCESS_PW_LENGTH, (unsigned char)(sPassword.length()/2)));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_ACCESS_PW, sPassword));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DB_ADR_EXT, uiFirstDataBlock));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DBN, uiNoOfDataBlocks));
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char)0x23));
	
	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB0);
	if(iBack == 0)
	{
		// copy received data blocks into buffer
		switch(uiBank)
		{
		case FEDM_ISC_ISO_BANK_RESERVED:
			memcpy(pucData, &m_pTabItem->m_ucRxDB_ResBank[uiFirstDataBlock*2], uiNoOfDataBlocks * 2);
			break;
		case FEDM_ISC_ISO_BANK_EPC_MEMORY:
			memcpy(pucData, &m_pTabItem->m_ucRxDB_EpcBank[uiFirstDataBlock*2], uiNoOfDataBlocks * 2);
			break;
		case FEDM_ISC_ISO_BANK_TID_MEMORY:
			memcpy(pucData, &m_pTabItem->m_ucRxDB_TidBank[uiFirstDataBlock*2], uiNoOfDataBlocks * 2);
			break;
		case FEDM_ISC_ISO_BANK_USER_MEMORY:
			memcpy(pucData, &m_pTabItem->m_ucRxDB[uiFirstDataBlock*2], uiNoOfDataBlocks * 2);
			break;
		}
	}
	else if (iBack == 0x95)
	{
		// ISO Error
		m_pReader->GetData(FEDM_ISC_TMP_B0_ISO_ERROR, &m_ucISOErrorCode);
	}

	return iBack;
}


/***************************************************************************
  Begin        		:	11.05.2009 / M. Hultsch

  Version       	:	04.00.10 / 13.02.2012 / M. Hultsch
						- check of string length of password

						03.01.00 / 07.10.2009 / M. Hultsch

  Function			:	[0xB0][0x24] Write Multiple Blocks command
						with support for extended address mode
						NOTE: block size is always 2

  Parameters		:	unsigned int uiBank					- [in] bank number
						unsigned int uiFirstDataBlock		- [in] first data block address
						unsigned int uiNoOfDataBlocks		- [in] number of requested data blocks
						string sPassword					- [in] optional password
						unsigned char* pucData				- [in] buffer with data blocks

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_EPC_Class1_Gen2::WriteMultipleBlocks(
		unsigned int uiBank,
		unsigned int uiFirstDataBlock,
		unsigned int uiNoOfDataBlocks,
		std::string sPassword,
		unsigned char* pucData )
{
	FEDM_TRACE_FNC(m_pReader->GetLogger(), FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "Bank=" << uiBank << ", FirstDataBlock=" << uiFirstDataBlock << ", NoOfDataBlocks" << uiNoOfDataBlocks);

	FEDM_CHK3(pucData);

	int iErr = 0;

	// string with password must be even
	if(sPassword.length() > 0 && sPassword.length() % 2)
	{
		FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - Invalid length of Password (=" << sPassword.length() << ")");
		return FEDM_ERROR_ODD_STRING_LENGTH;
	}

	// check of internal buffer capacity
	switch(uiBank)
	{
	case FEDM_ISC_ISO_BANK_RESERVED:
		if( (uiFirstDataBlock + uiNoOfDataBlocks) * 2 > m_pTabItem->m_ucTxDB_ResBank.size() )
		{
			FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - (uiFirstDataBlock + uiNoOfDataBlocks) * 2 > TxDB_ResBank.size()");
			return FEDM_ERROR_ARRAY_BOUNDARY;
		}
		break;
	case FEDM_ISC_ISO_BANK_EPC_MEMORY:
		if( (uiFirstDataBlock + uiNoOfDataBlocks) * 2 > m_pTabItem->m_ucTxDB_EpcBank.size() )
		{
			FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - (uiFirstDataBlock + uiNoOfDataBlocks) * 2 > TxDB_EpcBank.size()");
			return FEDM_ERROR_ARRAY_BOUNDARY;
		}
		break;
	case FEDM_ISC_ISO_BANK_TID_MEMORY:
		if( (uiFirstDataBlock + uiNoOfDataBlocks) * 2 > m_pTabItem->m_ucTxDB_TidBank.size() )
		{
			FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - (uiFirstDataBlock + uiNoOfDataBlocks) * 2 > TxDB_TidBank.size()");
			return FEDM_ERROR_ARRAY_BOUNDARY;
		}
		break;
	case FEDM_ISC_ISO_BANK_USER_MEMORY:
		if( (uiFirstDataBlock + uiNoOfDataBlocks) * 2 > m_pTabItem->m_ucTxDB.size() )
		{
			FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - (uiFirstDataBlock + uiNoOfDataBlocks) * 2 > TxDB.size()");
			return FEDM_ERROR_ARRAY_BOUNDARY;
		}
		break;
	default:
		FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - Invalid Bank Number");
		return FEDM_ERROR_PARAMETER;
	}

	// prepare ISO Host Command
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE, (unsigned char)0));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_EXT_ADR, true));

	if(m_bNonAddressedMode)
	{
//		m_pReader->SetNonAddressedTabIndex(m_pTabItem->m_uiTabIndex);
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		if(m_pTabItem->m_ucSnrLen != 8)
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_UID_LF, true));
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_UID_LEN, m_pTabItem->m_ucSnrLen));
		}
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_BANK, (unsigned char)0));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_BANK_BANK_NR, uiBank));

	if(sPassword.length() > 0)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_BANK_ACCESS_FLAG, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_ACCESS_PW_LENGTH, (unsigned char)(sPassword.length()/2)));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_ACCESS_PW, sPassword));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DB_ADR_EXT, uiFirstDataBlock));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DBN, uiNoOfDataBlocks));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DB_SIZE, (unsigned char)2));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char)0x24));

	m_pTabItem->m_ucBlockSize = (unsigned char)2;

	// save data blocks in internal memory
	switch(uiBank)
	{
	case FEDM_ISC_ISO_BANK_RESERVED:
		memcpy(&m_pTabItem->m_ucTxDB_ResBank[uiFirstDataBlock*2], pucData, uiNoOfDataBlocks * 2);
		break;
	case FEDM_ISC_ISO_BANK_EPC_MEMORY:
		memcpy(&m_pTabItem->m_ucTxDB_EpcBank[uiFirstDataBlock*2], pucData, uiNoOfDataBlocks * 2);
		break;
	case FEDM_ISC_ISO_BANK_TID_MEMORY:
		memcpy(&m_pTabItem->m_ucTxDB_TidBank[uiFirstDataBlock*2], pucData, uiNoOfDataBlocks * 2);
		break;
	case FEDM_ISC_ISO_BANK_USER_MEMORY:
		memcpy(&m_pTabItem->m_ucTxDB[uiFirstDataBlock*2], pucData, uiNoOfDataBlocks * 2);
		break;
	}

	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB0);
	if(iBack == 0x03)
	{
		// if write error, save db address which causes the error
		// one or two byte db address is supported
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B0_RSP_DB_EXT_ADR_E, &m_uiErrorDBAddress));
	}
	else if (iBack == 0x95)
	{
		// ISO Error
		m_pReader->GetData(FEDM_ISC_TMP_B0_ISO_ERROR, &m_ucISOErrorCode);
	}

	return iBack;
}


/***************************************************************************
  Begin        		:	11.05.2009 / M. Hultsch

  Version       	:	04.00.10 / 13.02.2012 / M. Hultsch
						- check of string length of EPC and password

						03.01.00 / 07.10.2009 / M. Hultsch

  Function			:	write of an EPC into the EPC Memory
						
						uses [0xB0][0x24] Write Multiple Blocks command
						with support for extended address mode
						NOTE: block size is always 2

  Parameters		:	string sNewEpc			- [in] new EPC number
						string sPassword		- [in] optional password

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_EPC_Class1_Gen2::WriteEPC(
	std::string sNewEPC,
	std::string sPassword )
{
	FEDM_TRACE_FNC(m_pReader->GetLogger(), FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "NewEPC=" << sNewEPC);

	int iErr = 0;
	unsigned char ucClass1Gen2PC[2] = {0, 0};
	unsigned char ucEPC[32];

	// string with new EPC must contain complete blocks
	if(sNewEPC.length() % 4)
	{
		FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - Invalid length of new EPC (=" << sNewEPC.length() << ")");
		return FEDM_ERROR_UNVALID_IDD_LENGTH;
	}

	// string with password must be even
	if(sPassword.length() > 0 && sPassword.length() % 2)
	{
		FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - Invalid length of Password (=" << sPassword.length() << ")");
		return FEDM_ERROR_ODD_STRING_LENGTH;
	}

	memset(ucEPC, 0, 32);

	iErr = FEDM_ConvHexStrToHexUChar(sNewEPC, ucEPC, 32); // max 256 bit UID
	if(iErr < 0)
	{
		FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - Conversion error with EPC (=" << sNewEPC << ")");
		return iErr;
	}

	// build Protocol-Control word
	ucClass1Gen2PC[0] |= (((sNewEPC.length()/4)&0x1F) << 3); // 5 bit length, rest 0
//	ucClass1Gen2PC[1] |= (); // EPC Header according EPC Tag Data Standards

	// setup new EPC with PC in memory
	m_pTabItem->m_ucTxDB_EpcBank[2] = ucClass1Gen2PC[1];
	m_pTabItem->m_ucTxDB_EpcBank[3] = ucClass1Gen2PC[0];
	for(int iCnt=0; iCnt<(int)sNewEPC.length()/2; iCnt+=2)
	{
		m_pTabItem->m_ucTxDB_EpcBank[4+iCnt]	= ucEPC[iCnt+1];
		m_pTabItem->m_ucTxDB_EpcBank[4+iCnt+1]	= ucEPC[iCnt];
	}

	// prepare ISO Host Command

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE, (unsigned char)0));
	if(m_bNonAddressedMode)
	{
//		m_pReader->SetNonAddressedTabIndex(m_pTabItem->m_uiTabIndex);
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		if(m_pTabItem->m_ucSnrLen != 8)
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_UID_LF, true));
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_UID_LEN, m_pTabItem->m_ucSnrLen));
		}
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_EXT_ADR, true));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_BANK, (unsigned char)0));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_BANK_BANK_NR, (unsigned char)FEDM_ISC_ISO_BANK_EPC_MEMORY));
	
	if(sPassword.length() > 0)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_BANK_ACCESS_FLAG, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_ACCESS_PW_LENGTH, (unsigned char)(sPassword.length()/2)));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_ACCESS_PW, sPassword));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DB_ADR_EXT, (unsigned int)1));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DBN, (unsigned char)(sNewEPC.length()/4 + 1)));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DB_SIZE, (unsigned char)2));
	m_pTabItem->m_ucBlockSize = 2;
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char)0x24));

	int iBack = m_pReader->SendProtocol(0xB0);
	if (iBack == 0x95)
	{
		// ISO Error
		m_pReader->GetData(FEDM_ISC_TMP_B0_ISO_ERROR, &m_ucISOErrorCode);
	}

	return iBack;
}


/***************************************************************************
  Begin        		:	11.05.2009 / M. Hultsch

  Version       	:	04.03.01 / 01.11.2012 / M. Hultsch
						- extended parameter list for support of Recommissioning Bits

						04.00.10 / 13.02.2012 / M. Hultsch
						- check of string length of password

						03.01.00 / 07.10.2009 / M. Hultsch

  Function			:	Kill of tag
						
						uses [0xB3][0x18] Kill command

  Parameters		:	string sPassword		- [in] optional password
						bool bRecom_AssertedLSB	- [in] optional: recommissioning bit Asserted LSB
						bool bRecom_Asserted2SB	- [in] optional: recommissioning bit Asserted 2SB
						bool bRecom_Asserted3SB	- [in] optional: recommissioning bit Asserted 3SB

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_EPC_Class1_Gen2::Kill(
	std::string sPassword,
	bool bRecom_AssertedLSB,
	bool bRecom_Asserted2SB,
	bool bRecom_Asserted3SB )
{
	FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__);

	int iErr = 0;
	unsigned char ucRecomBits = 0;

	// string with password must be even
	if(sPassword.length() > 0 && sPassword.length() % 2)
	{
		FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - Invalid length of Password (=" << sPassword.length() << ")");
		return FEDM_ERROR_ODD_STRING_LENGTH;
	}

	// prepare ISO Host Command
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
//		m_pReader->SetNonAddressedTabIndex(m_pTabItem->m_uiTabIndex);
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		if(m_pTabItem->m_ucSnrLen != 8)
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_MODE_EPC_LF, true));
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_REQ_EPC_LEN, m_pTabItem->m_ucSnrLen));
		}
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_REQ_EPC, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}
	
	if(sPassword.length() > 0)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_KILL_PW, sPassword));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_KILL_PW_LENGTH, (unsigned char)(sPassword.length()/2)));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_KILL_PW_LENGTH, (unsigned char)0));
	}

	if(bRecom_AssertedLSB || bRecom_Asserted2SB || bRecom_Asserted3SB)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_MODE_RECOM, true));
		if(bRecom_AssertedLSB)
			ucRecomBits |= 0x01;
		if(bRecom_Asserted2SB)
			ucRecomBits |= 0x02;
		if(bRecom_Asserted3SB)
			ucRecomBits |= 0x04;

		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_REQ_RECOM_BITS, ucRecomBits));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_REQ_RECOM_BITS, (unsigned char)0));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_CMD, (unsigned char)0x18));

	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB3);
	if (iBack == 0x95)
	{
		// ISO Error
		m_pReader->GetData(FEDM_ISC_TMP_B3_TAG_ERROR, &m_ucISOErrorCode);
	}

	return iBack;
}

/***************************************************************************
  Begin				:	09.12.2011 / M. Hultsch

  Version			:	04.00.07 / 09.12.2011 / M. Hultsch

  Function			:	[0xB3][0x22] Lock (simplified Lock method)
						- wrapper for Lock method

  Parameters		:	string Password
						byte lockKill		- [in] lock settings for kill
						byte lockAccess		- [in] lock settings for access pasword
						byte lockEPC		- [in] lock settings for  EPC memory
						byte lockTID		- [in] lock settings for TID memory
						byte lockUser		- [in] lock settings for User memory

  Return Value		:	int status/error code value
***************************************************************************/
int FedmIscTagHandler_EPC_Class1_Gen2::Lock(
	std::string sPassword,
	unsigned char ucLockKill,
	unsigned char ucLockAccess,
	unsigned char ucLockEPC,
	unsigned char ucLockTID,
	unsigned char ucLockUser)
{
	return Lock(sPassword,
				(unsigned char)((ucLockKill & 0x30) >> 4),
				(unsigned char)(ucLockKill & 0x03),
				(unsigned char)((ucLockAccess & 0x30) >> 4),
				(unsigned char)(ucLockAccess & 0x03),
				(unsigned char)((ucLockEPC & 0x30) >> 4),
				(unsigned char)(ucLockEPC & 0x03),
				(unsigned char)((ucLockTID & 0x30) >> 4),
				(unsigned char)(ucLockTID & 0x03),
				(unsigned char)((ucLockUser & 0x30) >> 4),
				(unsigned char)(ucLockUser & 0x03));
}


/***************************************************************************
  Begin        		:	11.05.2009 / M. Hultsch

  Version       	:	04.00.10 / 13.02.2012 / M. Hultsch
						- check of string length of password

						03.02.08 / 30.11.2010 / M. Hultsch

  Function			:	Lock of tag (Lock method according EPC Class1 Gen2 specification)
						
						uses [0xB3][0x22] Lock command

  Parameters		:	string sPassword				- [in] optional password
						unsigned char ucKillMask		- [in] 2 Bit mask settings for kill
						unsigned char ucKillAction		- [in] 2 Bit action settings for kill
						unsigned char ucAccessMask		- [in] 2 Bit mask settings for access pasword
						unsigned char ucAccessAction	- [in] 2 Bit action settings for access pasword
						unsigned char ucEPCMask			- [in] 2 Bit mask settings for  EPC memory
						unsigned char ucEPCAction		- [in] 2 Bit action settings for  EPC memory
						unsigned char ucTIDMask			- [in] 2 Bit mask settings for TID memory
						unsigned char ucTIDAction		- [in] 2 Bit action settings for TID memory
						unsigned char ucUserMask		- [in] 2 Bit mask settings for User memory
						unsigned char ucUserAction		- [in] 2 Bit action settings for User memory

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_EPC_Class1_Gen2::Lock(
	std::string sPassword,
	unsigned char ucKillMask,
	unsigned char ucKillAction,
	unsigned char ucAccessMask,
	unsigned char ucAccessAction,
	unsigned char ucEPCMask,
	unsigned char ucEPCAction,
	unsigned char ucTIDMask,
	unsigned char ucTIDAction,
	unsigned char ucUserMask,
	unsigned char ucUserAction )
{
	FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__);

	int iErr = 0;
	unsigned char ucLockData[3];

	// string with password must be even
	if(sPassword.length() > 0 && sPassword.length() % 2)
	{
		FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - Invalid length of Password (=" << sPassword.length() << ")");
		return FEDM_ERROR_ODD_STRING_LENGTH;
	}

	memset(ucLockData, 0, 3);

	ucLockData[0] |= ((ucKillMask & 0x03) << 6);
	ucLockData[0] |= ((ucAccessMask & 0x03) << 4);
	ucLockData[0] |= ((ucEPCMask & 0x03) << 2);
	ucLockData[0] |= ((ucTIDMask & 0x03));
	ucLockData[1] |= ((ucUserMask & 0x03) << 6);
	ucLockData[1] |= ((ucKillAction & 0x03) << 4);
	ucLockData[1] |= ((ucAccessAction & 0x03) << 2);
	ucLockData[1] |= ((ucEPCAction & 0x03));
	ucLockData[2] |= ((ucTIDAction & 0x03) << 6);
	ucLockData[2] |= ((ucUserAction & 0x03) << 4);

	// prepare ISO Host Command
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_MODE, (unsigned char)0));
	if(m_bNonAddressedMode)
	{
//		m_pReader->SetNonAddressedTabIndex(m_pTabItem->m_uiTabIndex);
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		if(m_pTabItem->m_ucSnrLen != 8)
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_MODE_EPC_LF, true));
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_REQ_EPC_LEN, m_pTabItem->m_ucSnrLen));
		}
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_REQ_EPC, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_REQ_TR_TYPE, (unsigned char)FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_LOCK_DATA_LENGTH, (unsigned char)3));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_LOCK_DATA, ucLockData, 3));

	if(sPassword.length() > 0)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_ACCESS_PW, sPassword));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_ACCESS_PW_LENGTH, (unsigned char)(sPassword.length()/2)));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_ACCESS_PW_LENGTH, (unsigned char)0));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_CMD, (unsigned char)0x22));

	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB3);
	if (iBack == 0x95)
	{
		// ISO Error
		m_pReader->GetData(FEDM_ISC_TMP_B3_TAG_ERROR, &m_ucISOErrorCode);
	}

	return iBack;
}


/***************************************************************************
  Begin        		:	21.06.2012 / M. Hultsch

  Version       	:	04.02.04 / 21.06.2012 / M. Hultsch

  Function			:	Set blocks to permalock
						- optional command according EPC Class1 Gen2 spec and not supported by all tags
						
						uses [0xB3][0x25] Block Permalock command
						NOTE: mask is limited to 8 bytes

  Parameters		:	unsigned int uiBank					- [in] bank number
						unsigned int uiBlockPointer			- [in] first block (consider: vendor specific block size)
						unsigned int uiBlockRange			- [in] number of blocks (limited to 4)
						string sPassword					- [in] optional password
						unsigned char* pucMask				- [in] buffer with mask

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_EPC_Class1_Gen2::BlockPermalock(
	unsigned int uiBank,
	unsigned int uiBlockPointer,
	unsigned int uiBlockRange,
	std::string sPassword,
	unsigned char* pucMask )
{
	FEDM_TRACE_FNC(m_pReader->GetLogger(), FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "Bank=" << uiBank << ", BlockPointer=" << uiBlockPointer << ", BlockRange=" << uiBlockRange);

	FEDM_CHK3(pucMask);

	int iErr = 0;

	// test buffer size
	if(uiBlockRange > 4)
	{
		FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - Block Range > 4");
		return FEDM_ERROR_PARAMETER;
	}

	// string with password must be even
	if(sPassword.length() > 0 && sPassword.length() % 2)
	{
		FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - Invalid length of Password (=" << sPassword.length() << ")");
		return FEDM_ERROR_ODD_STRING_LENGTH;
	}

	// test bank number
	switch(uiBank)
	{
	case FEDM_ISC_ISO_BANK_RESERVED:
	case FEDM_ISC_ISO_BANK_EPC_MEMORY:
	case FEDM_ISC_ISO_BANK_TID_MEMORY:
	case FEDM_ISC_ISO_BANK_USER_MEMORY:
		break;
	default:
		FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - Invalid Bank Number");
		return FEDM_ERROR_PARAMETER;
	}

	// prepare ISO Host Command
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_MODE, (unsigned char)0));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_MODE_EXT_ADR, true));

	if(m_bNonAddressedMode)
	{
//		m_pReader->SetNonAddressedTabIndex(m_pTabItem->m_uiTabIndex);
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		if(m_pTabItem->m_ucSnrLen != 8)
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_MODE_EPC_LF, true));
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_REQ_EPC_LEN, m_pTabItem->m_ucSnrLen));
		}
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_REQ_EPC, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_REQ_BANK, (unsigned char)0));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_REQ_BANK_BANK_NR, uiBank));

	if(sPassword.length() > 0)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_REQ_BANK_ACCESS_FLAG, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_ACCESS_PW_LENGTH, (unsigned char)(sPassword.length()/2)));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_ACCESS_PW, sPassword));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_REQ_BLOCK_PTR, uiBlockPointer));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_REQ_BLOCK_RANGE, uiBlockRange));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_REQ_MASK, pucMask, 2*uiBlockRange));
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_CMD, (unsigned char)0x25));
	
	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB3);
	if (iBack == 0x95)
	{
		// ISO Error
		m_pReader->GetData(FEDM_ISC_TMP_B3_TAG_ERROR, &m_ucISOErrorCode);
	}

	return iBack;
}


/***************************************************************************
  Begin        		:	21.06.2012 / M. Hultsch

  Version       	:	04.02.04 / 21.06.2012 / M. Hultsch

  Function			:	Read blocks permalock status
						- optional command according EPC Class1 Gen2 spec and not supported by all tags
						
						uses [0xB0][0x26] Read Permalock Status command
						NOTE: mask is limited to 8 bytes

  Parameters		:	unsigned int uiBank					- [in] bank number
						unsigned int uiBlockPointer			- [in] first block (consider: vendor specific block size)
						unsigned int uiBlockRange			- [in] number of blocks (limited to 4)
						string sPassword					- [in] optional password
						unsigned char* pucMask				- [out] buffer for mask

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_EPC_Class1_Gen2::ReadPermalockStatus(
	unsigned int uiBank,
	unsigned int uiBlockPointer,
	unsigned int uiBlockRange,
	std::string sPassword,
	unsigned char* pucMask )
{
	FEDM_TRACE_FNC(m_pReader->GetLogger(), FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "Bank=" << uiBank << ", BlockPointer=" << uiBlockPointer << ", BlockRange=" << uiBlockRange);

	FEDM_CHK3(pucMask);

	int iErr = 0;

	// test buffer size
	if(uiBlockRange > 4)
	{
		FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - Block Range > 4");
		return FEDM_ERROR_PARAMETER;
	}

	// string with password must be even
	if(sPassword.length() > 0 && sPassword.length() % 2)
	{
		FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - Invalid length of Password (=" << sPassword.length() << ")");
		return FEDM_ERROR_ODD_STRING_LENGTH;
	}

	// test bank number
	switch(uiBank)
	{
	case FEDM_ISC_ISO_BANK_RESERVED:
	case FEDM_ISC_ISO_BANK_EPC_MEMORY:
	case FEDM_ISC_ISO_BANK_TID_MEMORY:
	case FEDM_ISC_ISO_BANK_USER_MEMORY:
		break;
	default:
		FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - Invalid Bank Number");
		return FEDM_ERROR_PARAMETER;
	}

	// prepare ISO Host Command
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_MODE, (unsigned char)0));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_MODE_EXT_ADR, true));

	if(m_bNonAddressedMode)
	{
//		m_pReader->SetNonAddressedTabIndex(m_pTabItem->m_uiTabIndex);
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		if(m_pTabItem->m_ucSnrLen != 8)
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_MODE_EPC_LF, true));
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_REQ_EPC_LEN, m_pTabItem->m_ucSnrLen));
		}
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_REQ_EPC, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_REQ_BANK, (unsigned char)0));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_REQ_BANK_BANK_NR, uiBank));

	if(sPassword.length() > 0)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_REQ_BANK_ACCESS_FLAG, true));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_ACCESS_PW_LENGTH, (unsigned char)(sPassword.length()/2)));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_ACCESS_PW, sPassword));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_REQ_BLOCK_PTR, uiBlockPointer));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_REQ_BLOCK_RANGE, uiBlockRange));
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B3_CMD, (unsigned char)0x26));
	
	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB3);
	if(iBack == 0)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B3_RSP_MASK, pucMask, 2*uiBlockRange));
	}
	else if (iBack == 0x95)
	{
		// ISO Error
		m_pReader->GetData(FEDM_ISC_TMP_B3_TAG_ERROR, &m_ucISOErrorCode);
	}

	return iBack;
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_EPC_C1_G2)
