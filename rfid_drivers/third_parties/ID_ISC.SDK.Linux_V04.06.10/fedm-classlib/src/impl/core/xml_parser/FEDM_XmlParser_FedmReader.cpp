/*-------------------------------------------------------
|                                                       |
|	FEDM_XmlParser_FedmReader.cpp						|
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
Begin        		:	06.02.2008
Version				:	03.00.08 / 30.06.2009 / M. Hultsch
Operation Systems	:	independent
Function			:	xml reader class based on FEDM parser API

OBID® and OBID i-scan® are registered Trademarks of FEIG ELECTRONIC GmbH.
Linux® is a registered trademark of Linus Torvalds.
Microsoft® and Windows® are registered trademarks of Microsoft Corporation.
*/


//#############################
//      I N C L U D E S
//#############################
#include "FEDM_XmlParser_FedmReader.h"				// public include file of library
#include "FEDM_XmlParser_Base.h"


//#######################################################################################
//	class FEDM_XmlParser_FedmReader
//#######################################################################################
FEDM_XmlParser_FedmReader::FEDM_XmlParser_FedmReader(void)
{
}


FEDM_XmlParser_FedmReader::~FEDM_XmlParser_FedmReader(void)
{
}

// open a xml document
int FEDM_XmlParser_FedmReader::OpenDoc(char* sFileName)
{
	CloseDoc();

	return FEDM_XMLBase::OpenDoc(sFileName, "r");
}

// close xml document
int FEDM_XmlParser_FedmReader::CloseDoc()
{
	return FEDM_XMLBase::CloseDoc();
}

// read the xml text from file
int FEDM_XmlParser_FedmReader::LoadDoc()
{
	// is it a xml document?
	if(FEDM_XMLBase::IsXmlDoc())
	{
		FEDM_XMLBase::CloseDoc();
		return m_iLastError;
	}

	// has the xml document an OBID tag?
	if(FEDM_XMLBase::HasOBIDTag())
	{
		FEDM_XMLBase::CloseDoc();
		return m_iLastError;
	}
	
	// read complete xml document into string
	if(FEDM_XMLBase::ReadDoc())
	{
		FEDM_XMLBase::CloseDoc();
		return m_iLastError;
	}

	return FEDM_XMLBase::CloseDoc();
}

// read the xml text from file
int FEDM_XmlParser_FedmReader::LoadDoc(const char* szOemRootTag)
{
	// is it a xml document?
	if(FEDM_XMLBase::IsXmlDoc())
	{
		FEDM_XMLBase::CloseDoc();
		return m_iLastError;
	}

	if(strlen(szOemRootTag) > 0)
	{
		// has the xml document an OBID tag?
		if(FEDM_XMLBase::HasRootTag(szOemRootTag))
		{
			FEDM_XMLBase::CloseDoc();
			return m_iLastError;
		}
	}
	else
	{
		// has the xml document an OBID tag?
		if(FEDM_XMLBase::HasOBIDTag())
		{
			FEDM_XMLBase::CloseDoc();
			return m_iLastError;
		}
	}
	
	// read complete xml document into string
	if(FEDM_XMLBase::ReadDoc())
	{
		FEDM_XMLBase::CloseDoc();
		return m_iLastError;
	}

	return FEDM_XMLBase::CloseDoc();
}

int FEDM_XmlParser_FedmReader::LoadStream(wstring sXmlStream)
{
	return FEDM_XMLBase::LoadStream(sXmlStream);
}

void FEDM_XmlParser_FedmReader::DeleteXmlTree()
{
	FEDM_XMLBase::DeleteXmlTree();
}

int FEDM_XmlParser_FedmReader::AddTagItem(FEDM_XML_TREEITEM* pParent, FEDM_XML_TREEITEM* pChild)
{
	return FEDM_XMLBase::AddTagItem(pParent, pChild);
}

FEDM_XML_TREEITEM* FEDM_XmlParser_FedmReader::FindTag(	wstring sTag,
														unsigned int uiTagLevel,
														bool bNext )
{
	FEDM_XML_TREEITEM* item = FEDM_XMLBase::FindTag(sTag, uiTagLevel, bNext);
	if(item == NULL)
		m_pXmlParser->SetLastError(m_iLastError);
	else
		m_pXmlParser->SetLastError(FEDM_OK);

	return item;
}

FEDM_XML_TREEITEM* FEDM_XmlParser_FedmReader::FindTag(	wstring sTag,
														wstring sAttribute,
														unsigned int uiTagLevel,
														bool bNext )
{
	FEDM_XML_TREEITEM* item =  FEDM_XMLBase::FindTag(sTag, sAttribute, uiTagLevel, bNext);
	if(item == NULL)
		m_pXmlParser->SetLastError(m_iLastError);
	else
		m_pXmlParser->SetLastError(FEDM_OK);

	return item;
}
