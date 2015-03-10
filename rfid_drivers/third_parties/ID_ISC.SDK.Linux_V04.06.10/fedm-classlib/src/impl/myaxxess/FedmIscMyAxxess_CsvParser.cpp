/*-------------------------------------------------------
|                                                       |
|	FedmIscMyAxxess_CsvParser.cpp			            |
|                                                       |
---------------------------------------------------------

Copyright © 2009-2010	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	25.06.2009
Version       		:	03.01.00 / 09.02.2010 / B. Tschunko, M. Hultsch
Operation Systems	:	independent
Function			:	parser for myAxxess Data Files

OBID® and OBID i-scan® are registered Trademarks of FEIG ELECTRONIC GmbH.
Linux® is a registered trademark of Linus Torvalds.
Microsoft® and Windows® are registered trademarks of Microsoft Corporation.
*/


//#############################
//      I N C L U D E S
//#############################
#include "FedmIscCore.h"
#include "FedmIscMyAxxess_CsvParser.h"


//#######################################################################################
//	class FedmIscMyAxxess_CsvParser
//#######################################################################################
FedmIscMyAxxess_CsvParser::FedmIscMyAxxess_CsvParser(FedmIscMyAxxessReader* pAxxessReader)
{
	m_pAxxessReader = pAxxessReader;
}


FedmIscMyAxxess_CsvParser::~FedmIscMyAxxess_CsvParser(void)
{
}

//#####################################################################################
// serialization functions
//#####################################################################################

