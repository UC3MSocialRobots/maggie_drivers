/*-------------------------------------------------------
|                                                       |
|                  FEDM_DataBase.cpp                    |
|                                                       |
---------------------------------------------------------

Copyright © 2000-2014	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	13.05.2000

Version       		:	04.06.08 / 22.10.2014 / M. Hultsch
						- OptionalSetDataAction extended for ID ISC.PRH200

						04.06.02 / 08.05.2012 / M. Hultsch
						- OptionalSetDataAction extended for ID ISC.LRU3000

						04.05.04 / 04.09.2013 / M. Hultsch
						- OptionalSetDataAction extended for ID ISC.LRU1002

						04.03.00 / 20.08.2012 / M. Hultsch
						- OptionalSetDataAction extended for ID ISC.LR1002 and LR2500-B (Direction)

						04.02.06 / 03.08.2012 / M. Hultsch
						- OptionalSetDataAction extended for ID ISC.LR1002

						04.00.10 / 06.02.2012 / M. Hultsch
						- support for external logmanager (FELOGMNG.dll), when #define _FELOGMNG_SUPPORT is set

						04.00.08 / 05.01.2012 / M. Hultsch
						- OptionalSetDataAction extended for ID ISC.MRU102

						04.00.04 / 28.09.2011 / M. Hultsch
						- only for DLL: reference counter initializes module handle

						03.03.03 / 09.05.2011 / M. Hultsch
						- parameter m_ucReaderType moved from unsigned char to unsigned int

						03.03.00 / 01.03.2011 / M. Hultsch

Operation Systems	:	independent

Function			:	abstract class with data memories


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#include "FEDM_DataBase.h"
#include "FEDM_Functions.h"
#include "FEDM_XmlReaderCfgDataModul.h"
#include "FEDM_XmlReaderCfgProfileModul.h"
#include "i_scan/FEDM_ISC.h"

#if _MSC_VER >= 1400
	#pragma warning(disable : 4996)
#endif


#if defined(_FEDM_ISC_CORE_DLL) || defined(_FEDM_ISC_CORE_LIB) || defined(_OBIDISC4J_DLL) || (defined(_FEDM_NO_DLL) && defined(_FELOGMNG_SUPPORT))
// reference counter for created reader objects
int g_iInstallCounter = 0;
#endif


//#####################################################################################
// Construction/Destruction
//#####################################################################################

FEDM_DataBase::FEDM_DataBase()
{
	m_iReaderHnd	= 0;		// reader handle for FExxx.DLL
	m_ucBusAdr		= 0xFF;		// bus address for reader

	m_sFamCode[0]		= '\0';
	m_sReaderName[0]	= '\0';
//	m_ucReaderType		= 0x00;
	m_uiReaderType		= 0;	// extended reader type number

	m_iRFC_DataBlockSize	= 16;
	m_iACC_DataBlockSize	= 32;
	m_iPubMemBlockSize		= 4;

	m_iUsedEEDataBlocks		= 0;
	m_iUsedRAMDataBlocks	= 0;
	m_iUsedAccEEDataBlocks	= 0;
	m_iUsedTmpDataBlocks	= 0;
	m_iUsedMjpDataBlocks	= 0;
	m_iUsedSNMemBlocks		= 0;
	m_iUsedIDMemBlocks		= 0;
	m_iUsedACMemBlocks		= 0;
	m_iUsedPubMemBlocks		= 0;
	m_iUsedSecMemBlocks		= 0;
	m_iUsedConfMemBlocks	= 0;
	m_iUsedDateMemBlocks	= 0;

#if defined(_FEDM_ISC_CORE_DLL) || defined(_OBIDISC4J_DLL) || (defined(_FEDM_NO_DLL) && defined(_FELOGMNG_SUPPORT))
	g_iInstallCounter++;
	if(g_iInstallCounter > 0x0FFFFFFF)
		g_iInstallCounter = 1;

	m_iModuleHnd = g_iInstallCounter;
#else
	m_iModuleHnd = 0;
#endif


#ifdef _FELOGMNG_SUPPORT
	// for logmanager support
	string sVersion = "FEDM V";
	sVersion += FEDM_VERSION;
	sVersion += " from ";
	sVersion += __DATE__;
	sVersion += " loaded";
	m_pLogger = new FEDM_Logger(FELOGMNG_SRC_FEDM_CORE, g_iInstallCounter, sVersion);
	if(m_pLogger != NULL)
	{
		if(m_pLogger->m_pLib == NULL)
		{
			delete m_pLogger;
			m_pLogger = NULL;
		}
	}
#else
	m_pLogger = NULL;
#endif
}

FEDM_DataBase::~FEDM_DataBase()
{
#ifdef _FELOGMNG_SUPPORT
	if(m_pLogger != NULL)
	{
		delete m_pLogger;
		m_pLogger = NULL;
	}
#endif
}


//#####################################################################################
// functions for important settings
//#####################################################################################

int FEDM_DataBase::GetReaderHnd()
{
	return m_iReaderHnd;
}


void FEDM_DataBase::SetReaderHnd(int iReaderHnd)
{
	m_iReaderHnd = iReaderHnd;
}


void FEDM_DataBase::SetBusAddress(unsigned char ucBusAdr)
{
	m_ucBusAdr = ucBusAdr;
}

void FEDM_DataBase::SetModuleHnd(int iHnd)
{
	m_iModuleHnd = iHnd;
}

int FEDM_DataBase::GetModuleHnd()
{
	return m_iModuleHnd;
}


//#####################################################################################
// query functions
//#####################################################################################

unsigned char FEDM_DataBase::GetBusAddress()
{
	return m_ucBusAdr;
}

char* FEDM_DataBase::GetFamilyCode()
{
	return m_sFamCode;
}

char* FEDM_DataBase::GetReaderName()
{
	return m_sReaderName;
}

unsigned int FEDM_DataBase::GetReaderType()
{
	return m_uiReaderType;
}


int FEDM_DataBase::GetAccessID(string sParaName, string& sAccessID, bool bEEPROM)
{
	FEDM_MAP_ACCESS_ID_ITOR		itor;

	itor = m_mapAccessID.find(sParaName);
	if(itor == m_mapAccessID.end())
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(ParaName=" << sParaName.c_str() << ", .., ..)" << GetErrorText(FEDM_ERROR_UNSUPPORTED_NAMESPACE));
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED_NAMESPACE);
	}

	sAccessID = itor->second;

	if(! bEEPROM)
		sAccessID = FEDM_ToRAM(sAccessID.c_str());

	return FEDM_OK;
}



//#####################################################################################
// functions for XML serialization
//#####################################################################################


/***************************************************************************
  Begin        		:	11.04.2003 / M. Hultsch
  Version       	:	02.04.04 / 22.08.2006 / M. Hultsch

  Function			:	main-function for XML file-I/O

  Parameters		:	FEDM_XMLReaderCfgDataModul xml	-	pointer to the xml handler
						bool bRead			-	true: read from file
												false: write into file
						char* sFileName		-	file name

  Return value		:	OK or error code

***************************************************************************/
//#ifdef _FEDM_XML_SUPPORT
int FEDM_DataBase::Serialize(FEDM_XMLReaderCfgDataModul* xml, 
							 bool bRead, 
							 char* sFileName)
{
#ifdef _FEDM_XML_SUPPORT
	if(bRead)
		return xml->SerializeIn(this, sFileName);
	else
		return xml->SerializeOut(this, sFileName);
#else
	return FEDM_ERROR_UNSUPPORTED;
#endif
}

/***************************************************************************
  Begin        		:	11.04.2003 / M. Hultsch
  Version       	:	03.00.00 / 20.10.2008 / M. Hultsch

  Function			:	sub-function for read from XML file
						for every iMemType call SerializeIn

  Parameters		:	int iMemType			- mem type identifier
						FEDM_XMLBase* xml		- xml handler
						FEDM_XML_TREEITEM* root	- root xml tree item

  Return value		:	OK or error code

***************************************************************************/
int FEDM_DataBase::SerializeIn( int iMemType,
							    FEDM_XMLBase* xml,
							    FEDM_XML_TREEITEM* root)
{
#ifdef _FEDM_XML_SUPPORT
	FEDM_CHK3(root);

	bool bMarkAsRead = false;
	int iFirstBlock = 0;
	int iMaxBlocks = 0;
	int iBlockSize = 0;
	size_t iArraySize = 0;
	char cAttribute[5];
	unsigned char ucValue;
	wstring sTag;
	string sAttribute1;
	string sAttribute2;
	string sValue;
	wstring wsAttribute;
	wstring wsValue;
	wchar_t wcValue[5];
	FEDM_XML_TREEITEM* child = NULL;
	FEDM_XML_TREEITEM* parent = NULL;
	FEDM_BYTE_ARRAY_ITOR itor;

	switch(iMemType)
	{
	// tag attribute for data array "Reader EEPROM-Parameter"
	case FEDM_RFC_EEDATA_MEM:
		child = xml->FindTag( FEDM_XML_TAG_DATA_ARRAY_UC,
							  FEDM_XML_ATTRIB_NAME_UC,
							  FEDM_XML_STRING_EEPROM_DATA_UC,
							  false,
							  true);
		if(child == NULL)
			return xml->GetLastError();

		parent = child;
		itor = m_RFC_EEData.begin();
		iArraySize = m_RFC_EEData.capacity();
		bMarkAsRead = true;
		break;

	// tag attribute for data array "Reader RAM-Parameter"
	case FEDM_RFC_RAMDATA_MEM:
		child = xml->FindTag( FEDM_XML_TAG_DATA_ARRAY_UC,
							  FEDM_XML_ATTRIB_NAME_UC,
							  FEDM_XML_STRING_RAM_DATA_UC,
							  false,
							  true);
		if(child == NULL)
			return xml->GetLastError();

		parent = child;
		itor = m_RFC_RAMData.begin();
		iArraySize = m_RFC_RAMData.capacity();
		bMarkAsRead = true;
		break;

	case FEDM_ACC_EEDATA_MEM:
		iFirstBlock = 64;
		child = xml->FindTag( FEDM_XML_TAG_DATA_ARRAY_UC,
							  FEDM_XML_ATTRIB_NAME_UC,
							  FEDM_XML_STRING_ACC_EEPROM_DATA_UC,
							  false,
							  true);
		if(child == NULL)
			return xml->GetLastError();

		parent = child;
		itor = m_ACC_EEData.begin();
		iArraySize = m_ACC_EEData.capacity();
		bMarkAsRead = true;
		break;

	case FEDM_ACC_RAMDATA_MEM:
		iFirstBlock = 64;
		child = xml->FindTag( FEDM_XML_TAG_DATA_ARRAY_UC,
							  FEDM_XML_ATTRIB_NAME_UC,
							  FEDM_XML_STRING_ACC_RAM_DATA_UC,
							  false,
							  true);
		if(child == NULL)
			return xml->GetLastError();

		parent = child;
		itor = m_ACC_RAMData.begin();
		iArraySize = m_ACC_RAMData.capacity();
		bMarkAsRead = true;
		break;

	// tag attribute for data array "Temporary Protocol Data"
	case FEDM_TMPDATA_MEM:
		child = xml->FindTag( FEDM_XML_TAG_DATA_ARRAY_UC,
							  FEDM_XML_ATTRIB_NAME_UC,
							  FEDM_XML_STRING_TMP_DATA_UC,
							  false,
							  true);
		if(child == NULL)
			return xml->GetLastError();

		parent = child;
		itor = m_TmpData.begin();
		iArraySize = m_TmpData.capacity();
		break;

	// tag attribute for data array "Temporary Multijob Protocol Data"
	case FEDM_MJPDATA_MEM:
		child = xml->FindTag( FEDM_XML_TAG_DATA_ARRAY_UC,
							  FEDM_XML_ATTRIB_NAME_UC,
							  FEDM_XML_STRING_MJP_DATA_UC,
							  false,
							  true);
		if(child == NULL)
			return xml->GetLastError();

		parent = child;
		itor = m_MjpData.begin();
		iArraySize = m_MjpData.capacity();
		break;

	// tag attribute for data array 
	case FEDM_SN_MEM:
		child = xml->FindTag( FEDM_XML_TAG_DATA_ARRAY_UC,
							  FEDM_XML_ATTRIB_NAME_UC,
							  FEDM_XML_STRING_SN_DATA_UC,
							  false,
							  true);
		if(child == NULL)
			return xml->GetLastError();

		parent = child;
		itor = m_SN_Mem.begin();
		iArraySize = m_SN_Mem.capacity();
		break;

	// tag attribute for data array 
	case FEDM_ID_MEM:
		child = xml->FindTag( FEDM_XML_TAG_DATA_ARRAY_UC,
							  FEDM_XML_ATTRIB_NAME_UC,
							  FEDM_XML_STRING_ID_DATA_UC,
							  false,
							  true);
		if(child == NULL)
			return xml->GetLastError();

		parent = child;
		itor = m_ID_Mem.begin();
		iArraySize = m_ID_Mem.capacity();
		break;

	// tag attribute for data array 
	case FEDM_AC_MEM:
		child = xml->FindTag( FEDM_XML_TAG_DATA_ARRAY_UC,
							  FEDM_XML_ATTRIB_NAME_UC,
							  FEDM_XML_STRING_AC_DATA_UC,
							  false,
							  true);
		if(child == NULL)
			return xml->GetLastError();

		parent = child;
		itor = m_AC_Mem.begin();
		iArraySize = m_AC_Mem.capacity();
		break;

	// tag attribute for data array 
	case FEDM_PUB_MEM:
		child = xml->FindTag( FEDM_XML_TAG_DATA_ARRAY_UC,
							  FEDM_XML_ATTRIB_NAME_UC,
							  FEDM_XML_STRING_PUB_MEM_UC,
							  false,
							  true);
		if(child == NULL)
			return xml->GetLastError();

		parent = child;
		itor = m_PubMem.begin();
		iArraySize = m_PubMem.capacity();
		break;

	// tag attribute for data array 
	case FEDM_SEC_MEM:
		child = xml->FindTag( FEDM_XML_TAG_DATA_ARRAY_UC,
							  FEDM_XML_ATTRIB_NAME_UC,
							  FEDM_XML_STRING_SEC_MEM_UC,
							  false,
							  true);
		if(child == NULL)
			return xml->GetLastError();

		parent = child;
		itor = m_SecMem.begin();
		iArraySize = m_SecMem.capacity();
		break;

	// tag attribute for data array 
	case FEDM_CONF_MEM:
		child = xml->FindTag( FEDM_XML_TAG_DATA_ARRAY_UC,
							  FEDM_XML_ATTRIB_NAME_UC,
							  FEDM_XML_STRING_CFG_MEM_UC,
							  false,
							  true);
		if(child == NULL)
			return xml->GetLastError();

		parent = child;
		itor = m_ConfMem.begin();
		iArraySize = m_ConfMem.capacity();
		break;

	// tag attribute for data array 
	case FEDM_DATE_MEM:
		child = xml->FindTag( FEDM_XML_TAG_DATA_ARRAY_UC,
							  FEDM_XML_ATTRIB_NAME_UC,
							  FEDM_XML_STRING_DATE_MEM_UC,
							  false,
							  true);
		if(child == NULL)
			return xml->GetLastError();

		parent = child;
		itor = m_DateMem.begin();
		iArraySize = m_DateMem.capacity();
		break;

	default:
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	// get number of blocks
	xml->GetTagAttrib(parent, 1, wsAttribute, wsValue);
	if(wsAttribute == FEDM_XML_ATTRIB_BLOCKS_UC)
	{
		#if defined(_FEDM_SUPPORT_UCLINUX)
			char buf[10];
			FEDM_SWSCANF(wsValue.c_str(), buf);
			iMaxBlocks = atoi(buf);
		#else
			swscanf(wsValue.c_str(), L"%d", &iMaxBlocks);
		#endif
		if(iMaxBlocks <= 0)
		{
			delete child;
			FEDM_RETURN(FEDM_XML_ERROR_NO_TAG_ATTRIBUTE);
		}
	}

	// get blocksize
	xml->GetTagAttrib(parent, 2, wsAttribute, wsValue);
	if(wsAttribute == FEDM_XML_ATTRIB_SIZE_UC)
	{
		#if defined(_FEDM_SUPPORT_UCLINUX)
			char buf[10];
			FEDM_SWSCANF(wsValue.c_str(), buf);
			iBlockSize = atoi(buf);
		#else
			swscanf(wsValue.c_str(), L"%d", &iBlockSize);
		#endif
		if(iBlockSize != 32 && iBlockSize != 16 && iBlockSize != 4)
		{
			delete child;
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
	}

	if(iMaxBlocks * iBlockSize > (int)iArraySize)
	{
		delete child;
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	xml->AddTagItem(root, child);

	// tags for data blocks
	for(int iBlock=0; iBlock<iMaxBlocks; ++iBlock)
	{
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iBlock);
#else
		swprintf(wcValue, 5, L"%d", iBlock);
#endif
		if( iMemType == FEDM_RFC_EEDATA_MEM || 
			iMemType == FEDM_RFC_RAMDATA_MEM || 
			iMemType == FEDM_ACC_EEDATA_MEM || 
			iMemType == FEDM_ACC_RAMDATA_MEM )
		{
			sTag = FEDM_XML_TAG_CFG_UC;
		}
		else
		{
			sTag = FEDM_XML_TAG_DATA_UC;
		}
		sTag += wcValue;
		child = xml->FindTag(sTag, true, false);
		if(child == NULL)
		{
			itor += iBlockSize;
			continue;
//			xml->CloseDoc();
//			return xml->GetLastError();
		}

		xml->AddTagItem(parent, child);

		// copy all data bytes from attributes into data array
		for(int iByte=0; iByte<iBlockSize; ++iByte)
		{
			xml->GetTagAttrib(child, iByte, sAttribute1, sValue);

			sprintf(cAttribute, "b%d", iByte);
			sAttribute2 = cAttribute;
			FEDM_ConvHexStrToHexUChar((string)sValue.c_str(), &ucValue, 1);

			if(sAttribute1.c_str() == sAttribute2)
			{
				if(iByte==(iBlockSize-1) && bMarkAsRead)
				{
					(*itor) = 0x03;	// mark cfg block as read with a 0x01 in last (unused) byte
									// mark cfg block as modified with 0x02
				}
				else
				{
					(*itor) = ucValue;
				}
			}
			itor++;
		}
	}

	return FEDM_OK;
#else
	return FEDM_ERROR_UNSUPPORTED;
#endif
}

/***************************************************************************
  Begin        		:	11.04.2003 / M. Hultsch
  Version       	:	03.00.00 / 20.10.2008 / M. Hultsch

  Function			:	sub-function for write into XML file
						for every iMemType call SerializeIn

  Parameters		:	int iMemType			- mem type identifier
					    FEDM_XMLBase* xml		- xml handler
						FEDM_XML_TREEITEM* root	- root xml tree item
						int iMaxBlocks			- maximal blocks
						int iBlockSize			- block size of each block

  Return value		:	OK or error code

***************************************************************************/
int FEDM_DataBase::SerializeOut( int iMemType,
							     FEDM_XMLBase* xml,
								 FEDM_XML_TREEITEM* root )
{
#ifdef _FEDM_XML_SUPPORT
	FEDM_CHK3(root);

	int iFirstBlock = 0;
	int iMaxBlocks = 0;
	int iBlockSize = 16;
	int iTmpDataBlockSize = 32;

	wstring str;
	wchar_t wcValue[5];
	wchar_t wcAttrib[5];
	FEDM_XML_TREEITEM* parent = NULL;
	FEDM_XML_TREEITEM* child = NULL;
	FEDM_BYTE_ARRAY_ITOR itor;

	child = xml->BuildTag(FEDM_XML_TAG_DATA_ARRAY_UC);
	if(child == NULL)
	{
		xml->CloseDoc();
		return xml->GetLastError();
	}

	switch(iMemType)
	{
	// tag attribute for data array "Reader EEPROM-Parameter"
	case FEDM_RFC_EEDATA_MEM:
		iMaxBlocks = m_iUsedEEDataBlocks;
		iBlockSize = m_iRFC_DataBlockSize;
		if(iMaxBlocks == 0 || iMaxBlocks*iBlockSize > (int)m_RFC_EEData.capacity())
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_NAME_UC, FEDM_XML_STRING_EEPROM_DATA_UC);
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iMaxBlocks);
#else
		swprintf(wcValue, 5, L"%d", iMaxBlocks);
#endif
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_BLOCKS_UC, wcValue);
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iBlockSize);
#else
		swprintf(wcValue, 5, L"%d", m_iRFC_DataBlockSize);
