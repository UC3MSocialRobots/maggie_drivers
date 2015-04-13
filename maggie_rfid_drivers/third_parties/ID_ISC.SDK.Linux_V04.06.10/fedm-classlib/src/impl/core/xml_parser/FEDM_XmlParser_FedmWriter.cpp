/*-------------------------------------------------------
|                                                       |
|	FEDM_XmlParser_FedmWriter.cpp						|
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


//#############################
//      I N C L U D E S
//#############################
#include "FEDM_XmlParser_FedmWriter.h"				// public include file of library
#include "FEDM_XmlParser_Base.h"


//#######################################################################################
//	class FEDM_XmlParser_FedmWriter
//#######################################################################################
FEDM_XmlParser_FedmWriter::FEDM_XmlParser_FedmWriter(void)
{
}


FEDM_XmlParser_FedmWriter::~FEDM_XmlParser_FedmWriter(void)
{
}


void FEDM_XmlParser_FedmWriter::SetComment(wstring sComment)
{
	m_sXmlComment = sComment;
}

// open a xml document
int FEDM_XmlParser_FedmWriter::OpenDoc(char* sFileName)
{
	FEDM_XMLBase::OpenDoc(sFileName, "w", true);
	m_pXmlParser->SetLastError(m_iLastError);

	return m_iLastError;
}

// close xml document
int FEDM_XmlParser_FedmWriter::CloseDoc()
{
	FEDM_XMLBase::CloseDoc();
	m_pXmlParser->SetLastError(m_iLastError);

	return m_iLastError;
}

// write the xml text into file
int FEDM_XmlParser_FedmWriter::WriteDoc()
{
	FEDM_XMLBase::WriteDoc();
	m_pXmlParser->SetLastError(m_iLastError);

	return m_iLastError;
}

// build and return xml string
wstring FEDM_XmlParser_FedmWriter::WriteStream()
{
	m_pXmlParser->SetLastError(FEDM_OK);
	
	return FEDM_XMLBase::WriteStream();
}

void FEDM_XmlParser_FedmWriter::DeleteXmlTree()
{
	FEDM_XMLBase::DeleteXmlTree();
}

FEDM_XML_TREEITEM* FEDM_XmlParser_FedmWriter::BuildTag(wstring sTag, bool bNewLine, bool bEmpty)
{
	FEDM_XML_TREEITEM* item = FEDM_XMLBase::BuildTag(sTag, bNewLine, bEmpty);
	if(item == NULL)
		m_pXmlParser->SetLastError(m_iLastError);
	else
		m_pXmlParser->SetLastError(FEDM_OK);

	return item;
}

int FEDM_XmlParser_FedmWriter::AddTagValue(FEDM_XML_TREEITEM* item, wstring sValue)
{
	return FEDM_XMLBase::AddTagValue(item, sValue);
}

int FEDM_XmlParser_FedmWriter::AddTagAttrib(FEDM_XML_TREEITEM* item, wstring sAttrib, wstring sValue)
{
	return FEDM_XMLBase::AddTagAttrib(item, sAttrib, sValue);
}

int FEDM_XmlParser_FedmWriter::AddTagItem(FEDM_XML_TREEITEM* pParent, FEDM_XML_TREEITEM* pChild)
{
	return FEDM_XMLBase::AddTagItem(pParent, (FEDM_XML_TREEITEM*)pChild);
}
