/*-------------------------------------------------------
|                                                       |
|                 FEDM_BRMTabItem.cpp                   |
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
Begin        		:	07.06.2000

Version       		:	04.06.05 / 02.09.2014 / M. Hultsch
						- bugfix for extended PC (XPC) for EPC Class1 Gen2

						04.03.01 / 09.11.2012 / M. Hultsch
						- new method: GetISO15693Manufacturer()
						- support for Extended PC W1 for EPC C1 G2

						04.02.04 / 19.06.2012 / M. Hultsch
						- support for direction information (in combination with Gate People Counter)

						04.00.00 / 21.07.2011 / M. Hultsch
						- max. UID length increased to 96 Bytes

						03.03.04 / 15.06.2011 / M. Hultsch

Operation Systems	:	independent

Function			:	class for buffered read mode of OBID i-scan® and 
						OBID® classic-pro reader family


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#include "../FEDM.h"
#include "FEDM_BRMTabItem.h"
#include "FEDM_ISCReader.h"

#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FEDM_BRMTabItem
//####################################################################

FEDM_BRMTabItem::FEDM_BRMTabItem()
{
	m_iRxDB_BlockCount	= 256;
	m_iRxDB_BlockSize	=   4;

	m_uiTabIndex	= 0;

	for(int i=0; i<4; i++)
		m_ucRxDB_TidBank[i] = 0;
}

FEDM_BRMTabItem::FEDM_BRMTabItem(unsigned int uiTabIndex)
{
	m_iRxDB_BlockCount	= 256;
	m_iRxDB_BlockSize	=   4;

	m_uiTabIndex	= uiTabIndex;
}

FEDM_BRMTabItem::~FEDM_BRMTabItem()
{
	m_ucRxDB.clear();
}

void FEDM_BRMTabItem::Init()
{
	m_iRxDB_BlockCount	= 256;
	m_iRxDB_BlockSize	=   4;

	for(int i=0; i<4; i++)
		m_ucRxDB_TidBank[i] = 0;

	if(m_ucRxDB.size() != FEDM_ISC_BRM_TABLE_RxDB_SIZE)
	{
		m_ucRxDB.reserve(FEDM_ISC_BRM_TABLE_RxDB_SIZE);	// 256 blocks * 4 bytes = 1024 byte
		m_ucRxDB.resize(FEDM_ISC_BRM_TABLE_RxDB_SIZE);	// 256 blocks * 4 bytes = 1024 byte
	}

	memset(m_ucSnr,				0, FEDM_ISC_MAX_UID_LENGTH);
	memset(m_ucClass1Gen2_PC,	0, 2);
	memset(m_ucClass1Gen2_XPC_W1,	0, 2);
	memset(m_ucTimer,			0, 4);	// Hour, Minute, Milliseconds (2 byte)
	memset(m_ucDate,			0, 5);	// Century, Year, Month, Day, Timezone
	memset(m_ucMacAddress,		0, 6);
	memset(&m_ucRxDB[0],		0, FEDM_ISC_BRM_TABLE_RxDB_SIZE);

	m_ucAFI			= 0;
	m_ucDsfID		= 0;
	m_ucDBAdr		= 0;
	m_uiDBN			= 0;
	m_ucTrType		= 0xFF;
	m_ucEpcType		= 0x00;
	m_ucIDDT		= 0;		// only UHF
	m_ucSnrLen		= 8;		// only UHF
	m_ucBlockSize	= 4;		// default size: 4
	m_ucAntNr		= 0;
	m_ucDirection	= 0;

	m_bIsSnr		= false;
	m_bIsEpc		= false;
	m_bIsAFI		= false;
	m_bIsDB			= false;
	m_bIsTimer		= false;
	m_bIsDate		= false;
	m_bIsAntNr		= false;
	m_bIsInput		= false;
	m_bIsRSSI		= false;
	m_bIsMacAddr	= false;
	m_bIsDirection	= false;
	m_ucInput		= 0;			// input states
	m_ucState		= 0;			// status flags, comming with input states

	ClearAntennaValues();
}


void FEDM_BRMTabItem::Init(int iRxDB_BlockCount, int iRxDB_BlockSize)
{
	m_iRxDB_BlockCount	= iRxDB_BlockCount;
	m_iRxDB_BlockSize	= iRxDB_BlockSize;

	for(int i=0; i<4; i++)
		m_ucRxDB_TidBank[i] = 0;

	if(m_ucRxDB.size() != iRxDB_BlockCount * iRxDB_BlockSize)
	{
		m_ucRxDB.reserve(iRxDB_BlockCount * iRxDB_BlockSize);
		m_ucRxDB.resize(iRxDB_BlockCount * iRxDB_BlockSize);
	}

	memset(m_ucSnr,				0, FEDM_ISC_MAX_UID_LENGTH);
	memset(m_ucClass1Gen2_PC,	0, 2);
	memset(m_ucClass1Gen2_XPC_W1,	0, 2);
	memset(m_ucTimer,			0, 4);	// Hour, Minute, Milliseconds (2 byte)
	memset(m_ucDate,			0, 5);	// Century, Year, Month, Day, Timezone
	memset(m_ucMacAddress,		0, 6);
	memset(&m_ucRxDB[0],		0, iRxDB_BlockCount * iRxDB_BlockSize);

	m_ucAFI			= 0;
	m_ucDsfID		= 0;
	m_ucDBAdr		= 0;
	m_uiDBN			= 0;
	m_ucTrType		= 0xFF;
	m_ucEpcType		= 0x00;
	m_ucIDDT		= 0;		// only UHF
	m_ucSnrLen		= 8;		// only UHF
	m_ucBlockSize	= 4;		// default size: 4
	m_ucAntNr		= 0;
	m_ucInput		= 0;			// input states
	m_ucState		= 0;			// status flags, comming with input states
	m_ucDirection	= 0;

	m_bIsSnr		= false;
	m_bIsEpc		= false;
	m_bIsAFI		= false;
	m_bIsDB			= false;
	m_bIsTimer		= false;
	m_bIsDate		= false;
	m_bIsAntNr		= false;
	m_bIsInput		= false;
	m_bIsRSSI		= false;
	m_bIsMacAddr	= false;
	m_bIsDirection	= false;

	ClearAntennaValues();
}

void FEDM_BRMTabItem::GetTableSizes(int& iRxDB_BlockCount, int& iRxDB_BlockSize)
{
	iRxDB_BlockCount	= m_iRxDB_BlockCount;
	iRxDB_BlockSize		= m_iRxDB_BlockSize;
}

void FEDM_BRMTabItem::ClearAntennaValues()
{
	m_ucFlags		= 0;
	m_ucAntCount	= 0;	// number of antenna reads
	memset(&m_ucAntNumber[0],	0, FEDM_ISC_BRM_TABLE_MAX_ANTENNA);	// antenna number
	memset(&m_ucAntRSSI[0],		0, FEDM_ISC_BRM_TABLE_MAX_ANTENNA);	// antenna Received Signal Strength Indication
	memset(&m_ucAntRes1[0],		0, FEDM_ISC_BRM_TABLE_MAX_ANTENNA);	// antenna reserved
}

bool FEDM_BRMTabItem::IsExtendedPC_W1()
{
	if(m_ucClass1Gen2_PC[0] & 0x02)
		return true;
	else
		return false;
}


// get serial number as 64-bit integer
__int64 FEDM_BRMTabItem::GetSnr()
{
	__int64 i64Snr = 0;

	// build serial number and change Motorola format into Intel format
	for ( int i=0; i<8; i++ )
		i64Snr += ( (__int64)m_ucSnr[7-i] << (8*i) );

	return i64Snr;
}

int	FEDM_BRMTabItem::GetEpc(char* cEpc, int iBufLen)
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

int	FEDM_BRMTabItem::GetEpcRaw(unsigned char* cEpc, int iBufLen)
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

int FEDM_BRMTabItem::GetEpcHeader()
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

__int64 FEDM_BRMTabItem::GetEpcDomainManager()
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

__int64 FEDM_BRMTabItem::GetEpcObjectClass()
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

__int64 FEDM_BRMTabItem::GetEpcSnr()
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


const char* FEDM_BRMTabItem::GetISO15693Manufacturer()
{
	if(m_ucTrType == FEDM_ISC_TR_TYPE_ISO15693)
	{
		return FEDM_TabItem::GetISO15693Manufacturer(m_ucSnr[1]);
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

unsigned int FEDM_BRMTabItem::GetEpcC1G2TagModelNumber()
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
//	return (((unsigned int)(m_ucRxDB_TidBank[3] & 0x0F)) << 8) + m_ucRxDB_TidBank[2];
}

unsigned int FEDM_BRMTabItem::GetEpcC1G2MaskDesignerID()
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

const char* FEDM_BRMTabItem::GetEpcC1G2MaskDesignerName()
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


/***************************************************************************
  Version       	:	04.02.04 / 19.06.2012 / M. Hultsch
						- support for direction information (in combination with Gate People Counter)

  Function			:	sets one received data record into BRM-table
						
						!! Attention!!
						When serialnumber is multiple in data stream,
						then the record set is only insert once in the table

  Parameters		:	unsigned char ucCmd		-	used command
						unsigned char* ucData	-	pointer to buffer with data
						unsigned char* ucTrData	-	flag field (see CFG11 in Reader configuration)	
						unsigned int& uiByteCnt	-	reference to byte counter

  Return value		:	OK (=0) or error code (<0)
***************************************************************************/
int FEDM_BRMTabItem::SetData(unsigned char ucCmd, unsigned char* ucData, unsigned char* ucTrData, unsigned int& uiByteCnt)
{
	int iCnt;
	int iErr = 0;
	unsigned int uiRecLen = 0;
	unsigned int uiByteCntBegin = uiByteCnt;
		
	if(ucCmd == 0x22) // advanced buffered read mode
	{
		uiRecLen  = (ucData[uiByteCnt++] << 8);	// MSB of record length;
		uiRecLen +=  ucData[uiByteCnt++];		// LSB of record length;
	}

	if(ucCmd == 0x74) // get input
	{
		if( (ucTrData[0]&0x80) && (ucTrData[1]&0x01) )	// IN flag is set
		{
			m_ucInput = ucData[uiByteCnt++];	
			m_bIsInput = true;
			return FEDM_OK;
		}
	}

	if( (ucTrData[0]&0x01) == 0x01 )	// SNR flag is set
	{
		switch(ucCmd)
		{
		case 0x21:
			FEDM_CHK1(iErr, SetSnr_0x21(ucData, uiByteCnt));
			break;
		case 0x22:
			FEDM_CHK1(iErr, SetSnr_0x22(ucTrData, ucData, uiByteCnt));
			break;
		}
	}

	if( (ucTrData[0]&0x02) == 0x02 )	// DB flag is set
	{
		if(ucCmd == 0x21) // standard buffered read mode
		{
			m_ucDBAdr	= ucData[uiByteCnt++];	// first block address
			m_uiDBN		= ucData[uiByteCnt++];	// number of blocks
		}
		else if(ucCmd == 0x22) // advanced buffered read mode
		{
			m_uiDBN	 = (ucData[uiByteCnt++] << 8);	// MSB of number of blocks
			m_uiDBN	+=  ucData[uiByteCnt++];		// LSB of number of blocks
		}

		if(ucCmd == 0x22) // advanced buffered read mode
			m_ucBlockSize = ucData[uiByteCnt++];	// blocksize in byte

		if( ((int)FEDM_ISC_BRM_TABLE_RxDB_SIZE - (int)(m_ucDBAdr + m_uiDBN)*m_ucBlockSize) < 0)
			return FEDM_ERROR_ARRAY_BOUNDARY;	// array overflow
		
		for(unsigned int uiBlockNr=m_ucDBAdr; uiBlockNr<m_ucDBAdr+m_uiDBN; ++uiBlockNr)
		{
			for(iCnt=0; iCnt<m_ucBlockSize; iCnt++)
				m_ucRxDB[uiBlockNr*m_ucBlockSize+iCnt] = ucData[uiByteCnt++];
		}
		
		m_bIsDB = true;
	}

	if( (ucTrData[0]&0x20) == 0x20 )	// TIME flag is set
	{
		for(iCnt=0; iCnt<4; iCnt++)
			m_ucTimer[iCnt] = ucData[uiByteCnt++];
		
		m_bIsTimer = true;
	}

	if( (ucTrData[0]&0x40) == 0x40 )	// DATE flag is set
	{
		for(iCnt=0; iCnt<5; iCnt++)
			m_ucDate[iCnt] = ucData[uiByteCnt++];
		
		m_bIsDate = true;
	}

	if( (ucTrData[0]&0x10) == 0x10 )	// ANT flag is set
	{
		m_ucAntNr = ucData[uiByteCnt++];
		
		m_bIsAntNr = true;
	}

	if(ucCmd == 0x22) // advanced buffered read mode
	{
		if( (ucTrData[0]&0x80) && (ucTrData[1]&0x01) )	// IN flag is set
		{
			m_ucInput = ucData[uiByteCnt++];
			m_ucState = ucData[uiByteCnt++];
			
			m_bIsInput = true;
		}
	}

	if(ucCmd == 0x22) // MAC-Address
	{
		if( (ucTrData[0]&0x80) && (ucTrData[1]&0x02) )	// MAC flag is set
		{
			memcpy(m_ucMacAddress, &ucData[uiByteCnt], 6);
			uiByteCnt += 6;
			
			m_bIsMacAddr = true;
		}
	}

	if(ucCmd == 0x22) // advanced buffered read mode
	{
		if( (ucTrData[0]&0x80) && (ucTrData[1]&0x10) )	// ANT_EXT flag is set
		{
			m_ucAntCount  = ucData[uiByteCnt++];
			if(m_ucAntCount > FEDM_ISC_BRM_TABLE_MAX_ANTENNA)
				return FEDM_ERROR_ARRAY_BOUNDARY;

			for(iCnt=0; iCnt<m_ucAntCount; iCnt++)
			{
				m_ucAntNumber[iCnt] = ucData[uiByteCnt++];
				m_ucAntRSSI[iCnt]	= ucData[uiByteCnt++];
				m_ucAntRes1[iCnt]	= ucData[uiByteCnt++];
				uiByteCnt += 3;	// 3 byte reserved for future use
			}

			m_bIsRSSI = true;
		}
	}

	if(ucCmd == 0x22) // advanced buffered read mode
	{
		if( (ucTrData[0]&0x80) && (ucTrData[1]&0x20) )	// DIR flag is set
		{
			m_ucDirection = ucData[uiByteCnt++];
			
			m_bIsDirection = true;
		}
	}

	if(ucCmd == 0x22) // advanced buffered read mode
	{
		// for handling future extensions
		if(uiByteCnt > uiByteCntBegin && (uiByteCnt - uiByteCntBegin) < uiRecLen)
			uiByteCnt += uiRecLen - (uiByteCnt - uiByteCntBegin);
	}

	return FEDM_OK;
}

