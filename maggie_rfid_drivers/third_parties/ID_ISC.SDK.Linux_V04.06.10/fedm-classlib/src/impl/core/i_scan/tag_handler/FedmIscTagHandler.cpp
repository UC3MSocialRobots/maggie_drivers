/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler.cpp                            |
|                                                       |
---------------------------------------------------------

Copyright © 2009-2014	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	30.03.2009

Version       		:	04.06.01 / 07.03.2014 / M. Hultsch
						- modification in Init: reset of m_sTagName removed

						04.03.01 / 15.11.2012 / M. Hultsch

Operation Systems	:	independent

Function			:	base class for OBID i-scan® and 
						OBID® classic-pro transponder

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#include "FedmIscTagHandler.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"
#include "../../FEDM_Functions.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif


#if _MSC_VER <= 1200 // for Visual Studio 6
	const unsigned int FedmIscTagHandler::TYPE_BASIC = 1;
	const unsigned int FedmIscTagHandler::TYPE_EPC_CLASS1_GEN2 = 10;
	const unsigned int FedmIscTagHandler::TYPE_EPC_CLASS1_GEN2_IDS_SL900A = 11;
	const unsigned int FedmIscTagHandler::TYPE_ISO14443 = 20;
	const unsigned int FedmIscTagHandler::TYPE_ISO14443_INNOVATRON = 21;
	const unsigned int FedmIscTagHandler::TYPE_ISO14443_2 = 30;
	const unsigned int FedmIscTagHandler::TYPE_ISO14443_2_INNOVISION_JEWEL = 31;
	const unsigned int FedmIscTagHandler::TYPE_ISO14443_2_STM_SR176 = 32;
	const unsigned int FedmIscTagHandler::TYPE_ISO14443_2_STM_SRIxxx = 33;
	const unsigned int FedmIscTagHandler::TYPE_ISO14443_3 = 40;
	const unsigned int FedmIscTagHandler::TYPE_ISO14443_3_INFINEON_MY_D = 41;
	const unsigned int FedmIscTagHandler::TYPE_ISO14443_3_INFINEON_MY_D_MOVE = 42;
	const unsigned int FedmIscTagHandler::TYPE_ISO14443_3_MIFARE_CLASSIC = 43;
	const unsigned int FedmIscTagHandler::TYPE_ISO14443_3_MIFARE_ULTRALIGHT = 44;
	const unsigned int FedmIscTagHandler::TYPE_ISO14443_3_MIFARE_PLUS_SL1 = 45;
	const unsigned int FedmIscTagHandler::TYPE_ISO14443_3_MIFARE_PLUS_SL2 = 46;
	const unsigned int FedmIscTagHandler::TYPE_ISO14443_3_ASK_CTX = 47;
	const unsigned int FedmIscTagHandler::TYPE_ISO14443_4 = 50;
	const unsigned int FedmIscTagHandler::TYPE_ISO14443_4_MIFARE_DESFIRE = 51;
	const unsigned int FedmIscTagHandler::TYPE_ISO14443_4_MIFARE_PLUS_SL1 = 52;
	const unsigned int FedmIscTagHandler::TYPE_ISO14443_4_MIFARE_PLUS_SL2 = 53;
	const unsigned int FedmIscTagHandler::TYPE_ISO14443_4_MIFARE_PLUS_SL3 = 54;
	const unsigned int FedmIscTagHandler::TYPE_ISO14443_4_MAXIM = 60;
	const unsigned int FedmIscTagHandler::TYPE_ISO14443_4_MAXIM_MAX66000 = 61;
	const unsigned int FedmIscTagHandler::TYPE_ISO14443_4_MAXIM_MAX66020 = 62;
	const unsigned int FedmIscTagHandler::TYPE_ISO14443_4_MAXIM_MAX66040 = 63;
	const unsigned int FedmIscTagHandler::TYPE_ISO18000_3M3 = 100;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693 = 0xE0000000;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_STM = 0xE0020000;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_STM_LRI2K = 0xE0022000;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_STM_LRIS2K = 0xE0020280;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_STM_M24LR64R = 0xE00202C0;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_STM_LRIS64K = 0xE0020440;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_STM_M24LRxxER = 0xE00205C0;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_NXP = 0xE0040000;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_NXP_ICODE_SLI = 0xE0040100;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_NXP_ICODE_SLIX = 0xE0040110;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_NXP_ICODE_SLI_L = 0xE0040300;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_NXP_ICODE_SLIX_L = 0xE0040310;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_NXP_ICODE_SLI_S = 0xE0040200;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_NXP_ICODE_SLIX_S = 0xE0040210;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_Infineon = 0xE0050000;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_Infineon_my_d = 0xE005FFFF;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_TI = 0xE0070000;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_TI_Tag_it_HFI_Pro = 0xE007E000;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_TI_Tag_it_HFI_Plus = 0xE0078000;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_Fujitsu = 0xE0080000;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_Fujitsu_MB89R1xx = 0xE0080001;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_EM = 0xE0160000;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_EM_4034 = 0xE0160004;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_KSW = 0xE0170000;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_MAXIM = 0xE02B0000;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_MAXIM_MAX66100 = 0xE02B0010;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_MAXIM_MAX66120 = 0xE02B0020;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_MAXIM_MAX66140 = 0xE02B0030;
	const unsigned int FedmIscTagHandler::TYPE_ISO15693_IDS_SL13A = 0xE036FFFF;
