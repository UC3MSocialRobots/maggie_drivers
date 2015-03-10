/*-------------------------------------------------------
|                                                       |
|	FEDM_XmlParser_IReader.cpp			                |
|                                                       |
---------------------------------------------------------

Copyright � 2008-2011	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	23.01.2008
Version       		:	04.00.02 / 22.08.2011 / M. Hultsch
Operation Systems	:	independent
Function			:	abstract xml reader class for basic parser API

OBID� and OBID i-scan� are registered Trademarks of FEIG ELECTRONIC GmbH.
Linux� is a registered trademark of Linus Torvalds.
Microsoft� and Windows� are registered trademarks of Microsoft Corporation.
*/



//#############################
//      I N C L U D E S
//#############################
#include "FEDM_XmlParser_IReader.h"				// public include file of library
#include "FEDM_XmlParser_Base.h"


//#######################################################################################
//	class FEDM_XmlParser_Reader
//#######################################################################################
FEDM_XmlParser_Reader::FEDM_XmlParser_Reader(void)
{
	m_pXmlParser = NULL;
}

FEDM_XmlParser_Reader::~FEDM_XmlParser_Reader(void)
{
}

//#####################################################################################
// serialization functions
//#####################################################################################

/***************************************************************************
  Begin        	:	23.01.2008
  Version       :	04.00.02 / 22.08.2011 / M. Hultsch

  Function		:	read xml file-header

  Parameters	:	FEDM_XML_TREEITEM* root	- parent element
					const char* szVendor	- name of vendor

  Return value	:	FEDM_OK or error code (<0)

***************************************************************************/
int FEDM_XmlParser_Reader::SerializeHeaderIn(FEDM_XML_TREEITEM* root, const char* szVendor)
{
	if(root == NULL)
	{
		m_pXmlParser->SetLastError(FEDM_ERROR_NULL_POINTER);
		return FEDM_ERROR_NULL_POINTER;
	}

	char	cValue[512];
	wchar_t wcTmp[FEDM_MAX_TEXT_SIZE+2];
	wstring sTmp;
	unsigned int uiValue[3];
	FEDM_XML_TREEITEM* child	= NULL;
	FEDM_XML_TREEITEM* parent	= NULL;

	
	// #####################################################
	// header tag
	child = FindTag(FEDM_XML_TAG_FILE_HDR_UC, (unsigned int)1);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		m_pXmlParser->SetLastError(m_pXmlParser->GetLastError());
		return m_pXmlParser->GetLastError();
	}
	AddTagItem(root, child);
	parent = child;

	// header tag document type
	child = FindTag(FEDM_XML_TAG_DOC_TYPE_UC, (unsigned int)2);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		m_pXmlParser->SetLastError(m_pXmlParser->GetLastError());
		return m_pXmlParser->GetLastError();
	}
	if(child->sValue.length() < FEDM_MAX_TEXT_SIZE)
	{
		for (unsigned int i = 0; i < child->sValue.length(); i++)
			m_pXmlParser->m_XmlHeader.szDocType[i] = (char)(child->sValue.c_str())[i];
	}
	AddTagItem(parent, child);

	// header tag document version
	child = FindTag(FEDM_XML_TAG_DOC_VERSION_UC, (unsigned int)2, true);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		m_pXmlParser->SetLastError(m_pXmlParser->GetLastError());
		return m_pXmlParser->GetLastError();
	}
	memset(uiValue, 0, 3);
#if defined(_MSC_VER)
	swscanf(child->sValue.c_str(), L"%S", cValue);
#else
	swscanf(child->sValue.c_str(), L"%s", cValue);
#endif
	sscanf(cValue, "%u.%u", &uiValue[0], &uiValue[1]);
	m_pXmlParser->m_XmlHeader.uiDocVersion = (((unsigned char)uiValue[0]) << 8) + (unsigned char)uiValue[1];
	AddTagItem(parent, child);

	// header tag vendor
	child = FindTag(FEDM_XML_TAG_VENDOR_UC, (unsigned int)2, true);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		m_pXmlParser->SetLastError(m_pXmlParser->GetLastError());
		return m_pXmlParser->GetLastError();
	}
	if(strlen(szVendor) > 0)
	{
#if defined(__BORLANDC__) || defined(_MSC_VER)
                #if _MSC_VER <= 1200
                swprintf(wcTmp, L"%S", szVendor);
                #else
                        #if defined(_WIN32_WCE)
                swprintf(wcTmp, L"%S", szVendor);
                        #else
                swprintf(wcTmp, FEDM_MAX_TEXT_SIZE+2, L"%S", szVendor);
                        #endif
                #endif
#else
                swprintf(wcTmp, FEDM_MAX_TEXT_SIZE+2, L"%s", szVendor);
#endif
	}
	else
	{
#if defined(__BORLANDC__) || defined(_MSC_VER)
                #if _MSC_VER <= 1200
                swprintf(wcTmp, L"FEIG ELECTRONIC");
                #else
                        #if defined(_WIN32_WCE)
                swprintf(wcTmp, L"FEIG ELECTRONIC");
                        #else
                swprintf(wcTmp, FEDM_MAX_TEXT_SIZE+2, L"FEIG ELECTRONIC");
                        #endif
                #endif
#else
                swprintf(wcTmp, FEDM_MAX_TEXT_SIZE+2, L"FEIG ELECTRONIC");
#endif
	}
	sTmp = wcTmp;
	if(child->sValue != wcTmp)
	{
		delete child;
		DeleteXmlTree(); // clean up
		m_pXmlParser->SetLastError(FEDM_XML_ERROR_DOC_FILE_TYPE);
		return m_pXmlParser->GetLastError();		
	}
	if(strlen(szVendor) > 0)
		strcpy(m_pXmlParser->m_XmlHeader.szVendor, szVendor);
	else
		strcpy(m_pXmlParser->m_XmlHeader.szVendor, "FEIG ELECTRONIC");
	AddTagItem(parent, child);

	// header tag fedm version
	child = FindTag(FEDM_XML_TAG_FEDM_VERSION_UC, (unsigned int)2, true);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		m_pXmlParser->SetLastError(m_pXmlParser->GetLastError());
		return m_pXmlParser->GetLastError();
	}
	memset(uiValue, 0, 3);
