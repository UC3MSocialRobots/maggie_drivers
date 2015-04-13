/*-------------------------------------------------------
|                                                       |
|	FedmIscMyAxxess_XmlParser.cpp			            |
|                                                       |
---------------------------------------------------------

Copyright © 2009		FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	25.06.2009
Version       		:	03.00.08 / 29.06.2009 / M. Hultsch
Operation Systems	:	independent
Function			:	parser for myAxxess Data Files

OBID® and OBID i-scan® are registered Trademarks of FEIG ELECTRONIC GmbH.
Linux® is a registered trademark of Linus Torvalds.
Microsoft® and Windows® are registered trademarks of Microsoft Corporation.
*/

#if defined(_FEDM_MYAXXESS_XML_SUPPORT)

//#############################
//      I N C L U D E S
//#############################
#include "FedmIscMyAxxess_XmlParser.h"		// public include file of library
#include "FEDM_XmlParser_IWriter.h"			// public include file of library
#include "FEDM_XmlParser_Types.h"
#include "FedmIscCore.h"
#include "FedmIscMyAxxess_Xml.h"


//#######################################################################################
//	class FedmIscMyAxxess_XmlParser
//#######################################################################################
FedmIscMyAxxess_XmlParser::FedmIscMyAxxess_XmlParser(FedmIscMyAxxessReader* pAxxessReader)
: FEDM_XmlParser_Base(FEDM_XML_PARSER_TYPE_FEDM, FEDM_XML_PARSER_TYPE_FEDM)
{
	m_pAxxessReader = pAxxessReader;

	SetComment();
}


FedmIscMyAxxess_XmlParser::~FedmIscMyAxxess_XmlParser(void)
{
}

//#####################################################################################
// serialization functions
//#####################################################################################

