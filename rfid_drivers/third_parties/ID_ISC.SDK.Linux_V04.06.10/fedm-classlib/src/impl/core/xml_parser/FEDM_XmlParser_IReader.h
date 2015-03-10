/*-------------------------------------------------------
|                                                       |
|	FEDM_XmlParser_IReader.h			                |
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
Function			:	abstract xml reader class for basic parser API

OBID® and OBID i-scan® are registered Trademarks of FEIG ELECTRONIC GmbH.
Linux® is a registered trademark of Linus Torvalds.
Microsoft® and Windows® are registered trademarks of Microsoft Corporation.
*/


//###########################
//      D E F I N E S
//###########################
#ifndef FEDM_XMLPARSER_IREADER_H
#define FEDM_XMLPARSER_IREADER_H


//#############################
//      I N C L U D E S
//#############################
#include "FedmIscCore.h"
#include "FEDM_XmlParser_Types.h"	// public include file of library

class FEDM_XmlParser_Base;

//#######################################################################################
//	class FEDM_XmlParser_Reader
//#######################################################################################
class FEDM_XmlParser_Reader
{
public:
	FEDM_XmlParser_Reader(void);
	virtual ~FEDM_XmlParser_Reader(void);

	// serialization helper functions for file-header
	virtual int SerializeHeaderIn(FEDM_XML_TREEITEM* parent, const char* szVendor="");

	// document management
	virtual int OpenDoc(char* sFileName)=0;			// open a xml document
	virtual int CloseDoc()=0;						// close xml document
	virtual int LoadDoc()=0;						// read the xml text from file
	virtual int LoadDoc(const char* szOemRootTag)=0;	// read the xml text from file
	virtual int LoadStream(wstring sXmlStream)=0;	// load xml string

	virtual void DeleteXmlTree()=0;
	virtual int AddTagItem(FEDM_XML_TREEITEM* pParent, FEDM_XML_TREEITEM* pChild)=0;

	virtual FEDM_XML_TREEITEM* FindTag(	wstring sTag,
										unsigned int uiTagLevel,
										bool bNext=false )=0;

	virtual FEDM_XML_TREEITEM* FindTag(	wstring sTag,
										wstring sAttribute,
										unsigned int uiTagLevel,
										bool bNext=false )=0;

	int GetTagAttrib(	FEDM_XML_TREEITEM* item,
						int iIndex, 
						string& sAttrib, 
						string& sValue );

protected:
	FEDM_XmlParser_Base* m_pXmlParser;

	friend class FEDM_XmlParser_Base;
};

#endif // #ifndef FEDM_XMLPARSER_IREADER_H

