/*-------------------------------------------------------
|                                                       |
|				FedmIscReport_ReaderInfo.cpp            |
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
Begin        		:	17.06.2009

Version       		:	04.06.08 / 13.10.2014 / M. Hultsch
						- additional LAN link information with mode=0x50

						04.05.05 / 29.11.2013 / M. Hultsch
						- new mode 0x09 for WLAN-Firmware

						04.05.03 / 15.07.2013 / M. Hultsch
						- OBID myAXXESS onTop added

						04.02.06 / 23.07.2012 / M. Hultsch
						- new mode 0x08 (with firmware supported tag drivers)

						04.00.09 / 01.02.2012 / M. Hultsch
						- mode 0x04: modification for CPR-Reader

						03.03.00 / 01.02.2011 / M. Hultsch
						- ID ISC.LR2500-A added

						03.02.04 / 17.08.2010 / M. Hultsch
						- ID CPR30.xx added

						03.01.01 / 23.02.2010 / M. Hultsch
						- new mode: 0x61

						03.00.19 / 12.01.2010 / M. Hultsch
						- new modes: 0x07, 0x12

						03.00.14 / 15.10.2009 / M. Hultsch
						- mode 0x01 with additional data

						03.00.13 / 13.10.2009 / M. Hultsch
						- ID MAX50.xx added

						03.00.12 / 22.09.2009 / M. Hultsch

Operation Systems	:	independent

Function			:	report class for [0x66] Reader Info


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#include "FedmIscReport_ReaderInfo.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISCReaderID.h"
#include "../../FEDM_Functions.h"
#if !defined(_FEDM_NO_PD_SUPPORT)
	#include "../peripheral_devices/FedmIscPeripheralDevice.h"
#endif


FedmIscReport_ReaderInfo::FedmIscReport_ReaderInfo(FEDM_ISCReader* pReader)
{
	m_pReader = pReader;
}

FedmIscReport_ReaderInfo::~FedmIscReport_ReaderInfo()
{
}

string FedmIscReport_ReaderInfo::BuildReport(unsigned char ucMode)
{
#if _MSC_VER <= 1200
	m_sReport.erase(m_sReport.begin(), m_sReport.end());
#else
	m_sReport.clear();
#endif
	BuildReport(ucMode, m_sReport);
	return m_sReport;
}

int FedmIscReport_ReaderInfo::BuildReport(unsigned char ucMode, string& sReport)
{
	if(m_pReader == NULL)
		return FEDM_ERROR_NULL_POINTER;

	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();

	switch(ucMode)
	{
	case 0x00:
		if(pInfo->bIsMode0x00Read)
			Add0x00InfoText(sReport);		// [0x65] version info of µC
		break;

	case 0x01:
		if(pInfo->bIsMode0x01Read)
			Add0x01InfoText(sReport);		// version info of AC-Controller
		break;
	
	case 0x02:
		if(pInfo->bIsMode0x02Read)
			Add0x02InfoText(sReport);		// version info of USB-Controller
		break;
	
	case 0x03:
		if(pInfo->bIsMode0x03Read)
			Add0x03InfoText(sReport);		// version info of RF-Decoder
		break;
	
	case 0x04:
		if(pInfo->bIsMode0x04Read)
			Add0x04InfoText(sReport);		// version info of additional Reader Functions
		break;
	
	case 0x05:
		if(pInfo->bIsMode0x05Read)
			Add0x05InfoText(sReport);		// version info of RFC Bootloader
		break;
	
	case 0x06:
		if(pInfo->bIsMode0x06Read)
			Add0x06InfoText(sReport);		// information from keyboard unit
		break;
	
	case 0x07:
		if(pInfo->bIsMode0x07Read)
			Add0x07InfoText(sReport);		// information from CPRIO unit
		break;
	
	case 0x08:
		if(pInfo->bIsMode0x08Read)
			Add0x08InfoText(sReport);		// version info of Hardware
		break;
	
	case 0x09:
		if(pInfo->bIsMode0x09Read)
			Add0x09InfoText(sReport);		// version info of WLAN-Module
		break;
	
	case 0x10:
		if(pInfo->bIsMode0x10Read)
			Add0x10InfoText(sReport);		// version info of Hardware
		break;
	
	case 0x11:
		if(pInfo->bIsMode0x11Read)
			Add0x11InfoText(sReport);		// SAM info
		break;
	
	case 0x12:
		if(pInfo->bIsMode0x12Read)
			Add0x12InfoText(sReport);		// CPU infor of RFC
		break;
	
	case 0x40:
		if(pInfo->bIsMode0x40Read)
			Add0x40InfoText(sReport);		// version info of Hardware
		break;
	
	case 0x41:
		if(pInfo->bIsMode0x41Read)
			Add0x41InfoText(sReport);		// version info of Hardware
		break;
	
	case 0x50:
		if(pInfo->bIsMode0x50Read)
			Add0x50InfoText(sReport);		// LAN: MAC
		break;
	
	case 0x51:
		if(pInfo->bIsMode0x51Read)
			Add0x51InfoText(sReport);		// LAN: IP-Address
		break;
	
	case 0x52:
		if(pInfo->bIsMode0x52Read)
			Add0x52InfoText(sReport);		// LAN: Netmask
		break;
	
	case 0x53:
		if(pInfo->bIsMode0x53Read)
			Add0x53InfoText(sReport);		// LAN: Gateway-Address
		break;
	
	case 0x54:
		if(pInfo->bIsMode0x54Read)
			Add0x54InfoText(sReport);		// WLAN: MAC
		break;
	
	case 0x55:
		if(pInfo->bIsMode0x55Read)
			Add0x55InfoText(sReport);		// WLAN: IP-Address
		break;
	
	case 0x56:
		if(pInfo->bIsMode0x56Read)
			Add0x56InfoText(sReport);		// WLAN: Netmask
		break;
	
	case 0x57:
		if(pInfo->bIsMode0x57Read)
			Add0x57InfoText(sReport);		// WLAN: Gateway-Address
		break;
	
	case 0x60:
		if(pInfo->bIsMode0x60Read)
			Add0x60InfoText(sReport);		// version info of Hardware
		break;
	
	case 0x61:
		if(pInfo->bIsMode0x61Read)
			Add0x61InfoText(sReport);		// external peripheral devices information
		break;
	
	case 0x80:
		if(pInfo->bIsMode0x80Read)
			Add0x80InfoText(sReport);		// device info
		break;
	
	case 0xFF:
		AddAllInfoText(sReport);
		break;

	default:
		break;
	}

	return FEDM_OK;
}


// version info like [0x65] Software Version
void FedmIscReport_ReaderInfo::Add0x00InfoText(string& sReport, bool bSingle)
{
	char cBuffer[64];
	unsigned char ucSWRevHigh = 0;
	unsigned char ucSWRevLow = 0;
	unsigned char ucHWType = 0;
	unsigned char ucSWType = 0;
	unsigned char ucDRev = 0;
	unsigned char ucTrType[2];
	unsigned char ucTrTypeUhf[2];
	unsigned int uiRxBufLen = 0;
	unsigned int uiTxBufLen = 0;
	unsigned char ucReaderClassGen = 0;
	unsigned char ucReaderTypeOfClass = 0;
	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_SOFTVER_D_REV, &ucDRev);
		m_pReader->GetData(FEDM_ISC_TMP_SOFTVER_SW_REV_HIGH, &ucSWRevHigh);
		m_pReader->GetData(FEDM_ISC_TMP_SOFTVER_SW_REV_LOW, &ucSWRevLow);
		m_pReader->GetData(FEDM_ISC_TMP_SOFTVER_HW_TYPE, &ucHWType);
		m_pReader->GetData(FEDM_ISC_TMP_SOFTVER_SW_TYPE, &ucSWType); // reader type, if Bit 7 = 0 or reader class, if Bit 7 = 1
		m_pReader->GetData(FEDM_ISC_TMP_SOFTVER_TR_TYPE, ucTrType, 2);
		m_pReader->GetData(FEDM_ISC_TMP_SOFTVER_RX_BUF, &uiRxBufLen);
		m_pReader->GetData(FEDM_ISC_TMP_SOFTVER_TX_BUF, &uiTxBufLen);
		m_pReader->GetData(FEDM_ISC_TMP_SOFTVER_TR_TYPE_UHF, ucTrTypeUhf, 2);
		m_pReader->GetData(FEDM_ISC_TMP_SOFTVER_READER_CLASS_GEN, &ucReaderClassGen); // only valid, if ucSWType contains a reader class
		m_pReader->GetData(FEDM_ISC_TMP_SOFTVER_READER_TYPE_OF_CLASS, &ucReaderTypeOfClass); // only valid, if ucSWType contains a reader class
	}
	else
	{
		ucSWRevHigh 	= pInfo->ucRfcSwVer[0];
		ucSWRevLow		= pInfo->ucRfcSwVer[1];
		ucDRev			= pInfo->ucRfcDevVer;
		ucHWType		= pInfo->ucRfcHwType;
		ucSWType		= pInfo->ucReaderType; // reader type, if Bit 7 = 0 or reader class, if Bit 7 = 1
		ucTrType[0]		= pInfo->ucRfcTrType[0];
		ucTrType[1]		= pInfo->ucRfcTrType[1];
		uiRxBufLen		= (pInfo->ucRfcRxBufferSize[0]<<8) + pInfo->ucRfcRxBufferSize[1];
		uiTxBufLen		= (pInfo->ucRfcTxBufferSize[0]<<8) + pInfo->ucRfcTxBufferSize[1];
		ucTrTypeUhf[0]	= pInfo->ucRfcUhfTrType[0];
		ucTrTypeUhf[1]	= pInfo->ucRfcUhfTrType[1];
		ucReaderClassGen	= pInfo->ucReaderClassGen; // only valid, if ucSWType contains a reader class
		ucReaderTypeOfClass	= pInfo->ucReaderTypeOfClass; // only valid, if ucSWType contains a reader class
	}

//	sReport += "\n\n";

	sReport += "Firmware Version of RF Communication Controller:";
	sReport += "\r\n";
	if(ucSWType==FEDM_ISC_TYPE_ISCLR200)
		sprintf(cBuffer, "%02d.%02d.%02d", (ucSWRevHigh&0x0F), ucSWRevLow, ucDRev);
	else
		sprintf(cBuffer, "%02d.%02d.%02d", ucSWRevHigh, ucSWRevLow, ucDRev);

	sReport += "\tSoftware Revision:.....";
	sReport += cBuffer;
	if(pInfo->ucReaderType == FEDM_ISC_TYPE_ISCLRU3000)
	{
		if(ucSWRevHigh < 2)
		{
			sReport+= " (unsupported)";
		}
	}
	sReport += "\r\n";
	sprintf(cBuffer, "0x%02X", ucHWType);
	sReport += "\tHardware Type:.........";
	sReport += cBuffer;
	sReport += "\r\n";

	switch(ucSWType)
	{
	case FEDM_ISC_TYPE_CPRM02:
		if(ucHWType & 0x01)
			sReport += "\t   - with SAM socket\r\n";
		else
			sReport += "\t   - without SAM socket\r\n";

		if( (ucHWType & 0x03) == 0x03)
			sReport += "\t   - SAM inserted\r\n";
		else if( (ucHWType & 0x01) && !(ucHWType & 0x02) )
			sReport += "\t   - SAM not inserted\r\n";

		if( (ucHWType & 0xC0) == 0x00)
			sReport += "\t   - Model is -CA\r\n";
		else if( (ucHWType & 0xC0) == 0x40 )
			sReport += "\t   - Model is -BA\r\n";
		else if( (ucHWType & 0xC0) == 0x80 )
			sReport += "\t   - Model is -C\r\n";
		else if( (ucHWType & 0xC0) == 0xC0 )
			sReport += "\t   - Model is -B\r\n";

		break;

	case FEDM_ISC_TYPE_CPR02:
	case FEDM_ISC_TYPE_CPR04_U:
		if(ucHWType & 0x01)
			sReport += "\t   - with SAM socket\r\n";
		else
			sReport += "\t   - without SAM socket\r\n";

		if( (ucHWType & 0x03) == 0x03)
			sReport += "\t   - SAM inserted\r\n";
		else if( (ucHWType & 0x01) && !(ucHWType & 0x02) )
			sReport += "\t   - SAM not inserted\r\n";

		break;

	case FEDM_ISC_TYPE_CPR40_XX_U:
	case FEDM_ISC_TYPE_CPR40_XX:
		if(ucHWType & 0x01)
			sReport += "\t   - with SAM sockets\r\n";
		else
			sReport += "\t   - without SAM sockets\r\n";

		if( (ucHWType & 0x03) == 0x03)
			sReport += "\t   - SAM inserted in socket 1\r\n";
		else if( (ucHWType & 0x01) && !(ucHWType & 0x02) )
			sReport += "\t   - SAM socket 1 is not used\r\n";

		if( (ucHWType & 0x11) == 0x11)
			sReport += "\t   - SAM inserted in socket 2\r\n";
		else if( (ucHWType & 0x01) && !(ucHWType & 0x02) )
			sReport += "\t   - SAM socket 2 is not used\r\n";

		break;
	}

	if(ucSWType & 0x80)
	{
		// rfu
		sReport += "\tReader Class:..........";
		sprintf(cBuffer, "%02d", ucSWType);
		sReport += cBuffer;
		sReport += "\tReader Class Gen:......";
		sprintf(cBuffer, "%02d", ucReaderClassGen);
		sReport += cBuffer;
		sReport += "\tReader Type:...........";
		sprintf(cBuffer, "%02d", ucReaderTypeOfClass);
		sReport += cBuffer;
	}
	else
	{
		sReport += "\tReader Type:...........";
		sprintf(cBuffer, "%02d", ucSWType);
		sReport += cBuffer;
	}
	sReport += " (";
	sReport += m_pReader->GetReaderName();
	sReport += ")";
	sReport += "\r\n";
	if(ucSWType == FEDM_ISC_TYPE_ISCPRHD102)
	{
		sprintf(cBuffer, "0x%02X 0x%02X", ucTrType[0], ucTrType[1]);
		sReport += "\tHF  Transponder Types:.";
		sReport += cBuffer;
		sReport += "\r\n";

		sprintf(cBuffer, "0x%02X 0x%02X", ucTrTypeUhf[0], ucTrTypeUhf[1]);
		sReport += "\tUHF Transponder Types:.";
		sReport += cBuffer;
		sReport += "\r\n";
	}
	else if(ucSWType == FEDM_ISC_TYPE_CPR46_XX || ucSWType == FEDM_ISC_TYPE_CPR47_XX || ucSWType == FEDM_ISC_TYPE_MAX_ONTOP)
	{
		// supported tag drivers are displayed with mode 0x08
	}
	else
	{
		sprintf(cBuffer, "0x%02X 0x%02X", ucTrType[0], ucTrType[1]);
		sReport += "\tTransponder Types:.....";
		sReport += cBuffer;
		sReport += "\r\n";
	}

	sprintf(cBuffer, "%05d", uiRxBufLen);
	sReport += "\tMax. Rx buffer size:...";
	sReport += cBuffer;
	sReport += "\r\n";
	sprintf(cBuffer, "%05d", uiTxBufLen);
	sReport += "\tMax. Tx buffer size:...";
	sReport += cBuffer;
	sReport += "\r\n";
}


void FedmIscReport_ReaderInfo::Add0x01InfoText(string& sReport, bool bSingle)
{
	char cBuffer[64];
	unsigned char ucSWMajor = 0;
	unsigned char ucSWMinor = 0;
	unsigned char ucSWDev = 0;
	unsigned char ucHWType = 0;
	unsigned char ucEmbAppSWMajor = 0;
	unsigned char ucEmbAppSWMinor = 0;
	unsigned char ucEmbAppSWDev = 0;
	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO_SW_MAJOR, &ucSWMajor);
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO_SW_MINOR, &ucSWMinor);
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO_SW_DEV, &ucSWDev);
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO_HW_TYPE, &ucHWType);
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO_SW_MAJOR_EMB_APP, &ucEmbAppSWMajor);
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO_SW_MINOR_EMB_APP, &ucEmbAppSWMinor);
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO_SW_DEV_EMB_APP, &ucEmbAppSWDev);
	}
	else
	{
		ucSWMajor	= pInfo->ucAccSwVer[0];
		ucSWMinor	= pInfo->ucAccSwVer[1];
		ucSWDev		= pInfo->ucAccDevVer;
		ucHWType	= pInfo->ucAccHwType;
		ucEmbAppSWMajor = pInfo->ucAccEmbAppSwVer[0];
		ucEmbAppSWMinor = pInfo->ucAccEmbAppSwVer[1];
		ucEmbAppSWDev	= pInfo->ucAccEmbAppDevVer;
	}

//	sReport += "\n\n";
	sReport += "Firmware Version of Application Communication Controller:";
	sReport += "\r\n";
	sReport += "\tSoftware Revision:.....";
	sprintf(cBuffer, "%02d.%02d.%02d", ucSWMajor, ucSWMinor, ucSWDev);
	sReport += cBuffer;
	if(pInfo->ucReaderType == FEDM_ISC_TYPE_ISCLRU3000)
	{
		if(ucSWMajor < 2)
		{
			sReport+= " (unsupported)";
		}
	}
	sReport += "\r\n";
	sprintf(cBuffer, "%02d (0x%02X)", ucHWType, ucHWType);
	sReport += "\tHardware Type:.........";
	sReport += cBuffer;
	sReport += "\r\n";
	if(ucEmbAppSWMajor != 0 || ucEmbAppSWMinor != 0 || ucEmbAppSWDev != 0)
	{
		sReport += "\tSoftware Revision 2:...";
		sprintf(cBuffer, "%02d.%02d.%02d", ucEmbAppSWMajor, ucEmbAppSWMinor, ucEmbAppSWDev);
		sReport += cBuffer;
		sReport += "\r\n";
	}
}


// version info for USB controller
void FedmIscReport_ReaderInfo::Add0x02InfoText(string& sReport, bool bSingle)
{
	char cBuffer[64];
	unsigned char ucSWRevHigh = 0;
	unsigned char ucSWRevLow = 0;
	unsigned char ucHWType = 0;
	unsigned char ucDRev = 0;
	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO_SW_MAJOR, &ucSWRevHigh);
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO_SW_MINOR, &ucSWRevLow);
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO_SW_DEV, &ucDRev);
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO_HW_TYPE, &ucHWType);
	}
	else
	{
		ucSWRevHigh = pInfo->ucUsbSwVer[0];
		ucSWRevLow	= pInfo->ucUsbSwVer[1];
		ucDRev		= pInfo->ucUsbDevVer;
		ucHWType	= pInfo->ucUsbHwType;
	}


//	sReport += "\n\n";
	sReport += "Firmware Version of USB-Controller:";
	sReport += "\r\n";
	sprintf(cBuffer, "%02d.%02d.%02d", ucSWRevHigh, ucSWRevLow, ucDRev);

	sReport += "\tSoftware Revision:.....";
	sReport += cBuffer;
	sReport += "\r\n";
	sprintf(cBuffer, "%02d", ucHWType);
	sReport += "\tHardware Type:.........";
	sReport += cBuffer;
	sReport += "\r\n";
}


void FedmIscReport_ReaderInfo::Add0x03InfoText(string& sReport, bool bSingle)
{
	char cBuffer[64];
	unsigned char ucDecType[32];
	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();
	unsigned int uiReaderType = m_pReader->GetReaderType();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO, ucDecType, 11);
	}
	else
	{
		memcpy(ucDecType, &pInfo->ucDecoderType[0], 6);
	}

//	sReport += "\n\n";

	switch(uiReaderType)
	{
	case FEDM_ISC_TYPE_ISCLR2000:
	case FEDM_ISC_TYPE_ISCLR2500_B:
	case FEDM_ISC_TYPE_ISCMRU200:
	case FEDM_ISC_TYPE_ISCLRU1000:
	case FEDM_ISC_TYPE_ISCLRU2000:
		sReport += "Firmware Version of FPGA-Controller:";
		sReport += "\r\n";
		sReport += "\tSoftware Revision:.....";
		sprintf(cBuffer, "%d.%d.%d", ucDecType[0], ucDecType[1], ucDecType[2]);
		sReport += cBuffer;
		sReport += "\r\n";
		sReport += "\tFPGA-Type:.............";
		sprintf(cBuffer, "%d", ucDecType[3]);
		sReport += cBuffer;
		sReport += "\r\n";
		break;

	case FEDM_ISC_TYPE_ISCLR2500_A:
	case FEDM_ISC_TYPE_ISCLRU3000:
		sReport += "Firmware Version of FPGA-Controller:";
		sReport += "\r\n";
		sReport += "\tSoftware Revision:.....";
		sprintf(cBuffer, "%d.%d.%c.%d", ucDecType[0], ucDecType[1], ucDecType[2], ucDecType[3]);
		sReport += cBuffer;
		sReport += "\r\n";
		break;

	case FEDM_ISC_TYPE_MAXU1002:
	case FEDM_ISC_TYPE_ISCLRU1002:
		sReport += "RF-Decoder information:";
		sReport += "\r\n";
		sprintf(cBuffer, "\t0x%02X-0x%02X-0x%02X-0x%02X",
					ucDecType[0],
					ucDecType[1],
					ucDecType[2],
					ucDecType[3] );
		sReport += cBuffer;
		sReport += "\r\n";
		break;

	case FEDM_ISC_TYPE_CPRM02:
	case FEDM_ISC_TYPE_CPR02:
	case FEDM_ISC_TYPE_CPR04_U:
		sReport += "RF-Decoder information:";
		sReport += "\r\n";
		sprintf(cBuffer, "\t0x%02X-0x%02X-0x%02X-0x%02X-0x%02X",
					ucDecType[0],
					ucDecType[1],
					ucDecType[2],
					ucDecType[3],
					ucDecType[4] );
		sReport += cBuffer;
		sReport += "\r\n";
		break;

	case FEDM_ISC_TYPE_CPR20_XX:
	case FEDM_ISC_TYPE_CPR40_XX_U:
	case FEDM_ISC_TYPE_CPR40_XX:
	case FEDM_ISC_TYPE_CPR44_XX:
	case FEDM_ISC_TYPE_CPR46_XX:
	case FEDM_ISC_TYPE_CPR47_XX:
	case FEDM_ISC_TYPE_MAX_ONTOP:
		sReport += "RF-Decoder information:";
		sReport += "\r\n";
		sReport += "\tDecoder Information:...";
		sprintf(cBuffer, "0x%02X-0x%02X-0x%02X-0x%02X-0x%02X",
					ucDecType[0],
					ucDecType[1],
					ucDecType[2],
					ucDecType[3],
					ucDecType[4] );
		sReport += cBuffer;
		sReport += "\r\n";

		sReport += "\tSelf Test:.............";
		if(ucDecType[5])
			sReport += "OK";
		else
			sReport += "Not OK";

		sReport += "\r\n";
		break;
	}

}


void FedmIscReport_ReaderInfo::Add0x04InfoText(string& sReport, bool bSingle)
{
	int i=0;
	char cBuffer[64];
	unsigned char ucDecType[32];
	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();
	unsigned int uiReaderType = m_pReader->GetReaderType();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO, ucDecType, 11);
	}
	else
	{
		switch(uiReaderType)
		{
		case FEDM_ISC_TYPE_ISCMU02:
		case FEDM_ISC_TYPE_ISCLR2000:
		case FEDM_ISC_TYPE_ISCLR1002:
		case FEDM_ISC_TYPE_MAXU1002:
		case FEDM_ISC_TYPE_ISCLRU1002:
		case FEDM_ISC_TYPE_ISCLRU2000:
		case FEDM_ISC_TYPE_ISCLRU3000:
			ucDecType[0] = pInfo->ucReserved1;
			memcpy(&ucDecType[1], pInfo->ucFwIdentifierB, 10);
			break;

		case FEDM_ISC_TYPE_CPR20_XX:
		case FEDM_ISC_TYPE_CPR30_XX:
		case FEDM_ISC_TYPE_CPR40_XX_U:
		case FEDM_ISC_TYPE_CPR40_XX:
		case FEDM_ISC_TYPE_CPR44_XX:
		case FEDM_ISC_TYPE_CPR46_XX:
		case FEDM_ISC_TYPE_CPR47_XX:
		case FEDM_ISC_TYPE_CPR50_XX:
		case FEDM_ISC_TYPE_CPR52_XX:
		case FEDM_ISC_TYPE_CPR60_XX:
		// myAXXESS
		case FEDM_ISC_TYPE_MAX50_XX:
		case FEDM_ISC_TYPE_MAX_STANDARD:
		case FEDM_ISC_TYPE_MAX_COMFORT:
		case FEDM_ISC_TYPE_MAX_ONTOP:
			ucDecType[0] = pInfo->ucCprFctTemplate;
			ucDecType[1] = pInfo->ucCprFctList0;
			ucDecType[2] = pInfo->ucCprFctList1;
			ucDecType[3] = pInfo->ucCprFctList2;
			ucDecType[4] = pInfo->ucCprRfu0;
			ucDecType[5] = pInfo->ucCprRfu1;
			ucDecType[6] = pInfo->ucCprRfu2;
			ucDecType[7] = pInfo->ucCprRfu3;
			ucDecType[8] = pInfo->ucCprRfu4;
			memcpy(&ucDecType[9], pInfo->ucFwIdentifierC, 2);
			break;

		case FEDM_ISC_TYPE_CPRM02:
		case FEDM_ISC_TYPE_CPR02:
		case FEDM_ISC_TYPE_CPR04_U:
			ucDecType[0] = pInfo->ucCprFctTemplate;
			ucDecType[1] = pInfo->ucCprFctList;
			break;

		default:
			return;
		}
	}

	switch(uiReaderType)
	{
	case FEDM_ISC_TYPE_ISCMU02:
		sReport += "Additional Firmware Functions:";
		sReport += "\r\n\t";
		for(i=0; i<11; i++)
		{
			sprintf(cBuffer, "0x%02X ", ucDecType[i]);
			sReport += cBuffer;
		}
		sReport += "\r\n";

		switch(ucDecType[10])
		{
		case 0x00:	sReport += "\t   - RFC Standard Firmware\r\n";			break;
		}
		break;

	case FEDM_ISC_TYPE_ISCLR1002:
	case FEDM_ISC_TYPE_MAXU1002:
	case FEDM_ISC_TYPE_ISCLRU1002:
		sReport += "Additional Firmware Functions:";
		sReport += "\r\n\t";
		for(i=0; i<11; i++)
		{
			sprintf(cBuffer, "0x%02X ", ucDecType[i]);
			sReport += cBuffer;
		}
		sReport += "\r\n";
		break;

	case FEDM_ISC_TYPE_ISCLR2000:
	case FEDM_ISC_TYPE_ISCLRU2000:
	case FEDM_ISC_TYPE_ISCLRU3000:
//		sReport += "\n\n";
		sReport += "Additional Firmware Functions:";
		sReport += "\r\n\t";
		for(i=0; i<11; i++)
		{
			sprintf(cBuffer, "0x%02X ", ucDecType[i]);
			sReport += cBuffer;
		}
		sReport += "\r\n";

		if(ucDecType[1] == 0x80)
		{
			sReport += "\tSpecial Firmware\r\n";
		}
		else
		{
			switch(ucDecType[10])
			{
			case 0x00:	sReport += "\t   - RFC Standard Firmware\r\n";			break;
			case 0x01:	sReport += "\t   - RFC Firmware for ST Freedom\r\n";	break;
			case 0x02:	sReport += "\t   - RFC Firmware for NXP\r\n";			break;
			}
		}
		
		switch(ucDecType[2])
		{
		case 0x00:	sReport += "\t   - ACC Standard Firmware\r\n";				break;
		}
		break;

	case FEDM_ISC_TYPE_CPRM02:
	case FEDM_ISC_TYPE_CPR02:
	case FEDM_ISC_TYPE_CPR04_U:
//		sReport += "\n\n";
		sReport += "Additional Firmware Functions:";
		sReport += "\r\n";
		sReport += "\tTemplate:..............";
		sprintf(cBuffer, "0x%02X", ucDecType[0]);
		sReport += cBuffer;
		sReport += "\r\n";
		sReport += "\tFunctions:.............";
		sprintf(cBuffer, "0x%02X", ucDecType[1]);
		sReport += cBuffer;
		sReport += "\r\n";
		break;

	case FEDM_ISC_TYPE_CPR20_XX:
	case FEDM_ISC_TYPE_CPR30_XX:
	case FEDM_ISC_TYPE_CPR40_XX_U:
	case FEDM_ISC_TYPE_CPR40_XX:
	case FEDM_ISC_TYPE_CPR44_XX:
	case FEDM_ISC_TYPE_CPR46_XX:
	case FEDM_ISC_TYPE_CPR47_XX:
	case FEDM_ISC_TYPE_CPR50_XX:
	case FEDM_ISC_TYPE_CPR52_XX:
	case FEDM_ISC_TYPE_CPR60_XX:
	// myAXXESS
	case FEDM_ISC_TYPE_MAX50_XX:
	case FEDM_ISC_TYPE_MAX_STANDARD:
	case FEDM_ISC_TYPE_MAX_COMFORT:
	case FEDM_ISC_TYPE_MAX_ONTOP:
		sReport += "Additional Firmware Functions:";
		sReport += "\r\n";
		sReport += "\tTemplate:..............";
		sprintf(cBuffer, "0x%02X", ucDecType[0]);
		sReport += cBuffer;
		sReport += "\r\n";
		sReport += "\tFunctions:.............";
		sprintf(cBuffer, "0x%02X 0x%02X 0x%02X", ucDecType[1], ucDecType[2], ucDecType[3]);
		sReport += cBuffer;
		if(ucDecType[1]&0x01)
		{
			sReport += "\r\n";
			sReport += "\t   - with support for T=CL";
		}
		if(ucDecType[1]&0x02)
		{
			sReport += "\r\n";
			sReport += "\t   - with support for MAD";
		}
		if(ucDecType[1]&0x04)
		{
			sReport += "\r\n";
			sReport += "\t   - with support for DESFire Crypto";
		}
		if(ucDecType[1]&0x08)
		{
			sReport += "\r\n";
			sReport += "\t   - with support for MIFARE Plus Crypto";
		}
		if(ucDecType[1]&0x10)
		{
			sReport += "\r\n";
			sReport += "\t   - with support for Kovio";
		}
		if(ucDecType[1]&0x40)
		{
			sReport += "\r\n";
			sReport += "\t   - with support for T=CL with APDU-Length and EXT-MODE";
		}
		if(ucDecType[1]&0x80)
		{
			sReport += "\r\n";
			sReport += "\t   - with support for MAX (Access Control)";
		}
		if(ucDecType[2]&0x01)
		{
			sReport += "\r\n";
			sReport += "\t   - with support for EMVco";
		}
		if(ucDecType[2]&0x02)
		{
			sReport += "\r\n";
			sReport += "\t   - with support for EMV Slot1";
		}
		if(ucDecType[2]&0x04)
		{
			sReport += "\r\n";
			sReport += "\t   - with support for EMV Slot2";
		}
		if(ucDecType[2]&0x08)
		{
			sReport += "\r\n";
			sReport += "\t   - with support for EMV Slot3";
		}
		if(ucDecType[2]&0x10)
		{
			sReport += "\r\n";
			sReport += "\t   - with support for EMV Slot4";
		}
		if(ucDecType[3]&0x01)
		{
			sReport += "\r\n";
			sReport += "\t   - with support for SAM Presence Polling";
		}
		sReport += "\r\n";
		sReport += "\tFirmware-Identifier....";
		for(i=9; i<11; i++)
		{
			sprintf(cBuffer, "0x%02X ", ucDecType[i]);
			sReport += cBuffer;
		}
		sReport += "\r\n";
		break;
	}
}


void FedmIscReport_ReaderInfo::Add0x05InfoText(string& sReport, bool bSingle)
{
	char cBuffer[64];
	unsigned char ucSWMajor = 0;
	unsigned char ucSWMinor = 0;
	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO_SW_MAJOR, &ucSWMajor);
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO_SW_MINOR, &ucSWMinor);
	}
	else
	{
		ucSWMajor = pInfo->ucBootSwVer[0];
		ucSWMinor = pInfo->ucBootSwVer[1];
	}

//	sReport += "\n\n";

	sReport += "Firmware Version of RFC-Bootloader:";
	sReport += "\r\n";
	sReport += "\tSoftware Revision:.....";
	sprintf(cBuffer, "%02d.%02d", ucSWMajor, ucSWMinor);
	sReport += cBuffer;
	sReport += "\r\n";
}


void FedmIscReport_ReaderInfo::Add0x06InfoText(string& sReport, bool bSingle)
{
	char cBuffer[64];
	unsigned char ucInfo[32];
	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO, ucInfo, 8);
	}
	else
	{
		ucInfo[0] = pInfo->ucKeybStatus[0];
		ucInfo[1] = pInfo->ucKeybStatus[1];
		ucInfo[2] = pInfo->ucKeybSwVer[0];
		ucInfo[3] = pInfo->ucKeybSwVer[1];
		ucInfo[4] = pInfo->ucKeybHwVer[0];
		ucInfo[5] = pInfo->ucKeybHwVer[1];
		ucInfo[6] = pInfo->ucKeybHwVer[2];
		ucInfo[7] = pInfo->ucKeybHwVer[3];
	}

//	sReport += "\n\n";

	sReport += "Info about Keyboard-Controller:";
	sReport += "\r\n";
	sReport += "\tStatus:................";
	sprintf(cBuffer, "0x%02X 0x%02X", ucInfo[0], ucInfo[1]);
	sReport += cBuffer;
	sReport += "\r\n";
	if(ucInfo[0] & 0x01)
	{
		sReport += "\t  - The Reader is equipped with a Keyboard\r\n";
		if(ucInfo[1] & 0x01)
			sReport += "\t  - The Keyboard-Controller signals an error\r\n";
		if(ucInfo[1] & 0x02)
			sReport += "\t  - The Keyboard-Controller is in update mode\r\n";

		sReport += "\tSoftware Revision:.....";
		sprintf(cBuffer, "%02d.%02d", ucInfo[2], ucInfo[3]);
		sReport += cBuffer;
		sReport += "\r\n";
		sReport += "\tHardware Revision:.....";
		sprintf(cBuffer, "0x%02X%02X%02X%02X", ucInfo[4], ucInfo[5], ucInfo[6], ucInfo[7]);
		sReport += cBuffer;
	}
	else
	{
		sReport += "\t- The Reader is not equipped with a Keyboard";
	}
	sReport += "\r\n";
}


void FedmIscReport_ReaderInfo::Add0x07InfoText(string& sReport, bool bSingle)
{
	char cBuffer[64];
	unsigned char ucInfo[32];
	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO, ucInfo, 7);
	}
	else
	{
		ucInfo[0] = pInfo->ucCprioSwVer[0];
		ucInfo[1] = pInfo->ucCprioSwVer[1];
		ucInfo[2] = pInfo->ucCprioHwVer;
		ucInfo[3] = pInfo->ucCprioNoOfInputs;
		ucInfo[4] = pInfo->ucCprioNoOfOutputs;
		ucInfo[5] = pInfo->ucCprioNoOfRelays;
		ucInfo[6] = pInfo->ucCprioNoOfSignalers;
	}


	sReport += "Info about CPRIO unit:";
	sReport += "\r\n";

	if(	ucInfo[3] == 0 &&
		ucInfo[4] == 0 &&
		ucInfo[5] == 0 &&
		ucInfo[6] == 0 )
	{
		sReport += "\t  - no CPRIO unit detected\r\n";
	}
	else
	{
		sReport += "\tSoftware Revision:.....";
		sprintf(cBuffer, "%02d.%02d", ucInfo[0], ucInfo[1]);
		sReport += cBuffer;
		sReport += "\r\n";
		sReport += "\tHardware Revision:.....";
		sprintf(cBuffer, "0x%02X", ucInfo[2]);
		sReport += cBuffer;
		sReport += "\r\n";
		sprintf(cBuffer, "\t%d Inputs\r\n", ucInfo[3]);
		sReport += cBuffer;
		sprintf(cBuffer, "\t%d Outputs\r\n", ucInfo[4]);
		sReport += cBuffer;
		sprintf(cBuffer, "\t%d Relays\r\n", ucInfo[5]);
		sReport += cBuffer;
		sprintf(cBuffer, "\t%d Signalers\r\n", ucInfo[6]);
		sReport += cBuffer;
	}
//	sReport += "\r\n";
}


void FedmIscReport_ReaderInfo::Add0x08InfoText(string& sReport, bool bSingle)
{
	char cBuffer[64];
	unsigned char ucInfo[32];
	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO, ucInfo, 14);
	}
	else
	{
		memcpy(ucInfo, pInfo->ucRfcSupportedTagDriver, 14);
	}


	sReport += "Info about supported Transponder Types:";
	sReport += "\r\n";

	for(int i=0; i<14; i++)
	{
		if(ucInfo[i] != 0xFF)
		{
			sprintf(cBuffer, "\t0x%02X - %s\r\n", ucInfo[i], m_pReader->GetTagName(ucInfo[i]));
			sReport += cBuffer;
		}
	}
}


void FedmIscReport_ReaderInfo::Add0x09InfoText(string& sReport, bool bSingle)
{
	char cBuffer[64];
	unsigned char ucInfo[32];
	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO, ucInfo, 14);
	}
	else
	{
		memcpy(ucInfo, pInfo->ucWlanVer, 14);
	}


	sReport += "Info from WLAN-Module:";
	sReport += "\r\n\t";

	for(int cnt=0; cnt<14; cnt++)
	{
		if(cnt > 0)
		{
			sReport += " - ";
		}

		sprintf(cBuffer, "0x%02X", ucInfo[cnt]);
		sReport += cBuffer;
	}
	sReport += "\r\n";
}


void FedmIscReport_ReaderInfo::Add0x10InfoText(string& sReport, bool bSingle)
{
	char cBuffer[64];
	unsigned char ucDecType[32];
	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO, ucDecType, 11);
	}
	else
	{
		ucDecType[0] = pInfo->ucRfcHwInfo[0];
		ucDecType[1] = pInfo->ucRfcHwInfo[1];
		ucDecType[2] = pInfo->ucRfcDHw[0];
		ucDecType[3] = pInfo->ucRfcDHw[1];
		ucDecType[4] = pInfo->ucRfcAHw[0];
		ucDecType[5] = pInfo->ucRfcAHw[1];
		ucDecType[6] = pInfo->ucRfcFrequency;
		ucDecType[7] = pInfo->ucRfcPortTypes;
		ucDecType[8] = pInfo->ucReserved2;
		ucDecType[9] = pInfo->ucRfcInfo;
		ucDecType[10] = pInfo->ucBatteryStatus;
	}

//	sReport += "\n\n";

	sReport += "Hardware Information:";
	sReport += "\r\n";
	sprintf(cBuffer, "\t0x%02X%02X - 0x%02X%02X - 0x%02X%02X",
				ucDecType[0],
				ucDecType[1],
				ucDecType[2],
				ucDecType[3],
				ucDecType[4],
				ucDecType[5]);

	sReport += cBuffer;

/*	if( m_pReader->GetReaderType() == FEDM_ISC_TYPE_ISCLRU3000 && 
		m_pReader->m_mapOemNames.find(FEDM_ISC_TYPE_ISCLRU3000) == m_pReader->m_mapOemNames.end())
	{
		if( (ucDecType[1] & 0xC0) == 0x00)
		{
			sReport += "\r\n";
			sReport += "\t";
			sReport += FEDM_ISC_NAME_ISCLRU3500;
			sReport +=" identified";
		}
	}*/