/***************************************************************************
  Begin        	:	25.06.2009 / M. Hultsch
  Version       :	03.00.08 / 25.06.2009 / M. Hultsch

  Function		:	read xml document and save content in data map

  Parameters	:	char* szXmlFileName	- pointer to buffer with filename
					char* szAccessCode	- pointer to password
					unsigned int uiAccessCodeLen	- length of password

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FedmIscMyAxxess_XmlParser::SerializeIn(	char* szXmlFileName, 
											char* szAccessCode, 
											unsigned int uiAccessCodeLen)
{
	if(strlen(szXmlFileName) == 0)
		return FEDM_ERROR_STRING_LENGTH;

	int	iBack = 0;
	unsigned int uiCrc = 0;
	FEDM_XML_TREEITEM* root			= NULL;


	m_XmlHeader.Init();


	iBack = OpenDoc(szXmlFileName, "r");
	if(iBack)
		FEDM_RETURN(iBack);
	
	// read complete xml document into string
	iBack = LoadDoc();
	if(iBack)
		FEDM_RETURN(iBack);


	// parser in read mode
	m_bWriteMode = false;
	m_bIsParsed = false;


	// #####################################################
	// root tag
	root = FindTag(FEDM_XML_TAG_OBID_UC, (unsigned int)0);
	if(root == NULL)
	{
		return m_iLastError;
	}
	AddTagItem(NULL, root);

	// #####################################################
	// header tag
	FEDM_CHK2(iBack, SerializeHeaderIn(root));

	if(strcmp(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_AT_DOC_TYPE) == 0)
	{
		iBack = SerializeIn_AccessTable(root);
	}
	else if(strcmp(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_TT_DOC_TYPE) == 0)
	{
		iBack = SerializeIn_TimezoneTable(root);
	}
	else if(strcmp(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_HT_DOC_TYPE) == 0)
	{
		iBack = SerializeIn_HolidayTable(root);
	}
	else if(strcmp(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_ET_DOC_TYPE) == 0)
	{
		iBack = SerializeIn_EventTable(root);
	}
	else
	{
		DeleteXmlTree(); // clean up
		FEDM_RETURN(FEDM_XML_ERROR_DOC_FILE_TYPE);
	}

	if(iBack)
	{
		DeleteXmlTree(); // clean up
		FEDM_RETURN(iBack);
	}


	// clean up
	DeleteXmlTree();

	
	// calculate checksum
	if(strcmp(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_AT_DOC_TYPE) == 0)
	{
		uiCrc = CalcChecksum_AccessTable();
	}
	else if(strcmp(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_TT_DOC_TYPE) == 0)
	{
		uiCrc = CalcChecksum_TimezoneTable();
	}
	else if(strcmp(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_HT_DOC_TYPE) == 0)
	{
		uiCrc = CalcChecksum_HolidayTable();
	}
	else if(strcmp(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_ET_DOC_TYPE) == 0)
	{
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
	}

	if(uiAccessCodeLen > 0)
	{
		uiCrc = CalcCRC16CheckSum(	uiCrc, 
									(unsigned char*)szAccessCode, 
									uiAccessCodeLen );
	}

	// verify checksum
	if(uiCrc != m_XmlHeader.uiCrc)
	{
		if(strcmp(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_AT_DOC_TYPE) == 0)
		{
			m_pAxxessReader->ClearTable(FEDM_MYAXXESS_ACCESS_TABLE);
		}
		else if(strcmp(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_TT_DOC_TYPE) == 0)
		{
			m_pAxxessReader->ClearTable(FEDM_MYAXXESS_TIMEZONE_TABLE);
		}
		else if(strcmp(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_HT_DOC_TYPE) == 0)
		{
			m_pAxxessReader->ClearTable(FEDM_MYAXXESS_HOLIDAY_TABLE);
		}
		else if(strcmp(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_ET_DOC_TYPE) == 0)
		{
			FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
		}

		FEDM_RETURN(FEDM_ERROR_CRC);
	}
	
	m_bIsParsed = true;

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        	:	25.06.2009 / M. Hultsch
  Version       :	03.00.08 / 26.06.2009 / M. Hultsch

  Function		:	write content of data map into xml document

  Parameters	:	unsigned int uiTableID	- table identifier from FedmIscMyAxxessReader.h
					char* szXmlFileName	- pointer to buffer with filename
					char* szAccessCode	- pointer to password
					unsigned int uiAccessCodeLen	- length of password

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FedmIscMyAxxess_XmlParser::SerializeOut(unsigned int uiTableID,
											char* szXmlFileName,
											char* szAccessCode, 
											unsigned int uiAccessCodeLen)
{
	if(strlen(szXmlFileName) == 0)
		return FEDM_ERROR_STRING_LENGTH;

	wchar_t wcTmp[128];
	int	iBack = 0;
	int	iTmp[2];
	unsigned int uiCrc = FEDM_CRC16_PRESET;
	FEDM_XML_TREEITEM* root = NULL;


	// parser in write mode
	m_bWriteMode = true;

	//TODO_M
	// header settings
	if(strcmp(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_AT_DOC_TYPE) == 0)
	{
		strcpy(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_AT_DOC_TYPE);
	}
	else if(strcmp(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_TT_DOC_TYPE) == 0)
	{
		strcpy(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_TT_DOC_TYPE);
	}
	else if(strcmp(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_HT_DOC_TYPE) == 0)
	{
		strcpy(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_HT_DOC_TYPE);
	}
	else if(strcmp(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_ET_DOC_TYPE) == 0)
	{
		strcpy(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_ET_DOC_TYPE);
	}
	else
	{
		FEDM_RETURN(FEDM_XML_ERROR_DOC_FILE_TYPE);
	}


	sscanf(FEDM_XML_STRING_MAX_DOC_VERSION, "%d.%d", &iTmp[1], &iTmp[0]);
	m_XmlHeader.uiDocVersion = (((unsigned char)iTmp[1]) << 8) + (unsigned char)iTmp[0];


	// root tag
	root = BuildTag(FEDM_XML_TAG_OBID_UC);
	if(root == NULL)
	{
		return m_iLastError;
	}
	AddTagItem(NULL, root);


	// #####################################################
	// header tag
	FEDM_CHK2(iBack, SerializeHeaderOut(root));


	if(strcmp(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_AT_DOC_TYPE) == 0)
	{
		iBack = SerializeOut_AccessTable(root);
	}
	else if(strcmp(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_TT_DOC_TYPE) == 0)
	{
		iBack = SerializeOut_TimezoneTable(root);
	}
	else if(strcmp(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_HT_DOC_TYPE) == 0)
	{
		iBack = SerializeOut_HolidayTable(root);
	}
	else if(strcmp(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_ET_DOC_TYPE) == 0)
	{
		iBack = SerializeOut_EventTable(root, uiCrc);
	}
	else
	{
		DeleteXmlTree(); // clean up
		FEDM_RETURN(FEDM_XML_ERROR_DOC_FILE_TYPE);
	}

	if(iBack)
	{
		DeleteXmlTree(); // clean up
		FEDM_RETURN(iBack);
	}


	// calculate checksum
	if(strcmp(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_AT_DOC_TYPE) == 0)
	{
		m_XmlHeader.uiCrc = CalcChecksum_AccessTable();
	}
	else if(strcmp(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_TT_DOC_TYPE) == 0)
	{
		m_XmlHeader.uiCrc = CalcChecksum_TimezoneTable();
	}
	else if(strcmp(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_HT_DOC_TYPE) == 0)
	{
		m_XmlHeader.uiCrc = CalcChecksum_HolidayTable();
	}
	else if(strcmp(m_XmlHeader.szDocType, FEDM_XML_STRING_MAX_ET_DOC_TYPE) == 0)
	{
		// EventTable is a queue. Thus, crc is calculated together with serialization process
		m_XmlHeader.uiCrc = uiCrc;
	}

	if(uiAccessCodeLen > 0)
	{
		m_XmlHeader.uiCrc = CalcCRC16CheckSum(	m_XmlHeader.uiCrc, 
												(unsigned char*)szAccessCode, 
												uiAccessCodeLen );
	}
#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
	swprintf(wcTmp, L"%d", m_XmlHeader.uiCrc);
	#else
	swprintf(wcTmp, 128, L"%d", m_XmlHeader.uiCrc);
	#endif
#else
	swprintf(wcTmp, 128, L"%d", m_XmlHeader.uiCrc);
#endif
	root->aChildList[0]->aChildList[7]->sValue = wcTmp;


	// #####################################################

	iBack = OpenDoc(szXmlFileName, "w");
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

	DeleteXmlTree(); // clean up

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	25.06.2009 / M. Hultsch
  Version       :	03.00.08 / 25.06.2009 / M. Hultsch

  Function		:	read xml document for doc type access table

  Parameters	:	FEDM_XML_TREEITEM* root	- pointer to parent xml element

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FedmIscMyAxxess_XmlParser::SerializeIn_AccessTable(FEDM_XML_TREEITEM* root)
{
	char cValue[256];
	unsigned char ucCnt = 0;
	unsigned int uiBit = 0;
	unsigned int uiValue = 0;
	unsigned __int64 ui64Value = 0;
	FEDM_XML_TREEITEM* access_table	= NULL;
	FEDM_XML_TREEITEM* access_data	= NULL;
	FEDM_XML_TREEITEM* timezones	= NULL;
	FEDM_XML_TREEITEM* child		= NULL;

	FEDM_ISC_MYAXXESS_METADATA				metadata;
	FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM		item;

	metadata.Init();
	item.Init();


	// delete content of access table
	m_pAxxessReader->ClearTable(FEDM_MYAXXESS_ACCESS_TABLE);


	// #####################################################
	//	<access-table>
	//		...
	//	</access-table>
	
	// <access-table>
	child = FindTag(FEDM_XML_TAG_ACCESS_TABLE_UC, (unsigned int)1, true);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	AddTagItem(root, child);
	access_table = child;

	// <idd-format>
	child = FindTag(FEDM_XML_TAG_IDD_FORMAT_UC, (unsigned int)2, true);
	if(child != NULL)
	{
		AddTagItem(access_table, child);
		FEDM_SWSCANF(child->sValue, cValue);
		sscanf(cValue, "%u", &metadata.ucIDDFormat);
		switch(metadata.ucIDDFormat)
		{
		case FEDM_MYAXXESS_IDD_FORMAT_ASCII:
		case FEDM_MYAXXESS_IDD_FORMAT_HEX:
		case FEDM_MYAXXESS_IDD_FORMAT_NUM:
			break;
		default:
			DeleteXmlTree(); // clean up
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
	}
	else
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}

	// <idd-length>
	child = FindTag(FEDM_XML_TAG_IDD_LENGTH_UC, (unsigned int)2, true);
	if(child != NULL)
	{
		AddTagItem(access_table, child);
		FEDM_SWSCANF(child->sValue, cValue);
		sscanf(cValue, "%u", &metadata.ucIDDLength);
		switch(metadata.ucIDDFormat)
		{
		case FEDM_MYAXXESS_IDD_FORMAT_ASCII:
		case FEDM_MYAXXESS_IDD_FORMAT_HEX:
			if(metadata.ucIDDLength < 1 || metadata.ucIDDLength > 64)
			{
				DeleteXmlTree(); // clean up
				FEDM_RETURN(FEDM_ERROR_PARAMETER);
			}
			break;

		case FEDM_MYAXXESS_IDD_FORMAT_NUM:
			if(metadata.ucIDDLength < 1 || metadata.ucIDDLength > 8)
			{
				DeleteXmlTree(); // clean up
				FEDM_RETURN(FEDM_ERROR_PARAMETER);
			}
			break;
		}
	}
	else
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}


	// <access-data>
	child = FindTag(FEDM_XML_TAG_ACCESS_DATA_UC, (unsigned int)2, true);
	while(child)
	{
		item.Init();

		AddTagItem(access_table, child);
		access_data = child;

		// <idd>
		child = FindTag(FEDM_XML_TAG_IDD_UC, (unsigned int)3, true);
		if(child != NULL)
		{
			AddTagItem(access_data, child);
			FEDM_SWSCANF(child->sValue, cValue);
			switch(metadata.ucIDDFormat)
			{
			case FEDM_MYAXXESS_IDD_FORMAT_ASCII:
				if(strlen(cValue) != metadata.ucIDDLength)
				{
					DeleteXmlTree(); // clean up
					m_pAxxessReader->ClearTable(FEDM_MYAXXESS_ACCESS_TABLE);
					FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
				}

				memcpy(item.ucIDD, cValue, metadata.ucIDDLength);
				break;

			case FEDM_MYAXXESS_IDD_FORMAT_NUM:
				if(strlen(cValue) > 20)
				{
					DeleteXmlTree(); // clean up
					m_pAxxessReader->ClearTable(FEDM_MYAXXESS_ACCESS_TABLE);
					FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
				}

				sscanf(cValue, "%lld", &ui64Value);
				for(ucCnt=0; ucCnt<metadata.ucIDDLength; ucCnt++)
				{
					// change intel in motorola format
					item.ucIDD[metadata.ucIDDLength-ucCnt-1] = *( ((unsigned char*)&ui64Value) + ucCnt );
				}
				if ((ucCnt < 8) && (*( ((unsigned char*)&ui64Value) + ucCnt ) != 0))
				{
					DeleteXmlTree(); // clean up
					m_pAxxessReader->ClearTable(FEDM_MYAXXESS_ACCESS_TABLE);
					FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
				}

				// set all not used byte to zero
				memset(&item.ucIDD[metadata.ucIDDLength], 0, 128-metadata.ucIDDLength);
				break;

			case FEDM_MYAXXESS_IDD_FORMAT_HEX:
				if(strlen(cValue) != (metadata.ucIDDLength<<1))
				{
					DeleteXmlTree(); // clean up
					m_pAxxessReader->ClearTable(FEDM_MYAXXESS_ACCESS_TABLE);
					FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
				}

				FEDM_ConvHexCharToHexUChar(cValue, (metadata.ucIDDLength<<1), item.ucIDD, 128);
				break;
			}
		}

		// <timezones>
		child = FindTag(FEDM_XML_TAG_TIMEZONES_UC, (unsigned int)3, true);
		if(child != NULL)
		{
			AddTagItem(access_data, child);
			timezones = child;

			// <timezone>
			child = FindTag(FEDM_XML_TAG_TIMEZONE_UC, (unsigned int)4, true);
			while(child)
			{
				AddTagItem(timezones, child);
				AddTagItem(access_data, child);
				FEDM_SWSCANF(child->sValue, cValue);
				sscanf(cValue, "%u", &uiValue);
				if(uiValue >= 1 && uiValue <= 16)
				{
					uiBit = 1;
					uiBit <<= (uiValue-1);
					item.uiTimezones |= uiBit;
				}
				else
				{
					DeleteXmlTree(); // clean up
					FEDM_RETURN(FEDM_ERROR_PARAMETER);
				}

				child = FindTag(FEDM_XML_TAG_TIMEZONE_UC, (unsigned int)4, true);
			}
		}

		// <reserved> - ignored

		// save new record in table
		FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM* pItem = new FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM;
		memcpy(pItem, &item, sizeof(FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM));
		m_pAxxessReader->m_AccessTable.push_back(pItem);

		child = FindTag(FEDM_XML_TAG_ACCESS_DATA_UC, (unsigned int)2, true);
	}

	// at least: update metadata
	m_pAxxessReader->SetIDDFormat(metadata.ucIDDFormat);
	m_pAxxessReader->SetIDDLength(metadata.ucIDDLength);

	// #####################################################

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	25.06.2009 / M. Hultsch
  Version       :	03.00.08 / 25.06.2009 / M. Hultsch

  Function		:	read xml document for doc type holiday table

  Parameters	:	FEDM_XML_TREEITEM* root	- pointer to parent xml element

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FedmIscMyAxxess_XmlParser::SerializeIn_HolidayTable(FEDM_XML_TREEITEM* root)
{
	char cValue[128];
	unsigned int uiYear = 0;
	unsigned int uiMonth = 0;
	unsigned int uiDay = 0;
	FEDM_XML_TREEITEM* table	= NULL;
	FEDM_XML_TREEITEM* child	= NULL;

	FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM*	pItem = NULL;


	// delete content of holiday table
	m_pAxxessReader->ClearTable(FEDM_MYAXXESS_HOLIDAY_TABLE);


	// #####################################################
	//	<holiday-table>
	//		...
	//	</holiday-table>
	
	// <holiday-table>
	child = FindTag(FEDM_XML_TAG_HOLIDAY_TABLE_UC, (unsigned int)1, true);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	AddTagItem(root, child);
	table = child;

	// <holiday>
	child = FindTag(FEDM_XML_TAG_HOLIDAY_UC, (unsigned int)2, true);
	while(child != NULL)
	{
		AddTagItem(table, child);

		// date format is according to XML-Schema-Definition
		// The w3C xs:date data type is specified in the following form "YYYY-MM-DD" where:
		// YYYY indicates the year
		// MM indicates the month
		// DD indicates the day

		FEDM_SWSCANF(child->sValue, cValue);
		sscanf(cValue, "%u-%u-%u", &uiYear, &uiMonth, &uiDay);
		if(uiYear < 2000 || uiYear > 2099 || uiMonth < 1 || uiMonth > 12 || uiDay < 1 || uiDay > 31)
		{
			DeleteXmlTree(); // clean up
			m_pAxxessReader->ClearTable(FEDM_MYAXXESS_HOLIDAY_TABLE);
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}

		// save new record in table
		pItem = new FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM;
		pItem->Init();
		pItem->ucHoliday_Year = (unsigned char)(uiYear - 2000);
		pItem->ucHoliday_Month = (unsigned char)uiMonth;
		pItem->ucHoliday_Day = (unsigned char)uiDay;
		m_pAxxessReader->m_HolidayTable.push_back(pItem);

		child = FindTag(FEDM_XML_TAG_HOLIDAY_UC, (unsigned int)2, true);
	}

	// #####################################################

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	25.06.2009 / M. Hultsch
  Version       :	03.00.08 / 26.06.2009 / M. Hultsch

  Function		:	read xml document for doc type timezone table

  Parameters	:	FEDM_XML_TREEITEM* root	- pointer to parent xml element

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FedmIscMyAxxess_XmlParser::SerializeIn_TimezoneTable(FEDM_XML_TREEITEM* root)
{
	char cValue[128];
	unsigned int uiYear = 0;
	unsigned int uiMonth = 0;
	unsigned int uiDay = 0;
	unsigned int uiHour = 0;
	unsigned int uiMinute = 0;
	unsigned int uiSecond = 0;
	FEDM_XML_TREEITEM* table	= NULL;
	FEDM_XML_TREEITEM* zone		= NULL;
	FEDM_XML_TREEITEM* days		= NULL;
	FEDM_XML_TREEITEM* child	= NULL;

	FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM*	pItem = NULL;
	FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM	item;

	item.Init();

	// delete content of timezone table
	m_pAxxessReader->ClearTable(FEDM_MYAXXESS_TIMEZONE_TABLE);


	// #####################################################
	//	<timezone-table>
	//		...
	//	</timezone-table>
	
	// <timezone-table>
	child = FindTag(FEDM_XML_TAG_TIMEZONE_TABLE_UC, (unsigned int)1, true);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	AddTagItem(root, child);
	table = child;

	// <timezone>
	child = FindTag(FEDM_XML_TAG_TIMEZONE_UC, (unsigned int)2, true);
	while(child != NULL)
	{
		item.Init();

		AddTagItem(table, child);
		zone = child;

		// <days>
		child = FindTag(FEDM_XML_TAG_DAYS_UC, (unsigned int)3, true);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			m_pAxxessReader->ClearTable(FEDM_MYAXXESS_TIMEZONE_TABLE);
			return m_iLastError;
		}
		AddTagItem(zone, child);
		days = child;

		// <monday>
		child = FindTag(FEDM_XML_TAG_MONDAY_UC, (unsigned int)4, true);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			m_pAxxessReader->ClearTable(FEDM_MYAXXESS_TIMEZONE_TABLE);
			return m_iLastError;
		}
		AddTagItem(days, child);
		FEDM_SWSCANF(child->sValue, cValue);
		sscanf(cValue, "%u", &uiDay);
		if(uiDay > 0)
			item.ucDays |= 0x02;

		// <tuesday>
		child = FindTag(FEDM_XML_TAG_TUESDAY_UC, (unsigned int)4, true);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			m_pAxxessReader->ClearTable(FEDM_MYAXXESS_TIMEZONE_TABLE);
			return m_iLastError;
		}
		AddTagItem(days, child);
		FEDM_SWSCANF(child->sValue, cValue);
		sscanf(cValue, "%u", &uiDay);
		if(uiDay > 0)
			item.ucDays |= 0x04;

		// <wednesday>
		child = FindTag(FEDM_XML_TAG_WEDNESDAY_UC, (unsigned int)4, true);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			m_pAxxessReader->ClearTable(FEDM_MYAXXESS_TIMEZONE_TABLE);
			return m_iLastError;
		}
		AddTagItem(days, child);
		FEDM_SWSCANF(child->sValue, cValue);
		sscanf(cValue, "%u", &uiDay);
		if(uiDay > 0)
			item.ucDays |= 0x08;

		// <thursday>
		child = FindTag(FEDM_XML_TAG_THURSDAY_UC, (unsigned int)4, true);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			m_pAxxessReader->ClearTable(FEDM_MYAXXESS_TIMEZONE_TABLE);
			return m_iLastError;
		}
		AddTagItem(days, child);
		FEDM_SWSCANF(child->sValue, cValue);
		sscanf(cValue, "%u", &uiDay);
		if(uiDay > 0)
			item.ucDays |= 0x10;

		// <friday>
		child = FindTag(FEDM_XML_TAG_FRIDAY_UC, (unsigned int)4, true);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			m_pAxxessReader->ClearTable(FEDM_MYAXXESS_TIMEZONE_TABLE);
			return m_iLastError;
		}
		AddTagItem(days, child);
		FEDM_SWSCANF(child->sValue, cValue);
		sscanf(cValue, "%u", &uiDay);
		if(uiDay > 0)
			item.ucDays |= 0x20;

		// <saturday>
		child = FindTag(FEDM_XML_TAG_SATURDAY_UC, (unsigned int)4, true);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			m_pAxxessReader->ClearTable(FEDM_MYAXXESS_TIMEZONE_TABLE);
			return m_iLastError;
		}
		AddTagItem(days, child);
		FEDM_SWSCANF(child->sValue, cValue);
		sscanf(cValue, "%u", &uiDay);
		if(uiDay > 0)
			item.ucDays |= 0x40;

		// <sunday>
		child = FindTag(FEDM_XML_TAG_SUNDAY_UC, (unsigned int)4, true);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			m_pAxxessReader->ClearTable(FEDM_MYAXXESS_TIMEZONE_TABLE);
			return m_iLastError;
		}
		AddTagItem(days, child);
		FEDM_SWSCANF(child->sValue, cValue);
		sscanf(cValue, "%u", &uiDay);
		if(uiDay > 0)
			item.ucDays |= 0x01;

		// <date-from>
		child = FindTag(FEDM_XML_TAG_DATE_FROM_UC, (unsigned int)3, true);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			m_pAxxessReader->ClearTable(FEDM_MYAXXESS_TIMEZONE_TABLE);
			return m_iLastError;
		}
		AddTagItem(days, child);

		// date format is according to XML-Schema-Definition
		// The w3C xs:date data type is specified in the following form "YYYY-MM-DD" where:
		// YYYY indicates the year
		// MM indicates the month
		// DD indicates the day

		FEDM_SWSCANF(child->sValue, cValue);
		sscanf(cValue, "%u-%u-%u", &uiYear, &uiMonth, &uiDay);
		if(uiYear < 2000 || uiYear > 2099 || uiMonth < 1 || uiMonth > 12 || uiDay < 1 || uiDay > 31)
		{
			DeleteXmlTree(); // clean up
			m_pAxxessReader->ClearTable(FEDM_MYAXXESS_TIMEZONE_TABLE);
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		item.ucStartDate_Year	= (unsigned char)(uiYear - 2000);
		item.ucStartDate_Month	= (unsigned char)uiMonth;
		item.ucStartDate_Day	= (unsigned char)uiDay;

		// <date-to>
		child = FindTag(FEDM_XML_TAG_DATE_TO_UC, (unsigned int)3, true);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			m_pAxxessReader->ClearTable(FEDM_MYAXXESS_TIMEZONE_TABLE);
			return m_iLastError;
		}
		AddTagItem(days, child);

		// date format is according to XML-Schema-Definition
		// The w3C xs:date data type is specified in the following form "YYYY-MM-DD" where:
		// YYYY indicates the year
		// MM indicates the month
		// DD indicates the day

		FEDM_SWSCANF(child->sValue, cValue);
		sscanf(cValue, "%u-%u-%u", &uiYear, &uiMonth, &uiDay);
		if(uiYear < 2000 || uiYear > 2099 || uiMonth < 1 || uiMonth > 12 || uiDay < 1 || uiDay > 31)
		{
			DeleteXmlTree(); // clean up
			m_pAxxessReader->ClearTable(FEDM_MYAXXESS_TIMEZONE_TABLE);
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		item.ucEndDate_Year		= (unsigned char)(uiYear - 2000);
		item.ucEndDate_Month	= (unsigned char)uiMonth;
		item.ucEndDate_Day		= (unsigned char)uiDay;

		// <time-from>
		child = FindTag(FEDM_XML_TAG_TIME_FROM_UC, (unsigned int)3, true);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			m_pAxxessReader->ClearTable(FEDM_MYAXXESS_TIMEZONE_TABLE);
			return m_iLastError;
		}
		AddTagItem(days, child);

		// time format is according to XML-Schema-Definition
		// The w3C xs:time data type is specified in the following form "hh:mm:ss" where:
		// hh indicates the hour	{0..23}
		// mm indicates the minute	{0..59}
		// ss indicates the second (are ignored in myAxxess)

		FEDM_SWSCANF(child->sValue, cValue);
		sscanf(cValue, "%u:%u:%u", &uiHour, &uiMinute, &uiSecond);
		if(uiHour > 23 || uiMinute > 59)
		{
			DeleteXmlTree(); // clean up
			m_pAxxessReader->ClearTable(FEDM_MYAXXESS_TIMEZONE_TABLE);
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		item.ucStartTime_Hour	= (unsigned char)uiHour;
		item.ucStartTime_Minute	= (unsigned char)uiMinute;

		// <time-to>
		child = FindTag(FEDM_XML_TAG_TIME_TO_UC, (unsigned int)3, true);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			m_pAxxessReader->ClearTable(FEDM_MYAXXESS_TIMEZONE_TABLE);
			return m_iLastError;
		}
		AddTagItem(days, child);

		// time format is according to XML-Schema-Definition
		// The w3C xs:time data type is specified in the following form "hh:mm:ss" where:
		// hh indicates the hour	{0..23}
		// mm indicates the minute	{0..59}
		// ss indicates the second (are ignored in myAxxess)

		FEDM_SWSCANF(child->sValue, cValue);
		sscanf(cValue, "%u:%u:%u", &uiHour, &uiMinute, &uiSecond);
		if(uiHour > 23 || uiMinute > 59)
		{
			DeleteXmlTree(); // clean up
			m_pAxxessReader->ClearTable(FEDM_MYAXXESS_TIMEZONE_TABLE);
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		item.ucEndTime_Hour		= (unsigned char)uiHour;
		item.ucEndTime_Minute	= (unsigned char)uiMinute;


		// save new record in table
		pItem = new FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM;
		memcpy(pItem, &item, sizeof(FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM));
		m_pAxxessReader->m_TimezoneTable.push_back(pItem);

		child = FindTag(FEDM_XML_TAG_TIMEZONE_UC, (unsigned int)2, true);
	}

	// #####################################################

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	25.06.2009 / M. Hultsch
  Version       :	03.00.08 / 25.06.2009 / M. Hultsch

  Function		:	read xml document for doc type event table

  Parameters	:	FEDM_XML_TREEITEM* root	- pointer to parent xml element

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FedmIscMyAxxess_XmlParser::SerializeIn_EventTable(FEDM_XML_TREEITEM* root)
{
	FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
}


/***************************************************************************
  Begin        	:	25.06.2009 / M. Hultsch
  Version       :	03.00.08 / 25.06.2009 / M. Hultsch

  Function		:	write xml document for doc type access table

  Parameters	:	FEDM_XML_TREEITEM* root	- pointer to parent xml element

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FedmIscMyAxxess_XmlParser::SerializeOut_AccessTable(FEDM_XML_TREEITEM* root)
{
	char szValue[256];
	int	iBack = 0;
	unsigned int uiBit = 0;
	unsigned __int64 ui64Value = 0;
	wchar_t wcTmp[256];
	FEDM_XML_TREEITEM* table		= NULL;
	FEDM_XML_TREEITEM* access_data	= NULL;
	FEDM_XML_TREEITEM* timezones	= NULL;
	FEDM_XML_TREEITEM* child		= NULL;
	FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM* pItem = NULL;
	vector<FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM*>::iterator itor;

	// #####################################################
	//	<access-table>
	//		...
	//	</access-table>

	// #####################################################
	// add <access-table> tag
	child = BuildTag(FEDM_XML_TAG_ACCESS_TABLE_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	AddTagItem(root, child);
	table = child;

	// add <idd-format> tag
	child = BuildTag(FEDM_XML_TAG_IDD_FORMAT_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return GetLastError();
	}
	AddTagItem(table, child);

	switch(m_pAxxessReader->m_Metadata.ucIDDFormat)
	{
	case FEDM_MYAXXESS_IDD_FORMAT_ASCII:
	case FEDM_MYAXXESS_IDD_FORMAT_HEX:
	case FEDM_MYAXXESS_IDD_FORMAT_NUM:
		break;
	default:
		DeleteXmlTree(); // clean up
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}
#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
	swprintf(wcTmp, L"%d", m_pAxxessReader->m_Metadata.ucIDDFormat);
	#else
	swprintf(wcTmp, 256, L"%d", m_pAxxessReader->m_Metadata.ucIDDFormat);
	#endif
#else
	swprintf(wcTmp, 256, L"%d", m_pAxxessReader->m_Metadata.ucIDDFormat);
#endif
	AddTagValue(child, wcTmp);

	// add <idd-length> tag
	child = BuildTag(FEDM_XML_TAG_IDD_LENGTH_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return GetLastError();
	}
#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
	swprintf(wcTmp, L"%d", m_pAxxessReader->m_Metadata.ucIDDLength);
	#else
	swprintf(wcTmp, 256, L"%d", m_pAxxessReader->m_Metadata.ucIDDLength);
	#endif
#else
	swprintf(wcTmp, 256, L"%d", m_pAxxessReader->m_Metadata.ucIDDLength);
#endif
	AddTagValue(child, wcTmp);
	AddTagItem(table, child);

	// add <access-data> tag
	child = BuildTag(FEDM_XML_TAG_ACCESS_DATA_UC, false);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return GetLastError();
	}
	AddTagItem(table, child);
	access_data = child;

	// add all records
	for(itor  = m_pAxxessReader->m_AccessTable.begin();
		itor != m_pAxxessReader->m_AccessTable.end();
		itor++)
	{
		pItem = *itor;
		if(pItem == NULL)
			continue;

		// add <idd> tag
		child = BuildTag(FEDM_XML_TAG_IDD_UC, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return GetLastError();
		}

		switch(m_pAxxessReader->m_Metadata.ucIDDFormat)
		{
		case FEDM_MYAXXESS_IDD_FORMAT_ASCII:
			memcpy(szValue, pItem->ucIDD, m_pAxxessReader->m_Metadata.ucIDDLength);
			szValue[m_pAxxessReader->m_Metadata.ucIDDLength] = '\0';
			break;

		case FEDM_MYAXXESS_IDD_FORMAT_NUM:
			if(strlen((char*)pItem->ucIDD) > 20)
			{
				DeleteXmlTree(); // clean up
				FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
			}	
			ui64Value = 0;
			for(unsigned char ucCnt=0; ucCnt<m_pAxxessReader->m_Metadata.ucIDDLength; ucCnt++ )
			{
				// change Motorola format to Intel format
				ui64Value += ( (unsigned __int64)pItem->ucIDD[m_pAxxessReader->m_Metadata.ucIDDLength-ucCnt-1] << (8*ucCnt) );
			}
			break;

		case FEDM_MYAXXESS_IDD_FORMAT_HEX:
			FEDM_ConvHexUCharToHexChar(pItem->ucIDD, m_pAxxessReader->m_Metadata.ucIDDLength, szValue, 256);
			break;
		}

#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
		swprintf(wcTmp, L"%S", szValue);
	#else
		swprintf(wcTmp, 256, L"%S", szValue);
	#endif
#else
		swprintf(wcTmp, 256, L"%S", szValue);
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(access_data, child);

		// add <timezones> tag
		child = BuildTag(FEDM_XML_TAG_TIMEZONES_UC, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return GetLastError();
		}
		AddTagItem(access_data, child);
		timezones = child;

		uiBit = 1;
		for(int iCnt=1; iCnt<=16; iCnt++)
		{
			if(pItem->uiTimezones & uiBit)
			{
				// add <timezone> tag
				child = BuildTag(FEDM_XML_TAG_TIMEZONE_UC, false);
				if(child == NULL)
				{
					DeleteXmlTree(); // clean up
					return GetLastError();
				}
#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
				swprintf(wcTmp, L"%d", iCnt);
	#else
				swprintf(wcTmp, 256, L"%d", iCnt);
	#endif
#else
				swprintf(wcTmp, 256, L"%d", iCnt);
#endif
				AddTagValue(child, wcTmp);
				AddTagItem(timezones, child);

				uiBit <<= 1;
			}
		}

		// add <reserved> tag - ignored
	}


	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	25.06.2009 / M. Hultsch
  Version       :	03.00.08 / 26.06.2009 / M. Hultsch

  Function		:	write xml document for doc type holiday table

  Parameters	:	FEDM_XML_TREEITEM* root	- pointer to parent xml element

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FedmIscMyAxxess_XmlParser::SerializeOut_HolidayTable(FEDM_XML_TREEITEM* root)
{
	int	iBack = 0;
	wchar_t wcTmp[128];
	FEDM_XML_TREEITEM* table		= NULL;
	FEDM_XML_TREEITEM* child		= NULL;
	FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM* pItem = NULL;
	vector<FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM*>::iterator itor;

	// #####################################################
	//	<holiday-table>
	//		...
	//	</holiday-table>

	// #####################################################
	// add <holiday-table> tag
	child = BuildTag(FEDM_XML_TAG_HOLIDAY_TABLE_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	AddTagItem(root, child);
	table = child;

	// add all records
	for(itor  = m_pAxxessReader->m_HolidayTable.begin();
		itor != m_pAxxessReader->m_HolidayTable.end();
		itor++ )
	{
		pItem = *itor;
		if(pItem == NULL)
			continue;

		// add <holiday> tag
		child = BuildTag(FEDM_XML_TAG_HOLIDAY_UC, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return GetLastError();
		}

#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
		swprintf(wcTmp, L"%d-%02d-%02d", ((int)(pItem->ucHoliday_Year)+2000), pItem->ucHoliday_Month, pItem->ucHoliday_Day);
	#else
		swprintf(wcTmp, 128, L"%d-%02d-%02d", ((int)(pItem->ucHoliday_Year)+2000), pItem->ucHoliday_Month, pItem->ucHoliday_Day);
	#endif
#else
		swprintf(wcTmp, 128, L"%d-%02d-%02d", ((int)(pItem->ucHoliday_Year)+2000), pItem->ucHoliday_Month, pItem->ucHoliday_Day);
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(table, child);
	}


	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	25.06.2009 / M. Hultsch
  Version       :	03.00.08 / 26.06.2009 / M. Hultsch

  Function		:	write xml document for doc type timezone table

  Parameters	:	FEDM_XML_TREEITEM* root	- pointer to parent xml element

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FedmIscMyAxxess_XmlParser::SerializeOut_TimezoneTable(FEDM_XML_TREEITEM* root)
{
	int	iBack = 0;
	wchar_t wcTmp[128];
	FEDM_XML_TREEITEM* table	= NULL;
	FEDM_XML_TREEITEM* zone		= NULL;
	FEDM_XML_TREEITEM* days		= NULL;
	FEDM_XML_TREEITEM* child	= NULL;
	FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM* pItem = NULL;
	vector<FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM*>::iterator itor;

	// #####################################################
	//	<timezone-table>
	//		...
	//	</timezone-table>

	// #####################################################
	// add <timezone-table> tag
	child = BuildTag(FEDM_XML_TAG_TIMEZONE_TABLE_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	AddTagItem(root, child);
	table = child;

	// add all records
	for(itor  = m_pAxxessReader->m_TimezoneTable.begin();
		itor != m_pAxxessReader->m_TimezoneTable.end();
		itor++ )
	{
		pItem = *itor;
		if(pItem == NULL)
			continue;

		// add <timezone> tag
		child = BuildTag(FEDM_XML_TAG_TIMEZONE_UC, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return GetLastError();
		}
		AddTagItem(table, child);
		zone = child;

		// add <days> tag
		child = BuildTag(FEDM_XML_TAG_DAYS_UC, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return GetLastError();
		}
		AddTagItem(zone, child);
		days = child;

		// add <monday> tag
		child = BuildTag(FEDM_XML_TAG_MONDAY_UC, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return GetLastError();
		}
		if(pItem->ucDays & 0x02)	AddTagValue(child, L"1");
		else						AddTagValue(child, L"0");
		AddTagItem(days, child);

		// add <tuesday> tag
		child = BuildTag(FEDM_XML_TAG_TUESDAY_UC, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return GetLastError();
		}
		if(pItem->ucDays & 0x04)	AddTagValue(child, L"1");
		else						AddTagValue(child, L"0");
		AddTagItem(days, child);

		// add <wednesday> tag
		child = BuildTag(FEDM_XML_TAG_WEDNESDAY_UC, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return GetLastError();
		}
		if(pItem->ucDays & 0x08)	AddTagValue(child, L"1");
		else						AddTagValue(child, L"0");
		AddTagItem(days, child);

		// add <thursday> tag
		child = BuildTag(FEDM_XML_TAG_THURSDAY_UC, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return GetLastError();
		}
		if(pItem->ucDays & 0x10)	AddTagValue(child, L"1");
		else						AddTagValue(child, L"0");
		AddTagItem(days, child);

		// add <friday> tag
		child = BuildTag(FEDM_XML_TAG_FRIDAY_UC, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return GetLastError();
		}
		if(pItem->ucDays & 0x20)	AddTagValue(child, L"1");
		else						AddTagValue(child, L"0");
		AddTagItem(days, child);

		// add <saturday> tag
		child = BuildTag(FEDM_XML_TAG_SATURDAY_UC, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return GetLastError();
		}
		if(pItem->ucDays & 0x40)	AddTagValue(child, L"1");
		else						AddTagValue(child, L"0");
		AddTagItem(days, child);

		// add <sunday> tag
		child = BuildTag(FEDM_XML_TAG_SUNDAY_UC, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return GetLastError();
		}
		if(pItem->ucDays & 0x01)	AddTagValue(child, L"1");
		else						AddTagValue(child, L"0");
		AddTagItem(days, child);

		// add <date-from> tag
		child = BuildTag(FEDM_XML_TAG_DATE_FROM_UC, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return GetLastError();
		}
#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
		swprintf(wcTmp, L"%d-%02d-%02d", ((int)(pItem->ucStartDate_Year)+2000), pItem->ucStartDate_Month, pItem->ucStartDate_Day);
	#else
		swprintf(wcTmp, 128, L"%d-%02d-%02d", ((int)(pItem->ucStartDate_Year)+2000), pItem->ucStartDate_Month, pItem->ucStartDate_Day);
	#endif
#else
		swprintf(wcTmp, 128, L"%d-%02d-%02d", ((int)(pItem->ucStartDate_Year)+2000), pItem->ucStartDate_Month, pItem->ucStartDate_Day);
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(zone, child);

		// add <date-to> tag
		child = BuildTag(FEDM_XML_TAG_DATE_TO_UC, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return GetLastError();
		}
#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
		swprintf(wcTmp, L"%d-%02d-%02d", ((int)(pItem->ucEndDate_Year)+2000), pItem->ucEndDate_Month, pItem->ucEndDate_Day);
	#else
		swprintf(wcTmp, 128, L"%d-%02d-%02d", ((int)(pItem->ucEndDate_Year)+2000), pItem->ucEndDate_Month, pItem->ucEndDate_Day);
	#endif
#else
		swprintf(wcTmp, 128, L"%d-%02d-%02d", ((int)(pItem->ucEndDate_Year)+2000), pItem->ucEndDate_Month, pItem->ucEndDate_Day);
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(zone, child);

		// add <time-from> tag
		child = BuildTag(FEDM_XML_TAG_TIME_FROM_UC, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return GetLastError();
		}
#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
		swprintf(wcTmp, L"%02d:%02d:00", pItem->ucStartTime_Hour, pItem->ucStartTime_Minute);
	#else
		swprintf(wcTmp, 128, L"%02d:%02d:00", pItem->ucStartTime_Hour, pItem->ucStartTime_Minute);
	#endif
#else
		swprintf(wcTmp, 128, L"%02d:%02d:00", pItem->ucStartTime_Hour, pItem->ucStartTime_Minute);
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(zone, child);

		// add <time-to> tag
		child = BuildTag(FEDM_XML_TAG_TIME_TO_UC, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return GetLastError();
		}
#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
		swprintf(wcTmp, L"%02d:%02d:00", pItem->ucEndTime_Hour, pItem->ucEndTime_Minute);
	#else
		swprintf(wcTmp, 128, L"%02d:%02d:00", pItem->ucEndTime_Hour, pItem->ucEndTime_Minute);
	#endif
#else
		swprintf(wcTmp, 128, L"%02d:%02d:00", pItem->ucEndTime_Hour, pItem->ucEndTime_Minute);
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(zone, child);
	}


	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	25.06.2009 / M. Hultsch
  Version       :	03.00.08 / 29.06.2009 / M. Hultsch

  Function		:	write xml document for doc type event table

  Parameters	:	FEDM_XML_TREEITEM* root	- pointer to parent xml element
					unsigned int& uiCrc		- reference to crc

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FedmIscMyAxxess_XmlParser::SerializeOut_EventTable(FEDM_XML_TREEITEM* root, unsigned int& uiCrc)
{
	char szValue[256];
	int	iBack = 0;
	wchar_t wcTmp[256];
	unsigned __int64 ui64Value = 0;
	FEDM_XML_TREEITEM* table	= NULL;
	FEDM_XML_TREEITEM* ev		= NULL;
	FEDM_XML_TREEITEM* child	= NULL;
	FEDM_ISC_MYAXXESS_EVENT_TABLE_ITEM* pItem = NULL;


	// verify correct metadata info
	switch(m_pAxxessReader->m_Metadata.ucIDDFormat)
	{
	case FEDM_MYAXXESS_IDD_FORMAT_ASCII:
	case FEDM_MYAXXESS_IDD_FORMAT_HEX:
	case FEDM_MYAXXESS_IDD_FORMAT_NUM:
		break;
	default:
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}


	// calculate crc over some header data
	uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned char*)m_XmlHeader.szDocType, (unsigned int)strlen(m_XmlHeader.szDocType));
	uiCrc = CalcCRC16CheckSum(uiCrc, m_XmlHeader.uiDocVersion);

	// #####################################################
	//	<event-table>
	//		...
	//	</event-table>

	// #####################################################
	// add <event-table> tag
	child = BuildTag(FEDM_XML_TAG_EVENT_TABLE_UC);
	if(child == NULL)
	{
		DeleteXmlTree(); // clean up
		return m_iLastError;
	}
	AddTagItem(root, child);
	table = child;

	while(! m_pAxxessReader->m_EventTable.empty())
	{
		pItem = m_pAxxessReader->m_EventTable.front();
		if(pItem == NULL)
			continue;

		// add <event> tag
		child = BuildTag(FEDM_XML_TAG_EVENT_UC, false);
		if(child == NULL)
		{
			DeleteXmlTree(); // clean up
			return GetLastError();
		}
		AddTagItem(table, child);
		ev = child;

		// calculate crc over data
		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)pItem->ucEventID);
		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned char*)pItem->ucIDD, (unsigned int)m_pAxxessReader->m_Metadata.ucIDDLength);
		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)pItem->ucYear);
		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)pItem->ucMonth);
		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)pItem->ucDay);
		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)pItem->ucHour);
		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)pItem->ucMinute);
		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)pItem->ucSecond);
		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)pItem->ucInput);
		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)pItem->ucAction);
		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)pItem->uiError);
		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned char*)pItem->sSourceIP.c_str(), (unsigned int)pItem->sSourceIP.length());

		// add <event-id> tag
		child = BuildTag(FEDM_XML_TAG_EVENT_ID_UC, false);
		if(child == NULL)
		{
			delete pItem;
			DeleteXmlTree(); // clean up
			return GetLastError();
		}
#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
		swprintf(wcTmp, L"%d", pItem->ucEventID);
	#else
		swprintf(wcTmp, 256, L"%d", pItem->ucEventID);
	#endif
#else
		swprintf(wcTmp, 256, L"%d", pItem->ucEventID);
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(ev, child);

		// add <idd> tag
		child = BuildTag(FEDM_XML_TAG_IDD_UC, false);
		if(child == NULL)
		{
			delete pItem;
			DeleteXmlTree(); // clean up
			return GetLastError();
		}

		switch(m_pAxxessReader->m_Metadata.ucIDDFormat)
		{
		case FEDM_MYAXXESS_IDD_FORMAT_ASCII:
			memcpy(szValue, pItem->ucIDD, pItem->ucIDDLength);
			szValue[pItem->ucIDDLength] = '\0';
			break;

		case FEDM_MYAXXESS_IDD_FORMAT_NUM:
			if(strlen((char*)pItem->ucIDD) > 20)
			{
				delete pItem;
				DeleteXmlTree(); // clean up
				FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
			}	
			ui64Value = 0;
			for(unsigned char ucCnt=0; ucCnt<pItem->ucIDDLength; ucCnt++ )
			{
				// change Motorola format to Intel format
				ui64Value += ( (unsigned __int64)pItem->ucIDD[pItem->ucIDDLength-ucCnt-1] << (8*ucCnt) );
			}
			break;

		case FEDM_MYAXXESS_IDD_FORMAT_HEX:
			FEDM_ConvHexUCharToHexChar(pItem->ucIDD, pItem->ucIDDLength, szValue, 256);
			break;
		}

#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
		swprintf(wcTmp, L"%S", szValue);
	#else
		swprintf(wcTmp, 256, L"%S", szValue);
	#endif
#else
		swprintf(wcTmp, 256, L"%S", szValue);
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(ev, child);

		// add <date> tag
		child = BuildTag(FEDM_XML_TAG_DATE_UC, false);
		if(child == NULL)
		{
			delete pItem;
			DeleteXmlTree(); // clean up
			return GetLastError();
		}
#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
		swprintf(wcTmp, L"%d-%02d-%02d", ((int)(pItem->ucYear)+2000), pItem->ucMonth, pItem->ucDay);
	#else
		swprintf(wcTmp, 256, L"%d-%02d-%02d", ((int)(pItem->ucYear)+2000), pItem->ucMonth, pItem->ucDay);
	#endif
#else
		swprintf(wcTmp, 256, L"%d-%02d-%02d", ((int)(pItem->ucYear)+2000), pItem->ucMonth, pItem->ucDay);
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(ev, child);

		// add <time> tag
		child = BuildTag(FEDM_XML_TAG_TIME_UC, false);
		if(child == NULL)
		{
			delete pItem;
			DeleteXmlTree(); // clean up
			return GetLastError();
		}
#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
		swprintf(wcTmp, L"%02d:%02d:%02d", pItem->ucHour, pItem->ucMinute, pItem->ucSecond);
	#else
		swprintf(wcTmp, 256, L"%02d:%02d:%02d", pItem->ucHour, pItem->ucMinute, pItem->ucSecond);
	#endif
#else
		swprintf(wcTmp, 256, L"%02d:%02d:%02d", pItem->ucHour, pItem->ucMinute, pItem->ucSecond);
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(ev, child);

		// add <input> tag
		child = BuildTag(FEDM_XML_TAG_INPUT_UC, false);
		if(child == NULL)
		{
			delete pItem;
			DeleteXmlTree(); // clean up
			return GetLastError();
		}
#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
		swprintf(wcTmp, L"%d", pItem->ucInput);
	#else
		swprintf(wcTmp, 256, L"%d", pItem->ucInput);
	#endif
#else
		swprintf(wcTmp, 256, L"%d", pItem->ucInput);
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(ev, child);

		// add <action> tag
		child = BuildTag(FEDM_XML_TAG_ACTION_UC, false);
		if(child == NULL)
		{
			delete pItem;
			DeleteXmlTree(); // clean up
			return GetLastError();
		}
#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
		swprintf(wcTmp, L"%d", pItem->ucAction);
	#else
		swprintf(wcTmp, 256, L"%d", pItem->ucAction);
	#endif
#else
		swprintf(wcTmp, 256, L"%d", pItem->ucAction);
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(ev, child);

		// add <error> tag
		child = BuildTag(FEDM_XML_TAG_ERROR_UC, false);
		if(child == NULL)
		{
			delete pItem;
			DeleteXmlTree(); // clean up
			return GetLastError();
		}
#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
		swprintf(wcTmp, L"%d", pItem->uiError);
	#else
		swprintf(wcTmp, 256, L"%i", pItem->uiError);
	#endif
#else
		swprintf(wcTmp, 256, L"%i", pItem->uiError);
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(ev, child);

		// add <source> tag
		child = BuildTag(FEDM_XML_TAG_SOURCE_UC, false);
		if(child == NULL)
		{
			delete pItem;
			DeleteXmlTree(); // clean up
			return GetLastError();
		}
#if defined(__BORLANDC__) || defined(_MSC_VER)
	#if _MSC_VER <= 1200
		swprintf(wcTmp, L"%S", pItem->sSourceIP.c_str());
	#else
		swprintf(wcTmp, 256, L"%S", pItem->sSourceIP.c_str());
	#endif
#else
		swprintf(wcTmp, 256, L"%S", pItem->sSourceIP.c_str());
#endif
		AddTagValue(child, wcTmp);
		AddTagItem(ev, child);
	
		// item must be deleted, because item is no longer controlled by the queue
		delete pItem;
		m_pAxxessReader->m_EventTable.pop();

	} // while(! m_pAxxessReader->m_EventTable.empty())

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        	:	25.06.2009 / M. Hultsch
  Version       :	03.00.08 / 25.06.2009 / M. Hultsch

  Function		:	calculate CRC16 checksum over access table

  Parameters	:	-

  Return value	:	unsigned int uiCrc	- checksum
***************************************************************************/
unsigned int FedmIscMyAxxess_XmlParser::CalcChecksum_AccessTable()
{
	unsigned int uiCrc = FEDM_CRC16_PRESET;
	FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM* pItem = NULL;
	vector<FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM*>::iterator itor;

	// calculate crc over some header data
	uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned char*)m_XmlHeader.szDocType, (unsigned int)strlen(m_XmlHeader.szDocType));
	uiCrc = CalcCRC16CheckSum(uiCrc, m_XmlHeader.uiDocVersion);

	// calculate crc over data
	uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)m_pAxxessReader->m_Metadata.ucIDDFormat);
	uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)m_pAxxessReader->m_Metadata.ucIDDLength);

	for(itor  = m_pAxxessReader->m_AccessTable.begin();
		itor != m_pAxxessReader->m_AccessTable.end();
		itor++ )
	{
		pItem = *itor;
		if(pItem == NULL)
			continue;

		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned char*)pItem->ucIDD, (unsigned int)m_pAxxessReader->m_Metadata.ucIDDLength);
		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)pItem->uiTimezones);
		//uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)pItem->ucReserved); // ignored
	}

	return uiCrc;
}