/***************************************************************************
  Begin        	:	25.06.2009 / M. Hultsch
  Version       :	03.00.08 / 01.07.2009 / B. Tschunko

  Function		:	read CSV document and save content in data map

  Parameters	:	char* szCsvFileName	- pointer to buffer with filename

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FedmIscMyAxxess_CsvParser::SerializeIn(	char* szCsvFileName)
{
	int iBack = 0;
	char* sPrt = NULL;
	string sValue;

	if(strlen(szCsvFileName) == 0 || strlen(szCsvFileName) > FEDM_CSV_MAX_FILENAME_SIZE)
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_STRING_LENGTH);
		return m_pAxxessReader->GetLastError();
	}

	FILE* hCvsFile = fopen (szCsvFileName, "r");

    if (hCvsFile == NULL)
    {
		m_pAxxessReader->SetLastError(FEDM_ERROR_OPEN_FILE);
		return m_pAxxessReader->GetLastError();
    }

	// allocate memory for line
	char* pLine = new char[128+1];

	if (pLine == NULL)
	{
		fclose(hCvsFile);
		m_pAxxessReader->SetLastError(FEDM_ERROR_NO_MORE_MEM);
		return m_pAxxessReader->GetLastError();
	}

	memset(pLine, 0, 128+1);

	// read first line
    if(fscanf(hCvsFile, "%128[^\n]\n", pLine) == EOF)
	{
		delete [] pLine;           
		fclose(hCvsFile);
		m_pAxxessReader->SetLastError(FEDM_ERROR_READ_FILE);
		return m_pAxxessReader->GetLastError();
	}

	sPrt = strtok(pLine, ";");

	sValue = sPrt;
	FEDM_STRUPR((char *)(sValue.c_str()), sValue.length()+1);
	if (sValue == "TIMEZONETABLE")
	{
		iBack = SerializeIn_TimezoneTable(hCvsFile, pLine);
		if (iBack)
		{
			m_pAxxessReader->ClearTable(FEDM_MYAXXESS_TIMEZONE_TABLE);
		}

	}
	else if (sValue == "HOLIDAYTABLE")
	{
		iBack = SerializeIn_HolidayTable(hCvsFile, pLine);
		if (iBack)
		{
			m_pAxxessReader->ClearTable(FEDM_MYAXXESS_HOLIDAY_TABLE);
		}
	}
	else if (sValue == "ACCESSTABLE")
	{
		iBack = SerializeIn_AccessTable(hCvsFile, pLine);
		if (iBack)
		{
			m_pAxxessReader->ClearTable(FEDM_MYAXXESS_ACCESS_TABLE);
		}
	}
	else if (sValue == "EVENTTABLE")
	{
		iBack = SerializeIn_EventTable(hCvsFile, pLine);
		if (iBack)
		{
			m_pAxxessReader->ClearTable(FEDM_MYAXXESS_EVENT_TABLE);
		}
	}
	else
	{
		delete [] pLine;           
		fclose(hCvsFile);
		m_pAxxessReader->SetLastError(FEDM_ERROR_UNKNOWN_TABLE_ID);
		return m_pAxxessReader->GetLastError();
	}

	delete [] pLine;           
	fclose(hCvsFile);

	return iBack;
}

/***************************************************************************
  Begin        	:	25.06.2009 / M. Hultsch
  Version       :	03.00.08 / 01.07.2009 / B. Tschunko

  Function		:	write content of data map into CSV document

  Parameters	:	unsigned int uiTableID	- table identifier from FedmIscMyAxxessReader.h
					char* szCsvFileName	- pointer to buffer with filename

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FedmIscMyAxxess_CsvParser::SerializeOut(unsigned int uiTableID, 
											char* szCsvFileName)
{
	int iBack = 0;
	char* sPrt = NULL;

	if(strlen(szCsvFileName) == 0 || strlen(szCsvFileName) > FEDM_CSV_MAX_FILENAME_SIZE)
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_STRING_LENGTH);
		return m_pAxxessReader->GetLastError();
	}

	//check filetype cvs
	char szTmpCsvFileName[FEDM_CSV_MAX_FILENAME_SIZE+1];
	strcpy(szTmpCsvFileName, szCsvFileName);
	FEDM_STRUPR(szTmpCsvFileName, strlen(szTmpCsvFileName)+1);

	sPrt = strtok(szTmpCsvFileName, ".");
	if (sPrt == NULL)
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_OPEN_FILE);
		return m_pAxxessReader->GetLastError();
	}
	sPrt = strtok(NULL, ".");
	if (sPrt == NULL)
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_OPEN_FILE);
		return m_pAxxessReader->GetLastError();
	}
	if (strcmp(sPrt, "CSV"))
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_OPEN_FILE);
		return m_pAxxessReader->GetLastError();
	}

	FILE* hCvsFile = fopen (szCsvFileName, "w");

    if (hCvsFile == NULL)
    {
		m_pAxxessReader->SetLastError(FEDM_ERROR_OPEN_FILE);
		return m_pAxxessReader->GetLastError();
    }

	switch (uiTableID)
	{
	case FEDM_MYAXXESS_TIMEZONE_TABLE:

		iBack = SerializeOut_TimezoneTable(hCvsFile);
		if (iBack)
		{
			//TODO: delete file;
		}
		break;

	case FEDM_MYAXXESS_HOLIDAY_TABLE:

		iBack = SerializeOut_HolidayTable(hCvsFile);
		if (iBack)
		{
			//TODO: delete file;
		}
		break;

	case FEDM_MYAXXESS_ACCESS_TABLE:

		iBack = SerializeOut_AccessTable(hCvsFile);
		if (iBack)
		{
			//TODO: delete file;
		}
		break;

	case FEDM_MYAXXESS_EVENT_TABLE:

		iBack = SerializeOut_EventTable(hCvsFile);
		if (iBack)
		{
			//TODO: delete file;
		}
		break;

	default:
		m_pAxxessReader->SetLastError(FEDM_ERROR_UNKNOWN_TABLE_ID);
		return m_pAxxessReader->GetLastError();
	}

	fclose(hCvsFile);

	return iBack;
}

/***************************************************************************
  Begin        	:	29.06.2009 / B. Tschunko
  Version       :	03.00.08 / 01.07.2009 / B. Tschunko

  Function		:	read CSV  for doc type timezone table

  Parameters	:	FILE* hCvsFile	- opened CVS file
					char* pLine - pointer to read each line

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FedmIscMyAxxess_CsvParser::SerializeIn_TimezoneTable(	FILE* hCvsFile,
															char* pLine)
{
	int iBack = 0;
	char* sPrt = NULL;
	string sValue;
	unsigned int uiCnt = 0;
	unsigned int uiNumber = 0;

	FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM pItemTimezone;
	pItemTimezone.Init();

	vector<string> aName;
	vector<string> aValue;

    if (hCvsFile == NULL)
    {
		m_pAxxessReader->SetLastError(FEDM_ERROR_READ_FILE);
		return m_pAxxessReader->GetLastError();
    }

	m_pAxxessReader->ClearTable(FEDM_MYAXXESS_TIMEZONE_TABLE);

	// read second line
    if(fscanf(hCvsFile, "%128[^\n]\n", pLine) == EOF)
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_READ_FILE);
		return m_pAxxessReader->GetLastError();
	}

	// #### Check Table Header ####
 	sPrt = strtok(pLine, ";");
	sValue = sPrt;
	FEDM_STRUPR((char *)(sValue.c_str()), sValue.length()+1);
	if (sValue != "NUMBER")
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_UNSUPPORTED_NAMESPACE);
		return m_pAxxessReader->GetLastError();
	}

	sPrt = strtok(NULL, ";");
	sValue = sPrt;
	FEDM_STRUPR((char *)(sValue.c_str()), sValue.length()+1);
	if (sValue != "MO")
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_UNSUPPORTED_NAMESPACE);
		return m_pAxxessReader->GetLastError();
	}

	sPrt = strtok(NULL, ";");
	sValue = sPrt;
	FEDM_STRUPR((char *)(sValue.c_str()), sValue.length()+1);
	if (sValue != "TU")
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_UNSUPPORTED_NAMESPACE);
		return m_pAxxessReader->GetLastError();
	}

	sPrt = strtok(NULL, ";");
	sValue = sPrt;
	FEDM_STRUPR((char *)(sValue.c_str()), sValue.length()+1);
	if (sValue != "WE")
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_UNSUPPORTED_NAMESPACE);
		return m_pAxxessReader->GetLastError();
	}

	sPrt = strtok(NULL, ";");
	sValue = sPrt;
	FEDM_STRUPR((char *)(sValue.c_str()), sValue.length()+1);
	if (sValue != "TH")
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_UNSUPPORTED_NAMESPACE);
		return m_pAxxessReader->GetLastError();
	}

	sPrt = strtok(NULL, ";");
	sValue = sPrt;
	FEDM_STRUPR((char *)(sValue.c_str()), sValue.length()+1);
	if (sValue != "FR")
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_UNSUPPORTED_NAMESPACE);
		return m_pAxxessReader->GetLastError();
	}

	sPrt = strtok(NULL, ";");
	sValue = sPrt;
	FEDM_STRUPR((char *)(sValue.c_str()), sValue.length()+1);
	if (sValue != "SA")
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_UNSUPPORTED_NAMESPACE);
		return m_pAxxessReader->GetLastError();
	}

	sPrt = strtok(NULL, ";");
	sValue = sPrt;
	FEDM_STRUPR((char *)(sValue.c_str()), sValue.length()+1);
	if (sValue != "SU")
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_UNSUPPORTED_NAMESPACE);
		return m_pAxxessReader->GetLastError();
	}

	sPrt = strtok(NULL, ";");
	sValue = sPrt;
	FEDM_STRUPR((char *)(sValue.c_str()), sValue.length()+1);
	if (sValue != "DATEFROM")
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_UNSUPPORTED_NAMESPACE);
		return m_pAxxessReader->GetLastError();
	}

	sPrt = strtok(NULL, ";");
	sValue = sPrt;
	FEDM_STRUPR((char *)(sValue.c_str()), sValue.length()+1);
	if (sValue != "TIMEFROM")
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_UNSUPPORTED_NAMESPACE);
		return m_pAxxessReader->GetLastError();
	}

	sPrt = strtok(NULL, ";");
	sValue = sPrt;
	FEDM_STRUPR((char *)(sValue.c_str()), sValue.length()+1);
	if (sValue != "DATETO")
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_UNSUPPORTED_NAMESPACE);
		return m_pAxxessReader->GetLastError();
	}

	sPrt = strtok(NULL, ";");
	sValue = sPrt;
	FEDM_STRUPR((char *)(sValue.c_str()), sValue.length()+1);
	if (sValue != "TIMETO")
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_UNSUPPORTED_NAMESPACE);
		return m_pAxxessReader->GetLastError();
	}

	// #### read Table Data ####
	aName.push_back("");
	aName.push_back("");
	aName.push_back("");
	aName.push_back("");
	aName.push_back("");
	aValue.push_back("");
	aValue.push_back("");
	aValue.push_back("");
	aValue.push_back("");
	aValue.push_back("");

	bool bAddPlus = false;

	// max. 15 Timezone Table Entries
	for (uiCnt = 0; uiCnt < 15; uiCnt++)
	{
		aValue[0] = "";
		aValue[1] = "";
		aValue[2] = "";
		aValue[3] = "";
		aValue[4] = "";

		// read next line
		if(fscanf(hCvsFile, "%128[^\n]\n", pLine) == EOF)
		{
			break;
		}

		// check Number
		sPrt = strtok(pLine, ";");

		sscanf(sPrt, "%d", &uiNumber);
		if (uiNumber != (uiCnt+1))
		{
			m_pAxxessReader->SetLastError(FEDM_ERROR_PARAMETER);
			return m_pAxxessReader->GetLastError();
		}

		// get Days
		bAddPlus = false;
		aName[0] = "Days";
		for (unsigned char ucDay = 0; ucDay < 7; ucDay++)
		{
			sPrt = strtok(NULL, ";");
			if (sPrt == NULL)
			{
				m_pAxxessReader->SetLastError(FEDM_ERROR_PARAMETER);
				return m_pAxxessReader->GetLastError();
			}

			if (!strcmp(sPrt, "0"))
			{
				continue;
			}
			else if(!strcmp(sPrt, "1"))
			{
				if (bAddPlus)
					aValue[0] += "+";

				switch (ucDay)
				{
				case 0:
					aValue[0] += "Mo";
					break;

				case 1:
					aValue[0] += "Tu";
					break;

				case 2:
					aValue[0] += "We";
					break;

				case 3:
					aValue[0] += "Th";
					break;

				case 4:
					aValue[0] += "Fr";
					break;

				case 5:
					aValue[0] += "Sa";
					break;

				case 6:
					aValue[0] += "Su";
					break;
				}
				bAddPlus = true;
			}
			else
			{
				m_pAxxessReader->SetLastError(FEDM_ERROR_PARAMETER);
				return m_pAxxessReader->GetLastError();
			}
		}

		// get DateFrom
		sPrt = strtok(NULL, ";");
		if (sPrt == NULL)
		{
			m_pAxxessReader->SetLastError(FEDM_ERROR_PARAMETER);
			return m_pAxxessReader->GetLastError();
		}
		aName[1] = "DateFrom";
		aValue[1] = sPrt;

		// get TimeFrom
		sPrt = strtok(NULL, ";");
		if (sPrt == NULL)
		{
			m_pAxxessReader->SetLastError(FEDM_ERROR_PARAMETER);
			return m_pAxxessReader->GetLastError();
		}
		aName[2] = "TimeFrom";
		aValue[2] = sPrt;

		// get DateTo value
		sPrt = strtok(NULL, ";");
		if (sPrt == NULL)
		{
			m_pAxxessReader->SetLastError(FEDM_ERROR_PARAMETER);
			return m_pAxxessReader->GetLastError();
		}
		aName[3] = "DateTo";
		aValue[3] = sPrt;

		// get TimeTo value
		sPrt = strtok(NULL, ";");
		if (sPrt == NULL)
		{
			m_pAxxessReader->SetLastError(FEDM_ERROR_PARAMETER);
			return m_pAxxessReader->GetLastError();
		}
		aName[4] = "TimeTo";
		aValue[4] = sPrt;

		// build and add Timezone
		iBack = m_pAxxessReader->BuildTableItem(aName, aValue, &pItemTimezone);
		if(iBack != 0)
		{
			return iBack;
		}		
		iBack = m_pAxxessReader->AddTableItem(&pItemTimezone);
		if (iBack != 0)
		{
			return iBack;
		}
	}

	// all lines read
	if (uiCnt == 15)
	{
		if(fscanf(hCvsFile, "%128[^\n]\n", pLine) != EOF)
		{
			m_pAxxessReader->SetLastError(FEDM_ERROR_PARAMETER);
			return m_pAxxessReader->GetLastError();
		}
	}

	m_pAxxessReader->SetLastError(FEDM_OK);
	return m_pAxxessReader->GetLastError();
}

/***************************************************************************
  Begin        	:	29.06.2009 / B. Tschunko
  Version       :	03.00.08 / 01.07.2009 / B. Tschunko

  Function		:	read CSV  for doc type holiday table

  Parameters	:	FILE* hCvsFile	- opened CVS file
					char* pLine - pointer to read each line

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FedmIscMyAxxess_CsvParser::SerializeIn_HolidayTable(	FILE* hCvsFile,
															char* pLine)
{
	int iBack = 0;
	char* sPrt = NULL;
	string sValue;
	unsigned int uiCnt = 0;

	FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM pItemHoliday;
	pItemHoliday.Init();

	vector<string> aName;
	vector<string> aValue;

    if (hCvsFile == NULL)
    {
		m_pAxxessReader->SetLastError(FEDM_ERROR_READ_FILE);
		return m_pAxxessReader->GetLastError();
    }

	m_pAxxessReader->ClearTable(FEDM_MYAXXESS_HOLIDAY_TABLE);

	// read second line
    if(fscanf(hCvsFile, "%128[^\n]\n", pLine) == EOF)
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_READ_FILE);
		return m_pAxxessReader->GetLastError();
	}

	// #### Check Table Header ####
	sPrt = strtok(pLine, ";");
	sValue = sPrt;
	FEDM_STRUPR((char *)(sValue.c_str()), sValue.length()+1);
	if (sValue != "NUMBER")
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_UNSUPPORTED_NAMESPACE);
		return m_pAxxessReader->GetLastError();
	}

	sPrt = strtok(NULL, ";");
	sValue = sPrt;
	FEDM_STRUPR((char *)(sValue.c_str()), sValue.length()+1);
	if (sValue != "HOLIDAY")
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_UNSUPPORTED_NAMESPACE);
		return m_pAxxessReader->GetLastError();
	}

	// #### read Table Data ####
	aName.push_back("");
	aValue.push_back("");

	
	// max. 256 Holiday Table Entries
	for (uiCnt = 0; uiCnt < 256; uiCnt++)
	{
		aValue[0] = "";

		// read next line
		if(fscanf(hCvsFile, "%128[^\n]\n", pLine) == EOF)
		{
			break;
		}

		// remove Number
		sPrt = strtok(pLine, ";");

		// get Holiday
		sPrt = strtok(NULL, ";");
		if (sPrt == NULL)
		{
			m_pAxxessReader->SetLastError(FEDM_ERROR_PARAMETER);
			return m_pAxxessReader->GetLastError();
		}
		aName[0] = "Holiday";
		aValue[0] = sPrt;

		// build and add Holiday
		iBack = m_pAxxessReader->BuildTableItem(aName, aValue, &pItemHoliday);
		if(iBack != 0)
		{
			return iBack;
		}		
		iBack = m_pAxxessReader->AddTableItem(&pItemHoliday);
		if (iBack != 0)
		{
			return iBack;
		}
	}

	// all lines read
	if (uiCnt == 256)
	{
		if(fscanf(hCvsFile, "%128[^\n]\n", pLine) != EOF)
		{
			m_pAxxessReader->SetLastError(FEDM_ERROR_PARAMETER);
			return m_pAxxessReader->GetLastError();
		}
	}

	m_pAxxessReader->SetLastError(FEDM_OK);
	return m_pAxxessReader->GetLastError();
}

/***************************************************************************
  Begin        	:	29.06.2009 / B. Tschunko
  Version       :	03.00.08 / 01.07.2009 / B. Tschunko

  Function		:	read CSV  for doc type access table

  Parameters	:	FILE* hCvsFile	- opened CVS file
					char* pLine - pointer to read each line

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FedmIscMyAxxess_CsvParser::SerializeIn_AccessTable(	FILE* hCvsFile,
														char* pLine)
{
	int iBack = 0;
	char* sPrt = NULL;
	string sValue;
	unsigned int uiCnt = 0;
	unsigned int uiLength = 0;

	FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM pItemAccess;
	pItemAccess.Init();

	vector<string> aName;
	vector<string> aValue;

    if (hCvsFile == NULL)
    {
		m_pAxxessReader->SetLastError(FEDM_ERROR_READ_FILE);
		return m_pAxxessReader->GetLastError();
    }

	m_pAxxessReader->ClearTable(FEDM_MYAXXESS_ACCESS_TABLE);

	// read second line
    if(fscanf(hCvsFile, "%128[^\n]\n", pLine) == EOF)
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_READ_FILE);
		return m_pAxxessReader->GetLastError();
	}

	// #### Check IDD-Format Info ####
	sPrt = strtok(pLine, ";");
	sValue = sPrt;
	FEDM_STRUPR((char *)(sValue.c_str()), sValue.length()+1);
	if (sValue != "IDD-FORMAT")
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_UNSUPPORTED_NAMESPACE);
		return m_pAxxessReader->GetLastError();
	}

	sPrt = strtok(NULL, ";");
	sValue = sPrt;
	FEDM_STRUPR((char *)(sValue.c_str()), sValue.length()+1);
	if (sValue == "ASCII")
	{
		iBack = m_pAxxessReader->SetIDDFormat(FEDM_MYAXXESS_IDD_FORMAT_ASCII);
		if (iBack)
		{
			return iBack;
		}
	}
	else if(sValue == "NUM")
	{
		iBack = m_pAxxessReader->SetIDDFormat(FEDM_MYAXXESS_IDD_FORMAT_NUM);
		if (iBack)
		{
			return iBack;
		}
	}
	else if(sValue == "HEX")
	{
		iBack = m_pAxxessReader->SetIDDFormat(FEDM_MYAXXESS_IDD_FORMAT_HEX);
		if (iBack)
		{
			return iBack;
		}
	}
	else
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_PARAMETER);
		return m_pAxxessReader->GetLastError();
	}

	// read third line
    if(fscanf(hCvsFile, "%128[^\n]\n", pLine) == EOF)
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_READ_FILE);
		return m_pAxxessReader->GetLastError();
	}

	// #### Check IDD-Length Info ####
	sPrt = strtok(pLine, ";");
	sValue = sPrt;
	FEDM_STRUPR((char *)(sValue.c_str()), sValue.length()+1);
	if (sValue != "IDD-LENGTH")
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_UNSUPPORTED_NAMESPACE);
		return m_pAxxessReader->GetLastError();
	}

	sPrt = strtok(NULL, ";");
	sscanf(sPrt, "%d", &uiLength);
	iBack = m_pAxxessReader->SetIDDLength(uiLength);
	if (iBack)
	{
		return iBack;
	}

	// read forth line
    if(fscanf(hCvsFile, "%128[^\n]\n", pLine) == EOF)
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_READ_FILE);
		return m_pAxxessReader->GetLastError();
	}

	// #### Check Table Header ####
	sPrt = strtok(pLine, ";");
	sValue = sPrt;
	FEDM_STRUPR((char *)(sValue.c_str()), sValue.length()+1);
	if (sValue != "NUMBER")
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_UNSUPPORTED_NAMESPACE);
		return m_pAxxessReader->GetLastError();
	}

	sPrt = strtok(NULL, ";");
	sValue = sPrt;
	FEDM_STRUPR((char *)(sValue.c_str()), sValue.length()+1);
	if (sValue != "IDD")
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_UNSUPPORTED_NAMESPACE);
		return m_pAxxessReader->GetLastError();
	}

	sPrt = strtok(NULL, ";");
	sValue = sPrt;
	FEDM_STRUPR((char *)(sValue.c_str()), sValue.length()+1);
	if (sValue != "TIMEZONES")
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_UNSUPPORTED_NAMESPACE);
		return m_pAxxessReader->GetLastError();
	}
/*
	sPrt = strtok(NULL, ";");
	sValue = sPrt;
	FEDM_STRUPR((char *)(sValue.c_str()), sValue.length()+1);
	if (sValue != "RESERVED")
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_UNSUPPORTED_NAMESPACE);
		return m_pAxxessReader->GetLastError();
	}
*/

	// #### read Table Data ####
	aName.push_back("");
	aName.push_back("");