int FEDM_BRMTabItem::SetSnr_0x21(unsigned char* ucData, unsigned int& uiByteCnt)
{
	int		iCnt;
	unsigned char ucHeader = 0;

	memset(m_ucSnr,		0,  FEDM_ISC_MAX_UID_LENGTH);
	m_ucTrType = ucData[uiByteCnt++];

	// only for HF-Transponder
	switch(m_ucTrType)
	{
	case FEDM_ISC_TR_TYPE_ICODE1:
	case FEDM_ISC_TR_TYPE_TAGIT:
	case FEDM_ISC_TR_TYPE_ISO15693:
	case FEDM_ISC_TR_TYPE_ISO14443A:
	case FEDM_ISC_TR_TYPE_ISO14443B:
	//case FEDM_ISC_TR_TYPE_0x64:	// is an ISO15693
	case FEDM_ISC_TR_TYPE_ICODE_UID:
	case FEDM_ISC_TR_TYPE_JEWEL:
		m_ucSnrLen  = 8;
		for(iCnt=0; iCnt<8; iCnt++)
			m_ucSnr[iCnt] = ucData[uiByteCnt++];

		break;

	case FEDM_ISC_TR_TYPE_EPC:
		m_ucIDDT = 0x01;
		m_bIsEpc = true;
		ucHeader = (ucData[uiByteCnt] & 0xC0) >> 6;
		switch(ucHeader) // mask first two bits
		{
		case FEDM_ISC_EPC_TYPE_1:	// 96 bit EPC
			m_ucSnrLen  = 12;
			m_ucEpcType = FEDM_ISC_EPC_TYPE_1;
			for(iCnt=0; iCnt<12; iCnt++)
				m_ucSnr[iCnt] = ucData[uiByteCnt++];
			break;

		case FEDM_ISC_EPC_TYPE_2: // 64 bit Type 1 EPC
			m_ucSnrLen  = 8;
			m_ucEpcType = FEDM_ISC_EPC_TYPE_2;
			for(iCnt=0; iCnt<8; iCnt++)
				m_ucSnr[iCnt] = ucData[uiByteCnt++];
			break;

		case FEDM_ISC_EPC_TYPE_3: // 64 bit Type 2 EPC
			m_ucSnrLen  = 8;
			m_ucEpcType = FEDM_ISC_EPC_TYPE_3;
			for(iCnt=0; iCnt<8; iCnt++)
				m_ucSnr[iCnt] = ucData[uiByteCnt++];
			break;

		case FEDM_ISC_EPC_TYPE_4: // 64 bit Type 3 EPC
			m_ucSnrLen  = 8;
			m_ucEpcType = FEDM_ISC_EPC_TYPE_4;
			for(iCnt=0; iCnt<8; iCnt++)
				m_ucSnr[iCnt] = ucData[uiByteCnt++];
			break;
		}

		break;

	default:
		m_ucSnrLen  = 8;
		for(iCnt=0; iCnt<8; iCnt++)
			m_ucSnr[iCnt] = ucData[uiByteCnt++];

		break;
	}

	m_bIsSnr = true;
	return FEDM_OK;
}

