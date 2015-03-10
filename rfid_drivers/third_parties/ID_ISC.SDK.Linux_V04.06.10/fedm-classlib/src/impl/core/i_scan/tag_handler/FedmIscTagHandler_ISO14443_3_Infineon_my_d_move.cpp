/*-------------------------------------------------------
|                                                       |
| FedmIscTagHandler_ISO14443_3_Infineon_my_d_move.cpp   |
|                                                       |
---------------------------------------------------------

Copyright © 2009-2010	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Manuel Sahm
Begin        		:	17.12.2009

Version       		:	03.01.00 / 10.02.2010 / M. Sahm, M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID® classic-pro transponder ISO14443-3 Infineon my-d_move

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "FedmIscTagHandler_ISO14443_3_Infineon_my_d_move.h"
#include "../../FEDM_Functions.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"
#include "feisc.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscTagHandler_ISO14443_3_Infineon_my_d_move
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_ISO14443_3_Infineon_my_d_move::FedmIscTagHandler_ISO14443_3_Infineon_my_d_move(
	FEDM_ISCReader* pReader, 
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_3(pReader, FedmIscTagHandler::TYPE_ISO14443_3_INFINEON_MY_D_MOVE, pTabItem)
{
	m_sTagName = "ISO 14443-3 Type A : Infineon my-d move";
}

FedmIscTagHandler_ISO14443_3_Infineon_my_d_move::FedmIscTagHandler_ISO14443_3_Infineon_my_d_move(
	FEDM_ISCReader* pReader, 
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_3(pReader, uiTagHandlerType, pTabItem)
{
	m_sTagName = "ISO 14443-3 Type A : Infineon my-d move";
}

FedmIscTagHandler_ISO14443_3_Infineon_my_d_move::~FedmIscTagHandler_ISO14443_3_Infineon_my_d_move()
{
}

// set all data members to 0
/*void FedmIscTagHandler_ISO14443_3_Infineon_my_d_move::Init()
{
}*/


