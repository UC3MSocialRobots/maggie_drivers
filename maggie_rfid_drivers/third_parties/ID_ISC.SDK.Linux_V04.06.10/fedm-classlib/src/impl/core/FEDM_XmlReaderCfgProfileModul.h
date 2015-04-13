/*-------------------------------------------------------
|                                                       |
|              FEDM_XmlReaderCfgProfileModul.h	        |
|                                                       |
---------------------------------------------------------

Copyright © 2007-2009	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	13.08.2007
Version       		:	03.00.05 / 24.02.2009 / M. Hultsch

Operation Systems	:	independent

Function			:	main class for XML document handler
						
Note				:	this XML document handler is designed only for use
						inside the class library FEDM !!


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/



#if !defined(_FEDM_XML_READER_CFG_PROFILE_MODUL_H_INCLUDED_)
#define _FEDM_XML_READER_CFG_PROFILE_MODUL_H_INCLUDED_


#ifdef _MSC_VER
	#if _MSC_VER >= 1000
	#pragma once
	#endif // _MSC_VER >= 1000
#endif


#ifdef _FEDM_XML_SUPPORT


#include "FEDM.h"
#include "FEDM_XmlBase.h"


class FEDM_DataBase;


//#######################################################################################
// structure for XML Reader Profiling
//#######################################################################################
typedef struct _FEDM_XML_READER_CFG_PROFILE
{
	// reader specification attributes
	bool bCfgReset;
	bool bHasCustID;
	bool bHasRdrFunc;
	bool bHasTrpDrv;
	bool bHasFrequency;
	bool bHasRfcFw;
	bool bHasAccFw;

	unsigned int	uiCumstomerID;
	unsigned int	uiReaderFunction;
	unsigned int	uiTransponderDriver;
	unsigned int	uiFrequency;

	map<unsigned int, unsigned int>	mapControllerFW;
	
	void Init()
	{
		bCfgReset			= false;
		bHasCustID			= false;
		bHasRdrFunc			= false;
		bHasTrpDrv			= false;
		bHasFrequency		= false;
		bHasRfcFw			= false;
		bHasAccFw			= false;

		uiCumstomerID		= 0;
		uiReaderFunction	= 0;
		uiTransponderDriver	= 0;
		uiFrequency			= 0;

		mapControllerFW.clear();
		
		return;
	}

} FEDM_XML_READER_CFG_PROFILE;



//#######################################################################################
// main class for XML Reader Profiling
//#######################################################################################
class _FEDM_ISC_CORE_EXT_CLASS FEDM_XMLReaderCfgProfileModul : public FEDM_XMLBase
{
public:

	FEDM_XMLReaderCfgProfileModul();
	~FEDM_XMLReaderCfgProfileModul();

	// serialization functions
	int SerializeIn(FEDM_DataBase* pReader, char* sFileName);
	int SerializeOut(FEDM_DataBase* pReader, char* sFileName);

	// query functions
	int QueryDocType(char* sFileName, char* sDocType);
	int QueryDocVersion(char* sFileName, char* sDocVersion);
	int QueryReaderType(char* sFileName);

	// get/set functions
	void GetComment(string& sComment);
	void SetComment(string sComment);

	// reader requirements
	FEDM_XML_READER_CFG_PROFILE	m_ReaderRequirement;

protected:

	// file header
	wstring m_wsDocType;
	wstring m_wsDocVer;
	wstring m_wsComment;
	wstring m_wsFedmVer;
	wstring m_wsDate;
	wstring m_wsTime;
};

#endif // #ifdef _FEDM_XML_SUPPORT

#endif // !defined(_FEDM_XML_READER_CFG_PROFILE_MODUL_H_INCLUDED_)
