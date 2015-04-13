/*-------------------------------------------------------
|                                                       |
|	FEDM_XmlParser_TinyReader.cpp						|
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
Version				:	04.00.02 / 22.08.2011 / M. Hultsch
Operation Systems	:	independent
Function			:	xml reader class based on tinyxml parser API

OBID� and OBID i-scan� are registered Trademarks of FEIG ELECTRONIC GmbH.
Linux� is a registered trademark of Linus Torvalds.
Microsoft� and Windows� are registered trademarks of Microsoft Corporation.
*/



//#############################
//      I N C L U D E S
//#############################
#include "FEDM_XmlParser_TinyReader.h"				// public include file of library
#include "FEDM_XmlParser_Base.h"


//#######################################################################################
//	class FEDM_XmlParser_TinyReader
//#######################################################################################
FEDM_XmlParser_TinyReader::FEDM_XmlParser_TinyReader(void)
{
	m_uiLastTagLevel	= 0;
	m_pDoc				= NULL;

	// parent node pointer set by last FindTag()
	m_pLastParent		= NULL;

	// child node pointer for each tag level set by last FindTag()
	memset(m_pLastChild, NULL, 20*sizeof(TiXmlNode*));
}


FEDM_XmlParser_TinyReader::~FEDM_XmlParser_TinyReader(void)
{
	if(m_pDoc != NULL)
		delete m_pDoc;
}

// open a xml document
int FEDM_XmlParser_TinyReader::OpenDoc(char* sFileName)
{
	CloseDoc();
	m_pLastParent		= NULL;

	m_pDoc = new TiXmlDocument(sFileName);
	if(m_pDoc == NULL)
	{
		m_pXmlParser->SetLastError(FEDM_ERROR_OPEN_FILE);
		return FEDM_ERROR_OPEN_FILE;
	}

	return FEDM_OK;
}

// close xml document
int FEDM_XmlParser_TinyReader::CloseDoc()
{
	m_pXmlParser->SetLastError(FEDM_OK);

	if(m_pDoc != NULL)
	{
		delete m_pDoc;
		m_pDoc = NULL;
	}

	return FEDM_OK;
}

// read the xml text from file
int FEDM_XmlParser_TinyReader::LoadDoc()
{
	TiXmlNode* node = NULL;

	m_pXmlParser->SetLastError(FEDM_OK);

	if(m_pDoc == NULL)
	{
		m_pXmlParser->SetLastError(FEDM_ERROR_NULL_POINTER);
		return FEDM_ERROR_NULL_POINTER;
	}

	bool bBack = m_pDoc->LoadFile();
	if(! bBack)
	{
		CloseDoc();
		m_pXmlParser->SetLastError(FEDM_ERROR_READ_FILE);
		return FEDM_ERROR_READ_FILE;
	}

	node = m_pDoc->RootElement();
	if(node == NULL)
	{
		CloseDoc();
		m_pXmlParser->SetLastError(FEDM_ERROR_READ_FILE);
		return FEDM_ERROR_READ_FILE;
	}

	if(strcmp(node->Value(), "OBID") != 0)
	{
		CloseDoc();
		m_pXmlParser->SetLastError(FEDM_XML_ERROR_NO_OBID_TAG);
		return FEDM_XML_ERROR_NO_OBID_TAG;
	}

	memset(m_pLastChild, NULL, 20*sizeof(TiXmlNode*));
	return FEDM_OK;
}

// read the xml text from file
int FEDM_XmlParser_TinyReader::LoadDoc(const char* szOemRootTag)
{
	TiXmlNode* node = NULL;

	m_pXmlParser->SetLastError(FEDM_OK);

	if(m_pDoc == NULL)
	{
		m_pXmlParser->SetLastError(FEDM_ERROR_NULL_POINTER);
		return FEDM_ERROR_NULL_POINTER;
	}

	bool bBack = m_pDoc->LoadFile();
	if(! bBack)
	{
		CloseDoc();
		m_pXmlParser->SetLastError(FEDM_ERROR_READ_FILE);
		return FEDM_ERROR_READ_FILE;
	}

	node = m_pDoc->RootElement();
	if(node == NULL)
	{
		CloseDoc();
		m_pXmlParser->SetLastError(FEDM_ERROR_READ_FILE);
		return FEDM_ERROR_READ_FILE;
	}

	if(strlen(szOemRootTag) > 0)
	{
		if(strcmp(node->Value(), szOemRootTag) != 0)
		{
			CloseDoc();
			m_pXmlParser->SetLastError(FEDM_XML_ERROR_NO_OBID_TAG);
			return FEDM_XML_ERROR_NO_OBID_TAG;
		}
	}
	else
	{
		if(strcmp(node->Value(), "OBID") != 0)
		{
			CloseDoc();
			m_pXmlParser->SetLastError(FEDM_XML_ERROR_NO_OBID_TAG);
			return FEDM_XML_ERROR_NO_OBID_TAG;
		}
	}

	memset(m_pLastChild, NULL, 20*sizeof(TiXmlNode*));
	return FEDM_OK;
}

