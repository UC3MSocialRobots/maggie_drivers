/*-------------------------------------------------------
|                                                       |
|                 FEDM_ISOTabItem.cpp                   |
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
Begin        		:	07.06.2000

Version       		:	04.06.06 / 23.09.2013 / M. Hultsch
						- only for EPC Class1 Gen2:
						  bugfix for addressed Host commands, if Reader is configured for UID = EPC + TID

						04.06.05 / 02.09.2014 / M. Hultsch
						- bugfix for extended PC (XPC) for EPC Class1 Gen2

						04.05.02 / 02.07.2013 / M. Hultsch
						- new TagHandler for ISO 15693: NXP SLIX, SLIX-L, SLIX-S

						04.03.01 / 01.11.2012 / M. Hultsch
						- support for new tag types: ISO 15693 STM M24LRxxE-R, STM LRIS64K
						- support for Extended PC W1 for EPC C1 G2
						- support for new TagHandler for ISO 18000-3M3

						04.02.01 / 31.05.2012 / M. Hultsch
						- support for new tag types: Innovatron and ASK CTx

						04.00.10 / 14.02.2012 / M. Hultsch
						- support for external logmanager (FELOGMNG.dll), when #define _FELOGMNG_SUPPORT is set

						04.00.08 / 05.01.2012 / M. Hultsch
						- smart create condition for ISO 14443 TagHandler, if 
						  FEDM_ISCReader::m_bTagHandlerOption_CreateCondition_Smart is set (default: not set)

						04.00.00 / 21.07.2011 / M. Hultsch
						- max. UID length increased to 96 Bytes

						03.03.01 / 08.04.2011 / M. Hultsch
						- new TagHandler classes for ISO 15693 IDS SL13A
						  and EPC Class1 Gen2 IDS SL900A

						03.02.04 / 09.08.2010 / M. Hultsch
						- for ISO 14443A: tag manufacturer strings

						03.01.00 / 17.02.2010 / M. Hultsch
						- new TagHandler class for ISO 15693 STM M24LR64-R

						03.00.14 / 05.10.2009 / M. Hultsch
						- new TagHandler classes for ISO 15693
						- for ISO 15693: tag manufacturer strings

						03.00.13 / 05.10.2009 / M. Hultsch
						- new TagHandler classes for ISO 15693

						03.00.12 / 24.08.2009 / M. Hultsch
						- new method: GetSnr(string& sSnr)
						- new TagHandler classes for MIFARE Plus

						03.00.11 / 06.08.2009 / M. Hultsch
						- #define FEDM_TAG_HANDLER enables TagHandler-Support

						03.00.09 / 01.07.2009 / M. Hultsch
						- inventory with antenna values for HF-Reader

						03.00.07 / 11.05.2009 / M. Hultsch
						- tag handler management
						- inventory with antenna values

Operation Systems	:	independent

Function			:	class for host mode of OBID i-scan® and 
						OBID® classic-pro reader family


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#include "../FEDM.h"
#include "../FEDM_Functions.h"
#include "FEDM_ISOTabItem.h"
#include "FEDM_ISCReader.h"
#include "FEDM_ISCReaderID.h"
#ifdef _FEDM_TAG_HANDLER
	#include "tag_handler/FedmIscTagHandler.h"
#if !defined(_FEDM_NO_TAG_HANDLER_EPC_C1_G2)
	#include "tag_handler/FedmIscTagHandler_EPC_Class1_Gen2.h"
	#include "tag_handler/FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A.h"
#endif
#if !defined(_FEDM_NO_TAG_HANDLER_ISO18000_3M3)
	#include "tag_handler/FedmIscTagHandler_ISO18000_3M3.h"
#endif
#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
	#include "tag_handler/FedmIscTagHandler_ISO14443.h"
	#include "tag_handler/FedmIscTagHandler_ISO14443_Innovatron.h"
	#include "tag_handler/FedmIscTagHandler_ISO14443_2.h"
	#include "tag_handler/FedmIscTagHandler_ISO14443_2_Innovision_Jewel.h"
	#include "tag_handler/FedmIscTagHandler_ISO14443_2_STM_SR176.h"
	#include "tag_handler/FedmIscTagHandler_ISO14443_2_STM_SRIxxx.h"
	#include "tag_handler/FedmIscTagHandler_ISO14443_3.h"
	#include "tag_handler/FedmIscTagHandler_ISO14443_3_ASK_CTx.h"
	#include "tag_handler/FedmIscTagHandler_ISO14443_3_Infineon_my_d.h"
	#include "tag_handler/FedmIscTagHandler_ISO14443_3_Infineon_my_d_move.h"
	#include "tag_handler/FedmIscTagHandler_ISO14443_3_MIFARE_Classic.h"
	#include "tag_handler/FedmIscTagHandler_ISO14443_3_MIFARE_Ultralight.h"
	#include "tag_handler/FedmIscTagHandler_ISO14443_3_MIFARE_Plus.h"
	#include "tag_handler/FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL1.h"
	#include "tag_handler/FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL2.h"
	#include "tag_handler/FedmIscTagHandler_ISO14443_4.h"
	#include "tag_handler/FedmIscTagHandler_ISO14443_4_Maxim.h"
	#include "tag_handler/FedmIscTagHandler_ISO14443_4_Maxim_MAX66000.h"
	#include "tag_handler/FedmIscTagHandler_ISO14443_4_Maxim_MAX66020.h"
	#include "tag_handler/FedmIscTagHandler_ISO14443_4_Maxim_MAX66040.h"
	#include "tag_handler/FedmIscTagHandler_ISO14443_4_MIFARE_Plus.h"
	#include "tag_handler/FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL1.h"
	#include "tag_handler/FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL2.h"
	#include "tag_handler/FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3.h"
	#include "tag_handler/FedmIscTagHandler_ISO14443_4_MIFARE_DESFire.h"
#endif
#if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)
	#include "tag_handler/FedmIscTagHandler_ISO15693.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_STM.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_STM_LRI2K.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_STM_LRIS2K.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_STM_LRIS64K.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_STM_M24LR64R.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_STM_M24LRxxER.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_NXP.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_NXP_ICODE_SLI.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_NXP_ICODE_SLIX.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_L.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_NXP_ICODE_SLIX_L.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_S.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_NXP_ICODE_SLIX_S.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_Infineon.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_Infineon_my_d.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_TI.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_TI_Tag_it_HFI_Pro.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_TI_Tag_it_HFI_Plus.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_Fujitsu.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_EM.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_EM_4034.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_KSW.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_Maxim.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_Maxim_MAX66100.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_Maxim_MAX66120.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_Maxim_MAX66140.h"
	#include "tag_handler/FedmIscTagHandler_ISO15693_IDS_SL13A.h"
#endif
#endif

#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FEDM_ISOTabItem
//####################################################################

FEDM_ISOTabItem::FEDM_ISOTabItem()
{
	m_iRxDB_BlockCount	= 256;
	m_iRxDB_BlockSize	=  32;
	m_iTxDB_BlockCount	= 256;
	m_iTxDB_BlockSize	=  32;

	m_ucMemSizeLen	= 0;
	m_uiTabIndex	= 0;

	m_pReader = NULL;
#ifdef _FEDM_TAG_HANDLER
	m_pTagHandler = NULL;
#endif
}

FEDM_ISOTabItem::FEDM_ISOTabItem(unsigned int uiTabIndex, FEDM_ISCReader* pReader)
{
	m_iRxDB_BlockCount	= 256;
	m_iRxDB_BlockSize	=  32;
	m_iTxDB_BlockCount	= 256;
	m_iTxDB_BlockSize	=  32;

	m_ucMemSizeLen	= 0;
	m_uiTabIndex	= uiTabIndex;

	m_pReader		= pReader;
#ifdef _FEDM_TAG_HANDLER
	m_pTagHandler	= NULL;
#endif
}

FEDM_ISOTabItem::~FEDM_ISOTabItem()
{
	m_ucSecStatus.clear();
	m_ucTxDB.clear();
	m_ucRxDB.clear();
	m_ucTxDB_EpcBank.clear();
	m_ucRxDB_EpcBank.clear();
	m_ucTxDB_TidBank.clear();
	m_ucRxDB_TidBank.clear();
	m_ucTxDB_ResBank.clear();
	m_ucRxDB_ResBank.clear();

#ifdef _FEDM_TAG_HANDLER
	if(m_pTagHandler != NULL)
		delete m_pTagHandler;
#endif
}

#ifdef _FEDM_TAG_HANDLER
void FEDM_ISOTabItem::RemoveTagHandler()
{
	if(m_pTagHandler != NULL)
	{
		delete m_pTagHandler;
		m_pTagHandler = NULL;
	}
}
#endif

bool FEDM_ISOTabItem::HasTagHandler()
{
#ifdef _FEDM_TAG_HANDLER
	if(m_pTagHandler != NULL)
		return true;
#endif

	return false;
}


void FEDM_ISOTabItem::Init()
{
	m_iRxDB_BlockCount	= 256;
	m_iRxDB_BlockSize	=  32;
	m_iTxDB_BlockCount	= 256;
	m_iTxDB_BlockSize	=  32;

	m_ucSecStatus.reserve(FEDM_ISC_ISO_TABLE_SEC_STATUS_SIZE);	// 256 blocks
	m_ucSecStatus.resize(FEDM_ISC_ISO_TABLE_SEC_STATUS_SIZE);	// 256 blocks
	m_ucTxDB.reserve(FEDM_ISC_ISO_TABLE_TxDB_SIZE);	// 256 blocks * 32 bytes = 8192 byte
	m_ucTxDB.resize(FEDM_ISC_ISO_TABLE_TxDB_SIZE);	// 256 blocks * 32 bytes = 8192 byte
	m_ucRxDB.reserve(FEDM_ISC_ISO_TABLE_RxDB_SIZE);	// 256 blocks * 32 bytes = 8192 byte
	m_ucRxDB.resize(FEDM_ISC_ISO_TABLE_RxDB_SIZE);	// 256 blocks * 32 bytes = 8192 byte

	// only for EPCglobal Class1, Generation2 Transponder
	m_ucTxDB_EpcBank.reserve(FEDM_ISC_ISO_TABLE_EPC_BANK_SIZE);	// 128 blocks * 2 bytes
	m_ucTxDB_EpcBank.resize(FEDM_ISC_ISO_TABLE_EPC_BANK_SIZE);	// 128 blocks * 2 bytes
	m_ucRxDB_EpcBank.reserve(FEDM_ISC_ISO_TABLE_EPC_BANK_SIZE);	// 128 blocks * 2 bytes
	m_ucRxDB_EpcBank.resize(FEDM_ISC_ISO_TABLE_EPC_BANK_SIZE);	// 128 blocks * 2 bytes
	m_ucTxDB_TidBank.reserve(FEDM_ISC_ISO_TABLE_TID_BANK_SIZE);	// 128 blocks * 2 bytes
	m_ucTxDB_TidBank.resize(FEDM_ISC_ISO_TABLE_TID_BANK_SIZE);	// 128 blocks * 2 bytes
	m_ucRxDB_TidBank.reserve(FEDM_ISC_ISO_TABLE_TID_BANK_SIZE);	// 128 blocks * 2 bytes
	m_ucRxDB_TidBank.resize(FEDM_ISC_ISO_TABLE_TID_BANK_SIZE);	// 128 blocks * 2 bytes
	m_ucTxDB_ResBank.reserve(FEDM_ISC_ISO_TABLE_RES_BANK_SIZE);	// 128 blocks * 2 bytes
	m_ucTxDB_ResBank.resize(FEDM_ISC_ISO_TABLE_RES_BANK_SIZE);	// 128 blocks * 2 bytes
	m_ucRxDB_ResBank.reserve(FEDM_ISC_ISO_TABLE_RES_BANK_SIZE);	// 128 blocks * 2 bytes
	m_ucRxDB_ResBank.resize(FEDM_ISC_ISO_TABLE_RES_BANK_SIZE);	// 128 blocks * 2 bytes

	memset(m_ucMemSize,			0, 3);
	memset(m_ucSnr,				0, FEDM_ISC_MAX_UID_LENGTH);
	memset(m_ucClass1Gen2_PC,	0, 2);
	memset(m_ucClass1Gen2_XPC_W1,	0, 2);
	memset(&m_ucTxDB[0],		0, FEDM_ISC_ISO_TABLE_TxDB_SIZE);
	memset(&m_ucRxDB[0],		0, FEDM_ISC_ISO_TABLE_RxDB_SIZE);
	memset(&m_ucSecStatus[0],	0, FEDM_ISC_ISO_TABLE_SEC_STATUS_SIZE);
	memset(m_ucTxCB,			0, 16);	//  4 blocks of 4 byte
	memset(m_ucRxCB,			0, 16);	//  4 blocks of 4 byte

	// only for EPCglobal Class1, Generation2 Transponder
	memset(&m_ucTxDB_EpcBank[0],		0, FEDM_ISC_ISO_TABLE_EPC_BANK_SIZE);
	memset(&m_ucRxDB_EpcBank[0],		0, FEDM_ISC_ISO_TABLE_EPC_BANK_SIZE);
	memset(&m_ucTxDB_TidBank[0],		0, FEDM_ISC_ISO_TABLE_TID_BANK_SIZE);
	memset(&m_ucRxDB_TidBank[0],		0, FEDM_ISC_ISO_TABLE_TID_BANK_SIZE);
	memset(&m_ucTxDB_ResBank[0],		0, FEDM_ISC_ISO_TABLE_RES_BANK_SIZE);
	memset(&m_ucRxDB_ResBank[0],		0, FEDM_ISC_ISO_TABLE_RES_BANK_SIZE);

	m_ucAFI				= 0;
	m_ucDsfID			= 0;		// clears the content of the union
	m_ucOptInfo			= 0;		// only ISO 14443A (classic-pro reader)
	m_ucProductCode		= 0;
	m_ucFabCode			= 0;
	m_ucAppCode			= 0;
	m_ucEmbedderCode	= 0;
	m_ucVerlog			= 0;
	m_ucConfig			= 0;
	m_ucAtrLen			= 0;
	memset(m_ucAtr, 0, 33);
	m_ucTrType			= 0xFF;
	m_ucEpcType			= 0x00;
	m_ucSnrLen			= 8;		// only UHF
	m_ucICRef			= 0;
	m_ucBlockSize		= 4;		// default block size: 4
	m_uiValue			= 0;
	m_bIsBlockSizeSet	= false;
	m_bIsSelected		= false;
	m_bIsSnr			= false;
	m_bIsEpc			= false;
	m_bIsAFI			= false;
	m_bIsSysInfo		= false;
	m_bIsISO14443_4Info	= false;	// flag indicates valid 14443-4 Transponder Info
	m_bIsRSSI			= false;

	// ISO14443-4 Transponder Info
	m_ucFSCI			= 0;	// Max. Frame Size
	m_ucFWI				= 0;	// Frame Waiting Time
	m_ucDSI				= 0;	// Divisor Send Integer
	m_ucDRI				= 0;	// Divisor Receive Integer
	m_ucNad				= 0;	// Node Access
	m_ucCid				= 0;	// Card Identifier

	ClearAntennaValues();
}


void FEDM_ISOTabItem::Init(	int iRxDB_BlockCount, 
							int iRxDB_BlockSize, 
							int iTxDB_BlockCount,
							int iTxDB_BlockSize)
{
	m_iRxDB_BlockCount	= iRxDB_BlockCount;
	m_iRxDB_BlockSize	= iRxDB_BlockSize;
	m_iTxDB_BlockCount	= iTxDB_BlockCount;
	m_iTxDB_BlockSize	= iTxDB_BlockSize;

	m_ucSecStatus.reserve(iRxDB_BlockCount);
	m_ucSecStatus.resize(iRxDB_BlockCount);
	m_ucTxDB.reserve(iTxDB_BlockCount * iTxDB_BlockSize);
	m_ucTxDB.resize(iTxDB_BlockCount * iTxDB_BlockSize);
	m_ucRxDB.reserve(iRxDB_BlockCount * iRxDB_BlockSize);
	m_ucRxDB.resize(iRxDB_BlockCount * iRxDB_BlockSize);

	// only for EPCglobal Class1, Generation2 Transponder
	m_ucTxDB_EpcBank.reserve(FEDM_ISC_ISO_TABLE_EPC_BANK_SIZE);	// 128 blocks * 2 bytes
	m_ucTxDB_EpcBank.resize(FEDM_ISC_ISO_TABLE_EPC_BANK_SIZE);	// 128 blocks * 2 bytes
	m_ucRxDB_EpcBank.reserve(FEDM_ISC_ISO_TABLE_EPC_BANK_SIZE);	// 128 blocks * 2 bytes
	m_ucRxDB_EpcBank.resize(FEDM_ISC_ISO_TABLE_EPC_BANK_SIZE);	// 128 blocks * 2 bytes
	m_ucTxDB_TidBank.reserve(FEDM_ISC_ISO_TABLE_TID_BANK_SIZE);	// 128 blocks * 2 bytes
	m_ucTxDB_TidBank.resize(FEDM_ISC_ISO_TABLE_TID_BANK_SIZE);	// 128 blocks * 2 bytes
	m_ucRxDB_TidBank.reserve(FEDM_ISC_ISO_TABLE_TID_BANK_SIZE);	// 128 blocks * 2 bytes
	m_ucRxDB_TidBank.resize(FEDM_ISC_ISO_TABLE_TID_BANK_SIZE);	// 128 blocks * 2 bytes
	m_ucTxDB_ResBank.reserve(FEDM_ISC_ISO_TABLE_RES_BANK_SIZE);	// 128 blocks * 2 bytes
	m_ucTxDB_ResBank.resize(FEDM_ISC_ISO_TABLE_RES_BANK_SIZE);	// 128 blocks * 2 bytes
	m_ucRxDB_ResBank.reserve(FEDM_ISC_ISO_TABLE_RES_BANK_SIZE);	// 128 blocks * 2 bytes
	m_ucRxDB_ResBank.resize(FEDM_ISC_ISO_TABLE_RES_BANK_SIZE);	// 128 blocks * 2 bytes

	memset(m_ucMemSize,			0, 3);
	memset(m_ucSnr,				0, FEDM_ISC_MAX_UID_LENGTH);
	memset(m_ucClass1Gen2_PC,	0, 2);
	memset(m_ucClass1Gen2_XPC_W1,	0, 2);
	memset(&m_ucTxDB[0],		0, iTxDB_BlockCount * iTxDB_BlockSize);
	memset(&m_ucRxDB[0],		0, iRxDB_BlockCount * iRxDB_BlockSize);
	memset(&m_ucSecStatus[0],	0, iRxDB_BlockCount);
	memset(m_ucTxCB,			0, 16);	//  4 blocks of 4 byte
	memset(m_ucRxCB,			0, 16);	//  4 blocks of 4 byte

	// only for EPCglobal Class1, Generation2 Transponder
	memset(&m_ucTxDB_EpcBank[0],		0, FEDM_ISC_ISO_TABLE_EPC_BANK_SIZE);
	memset(&m_ucRxDB_EpcBank[0],		0, FEDM_ISC_ISO_TABLE_EPC_BANK_SIZE);
	memset(&m_ucTxDB_TidBank[0],		0, FEDM_ISC_ISO_TABLE_TID_BANK_SIZE);
	memset(&m_ucRxDB_TidBank[0],		0, FEDM_ISC_ISO_TABLE_TID_BANK_SIZE);
	memset(&m_ucTxDB_ResBank[0],		0, FEDM_ISC_ISO_TABLE_RES_BANK_SIZE);
	memset(&m_ucRxDB_ResBank[0],		0, FEDM_ISC_ISO_TABLE_RES_BANK_SIZE);

	m_ucAFI				= 0;
	m_ucDsfID			= 0;		// clears the content of the union
	m_ucOptInfo			= 0;		// only ISO 14443A
	m_ucProductCode		= 0;
	m_ucFabCode			= 0;
	m_ucAppCode			= 0;
	m_ucEmbedderCode	= 0;
	m_ucVerlog			= 0;
	m_ucConfig			= 0;
	m_ucAtrLen			= 0;
	memset(m_ucAtr, 0, 33);
	m_ucTrType			= 0xFF;
	m_ucEpcType			= 0x00;
	m_ucSnrLen			= 8;		// only HF
	m_ucICRef			= 0;
	
	m_ucBlockSize	= 4;
	if(iRxDB_BlockSize < m_ucBlockSize)
		m_ucBlockSize	= (unsigned char)iRxDB_BlockSize;

	m_uiValue			= 0;
	m_bIsBlockSizeSet	= false;
	m_bIsSelected		= false;
	m_bIsSnr			= false;
	m_bIsEpc			= false;
	m_bIsAFI			= false;
	m_bIsSysInfo		= false;
	m_bIsISO14443_4Info	= false;	// flag indicates valid 14443-4 Transponder Info
	m_bIsRSSI			= false;

	// ISO14443-4 Transponder Info
	m_ucFSCI			= 0;	// Max. Frame Size
	m_ucFWI				= 0;	// Frame Waiting Time
	m_ucDSI				= 0;	// Divisor Send Integer
	m_ucDRI				= 0;	// Divisor Receive Integer
	m_ucNad				= 0;	// Node Access
	m_ucCid				= 0;	// Card Identifier

	ClearAntennaValues();
}

void FEDM_ISOTabItem::GetTableSizes(int& iRxDB_BlockCount, int& iRxDB_BlockSize, int& iTxDB_BlockCount, int& iTxDB_BlockSize)
{
	iRxDB_BlockCount	= m_iRxDB_BlockCount;
	iRxDB_BlockSize		= m_iRxDB_BlockSize;
	iTxDB_BlockCount	= m_iTxDB_BlockCount;
	iTxDB_BlockSize		= m_iTxDB_BlockSize;
}

void FEDM_ISOTabItem::ClearAntennaValues()
{
	m_ucFlags		= 0;
	m_ucAntCount	= 0;	// number of antenna reads
	memset(&m_ucAntNumber[0],	0, FEDM_ISC_ISO_TABLE_MAX_ANTENNA);	// antenna number
	memset(&m_ucAntStatus[0],	0, FEDM_ISC_ISO_TABLE_MAX_ANTENNA);	// antenna status
	memset(&m_ucAntRSSI[0],		0, FEDM_ISC_ISO_TABLE_MAX_ANTENNA);	// antenna Received Signal Strength Indication
}

bool FEDM_ISOTabItem::IsExtendedPC_W1()
{
	if(m_ucClass1Gen2_PC[0] & 0x02)
		return true;
	else
		return false;
}

// serial number as STL-string
int FEDM_ISOTabItem::GetSnr(std::string& sSnr)
{
	return FEDM_ConvHexUCharToHexStr(m_ucSnr, m_ucSnrLen, sSnr);
}

// get lower serial number as 64-bit integer
__int64 FEDM_ISOTabItem::GetSnr()
{
	__int64 i64Snr = 0;

	// build serial number and change Motorola format into Intel format
	for ( int i=0; i<8; i++ )
		i64Snr += ( (__int64)m_ucSnr[7-i] << (8*i) );

	return i64Snr;
}

// get part of serial number as 64-bit integer
// iPartNr 1..4
__int64 FEDM_ISOTabItem::GetSnr(int i64BitPart)
{
	__int64 i64Snr = 0;

	if(i64BitPart < 1 || i64BitPart > 4)
		return 0;

	i64BitPart--;

	// build serial number and change Motorola format into Intel format
	for ( int i=i64BitPart*8; i<(i64BitPart+1)*8; i++ )
		i64Snr += ( (__int64)m_ucSnr[(i64BitPart+1)*8-i] << (8*(i-(i64BitPart*8))) );

	return i64Snr;
}

int	FEDM_ISOTabItem::GetEpc(char* cEpc, int iBufLen)
{
	if(! m_bIsEpc)
		return FEDM_ERROR_NOT_AN_EPC;

	switch(m_ucEpcType)
	{
	case FEDM_ISC_EPC_TYPE_1:
		if(iBufLen < 28)
			return FEDM_ERROR_BUFFER_LENGTH;

		sprintf(cEpc, "%02X.%07lX.%06lX.%09llX", GetEpcHeader(), (unsigned long)GetEpcDomainManager(), (unsigned long)GetEpcObjectClass(), GetEpcSnr());
		break;

	case FEDM_ISC_EPC_TYPE_2:
		if(iBufLen < 23)
			return FEDM_ERROR_BUFFER_LENGTH;

		sprintf(cEpc, "%02X.%06lX.%05lX.%06lX", GetEpcHeader(), (unsigned long)GetEpcDomainManager(), (unsigned long)GetEpcObjectClass(), (unsigned long)GetEpcSnr());
		break;

	case FEDM_ISC_EPC_TYPE_3:
		if(iBufLen < 23)
			return FEDM_ERROR_BUFFER_LENGTH;

		sprintf(cEpc, "%02X.%04lX.%04lX.%09llX", GetEpcHeader(), (unsigned long)GetEpcDomainManager(), (unsigned long)GetEpcObjectClass(), GetEpcSnr());
		break;

	case FEDM_ISC_EPC_TYPE_4:
		if(iBufLen < 23)
			return FEDM_ERROR_BUFFER_LENGTH;

		sprintf(cEpc, "%02X.%07lX.%04lX.%08lX", GetEpcHeader(), (unsigned long)GetEpcDomainManager(), (unsigned long)GetEpcObjectClass(), (unsigned long)GetEpcSnr());
		break;

	default:
		return FEDM_ERROR_UNKNOWN_EPC_TYPE;
	}

	return FEDM_OK;
}

int	FEDM_ISOTabItem::GetEpcRaw(unsigned char* cEpc, int iBufLen)
{
	if(! m_bIsEpc)
		return FEDM_ERROR_NOT_AN_EPC;

	switch(m_ucEpcType)
	{
	case FEDM_ISC_EPC_TYPE_1:
		if(iBufLen < 12)
			return FEDM_ERROR_BUFFER_LENGTH;

		memcpy(cEpc, (const char*)m_ucSnr, 12);
		break;

	case FEDM_ISC_EPC_TYPE_2:
	case FEDM_ISC_EPC_TYPE_3:
	case FEDM_ISC_EPC_TYPE_4:
		if(iBufLen < 8)
			return FEDM_ERROR_BUFFER_LENGTH;

		memcpy(cEpc, (const char*)m_ucSnr, 8);
		break;

	default:
		return FEDM_ERROR_UNKNOWN_EPC_TYPE;
	}

	return FEDM_OK;
}

int FEDM_ISOTabItem::GetEpcHeader()
{
	if(! m_bIsEpc)
		return FEDM_ERROR_NOT_AN_EPC;

	switch(m_ucEpcType)
	{
	case FEDM_ISC_EPC_TYPE_1:
		return m_ucSnr[0];
	case FEDM_ISC_EPC_TYPE_2:
		return ((m_ucSnr[0]&0xC0)>>6);
	case FEDM_ISC_EPC_TYPE_3:
		return ((m_ucSnr[0]&0xC0)>>6);
	case FEDM_ISC_EPC_TYPE_4:
		return ((m_ucSnr[0]&0xC0)>>6);
	}

	return FEDM_ERROR_UNKNOWN_EPC_TYPE;
}

__int64 FEDM_ISOTabItem::GetEpcDomainManager()
{
	if(! m_bIsEpc)
		return FEDM_ERROR_NOT_AN_EPC;

	switch(m_ucEpcType)
	{
	case FEDM_ISC_EPC_TYPE_1:
		return (((__int64)m_ucSnr[1]<<20) + ((__int64)m_ucSnr[2]<<12) + ((__int64)m_ucSnr[3]<<4) + ((__int64)(m_ucSnr[4]&0xF0)>>4));
	case FEDM_ISC_EPC_TYPE_2:
		return (((__int64)(m_ucSnr[0]&0x3F)<<15) + ((__int64)m_ucSnr[1]<<7) + ((__int64)(m_ucSnr[2]&0xFE)>>1));
	case FEDM_ISC_EPC_TYPE_3:
		return (((__int64)(m_ucSnr[0]&0x3F)<<9) + ((__int64)m_ucSnr[1]<<1) + ((__int64)(m_ucSnr[2]&0x80)>>7));
	case FEDM_ISC_EPC_TYPE_4:
		return (((__int64)(m_ucSnr[0]&0x3F)<<20) + ((__int64)m_ucSnr[1]<<12) + ((__int64)m_ucSnr[2]<<4) + ((__int64)(m_ucSnr[3]&0xF0)>>4));
	}

	return FEDM_ERROR_UNKNOWN_EPC_TYPE;
}

__int64 FEDM_ISOTabItem::GetEpcObjectClass()
{
	if(! m_bIsEpc)
		return FEDM_ERROR_NOT_AN_EPC;

	switch(m_ucEpcType)
	{
	case FEDM_ISC_EPC_TYPE_1:
		return (((m_ucSnr[4]&0x0F)<<20) + (m_ucSnr[5]<<12) + (m_ucSnr[6]<<4) + ((m_ucSnr[7]&0xF0)>>4));
	case FEDM_ISC_EPC_TYPE_2:
		return (((m_ucSnr[2]&0x01)<<16) + (m_ucSnr[3]<<8) + m_ucSnr[4]);
	case FEDM_ISC_EPC_TYPE_3:
		return (((m_ucSnr[2]&0x7F)<<6) + ((m_ucSnr[3]&0xFC)>>2));
	case FEDM_ISC_EPC_TYPE_4:
		return (((m_ucSnr[3]&0x0F)<<9) + (m_ucSnr[4]<<1) + ((m_ucSnr[5]&0x80)>>7));
	}

	return FEDM_ERROR_UNKNOWN_EPC_TYPE;
}

__int64 FEDM_ISOTabItem::GetEpcSnr()
{
	__int64 i64Data = 0;

	if(! m_bIsEpc)
		return FEDM_ERROR_NOT_AN_EPC;

	switch(m_ucEpcType)
	{
	case FEDM_ISC_EPC_TYPE_1:
		i64Data = (((__int64)(m_ucSnr[7]&0x0F))<<32) + (((__int64)(m_ucSnr[8]))<<24) + (m_ucSnr[9]<<16) + (m_ucSnr[10]<<8) + m_ucSnr[11];
		break;
	case FEDM_ISC_EPC_TYPE_2:
		i64Data = (m_ucSnr[5]<<16) + (m_ucSnr[6]<<8) + m_ucSnr[7];
		break;
	case FEDM_ISC_EPC_TYPE_3:
		i64Data = ((__int64)(m_ucSnr[3]&0x03)<<32) + (m_ucSnr[4]<<24) + (m_ucSnr[5]<<16) + (m_ucSnr[6]<<8) + m_ucSnr[7];
		break;
	case FEDM_ISC_EPC_TYPE_4:
		i64Data = ((m_ucSnr[5]&0x7F)<<16) + (m_ucSnr[6]<<8) + m_ucSnr[7];
		break;
	default:
		return FEDM_ERROR_UNKNOWN_EPC_TYPE;
	}

	return i64Data;
}

int FEDM_ISOTabItem::SetData(unsigned char ucCmd, unsigned char* ucData, int& iByteCnt, unsigned char ucBank, unsigned int uiAdr)
{
	bool bAntenna = false;
	int	iCnt;
	int iErr = 0;

	switch(ucCmd)
	{
	case 0x01:	// [0x01] Read Serial Number
		FEDM_CHK1(iErr, SetSnr(ucData, iByteCnt));
		m_pReader->GetData(FEDM_ISC_TMP_B0_MODE_ANT, &bAntenna);
		if(bAntenna)
		{
			if(m_ucFlags & 0x01)
				m_bIsSnr = true;
		}
		else
		{
			m_bIsSnr = true;
		}
		break;

	case 0x23:	// [0x23] Read Multiple Blocks
		if( uiAdr >= m_ucSecStatus.size())
		{
			FEDM_TRACE(m_pReader->m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(Cmd=0x23, ..) - uiAdr >= m_ucSecStatus.size()");
			return FEDM_ERROR_ARRAY_BOUNDARY;	// array overflow
		}

		m_ucSecStatus[uiAdr] = ucData[iByteCnt++];

		switch(ucBank)
		{
		case FEDM_ISC_ISO_BANK_RESERVED:
			if( ((uiAdr+1)*m_ucBlockSize - 1) >= m_ucRxDB_ResBank.size())
			{
				FEDM_TRACE(m_pReader->m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(Cmd=0x23, ..) - (uiAdr+1)*m_ucBlockSize - 1 >= m_ucRxDB_ResBank.size()");
				return FEDM_ERROR_ARRAY_BOUNDARY;	// array overflow
			}

			// attention: swap of byte order (MSB <-> LSB)
			for(iCnt=0; iCnt<m_ucBlockSize; iCnt++)
				m_ucRxDB_ResBank[(uiAdr+1)*m_ucBlockSize - 1 - iCnt] = ucData[iByteCnt++];
			break;
		
		case FEDM_ISC_ISO_BANK_EPC_MEMORY:
			if( ((uiAdr+1)*m_ucBlockSize - 1) >= m_ucRxDB_EpcBank.size())
			{
				FEDM_TRACE(m_pReader->m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(Cmd=0x23, ..) - (uiAdr+1)*m_ucBlockSize - 1 >= m_ucRxDB_EpcBank.size()");
				return FEDM_ERROR_ARRAY_BOUNDARY;	// array overflow
			}

			// attention: swap of byte order (MSB <-> LSB)
			for(iCnt=0; iCnt<m_ucBlockSize; iCnt++)
				m_ucRxDB_EpcBank[(uiAdr+1)*m_ucBlockSize - 1 - iCnt] = ucData[iByteCnt++];
			break;
		
		case FEDM_ISC_ISO_BANK_TID_MEMORY:
			if( ((uiAdr+1)*m_ucBlockSize - 1) >= m_ucRxDB_TidBank.size())
			{
				FEDM_TRACE(m_pReader->m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(Cmd=0x23, ..) - (uiAdr+1)*m_ucBlockSize - 1 >= m_ucRxDB_TidBank.size()");
				return FEDM_ERROR_ARRAY_BOUNDARY;	// array overflow
			}

			// attention: swap of byte order (MSB <-> LSB)
			for(iCnt=0; iCnt<m_ucBlockSize; iCnt++)
				m_ucRxDB_TidBank[(uiAdr+1)*m_ucBlockSize - 1 - iCnt] = ucData[iByteCnt++];
			break;
		
		case FEDM_ISC_ISO_BANK_USER_MEMORY:
			if( ((uiAdr+1)*m_ucBlockSize - 1) >= m_ucRxDB.size())
			{
				FEDM_TRACE(m_pReader->m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(Cmd=0x23, ..) - (uiAdr+1)*m_ucBlockSize - 1 >= m_ucRxDB.size()");
				return FEDM_ERROR_ARRAY_BOUNDARY;	// array overflow
			}

			// attention: swap of byte order (MSB <-> LSB)
			for(iCnt=0; iCnt<m_ucBlockSize; iCnt++)
				m_ucRxDB[(uiAdr+1)*m_ucBlockSize - 1 - iCnt] = ucData[iByteCnt++];
			break;

		default:
			FEDM_TRACE(m_pReader->m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(Cmd=0x23, ..) - unknown bank");
			return FEDM_ERROR_PARAMETER;
		}
		break;

	case 0xA0:	// [0xA0] Read Config Block
		if(uiAdr > 3)
		{
			FEDM_TRACE(m_pReader->m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(Cmd=0xA0, ..) - uiAdr > 3");
			return FEDM_ERROR_ARRAY_BOUNDARY;	// array overflow
		}

		for(iCnt=0; iCnt<4; iCnt++)
			m_ucRxCB[uiAdr][iCnt] = ucData[iByteCnt++];

		break;

	default:
		FEDM_TRACE(m_pReader->m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(Cmd=0x" << std::hex << std::uppercase << (int)ucCmd << ", ..) - unknown command");
		return FEDM_ERROR_UNKNOWN_COMMAND;
	}

	return FEDM_OK;
}

int FEDM_ISOTabItem::GetData(unsigned char ucCmd, unsigned char* ucData, int& iByteCnt, unsigned char ucBank, unsigned int uiAdr)
{
	int iCnt;

	switch(ucCmd)
	{
	case 0x24:	// [0x24] Write Multiple Blocks
		switch(ucBank)
		{
		case FEDM_ISC_ISO_BANK_RESERVED:
			if( ((uiAdr+1)*m_ucBlockSize - 1) >= m_ucTxDB_ResBank.size())
			{
				FEDM_TRACE(m_pReader->m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(Cmd=0x24, ..) - (uiAdr+1)*m_ucBlockSize - 1 >= m_ucTxDB_ResBank.size()");
				return FEDM_ERROR_ARRAY_BOUNDARY;	// array overflow
			}

			// attention: swap of byte order (MSB <-> LSB)
			for(iCnt=0; iCnt<m_ucBlockSize; iCnt++)
				ucData[iByteCnt++] = m_ucTxDB_ResBank[(uiAdr+1)*m_ucBlockSize - 1 - iCnt];
			break;
		
		case FEDM_ISC_ISO_BANK_EPC_MEMORY:
			if( ((uiAdr+1)*m_ucBlockSize - 1) >= m_ucTxDB_EpcBank.size())
			{
				FEDM_TRACE(m_pReader->m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(Cmd=0x24, ..) - (uiAdr+1)*m_ucBlockSize - 1 >= m_ucTxDB_EpcBank.size()");
				return FEDM_ERROR_ARRAY_BOUNDARY;	// array overflow
			}

			// attention: swap of byte order (MSB <-> LSB)
			for(iCnt=0; iCnt<m_ucBlockSize; iCnt++)
				ucData[iByteCnt++] = m_ucTxDB_EpcBank[(uiAdr+1)*m_ucBlockSize - 1 - iCnt];
			break;
		
		case FEDM_ISC_ISO_BANK_TID_MEMORY:
			if( ((uiAdr+1)*m_ucBlockSize - 1) >= m_ucTxDB_TidBank.size())
			{
				FEDM_TRACE(m_pReader->m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(Cmd=0x24, ..) - (uiAdr+1)*m_ucBlockSize - 1 >= m_ucTxDB_TidBank.size()");
				return FEDM_ERROR_ARRAY_BOUNDARY;	// array overflow
			}

			// attention: swap of byte order (MSB <-> LSB)
			for(iCnt=0; iCnt<m_ucBlockSize; iCnt++)
				ucData[iByteCnt++] = m_ucTxDB_TidBank[(uiAdr+1)*m_ucBlockSize - 1 - iCnt];
			break;
		
		case FEDM_ISC_ISO_BANK_USER_MEMORY:
			if( ((uiAdr+1)*m_ucBlockSize - 1) >= m_ucTxDB.size())
			{
				FEDM_TRACE(m_pReader->m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(Cmd=0x24, ..) - (uiAdr+1)*m_ucBlockSize - 1 >= m_ucTxDB.size()");
				return FEDM_ERROR_ARRAY_BOUNDARY;	// array overflow
			}

			// attention: swap of byte order (MSB <-> LSB)
			for(iCnt=0; iCnt<m_ucBlockSize; iCnt++)
				ucData[iByteCnt++] = m_ucTxDB[(uiAdr+1)*m_ucBlockSize - 1 - iCnt];
			break;

		default:
			FEDM_TRACE(m_pReader->m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(Cmd=0x23, ..) - unknown bank");
			return FEDM_ERROR_PARAMETER;
		}
		break;

	case 0xA1:	// [0xA1] Write Config Block
		if(uiAdr > 3)
		{
			FEDM_TRACE(m_pReader->m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(Cmd=0xA1, ..) - uiAdr > 3");
			return FEDM_ERROR_ARRAY_BOUNDARY;	// array overflow
		}

		for(iCnt=0; iCnt<4; iCnt++)
			ucData[iByteCnt++] = m_ucTxCB[uiAdr][iCnt];

		break;

	default:
		FEDM_TRACE(m_pReader->m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(Cmd=0x" << std::hex << std::uppercase << (int)ucCmd << ", ..) - unknown command");
		return FEDM_ERROR_UNKNOWN_COMMAND;
	}

	return FEDM_OK;
}

int FEDM_ISOTabItem::SetSnr(unsigned char* ucData, int& iByteCnt)
{
	bool	bAntenna = false;
	int iBack = 0;

	m_pReader->GetData(FEDM_ISC_TMP_B0_MODE_ANT, &bAntenna);

	m_bIsEpc = false;

	if(bAntenna)
	{
		FEDM_CHK1(iBack, SetSnr_with_Antenna(ucData, iByteCnt));
	}
	else
	{
		FEDM_CHK1(iBack, SetSnr_wo_Antenna(ucData, iByteCnt));
	}

#ifdef _FEDM_TAG_HANDLER
	if( m_pReader != NULL && m_pReader->m_bEnableTagHandler )
	{
		FEDM_CHK1(iBack, CreateTagHandler());
	}
#endif

	return FEDM_OK;
}

const char* FEDM_ISOTabItem::GetISO15693Manufacturer()
{
	if(m_ucTrType == FEDM_ISC_TR_TYPE_ISO15693)
	{
		return FEDM_TabItem::GetISO15693Manufacturer(m_ucSnr[1]);
	}

	return "";
}


const char* FEDM_ISOTabItem::GetISO14443AManufacturer()
{
	if(m_ucTrType == FEDM_ISC_TR_TYPE_ISO14443A)
	{
		if(m_ucTrInfo & 0x04)
		{
			return FEDM_TabItem::GetISO15693Manufacturer(m_ucSnr[9]);
		}
		else
		{
			if( (m_ucOptInfo & 0x03) > 0)
				return FEDM_TabItem::GetISO15693Manufacturer(m_ucSnr[7]);
		}
	}

	return "";
}

/*
TID Memory content in the first 4 bytes
bit 0..7: ISO/IEC 15963 (ISO/IEC 7816-6) class-identifier

If class-identifier is:
0xE0: ISO/IEC 7816-6
0xE3: ISO/IEC 7816-6

bit  8..15: manufacturer identifier
bit 16..63: unique Tag serial number


If class-identifier is:
0xE2: EPC Class1 Gen2

bit  8: XTID (X) indicator (whether a Tag implements an XTID)
bit  9: Security (S) indicator (whether a Tag supports the Authenticate and/or Challenge commands)
bit 10: File (F) indicator (whether a Tag supports the FileOpen command)
bit 11..19: a 9-bit Tag mask-designer identifier (obtainable from the registration authority)
bit 20..31: a Tag-manufacturer-defined 12-bit Tag model number
bit >= 32.. : as defined in the GS1 EPC Tag Data Standard

Notes:
EPC standard has reverse byte order compared with data blocks from OBID Readers !!
Thus: bit 0..7 is in TID[1] and so on
*/