//	aName.push_back("");
	aValue.push_back("");
	aValue.push_back("");
//	aValue.push_back("");

	// max. 65535 Access Table Entries
	for (uiCnt = 0; uiCnt < 65535; uiCnt++)
	{
		aValue[0] = "";
		aValue[1] = "";
//		aValue[2] = "";

		// read next line
		if(fscanf(hCvsFile, "%128[^\n]\n", pLine) == EOF)
		{
			break;
		}

		// remove Number
		sPrt = strtok(pLine, ";");

		// get IDD
		sPrt = strtok(NULL, ";");
		if (sPrt == NULL)
		{
			m_pAxxessReader->SetLastError(FEDM_ERROR_PARAMETER);
			return m_pAxxessReader->GetLastError();
		}
		aName[0] = "IDD";
		aValue[0] = sPrt;


		// get Timezones
		sPrt = strtok(NULL, ";");
		if (sPrt == NULL)
		{
			m_pAxxessReader->SetLastError(FEDM_ERROR_PARAMETER);
			return m_pAxxessReader->GetLastError();
		}
		aName[1] = "Timezones";
		aValue[1] = sPrt;
/*
		// get Reserved
		sPrt = strtok(NULL, ";");
		if (sPrt == NULL)
		{
			m_pAxxessReader->SetLastError(FEDM_ERROR_PARAMETER);
			return m_pAxxessReader->GetLastError();
		}
		aName[2] = "Reserved";
		aValue[2] = sPrt;
*/		
		// build and add Access Entry
		iBack = m_pAxxessReader->BuildTableItem(aName, aValue, &pItemAccess);
		if(iBack != 0)
		{
			return iBack;
		}		
		iBack = m_pAxxessReader->AddTableItem(&pItemAccess);
		if (iBack != 0)
		{
			return iBack;
		}
	}

	// all lines read
	if (uiCnt == 65535)
	{
	    if(fscanf(hCvsFile, "%128[^\n]\n", pLine) != EOF)
		{
			m_pAxxessReader->SetLastError(FEDM_ERROR_PARAMETER);
			return m_pAxxessReader->GetLastError();
		}
	}

	m_pAxxessReader->SetLastError(FEDM_OK);
	return m_pAxxessReader->GetLastError();
}