int FEDM_XmlParser_TinyReader::LoadStream(wstring sXmlStream)
{
	unsigned int i=0;

	CloseDoc();

	// parent node pointer set by last FindTag()
	m_pLastParent		= NULL;

	// child node pointer for each tag level set by last FindTag()
	memset(m_pLastChild, NULL, 20*sizeof(TiXmlNode*));

	m_pDoc = new TiXmlDocument;
	if(m_pDoc == NULL)
	{
		m_pXmlParser->SetLastError(FEDM_ERROR_OPEN_FILE);
		return FEDM_ERROR_OPEN_FILE;
	}

	unsigned int len = (unsigned int)sXmlStream.size();
	char* str = new char[(len+1)*sizeof(char)];
	for(i=0; i<sXmlStream.size(); ++i)
		str[i] = (char)sXmlStream[i];
	str[i] = 0;

	m_pDoc->Parse(str);
	TiXmlNode* node = m_pDoc->RootElement();
	if(node == NULL)
	{
		delete [] str;
		m_pXmlParser->SetLastError(FEDM_XML_ERROR_TAG_NOT_FOUND);
		return FEDM_XML_ERROR_TAG_NOT_FOUND;
	}

	delete [] str;
	m_pXmlParser->SetLastError(FEDM_OK);
	return FEDM_OK;
}

void FEDM_XmlParser_TinyReader::DeleteXmlTree()
{
	return;	// nothing to do
}

int FEDM_XmlParser_TinyReader::AddTagItem(FEDM_XML_TREEITEM* pParent, FEDM_XML_TREEITEM* pChild)
{
	FEDM_XML_ATTRIBUTE_LIST_ITOR AttrItor;

	m_pXmlParser->SetLastError(FEDM_OK);

	// we misuse this function to delete the child
	for(AttrItor = pChild->aAttribList.begin(); AttrItor != pChild->aAttribList.end(); AttrItor++)
	{
		(*AttrItor)->sAttribute.erase();
		(*AttrItor)->sValue.erase();

		delete *AttrItor;
	}

	delete pChild;

	return FEDM_OK;
}