unsigned int FEDM_ISOTabItem::GetEpcC1G2TagModelNumber()
{
	if(	m_ucTrType == FEDM_ISC_TR_TYPE_ISO18000_3M3 ||
		m_ucTrType == FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2 )
	{
		switch(m_ucRxDB_TidBank[1])
		{
		case 0xE0: // ISO/IEC 7816-6
		case 0xE3: // ISO/IEC 7816-6
			break;
		case 0xE2: // EPC C1G2
			return (((unsigned int)(m_ucRxDB_TidBank[3] & 0x0F)) << 8) + m_ucRxDB_TidBank[2];
		}
	}

	return 0xFFFFFFFF; // indicates an error
}

unsigned int FEDM_ISOTabItem::GetEpcC1G2MaskDesignerID()
{
	if(	m_ucTrType == FEDM_ISC_TR_TYPE_ISO18000_3M3 ||
		m_ucTrType == FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2 )
	{
		switch(m_ucRxDB_TidBank[1])
		{
		case 0xE0: // ISO/IEC 7816-6
		case 0xE3: // ISO/IEC 7816-6
			return (unsigned int)m_ucRxDB_TidBank[0];
		case 0xE2: // EPC C1G2
			return (((unsigned int)(m_ucRxDB_TidBank[0] & 0x1F)) << 4) + ((m_ucRxDB_TidBank[3] & 0xF0) >> 4);
		}
	}

	return 0xFFFFFFFF; // indicates an error
}

