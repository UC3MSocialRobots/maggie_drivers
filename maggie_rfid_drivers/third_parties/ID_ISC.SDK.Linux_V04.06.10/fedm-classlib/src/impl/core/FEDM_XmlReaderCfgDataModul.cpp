/*-------------------------------------------------------
|                                                       |
|              FEDM_XmlReaderCfgDataModul.cpp           |
|                                                       |
---------------------------------------------------------

Copyright  2000-2012	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	23.04.2003
Version       		:	04.00.09 / 01.02.2012 / M. Hultsch
						- AddReaderInfo: mode 0x04 >> modification for CPR-Reader 

						03.03.00 / 01.02.2011 / M. Hultsch

Operation Systems	:	independent

Function			:	main class for XML document handler
						
Note				:	this XML document handler is designed only for use
						inside the class library FEDM !!


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#ifdef _FEDM_XML_SUPPORT


#include "FEDM_XmlReaderCfgDataModul.h"
#include "FEDM_DataBase.h"
#include "i_scan/FEDM_ISC.h"
#include <time.h>

#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif




//#####################################################################################
// Construction/Destruction
//#####################################################################################

/***************************************************************************
  Begin        	:	23.04.2003 / M. Hultsch
  Version       :	03.00.00 / 16.04.2008 / M. Hultsch

  Function		:	constructor

  Parameters	:	-

  Return value	:	-

***************************************************************************/
FEDM_XMLReaderCfgDataModul::FEDM_XMLReaderCfgDataModul()
{
	m_pReaderInfo		= NULL;

	m_wsDocType			= L"";
	m_wsDocVer			= L"";
	m_wsComment			= L"";
	m_wsPrgName			= L"";
	m_wsPrgVer			= L"";
	m_wsFedmVer			= L"";
	m_wsHost			= L"";	// for TCP/IP
	m_wsPort			= L"";	// for TCP/IP
	m_wsDate			= L"";
	m_wsTime			= L"";
	m_wsCommMode		= L"";	// last communication mode
}

/***************************************************************************
  Begin        	:	23.04.2003 / M. Hultsch
  Version       :	01.08.00 / 23.04.2003 / M. Hultsch

  Function		:	destructor

  Parameters	:	-

  Return value	:	-

***************************************************************************/
FEDM_XMLReaderCfgDataModul::~FEDM_XMLReaderCfgDataModul()
{
}


//#####################################################################################
// serialization functions
//#####################################################################################