//	if(pInfo->bIsRfcFrequency)
	{
		switch(m_pReader->GetReaderType())
		{
		case FEDM_ISC_TYPE_ISCLRU2000:
			if(pInfo->ucRfcSwVer[0] >= 2)
			{
				sReport += "\r\n";
				sReport += "\tFrequency Information:.";
				if((ucDecType[6]&0x03) == 1)
					sReport += "EU";
				else if((ucDecType[6]&0x03) == 2)
					sReport += "FCC";
				else
					sReport += "unknown";

				sReport += "\r\n";
				sReport += "\tSupported Port Types:..";
				if(ucDecType[7] & 0x01)
				{
					sReport += "Serial";
					if(ucDecType[7] & 0xFE)
						sReport += ", ";
				}
				if(ucDecType[7] & 0x02)
				{
					sReport += "RS4xx";
					if(ucDecType[7] & 0xFC)
					sReport += ", ";
				}
				if(ucDecType[7] & 0x04)
				{
					sReport += "LAN";
					if(ucDecType[7] & 0xF8)
						sReport += ", ";
				}
				if(ucDecType[7] & 0x08)
				{
					sReport += "WLAN";
					if(ucDecType[7] & 0xF0)
						sReport += ", ";
				}
			}
			break;

		case FEDM_ISC_TYPE_ISCPRHD102:
		case FEDM_ISC_TYPE_ISCMRU102:
		case FEDM_ISC_TYPE_ISCMU02:
		case FEDM_ISC_TYPE_MAXU1002:
		case FEDM_ISC_TYPE_ISCLRU1002:
		case FEDM_ISC_TYPE_ISCLRU3000:
			sReport += "\r\n";
			sReport += "\tFrequency Information:.";
			if((ucDecType[6]&0x03) == 1)
				sReport += "EU";
			else if((ucDecType[6]&0x03) == 2)
				sReport += "FCC";
			else if((ucDecType[6]&0x03) == 3)
				sReport += "EU and FCC";
			else
				sReport += "unknown";

			sReport += "\r\n";
			sReport += "\tSupported Port Types:..";
			if(ucDecType[7] & 0x01)
			{
				sReport += "Serial";
				if(ucDecType[7] & 0xFE)
					sReport += ", ";
			}
			if(ucDecType[7] & 0x02)
			{
				sReport += "RS4xx";
				if(ucDecType[7] & 0xFC)
				sReport += ", ";
			}
			if(ucDecType[7] & 0x04)
			{
				sReport += "LAN";
				if(ucDecType[7] & 0xF8)
					sReport += ", ";
			}
			if(ucDecType[7] & 0x08)
			{
				sReport += "WLAN";
				if(ucDecType[7] & 0xF0)
					sReport += ", ";
			}
			if(ucDecType[7] & 0x10)
			{
				sReport += "USB";
				if(ucDecType[7] & 0xE0)
					sReport += ", ";
			}
			if(ucDecType[7] & 0x20)
			{
				sReport += "Bluetooth";
				if(ucDecType[7] & 0xC0)
					sReport += ", ";
			}
			if(ucDecType[7] & 0x80)
			{
				sReport += "Discovery";
			}

			break;

		case FEDM_ISC_TYPE_ISCMRU200:
			sReport += "\r\n";
			sReport += "\tFrequency Information:.";
			if((ucDecType[6]&0x03) == 1)
				sReport += "EU";
			else if((ucDecType[6]&0x03) == 2)
				sReport += "FCC";
			else
				sReport += "unknown";

			break;

		// megalock C
		case FEDM_ISC_TYPE_MLC_COM:
			sReport += "\r\n";
			sReport += "\tSupported Port Types:..";
			if(ucDecType[7] & 0x10)
			{
				sReport += "USB";
				if(ucDecType[7] & 0xE0)
					sReport += ", ";
			}

			sReport += "\r\n";
			sReport += "\tBattery Status:........";
			sprintf(cBuffer, "%d", (int)ucDecType[10]);
			sReport += cBuffer;
			
			break;


		default:
			sReport += "\r\n";
			sReport += "\tSupported Port Types:..";
			if(ucDecType[7] & 0x01)
			{
				sReport += "Serial";
				if(ucDecType[7] & 0xFE)
					sReport += ", ";
			}
			if(ucDecType[7] & 0x02)
			{
				sReport += "RS4xx";
				if(ucDecType[7] & 0xFC)
				sReport += ", ";
			}
			if(ucDecType[7] & 0x04)
			{
				sReport += "LAN";
				if(ucDecType[7] & 0xF8)
					sReport += ", ";
			}
			if(ucDecType[7] & 0x08)
			{
				sReport += "WLAN";
				if(ucDecType[7] & 0xF0)
					sReport += ", ";
			}
			if(ucDecType[7] & 0x10)
			{
				sReport += "USB";
				if(ucDecType[7] & 0xE0)
					sReport += ", ";
			}
			if(ucDecType[7] & 0x20)
			{
				sReport += "Bluetooth";
				if(ucDecType[7] & 0xC0)
					sReport += ", ";
			}
			if(ucDecType[7] & 0x80)
			{
				sReport += "Discovery";
			}

			break;
		}
	}

	sReport += "\r\n";
}