/***************************************************************************
  Begin        	:	25.06.2009 / M. Hultsch
  Version       :	03.00.08 / 25.06.2009 / M. Hultsch

  Function		:	calculate CRC16 checksum over holiday table

  Parameters	:	-

  Return value	:	unsigned int uiCrc	- checksum
***************************************************************************/
unsigned int FedmIscMyAxxess_XmlParser::CalcChecksum_HolidayTable()
{
	unsigned int uiCrc = FEDM_CRC16_PRESET;
	FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM* pItem = NULL;
	vector<FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM*>::iterator itor;

	// calculate crc over some header data
	uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned char*)m_XmlHeader.szDocType, (unsigned int)strlen(m_XmlHeader.szDocType));
	uiCrc = CalcCRC16CheckSum(uiCrc, m_XmlHeader.uiDocVersion);

	// calculate crc over data
	for(itor  = m_pAxxessReader->m_HolidayTable.begin();
		itor != m_pAxxessReader->m_HolidayTable.end();
		itor++ )
	{
		pItem = *itor;
		if(pItem == NULL)
			continue;

		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)pItem->ucHoliday_Year);
		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)pItem->ucHoliday_Month);
		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)pItem->ucHoliday_Day);
	}

	return uiCrc;
}


/***************************************************************************
  Begin        	:	25.06.2009 / M. Hultsch
  Version       :	03.00.08 / 25.06.2009 / M. Hultsch

  Function		:	calculate CRC16 checksum over timezone table

  Parameters	:	-

  Return value	:	unsigned int uiCrc	- checksum
***************************************************************************/
unsigned int FedmIscMyAxxess_XmlParser::CalcChecksum_TimezoneTable()
{
	unsigned int uiCrc = FEDM_CRC16_PRESET;
	FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM* pItem = NULL;
	vector<FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM*>::iterator itor;

	// calculate crc over some header data
	uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned char*)m_XmlHeader.szDocType, (unsigned int)strlen(m_XmlHeader.szDocType));
	uiCrc = CalcCRC16CheckSum(uiCrc, m_XmlHeader.uiDocVersion);

	// calculate crc over data
	for(itor  = m_pAxxessReader->m_TimezoneTable.begin();
		itor != m_pAxxessReader->m_TimezoneTable.end();
		itor++ )
	{
		pItem = *itor;
		if(pItem == NULL)
			continue;

		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)pItem->ucDays);
		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)pItem->ucStartDate_Year);
		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)pItem->ucStartDate_Month);
		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)pItem->ucStartDate_Day);
		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)pItem->ucEndDate_Year);
		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)pItem->ucEndDate_Month);
		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)pItem->ucEndDate_Day);
		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)pItem->ucStartTime_Hour);
		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)pItem->ucStartTime_Minute);
		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)pItem->ucEndTime_Hour);
		uiCrc = CalcCRC16CheckSum(uiCrc, (unsigned int)pItem->ucEndTime_Minute);
	}

	return uiCrc;
}


