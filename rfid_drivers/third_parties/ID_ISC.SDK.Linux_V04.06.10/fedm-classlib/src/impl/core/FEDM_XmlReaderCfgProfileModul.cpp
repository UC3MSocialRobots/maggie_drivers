/*-------------------------------------------------------
|                                                       |
|              FEDM_XmlReaderCfgProfileModul.cpp        |
|                                                       |
---------------------------------------------------------

Copyright  2007-2009	FEIG ELECTRONIC GmbH, All Rights Reserved.
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


#ifdef _FEDM_XML_SUPPORT


#include "FEDM_XmlReaderCfgProfileModul.h"
#include "FEDM_DataBase.h"
#include "FEDM_Xml.h"
#include <time.h>

#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif

#ifdef _FEDM_LINUX
	#include <string.h>
#endif


//#####################################################################################
// Construction/Destruction
//#####################################################################################

/***************************************************************************
  Begin        	:	13.08.2007 / M. Hultsch
  Version       :	03.00.00 / 19.09.2007 / M. Hultsch

  Function		:	constructor

  Parameters	:	-

  Return value	:	-
***************************************************************************/
FEDM_XMLReaderCfgProfileModul::FEDM_XMLReaderCfgProfileModul()
{
	m_wsDocType			= L"";
	m_wsDocVer			= L"";
	m_wsComment			= L"";
	m_wsFedmVer			= L"";
	m_wsDate			= L"";
	m_wsTime			= L"";

	m_ReaderRequirement.Init();
}

/***************************************************************************
  Begin        	:	13.08.2007 / M. Hultsch
  Version       :	03.00.00 / 13.08.2007 / M. Hultsch

  Function		:	destructor

  Parameters	:	-

  Return value	:	-
***************************************************************************/
FEDM_XMLReaderCfgProfileModul::~FEDM_XMLReaderCfgProfileModul()
{
}


//#####################################################################################
// serialization functions
//#####################################################################################