/***************************************************************************
  Begin        	:	06.07.2009 / B. Tschunko
  Version       :	03.00.08 / 06.07.2009 / B. Tschunko

  Function		:	read CSV  for doc type event table

  Parameters	:	FILE* hCvsFile	- opened CVS file
					char* pLine - pointer to read each line

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FedmIscMyAxxess_CsvParser::SerializeIn_EventTable(	FILE* hCvsFile,
														char* pLine)
{
	m_pAxxessReader->SetLastError(FEDM_ERROR_UNSUPPORTED);
	return m_pAxxessReader->GetLastError();
}




/***************************************************************************
  Begin        	:	01.07.2009 / B. Tschunko
  Version       :	03.00.08 / 01.07.2009 / B. Tschunko

  Function		:	write CSV  for doc type timezone table

  Parameters	:	FILE* hCvsFile	- opened CVS file

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FedmIscMyAxxess_CsvParser::SerializeOut_TimezoneTable(	FILE* hCvsFile)
{
	unsigned int uiNumber = 0;
	char	buf[512];

    if (hCvsFile == NULL)
    {
		m_pAxxessReader->SetLastError(FEDM_ERROR_READ_FILE);
		return m_pAxxessReader->GetLastError();
    }

	// #### Write Table Identifier ####
	sprintf(buf, "TimezoneTable\n");
	fwrite(buf, sizeof(char), strlen(buf), hCvsFile);

	// #### Write Table Header ####
	sprintf(buf, "Number;Mo;Tu;We;Th;Fr;Sa;Su;DateFrom;TimeFrom;DateTo;TimeTo\n");
	fwrite(buf, sizeof(char), strlen(buf), hCvsFile);

	// #### Write Table Data ####
	FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM*	pTimezoneItem	= NULL;
	vector<FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM*>::iterator	itorTimezone;

	uiNumber = 0;
	// load Holiday Table
	for(itorTimezone  = m_pAxxessReader->m_TimezoneTable.begin();
		itorTimezone != m_pAxxessReader->m_TimezoneTable.end();
		itorTimezone++)
	{
		if ((*itorTimezone) == NULL)
			continue;

		pTimezoneItem = *itorTimezone;

		switch (m_pAxxessReader->m_ucDateFormat)
		{
		case FEDM_MYAXXESS_DATE_FORMAT_ISO8601:
			sprintf(buf, 
					"%i;%d;%d;%d;%d;%d;%d;%d;20%02d-%02d-%02d;%02d:%02d;20%02d-%02d-%02d;%02d:%02d\n",	
					++uiNumber,
					((pTimezoneItem->ucDays>>1) & 0x01),
					((pTimezoneItem->ucDays>>2) & 0x01),
					((pTimezoneItem->ucDays>>3) & 0x01),
					((pTimezoneItem->ucDays>>4) & 0x01),
					((pTimezoneItem->ucDays>>5) & 0x01),
					((pTimezoneItem->ucDays>>6) & 0x01),
					(pTimezoneItem->ucDays & 0x01),
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
			sprintf(buf, 
					"%i;%d;%d;%d;%d;%d;%d;%d;%02d.%02d.20%02d;%02d:%02d;%02d.%02d.20%02d;%02d:%02d\n",	
					++uiNumber,
					((pTimezoneItem->ucDays>>1) & 0x01),
					((pTimezoneItem->ucDays>>2) & 0x01),
					((pTimezoneItem->ucDays>>3) & 0x01),
					((pTimezoneItem->ucDays>>4) & 0x01),
					((pTimezoneItem->ucDays>>5) & 0x01),
					((pTimezoneItem->ucDays>>6) & 0x01),
					(pTimezoneItem->ucDays & 0x01),
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
			sprintf(buf, 
					"%i;%d;%d;%d;%d;%d;%d;%d;%02d/%02d/20%02d;%02d:%02d;%02d/%02d/20%02d;%02d:%02d\n",	
					++uiNumber,
					((pTimezoneItem->ucDays>>1) & 0x01),
					((pTimezoneItem->ucDays>>2) & 0x01),
					((pTimezoneItem->ucDays>>3) & 0x01),
					((pTimezoneItem->ucDays>>4) & 0x01),
					((pTimezoneItem->ucDays>>5) & 0x01),
					((pTimezoneItem->ucDays>>6) & 0x01),
					(pTimezoneItem->ucDays & 0x01),
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
			m_pAxxessReader->SetLastError(FEDM_ERROR_PARAMETER);
			return m_pAxxessReader->GetLastError();
		}

		fwrite(buf, sizeof(char), strlen(buf), hCvsFile);
	}

	m_pAxxessReader->SetLastError(FEDM_OK);
	return m_pAxxessReader->GetLastError();
}

/***************************************************************************
  Begin        	:	01.07.2009 / B. Tschunko
  Version       :	03.00.08 / 01.07.2009 / B. Tschunko

  Function		:	write CSV  for doc type holiday table

  Parameters	:	FILE* hCvsFile	- opened CVS file

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FedmIscMyAxxess_CsvParser::SerializeOut_HolidayTable(	FILE* hCvsFile)
{
	unsigned int uiNumber = 0;
	char	buf[512];

    if (hCvsFile == NULL)
    {
		m_pAxxessReader->SetLastError(FEDM_ERROR_READ_FILE);
		return m_pAxxessReader->GetLastError();
    }

	// #### Write Table Identifier ####
	sprintf(buf, "HolidayTable\n");
	fwrite(buf, sizeof(char), strlen(buf), hCvsFile);

	// #### Write Table Header ####
	sprintf(buf, "Number;Holiday\n");
	fwrite(buf, sizeof(char), strlen(buf), hCvsFile);

	// #### Write Table Data ####
	FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM*	pHolidayItem	= NULL;
	vector<FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM*>::iterator	itorHoliday;

	uiNumber = 0;
	// load Holiday Table
	for(itorHoliday  = m_pAxxessReader->m_HolidayTable.begin();
		itorHoliday != m_pAxxessReader->m_HolidayTable.end();
		itorHoliday++)
	{
		if ((*itorHoliday) == NULL)
			continue;

		pHolidayItem = *itorHoliday;

		switch (m_pAxxessReader->m_ucDateFormat)
		{
		case FEDM_MYAXXESS_DATE_FORMAT_ISO8601:
			sprintf(buf, 
					"%i;20%02d-%02d-%02d\n",	
					++uiNumber,				
					pHolidayItem->ucHoliday_Year, 		
					pHolidayItem->ucHoliday_Month,		
					pHolidayItem->ucHoliday_Day);
			break;

		case FEDM_MYAXXESS_DATE_FORMAT_GER:
			sprintf(buf,
					"%i;%02d.%02d.20%02d\n",
					++uiNumber,
					pHolidayItem->ucHoliday_Day,
					pHolidayItem->ucHoliday_Month,
					pHolidayItem->ucHoliday_Year);
			break;

		case FEDM_MYAXXESS_DATE_FORMAT_US:
			sprintf(buf, 
					"%i;%02d/%02d/20%02d\n",
					++uiNumber,
					pHolidayItem->ucHoliday_Month,
					pHolidayItem->ucHoliday_Day,
					pHolidayItem->ucHoliday_Year);
			break;

		default:
			m_pAxxessReader->SetLastError(FEDM_ERROR_PARAMETER);
			return m_pAxxessReader->GetLastError();
		}

		fwrite(buf, sizeof(char), strlen(buf), hCvsFile);
	}

	m_pAxxessReader->SetLastError(FEDM_OK);
	return m_pAxxessReader->GetLastError();
}

/***************************************************************************
  Begin        	:	01.07.2009 / B. Tschunko
  Version       :	03.00.08 / 01.07.2009 / B. Tschunko

  Function		:	write CSV  for doc type access table

  Parameters	:	FILE* hCvsFile	- opened CVS file

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FedmIscMyAxxess_CsvParser::SerializeOut_AccessTable(	FILE* hCvsFile)
{
	unsigned int uiNumber = 0;
	char	buf[512];
	char	szIDD[256];
	unsigned __int64 ui64IDD = 0;
	bool bAddPlus = false;
	char szTimezones[64];

	memset(szIDD, 0, 64);

    if (hCvsFile == NULL)
    {
		m_pAxxessReader->SetLastError(FEDM_ERROR_READ_FILE);
		return m_pAxxessReader->GetLastError();
    }

	// #### Write Table Identifier ####
	sprintf(buf, "AccessTable\n");
	fwrite(buf, sizeof(char), strlen(buf), hCvsFile);

	// #### IDD-Format Info ####
	switch (m_pAxxessReader->m_Metadata.ucIDDFormat)
	{
	case FEDM_MYAXXESS_IDD_FORMAT_ASCII:
		sprintf(buf, "IDD-Format;ASCII\n");
		break;

	case FEDM_MYAXXESS_IDD_FORMAT_NUM:
		sprintf(buf, "IDD-Format;Num\n");
		break;

	case FEDM_MYAXXESS_IDD_FORMAT_HEX:
		sprintf(buf, "IDD-Format;Hex\n");
		break;

	default:
		m_pAxxessReader->SetLastError(FEDM_ERROR_PARAMETER);
		return m_pAxxessReader->GetLastError();
	}
	fwrite(buf, sizeof(char), strlen(buf), hCvsFile);

	// #### IDD-Length Info ####
	if ((m_pAxxessReader->m_Metadata.ucIDDLength < 1) || (m_pAxxessReader->m_Metadata.ucIDDLength > 64))
	{
		m_pAxxessReader->SetLastError(FEDM_ERROR_PARAMETER);
		return m_pAxxessReader->GetLastError();
	}
	sprintf(buf, "IDD-Length;%d\n", m_pAxxessReader->m_Metadata.ucIDDLength);
	fwrite(buf, sizeof(char), strlen(buf), hCvsFile);

	// #### Write Table Header ####
	sprintf(buf, "Number;IDD;Timezones;Reserved\n");
	fwrite(buf, sizeof(char), strlen(buf), hCvsFile);

	// #### Write Table Data ####
	FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM*	pAccessItem	= NULL;
	vector<FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM*>::iterator	itorAccess;

	uiNumber = 0;

	// load Access Table
	for(itorAccess  = m_pAxxessReader->m_AccessTable.begin();
		itorAccess != m_pAxxessReader->m_AccessTable.end();
		itorAccess++)
	{
		if ((*itorAccess) == NULL)
			continue;

		pAccessItem = *itorAccess;

		switch(m_pAxxessReader->m_Metadata.ucIDDFormat)
		{
		case FEDM_MYAXXESS_IDD_FORMAT_ASCII:
			memcpy(szIDD, pAccessItem->ucIDD, m_pAxxessReader->m_Metadata.ucIDDLength);
			szIDD[m_pAxxessReader->m_Metadata.ucIDDLength] = '\0';
			break;

		case FEDM_MYAXXESS_IDD_FORMAT_NUM:
			if(strlen((char*)pAccessItem->ucIDD) > 20)
			{
				m_pAxxessReader->SetLastError(FEDM_ERROR_STRING_LENGTH);
				return m_pAxxessReader->GetLastError();
			}
			ui64IDD = 0;
			for (unsigned char ucByte = 0; ucByte < m_pAxxessReader->m_Metadata.ucIDDLength; ucByte++)
			{
				// change Motorola format to Intel format
				ui64IDD += ( (unsigned __int64)pAccessItem->ucIDD[m_pAxxessReader->m_Metadata.ucIDDLength-ucByte-1] << (8*ucByte) );
			}
			sprintf(szIDD,"%lld",ui64IDD);
			break;

		case FEDM_MYAXXESS_IDD_FORMAT_HEX:
			FEDM_ConvHexUCharToHexChar(pAccessItem->ucIDD, m_pAxxessReader->m_Metadata.ucIDDLength, szIDD, 256);
			break;
		}

		bAddPlus = false;
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

		sprintf(buf, 
				"%i;%s;%s;%d\n",	
				++uiNumber,
				szIDD,
				szTimezones,
				pAccessItem->ucReserved);

		fwrite(buf, sizeof(char), strlen(buf), hCvsFile);
	}

	m_pAxxessReader->SetLastError(FEDM_OK);
	return m_pAxxessReader->GetLastError();
}


/***************************************************************************
  Begin        	:	01.07.2009 / B. Tschunko
  Version       :	03.00.13 / 30.09.2009 / B. Tschunko, M. Hultsch

  Function		:	write CSV  for doc type event table

  Parameters	:	FILE* hCvsFile	- opened CVS file

  Return value	:	FEDM_OK or error code (<0)
***************************************************************************/
int FedmIscMyAxxess_CsvParser::SerializeOut_EventTable(	FILE* hCvsFile)
{
	unsigned int uiNumber = 0;
	char	buf[512];
	char	szIDD[256];
	char	szTimestamp[64];
	char	szStatus[32];
	char	szInput[32];
	char	szAnt[32];
	unsigned __int64 ui64IDD = 0;

    if (hCvsFile == NULL)
    {
		m_pAxxessReader->SetLastError(FEDM_ERROR_READ_FILE);
		return m_pAxxessReader->GetLastError();
    }

	memset(szIDD, 0, 64);
	memset(szTimestamp, 0, 64);
	memset(szStatus, 0, 32);
	memset(szInput, 0, 32);
	memset(szAnt, 0, 32);

	// #### Write Table Identifier ####
	sprintf(buf, "EventTable\n");
	fwrite(buf, sizeof(char), strlen(buf), hCvsFile);

	// #### Write IDD Format ####
	switch (m_pAxxessReader->m_Metadata.ucIDDFormat)
	{
	case FEDM_MYAXXESS_IDD_FORMAT_ASCII:
		sprintf(buf, "IDD-Format;ASCII\n");
		break;

	case FEDM_MYAXXESS_IDD_FORMAT_NUM:
		sprintf(buf, "IDD-Format;Num\n");
		break;

	case FEDM_MYAXXESS_IDD_FORMAT_HEX:
		sprintf(buf, "IDD-Format;Hex\n");
		break;

	default:
		m_pAxxessReader->SetLastError(FEDM_ERROR_PARAMETER);
		return m_pAxxessReader->GetLastError();
	}
	fwrite(buf, sizeof(char), strlen(buf), hCvsFile);

	// #### Write Table Header ####
	sprintf(buf, "Number;EventID;IDD;Date;Time;Input;Ant;Action;Error;Source\n");
	fwrite(buf, sizeof(char), strlen(buf), hCvsFile);

	// #### Write Table Data ####
	FEDM_ISC_MYAXXESS_EVENT_TABLE_ITEM*	pEventItem	= NULL;

	uiNumber = 0;
	// load Event Table
	while(! m_pAxxessReader->m_EventTable.empty())
	{
		pEventItem = m_pAxxessReader->m_EventTable.front();
		if(pEventItem == NULL)
			continue;

		if(pEventItem->ucDataLayout & 0x01)
		{
			switch(m_pAxxessReader->m_Metadata.ucIDDFormat)
			{
			case FEDM_MYAXXESS_IDD_FORMAT_ASCII:
				memcpy(szIDD, pEventItem->ucIDD, pEventItem->ucIDDLength);
				szIDD[pEventItem->ucIDDLength] = '\0';
				break;

			case FEDM_MYAXXESS_IDD_FORMAT_NUM:
				if(strlen((char*)pEventItem->ucIDD) > 20)
				{
					m_pAxxessReader->m_EventTable.pop();
					m_pAxxessReader->SetLastError(FEDM_ERROR_STRING_LENGTH);
					return m_pAxxessReader->GetLastError();
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
			}
		}

		if(pEventItem->ucDataLayout & 0x02)
		{
			switch (m_pAxxessReader->m_ucDateFormat)
			{
			case FEDM_MYAXXESS_DATE_FORMAT_ISO8601:
				sprintf(szTimestamp,
						"20%02d-%02d-%02d;%02d:%02d",	
						pEventItem->ucYear, 
						pEventItem->ucMonth, 
						pEventItem->ucDay, 
						pEventItem->ucHour, 
						pEventItem->ucMinute);
				break;

			case FEDM_MYAXXESS_DATE_FORMAT_GER:
				sprintf(szTimestamp,
						"%02d.%02d.20%02d;%02d:%02d",	
						pEventItem->ucDay, 
						pEventItem->ucMonth, 
						pEventItem->ucYear,
						pEventItem->ucHour, 
						pEventItem->ucMinute);
				break;

			case FEDM_MYAXXESS_DATE_FORMAT_US:
				sprintf(szTimestamp, 
						"%02d/%02d/20%02d;%02d:%02d",	
						pEventItem->ucMonth,
						pEventItem->ucDay, 
						pEventItem->ucYear,
						pEventItem->ucHour, 
						pEventItem->ucMinute);
				break;

			default:
				m_pAxxessReader->SetLastError(FEDM_ERROR_PARAMETER);
				return m_pAxxessReader->GetLastError();
			}
		}

		if(pEventItem->ucDataLayout & 0x04)
		{
			sprintf(szStatus,
					"%d;%i",	
					pEventItem->ucAction,
					pEventItem->uiError);
		}

		if(pEventItem->ucDataLayout & 0x08)
		{
			sprintf(szInput,
					"%d",
					pEventItem->ucInput);
		}

		if(pEventItem->ucDataLayout & 0x10)
		{
			sprintf(szAnt,
					"%d",
					pEventItem->ucAnt);
		}

		sprintf(buf,
				"%i;%d;%s;%s;%s;%s;%s;%s\n",	
				++uiNumber,
				pEventItem->ucEventID,
				szIDD,
				szTimestamp,
				szInput,
				szAnt,
				szStatus,
				pEventItem->sSourceIP.c_str());


		fwrite(buf, sizeof(char), strlen(buf), hCvsFile);

		// item must be deleted, because item is no longer controlled by the queue
		m_pAxxessReader->m_EventTable.pop();

	} // while(! m_pAxxessReader->m_EventTable.empty())

	m_pAxxessReader->SetLastError(FEDM_OK);
	return m_pAxxessReader->GetLastError();
}
