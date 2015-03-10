/*-------------------------------------------------------
|                                                       |
|	FEDM_XmlParser_Base.cpp								|
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
Version       		:	04.00.02 / 22.08.2011 / M. Hultsch
Operation Systems	:	independent
Function			:	abstract class for basic parser API

OBID� and OBID i-scan� are registered Trademarks of FEIG ELECTRONIC GmbH.
Linux� is a registered trademark of Linus Torvalds.
Microsoft� and Windows� are registered trademarks of Microsoft Corporation.
*/


//#############################
//      I N C L U D E S
//#############################
#include "FEDM_XmlParser_Base.h"				// public include file of library
#include "FEDM_XmlParser_TinyReader.h"
#include "FEDM_XmlParser_FedmReader.h"
#include "FEDM_XmlParser_FedmWriter.h"


//#######################################################################################
//	class FEDM_XmlParser_Base
//#######################################################################################
FEDM_XmlParser_Base::FEDM_XmlParser_Base(unsigned int uiReaderType, unsigned int uiWriterType)
{
	m_bIsParsed		= false;
	m_bWriteMode	= false;

	m_pXmlReader	= NULL;
	m_pXmlWriter	= NULL;

	memset(m_szAccessCode, 0, FEDM_MAX_NAME_SIZE);
	m_uiAccessCodeLen = 0;

	m_XmlHeader.Init();

	if(uiReaderType == FEDM_XML_PARSER_TYPE_TINY)
		m_pXmlReader = (FEDM_XmlParser_Reader*)new FEDM_XmlParser_TinyReader;
	else if(uiReaderType == FEDM_XML_PARSER_TYPE_FEDM)
		m_pXmlReader = (FEDM_XmlParser_Reader*)new FEDM_XmlParser_FedmReader;

	m_pXmlWriter = (FEDM_XmlParser_Writer*)new FEDM_XmlParser_FedmWriter;

	if(m_pXmlReader != NULL)
		m_pXmlReader->m_pXmlParser = this;

	if(m_pXmlWriter != NULL)
		m_pXmlWriter->m_pXmlParser = this;
}


FEDM_XmlParser_Base::~FEDM_XmlParser_Base(void)
{
	if(m_pXmlReader != NULL)
		delete m_pXmlReader;

	if(m_pXmlWriter != NULL)
		delete m_pXmlWriter;
}


//#####################################################################################
// serialization functions
//#####################################################################################

/***************************************************************************
  Begin        	:	23.01.2008
  Version       :	04.00.02 / 22.08.2011 / M. Hultsch

  Function		:	read xml file-header

  Parameters	:	FEDM_XML_TREEITEM* root	parent element

  Return value	:	FEDM_OK or error code (<0)

***************************************************************************/
int FEDM_XmlParser_Base::SerializeHeaderIn(FEDM_XML_TREEITEM* root, const char* szVendor)
{
	return m_pXmlReader->SerializeHeaderIn(root, szVendor);
}


/***************************************************************************
  Begin        	:	23.01.2008
  Version       :	04.00.02 / 22.08.2011 / M. Hultsch

  Function		:	write file-header

  Parameters	:	FEDM_XML_TREEITEM* root	parent element

  Return value	:	FEDM_OK or error code (<0)

***************************************************************************/
int FEDM_XmlParser_Base::SerializeHeaderOut(FEDM_XML_TREEITEM* root, const char* szVendor)
{
	int iBack = m_pXmlWriter->SerializeHeaderOut(root, szVendor);
	if(iBack)
		SetLastError(iBack);

	return iBack;
}

/***************************************************************************
  Begin        	:	23.01.2008
  Version       :	03.00.00 / 23.01.2008 / M. Hultsch

  Description	:	calculation of CRC16 checksum (same algorithm as for protocol frame)

  Parameters	:	unsigned char* cBuffer			- pointer to protocol buffer
					int    cBufLen			- number of chars to build the checksum

  Return Value	: unsigned int	 CRC16 checksum
***************************************************************************/
unsigned int FEDM_XmlParser_Base::CalcCRC16CheckSum(unsigned int uiCrcPreset,
													unsigned int uiData)
{
	unsigned int uiCrc = uiCrcPreset;
	unsigned char ucBuffer[4];

	ucBuffer[0] =  (uiData & 0x000000FF);
	ucBuffer[1] = ((uiData & 0x0000FF00) >>  8);
	ucBuffer[2] = ((uiData & 0x00FF0000) >> 16);
	ucBuffer[3] = ((uiData & 0xFF000000) >> 24);

	uiCrc = CalcCRC16CheckSum(uiCrc, ucBuffer, 4);

	return uiCrc;
}