/***************************************************************************
  Begin        	:	13.08.2007 / M. Hultsch
  Version       :	03.00.05 / 24.02.2009 / M. Hultsch

  Function		:	read xml document and save content in reader class

  Parameters	:	FEDM_DataBase* pReader
					char* sFileName	-	file name

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FEDM_XMLReaderCfgProfileModul::SerializeIn(FEDM_DataBase* pReader, char* sFileName)
{
	FEDM_CHK5(pReader);
	FEDM_CHK5(sFileName);

	char	cValue[64];
	int iTmp[3];
	unsigned int i = 0;
	unsigned int uiTmp;
	int		iBack = 0;
	FEDM_XML_TREEITEM* root = NULL;
	FEDM_XML_TREEITEM* parent = NULL;
	FEDM_XML_TREEITEM* child = NULL;

	memset(iTmp, 0, 3);

	
	iBack = OpenDoc(sFileName, "r");
	if(iBack)
		return iBack;
	
	// is it a xml document?
	if(IsXmlDoc())
	{
		CloseDoc();
		return m_iLastError;
	}

	// has the xml document an OBID tag?
	if(HasOBIDTag())
	{
		CloseDoc();
		return m_iLastError;
	}
	
	// read complete xml document into string
	if(ReadDoc())
	{
		CloseDoc();
		return m_iLastError;
	}

	CloseDoc();


	// root tag
	root = FindTag(FEDM_XML_TAG_OBID_UC);
	if(root == NULL)
		return m_iLastError;
	AddTagItem(NULL, root);

	// header tag
	child = FindTag(FEDM_XML_TAG_FILE_HDR_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	AddTagItem(root, child);
	parent = child;

	// header tag document type
	child = FindTag(FEDM_XML_TAG_DOC_TYPE_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	if(child->sValue != FEDM_XML_STRING_DOC_TYPE_4_UC)
	{
		delete child;
		DeleteXmlTree(); // clean up
		FEDM_RETURN(FEDM_XML_ERROR_DOC_FILE_TYPE);
	}
	m_wsDocType = child->sValue;
	AddTagItem(parent, child);

	// header tag document version
	child = FindTag(FEDM_XML_TAG_DOC_VERSION_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	if(child->sValue != FEDM_XML_STRING_PROFILE_DOC_VERSION_UC)
	{
		delete child;
		DeleteXmlTree(); // clean up
		FEDM_RETURN(FEDM_XML_ERROR_DOC_FILE_VERSION);
	}
	m_wsDocVer = child->sValue;
	AddTagItem(parent, child);

	// header tag comment
	child = FindTag(FEDM_XML_TAG_COMMENT_UC);
	if(child != NULL)
	{
		AddTagItem(parent, child);
		m_wsComment = child->sValue;
	}

	// reader tag
	child = FindTag(FEDM_XML_TAG_READER_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	AddTagItem(root, child);
	parent = child;

	// reader tag reader type
	child = FindTag(FEDM_XML_TAG_READER_TYPE_UC);
	if(child != NULL)
	{
		AddTagItem(parent, child);
		if(child->sValue.length()>0 && child->sValue.length()<FEDM_MAX_NAME_SIZE)
		{
			memset(cValue, 0, FEDM_MAX_NAME_SIZE);
			for(i = 0; i < child->sValue.length(); i++)
				cValue[i] = (char)(child->sValue.c_str())[i];

			sscanf(cValue, "%u", &uiTmp);
			if(uiTmp != pReader->GetReaderType())
			{
				DeleteXmlTree(); // clean up
				FEDM_RETURN(FEDM_ERROR_WRONG_READER_TYPE);
			}
		}
		else
		{
			DeleteXmlTree(); // clean up
			FEDM_RETURN(FEDM_XML_ERROR_NO_TAG_VALUE);
		}
	}

	m_ReaderRequirement.Init();

	// optional reader tag: CFG-Reset
	child = FindTag(FEDM_XML_TAG_CFG_RESET_UC);
	if(child != NULL)
	{
		AddTagItem(parent, child);
		if(child->sValue.length()>0 && child->sValue.length()<2)
		{
			memset(cValue, 0, 2);
			for(i = 0; i < 1; i++)
				cValue[i] = (char)(child->sValue.c_str())[i];
			sscanf(cValue, "%X", &uiTmp);
			if(uiTmp == 1)
				m_ReaderRequirement.bCfgReset = true;
		}
	}

	// optional reader tag: RFC firmware
	child = FindTag(FEDM_XML_TAG_FIRMWARE_VERSION_UC,
					FEDM_XML_TAG_CONTROLLER_TYPE_UC,
					L"RFC",
					false,
					true);
	if(child != NULL)
	{
		AddTagItem(parent, child);
		if(child->sValue.length()>0 && child->sValue.length()<FEDM_MAX_NAME_SIZE)
		{
			memset(cValue, 0, FEDM_MAX_NAME_SIZE);
			for(i = 0; i < child->sValue.length(); i++)
				cValue[i] = (char)(child->sValue.c_str())[i];
			sscanf(cValue, "%d.%d.%d", &iTmp[0], &iTmp[1], &iTmp[2]);
			uiTmp = (iTmp[0]<<16) + (iTmp[1]<<8) + iTmp[2];
			m_ReaderRequirement.mapControllerFW.insert(make_pair(FEDM_CNTRL_TYPE_RFC, uiTmp));
			m_ReaderRequirement.bHasRfcFw = true;
		}
	}

	// optional reader tag: ACC firmware
	child = FindTag(FEDM_XML_TAG_FIRMWARE_VERSION_UC,
					FEDM_XML_TAG_CONTROLLER_TYPE_UC,
					L"ACC",
					false,
					true);
	if(child != NULL)
	{
		AddTagItem(parent, child);
		if(child->sValue.length()>0 && child->sValue.length()<FEDM_MAX_NAME_SIZE)
		{
			memset(cValue, 0, FEDM_MAX_NAME_SIZE);
			for(i = 0; i < child->sValue.length(); i++)
				cValue[i] = (char)(child->sValue.c_str())[i];
			sscanf(cValue, "%d.%d.%d", &iTmp[0], &iTmp[1], &iTmp[2]);
			uiTmp = (iTmp[0]<<16) + (iTmp[1]<<8) + iTmp[2];
			m_ReaderRequirement.mapControllerFW.insert(make_pair(FEDM_CNTRL_TYPE_ACC, uiTmp));
			m_ReaderRequirement.bHasAccFw = true;
		}
	}

	// optional reader tag: cutomer id
	child = FindTag(FEDM_XML_TAG_CUSTOMER_ID_UC);
	if(child != NULL)
	{
		AddTagItem(parent, child);
		if(child->sValue.length()>0 && child->sValue.length()<FEDM_MAX_NAME_SIZE)
		{
			memset(cValue, 0, FEDM_MAX_NAME_SIZE);
			for(i = 0; i < child->sValue.length(); i++)
				cValue[i] = (char)(child->sValue.c_str())[i];
			sscanf(cValue, "%X", &m_ReaderRequirement.uiCumstomerID);
			m_ReaderRequirement.bHasCustID = true;
		}
	}

	// optional reader tag: reader functions
	child = FindTag(FEDM_XML_TAG_READER_FUNCTIONS_UC);
	if(child != NULL)
	{
		AddTagItem(parent, child);
		if(child->sValue.length()>0 && child->sValue.length()<FEDM_MAX_NAME_SIZE)
		{
			memset(cValue, 0, FEDM_MAX_NAME_SIZE);
			for(i = 0; i < child->sValue.length(); i++)
				cValue[i] = (char)(child->sValue.c_str())[i];
			sscanf(cValue, "%X", &m_ReaderRequirement.uiReaderFunction);
			m_ReaderRequirement.bHasRdrFunc = true;
		}
	}

	// optional reader tag: transponder driver
	child = FindTag(FEDM_XML_TAG_TRANSPONDER_DRIVER_UC);
	if(child != NULL)
	{
		AddTagItem(parent, child);
		if(child->sValue.length()>0 && child->sValue.length()<FEDM_MAX_NAME_SIZE)
		{
			memset(cValue, 0, FEDM_MAX_NAME_SIZE);
			for(i = 0; i < child->sValue.length(); i++)
				cValue[i] = (char)(child->sValue.c_str())[i];
			sscanf(cValue, "%X", &m_ReaderRequirement.uiTransponderDriver);
			m_ReaderRequirement.bHasTrpDrv = true;
		}
	}

	// optional reader tag: frequency
	child = FindTag(FEDM_XML_TAG_FREQUENCY_UC);
	if(child != NULL)
	{
		AddTagItem(parent, child);
		if(child->sValue.length()>0 && child->sValue.length()<FEDM_MAX_NAME_SIZE)
		{
			memset(cValue, 0, FEDM_MAX_NAME_SIZE);
			for(i = 0; i < child->sValue.length(); i++)
				cValue[i] = (char)(child->sValue.c_str())[i];
			sscanf(cValue, "%d", &m_ReaderRequirement.uiFrequency);
			m_ReaderRequirement.bHasFrequency = true;
		}
	}


	// ... transfer content of tag items to byte arrays
	iBack = pReader->SerializeProfileIn(this, root);
	if(iBack)
	{
		DeleteXmlTree(); // clean up
		return iBack;
	}

	// clean up
	DeleteXmlTree();

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        	:	13.08.2007 / M. Hultsch
  Version       :	03.00.05 / 22.02.2009 / M. Hultsch

  Function		:	write content of reader class into xml document

  Parameters	:	FEDM_DataBase* pReader
					char* sFileName	-	file name

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FEDM_XMLReaderCfgProfileModul::SerializeOut(FEDM_DataBase* pReader, char* sFileName)
{
	FEDM_CHK5(pReader);
	FEDM_CHK5(sFileName);

	int		iBack = 0;
//	char	cValue[32];
	wchar_t wcTmp[128];
#if !defined(_FEDM_SUPPORT_UCLINUX) && !defined(_WIN32_WCE)
	time_t	ltime;
	struct	tm *tmtime;
#endif
	FEDM_XML_TREEITEM* root = NULL;
	FEDM_XML_TREEITEM* parent = NULL;
	FEDM_XML_TREEITEM* child = NULL;
	map<unsigned int, unsigned int>::iterator itor;


	// root tag
	root = BuildTag(FEDM_XML_TAG_OBID_UC);
	if(root == NULL)
	{
		return m_iLastError;
	}
	AddTagItem(NULL, root);

	// header tag
	child = BuildTag(FEDM_XML_TAG_FILE_HDR_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	AddTagItem(root, child);
	parent = child;

	// header tag document type
	child = BuildTag(FEDM_XML_TAG_DOC_TYPE_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	AddTagValue(child, FEDM_XML_STRING_DOC_TYPE_4_UC);
	AddTagItem(parent, child);

	// header tag document version
	child = BuildTag(FEDM_XML_TAG_DOC_VERSION_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	AddTagValue(child, FEDM_XML_STRING_PROFILE_DOC_VERSION_UC);
	AddTagItem(parent, child);

	// header tag fedm library version number
	child = BuildTag(FEDM_XML_TAG_FEDM_VERSION_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	AddTagValue(child, FEDM_VERSION_UC);
	AddTagItem(parent, child);

	// header tag date
#if !defined(_FEDM_SUPPORT_UCLINUX) && !defined(_WIN32_WCE)
	child = BuildTag(FEDM_XML_TAG_DATE_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	time(&ltime);
	tmtime = localtime(&ltime);
	wcsftime(wcTmp, 128, L"%x", tmtime);
	AddTagValue(child, wcTmp);
	AddTagItem(parent, child);
#endif

	// header tag time
#if !defined(_FEDM_SUPPORT_UCLINUX) && !defined(_WIN32_WCE)
	child = BuildTag(FEDM_XML_TAG_TIME_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	wcsftime(wcTmp, 128, L"%X", tmtime);
	AddTagValue(child, wcTmp);
	AddTagItem(parent, child);
#endif

	// header tag comment
	if(m_wsComment.size() > 0)
	{
		child = BuildTag(FEDM_XML_TAG_COMMENT_UC, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return m_iLastError;
		}
		AddTagValue(child, m_wsComment);
		AddTagItem(parent, child);
	}


	// reader tag
	child = BuildTag(FEDM_XML_TAG_READER_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	AddTagItem(root, child);
	parent = child;

	// tag reader name
	child = BuildTag(FEDM_XML_TAG_READER_NAME_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
#if defined(__BORLANDC__) || defined(_MSC_VER)
	swprintf(wcTmp, L"%S", pReader->GetReaderName());
#else
	swprintf(wcTmp, 128, L"%s", pReader->GetReaderName());
#endif
	AddTagValue(child, wcTmp);
	AddTagItem(parent, child);

	// tag reader type
	child = BuildTag(FEDM_XML_TAG_READER_TYPE_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
#if defined(__BORLANDC__) || defined(_MSC_VER)
	swprintf(wcTmp, L"%u", pReader->GetReaderType());
#else
	swprintf(wcTmp, 128, L"%u", pReader->GetReaderType());
#endif
	AddTagValue(child, wcTmp);
	AddTagItem(parent, child);

	// optional tags from m_ReaderRequirement: CFG-Reset
	if(m_ReaderRequirement.bCfgReset)
	{
		child = BuildTag(FEDM_XML_TAG_CFG_RESET_UC, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return m_iLastError;
		}
#if defined(__BORLANDC__) || defined(_MSC_VER)
		swprintf(wcTmp, L"%d", (int)1);
#else
		swprintf(wcTmp, 128, L"%d", (int)1);
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(parent, child);
	}

	// optional tags from m_ReaderRequirement: RFC firmware
	if(m_ReaderRequirement.bHasRfcFw)
	{
		itor = m_ReaderRequirement.mapControllerFW.find(FEDM_CNTRL_TYPE_RFC);
		if(itor != m_ReaderRequirement.mapControllerFW.end())
		{
			child = BuildTag(FEDM_XML_TAG_FIRMWARE_VERSION_UC, false);
			if(child == NULL)
			{
				DeleteXmlTree(); // clean up
				return m_iLastError;
			}
			AddTagAttrib(child, FEDM_XML_TAG_CONTROLLER_TYPE_UC, L"RFC");
#if defined(__BORLANDC__) || defined(_MSC_VER)
			swprintf(wcTmp, L"%d.%d.%d", ((itor->second&0x00FF0000)>>16), ((itor->second&0x0000FF00)>>8), (itor->second&0x000000FF));
#else
			swprintf(wcTmp, 128, L"%d.%d.%d", ((itor->second&0x00FF0000)>>16), ((itor->second&0x0000FF00)>>8), (itor->second&0x000000FF));
#endif
			AddTagValue(child, wcTmp);
			AddTagItem(parent, child);
		}
	}

	// optional tags from m_ReaderRequirement: ACC firmware
	if(m_ReaderRequirement.bHasAccFw)
	{
		itor = m_ReaderRequirement.mapControllerFW.find(FEDM_CNTRL_TYPE_ACC);
		if(itor != m_ReaderRequirement.mapControllerFW.end())
		{
			child = BuildTag(FEDM_XML_TAG_FIRMWARE_VERSION_UC, false);
			if(child == NULL)
			{
				DeleteXmlTree(); // clean up
				return m_iLastError;
			}
			AddTagAttrib(child, FEDM_XML_TAG_CONTROLLER_TYPE_UC, L"ACC");
#if defined(__BORLANDC__) || defined(_MSC_VER)
			swprintf(wcTmp, L"%d.%d.%d", ((itor->second&0x00FF0000)>>16), ((itor->second&0x0000FF00)>>8), (itor->second&0x000000FF));
#else
			swprintf(wcTmp, 128, L"%d.%d.%d", ((itor->second&0x00FF0000)>>16), ((itor->second&0x0000FF00)>>8), (itor->second&0x000000FF));
#endif
			AddTagValue(child, wcTmp);
			AddTagItem(parent, child);
		}
	}

	// optional tags from m_ReaderRequirement: customer id
	if(m_ReaderRequirement.bHasCustID)
	{
		child = BuildTag(FEDM_XML_TAG_CUSTOMER_ID_UC, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return m_iLastError;
		}
#if defined(__BORLANDC__) || defined(_MSC_VER)
		swprintf(wcTmp, L"0x%08X", m_ReaderRequirement.uiCumstomerID);
#else
		swprintf(wcTmp, 128, L"0x%08X", m_ReaderRequirement.uiCumstomerID);
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(parent, child);
	}

	// optional tags from m_ReaderRequirement: reader function
	if(m_ReaderRequirement.bHasRdrFunc)
	{
		child = BuildTag(FEDM_XML_TAG_READER_FUNCTIONS_UC, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return m_iLastError;
		}
#if defined(__BORLANDC__) || defined(_MSC_VER)
		swprintf(wcTmp, L"0x%08X", m_ReaderRequirement.uiReaderFunction);
#else
		swprintf(wcTmp, 128, L"0x%08X", m_ReaderRequirement.uiReaderFunction);
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(parent, child);
	}

	// optional tags from m_ReaderRequirement: transponder driver
	if(m_ReaderRequirement.bHasTrpDrv)
	{
		child = BuildTag(FEDM_XML_TAG_TRANSPONDER_DRIVER_UC, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return m_iLastError;
		}
#if defined(__BORLANDC__) || defined(_MSC_VER)
		swprintf(wcTmp, L"0x%08X", m_ReaderRequirement.uiTransponderDriver);
#else
		swprintf(wcTmp, 128, L"0x%08X", m_ReaderRequirement.uiTransponderDriver);
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(parent, child);
	}

	// optional tags from m_ReaderRequirement: region
	if(m_ReaderRequirement.bHasFrequency)
	{
		child = BuildTag(FEDM_XML_TAG_FREQUENCY_UC, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return m_iLastError;
		}
#if defined(__BORLANDC__) || defined(_MSC_VER)
		swprintf(wcTmp, L"%d", m_ReaderRequirement.uiFrequency);
#else
		swprintf(wcTmp, 128, L"%d", m_ReaderRequirement.uiFrequency);
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(parent, child);
	}
	

	// add data array tags
	iBack = pReader->SerializeProfileOut(this, root);
	if(iBack)
	{
		DeleteXmlTree(); // clean up
		return iBack;
	}

	iBack = OpenDoc(sFileName, "w");
	if(iBack)
	{
		DeleteXmlTree(); // clean up
		return iBack;
	}
	else
	{
		WriteDoc();
	}


	CloseDoc();

	FEDM_RETURN(FEDM_OK);
}




//#####################################################################################
// query functions
//#####################################################################################

/***************************************************************************
  Begin        	:	13.08.2007 / M. Hultsch
  Version       :	03.00.00 / 13.08.2007 / M. Hultsch

  Function		:	open a xml document and get the document type

  Parameters	:	char* sFileName	- pointer to string with file name
					char* sDocType	- pointer to string for document type

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FEDM_XMLReaderCfgProfileModul::QueryDocType(char* sFileName, char* sDocType)
{
	int iBack = 0;
	FEDM_XML_TREEITEM* root = NULL;
	FEDM_XML_TREEITEM* parent = NULL;
	FEDM_XML_TREEITEM* child = NULL;

	
	iBack = OpenDoc(sFileName, "r");
	if(iBack)
		return iBack;
	
	// is it a xml document?
	if(IsXmlDoc())
	{
		CloseDoc();
		return m_iLastError;
	}

	// has the xml document an OBID tag?
	if(HasOBIDTag())
	{
		CloseDoc();
		return m_iLastError;
	}
	
	// read complete xml document into string
	if(ReadDoc())
	{
		CloseDoc();
		return m_iLastError;
	}

	CloseDoc();

	// root tag
	root = FindTag(FEDM_XML_TAG_OBID_UC);
	if(root == NULL)
		return m_iLastError;
	AddTagItem(NULL, root);

	// header tag
	child = FindTag(FEDM_XML_TAG_FILE_HDR_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	AddTagItem(root, child);
	parent = child;

	// header tag document type
	child = FindTag(FEDM_XML_TAG_DOC_TYPE_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}

#if defined(__BORLANDC__) || defined(_MSC_VER)
	swscanf(child->sValue.c_str(), L"%S", sDocType);
#elif defined(_FEDM_SUPPORT_UCLINUX)
	FEDM_SWSCANF(child->sValue.c_str(), sDocType);
#else
	swscanf(child->sValue.c_str(), L"%s", sDocType);
#endif
	
	delete child;
	DeleteXmlTree(); // clean up

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	13.08.2007 / M. Hultsch
  Version       :	03.00.00 / 13.08.2007 / M. Hultsch

  Function		:	open a xml document and get the doc version

  Parameters	:	char* sFileName		- pointer to string with file name
					char* sDocVersion	- pointer to string for document version

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FEDM_XMLReaderCfgProfileModul::QueryDocVersion(char* sFileName, char* sDocVersion)
{
	int iBack = 0;
	FEDM_XML_TREEITEM* root = NULL;
	FEDM_XML_TREEITEM* parent = NULL;
	FEDM_XML_TREEITEM* child = NULL;

	
	iBack = OpenDoc(sFileName, "r");
	if(iBack)
		return iBack;
	
	// is it a xml document?
	if(IsXmlDoc())
	{
		CloseDoc();
		return m_iLastError;
	}

	// has the xml document an OBID tag?
	if(HasOBIDTag())
	{
		CloseDoc();
		return m_iLastError;
	}
	
	// read complete xml document into string
	if(ReadDoc())
	{
		CloseDoc();
		return m_iLastError;
	}

	CloseDoc();

	// root tag
	root = FindTag(FEDM_XML_TAG_OBID_UC);
	if(root == NULL)
		return m_iLastError;
	AddTagItem(NULL, root);

	// header tag
	child = FindTag(FEDM_XML_TAG_FILE_HDR_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	AddTagItem(root, child);
	parent = child;

	// header tag document type
	child = FindTag(FEDM_XML_TAG_DOC_TYPE_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	m_wsDocType = child->sValue;
	AddTagItem(parent, child);

	// header tag document version
	child = FindTag(FEDM_XML_TAG_DOC_VERSION_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}

#if defined(__BORLANDC__) || defined(_MSC_VER)
	swscanf(child->sValue.c_str(), L"%S", sDocVersion);
#elif defined(_FEDM_SUPPORT_UCLINUX)
	FEDM_SWSCANF(child->sValue.c_str(), sDocVersion);
#else
	swscanf(child->sValue.c_str(), L"%s", sDocVersion);
#endif

	delete child;
	DeleteXmlTree(); // clean up

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	13.08.2007 / M. Hultsch
  Version       :	03.00.02 / 14.01.2009 / M. Hultsch

  Function		:	open a xml document and get the reader type

  Parameters	:	-

  Return value	:	reader type (>0) or error code (<0)
***************************************************************************/
int FEDM_XMLReaderCfgProfileModul::QueryReaderType(char* sFileName)
{
	char cValue[5];
	int iType = 0;
	int iBack = 0;
	FEDM_XML_TREEITEM* root = NULL;
	FEDM_XML_TREEITEM* parent = NULL;
	FEDM_XML_TREEITEM* child = NULL;

	
	iBack = OpenDoc(sFileName, "r");
	if(iBack)
		return iBack;
	
	// is it a xml document?
	if(IsXmlDoc())
	{
		CloseDoc();
		return m_iLastError;
	}

	// has the xml document an OBID tag?
	if(HasOBIDTag())
	{
		CloseDoc();
		return m_iLastError;
	}
	
	// read complete xml document into string
	if(ReadDoc())
	{
		CloseDoc();
		return m_iLastError;
	}

	CloseDoc();

	// root tag
	root = FindTag(FEDM_XML_TAG_OBID_UC);
	if(root == NULL)
		return m_iLastError;
	AddTagItem(NULL, root);

	// header tag
	child = FindTag(FEDM_XML_TAG_FILE_HDR_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	AddTagItem(root, child);
	parent = child;

	// header tag document type
	child = FindTag(FEDM_XML_TAG_DOC_TYPE_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	if(child->sValue != FEDM_XML_STRING_DOC_TYPE_4_UC)
	{
		delete child;
		DeleteXmlTree(); // clean up
		FEDM_RETURN(FEDM_XML_ERROR_DOC_FILE_TYPE);
	}
	m_wsDocType = child->sValue;
	AddTagItem(parent, child);

	// header tag document version
	child = FindTag(FEDM_XML_TAG_DOC_VERSION_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	if(child->sValue != FEDM_XML_STRING_PROFILE_DOC_VERSION_UC)
	{
		delete child;
		DeleteXmlTree(); // clean up
		return FEDM_XML_ERROR_DOC_FILE_VERSION;
	}
	m_wsDocVer = child->sValue;
	AddTagItem(parent, child);

	// reader tag
	child = FindTag(FEDM_XML_TAG_READER_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	AddTagItem(root, child);
	parent = child;

	// reader tag reader type
	child = FindTag(FEDM_XML_TAG_READER_TYPE_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}

#if defined(__BORLANDC__) || defined(_MSC_VER)
	swscanf(child->sValue.c_str(), L"%S", cValue);
#elif defined(_FEDM_SUPPORT_UCLINUX)
	FEDM_SWSCANF(child->sValue.c_str(), cValue);
#else
	swscanf(child->sValue.c_str(), L"%s", cValue);
#endif
	sscanf(cValue, "%d", &iType);

	delete child;
	DeleteXmlTree(); // clean up

	return iType;
}



//#####################################################################################
// get/set functions
//#####################################################################################

/***************************************************************************
  Begin        	:	13.08.2007 / M. Hultsch
  Version       :	03.00.00 / 13.08.2007 / M. Hultsch

  Function		:	gets the comment

  Parameters	:	string& sComment	- reference to string with comment

  Return value	:	-
***************************************************************************/
void FEDM_XMLReaderCfgProfileModul::GetComment(string& sComment)
{
	char cTmp[256];

	if(m_wsComment.size() > 256)
		return;

#if defined(__BORLANDC__) || defined(_MSC_VER)
	sprintf(cTmp, "%S", m_wsComment.c_str());
#else
	sprintf(cTmp, "%s", (const char*)m_wsComment.c_str());
#endif

	sComment = cTmp;
}

/***************************************************************************
  Begin        	:	13.08.2007 / M. Hultsch
  Version       :	03.00.00 / 13.08.2007 / M. Hultsch

  Function		:	sets the comment

  Parameters	:	string sComment	- string with comment

  Return value	:	-
***************************************************************************/
void FEDM_XMLReaderCfgProfileModul::SetComment(string sComment)
{
	wchar_t wcTmp[256];

	if(sComment.size() > 256)
		return;

#if defined(__BORLANDC__) || defined(_MSC_VER)
	swprintf(wcTmp, L"%S", sComment.c_str());
#else
	swprintf(wcTmp, 256, L"%s", sComment.c_str());
#endif

	m_wsComment = wcTmp;
}

#endif // #ifdef _FEDM_XML_SUPPORT