void FedmIscReport_ReaderInfo::Add0x11InfoText(string& sReport, bool bSingle)
{
	char cBuffer[64];
	unsigned char ucSlotInfo[32];
	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO, ucSlotInfo, 5);
	}
	else
	{
		ucSlotInfo[0] = pInfo->ucNoOfSlots;
		ucSlotInfo[1] = pInfo->ucSlotInfo[0];
		ucSlotInfo[2] = pInfo->ucSlotInfo[1];
		ucSlotInfo[3] = pInfo->ucSlotInfo[2];
		ucSlotInfo[4] = pInfo->ucSlotInfo[3];
	}

//	sReport += "\n\n";

	sReport += "SAM Information:";
	sReport += "\r\n";
	sReport += "\tNumber of SAM Slots:...";
	sprintf(cBuffer, "%u", (unsigned int)ucSlotInfo[0]);
	sReport += cBuffer;
	sReport += "\r\n";

	if(ucSlotInfo[0] >= 1)
	{
		if(ucSlotInfo[1] & 0x01)
		{
			sReport += "\t   - SAM inserted in socket 1\r\n";
			sReport += "\t	 Slot-Type: ";
			switch( ((ucSlotInfo[1] & 0xF0) >> 4) )
			{
			case 0x00:
				sReport += "smartcard holder slot\r\n";
				break;
			case 0x01:
				sReport += "NXP SAM\r\n";
				break;
			case 0x02:
				sReport += "Infineon SAM\r\n";
				break;
			}
		}
		else
		{
			sReport += "\t   - SAM socket 1 is not used\r\n";
		}
	}

	if(ucSlotInfo[0] >= 2)
	{
		if(ucSlotInfo[2] & 0x01)
		{
			sReport += "\t   - SAM inserted in socket 2\r\n";
			sReport += "\t	 Slot-Type: ";
			switch( ((ucSlotInfo[2] & 0xF0) >> 4) )
			{
			case 0x00:
				sReport += "smartcard holder slot\r\n";
				break;
			case 0x01:
				sReport += "NXP SAM\r\n";
				break;
			case 0x02:
				sReport += "Infineon SAM\r\n";
				break;
			}
		}
		else
		{
			sReport += "\t   - SAM socket 2 is not used\r\n";
		}
	}

	if(ucSlotInfo[0] >= 3)
	{
		if(ucSlotInfo[3] & 0x01)
		{
			sReport += "\t   - SAM inserted in socket 3\r\n";
			sReport += "\t	 Slot-Type: ";
			switch( ((ucSlotInfo[3] & 0xF0) >> 4) )
			{
			case 0x00:
				sReport += "smartcard holder slot\r\n";
				break;
			case 0x01:
				sReport += "NXP SAM\r\n";
				break;
			case 0x02:
				sReport += "Infineon SAM\r\n";
				break;
			}
		}
		else
		{
			sReport += "\t   - SAM socket 3 is not used\r\n";
		}
	}

	if(ucSlotInfo[0] >= 4)
	{
		if(ucSlotInfo[4] & 0x01)
		{
			sReport += "\t   - SAM inserted in socket 4\r\n";
			sReport += "\t	 Slot-Type: ";
			switch( ((ucSlotInfo[4] & 0xF0) >> 4) )
			{
			case 0x00:
				sReport += "smartcard holder slot\r\n";
				break;
			case 0x01:
				sReport += "NXP SAM\r\n";
				break;
			case 0x02:
				sReport += "Infineon SAM\r\n";
				break;
			}
		}
		else
		{
			sReport += "\t   - SAM socket 4 is not used\r\n";
		}
	}
}


