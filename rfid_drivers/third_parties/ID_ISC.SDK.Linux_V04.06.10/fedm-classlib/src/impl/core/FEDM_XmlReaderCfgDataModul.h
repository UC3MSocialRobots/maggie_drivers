/*-------------------------------------------------------
|                                                       |
|              FEDM_XmlReaderCfgDataModul.h	            |
|                                                       |
---------------------------------------------------------

Copyright © 2000-2008	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	23.04.2003
Version       		:	03.00.00 / 16.04.2008 / M. Hultsch

Operation Systems	:	independent

Function			:	main class for XML document handler
						
Note				:	this XML document handler is designed only for use
						inside the class library FEDM !!


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/



#if !defined(_FEDM_XMLREADERCFGDATAMODUL_H_INCLUDED_)
#define _FEDM_XMLREADERCFGDATAMODUL_H_INCLUDED_



#ifdef _FEDM_XML_SUPPORT


#include "FEDM.h"
#include "FEDM_XmlBase.h"
#include "i_scan/FEDM_ISCReaderInfo.h"


class FEDM_DataBase;



class _FEDM_ISC_CORE_EXT_CLASS FEDM_XMLReaderCfgDataModul : public FEDM_XMLBase
{
public:

	FEDM_XMLReaderCfgDataModul();
	~FEDM_XMLReaderCfgDataModul();

	// serialization functions
	int SerializeIn(FEDM_DataBase* pReader, char* sFileName);
	int SerializeOut(FEDM_DataBase* pReader, char* sFileName);

	// query functions
	int QueryDocType(char* sFileName, char* sDocType);
	int QueryDocVersion(char* sFileName, char* sDocVersion);
	int QueryReaderFamily(char* sFileName, char* sFamCode);
	int QueryReaderType(char* sFileName);

	// get/set functions
	void SetReaderInfo(FEDM_ISC_READER_INFO* pReaderInfo);

	void GetComment(string& sComment);
	void GetPrgName(string& sPrgName);
	void GetPrgVer(string& sPrgVer);
	void GetHost(string& sHost);
	void GetPort(string& sPort);
	void GetCommMode(string& sMode);
	void SetComment(string sComment);
	void SetPrgName(string sPrgName);
	void SetPrgVer(string sPrgVer);

private:
	int AddReaderInfo(FEDM_DataBase* pReader, FEDM_XML_TREEITEM* root);
	int GetReaderInfo(FEDM_DataBase* pReader, FEDM_XML_TREEITEM* root);

protected:
	FEDM_ISC_READER_INFO* m_pReaderInfo;

	// file header
	wstring m_wsDocType;
	wstring m_wsDocVer;
	wstring m_wsComment;
	wstring m_wsPrgName;
	wstring m_wsPrgVer;
	wstring m_wsFedmVer;
	wstring m_wsHost;	// for TCP/IP
	wstring m_wsPort;	// for TCP/IP
	wstring m_wsDate;
	wstring m_wsTime;
	wstring m_wsCommMode;	// last communication mode
};

#endif // #ifdef _FEDM_XML_SUPPORT

#endif // !defined(_FEDM_XMLREADERCFGDATAMODUL_H_INCLUDED_)
