/*-------------------------------------------------------
|                                                       |
|                     FEDM_XMLBase.cpp	                |
|                                                       |
---------------------------------------------------------

Copyright � 2000-2011	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	01.04.2003
Version       		:	04.00.02 / 22.08.2011 / M. Hultsch

Operation Systems	:	independent

Function			:	base class for XML document handler
						
Note				:	this XML document handler is designed only for use
						inside the class library FEDM !!


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#ifdef _FEDM_XML_SUPPORT

//	#define _FEDM_TRACE

#include "FEDM_XmlBase.h"
//#include <io.h>
//#include <fcntl.h>
#if !defined(_WIN32_WCE)
#include <sys/stat.h>
#endif

#ifdef _FEDM_LINUX
	#include <string.h>
#endif

#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif


//#####################################################################################
// Construction/Destruction
//#####################################################################################

/***************************************************************************
  Begin        	:	01.04.2003 / M. Hultsch
  Version       :	01.08.00 / 23.04.2003 / M. Hultsch

  Function		:	constructor

  Parameters	:	-

  Return value	:	-
***************************************************************************/
FEDM_XMLBase::FEDM_XMLBase()
{
	m_iLastError		= 0;	// last error code
	m_sXmlStream		= L"";	// xml text in UNICODE
	m_file				= NULL;	// file handle

	m_wsXmlVer			= L"";
	m_wsEncoding		= L"";
	m_wsStandalone		= L"";
}

/***************************************************************************
  Begin        	:	01.04.2003 / M. Hultsch
  Version       :	01.08.00 / 07.04.2003 / M. Hultsch

  Function		:	destructor

  Parameters	:	-

  Return value	:	-
***************************************************************************/
FEDM_XMLBase::~FEDM_XMLBase()
{
	DeleteXmlTree();

	if(m_file != NULL)
		fclose(m_file);
}

/***************************************************************************
  Begin        	:	07.04.2003 / M. Hultsch
  Version       :	01.08.00 / 07.04.2003 / M. Hultsch

  Function		:	main delete function for tree items

  Parameters	:	-

  Return value	:	-
***************************************************************************/
void FEDM_XMLBase::DeleteXmlTree()
{
	FEDM_XML_TREE_LIST_ITOR Itor;

	for(Itor = m_XmlTree.begin(); Itor != m_XmlTree.end(); Itor++)
	{
		if((*Itor) != NULL)
		{
			DeleteTagItem(*(Itor));
		}
	}

	m_XmlTree.clear();
}

