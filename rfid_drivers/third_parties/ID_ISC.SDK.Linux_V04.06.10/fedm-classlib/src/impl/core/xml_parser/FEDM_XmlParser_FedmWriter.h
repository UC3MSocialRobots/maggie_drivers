/*-------------------------------------------------------
|                                                       |
|	FEDM_XmlParser_FedmWriter.h			                |
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
Function			:	xml writer class based on FEDM parser API

OBID® and OBID i-scan® are registered Trademarks of FEIG ELECTRONIC GmbH.
Linux® is a registered trademark of Linus Torvalds.
Microsoft® and Windows® are registered trademarks of Microsoft Corporation.
*/


//###########################
//      D E F I N E S
//###########################
#ifndef FEDM_XMLPARSER_FEDM_WRITER_H
#define FEDM_XMLPARSER_FEDM_WRITER_H


//#############################
//      I N C L U D E S
//#############################
#include "FEDM_XmlParser_IWriter.h"
#include "FedmIscCore.h"



//#######################################################################################
//	class FEDM_XmlParser_FedmWriter
//#######################################################################################
class FEDM_XmlParser_FedmWriter : public FEDM_XmlParser_Writer, public FEDM_XMLBase
{
public:
	FEDM_XmlParser_FedmWriter(void);
	virtual ~FEDM_XmlParser_FedmWriter(void);

	virtual void SetComment(wstring sComment);


	// document management
	virtual int OpenDoc(char* sFileName);	// open a xml document
	virtual int CloseDoc();					// close xml document
	virtual int WriteDoc();					// write the xml text into file
	virtual wstring WriteStream();			// build and return xml string

	virtual void DeleteXmlTree();

	virtual FEDM_XML_TREEITEM* BuildTag(wstring sTag, bool bNewLine=true, bool bEmpty=false);
	virtual int AddTagValue(FEDM_XML_TREEITEM* item, wstring sValue);
	virtual int AddTagAttrib(FEDM_XML_TREEITEM* item, wstring sAttrib, wstring sValue);
	virtual int AddTagItem(FEDM_XML_TREEITEM* pParent, FEDM_XML_TREEITEM* pChild);
};

#endif // #ifndef FEDM_XMLPARSER_FEDM_WRITER_H