#endif
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_SIZE_UC, wcValue);
		xml->AddTagItem(root, child);
		parent = child;
		itor = m_RFC_EEData.begin();
		break;

	// tag attribute for data array "Reader RAM-Parameter"
	case FEDM_RFC_RAMDATA_MEM:
		iMaxBlocks = m_iUsedRAMDataBlocks;
		iBlockSize = m_iRFC_DataBlockSize;
		if(iMaxBlocks == 0 || iMaxBlocks*iBlockSize > (int)m_RFC_RAMData.capacity())
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_NAME_UC, FEDM_XML_STRING_RAM_DATA_UC);
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iMaxBlocks);
#else
		swprintf(wcValue, 5, L"%d", iMaxBlocks);
#endif
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_BLOCKS_UC, wcValue);
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iBlockSize);
#else
		swprintf(wcValue, 5, L"%d", m_iRFC_DataBlockSize);
#endif
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_SIZE_UC, wcValue);
		xml->AddTagItem(root, child);
		parent = child;
		itor = m_RFC_RAMData.begin();
		break;

	// tag attribute for data array "Reader EEPROM-Parameter for ACC"
	case FEDM_ACC_EEDATA_MEM:
		iFirstBlock = 64;
		iBlockSize = 32;
		iMaxBlocks = m_iUsedAccEEDataBlocks;
		if(iMaxBlocks == 0 || iMaxBlocks*iBlockSize > (int)m_ACC_EEData.capacity())
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_NAME_UC, FEDM_XML_STRING_ACC_EEPROM_DATA_UC);
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iMaxBlocks);
#else
		swprintf(wcValue, 5, L"%d", iMaxBlocks);
#endif
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_BLOCKS_UC, wcValue);
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iBlockSize);
#else
		swprintf(wcValue, 5, L"%d", iBlockSize);
#endif
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_SIZE_UC, wcValue);
		xml->AddTagItem(root, child);
		parent = child;
		itor = m_ACC_EEData.begin();
		itor += iFirstBlock*iBlockSize;
		break;

	// tag attribute for data array "Reader RAM-Parameter for ACC"
	case FEDM_ACC_RAMDATA_MEM:
		iFirstBlock = 64;
		iBlockSize = 32;
		iMaxBlocks = m_iUsedAccRAMDataBlocks;
		if(iMaxBlocks == 0 || iMaxBlocks*iBlockSize > (int)m_ACC_RAMData.capacity())
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_NAME_UC, FEDM_XML_STRING_ACC_RAM_DATA_UC);
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iMaxBlocks);
#else
		swprintf(wcValue, 5, L"%d", iMaxBlocks);
#endif
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_BLOCKS_UC, wcValue);
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iBlockSize);
#else
		swprintf(wcValue, 5, L"%d", iBlockSize);
#endif
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_SIZE_UC, wcValue);
		xml->AddTagItem(root, child);
		parent = child;
		itor = m_ACC_RAMData.begin();
		itor += iFirstBlock*iBlockSize;
		break;

	// tag attribute for data array "Temporary Protocol Data"
	case FEDM_TMPDATA_MEM:
		iMaxBlocks = m_iUsedTmpDataBlocks;
		iBlockSize = iTmpDataBlockSize;
		if(iMaxBlocks == 0 || iMaxBlocks*iBlockSize > (int)m_TmpData.capacity())
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_NAME_UC, FEDM_XML_STRING_TMP_DATA_UC);
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iMaxBlocks);
#else
		swprintf(wcValue, 5, L"%d", iMaxBlocks);
#endif
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_BLOCKS_UC, wcValue);
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iBlockSize);
#else
		swprintf(wcValue, 5, L"%d", iBlockSize);
#endif
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_SIZE_UC, wcValue);
		xml->AddTagItem(root, child);
		parent = child;
		itor = m_TmpData.begin();
		break;

	// tag attribute for data array "Temporary Multijob Protocol Data"
	case FEDM_MJPDATA_MEM:
		iMaxBlocks = m_iUsedMjpDataBlocks;
		if(iMaxBlocks == 0 || iMaxBlocks*iBlockSize > (int)m_MjpData.capacity())
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_NAME_UC, FEDM_XML_STRING_MJP_DATA_UC);
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iMaxBlocks);
#else
		swprintf(wcValue, 5, L"%d", iMaxBlocks);
#endif
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_BLOCKS_UC, wcValue);
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iBlockSize);
#else
		swprintf(wcValue, 5, L"%d", iBlockSize);
#endif
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_SIZE_UC, wcValue);
		xml->AddTagItem(root, child);
		parent = child;
		itor = m_MjpData.begin();
		break;

	// tag attribute for data array 
	case FEDM_SN_MEM:
		iMaxBlocks = m_iUsedSNMemBlocks;
		if(iMaxBlocks == 0 || iMaxBlocks*iBlockSize > (int)m_SN_Mem.capacity())
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_NAME_UC, FEDM_XML_STRING_SN_DATA_UC);
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iMaxBlocks);
#else
		swprintf(wcValue, 5, L"%d", iMaxBlocks);
#endif
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_BLOCKS_UC, wcValue);
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iBlockSize);
#else
		swprintf(wcValue, 5, L"%d", iBlockSize);
#endif
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_SIZE_UC, wcValue);
		xml->AddTagItem(root, child);
		parent = child;
		itor = m_SN_Mem.begin();
		break;

	// tag attribute for data array 
	case FEDM_ID_MEM:
		iMaxBlocks = m_iUsedIDMemBlocks;
		if(iMaxBlocks == 0 || iMaxBlocks*iBlockSize > (int)m_ID_Mem.capacity())
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_NAME_UC, FEDM_XML_STRING_ID_DATA_UC);
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iMaxBlocks);
#else
		swprintf(wcValue, 5, L"%d", iMaxBlocks);
#endif
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_BLOCKS_UC, wcValue);
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iBlockSize);
#else
		swprintf(wcValue, 5, L"%d", iBlockSize);
#endif
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_SIZE_UC, wcValue);
		xml->AddTagItem(root, child);
		parent = child;
		itor = m_ID_Mem.begin();
		break;

	// tag attribute for data array 
	case FEDM_AC_MEM:
		iMaxBlocks = m_iUsedACMemBlocks;
		if(iMaxBlocks == 0 || iMaxBlocks*iBlockSize > (int)m_AC_Mem.capacity())
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_NAME_UC, FEDM_XML_STRING_AC_DATA_UC);
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iMaxBlocks);
#else
		swprintf(wcValue, 5, L"%d", iMaxBlocks);
#endif
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_BLOCKS_UC, wcValue);
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iBlockSize);
#else
		swprintf(wcValue, 5, L"%d", iBlockSize);
#endif
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_SIZE_UC, wcValue);
		xml->AddTagItem(root, child);
		parent = child;
		itor = m_AC_Mem.begin();
		break;

	// tag attribute for data array 
	case FEDM_PUB_MEM:
		iMaxBlocks = m_iUsedPubMemBlocks;
		iBlockSize = m_iPubMemBlockSize;
		if( iMaxBlocks == 0 || iBlockSize == 0 || iMaxBlocks*iBlockSize > (int)m_PubMem.capacity() )
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_NAME_UC, FEDM_XML_STRING_PUB_MEM_UC);
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iMaxBlocks);
#else
		swprintf(wcValue, 5, L"%d", iMaxBlocks);
#endif
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_BLOCKS_UC, wcValue);
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iBlockSize);
#else
		swprintf(wcValue, 5, L"%d", iBlockSize);
#endif
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_SIZE_UC, wcValue);
		xml->AddTagItem(root, child);
		parent = child;
		itor = m_PubMem.begin();
		break;

	// tag attribute for data array 
	case FEDM_SEC_MEM:
		iMaxBlocks = m_iUsedSecMemBlocks;
		if(iMaxBlocks == 0 || iMaxBlocks*iBlockSize > (int)m_SecMem.capacity())
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_NAME_UC, FEDM_XML_STRING_SEC_MEM_UC);
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iMaxBlocks);
#else
		swprintf(wcValue, 5, L"%d", iMaxBlocks);
#endif
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_BLOCKS_UC, wcValue);
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iBlockSize);
#else
		swprintf(wcValue, 5, L"%d", iBlockSize);
#endif
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_SIZE_UC, wcValue);
		xml->AddTagItem(root, child);
		parent = child;
		itor = m_SecMem.begin();
		break;

	// tag attribute for data array 
	case FEDM_CONF_MEM:
		iMaxBlocks = m_iUsedConfMemBlocks;
		if(iMaxBlocks == 0 || iMaxBlocks*iBlockSize > (int)m_ConfMem.capacity())
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_NAME_UC, FEDM_XML_STRING_CFG_MEM_UC);
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iMaxBlocks);
#else
		swprintf(wcValue, 5, L"%d", iMaxBlocks);
#endif
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_BLOCKS_UC, wcValue);
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iBlockSize);
#else
		swprintf(wcValue, 5, L"%d", iBlockSize);
#endif
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_SIZE_UC, wcValue);
		xml->AddTagItem(root, child);
		parent = child;
		itor = m_ConfMem.begin();
		break;

	// tag attribute for data array 
	case FEDM_DATE_MEM:
		iMaxBlocks = m_iUsedDateMemBlocks;
		if(iMaxBlocks == 0 || iMaxBlocks*iBlockSize > (int)m_DateMem.capacity())
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_NAME_UC, FEDM_XML_STRING_DATE_MEM_UC);
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iMaxBlocks);
#else
		swprintf(wcValue, 5, L"%d", iMaxBlocks);
#endif
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_BLOCKS_UC, wcValue);
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iBlockSize);
#else
		swprintf(wcValue, 5, L"%d", iBlockSize);
#endif
		xml->AddTagAttrib(child, FEDM_XML_ATTRIB_SIZE_UC, wcValue);
		xml->AddTagItem(root, child);
		parent = child;
		itor = m_DateMem.begin();
		break;

	default:
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	// tags for data blocks
	for(int iBlock=iFirstBlock; iBlock<iMaxBlocks; ++iBlock)
	{
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iBlock);
#else
		swprintf(wcValue, 5, L"%d", iBlock);
#endif
		if( iMemType == FEDM_RFC_EEDATA_MEM ||
			iMemType == FEDM_RFC_RAMDATA_MEM ||
			iMemType == FEDM_ACC_EEDATA_MEM ||
			iMemType == FEDM_ACC_RAMDATA_MEM )
			str = FEDM_XML_TAG_CFG_UC;
		else
			str = FEDM_XML_TAG_DATA_UC;
		str += wcValue;
		child = xml->BuildTag(str, false, true);
		if(child == NULL)
		{
			xml->CloseDoc();
			return xml->GetLastError();
		}

		// alle Datenbytes als Attribute in tags einfuegen
		for(int iByte=0; iByte<iBlockSize; ++iByte)
		{
#if defined(_MSC_VER) || defined(__BORLANDC__)
			swprintf(wcAttrib, L"b%d", iByte);
			swprintf(wcValue, L"%02X", *itor);//Array[iBlock*16 + iByte]);
#else
			swprintf(wcAttrib, 5, L"b%d", iByte);
			swprintf(wcValue, 5, L"%02X", *itor);//Array[iBlock*16 + iByte]);
#endif
			xml->AddTagAttrib(child, wcAttrib, wcValue);
			itor++;
		}

		xml->AddTagItem(parent, child);
	}

	return FEDM_OK;
#else
	return FEDM_ERROR_UNSUPPORTED;
#endif
}


/***************************************************************************
  Begin        		:	13.08.2007 / M. Hultsch
  Version       	:	03.00.02 / 14.01.2009 / M. Hultsch

  Function			:	main-function for XML based reader customization

  Parameters		:	FEDM_XMLReaderCfgProfileModul xml	-	pointer to the xml handler
						bool bRead			-	true: read from file
												false: write into file
						char* sFileName		-	file name

  Return value		:	OK or error code

***************************************************************************/
int FEDM_DataBase::SerializeProfile(FEDM_XMLReaderCfgProfileModul* xml, 
									bool bRead, 
									char* sFileName)
{
#ifdef _FEDM_XML_SUPPORT
	if(bRead)
		return xml->SerializeIn(this, sFileName);
	else
		return xml->SerializeOut(this, sFileName);
#else
	return FEDM_ERROR_UNSUPPORTED;
#endif
}