void FedmIscReport_ReaderInfo::Add0x12InfoText(string& sReport, bool bSingle)
{
	char cBuffer[64];
	unsigned char ucInfo[32];
	unsigned int uiFlashSize = 0;
	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO, ucInfo, 8);
	}
	else
	{
		memcpy(&ucInfo[0], &pInfo->ucRfcCpuManufacturer, 8);
	}

	uiFlashSize  = (((unsigned int)ucInfo[4])<<24);
	uiFlashSize |= (((unsigned int)ucInfo[5])<<16);
	uiFlashSize |= (((unsigned int)ucInfo[6])<< 8);
	uiFlashSize |=   (unsigned int)ucInfo[7];

	sReport += "CPU Information of RF Communication Controller:";
	sReport += "\r\n";
	sprintf(cBuffer, "\tManufacturer:...%d\r\n", ucInfo[0]);
	sReport += cBuffer;
	sprintf(cBuffer, "\tType:...........0x%02X%02X\r\n", ucInfo[1], ucInfo[2]);
	sReport += cBuffer;
	sprintf(cBuffer, "\tRevision:.......%d\r\n", ucInfo[3]);
	sReport += cBuffer;
	sprintf(cBuffer, "\tFlash-Size:.....%u KB\r\n", uiFlashSize);
	sReport += cBuffer;
}


