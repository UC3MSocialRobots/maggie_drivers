/*-------------------------------------------------------
|                                                       |
|	FEDM_XmlParser_IWriter.h			                |
|                                                       |
---------------------------------------------------------

Copyright © 2008		FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	23.01.2008
Version       		:	03.00.00 / 23.01.2008 / M. Hultsch
Operation Systems	:	independent
Function			:	abstract xml writer class for basic parser API

OBID® and OBID i-scan® are registered Trademarks of FEIG ELECTRONIC GmbH.
Linux® is a registered trademark of Linus Torvalds.
Microsoft® and Windows® are registered trademarks of Microsoft Corporation.
*/


//###########################
//      D E F I N E S
//###########################
#ifndef FEDM_XMLPARSER_IWRITER_H
#define FEDM_XMLPARSER_IWRITER_H


//#############################
//      I N C L U D E S
//#############################
#include "FedmIscCore.h"
#include "FEDM_XmlParser_Types.h"	// public include file of library

class FEDM_XmlParser_Base;


//#######################################################################################
//	class FEDM_XmlParser_Writer
//#######################################################################################
class FEDM_XmlParser_Writer
{
public:
	FEDM_XmlParser_Writer(void);
	virtual ~FEDM_XmlParser_Writer(void);

	virtual void SetComment(wstring sComment)=0;

	// serialization helper functions for file-header
	virtual int SerializeHeaderOut(FEDM_XML_TREEITEM* parent, const char* szVendor="");


	// document management
	virtual int OpenDoc(char* sFileName)=0;	// open a xml document
	virtual int CloseDoc()=0;				// close xml document
	virtual int WriteDoc()=0;				// write the xml text into file
	virtual wstring WriteStream()=0;		// build and return xml string

	virtual void DeleteXmlTree()=0;
	
	virtual FEDM_XML_TREEITEM* BuildTag(wstring sTag, bool bNewLine=true, bool bEmpty=false)=0;
	virtual int AddTagValue(FEDM_XML_TREEITEM* item, wstring sValue)=0;
	virtual int AddTagAttrib(FEDM_XML_TREEITEM* item, wstring sAttrib, wstring sValue)=0;
	virtual int AddTagItem(FEDM_XML_TREEITEM* pParent, FEDM_XML_TREEITEM* pChild)=0;


	FEDM_XmlParser_Base* m_pXmlParser;

	friend class FEDM_XmlParser_Base;
};

#endif // #ifndef FEDM_XMLPARSER_IWRITER_H