/***************************************************************************
  Begin        		:	13.08.2007 / M. Hultsch
  Version       	:	03.00.02 / 14.01.2009 / M. Hultsch

  Function			:	read sub-function for XML based reader configuration
						only for tag attribute name="Reader EEPROM-Parameter"

  Parameters		:	FEDM_XMLBase* xml		- xml handler
						FEDM_XML_TREEITEM* root	- root xml tree item

  Return value		:	OK or error code

***************************************************************************/
int FEDM_DataBase::SerializeProfileIn(	FEDM_XMLBase* xml,
										FEDM_XML_TREEITEM* root)
{
#ifdef _FEDM_XML_SUPPORT
	FEDM_CHK3(root);

	int iByte=0;
	int iMaxBlocks = 0;
	int iBlockSize = 0;
	size_t iArraySize = 0;
	char cAttribute[5];
	unsigned char ucValue;
	unsigned char ucMask[32];
	wstring sTag;
	string sAttribute;
	string sAttribute1;
	string sAttribute2;
	string sValue;
	wstring wsAttribute;
	wstring wsValue;
	wchar_t wcValue[5];
	FEDM_XML_TREEITEM* child = NULL;
	FEDM_XML_TREEITEM* parent = NULL;
	FEDM_BYTE_ARRAY_ITOR itor_data;
	FEDM_BYTE_ARRAY_ITOR itor_mask;

	child = xml->FindTag(FEDM_XML_TAG_DATA_ARRAY_UC,
						  FEDM_XML_ATTRIB_NAME_UC,
						  FEDM_XML_STRING_EEPROM_DATA_UC,
						  false,
						  true);
	if(child == NULL)
		return xml->GetLastError();

	parent = child;
	itor_data = m_RFC_EEData.begin();
	iArraySize = m_RFC_EEData.capacity();
	itor_mask = m_RFC_CfgProfileMask.begin();

	// get number of blocks
	xml->GetTagAttrib(parent, 1, wsAttribute, wsValue);
	if(wsAttribute == FEDM_XML_ATTRIB_BLOCKS_UC)
	{
		#if defined(_FEDM_SUPPORT_UCLINUX)
			char buf[10];
			FEDM_SWSCANF(wsValue.c_str(), buf);
			iMaxBlocks = atoi(buf);
		#else
			swscanf(wsValue.c_str(), L"%d", &iMaxBlocks);
		#endif
		if(iMaxBlocks <= 0)
		{
			delete child;
			FEDM_RETURN(FEDM_XML_ERROR_NO_TAG_ATTRIBUTE);
		}
	}

	// get blocksize
	xml->GetTagAttrib(parent, 2, wsAttribute, wsValue);
	if(wsAttribute == FEDM_XML_ATTRIB_SIZE_UC)
	{
		#if defined(_FEDM_SUPPORT_UCLINUX)
			char buf[10];
			FEDM_SWSCANF(wsValue.c_str(), buf);
			iBlockSize = atoi(buf);
		#else
			swscanf(wsValue.c_str(), L"%d", &iBlockSize);
		#endif
		if(iBlockSize != 32 && iBlockSize != 16 && iBlockSize != 4)
		{
			delete child;
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
	}

	if(iMaxBlocks * iBlockSize > (int)iArraySize)
	{
		delete child;
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	xml->AddTagItem(root, child);

	// tags for data blocks
	for(int iBlock=0; iBlock<iMaxBlocks; ++iBlock)
	{
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iBlock);
#else
		swprintf(wcValue, 5, L"%d", iBlock);
#endif
		sTag = FEDM_XML_TAG_CFG_UC;
		sTag += wcValue;
		child = xml->FindTag(sTag, true, false);
		if(child == NULL)
		{
			xml->CloseDoc();
			return xml->GetLastError();
		}

		xml->AddTagItem(parent, child);

		// attributes for customization: data mask
		xml->GetTagAttrib(child, 0, sAttribute, sValue);
		FEDM_ConvHexStrToHexUChar((string)sValue.c_str(), ucMask, iBlockSize);
		for(iByte=0; iByte<iBlockSize; ++iByte)
		{
			(*itor_mask) = ucMask[iByte];
			itor_mask++;
		}

		// merge all data bytes from attributes into data array
		// mask controls the merging
		for(iByte=0; iByte<iBlockSize; ++iByte)
		{
			xml->GetTagAttrib(child, iByte+1, sAttribute1, sValue);

			sprintf(cAttribute, "b%d", iByte);
			sAttribute2 = cAttribute;
			FEDM_ConvHexStrToHexUChar((string)sValue.c_str(), &ucValue, 1);

			if(ucMask[iByte] > 0 && sAttribute1.c_str() == sAttribute2)
			{
				(*itor_data) = ((*itor_data) & (~ucMask[iByte])) | (ucValue & ucMask[iByte]);
			}
			itor_data++;
		}
	}

	return FEDM_OK;
#else
	return FEDM_ERROR_UNSUPPORTED;
#endif
}

/***************************************************************************
  Begin        		:	13.08.2007 / M. Hultsch
  Version       	:	03.00.02 / 14.01.2009 / M. Hultsch

  Function			:	write sub-function for XML based reader customization
						only for tag attribute name="Reader EEPROM-Parameter"

  Parameters		:	FEDM_XMLBase* xml		- xml handler
						FEDM_XML_TREEITEM* root	- root xml tree item
						int iMaxBlocks			- maximal blocks
						int iBlockSize			- block size of each block

  Return value		:	OK or error code

***************************************************************************/
int FEDM_DataBase::SerializeProfileOut(	FEDM_XMLBase* xml,
										FEDM_XML_TREEITEM* root )
{
#ifdef _FEDM_XML_SUPPORT
	FEDM_CHK3(root);

	int iByte=0;
	int iMaxBlocks = 0;
	int iBlockSize = 16;
	int iTmpDataBlockSize = 32;

	wstring str;
	wstring wsValue;
	wstring wsAttrib;
	wchar_t wcValue[5];
	wchar_t wcAttrib[32];
	FEDM_XML_TREEITEM* parent = NULL;
	FEDM_XML_TREEITEM* child = NULL;
	FEDM_BYTE_ARRAY_ITOR itor_data;
	FEDM_BYTE_ARRAY_ITOR itor_mask;

	child = xml->BuildTag(FEDM_XML_TAG_DATA_ARRAY_UC);
	if(child == NULL)
	{
		xml->CloseDoc();
		return xml->GetLastError();
	}

	// tag attribute for data array "Reader EEPROM-Parameter"
	iMaxBlocks = m_iUsedEEDataBlocks;
	iBlockSize = m_iRFC_DataBlockSize;
	if(iMaxBlocks == 0 || iMaxBlocks*iBlockSize > (int)m_RFC_EEData.capacity())
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	xml->AddTagAttrib(child, FEDM_XML_ATTRIB_NAME_UC, FEDM_XML_STRING_EEPROM_DATA_UC);
#if defined(_MSC_VER) || defined(__BORLANDC__)
	swprintf(wcValue, L"%d", iMaxBlocks);
#else
	swprintf(wcValue, 5, L"%d", iMaxBlocks);
#endif
	xml->AddTagAttrib(child, FEDM_XML_ATTRIB_BLOCKS_UC, wcValue);
#if defined(_MSC_VER) || defined(__BORLANDC__)
	swprintf(wcValue, L"%d", iBlockSize);
#else
	swprintf(wcValue, 5, L"%d", iBlockSize);
#endif
	xml->AddTagAttrib(child, FEDM_XML_ATTRIB_SIZE_UC, wcValue);
	xml->AddTagItem(root, child);
	parent = child;
	itor_data = m_RFC_EEData.begin();
	itor_mask = m_RFC_CfgProfileMask.begin();

	// tags for data blocks
	for(int iBlock=0; iBlock<iMaxBlocks; ++iBlock)
	{
#if defined(_MSC_VER) || defined(__BORLANDC__)
		swprintf(wcValue, L"%d", iBlock);
#else
		swprintf(wcValue, 5, L"%d", iBlock);
#endif
		str = FEDM_XML_TAG_CFG_UC;
		str += wcValue;
		child = xml->BuildTag(str, false, true);
		if(child == NULL)
		{
			xml->CloseDoc();
			return xml->GetLastError();
		}

		// attributes for customization: data mask
		wsAttrib	= L"mask";
		str.erase(str.begin(), str.end());//clear();
		for(iByte=0; iByte<iBlockSize; ++iByte)
		{
#if defined(_MSC_VER) || defined(__BORLANDC__)
			swprintf(wcValue, L"%02X", *itor_mask);
#else
			swprintf(wcValue, 5, L"%02X", *itor_mask);
#endif
			itor_mask++;
			str += wcValue;
		}
		xml->AddTagAttrib(child, wsAttrib, str);

		// alle Datenbytes als Attribute in tags einfuegen
		for(iByte=0; iByte<iBlockSize; ++iByte)
		{
#if defined(_MSC_VER) || defined(__BORLANDC__)
			swprintf(wcAttrib, L"b%d", iByte);
			swprintf(wcValue, L"%02X", *itor_data);
#else
			swprintf(wcAttrib, 5, L"b%d", iByte);
			swprintf(wcValue, 5, L"%02X", *itor_data);
#endif
			xml->AddTagAttrib(child, wcAttrib, wcValue);
			itor_data++;
		}

		xml->AddTagItem(parent, child);
	}

	return FEDM_OK;
#else
	return FEDM_ERROR_UNSUPPORTED;
#endif
}


//#####################################################################################
// access functions
//#####################################################################################

/***************************************************************************
  Begin        	:	13.05.2000 / M. Hultsch
  Version       :	03.00.00 / 02.04.2008 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	const char* ID	-	parameter identifier
					bool* Data		-	pointer to value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_DataBase::GetData(const char* ID, bool* Data)
{
	FEDM_CHK3(ID);
	FEDM_CHK3(Data);

	int iBack = 0;

	// extract MemID
	int MemID = FEDM_GetMemIDOfID(ID);
	
	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_RFC_EEData, m_iRFC_DataBlockSize);
		break;
	case FEDM_RFC_RAMDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_RFC_RAMData, m_iRFC_DataBlockSize);
		break;
	case FEDM_ACC_EEDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_ACC_EEData, 32);
		break;
	case FEDM_ACC_RAMDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_ACC_RAMData, 32);
		break;
	case FEDM_TMPDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_TmpData, 32);
		break;
	case FEDM_MJPDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_MjpData, 16);
		break;
	case FEDM_SN_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_SN_Mem, 16);
		break;
	case FEDM_ID_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_ID_Mem, 16);
		break;
	case FEDM_AC_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_AC_Mem, 16);
		break;
	case FEDM_PUB_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_PubMem, m_iPubMemBlockSize);
		break;
	case FEDM_SEC_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_SecMem, 4);
		break;
	case FEDM_CONF_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_ConfMem, 4);
		break;
	case FEDM_DATE_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_DateMem, 16);
		break;
	case FEDM_RFC_DATA_MASK_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_RFC_CfgProfileMask, m_iRFC_DataBlockSize);
		break;
	default:
		iBack = FEDM_ERROR_UNKNOWN_MEM_ID;
		break;
	}

	if(iBack < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(ID=" << ID << ", bool*) - " << GetErrorText(iBack));
	}
	return iBack;
}

/***************************************************************************
  Begin        	:	13.05.2000 / M. Hultsch
  Version       :	03.00.00 / 02.04.2008 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	const char* ID	-	parameter identifier
					BOOL* Data		-	pointer to value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_DataBase::GetData(const char* ID, BOOL* Data)
{
	FEDM_CHK3(ID);
	FEDM_CHK3(Data);

	int iBack = 0;

	// extract MemID
	int MemID = FEDM_GetMemIDOfID(ID);
	
	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM:		
		iBack = FEDM_Base::GetData(ID, Data, m_RFC_EEData, m_iRFC_DataBlockSize);
		break;
	case FEDM_RFC_RAMDATA_MEM:		
		iBack = FEDM_Base::GetData(ID, Data, m_RFC_RAMData, m_iRFC_DataBlockSize);
		break;
	case FEDM_ACC_EEDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_ACC_EEData, 32);
		break;
	case FEDM_ACC_RAMDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_ACC_RAMData, 32);
		break;
	case FEDM_TMPDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_TmpData, 32);
		break;
	case FEDM_MJPDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_MjpData, 16);
		break;
	case FEDM_SN_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_SN_Mem, 16);
		break;
	case FEDM_ID_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_ID_Mem, 16);
		break;
	case FEDM_AC_MEM:			
		iBack = FEDM_Base::GetData(ID, Data, m_AC_Mem, 16);
		break;
	case FEDM_PUB_MEM:			
		iBack = FEDM_Base::GetData(ID, Data, m_PubMem, m_iPubMemBlockSize);
		break;
	case FEDM_SEC_MEM:			
		iBack = FEDM_Base::GetData(ID, Data, m_SecMem, 4);
		break;
	case FEDM_CONF_MEM:		
		iBack = FEDM_Base::GetData(ID, Data, m_ConfMem, 4);
		break;
	case FEDM_DATE_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_DateMem, 16);
		break;
	case FEDM_RFC_DATA_MASK_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_RFC_CfgProfileMask, m_iRFC_DataBlockSize);
		break;
	default:
		iBack = FEDM_ERROR_UNKNOWN_MEM_ID;
		break;
	}

	if(iBack < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(ID=" << ID << ",BOOL*) - " << GetErrorText(iBack));
	}
	return iBack;
}
#endif

/***************************************************************************
  Begin        	:	13.05.2000 / M. Hultsch
  Version       :	03.00.00 / 02.04.2008 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	const char* ID	-	parameter identifier
					unsigned char* Data		-	pointer to value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_DataBase::GetData(const char* ID, unsigned char* Data)
{
	FEDM_CHK3(ID);
	FEDM_CHK3(Data);

	int iBack = 0;

	// extract MemID
	int MemID = FEDM_GetMemIDOfID(ID);
	
	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM:		
		iBack = FEDM_Base::GetData(ID, Data, m_RFC_EEData, m_iRFC_DataBlockSize);
		break;
	case FEDM_RFC_RAMDATA_MEM:		
		iBack = FEDM_Base::GetData(ID, Data, m_RFC_RAMData, m_iRFC_DataBlockSize);
		break;
	case FEDM_ACC_EEDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_ACC_EEData, 32);
		break;
	case FEDM_ACC_RAMDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_ACC_RAMData, 32);
		break;
	case FEDM_TMPDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_TmpData, 32);
		break;
	case FEDM_MJPDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_MjpData, 16);
		break;
	case FEDM_SN_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_SN_Mem, 16);
		break;
	case FEDM_ID_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_ID_Mem, 16);
		break;
	case FEDM_AC_MEM:			
		iBack = FEDM_Base::GetData(ID, Data, m_AC_Mem, 16);
		break;
	case FEDM_PUB_MEM:			
		iBack = FEDM_Base::GetData(ID, Data, m_PubMem, m_iPubMemBlockSize);
		break;
	case FEDM_SEC_MEM:			
		iBack = FEDM_Base::GetData(ID, Data, m_SecMem, 4);
		break;
	case FEDM_CONF_MEM:		
		iBack = FEDM_Base::GetData(ID, Data, m_ConfMem,4 );
		break;
	case FEDM_DATE_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_DateMem, 16);
		break;
	case FEDM_RFC_DATA_MASK_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_RFC_CfgProfileMask, m_iRFC_DataBlockSize);
		break;
	default:
		iBack = FEDM_ERROR_UNKNOWN_MEM_ID;
		break;
	}

	if(iBack < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(ID=" << ID << ", unsigned char*) - " << GetErrorText(iBack));
	}
	return iBack;
}

/***************************************************************************
  Begin        	:	13.05.2000 / M. Hultsch
  Version       :	03.00.00 / 02.04.2008 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	const char* ID	-	parameter identifier
					unsigned char* Data		-	pointer to value
					int Cnt			-	length of Data

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_DataBase::GetData(const char* ID, unsigned char* Data, int Cnt)
{
	FEDM_CHK3(ID);
	FEDM_CHK3(Data);

	int iBack = 0;

	// extract MemID
	int MemID = FEDM_GetMemIDOfID(ID);
	
	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM:		
		iBack = FEDM_Base::GetData(ID, Data, Cnt, m_RFC_EEData, m_iRFC_DataBlockSize);
		break;
	case FEDM_RFC_RAMDATA_MEM:		
		iBack = FEDM_Base::GetData(ID, Data, Cnt, m_RFC_RAMData, m_iRFC_DataBlockSize);
		break;
	case FEDM_ACC_EEDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, Cnt, m_ACC_EEData, 32);
		break;
	case FEDM_ACC_RAMDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, Cnt, m_ACC_RAMData, 32);
		break;
	case FEDM_TMPDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, Cnt, m_TmpData, 32);
		break;
	case FEDM_MJPDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, Cnt, m_MjpData, 16);
		break;
	case FEDM_SN_MEM:
		iBack = FEDM_Base::GetData(ID, Data, Cnt, m_SN_Mem, 16);
		break;
	case FEDM_ID_MEM:
		iBack = FEDM_Base::GetData(ID, Data, Cnt, m_ID_Mem, 16);
		break;
	case FEDM_AC_MEM:			
		iBack = FEDM_Base::GetData(ID, Data, Cnt, m_AC_Mem, 16);
		break;
	case FEDM_PUB_MEM:			
		iBack = FEDM_Base::GetData(ID, Data, Cnt, m_PubMem, m_iPubMemBlockSize);
		break;
	case FEDM_SEC_MEM:			
		iBack = FEDM_Base::GetData(ID, Data, Cnt, m_SecMem, 4);
		break;
	case FEDM_CONF_MEM:		
		iBack = FEDM_Base::GetData(ID, Data, Cnt, m_ConfMem, 4);
		break;
	case FEDM_DATE_MEM:
		iBack = FEDM_Base::GetData(ID, Data, Cnt, m_DateMem, 16);
		break;
	case FEDM_RFC_DATA_MASK_MEM:
		iBack = FEDM_Base::GetData(ID, Data, Cnt, m_RFC_CfgProfileMask, m_iRFC_DataBlockSize);
		break;
	default:
		iBack = FEDM_ERROR_UNKNOWN_MEM_ID;
		break;
	}

	if(iBack < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(ID=" << ID << ", unsigned char*, int) - " << GetErrorText(iBack));
	}
	return iBack;
}

/***************************************************************************
  Begin        	:	13.05.2000 / M. Hultsch
  Version       :	03.00.00 / 02.04.2008 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	const char* ID	-	parameter identifier
					unsigned int* Data		-	pointer to value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_DataBase::GetData(const char* ID, unsigned int* Data)
{
	FEDM_CHK3(ID);
	FEDM_CHK3(Data);

	int iBack = 0;

	// extract MemID
	int MemID = FEDM_GetMemIDOfID(ID);
	
	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM:		
		iBack = FEDM_Base::GetData(ID, Data, m_RFC_EEData, m_iRFC_DataBlockSize);
		break;
	case FEDM_RFC_RAMDATA_MEM:		
		iBack = FEDM_Base::GetData(ID, Data, m_RFC_RAMData, m_iRFC_DataBlockSize);
		break;
	case FEDM_ACC_EEDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_ACC_EEData, 32);
		break;
	case FEDM_ACC_RAMDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_ACC_RAMData, 32);
		break;
	case FEDM_TMPDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_TmpData, 32);
		break;
	case FEDM_MJPDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_MjpData, 16);
		break;
	case FEDM_SN_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_SN_Mem, 16);
		break;
	case FEDM_ID_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_ID_Mem, 16);
		break;
	case FEDM_AC_MEM:			
		iBack = FEDM_Base::GetData(ID, Data, m_AC_Mem, 16);
		break;
	case FEDM_PUB_MEM:			
		iBack = FEDM_Base::GetData(ID, Data, m_PubMem, m_iPubMemBlockSize);
		break;
	case FEDM_SEC_MEM:			
		iBack = FEDM_Base::GetData(ID, Data, m_SecMem, 4);
		break;
	case FEDM_CONF_MEM:		
		iBack = FEDM_Base::GetData(ID, Data, m_ConfMem, 4);
		break;
	case FEDM_DATE_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_DateMem, 16);
		break;
	case FEDM_RFC_DATA_MASK_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_RFC_CfgProfileMask, m_iRFC_DataBlockSize);
		break;
	default:
		iBack = FEDM_ERROR_UNKNOWN_MEM_ID;
		break;
	}

	if(iBack < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(ID=" << ID << ", unsigned int*) - " << GetErrorText(iBack));
	}
	return iBack;
}

/***************************************************************************
  Begin        	:	13.05.2000 / M. Hultsch
  Version       :	03.00.00 / 02.04.2008 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	const char* ID		-	parameter identifier
					__int64* Data		-	pointer to value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_DataBase::GetData(const char* ID, __int64* Data)
{
	FEDM_CHK3(ID);
	FEDM_CHK3(Data);

	int iBack = 0;

	// extract MemID
	int MemID = FEDM_GetMemIDOfID(ID);
	
	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM:		
		iBack = FEDM_Base::GetData(ID, Data, m_RFC_EEData, m_iRFC_DataBlockSize);
		break;
	case FEDM_RFC_RAMDATA_MEM:		
		iBack = FEDM_Base::GetData(ID, Data, m_RFC_RAMData, m_iRFC_DataBlockSize);
		break;
	case FEDM_ACC_EEDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_ACC_EEData, 32);
		break;
	case FEDM_ACC_RAMDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_ACC_RAMData, 32);
		break;
	case FEDM_TMPDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_TmpData, 32);
		break;
	case FEDM_MJPDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_MjpData, 16);
		break;
	case FEDM_SN_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_SN_Mem, 16);
		break;
	case FEDM_ID_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_ID_Mem, 16);
		break;
	case FEDM_AC_MEM:			
		iBack = FEDM_Base::GetData(ID, Data, m_AC_Mem, 16);
		break;
	case FEDM_PUB_MEM:			
		iBack = FEDM_Base::GetData(ID, Data, m_PubMem, m_iPubMemBlockSize);
		break;
	case FEDM_SEC_MEM:			
		iBack = FEDM_Base::GetData(ID, Data, m_SecMem, 4);
		break;
	case FEDM_CONF_MEM:		
		iBack = FEDM_Base::GetData(ID, Data, m_ConfMem, 4);
		break;
	case FEDM_DATE_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_DateMem, 16);
		break;
	case FEDM_RFC_DATA_MASK_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_RFC_CfgProfileMask, m_iRFC_DataBlockSize);
		break;
	default:
		iBack = FEDM_ERROR_UNKNOWN_MEM_ID;
		break;
	}

	if(iBack < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(ID=" << ID << ", __int64*) - " << GetErrorText(iBack));
	}
	return iBack;
}

/***************************************************************************
  Begin        	:	13.05.2000 / M. Hultsch
  Version       :	03.00.00 / 02.04.2008 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	const char* ID	-	parameter identifier
					CString& Data	-	reference of value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_DataBase::GetData(const char* ID, CString& Data)
{
	FEDM_CHK3(ID);
	FEDM_CHK3(&Data);

	int iBack = 0;

	// extract MemID
	int MemID = FEDM_GetMemIDOfID(ID);
	
	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM:		
		iBack = FEDM_Base::GetData(ID, Data, m_RFC_EEData, m_iRFC_DataBlockSize);
		break;
	case FEDM_RFC_RAMDATA_MEM:		
		iBack = FEDM_Base::GetData(ID, Data, m_RFC_RAMData, m_iRFC_DataBlockSize);
		break;
	case FEDM_ACC_EEDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_ACC_EEData, 32);
		break;
	case FEDM_ACC_RAMDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_ACC_RAMData, 32);
		break;
	case FEDM_TMPDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_TmpData, 32);
		break;
	case FEDM_MJPDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_MjpData, 16);
		break;
	case FEDM_SN_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_SN_Mem, 16);
		break;
	case FEDM_ID_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_ID_Mem, 16);
		break;
	case FEDM_AC_MEM:			
		iBack = FEDM_Base::GetData(ID, Data, m_AC_Mem, 16);
		break;
	case FEDM_PUB_MEM:			
		iBack = FEDM_Base::GetData(ID, Data, m_PubMem, m_iPubMemBlockSize);
		break;
	case FEDM_SEC_MEM:			
		iBack = FEDM_Base::GetData(ID, Data, m_SecMem, 4);
		break;
	case FEDM_CONF_MEM:		
		iBack = FEDM_Base::GetData(ID, Data, m_ConfMem, 4);
		break;
	case FEDM_DATE_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_DateMem, 16);
		break;
	case FEDM_RFC_DATA_MASK_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_RFC_CfgProfileMask, m_iRFC_DataBlockSize);
		break;
	default:
		iBack = FEDM_ERROR_UNKNOWN_MEM_ID;
		break;
	}

	if(iBack < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(ID=" << ID << ", CString&) - " << GetErrorText(iBack));
	}
	return iBack;
}
#endif

/***************************************************************************
  Begin        	:	03.07.2000 / M. Hultsch
  Version       :	03.00.00 / 02.04.2008 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	const char* ID	-	parameter identifier
					string& Data	-	reference of value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_DataBase::GetData(const char* ID, string& Data)
{
	FEDM_CHK3(ID);
	FEDM_CHK3(&Data);

	int iBack = 0;

	// extract MemID
	int MemID = FEDM_GetMemIDOfID(ID);
	
	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM:		
		iBack = FEDM_Base::GetData(ID, Data, m_RFC_EEData, m_iRFC_DataBlockSize);
		break;
	case FEDM_RFC_RAMDATA_MEM:		
		iBack = FEDM_Base::GetData(ID, Data, m_RFC_RAMData, m_iRFC_DataBlockSize);
		break;
	case FEDM_ACC_EEDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_ACC_EEData, 32);
		break;
	case FEDM_ACC_RAMDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_ACC_RAMData, 32);
		break;
	case FEDM_TMPDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_TmpData, 32);
		break;
	case FEDM_MJPDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_MjpData, 16);
		break;
	case FEDM_SN_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_SN_Mem, 16);
		break;
	case FEDM_ID_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_ID_Mem, 16);
		break;
	case FEDM_AC_MEM:			
		iBack = FEDM_Base::GetData(ID, Data, m_AC_Mem, 16);
		break;
	case FEDM_PUB_MEM:			
		iBack = FEDM_Base::GetData(ID, Data, m_PubMem, m_iPubMemBlockSize);
		break;
	case FEDM_SEC_MEM:			
		iBack = FEDM_Base::GetData(ID, Data, m_SecMem, 4);
		break;
	case FEDM_CONF_MEM:		
		iBack = FEDM_Base::GetData(ID, Data, m_ConfMem, 4);
		break;
	case FEDM_DATE_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_DateMem, 16);
		break;
	case FEDM_RFC_DATA_MASK_MEM:
		iBack = FEDM_Base::GetData(ID, Data, m_RFC_CfgProfileMask, m_iRFC_DataBlockSize);
		break;
	default:
		iBack = FEDM_ERROR_UNKNOWN_MEM_ID;
		break;
	}

	if(iBack < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(ID=" << ID << ", string&) - " << GetErrorText(iBack));
	}
	return iBack;
}

/***************************************************************************
  Begin        	:	13.05.2000 / M. Hultsch
  Version       :	03.00.00 / 02.04.2008 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	const char* ID	-	parameter identifier
					char* Data		-	pointer to value
					int DataLen		-	length of Data

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_DataBase::GetData(const char* ID, char* Data, int DataLen)
{
	FEDM_CHK3(ID);
	FEDM_CHK3(Data);

	int iBack = 0;

	// extract MemID
	int MemID = FEDM_GetMemIDOfID(ID);
	
	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM:		
		iBack = FEDM_Base::GetData(ID, Data, DataLen, m_RFC_EEData, m_iRFC_DataBlockSize);
		break;
	case FEDM_RFC_RAMDATA_MEM:		
		iBack = FEDM_Base::GetData(ID, Data, DataLen, m_RFC_RAMData, m_iRFC_DataBlockSize);
		break;
	case FEDM_ACC_EEDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, DataLen, m_ACC_EEData, 32);
		break;
	case FEDM_ACC_RAMDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, DataLen, m_ACC_RAMData, 32);
		break;
	case FEDM_TMPDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, DataLen, m_TmpData, 32);
		break;
	case FEDM_MJPDATA_MEM:
		iBack = FEDM_Base::GetData(ID, Data, DataLen, m_MjpData, 16);
		break;
	case FEDM_SN_MEM:
		iBack = FEDM_Base::GetData(ID, Data, DataLen, m_SN_Mem, 16);
		break;
	case FEDM_ID_MEM:
		iBack = FEDM_Base::GetData(ID, Data, DataLen, m_ID_Mem, 16);
		break;
	case FEDM_AC_MEM:			
		iBack = FEDM_Base::GetData(ID, Data, DataLen, m_AC_Mem, 16);
		break;
	case FEDM_PUB_MEM:			
		iBack = FEDM_Base::GetData(ID, Data, DataLen, m_PubMem, m_iPubMemBlockSize);
		break;
	case FEDM_SEC_MEM:			
		iBack = FEDM_Base::GetData(ID, Data, DataLen, m_SecMem, 4);
		break;
	case FEDM_CONF_MEM:		
		iBack = FEDM_Base::GetData(ID, Data, DataLen, m_ConfMem, 4);
		break;
	case FEDM_DATE_MEM:
		iBack = FEDM_Base::GetData(ID, Data, DataLen, m_DateMem, 16);
		break;
	case FEDM_RFC_DATA_MASK_MEM:
		iBack = FEDM_Base::GetData(ID, Data, DataLen, m_RFC_CfgProfileMask, m_iRFC_DataBlockSize);
		break;
	default:
		iBack = FEDM_ERROR_UNKNOWN_MEM_ID;
		break;
	}

	if(iBack < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(ID=" << ID << ", char*, int) - " << GetErrorText(iBack));
	}
	return iBack;
}

/***************************************************************************
  Begin        	:	13.05.2000 / M. Hultsch
  Version       :	03.00.03 / 26.01.2009 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	const char* ID	-	parameter identifier
					bool Data		-	value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_DataBase::SetData( const char* ID, bool Data )
{
	FEDM_CHK3(ID);

	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int iBack = 0;

	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );

	
	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM: // modifies RAM and EEPROM
		iBack = FEDM_Base::SetData(ID, Data, m_RFC_EEData, m_iRFC_DataBlockSize);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
		}
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, (__int64)Data );
		}
		break;
	
	case FEDM_RFC_RAMDATA_MEM:		
		iBack = FEDM_Base::SetData(ID, Data, m_RFC_RAMData, m_iRFC_DataBlockSize);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
		}
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, (__int64)Data );
		}
		break;
	
	case FEDM_ACC_EEDATA_MEM: // modifies RAM and EEPROM
		iBack = FEDM_Base::SetData(ID, Data, m_ACC_EEData, 32);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_ACC_EEData[BlockAdr * 32 + 31] |= 0x02;
		}
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, (__int64)Data );
		}
		break;
	
	case FEDM_ACC_RAMDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_ACC_RAMData, 32);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_ACC_RAMData[BlockAdr * 32 + 31] |= 0x02;
		}
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, (__int64)Data );
		}
		break;
	
	case FEDM_TMPDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_TmpData, 32);
		break;
	case FEDM_MJPDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_MjpData, 16);
		break;
	case FEDM_SN_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_SN_Mem, 16);
		break;
	case FEDM_ID_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_ID_Mem, 16);
		break;
	case FEDM_AC_MEM:			
		iBack = FEDM_Base::SetData(ID, Data, m_AC_Mem, 16);
		break;
	case FEDM_PUB_MEM:			
		iBack = FEDM_Base::SetData(ID, Data, m_PubMem, m_iPubMemBlockSize);
		break;
	case FEDM_SEC_MEM:			
		iBack = FEDM_Base::SetData(ID, Data, m_SecMem, 4);
		break;
	case FEDM_CONF_MEM:		
		iBack = FEDM_Base::SetData(ID, Data, m_ConfMem, 4);
		break;
	case FEDM_DATE_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_DateMem, 16);
		break;
	case FEDM_RFC_DATA_MASK_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_RFC_CfgProfileMask, m_iRFC_DataBlockSize);
		break;
	default:
		iBack = FEDM_ERROR_UNKNOWN_MEM_ID;
		break;
	}

	if(iBack < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(ID=" << ID << ", Data=" << Data << ") - " << GetErrorText(iBack));
	}
	return iBack;
}

/***************************************************************************
  Begin        	:	13.05.2000 / M. Hultsch
  Version       :	03.00.03 / 26.01.2009 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	const char* ID	-	parameter identifier
					BOOL Data		-	value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_DataBase::SetData( const char* ID, BOOL Data )
{
	FEDM_CHK3(ID);

	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int iBack = 0;

	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );
	
	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM: // modifies RAM and EEPROM
		iBack = FEDM_Base::SetData(ID, Data ? true : false, m_RFC_EEData, m_iRFC_DataBlockSize);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
		}
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, (__int64)Data );
		}
		break;
	
	case FEDM_RFC_RAMDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data ? true : false, m_RFC_RAMData, m_iRFC_DataBlockSize);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
		}
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, (__int64)Data );
		}
		break;
	
	case FEDM_ACC_EEDATA_MEM: // modifies RAM and EEPROM
		iBack = FEDM_Base::SetData(ID, Data ? true : false, m_ACC_EEData, 32);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_ACC_EEData[BlockAdr * 32 + 31] |= 0x02;
		}
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, (__int64)Data );
		}
		break;
	
	case FEDM_ACC_RAMDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data ? true : false, m_ACC_RAMData, 32);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_ACC_RAMData[BlockAdr * 32 + 31] |= 0x02;
		}
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, (__int64)Data );
		}
		break;
	
	case FEDM_TMPDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data ? true : false, m_TmpData, 32);
		break;
	case FEDM_MJPDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data ? true : false, m_MjpData, 16);
		break;
	case FEDM_SN_MEM:
		iBack = FEDM_Base::SetData(ID, Data ? true : false, m_SN_Mem, 16);
		break;
	case FEDM_ID_MEM:
		iBack = FEDM_Base::SetData(ID, Data ? true : false, m_ID_Mem, 16);
		break;
	case FEDM_AC_MEM:			
		iBack = FEDM_Base::SetData(ID, Data ? true : false, m_AC_Mem, 16);
		break;
	case FEDM_PUB_MEM:			
		iBack = FEDM_Base::SetData(ID, Data ? true : false, m_PubMem, m_iPubMemBlockSize);
		break;
	case FEDM_SEC_MEM:			
		iBack = FEDM_Base::SetData(ID, Data ? true : false, m_SecMem, 4);
		break;
	case FEDM_CONF_MEM:		
		iBack = FEDM_Base::SetData(ID, Data ? true : false, m_ConfMem, 4);
		break;
	case FEDM_DATE_MEM:
		iBack = FEDM_Base::SetData(ID, Data ? true : false, m_DateMem, 16);
		break;
	case FEDM_RFC_DATA_MASK_MEM:
		iBack = FEDM_Base::SetData(ID, Data ? true : false, m_RFC_CfgProfileMask, m_iRFC_DataBlockSize);
		break;
	default:
		iBack = FEDM_ERROR_UNKNOWN_MEM_ID;
		break;
	}

	if(iBack < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(ID=" << ID << ", Data=" << Data << ") - " << GetErrorText(iBack));
	}
	return iBack;
}
#endif

/***************************************************************************
  Begin        	:	13.05.2000 / M. Hultsch
  Version       :	03.00.03 / 26.01.2009 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	const char* ID	-	parameter identifier
					unsigned char Data		-	value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_DataBase::SetData( const char* ID, unsigned char Data )
{
	FEDM_CHK3(ID);

	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int iBack = 0;

	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );
	
	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM: // modifies RAM and EEPROM
		iBack = FEDM_Base::SetData(ID, Data, m_RFC_EEData, m_iRFC_DataBlockSize);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
		}
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, (__int64)Data );
		}
		break;
	
	case FEDM_RFC_RAMDATA_MEM:		
		iBack = FEDM_Base::SetData(ID, Data, m_RFC_RAMData, m_iRFC_DataBlockSize);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
		}
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, (__int64)Data );
		}
		break;
	
	case FEDM_ACC_EEDATA_MEM: // modifies RAM and EEPROM
		iBack = FEDM_Base::SetData(ID, Data, m_ACC_EEData, 32);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_ACC_EEData[BlockAdr * 32 + 31] |= 0x02;
		}
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, (__int64)Data );
		}
		break;
	
	case FEDM_ACC_RAMDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_ACC_RAMData, 32);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_ACC_RAMData[BlockAdr * 32 + 31] |= 0x02;
		}
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, (__int64)Data );
		}
		break;
	
	case FEDM_TMPDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_TmpData, 32);
		break;
	case FEDM_MJPDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_MjpData, 16);
		break;
	case FEDM_SN_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_SN_Mem, 16);
		break;
	case FEDM_ID_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_ID_Mem, 16);
		break;
	case FEDM_AC_MEM:			
		iBack = FEDM_Base::SetData(ID, Data, m_AC_Mem, 16);
		break;
	case FEDM_PUB_MEM:			
		iBack = FEDM_Base::SetData(ID, Data, m_PubMem, m_iPubMemBlockSize);
		break;
	case FEDM_SEC_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_SecMem, 4);
		break;
	case FEDM_CONF_MEM:		
		iBack = FEDM_Base::SetData(ID, Data, m_ConfMem, 4);
		break;
	case FEDM_DATE_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_DateMem, 16);
		break;
	case FEDM_RFC_DATA_MASK_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_RFC_CfgProfileMask, m_iRFC_DataBlockSize);
		break;
	default:
		iBack = FEDM_ERROR_UNKNOWN_MEM_ID;
		break;
	}

	if(iBack < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(ID=" << ID << ", Data=0x" << std::hex << std::uppercase << (int)Data << ") - " << GetErrorText(iBack));
	}
	return iBack;
}

/***************************************************************************
  Begin        	:	13.05.2000 / M. Hultsch
  Version       :	03.00.03 / 26.01.2009 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	const char* ID		-	parameter identifier
					unsigned char* Data	-	pointer to value
					int Cnt				-	length of Data

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_DataBase::SetData( const char* ID, unsigned char* Data, int Cnt )
{
	FEDM_CHK3(ID);
	FEDM_CHK3(Data);

	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int iBack = 0;
	int iLastIndex = 0;
	int iBlockAdr = 0;

	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );
	
	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM: // modifies RAM and EEPROM
/*		iBack = FEDM_Base::SetData(ID, Data, Cnt, m_RFC_RAMData, 16);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_RFC_RAMData[BlockAdr * 16 + 15] |= 0x02;
			iBlockAdr = BlockAdr;
			iLastIndex = BytePos + ByteCnt;
			while(iLastIndex > 16)
			{
				iBlockAdr++;
				m_RFC_RAMData[iBlockAdr * 16 + 15] |= 0x02;
				iLastIndex -= 16;
			}
		}*/
		iBack = FEDM_Base::SetData(ID, Data, Cnt, m_RFC_EEData, m_iRFC_DataBlockSize);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			iBlockAdr = BlockAdr;
			iLastIndex = BytePos + ByteCnt;
			while(iLastIndex > m_iRFC_DataBlockSize)
			{
				iBlockAdr++;
				m_RFC_EEData[iBlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				iLastIndex -= m_iRFC_DataBlockSize;
			}
		}
		
		break;
	
	case FEDM_RFC_RAMDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, Cnt, m_RFC_RAMData, m_iRFC_DataBlockSize);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			iLastIndex = BytePos + ByteCnt;
			while(iLastIndex > m_iRFC_DataBlockSize)
			{
				BlockAdr++;
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				iLastIndex -= m_iRFC_DataBlockSize;
			}
		}
		
		break;
	
	case FEDM_ACC_EEDATA_MEM: // modifies RAM and EEPROM