void FedmIscReport_ReaderInfo::Add0x40InfoText(string& sReport, bool bSingle)
{
	char cBuffer[64];
	unsigned char ucInfo[32];
	unsigned int uiNoOfCfgPages = 0;
	string sFormat = "";
	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO, ucInfo, 18);
	}
	else
	{
		ucInfo[0] = pInfo->ucNoOfCfgPages[0];
		ucInfo[1] = pInfo->ucNoOfCfgPages[1];

		for(int i=0; i<16; i++)
		{
			ucInfo[2+i] = pInfo->ucCfgReadPermissions[i];
		}
	}

	uiNoOfCfgPages = (ucInfo[0]<<8) + ucInfo[1];

	for(unsigned int i=0; i<uiNoOfCfgPages/8; i++)
		sFormat += "%02X";

	if(uiNoOfCfgPages%8)
		sFormat += "%02X";


//	sReport += "\n\n";
	sReport += "Configuration Information for read";
	sReport += "\r\n";

	sReport += "\tCFG Pages:.............";
	sprintf(cBuffer, "%u", uiNoOfCfgPages);
	sReport += cBuffer;
	sReport += "\r\n";

	sReport += "\tCFG read permission:...";
	sprintf(cBuffer,
			sFormat.c_str(),
			ucInfo[2],
			ucInfo[3],
			ucInfo[4],
			ucInfo[5],
			ucInfo[6],
			ucInfo[7],
			ucInfo[8],
			ucInfo[9],
			ucInfo[10],
			ucInfo[11],
			ucInfo[12],
			ucInfo[13],
			ucInfo[14],
			ucInfo[15],
			ucInfo[16],
			ucInfo[17] );

	sReport += cBuffer;
	sReport += "\r\n";
}


