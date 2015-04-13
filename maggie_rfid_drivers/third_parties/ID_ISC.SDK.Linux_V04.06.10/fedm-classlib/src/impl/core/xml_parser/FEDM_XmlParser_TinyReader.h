/*-------------------------------------------------------
|                                                       |
|	FEDM_XmlParser_TinyReader.h			                |
|                                                       |
---------------------------------------------------------

Copyright © 2008-2011	FEIG ELECTRONIC GmbH, All Rights Reserved.
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
Function			:	xml reader class based on tinyxml parser API

OBID® and OBID i-scan® are registered Trademarks of FEIG ELECTRONIC GmbH.
Linux® is a registered trademark of Linus Torvalds.
Microsoft® and Windows® are registered trademarks of Microsoft Corporation.
*/


//###########################
//      D E F I N E S
//###########################
#ifndef FEDM_XMLPARSER_TINY_READER_H
#define FEDM_XMLPARSER_TINY_READER_H



#ifdef _MSC_VER
	// The (microsoft) debugger can't handle symbols more than 255 characters long.
	// STL often creates symbols longer than that.
	// When symbols are longer than 255 characters, the warning is disabled.
	#pragma warning(disable:4786)	// for MFC
#endif


//#############################
//      I N C L U D E S
//#############################
#include "FEDM_XmlParser_IReader.h"
#include "tinyxml.h"



//#######################################################################################
//	class FEDM_XmlParser_TinyReader
//#######################################################################################
class FEDM_XmlParser_TinyReader : public FEDM_XmlParser_Reader
{
public:
	FEDM_XmlParser_TinyReader(void);
	virtual ~FEDM_XmlParser_TinyReader(void);


	// document management
	virtual int OpenDoc(char* sFileName);		// open a xml document
	virtual int CloseDoc();						// close xml document
	virtual int LoadDoc();						// read the xml text from file
	virtual int LoadDoc(const char* szOemRootTag);	// read the xml text from file
	virtual int LoadStream(wstring sXmlStream);	// load xml string

	virtual void DeleteXmlTree();
	virtual int AddTagItem(FEDM_XML_TREEITEM* pParent, FEDM_XML_TREEITEM* pChild);

	virtual FEDM_XML_TREEITEM* FindTag(	wstring sTag,
								unsigned int uiTagLevel,
								bool bNext=false);

	virtual FEDM_XML_TREEITEM* FindTag(	wstring sTag,
								wstring sAttribute,
								unsigned int uiTagLevel,
								bool bNext=false);


protected:
	unsigned int	m_uiLastTagLevel;	// tag level of last FindTag()

	// tinyXML members
	TiXmlDocument*	m_pDoc;				// xml document pointer from OpenDoc()
	TiXmlNode*		m_pLastParent;		// parent node pointer set by last FindTag()
	TiXmlNode*		m_pLastChild[20];	// child node pointer for each tag level set by last FindTag()
};


#endif // #ifndef FEDM_XMLPARSER_TINY_READER_H