/***************************************************************************
  Begin        	:	23.01.2008
  Version       :	03.00.00 / 23.01.2008 / M. Hultsch

  Description	:	calculation of CRC16 checksum (same algorithm as for protocol frame)

  Parameters	:	unsigned char* cBuffer			- pointer to protocol buffer
					int    cBufLen			- number of chars to build the checksum

  Return Value	: unsigned int	 CRC16 checksum
***************************************************************************/
unsigned int FEDM_XmlParser_Base::CalcCRC16CheckSum(unsigned int uiCrcPreset,
													unsigned char* ucBuffer, 
													unsigned int uiBufLen)
{
	unsigned int i, j;
	unsigned int uiCrc = uiCrcPreset;

	for(i=0;i<uiBufLen;i++)
	{
		uiCrc ^= ucBuffer[i];

		for(j=0; j<8; j++)
		{
			if(uiCrc & 0x0001)
			{
				uiCrc = (unsigned int)((uiCrc >> 1) ^ (unsigned int)FEDM_CRC16_POLYNOM);
			}
			else
			{
				uiCrc = (unsigned int)(uiCrc >> 1);
			}
		}
	}

	return uiCrc;
}



// open a xml document
int FEDM_XmlParser_Base::OpenDoc(char* sFileName, char* sMode)
{
	if(strcmp(sMode, "w")==0)
	{
		return m_pXmlWriter->OpenDoc(sFileName);
	}
	else if(strcmp(sMode, "r")==0)
	{
		return m_pXmlReader->OpenDoc(sFileName);
	}

	FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
}

// close xml document
int FEDM_XmlParser_Base::CloseDoc()
{
	return m_pXmlWriter->CloseDoc();
}

// read the xml text from file
int FEDM_XmlParser_Base::LoadDoc()
{
	return m_pXmlReader->LoadDoc();
}

// read the xml text from file
int FEDM_XmlParser_Base::LoadDoc(const char* szOemRootTag)
{
	return m_pXmlReader->LoadDoc(szOemRootTag);
}

// read the xml text from stream
int FEDM_XmlParser_Base::LoadStream(wstring sXmlStream)
{
	return m_pXmlReader->LoadStream(sXmlStream);
}

// write the xml text into file
int FEDM_XmlParser_Base::WriteDoc()
{
	return m_pXmlWriter->WriteDoc();
}
wstring FEDM_XmlParser_Base::WriteStream()
{
	return m_pXmlWriter->WriteStream();
}

void FEDM_XmlParser_Base::DeleteXmlTree()
{
	if(m_bWriteMode)
		m_pXmlWriter->DeleteXmlTree();
	else
		m_pXmlReader->DeleteXmlTree();
}

int FEDM_XmlParser_Base::AddTagItem(FEDM_XML_TREEITEM* pParent, FEDM_XML_TREEITEM* pChild)
{
	if(m_bWriteMode)
		return m_pXmlWriter->AddTagItem(pParent, pChild);
	else
		return m_pXmlReader->AddTagItem(pParent, pChild);
}


// ##################################################################################
// interface for reader class
// ##################################################################################

FEDM_XML_TREEITEM* FEDM_XmlParser_Base::FindTag(	wstring sTag,
													unsigned int uiTagLevel,
													bool bNext)
{
	return m_pXmlReader->FindTag(sTag, uiTagLevel, bNext);
}

FEDM_XML_TREEITEM* FEDM_XmlParser_Base::FindTag(	wstring sTag,
													wstring sAttribute,
													unsigned int uiTagLevel,
													bool bNext)
{
	return m_pXmlReader->FindTag(sTag, sAttribute, uiTagLevel, bNext);
}

int	FEDM_XmlParser_Base::GetTagAttrib(	FEDM_XML_TREEITEM* item,
										int iIndex, 
										string& sAttrib, 
										string& sValue )
{
	return m_pXmlReader->GetTagAttrib(item, iIndex, sAttrib, sValue);
}


// ##################################################################################
// interface for writer class
// ##################################################################################

FEDM_XML_TREEITEM* FEDM_XmlParser_Base::BuildTag(wstring sTag, bool bNewLine, bool bEmpty)
{
	return m_pXmlWriter->BuildTag(sTag, bNewLine, bEmpty);
}

int FEDM_XmlParser_Base::AddTagValue(FEDM_XML_TREEITEM* item, wstring sValue)
{
	return m_pXmlWriter->AddTagValue(item, sValue);
}

int FEDM_XmlParser_Base::AddTagAttrib(FEDM_XML_TREEITEM* item, wstring sAttrib, wstring sValue)
{
	return m_pXmlWriter->AddTagAttrib(item, sAttrib, sValue);
}

void FEDM_XmlParser_Base::SetComment(wstring sComment)
{
	m_pXmlWriter->SetComment(sComment);
}