#if defined(_MSC_VER)
	swscanf(child->sValue.c_str(), L"%S", cValue);
#else
	swscanf(child->sValue.c_str(), L"%s", cValue);
#endif
	sscanf(cValue, "%02u.%02u.%02u", &uiValue[0], &uiValue[1], &uiValue[2]);
	m_pXmlParser->m_XmlHeader.uiFedmVersion = (((unsigned char)uiValue[0]) << 16) + (((unsigned char)uiValue[1]) << 8) + (unsigned char)uiValue[2];
	AddTagItem(parent, child);

	// header tag comment
	child = FindTag(FEDM_XML_TAG_COMMENT_UC, (unsigned int)2, true);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		m_pXmlParser->SetLastError(m_pXmlParser->GetLastError());
		return m_pXmlParser->GetLastError();
	}
	if(child->sValue.length() < FEDM_MAX_TEXT_SIZE)
	{
		for (unsigned int i = 0; i < child->sValue.length(); i++)
			m_pXmlParser->m_XmlHeader.szComment[i] = (char)(child->sValue.c_str())[i];
	}
	AddTagItem(parent, child);

	// header tag checksum
	child = FindTag(FEDM_XML_TAG_CRC_UC, (unsigned int)2, true);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		m_pXmlParser->SetLastError(m_pXmlParser->GetLastError());
		return m_pXmlParser->GetLastError();
	}
#if defined(_MSC_VER)
	swscanf(child->sValue.c_str(), L"%S", cValue);
#else
	swscanf(child->sValue.c_str(), L"%s", cValue);
#endif
	sscanf(cValue, "%u", &m_pXmlParser->m_XmlHeader.uiCrc);
	AddTagItem(parent, child);

	m_pXmlParser->SetLastError(FEDM_OK);
	return m_pXmlParser->GetLastError();
}

/***************************************************************************
  Begin        	:	23.01.2008
  Version       :	03.00.00 / 23.01.2008 / M. Hultsch

  Function		:	query at index one attribute (name and value) from tags attribute list

  Parameters	:	FEDM_XML_TREEITEM* item	- pointer to tree item
					int iIndex				- index in attribute list
					string sAttrib			- attribute name
					string& sValue			- attribute value

  Return value	:	

***************************************************************************/
int FEDM_XmlParser_Reader::GetTagAttrib(	FEDM_XML_TREEITEM* item,
											int iIndex, 
											string& sAttrib, 
											string& sValue)
{
	if(item == NULL)
	{
		m_pXmlParser->SetLastError(FEDM_ERROR_NULL_POINTER);
		return FEDM_ERROR_NULL_POINTER;
	}
	
	if((int)item->aAttribList.size() <= iIndex)
	{
		m_pXmlParser->SetLastError(FEDM_XML_ERROR_NO_TAG_ATTRIBUTE);
		return m_pXmlParser->GetLastError();
	}

	char cBuffer[256];

	// attribute name: convert UNICODE to char
	if(item->aAttribList[iIndex]->sAttribute.size() > 256)
	{
		m_pXmlParser->SetLastError(FEDM_ERROR_STRING_LENGTH);
		return m_pXmlParser->GetLastError();	
	}

	// attribute value: convert UNICODE to char
	if(item->aAttribList[iIndex]->sValue.size() > 256)
	{
		m_pXmlParser->SetLastError(FEDM_ERROR_STRING_LENGTH);
		return m_pXmlParser->GetLastError();	
	}

#if defined(__BORLANDC__) || defined(_MSC_VER)
	sprintf(cBuffer, "%S", item->aAttribList[iIndex]->sAttribute.c_str());
#else
	sprintf(cBuffer, "%S", (const char*)item->aAttribList[iIndex]->sAttribute.c_str());
#endif
	sAttrib = cBuffer;

#if defined(__BORLANDC__) || defined(_MSC_VER)
	sprintf(cBuffer, "%S", item->aAttribList[iIndex]->sValue.c_str());
#else
	sprintf(cBuffer, "%S", (const char*)item->aAttribList[iIndex]->sValue.c_str());
#endif
	sValue = cBuffer;

	m_pXmlParser->SetLastError(FEDM_OK);
	return m_pXmlParser->GetLastError();
}