void FedmIscReport_ReaderInfo::Add0x41InfoText(string& sReport, bool bSingle)
{
	char cBuffer[64];
	unsigned char ucInfo[32];
	unsigned int uiNoOfCfgPages = 0;
	string sFormat = "";
	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO, ucInfo, 18);
	}
	else
	{
		ucInfo[0] = pInfo->ucNoOfCfgPages[0];
		ucInfo[1] = pInfo->ucNoOfCfgPages[1];

		for(int i=0; i<16; i++)
		{
			ucInfo[2+i] = pInfo->ucCfgReadPermissions[i];
		}
	}

	uiNoOfCfgPages = (ucInfo[0]<<8) + ucInfo[1];

	for(unsigned int i=0; i<uiNoOfCfgPages/8; i++)
		sFormat += "%02X";

	if(uiNoOfCfgPages%8)
		sFormat += "%02X";


//	sReport += "\n\n";
	sReport += "Configuration Information for write";
	sReport += "\r\n";

	sReport += "\tCFG Pages:.............";
	sprintf(cBuffer, "%u", uiNoOfCfgPages);
	sReport += cBuffer;
	sReport += "\r\n";

	sReport += "\tCFG write permission:..";
	sprintf(cBuffer,
			sFormat.c_str(),
			ucInfo[2],
			ucInfo[3],
			ucInfo[4],
			ucInfo[5],
			ucInfo[6],
			ucInfo[7],
			ucInfo[8],
			ucInfo[9],
			ucInfo[10],
			ucInfo[11],
			ucInfo[12],
			ucInfo[13],
			ucInfo[14],
			ucInfo[15],
			ucInfo[16],
			ucInfo[17] );

	sReport += cBuffer;
	sReport += "\r\n";
}


void FedmIscReport_ReaderInfo::Add0x50InfoText(string& sReport, bool bSingle)
{
	char cBuffer[64];
	unsigned char ucInfo[32];
	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO, ucInfo, 30);
		sReport += "LAN-Information\n";
	}
	else
	{
		ucInfo[0] = pInfo->ucLanFlags;
		memcpy(&ucInfo[1], &pInfo->ucLanMac[0], 6);
		ucInfo[7] = pInfo->ucLanLinkInfo;
	}

//	sReport += "\r\n";
	if( (ucInfo[0] & 0x02) ||	// IPv4 supported
		(ucInfo[0] & 0x01) )	// IPv6 supported
	{
		if( ucInfo[0] & ((ucInfo[0]&0x03)<<2) )
		{
			sReport += "\tLAN disabled";
		}
		else
		{
			sReport += "\tMAC-Address:...........";
			sprintf(cBuffer,
					"%02X:%02X:%02X:%02X:%02X:%02X",
					ucInfo[1],
					ucInfo[2],
					ucInfo[3],
					ucInfo[4],
					ucInfo[5],
					ucInfo[6] );

			sReport += cBuffer;
		}
	}

	sReport += "\r\n";

	if (ucInfo[7] & 0x01)
	{
		sReport += "\tAuto-Negotiation:......Off";
		sReport += "\r\n";

		if (ucInfo[7] & 0x02)
		{
			sReport += "\tDuplexmode:............Full Duplex";
			sReport += "\r\n";
		}
		else
		{
			sReport += "\tDuplexmode:............Half Duplex";
			sReport += "\r\n";
		}

		if (ucInfo[7] & 0x04)
		{
			sReport += "\tSpeed:.................100 Mbit";
			sReport += "\r\n";
		}
		else
		{
			sReport += "\tSpeed:.................10 Mbit";
			sReport += "\r\n";
		}
	}
}


void FedmIscReport_ReaderInfo::Add0x51InfoText(string& sReport, bool bSingle)
{
	char cBuffer[256];
	unsigned char ucInfo[32];
	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO, ucInfo, 30);
		sReport += "LAN-Information\n";
	}
	else
	{
		ucInfo[0] = pInfo->ucLanFlags;
		memcpy(&ucInfo[1], &pInfo->ucLanIpV4[0], 4);
		memcpy(&ucInfo[5], &pInfo->ucLanIpV6[0], 16);
	}


//	sReport += "\r\n";
	if(ucInfo[0] & 0x02)	// IPv4 supported
	{
		if(ucInfo[0] & 0x08)	// IPv4 disabled
		{
			sReport += "\tIPv4-Channel disabled";
		}
		else
		{
			if(ucInfo[0] & 0x20)	// DHCP v4
				sReport += "\tIPv4-Address:..[DHCP]..";
			else
				sReport += "\tIPv4-Address:..........";

			sprintf(cBuffer,
					"%d.%d.%d.%d",
					ucInfo[1],
					ucInfo[2],
					ucInfo[3],
					ucInfo[4] );

			sReport += cBuffer;
		}
	}
	
	if(ucInfo[0] & 0x01)	// IPv6 supported
	{
		if(ucInfo[0] & 0x04)	// IPv6 disabled
		{
			sReport += "\tIPv6-Channel disabled";
		}
		else
		{
			if(ucInfo[0] & 0x10)	// DHCP v6
				sReport += "\tIPv6-Address:..[DHCP]..";
			else
				sReport += "\tIPv6-Address:..........";

			sprintf(cBuffer,
					"%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X",
					ucInfo[5],
					ucInfo[6],
					ucInfo[7],
					ucInfo[8],
					ucInfo[9],
					ucInfo[10],
					ucInfo[11],
					ucInfo[12],
					ucInfo[13],
					ucInfo[14],
					ucInfo[15],
					ucInfo[16],
					ucInfo[17],
					ucInfo[18],
					ucInfo[19],
					ucInfo[20] );

			sReport += cBuffer;
		}
	}

	sReport += "\r\n";
}


void FedmIscReport_ReaderInfo::Add0x52InfoText(string& sReport, bool bSingle)
{
	char cBuffer[256];
	unsigned char ucInfo[32];
	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO, ucInfo, 30);
		sReport += "LAN-Information\n";
	}
	else
	{
		ucInfo[0] = pInfo->ucLanFlags;
		memcpy(&ucInfo[1], &pInfo->ucLanNetmaskV4[0], 4);
		memcpy(&ucInfo[5], &pInfo->ucLanNetmaskV6[0], 16);
	}


//	sReport += "\r\n";
	if(ucInfo[0] & 0x02)	// IPv4 supported
	{
		if(ucInfo[0] & 0x08)	// IPv4 disabled
		{
			sReport += "\tIPv4-Channel disabled";
		}
		else
		{
			if(ucInfo[0] & 0x20)	// DHCP v4
				sReport += "\tIPv4-Netmask:..[DHCP]..";
			else
				sReport += "\tIPv4-Netmask:..........";

			sprintf(cBuffer,
					"%d.%d.%d.%d",
					ucInfo[1],
					ucInfo[2],
					ucInfo[3],
					ucInfo[4] );
	
			sReport += cBuffer;
		}
	}

	if(ucInfo[0] & 0x01)	// IPv6 supported
	{
		if(ucInfo[0] & 0x04)	// IPv6 disabled
		{
			sReport += "\tIPv6-Channel disabled";
		}
		else
		{
			if(ucInfo[0] & 0x10)	// DHCP v6
				sReport += "\tIPv6-Netmask:..[DHCP]..";
			else
				sReport += "\tIPv6-Netmask:..........";

			sprintf(cBuffer,
					"%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X",
					ucInfo[5],
					ucInfo[6],
					ucInfo[7],
					ucInfo[8],
					ucInfo[9],
					ucInfo[10],
					ucInfo[11],
					ucInfo[12],
					ucInfo[13],
					ucInfo[14],
					ucInfo[15],
					ucInfo[16],
					ucInfo[17],
					ucInfo[18],
					ucInfo[19],
					ucInfo[20] );

			sReport += cBuffer;
		}
	}

	sReport += "\r\n";
}


void FedmIscReport_ReaderInfo::Add0x53InfoText(string& sReport, bool bSingle)
{
	char cBuffer[256];
	unsigned char ucInfo[32];
	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO, ucInfo, 30);
		sReport += "LAN-Information\n";
	}
	else
	{
		ucInfo[0] = pInfo->ucLanFlags;
		memcpy(&ucInfo[1], &pInfo->ucLanGatewayV4[0], 4);
		memcpy(&ucInfo[5], &pInfo->ucLanGatewayV6[0], 16);
	}


//	sReport += "\r\n";
	if(ucInfo[0] & 0x02)	// IPv4 supported
	{
		if(ucInfo[0] & 0x08)	// IPv4 disabled
		{
			sReport += "\tIPv4-Channel disabled";
		}
		else
		{
			if(ucInfo[0] & 0x20)	// DHCP v4
				sReport += "\tIPv4-Gateway:..[DHCP]..";
			else
				sReport += "\tIPv4-Gateway:..........";

			sprintf(cBuffer,
					"%d.%d.%d.%d",
					ucInfo[1],
					ucInfo[2],
					ucInfo[3],
					ucInfo[4] );

			sReport += cBuffer;
		}
	}

	if(ucInfo[0] & 0x01)	// IPv6 supported
	{
		if(ucInfo[0] & 0x04)	// IPv6 disabled
		{
			sReport += "\tIPv6-Channel disabled";
		}
		else
		{
			if(ucInfo[0] & 0x10)	// DHCP v6
				sReport += "\tIPv6-Gateway:..[DHCP]..";
			else
				sReport += "\tIPv6-Gateway:..........";

			sprintf(cBuffer,
					"%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X",
					ucInfo[5],
					ucInfo[6],
					ucInfo[7],
					ucInfo[8],
					ucInfo[9],
					ucInfo[10],
					ucInfo[11],
					ucInfo[12],
					ucInfo[13],
					ucInfo[14],
					ucInfo[15],
					ucInfo[16],
					ucInfo[17],
					ucInfo[18],
					ucInfo[19],
					ucInfo[20] );

			sReport += cBuffer;
		}
	}

	sReport += "\r\n";
}


