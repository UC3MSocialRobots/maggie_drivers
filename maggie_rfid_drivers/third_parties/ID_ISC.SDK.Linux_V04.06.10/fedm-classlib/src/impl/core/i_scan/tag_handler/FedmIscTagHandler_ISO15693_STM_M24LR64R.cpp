/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO15693_STM_M24LR64R.cpp      |
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
Begin        		:	25.11.2009

Version       		:	04.06.01 / 07.03.2014 / M. Hultsch
						- new methods: LockMultipleBlocks and GetMultipleBlockSecurityStatus

						03.03.00 / 10.02.2011 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID i-scan® HF transponder ISO15693 of manufacturer STMicroelectronics SA
						specialized tag handler on top with custom specific commands

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)

#include "FedmIscTagHandler_ISO15693_STM_M24LR64R.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"
#include "../../FEDM_Functions.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscTagHandler_ISO15693_STM_M24LR64R
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_ISO15693_STM_M24LR64R::FedmIscTagHandler_ISO15693_STM_M24LR64R(
	FEDM_ISCReader* pReader,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO15693_STM(pReader, FedmIscTagHandler::TYPE_ISO15693_STM_M24LR64R, pTabItem)
{
	m_sTagName = "ISO 15693 : ";
	m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_STM);
	m_sTagName += " M24LR64-R";
}

FedmIscTagHandler_ISO15693_STM_M24LR64R::FedmIscTagHandler_ISO15693_STM_M24LR64R(
	FEDM_ISCReader* pReader,
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO15693_STM(pReader, uiTagHandlerType, pTabItem)
{
	m_sTagName = "ISO 15693 : ";
	m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_STM);
	m_sTagName += " M24LR64-R";
}

FedmIscTagHandler_ISO15693_STM_M24LR64R::~FedmIscTagHandler_ISO15693_STM_M24LR64R()
{
}

// set all data members to 0
/*void FedmIscTagHandler_ISO15693_STM_M24LR64R::Init()
{
}*/