#endif



//####################################################################
// class FedmIscTagHandler
// >> supports only ISO Host Commands <<
//####################################################################

_FEDM_ISC_CORE_EXT_CLASS FedmIscTagHandler::FedmIscTagHandler(FEDM_ISCReader* pReader, unsigned int uiTagHandlerType, FEDM_ISOTabItem* pTabItem)
{
	if(pReader == NULL || pTabItem == NULL)
		throw FEDM_ERROR_NULL_POINTER;

	m_uiTagHandlerType	= uiTagHandlerType;
	m_pReader			= pReader;
	m_pTabItem			= pTabItem;
	m_sTagName			= m_pReader->GetTagName(pTabItem->m_ucTrType);

	m_bNonAddressedMode	= false;
	m_uiErrorDBAddress = 0;
	m_ucISOErrorCode = 0;
}

_FEDM_ISC_CORE_EXT_CLASS FedmIscTagHandler::~FedmIscTagHandler()
{
}

_FEDM_ISC_CORE_EXT_CLASS void FedmIscTagHandler::Init()
{
	//m_sTagName = m_pReader->GetTagName(m_pTabItem->m_ucTrType);

	m_bNonAddressedMode	= false;
	m_uiErrorDBAddress = 0;
	m_ucISOErrorCode = 0;
}

_FEDM_ISC_CORE_EXT_CLASS unsigned int FedmIscTagHandler::GetTagHandlerType()
{
	return m_uiTagHandlerType;
}

_FEDM_ISC_CORE_EXT_CLASS unsigned int FedmIscTagHandler::GetTagDriverType()
{
	return (unsigned int)m_pTabItem->m_ucTrType;
}

#if defined(__BORLANDC__)
const char* FedmIscTagHandler::GetTagName()
#else
_FEDM_ISC_CORE_EXT_CLASS const char* FedmIscTagHandler::GetTagName()
#endif
{
	return m_sTagName.c_str();
}

#if defined(__BORLANDC__)
const char* FedmIscTagHandler::GetUID()
#else
_FEDM_ISC_CORE_EXT_CLASS const char* FedmIscTagHandler::GetUID()
#endif
{
	int iBack = FEDM_ConvHexUCharToHexStr(m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen, m_sUid);
	if(iBack)
		return "";

	return m_sUid.c_str();
}