int FEDM_BRMTabItem::SetSnr_0x22(unsigned char* ucTrData, unsigned char* ucData, unsigned int& uiByteCnt)
{
	int		iCnt;
	unsigned char ucHeader = 0;
	unsigned char ucEpcLen = 0;

	memset(m_ucSnr,		0,  FEDM_ISC_MAX_UID_LENGTH);
	m_ucTrType = ucData[uiByteCnt++];

	if(! (m_ucTrType & 0x80))	// HF-Transponder
	{
		switch(m_ucTrType)
		{
		case FEDM_ISC_TR_TYPE_ISO15693:
			m_ucIDDT	= ucData[uiByteCnt++];
			if(ucTrData[1] & 0x04)
				m_ucSnrLen	= ucData[uiByteCnt++] - 2; // AFI and DSFID are in the UID stream
			else
				m_ucSnrLen	= ucData[uiByteCnt++];

			if(m_ucSnrLen > FEDM_ISC_MAX_UID_LENGTH)
				return FEDM_ERROR_ARRAY_BOUNDARY;

			if(ucTrData[1] & 0x04)
			{
				m_ucAFI		= ucData[uiByteCnt++];
				m_ucDsfID	= ucData[uiByteCnt++];
				m_bIsAFI	= true;
			}

			for(iCnt=0; iCnt<(int)m_ucSnrLen; iCnt++)
				m_ucSnr[iCnt] = ucData[uiByteCnt++];

			if(m_ucSnrLen > 0)
				m_bIsSnr = true;

			break;

		case FEDM_ISC_TR_TYPE_ICODE1:
		case FEDM_ISC_TR_TYPE_TAGIT:
		case FEDM_ISC_TR_TYPE_ISO14443A:
		case FEDM_ISC_TR_TYPE_ISO14443B:
		//case FEDM_ISC_TR_TYPE_0x64:	// is an ISO15693
		case FEDM_ISC_TR_TYPE_ICODE_UID:
		case FEDM_ISC_TR_TYPE_JEWEL:
		case FEDM_ISC_TR_TYPE_MCRFxxx:
			m_ucIDDT	= ucData[uiByteCnt++]; // TR-INFO for ISO 14443
			m_ucSnrLen	= ucData[uiByteCnt++];

			if(m_ucSnrLen > FEDM_ISC_MAX_UID_LENGTH)
				return FEDM_ERROR_ARRAY_BOUNDARY;

			for(iCnt=0; iCnt<(int)m_ucSnrLen; iCnt++)
				m_ucSnr[iCnt] = ucData[uiByteCnt++];

			if(m_ucSnrLen > 0)
				m_bIsSnr = true;

			break;

		case FEDM_ISC_TR_TYPE_EPC:
			m_ucIDDT	= ucData[uiByteCnt++];
			m_ucSnrLen	= ucData[uiByteCnt++];

			if(m_ucSnrLen > FEDM_ISC_MAX_UID_LENGTH)
				return FEDM_ERROR_ARRAY_BOUNDARY;
			
			for(iCnt=0; iCnt<(int)m_ucSnrLen; iCnt++)
				m_ucSnr[iCnt] = ucData[uiByteCnt++];

			m_bIsEpc = true;
			ucHeader = (m_ucSnr[0] & 0xC0) >> 6;
			switch(ucHeader) // mask first two bits
			{
			case FEDM_ISC_EPC_TYPE_1:	// 96 bit EPC
				m_ucEpcType = FEDM_ISC_EPC_TYPE_1;
				break;

			case FEDM_ISC_EPC_TYPE_2: // 64 bit Type 1 EPC
				m_ucEpcType = FEDM_ISC_EPC_TYPE_2;
				break;

			case FEDM_ISC_EPC_TYPE_3: // 64 bit Type 2 EPC
				m_ucEpcType = FEDM_ISC_EPC_TYPE_3;
				break;

			case FEDM_ISC_EPC_TYPE_4: // 64 bit Type 3 EPC
				m_ucEpcType = FEDM_ISC_EPC_TYPE_4;
				break;
			}

			if(m_ucSnrLen > 0)
				m_bIsSnr = true;

			break;

		case FEDM_ISC_TR_TYPE_ISO18000_3M3:
			m_ucIDDT	= ucData[uiByteCnt++];
			m_ucSnrLen	= ucData[uiByteCnt++] - 2;	// sub 2 bytes for PC

			if(m_ucSnrLen > FEDM_ISC_MAX_UID_LENGTH)
				return FEDM_ERROR_ARRAY_BOUNDARY;

			m_ucClass1Gen2_PC[0] = ucData[uiByteCnt++];
			m_ucClass1Gen2_PC[1] = ucData[uiByteCnt++];

			// extended PC W1
			if(m_ucClass1Gen2_PC[0] & 0x02)
			{
				m_ucClass1Gen2_XPC_W1[0] = ucData[uiByteCnt++];
				m_ucClass1Gen2_XPC_W1[1] = ucData[uiByteCnt++];
				m_ucSnrLen -= 2; // subtract extended Protocol-Control (XPC)
			}

			for(iCnt=0; iCnt<(int)m_ucSnrLen; iCnt++)
				m_ucSnr[iCnt] = ucData[uiByteCnt++];

			if(m_ucSnrLen > 0)
				m_bIsSnr = true;

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
				if( ucEpcLen < m_ucSnrLen && (m_ucSnrLen - ucEpcLen) >= 4)
				{
					// save TID from UID
					m_ucRxDB_TidBank[1] = m_ucSnr[ucEpcLen];
					m_ucRxDB_TidBank[0] = m_ucSnr[ucEpcLen+1];
					m_ucRxDB_TidBank[3] = m_ucSnr[ucEpcLen+2];
					m_ucRxDB_TidBank[2] = m_ucSnr[ucEpcLen+3];
				}
			}
			break;

		case FEDM_ISC_TR_TYPE_KEYBOARD:
			m_ucSnrLen = ucData[uiByteCnt++];

			if(m_ucSnrLen > FEDM_ISC_MAX_UID_LENGTH)
				return FEDM_ERROR_ARRAY_BOUNDARY;

			for(iCnt=0; iCnt<(int)m_ucSnrLen; iCnt++)
				m_ucSnr[iCnt] = ucData[uiByteCnt++];

			if(m_ucSnrLen > 0)
				m_bIsSnr = true;

			break;

		default:
			m_ucIDDT	= ucData[uiByteCnt++];
			m_ucSnrLen	= ucData[uiByteCnt++];

			if(m_ucSnrLen > FEDM_ISC_MAX_UID_LENGTH)
				return FEDM_ERROR_ARRAY_BOUNDARY;

			for(iCnt=0; iCnt<(int)m_ucSnrLen; iCnt++)
				m_ucSnr[iCnt] = ucData[uiByteCnt++];

			if(m_ucSnrLen > 0)
				m_bIsSnr = true;

			break;
		}
	}
	else if(m_ucTrType & 0x80)	// UHF-Transponder
	{
		switch(m_ucTrType)
		{
		case FEDM_ISC_TR_TYPE_ISO18000_6_A:
		case FEDM_ISC_TR_TYPE_ISO18000_6_B:
		case FEDM_ISC_TR_TYPE_EM4222:
			m_ucIDDT	= ucData[uiByteCnt++];
			m_ucSnrLen	= ucData[uiByteCnt++];

			if(m_ucSnrLen > FEDM_ISC_MAX_UID_LENGTH)
				return FEDM_ERROR_ARRAY_BOUNDARY;

			for(iCnt=0; iCnt<(int)m_ucSnrLen; iCnt++)
				m_ucSnr[iCnt] = ucData[uiByteCnt++];

			if(m_ucSnrLen > 0)
				m_bIsSnr = true;

			break;

		case FEDM_ISC_TR_TYPE_EPC_CLASS0:
		case FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN1:
			m_ucIDDT	= ucData[uiByteCnt++];
			m_ucSnrLen	= ucData[uiByteCnt++];

			if(m_ucSnrLen > FEDM_ISC_MAX_UID_LENGTH)
				return FEDM_ERROR_ARRAY_BOUNDARY;

			for(iCnt=0; iCnt<(int)m_ucSnrLen; iCnt++)
				m_ucSnr[iCnt] = ucData[uiByteCnt++];

			if(m_ucSnrLen > 0)
				m_bIsSnr = true;

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
			m_ucIDDT	= ucData[uiByteCnt++];
			m_ucSnrLen	= ucData[uiByteCnt++] - 2;	// sub 2 bytes for PC

			if(m_ucSnrLen > FEDM_ISC_MAX_UID_LENGTH)
				return FEDM_ERROR_ARRAY_BOUNDARY;

			m_ucClass1Gen2_PC[0] = ucData[uiByteCnt++];
			m_ucClass1Gen2_PC[1] = ucData[uiByteCnt++];

			// extended PC W1
			if(m_ucClass1Gen2_PC[0] & 0x02)
			{
				m_ucClass1Gen2_XPC_W1[0] = ucData[uiByteCnt++];
				m_ucClass1Gen2_XPC_W1[1] = ucData[uiByteCnt++];
				m_ucSnrLen -= 2; // subtract extended Protocol-Control (XPC)
			}

			for(iCnt=0; iCnt<(int)m_ucSnrLen; iCnt++)
				m_ucSnr[iCnt] = ucData[uiByteCnt++];

			if(m_ucSnrLen > 0)
				m_bIsSnr = true;


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
				if( ucEpcLen < m_ucSnrLen && (m_ucSnrLen - ucEpcLen) >= 4)
				{
					// save TID from UID
					m_ucRxDB_TidBank[1] = m_ucSnr[ucEpcLen];
					m_ucRxDB_TidBank[0] = m_ucSnr[ucEpcLen+1];
					m_ucRxDB_TidBank[3] = m_ucSnr[ucEpcLen+2];
					m_ucRxDB_TidBank[2] = m_ucSnr[ucEpcLen+3];
				}
			}
			break;
		}
	}

	return FEDM_OK;
}
