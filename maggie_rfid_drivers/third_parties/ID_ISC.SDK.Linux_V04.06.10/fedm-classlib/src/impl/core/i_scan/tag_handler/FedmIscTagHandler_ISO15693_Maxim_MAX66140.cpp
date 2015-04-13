/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO15693_Maxim_MAX66140.cpp    |
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
					
Author         		:	Marcel Raner
Begin        		:	20.09.2010

Version       		:	03.03.02 / 29.04.2011 / M. Raner, M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID i-scan® HF transponder ISO15693 of manufacturer MAXIM

NOTE				:	this class supports selected, addressed and Nonaddressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)

#include "FedmIscTagHandler_ISO15693_Maxim_MAX66140.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISOTabItem.h"
#include "../FEDM_ISCReaderID.h"
#include "../FEDM_ISC.h"
#include "../../FEDM_Functions.h"
#include "feisc.h"


#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif

// constructor
FedmIscTagHandler_ISO15693_Maxim_MAX66140::FedmIscTagHandler_ISO15693_Maxim_MAX66140(
	FEDM_ISCReader* pReader,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO15693_Maxim(pReader, FedmIscTagHandler::TYPE_ISO15693_MAXIM_MAX66140, pTabItem)
{
	m_sTagName = "ISO 15693 : ";
	m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_MAXIM);
	m_sTagName += " MAX66140";
	FedmIscTagHandler_ISO15693_Maxim_MAX66140::UpdateChallenge();
}

FedmIscTagHandler_ISO15693_Maxim_MAX66140::FedmIscTagHandler_ISO15693_Maxim_MAX66140(
	FEDM_ISCReader* pReader,
	unsigned int uiTagHandlerType,
	FEDM_ISOTabItem* pTabItem )
	: FedmIscTagHandler_ISO15693_Maxim(pReader, uiTagHandlerType, pTabItem)
{
	m_sTagName = "ISO 15693 : ";
	m_sTagName += FEDM_TabItem::GetISO15693Manufacturer(FEDM_ISC_ISO_MFR_MAXIM);
	m_sTagName += " MAX66140";
	FedmIscTagHandler_ISO15693_Maxim_MAX66140::UpdateChallenge();
}

// destructor
FedmIscTagHandler_ISO15693_Maxim_MAX66140::~FedmIscTagHandler_ISO15693_Maxim_MAX66140()
{
}

// set all data members to 0
/*void Init()
{
}*/