/***************************************************************************
  Begin        		:	01.07.2011 / M. Hultsch

  Version       	:	03.03.04 / 01.07.2011 / M. Hultsch

  Function			:	return of antenna information
						1st Note: available only, if Inventory command was executed with ANT option
						2nd Note: the arrays ucNumber, ucStatus and ucRSSI must have the size of 8

  Parameters		:	unsigned char* ucNumber		- [out] array with antenna numbers
						unsigned char* ucStatus		- [out] array with status
						unsigned char* ucRSSI		- [out] array with RSSI values

  Return value		:	>0	: number of antennas
						<0	: error code
***************************************************************************/
_FEDM_ISC_CORE_EXT_CLASS int FedmIscTagHandler::GetRSSI(unsigned char* ucNumber, unsigned char* ucStatus, unsigned char* ucRSSI)
{
	int cnt = 0;

	if(m_pTabItem->m_ucAntCount > 0)
	{
		for(cnt=0; cnt<m_pTabItem->m_ucAntCount; cnt++)
		{
			ucNumber[cnt] = m_pTabItem->m_ucAntNumber[cnt];
			ucStatus[cnt] = m_pTabItem->m_ucAntStatus[cnt];
			ucRSSI[cnt] = m_pTabItem->m_ucAntRSSI[cnt];
		}

		return m_pTabItem->m_ucAntCount;
	}

	return FEDM_ERROR_NO_DATA;
}