/***************************************************************************
  Begin        		:	17.12.2009 / M. Sahm

  Version       	:	03.01.00 / 20.01.2010 / M. Sahm, M. Hultsch

  Function			:	[0x30][ADDR][CRC16]: Read 4 Blocks from my-d_move

						NOTE: only for selected tags

  Parameters		:	unsigned char Address					- [in]	address: 0x00...0x25
						unsigned char* pucData					- [out]	buffer with data
						unsigned int uiDataBufferLen			- [in]	Size of the DataBuffer 

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_3_Infineon_my_d_move::Read4Blocks(unsigned char ucAddress, unsigned char* pucData, unsigned int uiDataBufferLen)
{
	FEDM_CHK3(pucData);

	//Check buffer size
	if(uiDataBufferLen < 16)
	{
		return FEDM_ERROR_ARRAY_BOUNDARY;
	}

	if(ucAddress < 0 || ucAddress > 0x25)
	{
		return FEDM_ERROR_PARAMETER;	
	}

	int iErr = 0;
	unsigned char ucTransBuf[8];
	unsigned char ucRespBuf[64];
	int iRespLen = 0;

	// Build Transmit Data
	ucTransBuf[0] = 0x7F;			// Timoeut = 127ms

	switch(m_pReader->GetReaderType())
	{
		case FEDM_ISC_TYPE_CPR30_XX:
		case FEDM_ISC_TYPE_CPR40_XX_U:
		case FEDM_ISC_TYPE_CPR40_XX:
		case FEDM_ISC_TYPE_CPR44_XX:
			ucTransBuf[1] = 0x05;			// CRC-Info	   = TxCRC Enabled, RxCRC Disabled and Parity Enabled
		break;

		case FEDM_ISC_TYPE_CPR50_XX:
		case FEDM_ISC_TYPE_CPR52_XX:
			ucTransBuf[1] = 0x07;			// CRC-Info	   = TxCRC Enabled, RxCRC Disabled and Parity Odd enabled und Parity Enabled
		break;

		default:
			return FEDM_ERROR_UNKNOWN_READER_TYPE;
	}
	
	ucTransBuf[2] = 0x00;		// TX-Settings = 106kBit/s
	ucTransBuf[3] = 0x00;		// RX-Settings = 106kBit/s
	ucTransBuf[4] = 0x00;		// ReqBits[0]
	ucTransBuf[5] = 0x10;		// ReqBits[1]  = 16 Bits : 0x30 ADDR
	ucTransBuf[6] = 0x30;		// ReqData[0]
	ucTransBuf[7] = ucAddress;	// ReqData[1]

	
	FEDM_CHK1	(iErr, FEISC_0xBD_ISOTranspCmd(	m_pReader->GetReaderHnd(), 
												m_pReader->GetBusAddress(), 
												0x03,										// Mode 3
												144,										// expected 16 Byte Data + 2 Bytes CRC16
												ucTransBuf,									// ucTransBuf
												8,											// DataToSend
												ucRespBuf,									// ucRespBuf
												&iRespLen,									// Length of response in Bytes
												0)											// hex array
				);

	if(iErr == 0)
	{
		//OK - Analyze the response Protocol
		unsigned int uiRespBits = ((unsigned int)ucRespBuf[0] << 8) | (unsigned int)ucRespBuf[1];
		if((uiRespBits == 4) && ((ucRespBuf[2] == 0x00) || (ucRespBuf[2] == 0x01)))
		{
			//STATUS: NACK0(0x0), or NACK1(0x1)
			return 0x01; //No Transponder in Reader-Field
		}
		else if(uiRespBits == 144)
		{
			if(CalcCRC16Csum(&ucRespBuf[2], 18) == 0)
			{
				memcpy(pucData, &ucRespBuf[2], 16);
				return FEDM_OK;
			}
			else
			{
				//CRC-Error
				return 0x01; //No Transponder in Reader-Field				
			}
		}
		else
		{
			//Error
			return 0x01; //No Transponder in Reader-Field
		}
	}
	else
	{
		//Status Byte > 0
		return iErr;
	}
}

/***************************************************************************
  Begin        		:	17.12.2009 / M. Sahm

  Version       	:	03.01.00 / 17.12.2009 / M. Sahm

  Function			:	[0xA2][ADDR][4 byte Data][CRC16]: Write 1 Block to my-d_move

						NOTE: only for selected tags

  Parameters		:	unsigned char Address					- [in]	address: 0x02...0x24
						unsigned char* pucData					- [in]	buffer with data
						unsigned int uiDataBufferLen			- [in]	Length of the Data in the Buffer 

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_3_Infineon_my_d_move::Write1Block(unsigned char ucAddress, unsigned char* pucData, unsigned int uiDataBufferLen)
{
	FEDM_CHK3(pucData);


	//Check buffer size
	if(uiDataBufferLen < 4)
	{
		return FEDM_ERROR_ARRAY_BOUNDARY;
	}

	if(ucAddress < 0x02 || ucAddress > 0x24)
	{
		return FEDM_ERROR_PARAMETER;	
	}

	int iErr = 0;
	unsigned char ucTransBuf[12];
	unsigned char ucRespBuf[64];
	int iRespLen = 0;

	// Build Transmit Data
	ucTransBuf[0] = 0x7F;			// Timoeut = 127ms

	switch(m_pReader->GetReaderType())
	{
		case FEDM_ISC_TYPE_CPR30_XX:
		case FEDM_ISC_TYPE_CPR40_XX_U:
		case FEDM_ISC_TYPE_CPR40_XX:
		case FEDM_ISC_TYPE_CPR44_XX:
			ucTransBuf[1] = 0x05;			// CRC-Info	   = TxCRC Enabled, RxCRC disabled and Parity Enabled
		break;

		case FEDM_ISC_TYPE_CPR50_XX:
		case FEDM_ISC_TYPE_CPR52_XX:
			ucTransBuf[1] = 0x07;			// CRC-Info	   = TxCRC Enabled, RxCRC disabled and Parity Odd enabled und Parity Enabled
		break;

		default:
			return FEDM_ERROR_UNKNOWN_READER_TYPE;
	}
	
	ucTransBuf[2] = 0x00;			// TX-Settings = 106kBit/s
	ucTransBuf[3] = 0x00;			// RX-Settings = 106kBit/s
	ucTransBuf[4] = 0x00;			// ReqBits[0]
	ucTransBuf[5] = 0x30;			// ReqBits[1]  = 48 Bits : 0xA2 ADDR Data0 Data1 Data2 Data3
	ucTransBuf[6] = 0xA2;			// ReqData[0]
	ucTransBuf[7] = ucAddress;		// ReqData[1]
	memcpy(&ucTransBuf[8], pucData, 4);	// ReqData[2]...[5]

	
	FEDM_CHK1	(iErr, FEISC_0xBD_ISOTranspCmd(	m_pReader->GetReaderHnd(), 
												m_pReader->GetBusAddress(), 
												0x03,										// Mode 3
												4,											// expected 4 Bit Data
												ucTransBuf,									// ucTransBuf
												12,											// DataToSend
												ucRespBuf,									// ucRespBuf
												&iRespLen,									// Length of response in Bytes
												0)											// hex array
				);

	if(iErr == 0)
	{
		//OK - Analyze the response Protocol
		unsigned int uiRespBits = ((unsigned int)ucRespBuf[0] << 8) | (unsigned int)ucRespBuf[1];
		if((uiRespBits == 4) && ((ucRespBuf[2] == 0x00) || (ucRespBuf[2] == 0x01)))
		{
			//STATUS: NACK0(0x0), or NACK1(0x1)
			return 0x01; //No Transponder in Reader-Field
		}
		else if((uiRespBits == 4) && (ucRespBuf[2] == 0x0A))
		{
			//STATUS: ACK0(0xA)
			return FEDM_OK;
		}
		else
		{
			//Error
			return 0x01; //No Transponder in Reader-Field
		}
	}
	else
	{
		//Status Byte > 0
		return iErr;
	}
}

/***************************************************************************
  Begin        		:	17.12.2009 / M. Sahm

  Version       	:	03.01.00 / 17.12.2009 / M. Sahm

  Function			:	[0xA0][ADDR][16 Byte data][CRC16]: Compatibility Write Command from my-d_move

						NOTE: only for selected tags

  Parameters		:	unsigned char Address					- [in]	address: 0x02...0x24
						unsigned char* pucData					- [in]	buffer with data
						unsigned int uiDataBufferLen			- [in]	Length of the Data in the Buffer 

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_3_Infineon_my_d_move::CompatibilityWriteCmd(unsigned char ucAddress, unsigned char* pucData, unsigned int uiDataBufferLen)
{
	FEDM_CHK3(pucData);

	//Check buffer size
	if(uiDataBufferLen < 16)
	{
		return FEDM_ERROR_ARRAY_BOUNDARY;
	}

	if(ucAddress < 0x02 || ucAddress > 0x24)
	{
		return FEDM_ERROR_PARAMETER;	
	}

	int iErr = 0;
	unsigned char ucTransBuf[22];
	unsigned char ucRespBuf[64];
	int iRespLen = 0;

	//------------------//
	// ----- STEP 1 --- //
	//------------------//

	// Build Transmit Data
	ucTransBuf[0] = 0x7F;			// Timoeut = 127ms

	switch(m_pReader->GetReaderType())
	{
		case FEDM_ISC_TYPE_CPR30_XX:
		case FEDM_ISC_TYPE_CPR40_XX_U:
		case FEDM_ISC_TYPE_CPR40_XX:
		case FEDM_ISC_TYPE_CPR44_XX:
			ucTransBuf[1] = 0x05;			// CRC-Info	   = TxCRC Enabled, RxCRC disabled and Parity Enabled
		break;

		case FEDM_ISC_TYPE_CPR50_XX:
		case FEDM_ISC_TYPE_CPR52_XX:
			ucTransBuf[1] = 0x07;			// CRC-Info	   = TxCRC Enabled, RxCRC disabled and Parity Odd enabled und Parity Enabled
		break;

		default:
			return FEDM_ERROR_UNKNOWN_READER_TYPE;
	}
	
	ucTransBuf[2] = 0x00;			// TX-Settings = 106kBit/s
	ucTransBuf[3] = 0x00;			// RX-Settings = 106kBit/s
	ucTransBuf[4] = 0x00;			// ReqBits[0]
	ucTransBuf[5] = 0x10;			// ReqBits[1]  = 16 Bits : 0xA0 ADDR
	ucTransBuf[6] = 0xA0;			// ReqData[0]
	ucTransBuf[7] = ucAddress;		// ReqData[1]

	
	FEDM_CHK1	(iErr, FEISC_0xBD_ISOTranspCmd(	m_pReader->GetReaderHnd(), 
												m_pReader->GetBusAddress(), 
												0x03,										// Mode 3
												4,											// expected 4 Bit Data
												ucTransBuf,									// ucTransBuf
												8,											// DataToSend
												ucRespBuf,									// ucRespBuf
												&iRespLen,									// Length of response in Bytes
												0)											// hex array
				);

	if(iErr == 0)
	{
		//OK - Analyze the response Protocol
		unsigned int uiRespBits = ((unsigned int)ucRespBuf[0] << 8) | (unsigned int)ucRespBuf[1];
		if((uiRespBits == 4) && ((ucRespBuf[2] == 0x00) || (ucRespBuf[2] == 0x01)))
		{
			//STATUS: NACK0(0x0), or NACK1(0x1)
			return 0x01; //No Transponder in Reader-Field
		}
		else if((uiRespBits == 4) && (ucRespBuf[2] == 0x0A))
		{
			//STATUS: ACK0(0xA)
		}
		else
		{
			//Error
			return 0x01; //No Transponder in Reader-Field
		}
	}
	else
	{
		//Status Byte > 0
		return iErr;
	}


	//------------------//
	// ----- STEP 2 --- //
	//------------------//
	iRespLen = 0;

	// Build Transmit Data
	ucTransBuf[0] = 0x7F;			// Timoeut = 127ms

	switch(m_pReader->GetReaderType())
	{
		case FEDM_ISC_TYPE_CPR30_XX:
		case FEDM_ISC_TYPE_CPR40_XX_U:
		case FEDM_ISC_TYPE_CPR40_XX:
		case FEDM_ISC_TYPE_CPR44_XX:
			ucTransBuf[1] = 0x05;			// CRC-Info	   = TxCRC Enabled, RxCRC disabled and Parity Enabled
		break;

		case FEDM_ISC_TYPE_CPR50_XX:
		case FEDM_ISC_TYPE_CPR52_XX:
			ucTransBuf[1] = 0x07;			// CRC-Info	   = TxCRC Enabled, RxCRC disabled and Parity Odd enabled und Parity Enabled
		break;

		default:
			return FEDM_ERROR_UNKNOWN_READER_TYPE;
	}
	
	ucTransBuf[2] = 0x00;			// TX-Settings = 106kBit/s
	ucTransBuf[3] = 0x00;			// RX-Settings = 106kBit/s
	ucTransBuf[4] = 0x00;			// ReqBits[0]
	ucTransBuf[5] = 0x80;			// ReqBits[1]  = 128 Bits : Data0 ... Data15
	memcpy(&ucTransBuf[6], pucData, 16);
	
	FEDM_CHK1	(iErr, FEISC_0xBD_ISOTranspCmd(	m_pReader->GetReaderHnd(), 
												m_pReader->GetBusAddress(), 
												0x03,										// Mode 3
												4,											// expected 4 Bit Data
												ucTransBuf,									// ucTransBuf
												22,											// DataToSend
												ucRespBuf,									// ucRespBuf
												&iRespLen,									// Length of response in Bytes
												0)											// hex array
				);

	if(iErr == 0)
	{
		//OK - Analyze the response Protocol
		unsigned int uiRespBits = ((unsigned int)ucRespBuf[0] << 8) | (unsigned int)ucRespBuf[1];
		if((uiRespBits == 4) && ((ucRespBuf[2] == 0x00) || (ucRespBuf[2] == 0x01)))
		{
			//STATUS: NACK0(0x0), or NACK1(0x1)
			return 0x01; //No Transponder in Reader-Field
		}
		else if((uiRespBits == 4) && (ucRespBuf[2] == 0x0A))
		{
			//STATUS: ACK0(0xA)
			return FEDM_OK;
		}
		else
		{
			//Error
			return 0x01; //No Transponder in Reader-Field
		}
	}
	else
	{
		//Status Byte > 0
		return iErr;
	}
}

/***************************************************************************
  Begin        		:	17.12.2009 / M. Sahm

  Version       	:	03.01.00 / 17.12.2009 / M. Sahm

  Function			:	[0x31][ADDR][CRC16]: Read 2 Blocks from my-d_move

						NOTE: only for selected tags

  Parameters		:	unsigned char Address					- [in]	address: 0x00...0x25
						unsigned char* pucData					- [out]	buffer with data
						unsigned int uiDataBufferLen			- [in]	Size of the DataBuffer 

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_3_Infineon_my_d_move::Read2Blocks(unsigned char ucAddress, unsigned char* pucData, unsigned int uiDataBufferLen)
{
	FEDM_CHK3(pucData);

	//Check buffer size
	if(uiDataBufferLen < 8)
	{
		return FEDM_ERROR_ARRAY_BOUNDARY;
	}

	if(ucAddress < 0 || ucAddress > 0x25)
	{
		return FEDM_ERROR_PARAMETER;	
	}

	int iErr = 0;
	unsigned char ucTransBuf[8];
	unsigned char ucRespBuf[64];
	int iRespLen = 0;

	// Build Transmit Data
	ucTransBuf[0] = 0x7F;			// Timoeut = 127ms

	switch(m_pReader->GetReaderType())
	{
		case FEDM_ISC_TYPE_CPR30_XX:
		case FEDM_ISC_TYPE_CPR40_XX_U:
		case FEDM_ISC_TYPE_CPR40_XX:
		case FEDM_ISC_TYPE_CPR44_XX:
			ucTransBuf[1] = 0x05;			// CRC-Info	   = TxCRC Enabled, RxCRC Disabled and Parity Enabled
		break;

		case FEDM_ISC_TYPE_CPR50_XX:
		case FEDM_ISC_TYPE_CPR52_XX:
			ucTransBuf[1] = 0x07;			// CRC-Info	   = TxCRC Enabled, RxCRC Disabled and Parity Odd enabled und Parity Enabled
		break;

		default:
			return FEDM_ERROR_UNKNOWN_READER_TYPE;
	}
	
	ucTransBuf[2] = 0x00;			// TX-Settings = 106kBit/s
	ucTransBuf[3] = 0x00;			// RX-Settings = 106kBit/s
	ucTransBuf[4] = 0x00;			// ReqBits[0]
	ucTransBuf[5] = 0x10;			// ReqBits[1]  = 16 Bits : 0x30 ADDR
	ucTransBuf[6] = 0x31;			// ReqData[0]
	ucTransBuf[7] = ucAddress;		// ReqData[1]

	
	FEDM_CHK1	(iErr, FEISC_0xBD_ISOTranspCmd(	m_pReader->GetReaderHnd(), 
												m_pReader->GetBusAddress(), 
												0x03,										// Mode 3
												80,										// expected 8 Byte Data + 2 Bytes CRC16
												ucTransBuf,									// ucTransBuf
												8,											// DataToSend
												ucRespBuf,									// ucRespBuf
												&iRespLen,									// Length of response in Bytes
												0)											// hex array
				);

	if(iErr == 0)
	{
		//OK - Analyze the response Protocol
		unsigned int uiRespBits = ((unsigned int)ucRespBuf[0] << 8) | (unsigned int)ucRespBuf[1];
		if((uiRespBits == 4) && ((ucRespBuf[2] == 0x00) || (ucRespBuf[2] == 0x01)))
		{
			//STATUS: NACK0(0x0), or NACK1(0x1)
			return 0x01; //No Transponder in Reader-Field
		}
		else if(uiRespBits == 80)
		{
			if(CalcCRC16Csum(&ucRespBuf[2], 10) == 0)
			{
				memcpy(pucData, &ucRespBuf[2], 8);
				return FEDM_OK;
			}
			else
			{
				//CRC-Error
				return 0x01; //No Transponder in Reader-Field				
			}
		}
		else
		{
			//Error
			return 0x01; //No Transponder in Reader-Field
		}
	}
	else
	{
		//Status Byte > 0
		return iErr;
	}
}

/***************************************************************************
  Begin        		:	17.12.2009 / M. Sahm

  Version       	:	03.01.00 / 17.12.2009 / M. Sahm

  Function			:	[0xA1][ADDR][8 Byte Data][CRC16]: Write 2 Blocks to my-d_move

						NOTE: only for selected tags

  Parameters		:	unsigned char Address					- [in]	address: 0x00...0x25
						unsigned char* pucData					- [in]	buffer with data
						unsigned int uiDataBufferLen			- [in]	Length of the Data in the Buffer 

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_3_Infineon_my_d_move::Write2Blocks(unsigned char ucAddress, unsigned char* pucData, unsigned int uiDataBufferLen)
{
	FEDM_CHK3(pucData);

	//Check buffer size
	if(uiDataBufferLen < 4)
	{
		return FEDM_ERROR_ARRAY_BOUNDARY;
	}

	if(ucAddress < 0x04 || ucAddress > 0x22)
	{
		return FEDM_ERROR_PARAMETER;	
	}

	//Only even start addresses allowed
	if(ucAddress % 2)
	{
		return FEDM_ERROR_PARAMETER;
	}

	int iErr = 0;
	unsigned char ucTransBuf[16];
	unsigned char ucRespBuf[64];
	int iRespLen = 0;

	// Build Transmit Data
	ucTransBuf[0] = 0x7F;			// Timoeut = 127ms

	switch(m_pReader->GetReaderType())
	{
		case FEDM_ISC_TYPE_CPR30_XX:
		case FEDM_ISC_TYPE_CPR40_XX_U:
		case FEDM_ISC_TYPE_CPR40_XX:
		case FEDM_ISC_TYPE_CPR44_XX:
			ucTransBuf[1] = 0x05;			// CRC-Info	   = TxCRC Enabled, RxCRC disabled and Parity Enabled
		break;

		case FEDM_ISC_TYPE_CPR50_XX:
		case FEDM_ISC_TYPE_CPR52_XX:
			ucTransBuf[1] = 0x07;			// CRC-Info	   = TxCRC Enabled, RxCRC disabled and Parity Odd enabled und Parity Enabled
		break;

		default:
			return FEDM_ERROR_UNKNOWN_READER_TYPE;
	}
	
	ucTransBuf[2] = 0x00;			// TX-Settings = 106kBit/s
	ucTransBuf[3] = 0x00;			// RX-Settings = 106kBit/s
	ucTransBuf[4] = 0x00;			// ReqBits[0]
	ucTransBuf[5] = 0x50;			// ReqBits[1]  = 80 Bits : 0xA1 ADDR Data0...Data7
	ucTransBuf[6] = 0xA1;			// ReqData[0]
	ucTransBuf[7] = ucAddress;		// ReqData[1]
	memcpy(&ucTransBuf[8], pucData, 8);// ReqData[2]...[9]

	
	FEDM_CHK1	(iErr, FEISC_0xBD_ISOTranspCmd(	m_pReader->GetReaderHnd(), 
												m_pReader->GetBusAddress(), 
												0x03,										// Mode 3
												4,											// expected 4 Bit Data
												ucTransBuf,									// ucTransBuf
												16,											// DataToSend
												ucRespBuf,									// ucRespBuf
												&iRespLen,									// Length of response in Bytes
												0)											// hex array
				);

	if(iErr == 0)
	{
		//OK - Analyze the response Protocol
		unsigned int uiRespBits = ((unsigned int)ucRespBuf[0] << 8) | (unsigned int)ucRespBuf[1];
		if((uiRespBits == 4) && ((ucRespBuf[2] == 0x00) || (ucRespBuf[2] == 0x01)))
		{
			//STATUS: NACK0(0x0), or NACK1(0x1)
			return 0x01; //No Transponder in Reader-Field
		}
		else if((uiRespBits == 4) && (ucRespBuf[2] == 0x0A))
		{
			//STATUS: ACK0(0xA)
			return FEDM_OK;
		}
		else
		{
			//Error
			return 0x01; //No Transponder in Reader-Field
		}
	}
	else
	{
		//Status Byte > 0
		return iErr;
	}
}

/***************************************************************************
  Begin        		:	17.12.2009 / M. Sahm

  Version       	:	03.01.00 / 17.12.2009 / M. Sahm

  Function			:	[0xB1][4 Byte Password][CRC16]: Set Password of my-d_move

						NOTE: only for selected tags

  Parameters		:	string&	sPassword							- [in]	password

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_3_Infineon_my_d_move::SetPassword(string& sPassword)
{
	//Check buffer size
	if(sPassword.length() < 8)
	{
		return FEDM_ERROR_STRING_LENGTH;
	}

	int iErr = 0;
	unsigned char ucTransBuf[12];
	unsigned char ucRespBuf[64];
	int iRespLen = 0;

	// Build Transmit Data
	ucTransBuf[0] = 0x7F;			// Timoeut = 127ms

	switch(m_pReader->GetReaderType())
	{
		case FEDM_ISC_TYPE_CPR30_XX:
		case FEDM_ISC_TYPE_CPR40_XX_U:
		case FEDM_ISC_TYPE_CPR40_XX:
		case FEDM_ISC_TYPE_CPR44_XX:
			ucTransBuf[1] = 0x05;			// CRC-Info	   = TxCRC Enabled, RxCRC Disabled and Parity Enabled
		break;

		case FEDM_ISC_TYPE_CPR50_XX:
		case FEDM_ISC_TYPE_CPR52_XX:
			ucTransBuf[1] = 0x07;			// CRC-Info	   = TxCRC Enabled, RxCRC Disabled and Parity Odd enabled und Parity Enabled
		break;

		default:
			return FEDM_ERROR_UNKNOWN_READER_TYPE;
	}
	
	ucTransBuf[2] = 0x00;			// TX-Settings = 106kBit/s
	ucTransBuf[3] = 0x00;			// RX-Settings = 106kBit/s
	ucTransBuf[4] = 0x00;			// ReqBits[0]
	ucTransBuf[5] = 0x28;			// ReqBits[1]  = 40 Bits : 0xB1 P0...P3
	ucTransBuf[6] = 0xB1;			// ReqData[0]
	FEDM_ConvHexStrToHexUChar(sPassword, &ucTransBuf[7], 4); //ReqData[1]...[4]

	
	FEDM_CHK1	(iErr, FEISC_0xBD_ISOTranspCmd(	m_pReader->GetReaderHnd(), 
												m_pReader->GetBusAddress(), 
												0x03,										// Mode 3
												48,											// expected 4 Bytes Data + 2 Bytes CRC
												ucTransBuf,									// ucTransBuf
												11,											// DataToSend
												ucRespBuf,									// ucRespBuf
												&iRespLen,									// Length of response in Bytes
												0)											// hex array
				);

	if(iErr == 0)
	{
		//OK - Analyze the response Protocol
		unsigned int uiRespBits = ((unsigned int)ucRespBuf[0] << 8) | (unsigned int)ucRespBuf[1];
		if((uiRespBits == 4) && ((ucRespBuf[2] == 0x00) || (ucRespBuf[2] == 0x01)))
		{
			//STATUS: NACK0(0x0), or NACK1(0x1)
			return 0x01; //No Transponder in Reader-Field
		}
		else if(uiRespBits == 48)
		{
			if(CalcCRC16Csum(&ucRespBuf[2], 6) == 0)
			{
				FEDM_ConvHexUCharToHexStr(&ucRespBuf[2], 4, sPassword);
				return FEDM_OK;
			}
			else
			{
				//CRC-Error
				return 0x01; //No Transponder in Reader-Field				
			}
		}
		else
		{
			//Error
			return 0x01; //No Transponder in Reader-Field
		}
	}
	else
	{
		//Status Byte > 0
		return iErr;
	}

}

/***************************************************************************
  Begin        		:	17.12.2009 / M. Sahm

  Version       	:	03.01.00 / 17.12.2009 / M. Sahm

  Function			:	[0xB2][4 Byte Password][CRC16]: Access with password of my-d_move

						NOTE: only for selected tags

  Parameters		:	string sPassword				- [in]	password

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_3_Infineon_my_d_move::Access(string sPassword)
{
	//Check buffer size
	if(sPassword.length() < 8)
	{
		return FEDM_ERROR_STRING_LENGTH;
	}

	int iErr = 0;
	unsigned char ucTransBuf[12];
	unsigned char ucRespBuf[64];
	int iRespLen = 0;

	// Build Transmit Data
	ucTransBuf[0] = 0x7F;			// Timoeut = 127ms

	switch(m_pReader->GetReaderType())
	{
		case FEDM_ISC_TYPE_CPR30_XX:
		case FEDM_ISC_TYPE_CPR40_XX_U:
		case FEDM_ISC_TYPE_CPR40_XX:
		case FEDM_ISC_TYPE_CPR44_XX:
			ucTransBuf[1] = 0x05;			// CRC-Info	   = TxCRC Enabled, RxCRC Disabled and Parity Enabled
		break;

		case FEDM_ISC_TYPE_CPR50_XX:
		case FEDM_ISC_TYPE_CPR52_XX:
			ucTransBuf[1] = 0x07;			// CRC-Info	   = TxCRC Enabled, RxCRC Disabled and Parity Odd enabled und Parity Enabled
		break;

		default:
			return FEDM_ERROR_UNKNOWN_READER_TYPE;
	}
	
	ucTransBuf[2] = 0x00;			// TX-Settings = 106kBit/s
	ucTransBuf[3] = 0x00;			// RX-Settings = 106kBit/s
	ucTransBuf[4] = 0x00;			// ReqBits[0]
	ucTransBuf[5] = 0x28;			// ReqBits[1]  = 40 Bits : 0xB2 Data0...Data3
	ucTransBuf[6] = 0xB2;			// ReqData[0]
	FEDM_ConvHexStrToHexUChar(sPassword, &ucTransBuf[7], 5); //ReqData[1]...[4]

	
	FEDM_CHK1	(iErr, FEISC_0xBD_ISOTranspCmd(	m_pReader->GetReaderHnd(), 
												m_pReader->GetBusAddress(), 
												0x03,										// Mode 3
												4,											// expected 4 Bit Data
												ucTransBuf,									// ucTransBuf
												11,											// DataToSend
												ucRespBuf,									// ucRespBuf
												&iRespLen,									// Length of response in Bytes
												0)											// hex array
				);

	if(iErr == 0)
	{
		//OK - Analyze the response Protocol
		unsigned int uiRespBits = ((unsigned int)ucRespBuf[0] << 8) | (unsigned int)ucRespBuf[1];
		if((uiRespBits == 4) && ((ucRespBuf[2] == 0x00) || (ucRespBuf[2] == 0x01)))
		{
			//STATUS: NACK0(0x0), or NACK1(0x1)
			return 0x01; //No Transponder in Reader-Field
		}
		else if((uiRespBits == 4) && (ucRespBuf[2] == 0x0A))
		{
			//STATUS: ACK0(0xA)
			return FEDM_OK;
		}
		else
		{
			//Error
			return 0x01; //No Transponder in Reader-Field
		}
	}
	else
	{
		//Status Byte > 0
		return iErr;
	}
}

/***************************************************************************
  Begin        		:	17.12.2009 / M. Sahm

  Version       	:	03.01.00 / 17.12.2009 / M. Sahm

  Function			:	[0xD0][2 Byte Counter Value][CRC16]: Decrement the Value Counter

						NOTE: only for selected tags

  Parameters		:	unsigned int uiCounterValue				- [in]	LSByte = CNT0
																		MSByte = CNT1
						
  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_3_Infineon_my_d_move::Decrement(unsigned int& uiCounterValue)
{
	//Check buffer size
	if(uiCounterValue > 4095)
	{
		return FEDM_ERROR_PARAMETER;
	}

	int iErr = 0;
	unsigned char ucTransBuf[9];
	unsigned char ucRespBuf[64];
	int iRespLen = 0;

	// Build Transmit Data
	ucTransBuf[0] = 0x7F;			// Timoeut = 127ms

	switch(m_pReader->GetReaderType())
	{
		case FEDM_ISC_TYPE_CPR30_XX:
		case FEDM_ISC_TYPE_CPR40_XX_U:
		case FEDM_ISC_TYPE_CPR40_XX:
		case FEDM_ISC_TYPE_CPR44_XX:
			ucTransBuf[1] = 0x05;			// CRC-Info	   = TxCRC Enabled, RxCRC Disabled and Parity Enabled
		break;

		case FEDM_ISC_TYPE_CPR50_XX:
		case FEDM_ISC_TYPE_CPR52_XX:
			ucTransBuf[1] = 0x07;			// CRC-Info	   = TxCRC Enabled, RxCRC Disabled and Parity Odd enabled und Parity Enabled
		break;

		default:
			return FEDM_ERROR_UNKNOWN_READER_TYPE;
	}
	
	ucTransBuf[2] = 0x00;			// TX-Settings = 106kBit/s
	ucTransBuf[3] = 0x00;			// RX-Settings = 106kBit/s
	ucTransBuf[4] = 0x00;			// ReqBits[0]
	ucTransBuf[5] = 0x18;			// ReqBits[1]  = 24 Bits : 0xD0 Data0 Data1
	ucTransBuf[6] = 0xD0;			// ReqData[0]
	ucTransBuf[7] = (unsigned char) (uiCounterValue & 0xFF);			// ReqData[1]
	ucTransBuf[8] = (unsigned char)((uiCounterValue >> 8) & 0xFF);	// ReqData[2]
	
	
	FEDM_CHK1	(iErr, FEISC_0xBD_ISOTranspCmd(	m_pReader->GetReaderHnd(), 
												m_pReader->GetBusAddress(), 
												0x03,										// Mode 3
												32,											// expected 16 Bit Data + 2 Bytes CRC
												ucTransBuf,									// ucTransBuf
												9,											// DataToSend
												ucRespBuf,									// ucRespBuf
												&iRespLen,									// Length of response in Bytes
												0)											// hex array
				);

	if(iErr == 0)
	{
		//OK - Analyze the response Protocol
		unsigned int uiRespBits = ((unsigned int)ucRespBuf[0] << 8) | (unsigned int)ucRespBuf[1];
		if((uiRespBits == 4) && ((ucRespBuf[2] == 0x00) || (ucRespBuf[2] == 0x01)))
		{
			//STATUS: NACK0(0x0), or NACK1(0x1)
			return 0x01; //No Transponder in Reader-Field
		}
		else if(uiRespBits == 32)
		{
			if(CalcCRC16Csum(&ucRespBuf[2], 4) == 0)
			{
				uiCounterValue = ucRespBuf[2];
				uiCounterValue |= (ucRespBuf[3] << 8);
				return FEDM_OK;
			}
			else
			{
				//CRC-Error
				return 0x01; //No Transponder in Reader-Field				
			}
		}
		else
		{
			//Error
			return 0x01; //No Transponder in Reader-Field
		}
	}
	else
	{
		//Status Byte > 0
		return iErr;
	}
}

/***************************************************************************
  Begin        		:	20.01.2010 / M. Sahm

  Version       	:	03.01.00 / 20.10.2010 / M. Sahm

  Function			:	Claculates the CRC16 from ISO 14443A Data


  Parameters		:	unsigned char *			pData
						unsigned int			uiDataLen
						
  Return value		:	CRC16 Value
***************************************************************************/
unsigned short FedmIscTagHandler_ISO14443_3_Infineon_my_d_move::CalcCRC16Csum(unsigned char *pucData, unsigned int uiDataLen)
{
	unsigned short usCrc = 0x6363;
	unsigned char chBlock;

	do
	{
		chBlock = *pucData++;
		chBlock = (chBlock^(unsigned char)((usCrc) & 0x00FF));
		chBlock = (chBlock^(chBlock<<4));
		usCrc = (usCrc>>8)^((unsigned short)chBlock<<8)^((unsigned short) chBlock <<3)^((unsigned short)chBlock >>4);
	}while(--uiDataLen);

	return usCrc;
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