/***************************************************************************
  Begin        		:	23.09.2010 / M. Raner

  Version       	:	03.02.05 / 23.09.2010 / M. Raner

  Function			:	Read Memory Page (use ReadSingleBlock())
  
  Parameters		:	unsigned char	ucRequestFlags		- [in]	request flags byte
						unsigned char	ucPageNumber		- [in]	number of page, which should be read
						unsigned char&	ucResponseFlags		- [out] 
						unsigned char*	pucMemoryPageData	- [out]	32byte memory page data

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_Maxim_MAX66140::GetMemoryPageData(
	unsigned char ucRequestFlags, 
	unsigned char ucPageNumber, 
	unsigned char& ucResponseFlags,
	unsigned char* pucMemoryPageData)
{
	FEDM_CHK3(pucMemoryPageData);

	int iBack = 0;
	unsigned char ucSecurityStatus; //temporary
	unsigned char pucMemoryData[8]; //temporary

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
		iBack = FedmIscTagHandler_ISO15693_Maxim_MAX66140::ReadSingleBlock(ucRequestFlags, ucPageNumber * 4 + iCnt, ucResponseFlags, ucSecurityStatus, pucMemoryData);
		if(iBack == 0x95)
		{
			return iBack;
		}
		else if(iBack == 0)
		{
			if(ucResponseFlags)
				return 0;

			for(int iCnt2 = 0; iCnt2 < 8; iCnt2++)
			{
				pucMemoryPageData[iCnt*8+iCnt2] = pucMemoryData[iCnt2];
			}
		}
	}

	return 0;
}

/***************************************************************************
  Begin        		:	23.09.2010 / M. Raner

  Version       	:	03.02.05 / 23.09.2010 / M. Raner

  Function			:	computes PageMAC
  
  Parameters		:	unsigned char* pucSecret			- [in]	request flags byte
						unsigned char* pucMemoryPageData	- [in]	32byte memory page data
						unsigned char ucPageNumber			- [in]	number of page involved in computation
						unsigned char* pucUID				- [in]	uid value of tag
						unsigned char* pucMAC				- [out]	computed page MAC

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_Maxim_MAX66140::UserComputePageMAC(
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

		buffer[36] = FedmIscTagHandler_ISO15693_Maxim_MAX66140::ucChallenge[0];
		buffer[37] = FedmIscTagHandler_ISO15693_Maxim_MAX66140::ucChallenge[1];

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

		buffer[52] = FedmIscTagHandler_ISO15693_Maxim_MAX66140::ucChallenge[2];
		buffer[53] = FedmIscTagHandler_ISO15693_Maxim_MAX66140::ucChallenge[3];
		buffer[54] = FedmIscTagHandler_ISO15693_Maxim_MAX66140::ucChallenge[4];

		buffer[55] = 0x80;
		
		for(iCnt = 56; iCnt < 62; iCnt++)
		{
			buffer[iCnt] = 0x00;
		}

		buffer[62] = 0x01;
		buffer[63] = 0xB8;
	}

	unsigned int edcba[5];
	FedmIscTagHandler_ISO15693_Maxim_MAX66140::ComputeMAC(buffer,edcba);

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
  Begin        		:	23.09.2010 / M. Raner

  Version       	:	03.02.05 / 23.09.2010 / M. Raner

  Function			:	computes a new Secret Value
  
  Parameters		:	unsigned char* pucSecret			- [in]	old secret value
						unsigned char* pucMemoryPageData	- [in]	memory page must match memory page number in GenerateSecret()
						unsigned char* pucPartialSecret		- [in]	this value must match partial secret value in GenerateSecret()
						unsigned char* pucNewSecret			- [out]	computed new secret

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int	FedmIscTagHandler_ISO15693_Maxim_MAX66140::UserComputeSecret(
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
	FedmIscTagHandler_ISO15693_Maxim_MAX66140::ComputeMAC(buffer,edcba);

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
  Begin        		:	24.09.2010 / M. Raner

  Version       	:	03.02.05 / 24.09.2010 / M. Raner

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
int FedmIscTagHandler_ISO15693_Maxim_MAX66140::UserComputeCopyBufferMAC(
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
	FedmIscTagHandler_ISO15693_Maxim_MAX66140::ComputeMAC(buffer,edcba);

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
  Begin        		:	22.09.2010 / M. Raner

  Version       	:	03.02.05 / 22.09.2010 / M. Raner

  Function			:	computes MAC for UserCompute... functions
  
  Parameters		:	unsigned char* ucM		- [in]	input data of sixteen 32-bit Words
						unsigned int* uiMAC		- [out]	computed MAC

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_Maxim_MAX66140::ComputeMAC(
	unsigned char* ucM, 
	unsigned int* uiMAC)
{
	FEDM_CHK3(ucM);
	FEDM_CHK3(uiMAC);

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
            uiW[iCnt2] = (unsigned int)ucM[iCnt2 * 4 + (iCnt * 64)] << 24 | (unsigned int)ucM[iCnt2 * 4 + 1 + (iCnt * 64)] << 16 | (unsigned int)ucM[iCnt2 * 4 + 2 + (iCnt * 64)] << 8 | (unsigned int)ucM[iCnt2 * 4 + 3 + (iCnt * 64)];
        }

        /* Expand from 16 to 80 words */
        for (iCnt2 = 16; iCnt2 < 80; iCnt2++)
        {
			uiW[iCnt2] = (((uiW[iCnt2 - 3] ^ uiW[iCnt2 - 8] ^ uiW[iCnt2 - 14] ^ uiW[iCnt2 - 16]) << 1) | ((uiW[iCnt2 - 3] ^ uiW[iCnt2 - 8] ^ uiW[iCnt2 - 14] ^ uiW[iCnt2 - 16]) >> (32 - 1)));
        }

        for (int iCnt2 = 0; iCnt2 < 80; iCnt2++)
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

	uiMAC[0] = uiE;
	uiMAC[1] = uiD;
	uiMAC[2] = uiC;
	uiMAC[3] = uiB;
	uiMAC[4] = uiA;

	return 0;
}