/***************************************************************************
  Begin        		:	25.02.2011 / M. Hultsch

  Version       	:	03.03.00 / 25.02.2011 / M. Hultsch

  Function			:	basic [0xB0][0x23] read multiple blocks command
						without support for extended address mode

  Parameters		:	unsigned int uiFirstDataBlock		- [in] first data block address
						unsigned int uiNoOfDataBlocks		- [in] number of requested data blocks
						unsigned int& uiBlockSize			- [out] block size of tag
						unsigned char* pucData				- [out] buffer with data blocks

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
_FEDM_ISC_CORE_EXT_CLASS int FedmIscTagHandler::ReadMultipleBlocks(
		unsigned int uiFirstDataBlock,
		unsigned int uiNoOfDataBlocks,
		unsigned int& uiBlockSize,
		unsigned char* pucData )
{
	FEDM_TRACE_FNC(m_pReader->GetLogger(), FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "FirstDataBlock=" << uiFirstDataBlock << ", NoOfDataBlocks=" << uiNoOfDataBlocks);

	int iErr = 0;

	// prepare ISO Host Command
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
//		m_pReader->SetNonAddressedTabIndex(m_pTabItem->m_uiTabIndex);
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_SEL));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DB_ADR, uiFirstDataBlock));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DBN, uiNoOfDataBlocks));
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char)0x23));
	
	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB0);
	if(iBack == 0)
	{
		// save received data blocks in internal memory
		uiBlockSize = m_pTabItem->m_ucBlockSize;
		memcpy(pucData, &m_pTabItem->m_ucRxDB[uiFirstDataBlock*m_pTabItem->m_ucBlockSize], uiNoOfDataBlocks * m_pTabItem->m_ucBlockSize);
	}
	else if (iBack == 0x95)
	{
		// ISO Error
		m_pReader->GetData(FEDM_ISC_TMP_B0_ISO_ERROR, &m_ucISOErrorCode);
	}

	return iBack;
}


/***************************************************************************
  Begin        		:	31.03.2009 / M. Hultsch

  Version       	:	03.01.00 / 07.10.2009 / M. Hultsch

  Function			:	basic [0xB0][0x23] read multiple blocks command
						without support for extended address mode

  Parameters		:	unsigned int uiFirstDataBlock		- [in] first data block address
						unsigned int uiNoOfDataBlocks		- [in] number of requested data blocks
						unsigned int& uiBlockSize			- [out] block size of tag
						unsigned char* pucSecurityStatus	- [out] buffer with security info
						unsigned char* pucData				- [out] buffer with data blocks

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
_FEDM_ISC_CORE_EXT_CLASS int FedmIscTagHandler::ReadMultipleBlocks(
		unsigned int uiFirstDataBlock,
		unsigned int uiNoOfDataBlocks,
		unsigned int& uiBlockSize,
		unsigned char* pucSecurityStatus,
		unsigned char* pucData )
{
	FEDM_TRACE_FNC(m_pReader->GetLogger(), FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "FirstDataBlock=" << uiFirstDataBlock << ", NoOfDataBlocks=" << uiNoOfDataBlocks);

	int iErr = 0;

	// prepare ISO Host Command
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE, (unsigned char)0));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_SEC, true));

	if(m_bNonAddressedMode)
	{
//		m_pReader->SetNonAddressedTabIndex(m_pTabItem->m_uiTabIndex);
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_SEL));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DB_ADR, uiFirstDataBlock));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DBN, uiNoOfDataBlocks));
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char)0x23));
	
	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB0);
	if(iBack == 0)
	{
		// save received data blocks in internal memory
		uiBlockSize = m_pTabItem->m_ucBlockSize;
		memcpy(pucSecurityStatus, &m_pTabItem->m_ucSecStatus[uiFirstDataBlock], uiNoOfDataBlocks);
		memcpy(pucData, &m_pTabItem->m_ucRxDB[uiFirstDataBlock*m_pTabItem->m_ucBlockSize], uiNoOfDataBlocks * m_pTabItem->m_ucBlockSize);
	}
	else if (iBack == 0x95)
	{
		// ISO Error
		m_pReader->GetData(FEDM_ISC_TMP_B0_ISO_ERROR, &m_ucISOErrorCode);
	}

	return iBack;
}


/***************************************************************************
  Begin        		:	31.03.2009 / M. Hultsch

  Version       	:	03.01.00 / 07.10.2009 / M. Hultsch

  Function			:	basic [0xB0][0x24] write multiple blocks command
						without support for extended address mode

  Parameters		:	unsigned int uiFirstDataBlock		- [in] first data block address
						unsigned int uiNoOfDataBlocks		- [in] number of requested data blocks
						unsigned int uiBlockSize			- [in] block size of tag
						unsigned char* pucData				- [in] buffer with data blocks

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
_FEDM_ISC_CORE_EXT_CLASS int FedmIscTagHandler::WriteMultipleBlocks(
		unsigned int uiFirstDataBlock,
		unsigned int uiNoOfDataBlocks,
		unsigned int uiBlockSize,
		unsigned char* pucData )
{
	FEDM_TRACE_FNC(m_pReader->GetLogger(), FELOGMNG_DBG_FUNCTION_CALL | FELOGMNG_DBG_LF_COMM_TAG_HANDLER, __FUNCTION__, FELOGMNG_DBG_FUNCTION_PARAMETER, "FirstDataBlock=" << uiFirstDataBlock << ", NoOfDataBlocks=" << uiNoOfDataBlocks << ", BlockSize=" << uiBlockSize);

	int iErr = 0;

	// check of internal buffer capacity
	if( (uiFirstDataBlock + uiNoOfDataBlocks) * uiBlockSize > m_pTabItem->m_ucTxDB.size() )
	{
		FEDM_TRACE(m_pReader->GetLogger(), FELOGMNG_DBG_ERROR, __FUNCTION__ << " - (uiFirstDataBlock + uiNoOfDataBlocks) * uiBlockSize > TxDB.size()");
		return FEDM_ERROR_ARRAY_BOUNDARY;
	}

	// prepare ISO Host Command
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE, (unsigned char)0));
	if(m_bNonAddressedMode)
	{
//		m_pReader->SetNonAddressedTabIndex(m_pTabItem->m_uiTabIndex);
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_SEL));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DB_ADR, uiFirstDataBlock));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DBN, uiNoOfDataBlocks));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DB_SIZE, uiBlockSize));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char)0x24));

	m_pTabItem->m_ucBlockSize = (unsigned char)uiBlockSize;

	// save data blocks in internal memory
	memcpy(&m_pTabItem->m_ucTxDB[uiFirstDataBlock*uiBlockSize], pucData, uiNoOfDataBlocks * uiBlockSize);

	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB0);
	if(iBack == 0x03)
	{
		// if write error, save db address which causes the error
		// one or two byte db address is supported
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B0_RSP_DB_ADR_E, &m_uiErrorDBAddress));
	}
	else if (iBack == 0x95)
	{
		// ISO Error
		m_pReader->GetData(FEDM_ISC_TMP_B0_ISO_ERROR, &m_ucISOErrorCode);
	}

	return iBack;
}