void FedmIscReport_ReaderInfo::Add0x54InfoText(string& sReport, bool bSingle)
{
	char cBuffer[64];
	unsigned char ucInfo[32];
	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO, ucInfo, 30);
		sReport += "WLAN-Information\n";
	}
	else
	{
		ucInfo[0] = pInfo->ucWlanFlags;
		memcpy(&ucInfo[1], &pInfo->ucWlanMac[0], 6);
	}

	if( (ucInfo[0] & 0x02) ||	// IPv4 supported
		(ucInfo[0] & 0x01) )	// IPv6 supported
	{
		if( ucInfo[0] & ((ucInfo[0]&0x03)<<2) )
		{
			sReport += "\tWLAN disabled";
		}
		else
		{
			sReport += "\tMAC-Address:...........";
			sprintf(cBuffer,
					"%02X:%02X:%02X:%02X:%02X:%02X",
					ucInfo[1],
					ucInfo[2],
					ucInfo[3],
					ucInfo[4],
					ucInfo[5],
					ucInfo[6] );

			sReport += cBuffer;
		}
	}

	sReport += "\r\n";
}


void FedmIscReport_ReaderInfo::Add0x55InfoText(string& sReport, bool bSingle)
{
	char cBuffer[256];
	unsigned char ucInfo[32];
	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO, ucInfo, 30);
		sReport += "WLAN-Information\n";
	}
	else
	{
		ucInfo[0] = pInfo->ucWlanFlags;
		memcpy(&ucInfo[1], &pInfo->ucWlanIpV4[0], 4);
		memcpy(&ucInfo[5], &pInfo->ucWlanIpV6[0], 16);
	}


//	sReport += "\r\n";
	if(ucInfo[0] & 0x02)	// IPv4 supported
	{
		if(ucInfo[0] & 0x08)	// IPv4 disabled
		{
			sReport += "\tIPv4-Channel disabled";
		}
		else
		{
			if(ucInfo[0] & 0x20)	// DHCP v4
				sReport += "\tIPv4-Address:..[DHCP]..";
			else
				sReport += "\tIPv4-Address:..........";
		
				sprintf(cBuffer,
					"%d.%d.%d.%d",
					ucInfo[1],
					ucInfo[2],
					ucInfo[3],
					ucInfo[4] );

			sReport += cBuffer;
		}
	}

	if(ucInfo[0] & 0x01)	// IPv6 supported
	{
		if(ucInfo[0] & 0x04)	// IPv6 disabled
		{
			sReport += "\tIPv6-Channel disabled";
		}
		else
		{
			if(ucInfo[0] & 0x10)	// DHCP v6
				sReport += "\tIPv6-Address:..[DHCP]..";
			else
				sReport += "\tIPv6-Address:..........";

			sprintf(cBuffer,
					"%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X",
					ucInfo[5],
					ucInfo[6],
					ucInfo[7],
					ucInfo[8],
					ucInfo[9],
					ucInfo[10],
					ucInfo[11],
					ucInfo[12],
					ucInfo[13],
					ucInfo[14],
					ucInfo[15],
					ucInfo[16],
					ucInfo[17],
					ucInfo[18],
					ucInfo[19],
					ucInfo[20] );

			sReport += cBuffer;
		}
	}

	sReport += "\r\n";
}


void FedmIscReport_ReaderInfo::Add0x56InfoText(string& sReport, bool bSingle)
{
	char cBuffer[256];
	unsigned char ucInfo[32];
	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO, ucInfo, 30);
		sReport += "WLAN-Information\n";
	}
	else
	{
		ucInfo[0] = pInfo->ucWlanFlags;
		memcpy(&ucInfo[1], &pInfo->ucWlanNetmaskV4[0], 4);
		memcpy(&ucInfo[5], &pInfo->ucWlanNetmaskV6[0], 16);
	}

//	sReport += "\r\n";
	if(ucInfo[0] & 0x02)	// IPv4 supported
	{
		if(ucInfo[0] & 0x08)	// IPv4 disabled
		{
			sReport += "\tIPv4-Channel disabled";
		}
		else
		{
			if(ucInfo[0] & 0x20)	// DHCP v4
				sReport += "\tIPv4-Netmask:..[DHCP]..";
			else
				sReport += "\tIPv4-Netmask:..........";

			sprintf(cBuffer,
					"%d.%d.%d.%d",
					ucInfo[1],
					ucInfo[2],
					ucInfo[3],
					ucInfo[4] );

			sReport += cBuffer;
		}
	}

	if(ucInfo[0] & 0x01)	// IPv6 supported
	{
		if(ucInfo[0] & 0x04)	// IPv6 disabled
		{
			sReport += "\tIPv6-Channel disabled";
		}
		else
		{
			if(ucInfo[0] & 0x10)	// DHCP v6
				sReport += "\tIPv6-Netmask:..[DHCP]..";
			else
				sReport += "\tIPv6-Netmask:..........";
		
			sprintf(cBuffer,
					"%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X",
					ucInfo[5],
					ucInfo[6],
					ucInfo[7],
					ucInfo[8],
					ucInfo[9],
					ucInfo[10],
					ucInfo[11],
					ucInfo[12],
					ucInfo[13],
					ucInfo[14],
					ucInfo[15],
					ucInfo[16],
					ucInfo[17],
					ucInfo[18],
					ucInfo[19],
					ucInfo[20] );

			sReport += cBuffer;
		}
	}

	sReport += "\r\n";
}


void FedmIscReport_ReaderInfo::Add0x57InfoText(string& sReport, bool bSingle)
{
	char cBuffer[256];
	unsigned char ucInfo[32];
	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO, ucInfo, 30);
		sReport += "WLAN-Information\n";
	}
	else
	{
		ucInfo[0] = pInfo->ucWlanFlags;
		memcpy(&ucInfo[1], &pInfo->ucWlanGatewayV4[0], 4);
		memcpy(&ucInfo[5], &pInfo->ucWlanGatewayV6[0], 16);
	}

//	sReport += "\r\n";
	if(ucInfo[0] & 0x02)	// IPv4 supported
	{
		if(ucInfo[0] & 0x08)	// IPv4 disabled
		{
			sReport += "\tIPv4-Channel disabled";
		}
		else
		{
			if(ucInfo[0] & 0x20)	// DHCP v4
				sReport += "\tIPv4-Gateway:..[DHCP]..";
			else
				sReport += "\tIPv4-Gateway:..........";

			sprintf(cBuffer,
					"%d.%d.%d.%d",
					ucInfo[1],
					ucInfo[2],
					ucInfo[3],
					ucInfo[4] );

			sReport += cBuffer;
		}
	}

	if(ucInfo[0] & 0x01)	// IPv6 supported
	{
		if(ucInfo[0] & 0x04)	// IPv6 disabled
		{
			sReport += "\tIPv6-Channel disabled";
		}
		else
		{
			if(ucInfo[0] & 0x10)	// DHCP v6
				sReport += "\tIPv6-Gateway:..[DHCP]..";
			else
				sReport += "\tIPv6-Gateway:..........";
		
			sprintf(cBuffer,
					"%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X",
					ucInfo[5],
					ucInfo[6],
					ucInfo[7],
					ucInfo[8],
					ucInfo[9],
					ucInfo[10],
					ucInfo[11],
					ucInfo[12],
					ucInfo[13],
					ucInfo[14],
					ucInfo[15],
					ucInfo[16],
					ucInfo[17],
					ucInfo[18],
					ucInfo[19],
					ucInfo[20] );

			sReport += cBuffer;
		}
	}

	sReport += "\r\n";
}

void FedmIscReport_ReaderInfo::Add0x60InfoText(string& sReport, bool bSingle)
{
	char cBuffer[64];
	unsigned char ucInfo[32];
	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO, ucInfo, 18);
	}
	else
	{
		ucInfo[0] = pInfo->ucNoOfInputs;
		ucInfo[1] = pInfo->ucNoOfOutputs;
		ucInfo[2] = pInfo->ucNoOfRelays;
	}

	sReport += "Input/Output capabilities";
	sReport += "\r\n";

	sReport += "\tNumber of Inputs:......";
	sprintf(cBuffer, "%d", ucInfo[0]);
	sReport += cBuffer;
	sReport += "\r\n";

	sReport += "\tNumber of Outputs:.....";
	sprintf(cBuffer, "%d", ucInfo[1]);
	sReport += cBuffer;
	sReport += "\r\n";

	sReport += "\tNumber of Relays:......";
	sprintf(cBuffer, "%d", ucInfo[2]);
	sReport += cBuffer;
	sReport += "\r\n";
}

void FedmIscReport_ReaderInfo::Add0x61InfoText(string& sReport, bool bSingle)
{
#if !defined(_FEDM_NO_PD_SUPPORT)
	char cBuffer[64];
	unsigned char ucInfo[32];
	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();
	memset(ucInfo, 0, 32);

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_READER_INFO, ucInfo, 7);
	}
	else
	{
		ucInfo[0] = pInfo->ucNoOfPeripheralDevices;
		if(pInfo->ucNoOfPeripheralDevices >= 1)
		{
			ucInfo[1] = pInfo->aPeripheralDevice[0].ucPDType;
			ucInfo[2] = pInfo->aPeripheralDevice[0].ucPDBusAdr;
		}
		if(pInfo->ucNoOfPeripheralDevices >= 2)
		{
			ucInfo[3] = pInfo->aPeripheralDevice[1].ucPDType;
			ucInfo[4] = pInfo->aPeripheralDevice[1].ucPDBusAdr;
		}
		if(pInfo->ucNoOfPeripheralDevices >= 3)
		{
			ucInfo[5] = pInfo->aPeripheralDevice[2].ucPDType;
			ucInfo[6] = pInfo->aPeripheralDevice[2].ucPDBusAdr;
		}
	}

	sReport += "Peripheral Devices";
	sReport += "\r\n";

	sReport += "\tNumber of Devices:.....";
	sprintf(cBuffer, "%d", ucInfo[0]);
	sReport += cBuffer;
	sReport += "\r\n";

	for(unsigned char ucCnt=0; ucCnt<ucInfo[0]; ucCnt++)
	{
		sprintf(cBuffer, "\t%d. Device:.............", ucCnt+1);
		sReport += cBuffer;
		switch(ucInfo[1+2*ucCnt])
		{
		case FEDM_ISC_PD_TYPE_PEOPLE_COUNTER:
			sReport += "People Counter";
			break;
		case FEDM_ISC_PD_TYPE_EXTERNAL_IO:
			sReport += "External I/O-Board";
			break;
		}
		sReport += "\r\n";

		sprintf(cBuffer, "\t   Bus-Address:........", ucCnt);
		sReport += cBuffer;
		sprintf(cBuffer, "%d", ucInfo[2+2*ucCnt]);
		sReport += cBuffer;
		sReport += "\r\n";
	}