const char* FEDM_ISOTabItem::GetEpcC1G2MaskDesignerName()
{
	unsigned int uiMDID = 0;

	if(	m_ucTrType == FEDM_ISC_TR_TYPE_ISO18000_3M3 ||
		m_ucTrType == FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2 )
	{
		switch(m_ucRxDB_TidBank[1])
		{
		case 0xE0: // ISO/IEC 7816-6
		case 0xE3: // ISO/IEC 7816-6
			return FEDM_TabItem::GetISO15693Manufacturer(m_ucRxDB_TidBank[0]);
		case 0xE2: // EPC C1G2
			uiMDID = ((m_ucRxDB_TidBank[0] & 0x1F) << 4) + ((m_ucRxDB_TidBank[3] & 0xF0) >> 4);
			return FEDM_TabItem::GetEpcC1G2MaskDesignerName(uiMDID);
		}
	}

	return "";
}

#ifdef _FEDM_TAG_HANDLER
const char* FEDM_ISOTabItem::GetTagName()
{
	if(m_pTagHandler != NULL)
		return m_pTagHandler->GetTagName();

	return "";
}
#endif

/***************************************************************************
  Begin        		:	06.05.2009 / M. Hultsch

  Version       	:	04.03.01 / 15.11.2012 / M. Hultsch
						- support for ISO 18000-3M3


						04.02.01 / 31.05.2012 / M. Hultsch
						- support for Innovatron and ASK CTx

						03.03.01 / 08.04.2011 / M. Hultsch

  Function			:	creates a new tag handler class, if necessary
						NOTE: old tag handler class will not be deleted if the 
							  handler type is the same as the new handler type.
							  This reduces memory allocation

  Parameters		:	-

  Return value		:	0				- if creation was successful
						error code (<0)	- if something goes wrong
***************************************************************************/
#ifdef _FEDM_TAG_HANDLER
int FEDM_ISOTabItem::CreateTagHandler()
{
	switch(m_ucTrType)
	{
	case FEDM_ISC_TR_TYPE_ICODE1:
	case FEDM_ISC_TR_TYPE_TAGIT:
	case FEDM_ISC_TR_TYPE_EPC:
	case FEDM_ISC_TR_TYPE_ICODE_UID:
	case FEDM_ISC_TR_TYPE_MCRFxxx:
	case FEDM_ISC_TR_TYPE_ISO18000_6_A:
	case FEDM_ISC_TR_TYPE_ISO18000_6_B:
	case FEDM_ISC_TR_TYPE_EPC_CLASS0:
	case FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN1:
	case FEDM_ISC_TR_TYPE_EM4222:
		if(m_pTagHandler != NULL)
		{
			if(m_pTagHandler->m_uiTagHandlerType != FedmIscTagHandler::TYPE_BASIC)
			{
				delete m_pTagHandler;
				m_pTagHandler = new FedmIscTagHandler(m_pReader, FedmIscTagHandler::TYPE_BASIC, this);
			}
			else
			{
				m_pTagHandler->Init();
			}
		}
		else
		{
			m_pTagHandler = new FedmIscTagHandler(m_pReader, FedmIscTagHandler::TYPE_BASIC, this);
		}

		break;

#if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)
	case FEDM_ISC_TR_TYPE_ISO15693:
		CreateTagHandler_ISO15693();
		break;

	/*case FEDM_ISC_TR_TYPE_0x64:	// is an ISO15693
		if(m_pTagHandler != NULL)
		{
			if(m_pTagHandler->m_uiTagHandlerType != FedmIscTagHandler::TYPE_ISO15693)
			{
				delete m_pTagHandler;
				m_pTagHandler = new FedmIscTagHandler_ISO15693(m_pReader, this);
			}
			else
			{
				m_pTagHandler->Init();
			}
		}
		else
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693(m_pReader, this);
		}

		break;*/
#endif

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
	case FEDM_ISC_TR_TYPE_ISO14443A:
	case FEDM_ISC_TR_TYPE_ISO14443B:
		// preliminary tag handler to execute a select command
		// concrete tag handler can be created not until select command
		if(m_pTagHandler != NULL)
		{
			if(m_pReader->m_bTagHandlerOption_CreateCondition_Smart)
			{
				if(dynamic_cast<FedmIscTagHandler_ISO14443*>(m_pTagHandler) == NULL) // keep TagHandler alive, if the base is from ISO 14443
				{
					delete m_pTagHandler;
					m_pTagHandler = new FedmIscTagHandler_ISO14443(m_pReader, this);
				}
				else
				{
					m_pTagHandler->Init();
				}
			}
			else
			{
				if(m_pTagHandler->m_uiTagHandlerType != FedmIscTagHandler::TYPE_ISO14443)
				{
					delete m_pTagHandler;
					m_pTagHandler = new FedmIscTagHandler_ISO14443(m_pReader, this);
				}
				else
				{
					m_pTagHandler->Init();
				}
			}
		}
		else
		{
			m_pTagHandler = new FedmIscTagHandler_ISO14443(m_pReader, this);
		}

		break;

	case FEDM_ISC_TR_TYPE_JEWEL:
		if(m_pTagHandler != NULL)
		{
			if(m_pTagHandler->m_uiTagHandlerType != FedmIscTagHandler::TYPE_ISO14443_2_INNOVISION_JEWEL)
			{
				delete m_pTagHandler;
				m_pTagHandler = new FedmIscTagHandler_ISO14443_2_Innovision_Jewel(m_pReader, this);
			}
			else
			{
				m_pTagHandler->Init();
			}
		}
		else
		{
			m_pTagHandler = new FedmIscTagHandler_ISO14443_2_Innovision_Jewel(m_pReader, this);
		}

		break;

	case FEDM_ISC_TR_TYPE_STM_SR176:
		if(m_pTagHandler != NULL)
		{
			if(m_pTagHandler->m_uiTagHandlerType != FedmIscTagHandler::TYPE_ISO14443_2_STM_SR176)
			{
				delete m_pTagHandler;
				m_pTagHandler = new FedmIscTagHandler_ISO14443_2_STM_SR176(m_pReader, this);
			}
			else
			{
				m_pTagHandler->Init();
			}
		}
		else
		{
			m_pTagHandler = new FedmIscTagHandler_ISO14443_2_STM_SR176(m_pReader, this);
		}

		break;

	case FEDM_ISC_TR_TYPE_STM_SRIxx:
		if(m_pTagHandler != NULL)
		{
			if(m_pTagHandler->m_uiTagHandlerType != FedmIscTagHandler::TYPE_ISO14443_2_STM_SRIxxx)
			{
				delete m_pTagHandler;
				m_pTagHandler = new FedmIscTagHandler_ISO14443_2_STM_SRIxxx(m_pReader, this);
			}
			else
			{
				m_pTagHandler->Init();
			}
		}
		else
		{
			m_pTagHandler = new FedmIscTagHandler_ISO14443_2_STM_SRIxxx(m_pReader, this);
		}

		break;

	case FEDM_ISC_TR_TYPE_INNOVATRON:
		if(m_pTagHandler != NULL)
		{
			if(m_pTagHandler->m_uiTagHandlerType != FedmIscTagHandler::TYPE_ISO14443_INNOVATRON)
			{
				delete m_pTagHandler;
				m_pTagHandler = new FedmIscTagHandler_ISO14443_Innovatron(m_pReader, this);
			}
			else
			{
				m_pTagHandler->Init();
			}
		}
		else
		{
			m_pTagHandler = new FedmIscTagHandler_ISO14443_Innovatron(m_pReader, this);
		}

		break;

	case FEDM_ISC_TR_TYPE_ASK_CTx:
		if(m_pTagHandler != NULL)
		{
			if(m_pTagHandler->m_uiTagHandlerType != FedmIscTagHandler::TYPE_ISO14443_3_ASK_CTX)
			{
				delete m_pTagHandler;
				m_pTagHandler = new FedmIscTagHandler_ISO14443_3_ASK_CTx(m_pReader, this);
			}
			else
			{
				m_pTagHandler->Init();
			}
		}
		else
		{
			m_pTagHandler = new FedmIscTagHandler_ISO14443_3_ASK_CTx(m_pReader, this);
		}

		break;
#endif

#if !defined(_FEDM_NO_TAG_HANDLER_ISO18000_3M3)
	case FEDM_ISC_TR_TYPE_ISO18000_3M3:
		if(m_pTagHandler != NULL)
		{
			if(dynamic_cast<FedmIscTagHandler_ISO18000_3M3*>(m_pTagHandler) == NULL)
			{
				delete m_pTagHandler;
				m_pTagHandler = new FedmIscTagHandler_ISO18000_3M3(m_pReader, this);
			}
			else
			{
				m_pTagHandler->Init();
			}
		}
		else
		{
			m_pTagHandler = new FedmIscTagHandler_ISO18000_3M3(m_pReader, this);
		}

		break;
#endif

#if !defined(_FEDM_NO_TAG_HANDLER_EPC_C1_G2)
	case FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2:
		if(m_pTagHandler != NULL)
		{
			//if(m_pTagHandler->m_uiTagHandlerType != FedmIscTagHandler::TYPE_EPC_CLASS1_GEN2)
			if(dynamic_cast<FedmIscTagHandler_EPC_Class1_Gen2*>(m_pTagHandler) == NULL)
			{
				delete m_pTagHandler;
				m_pTagHandler = new FedmIscTagHandler_EPC_Class1_Gen2(m_pReader, this);
			}
			else
			{
				m_pTagHandler->Init();
			}
		}
		else
		{
			m_pTagHandler = new FedmIscTagHandler_EPC_Class1_Gen2(m_pReader, this);
		}

		break;
#endif

	case FEDM_ISC_TR_TYPE_KEYBOARD:
		// no tag handler necessary
		break;

	default:
		if(m_pTagHandler != NULL)
		{
			if(m_pTagHandler->m_uiTagHandlerType != FedmIscTagHandler::TYPE_BASIC)
			{
				delete m_pTagHandler;
				m_pTagHandler = new FedmIscTagHandler(m_pReader, FedmIscTagHandler::TYPE_BASIC, this);
			}
			else
			{
				m_pTagHandler->Init();
			}
		}
		else
		{
			m_pTagHandler = new FedmIscTagHandler(m_pReader, FedmIscTagHandler::TYPE_BASIC, this);
		}

		break;
	}

	return FEDM_OK;
}
#endif

