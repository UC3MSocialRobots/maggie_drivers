/*-------------------------------------------------------
|                                                       |
|	FEDM_XmlParser_IWriter.cpp			                |
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
Version				:	04.00.02 / 22.08.2011 / M. Hultsch
Operation Systems	:	independent
Function			:	abstract xml writer class for basic parser API

OBID� and OBID i-scan� are registered Trademarks of FEIG ELECTRONIC GmbH.
Linux� is a registered trademark of Linus Torvalds.
Microsoft� and Windows� are registered trademarks of Microsoft Corporation.
*/



//#############################
//      I N C L U D E S
//#############################
#ifdef _FEDM_LINUX
#include <sys/time.h>
#endif
#include <time.h>
#include "FEDM_XmlParser_IWriter.h"				// public include file of library
#include "FEDM_XmlParser_Base.h"


//#######################################################################################
//	class FEDM_XmlParser_Writer
//#######################################################################################
FEDM_XmlParser_Writer::FEDM_XmlParser_Writer(void)
{
	m_pXmlParser = NULL;
}

FEDM_XmlParser_Writer::~FEDM_XmlParser_Writer(void)
{
}


//#####################################################################################
// serialization functions
//#####################################################################################

/***************************************************************************
  Begin        	:	23.01.2008
  Version       :	04.00.02 / 22.08.2011 / M. Hultsch

  Function		:	write file-header

  Parameters	:	FEDM_XML_TREEITEM* root	parent element

  Return value	:	FEDM_OK or error code (<0)

***************************************************************************/
int FEDM_XmlParser_Writer::SerializeHeaderOut(FEDM_XML_TREEITEM* root, const char* szVendor)
{
	if(root == NULL)
	{
		m_pXmlParser->SetLastError(FEDM_ERROR_NULL_POINTER);
		return FEDM_ERROR_NULL_POINTER;
	}

	unsigned char ucTmp[2];
	wchar_t wcTmp[512];
#ifdef _FEDM_WINDOWS
#if _MSC_VER == 1200
	time_t long_time;
	struct tm* tmtime;
#else
	#ifdef __BORLANDC__
	time_t long_time;
	struct tm* tmtime;
	#else
	__time64_t long_time;
	struct tm tmtime;
	#endif
#endif
#endif
#ifdef _FEDM_LINUX
	time_t	ltime;
	struct	tm* tmtime;
	struct	timeval tv;
#endif
	FEDM_XML_TREEITEM* child	= NULL;
	FEDM_XML_TREEITEM* parent	= NULL;

	// header tag
	child = BuildTag(FEDM_XML_TAG_FILE_HDR_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		m_pXmlParser->SetLastError(m_pXmlParser->GetLastError());
		return m_pXmlParser->GetLastError();
	}
	AddTagItem(root, child);
	parent = child;

	// header tag document type
	child = BuildTag(FEDM_XML_TAG_DOC_TYPE_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		m_pXmlParser->SetLastError(m_pXmlParser->GetLastError());
		return m_pXmlParser->GetLastError();
	}
#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
	swprintf(wcTmp, L"%S", m_pXmlParser->m_XmlHeader.szDocType);
	#else
		#if defined(_WIN32_WCE)
	swprintf(wcTmp, L"%S", m_pXmlParser->m_XmlHeader.szDocType);
		#else
	swprintf(wcTmp, 512, L"%S", m_pXmlParser->m_XmlHeader.szDocType);
		#endif
	#endif
#else
		swprintf(wcTmp, 512, L"%s", m_pXmlParser->m_XmlHeader.szDocType);
#endif
	AddTagValue(child, wcTmp);
	AddTagItem(parent, child);

	// header tag document version
	child = BuildTag(FEDM_XML_TAG_DOC_VERSION_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		m_pXmlParser->SetLastError(m_pXmlParser->GetLastError());
		return m_pXmlParser->GetLastError();
	}
	ucTmp[0] = (unsigned char)m_pXmlParser->m_XmlHeader.uiDocVersion;
	ucTmp[1] = (unsigned char)(m_pXmlParser->m_XmlHeader.uiDocVersion >> 8);
#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
	swprintf(wcTmp, L"%d.%d", (int)ucTmp[1], (int)ucTmp[0]);
	#else
		#if defined(_WIN32_WCE)
	swprintf(wcTmp, L"%d.%d", (int)ucTmp[1], (int)ucTmp[0]);
		#else
	swprintf(wcTmp, 512, L"%d.%d", (int)ucTmp[1], (int)ucTmp[0]);
		#endif
	#endif
#else
	swprintf(wcTmp, 512, L"%d.%d", (int)ucTmp[1], (int)ucTmp[0]);
#endif
	AddTagValue(child, wcTmp);
	AddTagItem(parent, child);

	// header tag vendor
	child = BuildTag(FEDM_XML_TAG_VENDOR_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		m_pXmlParser->SetLastError(m_pXmlParser->GetLastError());
		return m_pXmlParser->GetLastError();
	}
	if(strlen(szVendor) > 0)
	{
#if defined(__BORLANDC__) || defined(_MSC_VER)
                #if _MSC_VER <= 1200
                swprintf(wcTmp, L"%S", szVendor);
                #else
                        #if defined(_WIN32_WCE)
                swprintf(wcTmp, L"%S", szVendor);
                        #else
                swprintf(wcTmp, 512, L"%S", szVendor);
                        #endif
                #endif
#else
                swprintf(wcTmp, 512, L"%s", szVendor);
#endif
                AddTagValue(child, wcTmp);
	}
	else
	{
		AddTagValue(child, L"FEIG ELECTRONIC");
	}
	AddTagItem(parent, child);

	// header tag fedm-version
	child = BuildTag(FEDM_XML_TAG_FEDM_VERSION_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		m_pXmlParser->SetLastError(m_pXmlParser->GetLastError());
		return m_pXmlParser->GetLastError();
	}
	AddTagValue(child, FEDM_VERSION_UC);
	AddTagItem(parent, child);

	// header tag date
	child = BuildTag(FEDM_XML_TAG_DATE_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		m_pXmlParser->SetLastError(m_pXmlParser->GetLastError());
		return m_pXmlParser->GetLastError();
	}

#ifdef _FEDM_WINDOWS
#if _MSC_VER == 1200
	time( &long_time );
	tmtime = localtime( &long_time );
	wcsftime(wcTmp, 128, L"%Y-%m-%d", tmtime);
#else
	#ifdef __BORLANDC__
	time( &long_time );
	tmtime = localtime( &long_time );
	wcsftime(wcTmp, 128, L"%Y-%m-%d", tmtime);
	#else
	_time64( &long_time );           // Get time as 64-bit integer.
                                     // Convert to local time.
	_localtime64_s( &tmtime, &long_time ); // C4996
	wcsftime(wcTmp, 128, L"%Y-%m-%d", &tmtime);
	#endif
#endif
#endif

#ifdef _FEDM_LINUX
	gettimeofday(&tv, NULL);	// get the time in �s
	ltime = tv.tv_sec;			// convert the �s into time of the day
	tmtime = localtime(&ltime);
	wcsftime(wcTmp, 128, L"%Y-%m-%d", tmtime);
#endif
	AddTagValue(child, wcTmp);
	AddTagItem(parent, child);

	// header tag time
	child = BuildTag(FEDM_XML_TAG_TIME_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		m_pXmlParser->SetLastError(m_pXmlParser->GetLastError());
		return m_pXmlParser->GetLastError();
	}

#ifdef _FEDM_WINDOWS
#if _MSC_VER == 1200
	wcsftime(wcTmp, 128, L"%X", tmtime);
#else
	#ifdef __BORLANDC__
	wcsftime(wcTmp, 128, L"%X", tmtime);
	#else
	wcsftime(wcTmp, 128, L"%X", &tmtime);
	#endif
#endif
#endif

#ifdef _FEDM_LINUX
	wcsftime(wcTmp, 128, L"%X", tmtime);
#endif

	AddTagValue(child, wcTmp);
	AddTagItem(parent, child);

	// header tag comment
	child = BuildTag(FEDM_XML_TAG_COMMENT_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		m_pXmlParser->SetLastError(m_pXmlParser->GetLastError());
		return m_pXmlParser->GetLastError();
	}
#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
	swprintf(wcTmp, L"%S", m_pXmlParser->m_XmlHeader.szComment);
	#else
		#if defined(_WIN32_WCE)
	swprintf(wcTmp, L"%S", m_pXmlParser->m_XmlHeader.szComment);
		#else
	swprintf(wcTmp, 512, L"%S", m_pXmlParser->m_XmlHeader.szComment);
		#endif
	#endif
#else
	swprintf(wcTmp, 512, L"%s", m_pXmlParser->m_XmlHeader.szComment);
#endif
	AddTagValue(child, wcTmp);
	AddTagItem(parent, child);

	// header tag checksum
	child = BuildTag(FEDM_XML_TAG_CRC_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		m_pXmlParser->SetLastError(m_pXmlParser->GetLastError());
		return m_pXmlParser->GetLastError();
	}
#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
	swprintf(wcTmp, L"%d", m_pXmlParser->m_XmlHeader.uiCrc);
	#else
		#if defined(_WIN32_WCE)
	swprintf(wcTmp, L"%d", m_pXmlParser->m_XmlHeader.uiCrc);
		#else
	swprintf(wcTmp, 512, L"%d", m_pXmlParser->m_XmlHeader.uiCrc);
		#endif
	#endif
#else
	swprintf(wcTmp, 512, L"%d", m_pXmlParser->m_XmlHeader.uiCrc);
#endif
	AddTagValue(child, wcTmp);
	AddTagItem(parent, child);

	m_pXmlParser->SetLastError(FEDM_OK);
	return m_pXmlParser->GetLastError();
}