void FedmIscMyAxxess_XmlParser::SetComment()
{
	wstring sXmlComment;

	sXmlComment  = L"\n\n";
	sXmlComment += L"<!-- Copyright Â© 2009 -->\n";
	sXmlComment += L"<!-- FEIG ELECTRONIC GmbH, All Rights Reserved. -->\n";
	sXmlComment += L"<!-- Lange Strasse 4 -->\n";
	sXmlComment += L"<!-- D-35781 Weilburg -->\n";
	sXmlComment += L"<!-- Federal Republic of Germany -->\n";
	sXmlComment += L"<!-- phone    : +49 6471 31090 -->\n";
	sXmlComment += L"<!-- fax      : +49 6471 310999 -->\n";
	sXmlComment += L"<!-- e-mail   : obid-support@feig.de -->\n";
	sXmlComment += L"<!-- Internet : http://www.feig.de -->\n";
	sXmlComment += L"<!-- OBIDÂ® and OBID i-scanÂ® are registered Trademarks of FEIG ELECTRONIC GmbH -->\n";
	sXmlComment += L"\n";
	sXmlComment += L"<!-- This file contains a myAxxess-Reader file for one OBID i-scanÂ® or OBIDÂ® classic-pro reader -->\n";
	sXmlComment += L"\n\n";

	FEDM_XmlParser_Base::SetComment(sXmlComment);
}

#endif // #if defined(_FEDM_MYAXXESS_XML_SUPPORT)
