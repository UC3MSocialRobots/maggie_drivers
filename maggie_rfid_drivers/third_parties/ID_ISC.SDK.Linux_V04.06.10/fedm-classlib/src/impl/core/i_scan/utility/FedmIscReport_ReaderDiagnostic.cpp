/*-------------------------------------------------------
|                                                       |
|				FedmIscReport_ReaderDiagnostic.cpp      |
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
Begin        		:	17.06.2009

Version       		:	04.06.01 / 05.03.2013 / M. Hultsch
						- support for new Reader ISC.PRH200
						- extended API for external diagnostic object

						04.04.00 / 05.12.2012 / M. Hultsch
						- support for Mode 0x21
						
						04.01.00 / 23.03.2011 / M. Hultsch
						- support for LR1002
						
						03.03.00 / 01.02.2011 / M. Hultsch
						- support for LR2500-A
						
						03.02.05 / 23.09.2010 / M. Hultsch
						- support for LR2500-B
						
						03.00.14 / 20.10.2009 / M. Hultsch

Operation Systems	:	independent

Function			:	report class for [0x6E] Reader Diagnostic


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#include "FedmIscReport_ReaderDiagnostic.h"
#include "../FEDM_ISCReader.h"
#include "../FEDM_ISCReaderID.h"
#include "../../FEDM_Functions.h"


FedmIscReport_ReaderDiagnostic::FedmIscReport_ReaderDiagnostic(FEDM_ISCReader* pReader)
{
	m_pReader = pReader;
}

FedmIscReport_ReaderDiagnostic::~FedmIscReport_ReaderDiagnostic()
{
}

string FedmIscReport_ReaderDiagnostic::BuildReport(unsigned char ucMode)
{
#if _MSC_VER <= 1200
	m_sReport.erase(m_sReport.begin(), m_sReport.end());
#else
	m_sReport.clear();
#endif
	BuildReport(ucMode, m_sReport);
	return m_sReport;
}

string FedmIscReport_ReaderDiagnostic::BuildReport(unsigned char ucMode, FEDM_ISC_READER_DIAGNOSTIC* pInfo)
{
#if _MSC_VER <= 1200
	m_sReport.erase(m_sReport.begin(), m_sReport.end());
#else
	m_sReport.clear();
#endif
	BuildReport(ucMode, m_sReport, pInfo);
	return m_sReport;
}

int FedmIscReport_ReaderDiagnostic::BuildReport(unsigned char ucMode, string& sReport)
{
	if(m_pReader == NULL)
		return FEDM_ERROR_NULL_POINTER;

	FEDM_ISC_READER_DIAGNOSTIC* pInfo = m_pReader->GetReaderDiagnostic();

	switch(ucMode)
	{
	case 0x01:
		if(pInfo->bIsMode0x01Read)
			Add0x01InfoText(sReport);
		break;

	case 0x02:
		if(pInfo->bIsMode0x02Read)
			Add0x02InfoText(sReport);
		break;

	case 0x03:
		if(pInfo->bIsMode0x03Read)
			Add0x03InfoText(sReport);
		break;

	case 0x04:
		if(pInfo->bIsMode0x04Read)
			Add0x04InfoText(sReport);
		break;

	case 0x05:
		if(pInfo->bIsMode0x05Read)
			Add0x05InfoText(sReport);
		break;

	case 0x06:
		if(pInfo->bIsMode0x06Read)
			Add0x06InfoText(sReport);
		break;

	case 0x07:
		if(pInfo->bIsMode0x07Read)
			Add0x07InfoText(sReport);
		break;

	case 0x20:
		if(pInfo->bIsMode0x20Read)
			Add0x20InfoText(sReport);
		break;

	case 0x21:
		if(pInfo->bIsMode0x21Read)
			Add0x21InfoText(sReport);
		break;

	case 0xFF:
		AddAllInfoText(sReport);
		break;

	default:
		break;
	}

	return FEDM_OK;
}

int FedmIscReport_ReaderDiagnostic::BuildReport(unsigned char ucMode, string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo)
{
	if(pInfo == NULL)
		return FEDM_ERROR_NULL_POINTER;

	switch(ucMode)
	{
	case 0x01:
		if(pInfo->bIsMode0x01Read)
			Add0x01InfoText(sReport, pInfo);
		break;

	case 0x02:
		if(pInfo->bIsMode0x02Read)
			Add0x02InfoText(sReport, pInfo);
		break;

	case 0x03:
		if(pInfo->bIsMode0x03Read)
			Add0x03InfoText(sReport, pInfo);
		break;

	case 0x04:
		if(pInfo->bIsMode0x04Read)
			Add0x04InfoText(sReport, pInfo);
		break;

	case 0x05:
		if(pInfo->bIsMode0x05Read)
			Add0x05InfoText(sReport, pInfo);
		break;

	case 0x06:
		if(pInfo->bIsMode0x06Read)
			Add0x06InfoText(sReport, pInfo);
		break;

	case 0x07:
		if(pInfo->bIsMode0x07Read)
			Add0x07InfoText(sReport, pInfo);
		break;

	case 0x20:
		if(pInfo->bIsMode0x20Read)
			Add0x20InfoText(sReport, pInfo);
		break;

	case 0x21:
		if(pInfo->bIsMode0x21Read)
			Add0x21InfoText(sReport, pInfo);
		break;

	case 0xFF:
		AddAllInfoText(sReport, pInfo);
		break;

	default:
		break;
	}

	return FEDM_OK;
}

void FedmIscReport_ReaderDiagnostic::Add0x01InfoText(string& sReport, bool bSingle)
{
	FEDM_ISC_READER_DIAGNOSTIC* pInfo = m_pReader->GetReaderDiagnostic();
	unsigned int uiReaderType = m_pReader->GetReaderType();
	pInfo->ucReserved3			= ((uiReaderType & 0xFF000000)>>24);
	pInfo->ucReaderTypeOfClass	= ((uiReaderType & 0x00FF0000)>>16);
	pInfo->ucReaderClassGen		= ((uiReaderType & 0x0000FF00)>> 8);
	pInfo->ucReaderClass		= ((uiReaderType & 0x000000FF));

	Add0x01InfoText(sReport, pInfo, bSingle);
}

void FedmIscReport_ReaderDiagnostic::Add0x01InfoText(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle)
{
	switch(pInfo->GetReaderType())
	{
	case FEDM_ISC_TYPE_ISCMR102:
	case FEDM_ISC_TYPE_ISCMR10X:
		Add0x01InfoText_MR102(sReport, pInfo, bSingle);
		break;
	
	case FEDM_ISC_TYPE_ISCMR200:
		Add0x01InfoText_MR200(sReport, pInfo, bSingle);
		break;
	
	case FEDM_ISC_TYPE_ISCPRH200:
		Add0x01InfoText_PRH200(sReport, pInfo, bSingle);
		break;
	
	case FEDM_ISC_TYPE_ISCLR200:
		Add0x01InfoText_LR200(sReport, pInfo, bSingle);
		break;
	
	case FEDM_ISC_TYPE_ISCLR1002:
		Add0x01InfoText_LR1002(sReport, pInfo, bSingle);
		break;

	case FEDM_ISC_TYPE_ISCLR2000:
	case FEDM_ISC_TYPE_ISCLR2500_A:
	case FEDM_ISC_TYPE_ISCLR2500_B:
		Add0x01InfoText_LR2000(sReport, pInfo, bSingle);
		break;
	
	case FEDM_ISC_TYPE_ISCMRU200:
	case FEDM_ISC_TYPE_MAXU1002:
	case FEDM_ISC_TYPE_ISCLRU1002:
	case FEDM_ISC_TYPE_ISCLRU1000:
	case FEDM_ISC_TYPE_ISCLRU2000:
	case FEDM_ISC_TYPE_ISCLRU3000:
		Add0x01InfoText_LRU1000(sReport, pInfo, bSingle);
		break;

	default:
		break;
	}
}


void FedmIscReport_ReaderDiagnostic::Add0x01InfoText_MR102(string& sReport, bool bSingle)
{
	FEDM_ISC_READER_DIAGNOSTIC* pInfo = m_pReader->GetReaderDiagnostic();
	Add0x01InfoText_MR102(sReport, pInfo, bSingle);
}

void FedmIscReport_ReaderDiagnostic::Add0x01InfoText_MR102(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle)
{
	bool bTmpAlarm		= false;
	bool bZAnt			= false;
	bool bNoise			= false;
	bool bControl		= false;
	bool bDCAntError	= false;
	unsigned char	ucData[13];

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_DIAG_DATA, &ucData[0], 13);
	}
	else
	{
		ucData[0] = pInfo->ucFlagsA;
		ucData[1] = pInfo->ucFlagsB;
	}

	
	if(ucData[0] & 0x80)
		bTmpAlarm = true;
	if(ucData[0] & 0x04)
		bZAnt = true;
	if(ucData[0] & 0x02)
		bNoise = true;
	if(ucData[0] & 0x10)
		bControl = true;

	if(ucData[1] & 0x04)
		bDCAntError = true;

	sReport += " ";
	sReport += "General RF Status:";
	sReport += "\r\n";

	if(bNoise)
		sReport += "\t1: Noise-Level..........Error\r\n";
	else
		sReport += "\t1: Noise-Level..........OK\r\n";

	if(bZAnt)
		sReport += "\t2: Ant |Z| <> 50 Ohm....Yes\r\n";
	else
		sReport += "\t2: Ant |Z| <> 50 Ohm....No\r\n";

	if(bControl)
		sReport += "\t4:  False Power.........Yes\r\n";
	else
		sReport += "\t4:  False Power.........No\r\n";


	if(bDCAntError)
		sReport += "\t10: DC-Power Antenna....Error\r\n";
	else
		sReport += "\t10: DC-Power Antenna....OK\r\n";
}


void FedmIscReport_ReaderDiagnostic::Add0x01InfoText_MR200(string& sReport, bool bSingle)
{
	FEDM_ISC_READER_DIAGNOSTIC* pInfo = m_pReader->GetReaderDiagnostic();
	Add0x01InfoText_MR200(sReport, pInfo, bSingle);
}

void FedmIscReport_ReaderDiagnostic::Add0x01InfoText_MR200(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle)
{
	bool bG50			= false;
	bool bL50			= false;
	bool bNoise			= false;
	bool bRFHardware	= false;
	bool bMux			= false;
	bool bDat			= false;
	unsigned char	ucData[13];

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_DIAG_DATA, &ucData[0], 13);
	}
	else
	{
		ucData[0] = pInfo->ucFlagsA;
		ucData[1] = pInfo->ucFlagsB;
	}

	
	if(ucData[0] & 0x08)
		bG50 = true;
	if(ucData[0] & 0x04)
		bL50 = true;
	if(ucData[0] & 0x02)
		bNoise = true;
	if(ucData[0] & 0x01)
		bRFHardware = true;

	if(ucData[1] & 0x01)
		bMux = true;
	if(ucData[1] & 0x02)
		bDat = true;

	sReport += " ";
	sReport += "General RF Status:";
	sReport += "\r\n";

	if(bRFHardware)
		sReport += "\t0: RF-Hardware..........Error\r\n";
	else
		sReport += "\t0: RF-Hardware..........OK\r\n";

	if(bNoise)
		sReport += "\t1: Noise-Level..........Error\r\n";
	else
		sReport += "\t1: Noise-Level..........OK\r\n";

	if(!bL50 && !bG50)
	{
		sReport += "\t2:.|Z|..................OK\r\n";
	}
	else
	{
		if(bG50)
			sReport += "\t2:.|Z|..................> 50 Ohm\r\n";
		else if(bL50)
			sReport += "\t2:.|Z|..................< 50 Ohm\r\n";
	}

	if(bMux)
		sReport += "\t8: Mux-Status...........OK\r\n";
	else
		sReport += "\t8: Mux-Status...........NOK\r\n";

	if(bDat)
		sReport += "\t8: DAT-Status...........OK\r\n";
	else
		sReport += "\t8: DAT-Status...........NOK\r\n";
}

void FedmIscReport_ReaderDiagnostic::Add0x01InfoText_PRH200(string& sReport, bool bSingle)
{
	FEDM_ISC_READER_DIAGNOSTIC* pInfo = m_pReader->GetReaderDiagnostic();
	Add0x01InfoText_PRH200(sReport, pInfo, bSingle);
}

void FedmIscReport_ReaderDiagnostic::Add0x01InfoText_PRH200(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle)
{
	bool bTmpAlarm				= false;
	bool bTmpWarn				= false;
	unsigned char ucBatAlarm	= 0x00;
	unsigned char	ucData[13];

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_DIAG_DATA, &ucData[0], 13);
	}
	else
	{
		ucData[0] = pInfo->ucFlagsA;
		ucData[1] = pInfo->ucFlagsB;
	}

	
	if(ucData[0] & 0x80)
		bTmpAlarm = true;
	if(ucData[0] & 0x20)
		bTmpWarn = true;

	ucBatAlarm = ((ucData[1] & 0x30) >> 4);

	sReport += " ";
	sReport += "Alarm Status:";
	sReport += "\r\n";

	if(bTmpWarn)
		sReport += "\t5: Temp-Warning.........Yes\r\n";
	else
		sReport += "\t5: Temp-Warning.........No\r\n";

	if(bTmpAlarm)
		sReport += "\t7: Temp-Alarm...........Yes\r\n";
	else
		sReport += "\t7: Temp-Alarm...........No\r\n";

	switch(ucBatAlarm)
	{
	case 0:
		sReport += "\t8: Battery-Level........OK\r\n";
		break;
	case 1:
		sReport += "\t8: Battery-Level.......<10%\r\n";
		break;
	case 2:
		sReport += "\t8: Battery-Level.......<5%\r\n";
		break;
	case 3:
		sReport += "\t8: Battery-Level.......<1%\r\n";
		break;
	}
}

void FedmIscReport_ReaderDiagnostic::Add0x01InfoText_LR200(string& sReport, bool bSingle)
{
	FEDM_ISC_READER_DIAGNOSTIC* pInfo = m_pReader->GetReaderDiagnostic();
	Add0x01InfoText_LR200(sReport, pInfo, bSingle);
}

void FedmIscReport_ReaderDiagnostic::Add0x01InfoText_LR200(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle)
{
	bool bTmpAlarm		= false;
	bool bBit6			= false;
	bool bTmpWarn		= false;
	bool bControl		= false;
	bool bG50			= false;
	bool bL50			= false;
	bool bNoise			= false;
	bool bRFHardware	= false;
	bool bMux			= false;
	unsigned char	ucData[13];

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_DIAG_DATA, &ucData[0], 13);
	}
	else
	{
		ucData[0] = pInfo->ucFlagsA;
		ucData[1] = pInfo->ucFlagsB;
	}

	
	if(ucData[0] & 0x80)
		bTmpAlarm = true;
	if(ucData[0] & 0x40)
		bBit6 = true;
	if(ucData[0] & 0x20)
		bTmpWarn = true;
	if(ucData[0] & 0x10)
		bControl = true;
	if(ucData[0] & 0x08)
		bG50 = true;
	if(ucData[0] & 0x04)
		bL50 = true;
	if(ucData[0] & 0x02)
		bNoise = true;
	if(ucData[0] & 0x01)
		bRFHardware = true;

	if(ucData[1] & 0x01)
		bMux = true;

	sReport += " ";
	sReport += "General RF Status:";
	sReport += "\r\n";

	if(bRFHardware)
		sReport += "\t0: RF-Hardware..........Error\r\n";
	else
		sReport += "\t0: RF-Hardware..........OK\r\n";

	if(bNoise)
		sReport += "\t1: Noise-Level..........Error\r\n";
	else
		sReport += "\t1: Noise-Level..........OK\r\n";

	if(!bL50 && !bG50)
	{
		sReport += "\t2:.|Z|..................OK\r\n";
	}
	else
	{
		if(bG50)
			sReport += "\t2:.|Z|..................> 50 Ohm\r\n";
		else if(bL50)
			sReport += "\t2:.|Z|..................< 50 Ohm\r\n";
	}

	if(bControl)
		sReport += "\t4: False Power..........Yes\r\n";
	else
		sReport += "\t4: False Power..........No\r\n";

	if(bTmpWarn)
		sReport += "\t5: Temp-Warning.........Yes\r\n";
	else
		sReport += "\t5: Temp-Warning.........No\r\n";

	if(bBit6)
		sReport += "\t6: .....................True\r\n";
	else
		sReport += "\t6: .....................False\r\n";

	if(bTmpAlarm)
		sReport += "\t7: Temp-Alarm...........Yes\r\n";
	else
		sReport += "\t7: Temp-Alarm...........No\r\n";

	if(bMux)
		sReport += "\t8: MUX-Status...........NOK\r\n";
	else
		sReport += "\t8: MUX-Status...........OK\r\n";
}


void FedmIscReport_ReaderDiagnostic::Add0x01InfoText_LR1002(string& sReport, bool bSingle)
{
	FEDM_ISC_READER_DIAGNOSTIC* pInfo = m_pReader->GetReaderDiagnostic();
	Add0x01InfoText_LR1002(sReport, pInfo, bSingle);
}

void FedmIscReport_ReaderDiagnostic::Add0x01InfoText_LR1002(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle)
{
	bool bTmpAlarm		= false;
	bool bTmpWarn		= false;
	bool bControl		= false;
	bool bZAnt			= false;
	bool bNoise			= false;
	bool bRFHardware	= false;
	bool bMux			= false;
	bool bDat			= false;
	bool bDCAntError	= false;
	unsigned char	ucData[13];

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_DIAG_DATA, &ucData[0], 13);
	}
	else
	{
		ucData[0] = pInfo->ucFlagsA;
		ucData[1] = pInfo->ucFlagsB;
	}

	
	if(ucData[0] & 0x80)
		bTmpAlarm = true;
	if(ucData[0] & 0x20)
		bTmpWarn = true;
	if(ucData[0] & 0x10)
		bControl = true;
	if(ucData[0] & 0x04)
		bZAnt = true;
	if(ucData[0] & 0x02)
		bNoise = true;
	if(ucData[0] & 0x01)
		bRFHardware = true;

	if(ucData[1] & 0x01)
		bMux = true;
	if(ucData[1] & 0x02)
		bDat = true;
	if(ucData[1] & 0x04)
		bDCAntError = true;

	sReport += " ";
	sReport += "General RF Status:";
	sReport += "\r\n";

	if(bRFHardware)
		sReport += "\t0:  RF-Hardware.........Error\r\n";
	else
		sReport += "\t0:  RF-Hardware.........OK\r\n";

	if(bNoise)
		sReport += "\t1:  Noise-Level.........Error\r\n";
	else
		sReport += "\t1:  Noise-Level.........OK\r\n";

	if(bZAnt)
		sReport += "\t2:  Ant |Z| <> 50 Ohm...Yes\r\n";
	else
		sReport += "\t2:  Ant |Z| <> 50 Ohm...No\r\n";

	if(bControl)
		sReport += "\t4:  False Power.........Yes\r\n";
	else
		sReport += "\t4:  False Power.........No\r\n";

	if(bTmpWarn)
		sReport += "\t5:  Temp-Warning........Yes\r\n";
	else
		sReport += "\t5:  Temp-Warning........No\r\n";

	if(bTmpAlarm)
		sReport += "\t7:  Temp-Alarm..........Yes\r\n";
	else
		sReport += "\t7:  Temp-Alarm..........No\r\n";

	if(bMux)
		sReport += "\t8:  MUX-Status..........NOK\r\n";
	else
		sReport += "\t8:  MUX-Status..........OK\r\n";

	if(bDat)
		sReport += "\t9:  DAT-Status..........NOK\r\n";
	else
		sReport += "\t9:  DAT-Status..........OK\r\n";


	if(bDCAntError)
		sReport += "\t10: DC-Power Antenna....Error\r\n";
	else
		sReport += "\t10: DC-Power Antenna....OK\r\n";
}

void FedmIscReport_ReaderDiagnostic::Add0x01InfoText_LR2000(string& sReport, bool bSingle)
{
	FEDM_ISC_READER_DIAGNOSTIC* pInfo = m_pReader->GetReaderDiagnostic();
	Add0x01InfoText_LR2000(sReport, pInfo, bSingle);
}

void FedmIscReport_ReaderDiagnostic::Add0x01InfoText_LR2000(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle)
{
	bool bTmpAlarm		= false;
	bool bSync			= false;
	bool bTmpWarn		= false;
	bool bControl		= false;
	bool bG50			= false;
	bool bL50			= false;
	bool bNoise			= false;
	bool bRFHardware	= false;
	bool bMux			= false;
	bool bDat			= false;
	unsigned char	ucData[13];

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_DIAG_DATA, &ucData[0], 13);
	}
	else
	{
		ucData[0] = pInfo->ucFlagsA;
		ucData[1] = pInfo->ucFlagsB;
	}

	
	if(ucData[0] & 0x80)
		bTmpAlarm = true;
	if(ucData[0] & 0x40)
		bSync = true;
	if(ucData[0] & 0x20)
		bTmpWarn = true;
	if(ucData[0] & 0x10)
		bControl = true;
	if(ucData[0] & 0x08)
		bG50 = true;
	if(ucData[0] & 0x04)
		bL50 = true;
	if(ucData[0] & 0x02)
		bNoise = true;
	if(ucData[0] & 0x01)
		bRFHardware = true;

	if(ucData[1] & 0x01)
		bMux = true;
	if(ucData[1] & 0x02)
		bDat = true;

	sReport += " ";
	sReport += "General RF Status:";
	sReport += "\r\n";

	if(bRFHardware)
		sReport += "\t0: RF-Hardware..........Error\r\n";
	else
		sReport += "\t0: RF-Hardware..........OK\r\n";

	if(bNoise)
		sReport += "\t1: .....................Error\r\n";
	else
		sReport += "\t1: .....................OK\r\n";

	if(!bL50 && !bG50)
	{
		sReport += "\t2:.|Z|..................OK\r\n";
	}
	else
	{
		if(bG50 && bL50)
		{
			sReport += "\t2:.|Z|..................Phase Error\r\n";
		}
		else
		{
			if(bG50)
				sReport += "\t2:.|Z|..................> 50 Ohm\r\n";
			else if(bL50)
				sReport += "\t2:.|Z|..................< 50 Ohm\r\n";
		}
	}

	if(bControl)
		sReport += "\t4: False Power..........Yes\r\n";
	else
		sReport += "\t4: False Power..........No\r\n";

	if(bTmpWarn)
		sReport += "\t5: Temp-Warning.........Yes\r\n";
	else
		sReport += "\t5: Temp-Warning.........No\r\n";

	if(bSync)
		sReport += "\t6: Sync.................Timeout\r\n";
	else
		sReport += "\t6: Sync.................OK\r\n";

	if(bTmpAlarm)
		sReport += "\t7: Temp-Alarm...........Yes\r\n";
	else
		sReport += "\t7: Temp-Alarm...........No\r\n";

	if(bMux)
		sReport += "\t8: MUX-Status...........NOK\r\n";
	else
		sReport += "\t8: MUX-Status...........OK\r\n";

	if(bDat)
		sReport += "\t9: DAT-Status...........NOK\r\n";
	else
		sReport += "\t9: DAT-Status...........OK\r\n";
}

void FedmIscReport_ReaderDiagnostic::Add0x01InfoText_LRU1000(string& sReport, bool bSingle)
{
	FEDM_ISC_READER_DIAGNOSTIC* pInfo = m_pReader->GetReaderDiagnostic();
	Add0x01InfoText_LRU1000(sReport, pInfo, bSingle);
}

void FedmIscReport_ReaderDiagnostic::Add0x01InfoText_LRU1000(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle)
{
	bool bChAlloc		= false;
	bool bControl		= false;
	bool bNoise			= false;
	bool bRFHardware	= false;
	bool bZAnt4			= false;
	bool bZAnt3			= false;
	bool bZAnt2			= false;
	bool bZAnt1			= false;
	unsigned char	ucData[13];

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_DIAG_DATA, &ucData[0], 13);
	}
	else
	{
		ucData[0] = pInfo->ucFlagsA;
		ucData[1] = pInfo->ucFlagsB;
	}
	
	if(ucData[0] & 0x20)
		bChAlloc = true;
	if(ucData[0] & 0x10)
		bControl = true;
	if(ucData[0] & 0x02)
		bNoise = true;
	if(ucData[0] & 0x01)
		bRFHardware = true;

	if(ucData[1] & 0x08)
		bZAnt4 = true;
	if(ucData[1] & 0x04)
		bZAnt3 = true;
	if(ucData[1] & 0x02)
		bZAnt2 = true;
	if(ucData[1] & 0x01)
		bZAnt1 = true;

	sReport += " ";
	sReport += "General RF Status:";
	sReport += "\r\n";

	if(bRFHardware)
		sReport += "\t0: RF-Hardware..........Error\r\n";
	else
		sReport += "\t0: RF-Hardware..........OK\r\n";

	if(bNoise)
		sReport += "\t1: Noise-Level..........Error\r\n";
	else
		sReport += "\t1: Noise-Level..........OK\r\n";

	if(bControl)
		sReport += "\t4: False Power..........Yes\r\n";
	else
		sReport += "\t4: False Power..........No\r\n";

	if(bChAlloc)
		sReport += "\t5: Channel Allocation...Yes\r\n";
	else
		sReport += "\t5: Channel Allocation...No\r\n";

	sReport += "\r\n";

	if(bZAnt1)
		sReport += "\t0: Ant1 |Z| <> 50 Ohm...Yes\r\n";
	else
		sReport += "\t0: Ant1 |Z| <> 50 Ohm...No\r\n";

	if(bZAnt2)
		sReport += "\t1: Ant2 |Z| <> 50 Ohm...Yes\r\n";
	else
		sReport += "\t1: Ant2 |Z| <> 50 Ohm...No\r\n";

	if(m_pReader->GetReaderType() != FEDM_ISC_TYPE_ISCMRU200)
	{
		if(bZAnt3)
			sReport += "\t2: Ant3 |Z| <> 50 Ohm...Yes\r\n";
		else
			sReport += "\t2: Ant3 |Z| <> 50 Ohm...No\r\n";

		if(bZAnt4)
			sReport += "\t3: Ant4 |Z| <> 50 Ohm...Yes\r\n";
		else
			sReport += "\t3: Ant4 |Z| <> 50 Ohm...No\r\n";
	}
}

void FedmIscReport_ReaderDiagnostic::Add0x02InfoText(string& sReport, bool bSingle)
{
	FEDM_ISC_READER_DIAGNOSTIC* pInfo = m_pReader->GetReaderDiagnostic();
	Add0x02InfoText(sReport, pInfo, bSingle);
}

void FedmIscReport_ReaderDiagnostic::Add0x02InfoText(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle)
{
	char cBuffer[16];
	unsigned char	ucData[13];
	unsigned int uiImpedance = 0;

	unsigned int uiReaderType = m_pReader->GetReaderType();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_DIAG_DATA, &ucData[0], 13);
		if(uiReaderType == FEDM_ISC_TYPE_ISCMR200)
		{
			uiImpedance = (((unsigned int)ucData[0])<<8) + ucData[1];
		}
	}
	else
	{
		switch(uiReaderType)
		{
		case FEDM_ISC_TYPE_ISCMR200:
			uiImpedance = pInfo->uiImpedance;
			break;

		case FEDM_ISC_TYPE_ISCLR200:
		case FEDM_ISC_TYPE_ISCLR2000:
			ucData[0] = pInfo->ucRfPower;
			ucData[1] = pInfo->ucRfModulation;
			ucData[2] = pInfo->ucAmpTemp;
			break;
		}
	}


	if(!bSingle)
		sReport += "\r\n";

	switch(uiReaderType)
	{
	case FEDM_ISC_TYPE_ISCLR2000:
		sprintf(cBuffer, "%u", (unsigned int)ucData[2]);
		sReport += "\tTemperature of RF final stage.: ";
		sReport += cBuffer;
		sReport += " (°C)\r\n";
		break;
	}
}

void FedmIscReport_ReaderDiagnostic::Add0x03InfoText(string& sReport, bool bSingle)
{
	FEDM_ISC_READER_DIAGNOSTIC* pInfo = m_pReader->GetReaderDiagnostic();
	Add0x03InfoText(sReport, pInfo, bSingle);
}

void FedmIscReport_ReaderDiagnostic::Add0x03InfoText(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle)
{
	char cBuffer[16];
	unsigned char	ucData[13];

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_DIAG_DATA, &ucData[0], 13);
	}
	else
	{
		ucData[0] = pInfo->ucMuxStatusCh1;
		ucData[1] = pInfo->ucMuxStatusCh2;
		ucData[2] = pInfo->ucMuxStatusCh3;
		ucData[3] = pInfo->ucMuxStatusCh4;
		ucData[4] = pInfo->ucMuxStatusCh5;
		ucData[5] = pInfo->ucMuxStatusCh6;
		ucData[6] = pInfo->ucMuxStatusCh7;
		ucData[7] = pInfo->ucMuxStatusCh8;
	}


	if(!bSingle)
		sReport += "\r\n";

	sReport += " ";
	sReport += "Multiplexer Status:";
	sReport += "\r\n";

	sprintf(cBuffer, "0x%02X\r\n", ucData[0]);
	sReport += "\tMUX-Status Channel 1....";
	sReport += cBuffer;
	sprintf(cBuffer, "0x%02X\r\n", ucData[1]);
	sReport += "\tMUX-Status Channel 2....";
	sReport += cBuffer;
	sprintf(cBuffer, "0x%02X\r\n", ucData[2]);
	sReport += "\tMUX-Status Channel 3....";
	sReport += cBuffer;
	sprintf(cBuffer, "0x%02X\r\n", ucData[3]);
	sReport += "\tMUX-Status Channel 4....";
	sReport += cBuffer;
	sprintf(cBuffer, "0x%02X\r\n", ucData[4]);
	sReport += "\tMUX-Status Channel 5....";
	sReport += cBuffer;
	sprintf(cBuffer, "0x%02X\r\n", ucData[5]);
	sReport += "\tMUX-Status Channel 6....";
	sReport += cBuffer;
	sprintf(cBuffer, "0x%02X\r\n", ucData[6]);
	sReport += "\tMUX-Status Channel 7....";
	sReport += cBuffer;
	sprintf(cBuffer, "0x%02X\r\n", ucData[7]);
	sReport += "\tMUX-Status Channel 8....";
	sReport += cBuffer;
}

void FedmIscReport_ReaderDiagnostic::Add0x04InfoText(string& sReport, bool bSingle)
{
	FEDM_ISC_READER_DIAGNOSTIC* pInfo = m_pReader->GetReaderDiagnostic();
	Add0x04InfoText(sReport, pInfo, bSingle);
}

void FedmIscReport_ReaderDiagnostic::Add0x04InfoText(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle)
{
	bool bEEDev1		= false;
	bool bEEDev2		= false;
	bool bFilter		= false;
	bool bRFDecoder		= false;
	bool bParaMismatch	= false;
	bool bPeopleCounterError	= false;
	bool bExt_ADC_DAC_Error = false;
	unsigned char	ucData[13];

	unsigned int uiReaderType = m_pReader->GetReaderType();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_DIAG_DATA, &ucData[0], 13);
	}
	else
	{
		ucData[0] = pInfo->ucIntErrorA;
		ucData[1] = pInfo->ucIntErrorB;
	}

	
	if(ucData[1] & 0x01)
		bEEDev1 = true;
	if(ucData[1] & 0x02)
		bEEDev2 = true;
	if(ucData[1] & 0x04)
		bFilter = true;
	if(ucData[1] & 0x08)
		bRFDecoder = true;
	if(ucData[1] & 0x10)
		bParaMismatch = true;
	if(ucData[1] & 0x20)
		bPeopleCounterError = true;
	if(ucData[1] & 0x40)
		bExt_ADC_DAC_Error = true;

	if(!bSingle)
		sReport += "\r\n";

	sReport += " ";
	sReport += "Hardware and Configuration Status:";
	sReport += "\r\n";

	sReport += "\tCommunication with...\r\n";

	if(bEEDev1)
		sReport += "\t0: EEPROM-Device1.......Error\r\n";
	else
		sReport += "\t0: EEPROM-Device1.......OK\r\n";

	switch(uiReaderType)
	{
	case FEDM_ISC_TYPE_ISCPRH102:
	case FEDM_ISC_TYPE_ISCPRH200:
	case FEDM_ISC_TYPE_ISCPRHD102:
	case FEDM_ISC_TYPE_ISCMU02:
	case FEDM_ISC_TYPE_ISCMR102:
	case FEDM_ISC_TYPE_ISCMR10X:
	case FEDM_ISC_TYPE_MAXU1002:
	case FEDM_ISC_TYPE_ISCLRU1002:
		break;

	default:
		if(bEEDev2)
			sReport += "\t1: EEPROM-Device2.......Error\r\n";
		else
			sReport += "\t1: EEPROM-Device2.......OK\r\n";
		break;
	}

	switch(uiReaderType)
	{
	case FEDM_ISC_TYPE_MAXU1002:
	case FEDM_ISC_TYPE_ISCLRU1002:
		break;

	case FEDM_ISC_TYPE_ISCLR2000:
	case FEDM_ISC_TYPE_ISCLR2500_A:
	case FEDM_ISC_TYPE_ISCLR2500_B:
		if(bFilter)
			sReport += "\t2: RF-Power.............Out of Control Range\r\n";
		else
			sReport += "\t2: RF-Power.............OK\r\n";
		break;

	case FEDM_ISC_TYPE_ISCLRU2000:
	case FEDM_ISC_TYPE_ISCLRU3000:
		if(bFilter)
			sReport += "\t2: Hardware-Filter......Error\r\n";
		else
			sReport += "\t2: Hardware-Filter......OK\r\n";
		break;
	}

	switch(uiReaderType)
	{
	case FEDM_ISC_TYPE_ISCPRH200:
	case FEDM_ISC_TYPE_ISCMR102:
	case FEDM_ISC_TYPE_ISCMR10X:
		break;

	default:
		if(bRFDecoder)
			sReport += "\t3: RF-Decoder...........Error\r\n";
		else
			sReport += "\t3: RF-Decoder...........OK\r\n";
		break;
	}

	switch(uiReaderType)
	{
	case FEDM_ISC_TYPE_MAXU1002:
	case FEDM_ISC_TYPE_ISCLRU1002:
		break;

	case FEDM_ISC_TYPE_ISCLRU2000:
	case FEDM_ISC_TYPE_ISCLRU3000:
		if(bParaMismatch)
			sReport += "\t4: Parameter Mismatch detected\r\n";
		break;
	}

	switch(uiReaderType)
	{
	case FEDM_ISC_TYPE_MAXU1002:
	case FEDM_ISC_TYPE_ISCLRU1002:
		break;

	case FEDM_ISC_TYPE_ISCLR2500_A:
	case FEDM_ISC_TYPE_ISCLR2500_B:
		if(bPeopleCounterError)
			sReport += "\t5: People Counter.......Error\r\n";
		else
			sReport += "\t5: People Counter.......OK\r\n";
		break;
	}


	switch(uiReaderType)
	{
	case FEDM_ISC_TYPE_ISCLRU3000:
		if(bExt_ADC_DAC_Error)
			sReport += "\t6: Peripheral DAC/ADC Error detected\r\n";
		break;
	}
}

void FedmIscReport_ReaderDiagnostic::Add0x05InfoText(string& sReport, bool bSingle)
{
	FEDM_ISC_READER_DIAGNOSTIC* pInfo = m_pReader->GetReaderDiagnostic();
	Add0x05InfoText(sReport, pInfo, bSingle);
}

void FedmIscReport_ReaderDiagnostic::Add0x05InfoText(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle)
{
	char cBuffer[16];
	unsigned char	ucData[13];
	
	unsigned int uiReaderType = m_pReader->GetReaderType();

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_DIAG_DATA, &ucData[0], 13);
	}
	else
	{
		ucData[0] = pInfo->CNTRL_OUT1.ucMux_PowerAmp;
		ucData[1] = pInfo->PHASE_LT50_OUT1.ucMux_PhaseError_Less;
		ucData[2] = pInfo->PHASE_GT50_OUT1.ucMux_PhaseError_More;
		ucData[3] = pInfo->CNTRL_OUT2.ucMux_PowerAmp;
		ucData[4] = pInfo->PHASE_LT50_OUT2.ucMux_PhaseError_Less;
		ucData[5] = pInfo->PHASE_GT50_OUT2.ucMux_PhaseError_More;
		ucData[6] = pInfo->CNTRL_OUT3.ucMux_PowerAmp;
		ucData[7] = pInfo->PHASE_LT50_OUT3.ucMux_PhaseError_Less;
		ucData[8] = pInfo->PHASE_GT50_OUT3.ucMux_PhaseError_More;
		ucData[9] = pInfo->CNTRL_OUT4.ucMux_PowerAmp;
		ucData[10] = pInfo->PHASE_LT50_OUT4.ucMux_PhaseError_Less;
		ucData[11] = pInfo->PHASE_GT50_OUT4.ucMux_PhaseError_More;
	}


	if(!bSingle)
		sReport += "\r\n";

	sReport += " ";
	sReport += "Status of external Multiplexer:";
	sReport += "\r\n";

	switch(uiReaderType)
	{
	case FEDM_ISC_TYPE_ISCLR1002:
	case FEDM_ISC_TYPE_ISCLR2000:
	case FEDM_ISC_TYPE_ISCLR2500_A:
	case FEDM_ISC_TYPE_ISCLR2500_B:
		sprintf(cBuffer, "0x%02X\r\n", ucData[0]);
		sReport += "\tPower-Status for ext. MUX at Output1...............";
		sReport += cBuffer;
		sprintf(cBuffer, "0x%02X\r\n", ucData[1]);
		sReport += "\tImpedance-Status (<50R) for ext. MUX at Output1....";
		sReport += cBuffer;
		sprintf(cBuffer, "0x%02X\r\n", ucData[2]);
		sReport += "\tImpedance-Status (>50R) for ext. MUX at Output1....";
		sReport += cBuffer;
		break;
	case FEDM_ISC_TYPE_ISCLRU2000:
	case FEDM_ISC_TYPE_ISCLRU3000:
		sprintf(cBuffer, "0x%02X\r\n", ucData[0]);
		sReport += "\tPower-Status for ext. MUX at Output1...............";
		sReport += cBuffer;
		sprintf(cBuffer, "0x%02X\r\n", ucData[3]);
		sReport += "\tPower-Status for ext. MUX at Output2...............";
		sReport += cBuffer;
		sprintf(cBuffer, "0x%02X\r\n", ucData[6]);
		sReport += "\tPower-Status for ext. MUX at Output3...............";
		sReport += cBuffer;
		sprintf(cBuffer, "0x%02X\r\n", ucData[9]);
		sReport += "\tPower-Status for ext. MUX at Output4...............";
		sReport += cBuffer;
		break;
	}
}

void FedmIscReport_ReaderDiagnostic::Add0x06InfoText(string& sReport, bool bSingle)
{
	FEDM_ISC_READER_DIAGNOSTIC* pInfo = m_pReader->GetReaderDiagnostic();
	Add0x06InfoText(sReport, pInfo, bSingle);
}

void FedmIscReport_ReaderDiagnostic::Add0x06InfoText(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle)
{
	char cBuffer[16];
	unsigned char	ucData[13];

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_DIAG_DATA, &ucData[0], 13);
	}
	else
	{
		memcpy(&ucData[0], (char*)&pInfo->usErrorFlags, 6);
	}

	if(!bSingle)
		sReport += "\r\n";

	sReport += " ";
	sReport += "MAX Status:";
	sReport += "\r\n";

	sReport += "\tError Flags...\r\n";

	if(ucData[1] & 0x01)
		sReport += "\t 0: Real-Time Clock........Error\r\n";
	else
		sReport += "\t 0: Real-Time Clock........OK\r\n";

	if(ucData[1] & 0x02)
		sReport += "\t 1: Metadata...............Error\r\n";
	else
		sReport += "\t 1: Metadata...............OK\r\n";

	if(ucData[1] & 0x04)
		sReport += "\t 2: Timezone Table.........Error\r\n";
	else
		sReport += "\t 2: Timezone Table.........OK\r\n";

	if(ucData[1] & 0x08)
		sReport += "\t 3: Holiday Table..........Error\r\n";
	else
		sReport += "\t 3: Holiday Table..........OK\r\n";

	if(ucData[1] & 0x10)
		sReport += "\t 4: Access Table...........Error\r\n";
	else
		sReport += "\t 4: Access Table...........OK\r\n";

	if(ucData[1] & 0x40)
		sReport += "\t 6: Reader Config..........Error\r\n";
	else
		sReport += "\t 6: Reader Config..........OK\r\n";

	if(ucData[1] & 0x80)
		sReport += "\t 7: Table Update...........Error\r\n";
	else
		sReport += "\t 7: Table Update...........OK\r\n";

	if(ucData[0] & 0x01)
		sReport += "\t15: Event-Table Overflow...Yes\r\n";
	else
		sReport += "\t15: Event-Table Overflow...No\r\n";

	sReport += "\r\n";
	sprintf(cBuffer, "%u\r\n", (((unsigned int)ucData[2])<<8) + ucData[3]);
	sReport += "\tEvent-Table-Size...........";
	sReport += cBuffer;

	sprintf(cBuffer, "%u\r\n", (((unsigned int)ucData[4])<<8) + ucData[5]);
	sReport += "\tEvent-Table-Length.........";
	sReport += cBuffer;
}

void FedmIscReport_ReaderDiagnostic::Add0x07InfoText(string& sReport, bool bSingle)
{
	FEDM_ISC_READER_DIAGNOSTIC* pInfo = m_pReader->GetReaderDiagnostic();
	Add0x07InfoText(sReport, pInfo, bSingle);
}

void FedmIscReport_ReaderDiagnostic::Add0x07InfoText(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle)
{
	unsigned char ucData[30];

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_DIAG_DATA, &ucData[0], 30);
	}
	else
	{
		memcpy(&ucData[0], (char*)&pInfo->ucNoOfLEDs, 30);
	}

	if(!bSingle)
		sReport += "\r\n";

	sReport += " ";
	sReport += "LED Status:";
	sReport += "\r\n";


	// LED for RF-On
	switch(ucData[1] & 0x03)
	{
	case 0: // off
		sReport += "\tRF-On (green)...............off";
		break;
	case 1: // on
		sReport += "\tRF-On (green)...............on";
		break;
	case 2: // flashing
		sReport += "\tRF-On (green)...............(flashing)";
		break;
	default: // unknown
		sReport += "\tRF-On (green)...............(unknown value)";
	}
	sReport += "\r\n";

	// LED for Tag detect
	switch( (ucData[1] & 0x30) >> 4)
	{
	case 0: // off
		sReport += "\tTag detected (blue).........no";
		break;
	case 1: // on
		sReport += "\tTag detected (blue).........yes";
		break;
	case 2: // flashing
		sReport += "\tTag detected (blue).........(flashing)";
		break;
	default: // unknown
		sReport += "\tTag detected (blue).........(unknown value)";
	}
	sReport += "\r\n";

	// LED for Special Function
	switch(ucData[2] & 0x03)
	{
	case 0: // off
		sReport += "\tSpecial Function (green)....disabled";
		break;
	case 1: // on
		sReport += "\tSpecial Function (green)....enabled";
		break;
	case 2: // flashing
		sReport += "\tSpecial Function (green)....(flashing)";
		break;
	default: // unknown
		sReport += "\tSpecial Function (green)....(unknown value)";
	}
	sReport += "\r\n";

	// LED for Tag detected with Special Function
	switch( (ucData[2] & 0x30) >> 4)
	{
	case 0: // off
		sReport += "\tSF: Tag detected (red)......no";
		break;
	case 1: // on
		sReport += "\tSF: Tag detected (red)......yes";
		break;
	case 2: // flashing
		sReport += "\tSF: Tag detected (red)......(flashing)";
		break;
	default: // unknown
		sReport += "\tSF: Tag detected (red)......(unknown value)";
	}
	sReport += "\r\n";

	// LED for Wifi
	switch(ucData[3] & 0x03)
	{
	case 0: // off
		sReport += "\tWiFi Status (white).........connected";
		break;
	case 1: // on
		sReport += "\tWiFi Status (white).........(on)";
		break;
	case 2: // flashing
		sReport += "\tWiFi Status (white).........ambiguous";
		break;
	default: // unknown
		sReport += "\tWiFi Status (white).........(unknown value)";
	}
	sReport += "\r\n";

	// LED for Battery
	switch(ucData[4])
	{
	case 0x00: // off
		sReport += "\tBattery Status..............OK";
		break;
	case 0x01: // orange on
		sReport += "\tBattery Status (orange).....<15% of charge capacity";
		break;
	case 0x10: // red on
		sReport += "\tBattery Status (red)........empty; no RF";
		break;
	case 0x20: // red flashing
		sReport += "\tBattery Status (red flash)..automatic-off";
		break;
	default: // unknown
		sReport += "\tBattery Status...............(unknown value)";
	}
	sReport += "\r\n";

	// LED for Normal Mode
	switch(ucData[5])
	{
	case 0x00: // off
		sReport += "\tMode........................(off)";
		break;
	case 0x01: // green on
		sReport += "\tMode (green)................Normal";
		break;
	case 0x10: // white on
		sReport += "\tMode (white)................Boost";
		break;
	default: // unknown
		sReport += "\tMode........................(unknown value)";
	}
	sReport += "\r\n";
}

void FedmIscReport_ReaderDiagnostic::Add0x20InfoText(string& sReport, bool bSingle)
{
	FEDM_ISC_READER_DIAGNOSTIC* pInfo = m_pReader->GetReaderDiagnostic();
	Add0x20InfoText(sReport, pInfo, bSingle);
}

void FedmIscReport_ReaderDiagnostic::Add0x20InfoText(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle)
{
	unsigned char ucData[31];

	memset(ucData, 0, 31);

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_DIAG_DATA, ucData, 31);
	}
	else
	{
		memcpy(ucData, pInfo->ucFirmwareStatus, 31);
	}


	if(!bSingle)
		sReport += "\r\n";

	sReport += " ";
	sReport += "Firmware Status:";
	sReport += "\r\n";

	sReport += "\tResult of Firmware Update:\t";
	sReport += (char*)ucData;
	sReport += "\r\n";
}

void FedmIscReport_ReaderDiagnostic::Add0x21InfoText(string& sReport, bool bSingle)
{
	FEDM_ISC_READER_DIAGNOSTIC* pInfo = m_pReader->GetReaderDiagnostic();
	Add0x21InfoText(sReport, pInfo, bSingle);
}

void FedmIscReport_ReaderDiagnostic::Add0x21InfoText(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle)
{
	unsigned char ucData[31];

	memset(ucData, 0, 31);

	if(bSingle)
	{
		m_pReader->GetData(FEDM_ISC_TMP_DIAG_DATA, ucData, 31);
	}
	else
	{
		ucData[0] = pInfo->ucConfigParaChange;
	}


	if(!bSingle)
		sReport += "\r\n";

	sReport += " ";
	sReport += "Status of Configuration Parameter:";
	sReport += "\r\n";

	if(ucData[0] == 0x00)
		sReport += "\tParameter unchanged\t";
	else if(ucData[0] == 0x01)
		sReport += "\tParameter changed after Firmware Update:\t";

	sReport += "\r\n";
}

void FedmIscReport_ReaderDiagnostic::AddAllInfoText(string& sReport)
{
	FEDM_ISC_READER_DIAGNOSTIC* pInfo = m_pReader->GetReaderDiagnostic();
	unsigned int uiReaderType = m_pReader->GetReaderType();
	pInfo->ucReserved3			= ((uiReaderType & 0xFF000000)>>24);
	pInfo->ucReaderTypeOfClass	= ((uiReaderType & 0x00FF0000)>>16);
	pInfo->ucReaderClassGen		= ((uiReaderType & 0x0000FF00)>> 8);
	pInfo->ucReaderClass		= ((uiReaderType & 0x000000FF));

	AddAllInfoText(sReport, pInfo);
}

void FedmIscReport_ReaderDiagnostic::AddAllInfoText(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo)
{
	unsigned int uiReaderType = pInfo->GetReaderType();

	if(pInfo->bIsMode0x01Read)
	{
		switch(uiReaderType)
		{
		case FEDM_ISC_TYPE_ISCMR102:
		case FEDM_ISC_TYPE_ISCMR10X:
			Add0x01InfoText_MR102(sReport, pInfo, false);
			break;

		case FEDM_ISC_TYPE_ISCMR200:
			Add0x01InfoText_MR200(sReport, pInfo, false);
			break;

		case FEDM_ISC_TYPE_ISCLR200:
			Add0x01InfoText_LR200(sReport, pInfo, false);
			break;

		case FEDM_ISC_TYPE_ISCPRH200:
			Add0x01InfoText_PRH200(sReport, pInfo, false);
			break;
	
		case FEDM_ISC_TYPE_ISCLR1002:
			Add0x01InfoText_LR1002(sReport, pInfo, false);
			break;

		case FEDM_ISC_TYPE_ISCLR2000:
		case FEDM_ISC_TYPE_ISCLR2500_A:
		case FEDM_ISC_TYPE_ISCLR2500_B:
			Add0x01InfoText_LR2000(sReport, pInfo, false);
			break;

		case FEDM_ISC_TYPE_ISCMRU200:
		case FEDM_ISC_TYPE_ISCLRU1000:
		case FEDM_ISC_TYPE_MAXU1002:
		case FEDM_ISC_TYPE_ISCLRU1002:
		case FEDM_ISC_TYPE_ISCLRU2000:
		case FEDM_ISC_TYPE_ISCLRU3000:
			Add0x01InfoText_LRU1000(sReport, pInfo, false);
			break;
		}
	}

	if(pInfo->bIsMode0x02Read)
	{
		Add0x02InfoText(sReport, pInfo, false);
	}
	
	if(pInfo->bIsMode0x03Read)
	{
		Add0x03InfoText(sReport, pInfo, false);
	}
	
	if(pInfo->bIsMode0x04Read)
	{
		Add0x04InfoText(sReport, pInfo, false);
	}
	
	if(pInfo->bIsMode0x05Read)
	{
		Add0x05InfoText(sReport, pInfo, false);
	}
	
	if(pInfo->bIsMode0x06Read)
	{
		Add0x06InfoText(sReport, pInfo, false);
	}
	
	if(pInfo->bIsMode0x07Read)
	{
		Add0x07InfoText(sReport, pInfo, false);
	}
	
	if(pInfo->bIsMode0x20Read)
	{
		Add0x20InfoText(sReport, pInfo, false);
	}
	
	if(pInfo->bIsMode0x21Read)
	{
		Add0x21InfoText(sReport, pInfo, false);
	}
}
