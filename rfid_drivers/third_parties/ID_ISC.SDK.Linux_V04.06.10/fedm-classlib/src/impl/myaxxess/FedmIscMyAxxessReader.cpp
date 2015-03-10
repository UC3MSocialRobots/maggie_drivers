/*-------------------------------------------------------
|                                                       |
|                FedmIscMyAxxessReader.cpp              |
|                                                       |
---------------------------------------------------------

Copyright  2009-2011	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	02.03.2009

Version       		:	04.00.00 / 28.07.2011 / M. Hultsch
						- keep-alive option for event connection

						03.02.00 / 25.05.2010 / B. Tschunko, M. Hultsch
						- GetTableItem returns 0 if end of table is reached

						03.01.00 / 17.12.2009 / B. Tschunko, M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID i-scan and OBID classic-pro reader family with MyAxxess functionality

OBID and OBID i-scan are registered Trademarks of FEIG ELECTRONIC GmbH
Linux is a registered trademark of Linus Torvalds.
Windows is a registered trademark of Microsoft Corporation in the United States and other countries.
*/



#include <map>
#include <assert.h>
#ifdef _FEDM_LINUX
	#include <string.h>
#endif
#include "FedmIscMyAxxessReader.h"
#include "FedmIscMyAxxess_XmlParser.h"
#include "FedmIscMyAxxess_CsvParser.h"
#include "feisc.h"	// declaration of constants, datatypes and DLL-functions


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FedmIscMyAxxessReader::FedmIscMyAxxessReader(unsigned char ucIDDLength, unsigned char ucIDDFormat)
{
	m_bInternalReaderModule	= true;
	m_pReaderModule			= new FEDM_ISCReaderModule;
	m_ucDateFormat			= FEDM_MYAXXESS_DATE_FORMAT_ISO8601;
	m_uiTimeout				= 0;
	m_uiPackageLen			= 0;
	m_uiEventCallerType		= 0;	// 0 for C++
									// 1 for .NET

	m_cbEvent				= NULL;
	m_cbKeepAlive			= NULL;

//TODO_M
#if defined(_FEDM_MYAXXESS_XML_SUPPORT)
	m_pXmlParser			= NULL;//new FedmIscMyAxxess_XmlParser(this);
#endif
	m_pCsvParser			= new FedmIscMyAxxess_CsvParser(this);

	m_Metadata.Init();
	m_CrcList.Init();

#ifdef _FEDM_WINDOWS
	VERIFY(m_pReaderModule);
	SetIDDFormat(ucIDDFormat);
	SetIDDLength(ucIDDLength);
#endif
#ifdef _FEDM_LINUX
	assert(m_pReaderModule);
	SetIDDFormat(ucIDDFormat);
	SetIDDLength(ucIDDLength);
#endif
}

FedmIscMyAxxessReader::FedmIscMyAxxessReader(FEDM_ISCReaderModule* pReaderModule, unsigned char ucIDDLength, unsigned char ucIDDFormat)
{
	m_bInternalReaderModule	= false;
	m_pReaderModule			= pReaderModule;
	m_ucDateFormat			= FEDM_MYAXXESS_DATE_FORMAT_ISO8601;
	m_uiTimeout				= 0;
	m_uiPackageLen			= 0;
	m_uiEventCallerType		= 0;	// 0 for C++
									// 1 for .NET

	m_cbEvent				= NULL;
	m_cbKeepAlive			= NULL;

//TODO_M
#if defined(_FEDM_MYAXXESS_XML_SUPPORT)
	m_pXmlParser			= NULL;//new FedmIscMyAxxess_XmlParser(this);
#endif
	m_pCsvParser			= new FedmIscMyAxxess_CsvParser(this);

	m_Metadata.Init();
	m_CrcList.Init();

#ifdef _FEDM_WINDOWS
	VERIFY(m_pReaderModule);
	SetIDDFormat(ucIDDFormat);
	SetIDDLength(ucIDDLength);
#endif
#ifdef _FEDM_LINUX
	assert(m_pReaderModule);
	SetIDDFormat(ucIDDFormat);
	SetIDDLength(ucIDDLength);
#endif
}


FedmIscMyAxxessReader::~FedmIscMyAxxessReader()
{
	ClearTable(FEDM_MYAXXESS_TIMEZONE_TABLE);
	ClearTable(FEDM_MYAXXESS_HOLIDAY_TABLE);
	ClearTable(FEDM_MYAXXESS_ACCESS_TABLE);
	ClearTable(FEDM_MYAXXESS_EVENT_TABLE);

#if defined(_FEDM_MYAXXESS_XML_SUPPORT)
	if(m_pXmlParser != NULL)
		delete m_pXmlParser;
#endif

	if(m_pCsvParser != NULL)
		delete m_pCsvParser;

	if(m_bInternalReaderModule && m_pReaderModule != NULL)
		delete m_pReaderModule;
}


//////////////////////////////////////////////////////////////////////
// public functions
//////////////////////////////////////////////////////////////////////