/***************************************************************************
  Begin        		:	05.10.2009 / M. Hultsch

  Version       	:	04.03.01 / 25.09.2012 / M. Hultsch
						- support for new tag types: ISO 15693 STM M24LRxxE-R, STM LRIS64K

						03.03.01 / 31.03.2011 / M. Hultsch

  Function			:	creates a new tag handler class for ISO 15693 tags, if necessary
						NOTE: old tag handler class will not be deleted if the 
							  handler type is the same as the new handler type.
							  This reduces memory allocation

  Parameters		:	-

  Return value		:	0				- if creation was successful
						error code (<0)	- if something goes wrong
***************************************************************************/
#ifdef _FEDM_TAG_HANDLER
#if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)
int FEDM_ISOTabItem::CreateTagHandler_ISO15693()
{
	bool bCreateNew = false;
	unsigned int uiTagHandlerType = 0;

	// new tag handler type is determined by
	// a) tag manufacturer code according ISO 7816-6
	// b) chip identifier
	switch(m_ucSnr[1])
	{
	case FEDM_ISC_ISO_MFR_STM:
		if( (m_ucSnr[2]&0xFC) == 0x20 )			// LRI2K
			uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_STM_LRI2K;
		else if( (m_ucSnr[2]&0xFC) == 0x40 )	// LRI1K
			uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_STM_LRI2K;
		else if( (m_ucSnr[2]&0xFC) == 0x28 )	// LRIS2K
			uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_STM_LRIS2K;
		else if( (m_ucSnr[2]&0xFC) == 0x2C )	// M24LR64-R
			uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_STM_M24LR64R;
		else if( (m_ucSnr[2]&0xFC) == 0x44 )	// LRIS64K
			uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_STM_LRIS64K;
		else if( (m_ucSnr[2]&0xFC) == 0x5C )	// M24LR64E-R
			uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_STM_M24LRxxER;
		else if( (m_ucSnr[2]&0xFC) == 0x4C )	// M24LR16E-R
			uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_STM_M24LRxxER;
		else if( (m_ucSnr[2]&0xFC) == 0x58 )	// M24LR04E-R
			uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_STM_M24LRxxER;
		else
			uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_STM;
		break;

	case FEDM_ISC_ISO_MFR_NXP:
		if( m_ucSnr[2] == 0x01 ) // chip-ID
		{
			if( m_ucSnr[3] & 0x10 )
				uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_NXP_ICODE_SLIX;
			else if( m_ucSnr[3] & 0x08 )
				uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_NXP_ICODE_SLIX; // in future: SLIX2 ?
			else
				uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_NXP_ICODE_SLI;
		}
		else if( m_ucSnr[2] == 0x02 ) // chip-ID
		{
			if( m_ucSnr[3] & 0x10 )
				uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_NXP_ICODE_SLIX_S;
			else
				uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_NXP_ICODE_SLI_S;
		}
		else if( m_ucSnr[2] == 0x03 ) // chip-ID
		{
			if( m_ucSnr[3] & 0x10 )
				uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_NXP_ICODE_SLIX_L;
			else
				uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_NXP_ICODE_SLI_L;
		}
		else
		{
			uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_NXP;
		}
		break;

	case FEDM_ISC_ISO_MFR_INFINEON:
		if( (m_ucSnr[2]&0x1E) == 0x00 )
			uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_Infineon_my_d;
		else
			uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_Infineon;
		break;

	case FEDM_ISC_ISO_MFR_TI:
		if( (m_ucSnr[2]&0xF0) == 0xC0 )
			uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_TI_Tag_it_HFI_Pro;
		else if( (m_ucSnr[2]&0x70) == 0x00 )
			uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_TI_Tag_it_HFI_Plus;
		else
			uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_TI;
		break;

	case FEDM_ISC_ISO_MFR_FUJITSU:
		if( m_ucSnr[2] == 0x00 )
			uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_Fujitsu_MB89R1xx;
		else if( m_ucSnr[2] == 0x01 )
			uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_Fujitsu_MB89R1xx;
		else if( m_ucSnr[2] == 0x02 )
			uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_Fujitsu_MB89R1xx;
		else
			uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_Fujitsu;
		break;

	case FEDM_ISC_ISO_MFR_EM:
		if( (m_ucSnr[2]&0x7C) == 0x04 )
			uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_EM_4034;
		else
			uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_EM;
		break;

	case FEDM_ISC_ISO_MFR_MAXIM:
		if( (m_ucSnr[3]&0xF0) == 0x10 )
			uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_MAXIM_MAX66100;
		else if( (m_ucSnr[3]&0xF0) == 0x20 )
			uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_MAXIM_MAX66120;
		else if( (m_ucSnr[3]&0xF0) == 0x30 )
			uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_MAXIM_MAX66140;
		else
			uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_MAXIM;
		break;

	case FEDM_ISC_ISO_MFR_KSW:
		uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_KSW;
		break;

	case FEDM_ISC_ISO_MFR_IDS:
		uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693_IDS_SL13A;
		break;

	default:
		uiTagHandlerType = FedmIscTagHandler::TYPE_ISO15693;
		break;
	}

	if(m_pTagHandler != NULL)
	{
		if(m_pTagHandler->m_uiTagHandlerType != uiTagHandlerType)
		{
			delete m_pTagHandler;
			bCreateNew = true;
		}
		else
		{
			m_pTagHandler->Init();
		}
	}
	else
	{
		bCreateNew = true;
	}

	if(bCreateNew)
	{
		if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_STM)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_STM(m_pReader, this);
		}
		else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_STM_LRI2K)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_STM_LRI2K(m_pReader, this);
		}
		else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_STM_LRIS2K)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_STM_LRIS2K(m_pReader, this);
		}
		else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_STM_LRIS64K)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_STM_LRIS64K(m_pReader, this);
		}
		else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_STM_M24LR64R)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_STM_M24LR64R(m_pReader, this);
		}
		else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_STM_M24LRxxER)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_STM_M24LRxxER(m_pReader, this);
		}
		else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_NXP)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_NXP(m_pReader, this);
		}
		else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_NXP_ICODE_SLI)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_NXP_ICODE_SLI(m_pReader, this);
		}
		else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_NXP_ICODE_SLIX)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_NXP_ICODE_SLIX(m_pReader, this);
		}
		else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_NXP_ICODE_SLI_S)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_S(m_pReader, this);
		}
		else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_NXP_ICODE_SLIX_S)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_NXP_ICODE_SLIX_S(m_pReader, this);
		}
		else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_NXP_ICODE_SLI_L)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_L(m_pReader, this);
		}
		else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_NXP_ICODE_SLIX_L)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_NXP_ICODE_SLIX_L(m_pReader, this);
		}
		else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_Infineon)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_Infineon(m_pReader, this);
		}
		else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_Infineon_my_d)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_Infineon_my_d(m_pReader, this);
		}
		else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_TI)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_TI(m_pReader, this);
		}
		else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_TI_Tag_it_HFI_Pro)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_TI_Tag_it_HFI_Pro(m_pReader, this);
		}
		else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_TI_Tag_it_HFI_Plus)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_TI_Tag_it_HFI_Plus(m_pReader, this);
		}
		else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_Fujitsu)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_Fujitsu(m_pReader, this);
		}
		else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_Fujitsu_MB89R1xx)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx(m_pReader, this);
		}
		else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_EM)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_EM(m_pReader, this);
		}
		else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_EM_4034)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_EM_4034(m_pReader, this);
		}
		else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_KSW)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_KSW(m_pReader, this);
		}
		else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_MAXIM)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_Maxim(m_pReader, this);
		}
		else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_MAXIM_MAX66100)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_Maxim_MAX66100(m_pReader, this);
		}
		else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_MAXIM_MAX66120)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_Maxim_MAX66120(m_pReader, this);
		}
		else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_MAXIM_MAX66140)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_Maxim_MAX66140(m_pReader, this);
		}
		else if(uiTagHandlerType == FedmIscTagHandler::TYPE_ISO15693_IDS_SL13A)
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693_IDS_SL13A(m_pReader, this);
		}
		else
		{
			m_pTagHandler = new FedmIscTagHandler_ISO15693(m_pReader, this);
		}
	}

	return FEDM_OK;
}
#endif
#endif