/***************************************************************************
  Begin        		:	10.02.2011 / M. Hultsch

  Version       	:	03.03.00 / 10.02.2011 / M. Hultsch

  Function			:	[0xB0][0x23] Read Multiple Blocks command
						with support for extended address mode

  Parameters		:	unsigned int uiFirstDataBlock		- [in] first data block address
						unsigned int uiNoOfDataBlocks		- [in] number of requested data blocks
						unsigned int& uiBlockSize			- [out] block size of tag
						unsigned char* pucSecurityStatus	- [out] buffer with security info
						unsigned char* pucData				- [out] buffer with data blocks

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_STM_M24LR64R::ReadMultipleBlocks(
		unsigned int uiFirstDataBlock,
		unsigned int uiNoOfDataBlocks,
		unsigned int& uiBlockSize,
		unsigned char* pucSecurityStatus,
		unsigned char* pucData )
{
	int iErr = 0;
	unsigned int uiBank = FEDM_ISC_ISO_BANK_USER_MEMORY;

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

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DB_ADR_EXT, uiFirstDataBlock));
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

	return iBack;
}


/***************************************************************************
  Begin        		:	10.02.2011 / M. Hultsch

  Version       	:	03.03.00 / 10.02.2011 / M. Hultsch

  Function			:	[0xB0][0x24] Write Multiple Blocks command
						with support for extended address mode

  Parameters		:	unsigned int uiBank					- [in] bank number
						unsigned int uiFirstDataBlock		- [in] first data block address
						unsigned int uiNoOfDataBlocks		- [in] number of requested data blocks
						string sPassword					- [in] optional password
						unsigned char* pucData				- [in] buffer with data blocks

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_STM_M24LR64R::WriteMultipleBlocks(
		unsigned int uiFirstDataBlock,
		unsigned int uiNoOfDataBlocks,
		unsigned int uiBlockSize,
		unsigned char* pucData )
{
	int iErr = 0;
	unsigned int uiBank = FEDM_ISC_ISO_BANK_USER_MEMORY;

	// check of internal buffer capacity
	if( (uiFirstDataBlock + uiNoOfDataBlocks) * uiBlockSize > m_pTabItem->m_ucTxDB.size() )
		return FEDM_ERROR_ARRAY_BOUNDARY;

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

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DB_ADR_EXT, uiFirstDataBlock));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DBN, uiNoOfDataBlocks));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DB_SIZE, (unsigned char)uiBlockSize));

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
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B0_RSP_DB_EXT_ADR_E, &m_uiErrorDBAddress));
	}

	return iBack;
}


/***************************************************************************
  Begin        		:	07.03.2014 / M. Hultsch

  Version       	:	04.06.01 / 07.03.2014 / M. Hultsch

  Function			:	basic [0xB0][0x22] Lock Multiple Blocks command
						with support for extended address mode

  Parameters		:	unsigned int uiFirstDataBlock		- [in] first data block address
						unsigned int uiNoOfDataBlocks		- [in] number of requested data blocks

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_STM_M24LR64R::LockMultipleBlocks(
	unsigned int uiFirstDataBlock,
	unsigned int uiNoOfDataBlocks )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE, (unsigned char)0));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_EXT_ADR, true));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else
	{
		if(m_pTabItem->m_bIsSelected)
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_SEL));
		}
		else
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
		}
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_BANK, (unsigned char)3)); // user memory
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DB_ADR_EXT, uiFirstDataBlock));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DBN, uiNoOfDataBlocks));
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char)0x22));

    m_ucISOErrorCode = 0;

	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB0);
	if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B0_ISO_ERROR, &m_ucISOErrorCode));
	}

	return iBack;
}


/***************************************************************************
  Begin        		:	07.03.2014 / M. Hultsch

  Version       	:	04.06.01 / 07.03.2014 / M. Hultsch

  Function			:	basic [0xB0][0x2C] Get Multiple Block Security Status command
						with support for extended address mode

  Parameters		:	unsigned int uiFirstDataBlock		- [in] first data block address
						unsigned int uiNoOfDataBlocks		- [in] number of requested data blocks
						unsigned char* pucSecurityStatus	- [out] buffer with security info

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_STM_M24LR64R::GetMultipleBlockSecurityStatus(
	unsigned int uiFirstDataBlock,
	unsigned int uiNoOfDataBlocks,
	unsigned char* pucSecurityStatus )
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE, (unsigned char)0));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_EXT_ADR, true));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else
	{
		if(m_pTabItem->m_bIsSelected)
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_SEL));
		}
		else
		{
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
			FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
		}
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_BANK, (unsigned char)3)); // user memory
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DB_ADR_EXT, uiFirstDataBlock));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_REQ_DBN, uiNoOfDataBlocks));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B0_CMD, (unsigned char)0x2C));

    m_ucISOErrorCode = 0;
	
	// ... and execute
	int iBack = m_pReader->SendProtocol(0xB0);
	if(iBack == 0)
	{
		// save received data blocks in internal memory
		memcpy(pucSecurityStatus, &m_pTabItem->m_ucSecStatus[uiFirstDataBlock], uiNoOfDataBlocks);
	}
	else if(iBack == 0x95)
	{
		FEDM_CHK1(iErr, m_pReader->GetData(FEDM_ISC_TMP_B0_ISO_ERROR, &m_ucISOErrorCode));
	}

	return iBack;
}


/***************************************************************************
  Begin        		:	09.10.2009 / M. Hultsch

  Version       	:	03.01.00 / 09.10.2009 / M. Hultsch

  Function			:	[0xB1][0xB1] WriteSectorPassword

  Parameters		:	unsigned char PasswordNumber
						string sPassword

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_STM_M24LR64R::WriteSectorPassword(
	unsigned char ucPasswordNumber,
	string sPassword)
{
	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xB1));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_SEL));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_STM_PASSWORD_NO, ucPasswordNumber));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_STM_PASSWORD, sPassword));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_STM));
	
	// ... and execute
	return m_pReader->SendProtocol(0xB1);
}

/***************************************************************************
  Begin        		:	09.10.2009 / M. Hultsch

  Version       	:	03.01.00 / 09.10.2009 / M. Hultsch

  Function			:	[0xB1][0xB2] LockSectorPassword

  Parameters		:	unsigned char SectorNumber
						unsigned char ucSectorSecurityStatus

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_STM_M24LR64R::LockSectorPassword(
	unsigned char ucSectorNumber,
	unsigned char ucSectorSecurityStatus)
{
	int iErr = 0;
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xB2));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_SEL));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}
	
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_STM_INFO_BYTE, (unsigned char)0x02));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_STM_BLOCK_NO, ucSectorNumber));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_STM_PROTECT_STATUS, ucSectorSecurityStatus));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_STM));
	
	// ... and execute
	return m_pReader->SendProtocol(0xB1);
}

/***************************************************************************
  Begin        		:	09.10.2009 / M. Hultsch

  Version       	:	03.01.00 / 09.10.2009 / M. Hultsch

  Function			:	[0xB1][0xB3] PresentPassword

  Parameters		:	unsigned char PasswordNumber
						string sPassword

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_STM_M24LR64R::PresentSectorPassword(
	unsigned char ucPasswordNumber,
	string sPassword)
{
	int iErr = 0;

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_CMD, (unsigned char)0xB3));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE, (unsigned char)0));

	if(m_bNonAddressedMode)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_NONADR));
	}
	else if(m_pTabItem->m_bIsSelected)
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_SEL));
	}
	else
	{
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MODE_ADR, (unsigned char)FEDM_ISC_ISO_MODE_ADR));
		FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_UID, m_pTabItem->m_ucSnr, m_pTabItem->m_ucSnrLen));
	}

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_STM_PASSWORD_NO, ucPasswordNumber));
	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_REQ_STM_PASSWORD, sPassword));

	FEDM_CHK1(iErr, m_pReader->SetData(FEDM_ISC_TMP_B1_MFR, (unsigned char)FEDM_ISC_ISO_MFR_STM));
	
	// ... and execute
	return m_pReader->SendProtocol(0xB1);
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)
