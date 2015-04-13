/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO14443_4_Maxim_MAX66040.cpp  |
|                                                       |
---------------------------------------------------------

Copyright © 2010-2011	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	04.10.2010

Version       		:	03.03.02 / 29.04.2011 / M. Raner, M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID® classic-pro transponder ISO14443 of manufacturer MAXIM

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "FedmIscTagHandler_ISO14443_4_Maxim_MAX66040.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"

#include "feisc.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif



//####################################################################
// class FedmIscTagHandler_ISO14443_Maxim
// >> supports only ISO Host Commands <<
//####################################################################

FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::FedmIscTagHandler_ISO14443_4_Maxim_MAX66040(
	FEDM_ISCReader* pReader, 
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_4_Maxim(pReader, FedmIscTagHandler::TYPE_ISO14443_4_MAXIM_MAX66040, pTabItem)
{
	m_sTagName = "ISO 14443 Type B : Maxim MAX66040";
	UpdateChallenge();
}

FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::FedmIscTagHandler_ISO14443_4_Maxim_MAX66040(
	FEDM_ISCReader* pReader, 
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO14443_4_Maxim(pReader, uiTagHandlerType, pTabItem)
{
	m_sTagName = "ISO 14443 Type B : Maxim MAX66040";
	UpdateChallenge();
}

FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::~FedmIscTagHandler_ISO14443_4_Maxim_MAX66040()
{
}

// set all data members to 0
/*void FedmIscTagHandler_ISO14443::Init()
{
}*/

/***************************************************************************
  Begin        		:	06.10.2010 / M. Raner

  Version       	:	03.02.05 / 06.10.2010 / M. Raner

  Function			:	computes PageMAC
  
  Parameters		:	unsigned char* pucSecret			- [in]	request flags byte
						unsigned char* pucMemoryPageData	- [in]	32byte memory page data
						unsigned char  ucPageNumber			- [in]	number of page involved in computation
						unsigned char* pucUID				- [in]	uid value of tag
						unsigned char* pucMAC				- [out]	computed page MAC

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::UserComputePageMAC(
		unsigned char* pucSecret, 
		unsigned char* pucMemoryPageData, 
		unsigned char ucPageNumber, 
		unsigned char* pucUID,
		unsigned char* pucMAC)
{
	FEDM_CHK3(pucSecret);
	FEDM_CHK3(pucMemoryPageData);
	FEDM_CHK3(pucUID);
	FEDM_CHK3(pucMAC);

	unsigned char buffer[64];

	int iCnt = 0;

	if((ucPageNumber >= 0x00) && (ucPageNumber <= 0x04))
	{
		buffer[0] = pucSecret[0];
		buffer[1] = pucSecret[1];
		buffer[2] = pucSecret[2];
		buffer[3] = pucSecret[3];

		for(iCnt = 4; iCnt < 36; iCnt++)
		{
			buffer[iCnt] = pucMemoryPageData[iCnt - 4];
		}

		if(ucPageNumber == 0x04)
		{
			for(iCnt = 20; iCnt < 36; iCnt++)
			{
				buffer[iCnt] = 0xFF;
			}
		}

		buffer[36] = FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::ucChallenge[0];
		buffer[37] = FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::ucChallenge[1];

		buffer[38] = 0xFF;
		buffer[39] = 0xFF;

		buffer[40] = (unsigned char)((0x4 << 4) |ucPageNumber);

		for(iCnt = 41; iCnt < 48; iCnt++)
		{	
			buffer[iCnt] = pucUID[iCnt - 41];	
		}
		
		buffer[48] = pucSecret[4];
		buffer[49] = pucSecret[5];
		buffer[50] = pucSecret[6];
		buffer[51] = pucSecret[7];

		buffer[52] = FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::ucChallenge[2];
		buffer[53] = FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::ucChallenge[3];
		buffer[54] = FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::ucChallenge[4];

		buffer[55] = 0x80;
		
		for(iCnt = 56; iCnt < 62; iCnt++)
		{
			buffer[iCnt] = 0x00;
		}

		buffer[62] = 0x01;
		buffer[63] = 0xB8;
	}

	unsigned int edcba[5];
	FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::ComputeMAC(buffer,edcba);

	for(iCnt = 0; iCnt < 4; iCnt++)
	{
		pucMAC[0+iCnt] = (unsigned char)((edcba[0] >> (iCnt * 8)) & 0xFF);
	}
	for(iCnt = 0; iCnt < 4; iCnt++)
	{
		pucMAC[4+iCnt] = (unsigned char)((edcba[1] >> (iCnt * 8)) & 0xFF);
	}
	for(iCnt = 0; iCnt < 4; iCnt++)
	{
		pucMAC[8+iCnt] = (unsigned char)((edcba[2] >> (iCnt * 8)) & 0xFF);
	}
	for(iCnt = 0; iCnt < 4; iCnt++)
	{
		pucMAC[12+iCnt] = (unsigned char)((edcba[3] >> (iCnt * 8)) & 0xFF);
	}
	for(iCnt = 0; iCnt < 4; iCnt++)
	{
		pucMAC[16+iCnt] = (unsigned char)((edcba[4] >> (iCnt * 8)) & 0xFF);
	}

	return 0;
}

/***************************************************************************
  Begin        		:	06.10.2010 / M. Raner

  Version       	:	03.02.05 / 06.10.2010 / M. Raner

  Function			:	computes a new Secret Value
  
  Parameters		:	unsigned char* pucSecret			- [in]	old secret value
						unsigned char* pucMemoryPageData	- [in]	memory page must match memory page number in GenerateSecret()
						unsigned char* pucPartialSecret		- [in]	this value must match partial secret value in GenerateSecret()
						unsigned char* pucNewSecret			- [out]	computed new secret

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int	FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::UserComputeSecret(
		unsigned char* pucSecret, 
		unsigned char* pucMemoryPageData, 
		unsigned char* pucPartialSecret, 
		unsigned char* pucNewSecret)
{
	FEDM_CHK3(pucSecret);
	FEDM_CHK3(pucMemoryPageData);
	FEDM_CHK3(pucPartialSecret);
	FEDM_CHK3(pucNewSecret);

	unsigned char buffer[64];
	int iCnt = 0;

	buffer[0] = pucSecret[0];
	buffer[1] = pucSecret[1];
	buffer[2] = pucSecret[2];
	buffer[3] = pucSecret[3];

	for(iCnt = 4; iCnt < 36; iCnt++)
	{
		buffer[iCnt] = pucMemoryPageData[iCnt - 4];
	}

	buffer[36] = 0xFF;
	buffer[37] = 0xFF;
	buffer[38] = 0xFF;
	buffer[39] = 0xFF;

	buffer[40] = pucPartialSecret[0] & 0x3f;

	for(iCnt = 41; iCnt < 48; iCnt++)
	{
		buffer[iCnt] = pucPartialSecret[iCnt-40];
	}
	
	buffer[48] = pucSecret[4];
	buffer[49] = pucSecret[5];
	buffer[50] = pucSecret[6];
	buffer[51] = pucSecret[7];

	buffer[52] = 0xFF;
	buffer[53] = 0xFF;
	buffer[54] = 0xFF;

	buffer[55] = 0x80;
	
	for(iCnt = 56; iCnt < 62; iCnt++)
	{
		buffer[iCnt] = 0x00;
	}

	buffer[62] = 0x01;
	buffer[63] = 0xB8;

	unsigned int edcba[5];
	FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::ComputeMAC(buffer,edcba);

	for(iCnt = 0; iCnt < 4; iCnt++)
	{
		pucNewSecret[0+iCnt] = (unsigned char)((edcba[0] >> (iCnt * 8)) & 0xFF);
	}
	for(iCnt = 0; iCnt < 4; iCnt++)
	{
		pucNewSecret[4+iCnt] = (unsigned char)((edcba[1] >> (iCnt * 8)) & 0xFF);
	}

	return 0;
}

/***************************************************************************
  Begin        		:	06.10.2010 / M. Raner

  Version       	:	03.02.05 / 06.10.2010 / M. Raner

  Function			:	UserComputeCopyBufferMAC
  
  Parameters		:	unsigned char* pucSecret				- [in]	secret
						unsigned char* pucMemoryPageData		- [in]	memory page must match memory page number used in CopyBuffer()
						unsigned char* pucReadWriteBufferData	- [in]	must match data in read/write Buffer
						unsigned char* pucIntegrityBytes		- [in]	integrity bytes(write counter) of datablock
						unsigned char pucPageNumber				- [in]	number of page used in CopyBuffer()
						unsigned char* pucUID					- [in]	uid value of tag
						unsigned char* pucMAC					- [out]	computed CopyBuffer MAC

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::UserComputeCopyBufferMAC(
		unsigned char* pucSecret, 
		unsigned char* pucMemoryPageData, 
		unsigned char* pucReadWriteBufferData, 
		unsigned char* pucIntegrityBytes,
		unsigned char pucPageNumber, 
		unsigned char* pucUID,
		unsigned char* pucMAC)
{
	FEDM_CHK3(pucSecret);
	FEDM_CHK3(pucMemoryPageData);
	FEDM_CHK3(pucReadWriteBufferData);
	FEDM_CHK3(pucIntegrityBytes);
	FEDM_CHK3(pucUID);
	FEDM_CHK3(pucMAC);

	unsigned char buffer[64];

	int iCnt = 0;

	if((pucPageNumber >= 0x00) && (pucPageNumber <= 0x04))
	{
		buffer[0] = pucSecret[0];
		buffer[1] = pucSecret[1];
		buffer[2] = pucSecret[2];
		buffer[3] = pucSecret[3];

		for(iCnt = 4; iCnt < 32; iCnt++)
		{
			buffer[iCnt] = pucMemoryPageData[iCnt - 4];
		}

		if(pucPageNumber == 0x04)
		{
			for(iCnt = 20; iCnt < 32; iCnt++)
			{
				buffer[iCnt] = 0xFF;
			}
		}

		for(iCnt = 32; iCnt < 40; iCnt++)
		{
			buffer[iCnt] = pucReadWriteBufferData[iCnt-32];
		}

		buffer[40] = pucPageNumber;

		for(iCnt = 41; iCnt < 48; iCnt++)
		{
			buffer[iCnt] = pucUID[iCnt - 41];	
		}
		
		buffer[48] = pucSecret[4];
		buffer[49] = pucSecret[5];
		buffer[50] = pucSecret[6];
		buffer[51] = pucSecret[7];

		buffer[52] = pucIntegrityBytes[0];
		buffer[53] = pucIntegrityBytes[1];
		buffer[54] = 0xFF;

		buffer[55] = 0x80;
		
		for(iCnt = 56; iCnt < 62; iCnt++)
		{
			buffer[iCnt] = 0x00;
		}

		buffer[62] = 0x01;
		buffer[63] = 0xB8;
	}	

	unsigned int edcba[5];
	FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::ComputeMAC(buffer,edcba);

	for(iCnt = 0; iCnt < 4; iCnt++)
	{
		pucMAC[0+iCnt] = (unsigned char)((edcba[0] >> (iCnt * 8)) & 0xFF);
	}
	for(iCnt = 0; iCnt < 4; iCnt++)
	{
		pucMAC[4+iCnt] = (unsigned char)((edcba[1] >> (iCnt * 8)) & 0xFF);
	}
	for(iCnt = 0; iCnt < 4; iCnt++)
	{
		pucMAC[8+iCnt] = (unsigned char)((edcba[2] >> (iCnt * 8)) & 0xFF);
	}
	for(iCnt = 0; iCnt < 4; iCnt++)
	{
		pucMAC[12+iCnt] = (unsigned char)((edcba[3] >> (iCnt * 8)) & 0xFF);
	}
	for(iCnt = 0; iCnt < 4; iCnt++)
	{
		pucMAC[16+iCnt] = (unsigned char)((edcba[4] >> (iCnt * 8)) & 0xFF);
	}

	return 0;
}

/***************************************************************************
  Begin        		:	06.10.2010 / M. Raner

  Version       	:	03.02.05 / 06.10.2010 / M. Raner

  Function			:	Read Memory Page (use ReadSingleBlock())
  
  Parameters		:	unsigned char	ucPageNumber		- [in]	number of page, which should be read
						unsigned char&  ucIndicator			- [out] error indicator from transponder
						unsigned char*	pucMemoryPageData	- [out]	32byte memory page data

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::GetMemoryPageData( 
		unsigned char ucPageNumber, 
		unsigned char& ucIndicator,
		unsigned char* pucMemoryPageData)
{
	FEDM_CHK3(pucMemoryPageData);
	int iBack = 0;

	unsigned char pucMemoryData[8];
	int iRounds = 0;

	if(ucPageNumber >= 0 && ucPageNumber < 4)
	{
		iRounds = 4;
	}
	else if(ucPageNumber == 4)
	{
		iRounds = 3;
	}

	for(int iCnt = 0; iCnt < iRounds; iCnt++)
	{
		iBack = ReadSingleBlock(ucPageNumber * 4 + iCnt, ucIndicator, pucMemoryData);
		if(iBack != 0)
		{
			return iBack;
		}

		if(ucIndicator)
			return 0;

		for(int iCnt2 = 0; iCnt2 < 8; iCnt2++)
		{
			pucMemoryPageData[iCnt*8+iCnt2] = pucMemoryData[iCnt2];
		}
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	06.10.2010 / M. Raner

  Version       	:	03.02.05 / 06.10.2010 / M. Raner

  Function			:	computes MAC for UserCompute... functions
  
  Parameters		:	unsigned char*	pucM	- [in]	input data of sixteen 32-bit Words
						unsigned int*	puiMAC	- [out]	computed MAC

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::ComputeMAC(							
		unsigned char* pucM, 
		unsigned int* puiMAC)
{
	FEDM_CHK3(pucM);
	FEDM_CHK3(puiMAC);

	/*
		This function includes a shriked version of SHA1 algorithm. It starts with input of 
		eighty 32-bit words and ends with output of the values A,B,C,D,E of the 160-bit key.
		The final constants, defined in ISO/IEC 10118-3 specification, are removed in this 
		shriked version. The final constants are normally added after each 
		block is computed. Because the MAX66x40 only processes one block, adding these 
		constants does not add additional security.
	*/
	unsigned int uiW[80];				/* eighty 32-bit words */
	unsigned int uiA = 0x67452301;		/* init values defined in FIPS 180-1 */
    unsigned int uiB = 0xEFCDAB89;		/* init values defined in FIPS 180-1 */
    unsigned int uiC = 0x98BADCFE;		/* init values defined in FIPS 180-1 */
    unsigned int uiD = 0x10325476;		/* init values defined in FIPS 180-1 */
    unsigned int uiE = 0xC3D2E1F0;		/* init values defined in FIPS 180-1 */
    unsigned int uiF;
    unsigned int uiK;
    unsigned int uiTmp;
	int iCnt2 = 0;

	for(int iCnt = 0; iCnt*64 < 64; iCnt++)
	{
		for (iCnt2 = 0; iCnt2 < 16; iCnt2++)
        {
            uiW[iCnt2] = (unsigned int)pucM[iCnt2 * 4 + (iCnt * 64)] << 24 | (unsigned int)pucM[iCnt2 * 4 + 1 + (iCnt * 64)] << 16 | (unsigned int)pucM[iCnt2 * 4 + 2 + (iCnt * 64)] << 8 | (unsigned int)pucM[iCnt2 * 4 + 3 + (iCnt * 64)];
        }

        /* Expand from 16 to 80 words */
        for (iCnt2 = 16; iCnt2 < 80; iCnt2++)
        {
			uiW[iCnt2] = (((uiW[iCnt2 - 3] ^ uiW[iCnt2 - 8] ^ uiW[iCnt2 - 14] ^ uiW[iCnt2 - 16]) << 1) | ((uiW[iCnt2 - 3] ^ uiW[iCnt2 - 8] ^ uiW[iCnt2 - 14] ^ uiW[iCnt2 - 16]) >> (32 - 1)));
        }

        for (iCnt2 = 0; iCnt2 < 80; iCnt2++)
        {
            if (iCnt2 >= 0 && iCnt2 <= 19)
            {
                uiF = (uiB & uiC) | (~uiB & uiD);	/* defined in FIPS 180-1 */
                uiK = 0x5A827999;						/* defined in FIPS 180-1 */
            }
            else if (iCnt2 >= 20 && iCnt2 <= 39)
            {
                uiF = uiB ^ uiC ^ uiD;	/* defined in FIPS 180-1 */
                uiK = 0x6ED9EBA1;			/* defined in FIPS 180-1 */
            }
            else if (iCnt2 >= 40 && iCnt2 <= 59)
            {
                uiF = (uiB & uiC) | (uiB & uiD) | (uiC & uiD);	/* defined in FIPS 180-1 */
                uiK = 0x8F1BBCDC;									/* defined in FIPS 180-1 */
            }
            else if (iCnt2 >= 60 && iCnt2 <= 79)
            {
                uiF = uiB ^ uiC ^ uiD;	/* defined in FIPS 180-1 */
                uiK = 0xCA62C1D6;			/* defined in FIPS 180-1 */
            }

			uiTmp = ((uiA << 5) | (uiA >> (32 - 5))) + uiF + uiE + uiK + uiW[iCnt2];
            uiE = uiD;
            uiD = uiC;
			uiC = ((uiB << 30) | (uiB >> (32 - 30)));
            uiB = uiA;
            uiA = uiTmp;
        }
	}

	puiMAC[0] = uiE;
	puiMAC[1] = uiD;
	puiMAC[2] = uiC;
	puiMAC[3] = uiB;
	puiMAC[4] = uiA;

	return 0;
}

/***************************************************************************
  Begin        		:	06.10.2010 / M. Raner

  Version       	:	03.02.05 / 06.10.2010 / M. Raner

  Function			:	generates new random challenge
  
  Parameters		:	-

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::UpdateChallenge()
{
	for(int iCnt = 0; iCnt < 5; iCnt++)
	{
		ucChallenge[iCnt] = (unsigned char)rand();
	}
	return 0;
}

/***************************************************************************
  Begin        		:	08.10.2010 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xB2][0xBE][0xA0] WriteBuffer
  
  Parameters		:	unsigned char ucControlByte		- [in]	control code (load, compute, locking)
						unsigned char* pucPartialSecret	- [in]	secret to be load, used in computation
						unsigned char&	ucIndicator		- [out] response indicator byte (if 0x00 then command execution was successful)
						unsigned char& ucPageCRCStatus	- [out]	status code of crc check

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::GenerateSecret(	
		unsigned char ucControlByte,
		unsigned char* pucPartialSecret,
		unsigned char& ucIndicator,
		unsigned char& ucPageCRCStatus)
{
	//******************************************************************************
	// ParameterDescription

		/*
		control byte(size: 1byte)
			code:	function:							info:
			0xF0	load secret without locking			
			0x0F	load secret with locking			secret write protection byte is changed to 0x0A
			0xn5	compute secret without locking		n specifies the page number to be used in computation
			0xnA	compute and lock secret				WARNING: permanently write protect the secret, n specifies the page number to be used in computation
			0xAA	load and lock secret				WARNING: permanently write protect the secret

		partial secret(size 8byte)
			controlByte:		function:
			0xF0, 0x0F, 0xAA	the new secret that will be loaded
			0xn5, 0xnA			partial secret to be used in computation of new secret
		*/


	//******************************************************************************
	// ParameterTesting

	//Test Pointer for NULL
	FEDM_CHK3(pucPartialSecret);

	//Check ControlByte for valid values
	if(!(ucControlByte == 0xF0 || 
		ucControlByte == 0x0F || 
		(ucControlByte & 0x05) == 0x05 || 
		(ucControlByte & 0x0A) == 0x0A ||
		ucControlByte == 0xAA))
	{
		//Error: Control byte is invalid
		return FEDM_ERROR_PARAMETER;
	}

	//********************************************************************
	// Declaration
	unsigned int uiLengthOfResponseData = 0;

	//********************************************************************
	// RequestData
	m_ucRequestData[0] = 0xA0;	// Command
	m_ucRequestData[1] = ucControlByte;
	for(int iCnt = 0; iCnt < 8; iCnt++)
	{
		m_ucRequestData[2+iCnt] = pucPartialSecret[iCnt];
	}

	//******************************************************************************
	// CommandExecute
	//execute command
	int iBack = Apdu(m_ucRequestData, 10, m_ucResponseData, 256, uiLengthOfResponseData);
	if(iBack)
		return iBack;

	//******************************************************************************
	// ResponseData
	if(uiLengthOfResponseData > 0)
	{
		ucIndicator = m_ucResponseData[0];
		if(ucIndicator == 0)
		{
			ucPageCRCStatus = m_ucResponseData[1];
		}
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	06.10.2010 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xB2][0xBE][0xA1] WriteBuffer
  
  Parameters		:	unsigned char	ucBlockNumber		- [in]	number of block(0x00 to 0x11)
						unsigned char*	pucNewBlockData		- [in]	New block-data
						unsigned char&	ucIndicator			- [out] response indicator byte (if 0x00 then command execution was successful)

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::WriteBuffer(						
		unsigned char ucBlockNumber,
		unsigned char* pucNewBlockData,
		unsigned char& ucIndicator)
{
	//********************************************************************
	// ParameterTesting
	//Test Pointer for NULL
	FEDM_CHK3(pucNewBlockData);

	//********************************************************************
	// Declaration
	unsigned int uiLengthOfResponseData = 0;

	//********************************************************************
	// RequestData
	m_ucRequestData[0] = 0xA1;	// Command
	m_ucRequestData[1] = ucBlockNumber;
	for(int iCnt = 0; iCnt < 8; iCnt++)
	{
		m_ucRequestData[2+iCnt] = pucNewBlockData[iCnt];
	}

	//********************************************************************
	// CommandExecute
	int iBack = Apdu(m_ucRequestData, 10, m_ucResponseData, 256, uiLengthOfResponseData);
	if(iBack)
		return iBack;

	//********************************************************************
	// ResponseData
	if(uiLengthOfResponseData > 0)
	{
		ucIndicator = m_ucResponseData[0];
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	06.10.2010 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xB2][0xBE][0xA2] ReadBuffer
  
  Parameters		:	unsigned char&	ucIndicator				- [out]	response indicator byte (if 0x00 then command execution was successful)
						unsigned char&	ucBlockNumber			- [out]	number of block
						unsigned char*	pucReadWriteBufferData	- [out]	data of read/write Buffer

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::ReadBuffer(							
		unsigned char& ucIndicator,
		unsigned char& ucBlockNumber,
		unsigned char* pucReadWriteBufferData)
{
	//********************************************************************
	// ParameterTesting
	//Test Pointer for NULL
	FEDM_CHK3(pucReadWriteBufferData);
	
	//********************************************************************
	// Declaration
	unsigned int uiLengthOfResponseData = 0;

	//********************************************************************
	// RequestData
	m_ucRequestData[0] = 0xA2;	// Command

	//********************************************************************
	// CommandExecute
	int iBack = Apdu(m_ucRequestData, 1, m_ucResponseData, 256, uiLengthOfResponseData);
	if(iBack)
		return iBack;

	//********************************************************************
	// ResponseData
	if(uiLengthOfResponseData > 0)
	{
		ucIndicator = m_ucResponseData[0];
		if(ucIndicator == 0)
		{
			ucBlockNumber = m_ucResponseData[1];
			for(int iCnt = 0; iCnt < 8; iCnt++)
			{
				pucReadWriteBufferData[iCnt] = m_ucResponseData[2 + iCnt];
			}
		}
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	06.10.2010 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xB2][0xBE][0xA3] CopyBuffer
  
  Parameters		:	unsigned char	ucBlockNumber	- [in]	number of block(0x00 to 0x12)
						unsigned char*	pucMAC			- [in]	CopyBufferMAC from UserComputeCopyBufferMAC()
						unsigned char&	ucIndicator		- [out]	response indicator byte (if 0x00 then command execution was successful)
						unsigned char&	ucPageCRCStatus	- [out]	status code of crc check

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::CopyBuffer(							
		unsigned char ucBlockNumber,
		unsigned char* pucMAC,
		unsigned char& ucIndicator,
		unsigned char& ucPageCRCStatus)
{
	//********************************************************************
	// ParameterTesting
	//Test Pointer for NULL
	FEDM_CHK3(pucMAC);

	//********************************************************************
	// Declaration
	unsigned int uiLengthOfResponseData = 0;

	//********************************************************************
	// RequestData
	m_ucRequestData[0] = 0xA3;	// Command
	m_ucRequestData[1] = ucBlockNumber;

	for(int iCnt = 0; iCnt < 20; iCnt++)
	{
		m_ucRequestData[2+iCnt] = pucMAC[iCnt];
	}

	//********************************************************************
	// CommandExecute
	int iBack = Apdu(m_ucRequestData, 22, m_ucResponseData, 256, uiLengthOfResponseData);
	if(iBack)
		return iBack;

	//********************************************************************
	// ResponseData
	if(uiLengthOfResponseData > 0)
	{
		ucIndicator = m_ucResponseData[0];
		if(ucIndicator == 0)
		{
			ucPageCRCStatus = m_ucResponseData[1];
		}
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	06.10.2010 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xB2][0xBE][0x20] ReadSingleBlock
  
  Parameters		:	unsigned char	ucBlockNumber		- [in]	number of block(0x00 to 0x12)
						unsigned char&	ucIndicator			- [out]	response indicator byte (if 0x00 then command execution was successful)
						unsigned char*	pucMemoryData		- [out]	readed memory data

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::ReadSingleBlock(					
		unsigned char ucBlockNumber,
		unsigned char& ucIndicator,
		unsigned char* pucMemoryData)
{
	//********************************************************************
	// ParameterTesting
	//Test Pointer for NULL
	FEDM_CHK3(pucMemoryData);

	//********************************************************************
	// Declaration
	unsigned int uiLengthOfResponseData = 0;

	//********************************************************************
	// RequestData
	m_ucRequestData[0] = 0x20;	// Command
	m_ucRequestData[1] = ucBlockNumber;

	//********************************************************************
	// CommandExecute
	int iBack = Apdu(m_ucRequestData, 2, m_ucResponseData, 256, uiLengthOfResponseData);
	if(iBack)
		return iBack;

	//********************************************************************
	// ResponseData
	if(uiLengthOfResponseData > 0)
	{
		ucIndicator = m_ucResponseData[0];
		if(ucIndicator == 0)
		{
			for(int iCnt = 0; iCnt < 8; iCnt++)
			{
				pucMemoryData[iCnt] = m_ucResponseData[1 + iCnt];
			}
		}
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	06.10.2010 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xB2][0xBE][0xB0] ReadSingleBlockWithBlockSecurity
  
  Parameters		:	unsigned char	ucBlockNumber		- [in]	number of block(0x00 to 0x12)
						unsigned char&	ucIndicator			- [out]	response indicator byte (if 0x00 then command execution was successful)
						unsigned char&	ucSecurityStatus	- [out]	security status code
						unsigned char*	pucMemoryData		- [out]	readed memory data

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::ReadSingleBlockWithBlockSecurity(	
		unsigned char ucBlockNumber,
		unsigned char& ucIndicator,
		unsigned char& ucSecurityStatus,
		unsigned char* pucMemoryData)
{
	//********************************************************************
	// ParameterTesting
	//Test Pointer for NULL
	FEDM_CHK3(pucMemoryData);

	//********************************************************************
	// Declaration
	unsigned int uiLengthOfResponseData = 0;

	//********************************************************************
	// RequestData
	m_ucRequestData[0] = 0xB0;	// Command
	m_ucRequestData[1] = ucBlockNumber;

	//********************************************************************
	// CommandExecute
	int iBack = Apdu(m_ucRequestData, 2, m_ucResponseData, 256, uiLengthOfResponseData);
	if(iBack)
		return iBack;

	//********************************************************************
	// ResponseData
	if(uiLengthOfResponseData > 0)
	{
		ucIndicator = m_ucResponseData[0];
		if(ucIndicator == 0)
		{
			ucSecurityStatus = m_ucResponseData[1];
			for(int iCnt = 0; iCnt < 8; iCnt++)
			{
				pucMemoryData[iCnt] = m_ucResponseData[2 + iCnt];
			}
		}
	}


	return iBack;
}

/***************************************************************************
  Begin        		:	06.10.2010 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xB2][0xBE][0xA4] CustomReadBlock
  
  Parameters		:	unsigned char	ucBlockNumber		- [in]	number of block(0x00 to 0x12)
						unsigned char&	ucIndicator			- [out]	response indicator byte (if 0x00 then command execution was successful)
						unsigned char*	pucMemoryData		- [out]	readed memory data
						unsigned char*	pucIntegrityBytes	- [out]	integrity bytes(write counter) of datablock

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::CustomReadBlock(					
		unsigned char ucBlockNumber,
		unsigned char& ucIndicator,
		unsigned char* pucMemoryData,
		unsigned char* pucIntegrityBytes)
{
	//********************************************************************
	// ParameterTesting
	//Test Pointer for NULL
	FEDM_CHK3(pucMemoryData);
	FEDM_CHK3(pucIntegrityBytes);

	//********************************************************************
	// Declaration
	unsigned int uiLengthOfResponseData = 0;

	//********************************************************************
	// RequestData
	m_ucRequestData[0] = 0xA4;	// Command
	m_ucRequestData[1] = ucBlockNumber;

	//********************************************************************
	// CommandExecute
	int iBack = Apdu(m_ucRequestData, 2, m_ucResponseData, 256, uiLengthOfResponseData);
	if(iBack)
		return iBack;

	//********************************************************************
	// ResponseData
	if(uiLengthOfResponseData > 0)
	{
		ucIndicator = m_ucResponseData[0];
		if(ucIndicator == 0)
		{
			for(int iCnt = 0; iCnt < 8; iCnt++)
			{
				pucMemoryData[iCnt] = m_ucResponseData[1 + iCnt];
			}
			pucIntegrityBytes[0] = m_ucResponseData[9];
			pucIntegrityBytes[1] = m_ucResponseData[10];
		}
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	06.10.2010 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xB2][0xBE][0xB4] CustomReadBlockWithBlockSecurity
  
  Parameters		:	unsigned char	ucBlockNumber		- [in]	number of block(0x00 to 0x12)
						unsigned char&	ucIndicator			- [out]	response indicator byte (if 0x00 then command execution was successful)
						unsigned char&	ucSecurityStatus	- [out]	security status code
						unsigned char*	pucMemoryData		- [out]	readed memory data
						unsigned char*	pucIntegrityBytes	- [out]	integrity bytes(write counter) of datablock

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::CustomReadBlockWithBlockSecurity(	
		unsigned char ucBlockNumber,
		unsigned char& ucIndicator,
		unsigned char& ucSecurityStatus,
		unsigned char* pucMemoryData,
		unsigned char* pucIntegrityBytes)
{
	//********************************************************************
	// ParameterTesting
	//Test Pointer for NULL
	FEDM_CHK3(pucMemoryData);
	FEDM_CHK3(pucIntegrityBytes);

	//********************************************************************
	// Declaration
	unsigned int uiLengthOfResponseData = 0;

	//********************************************************************
	// RequestData
	m_ucRequestData[0] = 0xB4;	// Command
	m_ucRequestData[1] = ucBlockNumber;

	//********************************************************************
	// CommandExecute
	int iBack = Apdu(m_ucRequestData, 2, m_ucResponseData, 256, uiLengthOfResponseData);
	if(iBack)
		return iBack;

	//********************************************************************
	// ResponseData
	if(uiLengthOfResponseData > 0)
	{
		ucIndicator = m_ucResponseData[0];
		if(ucIndicator == 0)
		{
			ucSecurityStatus = m_ucResponseData[1];
			for(int iCnt = 0; iCnt < 8; iCnt++)
			{
				pucMemoryData[iCnt] = m_ucResponseData[2 + iCnt];
			}
			pucIntegrityBytes[0] = m_ucResponseData[10];
			pucIntegrityBytes[1] = m_ucResponseData[11];
		}
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	06.10.2010 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xB2][0xBE][0xA5] ComputePageMAC
  
  Parameters		:	unsigned char	ucPageNumber		- [in]	number of page will involve in computation
						unsigned char&	ucIndicator			- [out]	response indicator byte (if 0x00 then command execution was successful)
						unsigned char&	ucPageCRCStatus		- [out]	status code of crc check
						unsigned char*	pucMAC				- [out]	from transponder computed PageMAC

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::ComputePageMAC(						
		unsigned char ucPageNumber,
		unsigned char& ucIndicator,
		unsigned char& ucPageCRCStatus,
		unsigned char* pucMAC)
{
	//********************************************************************
	// ParameterTesting
	//Test Pointer for NULL
	FEDM_CHK3(pucMAC);

	//********************************************************************
	// Declaration
	unsigned int uiLengthOfResponseData = 0;

	//********************************************************************
	// RequestData
	m_ucRequestData[0] = 0xA5;	// Command
	m_ucRequestData[1] = ucPageNumber;

	for(int iCnt = 0; iCnt < 5; iCnt++)
	{
		m_ucRequestData[2 + iCnt] = FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::ucChallenge[iCnt];
	}

	//********************************************************************
	// CommandExecute
	int iBack = Apdu(m_ucRequestData, 7, m_ucResponseData, 256, uiLengthOfResponseData);
	if(iBack)
		return iBack;

	//********************************************************************
	// ResponseData
	if(uiLengthOfResponseData > 0)
	{
		ucIndicator = m_ucResponseData[0];
		if(ucIndicator == 0)
		{
			ucPageCRCStatus = m_ucResponseData[1];
			for(int iCnt = 0; iCnt < 20; iCnt++)
			{
				pucMAC[iCnt] = m_ucResponseData[2 + iCnt];
			}
		}
	}

	return iBack;
}

/*******************************************************************************
  Begin        		:	06.10.2010 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xB2][0xBE][0x27] WriteAFI
  
  Parameters		:	unsigned char	ucAfiValue			- [in]	new AFI Value that will be set on tag
						unsigned char&	ucIndicator			- [out]	response indicator byte (if 0x00 then command execution was successful)

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
*******************************************************************************/ 
int FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::WriteAFI(							
		unsigned char ucAfiValue,
		unsigned char& ucIndicator)
{
	//********************************************************************
	// Declaration
	unsigned int uiLengthOfResponseData = 0;

	//********************************************************************
	// RequestData
	m_ucRequestData[0] = 0x27;	// Command
	m_ucRequestData[1] = ucAfiValue;

	//********************************************************************
	// CommandExecute
	int iBack = Apdu(m_ucRequestData, 2, m_ucResponseData, 256, uiLengthOfResponseData);
	if(iBack)
		return iBack;

	//********************************************************************
	// ResponseData
	if(uiLengthOfResponseData > 0)
	{
		ucIndicator = m_ucResponseData[0];
	}

	return iBack;
}

/*******************************************************************************
  Begin        		:	06.10.2010 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xB2][0xBE][0x28] LockAFI
  
  Parameters		:	unsigned char&	ucIndicator			- [out]	response indicator byte (if 0x00 then command execution was successful)

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
*******************************************************************************/ 
int FedmIscTagHandler_ISO14443_4_Maxim_MAX66040::LockAFI(							
		unsigned char& ucIndicator)
{
	//********************************************************************
	// Declaration
	unsigned int uiLengthOfResponseData = 0;

	//********************************************************************
	// RequestData
	m_ucRequestData[0] = 0x28;	// Command

	//********************************************************************
	// CommandExecute
	int iBack = Apdu(m_ucRequestData, 1, m_ucResponseData, 256, uiLengthOfResponseData);
	if(iBack)
		return iBack;

	//********************************************************************
	// ResponseData
	if(uiLengthOfResponseData > 0)
	{
		ucIndicator = m_ucResponseData[0];
	}

	return iBack;
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