int FEDM_ISOTabItem::SetSnr_wo_Antenna(unsigned char* ucData, int& iByteCnt)
{
	int		iCnt;
	unsigned char ucHeader = 0;
	unsigned char ucEpcLen = 0;

	memset(m_ucSnr,		0,  FEDM_ISC_MAX_UID_LENGTH);

	m_bIsEpc = false;

	m_ucTrType = ucData[iByteCnt++];

	ClearAntennaValues();
	
	if((m_ucTrType & 0x80) == 0)	// HF-Transponder
	{
		switch(m_ucTrType)
		{
		case FEDM_ISC_TR_TYPE_ICODE1:
		case FEDM_ISC_TR_TYPE_TAGIT:
			m_ucSnrLen  = 8;
			m_ucDsfID  = ucData[iByteCnt++];
			for(iCnt=0; iCnt<8; iCnt++)
				m_ucSnr[iCnt] = ucData[iByteCnt++];

			break;

		case FEDM_ISC_TR_TYPE_ISO15693:
		//case FEDM_ISC_TR_TYPE_0x64:	// is an ISO15693
			m_ucSnrLen  = 8;
			m_ucDsfID  = ucData[iByteCnt++];
			for(iCnt=0; iCnt<8; iCnt++)
				m_ucSnr[iCnt] = ucData[iByteCnt++];

			break;

		case FEDM_ISC_TR_TYPE_ISO14443A:
			m_ucTrInfo  = ucData[iByteCnt++];
			m_ucOptInfo  = ucData[iByteCnt++];
			if(m_ucTrInfo & 0x04)
			{
				m_ucSnrLen  = 10;
				for(iCnt=0; iCnt<10; iCnt++)
					m_ucSnr[iCnt] = ucData[iByteCnt++];
			}
			else
			{
				m_ucSnrLen  = 8;
				m_ucSnr[0] = 0;
				for(iCnt=1; iCnt<8; iCnt++)
					m_ucSnr[iCnt] = ucData[iByteCnt++];
			}

			break;

		case FEDM_ISC_TR_TYPE_ISO14443B:
			m_ucSnrLen  = 8;
			m_ucProtoInfo  = ucData[iByteCnt++];
			for(iCnt=0; iCnt<8; iCnt++)
				m_ucSnr[iCnt] = ucData[iByteCnt++];

			break;

		case FEDM_ISC_TR_TYPE_EPC:
			ucHeader = (ucData[iByteCnt] & 0xC0) >> 6;

			switch(ucHeader) // mask first two bits
			{
			case FEDM_ISC_EPC_TYPE_1:	// 96 bit EPC
				m_ucSnrLen  = 12;
				m_ucEpcType = FEDM_ISC_EPC_TYPE_1;
				for(iCnt=0; iCnt<12; iCnt++)
					m_ucSnr[iCnt] = ucData[iByteCnt++];
				m_bIsEpc = true;
				break;

			case FEDM_ISC_EPC_TYPE_2: // 64 bit Type 1 EPC
				m_ucSnrLen  = 8;
				m_ucEpcType = FEDM_ISC_EPC_TYPE_2;
				for(iCnt=0; iCnt<8; iCnt++)
					m_ucSnr[iCnt] = ucData[iByteCnt++];
				m_bIsEpc = true;
				break;

			case FEDM_ISC_EPC_TYPE_3: // 64 bit Type 2 EPC
				m_ucSnrLen  = 8;
				m_ucEpcType = FEDM_ISC_EPC_TYPE_3;
				for(iCnt=0; iCnt<8; iCnt++)
					m_ucSnr[iCnt] = ucData[iByteCnt++];
				m_bIsEpc = true;
				break;

			case FEDM_ISC_EPC_TYPE_4: // 64 bit Type 3 EPC
				m_ucSnrLen  = 8;
				m_ucEpcType = FEDM_ISC_EPC_TYPE_4;
				for(iCnt=0; iCnt<8; iCnt++)
					m_ucSnr[iCnt] = ucData[iByteCnt++];
				m_bIsEpc = true;
				break;
			}

			break;

		case FEDM_ISC_TR_TYPE_ICODE_UID:
			m_ucSnrLen  = 19;
			for(iCnt=0; iCnt<19; iCnt++)
				m_ucSnr[iCnt] = ucData[iByteCnt++];

			break;

		case FEDM_ISC_TR_TYPE_JEWEL:
			m_ucSnrLen  = 8;

			// ignore two bytes without any content (bytes are for future use)
			iByteCnt += 2;

			// copy only 6 bytes, but snr is a 8-byte snr with two leading zero bytes
			memset(m_ucSnr, 0, FEDM_ISC_MAX_UID_LENGTH);
			for(iCnt=0; iCnt<6; iCnt++)
				m_ucSnr[iCnt+2] = ucData[iByteCnt++];

			break;

		case FEDM_ISC_TR_TYPE_ISO18000_3M3:
			m_ucIDDT	= ucData[iByteCnt++];
			m_ucSnrLen  = ucData[iByteCnt++] - 2; // subtract Protocol-Control (PC)

			if(m_ucSnrLen > FEDM_ISC_MAX_UID_LENGTH)
			{
				FEDM_TRACE(m_pReader->m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " - EPC Class1 Gen2 HF: UID > " << FEDM_ISC_MAX_UID_LENGTH);
				return FEDM_ERROR_ARRAY_BOUNDARY;
			}

			m_ucClass1Gen2_PC[0] = ucData[iByteCnt++];
			m_ucClass1Gen2_PC[1] = ucData[iByteCnt++];

			// extended PC W1
			if(m_ucClass1Gen2_PC[0] & 0x02)
			{
				m_ucClass1Gen2_XPC_W1[0] = ucData[iByteCnt++];
				m_ucClass1Gen2_XPC_W1[1] = ucData[iByteCnt++];
				m_ucSnrLen -= 2; // subtract extended Protocol-Control (XPC)
			}

			for(iCnt=0; iCnt<(int)m_ucSnrLen; iCnt++)
				m_ucSnr[iCnt] = ucData[iByteCnt++];

			if(m_ucIDDT == 0x01)
			{
				ucHeader = (m_ucSnr[0] & 0xC0) >> 6;
				switch(ucHeader) // mask first two bits
				{
				case FEDM_ISC_EPC_TYPE_1:	// 96 bit EPC
					m_ucEpcType = FEDM_ISC_EPC_TYPE_1;
					m_bIsEpc = true;
					break;

				case FEDM_ISC_EPC_TYPE_2: // 64 bit Type 1 EPC
					m_ucEpcType = FEDM_ISC_EPC_TYPE_2;
					m_bIsEpc = true;
					break;

				case FEDM_ISC_EPC_TYPE_3: // 64 bit Type 2 EPC
					m_ucEpcType = FEDM_ISC_EPC_TYPE_3;
					m_bIsEpc = true;
					break;

				case FEDM_ISC_EPC_TYPE_4: // 64 bit Type 3 EPC
					m_ucEpcType = FEDM_ISC_EPC_TYPE_4;
					m_bIsEpc = true;
					break;
				}
			}
			else if(m_ucIDDT == 0x02)
			{
				// copy received TID to TID memory
				ucEpcLen = ((m_ucClass1Gen2_PC[0] & 0xF8) >> 3) * 2;	
				if(m_ucClass1Gen2_PC[0] & 0x02) // extended PC W1
					ucEpcLen -= 2;

				if( ucEpcLen < m_ucSnrLen && (m_ucSnrLen - ucEpcLen) >= 4)
				{
					// take TID as UID for next request
					m_ucRxDB_TidBank[1] = m_ucSnr[ucEpcLen];
					m_ucRxDB_TidBank[0] = m_ucSnr[ucEpcLen+1];
					m_ucRxDB_TidBank[3] = m_ucSnr[ucEpcLen+2];
					m_ucRxDB_TidBank[2] = m_ucSnr[ucEpcLen+3];
				}
			}
			break;

		case FEDM_ISC_TR_TYPE_STM_SR176:
			m_ucSnrLen  = 8;
			m_ucChipID  = ucData[iByteCnt++];
			for(iCnt=0; iCnt<8; iCnt++)
				m_ucSnr[iCnt] = ucData[iByteCnt++];

			break;

		case FEDM_ISC_TR_TYPE_STM_SRIxx:
			m_ucSnrLen  = 8;
			m_ucChipID  = ucData[iByteCnt++];
			for(iCnt=0; iCnt<8; iCnt++)
				m_ucSnr[iCnt] = ucData[iByteCnt++];

			break;

		case FEDM_ISC_TR_TYPE_MCRFxxx:
			m_ucSnrLen  = ucData[iByteCnt++];
			for(iCnt=0; iCnt<m_ucSnrLen; iCnt++)
				m_ucSnr[iCnt] = ucData[iByteCnt++];

			break;

		case FEDM_ISC_TR_TYPE_KEYBOARD:
			m_ucSnrLen = ucData[iByteCnt++];
			for(iCnt=0; iCnt<(int)m_ucSnrLen; iCnt++)
				m_ucSnr[iCnt] = ucData[iByteCnt++];

			break;
		
		case FEDM_ISC_TR_TYPE_INNOVATRON:
			m_ucSnrLen  = 8;
			for(iCnt=0; iCnt<8; iCnt++)
				m_ucSnr[iCnt] = ucData[iByteCnt++];
			
			m_ucVerlog	= ucData[iByteCnt++];
			m_ucConfig	= ucData[iByteCnt++];
			m_ucAtrLen	= ucData[iByteCnt++];
			if(m_ucAtrLen <= 33)
			{
				for(iCnt=0; iCnt <(int)m_ucAtrLen; iCnt++)
					m_ucAtr[iCnt] = ucData[iByteCnt++];
			}
			break;

		case FEDM_ISC_TR_TYPE_ASK_CTx:
			m_ucProductCode	= ucData[iByteCnt++];
			m_ucFabCode	= ucData[iByteCnt++];
			m_ucAppCode	= ucData[iByteCnt++];
			m_ucEmbedderCode	= ucData[iByteCnt++];
			m_ucSnrLen  = 8;
			for(iCnt=0; iCnt<8; iCnt++)
				m_ucSnr[iCnt] = ucData[iByteCnt++];

			break;

		case 0x7F: // generic transponder type
			m_ucTrSubType = ucData[iByteCnt++];
			m_ucSnrLen = ucData[iByteCnt++];
			for(iCnt=0; iCnt<(int)m_ucSnrLen; iCnt++)
				m_ucSnr[iCnt] = ucData[iByteCnt++];

			break;
		}
	}
	else if((m_ucTrType & 0x80) == 0x80)	// UHF-Transponder
	{
		switch(m_ucTrType)
		{
		case FEDM_ISC_TR_TYPE_ISO18000_6_A:
		case FEDM_ISC_TR_TYPE_ISO18000_6_B:
		case FEDM_ISC_TR_TYPE_EM4222:
			m_ucIDDT	= ucData[iByteCnt++];
			m_ucSnrLen  = ucData[iByteCnt++];

			if(m_ucSnrLen > FEDM_ISC_MAX_UID_LENGTH)
			{
				FEDM_TRACE(m_pReader->m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " - ISO 18000-6: UID > " << FEDM_ISC_MAX_UID_LENGTH);
				return FEDM_ERROR_ARRAY_BOUNDARY;
			}

			for(iCnt=0; iCnt<(int)m_ucSnrLen; iCnt++)
				m_ucSnr[iCnt] = ucData[iByteCnt++];

			break;

		case FEDM_ISC_TR_TYPE_EPC_CLASS0:
		case FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN1:
			m_ucIDDT	= ucData[iByteCnt++];
			m_ucSnrLen  = ucData[iByteCnt++];

			if(m_ucSnrLen > FEDM_ISC_MAX_UID_LENGTH)
			{
				FEDM_TRACE(m_pReader->m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " - EPC Class0/1: UID > " << FEDM_ISC_MAX_UID_LENGTH);
				return FEDM_ERROR_ARRAY_BOUNDARY;
			}

			for(iCnt=0; iCnt<(int)m_ucSnrLen; iCnt++)
				m_ucSnr[iCnt] = ucData[iByteCnt++];

			if(m_ucIDDT == 0x01)
			{
				ucHeader = (m_ucSnr[0] & 0xC0) >> 6;
				switch(ucHeader) // mask first two bits
				{
				case FEDM_ISC_EPC_TYPE_1:	// 96 bit EPC
					m_ucEpcType = FEDM_ISC_EPC_TYPE_1;
					m_bIsEpc = true;
					break;

				case FEDM_ISC_EPC_TYPE_2: // 64 bit Type 1 EPC
					m_ucEpcType = FEDM_ISC_EPC_TYPE_2;
					m_bIsEpc = true;
					break;

				case FEDM_ISC_EPC_TYPE_3: // 64 bit Type 2 EPC
					m_ucEpcType = FEDM_ISC_EPC_TYPE_3;
					m_bIsEpc = true;
					break;

				case FEDM_ISC_EPC_TYPE_4: // 64 bit Type 3 EPC
					m_ucEpcType = FEDM_ISC_EPC_TYPE_4;
					m_bIsEpc = true;
					break;
				}
			}
			break;

		case FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2:
			m_ucIDDT	= ucData[iByteCnt++];
			m_ucSnrLen  = ucData[iByteCnt++] - 2; // subtract Protocol-Control (PC)

			if(m_ucSnrLen > FEDM_ISC_MAX_UID_LENGTH)
			{
				FEDM_TRACE(m_pReader->m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " - EPC Class1 Gen2 UHF: UID > " << FEDM_ISC_MAX_UID_LENGTH);
				return FEDM_ERROR_ARRAY_BOUNDARY;
			}

			m_ucClass1Gen2_PC[0] = ucData[iByteCnt++];
			m_ucClass1Gen2_PC[1] = ucData[iByteCnt++];

			// extended PC W1
			if(m_ucClass1Gen2_PC[0] & 0x02)
			{
				m_ucClass1Gen2_XPC_W1[0] = ucData[iByteCnt++];
				m_ucClass1Gen2_XPC_W1[1] = ucData[iByteCnt++];
				m_ucSnrLen -= 2; // subtract extended Protocol-Control (XPC)
			}

			for(iCnt=0; iCnt<(int)m_ucSnrLen; iCnt++)
				m_ucSnr[iCnt] = ucData[iByteCnt++];

			if(m_ucIDDT == 0x01)
			{	
				ucHeader = (m_ucSnr[0] & 0xC0) >> 6;
				switch(ucHeader) // mask first two bits
				{
				case FEDM_ISC_EPC_TYPE_1:	// 96 bit EPC
					m_ucEpcType = FEDM_ISC_EPC_TYPE_1;
					m_bIsEpc = true;
					break;

				case FEDM_ISC_EPC_TYPE_2: // 64 bit Type 1 EPC
					m_ucEpcType = FEDM_ISC_EPC_TYPE_2;
					m_bIsEpc = true;
					break;

				case FEDM_ISC_EPC_TYPE_3: // 64 bit Type 2 EPC
					m_ucEpcType = FEDM_ISC_EPC_TYPE_3;
					m_bIsEpc = true;
					break;

				case FEDM_ISC_EPC_TYPE_4: // 64 bit Type 3 EPC
					m_ucEpcType = FEDM_ISC_EPC_TYPE_4;
					m_bIsEpc = true;
					break;
				}
			}
			else if(m_ucIDDT == 0x02)
			{
				// copy received TID to TID memory
				ucEpcLen = ((m_ucClass1Gen2_PC[0] & 0xF8) >> 3) * 2;
				if(m_ucClass1Gen2_PC[0] & 0x02) // extended PC W1
					ucEpcLen -= 2;

				if( ucEpcLen < m_ucSnrLen && (m_ucSnrLen - ucEpcLen) >= 4)
				{
					// take TID as UID for next request
					m_ucRxDB_TidBank[1] = m_ucSnr[ucEpcLen];
					m_ucRxDB_TidBank[0] = m_ucSnr[ucEpcLen+1];
					m_ucRxDB_TidBank[3] = m_ucSnr[ucEpcLen+2];
					m_ucRxDB_TidBank[2] = m_ucSnr[ucEpcLen+3];
				}
			}
			break;

		default:
			m_ucIDDT	= ucData[iByteCnt++];
			m_ucSnrLen  = ucData[iByteCnt++];

			if(m_ucSnrLen > FEDM_ISC_MAX_UID_LENGTH)
			{
				FEDM_TRACE(m_pReader->m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " - Tr-Type=0x" << std::hex << std::uppercase << (int)m_ucTrType << ": UID > " << FEDM_ISC_MAX_UID_LENGTH);
				return FEDM_ERROR_ARRAY_BOUNDARY;
			}

			for(iCnt=0; iCnt<(int)m_ucSnrLen; iCnt++)
				m_ucSnr[iCnt] = ucData[iByteCnt++];

			break;
		}
	}

	return FEDM_OK;
}