/***************************************************************************
  Begin        	:	07.04.2003 / M. Hultsch
  Version       :	01.08.00 / 24.04.2003 / M. Hultsch

  Function		:	delete of a child tree item
					- for recursive use -

  Parameters	:	FEDM_XML_TREEITEM* item - parent item

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FEDM_XMLBase::DeleteTagItem(FEDM_XML_TREEITEM* item)
{
	FEDM_CHK5(item);

#if defined(_MSC_VER) && defined(_FEDM_TRACE) && defined(_DEBUG)
	char buf[256];
#endif
	FEDM_XML_TREE_LIST_ITOR TreeItor;
	FEDM_XML_ATTRIBUTE_LIST_ITOR AttrItor;

	for(TreeItor = item->aChildList.begin(); TreeItor != item->aChildList.end(); TreeItor++)
	{
		if((*TreeItor) != NULL)
		{
			DeleteTagItem(*TreeItor);
		}
	}

	item->aChildList.clear();

	for(AttrItor = item->aAttribList.begin(); AttrItor != item->aAttribList.end(); AttrItor++)
	{
#if defined(_MSC_VER) && defined(_FEDM_TRACE) && defined(_DEBUG)
		sprintf(buf, "%S=%S", (*AttrItor)->sAttribute.c_str(), (*AttrItor)->sValue.c_str());
		TRACE("deleting attribute: %s\n", buf);
		::Sleep(0);
#endif
		(*AttrItor)->sAttribute.erase();
		(*AttrItor)->sValue.erase();

		delete *AttrItor;
	}

#if defined(_MSC_VER) && defined(_FEDM_TRACE) && defined(_DEBUG)
	swscanf(item->sTag.c_str(), L"%S", buf);
	TRACE("deleting tag: %s\n", buf);
	::Sleep(0);
#endif
	delete item;

	return FEDM_OK;
}


//#####################################################################################
// query functions
//#####################################################################################

/***************************************************************************
  Begin        	:	28.03.2007 / M. Hultsch
  Version       :	02.05.08 / 28.03.2007 / M. Hultsch

  Function		:	open a xml document and get the document type

  Parameters	:	char* sFileName		- pointer to string with file name
					char* sDocType		- pointer to string for document type
					char* sDocVersion	- pointer to string for document version

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FEDM_XMLBase::QueryDocType(char* sFileName, char* sDocType, char* sDocVersion)
{
	int iBack = 0;
	unsigned int i = 0;
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
	AddTagItem(parent, child);
	m_wsDocType = child->sValue;

#if defined(__BORLANDC__) || defined(_MSC_VER)
	for (i=0; i<child->sValue.length(); i++)
		sDocType[i] = (char)(child->sValue.c_str())[i];
	sDocType[i] = '\0';
#elif defined(_FEDM_SUPPORT_UCLINUX)
	FEDM_SWSCANF(child->sValue.c_str(), sDocType);
#else
	swscanf(child->sValue.c_str(), L"%s", sDocType);
#endif

	// header tag document version
	child = FindTag(FEDM_XML_TAG_DOC_VERSION_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	m_wsDocVer = child->sValue;

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
  Begin        	:	08.08.2011 / M. Hultsch
  Version       :	04.00.02 / 08.08.2011 / M. Hultsch

  Function		:	open a xml document and get the document type

  Parameters	:	char* sFileName		- pointer to string with file name
					char* sRootTag		- pointer to root tag name
					char* sDocType		- pointer to string for document type
					char* sDocVersion	- pointer to string for document version

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FEDM_XMLBase::QueryDocType(char* sFileName, const char* sRootTag, char* sDocType, char* sDocVersion)
{
	int iBack = 0;
	unsigned int i = 0;
	wchar_t wcRootTag[64];
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

	// read complete xml document into string
	if(ReadDoc())
	{
		CloseDoc();
		return m_iLastError;
	}

	CloseDoc();

	// root tag
	if(strlen(sRootTag) > 0)
	{
#if defined(__BORLANDC__)
		swprintf(wcRootTag, L"%S", sRootTag);
#elif defined(_MSC_VER)
	#if defined(_WIN32_WCE)
		swprintf(wcRootTag, L"%S", sRootTag);
	#else
	#if _MSC_VER <= 1200
		swprintf(wcRootTag, L"%S", sRootTag);
	#else
		swprintf(wcRootTag, 64, L"%S", sRootTag);
	#endif
	#endif
#else
		swprintf(wcRootTag, 64, L"%S", sRootTag);
#endif
		root = FindTag(wcRootTag);
		if(root == NULL)
			return m_iLastError;
		AddTagItem(NULL, root);
	}
	else
	{
		root = new FEDM_XML_TREEITEM;
		AddTagItem(NULL, root);
	}

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
	AddTagItem(parent, child);
	m_wsDocType = child->sValue;

#if defined(__BORLANDC__) || defined(_MSC_VER)
	for (i=0; i<child->sValue.length(); i++)
		sDocType[i] = (char)(child->sValue.c_str())[i];
	sDocType[i] = '\0';
#elif defined(_FEDM_SUPPORT_UCLINUX)
	FEDM_SWSCANF(child->sValue.c_str(), sDocType);
#else
	swscanf(child->sValue.c_str(), L"%s", sDocType);
#endif

	// header tag document version
	child = FindTag(FEDM_XML_TAG_DOC_VERSION_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	m_wsDocVer = child->sValue;

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




//#####################################################################################
// document management
//#####################################################################################

/***************************************************************************
  Begin        	:	01.04.2003 / M. Hultsch
  Version       :	01.08.00 / 03.04.2003 / M. Hultsch

  Function		:	open a file
					append xml header, if write mode

  Parameters	:	char*	sFileName		- pointer to file name with path
					char*	sMode			- file open options
											  "r" for read
											  "w" for write
					bool	bAddCopyright	- add copiright comment after head line

  Return value	:	FEDM_OK or error code
***************************************************************************/
int FEDM_XMLBase::OpenDoc(char* sFileName, char* sMode, bool bAddCopyright)
{
	FEDM_CHK5(sFileName);
	FEDM_CHK5(sMode);

	int iBack = 0;

	m_file = fopen(sFileName, sMode);
	if(m_file == NULL)
		FEDM_RETURN(FEDM_ERROR_OPEN_FILE);

#if defined(_MSC_VER) && defined(_FEDM_TRACE) && defined(_DEBUG)
	TRACE("open xml doc: %s\n", sFileName);
	::Sleep(0);
#endif

	// add header if write mode
	if(strcmp("w", sMode) == 0)
	{
		m_sXmlStream  = L"<?xml version=";
		m_sXmlStream += L"\"";
		m_sXmlStream += FEDM_XML_STRING_XML_VERSION_UC;
		m_sXmlStream += L"\"";
		m_sXmlStream += L" encoding=";
		m_sXmlStream += L"\"";
		m_sXmlStream += FEDM_XML_STRING_XML_ENCODING_UC;
		m_sXmlStream += L"\"";
		m_sXmlStream += L" standalone=";
		m_sXmlStream += L"\"";
		m_sXmlStream += FEDM_XML_STRING_XML_STANDALONE_UC;
		m_sXmlStream += L"\"";
		m_sXmlStream += L"?>";
		m_sXmlStream += L"\n";

		if(bAddCopyright)
		{
			m_sXmlStream += m_sXmlComment;
		}
	}
	else
	{
		// query the file size to reserve memory for the wstring buffer
#if defined(_FEDM_WINDOWS)
#if defined(_WIN32_WCE)
		CFileStatus status;
		CString strFileName = sFileName;
		if(CFile::GetStatus(strFileName, status))
		{
			m_sXmlStream.reserve((unsigned int)status.m_size + 1);
		}
#else
		struct _stat buf;
		iBack = _stat(sFileName, &buf);
		
		if(iBack == 0)
		{
			// resize buffer for m_sXmlStream
			m_sXmlStream.reserve(buf.st_size + 1);
		}
#endif
#endif
#if defined(_FEDM_LINUX)
		struct stat buf;
		iBack = stat(sFileName, &buf);
		
		if(iBack == 0)
		{
			// resize buffer for m_sXmlStream
			m_sXmlStream.reserve(buf.st_size + 1);
		}
#endif
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	01.04.2003 / M. Hultsch
  Version       :	01.08.00 / 03.04.2003 / M. Hultsch

  Function		:	close the file

  Parameters	:	-

  Return value	:	FEDM_OK
***************************************************************************/
int FEDM_XMLBase::CloseDoc()
{
	if(m_file != NULL)
	{
#if defined(_MSC_VER) && defined(_FEDM_TRACE) && defined(_DEBUG)
		TRACE("close xml doc\n");
		::Sleep(0);
#endif
		fclose(m_file);
	}

	m_file = NULL;

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	01.04.2003 / M. Hultsch
  Version       :	01.08.00 / 14.05.2003 / M. Hultsch

  Function		:	checks, if file is a xml document

  Parameters	:	-

  Return value	:	FEDM_OK or error code
***************************************************************************/
int FEDM_XMLBase::IsXmlDoc()
{
	if(m_file == NULL)
		FEDM_RETURN(FEDM_ERROR_NULL_POINTER);

	char cBuffer[17];
	char* cSubStr;
	size_t iLen = 0;

	// set read position to begin of file
	if(fseek(m_file, 0, SEEK_SET))
		FEDM_RETURN(FEDM_ERROR_READ_FILE);


	// read first 16 chars (string "<?xml" inside expected)
	iLen = fread(cBuffer, sizeof(char), 16, m_file);
	if(iLen < 16)
		FEDM_RETURN(FEDM_XML_ERROR_NO_XML_FILE);

	cBuffer[16] = '\0';

	// is string "<?xml" inside?
	cSubStr = strstr(cBuffer, "<?xml");
	if(cSubStr == NULL)
		FEDM_RETURN(FEDM_XML_ERROR_NO_XML_FILE);

	cSubStr[5] = '\0';

	if(strcmp("<?xml", cSubStr) == 0)
		FEDM_RETURN(FEDM_OK);

	// no string match found
	FEDM_RETURN(FEDM_XML_ERROR_NO_XML_FILE);
}

/***************************************************************************
  Begin        	:	01.04.2003 / M. Hultsch
  Version       :	02.05.08 / 20.03.2007 / M. Hultsch

  Function		:	checks, if xml document has an OBID tag

  Parameters	:	-

  Return value	:	FEDM_OK or error code
***************************************************************************/
int FEDM_XMLBase::HasOBIDTag()
{
	if(m_file == NULL)
		FEDM_RETURN(FEDM_ERROR_NULL_POINTER);

	char cBuffer[128];
	char* cSubStr = NULL;
	size_t iLen = 0;

	// set read position to begin of file
	if(fseek(m_file, 0, SEEK_SET))
		FEDM_RETURN(FEDM_ERROR_READ_FILE);

	// read first 128 wide chars (string "<OBID>" inside expected, or repeat)
	iLen = fread(cBuffer, sizeof(char), 128, m_file);
	while(iLen > 0)
	{
		cSubStr = strstr(cBuffer, "<OBID>");
		if(cSubStr != NULL)
			break;

		iLen = fread(cBuffer, sizeof(char), 128, m_file);
	}

	if(cSubStr == NULL)
		FEDM_RETURN(FEDM_XML_ERROR_NO_OBID_TAG);

	cSubStr[6] = '\0';

	if(strcmp("<OBID>", cSubStr) == 0)
		FEDM_RETURN(FEDM_OK);

	// no string match found
	FEDM_RETURN(FEDM_XML_ERROR_NO_OBID_TAG);
}

/***************************************************************************
  Begin        	:	22.08.2011 / M. Hultsch
  Version       :	04.00.02 / 22.08.2011 / M. Hultsch

  Function		:	checks, if xml document has a root tag <szOemRootTag>

  Parameters	:	-

  Return value	:	FEDM_OK or error code
***************************************************************************/
int FEDM_XMLBase::HasRootTag(const char* szOemRootTag)
{
	if(m_file == NULL)
		FEDM_RETURN(FEDM_ERROR_NULL_POINTER);

	char cBuffer[128];
	char* cSubStr = NULL;
	size_t iLen = 0;
	string sRootTag = "<";
	sRootTag += szOemRootTag;
	sRootTag += ">";

	// set read position to begin of file
	if(fseek(m_file, 0, SEEK_SET))
		FEDM_RETURN(FEDM_ERROR_READ_FILE);

	// read first 128 wide chars (string "<OBID>" inside expected, or repeat)
	iLen = fread(cBuffer, sizeof(char), 128, m_file);
	while(iLen > 0)
	{
		cSubStr = strstr(cBuffer, sRootTag.c_str());
		if(cSubStr != NULL)
			break;

		iLen = fread(cBuffer, sizeof(char), 128, m_file);
	}

	if(cSubStr == NULL)
		FEDM_RETURN(FEDM_XML_ERROR_TAG_NOT_FOUND);

	cSubStr[sRootTag.size()] = '\0';

	if(strcmp(sRootTag.c_str(), cSubStr) == 0)
		FEDM_RETURN(FEDM_OK);

	// no string match found
	FEDM_RETURN(FEDM_XML_ERROR_TAG_NOT_FOUND);
}

/***************************************************************************
  Begin        	:	01.04.2003 / M. Hultsch
  Version       :	01.08.00 / 04.09.2003 / M. Hultsch

  Function		:	read the complete xml document

  Parameters	:	-

  Return value	:	FEDM_OK or error code
***************************************************************************/
int FEDM_XMLBase::ReadDoc()
{
	bool bFirstRead = true;
	char cBuffer[101];
	wchar_t wcBuffer[101];
	size_t iRead = 0;

	if(m_file == NULL)
		FEDM_RETURN(FEDM_ERROR_NULL_POINTER);

	// set read position to begin of file
	if(fseek(m_file, 0, SEEK_SET))
		FEDM_RETURN(FEDM_ERROR_READ_FILE);

	// delete content of xml string
	m_sXmlStream.erase();

	// read loop
	while(! feof(m_file))
	{
		iRead = fread(cBuffer, sizeof(char), 100, m_file);
		if(iRead <= 0)
		{
			int err = ferror(m_file);
			if(err)
			{
				FEDM_RETURN(FEDM_ERROR_READ_FILE);
			}
			else
			{
			}
		}

		cBuffer[iRead] = '\0';
#if defined(__BORLANDC__) || defined(_MSC_VER)
		swprintf(wcBuffer, L"%S", cBuffer);
#else
		swprintf(wcBuffer, 101, L"%s", cBuffer);
#endif
		m_sXmlStream += wcBuffer;

		if(bFirstRead)
		{
			bFirstRead = false;
			ReadXmlHeader(cBuffer);
		}
	}

	// delete old xml tree
	DeleteXmlTree();

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	14.09.2007 / M. Hultsch
  Version       :	03.00.08 / 30.06.2009 / M. Hultsch

  Function		:	ste xml stream to parse for

  Parameters	:	wstring sXmlStream

  Return value	:	FEDM_OK or error code
***************************************************************************/
int FEDM_XMLBase::LoadStream(wstring sXmlStream)
{
	// delete content of xml string
	m_sXmlStream.erase();

	m_sXmlStream = sXmlStream;

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	01.04.2003 / M. Hultsch
  Version       :	01.08.00 / 03.09.2003 / M. Hultsch

  Function		:	write the complete xml document into the file

  Parameters	:	-

  Return value	:	FEDM_OK or error code
***************************************************************************/
int FEDM_XMLBase::WriteDoc()
{
	if(m_file == NULL)
		FEDM_RETURN(FEDM_ERROR_NULL_POINTER);

	FEDM_XML_TREE_LIST_ITOR Itor;

	// build xml string
	for(Itor = m_XmlTree.begin(); Itor != m_XmlTree.end(); Itor++)
	{
		if((*Itor) != NULL)
			WriteTagStream(*Itor);
	}

	// write xml string into document
	int iBack = fwprintf(m_file, m_sXmlStream.c_str());

	// delete content of xml string
	m_sXmlStream.erase();
	
	// delete xml tree
	DeleteXmlTree();

	if(iBack <= 0)
		FEDM_RETURN(FEDM_ERROR_WRITE_FILE);

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	13.09.2007 / M. Hultsch
  Version       :	03.00.00 / 13.09.2007 / M. Hultsch

  Function		:	write the complete xml document into a stream

  Parameters	:	-

  Return value	:	FEDM_OK or error code
***************************************************************************/
wstring FEDM_XMLBase::WriteStream()
{
	FEDM_XML_TREE_LIST_ITOR Itor;

	// delete content of xml string
	m_sXmlStream.erase();
	
	// build xml string
	for(Itor = m_XmlTree.begin(); Itor != m_XmlTree.end(); Itor++)
	{
		if((*Itor) != NULL)
			WriteTagStream(*Itor);
	}


	// delete xml tree
	DeleteXmlTree();

	return m_sXmlStream;
}

/***************************************************************************
  Begin        	:	01.04.2003 / M. Hultsch
  Version       :	04.00.02 / 22.08.2011 / M. Hultsch

  Function		:	write one tag into the xml string
					- for recursive use -

  Parameters	:	FEDM_XML_TREEITEM* item

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FEDM_XMLBase::WriteTagStream(FEDM_XML_TREEITEM* item)
{
	FEDM_CHK5(item);

	int iPos = 0;
	wstring wStr;
	FEDM_XML_TREE_LIST_ITOR TreeItor;
	FEDM_XML_ATTRIBUTE_LIST_ITOR AttrItor;

	// building string part 1a: start tag
	m_sXmlStream += L"<";
	m_sXmlStream += item->sTag;

#if defined(_MSC_VER) && defined(_FEDM_TRACE) && defined(_DEBUG)
	char buf[256];
	swscanf(item->sTag.c_str(), L"%S", buf);
	TRACE("writing tag: %s\n", buf);
	::Sleep(0);
#endif

	// building string part 2: insert tag attributes
	for(AttrItor = item->aAttribList.begin(); AttrItor != item->aAttribList.end(); AttrItor++)
	{
		m_sXmlStream += L" ";
		m_sXmlStream += (*AttrItor)->sAttribute;
		m_sXmlStream += L"=";
		m_sXmlStream += L"\"";
		m_sXmlStream += (*AttrItor)->sValue;
		m_sXmlStream += L"\"";

#if defined(_MSC_VER) && defined(_FEDM_TRACE) && defined(_DEBUG)
		char buf[256];
		sprintf(buf, "%S=%S", (*AttrItor)->sAttribute.c_str(), (*AttrItor)->sValue.c_str());
		TRACE("writing attribute: %s\n", buf);
		::Sleep(0);
#endif
	}

	// building string part 1b: start tag
	if(item->bEmpty)
		m_sXmlStream += L"/";
	m_sXmlStream += L">";
	if(item->bNewLine)		// add new line char
		m_sXmlStream += L"\n";

	// building string part 3: insert child tags
	for(TreeItor = item->aChildList.begin(); TreeItor != item->aChildList.end(); TreeItor++)
	{
		if((*TreeItor) != NULL)
			WriteTagStream(*TreeItor);
	}

	// building string part 4: tag value
	if(! item->bEmpty)
	{
		// search for: < > & " ' 
		iPos = (int)item->sValue.find(L"&");
		while(iPos >= 0)
		{
			item->sValue.replace(iPos, 1, L"&amp;");
			iPos = (int)item->sValue.find(L"&", iPos+1);
		}

		iPos = (int)item->sValue.find(L"<");
		while(iPos >= 0)
		{
			item->sValue.replace(iPos, 1, L"&lt;");
			iPos = (int)item->sValue.find(L"<", iPos+1);
		}

		iPos = (int)item->sValue.find(L">");
		while(iPos >= 0)
		{
			item->sValue.replace(iPos, 1, L"&gt;");
			iPos = (int)item->sValue.find(L">", iPos+1);
		}

		iPos = (int)item->sValue.find(L"\"");
		while(iPos >= 0)
		{
			item->sValue.replace(iPos, 1, L"&quot;");
			iPos = (int)item->sValue.find(L"\"", iPos+1);
		}

		iPos = (int)item->sValue.find(L"'");
		while(iPos >= 0)
		{
			item->sValue.replace(iPos, 1, L"&apos;");
			iPos = (int)item->sValue.find(L"'", iPos+1);
		}

		m_sXmlStream += item->sValue;
	}

	// building string part 5: end tag
	if(! item->bEmpty)	// tag name in end tag
	{
		m_sXmlStream += L"</";
		m_sXmlStream += item->sTag;
		m_sXmlStream += L">";
	}

	m_sXmlStream += L"\n";

	FEDM_RETURN(FEDM_OK);
}



//#####################################################################################
// tag functions
//#####################################################################################

/***************************************************************************
  Begin        	:	08.04.2003 / M. Hultsch
  Version       :	01.08.00 / 08.04.2003 / M. Hultsch

  Function		:	build a tree item

  Parameters	:	wstring sTag	- tags name (in UNICODE)
					bool bNewLine	- new line after end tag
					bool bEmpty		- empty tag

  Return value	:	FEDM_XML_TREEITEM*	- pointer to new tree item
***************************************************************************/
FEDM_XML_TREEITEM* FEDM_XMLBase::BuildTag(wstring sTag, bool bNewLine, bool bEmpty)
{
	if(sTag.empty())
	{
		SetLastError(FEDM_ERROR_PARAMETER);
		return NULL;
	}

	FEDM_XML_TREEITEM* item = new FEDM_XML_TREEITEM;

	if(item == NULL)
	{
		SetLastError(FEDM_ERROR_NO_MORE_MEM);
		return NULL;
	}

	item->sTag = sTag;
	item->bNewLine = bNewLine;
	item->bEmpty = bEmpty;

#if defined(_MSC_VER) && defined(_FEDM_TRACE) && defined(_DEBUG)
	char buf[256];
	swscanf(sTag.c_str(), L"%S", buf);
	TRACE("build tag: %s\n", buf);
	::Sleep(0);
#endif

	SetLastError(FEDM_OK);
	return item;
}

/***************************************************************************
  Begin        	:	07.04.2003 / M. Hultsch
  Version       :	01.08.00 / 08.04.2003 / M. Hultsch

  Function		:	add a value to a tree item

  Parameters	:	FEDM_XML_TREEITEM* item	- pointer to tree item
					wstring sValue			- tags value (in UNICODE)

  Return value	:	FEDM_OK or error code
***************************************************************************/
int FEDM_XMLBase::AddTagValue(FEDM_XML_TREEITEM* item, wstring sValue)
{
	FEDM_CHK5(item);

	if(sValue.empty())
	{
		item->bEmpty = true;
		FEDM_RETURN(FEDM_OK);
	}

	item->sValue = sValue;
	item->bEmpty = false;
#if defined(_MSC_VER) && defined(_FEDM_TRACE) && defined(_DEBUG)
	char buf[256];
	sprintf(buf, "%S", item->sValue.c_str());
	TRACE("add tag value: %s\n", buf);
	::Sleep(0);
#endif

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	07.04.2003 / M. Hultsch
  Version       :	03.00.00 / 05.05.2007 / M. Hultsch

  Function		:	add an attribute to a tree item

  Parameters	:	FEDM_XML_TREEITEM* item	- pointer to tree item
					wstring sAttrib			- tags attribute (in UNICODE)
					wstring sValue			- tags attribute value (in UNICODE)

  Return value	:	FEDM_OK or error code
***************************************************************************/
int FEDM_XMLBase::AddTagAttrib(FEDM_XML_TREEITEM* item, wstring sAttrib, wstring sValue)
{
	FEDM_CHK5(item);

	if(sAttrib.empty())
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	
	FEDM_XML_ATTRIBUTE* pAttribute = new FEDM_XML_ATTRIBUTE;

	if(pAttribute == NULL)
		FEDM_RETURN(FEDM_ERROR_NO_MORE_MEM);

	pAttribute->sAttribute = sAttrib;
	pAttribute->sValue = sValue;

	item->aAttribList.push_back(pAttribute);
#if defined(_MSC_VER) && defined(_FEDM_TRACE) && defined(_DEBUG)
	char buf[256];
	sprintf(buf, "%S=%S", pAttribute->sAttribute.c_str(), pAttribute->sValue.c_str());
	TRACE("add tag attribute: %s\n", buf);
	::Sleep(0);
#endif

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	01.04.2003 / M. Hultsch
  Version       :	01.08.00 / 08.04.2003 / M. Hultsch

  Function		:	adds a tag to the xml tree

  Parameters	:	FEDM_XML_TREEITEM* pParent	- pointer to tree item of parent
					FEDM_XML_TREEITEM* pChild	- pointer to tree item of child

  Return value	:	FEDM_OK or error code
***************************************************************************/
int FEDM_XMLBase::AddTagItem(FEDM_XML_TREEITEM* pParent, FEDM_XML_TREEITEM* pChild)
{
	FEDM_CHK5(pChild);

#if defined(_MSC_VER) && defined(_FEDM_TRACE) && defined(_DEBUG)
	char buf[256];
#endif

	if(pParent == NULL)
	{
		// root item

		if(m_XmlTree.empty())
		{
			m_XmlTree.push_back(pChild);
#if defined(_MSC_VER) && defined(_FEDM_TRACE) && defined(_DEBUG)
			sprintf(buf, "%S", pChild->sTag.c_str());
			TRACE("add root tag: %s\n", buf);
			::Sleep(0);
#endif
		}
		else
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}
	else
	{
		// child item
		pParent->aChildList.push_back(pChild);
#if defined(_MSC_VER) && defined(_FEDM_TRACE) && defined(_DEBUG)
		sprintf(buf, "%S", pChild->sTag.c_str());
		TRACE("add child tag: %s\n", buf);
		::Sleep(0);
#endif
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	01.04.2003 / M. Hultsch
  Version       :	03.00.00 / 05.07.2007 / M. Hultsch

  Function		:	itentify a tag inside the xml string
					pick out all attributes and values and create a tree item

  Parameters	:	wstring sTag		- tag string to be identify (in UNICODE)
					bool bInside=false	- find the tag inside the positions iPos1 
										  and iPos2 of last function call.
										  This allows the search of child tags 
										  inside a previously found parent tag
					bool bSavePos		- save the tag positions iPos1 and iPos2
					bool bNext			- continue with search behind iPosEnd 

  Return value	:	FEDM_XML_TREEITEM*	- pointer to tree item
					NULL				- in error case
***************************************************************************/
FEDM_XML_TREEITEM* FEDM_XMLBase::FindTag(wstring sTag, 
										 bool bInside, 
										 bool bSavePos,
										 bool bNext)
{
	if(m_sXmlStream.empty())
	{
		SetLastError(FEDM_ERROR_NO_DATA);
		return NULL;
	}

	// static int because of its reuse in the following function call
	static int iPosStart = 0;	// position start of tag
	static int iPosEnd   = 0;	// position end of tag
	static int iTagLen	 = 0;	// length of start tag

	bool bAttrib = true;
	int iPos0 = 0;	// position start of empty tag
	int iPos1 = 0;	// position start of tag
	int iPos2 = 0;	// position end of tag
	int iPos3 = 0;	// position inside tag
	int iPos4 = 0;	// position inside tag
	int iPos5 = 0;	// position inside tag
	wstring s0, s1, s2, s3, s4;
	FEDM_XML_TREEITEM* item = NULL;
	FEDM_XML_ATTRIBUTE* attrib = NULL;

#if defined(_MSC_VER) && defined(_FEDM_TRACE) && defined(_DEBUG)
	char buf[256];
#endif

	// build search string for empty tag
	s0 = L"<";
	s0 += sTag;
	s0 += L"/>";

	// build search string for beginning tag
	s1 = L"<";
	s1 += sTag;

	// build search string for end tag
	s2 = L"</";
	s2 += sTag;
	s2 += L">";

	// create item structure
	item = new FEDM_XML_TREEITEM;
	if(item == NULL)
	{
		SetLastError(FEDM_ERROR_NO_MORE_MEM);
		return NULL;
	}
	item->sTag = sTag;
#if defined(_MSC_VER) && defined(_FEDM_TRACE) && defined(_DEBUG)
	sprintf(buf, "%S", sTag.c_str());
	TRACE("new tag: %s\n", buf);
	::Sleep(0);
#endif

	// set search position behind iPosEnd
	if(bNext)
	{
		iPosStart += iTagLen;
	}

	// search begin of tag
	if(bInside)
	{
		// search begin of empty tag inside the previously found tag
		iPos0 = (int)m_sXmlStream.find(s0.c_str(), iPosStart);
		if(iPos0 > 0 && iPos0 > iPosStart && iPos0 < iPosEnd)
		{
			item->bEmpty = true;
			SetLastError(FEDM_OK);
			return item;
		}

		// search begin of tag inside the previously found tag
		iPos1 = (int)m_sXmlStream.find(s1.c_str(), iPosStart);

		if(iPos1 < 0 || iPos1 < iPosStart || iPos1 > iPosEnd)
		{
			SetLastError(FEDM_XML_ERROR_TAG_NOT_FOUND);
			DeleteTagItem(item);
			return NULL;
		}

		// search end of tag
		iPos2 = (int)m_sXmlStream.find(s2.c_str(), iPos1);
		if(iPos2 < 0 || iPos2 < iPosStart || iPos2 > iPosEnd)
		{	// empty tag?
		
			// build search string for empty end tag..
			s2 = L"/>";
			// ..and try again
			iPos2 = (int)m_sXmlStream.find(s2.c_str(), iPos1);
			
			if(iPos2 < 0 || iPos2 < iPosStart || iPos2 > iPosEnd)
			{
				SetLastError(FEDM_XML_ERROR_TAG_NOT_FOUND);
				DeleteTagItem(item);
				return NULL;
			}

			// is there any '<' char inside?
			iPos3 = (int)m_sXmlStream.find(L"<", iPos1, iPos2-iPos1);
			if(iPos3 > 0 && (iPos3 < iPos1 || iPos3 > iPos2))
			{
				SetLastError(FEDM_XML_ERROR_DOC_NOT_WELL_FORMED);
				DeleteTagItem(item);
				return NULL;
			}

			item->bEmpty = true;

			// if no attributes, we can return
			if(iPos2 - iPos1 - s1.size() == 0)
			{
				SetLastError(FEDM_OK);
				return item;
			}
		}
		else
		{
			item->bEmpty = false;
		}
	}
	else
	{
		// search begin of tag inside the complete xml string
		if(bNext)
		{
			// search begin of empty tag
			iPos0 = (int)m_sXmlStream.find(s0.c_str(), iPosStart);
			if(iPos0 > 0 && iPos0 > iPosStart && iPos0 < iPosEnd)
			{
				item->bEmpty = true;
				SetLastError(FEDM_OK);
				return item;
			}

			// start position �s behind last start position
			iPos1 = (int)m_sXmlStream.find(s1.c_str(), iPosStart);
		}
		else
		{
			// search begin of empty tag inside the complete xml string
			iPos0 = (int)m_sXmlStream.find(s0.c_str());
			if(iPos0 > 0)
			{
				item->bEmpty = true;
				SetLastError(FEDM_OK);
				return item;
			}

			// start position is at begin of xml string
			iPos1 = (int)m_sXmlStream.find(s1.c_str());
		}

		if(iPos1 < 0 || iPos1 == (int)m_sXmlStream.size())
		{
			SetLastError(FEDM_XML_ERROR_TAG_NOT_FOUND);
			DeleteTagItem(item);
			return NULL;
		}

		// search end of tag
		iPos2 = (int)m_sXmlStream.find(s2, iPos1);
		if(iPos2 < 0 || iPos2 == (int)m_sXmlStream.size())
		{	// empty tag?
		
			// build search string for empty end tag..
			s2 = L"/>";
			// ..and try again
			iPos2 = (int)m_sXmlStream.find(s2, iPos1);
			
			if(iPos2 < 0 || iPos2 == (int)m_sXmlStream.size())
			{
				SetLastError(FEDM_XML_ERROR_TAG_NOT_FOUND);
				DeleteTagItem(item);
				return NULL;
			}

			// is there any '<' char inside?
			iPos3 = (int)m_sXmlStream.find(L"<", iPos1, iPos2-iPos1);
			if(iPos3 > 0 && (iPos3 < iPos1 || iPos3 > iPos2))
			{
				SetLastError(FEDM_XML_ERROR_DOC_NOT_WELL_FORMED);
				DeleteTagItem(item);
				return NULL;
			}

			item->bEmpty = true;

			// if no attributes, we can return
			if(iPos2 - iPos1 - s1.size() == 0)
			{
				SetLastError(FEDM_OK);
				return item;
			}
		}
		else
		{	
			item->bEmpty = false;
		}
	}

	// get next char and check, if it is a space char or '>' char
	s3 = m_sXmlStream.at(iPos1 + s1.size());
	if(s3 == L" ")
	{	
		// get end position of start tag
		iPos3 = (int)m_sXmlStream.find_first_of(L">", iPos1);

		// extract substring with all attrubutes
		s3 = m_sXmlStream.substr(iPos1 + s1.size() + 1, iPos3 - iPos1 - s1.size() - 1);

		// tokenize
		bAttrib = true;	// toggles between attributes name and attributes value
		iPos4 = 0; // set start position
		while(iPos4 < (int)s3.size())
		{
			if(bAttrib)
			{
				iPos5 = (int)s3.find_first_of(L"=", iPos4);
				if(iPos5 < 0 || iPos5 >= (int)s3.size())
				{
					SetLastError(FEDM_XML_ERROR_DOC_NOT_WELL_FORMED);
					break;
				}

				// create attribute pair
				attrib = new FEDM_XML_ATTRIBUTE;
				if(attrib == NULL)
				{
					SetLastError(FEDM_ERROR_NO_MORE_MEM);
					break;
				}

				// add attribute name
				if(s3[iPos4] == L' ')
					attrib->sAttribute = s3.substr(iPos4+1, iPos5-iPos4-1);
				else
					attrib->sAttribute = s3.substr(iPos4, iPos5-iPos4);

				bAttrib = false;	// continue with attribute value
				iPos4 = iPos5 + 1;	// positioning after '='
			}
			else
			{
				if(s3[iPos4] == L'\"')
				{
					iPos4++; // positioning after first '"'
					iPos5 = (int)s3.find_first_of(L"\"", iPos4);
					if(iPos5 <= 0 || iPos5 >= (int)s3.size())
					{
						SetLastError(FEDM_XML_ERROR_DOC_NOT_WELL_FORMED);
						break;
					}

					// add attribute value
					attrib->sValue = s3.substr(iPos4, iPos5-iPos4);;

					// add attribute pair to attribute list
					item->aAttribList.push_back(attrib);
					bAttrib = true;	// continue with attribute name
					iPos4 = iPos5 + 1;	// positioning after '"'
#if defined(_MSC_VER) && defined(_FEDM_TRACE) && defined(_DEBUG)
					sprintf(buf, "%S=%S", attrib->sAttribute.c_str(), attrib->sValue.c_str());
					TRACE("new tag attribute: %s\n", buf);
					::Sleep(0);
#endif
				}
				else
				{
					delete attrib;
					SetLastError(FEDM_XML_ERROR_DOC_NOT_WELL_FORMED);
					break;
				}
			}
		} // while(iPos4 < (int)s3.size())

		if(!item->bEmpty)
		{
			// get next char and check, if it is a LF char
			s3 = m_sXmlStream.at(iPos3 + 1);
			if(s3 == L"\n")
			{
				item->bNewLine = true;
			}
			else if(s3 != L"<")
			{
				item->bNewLine = false;
				if(iPos2 - iPos3 - 1 > 0)
				{
					// get tag value
					item->sValue = m_sXmlStream.substr(iPos3 + 1, iPos2 - iPos3 - 1);

					// search for: < > & " ' 
					int iPos = (int)item->sValue.find(L"&amp;");
					while(iPos >= 0)
					{
						item->sValue.erase(iPos+1, 4);
						iPos = (int)item->sValue.find(L"&amp;", iPos+1);
					}

					iPos = (int)item->sValue.find(L"&lt;");
					while(iPos >= 0)
					{
						item->sValue.replace(iPos, 1, L"<");
						iPos = (int)item->sValue.find(L"&lt;", iPos+1);
					}

					iPos = (int)item->sValue.find(L"&gt;");
					while(iPos >= 0)
					{
						item->sValue.replace(iPos, 1, L">");
						iPos = (int)item->sValue.find(L"&gt;", iPos+1);
					}

					iPos = (int)item->sValue.find(L"&quot;");
					while(iPos >= 0)
					{
						item->sValue.replace(iPos, 1, L"\"");
						iPos = (int)item->sValue.find(L"&quot;", iPos+1);
					}

					iPos = (int)item->sValue.find(L"&apos;");
					while(iPos >= 0)
					{
						item->sValue.replace(iPos, 1, L"'");
						iPos = (int)item->sValue.find(L"&apos;", iPos+1);
					}
				}
			}
		}
	}
	else if(s3 == L">")
	{
		// get next char and check, if it is a LF char
		s3 = m_sXmlStream.at(iPos1 + s1.size() + 1);
		if(s3 == L"\n")
		{
			item->bNewLine = true;
		}
		else if(s3 != L"<")
		{
			item->bNewLine = false;
			if(iPos2 - iPos1 - s1.size() - 1 > 0)
			{
				// get tag value
				item->sValue = m_sXmlStream.substr(iPos1 + s1.size() + 1, iPos2 - iPos1 - s1.size() - 1);

				// search for: < > & " ' 
				int iPos = (int)item->sValue.find(L"&amp;");
				while(iPos >= 0)
				{
					item->sValue.erase(iPos+1, 4);
					iPos = (int)item->sValue.find(L"&amp;", iPos+1);
				}

				iPos = (int)item->sValue.find(L"&lt;");
				while(iPos >= 0)
				{
					item->sValue.replace(iPos, 1, L"<");
					iPos = (int)item->sValue.find(L"&lt;", iPos+1);
				}

				iPos = (int)item->sValue.find(L"&gt;");
				while(iPos >= 0)
				{
					item->sValue.replace(iPos, 1, L">");
					iPos = (int)item->sValue.find(L"&gt;", iPos+1);
				}

				iPos = (int)item->sValue.find(L"&quot;");
				while(iPos >= 0)
				{
					item->sValue.replace(iPos, 1, L"\"");
					iPos = (int)item->sValue.find(L"&quot;", iPos+1);
				}

				iPos = (int)item->sValue.find(L"&apos;");
				while(iPos >= 0)
				{
					item->sValue.replace(iPos, 1, L"'");
					iPos = (int)item->sValue.find(L"&apos;", iPos+1);
				}
			}
		}
	}
	else if(s3 == L"/")
	{
		// empty tag
		item->bEmpty = true;
	}
	else
	{
		SetLastError(FEDM_XML_ERROR_DOC_NOT_WELL_FORMED);
		DeleteTagItem(item);
		return NULL;
	}

	if(bSavePos)
	{
		if(iPos1 > 0 && iPos2-iPos1 > 0)
		{
			iPosStart = iPos1;
			iPosEnd	  = iPos2;
			iTagLen	  = (int)s1.size();
		}
	}

	SetLastError(FEDM_OK);
	return item;
}

/***************************************************************************
  Begin        	:	27.03.2007 / M. Hultsch
  Version       :	03.00.00 / 05.07.2007 / M. Hultsch

  Function		:	itentify a tag inside the xml string
					pick out all attributes and values and create a tree item
					all position are saved in an array

  Parameters	:	wstring sTag			- tag string to be identify (in UNICODE)
					unsigned int uiTagLevel	- level of this tag = index in save position
					bool bNext				- continue with search behind iPosEnd 

  Return value	:	FEDM_XML_TREEITEM*	- pointer to tree item
					NULL				- in error case
***************************************************************************/
FEDM_XML_TREEITEM* FEDM_XMLBase::FindTag(	wstring sTag,
											unsigned int uiTagLevel,
											bool bNext)
{
	if(m_sXmlStream.empty())
	{
		SetLastError(FEDM_ERROR_NO_DATA);
		return NULL;
	}

	if(uiTagLevel >= 15)
	{
		SetLastError(FEDM_ERROR_ARRAY_BOUNDARY);
		return NULL;
	}

	// static int because of its reuse in the following function call
	static int iPosStart[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};	// position start of tag
	static int iPosEnd[]   = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};	// position end of tag

	bool bRoot = false;
	bool bAttrib = true;
	int iPos0 = 0;	// position start of empty tag
	int iPos1 = 0;	// position start of tag
	int iPos2 = 0;	// position end of tag
	int iPos3 = 0;	// position inside tag
	int iPos4 = 0;	// position inside tag
	int iPos5 = 0;	// position inside tag
	
	unsigned int uiLevel = 0;
	unsigned int i = 0;

	int iNextPosStart = 0;
	int iNextPos = 0;

	wstring s0, s1, s2, s3, s4;
	FEDM_XML_TREEITEM* item = NULL;
	FEDM_XML_ATTRIBUTE* attrib = NULL;

	// build search string for empty tag
	s0 = L"<";
	s0 += sTag;
	s0 += L"/>";

	// build search string for beginning tag
	s1 = L"<";
	s1 += sTag;

	// build search string for end tag
	s2 = L"</";
	s2 += sTag;
	s2 += L">";

	// create item structure
	item = new FEDM_XML_TREEITEM;
	if(item == NULL)
	{
		SetLastError(FEDM_ERROR_NO_MORE_MEM);
		return NULL;
	}
	item->sTag = sTag;

	if(uiTagLevel == 0)	// root tag
	{
		// initializing position marker for new parsing process
		bRoot = true;
		for(i=0; i<15; i++)
		{
			iPosStart[i] = 0;
			iPosEnd[i] = 0;
		}
	}

	// set search position behind iPosEnd
	if(bNext && !bRoot)
	{
		for(i=uiTagLevel; i<15; i++)
		{
			if(iPosStart[i] > 0)
			{
				iPosStart[i] = iPosEnd[uiTagLevel] + 1;
			}
		}

		if(iPosStart[uiTagLevel] > iPosEnd[uiTagLevel])
			iPosEnd[uiTagLevel] = iPosEnd[uiTagLevel-1];

		// for multiple tags with same tagname in same level
		iNextPosStart = iPosStart[uiTagLevel-1] + 1;
		if(iPosStart[uiTagLevel] > iPosStart[uiTagLevel-1])
			iNextPosStart = iPosStart[uiTagLevel];

	}
	else
	{
		if(uiTagLevel == 0)	// root tag
			iNextPosStart = iPosStart[0];
		else
			iNextPosStart = iPosStart[uiTagLevel-1];
	}

	if(uiTagLevel == 0)	// root tag
	{
		uiTagLevel++;
		iPosEnd[0] = (int)m_sXmlStream.size();
	}


	// search begin of empty tag inside the previously found tag
	iPos0 = (int)m_sXmlStream.find(s0.c_str(), iNextPosStart);
	if(iPos0 > 0 && iPos0 > iNextPosStart && iPos0 < iPosEnd[uiTagLevel-1])
	{
		item->bEmpty = true;
		iPosEnd[uiTagLevel]	  = iPos0;
		SetLastError(FEDM_OK);
		return item;
	}

	// search begin of tag inside the previously found tag
	iPos1 = (int)m_sXmlStream.find(s1.c_str(), iNextPosStart);

	if(iPos1 < 0 || iPos1 < iNextPosStart || iPos1 > iPosEnd[uiTagLevel-1])
	{
		iPosEnd[uiTagLevel] = iPosStart[uiTagLevel]++;
		SetLastError(FEDM_XML_ERROR_TAG_NOT_FOUND);
		DeleteTagItem(item);
		return NULL;
	}

	// empty tag with attribute?
	iPos2 = (int)m_sXmlStream.find(L"/>", iPos1+1);
	if(iPos2 > iPos1)
	{
		iPos3 = (int)m_sXmlStream.find(L"<", iPos1+1);
		if(iPos3 > iPos2)
		{
			item->bEmpty = true;
			iPosEnd[uiTagLevel]	  = iPos2;

			// get next char and check, if it is a space char or '>' or '/' char
			s3 = m_sXmlStream.at(iPos1 + s1.size());
			if(s3 == L" ")
			{	
				// get end position of start tag
				iPos3 = (int)m_sXmlStream.find_first_of(L">", iPos1);

				// extract substring with all attributes
				s3 = m_sXmlStream.substr(iPos1 + s1.size() + 1, iPos3 - iPos1 - s1.size() - 1);

				// tokenize
				bAttrib = true;	// toggles between attributes name and attributes value
				iPos4 = 0; // set start position
				while(iPos4 < (int)s3.size())
				{
					if(bAttrib)
					{
						iPos5 = (int)s3.find_first_of(L"=", iPos4);
						if(iPos5 < 0 || iPos5 >= (int)s3.size())
						{
							SetLastError(FEDM_XML_ERROR_DOC_NOT_WELL_FORMED);
							break;
						}

						// create attribute pair
						attrib = new FEDM_XML_ATTRIBUTE;
						if(attrib == NULL)
						{
							SetLastError(FEDM_XML_ERROR_DOC_NOT_WELL_FORMED);
							break;
						}

						// add attribute name
						if(s3[iPos4] == L' ')
							attrib->sAttribute = s3.substr(iPos4+1, iPos5-iPos4-1);
						else
							attrib->sAttribute = s3.substr(iPos4, iPos5-iPos4);

						bAttrib = false;	// continue with attribute value
						iPos4 = iPos5 + 1;	// positioning after '='
					}
					else
					{
						if(s3[iPos4] == L'\"')
						{
							iPos4++; // positioning after first '"'
							iPos5 = (int)s3.find_first_of(L"\"", iPos4);
							if(iPos5 <= 0 || iPos5 >= (int)s3.size())
							{
								SetLastError(FEDM_XML_ERROR_DOC_NOT_WELL_FORMED);
								break;
							}

							// add attribute value
							attrib->sValue = s3.substr(iPos4, iPos5-iPos4);;

							// add attribute pair to attribute list
							item->aAttribList.push_back(attrib);
							bAttrib = true;	// continue with attribute name
							iPos4 = iPos5 + 1;	// positioning after '"'
						}
						else
						{
							delete attrib;
							SetLastError(FEDM_XML_ERROR_DOC_NOT_WELL_FORMED);
							break;
						}
					}
				} // while(iPos4 < (int)s3.size())
			}

			SetLastError(FEDM_OK);
			return item;
		}
	}

	// search first occurrence of end of tag
	iPos2 = (int)m_sXmlStream.find(s2.c_str(), iPos1);

	if(iPos2 > 0)
	{
		// if tag has inner tags
		s3 = L"<";
		iPos3 = (int)m_sXmlStream.find(s3.c_str(), iPos1 + 1);
		if(iPos3 > 0 && iPos3 < iPos2)
		{
			// verify, if this end tag is in the same tag level as the start tag
			// if inner tags with same tag name found, recalc iPos2
			iNextPos = iPos1 + (int)s1.size() + 1; // behind start of tag
			uiLevel = uiTagLevel;
			while(iPos2 > 0 && iPos2 < iPosEnd[0])
			{
				// empty tag inside ?
				iPos3 = (int)m_sXmlStream.find(s0.c_str(), iNextPos);
				if(iPos3 > 0 && iPos3 < iPos2)//iPosEnd[uiTagLevel-1])
					iNextPos = iPos3 + (int)s0.size();

				// begin of tag inside ?
				iPos3 = (int)m_sXmlStream.find(s1.c_str(), iNextPos);
				if(iPos3 > 0 && iPos3 < iPos2)
				{
					// check, if next char is a " " or "/" or ">"
					s3 = m_sXmlStream.at(iPos3 + s1.size());
					if(s3 != L" " && s3 != L"/" && s3 != L">")
					{
						if(uiLevel == uiTagLevel)
							break;
					}

					// inner start tag with same tag name found
					uiLevel++; // increment tag level
					iNextPos = iPos3 + (int)s1.size();
				}
				else
				{
					if(uiLevel == uiTagLevel)
						break;
				}

				// skip to next end of tag
				iPos2 = (int)m_sXmlStream.find(s2.c_str(), iPos2 + s2.size());
				if(iPos2 > 0 && iPos2 < iPosEnd[uiTagLevel-1])
				{
					// inner end tag with same tag name found
					uiLevel--;
				}
				else
				{
					if(uiLevel == uiTagLevel)
						break;
				}
			
			} // while(iPos2 > 0 && iPos2 < iPosEnd[0])

		} // if(iPos3 > 0)

	} // if(iPos2 > 0)

	if(iPos2 < 0 || iPos2 < iNextPosStart || iPos2 > iPosEnd[uiTagLevel-1])
	{	// empty tag?
	
		// build search string for empty end tag..
		s2 = L"/>";
		// ..and try again
		iPos2 = (int)m_sXmlStream.find(s2.c_str(), iPos1);
		
		if(iPos2 < 0 || iPos2 < iNextPosStart || iPos2 > iPosEnd[uiTagLevel-1])
		{
			iPosEnd[uiTagLevel] = iPosStart[uiTagLevel] + 1;
			SetLastError(FEDM_XML_ERROR_TAG_NOT_FOUND);
			DeleteTagItem(item);
			return NULL;
		}

		// is there any '<' char inside?
		iPos3 = (int)m_sXmlStream.find(L"<", iPos1, iPos2-iPos1);
		if(iPos3 > 0 && (iPos3 < iPos1 || iPos3 > iPos2))
		{
			SetLastError(FEDM_XML_ERROR_DOC_NOT_WELL_FORMED);
			DeleteTagItem(item);
			return NULL;
		}

		item->bEmpty = true;

		// if no attributes, we can return
		if(iPos2 - iPos1 - s1.size() == 0)
		{
			iPosEnd[uiTagLevel]	  = iPos2;
			SetLastError(FEDM_OK);
			return item;
		}
	}
	else
	{
		item->bEmpty = false;
	}

	// get next char and check, if it is a space char or '>' or '/' char
	s3 = m_sXmlStream.at(iPos1 + s1.size());
	if(s3 == L" ")
	{	
		// get end position of start tag
		iPos3 = (int)m_sXmlStream.find_first_of(L">", iPos1);

		// extract substring with all attributes
		s3 = m_sXmlStream.substr(iPos1 + s1.size() + 1, iPos3 - iPos1 - s1.size() - 1);

		// tokenize
		bAttrib = true;	// toggles between attributes name and attributes value
		iPos4 = 0; // set start position
		while(iPos4 < (int)s3.size())
		{
			if(bAttrib)
			{
				iPos5 = (int)s3.find_first_of(L"=", iPos4);
				if(iPos5 < 0 || iPos5 >= (int)s3.size())
				{
					SetLastError(FEDM_XML_ERROR_DOC_NOT_WELL_FORMED);
					break;
				}

				// create attribute pair
				attrib = new FEDM_XML_ATTRIBUTE;
				if(attrib == NULL)
				{
					SetLastError(FEDM_ERROR_NO_MORE_MEM);
					break;
				}

				// add attribute name
				if(s3[iPos4] == L' ')
					attrib->sAttribute = s3.substr(iPos4+1, iPos5-iPos4-1);
				else
					attrib->sAttribute = s3.substr(iPos4, iPos5-iPos4);

				bAttrib = false;	// continue with attribute value
				iPos4 = iPos5 + 1;	// positioning after '='
			}
			else
			{
				if(s3[iPos4] == L'\"')
				{
					iPos4++; // positioning after first '"'
					iPos5 = (int)s3.find_first_of(L"\"", iPos4);
					if(iPos5 <= 0 || iPos5 >= (int)s3.size())
					{
						SetLastError(FEDM_XML_ERROR_DOC_NOT_WELL_FORMED);
						break;
					}

					// add attribute value
					attrib->sValue = s3.substr(iPos4, iPos5-iPos4);;

					// add attribute pair to attribute list
					item->aAttribList.push_back(attrib);
					bAttrib = true;	// continue with attribute name
					iPos4 = iPos5 + 1;	// positioning after '"'
				}
				else
				{
					delete attrib;
					SetLastError(FEDM_XML_ERROR_DOC_NOT_WELL_FORMED);
					break;
				}
			}
		} // while(iPos4 < (int)s3.size())

		if(!item->bEmpty)
		{
			// get next char and check, if it is a LF char
			s3 = m_sXmlStream.at(iPos3 + 1);
			if(s3 == L"\n")
			{
				item->bNewLine = true;
			}
			else if(s3 != L"<")
			{
				item->bNewLine = false;
				if(iPos2 - iPos3 - 1 > 0)
				{
					// get tag value
					item->sValue = m_sXmlStream.substr(iPos3 + 1, iPos2 - iPos3 - 1);

					// search for: < > & " ' 
					int iPos = (int)item->sValue.find(L"&amp;");
					while(iPos >= 0)
					{
						item->sValue.erase(iPos+1, 4);
						iPos = (int)item->sValue.find(L"&amp;", iPos+1);
					}

					iPos = (int)item->sValue.find(L"&lt;");
					while(iPos >= 0)
					{
						item->sValue.replace(iPos, 1, L"<");
						iPos = (int)item->sValue.find(L"&lt;", iPos+1);
					}

					iPos = (int)item->sValue.find(L"&gt;");
					while(iPos >= 0)
					{
						item->sValue.replace(iPos, 1, L">");
						iPos = (int)item->sValue.find(L"&gt;", iPos+1);
					}

					iPos = (int)item->sValue.find(L"&quot;");
					while(iPos >= 0)
					{
						item->sValue.replace(iPos, 1, L"\"");
						iPos = (int)item->sValue.find(L"&quot;", iPos+1);
					}

					iPos = (int)item->sValue.find(L"&apos;");
					while(iPos >= 0)
					{
						item->sValue.replace(iPos, 1, L"'");
						iPos = (int)item->sValue.find(L"&apos;", iPos+1);
					}
				}
			}
		}
	}
	else if(s3 == L">")
	{
		// get next char and check, if it is a LF char
		s3 = m_sXmlStream.at(iPos1 + s1.size() + 1);
		if(s3 == L"\n")
		{
			item->bNewLine = true;
		}
		else if(s3 != L"<")
		{
			item->bNewLine = false;
			if(iPos2 - iPos1 - s1.size() - 1 > 0)
			{
				// get tag value
				item->sValue = m_sXmlStream.substr(iPos1 + s1.size() + 1, iPos2 - iPos1 - s1.size() - 1);

				// search for: < > & " ' 
				int iPos = (int)item->sValue.find(L"&amp;");
				while(iPos >= 0)
				{
					item->sValue.erase(iPos+1, 4);
					iPos = (int)item->sValue.find(L"&amp;", iPos+1);
				}

				iPos = (int)item->sValue.find(L"&lt;");
				while(iPos >= 0)
				{
					item->sValue.replace(iPos, 1, L"<");
					iPos = (int)item->sValue.find(L"&lt;", iPos+1);
				}

				iPos = (int)item->sValue.find(L"&gt;");
				while(iPos >= 0)
				{
					item->sValue.replace(iPos, 1, L">");
					iPos = (int)item->sValue.find(L"&gt;", iPos+1);
				}

				iPos = (int)item->sValue.find(L"&quot;");
				while(iPos >= 0)
				{
					item->sValue.replace(iPos, 1, L"\"");
					iPos = (int)item->sValue.find(L"&quot;", iPos+1);
				}

				iPos = (int)item->sValue.find(L"&apos;");
				while(iPos >= 0)
				{
					item->sValue.replace(iPos, 1, L"'");
					iPos = (int)item->sValue.find(L"&apos;", iPos+1);
				}
			}
		}
	}
	else if(s3 == L"/")
	{
		// empty tag
		item->bEmpty = true;
	}
	else
	{
		SetLastError(FEDM_XML_ERROR_DOC_NOT_WELL_FORMED);
		DeleteTagItem(item);
		return NULL;
	}

	if(iPos1 > 0 && iPos2-iPos1 > 0)
	{
		if(bRoot)
		{
			iPosStart[0]= iPos1;
			iPosEnd[0]	= iPos2;
		}
		else
		{
			iPosStart[uiTagLevel] = iPos1;
			iPosEnd[uiTagLevel]	  = iPos2;
		}
	}

	SetLastError(FEDM_OK);
	return item;
}


/***************************************************************************
  Begin        	:	17.04.2003 / M. Hultsch
  Version       :	01.08.00 / 24.04.2003 / M. Hultsch

  Function		:	itentify a tag inside the xml string
					pick out all attributes and values and create a tree item
					return item only if additional attribute specs are valid

  Parameters	:	wstring sTag			- tag string to be identify (in UNICODE)
					wstring sAttribName		- tag attribute name
					bool bInside			- find the tag inside the positions iPos1 
											  and iPos2 of last function call.
											  This allows the search of child tags 
											  inside a previously found parent tag
					bool bSavePos			- save the tag positions iPos1 and iPos2
											
  Return value	:	FEDM_XML_TREEITEM*		- pointer to tree item
					NULL					- in error case
***************************************************************************/
FEDM_XML_TREEITEM* FEDM_XMLBase::FindTag(wstring sTag, 
										 wstring sAttribName, 
										 bool bInside, 
										 bool bSavePos)
{
	bool bNext = false;

	while(true)
	{
		FEDM_XML_TREEITEM* item = FindTag(sTag, bInside, bSavePos, bNext);

		if(item == NULL)
		{
			SetLastError(FEDM_XML_ERROR_NO_TAG_ATTRIBUTE);
			return NULL;
		}

		for(int i=0; i<(int)item->aAttribList.size(); ++i)
		{
			if(item->aAttribList[i]->sAttribute == sAttribName)
			{
				SetLastError(FEDM_OK);
				return item;
			}
		}

		DeleteTagItem(item);

		bNext = true;
	}

	SetLastError(FEDM_XML_ERROR_NO_TAG_ATTRIBUTE);
	return NULL;
}


/***************************************************************************
  Begin        	:	28.03.2007 / M. Hultsch
  Version       :	02.05.08 / 28.03.2007 / M. Hultsch

  Function		:	itentify a tag inside the xml string
					pick out all attributes and values and create a tree item
					return item only if additional attribute specs are valid

  Parameters	:	wstring sTag			- tag string to be identify (in UNICODE)
					wstring sAttribName		- tag attribute name
					unsigned int uiTagLevel	- level of this tag = index in save position
					bool bNext				- continue with search behind iPosEnd 
											
  Return value	:	FEDM_XML_TREEITEM*		- pointer to tree item
					NULL					- in error case
***************************************************************************/
FEDM_XML_TREEITEM* FEDM_XMLBase::FindTag(	wstring sTag, 
											wstring sAttribName, 
											unsigned int uiTagLevel,
											bool bNext)
{
	while(true)
	{
		FEDM_XML_TREEITEM* item = FindTag(sTag, uiTagLevel, bNext);

		if(item == NULL)
		{
			SetLastError(FEDM_XML_ERROR_NO_TAG_ATTRIBUTE);
			return NULL;
		}

		for(int i=0; i<(int)item->aAttribList.size(); ++i)
		{
			if(item->aAttribList[i]->sAttribute == sAttribName)
			{
				SetLastError(FEDM_OK);
				return item;
			}
		}

		DeleteTagItem(item);

		bNext = true;
	}

	SetLastError(FEDM_XML_ERROR_NO_TAG_ATTRIBUTE);
	return NULL;
}


/***************************************************************************
  Begin        	:	17.04.2003 / M. Hultsch
  Version       :	01.08.00 / 24.04.2003 / M. Hultsch

  Function		:	itentify a tag inside the xml string
					pick out all attributes and values and create a tree item
					return item only if additional attribute specs are valid

  Parameters	:	wstring sTag			- tag string to be identify (in UNICODE)
					wstring sAttribName		- tag attribute name
					wstring sAttribValue	- tag attribute value
					bool bInside=false		- find the tag inside the positions iPos1 
											  and iPos2 of last function call.
											  This allows the search of child tags 
											  inside a previously found parent tag
					bool bSavePos			- save the tag positions iPos1 and iPos2

  Return value	:	FEDM_XML_TREEITEM*		- pointer to tree item
					NULL					- in error case
***************************************************************************/
FEDM_XML_TREEITEM* FEDM_XMLBase::FindTag(wstring sTag, 
										 wstring sAttribName, 
										 wstring sAttribValue, 
										 bool bInside, 
										 bool bSavePos)
{
	bool bNext = false;

	while(true)
	{
		FEDM_XML_TREEITEM* item = FindTag(sTag, bInside, bSavePos, bNext);

		if(item == NULL)
		{
			SetLastError(FEDM_XML_ERROR_NO_TAG_ATTRIBUTE);
			return NULL;
		}

		for(int i=0; i<(int)item->aAttribList.size(); ++i)
		{
			if( item->aAttribList[i]->sAttribute == sAttribName &&
				item->aAttribList[i]->sValue == sAttribValue)
			{
				SetLastError(FEDM_OK);
				return item;
			}
		}

		DeleteTagItem(item);

		bNext = true;
	}
	
	SetLastError(FEDM_XML_ERROR_NO_TAG_ATTRIBUTE);
	return NULL;
}



//#####################################################################################
// functions for query content from xml tree
//#####################################################################################

/***************************************************************************
  Begin        	:	11.04.2003 / M. Hultsch
  Version       :	01.08.00 / 11.04.2003 / M. Hultsch

  Function		:	query the root tag

  Parameters	:	-

  Return value	:	FEDM_XML_TREEITEM*	- pointer to tree item
					NULL				- in error case
***************************************************************************/
/*FEDM_XML_TREEITEM* FEDM_XMLBase::GetRootTag()
{
	if(m_XmlTree.size() == 0)
	{
		SetLastError(FEDM_XML_ERROR_NO_OBID_TAG);
		return NULL;
	}

	return m_XmlTree[0];
}*/

/***************************************************************************
  Begin        	:	11.04.2003 / M. Hultsch
  Version       :	01.08.00 / 11.04.2003 / M. Hultsch

  Function		:	query a child tag

  Parameters	:	int iIndex	- item index in tag child list

  Return value	:	FEDM_XML_TREEITEM*	- pointer to tree item
					NULL				- in error case
***************************************************************************/
/*FEDM_XML_TREEITEM* FEDM_XMLBase::GetChildTag(int iIndex)
{
	if(m_XmlTree.size() == 0)
	{
		SetLastError(FEDM_XML_ERROR_NO_OBID_TAG);
		return NULL;
	}

	if(m_XmlTree[0]->aChildList.size() <= iIndex)
	{
		SetLastError(FEDM_XML_ERROR_NO_CHILD_TAG);
		return NULL;
	}

	return m_XmlTree[0]->aChildList[iIndex];
}*/

/***************************************************************************
  Begin        	:	11.04.2003 / M. Hultsch
  Version       :	01.08.00 / 11.04.2003 / M. Hultsch

  Function		:	query of the number of childs

  Parameters	:	FEDM_XML_TREEITEM*	- pointer to tree item

  Return value	:	int	-	number of childs (>=0) or error code (<0)
***************************************************************************/
/*int FEDM_XMLBase::GetTagChildCount(FEDM_XML_TREEITEM* item)
{
	FEDM_CHK5(item);
	
	return item->aChildList.size();
}*/

/***************************************************************************
  Begin        	:	11.04.2003 / M. Hultsch
  Version       :	01.08.00 / 04.09.2003 / M. Hultsch

  Function		:	query of tag value

  Parameters	:	FEDM_XML_TREEITEM*	- pointer to tree item
					string& sValue		- reference to string

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FEDM_XMLBase::GetTagValue(FEDM_XML_TREEITEM* item, string& sValue)
{
	FEDM_CHK5(item);

	if(item->sValue.empty())
		FEDM_RETURN(FEDM_XML_ERROR_NO_TAG_VALUE);

	char cBuffer[256];

	// convert UNICODE to char
	if(item->sValue.size() > 256)
		FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);

#if defined(__BORLANDC__) || defined(_MSC_VER)
	sprintf(cBuffer, "%S", item->sValue.c_str());
#else
	sprintf(cBuffer, "%S", /*(const char*)*/item->sValue.c_str());
#endif
	sValue = cBuffer;

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	11.04.2003 / M. Hultsch
  Version       :	01.08.00 / 11.04.2003 / M. Hultsch

  Function		:	query of the number of attributes

  Parameters	:	FEDM_XML_TREEITEM*	- pointer to tree item

  Return value	:	int	-	number of attributes (>=0) or error code (<0)
***************************************************************************/
int FEDM_XMLBase::GetTagAttribCount(FEDM_XML_TREEITEM* item)
{
	FEDM_CHK5(item);
	
	return (int)item->aAttribList.size();
}

/***************************************************************************
  Begin        	:	22.04.2003 / M. Hultsch
  Version       :	01.08.00 / 22.04.2003 / M. Hultsch

  Function		:	query at index one attribute (name and value) from tags attribute list

  Parameters	:	FEDM_XML_TREEITEM* item	- pointer to tree item
					int iIndex				- index in attribute list
					wstring& sAttrib		- attribute name
					wstring& sValue			- attribute value

  Return value	:	
***************************************************************************/
int FEDM_XMLBase::GetTagAttrib(FEDM_XML_TREEITEM* item, int iIndex, wstring& wsAttrib, wstring& wsValue)
{
	FEDM_CHK5(item);
	
	if((int)item->aAttribList.size() <= iIndex)
		FEDM_RETURN(FEDM_XML_ERROR_NO_TAG_ATTRIBUTE);

	wsAttrib = item->aAttribList[iIndex]->sAttribute;
	wsValue = item->aAttribList[iIndex]->sValue;

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	11.04.2003 / M. Hultsch
  Version       :	01.08.00 / 04.09.2003 / M. Hultsch

  Function		:	query at index one attribute (name and value) from tags attribute list

  Parameters	:	FEDM_XML_TREEITEM* item	- pointer to tree item
					int iIndex				- index in attribute list
					string sAttrib			- attribute name
					string& sValue			- attribute value

  Return value	:	
***************************************************************************/
int FEDM_XMLBase::GetTagAttrib(FEDM_XML_TREEITEM* item, int iIndex, string& sAttrib, string& sValue)
{
	FEDM_CHK5(item);
	
	if((int)item->aAttribList.size() <= iIndex)
		FEDM_RETURN(FEDM_XML_ERROR_NO_TAG_ATTRIBUTE);

	char cBuffer[256];

	// attribute name: convert UNICODE to char
	if(item->aAttribList[iIndex]->sAttribute.size() > 256)
		FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);

	// attribute value: convert UNICODE to char
	if(item->aAttribList[iIndex]->sValue.size() > 256)
		FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);

#if defined(__BORLANDC__) || defined(_MSC_VER)
	sprintf(cBuffer, "%S", item->aAttribList[iIndex]->sAttribute.c_str());
#else
	sprintf(cBuffer, "%S", /*(const char*)*/item->aAttribList[iIndex]->sAttribute.c_str());
#endif
	sAttrib = cBuffer;

#if defined(__BORLANDC__) || defined(_MSC_VER)
	sprintf(cBuffer, "%S", item->aAttribList[iIndex]->sValue.c_str());
#else
	sprintf(cBuffer, "%S", /*(const char*)*/item->aAttribList[iIndex]->sValue.c_str());
#endif
	sValue = cBuffer;

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        	:	14.04.2003 / M. Hultsch
  Version       :	01.08.00 / 04.09.2003 / M. Hultsch

  Function		:	extracts the xml file header

  Parameters	:	char* cBuffer	- pointer to string with xml header

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FEDM_XMLBase::ReadXmlHeader(char* cBuffer)
{
	bool bVersion = false;
	bool bEncoding = false;
	bool bStandalone = false;
	char* cToken;
	wchar_t wcBuffer[101];

	// extract xml version
	cToken = strtok(cBuffer, "<? =\">");
	while(cToken)
	{
		if(strcmp(cToken, "version")==0)
		{
			bVersion = true;
			cToken = strtok(NULL, "<? =\">");
			continue;
		}

		if(strcmp(cToken, "encoding")==0)
		{
			bEncoding = true;
			cToken = strtok(NULL, "<? =\">");
			continue;
		}

		if(strcmp(cToken, "standalone")==0)
		{
			bStandalone = true;
			cToken = strtok(NULL, "<? =\">");
			continue;
		}

		if(bVersion)
		{
			if(strlen(cBuffer) <= 100)
			{
#if defined(__BORLANDC__) || defined(_MSC_VER)
				swprintf(wcBuffer, L"%S", cToken);
#else
				swprintf(wcBuffer, 101, L"%s", cToken);
#endif
				m_wsXmlVer = wcBuffer;
			}

			bVersion = false;
		}

		if(bEncoding)
		{
			if(strlen(cBuffer) <= 100)
			{
#if defined(__BORLANDC__) || defined(_MSC_VER)
				swprintf(wcBuffer, L"%S", cToken);
#else
				swprintf(wcBuffer, 101, L"%s", cToken);
#endif
				m_wsEncoding = wcBuffer;
			}

			bEncoding = false;
		}

		if(bStandalone)
		{
			if(strlen(cBuffer) <= 100)
			{
#if defined(__BORLANDC__) || defined(_MSC_VER)
				swprintf(wcBuffer, L"%S", cToken);
#else
				swprintf(wcBuffer, 101, L"%s", cToken);
#endif
				m_wsStandalone = wcBuffer;
			}

			bStandalone = false;
		}

		cToken = strtok(NULL, "<? =\">");
	}

	if( m_wsXmlVer.size() == 0 ||
		m_wsEncoding.size() == 0 ||
		m_wsStandalone.size() == 0 )
	{
		FEDM_RETURN(FEDM_XML_ERROR_NO_XML_FILE);
	}

	FEDM_RETURN(FEDM_OK);
}

void FEDM_XMLBase::SetLastError(int iError)
{ 
	m_iLastError = iError;
#if defined(_MSC_VER) && defined(_FEDM_TRACE) && defined(_DEBUG)
	if(iError != 0)
		TRACE("Error in FEDM_XMLBase: %d\n", iError);
#endif
}

int FEDM_XMLBase::GetLastError()
{
	return m_iLastError;
}

/*void FEDM_XMLBase::SetComment(wstring sComment)
{
	m_sXmlComment = sComment;
}*/

#endif // #ifdef _FEDM_XML_SUPPORT