#endif
}


void FedmIscReport_ReaderInfo::Add0x80InfoText(string& sReport, bool bSingle)
{
	char cBuffer[64];
	unsigned char ucVersion[30];
	unsigned long nDeviceID = 0;
	string	sDeviceID;
	string	sOldCusID;
	string	sOldFWVersion;
	string	sOldRdrFunction;
	string	sOldTrDriver;
	string	sOldUhfRdrFunction;
	string	sOldUhfTrDriver;
	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_GetAdrOfID(FEDM_ISC_TMP_READER_INFO, 32), &ucVersion[0], 30, FEDM_TMPDATA_MEM);
	}
	else
	{
		memcpy(&ucVersion[ 0], pInfo->ucRfcDevID, 4);
		memcpy(&ucVersion[ 4], pInfo->ucRfcLicenseCustom, 4);
		memcpy(&ucVersion[ 8], pInfo->ucRfcLicenseFw, 2);
		memcpy(&ucVersion[10], pInfo->ucRfcLicenseTrType, 2);
		memcpy(&ucVersion[12], pInfo->ucRfcLicenseFct, 2);
		memcpy(&ucVersion[14], pInfo->ucRfcLicenseUhfTrType, 2);
		memcpy(&ucVersion[16], pInfo->ucRfcLicenseUhfFct, 2);
	}

	FEDM_ConvHexUCharToHexStr(&ucVersion[ 0], 4, sDeviceID);
	FEDM_ConvHexUCharToHexStr(&ucVersion[ 4], 4, sOldCusID);
	FEDM_ConvHexUCharToHexStr(&ucVersion[ 8], 2, sOldFWVersion);
	FEDM_ConvHexUCharToHexStr(&ucVersion[10], 2, sOldTrDriver);
	FEDM_ConvHexUCharToHexStr(&ucVersion[12], 2, sOldRdrFunction);
	FEDM_ConvHexUCharToHexStr(&ucVersion[14], 2, sOldUhfTrDriver);
	FEDM_ConvHexUCharToHexStr(&ucVersion[16], 2, sOldUhfRdrFunction);


	sReport += "Serial Number and licensed options";
	sReport += "\r\n";
	sReport += "\tDevice-ID...............0x";
	sReport += sDeviceID;
	sReport += " (";
	sscanf(sDeviceID.c_str(), "%lx", &nDeviceID);
	sprintf(cBuffer, "%lu)", nDeviceID);
	sReport += cBuffer;
	sReport += "\r\n";
	sReport += "\tCustomer ID.............0x";
	sReport += sOldCusID;
	sReport += "\r\n";
	sReport += "\tFirmware Version........0x";
	sReport += sOldFWVersion;

	if(m_pReader->GetReaderType() == FEDM_ISC_TYPE_ISCPRHD102)
	{
		sReport += "\r\n";
		sReport += "\tHF-Transponder-Driver...0x";
		sReport += sOldTrDriver;
		sReport += "\r\n";
		sReport += "\tHF Firmware Functions...0x";
		sReport += sOldRdrFunction;
		sReport += "\r\n";
		sReport += "\tUHF-Transponder-Driver..0x";
		sReport += sOldUhfTrDriver;
		sReport += "\r\n";
		sReport += "\tUHF Firmware Functions..0x";
		sReport += sOldUhfRdrFunction;
	}
	else
	{
		sReport += "\r\n";
		sReport += "\tTransponder-Driver......0x";
		sReport += sOldTrDriver;
		sReport += "\r\n";
		sReport += "\tFirmware Functions......0x";
		sReport += sOldRdrFunction;
		switch(m_pReader->GetReaderType())
		{
		case FEDM_ISC_TYPE_ISCLRU3000:
			if(ucVersion[13] & 0x01)
			{
				// ISO 18000-6-C 
				sReport += " (with ISO 18000-6-C support)";
			}
			break;
		}
	}

/*	if( pInfo->bIsRfcFrequency)
	{
		switch(m_pReader->GetReaderType())
		{
		case FEDM_ISC_TYPE_ISCMU02:
		case FEDM_ISC_TYPE_ISCLRU1000:
		case FEDM_ISC_TYPE_ISCLRU2000:
		case FEDM_ISC_TYPE_ISCLRU3000:
			sReport += "\r\n";
			sReport += "\tFrequency Information:..";
			if((ucVersion[6]&0x03) == 1)
				sReport += "EU";
			else if((ucVersion[6]&0x03) == 2)
				sReport += "FCC";
			else if((ucVersion[6]&0x03) == 3)
				sReport += "EU and FCC";
			else
				sReport += "unknown";

			break;

		case FEDM_ISC_TYPE_ISCPRHD102:
			sReport += "\r\n";
			sReport += "\tHF-Drivers:.............";
			if((ucVersion[6]&0x80))
				sReport += "enabled";
			else
				sReport += "disabled";

			sReport += "\r\n";
			sReport += "\tUHF-Drivers:............";
			if((ucVersion[6]&0x40))
				sReport += "enabled";
			else
				sReport += "disabled";

			break;

		} // switch(m_pReader->GetReaderType())

	} // if( pInfo->bIsRfcFrequency)*/

	sReport += "\r\n";
}

void FedmIscReport_ReaderInfo::AddAllInfoText(string& sReport)
{
	FEDM_ISC_READER_INFO* pInfo = m_pReader->GetReaderInfo();

	if(pInfo->bIsMode0x00Read)
		Add0x00InfoText(sReport, false);

	if(pInfo->bIsMode0x01Read)
	{
		sReport += "\n";//\n";
		Add0x01InfoText(sReport, false);
	}

	if(pInfo->bIsMode0x02Read)
	{
		sReport += "\n";//\n";
		Add0x02InfoText(sReport, false);
	}

	if(pInfo->bIsMode0x03Read)
	{
		sReport += "\n";//\n";
		Add0x03InfoText(sReport, false);
	}

	if(pInfo->bIsMode0x04Read)
	{
		sReport += "\n";//\n";
		Add0x04InfoText(sReport, false);
	}

	if(pInfo->bIsMode0x05Read)
	{
		sReport += "\n";//\n";
		Add0x05InfoText(sReport, false);
	}

	if(pInfo->bIsMode0x06Read)
	{
		sReport += "\n";//\n";
		Add0x06InfoText(sReport, false);
	}

	if(pInfo->bIsMode0x07Read)
	{
		sReport += "\n";//\n";
		Add0x07InfoText(sReport, false);
	}

	if(pInfo->bIsMode0x08Read)
	{
		sReport += "\n";//\n";
		Add0x08InfoText(sReport, false);
	}

	if(pInfo->bIsMode0x09Read)
	{
		sReport += "\n";//\n";
		Add0x09InfoText(sReport, false);
	}

	if(pInfo->bIsMode0x10Read)
	{
		sReport += "\n";//\n";
		Add0x10InfoText(sReport, false);
	}

	if(pInfo->bIsMode0x11Read)
	{
		sReport += "\n";//\n";
		Add0x11InfoText(sReport, false);
	}

	if(pInfo->bIsMode0x12Read)
	{
		sReport += "\n";//\n";
		Add0x12InfoText(sReport, false);
	}

	if(pInfo->bIsMode0x40Read)
	{
		sReport += "\n";//\n";
		Add0x40InfoText(sReport, false);
	}

	if(pInfo->bIsMode0x41Read)
	{
		sReport += "\n";//\n";
		Add0x41InfoText(sReport, false);
	}

	if(	pInfo->bIsMode0x50Read ||
		pInfo->bIsMode0x51Read ||
		pInfo->bIsMode0x52Read ||
		pInfo->bIsMode0x53Read )
	{
		sReport += "\n";//\n";
		sReport += "LAN-Information\n";
	}

	if(pInfo->bIsMode0x50Read)
		Add0x50InfoText(sReport, false);

	if( (pInfo->ucLanFlags & ((pInfo->ucLanFlags&0x03)<<2) ) == 0 ) // IPv4 and/or IPv6 not disabled
	{
		if(pInfo->bIsMode0x51Read)
			Add0x51InfoText(sReport, false);

		if(pInfo->bIsMode0x52Read)
			Add0x52InfoText(sReport, false);

		if(pInfo->bIsMode0x53Read)
			Add0x53InfoText(sReport, false);
	}

	if(	pInfo->bIsMode0x54Read ||
		pInfo->bIsMode0x55Read ||
		pInfo->bIsMode0x56Read ||
		pInfo->bIsMode0x57Read )
	{
		sReport += "\n";//\n";
		sReport += "WLAN-Information\n";
	}

	if(pInfo->bIsMode0x54Read)
		Add0x54InfoText(sReport, false);

//	if(pInfo->ucWlanFlags & 0x03) // IPv4 or IPv6 supported
	if( (pInfo->ucWlanFlags & ((pInfo->ucWlanFlags&0x03)<<2) ) == 0 ) // IPv4 and/or IPv6 not disabled
	{
		if(pInfo->bIsMode0x55Read)
			Add0x55InfoText(sReport, false);

		if(pInfo->bIsMode0x56Read)
			Add0x56InfoText(sReport, false);

		if(pInfo->bIsMode0x57Read)
			Add0x57InfoText(sReport, false);
	}

	if(pInfo->bIsMode0x60Read)
	{
		sReport += "\n";//\n";
		Add0x60InfoText(sReport, false);
	}

	if(pInfo->bIsMode0x61Read)
	{
		sReport += "\n";//\n";
		Add0x61InfoText(sReport, false);
	}

	if(pInfo->bIsMode0x80Read)
	{
		sReport += "\n";//\n";
		Add0x80InfoText(sReport, false);
	}
}
