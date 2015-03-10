/*-------------------------------------------------------
|                                                       |
|				FedmIscReport_ReaderInfo.h              |
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

Version       		:	04.05.05 / 29.11.2013 / M. Hultsch
						- new mode 0x09 for WLAN-Firmware

						03.01.01 / 23.02.2010 / M. Hultsch

Operation Systems	:	independent

Function			:	report class for [0x66] Reader Info


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_REPORT_0x66_H_INCLUDED_)
#define _FEDM_ISC_REPORT_0x66_H_INCLUDED_


#include <string>
#include "../../FEDM.h"


// STL - Standard Template Library
using namespace std;

class FEDM_ISCReader;


class _FEDM_ISC_CORE_EXT_CLASS FedmIscReport_ReaderInfo
{
public:
	FedmIscReport_ReaderInfo(FEDM_ISCReader* pReader);
	~FedmIscReport_ReaderInfo();

// interface
public:
	int BuildReport(unsigned char ucMode, string& sReport);
	string BuildReport(unsigned char ucMode);

protected:
	virtual void Add0x00InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x01InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x02InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x03InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x04InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x05InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x06InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x07InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x08InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x09InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x10InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x11InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x12InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x40InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x41InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x50InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x51InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x52InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x53InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x54InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x55InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x56InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x57InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x60InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x61InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x80InfoText(string& sReport, bool bSingle=true);

	virtual void AddAllInfoText(string& sReport);

	FEDM_ISCReader*	m_pReader;

	string m_sReport;

};

#endif // _FEDM_ISC_REPORT_0x66_H_INCLUDED_