/***************************************************************************
  Begin        	:	23.06.2009 / B. Tschunko
  Version       :	03.00.12 / 04.09.2009 / B. Tschunko, M. Hultsch

  Function		:	set IDD length
					NOTE: IDD-Format must be set before

  Parameters	:	unsigned char ucIDDLength	- IDD length to set

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FedmIscMyAxxessReader::SetIDDLength (unsigned char ucIDDLength)
{
	switch(m_Metadata.ucIDDFormat)
	{
	case FEDM_MYAXXESS_IDD_FORMAT_ASCII:
	case FEDM_MYAXXESS_IDD_FORMAT_HEX:
		if ((ucIDDLength < 1) || (ucIDDLength > 64))
			FEDM_RETURN(FEDM_ERROR_UNVALID_IDD_LENGTH);
		break;

	case FEDM_MYAXXESS_IDD_FORMAT_NUM:
		if ((ucIDDLength < 1) || (ucIDDLength > 8))
			FEDM_RETURN(FEDM_ERROR_UNVALID_IDD_LENGTH);
		break;

	default:
		FEDM_RETURN(FEDM_ERROR_UNVALID_IDD_FORMAT);
	}

	m_Metadata.ucIDDLength = ucIDDLength;
	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	25.06.2009 / B. Tschunko
  Version       :	03.00.08 / 26.07.2009 / B. Tschunko

  Function		:	set IDD format

  Parameters	:	unsigned char ucIDDFormat	- IDD format to set

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FedmIscMyAxxessReader::SetIDDFormat (unsigned char ucIDDFormat)
{
	if ((ucIDDFormat < 1) || (ucIDDFormat > 3))
		FEDM_RETURN(FEDM_ERROR_UNVALID_IDD_FORMAT);

	m_Metadata.ucIDDFormat = ucIDDFormat;
	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	10.02.2010 / M. Hultsch
  Version       :	03.01.00 / 10.02.2010 / M. Hultsch

  Function		:	check IDD settings

  Parameters	:	-

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FedmIscMyAxxessReader::ValidateIDDSettings()
{
	switch(m_Metadata.ucIDDFormat)
	{
	case FEDM_MYAXXESS_IDD_FORMAT_ASCII:
	case FEDM_MYAXXESS_IDD_FORMAT_HEX:
		if ((m_Metadata.ucIDDLength < 1) || (m_Metadata.ucIDDLength > 64))
			FEDM_RETURN(FEDM_ERROR_UNVALID_IDD_LENGTH);
		break;

	case FEDM_MYAXXESS_IDD_FORMAT_NUM:
		if ((m_Metadata.ucIDDLength < 1) || (m_Metadata.ucIDDLength > 8))
			FEDM_RETURN(FEDM_ERROR_UNVALID_IDD_LENGTH);
		break;

	default:
		FEDM_RETURN(FEDM_ERROR_UNVALID_IDD_FORMAT);
	}

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        	:	01.07.2009 / B. Tschunko
  Version       :	03.00.08 / 01.07.2009 / B. Tschunko

  Function		:	set Date format

  Parameters	:	unsigned char ucIDDFormat	- IDD format to set

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FedmIscMyAxxessReader::SetDateFormat (unsigned char ucDateFormat)
{
	if (ucDateFormat < 1 || ucDateFormat > 3)
		FEDM_RETURN(FEDM_ERROR_PARAMETER);

	m_ucDateFormat = ucDateFormat;
	FEDM_RETURN(FEDM_OK);
}


// public API for table management
/***************************************************************************
  Begin        	:	01.07.2009 / B. Tschunko
  Version       :	03.01.00 / 10.02.2010 / B. Tschunko, M. Hultsch

  Function		:	clear specific table

  Parameters	:	unsigned int uiTableID	- table identifier

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FedmIscMyAxxessReader::ClearTable(unsigned int uiTableID)
{
	int iError = ValidateIDDSettings();
	if(iError)
		return iError;

	vector<FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM*>::iterator	itorTimezoneTable;
	vector<FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM*>::iterator		itorHolidayTable;
	vector<FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM*>::iterator		itorAccessTable;
	FEDM_ISC_MYAXXESS_EVENT_TABLE_ITEM* pEventTableItem = NULL;

	switch (uiTableID)
	{
	case FEDM_MYAXXESS_TIMEZONE_TABLE:

		for(itorTimezoneTable  = m_TimezoneTable.begin();
			itorTimezoneTable != m_TimezoneTable.end();
			itorTimezoneTable++)
		{
			if(*itorTimezoneTable != NULL)
				delete *itorTimezoneTable;
		}

		m_TimezoneTable.clear();
		break;

	case FEDM_MYAXXESS_HOLIDAY_TABLE:

		for(itorHolidayTable  = m_HolidayTable.begin();
			itorHolidayTable != m_HolidayTable.end();
			itorHolidayTable++)
		{
			if(*itorHolidayTable != NULL)
				delete *itorHolidayTable;
		}

		m_HolidayTable.clear();
		break;

	case FEDM_MYAXXESS_ACCESS_TABLE:

		for(itorAccessTable  = m_AccessTable.begin();
			itorAccessTable != m_AccessTable.end();
			itorAccessTable++)
		{
			if(*itorAccessTable != NULL)
				delete *itorAccessTable;
		}

		m_AccessTable.clear();
		break;

	case FEDM_MYAXXESS_EVENT_TABLE:

		while(! m_EventTable.empty())
		{
			pEventTableItem = m_EventTable.front();
			if(pEventTableItem != NULL)
			{
				m_EventTable.pop();
			}
		}
		break;

	default:
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);

	}

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        		:	02.03.2009 / M. Hultsch

  Version       	:	03.00.08 / 29.06.2009 / M. Hultsch

  Function			:	Read table data from file

  Parameters		:	char* szFilename		- path and name of file
						unsigned int uiFiletype	- type of file (default: FEDM_MYAXXESS_FILETYPE_XML)

  Return value		:	FEDM_OK
						or
						< 0 in error case
***************************************************************************/
int FedmIscMyAxxessReader::SerializeTableIn(	char* szFilename,
												unsigned int uiFileType )
{
	switch (uiFileType)
	{
	case FEDM_MYAXXESS_FILETYPE_XML:
#if defined(_FEDM_MYAXXESS_XML_SUPPORT)
		FEDM_CHK5(m_pXmlParser);

		return m_pXmlParser->SerializeIn( szFilename, "Test", 4);
#else
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
#endif
		break;

	case FEDM_MYAXXESS_FILETYPE_CSV:
		FEDM_CHK5(m_pCsvParser)

		return m_pCsvParser->SerializeIn( szFilename );
		break;

	default:

		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        		:	02.03.2009 / M. Hultsch

  Version       	:	03.01.00 / 10.02.2010 / B. Tschunko, M. Hultsch

  Function			:	Write table data in file

  Parameters		:	unsigned int uiTableID	- table identifier from FedmIscMyAxxessReader.h
						char* szFilename		- path and name of file
						unsigned int uiFiletype	- type of file (default: FEDM_MYAXXESS_FILETYPE_XML)

  Return value		:	FEDM_OK
						or
						< 0 in error case
***************************************************************************/
int FedmIscMyAxxessReader::SerializeTableOut(	unsigned int uiTableID,
												char* szFilename,
												unsigned int uiFileType )
{
	int iError = ValidateIDDSettings();
	if(iError)
		return iError;

	switch (uiFileType)
	{
	case FEDM_MYAXXESS_FILETYPE_XML:
#if defined(_FEDM_MYAXXESS_XML_SUPPORT)
		FEDM_CHK5(m_pXmlParser);

		return m_pXmlParser->SerializeOut( uiTableID, szFilename, "Test", 4);
#else
		FEDM_RETURN(FEDM_ERROR_UNSUPPORTED);
#endif

		break;

	case FEDM_MYAXXESS_FILETYPE_CSV:
		FEDM_CHK5(m_pCsvParser)

		return m_pCsvParser->SerializeOut( uiTableID, szFilename );
		break;

	default:
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        		:	06.07.2009 / B. Tschunko

  Version       	:	03.01.00 / 10.02.2010 / B. Tschunko, M. Hultsch

  Function			:	Add new Item to timezone table

  Parameters		:	FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM* pItem	- pointer if the new item

  Return value		:	FEDM_OK
						or
						< 0 in error case
***************************************************************************/
int FedmIscMyAxxessReader::AddTableItem(FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM* pItem)
{
	int iError = ValidateIDDSettings();
	if(iError)
		return iError;

	// check table size
	if (m_TimezoneTable.size() >= 15)
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	// check new item
	if (pItem->ucStartDate_Year > 99)
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	if ((pItem->ucStartDate_Month < 1) || (pItem->ucStartDate_Month > 12))
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	if ((pItem->ucStartDate_Day < 1) || (pItem->ucStartDate_Day > 31))
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	if (pItem->ucStartTime_Hour > 23)
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	if (pItem->ucStartTime_Minute > 59)
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	if (pItem->ucEndDate_Year > 99)
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	if ((pItem->ucEndDate_Month < 1) || (pItem->ucEndDate_Month > 12))
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	if ((pItem->ucEndDate_Day < 1) || (pItem->ucEndDate_Day > 31))
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	if (pItem->ucEndTime_Hour > 23)
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	if (pItem->ucEndTime_Minute > 59)
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	// add new item
	FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM* pTimezoneTableItem = new FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM;
	if (pTimezoneTableItem == NULL)
	{
		FEDM_RETURN(FEDM_ERROR_NO_MORE_MEM);
	}
	pTimezoneTableItem->Init();
	memcpy((unsigned char*)pTimezoneTableItem, (unsigned char*)pItem, sizeof(FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM));

	// add item to vector
	m_TimezoneTable.push_back(pTimezoneTableItem);

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        		:	06.07.2009 / B. Tschunko

  Version       	:	03.01.00 / 10.02.2010 / B. Tschunko, M. Hultsch

  Function			:	Add new Item to hloiday table

  Parameters		:	FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM* pItem	- pointer if the new item

  Return value		:	FEDM_OK
						or
						< 0 in error case
***************************************************************************/
int FedmIscMyAxxessReader::AddTableItem(FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM* pItem)
{
	int iError = ValidateIDDSettings();
	if(iError)
		return iError;

	// check table size
	if (m_HolidayTable.size() >= 256)
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	// check new item
	if (pItem->ucHoliday_Year > 99)
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	if ((pItem->ucHoliday_Month < 1) || (pItem->ucHoliday_Month > 12))
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	if ((pItem->ucHoliday_Day < 1) || (pItem->ucHoliday_Day > 31))
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	// add new item
	FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM* pHolidayTableItem = new FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM;
	if (pHolidayTableItem == NULL)
	{
		FEDM_RETURN(FEDM_ERROR_NO_MORE_MEM);
	}
	pHolidayTableItem->Init();
	memcpy((unsigned char*)pHolidayTableItem, (unsigned char*)pItem, sizeof(FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM));

	// add item to vector
	m_HolidayTable.push_back(pHolidayTableItem);

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        		:	06.07.2009 / B. Tschunko

  Version       	:	03.01.00 / 10.02.2010 / B. Tschunko, M. Hultsch

  Function			:	Add new Item to access table

  Parameters		:	FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM* pItem	- pointer if the new item

  Return value		:	FEDM_OK
						or
						< 0 in error case
***************************************************************************/
int FedmIscMyAxxessReader::AddTableItem(FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM* pItem)
{
	int iError = ValidateIDDSettings();
	if(iError)
		return iError;

	// check table size
	if (m_AccessTable.size() >= 65535)
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	// search Access Table
	FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM*	pAccessItem	= NULL;
	vector<FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM*>::iterator	itorAccess;
	for(itorAccess  = m_AccessTable.begin();
		itorAccess != m_AccessTable.end();
		itorAccess++)
	{
		if ((*itorAccess) == NULL)
			continue;

		pAccessItem = *itorAccess;

		// same IDD cannot be inserted
		if (memcmp(pAccessItem->ucIDD, pItem->ucIDD, m_Metadata.ucIDDLength) == 0)
		{
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
	}

	// add new item
	FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM* pAccessTableItem = new FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM;
	if (pAccessTableItem == NULL)
	{
		FEDM_RETURN(FEDM_ERROR_NO_MORE_MEM);
	}
	pAccessTableItem->Init();
	memcpy((unsigned char*)pAccessTableItem, (unsigned char*)pItem, sizeof(FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM));

	// add item to vector
	m_AccessTable.push_back(pAccessTableItem);

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        		:	02.03.2009 / M. Hultsch

  Version       	:	03.01.00 / 10.02.2010 / B. Tschunko, M. Hultsch

  Function			:	Add new Item to table

  Parameters		:	unsigned int uiTableID	- table identifier from FedmIscMyAxxessReader.h
						char* sItem				- string if the new item
												  <element1_name=element1_value;element2_name=element2_value;...>

  Return value		:	FEDM_OK
						or
						< 0 in error case
***************************************************************************/
int FedmIscMyAxxessReader::AddTableItem(unsigned int uiTableID, char* sItem)
{
	int iBack = 0;	
	unsigned char ucCnt = 0;
	char* sPrt = NULL;
	vector<string> aName;
	vector<string> aValue;

	int iError = ValidateIDDSettings();
	if(iError)
		return iError;


	// get name of first element
	if (strlen(sItem) > FEDM_MAX_TEXT_SIZE)
	{
		FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
	}

	sPrt = strtok(sItem, "=");

	while (sPrt != NULL && ucCnt < 16)
	{
		aName.push_back("");
		aName[ucCnt] = sPrt;

		// get value of element
		sPrt = strtok(NULL, ";");
		if (sPrt != NULL)
		{
			aValue.push_back("");
			aValue[ucCnt] = sPrt;
		}
		else
		{
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		// get name of next element
		sPrt = strtok(NULL, "=");
		ucCnt++;
   }

	switch (uiTableID)
	{
	case FEDM_MYAXXESS_TIMEZONE_TABLE:

		FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM pItemTimezone;
		pItemTimezone.Init();
		iBack = BuildTableItem(aName, aValue, &pItemTimezone);
		if(iBack != 0)
		{
			return iBack;
		}		
		return AddTableItem(&pItemTimezone);
		break;

	case FEDM_MYAXXESS_HOLIDAY_TABLE:

		FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM pItemHoliday;
		pItemHoliday.Init();
		iBack = BuildTableItem(aName, aValue, &pItemHoliday);
		if(iBack != 0)
		{
			return iBack;
		}		
		return AddTableItem(&pItemHoliday);
		break;

	case FEDM_MYAXXESS_ACCESS_TABLE:

		FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM pItemAccess;
		pItemAccess.Init();
		iBack = BuildTableItem(aName, aValue, &pItemAccess);
		if(iBack != 0)
		{
			return iBack;
		}		
		return AddTableItem(&pItemAccess);
		break;

	default:
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
}


/***************************************************************************
  Begin        		:	07.07.2009 / B. Tschunko

  Version       	:	03.02.00 / 25.05.2010 / B. Tschunko, M. Hultsch

  Function			:	get one table item from the timezone table
						NOTE: method returns the next possible index and at end of table
							  a 1 is returned (index of first item)

  Parameters		:	unsigned int uiIndex							- table index of item
						FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM* pItem	- pointer if the new item

  Return value		:	> 0 - index of next table item to be read
						or
						0   - end of table
						or
						< 0 - in error case
***************************************************************************/
int FedmIscMyAxxessReader::GetTableItem(unsigned int uiIndex, FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM* pItem)
{
	unsigned int uiNextIndex = 1;

	int iError = ValidateIDDSettings();
	if(iError)
		return iError;


	// check table size
	if ((uiIndex < 1) || (uiIndex > m_TimezoneTable.size()))
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	vector<FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM*>::iterator itorTimezone;

	// search Timezone Table
	for(itorTimezone  = m_TimezoneTable.begin();
		itorTimezone != m_TimezoneTable.end();
		itorTimezone++)
	{
		if ((*itorTimezone) == NULL)
			continue;

		if (uiNextIndex++ != uiIndex)
			continue;

		// copy item
		memcpy((unsigned char*)pItem, (unsigned char*)(*itorTimezone), sizeof(FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM));
		break;
	}

	if (uiNextIndex > m_TimezoneTable.size())
	{
		uiNextIndex = 0;
	}

	return ((int)uiNextIndex);
}


/***************************************************************************
  Begin        		:	07.07.2009 / B. Tschunko

  Version       	:	03.02.00 / 25.05.2010 / B. Tschunko, M. Hultsch

  Function			:	get one table item from the holiday table
						NOTE: method returns the next possible index and at end of table
							  a 1 is returned (index of first item)

  Parameters		:	unsigned int uiIndex							- table index of item
						FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM* pItem		- pointer if the new item

  Return value		:	> 0 - index of next table item to be read
						or
						0   - end of table
						or
						< 0 - in error case
***************************************************************************/
int FedmIscMyAxxessReader::GetTableItem(unsigned int uiIndex, FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM* pItem)
{
	unsigned int uiNextIndex = 1;

	int iError = ValidateIDDSettings();
	if(iError)
		return iError;


	// check table size
	if ((uiIndex < 1) || (uiIndex > m_HolidayTable.size()))
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	vector<FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM*>::iterator itorHoliday;

	// search Holiday Table
	for(itorHoliday  = m_HolidayTable.begin();
		itorHoliday != m_HolidayTable.end();
		itorHoliday++)
	{
		if ((*itorHoliday) == NULL)
			continue;

		if (uiNextIndex++ != uiIndex)
			continue;

		// copy item
		memcpy((unsigned char*)pItem, (unsigned char*)(*itorHoliday), sizeof(FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM));
		break;
	}

	if (uiNextIndex > m_HolidayTable.size())
	{
		uiNextIndex = 0;
	}

	return ((int)uiNextIndex);
}


/***************************************************************************
  Begin        		:	07.07.2009 / B. Tschunko

  Version       	:	03.02.00 / 25.05.2010 / B. Tschunko, M. Hultsch

  Function			:	get one table item from the access table
						NOTE: method returns the next possible index and at end of table
							  a 1 is returned (index of first item)

  Parameters		:	unsigned int uiIndex							- table index of item
						FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM* pItem		- pointer if the new item

  Return value		:	> 0 - index of next table item to be read
						or
						0   - end of table
						or
						< 0 - in error case
***************************************************************************/
int FedmIscMyAxxessReader::GetTableItem(unsigned int uiIndex, FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM* pItem)
{
	unsigned int uiNextIndex = 1;

	int iError = ValidateIDDSettings();
	if(iError)
		return iError;


	// check table size
	if ((uiIndex < 1) || (uiIndex > m_AccessTable.size()))
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	vector<FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM*>::iterator itorAccess;

	// search Access Table
	for(itorAccess  = m_AccessTable.begin();
		itorAccess != m_AccessTable.end();
		itorAccess++)
	{
		if ((*itorAccess) == NULL)
			continue;

		if (uiNextIndex++ != uiIndex)
			continue;

		// copy item
		memcpy((unsigned char*)pItem, (unsigned char*)(*itorAccess), sizeof(FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM));
		break;
	}

	if (uiNextIndex > m_AccessTable.size())
	{
		uiNextIndex = 0;
	}

	return ((int)uiNextIndex);
}


/***************************************************************************
  Begin        		:	07.07.2009 / B. Tschunko

  Version       	:	03.01.00 / 10.02.2010 / B. Tschunko, M. Hultsch

  Function			:	get the last table item from the event table

  Parameters		:	FEDM_ISC_MYAXXESS_EVENT_TABLE_ITEM* pItem		- pointer if the new item

  Return value		:	FEDM_OK
						or
						< 0 in error case
***************************************************************************/
int FedmIscMyAxxessReader::GetTableItem(FEDM_ISC_MYAXXESS_EVENT_TABLE_ITEM* pItem)
{
	int iError = ValidateIDDSettings();
	if(iError)
		return iError;

	// search Event Table
	FEDM_ISC_MYAXXESS_EVENT_TABLE_ITEM*	pEventItem	= NULL;
	
	if(m_EventTable.empty())
	{
		FEDM_RETURN(FEDM_ERROR_NO_TABLE_DATA);
	}

	pEventItem = m_EventTable.front();

	if(&pEventItem == NULL)
	{
		FEDM_RETURN(FEDM_ERROR_NULL_POINTER);
	}

	// copy item
	memcpy((unsigned char*)pItem, (unsigned char*)pEventItem, sizeof(FEDM_ISC_MYAXXESS_EVENT_TABLE_ITEM));
	
	// item must be deleted, because item is no longer controlled by the queue
	m_EventTable.pop();

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        		:	24.06.2009 / M. Hultsch

  Version       	:	03.01.00 / 10.02.2010 / B. Tschunko, M. Hultsch

  Function			:	get one table item from a specified internal table
						NOTE: method returns the next possible index and at end of table
							  a 1 is returned (index of first item)

  Parameters		:	unsigned int uiTableID	- table identifier from FedmIscMyAxxessReader.h
						unsigned int uiIndex	- table index of item
						char* sItem				- string if the new item
												  <element1_name=element1_value;element2_name=element2_value;...>

  Return value		:	> 0 - index of next table item to be read
						or
						< 0 in error case
***************************************************************************/
int FedmIscMyAxxessReader::GetTableItem(unsigned int uiTableID, unsigned int uiIndex, char* sItem)
{
	int iError = ValidateIDDSettings();
	if(iError)
		return iError;

	switch (uiTableID)
	{
	case FEDM_MYAXXESS_TIMEZONE_TABLE:

		return GetTableItem_Timezone(uiIndex, sItem);
		break;

	case FEDM_MYAXXESS_HOLIDAY_TABLE:

		return GetTableItem_Holiday(uiIndex, sItem);
		break;

	case FEDM_MYAXXESS_ACCESS_TABLE:

		return GetTableItem_Access(uiIndex, sItem);
		break;

	case FEDM_MYAXXESS_EVENT_TABLE:

		return GetTableItem_Event(sItem);
		break;

	default:
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
}


/***************************************************************************
  Begin        		:	07.07.2009 / B. Tschunko

  Version       	:	03.01.00 / 10.02.2010 / B. Tschunko, M. Hultsch

  Function			:	remove one table item from a specified internal table

  Parameters		:	unsigned int uiTableID	- table identifier from FedmIscMyAxxessReader.h
						unsigned int uiIndex	- table index of item

  Return value		:	FEDM_OK
						or
						< 0 in error case
***************************************************************************/
int FedmIscMyAxxessReader::RemoveTableItem(unsigned int uiTableID, unsigned int uiIndex)
{
	int iError = ValidateIDDSettings();
	if(iError)
		return iError;

	switch (uiTableID)
	{
	case FEDM_MYAXXESS_HOLIDAY_TABLE:

		// check table size
		if ((uiIndex < 1) || (uiIndex > m_HolidayTable.size()))
		{
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}

		// remove item
		m_HolidayTable.erase(m_HolidayTable.begin()+uiIndex-1);
		break;

	case FEDM_MYAXXESS_ACCESS_TABLE:

		// check table size
		if ((uiIndex < 1) || (uiIndex > m_AccessTable.size()))
		{
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}

		// remove item
		m_AccessTable.erase(m_AccessTable.begin()+uiIndex-1);
		break;

	default:
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        		:	07.07.2009 / B. Tschunko

  Version       	:	03.01.00 / 10.02.2010 / B. Tschunko, M. Hultsch

  Function			:	modify one table item of the timezone table

  Parameters		:	unsigned int uiIndex	- table index of item
						FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM* pItem	- pointer if the item to modify

  Return value		:	FEDM_OK
						or
						< 0 in error case
***************************************************************************/
int FedmIscMyAxxessReader::ModifyTableItem(unsigned int uiIndex, FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM* pItem)
{
	unsigned int uiNextIndex = 1;

	int iError = ValidateIDDSettings();
	if(iError)
		return iError;

	// check table size
	if ((uiIndex < 1) || (uiIndex > m_TimezoneTable.size()))
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	// check new item
	if (pItem->ucStartDate_Year > 99)
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	if ((pItem->ucStartDate_Month < 1) || (pItem->ucStartDate_Month > 12))
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	if ((pItem->ucStartDate_Day < 1) || (pItem->ucStartDate_Day > 31))
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	if (pItem->ucStartTime_Hour > 23)
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	if (pItem->ucStartTime_Minute > 59)
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	if (pItem->ucEndDate_Year > 99)
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	if ((pItem->ucEndDate_Month < 1) || (pItem->ucEndDate_Month > 12))
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	if ((pItem->ucEndDate_Day < 1) || (pItem->ucEndDate_Day > 31))
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	if (pItem->ucEndTime_Hour > 23)
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	if (pItem->ucEndTime_Minute > 59)
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	vector<FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM*>::iterator itorTimezone;

	// search Timezone Table
	for(itorTimezone  = m_TimezoneTable.begin();
		itorTimezone != m_TimezoneTable.end();
		itorTimezone++)
	{
		if ((*itorTimezone) == NULL)
			continue;

		if (uiNextIndex++ != uiIndex)
			continue;

		// copy item
		memcpy((unsigned char*)(*itorTimezone), (unsigned char*)pItem, sizeof(FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM));
		break;
	}

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        		:	07.07.2009 / B. Tschunko

  Version       	:	03.01.00 / 10.02.2010 / B. Tschunko, M. Hultsch

  Function			:	modify one table item of the holiday table

  Parameters		:	unsigned int uiIndex	- table index of item
						FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM* pItem	- pointer if the item to modify

  Return value		:	FEDM_OK
						or
						< 0 in error case
***************************************************************************/
int FedmIscMyAxxessReader::ModifyTableItem(unsigned int uiIndex, FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM* pItem)
{
	unsigned int uiNextIndex = 1;

	int iError = ValidateIDDSettings();
	if(iError)
		return iError;

	// check table size
	if ((uiIndex < 1) || (uiIndex > m_HolidayTable.size()))
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	// check new item
	if (pItem->ucHoliday_Year > 99)
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	if ((pItem->ucHoliday_Month < 1) || (pItem->ucHoliday_Month > 12))
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	if ((pItem->ucHoliday_Day < 1) || (pItem->ucHoliday_Day > 31))
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	vector<FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM*>::iterator itorHoliday;

	// search Holiday Table
	for(itorHoliday  = m_HolidayTable.begin();
		itorHoliday != m_HolidayTable.end();
		itorHoliday++)
	{
		if ((*itorHoliday) == NULL)
			continue;

		if (uiNextIndex++ != uiIndex)
			continue;

		// modify item
		memcpy((unsigned char*)(*itorHoliday), (unsigned char*)pItem, sizeof(FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM));
		break;
	}

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        		:	07.07.2009 / B. Tschunko

  Version       	:	03.01.00 / 10.02.2010 / B. Tschunko, M. Hultsch

  Function			:	modify one table item of the access table

  Parameters		:	unsigned int uiIndex	- table index of item
						FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM* pItem	- pointer if the item to modify

  Return value		:	FEDM_OK
						or
						< 0 in error case
***************************************************************************/
int FedmIscMyAxxessReader::ModifyTableItem(unsigned int uiIndex, FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM* pItem)
{
	unsigned int uiNextIndex = 1;

	int iError = ValidateIDDSettings();
	if(iError)
		return iError;

	// check table size
	if ((uiIndex < 1) || (uiIndex > m_AccessTable.size()))
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	vector<FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM*>::iterator itorAccess;

	// search Access Table
	for(itorAccess  = m_AccessTable.begin();
		itorAccess != m_AccessTable.end();
		itorAccess++)
	{
		if ((*itorAccess) == NULL)
			continue;

		if (uiNextIndex++ != uiIndex)
			continue;

		// copy item
		memcpy((unsigned char*)(*itorAccess), (unsigned char*)pItem, sizeof(FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM));
		break;
	}

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        		:	07.07.2009 / B. Tschunko

  Version       	:	03.01.00 / 10.02.2010 / B. Tschunko, M. Hultsch

  Function			:	modify one table item of a specified internal table

  Parameters		:	unsigned int uiTableID	- table identifier from FedmIscMyAxxessReader.h
						unsigned int uiIndex	- table index of item
						char* sItem				- string if the new item
												  <element1_name=element1_value;element2_name=element2_value;...>

  Return value		:	FEDM_OK
						or
						< 0 in error case
***************************************************************************/
int FedmIscMyAxxessReader::ModifyTableItem(unsigned int uiTableID, unsigned int uiIndex, char* sItem)
{
	int iBack = 0;
	unsigned char ucCnt = 0;
	char* sPrt = NULL;
	vector<string> aName;
	vector<string> aValue;

	int iError = ValidateIDDSettings();
	if(iError)
		return iError;

	// get name of first element
	if (strlen(sItem) > FEDM_MAX_TEXT_SIZE)
	{
		FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
	}

	sPrt = strtok(sItem, "=");

	while (sPrt != NULL && ucCnt < 16)
	{
		aName.push_back("");
		aName[ucCnt] = sPrt;

		// get value of element
		sPrt = strtok(NULL, ";");
		if (sPrt != NULL)
		{
			aValue.push_back("");
			aValue[ucCnt] = sPrt;
		}
		else
		{
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		// get name of next element
		sPrt = strtok(NULL, "=");
		ucCnt++;
   }

	switch (uiTableID)
	{
	case FEDM_MYAXXESS_TIMEZONE_TABLE:

		FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM pItemTimezone;
		pItemTimezone.Init();
		iBack = BuildTableItem(aName, aValue, &pItemTimezone);
		if(iBack != 0)
		{
			return iBack;
		}		
		return ModifyTableItem(uiIndex, &pItemTimezone);
		break;

	case FEDM_MYAXXESS_HOLIDAY_TABLE:

		FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM pItemHoliday;
		pItemHoliday.Init();
		iBack = BuildTableItem(aName, aValue, &pItemHoliday);
		if(iBack != 0)
		{
			return iBack;
		}		
		return ModifyTableItem(uiIndex, &pItemHoliday);
		break;

	case FEDM_MYAXXESS_ACCESS_TABLE:

		FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM pItemAccess;
		pItemAccess.Init();
		iBack = BuildTableItem(aName, aValue, &pItemAccess);
		if(iBack != 0)
		{
			return iBack;
		}		
		return ModifyTableItem(uiIndex, &pItemAccess);
		break;

	default:
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
}


// public API for table update

/***************************************************************************
  Begin        		:	02.03.2009 / M. Hultsch

  Version       	:	03.01.00 / 10.02.2010 / B. Tschunko, M. Hultsch

  Function			:	Write tables into reader

  Parameters		:	-

  Return value		:	FEDM_OK
						or
						< 0 in error case
***************************************************************************/
int FedmIscMyAxxessReader::WriteTables()
{
	int iBack = 0;
	int iOldTimeout = 0;

	int iError = ValidateIDDSettings();
	if(iError)
		return iError;

	m_CrcList.Init();

	// 1. step:	Send Start Update with Metadata
	iBack = WriteTables_Metadata();
	if(iBack != 0)
	{
		return iBack;
	}

	if(m_uiPackageLen > FEDM_MYAXXESS_MAX_BLOCKSIZE)
	{
		FEDM_RETURN(FEDM_ERROR_BLOCK_SIZE);
	}
/*
	if(FEDM_IS_COMPORT(m_iReaderHnd) && uiPackageLen > FEDM_FW_EXC_SERIAL_BLOCKSIZE)
	{
		// limitation for serial port
		uiPackageLen = FEDM_FW_EXC_SERIAL_BLOCKSIZE;
	}
*/
	// get old timeout of PC
	m_pReaderModule->GetPortPara("Timeout", &iOldTimeout);
	// set the timeout for PC
	m_pReaderModule->SetPortPara("Timeout", m_uiTimeout);

	// 2. step: Upload Timezone Table
	iBack = WriteTables_Timezone();
	if(iBack != 0)
	{
		return iBack;
	}

	// 3. step: Upload Holiday Table
	iBack = WriteTables_Holiday();
	if(iBack != 0)
	{
		return iBack;
	}

	// 4. step: Upload AccessData Table
	iBack = WriteTables_Access();
	if(iBack != 0)
	{
		return iBack;
	}

	// 5. step:	Send Update End with CrcList
	iBack = WriteTables_CrcList();
	if(iBack != 0)
	{
		return iBack;
	}

	// restore old timeout for PC
	m_pReaderModule->SetPortPara("Timeout", iOldTimeout);

	FEDM_RETURN(FEDM_OK);
}

/***************************************************************************
  Begin        		:	02.03.2009 / M. Hultsch

  Version       	:	03.01.00 / 10.02.2010 / B. Tschunko, M. Hultsch

  Function			:	Read table from reader

  Parameters		:	unsigned int uiTableID	- table identifier from FedmIscMyAxxessReader.h

  Return value		:	FEDM_OK
						or
						< 0 in error case
***************************************************************************/
int FedmIscMyAxxessReader::ReadTable(unsigned int uiTableID)
{
	int iBack = 0;

	int iError = ValidateIDDSettings();
	if(iError)
		return iError;


	switch (uiTableID)
	{
	case FEDM_MYAXXESS_CRC_LIST:

		return ReadTable_CrcList();
		break;

	case FEDM_MYAXXESS_METADATA:

		return ReadTable_Metadata();
		break;

	case FEDM_MYAXXESS_TIMEZONE_TABLE:

		return ReadTable_Timezone();
		break;

	case FEDM_MYAXXESS_HOLIDAY_TABLE:

		return ReadTable_Holiday();
		break;

	case FEDM_MYAXXESS_ACCESS_TABLE:

		iBack = ReadTable_Metadata();
		if (iBack != 0)
			return iBack;
		else
			return ReadTable_Access();
		break;

	case FEDM_MYAXXESS_EVENT_TABLE:

		iBack = ReadTable_Metadata();
		if (iBack != 0)
			return iBack;
		else
			return ReadTable_Event();
		break;

	default:
		FEDM_RETURN(FEDM_ERROR_UNKNOWN_TABLE_ID);
	}

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        		:	24.09.2009 / M. Hultsch

  Version       	:	03.01.00 / 10.02.2010 / M. Hultsch

  Function			:	Start of receive thread for MAX events

  Parameters		:	unsigned int uiPortNr	- TCP/IP-Port
						void* pAny				- pointer to anything, which is reflected as the first parameter 
												  in the callback function (e.g. can be used to pass the object pointer)
						LPFN_FEDM_MYAXXESS_EVENT_CB cbEvent			- callback function for access events
						LPFN_FEDM_MYAXXESS_KEEPALIVE_CB cbKeepAlive	- callback function for keep alive notifications
						int iAuthentType		- authentication type
												  0 : AES 128
												  1 : AES 192
												  2 : AES 256
						char* sAuthentKey		- authentication key in hex format (e.g. "0134A0F3...")
												  key length must correspond to authent type

  Return value		:	FEDM_OK
						or
						< 0 in error case
***************************************************************************/
int FedmIscMyAxxessReader::StartEventHandler(	unsigned int uiPortNr,
												void* pAny,
												LPFN_FEDM_MYAXXESS_EVENT_CB cbEvent,
												LPFN_FEDM_MYAXXESS_KEEPALIVE_CB cbKeepAlive,
												int iAuthentType,
												char* sAuthentKey )
{
	int iBack = 0;
	FEISC_TASK_INIT	TI;

	int iError = ValidateIDDSettings();
	if(iError)
		return iError;

	if(m_pReaderModule->m_bActiveAsyncTask)
		FEDM_RETURN(FEDM_ERROR_TASK_STILL_RUNNING);

	if(uiPortNr > 65535)
		FEDM_RETURN(FEDM_ERROR_PARAMETER);

	switch(iAuthentType)
	{
	case 0:
	case 1:
	case 2:
		FEDM_CHK5(sAuthentKey);
		break;
	}

	// initialize task structure for this class
	memset(&m_pReaderModule->m_TaskInit, 0, sizeof(FEDM_TASK_INIT));
	m_pReaderModule->m_TaskInit.uiFlag		= FEDM_TASKCB3;
	m_pReaderModule->m_TaskInit.uiUse		= FEDM_TASKID_MAX_EVENT;
	m_pReaderModule->m_TaskInit.pAny		= pAny;
	m_pReaderModule->m_TaskInit.uiTimeout	= 30;	// in seconds
	m_cbEvent				= cbEvent;
	m_cbKeepAlive			= cbKeepAlive;

	m_uiEventCallerType	= 0;	// 0 for C++
								// 1 for .NET

	// initialize task structure for FEISC which handles the task
	memset(&TI, 0, sizeof(FEISC_TASK_INIT));
	TI.uiFlag				= FEISC_TASKCB_3;
	TI.cbFct3				= cbsTaskRsp3;		// static member function
	TI.uiChannelType		= FEISC_TASK_CHANNEL_TYPE_NEW_TCP;
	TI.ucBusAdr				= m_pReaderModule->GetBusAddress();
	TI.iPortNr				= uiPortNr;
	TI.pAny					= (void*)this;		// backpointer to caller object
	TI.uiTimeout			= m_pReaderModule->m_TaskInit.uiTimeout;

	switch(iAuthentType)
	{
	case 0:
		TI.bCryptoMode = true;
		TI.uiAuthentKeyLength = 16;
		FEDM_CHK2(iBack, FEDM_ConvHexCharToHexUChar(sAuthentKey, 32, TI.ucAuthentKey, 16));
		break;
	case 1:
		TI.bCryptoMode = true;
		TI.uiAuthentKeyLength = 24;
		FEDM_CHK2(iBack, FEDM_ConvHexCharToHexUChar(sAuthentKey, 48, TI.ucAuthentKey, 24));
		break;
	case 2:
		TI.bCryptoMode = true;
		TI.uiAuthentKeyLength = 32;
		FEDM_CHK2(iBack, FEDM_ConvHexCharToHexUChar(sAuthentKey, 64, TI.ucAuthentKey, 32));
		break;
	}

	TI.bKeepAlive = true;
	TI.uiKeepAliveIdleTime = 500;
	TI.uiKeepAliveProbeCount = 5;
	TI.uiKeepAliveIntervalTime = 500;

	FEDM_CHK2(iBack, FEISC_StartAsyncTask(m_pReaderModule->GetReaderHnd(), FEISC_TASKID_MAX_EVENT, &TI, NULL));

	m_pReaderModule->m_bActiveAsyncTask = true; // asynchronous task is started successfully

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        		:	24.09.2009 / M. Hultsch

  Version       	:	04.00.00 / 28.07.2011 / M. Hultsch
						- keep-alive option

						03.01.00 / 10.02.2010 / M. Hultsch

  Function			:	Start of receive thread for MAX events
						NOTE: only for .NET-Wrapper

  Parameters		:	unsigned int uiPortNr	- TCP/IP-Port
						void* pAny				- pointer to anything, which is reflected as the first parameter 
												  in the callback function (e.g. can be used to pass the object pointer)
						LPFN_FEDM_MYAXXESS_EVENT_DOTNET_CB cbEvent			- callback function for access events
						LPFN_FEDM_MYAXXESS_KEEPALIVE_DOTNET_CB cbKeepAlive	- callback function for keep alive notifications
						int iAuthentType		- authentication type
												  0 : AES 128
												  1 : AES 192
												  2 : AES 256
						char* sAuthentKey		- authentication key in hex format (e.g. "0134A0F3...")
												  key length must correspond to authent type

  Return value		:	FEDM_OK
						or
						< 0 in error case
***************************************************************************/
int FedmIscMyAxxessReader::StartEventHandler4DotNet(	unsigned int uiPortNr,
														void* pAny,
														LPFN_FEDM_MYAXXESS_EVENT_DOTNET_CB cbEvent,
														LPFN_FEDM_MYAXXESS_KEEPALIVE_DOTNET_CB cbKeepAlive,
														int iAuthentType,
														char* sAuthentKey )
{
	int iBack = 0;
	FEISC_TASK_INIT	TI;

	int iError = ValidateIDDSettings();
	if(iError)
		return iError;

	if(m_pReaderModule->m_bActiveAsyncTask)
		FEDM_RETURN(FEDM_ERROR_TASK_STILL_RUNNING);

	if(uiPortNr > 65535)
		FEDM_RETURN(FEDM_ERROR_PARAMETER);

	switch(iAuthentType)
	{
	case 0:
	case 1:
	case 2:
		FEDM_CHK5(sAuthentKey);
		break;
	}

	// initialize task structure for this class
	memset(&m_pReaderModule->m_TaskInit, 0, sizeof(FEDM_TASK_INIT));
	m_pReaderModule->m_TaskInit.uiFlag		= FEDM_TASKCB3;
	m_pReaderModule->m_TaskInit.uiUse		= FEDM_TASKID_MAX_EVENT;
	m_pReaderModule->m_TaskInit.pAny		= pAny;
	m_pReaderModule->m_TaskInit.uiTimeout	= 30;	// in seconds
	m_cbStdCallEvent		= cbEvent;
	m_cbStdCallKeepAlive	= cbKeepAlive;

	m_uiEventCallerType	=	1;	// 0 for C++
								// 1 for .NET

	// initialize task structure for FEISC which handles the task
	memset(&TI, 0, sizeof(FEISC_TASK_INIT));
	TI.uiFlag				= FEISC_TASKCB_3;
	TI.cbFct3				= cbsTaskRsp3;		// static member function
	TI.uiChannelType		= FEISC_TASK_CHANNEL_TYPE_NEW_TCP;
	TI.ucBusAdr				= m_pReaderModule->GetBusAddress();
	TI.iPortNr				= uiPortNr;
	TI.pAny					= (void*)this;		// backpointer to caller object
	TI.uiTimeout			= m_pReaderModule->m_TaskInit.uiTimeout;

	switch(iAuthentType)
	{
	case 0:
		TI.bCryptoMode = true;
		TI.uiAuthentKeyLength = 16;
		FEDM_CHK2(iBack, FEDM_ConvHexCharToHexUChar(sAuthentKey, 32, TI.ucAuthentKey, 16));
		break;
	case 1:
		TI.bCryptoMode = true;
		TI.uiAuthentKeyLength = 24;
		FEDM_CHK2(iBack, FEDM_ConvHexCharToHexUChar(sAuthentKey, 48, TI.ucAuthentKey, 24));
		break;
	case 2:
		TI.bCryptoMode = true;
		TI.uiAuthentKeyLength = 32;
		FEDM_CHK2(iBack, FEDM_ConvHexCharToHexUChar(sAuthentKey, 64, TI.ucAuthentKey, 32));
		break;
	}

	TI.bKeepAlive = true;
	TI.uiKeepAliveIdleTime = 500;
	TI.uiKeepAliveProbeCount = 5;
	TI.uiKeepAliveIntervalTime = 500;

	FEDM_CHK2(iBack, FEISC_StartAsyncTask(m_pReaderModule->GetReaderHnd(), FEISC_TASKID_MAX_EVENT, &TI, NULL));

	m_pReaderModule->m_bActiveAsyncTask = true; // asynchronous task is started successfully

	FEDM_RETURN(FEDM_OK);
}


int FedmIscMyAxxessReader::StopEventHandler()
{
	int iBack = m_pReaderModule->CancelAsyncTask();

	if(iBack == FEDM_OK)
	{
		memset(&m_pReaderModule->m_TaskInit, 0, sizeof(FEDM_TASK_INIT));
		m_cbEvent			= NULL;
		m_cbKeepAlive		= NULL;
	}

	return iBack;
}


//////////////////////////////////////////////////////////////////////
// protected functions
//////////////////////////////////////////////////////////////////////
/***************************************************************************
  Begin        	:	18.06.2009 / B. Tschunko
  Version       :	03.00.08 / 18.06.2009 / B. Tschunko

  Function		:	calculate checksum 

  Parameters	:	-

  Return value	:	unsigned int uiCrc	- checksum
***************************************************************************/
unsigned int FedmIscMyAxxessReader::CalcCRC16CheckSum(unsigned short int uiCrcStart, 
													  unsigned short int uiCrcPoly, 
													  unsigned char* ucBuffer, 
													  unsigned int uiBufLen)
{
	unsigned short int uiCrc = uiCrcStart;
	unsigned int i, j;

	for(i=0;i<uiBufLen;i++)
	{
		uiCrc ^= ucBuffer[i];

		for(j=0; j<8; j++)
		{
			if(uiCrc & 0x0001)
			{
				uiCrc = (unsigned short int)((uiCrc >> 1) ^ uiCrcPoly);
			}
			else
			{
				uiCrc = (unsigned short int)(uiCrc >> 1);
			}
		}
	}
	return uiCrc;
}

int FedmIscMyAxxessReader::BuildTableItem(vector<string> aName, vector<string> aValue, FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM* pItem)
{
	unsigned char ucCnt = 0;
	char* sValue = NULL;
	unsigned int uiValue = 0;

	if (aName.size() != 5)
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	pItem->Init();

	for (ucCnt = 0; ucCnt < aName.size(); ucCnt++)
	{
		FEDM_STRUPR((char *)(aName[ucCnt].c_str()), aName[ucCnt].length()+1);
		if (aName[ucCnt] == "DAYS")
		{
			// if no weekday is listed, then this item contains special time conditions for holidays
			// thus, this is not a bug!
			if(aValue[ucCnt].empty())
			{
				continue;
			}

			FEDM_STRUPR((char *)(aValue[ucCnt].c_str()), aValue[ucCnt].length()+1);
			sValue = strtok((char *)(aValue[ucCnt].c_str()), "+");
			if (sValue != NULL)
			{
				for (int iCnt = 0; iCnt < 7; iCnt++)
				{	
					if(!strcmp(sValue, "MO"))
						pItem->ucDays |= 0x02;
					else if(!strcmp(sValue, "TU"))
						pItem->ucDays |= 0x04;
					else if(!strcmp(sValue, "WE"))
						pItem->ucDays |= 0x08;
					else if(!strcmp(sValue, "TH"))
						pItem->ucDays |= 0x10;
					else if(!strcmp(sValue, "FR"))
						pItem->ucDays |= 0x20;
					else if(!strcmp(sValue, "SA"))
						pItem->ucDays |= 0x40;
					else if (!strcmp(sValue, "SU"))
						pItem->ucDays |= 0x01;
					else
						FEDM_RETURN(FEDM_ERROR_PARAMETER);

					sValue = strtok(NULL, "+");
					if (sValue == NULL)
					{
						break;
					}
				}
				// too may days
				if (sValue != NULL)
				{
					FEDM_RETURN(FEDM_ERROR_PARAMETER);
				}
			}
			else
			{
				FEDM_RETURN(FEDM_ERROR_PARAMETER);
			}
			continue;
		}
		else if (aName[ucCnt] == "DATEFROM")
		{
			sValue = strtok((char *)(aValue[ucCnt].c_str()), ".");
			if((sValue != NULL) && ((strlen(sValue)) != (aValue[ucCnt].length())))
			{
				sscanf(sValue, "%d", &uiValue);
				if ((uiValue < 1) || (uiValue > 31))
				{
					FEDM_RETURN(FEDM_ERROR_PARAMETER);
				}
				else
				{
					pItem->ucStartDate_Day = (unsigned char)(uiValue);
				}

				sValue = strtok(NULL, ".");
				if(sValue != NULL)
				{
					sscanf(sValue, "%d", &uiValue);
					if ((uiValue < 1) || (uiValue > 12))
					{
						FEDM_RETURN(FEDM_ERROR_PARAMETER);
					}
					else
					{
						pItem->ucStartDate_Month = (unsigned char)(uiValue);
					}

					sValue = strtok(NULL, ".");
					if(sValue != NULL)
					{
						sscanf(sValue, "%d", &uiValue);
						if (uiValue > 99)
						{
							if ((uiValue < 2000) || (uiValue > 2099))
							{
								FEDM_RETURN(FEDM_ERROR_PARAMETER);
							}
							else
							{
								pItem->ucStartDate_Year = (unsigned char)(uiValue - 2000);
								continue;
							}
						}
						else
						{
							pItem->ucStartDate_Year = (unsigned char)(uiValue);
							continue;
						}
					}
					else
					{
						FEDM_RETURN(FEDM_ERROR_PARAMETER);
					}
				}
				else
				{
					FEDM_RETURN(FEDM_ERROR_PARAMETER);
				}
			}
			else
			{
				sValue = strtok((char *)(aValue[ucCnt].c_str()), "-");
				if((sValue != NULL) && ((strlen(sValue)) != (aValue[ucCnt].length())))
				{
					sscanf(sValue, "%d", &uiValue);
					if (uiValue > 99)
					{
						if ((uiValue < 2000) || (uiValue > 2099))
						{
							FEDM_RETURN(FEDM_ERROR_PARAMETER);
						}
						else
						{
							pItem->ucStartDate_Year = (unsigned char)(uiValue - 2000);
						}
					}
					else
					{
						pItem->ucStartDate_Year = (unsigned char)(uiValue);
					}

					sValue = strtok(NULL, "-");
					if(sValue != NULL)
					{
						sscanf(sValue, "%d", &uiValue);
						if ((uiValue < 1) || (uiValue > 12))
						{
							FEDM_RETURN(FEDM_ERROR_PARAMETER);
						}
						else
						{
							pItem->ucStartDate_Month = (unsigned char)(uiValue);
						}

						sValue = strtok(NULL, "-");
						if(sValue != NULL)
						{
							sscanf(sValue, "%d", &uiValue);
							if ((uiValue < 1) || (uiValue > 31))
							{
								FEDM_RETURN(FEDM_ERROR_PARAMETER);
							}
							else
							{
								pItem->ucStartDate_Day = (unsigned char)(uiValue);
								continue;
							}
						}
						else
						{
							FEDM_RETURN(FEDM_ERROR_PARAMETER);
						}
					}
					else
					{
						FEDM_RETURN(FEDM_ERROR_PARAMETER);
					}
				}
				else
				{
					sValue = strtok((char *)(aValue[ucCnt].c_str()), "/");
					if((sValue != NULL) && ((strlen(sValue)) != (aValue[ucCnt].length())))
					{
						sscanf(sValue, "%d", &uiValue);
						if ((uiValue < 1) || (uiValue > 12))
						{
							FEDM_RETURN(FEDM_ERROR_PARAMETER);
						}
						else
						{
							pItem->ucStartDate_Month = (unsigned char)(uiValue);
						}

						sValue = strtok(NULL, "/");
						if(sValue != NULL)
						{
							sscanf(sValue, "%d", &uiValue);
							if ((uiValue < 1) || (uiValue > 31))
							{
								FEDM_RETURN(FEDM_ERROR_PARAMETER);
							}
							else
							{
								pItem->ucStartDate_Day = (unsigned char)(uiValue);
							}

							sValue = strtok(NULL, "/");
							if(sValue != NULL)
							{
								sscanf(sValue, "%d", &uiValue);
								if (uiValue > 99)
								{
									if ((uiValue < 2000) || (uiValue > 2099))
									{
										FEDM_RETURN(FEDM_ERROR_PARAMETER);
									}
									else
									{
										pItem->ucStartDate_Year = (unsigned char)(uiValue - 2000);
										continue;
									}
								}
								else
								{
									pItem->ucStartDate_Year = (unsigned char)(uiValue);
									continue;
								}
							}
							else
							{
								FEDM_RETURN(FEDM_ERROR_PARAMETER);
							}
						}
						else
						{
							FEDM_RETURN(FEDM_ERROR_PARAMETER);
						}
					}
					else
					{
						FEDM_RETURN(FEDM_ERROR_PARAMETER);
					}
				}
			}
		}
		else if (aName[ucCnt] == "TIMEFROM")
		{
			sValue = strtok((char *)(aValue[ucCnt].c_str()), ":");
			if(sValue != NULL)
			{
				sscanf(sValue, "%d", &uiValue);
				if (uiValue > 23)
				{
					FEDM_RETURN(FEDM_ERROR_PARAMETER);
				}
				else
				{
					pItem->ucStartTime_Hour = (unsigned char)(uiValue);
				}

				sValue = strtok(NULL, ":");
				if(sValue != NULL)
				{
					sscanf(sValue, "%d", &uiValue);
					if (uiValue > 59)
					{
						FEDM_RETURN(FEDM_ERROR_PARAMETER);
					}
					else
					{
						pItem->ucStartTime_Minute = (unsigned char)(uiValue);
						continue;
					}
				}
				else
				{
					FEDM_RETURN(FEDM_ERROR_PARAMETER);
				}
			}
			else
			{
				FEDM_RETURN(FEDM_ERROR_PARAMETER);
			}
		}
		else if (aName[ucCnt] == "DATETO")
		{
			sValue = strtok((char *)(aValue[ucCnt].c_str()), ".");
			if((sValue != NULL) && ((strlen(sValue)) != (aValue[ucCnt].length())))
			{
				sscanf(sValue, "%d", &uiValue);
				if ((uiValue < 1) || (uiValue > 31))
				{
					FEDM_RETURN(FEDM_ERROR_PARAMETER);
				}
				else
				{
					pItem->ucEndDate_Day = (unsigned char)(uiValue);
				}

				sValue = strtok(NULL, ".");
				if(sValue != NULL)
				{
					sscanf(sValue, "%d", &uiValue);
					if ((uiValue < 1) || (uiValue > 12))
					{
						FEDM_RETURN(FEDM_ERROR_PARAMETER);
					}
					else
					{
						pItem->ucEndDate_Month = (unsigned char)(uiValue);
					}

					sValue = strtok(NULL, ".");
					if(sValue != NULL)
					{
						sscanf(sValue, "%d", &uiValue);
						if (uiValue > 99)
						{
							if ((uiValue < 2000) || (uiValue > 2099))
							{
								FEDM_RETURN(FEDM_ERROR_PARAMETER);
							}
							else
							{
								pItem->ucEndDate_Year = (unsigned char)(uiValue - 2000);
								continue;
							}
						}
						else
						{
							pItem->ucEndDate_Year = (unsigned char)(uiValue);
							continue;
						}
					}
					else
					{
						FEDM_RETURN(FEDM_ERROR_PARAMETER);
					}
				}
				else
				{
					FEDM_RETURN(FEDM_ERROR_PARAMETER);
				}
			}
			else
			{
				sValue = strtok((char *)(aValue[ucCnt].c_str()), "-");
				if((sValue != NULL) && ((strlen(sValue)) != (aValue[ucCnt].length())))
				{
					sscanf(sValue, "%d", &uiValue);
					if (uiValue > 99)
					{
						if ((uiValue < 2000) || (uiValue > 2099))
						{
							FEDM_RETURN(FEDM_ERROR_PARAMETER);
						}
						else
						{
							pItem->ucEndDate_Year = (unsigned char)(uiValue - 2000);
						}
					}
					else
					{
						pItem->ucEndDate_Year = (unsigned char)(uiValue);
					}

					sValue = strtok(NULL, "-");
					if(sValue != NULL)
					{
						sscanf(sValue, "%d", &uiValue);
						if ((uiValue < 1) || (uiValue > 12))
						{
							FEDM_RETURN(FEDM_ERROR_PARAMETER);
						}
						else
						{
							pItem->ucEndDate_Month = (unsigned char)(uiValue);
						}

						sValue = strtok(NULL, "-");
						if(sValue != NULL)
						{
							sscanf(sValue, "%d", &uiValue);
							if ((uiValue < 1) || (uiValue > 31))
							{
								FEDM_RETURN(FEDM_ERROR_PARAMETER);
							}
							else
							{
								pItem->ucEndDate_Day = (unsigned char)(uiValue);
								continue;
							}
						}
						else
						{
							FEDM_RETURN(FEDM_ERROR_PARAMETER);
						}
					}
					else
					{
						FEDM_RETURN(FEDM_ERROR_PARAMETER);
					}
				}
				else
				{
					sValue = strtok((char *)(aValue[ucCnt].c_str()), "/");
					if((sValue != NULL) && ((strlen(sValue)) != (aValue[ucCnt].length())))
					{
						sscanf(sValue, "%d", &uiValue);
						if ((uiValue < 1) || (uiValue > 12))
						{
							FEDM_RETURN(FEDM_ERROR_PARAMETER);
						}
						else
						{
							pItem->ucEndDate_Month = (unsigned char)(uiValue);
						}

						sValue = strtok(NULL, "/");
						if(sValue != NULL)
						{
							sscanf(sValue, "%d", &uiValue);
							if ((uiValue < 1) || (uiValue > 31))
							{
								FEDM_RETURN(FEDM_ERROR_PARAMETER);
							}
							else
							{
								pItem->ucEndDate_Day = (unsigned char)(uiValue);
							}

							sValue = strtok(NULL, "/");
							if(sValue != NULL)
							{
								sscanf(sValue, "%d", &uiValue);
								if (uiValue > 99)
								{
									if ((uiValue < 2000) || (uiValue > 2099))
									{
										FEDM_RETURN(FEDM_ERROR_PARAMETER);
									}
									else
									{
										pItem->ucEndDate_Year = (unsigned char)(uiValue - 2000);
										continue;
									}
								}
								else
								{
									pItem->ucEndDate_Year = (unsigned char)(uiValue);
									continue;
								}
							}
							else
							{
								FEDM_RETURN(FEDM_ERROR_PARAMETER);
							}
						}
						else
						{
							FEDM_RETURN(FEDM_ERROR_PARAMETER);
						}
					}
					else
					{
						FEDM_RETURN(FEDM_ERROR_PARAMETER);
					}
				}
			}
		}
		else if (aName[ucCnt] =="TIMETO")
		{
			sValue = strtok((char *)(aValue[ucCnt].c_str()), ":");
			if(sValue != NULL)
			{
				sscanf(sValue, "%d", &uiValue);
				if (uiValue > 23)
				{
					FEDM_RETURN(FEDM_ERROR_PARAMETER);
				}
				else
				{
					pItem->ucEndTime_Hour = (unsigned char)(uiValue);
				}

				sValue = strtok(NULL, ":");
				if(sValue != NULL)
				{
					sscanf(sValue, "%d", &uiValue);
					if (uiValue > 59)
					{
						FEDM_RETURN(FEDM_ERROR_PARAMETER);
					}
					else
					{
						pItem->ucEndTime_Minute = (unsigned char)(uiValue);
						continue;
					}
				}
				else
				{
					FEDM_RETURN(FEDM_ERROR_PARAMETER);
				}
			}
			else
			{
				FEDM_RETURN(FEDM_ERROR_PARAMETER);
			}
		}
		else
		{
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
	}

	FEDM_RETURN(FEDM_OK);
}

int FedmIscMyAxxessReader::BuildTableItem(vector<string> aName, vector<string> aValue, FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM* pItem)
{
	unsigned char ucCnt = 0;
	char* sValue = NULL;
	unsigned int uiValue = 0;

	if (aName.size() != 1)
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	pItem->Init();

	FEDM_STRUPR((char *)(aName[ucCnt].c_str()), aName[ucCnt].length()+1);
	if (aName[ucCnt] == "HOLIDAY")
	{
		sValue = strtok((char *)(aValue[ucCnt].c_str()), ".");
		if((sValue != NULL) && ((strlen(sValue)) != (aValue[ucCnt].length())))
		{
			sscanf(sValue, "%d", &uiValue);
			if ((uiValue < 1) || (uiValue > 31))
			{
				FEDM_RETURN(FEDM_ERROR_PARAMETER);
			}
			else
			{
				pItem->ucHoliday_Day = (unsigned char)(uiValue);
			}

			sValue = strtok(NULL, ".");
			if(sValue != NULL)
			{
				sscanf(sValue, "%d", &uiValue);
				if ((uiValue < 1) || (uiValue > 12))
				{
					FEDM_RETURN(FEDM_ERROR_PARAMETER);
				}
				else
				{
					pItem->ucHoliday_Month = (unsigned char)(uiValue);
				}

				sValue = strtok(NULL, ".");
				if(sValue != NULL)
				{
					sscanf(sValue, "%d", &uiValue);
					if (uiValue > 99)
					{
						if ((uiValue < 2000) || (uiValue > 2099))
						{
							FEDM_RETURN(FEDM_ERROR_PARAMETER);
						}
						else
						{
							pItem->ucHoliday_Year = (unsigned char)(uiValue - 2000);
							//continue;
						}
					}
					else
					{
						pItem->ucHoliday_Year = (unsigned char)(uiValue);
						//continue;
					}
				}
				else
				{
					FEDM_RETURN(FEDM_ERROR_PARAMETER);
				}
			}
			else
			{
				FEDM_RETURN(FEDM_ERROR_PARAMETER);
			}
		}
		else
		{
			sValue = strtok((char *)(aValue[ucCnt].c_str()), "-");
			if((sValue != NULL) && ((strlen(sValue)) != (aValue[ucCnt].length())))
			{
				sscanf(sValue, "%d", &uiValue);
				if (uiValue > 99)
				{
					if ((uiValue < 2000) || (uiValue > 2099))
					{
						FEDM_RETURN(FEDM_ERROR_PARAMETER);
					}
					else
					{
						pItem->ucHoliday_Year = (unsigned char)(uiValue - 2000);
						//continue;
					}
				}
				else
				{
					pItem->ucHoliday_Year = (unsigned char)(uiValue);
				}

				sValue = strtok(NULL, "-");
				if(sValue != NULL)
				{
					sscanf(sValue, "%d", &uiValue);
					if ((uiValue < 1) || (uiValue > 12))
					{
						FEDM_RETURN(FEDM_ERROR_PARAMETER);
					}
					else
					{
						pItem->ucHoliday_Month = (unsigned char)(uiValue);
					}

					sValue = strtok(NULL, "-");
					if(sValue != NULL)
					{
						sscanf(sValue, "%d", &uiValue);
						if ((uiValue < 1) || (uiValue > 31))
						{
							FEDM_RETURN(FEDM_ERROR_PARAMETER);
						}
						else
						{
							pItem->ucHoliday_Day = (unsigned char)(uiValue);
							//continue;
						}
					}
					else
					{
						FEDM_RETURN(FEDM_ERROR_PARAMETER);
					}
				}
				else
				{
					FEDM_RETURN(FEDM_ERROR_PARAMETER);
				}
			}
			else
			{
				sValue = strtok((char *)(aValue[ucCnt].c_str()), "/");
				if((sValue != NULL) && ((strlen(sValue)) != (aValue[ucCnt].length())))
				{
					sscanf(sValue, "%d", &uiValue);
					if ((uiValue < 1) || (uiValue > 12))
					{
						FEDM_RETURN(FEDM_ERROR_PARAMETER);
					}
					else
					{
						pItem->ucHoliday_Month = (unsigned char)(uiValue);
					}

					sValue = strtok(NULL, "/");
					if(sValue != NULL)
					{
						sscanf(sValue, "%d", &uiValue);
						if ((uiValue < 1) || (uiValue > 31))
						{
							FEDM_RETURN(FEDM_ERROR_PARAMETER);
						}
						else
						{
							pItem->ucHoliday_Day = (unsigned char)(uiValue);
						}

						sValue = strtok(NULL, "/");
						if(sValue != NULL)
						{
							sscanf(sValue, "%d", &uiValue);
							if (uiValue > 99)
							{
								if ((uiValue < 2000) || (uiValue > 2099))
								{
									FEDM_RETURN(FEDM_ERROR_PARAMETER);
								}
								else
								{
									pItem->ucHoliday_Year = (unsigned char)(uiValue - 2000);
									//continue;
								}
							}
							else
							{
								pItem->ucHoliday_Year = (unsigned char)(uiValue);
								//continue;
							}
						}
						else
						{
							FEDM_RETURN(FEDM_ERROR_PARAMETER);
						}
					}
					else
					{
						FEDM_RETURN(FEDM_ERROR_PARAMETER);
					}
				}
				else
				{
					FEDM_RETURN(FEDM_ERROR_PARAMETER);
				}
			}
		}
	}
	else
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	FEDM_RETURN(FEDM_OK);
}

int FedmIscMyAxxessReader::BuildTableItem(vector<string> aName, vector<string> aValue, FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM* pItem)
{
	unsigned char ucCnt = 0;
	char* sValue = NULL;
	unsigned int uiValue = 0;

	unsigned char ucByte = 0;
	unsigned __int64 ui64Value = 0;

	bool bOptReserved = false;

	string sIDD;

	if ((aName.size() == 2))
	{
		bOptReserved = false;
	}
	else if ((aName.size() == 3))
	{
		bOptReserved = true;
	}
	else
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	pItem ->Init();

	for (ucCnt = 0; ucCnt < aName.size(); ucCnt++)
	{
		FEDM_STRUPR((char *)(aName[ucCnt].c_str()), aName[ucCnt].length()+1);
		if (aName[ucCnt] == "IDD")
		{
			switch(m_Metadata.ucIDDFormat)
			{
			case FEDM_MYAXXESS_IDD_FORMAT_ASCII:
				if(aValue[ucCnt].length() != m_Metadata.ucIDDLength)
				{
					FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
				}

				memcpy (pItem->ucIDD, aValue[ucCnt].c_str(), m_Metadata.ucIDDLength);
				break;

			case FEDM_MYAXXESS_IDD_FORMAT_NUM:
				if(aValue[ucCnt].length() > 20)
				{
					FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
				}

				sscanf(aValue[ucCnt].c_str(), "%lld", &ui64Value);
				for (ucByte = 0; ucByte < m_Metadata.ucIDDLength; ucByte++)
				{
					// change intel in motorola format
					pItem->ucIDD[m_Metadata.ucIDDLength-ucByte-1] = *( ((unsigned char*)&ui64Value) + ucByte );
				}
				if ((ucByte < 8) && (*( ((unsigned char*)&ui64Value) + ucByte ) != 0))
				{
					FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
				}

				// set all not used byte to zero
				memset(&pItem->ucIDD[m_Metadata.ucIDDLength], 0, 128-m_Metadata.ucIDDLength);
				break;

			case FEDM_MYAXXESS_IDD_FORMAT_HEX:
				if( (aValue[ucCnt].length()%2) ||		// IDD not even
					(aValue[ucCnt].length() == 0) ||	// no IDD string
					(aValue[ucCnt].length() > (size_t)(m_Metadata.ucIDDLength<<1)) ) // IDD too long
				{
					FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
				}
				else if(aValue[ucCnt].length() == (size_t)(m_Metadata.ucIDDLength<<1))
				{
					for(size_t i=aValue[ucCnt].length(); i<(size_t)(m_Metadata.ucIDDLength<<1); i+=2)
						sIDD += "00";

					sIDD += aValue[ucCnt];
				}
				else
				{
					sIDD = aValue[ucCnt];
				}

				FEDM_ConvHexCharToHexUChar((char *)sIDD.c_str(), (m_Metadata.ucIDDLength<<1), pItem->ucIDD, 128);
				break;

			default:
				FEDM_RETURN(FEDM_ERROR_PARAMETER);

			}
			continue;
		}
		else if (aName[ucCnt] == "TIMEZONES")
		{
			sValue = strtok((char *)(aValue[ucCnt].c_str()), "+");
			if (sValue != NULL)
			{
				for (int iCnt = 0; iCnt < 16; iCnt++)
				{
					sscanf(sValue, "%d", &uiValue);
					if ((uiValue > 0) && (uiValue < 17))
					{
						// set bit for timezone
						pItem->uiTimezones |= (0x01<<(uiValue-1));
					}
					else
					{
							FEDM_RETURN(FEDM_ERROR_PARAMETER);
					}

					sValue = strtok(NULL, "+");
					if (sValue == NULL)
					{
						break;
					}
				}
				// too may days
				if (sValue != NULL)
				{
					FEDM_RETURN(FEDM_ERROR_PARAMETER);
				}
			}
			else
			{
				FEDM_RETURN(FEDM_ERROR_PARAMETER);
			}
			continue;

		}
		else if (aName[ucCnt] =="RESERVED")
		{
			if (!bOptReserved)
			{
				FEDM_RETURN(FEDM_ERROR_PARAMETER);
			}

			sscanf(aValue[ucCnt].c_str(), "%d", &uiValue);
			pItem->ucReserved = (unsigned char)(uiValue);
			continue;
		}
		else
		{
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
	}

	FEDM_RETURN(FEDM_OK);
}


int FedmIscMyAxxessReader::GetTableItem_Timezone(unsigned int uiIndex, char* sItem)
{
	unsigned int uiNextIndex = 1;
	bool bAddPlus = false;
	string szDays;

	if ((uiIndex < 1) || (uiIndex > m_TimezoneTable.size()))
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM*	pTimezoneItem	= NULL;
	vector<FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM*>::iterator itorTimezone;

	// search Timezone Table
	for(itorTimezone  = m_TimezoneTable.begin();
		itorTimezone != m_TimezoneTable.end();
		itorTimezone++)
	{
		if ((*itorTimezone) == NULL)
			continue;

		if (uiNextIndex++ != uiIndex)
			continue;

		pTimezoneItem = *itorTimezone;

		// built item
		for (unsigned char ucDay = 0; ucDay < 7; ucDay++)
		{
			if ((pTimezoneItem->ucDays >> ucDay) && 0x01)
			{
				if (bAddPlus)
					szDays += "+";

				switch (ucDay)
				{
				case 0:
					szDays += "Mo";
					break;

				case 1:
					szDays += "Tu";
					break;

				case 2:
					szDays += "We";
					break;

				case 3:
					szDays += "Th";
					break;

				case 4:
					szDays += "Fr";
					break;

				case 5:
					szDays += "Sa";
					break;

				case 6:
					szDays += "Su";
					break;


				}
				bAddPlus = true;
			}
		}

		switch (m_ucDateFormat)
		{
		case FEDM_MYAXXESS_DATE_FORMAT_ISO8601:
			sprintf(sItem, 
					"Days=%s;DateFrom=20%02d-%02d-%02d;TimeFrom=%02d:%02d;DateTo=20%02d-%02d-%02d;TimeTo=%02d:%02d",	
					szDays.c_str(),
					pTimezoneItem->ucStartDate_Year, 
					pTimezoneItem->ucStartDate_Month,
					pTimezoneItem->ucStartDate_Day,
					pTimezoneItem->ucStartTime_Hour,
					pTimezoneItem->ucStartTime_Minute, 
					pTimezoneItem->ucEndDate_Year, 
					pTimezoneItem->ucEndDate_Month,
					pTimezoneItem->ucEndDate_Day,
					pTimezoneItem->ucEndTime_Hour,
					pTimezoneItem->ucEndTime_Minute);
			break;

		case FEDM_MYAXXESS_DATE_FORMAT_GER:
			sprintf(sItem, 
					"Days=%s;DateFrom=%02d.%02d.20%02d;TimeFrom=%02d:%02d;DateTo=%02d.%02d.20%02d;TimeTo=%02d:%02d",	
					szDays.c_str(),
					pTimezoneItem->ucStartDate_Day,
					pTimezoneItem->ucStartDate_Month,
					pTimezoneItem->ucStartDate_Year, 
					pTimezoneItem->ucStartTime_Hour,
					pTimezoneItem->ucStartTime_Minute, 
					pTimezoneItem->ucEndDate_Day,
					pTimezoneItem->ucEndDate_Month,
					pTimezoneItem->ucEndDate_Year, 
					pTimezoneItem->ucEndTime_Hour,
					pTimezoneItem->ucEndTime_Minute);
			break;

		case FEDM_MYAXXESS_DATE_FORMAT_US:
			sprintf(sItem, 
					"Days=%s;DateFrom=%02d/%02d/20%02d;TimeFrom=%02d:%02d;DateTo=%02d/%02d/20%02d;TimeTo=%02d:%02d",	
					szDays.c_str(),
					pTimezoneItem->ucStartDate_Month,
					pTimezoneItem->ucStartDate_Day,
					pTimezoneItem->ucStartDate_Year, 
					pTimezoneItem->ucStartTime_Hour,
					pTimezoneItem->ucStartTime_Minute, 
					pTimezoneItem->ucEndDate_Month,
					pTimezoneItem->ucEndDate_Day,
					pTimezoneItem->ucEndDate_Year, 
					pTimezoneItem->ucEndTime_Hour,
					pTimezoneItem->ucEndTime_Minute);
			break;

		default:
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}

		break;
	}

	if (uiNextIndex > m_TimezoneTable.size())
	{
		uiNextIndex = 0;
	}

	return ((int)uiNextIndex);
}


int FedmIscMyAxxessReader::GetTableItem_Holiday(unsigned int uiIndex, char* sItem)
{
	unsigned int uiNextIndex = 1;

	if ((uiIndex < 1) || (uiIndex > m_HolidayTable.size()))
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM*	pHolidayItem	= NULL;
	vector<FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM*>::iterator itorHoliday;

	// search Holiday Table
	for(itorHoliday  = m_HolidayTable.begin();
		itorHoliday != m_HolidayTable.end();
		itorHoliday++)
	{
		if ((*itorHoliday) == NULL)
			continue;

		if (uiNextIndex++ != uiIndex)
			continue;

		pHolidayItem = *itorHoliday;

		// built item
		switch (m_ucDateFormat)
		{
		case FEDM_MYAXXESS_DATE_FORMAT_ISO8601:
			sprintf(sItem, 
					"Holiday=20%02d-%02d-%02d",	
					pHolidayItem->ucHoliday_Year, 		
					pHolidayItem->ucHoliday_Month,		
					pHolidayItem->ucHoliday_Day);
			break;

		case FEDM_MYAXXESS_DATE_FORMAT_GER:
			sprintf(sItem,
					"Holiday=%02d.%02d.20%02d",
					pHolidayItem->ucHoliday_Day,
					pHolidayItem->ucHoliday_Month,
					pHolidayItem->ucHoliday_Year);
			break;

		case FEDM_MYAXXESS_DATE_FORMAT_US:
			sprintf(sItem, 
					"Holiday=%02d/%02d/20%02d",
					pHolidayItem->ucHoliday_Month,
					pHolidayItem->ucHoliday_Day,
					pHolidayItem->ucHoliday_Year);
			break;

		default:
			FEDM_RETURN(FEDM_ERROR_PARAMETER);

		}
		break;
	}

	if (uiNextIndex > m_HolidayTable.size())
	{
		uiNextIndex = 0;
	}

	return ((int)uiNextIndex);
}


int FedmIscMyAxxessReader::GetTableItem_Access(unsigned int uiIndex, char* sItem)
{
	unsigned int uiNextIndex = 1;
	char szIDD[256];
	unsigned __int64 ui64IDD = 0;
	bool bAddPlus = false;
	char szTimezones[32];

	if ((uiIndex < 1) || (uiIndex > m_AccessTable.size()))
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM*	pAccessItem	= NULL;
	vector<FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM*>::iterator itorAccess;

	// search Access Table
	for(itorAccess  = m_AccessTable.begin();
		itorAccess != m_AccessTable.end();
		itorAccess++)
	{
		if ((*itorAccess) == NULL)
			continue;

		if (uiNextIndex++ != uiIndex)
			continue;

		pAccessItem = *itorAccess;

		// built item
		switch(m_Metadata.ucIDDFormat)
		{
		case FEDM_MYAXXESS_IDD_FORMAT_ASCII:
			memcpy(szIDD, pAccessItem->ucIDD, m_Metadata.ucIDDLength);
			szIDD[m_Metadata.ucIDDLength] = '\0';
			break;

		case FEDM_MYAXXESS_IDD_FORMAT_NUM:
			if(strlen((char*)pAccessItem->ucIDD) > 20)
			{
				FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
			}
			ui64IDD = 0;
			for (unsigned char ucByte = 0; ucByte < m_Metadata.ucIDDLength; ucByte++)
			{
				// change Motorola format to Intel format
				ui64IDD += ( (unsigned __int64)pAccessItem->ucIDD[m_Metadata.ucIDDLength-ucByte-1] << (8*ucByte) );
			}
			sprintf(szIDD,"%lld",ui64IDD);
			break;

		case FEDM_MYAXXESS_IDD_FORMAT_HEX:
			FEDM_ConvHexUCharToHexChar(pAccessItem->ucIDD, m_Metadata.ucIDDLength, szIDD, 256);
			break;

		default:
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}


		for (unsigned char ucCnt = 0; ucCnt < 16; ucCnt++)
		{
			if (pAccessItem->uiTimezones & (0x01 << ucCnt))
			{
				if (bAddPlus)
					sprintf(szTimezones, "%s+%d", szTimezones, ucCnt+1);
				else
					sprintf(szTimezones, "%d", ucCnt+1);
				bAddPlus = true;
			}
		}

		sprintf(sItem, 
				"IDD=%s;Timezones=%s;Reserved=%d",	
				szIDD,
				szTimezones,
				pAccessItem->ucReserved);
		break;
	}

	if (uiNextIndex > m_AccessTable.size())
	{
		uiNextIndex = 0;
	}

	return ((int)uiNextIndex);
}


int FedmIscMyAxxessReader::GetTableItem_Event(char* sItem)
{
	char	szIDD[256];
	char	szTimestamp[64];
	char	szStatus[32];
	char	szInput[32];
	char	szAnt[32];
	unsigned __int64 ui64IDD = 0;

	// search Event Table
	FEDM_ISC_MYAXXESS_EVENT_TABLE_ITEM*	pEventItem	= NULL;

	if(m_EventTable.empty())
	{
		FEDM_RETURN(FEDM_ERROR_NO_TABLE_DATA);
	}

	pEventItem = m_EventTable.front();

	if(pEventItem == NULL)
	{
		FEDM_RETURN(FEDM_ERROR_NULL_POINTER);
	}

	memset(szIDD, 0, 32);
	memset(szTimestamp, 0, 64);
	memset(szStatus, 0, 32);
	memset(szInput, 0, 32);
	memset(szAnt, 0, 32);

	sprintf(sItem,
			"EventID=%d;",	
			pEventItem->ucEventID);

	if(pEventItem->ucDataLayout & 0x01)
	{
		switch(m_Metadata.ucIDDFormat)
		{
		case FEDM_MYAXXESS_IDD_FORMAT_ASCII:
			memcpy(szIDD, pEventItem->ucIDD, pEventItem->ucIDDLength);
			szIDD[pEventItem->ucIDDLength] = '\0';
			break;

		case FEDM_MYAXXESS_IDD_FORMAT_NUM:
			if(strlen((char*)pEventItem->ucIDD) > 20)
			{
				m_EventTable.pop();
				FEDM_RETURN(FEDM_ERROR_STRING_LENGTH);
			}	
			ui64IDD = 0;
			for(unsigned char ucByte=0; ucByte<pEventItem->ucIDDLength; ucByte++ )
			{
				// change Motorola format to Intel format
				ui64IDD += ( (unsigned __int64)pEventItem->ucIDD[pEventItem->ucIDDLength-ucByte-1] << (8*ucByte) );
			}
			sprintf(szIDD,"%lld",ui64IDD);
			break;

		case FEDM_MYAXXESS_IDD_FORMAT_HEX:
			FEDM_ConvHexUCharToHexChar(pEventItem->ucIDD, pEventItem->ucIDDLength, szIDD, 256);
			break;

		default:
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}

		strcat(sItem, "IDD=");
		strcat(sItem, szIDD);
		strcat(sItem, ";");
	}

	if(pEventItem->ucDataLayout & 0x02)
	{
		switch (m_ucDateFormat)
		{
		case FEDM_MYAXXESS_DATE_FORMAT_ISO8601:
			sprintf(szTimestamp, 
					"Date=20%02d-%02d-%02d;Time=%02d:%02d;",	
					pEventItem->ucYear, 
					pEventItem->ucMonth, 
					pEventItem->ucDay, 
					pEventItem->ucHour, 
					pEventItem->ucMinute);
			break;

		case FEDM_MYAXXESS_DATE_FORMAT_GER:
			sprintf(szTimestamp, 
					"Date=%02d.%02d.20%02d;Time=%02d:%02d;",
					pEventItem->ucDay, 
					pEventItem->ucMonth, 
					pEventItem->ucYear,
					pEventItem->ucHour, 
					pEventItem->ucMinute);
			break;

		case FEDM_MYAXXESS_DATE_FORMAT_US:
			sprintf(szTimestamp,
					"Date=%02d/%02d/20%02d;Time=%02d:%02d;",
					pEventItem->ucMonth,
					pEventItem->ucDay, 
					pEventItem->ucYear,
					pEventItem->ucHour, 
					pEventItem->ucMinute);
			break;

		default:
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}

		strcat(sItem, szTimestamp);
	}

	if(pEventItem->ucDataLayout & 0x08)
	{
		sprintf(szInput,
				"Input=%d;",
				pEventItem->ucInput);

		strcat(sItem, szInput);
	}

	if(pEventItem->ucDataLayout & 0x10)
	{
		sprintf(szAnt,
				"Ant=%d;",
				pEventItem->ucAnt);

		strcat(sItem, szAnt);
	}

	if(pEventItem->ucDataLayout & 0x04)
	{
		sprintf(szStatus,
				"Action=%d;Error=%i;",	
				pEventItem->ucAction,
				pEventItem->uiError);

		strcat(sItem, szStatus);
	}

	strcat(sItem, "Source=");
	strcat(sItem, pEventItem->sSourceIP.c_str());

	// item must be deleted, because item is no longer controlled by the queue
	m_EventTable.pop();

	FEDM_RETURN(FEDM_OK);
}


int FedmIscMyAxxessReader::WriteTables_CrcList()
{
	int iOutLen = 0;
	int iInLen = 0;

	int iBack = 0;

	m_ucOut[iOutLen++] = (unsigned char)((m_CrcList.uiCrcMetadata & 0xFF00) >> 8);	// CRC of Metadata
	m_ucOut[iOutLen++] = (unsigned char)(m_CrcList.uiCrcMetadata & 0x00FF);
	m_ucOut[iOutLen++] = (unsigned char)((m_CrcList.uiCrcTimezone & 0xFF00) >> 8);	// CRC of Timzone Table
	m_ucOut[iOutLen++] = (unsigned char)(m_CrcList.uiCrcTimezone & 0x00FF);
	m_ucOut[iOutLen++] = (unsigned char)((m_CrcList.uiCrcHoliday & 0xFF00) >> 8);	// CRC of Holiday Table
	m_ucOut[iOutLen++] = (unsigned char)(m_CrcList.uiCrcHoliday & 0x00FF);
	m_ucOut[iOutLen++] = (unsigned char)((m_CrcList.uiCrcAccess & 0xFF00) >> 8);	// CRC of Access Table
	m_ucOut[iOutLen++] = (unsigned char)(m_CrcList.uiCrcAccess & 0x00FF);

	// send end update protocol
	iBack = FEISC_0x1F_MAXDataExchange(	m_pReaderModule->m_iReaderHnd, 
										m_pReaderModule->m_ucBusAdr, 
										FEDM_MYAXXESS_UPDATE_END, 
										0x00, 
										FEDM_MYAXXESS_CRC_LIST, 
										m_ucOut, 
										iOutLen, 
										m_ucIn, 
										&iInLen, 
										0x00 );
	if(iBack != 0)
	{
		return iBack;
	}

	FEDM_RETURN(FEDM_OK);
}

int FedmIscMyAxxessReader::WriteTables_Metadata()
{
	int iOutLen = 0;
	int iInLen = 0;

	int iBack = 0;

	// calculate Metadata
	m_Metadata.uiTableVersion = FEDM_MYAXXESS_TABLE_VERSION;
	m_Metadata.ucTimezoneEntries = (unsigned char)m_TimezoneTable.size();
	m_Metadata.ucHolidayEntries = (unsigned char)m_HolidayTable.size();
	m_Metadata.uiAccessEntries = (unsigned short int)m_AccessTable.size();

	if (m_Metadata.ucTimezoneEntries > 15)
		FEDM_RETURN(FEDM_ERROR_PARAMETER);

	if (m_Metadata.ucHolidayEntries > 256)
		FEDM_RETURN(FEDM_ERROR_PARAMETER);

	if (m_Metadata.uiAccessEntries > 65535)
		FEDM_RETURN(FEDM_ERROR_PARAMETER);

	if ((m_Metadata.ucIDDLength < 1) || (m_Metadata.ucIDDLength > 64))
		FEDM_RETURN(FEDM_ERROR_PARAMETER);

	if ((m_Metadata.ucIDDFormat < 1) || (m_Metadata.ucIDDFormat > 3))
		FEDM_RETURN(FEDM_ERROR_PARAMETER);

	m_CrcList.uiCrcMetadata = 0xFFFF;	// set startvaule for crc

	// load Metadata
	m_ucOut[iOutLen++] = (unsigned char)((m_Metadata.uiTableVersion & 0xFF00) >> 8);
	m_ucOut[iOutLen++] = (unsigned char)(m_Metadata.uiTableVersion & 0x00FF);
	m_ucOut[iOutLen++] = m_Metadata.ucTimezoneEntries;
	m_ucOut[iOutLen++] = m_Metadata.ucHolidayEntries;
	m_ucOut[iOutLen++] = (unsigned char)((m_Metadata.uiAccessEntries & 0xFF00) >> 8);
	m_ucOut[iOutLen++] = (unsigned char)(m_Metadata.uiAccessEntries & 0x00FF);
	m_ucOut[iOutLen++] = m_Metadata.ucIDDLength;
	m_ucOut[iOutLen++] = m_Metadata.ucIDDFormat;
	m_ucOut[iOutLen++] = m_Metadata.ucReserved[0];
	m_ucOut[iOutLen++] = m_Metadata.ucReserved[1];
	m_ucOut[iOutLen++] = m_Metadata.ucReserved[2];
	m_ucOut[iOutLen++] = m_Metadata.ucReserved[3];
	m_ucOut[iOutLen++] = m_Metadata.ucReserved[4];
	m_ucOut[iOutLen++] = m_Metadata.ucReserved[5];
	m_ucOut[iOutLen++] = m_Metadata.ucReserved[6];
	m_ucOut[iOutLen++] = m_Metadata.ucReserved[7];

	m_CrcList.uiCrcMetadata = CalcCRC16CheckSum(m_CrcList.uiCrcMetadata, 0x1021, &m_ucOut[0], iOutLen);

	// send start update protocol
	iBack = FEISC_0x1F_MAXDataExchange(	m_pReaderModule->m_iReaderHnd,
										m_pReaderModule->m_ucBusAdr, 
										FEDM_MYAXXESS_UPDATE_START, 
										0x00, 
										FEDM_MYAXXESS_METADATA, 
										m_ucOut, 
										iOutLen, 
										m_ucIn, 
										&iInLen, 
										0x00 );
	if(iBack != 0)
	{
		FEDM_RETURN(iBack);
	}

	if(iInLen < 4)
	{
		FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
	}

	// 2 byte package length, 2 byte timeout
	m_uiPackageLen	= (m_ucIn[0] << 8) + m_ucIn[1];
	m_uiTimeout		= (m_ucIn[2] << 8) + m_ucIn[3];

	FEDM_RETURN(FEDM_OK);
}


int FedmIscMyAxxessReader::WriteTables_Timezone()
{
	int iOutLen = 0;
	int iInLen = 0;

	int iBack = 0;

	unsigned int uiDataSets = 0;
	unsigned char ucLen = 0;

	FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM*	pTimezoneItem	= NULL;
	vector<FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM*>::iterator itorTimezone;

	m_CrcList.uiCrcTimezone = 0xFFFF;	// set startvaule for crc
	iOutLen = 3;
	uiDataSets = 0;

	// load Timezone Table
	for(itorTimezone  = m_TimezoneTable.begin();
		itorTimezone != m_TimezoneTable.end();
		itorTimezone++)
	{
		if ((*itorTimezone) == NULL)
			continue;

		pTimezoneItem = *itorTimezone;

/*		m_ucOut[iOutLen++] = pTimezoneItem->ucStartDate_Day;
		m_ucOut[iOutLen++] = pTimezoneItem->ucStartDate_Month;
		m_ucOut[iOutLen++] = (pTimezoneItem->ucStartDate_Year;
		m_ucOut[iOutLen++] = pTimezoneItem->ucEndDate_Day;
		m_ucOut[iOutLen++] = pTimezoneItem->ucEndDate_Month;
		m_ucOut[iOutLen++] = pTimezoneItem->ucEndDate_Year;
		m_ucOut[iOutLen++] = pTimezoneItem->ucWeekdays;
		m_ucOut[iOutLen++] = pTimezoneItem->ucStartTime_Hour;
		m_ucOut[iOutLen++] = pTimezoneItem->ucStartTime_Minute;
		m_ucOut[iOutLen++] = pTimezoneItem->ucEndTime_Hour;
		m_ucOut[iOutLen++] = pTimezoneItem->ucEndTime_Minute;
*/
		memcpy(&m_ucOut[iOutLen], (unsigned char*)pTimezoneItem, sizeof(FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM));
		iOutLen += sizeof(FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM);
		uiDataSets++;

		if ( (iOutLen + sizeof(FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM) + 15) > m_uiPackageLen )
		{
			m_ucOut[0] = (unsigned char)((uiDataSets & 0xFF00) >> 8);	// number of dataset
			m_ucOut[1] = (unsigned char)(uiDataSets & 0x00FF);
			m_ucOut[2] = sizeof(FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM);	// length of each dataset

			m_CrcList.uiCrcTimezone = CalcCRC16CheckSum(m_CrcList.uiCrcTimezone, 0x1021, &m_ucOut[3], (iOutLen-3));

			// send Timezone Table with MORE Flag
			iBack = FEISC_0x1F_MAXDataExchange(	m_pReaderModule->m_iReaderHnd,
												m_pReaderModule->m_ucBusAdr,
												FEDM_MYAXXESS_WRITE_TABLE, 
												0x80, 
												FEDM_MYAXXESS_TIMEZONE_TABLE, 
												m_ucOut, 
												iOutLen, 
												m_ucIn, 
												&iInLen, 
												0x00 );
			if(iBack != 0)
			{
				FEDM_RETURN(iBack);
			}
			iOutLen = 3;
			uiDataSets = 0;
		}		
	}

	m_ucOut[0] = (unsigned char)((uiDataSets & 0xFF00) >> 8);	// number of dataset
	m_ucOut[1] = (unsigned char)(uiDataSets & 0x00FF);
	m_ucOut[2] = sizeof(FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM);	// length of each dataset

	m_CrcList.uiCrcTimezone = CalcCRC16CheckSum(m_CrcList.uiCrcTimezone, 0x1021, &m_ucOut[3], (iOutLen-3));

	// send Timezone Table
	iBack = FEISC_0x1F_MAXDataExchange(	m_pReaderModule->m_iReaderHnd, 
										m_pReaderModule->m_ucBusAdr, 
										FEDM_MYAXXESS_WRITE_TABLE, 
										0x00, 
										FEDM_MYAXXESS_TIMEZONE_TABLE, 
										m_ucOut, 
										iOutLen, 
										m_ucIn, 
										&iInLen, 
										0x00 );
	if(iBack != 0)
	{
		return iBack;
	}

	FEDM_RETURN(FEDM_OK);
}


int FedmIscMyAxxessReader::WriteTables_Holiday()
{
	int iOutLen = 0;
	int iInLen = 0;

	int iBack = 0;

	unsigned int uiDataSets = 0;
	unsigned char ucLen = 0;

	FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM*	pHolidayItem	= NULL;
	vector<FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM*>::iterator	itorHoliday;

	m_CrcList.uiCrcHoliday = 0xFFFF;	// set startvaule for crc
	iOutLen = 3;
	uiDataSets = 0;

	// load Holiday Table
	for(itorHoliday  = m_HolidayTable.begin();
		itorHoliday != m_HolidayTable.end();
		itorHoliday++)
	{
		if ((*itorHoliday) == NULL)
			continue;

		pHolidayItem = *itorHoliday;

/*		m_ucOut[iOutLen++] = pHolidayItem->ucHoliday_Day;
		m_ucOut[iOutLen++] = pHolidayItem->ucHoliday_Month;
		m_ucOut[iOutLen++] = pHolidayItem->ucHoliday_Year;
*/
		memcpy(&m_ucOut[iOutLen], (unsigned char*)pHolidayItem, sizeof(FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM));
		iOutLen += sizeof(FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM);
		uiDataSets++;
		if ( (iOutLen + sizeof(FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM) + 15) > m_uiPackageLen )
		{
			m_ucOut[0] = (unsigned char)((uiDataSets & 0xFF00) >> 8);	// number of dataset
			m_ucOut[1] = (unsigned char)(uiDataSets & 0x00FF);
			m_ucOut[2] = sizeof(FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM);	// length of each dataset

			m_CrcList.uiCrcHoliday = CalcCRC16CheckSum(m_CrcList.uiCrcHoliday, 0x1021, &m_ucOut[3], (iOutLen-3));

			// send Holiday Table with MORE Flag
			iBack = FEISC_0x1F_MAXDataExchange(	m_pReaderModule->m_iReaderHnd, 
												m_pReaderModule->m_ucBusAdr, 
												FEDM_MYAXXESS_WRITE_TABLE, 
												0x80, 
												FEDM_MYAXXESS_HOLIDAY_TABLE, 
												m_ucOut, 
												iOutLen, 
												m_ucIn, 
												&iInLen, 
												0x00 );
			if(iBack != 0)
			{
				return iBack;
			}
			iOutLen = 3;
			uiDataSets = 0;
		}	
	}
	m_ucOut[0] = (unsigned char)((uiDataSets & 0xFF00) >> 8);	// number of dataset
	m_ucOut[1] = (unsigned char)(uiDataSets & 0x00FF);
	m_ucOut[2] = sizeof(FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM);	// length of each dataset

	m_CrcList.uiCrcHoliday = CalcCRC16CheckSum(m_CrcList.uiCrcHoliday, 0x1021, &m_ucOut[3], (iOutLen-3));

	// send Holiday Table
	iBack = FEISC_0x1F_MAXDataExchange(	m_pReaderModule->m_iReaderHnd, 
										m_pReaderModule->m_ucBusAdr, 
										FEDM_MYAXXESS_WRITE_TABLE, 
										0x00, 
										FEDM_MYAXXESS_HOLIDAY_TABLE, 
										m_ucOut, 
										iOutLen, 
										m_ucIn, 
										&iInLen, 
										0x00 );
	if(iBack != 0)
	{
		return iBack;
	}

	FEDM_RETURN(FEDM_OK);
}


int FedmIscMyAxxessReader::WriteTables_Access()
{
	int iOutLen = 0;
	int iInLen = 0;

	int iBack = 0;

	unsigned int uiDataSets = 0;
	unsigned char ucLen = 0;

	FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM*	pAccessItem		= NULL;
	vector<FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM*>::iterator	itorAccess;
	// sort Access Table on numeric IDDs
	map<string, vector<FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM*>::iterator> mapAccess;
	map<string, vector<FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM*>::iterator>::iterator itorAccessMap;
	string sIDD;

	for(itorAccess  = m_AccessTable.begin();
		itorAccess != m_AccessTable.end();
		itorAccess++ )
	{
		pAccessItem = *itorAccess;
		if(pAccessItem == NULL)
			continue;
		FEDM_ConvHexUCharToHexStr(pAccessItem->ucIDD, m_Metadata.ucIDDLength, sIDD);
		mapAccess.insert(make_pair(sIDD, itorAccess));
	}

	m_CrcList.uiCrcAccess = 0xFFFF;	// set startvalue for crc
	iOutLen = 3;
	uiDataSets = 0;

	pAccessItem		= NULL;
	// load AccessData Table
	for(itorAccessMap = mapAccess.begin();
		itorAccessMap != mapAccess.end();
		itorAccessMap++)
	{
		if (*itorAccessMap->second == NULL)
			continue;

		pAccessItem = *itorAccessMap->second;

		if ((iOutLen > 3) && ((iOutLen + (m_Metadata.ucIDDLength + 3) + 15) > (int)(m_uiPackageLen)))
		{
			m_ucOut[0] = (unsigned char)((uiDataSets & 0xFF00) >> 8);	// number of dataset
			m_ucOut[1] = (unsigned char)(uiDataSets & 0x00FF);
			m_ucOut[2] = m_Metadata.ucIDDLength + 3;					// length of each dataset

			m_CrcList.uiCrcAccess = CalcCRC16CheckSum(m_CrcList.uiCrcAccess, 0x1021, &m_ucOut[3], (iOutLen-3));

			// send Holiday Table with MORE Flag
			iBack = FEISC_0x1F_MAXDataExchange(	m_pReaderModule->m_iReaderHnd, 
												m_pReaderModule->m_ucBusAdr, 
												FEDM_MYAXXESS_WRITE_TABLE, 
												0x80, 
												FEDM_MYAXXESS_ACCESS_TABLE, 
												m_ucOut, 
												iOutLen, 
												m_ucIn, 
												&iInLen, 
												0x00 );
			if(iBack != 0)
			{
				return iBack;
			}
			iOutLen = 3;
			uiDataSets = 0;
		}

		// add data
		for (ucLen = 0; ucLen < m_Metadata.ucIDDLength; ucLen++)
			m_ucOut[iOutLen++] = pAccessItem->ucIDD[ucLen];

		m_ucOut[iOutLen++] = (unsigned char)((pAccessItem->uiTimezones & 0xFF00) >> 8);
		m_ucOut[iOutLen++] = (unsigned char)(pAccessItem->uiTimezones & 0x00FF);
		m_ucOut[iOutLen++] = pAccessItem->ucReserved;

		uiDataSets++;
	}

	m_ucOut[0] = (unsigned char)((uiDataSets & 0xFF00) >> 8);	// number of dataset
	m_ucOut[1] = (unsigned char)(uiDataSets & 0x00FF);
	m_ucOut[2] = m_Metadata.ucIDDLength + 3;					// length of each dataset

	m_CrcList.uiCrcAccess = CalcCRC16CheckSum(m_CrcList.uiCrcAccess, 0x1021, &m_ucOut[3], (iOutLen-3));

	// send Access Table
	iBack = FEISC_0x1F_MAXDataExchange(	m_pReaderModule->m_iReaderHnd, 
										m_pReaderModule->m_ucBusAdr, 
										FEDM_MYAXXESS_WRITE_TABLE, 
										0x00, 
										FEDM_MYAXXESS_ACCESS_TABLE, 
										m_ucOut, 
										iOutLen, 
										m_ucIn, 
										&iInLen, 
										0x00 );
	if(iBack != 0)
	{
		return iBack;
	}

	FEDM_RETURN(FEDM_OK);
}

int FedmIscMyAxxessReader::ReadTable_CrcList()
{
	int iOutLen = 0;
	int iInLen = 0;

	int iBack = 0;

	unsigned int uiDataSets = 0;
	unsigned char ucLen = 0;

	m_ucOut[0] = 0;		// first Req-Info
	m_ucOut[1] = 0;
	iOutLen = 2;

	iBack = FEISC_0x1F_MAXDataExchange(	m_pReaderModule->m_iReaderHnd, 
										m_pReaderModule->m_ucBusAdr, 
										FEDM_MYAXXESS_READ_TABLE, 
										0x00, 
										FEDM_MYAXXESS_CRC_LIST, 
										m_ucOut, 
										iOutLen, 
										m_ucIn, 
										&iInLen, 
										0x00 );
	if((iBack != 0))
	{
		return iBack;
	}

	uiDataSets = ((unsigned int)m_ucIn[0] << 8) | (unsigned int)m_ucIn[1];
	ucLen = m_ucIn[2];

	if (uiDataSets != 1)
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	if (ucLen != sizeof(FEDM_ISC_MYAXXESS_CRCLIST))
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}
	
	if (((uiDataSets * ucLen) + 3) != iInLen)
	{
		FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
	}

	m_CrcList.Init();
	// copy collected data to item
	m_CrcList.uiCrcMetadata = ((unsigned short int)m_ucIn[3] << 8) | (unsigned short int)m_ucIn[4];
	m_CrcList.uiCrcTimezone = ((unsigned short int)m_ucIn[5] << 8) | (unsigned short int)m_ucIn[6];
	m_CrcList.uiCrcHoliday = ((unsigned short int)m_ucIn[7] << 8) | (unsigned short int)m_ucIn[8];
	m_CrcList.uiCrcAccess = ((unsigned short int)m_ucIn[9] << 8) | (unsigned short int)m_ucIn[10];

	FEDM_RETURN(FEDM_OK);
}

int FedmIscMyAxxessReader::ReadTable_Metadata()
{

	int iOutLen = 0;
	int iInLen = 0;

	int iBack = 0;

	unsigned int uiDataSets = 0;
	unsigned char ucLen = 0;

	m_ucOut[0] = 0;		// first Req-Info
	m_ucOut[1] = 0;
	iOutLen = 2;

	iBack = FEISC_0x1F_MAXDataExchange(	m_pReaderModule->m_iReaderHnd, 
										m_pReaderModule->m_ucBusAdr, 
										FEDM_MYAXXESS_READ_TABLE, 
										0x00, 
										FEDM_MYAXXESS_METADATA, 
										m_ucOut, 
										iOutLen, 
										m_ucIn, 
										&iInLen, 
										0x00 );
	if((iBack != 0))
	{
		return iBack;
	}

	uiDataSets = ((unsigned int)m_ucIn[0] << 8) | (unsigned int)m_ucIn[1];
	ucLen = m_ucIn[2];

	if (uiDataSets != 1)
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}

	if (ucLen != sizeof(FEDM_ISC_MYAXXESS_METADATA))
	{
		FEDM_RETURN(FEDM_ERROR_PARAMETER);
	}
	
	if (((uiDataSets * ucLen) + 3) != iInLen)
	{
		FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
	}

	m_Metadata.Init();

	// copy collected data to item
	m_Metadata.uiTableVersion = ((unsigned short int)m_ucIn[3] << 8) | (unsigned short int)m_ucIn[4];
	m_Metadata.ucTimezoneEntries = m_ucIn[5];
	m_Metadata.ucHolidayEntries = m_ucIn[6];
	m_Metadata.uiAccessEntries = ((unsigned short int)m_ucIn[7] << 8) | (unsigned short int)m_ucIn[8];
	m_Metadata.ucIDDLength = m_ucIn[9];
	m_Metadata.ucIDDFormat = m_ucIn[10];
	m_Metadata.ucReserved[0] = m_ucIn[11];
	m_Metadata.ucReserved[1] = m_ucIn[12];
	m_Metadata.ucReserved[2] = m_ucIn[13];
	m_Metadata.ucReserved[3] = m_ucIn[14];
	m_Metadata.ucReserved[4] = m_ucIn[15];
	m_Metadata.ucReserved[5] = m_ucIn[16];
	m_Metadata.ucReserved[6] = m_ucIn[17];
	m_Metadata.ucReserved[7] = m_ucIn[18];

	FEDM_RETURN(FEDM_OK);
}


int FedmIscMyAxxessReader::ReadTable_Timezone()
{
	int iOutLen = 0;
	int iInLen = 0;

	int iBack = 0;

	unsigned int uiDataSets = 0;
	unsigned char ucLen = 0;
	int iInCnt = 0;

	unsigned int uiDataIndex = 0;		// first Req-Info

	ClearTable(FEDM_MYAXXESS_TIMEZONE_TABLE);

	do
	{
		m_ucOut[0] = (unsigned char)(uiDataIndex >> 8);	// Req-Info
		m_ucOut[1] = (unsigned char)(uiDataIndex);
		iOutLen = 2;

		iBack = FEISC_0x1F_MAXDataExchange(	m_pReaderModule->m_iReaderHnd, 
											m_pReaderModule->m_ucBusAdr, 
											FEDM_MYAXXESS_READ_TABLE, 
											0x00, 
											FEDM_MYAXXESS_TIMEZONE_TABLE, 
											m_ucOut, 
											iOutLen, 
											m_ucIn, 
											&iInLen, 
											0x00 );
		if((iBack != 0) && (iBack != 0x94))
		{
			return iBack;
		}

		uiDataSets = ((unsigned int)m_ucIn[0] << 8) | (unsigned int)m_ucIn[1];
		ucLen = m_ucIn[2];
		uiDataIndex += uiDataSets;

		iInCnt = 3;

		if (ucLen != sizeof(FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM))
		{
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		
		if (((uiDataSets * ucLen) + 3) != iInLen)
		{
			FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
		}

		while (uiDataSets-- != 0)
		{
			// copy collected data to new item
			FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM* pTimezoneTableItem = new FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM;
			if (pTimezoneTableItem == NULL)
			{
				FEDM_RETURN(FEDM_ERROR_NO_MORE_MEM);
			}

			pTimezoneTableItem->Init();
			// copy data
			memcpy((unsigned char*)pTimezoneTableItem, &m_ucIn[iInCnt], ucLen);
			iInCnt += ucLen;

			// add item to vector
			m_TimezoneTable.push_back(pTimezoneTableItem);
		}

	} while (iBack == 0x94);	// more table data

	FEDM_RETURN(FEDM_OK);
}

int FedmIscMyAxxessReader::ReadTable_Holiday()
{
	int iOutLen = 0;
	int iInLen = 0;

	int iBack = 0;

	unsigned int uiDataSets = 0;
	unsigned char ucLen = 0;
	int iInCnt = 0;

	unsigned int uiDataIndex = 0;		// first Req-Info

	ClearTable(FEDM_MYAXXESS_HOLIDAY_TABLE);

	do
	{
		m_ucOut[0] = (unsigned char)(uiDataIndex >> 8);	// Req-Info
		m_ucOut[1] = (unsigned char)(uiDataIndex);
		iOutLen = 2;

		iBack = FEISC_0x1F_MAXDataExchange(	m_pReaderModule->m_iReaderHnd, 
											m_pReaderModule->m_ucBusAdr, 
											FEDM_MYAXXESS_READ_TABLE, 
											0x00, 
											FEDM_MYAXXESS_HOLIDAY_TABLE, 
											m_ucOut, 
											iOutLen, 
											m_ucIn, 
											&iInLen, 
											0x00 );
		if((iBack != 0) && (iBack != 0x94))
		{
			return iBack;
		}

		uiDataSets = ((unsigned int)m_ucIn[0] << 8) | (unsigned int)m_ucIn[1];
		ucLen = m_ucIn[2];
		uiDataIndex += uiDataSets;

		iInCnt = 3;

		if (ucLen != sizeof(FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM))
		{
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		
		if (((uiDataSets * ucLen) + 3) != iInLen)
		{
			FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
		}

		while (uiDataSets-- != 0)
		{
			// copy collected data to new item
			FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM* pHolidayTableItem = new FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM;
			if (pHolidayTableItem == NULL)
			{
				FEDM_RETURN(FEDM_ERROR_NO_MORE_MEM);
			}

			pHolidayTableItem->Init();
			// copy data
			memcpy((unsigned char*)pHolidayTableItem, &m_ucIn[iInCnt], ucLen);
			iInCnt += ucLen;

			// add item to vector
			m_HolidayTable.push_back(pHolidayTableItem);
		}

	} while (iBack == 0x94);	// more table data

	FEDM_RETURN(FEDM_OK);
}

int FedmIscMyAxxessReader::ReadTable_Access()
{
	int iOutLen = 0;
	int iInLen = 0;

	int iBack = 0;

	unsigned int uiDataSets = 0;
	unsigned char ucLen = 0;
	int iInCnt = 0;

	unsigned int uiDataIndex = 0;		// first Req-Info

	ClearTable(FEDM_MYAXXESS_ACCESS_TABLE);

	do
	{
		m_ucOut[0] = (unsigned char)(uiDataIndex >> 8);	// Req-Info
		m_ucOut[1] = (unsigned char)(uiDataIndex);
		iOutLen = 2;

		iBack = FEISC_0x1F_MAXDataExchange(	m_pReaderModule->m_iReaderHnd, 
											m_pReaderModule->m_ucBusAdr, 
											FEDM_MYAXXESS_READ_TABLE, 
											0x00, 
											FEDM_MYAXXESS_ACCESS_TABLE, 
											m_ucOut, 
											iOutLen, 
											m_ucIn, 
											&iInLen, 
											0x00 );
		if((iBack != 0) && (iBack != 0x94))
		{
			return iBack;
		}

		uiDataSets = ((unsigned int)m_ucIn[0] << 8) | (unsigned int)m_ucIn[1];
		ucLen = m_ucIn[2];
		uiDataIndex += uiDataSets;

		iInCnt = 3;

		if ((ucLen > sizeof(FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM)) || (ucLen != (m_Metadata.ucIDDLength + 3)))
		{
			FEDM_RETURN(FEDM_ERROR_PARAMETER);
		}
		
		if (((uiDataSets * ucLen) + 3) != iInLen)
		{
			FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
		}

		while (uiDataSets-- != 0)
		{
			// copy collected data to new item
			FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM* pAccessTableItem = new FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM;
			if (pAccessTableItem == NULL)
			{
				FEDM_RETURN(FEDM_ERROR_NO_MORE_MEM);
			}

			pAccessTableItem->Init();

			// copy data
			memcpy((unsigned char*)pAccessTableItem, &m_ucIn[iInCnt], ucLen-3);
			pAccessTableItem->uiTimezones = ((unsigned short int)m_ucIn[iInCnt+ucLen-3] << 8) | (unsigned short int)m_ucIn[iInCnt+ucLen-2];
			pAccessTableItem->ucReserved = m_ucIn[iInCnt+ucLen-1];
			iInCnt += ucLen;

			// add item to vector
			m_AccessTable.push_back(pAccessTableItem);
		}

	} while (iBack == 0x94);	// more table data

	FEDM_RETURN(FEDM_OK);
}


int FedmIscMyAxxessReader::ReadTable_Event()
{
	int iOutLen = 0;
	int iInLen = 0;

	int iBack = 0;

	unsigned int uiDataSets = 0;
	unsigned char ucDataLayout = 0;
	int iInCnt = 0;
	unsigned int uiRecLen = 0;

	unsigned int uiReqInfo = 0;

	FEDM_ISC_MYAXXESS_EVENT_TABLE_ITEM EventTableItem;


	uiReqInfo = FEDM_MYAXXESS_MAX_BLOCKSIZE / sizeof(FEDM_ISC_MYAXXESS_EVENT_TABLE_ITEM);
	m_ucOut[0] = (unsigned char)((uiReqInfo & 0xFF00) >> 8);	// Req-Info
	m_ucOut[1] = (unsigned char)(uiReqInfo & 0x00FF);
	iOutLen = 2;

	do
	{
		iBack = FEISC_0x1F_MAXDataExchange(	m_pReaderModule->m_iReaderHnd, 
											m_pReaderModule->m_ucBusAdr, 
											FEDM_MYAXXESS_READ_TABLE, 
											0x00, 
											FEDM_MYAXXESS_EVENT_TABLE, 
											m_ucOut, 
											iOutLen, 
											m_ucIn, 
											&iInLen, 
											0x00 );
		if((iBack != 0) && (iBack != 0x93) && (iBack != 0x94))
		{
			return iBack;
		}

		ucDataLayout = m_ucIn[0];
		uiDataSets = ((unsigned int)m_ucIn[1] << 8) | (unsigned int)m_ucIn[2];

		iInCnt = 3;

/*		
		if (((uiDataSets * ucLen) + 3) != iInLen)
		{
			FEDM_RETURN(FEDM_ERROR_BUFFER_LENGTH);
		}
*/
		while (uiDataSets-- != 0)
		{
			// copy collected data to new item
			EventTableItem.Init();
			EventTableItem.ucDataLayout = ucDataLayout;
			uiRecLen = (unsigned int)m_ucIn[iInCnt++] << 8;
			uiRecLen |= (unsigned int)m_ucIn[iInCnt++];
			EventTableItem.ucEventID = m_ucIn[iInCnt++];

			if (ucDataLayout & 0x01)	// IDD Set
			{
				EventTableItem.ucIDDLength = m_ucIn[iInCnt++];

				if (EventTableItem.ucIDDLength > 64)
					FEDM_RETURN(FEDM_ERROR_PARAMETER);

				memcpy((unsigned char*)EventTableItem.ucIDD, &m_ucIn[iInCnt], EventTableItem.ucIDDLength);
				iInCnt += EventTableItem.ucIDDLength;
			}

			if (ucDataLayout & 0x02)	// Timestamp
			{
				EventTableItem.ucDay = m_ucIn[iInCnt++];
				EventTableItem.ucMonth = m_ucIn[iInCnt++];
				EventTableItem.ucYear = m_ucIn[iInCnt++];
				EventTableItem.ucHour = m_ucIn[iInCnt++];
				EventTableItem.ucMinute = m_ucIn[iInCnt++];
				EventTableItem.ucSecond = m_ucIn[iInCnt++];
			}

			if (ucDataLayout & 0x04)	// Event Status
			{
				EventTableItem.ucAction = m_ucIn[iInCnt++];
				EventTableItem.uiError = (unsigned short int)m_ucIn[iInCnt++] << 8;
				EventTableItem.uiError |= (unsigned short int)m_ucIn[iInCnt++];
			}

			if (ucDataLayout & 0x08)	// Input Data
			{
				EventTableItem.ucInput = m_ucIn[iInCnt++];
			}

			if (ucDataLayout & 0x10)	// Antenne No
			{
				EventTableItem.ucAnt = m_ucIn[iInCnt++];
			}

			if (FEDM_IS_TCPPORT(m_pReaderModule->GetPortHnd()))
			{
				char cTmp[32];
				m_pReaderModule->GetPortPara("HostAdr", cTmp);
				EventTableItem.sSourceIP = cTmp;
			}
			else
			{
				EventTableItem.sSourceIP = "0.0.0.0";
			}

			// copy collected data to new item
			FEDM_ISC_MYAXXESS_EVENT_TABLE_ITEM* pEventTableItem = new FEDM_ISC_MYAXXESS_EVENT_TABLE_ITEM;
			if (pEventTableItem == NULL)
			{
				FEDM_RETURN(FEDM_ERROR_NO_MORE_MEM);
			}
			pEventTableItem->Init();
			memcpy((unsigned char*)pEventTableItem, (unsigned char*)&EventTableItem, sizeof(FEDM_ISC_MYAXXESS_EVENT_TABLE_ITEM));

			// add item to vector
			m_EventTable.push(pEventTableItem);
		}

		m_ucOut[0] = (unsigned char)((uiReqInfo & 0xFF00) >> 8);	// Req-Info if more table data
		m_ucOut[1] = (unsigned char)(uiReqInfo & 0x00FF);
		iOutLen = 2;

	} while ((iBack == 0x94) || (iBack == 0x93));	// more table data or overflow

	FEDM_RETURN(FEDM_OK);
}


/***************************************************************************
  Begin        		:	24.09.2009 / M. Hultsch

  Version       	:	03.00.13 / 28.09.2009 / M. Hultsch

  Function			:	static callback function for invoke from DLL

  Parameters		:	void* pAny					- [in] pointer to caller object
						int iReaderHnd				- [in] reader handle (FEISC.DLL)
						int iTaskID					- [in] task identifier from FEISC_StartAsyncTask(..)
						int iError					- [in] error code
						unsigned char ucCmd			- [in] reader command
						unsigned char* ucRspData	- [in] pointer to response data
						int iRspLen					- [in] length of response data
						char* cIPAdr				- [in] pointer to ip address of reader
						int iPortNr					- [in] local or remote port number 
						unsigned char& ucAction		- [out] reference to action value

  Return value		:	-
***************************************************************************/
int FedmIscMyAxxessReader::cbsTaskRsp3(	void* pAny, 
										int iReaderHnd, 
										int iTaskID, 
										int iError, 
										unsigned char ucCmd,
										unsigned char* ucRspData, 
										int iRspLen,
										char* cIPAdr,
										int iPortNr,
										unsigned char& ucAction )
{
	if((FedmIscMyAxxessReader*)pAny == NULL)
		return FEDM_ERROR_NULL_POINTER;

	switch(iTaskID)
	{
	case FEISC_TASKID_MAX_EVENT:
		switch(ucCmd)
		{
		case 0x1F:
			return ((FedmIscMyAxxessReader*)pAny)->cbNewMaxEvent(	iReaderHnd, 
																	FEDM_TASKID_MAX_EVENT, 
																	iError, 
																	ucRspData, 
																	iRspLen, 
																	cIPAdr, 
																	iPortNr, 
																	ucAction );
			break;

		case 0x6E:
			((FedmIscMyAxxessReader*)pAny)->cbNewReaderDiagnostic(	iReaderHnd,
																	FEDM_TASKID_MAX_EVENT,
																	iError,
																	ucRspData, 
																	iRspLen, 
																	cIPAdr, 
																	iPortNr );
			break;

		default:
			return FEDM_ERROR_UNKNOWN_COMMAND;
		}

	default:
		return FEDM_ERROR_PARAMETER;
	}

	return FEDM_ERROR_UNSUPPORTED;
}



/***************************************************************************
  Begin        		:	24.09.2009 / M. Hultsch

  Version       	:	03.00.13 / 30.09.2009 / M. Hultsch

  Function			:	non-static callback function for invoke from DLL
						- every callback contains only one access event

  Parameters		:	int iReaderHnd				- [in] reader handle (FEISC.DLL)
						int iTaskID					- [in] task identifier from FEISC_StartAsyncTask(..)
						int iError					- [in] error code
						unsigned char* ucRspData	- [in] pointer to response data
						int iRspLen					- [in] length of response data
						char* cIPAdr				- [in] pointer to ip address of reader
						int iPortNr					- [in] local or remote port number 
						unsigned char& ucAction		- [out] reference to action value

  Return value		:	-
***************************************************************************/
int FedmIscMyAxxessReader::cbNewMaxEvent(	int iReaderHnd, 
											int iTaskID, 
											int iError, 
											unsigned char* ucRspData, 
											int iRspLen,
											char* cIPAdr,
											int iPortNr,
											unsigned char& ucAction )
{
	int iBack = 0;
	unsigned char ucDataLayout	= 0;
	unsigned int  uiDataSets	= 0;
	unsigned int  uiDataSize	= 0;
	unsigned int uiCnt = 0;
	FEDM_ISC_MYAXXESS_EVENT_TABLE_ITEM* pEventItem = NULL;
	vector<FEDM_ISC_MYAXXESS_EVENT_TABLE_ITEM*> aEvent;
	vector<FEDM_ISC_MYAXXESS_EVENT_TABLE_ITEM*>::iterator itor;

	if(iReaderHnd != m_pReaderModule->GetReaderHnd())
		return FEDM_ERROR_NO_READER_HANDLE;

	if(iTaskID != FEDM_TASKID_MAX_EVENT)
		return FEDM_ERROR_UNSUPPORTED;

	switch(m_uiEventCallerType)
	{
	case 0: // for C++
		if(m_cbEvent == NULL)
			return FEDM_ERROR_NULL_POINTER;
		break;

	case 1: // for .NET
		if(m_cbStdCallEvent == NULL)
			return FEDM_ERROR_NULL_POINTER;
		break;
	}

	// notify error cases
	if(iError != 0x00 && iError != 0x93)
		return m_cbEvent(m_pReaderModule->m_TaskInit.pAny, iError, aEvent, ucAction, cIPAdr, iPortNr);

	if(iRspLen <= 3)
		return m_cbEvent(m_pReaderModule->m_TaskInit.pAny, FEDM_ERROR_NO_DATA, aEvent, ucAction, cIPAdr, iPortNr);



	// extract event data
	ucDataLayout = ucRspData[uiCnt++];
	uiDataSets  = ((unsigned int)ucRspData[uiCnt++] << 8);
	uiDataSets |=  (unsigned int)ucRspData[uiCnt++];

	if(uiDataSets == 0)
		return m_cbEvent(m_pReaderModule->m_TaskInit.pAny, FEDM_ERROR_NO_DATA, aEvent, ucAction, cIPAdr, iPortNr);

	for(unsigned int uiRecCount = 0; uiRecCount < uiDataSets; uiRecCount++)
	{
		uiDataSize  = ((unsigned int)ucRspData[uiCnt++] << 8);
		uiDataSize |=  (unsigned int)ucRspData[uiCnt++];

		if( (unsigned int)iRspLen < (3 + uiDataSize) )
		{
			return m_cbEvent(m_pReaderModule->m_TaskInit.pAny, FEDM_ERROR_NO_DATA, aEvent, ucAction, cIPAdr, iPortNr);
		}
	
		pEventItem = new FEDM_ISC_MYAXXESS_EVENT_TABLE_ITEM;
		pEventItem->Init();

		pEventItem->ucDataLayout = ucDataLayout;
		pEventItem->ucEventID = ucRspData[uiCnt++];

		if (ucDataLayout & 0x01)	// IDD Set
		{
			pEventItem->ucIDDLength = ucRspData[uiCnt++];

			if (pEventItem->ucIDDLength > 64)
			{
				// send notofocation with collected events and with error code
				iBack = m_cbEvent(m_pReaderModule->m_TaskInit.pAny, FEDM_ERROR_BUFFER_LENGTH, aEvent, ucAction, cIPAdr, iPortNr);
				
				delete pEventItem;
				// delete temporary vector
				for(itor = aEvent.begin(); itor != aEvent.end(); itor++)
				{
					delete (*itor);
				}
				return iBack;
			}

			memcpy(pEventItem->ucIDD, &ucRspData[uiCnt], pEventItem->ucIDDLength);
			uiCnt += pEventItem->ucIDDLength;
		}

		if (ucDataLayout & 0x02)	// Timestamp
		{
			pEventItem->ucDay = ucRspData[uiCnt++];
			pEventItem->ucMonth = ucRspData[uiCnt++];
			pEventItem->ucYear = ucRspData[uiCnt++];
			pEventItem->ucHour = ucRspData[uiCnt++];
			pEventItem->ucMinute = ucRspData[uiCnt++];
			pEventItem->ucSecond = ucRspData[uiCnt++];
		}

		if (ucDataLayout & 0x04)	// Event Status
		{
			pEventItem->ucAction = ucRspData[uiCnt++];
			pEventItem->uiError  = ((unsigned short int)ucRspData[uiCnt++] << 8);
			pEventItem->uiError |=  (unsigned short int)ucRspData[uiCnt++];
		}

		if (ucDataLayout & 0x08)	// Input Data
		{
			pEventItem->ucInput = ucRspData[uiCnt++];
		}

		if (ucDataLayout & 0x10)	// Antenna No
		{
			pEventItem->ucAnt = ucRspData[uiCnt++];
		}

		pEventItem->sSourceIP = cIPAdr;

		aEvent.push_back(pEventItem);
	}

	switch(m_uiEventCallerType)
	{
	case 0: // for C++
		iBack = m_cbEvent(m_pReaderModule->m_TaskInit.pAny, FEDM_OK, aEvent, ucAction, cIPAdr, iPortNr);
		break;

	case 1: // for .NET
		iBack = m_cbStdCallEvent(FEDM_OK, aEvent, ucAction, cIPAdr, iPortNr);
		break;
	}

	// delete temporary vector
	for(itor = aEvent.begin(); itor != aEvent.end(); itor++)
	{
		delete (*itor);
	}

	return iBack;
}


/***************************************************************************
  Begin        		:	28.09.2009 / M. Hultsch

  Version       	:	03.00.13 / 28.09.2009 / M. Hultsch

  Function			:	non-static callback function for invoke from DLL

  Parameters		:	int iReaderHnd				- reader handle (FEISC.DLL)
						int iTaskID					- task identifier from FEISC_StartAsyncTask(..)
						int iError					- error code
						unsigned char* ucRspData	- pointer to response data
						int iRspLen					- length of response data
						char* cIPAdr				- pointer to ip address of reader
						int iPortNr					- local or remote port number 

  Return value		:	-
***************************************************************************/
void FedmIscMyAxxessReader::cbNewReaderDiagnostic(	int iReaderHnd, 
													int iTaskID, 
													int iError, 
													unsigned char* ucRspData, 
													int iRspLen,
													char* cIPAdr,
													int iPortNr )
{
	unsigned char ucData[13];
	int iErr = 0;
//	unsigned int uiIP[4];
//	unsigned char ucIP[4];
	unsigned int uiErrorFlags = 0;
	unsigned int uiTableSize = 0;
	unsigned int uiTableLength = 0;

//	memset(uiIP, 0, 4*sizeof(unsigned int));
//	memset (ucIP, 0, 4*sizeof(unsigned char));


	if(iReaderHnd != m_pReaderModule->GetReaderHnd())
		return;

	if(iTaskID != FEDM_TASKID_MAX_EVENT)
		return;

	switch(m_uiEventCallerType)
	{
	case 0: // for C++
		if(m_cbKeepAlive == NULL)
			return;
		break;

	case 1: // for .NET
		if(m_cbStdCallKeepAlive == NULL)
			return;
		break;
	}

	SetLastError(iError);

	
	// first step: extract reader diagnostic data
	if( iError == 0x00 || 
		iError == 0x84 )
	{	
		if(iRspLen != 6)
		{
			m_cbKeepAlive(m_pReaderModule->m_TaskInit.pAny, FEDM_ERROR_NO_DATA, 0, 0, 0, cIPAdr, iPortNr);
			return;
		}

		memset(ucData, 0, 13);

		// with mode 0x06, we receive 6 bytes
		memcpy(ucData, ucRspData, iRspLen);

		// set reader diagnostic mode 0x01
		FEDM_CHK6(iErr, m_pReaderModule->SetData(FEDM_ISC_TMP_DIAG_MODE, (unsigned char)0x06));

		// FEDM_ISC_TMP_DIAG_DATA addresses always 13 bytes
		FEDM_CHK6(iErr, m_pReaderModule->SetData(FEDM_ISC_TMP_DIAG_DATA, ucData, 13));

		uiErrorFlags	= (((unsigned int)ucRspData[0]) << 8) + ucRspData[1];
		uiTableSize		= (((unsigned int)ucRspData[2]) << 8) + ucRspData[3];
		uiTableLength	= (((unsigned int)ucRspData[4]) << 8) + ucRspData[5];
	}

/*	sscanf(cIPAdr, "%u.%u.%u.%u", &uiIP[0], &uiIP[1], &uiIP[2], &uiIP[3]);
	ucIP[0] = (uiIP[0] & 0x000000FF);
	ucIP[1] = (uiIP[1] & 0x000000FF);
	ucIP[2] = (uiIP[2] & 0x000000FF);
	ucIP[3] = (uiIP[3] & 0x000000FF);
	SetData(FEDM_ISC_TMP_NTF_REMOTE_IP_ADR, ucIP, 4);
	SetData(FEDM_ISC_TMP_NTF_IP_PORT_NR, (unsigned int)iPortNr);
*/
	// second step: invoke callback function of application
	switch(m_uiEventCallerType)
	{
	case 0: // for C++
		m_cbKeepAlive(m_pReaderModule->m_TaskInit.pAny, iError, uiErrorFlags, uiTableSize, uiTableLength, cIPAdr, iPortNr);
		break;

	case 1: // for .NET
		m_cbStdCallKeepAlive(iError, uiErrorFlags, uiTableSize, uiTableLength, cIPAdr, iPortNr);
		break;
	}
}

void FedmIscMyAxxessReader::SetLastError(int iError)
{
	m_pReaderModule->SetLastError(iError);
}

int FedmIscMyAxxessReader::GetLastError()
{
	return m_pReaderModule->GetLastError();
}

char* FedmIscMyAxxessReader::GetErrorText(int iErrorCode)
{
	return m_pReaderModule->GetErrorText(iErrorCode);
}