/*-------------------------------------------------------
|                                                       |
|                     FEDM_XMLBase.h	                |
|                                                       |
---------------------------------------------------------

Copyright © 2000-2011	FEIG ELECTRONIC GmbH, All Rights Reserved.
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



#if !defined(_FEDM_XMLBASE_H_INCLUDED_)
#define _FEDM_XMLBASE_H_INCLUDED_



#ifdef _FEDM_XML_SUPPORT


#include "FEDM.h"
#include "FEDM_Xml.h"



class _FEDM_ISC_CORE_EXT_CLASS FEDM_XMLBase
{
public:
	FEDM_XMLBase();
	~FEDM_XMLBase();
	void DeleteXmlTree();

	// query functions
	int QueryDocType(char* sFileName, char* sDocType, char* sDocVersion);
	int QueryDocType(char* sFileName, const char* sRootTag, char* sDocType, char* sDocVersion);

	// document management
	int OpenDoc(char* sFileName, char* sMode, bool bAddCopyright=false);	// open a xml document
	int CloseDoc();						// close xml document
	int IsXmlDoc();						// checks the xml header
	int HasOBIDTag();					// checks, if tag <OBID> is found
	int HasRootTag(const char* szOemRootTag);	// checks, if tag <szOemRootTag> is found
	int ReadDoc();						// read the xml text from file and put it into m_sXmlStream
	int WriteDoc();						// write the xml text from m_sXmlStream into file
	wstring WriteStream();				// build and return m_sXmlStream
	int LoadStream(wstring sXmlStream);	// load xml string

	// tag functions for creating and building the xml tree
	FEDM_XML_TREEITEM* BuildTag(wstring sTag, bool bNewLine=true, bool bEmpty=false);
	int AddTagValue(FEDM_XML_TREEITEM* item, wstring sValue);
	int AddTagAttrib(FEDM_XML_TREEITEM* item, wstring sAttrib, wstring sValue);
	int AddTagItem(FEDM_XML_TREEITEM* pParent, FEDM_XML_TREEITEM* pChild);

	// tag functions for searching in raw xml text string
	FEDM_XML_TREEITEM* FindTag( wstring sTag,
								bool bInside=false,
								bool bSavePos=false,
								bool bNext=false);
	FEDM_XML_TREEITEM* FindTag(	wstring sTag,
								wstring sAttribute,
								bool bInside=false,
								bool bSavePos=false);
	FEDM_XML_TREEITEM* FindTag( wstring sTag,
								wstring sAttribName, 
								wstring sAttribValue, 
								bool bInside=false,
								bool bSavePos=false);
	
	FEDM_XML_TREEITEM* FindTag(	wstring sTag,
								unsigned int uiTagLevel,
								bool bNext=false);
	FEDM_XML_TREEITEM* FindTag(	wstring sTag,
								wstring sAttribute,
								unsigned int uiTagLevel,
								bool bNext=false);
	
	// functions for query content from xml tree
//	FEDM_XML_TREEITEM* GetRootTag();
//	FEDM_XML_TREEITEM* GetChildTag(int iIndex);
//	int GetTagChildCount(FEDM_XML_TREEITEM* item);
	int GetTagValue(FEDM_XML_TREEITEM* item, string& sValue);
	int GetTagAttribCount(FEDM_XML_TREEITEM* item);
	int GetTagAttrib(FEDM_XML_TREEITEM* item, int iIndex, string& sAttrib, string& sValue);
	int GetTagAttrib(FEDM_XML_TREEITEM* item, int iIndex, wstring& wsAttrib, wstring& wsValue);

	// query functions
	int GetLastError();

//	void SetComment(wstring sComment);

protected:
	void SetLastError(int iError);

	// functions for deleting xml tree
	int DeleteTagItem(FEDM_XML_TREEITEM* item);	// for recursive use

	// function for writing one tag into the xml string
	int WriteTagStream(FEDM_XML_TREEITEM* item);

	// function for reading xml header
	int ReadXmlHeader(char* cBuffer);

	void AddComment();

	int		m_iLastError;			// last error code

	wstring m_sXmlStream;			// reader firmware data (xml text in UNICODE)
	wstring m_sXmlComment;			// comment in XML file
	FILE* m_file;					// file handle
	FEDM_XML_TREE_LIST m_XmlTree;	// tree with xml items

	// file header
	wstring m_wsXmlVer;
	wstring m_wsEncoding;
	wstring m_wsStandalone;
	wstring m_wsDocType;
	wstring m_wsDocVer;
	wstring m_wsDate;
	wstring m_wsTime;
};

#endif // #ifdef _FEDM_XML_SUPPORT

#endif // !defined(_FEDM_XMLBASE_H_INCLUDED_)