/*		iBack = FEDM_Base::SetData(ID, Data, Cnt, m_ACC_RAMData, 32);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_ACC_RAMData[BlockAdr * 32 + 31] |= 0x02;
			iBlockAdr = BlockAdr;
			iLastIndex = BytePos + ByteCnt;
			while(iLastIndex > 32)
			{
				iBlockAdr++;
				m_ACC_RAMData[iBlockAdr * 32 + 31] |= 0x02;
				iLastIndex -= 32;
			}
		}*/
		iBack = FEDM_Base::SetData(ID, Data, Cnt, m_ACC_EEData, 32);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_ACC_EEData[BlockAdr * 32 + 31] |= 0x02;
			iBlockAdr = BlockAdr;
			iLastIndex = BytePos + ByteCnt;
			while(iLastIndex > 32)
			{
				iBlockAdr++;
				m_ACC_EEData[iBlockAdr * 32 + 31] |= 0x02;
				iLastIndex -= 32;
			}
		}
		
		break;
	
	case FEDM_ACC_RAMDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, Cnt, m_ACC_RAMData, 32);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_ACC_RAMData[BlockAdr * 32 + 31] |= 0x02;
			iLastIndex = BytePos + ByteCnt;
			while(iLastIndex > 32)
			{
				BlockAdr++;
				m_ACC_RAMData[BlockAdr * 32 + 31] |= 0x02;
				iLastIndex -= 32;
			}
		}
		
		break;
	
	case FEDM_TMPDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, Cnt, m_TmpData, 32);
		break;
	case FEDM_MJPDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, Cnt, m_MjpData, 16);
		break;
	case FEDM_SN_MEM:
		iBack = FEDM_Base::SetData(ID, Data, Cnt, m_SN_Mem, 16);
		break;
	case FEDM_ID_MEM:
		iBack = FEDM_Base::SetData(ID, Data, Cnt, m_ID_Mem, 16);
		break;
	case FEDM_AC_MEM:			
		iBack = FEDM_Base::SetData(ID, Data, Cnt, m_AC_Mem, 16);
		break;
	case FEDM_PUB_MEM:
		iBack = FEDM_Base::SetData(ID, Data, Cnt, m_PubMem, m_iPubMemBlockSize);
		break;
	case FEDM_SEC_MEM:			
		iBack = FEDM_Base::SetData(ID, Data, Cnt, m_SecMem, 4);
		break;
	case FEDM_CONF_MEM:		
		iBack = FEDM_Base::SetData(ID, Data, Cnt, m_ConfMem, 4);
		break;
	case FEDM_DATE_MEM:
		iBack = FEDM_Base::SetData(ID, Data, Cnt, m_DateMem, 16);
		break;
	case FEDM_RFC_DATA_MASK_MEM:
		iBack = FEDM_Base::SetData(ID, Data, Cnt, m_RFC_CfgProfileMask, m_iRFC_DataBlockSize);
		break;
	default:
		iBack = FEDM_ERROR_UNKNOWN_MEM_ID;
		break;
	}

	if(iBack < 0)
	{
		string str;
		FEDM_ConvHexUCharToHexStr(Data, Cnt, str);
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(ID=" << ID << ", Data=" << str << ") - " << GetErrorText(iBack));
	}
	return iBack;
}

