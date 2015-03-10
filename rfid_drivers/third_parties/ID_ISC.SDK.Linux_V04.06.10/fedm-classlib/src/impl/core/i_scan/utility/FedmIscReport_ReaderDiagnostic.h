/*-------------------------------------------------------
|                                                       |
|				FedmIscReport_ReaderDiagnostic.h        |
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

Operation Systems	:	independent

Function			:	report class for [0x6E] Reader Diagnostic


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_REPORT_0x6E_H_INCLUDED_)
#define _FEDM_ISC_REPORT_0x6E_H_INCLUDED_


#include <string>
#include "../../FEDM.h"
#include "../FEDM_ISCReaderDiagnostic.h"


// STL - Standard Template Library
using namespace std;

class FEDM_ISCReader;


class _FEDM_ISC_CORE_EXT_CLASS FedmIscReport_ReaderDiagnostic
{
public:
	FedmIscReport_ReaderDiagnostic(FEDM_ISCReader* pReader);
	~FedmIscReport_ReaderDiagnostic();

// interface
public:
	string BuildReport(unsigned char ucMode);
	string BuildReport(unsigned char ucMode, FEDM_ISC_READER_DIAGNOSTIC* pInfo);

	virtual int BuildReport(unsigned char ucMode, string& sReport);
	virtual int BuildReport(unsigned char ucMode, string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo);

protected:
	virtual void Add0x01InfoText(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle=true);
	virtual void Add0x01InfoText_MR102(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle=true);
	virtual void Add0x01InfoText_MR200(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle=true);
	virtual void Add0x01InfoText_PRH200(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle=true);
	virtual void Add0x01InfoText_LR200(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle=true);
	virtual void Add0x01InfoText_LR1002(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle=true);
	virtual void Add0x01InfoText_LR2000(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle=true);
	virtual void Add0x01InfoText_LRU1000(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle=true);
	virtual void Add0x02InfoText(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle=true);
	virtual void Add0x03InfoText(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle=true);
	virtual void Add0x04InfoText(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle=true);
	virtual void Add0x05InfoText(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle=true);
	virtual void Add0x06InfoText(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle=true);
	virtual void Add0x07InfoText(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle=true);
	virtual void Add0x20InfoText(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle=true);
	virtual void Add0x21InfoText(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo, bool bSingle=true);

	virtual void AddAllInfoText(string& sReport, FEDM_ISC_READER_DIAGNOSTIC* pInfo);

	virtual void Add0x01InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x01InfoText_MR102(string& sReport, bool bSingle=true);
	virtual void Add0x01InfoText_MR200(string& sReport, bool bSingle=true);
	virtual void Add0x01InfoText_PRH200(string& sReport, bool bSingle=true);
	virtual void Add0x01InfoText_LR200(string& sReport, bool bSingle=true);
	virtual void Add0x01InfoText_LR1002(string& sReport, bool bSingle=true);
	virtual void Add0x01InfoText_LR2000(string& sReport, bool bSingle=true);
	virtual void Add0x01InfoText_LRU1000(string& sReport, bool bSingle=true);
	virtual void Add0x02InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x03InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x04InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x05InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x06InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x07InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x20InfoText(string& sReport, bool bSingle=true);
	virtual void Add0x21InfoText(string& sReport, bool bSingle=true);

	virtual void AddAllInfoText(string& sReport);

	FEDM_ISCReader*	m_pReader;

	string m_sReport;
};

#endif // _FEDM_ISC_REPORT_0x6E_H_INCLUDED_