FEDM_XML_TREEITEM* FEDM_XmlParser_TinyReader::FindTag(	wstring sTag,
														unsigned int uiTagLevel,
														bool bNext )
{
	char cTag[256];
	wchar_t wcValue[256];
	TiXmlNode* node				= NULL;
	TiXmlNode* parent			= NULL;
	TiXmlNode* child			= NULL;
	TiXmlElement* element		= NULL;
	TiXmlAttribute* attr		= NULL;
	FEDM_XML_TREEITEM* item		= NULL;
	FEDM_XML_ATTRIBUTE* attrib	= NULL;

	swscanf(sTag.c_str(), L"%S", cTag);
	
	// determine the parent node
	if(m_pLastParent == NULL)
	{
		if(uiTagLevel == 0) // root tag is (mostly) "OBID"
		{
			node = m_pDoc->RootElement();
			if(node == NULL)
			{
				m_pXmlParser->SetLastError(FEDM_XML_ERROR_TAG_NOT_FOUND);
				return NULL;
			}

			m_uiLastTagLevel = 0;
			m_pLastChild[0] = node;

			element = node->ToElement();
			if(element == NULL)
			{
				m_pXmlParser->SetLastError(FEDM_XML_ERROR_TAG_NOT_FOUND);
				return NULL;
			}

			const char* cTagFound = element->Value();
			if(cTagFound != NULL)
			{
				if(strcmp(cTag, cTagFound) != 0)
				{
					m_pXmlParser->SetLastError(FEDM_XML_ERROR_TAG_NOT_FOUND);
					return NULL;
				}
			}

			// create item structure
			item = new FEDM_XML_TREEITEM;
			item->sTag = sTag;

			// fill with value and attributes
			if(element->GetText() != NULL)
			{
				if(strcmp(element->GetText(), "(null)")!=0)
				{
#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
					swprintf(wcValue, L"%S", element->GetText());
	#else
		#if defined(_WIN32_WCE)
					swprintf(wcValue, L"%S", element->GetText());
		#else
					swprintf(wcValue, 256, L"%S", element->GetText());
		#endif
	#endif
#else
					swprintf(wcValue, 256, L"%s", element->GetText());
#endif
					item->sValue = wcValue;
				}
			}

			attr = element->FirstAttribute();
			while(attr)
			{
				// create attribute pair
				attrib = new FEDM_XML_ATTRIBUTE;
				if(attrib == NULL)
				{
					m_pXmlParser->SetLastError(FEDM_ERROR_NO_MORE_MEM);
					break;
				}

#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
				swprintf(wcValue, L"%S", attr->Name());
	#else
		#if defined(_WIN32_WCE)
				swprintf(wcValue, L"%S", attr->Name());
		#else
				swprintf(wcValue, 256, L"%S", attr->Name());
		#endif
	#endif
#else
				swprintf(wcValue, 256, L"%s", attr->Name());
#endif
				attrib->sAttribute = wcValue;
#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
				swprintf(wcValue, L"%S", attr->Value());
	#else
		#if defined(_WIN32_WCE)
				swprintf(wcValue, L"%S", attr->Value());
		#else
				swprintf(wcValue, 256, L"%S", attr->Value());
		#endif
	#endif
#else
				swprintf(wcValue, 256, L"%s", attr->Value());
#endif
				attrib->sValue = wcValue;
				item->aAttribList.push_back(attrib);

				attr = attr->Next();
			}

			return item;
		}
		else if(uiTagLevel == 1)
		{
			node = m_pDoc->RootElement();
			if(node == NULL)
			{
				m_pXmlParser->SetLastError(FEDM_XML_ERROR_TAG_NOT_FOUND);
				return NULL;
			}

			parent = node;
		}
		else
		{
			m_pXmlParser->SetLastError(FEDM_XML_ERROR_TAG_NOT_FOUND);
			return NULL;
		}
	}
	else
	{
		parent = m_pLastParent;

		// traverse upwards
		if(uiTagLevel < m_uiLastTagLevel)
		{
			for(unsigned int uiCnt=m_uiLastTagLevel; uiCnt>uiTagLevel; uiCnt--)
			{
				node = parent->Parent();
				if(node == NULL)
				{
					m_pXmlParser->SetLastError(FEDM_XML_ERROR_TAG_NOT_FOUND);
					return NULL;
				}

				parent = node;
				m_pLastChild[uiCnt] = NULL;
			}
		}
		else if(uiTagLevel > m_uiLastTagLevel) // traverse downwards
		{
			parent = m_pLastChild[m_uiLastTagLevel];
		}
	}

	if(bNext)
	{
		if(m_pLastChild[uiTagLevel] == NULL)
		{
			node = parent->FirstChild(cTag);
		}
		else
		{
			node = m_pLastChild[uiTagLevel]->NextSibling(cTag);
		}
	}
	else
	{
		node = parent->FirstChild(cTag);
	}

	if(node == NULL)
	{
		m_pXmlParser->SetLastError(FEDM_XML_ERROR_TAG_NOT_FOUND);
		return NULL;
	}

	child = node;
	element = child->ToElement();
	if(element == NULL)
	{
		m_pXmlParser->SetLastError(FEDM_XML_ERROR_TAG_NOT_FOUND);
		return NULL;
	}

	// create item structure
	item = new FEDM_XML_TREEITEM;
	if(item == NULL)
	{
		m_pXmlParser->SetLastError(FEDM_ERROR_NO_MORE_MEM);
		return NULL;
	}
	item->sTag = sTag;

	if(element->GetText() != NULL)
	{
		if(strcmp(element->GetText(), "(null)")!=0)
		{
#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
			swprintf(wcValue, L"%S", element->GetText());
	#else
		#if defined(_WIN32_WCE)
			swprintf(wcValue, L"%S", element->GetText());
		#else
			swprintf(wcValue, 256, L"%S", element->GetText());
		#endif
	#endif
#else
			swprintf(wcValue, 256, L"%s", element->GetText());
#endif
			item->sValue = wcValue;
		}
	}

	attr = element->FirstAttribute();
	while(attr)
	{
		// create attribute pair
		attrib = new FEDM_XML_ATTRIBUTE;
		if(attrib == NULL)
		{
			m_pXmlParser->SetLastError(FEDM_ERROR_NO_MORE_MEM);
			break;
		}

#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
		swprintf(wcValue, L"%S", attr->Name());
	#else
		#if defined(_WIN32_WCE)
		swprintf(wcValue, L"%S", attr->Name());
		#else
		swprintf(wcValue, 256, L"%S", attr->Name());
		#endif
	#endif
#else
		swprintf(wcValue, 256, L"%s", attr->Name());
#endif
		attrib->sAttribute = wcValue;
#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
		swprintf(wcValue, L"%S", attr->Value());
	#else
		#if defined(_WIN32_WCE)
		swprintf(wcValue, L"%S", attr->Value());
		#else
		swprintf(wcValue, 256, L"%S", attr->Value());
		#endif
	#endif
#else
		swprintf(wcValue, 256, L"%s", attr->Value());
#endif
		attrib->sValue = wcValue;
		item->aAttribList.push_back(attrib);

		attr = attr->Next();
	}


	// save search status for next FindTag()
	m_uiLastTagLevel			= uiTagLevel;
	m_pLastParent				= parent;
	m_pLastChild[uiTagLevel]	= child;

	return item;
}

FEDM_XML_TREEITEM* FEDM_XmlParser_TinyReader::FindTag(	wstring sTag,
														wstring sAttribute,
														unsigned int uiTagLevel,
														bool bNext )
{
	FEDM_XML_TREEITEM* item = FindTag(sTag, uiTagLevel, bNext);
	if(item == NULL)
		return NULL;

	for(int i=0; i<(int)item->aAttribList.size(); ++i)
	{
		if(item->aAttribList[i]->sAttribute == sAttribute)
		{
			m_pXmlParser->SetLastError(FEDM_OK);
			return item;
		}
	}

	m_pXmlParser->SetLastError(FEDM_XML_ERROR_NO_TAG_ATTRIBUTE);
	return NULL;
}
