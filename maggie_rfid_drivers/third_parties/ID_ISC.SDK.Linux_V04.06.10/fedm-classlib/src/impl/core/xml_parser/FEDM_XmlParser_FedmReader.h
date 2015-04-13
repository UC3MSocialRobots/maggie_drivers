/*-------------------------------------------------------
|                                                       |
|	FEDM_XmlParser_FedmReader.h			                |
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
Begin        		:	06.02.2008
Version       		:	04.00.02 / 22.08.2011 / M. Hultsch
Operation Systems	:	independent
Function			:	xml reader class based on FEDM parser API

OBID® and OBID i-scan® are registered Trademarks of FEIG ELECTRONIC GmbH.
Linux® is a registered trademark of Linus Torvalds.
Microsoft® and Windows® are registered trademarks of Microsoft Corporation.
*/


//###########################
//      D E F I N E S
//###########################
#ifndef FEDM_XMLPARSER_FEDM_READER_H
#define FEDM_XMLPARSER_FEDM_READER_H


//#############################
//      I N C L U D E S
//#############################
#include "FEDM_XmlParser_IReader.h"
#include "FedmIscCore.h"



//#######################################################################################
//	class FEDM_XmlParser_FedmReader
//#######################################################################################
class FEDM_XmlParser_FedmReader : public FEDM_XmlParser_Reader, public FEDM_XMLBase
{
public:
	FEDM_XmlParser_FedmReader(void);
	virtual ~FEDM_XmlParser_FedmReader(void);


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
};

#endif // #ifndef FEDM_XMLPARSER_FEDM_READER_H