/***************************************************************************
  Begin        	:	13.05.2000 / M. Hultsch
  Version       :	03.00.03 / 26.01.2009 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	const char* ID		-	parameter identifier
					unsigned int Data	-	value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_DataBase::SetData( const char* ID, unsigned int Data )
{
	FEDM_CHK3(ID);

	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int iBack = 0;
	int iLastIndex = 0;
	int iBlockAdr = 0;

	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );
	
	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM: // modifies RAM and EEPROM
		iBack = FEDM_Base::SetData(ID, Data, m_RFC_EEData, m_iRFC_DataBlockSize);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			iBlockAdr = BlockAdr;
			iLastIndex = BytePos + ByteCnt;
			while(iLastIndex > m_iRFC_DataBlockSize)
			{
				iBlockAdr++;
				m_RFC_EEData[iBlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				iLastIndex -= m_iRFC_DataBlockSize;
			}
		}	
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, (__int64)Data );
		}
		break;
	
	case FEDM_RFC_RAMDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_RFC_RAMData, m_iRFC_DataBlockSize);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			iLastIndex = BytePos + ByteCnt;
			while(iLastIndex > m_iRFC_DataBlockSize)
			{
				BlockAdr++;
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				iLastIndex -= m_iRFC_DataBlockSize;
			}
		}
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, (__int64)Data );
		}
		break;
	
	case FEDM_ACC_EEDATA_MEM: // modifies RAM and EEPROM
		iBack = FEDM_Base::SetData(ID, Data, m_ACC_EEData, 32);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_ACC_EEData[BlockAdr * 32 + 31] |= 0x02;
			iBlockAdr = BlockAdr;
			iLastIndex = BytePos + ByteCnt;
			while(iLastIndex > 32)
			{
				iBlockAdr++;
				m_ACC_EEData[iBlockAdr * 32 + 31] |= 0x02;
				iLastIndex -= 32;
			}
		}
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, (__int64)Data );
		}
		break;
	
	case FEDM_ACC_RAMDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_ACC_RAMData, 32);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_ACC_RAMData[BlockAdr * 32 + 31] |= 0x02;
			iLastIndex = BytePos + ByteCnt;
			while(iLastIndex > 32)
			{
				BlockAdr++;
				m_ACC_RAMData[BlockAdr * 32 + 31] |= 0x02;
				iLastIndex -= 32;
			}
		}
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, (__int64)Data );
		}
		break;

	case FEDM_TMPDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_TmpData, 32);
		break;
	case FEDM_MJPDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_MjpData, 16);
		break;
	case FEDM_SN_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_SN_Mem, 16);
		break;
	case FEDM_ID_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_ID_Mem, 16);
		break;
	case FEDM_AC_MEM:			
		iBack = FEDM_Base::SetData(ID, Data, m_AC_Mem, 16);
		break;
	case FEDM_PUB_MEM:			
		iBack = FEDM_Base::SetData(ID, Data, m_PubMem, m_iPubMemBlockSize);
		break;
	case FEDM_SEC_MEM:			
		iBack = FEDM_Base::SetData(ID, Data, m_SecMem, 4);
		break;
	case FEDM_CONF_MEM:		
		iBack = FEDM_Base::SetData(ID, Data, m_ConfMem, 4);
		break;
	case FEDM_DATE_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_DateMem, 16);
		break;
	case FEDM_RFC_DATA_MASK_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_RFC_CfgProfileMask, m_iRFC_DataBlockSize);
		break;
	default:
		iBack = FEDM_ERROR_UNKNOWN_MEM_ID;
		break;
	}

	if(iBack < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(ID=" << ID << ", Data=" << Data << ") - " << GetErrorText(iBack));
	}
	return iBack;
}

/***************************************************************************
  Begin        	:	13.05.2000 / M. Hultsch
  Version       :	03.00.03 / 26.01.2009 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	const char* ID	-	parameter identifier
					__int64 Data	-	value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_DataBase::SetData( const char* ID, __int64 Data )
{
	FEDM_CHK3(ID);

	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int iBack = 0;
	int iLastIndex = 0;
	int iBlockAdr = 0;

	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );
		
	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM: // modifies RAM and EEPROM
		iBack = FEDM_Base::SetData(ID, Data, m_RFC_EEData, m_iRFC_DataBlockSize);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			iBlockAdr = BlockAdr;
			iLastIndex = BytePos + ByteCnt;
			while(iLastIndex > m_iRFC_DataBlockSize)
			{
				iBlockAdr++;
				m_RFC_EEData[iBlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				iLastIndex -= m_iRFC_DataBlockSize;
			}
		}
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, (__int64)Data );
		}
		break;
	
	case FEDM_RFC_RAMDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_RFC_RAMData, m_iRFC_DataBlockSize);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			iLastIndex = BytePos + ByteCnt;
			while(iLastIndex > m_iRFC_DataBlockSize)
			{
				BlockAdr++;
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				iLastIndex -= m_iRFC_DataBlockSize;
			}
		}
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, (__int64)Data );
		}
		break;
	
	case FEDM_ACC_EEDATA_MEM: // modifies RAM and EEPROM
		iBack = FEDM_Base::SetData(ID, Data, m_ACC_EEData, 32);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_ACC_EEData[BlockAdr * 32 + 31] |= 0x02;
			iBlockAdr = BlockAdr;
			iLastIndex = BytePos + ByteCnt;
			while(iLastIndex > 32)
			{
				iBlockAdr++;
				m_ACC_EEData[iBlockAdr * 32 + 31] |= 0x02;
				iLastIndex -= 32;
			}
		}
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, (__int64)Data );
		}
		break;
	
	case FEDM_ACC_RAMDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_ACC_RAMData, 32);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_ACC_RAMData[BlockAdr * 32 + 31] |= 0x02;
			iLastIndex = BytePos + ByteCnt;
			while(iLastIndex > 32)
			{
				BlockAdr++;
				m_ACC_RAMData[BlockAdr * 32 + 31] |= 0x02;
				iLastIndex -= 32;
			}
		}
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, (__int64)Data );
		}
		break;

	case FEDM_TMPDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_TmpData, 32);
		break;
	case FEDM_MJPDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_MjpData, 16);
		break;
	case FEDM_SN_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_SN_Mem, 16);
		break;
	case FEDM_ID_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_ID_Mem, 16);
		break;
	case FEDM_AC_MEM:			
		iBack = FEDM_Base::SetData(ID, Data, m_AC_Mem, 16);
		break;
	case FEDM_PUB_MEM:			
		iBack = FEDM_Base::SetData(ID, Data, m_PubMem, m_iPubMemBlockSize);
		break;
	case FEDM_SEC_MEM:			
		iBack = FEDM_Base::SetData(ID, Data, m_SecMem, 4);
		break;
	case FEDM_CONF_MEM:		
		iBack = FEDM_Base::SetData(ID, Data, m_ConfMem, 4);
		break;
	case FEDM_DATE_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_DateMem, 16);
		break;
	case FEDM_RFC_DATA_MASK_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_RFC_CfgProfileMask, m_iRFC_DataBlockSize);
		break;
	default:
		iBack = FEDM_ERROR_UNKNOWN_MEM_ID;
		break;
	}

	if(iBack < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(ID=" << ID << ", Data=" << Data << ") - " << GetErrorText(iBack));
	}
	return iBack;
}

/***************************************************************************
  Begin        	:	13.05.2000 / M. Hultsch
  Version       :	03.00.03 / 26.01.2009 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	const char* ID	-	parameter identifier
					CString Data	-	value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_DataBase::SetData( const char* ID, CString Data )
{
	FEDM_CHK3(ID);

	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int iBack = 0;
	int iLastIndex = 0;
	int iBlockAdr = 0;

	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );
	
	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM: // modifies RAM and EEPROM
		iBack = FEDM_Base::SetData(ID, Data, m_RFC_EEData, m_iRFC_DataBlockSize);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			iBlockAdr = BlockAdr;
			iLastIndex = BytePos + ByteCnt;
			while(iLastIndex > m_iRFC_DataBlockSize)
			{
				iBlockAdr++;
				m_RFC_EEData[iBlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				iLastIndex -= m_iRFC_DataBlockSize;
			}
		}
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, Data );
		}
		break;

	case FEDM_RFC_RAMDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_RFC_RAMData, m_iRFC_DataBlockSize);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			iLastIndex = BytePos + ByteCnt;
			while(iLastIndex > m_iRFC_DataBlockSize)
			{
				BlockAdr++;
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				iLastIndex -= m_iRFC_DataBlockSize;
			}
		}
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, Data );
		}
		break;

	case FEDM_ACC_EEDATA_MEM: // modifies RAM and EEPROM
		iBack = FEDM_Base::SetData(ID, Data, m_ACC_EEData, 32);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_ACC_EEData[BlockAdr * 32 + 31] |= 0x02;
			iBlockAdr = BlockAdr;
			iLastIndex = BytePos + ByteCnt;
			while(iLastIndex > 32)
			{
				iBlockAdr++;
				m_ACC_EEData[iBlockAdr * 32 + 31] |= 0x02;
				iLastIndex -= 32;
			}
		}
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, Data );
		}
		break;

	case FEDM_ACC_RAMDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_ACC_RAMData, 32);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_ACC_RAMData[BlockAdr * 32 + 31] |= 0x02;
			iLastIndex = BytePos + ByteCnt;
			while(iLastIndex > 32)
			{
				BlockAdr++;
				m_ACC_RAMData[BlockAdr * 32 + 31] |= 0x02;
				iLastIndex -= 32;
			}
		}
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, Data );
		}
		break;
	
	case FEDM_TMPDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_TmpData, 32);
		break;
	case FEDM_MJPDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_MjpData, 16);
		break;
	case FEDM_SN_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_SN_Mem, 16);
		break;
	case FEDM_ID_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_ID_Mem, 16);
		break;
	case FEDM_AC_MEM:			
		iBack = FEDM_Base::SetData(ID, Data, m_AC_Mem, 16);
		break;
	case FEDM_PUB_MEM:			
		iBack = FEDM_Base::SetData(ID, Data, m_PubMem, m_iPubMemBlockSize);
		break;
	case FEDM_SEC_MEM:			
		iBack = FEDM_Base::SetData(ID, Data, m_SecMem, 4);
		break;
	case FEDM_CONF_MEM:		
		iBack = FEDM_Base::SetData(ID, Data, m_ConfMem, 4);
		break;
	case FEDM_DATE_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_DateMem, 16);
		break;
	case FEDM_RFC_DATA_MASK_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_RFC_CfgProfileMask, m_iRFC_DataBlockSize);
		break;
	default:
		iBack = FEDM_ERROR_UNKNOWN_MEM_ID;
		break;
	}

	if(iBack < 0)
	{
#if !defined(_WIN32_WCE)
		string str = Data;
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(ID=" << ID << ", Data=" << str << ") - " << GetErrorText(iBack));
#endif
	}
	return iBack;
}
#endif

/***************************************************************************
  Begin        	:	03.07.2000 / M. Hultsch
  Version       :	03.00.03 / 26.01.2009 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	const char* ID	-	parameter identifier
					string Data		-	value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_DataBase::SetData( const char* ID, string Data )
{
	FEDM_CHK3(ID);

	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int iBack = 0;
	int iLastIndex = 0;
	int iBlockAdr = 0;

	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );
	
	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM: // modifies RAM and EEPROM
		iBack = FEDM_Base::SetData(ID, Data, m_RFC_EEData, m_iRFC_DataBlockSize);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			iBlockAdr = BlockAdr;
			iLastIndex = BytePos + ByteCnt;
			while(iLastIndex > m_iRFC_DataBlockSize)
			{
				iBlockAdr++;
				m_RFC_EEData[iBlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				iLastIndex -= m_iRFC_DataBlockSize;
			}
		}
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, Data );
		}
		break;

	case FEDM_RFC_RAMDATA_MEM:		
		iBack = FEDM_Base::SetData(ID, Data, m_RFC_RAMData, m_iRFC_DataBlockSize);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			iLastIndex = BytePos + ByteCnt;
			while(iLastIndex > m_iRFC_DataBlockSize)
			{
				BlockAdr++;
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				iLastIndex -= m_iRFC_DataBlockSize;
			}
		}
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, Data );
		}
		break;
	
	case FEDM_ACC_EEDATA_MEM: // modifies RAM and EEPROM
		iBack = FEDM_Base::SetData(ID, Data, m_ACC_EEData, 32);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_ACC_EEData[BlockAdr * 32 + 31] |= 0x02;
			iBlockAdr = BlockAdr;
			iLastIndex = BytePos + ByteCnt;
			while(iLastIndex > 32)
			{
				iBlockAdr++;
				m_ACC_EEData[iBlockAdr * 32 + 31] |= 0x02;
				iLastIndex -= 32;
			}
		}
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, Data );
		}
		break;
	
	case FEDM_ACC_RAMDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_ACC_RAMData, 32);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_ACC_RAMData[BlockAdr * 32 + 31] |= 0x02;
			iLastIndex = BytePos + ByteCnt;
			while(iLastIndex > 32)
			{
				BlockAdr++;
				m_ACC_RAMData[BlockAdr * 32 + 31] |= 0x02;
				iLastIndex -= 32;
			}
		}
		
		if(iBack >= 0)
		{
			OptionalSetDataAction( ID, Data );
		}
		break;
	
	case FEDM_TMPDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_TmpData, 32);
		break;
	case FEDM_MJPDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_MjpData, 16);
		break;
	case FEDM_SN_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_SN_Mem, 16);
		break;
	case FEDM_ID_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_ID_Mem, 16);
		break;
	case FEDM_AC_MEM:			
		iBack = FEDM_Base::SetData(ID, Data, m_AC_Mem, 16);
		break;
	case FEDM_PUB_MEM:			
		iBack = FEDM_Base::SetData(ID, Data, m_PubMem, m_iPubMemBlockSize);
		break;
	case FEDM_SEC_MEM:			
		iBack = FEDM_Base::SetData(ID, Data, m_SecMem, 4);
		break;
	case FEDM_CONF_MEM:		
		iBack = FEDM_Base::SetData(ID, Data, m_ConfMem, 4);
		break;
	case FEDM_DATE_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_DateMem, 16);
		break;
	case FEDM_RFC_DATA_MASK_MEM:
		iBack = FEDM_Base::SetData(ID, Data, m_RFC_CfgProfileMask, m_iRFC_DataBlockSize);
		break;
	default:
		iBack = FEDM_ERROR_UNKNOWN_MEM_ID;
		break;
	}

	if(iBack < 0)
	{
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(ID=" << ID << ", Data=" << Data << ") - " << GetErrorText(iBack));
	}
	return iBack;
}

/***************************************************************************
  Begin        	:	13.05.2000 / M. Hultsch
  Version       :	03.00.03 / 26.01.2009 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	const char* ID	-	parameter identifier
					char* Data		-	pointer to value
					int DataLen		-	length of Data

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_DataBase::SetData( const char* ID, char* Data, int DataLen )
{
	FEDM_CHK3(ID);
	FEDM_CHK3(Data);

	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int iBack = 0;
	int iLastIndex = 0;
	int iBlockAdr = 0;

	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );
	
	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM: // modifies RAM and EEPROM
		iBack = FEDM_Base::SetData(ID, Data, DataLen, m_RFC_EEData, m_iRFC_DataBlockSize);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			iBlockAdr = BlockAdr;
			iLastIndex = BytePos + ByteCnt;
			while(iLastIndex > m_iRFC_DataBlockSize)
			{
				iBlockAdr++;
				m_RFC_EEData[iBlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				iLastIndex -= m_iRFC_DataBlockSize;
			}
		}

		break;
	
	case FEDM_RFC_RAMDATA_MEM:		
		iBack = FEDM_Base::SetData(ID, Data, DataLen, m_RFC_RAMData, m_iRFC_DataBlockSize);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			iLastIndex = BytePos + ByteCnt;
			while(iLastIndex > m_iRFC_DataBlockSize)
			{
				BlockAdr++;
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				iLastIndex -= m_iRFC_DataBlockSize;
			}
		}

		break;

	case FEDM_ACC_EEDATA_MEM: // modifies RAM and EEPROM
		iBack = FEDM_Base::SetData(ID, Data, DataLen, m_ACC_EEData, 32);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_ACC_EEData[BlockAdr * 32 + 31] |= 0x02;
			iBlockAdr = BlockAdr;
			iLastIndex = BytePos + ByteCnt;
			while(iLastIndex > 32)
			{
				iBlockAdr++;
				m_ACC_EEData[iBlockAdr * 32 + 31] |= 0x02;
				iLastIndex -= 32;
			}
		}

		break;

	case FEDM_ACC_RAMDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, DataLen, m_ACC_RAMData, 32);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			m_ACC_RAMData[BlockAdr * 32 + 31] |= 0x02;
			iLastIndex = BytePos + ByteCnt;
			while(iLastIndex > 32)
			{
				BlockAdr++;
				m_ACC_RAMData[BlockAdr * 32 + 31] |= 0x02;
				iLastIndex -= 32;
			}
		}

		break;

	case FEDM_TMPDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, DataLen, m_TmpData, 32);
		break;
	case FEDM_MJPDATA_MEM:
		iBack = FEDM_Base::SetData(ID, Data, DataLen, m_MjpData, 16);
		break;
	case FEDM_SN_MEM:
		iBack = FEDM_Base::SetData(ID, Data, DataLen, m_SN_Mem, 16);
		break;
	case FEDM_ID_MEM:
		iBack = FEDM_Base::SetData(ID, Data, DataLen, m_ID_Mem, 16);
		break;
	case FEDM_AC_MEM:			
		iBack = FEDM_Base::SetData(ID, Data, DataLen, m_AC_Mem, 16);
		break;
	case FEDM_PUB_MEM:			
		iBack = FEDM_Base::SetData(ID, Data, DataLen, m_PubMem, m_iPubMemBlockSize);
		break;
	case FEDM_SEC_MEM:			
		iBack = FEDM_Base::SetData(ID, Data, DataLen, m_SecMem, 4);
		break;
	case FEDM_CONF_MEM:		
		iBack = FEDM_Base::SetData(ID, Data, DataLen, m_ConfMem, 4);
		break;
	case FEDM_DATE_MEM:
		iBack = FEDM_Base::SetData(ID, Data, DataLen, m_DateMem, 16);
		break;
	case FEDM_RFC_DATA_MASK_MEM:
		iBack = FEDM_Base::SetData(ID, Data, DataLen, m_RFC_CfgProfileMask, m_iRFC_DataBlockSize);
		break;
	default:
		iBack = FEDM_ERROR_UNKNOWN_MEM_ID;
		break;
	}

	if(iBack < 0)
	{
		Data[DataLen] = 0;
		FEDM_TRACE(m_pLogger, FELOGMNG_DBG_ERROR, __FUNCTION__ << "(ID=" << ID << ", Data=" << Data << ") - " << GetErrorText(iBack));
	}
	return iBack;
}



//#####################################################################################
// functions for direct access with address
//#####################################################################################

/***************************************************************************
  Begin        	:	19.06.2000 / M. Hultsch
  Version       :	03.00.00 / 02.04.2008 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	int Adr		-	address
					bool* Data	-	pointer to value
					int MemID	-	memory identifier

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_DataBase::GetData( int Adr, unsigned char* Data, int MemID )
{
	FEDM_CHK3(Data);

	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM:		
		return FEDM_Base::GetData(Adr, Data, m_RFC_EEData);
	case FEDM_RFC_RAMDATA_MEM:		
		return FEDM_Base::GetData(Adr, Data, m_RFC_RAMData);
	case FEDM_ACC_EEDATA_MEM:
		return FEDM_Base::GetData(Adr, Data, m_ACC_EEData);
	case FEDM_ACC_RAMDATA_MEM:
		return FEDM_Base::GetData(Adr, Data, m_ACC_RAMData);
	case FEDM_TMPDATA_MEM:
		return FEDM_Base::GetData(Adr, Data, m_TmpData);
	case FEDM_MJPDATA_MEM:
		return FEDM_Base::GetData(Adr, Data, m_MjpData);
	case FEDM_SN_MEM:
		return FEDM_Base::GetData(Adr, Data, m_SN_Mem);
	case FEDM_ID_MEM:
		return FEDM_Base::GetData(Adr, Data, m_ID_Mem);
	case FEDM_AC_MEM:			
		return FEDM_Base::GetData(Adr, Data, m_AC_Mem);
	case FEDM_PUB_MEM:			
		return FEDM_Base::GetData(Adr, Data, m_PubMem);
	case FEDM_SEC_MEM:			
		return FEDM_Base::GetData(Adr, Data, m_SecMem);
	case FEDM_CONF_MEM:		
		return FEDM_Base::GetData(Adr, Data, m_ConfMem);
	case FEDM_DATE_MEM:
		return FEDM_Base::GetData(Adr, Data, m_DateMem);
	case FEDM_RFC_DATA_MASK_MEM:
		return FEDM_Base::GetData(Adr, Data, m_RFC_CfgProfileMask);
	default:
		return FEDM_ERROR_UNKNOWN_MEM_ID;
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	19.06.2000 / M. Hultsch
  Version       :	03.00.00 / 02.04.2008 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	int Adr				-	address
					unsigned char* Data	-	pointer to value
					int Cnt				-	length of Data
					int MemID			-	memory identifier

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_DataBase::GetData( int Adr, unsigned char* Data, int Cnt, int MemID )
{
	FEDM_CHK3(Data);

	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM:		
		return FEDM_Base::GetData(Adr, Data, Cnt, m_RFC_EEData);
	case FEDM_RFC_RAMDATA_MEM:		
		return FEDM_Base::GetData(Adr, Data, Cnt, m_RFC_RAMData);
	case FEDM_ACC_EEDATA_MEM:
		return FEDM_Base::GetData(Adr, Data, Cnt, m_ACC_EEData);
	case FEDM_ACC_RAMDATA_MEM:
		return FEDM_Base::GetData(Adr, Data, Cnt, m_ACC_RAMData);
	case FEDM_TMPDATA_MEM:
		return FEDM_Base::GetData(Adr, Data, Cnt, m_TmpData);
	case FEDM_MJPDATA_MEM:
		return FEDM_Base::GetData(Adr, Data, Cnt, m_MjpData);
	case FEDM_SN_MEM:
		return FEDM_Base::GetData(Adr, Data, Cnt, m_SN_Mem);
	case FEDM_ID_MEM:
		return FEDM_Base::GetData(Adr, Data, Cnt, m_ID_Mem);
	case FEDM_AC_MEM:			
		return FEDM_Base::GetData(Adr, Data, Cnt, m_AC_Mem);
	case FEDM_PUB_MEM:			
		return FEDM_Base::GetData(Adr, Data, Cnt, m_PubMem);
	case FEDM_SEC_MEM:			
		return FEDM_Base::GetData(Adr, Data, Cnt, m_SecMem);
	case FEDM_CONF_MEM:
		return FEDM_Base::GetData(Adr, Data, Cnt, m_ConfMem);
	case FEDM_DATE_MEM:
		return FEDM_Base::GetData(Adr, Data, Cnt, m_DateMem);
	case FEDM_RFC_DATA_MASK_MEM:
		return FEDM_Base::GetData(Adr, Data, Cnt, m_RFC_CfgProfileMask);
	default:
		return FEDM_ERROR_UNKNOWN_MEM_ID;
	}

	return FEDM_OK;
}


/***************************************************************************
  Begin        	:	19.06.2000 / M. Hultsch
  Version       :	03.00.00 / 02.04.2008 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	int Adr				-	address
					unsigned int* Data	-	pointer to value
					int MemID			-	memory identifier

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_DataBase::GetData( int Adr, unsigned int* Data, int MemID )
{
	FEDM_CHK3(Data);

	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM:		
		return FEDM_Base::GetData(Adr, Data, m_RFC_EEData);
	case FEDM_RFC_RAMDATA_MEM:		
		return FEDM_Base::GetData(Adr, Data, m_RFC_RAMData);
	case FEDM_ACC_EEDATA_MEM:
		return FEDM_Base::GetData(Adr, Data, m_ACC_EEData);
	case FEDM_ACC_RAMDATA_MEM:
		return FEDM_Base::GetData(Adr, Data, m_ACC_RAMData);
	case FEDM_TMPDATA_MEM:
		return FEDM_Base::GetData(Adr, Data, m_TmpData);
	case FEDM_MJPDATA_MEM:
		return FEDM_Base::GetData(Adr, Data, m_MjpData);
	case FEDM_SN_MEM:
		return FEDM_Base::GetData(Adr, Data, m_SN_Mem);
	case FEDM_ID_MEM:
		return FEDM_Base::GetData(Adr, Data, m_ID_Mem);
	case FEDM_AC_MEM:			
		return FEDM_Base::GetData(Adr, Data, m_AC_Mem);
	case FEDM_PUB_MEM:			
		return FEDM_Base::GetData(Adr, Data, m_PubMem);
	case FEDM_SEC_MEM:			
		return FEDM_Base::GetData(Adr, Data, m_SecMem);
	case FEDM_CONF_MEM:		
		return FEDM_Base::GetData(Adr, Data, m_ConfMem);
	case FEDM_DATE_MEM:
		return FEDM_Base::GetData(Adr, Data, m_DateMem);
	case FEDM_RFC_DATA_MASK_MEM:
		return FEDM_Base::GetData(Adr, Data, m_RFC_CfgProfileMask);
	default:
		return FEDM_ERROR_UNKNOWN_MEM_ID;
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	19.06.2000 / M. Hultsch
  Version       :	03.00.00 / 02.04.2008 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	int Adr			-	address
					__int64* Data	-	pointer to value
					int MemID		-	memory identifier

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_DataBase::GetData( int Adr, __int64* Data, int MemID )
{
	FEDM_CHK3(Data);

	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM:		
		return FEDM_Base::GetData(Adr, Data, m_RFC_EEData);
	case FEDM_RFC_RAMDATA_MEM:		
		return FEDM_Base::GetData(Adr, Data, m_RFC_RAMData);
	case FEDM_ACC_EEDATA_MEM:
		return FEDM_Base::GetData(Adr, Data, m_ACC_EEData);
	case FEDM_ACC_RAMDATA_MEM:
		return FEDM_Base::GetData(Adr, Data, m_ACC_RAMData);
	case FEDM_TMPDATA_MEM:
		return FEDM_Base::GetData(Adr, Data, m_TmpData);
	case FEDM_MJPDATA_MEM:
		return FEDM_Base::GetData(Adr, Data, m_MjpData);
	case FEDM_SN_MEM:
		return FEDM_Base::GetData(Adr, Data, m_SN_Mem);
	case FEDM_ID_MEM:
		return FEDM_Base::GetData(Adr, Data, m_ID_Mem);
	case FEDM_AC_MEM:			
		return FEDM_Base::GetData(Adr, Data, m_AC_Mem);
	case FEDM_PUB_MEM:			
		return FEDM_Base::GetData(Adr, Data, m_PubMem);
	case FEDM_SEC_MEM:			
		return FEDM_Base::GetData(Adr, Data, m_SecMem);
	case FEDM_CONF_MEM:		
		return FEDM_Base::GetData(Adr, Data, m_ConfMem);
	case FEDM_DATE_MEM:
		return FEDM_Base::GetData(Adr, Data, m_DateMem);
	case FEDM_RFC_DATA_MASK_MEM:
		return FEDM_Base::GetData(Adr, Data, m_RFC_CfgProfileMask);
	default:
		return FEDM_ERROR_UNKNOWN_MEM_ID;
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	19.06.2000 / M. Hultsch
  Version       :	03.00.00 / 02.04.2008 / M. Hultsch

  Function		:	read a value from specified memory

  Parameter		:	int Adr			-	address
					CString& Data	-	reference of value
					int Cnt			-	length of Data
					int MemID		-	memory identifier

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_DataBase::GetData( int Adr, CString& Data, int Cnt, int MemID )
{
	FEDM_CHK3(&Data);

	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM:		
		return FEDM_Base::GetData(Adr, Data, Cnt, m_RFC_EEData);
	case FEDM_RFC_RAMDATA_MEM:		
		return FEDM_Base::GetData(Adr, Data, Cnt, m_RFC_RAMData);
	case FEDM_ACC_EEDATA_MEM:
		return FEDM_Base::GetData(Adr, Data, Cnt, m_ACC_EEData);
	case FEDM_ACC_RAMDATA_MEM:
		return FEDM_Base::GetData(Adr, Data, Cnt, m_ACC_RAMData);
	case FEDM_TMPDATA_MEM:
		return FEDM_Base::GetData(Adr, Data, Cnt, m_TmpData);
	case FEDM_MJPDATA_MEM:
		return FEDM_Base::GetData(Adr, Data, Cnt, m_MjpData);
	case FEDM_SN_MEM:
		return FEDM_Base::GetData(Adr, Data, Cnt, m_SN_Mem);
	case FEDM_ID_MEM:
		return FEDM_Base::GetData(Adr, Data, Cnt, m_ID_Mem);
	case FEDM_AC_MEM:			
		return FEDM_Base::GetData(Adr, Data, Cnt, m_AC_Mem);
	case FEDM_PUB_MEM:			
		return FEDM_Base::GetData(Adr, Data, Cnt, m_PubMem);
	case FEDM_SEC_MEM:			
		return FEDM_Base::GetData(Adr, Data, Cnt, m_SecMem);
	case FEDM_CONF_MEM:		
		return FEDM_Base::GetData(Adr, Data, Cnt, m_ConfMem);
	case FEDM_DATE_MEM:
		return FEDM_Base::GetData(Adr, Data, Cnt, m_DateMem);
	case FEDM_RFC_DATA_MASK_MEM:
		return FEDM_Base::GetData(Adr, Data, Cnt, m_RFC_CfgProfileMask);
	default:
		return FEDM_ERROR_UNKNOWN_MEM_ID;
	}

	return FEDM_OK;
}
#endif

/***************************************************************************
  Begin        	:	13.06.2000 / M. Hultsch
  Version       :	03.00.03 / 26.01.2009 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	int Adr				-	address
					unsigned char Data	-	value
					int MemID			-	memory identifier

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_DataBase::SetData( int Adr, unsigned char Data, int MemID )
{
	int iBack = 0;
	int BlockAdr = 0;

	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM: // modifies RAM and EEPROM
		iBack = FEDM_Base::SetData(Adr, Data, m_RFC_EEData);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			BlockAdr = Adr >> 4;
			m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
		}
		return iBack;
	
	case FEDM_RFC_RAMDATA_MEM:		
		iBack = FEDM_Base::SetData(Adr, Data, m_RFC_RAMData);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			BlockAdr = Adr >> 4;
			m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
		}
		return iBack;
	
	case FEDM_ACC_EEDATA_MEM: // modifies RAM and EEPROM
		iBack = FEDM_Base::SetData(Adr, Data, m_ACC_EEData);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			BlockAdr = Adr >> 5;
			m_ACC_EEData[BlockAdr * 32 + 31] |= 0x02;
		}
		return iBack;
	
	case FEDM_ACC_RAMDATA_MEM:
		iBack = FEDM_Base::SetData(Adr, Data, m_ACC_RAMData);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			BlockAdr = Adr >> 5;
			m_ACC_RAMData[BlockAdr * 32 + 31] |= 0x02;
		}
		return iBack;
	
	case FEDM_TMPDATA_MEM:
		return FEDM_Base::SetData(Adr, Data, m_TmpData);
	case FEDM_MJPDATA_MEM:
		return FEDM_Base::SetData(Adr, Data, m_MjpData);
	case FEDM_SN_MEM:
		return FEDM_Base::SetData(Adr, Data, m_SN_Mem);
	case FEDM_ID_MEM:
		return FEDM_Base::SetData(Adr, Data, m_ID_Mem);
	case FEDM_AC_MEM:			
		return FEDM_Base::SetData(Adr, Data, m_AC_Mem);
	case FEDM_PUB_MEM:			
		return FEDM_Base::SetData(Adr, Data, m_PubMem);
	case FEDM_SEC_MEM:			
		return FEDM_Base::SetData(Adr, Data, m_SecMem);
	case FEDM_CONF_MEM:		
		return FEDM_Base::SetData(Adr, Data, m_ConfMem);
	case FEDM_DATE_MEM:
		return FEDM_Base::SetData(Adr, Data, m_DateMem);
	case FEDM_RFC_DATA_MASK_MEM:
		return FEDM_Base::SetData(Adr, Data, m_RFC_CfgProfileMask);
	default:
		return FEDM_ERROR_UNKNOWN_MEM_ID;
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	13.06.2000 / M. Hultsch
  Version       :	03.00.03 / 26.01.2009 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	int Adr				-	address
					unsigned char* Data	-	pointer of value
					int Cnt				-	length of Data
					int MemID			-	memory identifier

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_DataBase::SetData( int Adr, unsigned char* Data, int Cnt, int MemID )
{
	FEDM_CHK3(Data);

	int iBack = 0;
	int BlockAdr = 0;
	int iCnt = Cnt;

	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM: // modifies RAM and EEPROM
		iBack = FEDM_Base::SetData(Adr, Data, Cnt, m_RFC_EEData);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			BlockAdr = Adr >> 4;
			iCnt = Cnt;
			m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			while(iCnt > m_iRFC_DataBlockSize)
			{
				BlockAdr++;
				m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				iCnt -= m_iRFC_DataBlockSize;
			}
		}
		return iBack;
	
	case FEDM_RFC_RAMDATA_MEM:
		iBack = FEDM_Base::SetData(Adr, Data, Cnt, m_RFC_RAMData);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			BlockAdr = Adr >> 4;
			m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			while(Cnt > m_iRFC_DataBlockSize)
			{
				BlockAdr++;
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				Cnt -= m_iRFC_DataBlockSize;
			}
		}
		return iBack;
	
	case FEDM_ACC_EEDATA_MEM: // modifies RAM and EEPROM
		iBack = FEDM_Base::SetData(Adr, Data, Cnt, m_ACC_EEData);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			BlockAdr = Adr >> 5;
			iCnt = Cnt;
			m_ACC_EEData[BlockAdr * 32 + 31] |= 0x02;
			while(Cnt > 32)
			{
				BlockAdr++;
				m_ACC_EEData[BlockAdr * 32 + 31] |= 0x02;
				Cnt -= 32;
			}
		}
		return iBack;
	
	case FEDM_ACC_RAMDATA_MEM:
		iBack = FEDM_Base::SetData(Adr, Data, Cnt, m_ACC_RAMData);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			BlockAdr = Adr >> 5;
			m_ACC_RAMData[BlockAdr * 32 + 31] |= 0x02;
			while(Cnt > 32)
			{
				BlockAdr++;
				m_ACC_RAMData[BlockAdr * 32 + 31] |= 0x02;
				Cnt -= 32;
			}
		}
		return iBack;
	
	case FEDM_TMPDATA_MEM:
		return FEDM_Base::SetData(Adr, Data, Cnt, m_TmpData);
	case FEDM_MJPDATA_MEM:
		return FEDM_Base::SetData(Adr, Data, Cnt, m_MjpData);
	case FEDM_SN_MEM:
		return FEDM_Base::SetData(Adr, Data, Cnt, m_SN_Mem);
	case FEDM_ID_MEM:
		return FEDM_Base::SetData(Adr, Data, Cnt, m_ID_Mem);
	case FEDM_AC_MEM:			
		return FEDM_Base::SetData(Adr, Data, Cnt, m_AC_Mem);
	case FEDM_PUB_MEM:			
		return FEDM_Base::SetData(Adr, Data, Cnt, m_PubMem);
	case FEDM_SEC_MEM:			
		return FEDM_Base::SetData(Adr, Data, Cnt, m_SecMem);
	case FEDM_CONF_MEM:		
		return FEDM_Base::SetData(Adr, Data, Cnt, m_ConfMem);
	case FEDM_DATE_MEM:
		return FEDM_Base::SetData(Adr, Data, Cnt, m_DateMem);
	case FEDM_RFC_DATA_MASK_MEM:
		return FEDM_Base::SetData(Adr, Data, Cnt, m_RFC_CfgProfileMask);
	default:
		return FEDM_ERROR_UNKNOWN_MEM_ID;
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	13.06.2000 / M. Hultsch
  Version       :	03.00.03 / 26.01.2009 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	int Adr				-	address
					unsigned int Data	-	value
					int MemID			-	memory identifier

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_DataBase::SetData( int Adr, unsigned int Data, int MemID )
{
	int iBack = 0;
	int BlockAdr = 0;

	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM: // modifies RAM and EEPROM
		iBack = FEDM_Base::SetData(Adr, Data, m_RFC_EEData);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			BlockAdr = Adr >> 4;
			m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			if( BlockAdr < ((Adr + 4) >> 4))
			{
				BlockAdr++;
				m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		return iBack;

	case FEDM_RFC_RAMDATA_MEM:		
		iBack = FEDM_Base::SetData(Adr, Data, m_RFC_RAMData);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			BlockAdr = Adr >> 4;
			m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			if( BlockAdr < ((Adr + 4) >> 4))
			{
				BlockAdr++;
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		return iBack;

	case FEDM_ACC_EEDATA_MEM: // modifies RAM and EEPROM
		iBack = FEDM_Base::SetData(Adr, Data, m_ACC_EEData);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			BlockAdr = Adr >> 5;
			m_ACC_EEData[BlockAdr * 32 + 31] |= 0x02;
			if( BlockAdr < ((Adr + 4) >> 5))
			{
				BlockAdr++;
				m_ACC_EEData[BlockAdr * 32 + 31] |= 0x02;
			}
		}
		return iBack;

	case FEDM_ACC_RAMDATA_MEM:
		iBack = FEDM_Base::SetData(Adr, Data, m_ACC_RAMData);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			BlockAdr = Adr >> 5;
			m_ACC_RAMData[BlockAdr * 32 + 31] |= 0x02;
			if( BlockAdr < ((Adr + 4) >> 5))
			{
				BlockAdr++;
				m_ACC_RAMData[BlockAdr * 32 + 31] |= 0x02;
			}
		}
		return iBack;

	case FEDM_TMPDATA_MEM:
		return FEDM_Base::SetData(Adr, Data, m_TmpData);
	case FEDM_MJPDATA_MEM:
		return FEDM_Base::SetData(Adr, Data, m_MjpData);
	case FEDM_SN_MEM:
		return FEDM_Base::SetData(Adr, Data, m_SN_Mem);
	case FEDM_ID_MEM:
		return FEDM_Base::SetData(Adr, Data, m_ID_Mem);
	case FEDM_AC_MEM:			
		return FEDM_Base::SetData(Adr, Data, m_AC_Mem);
	case FEDM_PUB_MEM:			
		return FEDM_Base::SetData(Adr, Data, m_PubMem);
	case FEDM_SEC_MEM:			
		return FEDM_Base::SetData(Adr, Data, m_SecMem);
	case FEDM_CONF_MEM:		
		return FEDM_Base::SetData(Adr, Data, m_ConfMem);
	case FEDM_DATE_MEM:
		return FEDM_Base::SetData(Adr, Data, m_DateMem);
	case FEDM_RFC_DATA_MASK_MEM:
		return FEDM_Base::SetData(Adr, Data, m_RFC_CfgProfileMask);
	default:
		return FEDM_ERROR_UNKNOWN_MEM_ID;
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	13.06.2000 / M. Hultsch
  Version       :	03.00.03 / 26.01.2009 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	int Adr			-	address
					__int64 Data	-	value
					int MemID		-	memory identifier

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
int FEDM_DataBase::SetData( int Adr, __int64 Data, int MemID )
{
	int iBack = 0;
	int BlockAdr = 0;

	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM: // modifies RAM and EEPROM
		iBack =  FEDM_Base::SetData(Adr, Data, m_RFC_EEData);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			BlockAdr = Adr >> 4;
			m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			if( BlockAdr < ((Adr + 8) >> 4))
			{
				BlockAdr++;
				m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		return iBack;

	case FEDM_RFC_RAMDATA_MEM:
		iBack = FEDM_Base::SetData(Adr, Data, m_RFC_RAMData);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			BlockAdr = Adr >> 4;
			m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			if( BlockAdr < ((Adr + 8) >> 4))
			{
				BlockAdr++;
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		return iBack;

	case FEDM_ACC_EEDATA_MEM: // modifies RAM and EEPROM
		iBack = FEDM_Base::SetData(Adr, Data, m_ACC_EEData);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			BlockAdr = Adr >> 5;
			m_ACC_EEData[BlockAdr * 32 + 31] |= 0x02;
			if( BlockAdr < ((Adr + 8) >> 5))
			{
				BlockAdr++;
				m_ACC_EEData[BlockAdr * 32 + 31] |= 0x02;
			}
		}
		return iBack;

	case FEDM_ACC_RAMDATA_MEM:
		iBack = FEDM_Base::SetData(Adr, Data, m_ACC_RAMData);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			BlockAdr = Adr >> 5;
			m_ACC_RAMData[BlockAdr * 32 + 31] |= 0x02;
			if( BlockAdr < ((Adr + 8) >> 5))
			{
				BlockAdr++;
				m_ACC_RAMData[BlockAdr * 32 + 31] |= 0x02;
			}
		}
		return iBack;

	case FEDM_TMPDATA_MEM:
		return FEDM_Base::SetData(Adr, Data, m_TmpData);
	case FEDM_MJPDATA_MEM:
		return FEDM_Base::SetData(Adr, Data, m_MjpData);
	case FEDM_SN_MEM:
		return FEDM_Base::SetData(Adr, Data, m_SN_Mem);
	case FEDM_ID_MEM:
		return FEDM_Base::SetData(Adr, Data, m_ID_Mem);
	case FEDM_AC_MEM:			
		return FEDM_Base::SetData(Adr, Data, m_AC_Mem);
	case FEDM_PUB_MEM:			
		return FEDM_Base::SetData(Adr, Data, m_PubMem);
	case FEDM_SEC_MEM:			
		return FEDM_Base::SetData(Adr, Data, m_SecMem);
	case FEDM_CONF_MEM:		
		return FEDM_Base::SetData(Adr, Data, m_ConfMem);
	case FEDM_DATE_MEM:
		return FEDM_Base::SetData(Adr, Data, m_DateMem);
	case FEDM_RFC_DATA_MASK_MEM:
		return FEDM_Base::SetData(Adr, Data, m_RFC_CfgProfileMask);
	default:
		return FEDM_ERROR_UNKNOWN_MEM_ID;
	}

	return FEDM_OK;
}

/***************************************************************************
  Begin        	:	13.06.2000 / M. Hultsch
  Version       :	03.00.03 / 26.01.2009 / M. Hultsch

  Function		:	write a value in specified memory

  Parameter		:	int Adr			-	address
					CString Data	-	value
					int MemID		-	memory identifier

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
int FEDM_DataBase::SetData( int Adr, CString Data, int MemID )
{
	int iBack = 0;
	int BlockAdr = 0;
	int iLen = 0;

	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM: // modifies RAM and EEPROM
		iBack = FEDM_Base::SetData(Adr, Data, m_RFC_EEData);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			BlockAdr = Adr >> 4;
#if defined(_FEDM_MFC_SUPPORT)
			iLen = (Data.GetLength() >> 2);
#endif
#if defined(__BORLANDC__)
			iLen = (Data.Length() >> 2);
#endif
			m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			while(iLen > m_iRFC_DataBlockSize)
			{
				BlockAdr++;
				m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				iLen -= m_iRFC_DataBlockSize;
			}
		}
		return iBack;

	case FEDM_RFC_RAMDATA_MEM:
		iBack = FEDM_Base::SetData(Adr, Data, m_RFC_RAMData);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			BlockAdr = Adr >> 4;
#if defined(_FEDM_MFC_SUPPORT)
			iLen = (Data.GetLength() >> 2);
#endif
#if defined(__BORLANDC__)
			iLen = (Data.Length() >> 2);
#endif
			m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			while(iLen > m_iRFC_DataBlockSize)
			{
				BlockAdr++;
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				iLen -= m_iRFC_DataBlockSize;
			}
		}
		return iBack;

	case FEDM_ACC_EEDATA_MEM: // modifies RAM and EEPROM
		iBack = FEDM_Base::SetData(Adr, Data, m_ACC_EEData);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			BlockAdr = Adr >> 5;
#if defined(_FEDM_MFC_SUPPORT)
			iLen = (Data.GetLength() >> 2);
#endif
#if defined(__BORLANDC__)
			iLen = (Data.Length() >> 2);
#endif
			m_ACC_EEData[BlockAdr * 32 + 31] |= 0x02;
			while(iLen > 32)
			{
				BlockAdr++;
				m_ACC_EEData[BlockAdr * 32 + 31] |= 0x02;
				iLen -= 32;
			}
		}
		return iBack;

	case FEDM_ACC_RAMDATA_MEM:
		iBack = FEDM_Base::SetData(Adr, Data, m_ACC_RAMData);
		if(iBack == FEDM_MODIFIED)
		{
			// mark block as modified
			BlockAdr = Adr >> 5;
#if defined(_FEDM_MFC_SUPPORT)
			iLen = (Data.GetLength() >> 2);
#endif
#if defined(__BORLANDC__)
			iLen = (Data.Length() >> 2);
#endif
			m_ACC_RAMData[BlockAdr * 32 + 31] |= 0x02;
			while(iLen > 32)
			{
				BlockAdr++;
				m_ACC_RAMData[BlockAdr * 32 + 31] |= 0x02;
				iLen -= 32;
			}
		}
		return iBack;

	case FEDM_TMPDATA_MEM:
		return FEDM_Base::SetData(Adr, Data, m_TmpData);
	case FEDM_MJPDATA_MEM:
		return FEDM_Base::SetData(Adr, Data, m_MjpData);
	case FEDM_SN_MEM:
		return FEDM_Base::SetData(Adr, Data, m_SN_Mem);
	case FEDM_ID_MEM:
		return FEDM_Base::SetData(Adr, Data, m_ID_Mem);
	case FEDM_AC_MEM:			
		return FEDM_Base::SetData(Adr, Data, m_AC_Mem);
	case FEDM_PUB_MEM:			
		return FEDM_Base::SetData(Adr, Data, m_PubMem);
	case FEDM_SEC_MEM:			
		return FEDM_Base::SetData(Adr, Data, m_SecMem);
	case FEDM_CONF_MEM:		
		return FEDM_Base::SetData(Adr, Data, m_ConfMem);
	case FEDM_DATE_MEM:
		return FEDM_Base::SetData(Adr, Data, m_DateMem);
	case FEDM_RFC_DATA_MASK_MEM:
		return FEDM_Base::SetData(Adr, Data, m_RFC_CfgProfileMask);
	default:
		return FEDM_ERROR_UNKNOWN_MEM_ID;
	}

	return FEDM_OK;
}
#endif


/***************************************************************************
  Begin        	:	08.08.2008 / M. Hultsch
  Version       :	04.06.08 / 22.10.2014 / M. Hultsch
					- extended for ID ISC.PRH200

					04.06.02 / 08.05.2012 / M. Hultsch
					- extended for parameters:
					  AirInterface.Multiplexer.UHF.Internal.SelectedAntennas
					  AirInterface.Multiplexer.UHF.Internal.NoOfAntennas

					04.03.00 / 20.08.2012 / M. Hultsch

  Function		:	write additional value in specified memory
					NOTE: actually support only for boolean parameters

  Parameter		:	const char* ID	-	parameter identifier
					__int64 Data	-	value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
void FEDM_DataBase::OptionalSetDataAction( const char* ID, __int64 Data )
{
	bool bPara1 = false;
	bool bPara2 = false;
	bool bPara3 = false;
	bool bPara4 = false;
	bool bPara5 = false;
	unsigned char ucPara = 0;
	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int iBack = 0;

	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );

	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM: // modifies RAM and EEPROM
		if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==0 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.InputEvents
			(m_uiReaderType==FEDM_ISC_TYPE_ISCPRH200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMR200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_A ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMRU200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_MAXU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_EEData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==1 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.MacAddress
			(m_uiReaderType==FEDM_ISC_TYPE_CPR50_XX ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMR200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_A ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMRU200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_MAXU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_EEData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==2 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.AFI
			(m_uiReaderType==FEDM_ISC_TYPE_ISCLR2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_A ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B) )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_EEData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==4 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.RSSI
			(m_uiReaderType==FEDM_ISC_TYPE_ISCPRHD102 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCPRH200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMR200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_A ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMRU102 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMRU200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_MAXU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AntennaExtended
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_EEData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==5 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.Direction
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AntennaExtended
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_EEData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==2 && ByteCnt==1 && BitPos==2 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.DistanceMeasurement
			(m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 11 02 01 02 01", &bPara1, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.DistanceMeasurement
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData3
			iBack = FEDM_Base::SetData("03 03 11 01 01 07 01", bPara1, m_RFC_EEData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==15 && BytePos==1 && ByteCnt==1 && BitPos==3 && BitCnt==4 && // AirInterface.Multiplexer.UHF.Internal.SelectedAntennas
			(m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 15 01 01 03 04", &ucPara, m_RFC_EEData, m_iRFC_DataBlockSize);// AirInterface.Multiplexer.UHF.Internal.SelectedAntennas
			
			if(ucPara > 0)
			{
				// set also: AirInterface.Multiplexer.UHF.Internal.SelectionMode
				iBack = FEDM_Base::SetData("03 03 15 01 01 07 01", true, m_RFC_EEData, m_iRFC_DataBlockSize);
				if(iBack == FEDM_MODIFIED)
				{
					// mark block as modified
					m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				}
			}
			else
			{
				// reset also: AirInterface.Multiplexer.UHF.Internal.SelectionMode
				iBack = FEDM_Base::SetData("03 03 15 01 01 07 01", false, m_RFC_EEData, m_iRFC_DataBlockSize);
				if(iBack == FEDM_MODIFIED)
				{
					// mark block as modified
					m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				}
			}
		}
		else if( BlockAdr==15 && BytePos==1 && ByteCnt==1 && BitPos==0 && BitCnt==3 && // AirInterface.Multiplexer.UHF.Internal.NoOfAntennas
			(m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 15 01 01 00 03", &ucPara, m_RFC_EEData, m_iRFC_DataBlockSize);// AirInterface.Multiplexer.UHF.Internal.NoOfAntennas
			
			if(ucPara > 0)
			{
				// set also: AirInterface.Multiplexer.UHF.Internal.SelectionMode
				iBack = FEDM_Base::SetData("03 03 15 01 01 07 01", false, m_RFC_EEData, m_iRFC_DataBlockSize);
				if(iBack == FEDM_MODIFIED)
				{
					// mark block as modified
					m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				}
			}
		}
		break;
	
	case FEDM_RFC_RAMDATA_MEM:
		if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==0 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.InputEvents
			(m_uiReaderType==FEDM_ISC_TYPE_ISCPRH200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMR200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_A ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMRU200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_MAXU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_RAMData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==1 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.MacAddress
			(m_uiReaderType==FEDM_ISC_TYPE_CPR50_XX ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMR200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_A ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMRU200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_MAXU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_RAMData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==2 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.AFI
			(m_uiReaderType==FEDM_ISC_TYPE_ISCLR2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_A ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B) )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_RAMData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==4 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.RSSI
			(m_uiReaderType==FEDM_ISC_TYPE_ISCPRHD102 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCPRH200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMR200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_A ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMRU102 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMRU200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_MAXU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_RAMData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==5 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.Direction
			m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_RAMData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==2 && ByteCnt==1 && BitPos==2 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.DistanceMeasurement
			(m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 11 02 01 02 01", &bPara1, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.DistanceMeasurement
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData3
			iBack = FEDM_Base::SetData("03 03 11 01 01 07 01", bPara1, m_RFC_RAMData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==15 && BytePos==1 && ByteCnt==1 && BitPos==3 && BitCnt==4 && // AirInterface.Multiplexer.UHF.Internal.SelectedAntennas
			(m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 15 01 01 03 04", &ucPara, m_RFC_RAMData, m_iRFC_DataBlockSize);// AirInterface.Multiplexer.UHF.Internal.SelectedAntennas
			
			if(ucPara > 0)
			{
				// set also: AirInterface.Multiplexer.UHF.Internal.SelectionMode
				iBack = FEDM_Base::SetData("03 03 15 01 01 07 01", true, m_RFC_RAMData, m_iRFC_DataBlockSize);
				if(iBack == FEDM_MODIFIED)
				{
					// mark block as modified
					m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				}
			}
			else
			{
				// reset also: AirInterface.Multiplexer.UHF.Internal.SelectionMode
				iBack = FEDM_Base::SetData("03 03 15 01 01 07 01", false, m_RFC_RAMData, m_iRFC_DataBlockSize);
				if(iBack == FEDM_MODIFIED)
				{
					// mark block as modified
					m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				}
			}
		}
		else if( BlockAdr==15 && BytePos==1 && ByteCnt==1 && BitPos==0 && BitCnt==3 && // AirInterface.Multiplexer.UHF.Internal.NoOfAntennas
			(m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 15 01 01 00 03", &ucPara, m_RFC_RAMData, m_iRFC_DataBlockSize);// AirInterface.Multiplexer.UHF.Internal.NoOfAntennas
			
			if(ucPara > 0)
			{
				// set also: AirInterface.Multiplexer.UHF.Internal.SelectionMode
				iBack = FEDM_Base::SetData("03 03 15 01 01 07 01", false, m_RFC_RAMData, m_iRFC_DataBlockSize);
				if(iBack == FEDM_MODIFIED)
				{
					// mark block as modified
					m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				}
			}
		}
		break;
	}
}

/***************************************************************************
  Begin        	:	08.08.2008 / M. Hultsch
  Version       :	04.06.08 / 22.10.2014 / M. Hultsch
					- extended for ID ISC.PRH200

					04.06.02 / 08.05.2012 / M. Hultsch
					- extended for parameters:
					  AirInterface.Multiplexer.UHF.Internal.SelectedAntennas
					  AirInterface.Multiplexer.UHF.Internal.NoOfAntennas

					04.03.00 / 20.08.2012 / M. Hultsch

  Function		:	write additional value in specified memory
					NOTE: actually support only for boolean parameters

  Parameter		:	const char* ID	-	parameter identifier
					string Data		-	value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
void FEDM_DataBase::OptionalSetDataAction( const char* ID, string Data )
{
	bool bPara1 = false;
	bool bPara2 = false;
	bool bPara3 = false;
	bool bPara4 = false;
	bool bPara5 = false;
	unsigned char ucPara = 0;
	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int iBack = 0;

	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );

	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM: // modifies RAM and EEPROM
		if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==0 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.InputEvents
			(m_uiReaderType==FEDM_ISC_TYPE_ISCPRH200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMR200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_A ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMRU200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_MAXU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_EEData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==1 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.MacAddress
			(m_uiReaderType==FEDM_ISC_TYPE_CPR50_XX ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMR200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_A ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMRU200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_MAXU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_EEData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==2 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.AFI
			(m_uiReaderType==FEDM_ISC_TYPE_ISCLR2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_A ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B) )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_EEData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==4 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.RSSI
			(m_uiReaderType==FEDM_ISC_TYPE_ISCPRHD102 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCPRH200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMR200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_A ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMRU102 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMRU200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_MAXU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_EEData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==5 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.Direction
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_EEData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==2 && ByteCnt==1 && BitPos==2 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.DistanceMeasurement
			(m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 11 02 01 02 01", &bPara1, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.DistanceMeasurement
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData3
			iBack = FEDM_Base::SetData("03 03 11 01 01 07 01", (bool)(bPara1), m_RFC_EEData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==15 && BytePos==1 && ByteCnt==1 && BitPos==3 && BitCnt==4 && // AirInterface.Multiplexer.UHF.Internal.SelectedAntennas
			(m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 15 01 01 03 04", &ucPara, m_RFC_EEData, m_iRFC_DataBlockSize);// AirInterface.Multiplexer.UHF.Internal.SelectedAntennas
			
			if(ucPara > 0)
			{
				// set also: AirInterface.Multiplexer.UHF.Internal.SelectionMode
				iBack = FEDM_Base::SetData("03 03 15 01 01 07 01", true, m_RFC_EEData, m_iRFC_DataBlockSize);
				if(iBack == FEDM_MODIFIED)
				{
					// mark block as modified
					m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				}
			}
			else
			{
				// reset also: AirInterface.Multiplexer.UHF.Internal.SelectionMode
				iBack = FEDM_Base::SetData("03 03 15 01 01 07 01", false, m_RFC_EEData, m_iRFC_DataBlockSize);
				if(iBack == FEDM_MODIFIED)
				{
					// mark block as modified
					m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				}
			}
		}
		else if( BlockAdr==15 && BytePos==1 && ByteCnt==1 && BitPos==0 && BitCnt==3 && // AirInterface.Multiplexer.UHF.Internal.NoOfAntennas
			(m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 15 01 01 00 03", &ucPara, m_RFC_EEData, m_iRFC_DataBlockSize);// AirInterface.Multiplexer.UHF.Internal.NoOfAntennas
			
			if(ucPara > 0)
			{
				// set also: AirInterface.Multiplexer.UHF.Internal.SelectionMode
				iBack = FEDM_Base::SetData("03 03 15 01 01 07 01", false, m_RFC_EEData, m_iRFC_DataBlockSize);
				if(iBack == FEDM_MODIFIED)
				{
					// mark block as modified
					m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				}
			}
		}
		break;
	
	case FEDM_RFC_RAMDATA_MEM:
		if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==0 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.InputEvents
			(m_uiReaderType==FEDM_ISC_TYPE_ISCPRH200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMR200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_A ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMRU200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_MAXU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_RAMData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==1 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.MacAddress
			(m_uiReaderType==FEDM_ISC_TYPE_CPR50_XX ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMR200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_A ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMRU200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_MAXU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_RAMData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==2 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.AFI
			(m_uiReaderType==FEDM_ISC_TYPE_ISCLR2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_A ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B) )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_RAMData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==4 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.RSSI
			(m_uiReaderType==FEDM_ISC_TYPE_ISCPRHD102 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCPRH200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMR200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_A ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMRU102 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMRU200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_MAXU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_RAMData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==5 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.Direction
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_RAMData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==2 && ByteCnt==1 && BitPos==2 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.DistanceMeasurement
			(m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 11 02 01 02 01", &bPara1, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.DistanceMeasurement
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData3
			iBack = FEDM_Base::SetData("03 03 11 01 01 07 01", (bool)(bPara1), m_RFC_RAMData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==15 && BytePos==1 && ByteCnt==1 && BitPos==3 && BitCnt==4 && // AirInterface.Multiplexer.UHF.Internal.SelectedAntennas
			(m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 15 01 01 03 04", &ucPara, m_RFC_RAMData, m_iRFC_DataBlockSize);// AirInterface.Multiplexer.UHF.Internal.SelectedAntennas
			
			if(ucPara > 0)
			{
				// set also: AirInterface.Multiplexer.UHF.Internal.SelectionMode
				iBack = FEDM_Base::SetData("03 03 15 01 01 07 01", true, m_RFC_RAMData, m_iRFC_DataBlockSize);
				if(iBack == FEDM_MODIFIED)
				{
					// mark block as modified
					m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				}
			}
			else
			{
				// reset also: AirInterface.Multiplexer.UHF.Internal.SelectionMode
				iBack = FEDM_Base::SetData("03 03 15 01 01 07 01", false, m_RFC_RAMData, m_iRFC_DataBlockSize);
				if(iBack == FEDM_MODIFIED)
				{
					// mark block as modified
					m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				}
			}
		}
		else if( BlockAdr==15 && BytePos==1 && ByteCnt==1 && BitPos==0 && BitCnt==3 && // AirInterface.Multiplexer.UHF.Internal.NoOfAntennas
			(m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 15 01 01 00 03", &ucPara, m_RFC_RAMData, m_iRFC_DataBlockSize);// AirInterface.Multiplexer.UHF.Internal.NoOfAntennas
			
			if(ucPara > 0)
			{
				// set also: AirInterface.Multiplexer.UHF.Internal.SelectionMode
				iBack = FEDM_Base::SetData("03 03 15 01 01 07 01", false, m_RFC_RAMData, m_iRFC_DataBlockSize);
				if(iBack == FEDM_MODIFIED)
				{
					// mark block as modified
					m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				}
			}
		}
		break;
	}
}

/***************************************************************************
  Begin        	:	08.08.2008 / M. Hultsch
  Version       :	04.06.08 / 22.10.2014 / M. Hultsch
					- extended for ID ISC.PRH200

					04.06.02 / 08.05.2012 / M. Hultsch
					- extended for parameters:
					  AirInterface.Multiplexer.UHF.Internal.SelectedAntennas
					  AirInterface.Multiplexer.UHF.Internal.NoOfAntennas

					04.03.00 / 20.08.2012 / M. Hultsch
					NOTE: actually support only for boolean parameters

  Function		:	write additional value in specified memory

  Parameter		:	const char* ID	-	parameter identifier
					CString Data	-	value

  Return Value	:	FEDM_OK (0) or error code (<0)
***************************************************************************/
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
void FEDM_DataBase::OptionalSetDataAction( const char* ID, CString Data )
{
	bool bPara1 = false;
	bool bPara2 = false;
	bool bPara3 = false;
	bool bPara4 = false;
	bool bPara5 = false;
	unsigned char ucPara = 0;
	int FamID=0, MemID=0, BlockAdr=0, BytePos=0, ByteCnt=0, BitPos=0, BitCnt=0;
	int iBack = 0;

	// extract access parameters
	sscanf( ID, "%d %d %d %d %d %d %d",
			&FamID, &MemID, &BlockAdr, &BytePos, &ByteCnt, &BitPos, &BitCnt );

	switch( MemID )
	{
	case FEDM_RFC_EEDATA_MEM: // modifies RAM and EEPROM
		if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==0 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.InputEvents
			(m_uiReaderType==FEDM_ISC_TYPE_ISCPRH200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMR200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_A ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMRU200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_MAXU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_EEData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==1 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.MacAddress
			(m_uiReaderType==FEDM_ISC_TYPE_CPR50_XX ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMR200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_A ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMRU200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_MAXU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_EEData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==2 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.AFI
			(m_uiReaderType==FEDM_ISC_TYPE_ISCLR2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_A ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B) )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_EEData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==4 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.RSSI
			(m_uiReaderType==FEDM_ISC_TYPE_ISCPRHD102 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCPRH200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMR200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_A ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMRU102 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMRU200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_MAXU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_EEData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==5 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.Direction
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_EEData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==2 && ByteCnt==1 && BitPos==2 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.DistanceMeasurement
			(m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 11 02 01 02 01", &bPara1, m_RFC_EEData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.DistanceMeasurement
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData3
			iBack = FEDM_Base::SetData("03 03 11 01 01 07 01", (bool)(bPara1), m_RFC_EEData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==15 && BytePos==1 && ByteCnt==1 && BitPos==3 && BitCnt==4 && // AirInterface.Multiplexer.UHF.Internal.SelectedAntennas
			(m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 15 01 01 03 04", &ucPara, m_RFC_EEData, m_iRFC_DataBlockSize);// AirInterface.Multiplexer.UHF.Internal.SelectedAntennas
			
			if(ucPara > 0)
			{
				// set also: AirInterface.Multiplexer.UHF.Internal.SelectionMode
				iBack = FEDM_Base::SetData("03 03 15 01 01 07 01", true, m_RFC_EEData, m_iRFC_DataBlockSize);
				if(iBack == FEDM_MODIFIED)
				{
					// mark block as modified
					m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				}
			}
			else
			{
				// reset also: AirInterface.Multiplexer.UHF.Internal.SelectionMode
				iBack = FEDM_Base::SetData("03 03 15 01 01 07 01", false, m_RFC_EEData, m_iRFC_DataBlockSize);
				if(iBack == FEDM_MODIFIED)
				{
					// mark block as modified
					m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				}
			}
		}
		else if( BlockAdr==15 && BytePos==1 && ByteCnt==1 && BitPos==0 && BitCnt==3 && // AirInterface.Multiplexer.UHF.Internal.NoOfAntennas
			(m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 15 01 01 00 03", &ucPara, m_RFC_EEData, m_iRFC_DataBlockSize);// AirInterface.Multiplexer.UHF.Internal.NoOfAntennas
			
			if(ucPara > 0)
			{
				// set also: AirInterface.Multiplexer.UHF.Internal.SelectionMode
				iBack = FEDM_Base::SetData("03 03 15 01 01 07 01", false, m_RFC_EEData, m_iRFC_DataBlockSize);
				if(iBack == FEDM_MODIFIED)
				{
					// mark block as modified
					m_RFC_EEData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				}
			}
		}
		break;
	
	case FEDM_RFC_RAMDATA_MEM:
		if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==0 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.InputEvents
			(m_uiReaderType==FEDM_ISC_TYPE_ISCPRH200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMR200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_A ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMRU200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_MAXU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_RAMData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==1 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.MacAddress
			(m_uiReaderType==FEDM_ISC_TYPE_CPR50_XX ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMR200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_A ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMRU200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_MAXU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_RAMData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==2 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.AFI
			(m_uiReaderType==FEDM_ISC_TYPE_ISCLR2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_A ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B) )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_RAMData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==4 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.RSSI
			(m_uiReaderType==FEDM_ISC_TYPE_ISCPRHD102 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCPRH200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMR200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_A ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMRU102 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCMRU200 ||
			 m_uiReaderType==FEDM_ISC_TYPE_MAXU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU1002 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU2000 ||
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_RAMData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==1 && ByteCnt==1 && BitPos==5 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.Direction
			 m_uiReaderType==FEDM_ISC_TYPE_ISCLR2500_B )
		{
			FEDM_Base::GetData("03 03 11 01 01 00 01", &bPara1, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.InputEvents
			FEDM_Base::GetData("03 03 11 01 01 01 01", &bPara2, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.MacAddress
			FEDM_Base::GetData("03 03 11 01 01 02 01", &bPara3, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.AFI
			FEDM_Base::GetData("03 03 11 01 01 04 01", &bPara4, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.RSSI
			FEDM_Base::GetData("03 03 11 01 01 05 01", &bPara5, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.Direction
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData2
			iBack = FEDM_Base::SetData("03 03 11 00 01 07 01", (bool)(bPara1 | bPara2 | bPara3 | bPara4 | bPara5), m_RFC_RAMData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==11 && BytePos==2 && ByteCnt==1 && BitPos==2 && BitCnt==1 && // OperatingMode.AutoRead.DataSelector.DistanceMeasurement
			(m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 11 02 01 02 01", &bPara1, m_RFC_RAMData, m_iRFC_DataBlockSize);// OperatingMode.AutoRead.DataSelector.DistanceMeasurement
			
			// set also: OperatingMode.AutoRead.DataSelector.ExtensionFlag_TrData3
			iBack = FEDM_Base::SetData("03 03 11 01 01 07 01", (bool)(bPara1), m_RFC_RAMData, m_iRFC_DataBlockSize);
			if(iBack == FEDM_MODIFIED)
			{
				// mark block as modified
				m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
			}
		}
		else if( BlockAdr==15 && BytePos==1 && ByteCnt==1 && BitPos==3 && BitCnt==4 && // AirInterface.Multiplexer.UHF.Internal.SelectedAntennas
			(m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 15 01 01 03 04", &ucPara, m_RFC_RAMData, m_iRFC_DataBlockSize);// AirInterface.Multiplexer.UHF.Internal.SelectedAntennas
			
			if(ucPara > 0)
			{
				// set also: AirInterface.Multiplexer.UHF.Internal.SelectionMode
				iBack = FEDM_Base::SetData("03 03 15 01 01 07 01", true, m_RFC_RAMData, m_iRFC_DataBlockSize);
				if(iBack == FEDM_MODIFIED)
				{
					// mark block as modified
					m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				}
			}
			else
			{
				// reset also: AirInterface.Multiplexer.UHF.Internal.SelectionMode
				iBack = FEDM_Base::SetData("03 03 15 01 01 07 01", false, m_RFC_RAMData, m_iRFC_DataBlockSize);
				if(iBack == FEDM_MODIFIED)
				{
					// mark block as modified
					m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				}
			}
		}
		else if( BlockAdr==15 && BytePos==1 && ByteCnt==1 && BitPos==0 && BitCnt==3 && // AirInterface.Multiplexer.UHF.Internal.NoOfAntennas
			(m_uiReaderType==FEDM_ISC_TYPE_ISCLRU3000) )
		{
			FEDM_Base::GetData("03 03 15 01 01 00 03", &ucPara, m_RFC_RAMData, m_iRFC_DataBlockSize);// AirInterface.Multiplexer.UHF.Internal.NoOfAntennas
			
			if(ucPara > 0)
			{
				// set also: AirInterface.Multiplexer.UHF.Internal.SelectionMode
				iBack = FEDM_Base::SetData("03 03 15 01 01 07 01", false, m_RFC_RAMData, m_iRFC_DataBlockSize);
				if(iBack == FEDM_MODIFIED)
				{
					// mark block as modified
					m_RFC_RAMData[BlockAdr * m_iRFC_DataBlockSize + m_iRFC_DataBlockSize-1] |= 0x02;
				}
			}
		}
		break;
	}
}
#endif