/***************************************************************************
  Begin        	:	23.04.2003 / M. Hultsch
  Version       :	03.00.00 / 16.04.2008 / M. Hultsch

  Function		:	read xml document and save content in reader class

  Parameters	:	FEDM_DataBase* pReader
					char* sFileName	-	file name

  Return value	:	FEDM_OK or error code (<0)

***************************************************************************/
int FEDM_XMLReaderCfgDataModul::SerializeIn(FEDM_DataBase* pReader, char* sFileName)
{
	FEDM_CHK5(pReader);
	FEDM_CHK5(sFileName);

	char	cValue[32];
	char*	cFamCode = NULL;
	wchar_t wcFamCode[5];
	unsigned int uiTmp;
	int		iBack = 0;
	wstring	wsFamCode;
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
	if(child->sValue != FEDM_XML_STRING_DOC_TYPE_1_UC)
	{
		delete child;
		DeleteXmlTree(); // clean up
		FEDM_RETURN(FEDM_XML_ERROR_DOC_FILE_TYPE);
	}
	m_wsDocType = child->sValue;
	AddTagItem(parent, child);

	// header tag document version
	child = FindTag(FEDM_XML_TAG_DOC_VERSION_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	if(child->sValue != FEDM_XML_STRING_DOC_VERSION_UC)
	{
		delete child;
		DeleteXmlTree(); // clean up
		FEDM_RETURN(FEDM_XML_ERROR_DOC_FILE_VERSION);
	}
	m_wsDocVer = child->sValue;
	AddTagItem(parent, child);

	// header tag reader family name
	child = FindTag(FEDM_XML_TAG_READER_FAM_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}

	// check the family code
	if(child->sValue.size() > 4)
	{
		delete child;
		DeleteXmlTree(); // clean up
		FEDM_RETURN(FEDM_XML_ERROR_DOC_FILE_FAMILY);
	}
	cFamCode = pReader->GetFamilyCode();
#if defined(__BORLANDC__) || defined(_MSC_VER)
	swprintf(wcFamCode, L"%S", cFamCode);
#else
	swprintf(wcFamCode, 5, L"%s", cFamCode);
#endif
	wsFamCode = wcFamCode;
	if(child->sValue != wsFamCode)
	{
		delete child;
		DeleteXmlTree(); // clean up
		FEDM_RETURN(FEDM_XML_ERROR_DOC_FILE_FAMILY);
	}
	AddTagItem(parent, child);

	// header tag reader type
	child = FindTag(FEDM_XML_TAG_READER_TYPE_UC);
	if(child != NULL)
	{
		AddTagItem(parent, child);
#if defined(__BORLANDC__) || defined(_MSC_VER)
		swscanf(child->sValue.c_str(), L"%S", cValue);
#elif defined(_FEDM_SUPPORT_UCLINUX)
		FEDM_SWSCANF(child->sValue.c_str(), cValue);
#else
		swscanf(child->sValue.c_str(), L"%s", cValue);
#endif
		sscanf(cValue, "%u", &uiTmp);
		pReader->SetReaderType(uiTmp);
	}
	else
	{
		pReader->SetReaderType(0);
	}

	// header tag  bus address
	child = FindTag(FEDM_XML_TAG_BUSADR_UC);
	if(child != NULL)
	{
		AddTagItem(parent, child);
#if defined(__BORLANDC__) || defined(_MSC_VER)
		swscanf(child->sValue.c_str(), L"%S", cValue);
#elif defined(_FEDM_SUPPORT_UCLINUX)
		FEDM_SWSCANF(child->sValue.c_str(), cValue);
#else
		swscanf(child->sValue.c_str(), L"%s", cValue);
#endif
		sscanf(cValue, "%d", &uiTmp);
		pReader->SetBusAddress((unsigned char)uiTmp);
	}

	// header tag host-address
	child = FindTag(FEDM_XML_TAG_HOSTADR_UC);
	if(child != NULL)
	{
		AddTagItem(parent, child);
		m_wsHost = child->sValue;
	}

	// header tag port-number
	child = FindTag(FEDM_XML_TAG_PORTNR_UC);
	if(child != NULL)
	{
		AddTagItem(parent, child);
		m_wsPort = child->sValue;
	}

	// header tag communication mode
	child = FindTag(FEDM_XML_TAG_COMM_MODE_UC);
	if(child != NULL)
	{
		AddTagItem(parent, child);
		m_wsCommMode = child->sValue;
	}

	// header tag protocol-frame-support
	child = FindTag(FEDM_XML_TAG_FRAME_SUPPORT);
	if(child != NULL)
	{
		AddTagItem(parent, child);
		if(wcscmp(child->sValue.c_str(), L"advanced") == 0)
		{
			pReader->SetProtocolFrameSupport(FEDM_PRT_FRAME_ADVANCED);
		}
		else
		{
			pReader->SetProtocolFrameSupport(FEDM_PRT_FRAME_STANDARD);
		}
	}

	// header tag comment
	child = FindTag(FEDM_XML_TAG_COMMENT_UC);
	if(child != NULL)
	{
		AddTagItem(parent, child);
		m_wsComment = child->sValue;
	}

	// optional: tag <reader>
//	if(m_pReaderInfo)
//		GetReaderInfo(pReader, root);

	// ... transfer content of tag items to byte arrays
	iBack = pReader->SerializeIn(this, root);
	if(iBack)
	{
		DeleteXmlTree(); // clean up
		return iBack;
	}

	// clean up
	DeleteXmlTree();

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        	:	23.04.2003 / M. Hultsch
  Version       :	03.02.05 / 03.09.2010 / M. Hultsch

  Function		:	write content of reader class into xml document

  Parameters	:	FEDM_DataBase* pReader
					char* sFileName	-	file name

  Return value	:	FEDM_OK or error code (<0)

***************************************************************************/
int FEDM_XMLReaderCfgDataModul::SerializeOut(FEDM_DataBase* pReader, char* sFileName)
{
	FEDM_CHK5(pReader);
	FEDM_CHK5(sFileName);

	int		iBack = 0;
	char	cValue[32];
	wchar_t wcTmp[128];
#if !defined(_FEDM_SUPPORT_UCLINUX) && !defined(_WIN32_WCE)
	time_t	ltime;
	struct	tm *tmtime;
#endif
	FEDM_XML_TREEITEM* root = NULL;
	FEDM_XML_TREEITEM* parent = NULL;
	FEDM_XML_TREEITEM* child = NULL;

	int iPortHnd = pReader->GetPortHnd();


	// root tag
	root = BuildTag(FEDM_XML_TAG_OBID_UC);
	if(root == NULL)
	{
		return m_iLastError;
	}
	AddTagItem(NULL, root);

	// header tag
	child = BuildTag(FEDM_XML_TAG_FILE_HDR_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	AddTagItem(root, child);
	parent = child;

	// header tag document type
	child = BuildTag(FEDM_XML_TAG_DOC_TYPE_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	AddTagValue(child, FEDM_XML_STRING_DOC_TYPE_1_UC);
	AddTagItem(parent, child);

	// header tag document version
	child = BuildTag(FEDM_XML_TAG_DOC_VERSION_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	AddTagValue(child, FEDM_XML_STRING_DOC_VERSION_UC);
	AddTagItem(parent, child);

	// header tag reader family name
	child = BuildTag(FEDM_XML_TAG_READER_FAM_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
#if defined(__BORLANDC__) || defined(_MSC_VER)
	swprintf(wcTmp, L"%S", pReader->GetFamilyCode());
#else
	swprintf(wcTmp, 128, L"%s", pReader->GetFamilyCode());
#endif
	AddTagValue(child, wcTmp);
	AddTagItem(parent, child);

	// header tag reader name
	child = BuildTag(FEDM_XML_TAG_READER_NAME_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
#if defined(__BORLANDC__) || defined(_MSC_VER)
	swprintf(wcTmp, L"%S", pReader->GetReaderName());
#else
	swprintf(wcTmp, 128, L"%s", pReader->GetReaderName());
#endif
	AddTagValue(child, wcTmp);
	AddTagItem(parent, child);

	// header tag reader type
	child = BuildTag(FEDM_XML_TAG_READER_TYPE_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
#if defined(__BORLANDC__) || defined(_MSC_VER)
	swprintf(wcTmp, L"%u", pReader->GetReaderType());
#else
	swprintf(wcTmp, 128, L"%u", pReader->GetReaderType());
#endif
	AddTagValue(child, wcTmp);
	AddTagItem(parent, child);


	if(FEDM_IS_USBPORT(iPortHnd))
	{
#ifdef _FEDM_USB_SUPPORT
		// header tag usb device-id
		child = BuildTag(FEDM_XML_TAG_DEVICE_ID_UC, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return m_iLastError;
		}
#if !defined(_FEDM_SUPPORT_SLINK)
		LPFN_FEUSB_GET_DEVICE_PARA lpfn = (LPFN_FEUSB_GET_DEVICE_PARA)pReader->GetFeUsbFunction(FEUSB_GET_DEVICE_PARA);
		if(lpfn == NULL)
			return FEDM_ERROR_NULL_POINTER;

		if(0 == lpfn(iPortHnd, "Device-ID", cValue))
#else
		if(0 == FEUSB_GetDevicePara(iPortHnd, "Device-ID", cValue))
#endif
		{
#if defined(__BORLANDC__) || defined(_MSC_VER)
			swprintf(wcTmp, L"%S", cValue);
#else
			swprintf(wcTmp, 128, L"%s", cValue);
#endif
			AddTagValue(child, wcTmp);
			AddTagItem(parent, child);

			// header tag communication mode
			child = BuildTag(FEDM_XML_TAG_COMM_MODE_UC, false);
			if(child == NULL)
			{
				DeleteXmlTree(); // clean up
				return m_iLastError;
			}
			AddTagValue(child, FEDM_XML_STRING_COMM_MODE_USB_UC);
			AddTagItem(parent, child);
		}
		else
		{
			delete child;
		}
#endif
	}
	else if(FEDM_IS_TCPPORT(iPortHnd))
	{
#ifdef _FEDM_TCP_SUPPORT
		LPFN_FETCP_GET_SOCKET_PARA lpfn = (LPFN_FETCP_GET_SOCKET_PARA)pReader->GetFeTcpFunction(FETCP_GET_SOCKET_PARA);
		if(lpfn != NULL)
		{
			// header tag host-address
			child = BuildTag(FEDM_XML_TAG_HOSTADR_UC, false);
			if(child == NULL)
			{
				DeleteXmlTree(); // clean up
				return m_iLastError;
			}
			if(0 == lpfn(iPortHnd, "HostAdr", cValue))
			{
#if defined(__BORLANDC__) || defined(_MSC_VER)
				swprintf(wcTmp, L"%S", cValue);
#else
				swprintf(wcTmp, 128, L"%s", cValue);
#endif
				AddTagValue(child, wcTmp);
				AddTagItem(parent, child);
			}
			else
			{
				delete child;
			}

			// header tag port-number
			child = BuildTag(FEDM_XML_TAG_PORTNR_UC, false);
			if(child == NULL)
			{
				DeleteXmlTree(); // clean up
				return m_iLastError;
			}

			if(0 == lpfn(iPortHnd, "PortNr", cValue))
			{
#if defined(__BORLANDC__) || defined(_MSC_VER)
				swprintf(wcTmp, L"%S", cValue);
#else
				swprintf(wcTmp, 128, L"%s", cValue);
#endif
				AddTagValue(child, wcTmp);
				AddTagItem(parent, child);
			}
			else
			{
				delete child;
			}

			// header tag communication mode
			child = BuildTag(FEDM_XML_TAG_COMM_MODE_UC, false);
			if(child == NULL)
			{
				DeleteXmlTree(); // clean up
				return m_iLastError;
			}
			AddTagValue(child, FEDM_XML_STRING_COMM_MODE_TCP_UC);
			AddTagItem(parent, child);
		}
#endif
	}
	else
	{
#ifdef _FEDM_COM_SUPPORT
#if !defined(_FEDM_SUPPORT_SLINK)
		LPFN_FECOM_GET_PORT_PARA lpfn = (LPFN_FECOM_GET_PORT_PARA)pReader->GetFeComFunction(FECOM_GET_PORT_PARA);
		if(lpfn == NULL)
			return FEDM_ERROR_NULL_POINTER;
#endif
		// header tag bus address
		child = BuildTag(FEDM_XML_TAG_BUSADR_UC, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return m_iLastError;
		}
#if defined(__BORLANDC__) || defined(_MSC_VER)
		swprintf(wcTmp, L"%d", pReader->GetBusAddress());
#else
		swprintf(wcTmp, 128, L"%d", pReader->GetBusAddress());
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(parent, child);

		// header tag serial com port
		child = BuildTag(FEDM_XML_TAG_COMPORT_UC, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return m_iLastError;
		}

#if !defined(_FEDM_SUPPORT_SLINK)
		if(0 == lpfn(iPortHnd, "PortNr", cValue))
#else
		if(0 == FECOM_GetPortPara(iPortHnd, "PortNr", cValue))
#endif
		{
#if defined(__BORLANDC__) || defined(_MSC_VER)
			swprintf(wcTmp, L"%S", cValue);
#else
			swprintf(wcTmp, 128, L"%s", cValue);
#endif
			AddTagValue(child, wcTmp);
			AddTagItem(parent, child);

			// header tag communication mode
			child = BuildTag(FEDM_XML_TAG_COMM_MODE_UC, false);
			if(child == NULL)
			{
				DeleteXmlTree(); // clean up
				return m_iLastError;
			}
			AddTagValue(child, FEDM_XML_STRING_COMM_MODE_SERIAL_UC);
			AddTagItem(parent, child);
		}
		else
		{
			delete child;
		}
#endif
	}

	// header tag protocol-frame-support
	if(! FEDM_IS_USBPORT(iPortHnd))
	{
		child = BuildTag(FEDM_XML_TAG_FRAME_SUPPORT, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return m_iLastError;
		}
		if(pReader->GetProtocolFrameSupport() == FEDM_PRT_FRAME_ADVANCED)
		{
#if defined(__BORLANDC__) || defined(_MSC_VER)
			swprintf(wcTmp, L"%S", "advanced");
#else
			swprintf(wcTmp, 128, L"%s", "advanced");
#endif
		}
		else
		{
#if defined(__BORLANDC__) || defined(_MSC_VER)
			swprintf(wcTmp, L"%S", "standard");
#else
			swprintf(wcTmp, 128, L"%s", "standard");
#endif
		}
		AddTagValue(child, wcTmp);
		AddTagItem(parent, child);
	}
	

	// header tag comment
	if(m_wsComment.size() > 0)
	{
		child = BuildTag(FEDM_XML_TAG_COMMENT_UC, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return m_iLastError;
		}
		AddTagValue(child, m_wsComment);
		AddTagItem(parent, child);
	}

	// header tag program name
	child = BuildTag(FEDM_XML_TAG_PRG_NAME_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	AddTagValue(child, m_wsPrgName);
	AddTagItem(parent, child);

	// header tag program version number
	child = BuildTag(FEDM_XML_TAG_PRG_VERSION_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	AddTagValue(child, m_wsPrgVer);
	AddTagItem(parent, child);

	// header tag fedm library version number
	child = BuildTag(FEDM_XML_TAG_FEDM_VERSION_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	AddTagValue(child, FEDM_VERSION_UC);
	AddTagItem(parent, child);

	// header tag date
#if !defined(_FEDM_SUPPORT_UCLINUX) && !defined(_WIN32_WCE)
	child = BuildTag(FEDM_XML_TAG_DATE_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	time(&ltime);
	tmtime = localtime(&ltime);
	wcsftime(wcTmp, 128, L"%x", tmtime);
	AddTagValue(child, wcTmp);
	AddTagItem(parent, child);
#endif

	// header tag time
#if !defined(_FEDM_SUPPORT_UCLINUX) && !defined(_WIN32_WCE)
	child = BuildTag(FEDM_XML_TAG_TIME_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	wcsftime(wcTmp, 128, L"%X", tmtime);
	AddTagValue(child, wcTmp);
	AddTagItem(parent, child);
#endif

	// header tag <reader-info>
	if(m_pReaderInfo)
	{
		iBack = AddReaderInfo(pReader, root);
		if(iBack)
		{
			DeleteXmlTree(); // clean up
			return iBack;
		}
	}

	// add data array tags
	iBack = pReader->SerializeOut(this, root);
	if(iBack)
	{
		DeleteXmlTree(); // clean up
		return iBack;
	}

	iBack = OpenDoc(sFileName, "w");
	if(iBack)
	{
		DeleteXmlTree(); // clean up
		return iBack;
	}
	else
	{
		WriteDoc();
	}


	CloseDoc();

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        	:	16.04.2008 / M. Hultsch
  Version       :	03.00.00 / 16.01.2009 / M. Hultsch

  Function		:	get reader information from xml file in tag <reader>
					>> these tags are all optional <<

  Parameters	:	FEDM_DataBase* pReader	- pointer to reader object
					FEDM_XML_TREEITEM* root	- pointer to root element

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FEDM_XMLReaderCfgDataModul::GetReaderInfo(FEDM_DataBase* pReader, FEDM_XML_TREEITEM* root)
{
	char cValue[128];
	int iTmp = 0;
	unsigned int uiReaderType = 0;
	FEDM_XML_TREEITEM* reader = NULL;
	FEDM_XML_TREEITEM* child = NULL;
	FEDM_XML_TREEITEM* sub = NULL;
	FEDM_XML_ATTRIBUTE_LIST_ITOR AttrItor;

	// tag <reader-info>
	child = FindTag(FEDM_XML_TAG_READER_INFO_UC, false, true);
	if(child == NULL)
		return FEDM_OK;

	AddTagItem(root, child);
	reader = child;

	// header tags <controller>
	child = FindTag(FEDM_XML_TAG_CONTROLLER_UC, true);
	while(child)
	{
		AddTagItem(reader, child);
		sub = child;
		for(AttrItor = sub->aAttribList.begin(); AttrItor != sub->aAttribList.end(); AttrItor++)
		{
			if((*AttrItor)->sAttribute != FEDM_XML_ATTRIB_TYPE_UC)
				continue;

			if((*AttrItor)->sValue == L"RFC")
			{
				// header tag <reader-type>
				child = FindTag(FEDM_XML_TAG_READER_TYPE_UC, true);
				if(child == NULL)
					return FEDM_OK;
				AddTagItem(sub, child);
				FEDM_SWSCANF(child->sValue, cValue);
				sscanf(	cValue, 
						"%u", 
						(unsigned int*)&uiReaderType );

				if(uiReaderType & 0x00000080)
				{
					m_pReaderInfo->ucReaderClass		= (unsigned char)( uiReaderType & 0x000000FF);
					m_pReaderInfo->ucReaderClassGen		= (unsigned char)((uiReaderType & 0x0000FF00) >>  8);
					m_pReaderInfo->ucReaderTypeOfClass	= (unsigned char)((uiReaderType & 0x00FF0000) >> 16);
					m_pReaderInfo->ucReserved3			= (unsigned char)((uiReaderType & 0xFF000000) >> 24);
				}
				else
				{
					m_pReaderInfo->ucReaderType			= (unsigned char)(uiReaderType & 0x000000FF);
				}

				// tag <firmware-version>
				child = FindTag(FEDM_XML_TAG_FIRMWARE_VERSION_UC);
				if(child == NULL)
					return FEDM_OK;
				AddTagItem(sub, child);
				FEDM_SWSCANF(child->sValue, cValue);
				sscanf(	cValue, 
						"%d.%d.%d", 
						(unsigned int*)&m_pReaderInfo->ucRfcSwVer[0], 
						(unsigned int*)&m_pReaderInfo->ucRfcSwVer[1], 
						(unsigned int*)&m_pReaderInfo->ucRfcDevVer );


				// tag <hardware-type>
				child = FindTag(FEDM_XML_TAG_HARDWARE_TYPE_UC);
				if(child == NULL)
					return FEDM_OK;
				AddTagItem(sub, child);
				FEDM_SWSCANF(child->sValue, cValue);
				sscanf(cValue, "0x%02X", &iTmp);
				m_pReaderInfo->ucRfcHwType = (unsigned char)iTmp;

				m_pReaderInfo->bIsMode0x00Read = true;
			}
			else if((*AttrItor)->sValue == L"ACC")
			{
				m_pReaderInfo->bIsMode0x01Read = true;
			}
			else if((*AttrItor)->sValue == L"USBC")
			{
				m_pReaderInfo->bIsMode0x02Read = true;
			}
			else if((*AttrItor)->sValue == L"RFD")
			{
				m_pReaderInfo->bIsMode0x03Read = true;
			}
			else if((*AttrItor)->sValue == L"FPGA")
			{
				m_pReaderInfo->bIsMode0x03Read = true;
			}
			else if((*AttrItor)->sValue == L"BOOT")
			{
				m_pReaderInfo->bIsMode0x05Read = true;
			}
		}

		child = FindTag(FEDM_XML_TAG_CONTROLLER_UC, false, true, true);
	}

	m_pReaderInfo->bIsInfoRead = true;
	return FEDM_OK;
}


/***************************************************************************
  Begin        	:	16.04.2008 / M. Hultsch
  Version       :	04.00.09 / 01.02.2012 / M. Hultsch
					- mode 0x04: modification for CPR-Reader

					03.00.00 / 28.08.2008 / M. Hultsch

  Function		:	add reader information to xml file in tag <reader>

  Parameters	:	FEDM_DataBase* pReader	- pointer to reader object
					FEDM_XML_TREEITEM* root	- pointer to root element

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FEDM_XMLReaderCfgDataModul::AddReaderInfo(FEDM_DataBase* pReader, FEDM_XML_TREEITEM* root)
{
	unsigned int uiTmp = 0;
	wchar_t wcTmp[128];
	FEDM_XML_TREEITEM* reader = NULL;
	FEDM_XML_TREEITEM* child = NULL;
	FEDM_XML_TREEITEM* sub = NULL;

	child = BuildTag(FEDM_XML_TAG_READER_INFO_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}

	AddTagItem(root, child);
	reader = child;

	// header tag <reader-name>
	child = BuildTag(FEDM_XML_TAG_READER_NAME_UC, false);
	if(child == NULL)
	{
		return m_iLastError;
	}
#if defined(__BORLANDC__) || defined(_MSC_VER)
	swprintf(wcTmp, L"%S", pReader->GetReaderName());
#else
	swprintf(wcTmp, 128, L"%s", pReader->GetReaderName());
#endif
	AddTagValue(child, wcTmp);
	AddTagItem(reader, child);

	// header tag <reader-type>
	child = BuildTag(FEDM_XML_TAG_READER_TYPE_UC, false);
	if(child == NULL)
	{
		return m_iLastError;
	}
#if defined(__BORLANDC__) || defined(_MSC_VER)
	swprintf(wcTmp, L"%u", pReader->GetReaderType());
#else
	swprintf(wcTmp, 128, L"%u", pReader->GetReaderType());
#endif
	AddTagValue(child, wcTmp);
	AddTagItem(reader, child);

	// ####################################################################################
	// new standard protocol - must have all newer readers implemented
	// mode: 0x00
	// ####################################################################################
	if(m_pReaderInfo->bIsMode0x00Read)
	{
		// tag <controller>
		child = BuildTag(FEDM_XML_TAG_CONTROLLER_UC);
		if(child == NULL)
		{
			return m_iLastError;
		}
		AddTagAttrib(child, FEDM_XML_ATTRIB_TYPE_UC, L"RFC");
		AddTagItem(reader, child);
		sub = child;

		// tag <firmware-version>
		child = BuildTag(FEDM_XML_TAG_FIRMWARE_VERSION_UC);
		if(child == NULL)
		{
			return m_iLastError;
		}
#if defined(__BORLANDC__) || defined(_MSC_VER)
		swprintf(wcTmp, L"%d.%d.%d", m_pReaderInfo->ucRfcSwVer[0], m_pReaderInfo->ucRfcSwVer[1], m_pReaderInfo->ucRfcDevVer);
#else
		swprintf(wcTmp, 128, L"%d.%d.%d", m_pReaderInfo->ucRfcSwVer[0], m_pReaderInfo->ucRfcSwVer[1], m_pReaderInfo->ucRfcDevVer);
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(sub, child);

		// tag <hardware-type>
		child = BuildTag(FEDM_XML_TAG_HARDWARE_TYPE_UC);
		if(child == NULL)
		{
			return m_iLastError;
		}
#if defined(__BORLANDC__) || defined(_MSC_VER)
		swprintf(wcTmp, L"0x%02X", m_pReaderInfo->ucRfcHwType);
#else
		swprintf(wcTmp, 128, L"0x%02X", m_pReaderInfo->ucRfcHwType);
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(sub, child);

		if(pReader->GetReaderType() == FEDM_ISC_TYPE_ISCPRHD102)
		{
			// tag <hf-transponder-driver>
			child = BuildTag(FEDM_XML_TAG_HF_TRANSPONDER_DRIVER_UC);
			if(child == NULL)
			{
				return m_iLastError;
			}
#if defined(__BORLANDC__) || defined(_MSC_VER)
			swprintf(wcTmp, L"0x%02X%02X", m_pReaderInfo->ucRfcTrType[0], m_pReaderInfo->ucRfcTrType[1]);
#else
			swprintf(wcTmp, 128, L"0x%02X%02X", m_pReaderInfo->ucRfcTrType[0], m_pReaderInfo->ucRfcTrType[1]);
#endif
			AddTagValue(child, wcTmp);
			AddTagItem(sub, child);

			// tag <uhf-transponder-driver>
			child = BuildTag(FEDM_XML_TAG_UHF_TRANSPONDER_DRIVER_UC);
			if(child == NULL)
			{
				return m_iLastError;
			}
#if defined(__BORLANDC__) || defined(_MSC_VER)
			swprintf(wcTmp, L"0x%02X%02X", m_pReaderInfo->ucRfcUhfTrType[0], m_pReaderInfo->ucRfcUhfTrType[1]);
#else
			swprintf(wcTmp, 128, L"0x%02X%02X", m_pReaderInfo->ucRfcUhfTrType[0], m_pReaderInfo->ucRfcUhfTrType[1]);
#endif
			AddTagValue(child, wcTmp);
			AddTagItem(sub, child);
		}
		else
		{
			// tag <transponder-driver>
			child = BuildTag(FEDM_XML_TAG_TRANSPONDER_DRIVER_UC);
			if(child == NULL)
			{
				return m_iLastError;
			}
#if defined(__BORLANDC__) || defined(_MSC_VER)
			swprintf(wcTmp, L"0x%02X%02X", m_pReaderInfo->ucRfcTrType[0], m_pReaderInfo->ucRfcTrType[1]);
#else
			swprintf(wcTmp, 128, L"0x%02X%02X", m_pReaderInfo->ucRfcTrType[0], m_pReaderInfo->ucRfcTrType[1]);
#endif
			AddTagValue(child, wcTmp);
			AddTagItem(sub, child);
		}

		// tag <rx-buffer-size>
		child = BuildTag(FEDM_XML_TAG_RX_BUFFER_SIZE_UC);
		if(child == NULL)
		{
			return m_iLastError;
		}
		uiTmp  = ((unsigned int)m_pReaderInfo->ucRfcRxBufferSize[0]<<8);
		uiTmp += ((unsigned int)m_pReaderInfo->ucRfcRxBufferSize[1]);
#if defined(__BORLANDC__) || defined(_MSC_VER)
		swprintf(wcTmp, L"%d", uiTmp);
#else
		swprintf(wcTmp, 128, L"%d", uiTmp);
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(sub, child);

		// tag <tx-buffer-size>
		child = BuildTag(FEDM_XML_TAG_TX_BUFFER_SIZE_UC);
		if(child == NULL)
		{
			return m_iLastError;
		}
		uiTmp  = ((unsigned int)m_pReaderInfo->ucRfcTxBufferSize[0]<<8);
		uiTmp += ((unsigned int)m_pReaderInfo->ucRfcTxBufferSize[1]);
#if defined(__BORLANDC__) || defined(_MSC_VER)
		swprintf(wcTmp, L"%d", uiTmp);
#else
		swprintf(wcTmp, 128, L"%d", uiTmp);
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(sub, child);

	} //  if(m_pReaderInfo->bIsMode0x00Read)

	// ####################################################################################
	// mode: 0x01 - ACC firmware version
	// ####################################################################################
	if(m_pReaderInfo->bIsMode0x01Read)
	{
		// tag <controller>
		child = BuildTag(FEDM_XML_TAG_CONTROLLER_UC);
		if(child == NULL)
		{
			return m_iLastError;
		}
		AddTagAttrib(child, FEDM_XML_ATTRIB_TYPE_UC, L"ACC");
		AddTagItem(reader, child);
		sub = child;

		// tag <firmware-version>
		child = BuildTag(FEDM_XML_TAG_FIRMWARE_VERSION_UC);
		if(child == NULL)
		{
			return m_iLastError;
		}
#if defined(__BORLANDC__) || defined(_MSC_VER)
		swprintf(wcTmp, L"%d.%d.%d", m_pReaderInfo->ucAccSwVer[0], m_pReaderInfo->ucAccSwVer[1], m_pReaderInfo->ucAccDevVer);
#else
		swprintf(wcTmp, 128, L"%d.%d.%d", m_pReaderInfo->ucAccSwVer[0], m_pReaderInfo->ucAccSwVer[1], m_pReaderInfo->ucAccDevVer);
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(sub, child);

	} //  if(m_pReaderInfo->bIsMode0x01Read)

	// ####################################################################################
	// mode: 0x02 - USB-Controller firmware version
	// ####################################################################################
	if(m_pReaderInfo->bIsMode0x02Read)
	{
		// tag <controller>
		child = BuildTag(FEDM_XML_TAG_CONTROLLER_UC);
		if(child == NULL)
		{
			return m_iLastError;
		}
		AddTagAttrib(child, FEDM_XML_ATTRIB_TYPE_UC, L"USBC");
		AddTagItem(reader, child);
		sub = child;

		// tag <firmware-version>
		child = BuildTag(FEDM_XML_TAG_FIRMWARE_VERSION_UC);
		if(child == NULL)
		{
			return m_iLastError;
		}
#if defined(__BORLANDC__) || defined(_MSC_VER)
		swprintf(wcTmp, L"%d.%d.%d", m_pReaderInfo->ucUsbSwVer[0], m_pReaderInfo->ucUsbSwVer[1], m_pReaderInfo->ucUsbDevVer);
#else
		swprintf(wcTmp, 128, L"%d.%d.%d", m_pReaderInfo->ucUsbSwVer[0], m_pReaderInfo->ucUsbSwVer[1], m_pReaderInfo->ucUsbDevVer);
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(sub, child);

	} //  if(m_pReaderInfo->bIsMode0x02Read)

	// ####################################################################################
	// mode: 0x03 - FPGA firmware version
	//			  - CPR decoder info
	// ####################################################################################
	if(m_pReaderInfo->bIsMode0x03Read)
	{
		// tag <controller>
		child = BuildTag(FEDM_XML_TAG_CONTROLLER_UC);
		if(child == NULL)
		{
			return m_iLastError;
		}

		switch(m_pReaderInfo->ucReaderType)
		{
		// classic-pro
		case FEDM_ISC_TYPE_CPR40_XX_U:
		case FEDM_ISC_TYPE_CPR40_XX:
			AddTagAttrib(child, FEDM_XML_ATTRIB_TYPE_UC, L"RFD");
			AddTagItem(reader, child);
			sub = child;
			// tag <decoder-info>
			child = BuildTag(FEDM_XML_TAG_DECODER_INFO_UC);
			if(child == NULL)
			{
				return m_iLastError;
			}
#if defined(__BORLANDC__) || defined(_MSC_VER)
			swprintf(	wcTmp, 
						L"0x%02X 0x%02X 0x%02X 0x%02X 0x%02X", 
						m_pReaderInfo->ucDecoderType[0], 
						m_pReaderInfo->ucDecoderType[1], 
						m_pReaderInfo->ucDecoderType[2],
						m_pReaderInfo->ucDecoderType[3],
						m_pReaderInfo->ucDecoderType[4]	);
#else
			swprintf(	wcTmp, 
						128,
						L"0x%02X 0x%02X 0x%02X 0x%02X 0x%02X", 
						m_pReaderInfo->ucDecoderType[0], 
						m_pReaderInfo->ucDecoderType[1], 
						m_pReaderInfo->ucDecoderType[2],
						m_pReaderInfo->ucDecoderType[3],
						m_pReaderInfo->ucDecoderType[4]	);
#endif
			AddTagValue(child, wcTmp);
			AddTagItem(sub, child);

			// tag <self-test>
			child = BuildTag(FEDM_XML_TAG_SELF_TEST_UC);
			if(child == NULL)
			{
				return m_iLastError;
			}
#if defined(__BORLANDC__) || defined(_MSC_VER)
			swprintf(wcTmp, L"0x%02X", m_pReaderInfo->ucSelfTest);
#else
			swprintf(wcTmp, 128, L"0x%02X", m_pReaderInfo->ucSelfTest);
#endif
			AddTagValue(child, wcTmp);
			AddTagItem(sub, child);
			break;

		// HF-Reader
		case FEDM_ISC_TYPE_ISCLR2000:
		case FEDM_ISC_TYPE_ISCLR2500_A:
		case FEDM_ISC_TYPE_ISCLR2500_B:
		// UHF-Reader
		case FEDM_ISC_TYPE_ISCMRU200:
		case FEDM_ISC_TYPE_ISCLRU2000:
		case FEDM_ISC_TYPE_ISCLRU3000:
			AddTagAttrib(child, FEDM_XML_ATTRIB_TYPE_UC, L"FPGA");
			AddTagItem(reader, child);
			sub = child;

			// tag <firmware-version>
			child = BuildTag(FEDM_XML_TAG_FIRMWARE_VERSION_UC);
			if(child == NULL)
			{
				return m_iLastError;
			}
#if defined(__BORLANDC__) || defined(_MSC_VER)
			swprintf(wcTmp, L"%d.%d.%d", m_pReaderInfo->ucFpgaSwVer[0], m_pReaderInfo->ucFpgaSwVer[1], m_pReaderInfo->ucFpgaDevVer);
#else
			swprintf(wcTmp, 128, L"%d.%d.%d", m_pReaderInfo->ucFpgaSwVer[0], m_pReaderInfo->ucFpgaSwVer[1], m_pReaderInfo->ucFpgaDevVer);
#endif
			AddTagValue(child, wcTmp);
			AddTagItem(sub, child);
			break;
		}

	} //  if(m_pReaderInfo->bIsMode0x03Read)

	// ####################################################################################
	// mode: 0x05 - Bootloader firmware version
	// ####################################################################################
	if(m_pReaderInfo->bIsMode0x05Read)
	{
		// tag <controller>
		child = BuildTag(FEDM_XML_TAG_CONTROLLER_UC);
		if(child == NULL)
		{
			return m_iLastError;
		}
		AddTagAttrib(child, FEDM_XML_ATTRIB_TYPE_UC, L"BOOT");
		AddTagItem(reader, child);
		sub = child;

		// tag <firmware-version>
		child = BuildTag(FEDM_XML_TAG_FIRMWARE_VERSION_UC);
		if(child == NULL)
		{
			return m_iLastError;
		}
#if defined(__BORLANDC__) || defined(_MSC_VER)
		swprintf(wcTmp, L"%d.%d", m_pReaderInfo->ucBootSwVer[0], m_pReaderInfo->ucBootSwVer[1]);
#else
		swprintf(wcTmp, 128, L"%d.%d", m_pReaderInfo->ucBootSwVer[0], m_pReaderInfo->ucBootSwVer[1]);
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(sub, child);

	} //  if(m_pReaderInfo->bIsMode0x05Read)

	// ####################################################################################
	// mode: 0x04 - special firmware information
	// ####################################################################################
	if(m_pReaderInfo->bIsMode0x04Read)
	{
		// tag <additional-firmware-info>
		child = BuildTag(FEDM_XML_TAG_ADD_FIRMWARE_INFO_UC);
		if(child == NULL)
		{
			return m_iLastError;
		}
		AddTagItem(reader, child);
		sub = child;

		switch(m_pReaderInfo->ucReaderType)	// reader type
		{
		// classic-pro
		case FEDM_ISC_TYPE_CPRM02:
		case FEDM_ISC_TYPE_CPR02:
		case FEDM_ISC_TYPE_CPR04_U:
			// tag <template>
			child = BuildTag(FEDM_XML_TAG_TEMPLATE_UC);
			if(child == NULL)
			{
				return m_iLastError;
			}
#if defined(__BORLANDC__) || defined(_MSC_VER)
			swprintf(wcTmp, L"0x%02X", m_pReaderInfo->ucCprFctTemplate);
#else
			swprintf(wcTmp, 128, L"0x%02X", m_pReaderInfo->ucCprFctTemplate);
#endif
			AddTagValue(child, wcTmp);
			AddTagItem(sub, child);

			// tag <function-list>
			child = BuildTag(FEDM_XML_TAG_FUNCTION_LIST_UC);
			if(child == NULL)
			{
				return m_iLastError;
			}
#if defined(__BORLANDC__) || defined(_MSC_VER)
			swprintf(wcTmp, L"0x%02X", m_pReaderInfo->ucCprFctList);
#else
			swprintf(wcTmp, 128, L"0x%02X", m_pReaderInfo->ucCprFctList);
#endif
			AddTagValue(child, wcTmp);
			AddTagItem(sub, child);
			break;

		case FEDM_ISC_TYPE_CPR20_XX:
		case FEDM_ISC_TYPE_CPR30_XX:
		case FEDM_ISC_TYPE_CPR40_XX_U:
		case FEDM_ISC_TYPE_CPR40_XX:
		case FEDM_ISC_TYPE_CPR44_XX:
		case FEDM_ISC_TYPE_CPR46_XX:
		case FEDM_ISC_TYPE_CPR47_XX:
		case FEDM_ISC_TYPE_CPR50_XX:
		case FEDM_ISC_TYPE_CPR52_XX:
		case FEDM_ISC_TYPE_CPR60_XX:
		case FEDM_ISC_TYPE_MAX50_XX:
		// myAXXESS
		case FEDM_ISC_TYPE_MAX_STANDARD:
		case FEDM_ISC_TYPE_MAX_COMFORT:
		case FEDM_ISC_TYPE_MAX_ONTOP:
			// tag <template>
			child = BuildTag(FEDM_XML_TAG_TEMPLATE_UC);
			if(child == NULL)
			{
				return m_iLastError;
			}
#if defined(__BORLANDC__) || defined(_MSC_VER)
			swprintf(wcTmp, L"0x%02X", m_pReaderInfo->ucCprFctTemplate);
#else
			swprintf(wcTmp, 128, L"0x%02X", m_pReaderInfo->ucCprFctTemplate);
#endif
			AddTagValue(child, wcTmp);
			AddTagItem(sub, child);

			// tag <function-list>
			child = BuildTag(FEDM_XML_TAG_FUNCTION_LIST_UC);
			if(child == NULL)
			{
				return m_iLastError;
			}
#if defined(__BORLANDC__) || defined(_MSC_VER)
			swprintf(wcTmp, L"0x%02X 0x%02X 0x%02X", m_pReaderInfo->ucCprFctList0, m_pReaderInfo->ucCprFctList1, m_pReaderInfo->ucCprFctList2);
#else
			swprintf(wcTmp, 128, L"0x%02X 0x%02X 0x%02X", m_pReaderInfo->ucCprFctList0, m_pReaderInfo->ucCprFctList1, m_pReaderInfo->ucCprFctList2);
#endif
			AddTagValue(child, wcTmp);
			AddTagItem(sub, child);

			// tag <identifier>
			child = BuildTag(FEDM_XML_TAG_IDENTIFIER_UC);
			if(child == NULL)
			{
				return m_iLastError;
			}
#if defined(__BORLANDC__) || defined(_MSC_VER)
			swprintf(	wcTmp, 
						L"0x%02X 0x%02X", 
						m_pReaderInfo->ucFwIdentifierC[0], 
						m_pReaderInfo->ucFwIdentifierC[1] );
#else
			swprintf(	wcTmp, 
						128,
						L"0x%02X 0x%02X", 
						m_pReaderInfo->ucFwIdentifierC[0], 
						m_pReaderInfo->ucFwIdentifierC[1] );
#endif
			AddTagValue(child, wcTmp);
			AddTagItem(sub, child);

		case FEDM_ISC_TYPE_ISCMU02:
		case FEDM_ISC_TYPE_ISCLR2000:
		case FEDM_ISC_TYPE_ISCLR2500_A:
		case FEDM_ISC_TYPE_ISCLR2500_B:
		case FEDM_ISC_TYPE_ISCLRU2000:
		case FEDM_ISC_TYPE_ISCLRU3000:
			// tag <identifier>
			child = BuildTag(FEDM_XML_TAG_IDENTIFIER_UC);
			if(child == NULL)
			{
				return m_iLastError;
			}
#if defined(__BORLANDC__) || defined(_MSC_VER)
			swprintf(	wcTmp, 
						L"0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X", 
						m_pReaderInfo->ucFwIdentifierB[0], 
						m_pReaderInfo->ucFwIdentifierB[1], 
						m_pReaderInfo->ucFwIdentifierB[2],
						m_pReaderInfo->ucFwIdentifierB[3],
						m_pReaderInfo->ucFwIdentifierB[4],
						m_pReaderInfo->ucFwIdentifierB[5],
						m_pReaderInfo->ucFwIdentifierB[6],
						m_pReaderInfo->ucFwIdentifierB[7],
						m_pReaderInfo->ucFwIdentifierB[8],
						m_pReaderInfo->ucFwIdentifierB[9],
						m_pReaderInfo->ucFwIdentifierB[10]	);
#else
			swprintf(	wcTmp, 
						128,
						L"0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X", 
						m_pReaderInfo->ucFwIdentifierB[0], 
						m_pReaderInfo->ucFwIdentifierB[1], 
						m_pReaderInfo->ucFwIdentifierB[2],
						m_pReaderInfo->ucFwIdentifierB[3],
						m_pReaderInfo->ucFwIdentifierB[4],
						m_pReaderInfo->ucFwIdentifierB[5],
						m_pReaderInfo->ucFwIdentifierB[6],
						m_pReaderInfo->ucFwIdentifierB[7],
						m_pReaderInfo->ucFwIdentifierB[8],
						m_pReaderInfo->ucFwIdentifierB[9],
						m_pReaderInfo->ucFwIdentifierB[10]	);
#endif
			AddTagValue(child, wcTmp);
			AddTagItem(sub, child);
			break;
		}

	} //  if(m_pReaderInfo->bIsMode0x04Read)

	// ####################################################################################
	// mode: 0x10 - additional hardware information
	// ####################################################################################
	if(m_pReaderInfo->bIsMode0x10Read)
	{
		// tag <additional-hardware-info>
		child = BuildTag(FEDM_XML_TAG_ADD_HARDWARE_INFO_UC);
		if(child == NULL)
		{
			return m_iLastError;
		}
		AddTagItem(reader, child);
		sub = child;

		switch(m_pReaderInfo->ucReaderType)	// reader type
		{
		// HF-Reader
		case FEDM_ISC_TYPE_ISCPRHD102:
		case FEDM_ISC_TYPE_ISCLR2000:
		case FEDM_ISC_TYPE_ISCLR2500_A:
		case FEDM_ISC_TYPE_ISCLR2500_B:
		// UHF-Reader
		case FEDM_ISC_TYPE_ISCMRU102:
		case FEDM_ISC_TYPE_ISCMRU200:
		case FEDM_ISC_TYPE_ISCLRU1000:
		case FEDM_ISC_TYPE_ISCLRU2000:
		case FEDM_ISC_TYPE_ISCLRU3000:
			// tag <hw-info>
			child = BuildTag(FEDM_XML_TAG_HW_INFO_UC);
			if(child == NULL)
			{
				return m_iLastError;
			}
#if defined(__BORLANDC__) || defined(_MSC_VER)
			swprintf(wcTmp, L"0x%02X%02X", m_pReaderInfo->ucRfcHwInfo[0], m_pReaderInfo->ucRfcHwInfo[0]);
#else
			swprintf(wcTmp, 128, L"0x%02X%02X", m_pReaderInfo->ucRfcHwInfo[0], m_pReaderInfo->ucRfcHwInfo[0]);
#endif
			AddTagValue(child, wcTmp);
			AddTagItem(sub, child);

			// tag <ahw-info>
			child = BuildTag(FEDM_XML_TAG_AHW_INFO_UC);
			if(child == NULL)
			{
				return m_iLastError;
			}
#if defined(__BORLANDC__) || defined(_MSC_VER)
			swprintf(wcTmp, L"0x%02X%02X", m_pReaderInfo->ucRfcAHw[0], m_pReaderInfo->ucRfcAHw[0]);
#else
			swprintf(wcTmp, 128, L"0x%02X%02X", m_pReaderInfo->ucRfcAHw[0], m_pReaderInfo->ucRfcAHw[0]);
#endif
			AddTagValue(child, wcTmp);
			AddTagItem(sub, child);

			// tag <dhw-info>
			child = BuildTag(FEDM_XML_TAG_DHW_INFO_UC);
			if(child == NULL)
			{
				return m_iLastError;
			}
#if defined(__BORLANDC__) || defined(_MSC_VER)
			swprintf(wcTmp, L"0x%02X%02X", m_pReaderInfo->ucRfcDHw[0], m_pReaderInfo->ucRfcDHw[0]);
#else
			swprintf(wcTmp, 128, L"0x%02X%02X", m_pReaderInfo->ucRfcDHw[0], m_pReaderInfo->ucRfcDHw[0]);
#endif
			AddTagValue(child, wcTmp);
			AddTagItem(sub, child);

			if(m_pReaderInfo->ucRfcFrequency)
			{
				// tag <frequency>
				child = BuildTag(FEDM_XML_TAG_FREQUENCY_UC);
				if(child == NULL)
				{
					return m_iLastError;
				}
				switch(m_pReaderInfo->ucRfcFrequency&0x03)
				{
				case 1:
					AddTagValue(child, L"EU");
					break;
				case 2:
					AddTagValue(child, L"FCC");
					break;
				}
				AddTagItem(sub, child);
			}

			break;
		}
	
	} //  if(m_pReaderInfo->bIsMode0x10Read)

	// ####################################################################################
	// mode: 0x80 - device information with serial number and licensed options
	// ####################################################################################
	if(m_pReaderInfo->bIsMode0x80Read)
	{
		// tag <device-info>
		child = BuildTag(FEDM_XML_TAG_DEVICE_INFO_UC);
		if(child == NULL)
		{
			return m_iLastError;
		}
		AddTagItem(reader, child);
		sub = child;

		// tag <device-id>
		child = BuildTag(FEDM_XML_TAG_DEVICE_ID_UC);
		if(child == NULL)
		{
			return m_iLastError;
		}
#if defined(__BORLANDC__) || defined(_MSC_VER)
		swprintf(wcTmp, L"%u", m_pReaderInfo->GetDeviceID());
#else
		swprintf(wcTmp, 128, L"%u", m_pReaderInfo->GetDeviceID());
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(sub, child);

		// tag <customer-id>
		child = BuildTag(FEDM_XML_TAG_CUSTOMER_ID_UC);
		if(child == NULL)
		{
			return m_iLastError;
		}
		uiTmp  = ((unsigned int)m_pReaderInfo->ucRfcLicenseCustom[0]<<24);
		uiTmp += ((unsigned int)m_pReaderInfo->ucRfcLicenseCustom[1]<<16);
		uiTmp += ((unsigned int)m_pReaderInfo->ucRfcLicenseCustom[2]<<8);
		uiTmp += ((unsigned int)m_pReaderInfo->ucRfcLicenseCustom[3]);
#if defined(__BORLANDC__) || defined(_MSC_VER)
		swprintf(wcTmp, L"0x%08X", uiTmp);
#else
		swprintf(wcTmp, 128, L"0x%08X", uiTmp);
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(sub, child);

		// tag <firmware-version>
		child = BuildTag(FEDM_XML_TAG_FIRMWARE_VERSION_UC);
		if(child == NULL)
		{
			return m_iLastError;
		}
		uiTmp  = ((unsigned int)m_pReaderInfo->ucRfcLicenseFw[0]<<8);
		uiTmp += ((unsigned int)m_pReaderInfo->ucRfcLicenseFw[1]);
#if defined(__BORLANDC__) || defined(_MSC_VER)
		swprintf(wcTmp, L"0x%04X", uiTmp);
#else
		swprintf(wcTmp, 128, L"0x%04X", uiTmp);
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(sub, child);

		if(pReader->GetReaderType() == FEDM_ISC_TYPE_ISCPRHD102)
		{
			// tag <hf-transponder-driver>
			child = BuildTag(FEDM_XML_TAG_HF_TRANSPONDER_DRIVER_UC);
			if(child == NULL)
			{
				return m_iLastError;
			}
			uiTmp  = ((unsigned int)m_pReaderInfo->ucRfcLicenseTrType[0]<<8);
			uiTmp += ((unsigned int)m_pReaderInfo->ucRfcLicenseTrType[1]);
#if defined(__BORLANDC__) || defined(_MSC_VER)
			swprintf(wcTmp, L"0x%04X", uiTmp);
#else
			swprintf(wcTmp, 128, L"0x%04X", uiTmp);
#endif
			AddTagValue(child, wcTmp);
			AddTagItem(sub, child);

			// tag <hf-reader-functions>
			child = BuildTag(FEDM_XML_TAG_HF_READER_FUNCTIONS_UC);
			if(child == NULL)
			{
				return m_iLastError;
			}
			uiTmp  = ((unsigned int)m_pReaderInfo->ucRfcLicenseFct[0]<<8);
			uiTmp += ((unsigned int)m_pReaderInfo->ucRfcLicenseFct[1]);
#if defined(__BORLANDC__) || defined(_MSC_VER)
			swprintf(wcTmp, L"0x%04X", uiTmp);
#else
			swprintf(wcTmp, 128, L"0x%04X", uiTmp);
#endif
			AddTagValue(child, wcTmp);
			AddTagItem(sub, child);

			// tag <uhf-transponder-driver>
			child = BuildTag(FEDM_XML_TAG_UHF_TRANSPONDER_DRIVER_UC);
			if(child == NULL)
			{
				return m_iLastError;
			}
			uiTmp  = ((unsigned int)m_pReaderInfo->ucRfcLicenseUhfTrType[0]<<8);
			uiTmp += ((unsigned int)m_pReaderInfo->ucRfcLicenseUhfTrType[1]);
#if defined(__BORLANDC__) || defined(_MSC_VER)
			swprintf(wcTmp, L"0x%04X", uiTmp);
#else
			swprintf(wcTmp, 128, L"0x%04X", uiTmp);
#endif
			AddTagValue(child, wcTmp);
			AddTagItem(sub, child);

			// tag <uhf-reader-functions>
			child = BuildTag(FEDM_XML_TAG_UHF_READER_FUNCTIONS_UC);
			if(child == NULL)
			{
				return m_iLastError;
			}
			uiTmp  = ((unsigned int)m_pReaderInfo->ucRfcLicenseUhfFct[0]<<8);
			uiTmp += ((unsigned int)m_pReaderInfo->ucRfcLicenseUhfFct[1]);
#if defined(__BORLANDC__) || defined(_MSC_VER)
			swprintf(wcTmp, L"0x%04X", uiTmp);
#else
			swprintf(wcTmp, 128, L"0x%04X", uiTmp);
#endif
			AddTagValue(child, wcTmp);
			AddTagItem(sub, child);
		}
		else
		{
			// tag <transponder-driver>
			child = BuildTag(FEDM_XML_TAG_TRANSPONDER_DRIVER_UC);
			if(child == NULL)
			{
				return m_iLastError;
			}
			uiTmp  = ((unsigned int)m_pReaderInfo->ucRfcLicenseTrType[0]<<8);
			uiTmp += ((unsigned int)m_pReaderInfo->ucRfcLicenseTrType[1]);
#if defined(__BORLANDC__) || defined(_MSC_VER)
			swprintf(wcTmp, L"0x%04X", uiTmp);
#else
			swprintf(wcTmp, 128, L"0x%04X", uiTmp);
#endif
			AddTagValue(child, wcTmp);
			AddTagItem(sub, child);

			// tag <reader-functions>
			child = BuildTag(FEDM_XML_TAG_READER_FUNCTIONS_UC);
			if(child == NULL)
			{
				return m_iLastError;
			}
			uiTmp  = ((unsigned int)m_pReaderInfo->ucRfcLicenseFct[0]<<8);
			uiTmp += ((unsigned int)m_pReaderInfo->ucRfcLicenseFct[1]);
#if defined(__BORLANDC__) || defined(_MSC_VER)
			swprintf(wcTmp, L"0x%04X", uiTmp);
#else
			swprintf(wcTmp, 128, L"0x%04X", uiTmp);
#endif
			AddTagValue(child, wcTmp);
			AddTagItem(sub, child);
		}

	} //  if(m_pReaderInfo->bIsMode0x80Read)

	FEDM_RETURN(FEDM_OK);
}


//#####################################################################################
// query functions
//#####################################################################################

/***************************************************************************
  Begin        	:	23.04.2003 / M. Hultsch
  Version       :	01.09.08 / 21.06.2004 / M. Hultsch

  Function		:	open a xml document and get the document type

  Parameters	:	char* sFileName	- pointer to string with file name
					char* sDocType	- pointer to string for document type

  Return value	:	FEDM_OK or error code (<0)

***************************************************************************/
int FEDM_XMLReaderCfgDataModul::QueryDocType(char* sFileName, char* sDocType)
{
	int iBack = 0;
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

#if defined(__BORLANDC__) || defined(_MSC_VER)
	swscanf(child->sValue.c_str(), L"%S", sDocType);
#elif defined(_FEDM_SUPPORT_UCLINUX)
	FEDM_SWSCANF(child->sValue.c_str(), sDocType);
#else
	swscanf(child->sValue.c_str(), L"%s", sDocType);
#endif
	
	delete child;
	DeleteXmlTree(); // clean up

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	23.04.2003 / M. Hultsch
  Version       :	01.09.08 / 21.06.2004 / M. Hultsch

  Function		:	open a xml document and get the doc version

  Parameters	:	char* sFileName		- pointer to string with file name
					char* sDocVersion	- pointer to string for document version

  Return value	:	FEDM_OK or error code (<0)

***************************************************************************/
int FEDM_XMLReaderCfgDataModul::QueryDocVersion(char* sFileName, char* sDocVersion)
{
	int iBack = 0;
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
	m_wsDocType = child->sValue;
	AddTagItem(parent, child);

	// header tag document version
	child = FindTag(FEDM_XML_TAG_DOC_VERSION_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}

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
  Begin        	:	23.04.2003 / M. Hultsch
  Version       :	01.09.08 / 21.06.2004 / M. Hultsch

  Function		:	open a xml document and get the family code

  Parameters	:	char* sFileName	- pointer to string with file name
					char* sFamCode	- pointer to string for family code

  Return value	:	FEDM_OK or error code (<0)

***************************************************************************/
int FEDM_XMLReaderCfgDataModul::QueryReaderFamily(char* sFileName, char* sFamCode)
{
	int iBack = 0;
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
	m_wsDocType = child->sValue;
	AddTagItem(parent, child);

	// header tag document version
	child = FindTag(FEDM_XML_TAG_DOC_VERSION_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	if(child->sValue != FEDM_XML_STRING_DOC_VERSION_UC)
	{
		m_wsDocVer = child->sValue;
		delete child;
		DeleteXmlTree(); // clean up
		FEDM_RETURN(FEDM_XML_ERROR_DOC_FILE_VERSION);
	}
	AddTagItem(parent, child);

	// header tag reader family name
	child = FindTag(FEDM_XML_TAG_READER_FAM_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}

	// check the family code
	if(child->sValue.size() > 4)
	{
		delete child;
		DeleteXmlTree(); // clean up
		FEDM_RETURN(FEDM_XML_ERROR_DOC_FILE_FAMILY);
	}

#if defined(__BORLANDC__) || defined(_MSC_VER)
	swscanf(child->sValue.c_str(), L"%S", sFamCode);
#elif defined(_FEDM_SUPPORT_UCLINUX)
	FEDM_SWSCANF(child->sValue.c_str(), sFamCode);
#else
	swscanf(child->sValue.c_str(), L"%s", sFamCode);
#endif
	
	delete child;
	DeleteXmlTree(); // clean up

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	23.04.2003 / M. Hultsch
  Version       :	01.09.08 / 21.06.2004 / M. Hultsch

  Function		:	open a xml document and get the reader type

  Parameters	:	-

  Return value	:	reader type (>0) or error code (<0)

***************************************************************************/
int FEDM_XMLReaderCfgDataModul::QueryReaderType(char* sFileName)
{
	char cValue[5];
	int iType = 0;
	int iBack = 0;
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
	m_wsDocType = child->sValue;
	AddTagItem(parent, child);

	// header tag document version
	child = FindTag(FEDM_XML_TAG_DOC_VERSION_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	if(child->sValue != FEDM_XML_STRING_DOC_VERSION_UC)
	{
		m_wsDocVer = child->sValue;
		delete child;
		DeleteXmlTree(); // clean up
		return FEDM_XML_ERROR_DOC_FILE_VERSION;
	}
	AddTagItem(parent, child);

	// header tag reader type
	child = FindTag(FEDM_XML_TAG_READER_TYPE_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}

#if defined(__BORLANDC__) || defined(_MSC_VER)
	swscanf(child->sValue.c_str(), L"%S", cValue);
#elif defined(_FEDM_SUPPORT_UCLINUX)
	FEDM_SWSCANF(child->sValue.c_str(), cValue);
#else
	swscanf(child->sValue.c_str(), L"%s", cValue);
#endif
	sscanf(cValue, "%d", &iType);

	delete child;
	DeleteXmlTree(); // clean up

	return iType;
}



//#####################################################################################
// get/set functions
//#####################################################################################

void FEDM_XMLReaderCfgDataModul::SetReaderInfo(FEDM_ISC_READER_INFO* pReaderInfo)
{
	m_pReaderInfo = pReaderInfo;
}


/***************************************************************************
  Begin        	:	23.04.2003 / M. Hultsch
  Version       :	01.08.00 / 04.09.2003 / M. Hultsch

  Function		:	gets the comment

  Parameters	:	string& sComment	- reference to string with comment

  Return value	:	-

***************************************************************************/
void FEDM_XMLReaderCfgDataModul::GetComment(string& sComment)
{
	char cTmp[256];

	if(m_wsComment.size() > 256)
		return;

#if defined(__BORLANDC__) || defined(_MSC_VER)
	sprintf(cTmp, "%S", m_wsComment.c_str());
#else
	sprintf(cTmp, "%s", (const char*)m_wsComment.c_str());
#endif

	sComment = cTmp;
}

/***************************************************************************
  Begin        	:	23.04.2003 / M. Hultsch
  Version       :	01.08.00 / 04.09.2003 / M. Hultsch

  Function		:	gets the comment

  Parameters	:	string& sPrgName	- reference to string with program name

  Return value	:	-

***************************************************************************/
void FEDM_XMLReaderCfgDataModul::GetPrgName(string& sPrgName)
{
	char cTmp[256];

	if(m_wsPrgName.size() > 256)
		return;

#if defined(__BORLANDC__) || defined(_MSC_VER)
	sprintf(cTmp, "%S", m_wsPrgName.c_str());
#else
	sprintf(cTmp, "%s", (const char*)m_wsPrgName.c_str());
#endif

	sPrgName = cTmp;
}

/***************************************************************************
  Begin        	:	23.04.2003 / M. Hultsch
  Version       :	01.08.00 / 04.09.2003 / M. Hultsch

  Function		:	gets the program version number

  Parameters	:	string& sPrgVer	- reference to string with program version number

  Return value	:	-

***************************************************************************/
void FEDM_XMLReaderCfgDataModul::GetPrgVer(string& sPrgVer)
{
	char cTmp[256];

	if(m_wsPrgVer.size() > 256)
		return;

#if defined(__BORLANDC__) || defined(_MSC_VER)
	sprintf(cTmp, "%S", m_wsPrgVer.c_str());
#else
	sprintf(cTmp, "%s", (const char*)m_wsPrgVer.c_str());
#endif

	sPrgVer = cTmp;
}

/***************************************************************************
  Begin        	:	23.05.2003 / M. Hultsch
  Version       :	01.08.00 / 04.09.2003 / M. Hultsch

  Function		:	gets the host name/address

  Parameters	:	string& sHost	- reference to string with host name/address

  Return value	:	-

***************************************************************************/
void FEDM_XMLReaderCfgDataModul::GetHost(string& sHost)
{
	char cTmp[256];

	if(m_wsHost.size() > 256)
		return;

#if defined(__BORLANDC__) || defined(_MSC_VER)
	sprintf(cTmp, "%S", m_wsHost.c_str());
#else
	sprintf(cTmp, "%s", (const char*)m_wsHost.c_str());
#endif

	sHost = cTmp;
}

/***************************************************************************
  Begin        	:	23.05.2003 / M. Hultsch
  Version       :	01.08.00 / 04.09.2003 / M. Hultsch

  Function		:	gets the port number

  Parameters	:	string& sPort	- reference to string with port number

  Return value	:	-

***************************************************************************/
void FEDM_XMLReaderCfgDataModul::GetPort(string& sPort)
{
	char cTmp[256];

	if(m_wsPort.size() > 256)
		return;

#if defined(__BORLANDC__) || defined(_MSC_VER)
	sprintf(cTmp, "%S", m_wsPort.c_str());
#else
	sprintf(cTmp, "%s", (const char*)m_wsPort.c_str());
#endif

	sPort = cTmp;
}

/***************************************************************************
  Begin        	:	11.07.2003 / M. Hultsch
  Version       :	01.08.00 / 04.09.2003 / M. Hultsch

  Function		:	gets the last communication mode

  Parameters	:	string& sMode	- reference to string with communication mode

  Return value	:	-

***************************************************************************/
void FEDM_XMLReaderCfgDataModul::GetCommMode(string& sMode)
{
	char cTmp[256];

	if(m_wsCommMode.size() > 256)
		return;

#if defined(__BORLANDC__) || defined(_MSC_VER)
	sprintf(cTmp, "%S", m_wsCommMode.c_str());
#else
	sprintf(cTmp, "%s", (const char*)m_wsCommMode.c_str());
#endif

	sMode = cTmp;
}

/***************************************************************************
  Begin        	:	23.04.2003 / M. Hultsch
  Version       :	01.08.00 / 01.09.2003 / M. Hultsch

  Function		:	sets the comment

  Parameters	:	string sComment	- string with comment

  Return value	:	-

***************************************************************************/
void FEDM_XMLReaderCfgDataModul::SetComment(string sComment)
{
	wchar_t wcTmp[256];

	if(sComment.size() > 256)
		return;

#if defined(__BORLANDC__) || defined(_MSC_VER)
	swprintf(wcTmp, L"%S", sComment.c_str());
#else
	swprintf(wcTmp, 256, L"%s", sComment.c_str());
#endif

	m_wsComment = wcTmp;
}

/***************************************************************************
  Begin        	:	23.04.2003 / M. Hultsch
  Version       :	01.08.00 / 01.08.2003 / M. Hultsch

  Function		:	sets the program name

  Parameters	:	string sPrgName	- string with program name

  Return value	:	-

***************************************************************************/
void FEDM_XMLReaderCfgDataModul::SetPrgName(string sPrgName)
{
	wchar_t wcTmp[256];

	if(sPrgName.size() > 256)
		return;

#if defined(__BORLANDC__) || defined(_MSC_VER)
	swprintf(wcTmp, L"%S", sPrgName.c_str());
#else
	swprintf(wcTmp, 256, L"%s", sPrgName.c_str());
#endif

	m_wsPrgName = wcTmp;
}

/***************************************************************************
  Begin        	:	23.04.2003 / M. Hultsch
  Version       :	01.08.00 / 01.09.2003 / M. Hultsch

  Function		:	sets the program version number

  Parameters	:	string sPrgVer	- string with program version number

  Return value	:	-

***************************************************************************/
void FEDM_XMLReaderCfgDataModul::SetPrgVer(string sPrgVer)
{
	wchar_t wcTmp[256];

	if(sPrgVer.size() > 256)
		return;

#if defined(__BORLANDC__) || defined(_MSC_VER)
	swprintf(wcTmp, L"%S", sPrgVer.c_str());
#else
	swprintf(wcTmp, 256, L"%s", sPrgVer.c_str());
#endif

	m_wsPrgVer = wcTmp;
}

#endif // #ifdef _FEDM_XML_SUPPORT