/***************************************************************************
  Begin        		:	24.09.2010 / M. Raner

  Version       	:	03.02.05 / 24.09.2010 / M. Raner

  Function			:	generates new random challenge
  
  Parameters		:	-

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_Maxim_MAX66140::UpdateChallenge()
{
	for(int iCnt = 0; iCnt < 5; iCnt++)
	{
		ucChallenge[iCnt] = (unsigned char)rand();
	}
	return 0;
}

/***************************************************************************
  Begin        		:	21.09.2010 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xBF][0xA0] GenerateSecret
  
  Parameters		:	unsigned char ucRequestFlags	- [in]	request flags byte
						unsigned char ucControlByte		- [in]	control code (load, compute, locking)
						unsigned char* pucPartialSecret	- [in]	secret to be load, used in computation
						unsigned char& ucResponseFlags	- [out]	response flags(if 0x00 then command execution was successful)
						unsigned char& ucPageCRCStatus	- [out]	status code of crc check

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_Maxim_MAX66140::GenerateSecret( 
	unsigned char ucRequestFlags,
	unsigned char ucControlByte,
	unsigned char* pucPartialSecret,
	unsigned char& ucResponseFlags,
	unsigned char& ucPageCRCStatus)
{
	FEDM_CHK3(pucPartialSecret);

	//******************************************************************************
	// ParameterDescription

		/*
		request flags(size: 1byte)
			bit:	function:				info:
			8		no function				forced to 0
			7		no function				forced to 0
			6		addressed flag			addressed = 1, nonaddressed or selected = 0
			5		selected flag			selected = 1, (non)addressed = 0
			4		no function				forced to 0
			3		no function				forced to 0
			2		datarate flag			low datarate = 0, high datarate = 1
			1		subcarrier flag			single subcarrier = 0, two subcarriers = 1		

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


	//Check if request flags are addressed and selected
	if(((ucRequestFlags & 0x30) == 0x30))
	{
		//Error: Flags = Addressed and Selected
		return FEDM_ERROR_PARAMETER;
	}

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


	//******************************************************************************	
	// Declaration
	int iLengthOfRequestData = 14;
	if((ucRequestFlags & 0x20) == 0x20)
	{
		iLengthOfRequestData += 8;	//Addressed mode, length with uid
	}

	int iLengthOfResponseData;


	//******************************************************************************
	// RequestData
	if(ucControlByte == 0xF0)
	{
		m_ucRequestData[0] = 0x02;
		m_ucRequestData[1] = 0x12;
	}
	else if((ucControlByte & 0x05) == 0x05)
	{
		m_ucRequestData[0] = 0x02;
		m_ucRequestData[1] = 0x24;
	}
	else
	{
		m_ucRequestData[0] = 0x02;	//command response delay time = 0x201f * 590ns = 320.9us
		m_ucRequestData[1] = 0x1f;
	}

	m_ucRequestData[2] = ucRequestFlags & 0x33;	//important request flags
	m_ucRequestData[3] = 0xA0;					//command Code 
	m_ucRequestData[4] = 0x2B;					//MFG Code

	//if addressed mode then uid
	if((ucRequestFlags & 0x20) == 0x20)
	{
		for(int iCnt = 0; iCnt < 8; iCnt++)
		{
			m_ucRequestData[12 - iCnt] = m_pTabItem->m_ucSnr[iCnt];	//uid
		}
	}

	//control byte
	m_ucRequestData[iLengthOfRequestData - 9] = ucControlByte;

	//partial secret
	for(int iCnt = 0; iCnt < 8; iCnt++)
	{
		m_ucRequestData[iLengthOfRequestData - 8 + iCnt] = pucPartialSecret[iCnt];	//new block data
	}


	//******************************************************************************
	// CommandExecute
	//execute command
	int iBack = FEISC_0xBF_ISOTranspCmd(	m_pReader->GetReaderHnd(), 
											m_pReader->GetBusAddress(), 
											0x02, 
											0x20, 
											m_ucRequestData, 
											iLengthOfRequestData, 
											m_ucResponseData, 
											&iLengthOfResponseData, 
											0);
	
	if(iBack == 0x95)
	{
		m_ucISO15693ErrorCode = m_ucResponseData[0];
	}
	else if(iBack == 0)
	{
		//******************************************************************************
		// ResponseData
		ucResponseFlags = m_ucResponseData[0];
		if(ucResponseFlags == 0)
		{
			ucPageCRCStatus = m_ucResponseData[1];
		}
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	20.09.2010 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xBF][0xA1] WriteBuffer
  
  Parameters		:	unsigned char	ucRequestFlags		- [in]	request flags byte
						unsigned char	ucBlockNumber		- [in]	number of block(0x00 to 0x11)
						unsigned char*	pucNewBlockData		- [in]	New block-data
						unsigned char&	ucResponseFlags		- [out]	response flags(if 0x00 then command execution was successful)

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_Maxim_MAX66140::WriteBuffer(
	unsigned char ucRequestFlags,
	unsigned char ucBlockNumber,
	unsigned char* pucNewBlockData,
	unsigned char& ucResponseFlags)
{
	//******************************************************************************
	// ParameterDescription

		/*
			RequestFlags(size 1byte, important flags mask for WriteBuffer command 0x33)
				bits 8 and 4 have no function. They are both 0
				bit 7: option flag - used with block read commands to include the block security status in the response. In this case 0
				bit 6: specifies Addressed State. If this and bit5 are both 0 then state is non addressed.
				bit 5: specifies Selected State. If this and bit6 are both 0 then state is non addressed.
				bit 3: inventory flag - used with inventory Command only. For all other commands this bit must be 0.
				bit 2: datarate flag - low datarate = 0, high datarate = 1
				bit 1: subcarrier flag - single subcarrier = 0, two subcarriers = 1		
		*/


	//******************************************************************************
	// ParameterTesting
	//Test Pointer for NULL
	FEDM_CHK3(pucNewBlockData);

	if(((ucRequestFlags & 0x20) == 0x20) && ((ucRequestFlags & 0x10) == 0x10))
	{
		//Error: Flags = Addressed and Selected
		return FEDM_ERROR_PARAMETER;
	}


	//******************************************************************************
	// Declaration
	int iLengthOfRequestData = 14;
	if((ucRequestFlags & 0x20) == 0x20)
	{
		iLengthOfRequestData += 8;	//Addressed mode, length with uid
	}

	int iLengthOfResponseData;
	
	//******************************************************************************
	// RequestData
	m_ucRequestData[0] = 0x02;	//command response delay time = 0x201f * 590ns = 320.9us
	m_ucRequestData[1] = 0x1f;

	m_ucRequestData[2] = ucRequestFlags & 0x33;	//important request flags
	m_ucRequestData[3] = 0xA1;					//command Code 
	m_ucRequestData[4] = 0x2B;					//MFG Code

	if(iLengthOfRequestData == 22)
	{
		for(int iCnt = 0; iCnt < 8; iCnt++)
		{
			m_ucRequestData[12 - iCnt] = m_pTabItem->m_ucSnr[iCnt];	//uid
		}
		m_ucRequestData[13] = ucBlockNumber;			//block number
	}
	else
	{
		m_ucRequestData[5] = ucBlockNumber;			//block number
	}

	for(int iCnt = 0; iCnt < 8; iCnt++)
	{
		m_ucRequestData[iLengthOfRequestData - 8 + iCnt] = pucNewBlockData[iCnt];	//new block data
	}


	//******************************************************************************
	// CommandExecute
	int iBack = FEISC_0xBF_ISOTranspCmd(	m_pReader->GetReaderHnd(), 
											m_pReader->GetBusAddress(), 
											0x02, 
											0x18, 
											m_ucRequestData, 
											iLengthOfRequestData, 
											m_ucResponseData, 
											&iLengthOfResponseData, 
											0);

	if(iBack == 0x95)
	{
		m_ucISO15693ErrorCode = m_ucResponseData[0];
	}
	else if(iBack == 0)
	{
		//******************************************************************************
		// ResponseData
		ucResponseFlags = m_ucResponseData[0];	//response flags
		if(ucResponseFlags == 0)
		{
			if(ucResponseFlags == 0x10)
			{
				//0x10 invalid block number
				return FEDM_ERROR_UNKNOWN_MEM_ID;
			}
		}
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	21.09.2010 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xBF][0xA2] ReadBuffer
  
  Parameters		:	unsigned char	ucRequestFlags			- [in]	request flags byte
						unsigned char&	ucResponseFlags			- [out]	response flags(if 0x00 then command execution was successful)
						unsigned char&	ucBlockNumber			- [out]	number of block
						unsigned char*	pucReadWriteBufferData	- [out]	data of read/write Buffer

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_Maxim_MAX66140::ReadBuffer(
	unsigned char ucRequestFlags,
	unsigned char& ucResponseFlags,
	unsigned char& ucBlockNumber,
	unsigned char* pucReadWriteBufferData)
{
	//******************************************************************************
	// ParameterDescription

		/*
			RequestFlags(size 1byte, important flags mask for ReadBuffer command 0x33)
				bits 8 and 4 have no function. They are both 0
				bit 7: option flag - used with block read commands to include the block security status in the response. In this case 0
				bit 6: specifies Addressed State. If this and bit5 are both 0 then state is non addressed.
				bit 5: specifies Selected State. If this and bit6 are both 0 then state is non addressed.
				bit 3: inventory flag - used with inventory Command only. For all other commands this bit must be 0.
				bit 2: datarate flag - low datarate = 0, high datarate = 1
				bit 1: subcarrier flag - single subcarrier = 0, two subcarriers = 1		
		*/

	//******************************************************************************
	// ParameterTesting
	//Test Pointer for NULL
	FEDM_CHK3(pucReadWriteBufferData);

	if(((ucRequestFlags & 0x20) == 0x20) && ((ucRequestFlags & 0x10) == 0x10))
	{
		//Error: Flags = Addressed and Selected
		return FEDM_ERROR_PARAMETER;
	}


	//******************************************************************************
	// Declaration
	int iLengthOfRequestData = 5;
	if((ucRequestFlags & 0x20) == 0x20)
	{
		iLengthOfRequestData += 8;	//Addressed mode, length with uid
	}

	int iLengthOfResponseData;


	//******************************************************************************
	// RequestData
	m_ucRequestData[0] = 0x02;	//command response delay time = 0x201f * 590ns = 320.9us
	m_ucRequestData[1] = 0x1f;

	m_ucRequestData[2] = ucRequestFlags & 0x33;	//important request flags
	m_ucRequestData[3] = 0xA2;					//command Code 
	m_ucRequestData[4] = 0x2B;					//MFG Code

	if(iLengthOfRequestData == 13)
	{
		for(int iCnt = 0; iCnt < 8; iCnt++)
		{
			m_ucRequestData[12 - iCnt] = m_pTabItem->m_ucSnr[iCnt];	//uid
		}
	}

	//******************************************************************************
	// CommandExecute
	int iBack = FEISC_0xBF_ISOTranspCmd(	m_pReader->GetReaderHnd(), 
											m_pReader->GetBusAddress(), 
											0x02, 
											0x60, 
											m_ucRequestData, 
											iLengthOfRequestData, 
											m_ucResponseData, 
											&iLengthOfResponseData, 
											0);
	

	if(iBack == 0x95)
	{
		m_ucISO15693ErrorCode = m_ucResponseData[0];
	}
	else if(iBack == 0)
	{
		//******************************************************************************
		// ResponseData
		ucResponseFlags = m_ucResponseData[0];	//response flags
		if(ucResponseFlags == 0)
		{
			if(ucResponseFlags == 0xA3)
			{
				//0xA3 no valid data in read/write buffer
				return FEDM_ERROR_NO_DATA;
			}
			ucBlockNumber = m_ucResponseData[1];	//block number
			for(int iCnt = 0; iCnt < 8; iCnt++)
			{
				pucReadWriteBufferData[iCnt] = m_ucResponseData[2+iCnt];	//read write buffer
			}
		}
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	22.09.2010 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xBF][0xA3] CopyBuffer
  
  Parameters		:	unsigned char ucRequestFlags	- [in]	request flags byte
						unsigned char ucBlockNumber		- [in]	number of block(0x00 to 0x12)
						unsigned char* pucMAC			- [in]	CopyBufferMAC from UserComputeCopyBufferMAC()
						unsigned char& ucResponseFlags	- [out]	response flags(if 0x00 then command execution was successful)
						unsigned char& ucPageCRCStatus	- [out]	status code of crc check

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_Maxim_MAX66140::CopyBuffer(
	unsigned char ucRequestFlags,
	unsigned char ucBlockNumber,
	unsigned char* pucMAC,
	unsigned char& ucResponseFlags,
	unsigned char& ucPageCRCStatus)
{
	//******************************************************************************
	// ParameterTesting
	//Test Pointer for NULL
	FEDM_CHK3(pucMAC);

	//Check if request flags are addressed and selected
	if(((ucRequestFlags & 0x30) == 0x30))
	{
		//Error: Flags = Addressed and Selected
		return FEDM_ERROR_PARAMETER;
	}

	//Check BlockNumber for valid values
	if(!(ucBlockNumber >= 0x00 && ucBlockNumber <= 0x11))
	{
		//Error: block number not valid
		return FEDM_ERROR_PARAMETER;
	}


	//******************************************************************************
	// Declaration
	int iLengthOfRequestData = 26;
	if((ucRequestFlags & 0x20) == 0x20)
	{
		iLengthOfRequestData += 8;	//Addressed mode, length with uid
	}

	int iLengthOfResponseData = 0;


	//******************************************************************************
	// RequestData
	m_ucRequestData[0] = 0x02;	//command response delay time = 0x201f * 590ns = 320.9us
	m_ucRequestData[1] = 0x1f;

	m_ucRequestData[2] = ucRequestFlags & 0x33;	//important request flags
	m_ucRequestData[3] = 0xA3;					//command Code 
	m_ucRequestData[4] = 0x2B;					//MFG Code

	//if addressed mode then uid
	if((ucRequestFlags & 0x20) == 0x20)
	{
		for(int iCnt = 0; iCnt < 8; iCnt++)
		{
			m_ucRequestData[12 - iCnt] = m_pTabItem->m_ucSnr[iCnt];	//uid
		}
	}

	//block number
	m_ucRequestData[iLengthOfRequestData - 21] = ucBlockNumber;

	//mac
	for(int iCnt = 0; iCnt < 20; iCnt++)
	{
		m_ucRequestData[iLengthOfRequestData - 20 + iCnt] = pucMAC[iCnt];
	}


	//******************************************************************************
	// CommandExecute
	int iBack = FEISC_0xBF_ISOTranspCmd(	m_pReader->GetReaderHnd(), 
											m_pReader->GetBusAddress(), 
											0x02, 
											0x20, 
											m_ucRequestData, 
											iLengthOfRequestData, 
											m_ucResponseData, 
											&iLengthOfResponseData, 
											0);


	if(iBack == 0x95)
	{
		m_ucISO15693ErrorCode = m_ucResponseData[0];
	}
	else if(iBack == 0)
	{
		//******************************************************************************
		// ResponseData
		ucResponseFlags = m_ucResponseData[0];
		if(ucResponseFlags == 0)
		{
			ucPageCRCStatus = m_ucResponseData[1];
		}
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	21.09.2010 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xBF][0x20] ReadSingleBlock
  
  Parameters		:	unsigned char ucRequestFlags		- [in]	request flags byte
						unsigned char ucBlockNumber			- [in]	number of block(0x00 to 0x12)
						unsigned char& ucResponseFlags		- [out]	response flags(if 0x00 then command execution was successful)
						unsigned char& ucSecurityStatus		- [out]	security status code
						unsigned char* pucMemoryData		- [out]	readed memory data

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_Maxim_MAX66140::ReadSingleBlock(
	unsigned char ucRequestFlags,
	unsigned char ucBlockNumber,
	unsigned char& ucResponseFlags,
	unsigned char& ucSecurityStatus, /* <<<<<<<<<<<< Optional */
	unsigned char* pucMemoryData)
{
	//******************************************************************************
	// ParameterTesting
	//Test Pointer for NULL
	FEDM_CHK3(pucMemoryData);

	//Check if request flags are addressed and selected
	if(((ucRequestFlags & 0x30) == 0x30))
	{
		//Error: Flags = Addressed and Selected
		return FEDM_ERROR_PARAMETER;
	}

	//Check BlockNumber for valid values
	if(!(ucBlockNumber >= 0x00 && ucBlockNumber <= 0x12))
	{
		//Error: block number not valid
		return FEDM_ERROR_PARAMETER;
	}


	//******************************************************************************
	// Declaration
	int iLengthOfRequestData = 5;
	if((ucRequestFlags & 0x20) == 0x20)
	{
		iLengthOfRequestData += 8;	//Addressed mode, length with uid
	}

	int iLengthOfResponseData = 0;
	int iResponseDataOffset = 0;


	//******************************************************************************
	// RequestData
	m_ucRequestData[0] = 0x02;	//command response delay time = 0x201f * 590ns = 320.9us
	m_ucRequestData[1] = 0x1f;

	m_ucRequestData[2] = ucRequestFlags & 0x73;	//important request flags
	m_ucRequestData[3] = 0x20;					//command Code 

	//if addressed mode then uid
	if((ucRequestFlags & 0x20) == 0x20)
	{
		for(int iCnt = 0; iCnt < 8; iCnt++)
		{
			m_ucRequestData[11 - iCnt] = m_pTabItem->m_ucSnr[iCnt];	//uid
		}
	}

	//block number
	m_ucRequestData[iLengthOfRequestData - 1] = ucBlockNumber;


	//******************************************************************************
	// CommandExecute
	int iBack = FEISC_0xBF_ISOTranspCmd(	m_pReader->GetReaderHnd(), 
											m_pReader->GetBusAddress(), 
											0x02, 
											0x58, 
											m_ucRequestData, 
											iLengthOfRequestData, 
											m_ucResponseData, 
											&iLengthOfResponseData, 
											0);


	if(iBack == 0x95)
	{
		m_ucISO15693ErrorCode = m_ucResponseData[0];
	}
	else if(iBack == 0)
	{
		//******************************************************************************
		// ResponseData
		ucResponseFlags = m_ucResponseData[0];
		if(ucResponseFlags == 0)
		{
			if((ucRequestFlags & 0x40) == 0x40)
			{
				ucSecurityStatus = m_ucResponseData[1];
				iResponseDataOffset++;
			}
			for(int iCnt = 0; iCnt < 8; iCnt++)
			{
				pucMemoryData[iCnt] = m_ucResponseData[1 + iResponseDataOffset +iCnt];
			}
		}
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	21.09.2010 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xBF][0xA4] CustomReadBlock
  
  Parameters		:	unsigned char ucRequestFlags		- [in]	request flags byte
						unsigned char ucBlockNumber			- [in]	number of block(0x00 to 0x12)
						unsigned char& ucResponseFlags		- [out]	response flags(if 0x00 then command execution was successful)
						unsigned char& ucSecurityStatus		- [out]	security status code
						unsigned char* pucMemoryData		- [out]	readed memory data
						unsigned char* pucIntegrityBytes	- [out]	integrity bytes(write counter) of datablock

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_Maxim_MAX66140::CustomReadBlock(
	unsigned char ucRequestFlags,
	unsigned char ucBlockNumber,
	unsigned char& ucResponseFlags,
	unsigned char& ucSecurityStatus, /* <<<<<<<<<<<< Optional */
	unsigned char* pucMemoryData,
	unsigned char* pucIntegrityBytes)
{
	//******************************************************************************
	// ParameterTesting
	//Test Pointer for NULL
	FEDM_CHK3(pucMemoryData);
	FEDM_CHK3(pucIntegrityBytes);

	//Check if request flags are addressed and selected
	if(((ucRequestFlags & 0x30) == 0x30))
	{
		//Error: Flags = Addressed and Selected
		return FEDM_ERROR_PARAMETER;
	}

	//Check BlockNumber for valid values
	if(!(ucBlockNumber >= 0x00 && ucBlockNumber <= 0x12))
	{
		//Error: block number not valid
		return FEDM_ERROR_PARAMETER;
	}


	//******************************************************************************
	// Declaration
	int iLengthOfRequestData = 6;
	if((ucRequestFlags & 0x20) == 0x20)
	{
		iLengthOfRequestData += 8;	//Addressed mode, length with uid
	}

	int iLengthOfResponseData = 0;
	int iResponseDataOffset = 0;


	//******************************************************************************
	// RequestData
	m_ucRequestData[0] = 0x02;	//command response delay time = 0x201f * 590ns = 320.9us
	m_ucRequestData[1] = 0x1f;

	m_ucRequestData[2] = ucRequestFlags & 0x73;	//important request flags
	m_ucRequestData[3] = 0xA4;					//command Code 
	m_ucRequestData[4] = 0x2B;					//MFG Code

	//if addressed mode then uid
	if((ucRequestFlags & 0x20) == 0x20)
	{
		for(int iCnt = 0; iCnt < 8; iCnt++)
		{
			m_ucRequestData[12 - iCnt] = m_pTabItem->m_ucSnr[iCnt];	//uid
		}
	}

	//block number
	m_ucRequestData[iLengthOfRequestData - 1] = ucBlockNumber;


	//******************************************************************************
	// CommandExecute
	int iBack = FEISC_0xBF_ISOTranspCmd(	m_pReader->GetReaderHnd(), 
											m_pReader->GetBusAddress(), 
											0x01, 
											0x68, 
											m_ucRequestData, 
											iLengthOfRequestData, 
											m_ucResponseData, 
											&iLengthOfResponseData, 
											0);


	if(iBack == 0x95)
	{
		m_ucISO15693ErrorCode = m_ucResponseData[0];
	}
	else if(iBack == 0)
	{
		//******************************************************************************
		// ResponseData
		ucResponseFlags = m_ucResponseData[0];
		if(ucResponseFlags == 0)
		{
			if((ucRequestFlags & 0x40) == 0x40)
			{
				ucSecurityStatus = m_ucResponseData[1];
				iResponseDataOffset++;
			}
			for(int iCnt = 0; iCnt < 8; iCnt++)
			{
				pucMemoryData[iCnt] = m_ucResponseData[1 + iResponseDataOffset +iCnt];
			}
			pucIntegrityBytes[0] = m_ucResponseData[9 + iResponseDataOffset];
			pucIntegrityBytes[1] = m_ucResponseData[10 + iResponseDataOffset];
		}
	}

	return iBack;
}

/***************************************************************************
  Begin        		:	22.09.2010 / M. Raner

  Version       	:	03.03.02 / 28.04.2011 / M. Raner, M. Hultsch

  Function			:	[0xBF][0xA5] ComputePageMAC
  
  Parameters		:	unsigned char ucRequestFlags		- [in]	request flags byte
						unsigned char ucPageNumber			- [in]	number of page will involve in computation
						unsigned char& ucResponseFlags		- [out]	response flags(if 0x00 then command execution was successful)
						unsigned char& ucPageCRCStatus		- [out]	status code of crc check
						unsigned char* pucMAC				- [out]	from transponder computed PageMAC

  Return value		:	0				- if tramsmission was successful
						status byte (>1)- if the reader signals a problem
						error code (<0)	- if something goes wrong
***************************************************************************/
int FedmIscTagHandler_ISO15693_Maxim_MAX66140::ComputePageMAC(
	unsigned char ucRequestFlags,
	unsigned char ucPageNumber,
	unsigned char& ucResponseFlags,
	unsigned char& ucPageCRCStatus,
	unsigned char* pucMAC)
{
	//******************************************************************************
	// ParameterTesting
	//Test Pointer for NULL
	FEDM_CHK3(pucMAC);

	int iCnt = 0;

	//Check if request flags are addressed and selected
	if(((ucRequestFlags & 0x30) == 0x30))
	{
		//Error: Flags = Addressed and Selected
		return FEDM_ERROR_PARAMETER;
	}

	//Check PageNumber for valid values
	if(!(ucPageNumber >= 0x00 && ucPageNumber <= 0x04))
	{
		//Error: block number not valid
		return FEDM_ERROR_PARAMETER;
	}


	//******************************************************************************
	// Declaration
	int iLengthOfRequestData = 11;
	if((ucRequestFlags & 0x20) == 0x20)
	{
		iLengthOfRequestData += 8;	//Addressed mode, length with uid
	}

	int iLengthOfResponseData = 0;

	//******************************************************************************
	// RequestData
	m_ucRequestData[0] = 0x02;	//command response delay time = 0x201f * 590ns = 320.9us
	//if selected then change response time
	if(m_pTabItem->m_bIsSelected)
	{
		m_ucRequestData[1] = 0x41;
	}
	else
	{
		m_ucRequestData[1] = 0x45;
	}

	m_ucRequestData[2] = ucRequestFlags & 0x33;	//important request flags
	m_ucRequestData[3] = 0xA5;					//command Code 
	m_ucRequestData[4] = 0x2B;					//MFG Code


	//if addressed mode then uid
	if((ucRequestFlags & 0x20) == 0x20)
	{
		for(iCnt = 0; iCnt < 8; iCnt++)
		{
			m_ucRequestData[12 - iCnt] = m_pTabItem->m_ucSnr[iCnt];	//uid
		}
	}

	//page number
	m_ucRequestData[iLengthOfRequestData - 6] = ucPageNumber;

	//challenge
	for(iCnt = 0; iCnt < 5; iCnt++)
	{
		m_ucRequestData[iLengthOfRequestData - 5 + iCnt] = ucChallenge[iCnt];
	}

	//******************************************************************************
	// CommandExecute
	int iBack = FEISC_0xBF_ISOTranspCmd(	m_pReader->GetReaderHnd(), 
											m_pReader->GetBusAddress(), 
											0x02, 
											0xC0, 
											m_ucRequestData, 
											iLengthOfRequestData, 
											m_ucResponseData, 
											&iLengthOfResponseData, 
											0);


	if(iBack == 0x95)
	{
		m_ucISO15693ErrorCode = m_ucResponseData[0];
	}
	else if(iBack == 0)
	{
		//******************************************************************************
		// ResponseData
		ucResponseFlags = m_ucResponseData[0];
		if(ucResponseFlags == 0)
		{
			ucPageCRCStatus = m_ucResponseData[1];
			for(iCnt = 0; iCnt < 20; iCnt++)
			{
				pucMAC[iCnt] = m_ucResponseData[2+iCnt];
			}
		}
	}

	return iBack;
}

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)