int FEDM_ISOTabItem::SetSnr_with_Antenna(unsigned char* ucData, int& iByteCnt)
{
	int		iCnt;
	unsigned char ucHeader = 0;
	unsigned char ucEpcLen = 0;

	memset(m_ucSnr,		0,  FEDM_ISC_MAX_UID_LENGTH);

	m_bIsEpc = false;

	ClearAntennaValues();

	m_ucFlags = ucData[iByteCnt++];

	if(m_ucFlags & 0x01)
	{
		// we can identify the tag type
		m_ucTrType = ucData[iByteCnt++];

		if((m_ucTrType & 0x80) == 0)	// HF-Transponder
		{
			switch(m_ucTrType)
			{
			case FEDM_ISC_TR_TYPE_ICODE1:
			case FEDM_ISC_TR_TYPE_TAGIT:
				m_ucDsfID  = ucData[iByteCnt++];
				m_ucSnrLen  = ucData[iByteCnt++];
				for(iCnt=0; iCnt<m_ucSnrLen; iCnt++)
					m_ucSnr[iCnt] = ucData[iByteCnt++];

				break;
	
			case FEDM_ISC_TR_TYPE_ISO15693:
			//case FEDM_ISC_TR_TYPE_0x64:	// is an ISO15693
				m_ucDsfID  = ucData[iByteCnt++];
				m_ucSnrLen  = ucData[iByteCnt++];
				for(iCnt=0; iCnt<m_ucSnrLen; iCnt++)
					m_ucSnr[iCnt] = ucData[iByteCnt++];

				break;

			case FEDM_ISC_TR_TYPE_EPC:
				m_ucDsfID  = ucData[iByteCnt++];
				m_ucSnrLen  = ucData[iByteCnt++];

				ucHeader = (ucData[iByteCnt] & 0xC0) >> 6;

				switch(ucHeader) // mask first two bits
				{
				case FEDM_ISC_EPC_TYPE_1:	// 96 bit EPC
					m_ucSnrLen  = 12;
					m_ucEpcType = FEDM_ISC_EPC_TYPE_1;
					for(iCnt=0; iCnt<12; iCnt++)
						m_ucSnr[iCnt] = ucData[iByteCnt++];
					m_bIsEpc = true;
					break;

				case FEDM_ISC_EPC_TYPE_2: // 64 bit Type 1 EPC
					m_ucSnrLen  = 8;
					m_ucEpcType = FEDM_ISC_EPC_TYPE_2;
					for(iCnt=0; iCnt<8; iCnt++)
						m_ucSnr[iCnt] = ucData[iByteCnt++];
					m_bIsEpc = true;
					break;

				case FEDM_ISC_EPC_TYPE_3: // 64 bit Type 2 EPC
					m_ucSnrLen  = 8;
					m_ucEpcType = FEDM_ISC_EPC_TYPE_3;
					for(iCnt=0; iCnt<8; iCnt++)
						m_ucSnr[iCnt] = ucData[iByteCnt++];
					m_bIsEpc = true;
					break;

				case FEDM_ISC_EPC_TYPE_4: // 64 bit Type 3 EPC
					m_ucSnrLen  = 8;
					m_ucEpcType = FEDM_ISC_EPC_TYPE_4;
					for(iCnt=0; iCnt<8; iCnt++)
						m_ucSnr[iCnt] = ucData[iByteCnt++];
					m_bIsEpc = true;
					break;
				}

				break;
	
			case FEDM_ISC_TR_TYPE_ICODE_UID:
				m_ucDsfID  = ucData[iByteCnt++];
				m_ucSnrLen  = ucData[iByteCnt++]; // 19
				for(iCnt=0; iCnt<m_ucSnrLen; iCnt++)
					m_ucSnr[iCnt] = ucData[iByteCnt++];

				break;
		
			case FEDM_ISC_TR_TYPE_ISO14443A:
				m_ucTrInfo  = ucData[iByteCnt++];
				m_ucOptInfo  = ucData[iByteCnt++];
				if(m_ucTrInfo & 0x04)
				{
					m_ucSnrLen  = 10;
					for(iCnt=0; iCnt<10; iCnt++)
						m_ucSnr[iCnt] = ucData[iByteCnt++];
				}
				else
				{
					m_ucSnrLen  = 8;
					m_ucSnr[0] = 0;
					for(iCnt=1; iCnt<8; iCnt++)
						m_ucSnr[iCnt] = ucData[iByteCnt++];
				}

				break;

			case FEDM_ISC_TR_TYPE_ISO14443B:
				m_ucSnrLen  = 8;
				m_ucProtoInfo  = ucData[iByteCnt++];
				for(iCnt=0; iCnt<8; iCnt++)
					m_ucSnr[iCnt] = ucData[iByteCnt++];

				break;

//			case FEDM_ISC_TR_TYPE_JEWEL:
//				break;
			
			case FEDM_ISC_TR_TYPE_ISO18000_3M3:
				m_ucIDDT	= ucData[iByteCnt++];
				m_ucSnrLen  = ucData[iByteCnt++] - 2; // subtract Protocol-Control (PC)

				if(m_ucSnrLen > FEDM_ISC_MAX_UID_LENGTH)
				{
					FEDM_TRACE(m_pReader->m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " - EPC Class1 Gen2 HF: UID > " << FEDM_ISC_MAX_UID_LENGTH);
					return FEDM_ERROR_ARRAY_BOUNDARY;
				}

				m_ucClass1Gen2_PC[0] = ucData[iByteCnt++];
				m_ucClass1Gen2_PC[1] = ucData[iByteCnt++];

				// extended PC W1
				if(m_ucClass1Gen2_PC[0] & 0x02)
				{
					m_ucClass1Gen2_XPC_W1[0] = ucData[iByteCnt++];
					m_ucClass1Gen2_XPC_W1[1] = ucData[iByteCnt++];
					m_ucSnrLen -= 2; // subtract extended Protocol-Control (XPC)
				}

				for(iCnt=0; iCnt<(int)m_ucSnrLen; iCnt++)
					m_ucSnr[iCnt] = ucData[iByteCnt++];

				if(m_ucIDDT == 0x01)
				{
					ucHeader = (m_ucSnr[0] & 0xC0) >> 6;
					switch(ucHeader) // mask first two bits
					{
					case FEDM_ISC_EPC_TYPE_1:	// 96 bit EPC
						m_ucEpcType = FEDM_ISC_EPC_TYPE_1;
						m_bIsEpc = true;
						break;

					case FEDM_ISC_EPC_TYPE_2: // 64 bit Type 1 EPC
						m_ucEpcType = FEDM_ISC_EPC_TYPE_2;
						m_bIsEpc = true;
						break;

					case FEDM_ISC_EPC_TYPE_3: // 64 bit Type 2 EPC
						m_ucEpcType = FEDM_ISC_EPC_TYPE_3;
						m_bIsEpc = true;
						break;

					case FEDM_ISC_EPC_TYPE_4: // 64 bit Type 3 EPC
						m_ucEpcType = FEDM_ISC_EPC_TYPE_4;
						m_bIsEpc = true;
						break;
					}
				}
				else if(m_ucIDDT == 0x02)
				{
					// copy received TID to TID memory
					ucEpcLen = ((m_ucClass1Gen2_PC[0] & 0xF8) >> 3) * 2;
					if(m_ucClass1Gen2_PC[0] & 0x02) // extended PC W1
						ucEpcLen -= 2;

					if( ucEpcLen < m_ucSnrLen && (m_ucSnrLen - ucEpcLen) >= 4)
					{
						// take TID as UID for next request
						m_ucRxDB_TidBank[1] = m_ucSnr[ucEpcLen];
						m_ucRxDB_TidBank[0] = m_ucSnr[ucEpcLen+1];
						m_ucRxDB_TidBank[3] = m_ucSnr[ucEpcLen+2];
						m_ucRxDB_TidBank[2] = m_ucSnr[ucEpcLen+3];
					}
				}
				break;
	
//			case FEDM_ISC_TR_TYPE_STM_SR176:
//			case FEDM_ISC_TR_TYPE_STM_SRIxx:
//				break;

			case FEDM_ISC_TR_TYPE_MCRFxxx:
				m_ucIDDT	= ucData[iByteCnt++];
				m_ucSnrLen  = ucData[iByteCnt++];
				for(iCnt=0; iCnt<m_ucSnrLen; iCnt++)
					m_ucSnr[iCnt] = ucData[iByteCnt++];

				break;

//			case FEDM_ISC_TR_TYPE_KEYBOARD:
//				break;
		
			case FEDM_ISC_TR_TYPE_INNOVATRON:
				m_ucVerlog	= ucData[iByteCnt++];
				m_ucConfig	= ucData[iByteCnt++];
				m_ucAtrLen	= ucData[iByteCnt++];
				for(iCnt=0; iCnt <(int)m_ucAtrLen; iCnt++)
					m_ucAtr[iCnt] = ucData[iByteCnt++];

				break;

			case FEDM_ISC_TR_TYPE_ASK_CTx:
				m_ucProductCode	= ucData[iByteCnt++];
				m_ucFabCode	= ucData[iByteCnt++];
				m_ucAppCode	= ucData[iByteCnt++];
				m_ucEmbedderCode	= ucData[iByteCnt++];
				break;

			}
		}
		else if((m_ucTrType & 0x80) == 0x80)	// UHF-Transponder
		{
			switch(m_ucTrType)
			{
			case FEDM_ISC_TR_TYPE_ISO18000_6_A:
			case FEDM_ISC_TR_TYPE_ISO18000_6_B:
			case FEDM_ISC_TR_TYPE_EM4222:
				m_ucIDDT	= ucData[iByteCnt++];
				m_ucSnrLen  = ucData[iByteCnt++];

				if(m_ucSnrLen > FEDM_ISC_MAX_UID_LENGTH)
				{
					FEDM_TRACE(m_pReader->m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " - ISO18000-6: UID > " << FEDM_ISC_MAX_UID_LENGTH);
					return FEDM_ERROR_ARRAY_BOUNDARY;
				}

				for(iCnt=0; iCnt<(int)m_ucSnrLen; iCnt++)
					m_ucSnr[iCnt] = ucData[iByteCnt++];

				break;

			case FEDM_ISC_TR_TYPE_EPC_CLASS0:
			case FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN1:
				m_ucIDDT	= ucData[iByteCnt++];
				m_ucSnrLen  = ucData[iByteCnt++];

				if(m_ucSnrLen > FEDM_ISC_MAX_UID_LENGTH)
				{
					FEDM_TRACE(m_pReader->m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " - EPC Class0/1: UID > " << FEDM_ISC_MAX_UID_LENGTH);
					return FEDM_ERROR_ARRAY_BOUNDARY;
				}

				for(iCnt=0; iCnt<(int)m_ucSnrLen; iCnt++)
					m_ucSnr[iCnt] = ucData[iByteCnt++];

				if(m_ucIDDT == 0x01)
				{
					ucHeader = (m_ucSnr[0] & 0xC0) >> 6;
					switch(ucHeader) // mask first two bits
					{
					case FEDM_ISC_EPC_TYPE_1:	// 96 bit EPC
						m_ucEpcType = FEDM_ISC_EPC_TYPE_1;
						m_bIsEpc = true;
						break;

					case FEDM_ISC_EPC_TYPE_2: // 64 bit Type 1 EPC
						m_ucEpcType = FEDM_ISC_EPC_TYPE_2;
						m_bIsEpc = true;
						break;

					case FEDM_ISC_EPC_TYPE_3: // 64 bit Type 2 EPC
						m_ucEpcType = FEDM_ISC_EPC_TYPE_3;
						m_bIsEpc = true;
						break;

					case FEDM_ISC_EPC_TYPE_4: // 64 bit Type 3 EPC
						m_ucEpcType = FEDM_ISC_EPC_TYPE_4;
						m_bIsEpc = true;
						break;
					}
				}
				break;

			case FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2:
				m_ucIDDT	= ucData[iByteCnt++];
				m_ucSnrLen  = ucData[iByteCnt++] - 2; // subtract Protocol-Control (PC)

				if(m_ucSnrLen > FEDM_ISC_MAX_UID_LENGTH)
				{
					FEDM_TRACE(m_pReader->m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " - EPC Class1 Gen2 UHF: UID > " << FEDM_ISC_MAX_UID_LENGTH);
					return FEDM_ERROR_ARRAY_BOUNDARY;
				}

				m_ucClass1Gen2_PC[0] = ucData[iByteCnt++];
				m_ucClass1Gen2_PC[1] = ucData[iByteCnt++];

				// extended PC W1
				if(m_ucClass1Gen2_PC[0] & 0x02)
				{
					m_ucClass1Gen2_XPC_W1[0] = ucData[iByteCnt++];
					m_ucClass1Gen2_XPC_W1[1] = ucData[iByteCnt++];
					m_ucSnrLen -= 2; // subtract extended Protocol-Control (XPC)
				}

				for(iCnt=0; iCnt<(int)m_ucSnrLen; iCnt++)
					m_ucSnr[iCnt] = ucData[iByteCnt++];

				if(m_ucIDDT == 0x01)
				{	
					ucHeader = (m_ucSnr[0] & 0xC0) >> 6;
					switch(ucHeader) // mask first two bits
					{
					case FEDM_ISC_EPC_TYPE_1:	// 96 bit EPC
						m_ucEpcType = FEDM_ISC_EPC_TYPE_1;
						m_bIsEpc = true;
						break;

					case FEDM_ISC_EPC_TYPE_2: // 64 bit Type 1 EPC
						m_ucEpcType = FEDM_ISC_EPC_TYPE_2;
						m_bIsEpc = true;
						break;

					case FEDM_ISC_EPC_TYPE_3: // 64 bit Type 2 EPC
						m_ucEpcType = FEDM_ISC_EPC_TYPE_3;
						m_bIsEpc = true;
						break;

					case FEDM_ISC_EPC_TYPE_4: // 64 bit Type 3 EPC
						m_ucEpcType = FEDM_ISC_EPC_TYPE_4;
						m_bIsEpc = true;
						break;
					}
				}
				else if(m_ucIDDT == 0x02)
				{
					// copy received TID to TID memory
					ucEpcLen = ((m_ucClass1Gen2_PC[0] & 0xF8) >> 3) * 2;
					if(m_ucClass1Gen2_PC[0] & 0x02) // extended PC W1
						ucEpcLen -= 2;

					if( ucEpcLen < m_ucSnrLen && (m_ucSnrLen - ucEpcLen) >= 4)
					{
						// take TID as UID for next request
						m_ucRxDB_TidBank[1] = m_ucSnr[ucEpcLen];
						m_ucRxDB_TidBank[0] = m_ucSnr[ucEpcLen+1];
						m_ucRxDB_TidBank[3] = m_ucSnr[ucEpcLen+2];
						m_ucRxDB_TidBank[2] = m_ucSnr[ucEpcLen+3];
					}
				}
				break;

			default:
				m_ucIDDT	= ucData[iByteCnt++];
				m_ucSnrLen  = ucData[iByteCnt++];

				if(m_ucSnrLen > FEDM_ISC_MAX_UID_LENGTH)
				{
					FEDM_TRACE(m_pReader->m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " - Tr-Type=0x" << std::hex << std::uppercase << (int)m_ucTrType << ": UID > " << FEDM_ISC_MAX_UID_LENGTH);
					return FEDM_ERROR_ARRAY_BOUNDARY;
				}

				for(iCnt=0; iCnt<(int)m_ucSnrLen; iCnt++)
					m_ucSnr[iCnt] = ucData[iByteCnt++];

				break;
			}
		}

		if(m_ucFlags & 0x10)
		{
			m_ucAntCount  = ucData[iByteCnt++];
			if(m_ucAntCount > FEDM_ISC_ISO_TABLE_MAX_ANTENNA)
			{
				FEDM_TRACE(m_pReader->m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << " - Ant-Count=" << (int)m_ucAntCount << " > " << FEDM_ISC_ISO_TABLE_MAX_ANTENNA);
				return FEDM_ERROR_ARRAY_BOUNDARY;
			}

			for(iCnt=0; iCnt<m_ucAntCount; iCnt++)
			{
				m_ucAntNumber[iCnt] = ucData[iByteCnt++];
				m_ucAntStatus[iCnt] = ucData[iByteCnt++];
				m_ucAntRSSI[iCnt]	= ucData[iByteCnt++];
				iByteCnt += 4;	// 4 byte reserved for future use
			}

			m_bIsRSSI = true;
		}
	} // if(m_ucFlags & 0x01)
	else
	{
		// antenna information for only one antenna
		if(m_ucFlags & 0x10)
		{
			m_ucAntCount  = 1;

			m_ucAntNumber[0] = ucData[iByteCnt++];
			m_ucAntStatus[0] = ucData[iByteCnt++];
			m_ucAntRSSI[0]	= ucData[iByteCnt++];
			iByteCnt += 4;	// 4 byte reserved for future use

			m_bIsRSSI = true;
		}
	}

	return FEDM_OK;
}
