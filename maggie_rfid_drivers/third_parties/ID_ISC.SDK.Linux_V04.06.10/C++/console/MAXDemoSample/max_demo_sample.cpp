// obid_data_exchange.cpp : Legt das Klassenverhalten f�r die Anwendung fest.
//

#include "FedmIscMyAxxessReader.h"

#include "math.h"

#define TRUE	 	1
#define FALSE 		0

int main (void)
{
	int iBack = 0;
	char cErrorText[256];

	// create reader-object with default IDD-Format numerical and IDD-Length of 5 bytes
	FedmIscMyAxxessReader m_MaxReader(5);

	m_MaxReader.SetDateFormat(FEDM_MYAXXESS_DATE_FORMAT_ISO8601);

	// create one holiday item (use typical overloaded method)...
	FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM	HolidayItem;
	HolidayItem.ucHoliday_Year = 9;
	HolidayItem.ucHoliday_Month = 9;
	HolidayItem.ucHoliday_Day = 31;
	// ...and add to table
	m_MaxReader.AddTableItem(&HolidayItem);

	// create two timezone items (use alternative method)...
	char tmp1[] = "Days=Mo+Tu+We+Th+Fr+Sa+Su;DateFrom=2009-09-03;TimeFrom=23:12;DateTo=2009-09-31;TimeTo=22:12;";
	char tmp2[] = "Days=Mo+Tu+We+Th+Fr+Sa+Su;DateFrom=2009-10-03;TimeFrom=23:12;DateTo=2009-10-31;TimeTo=22:12;";
	// ...and add to table
	iBack = m_MaxReader.AddTableItem(FEDM_MYAXXESS_TIMEZONE_TABLE, tmp1);
	if (iBack)
	{
		printf("1: %s\n", m_MaxReader.GetErrorText(iBack));
		return iBack;
	}
	iBack = m_MaxReader.AddTableItem(FEDM_MYAXXESS_TIMEZONE_TABLE, tmp2);
	if (iBack)
	{
		printf("2: %s\n", m_MaxReader.GetErrorText(iBack));
		return iBack;
	}

	// create one access item (use alternative method)...
	char tmp4[] = "IDD=55555;Timezones=2;Reserved=0";
	// ...and add to table
	iBack = m_MaxReader.AddTableItem(FEDM_MYAXXESS_ACCESS_TABLE, tmp4);
	if (iBack)
	{
		printf("3: %s\n", m_MaxReader.GetErrorText(iBack));
		return iBack;
	}
	char tmp5[] = "IDD=1;Timezones=1+2;Reserved=0";
	iBack = m_MaxReader.AddTableItem(FEDM_MYAXXESS_ACCESS_TABLE, tmp5);
	if (iBack)
	{
		printf("4: %s\n", m_MaxReader.GetErrorText(iBack));
		return iBack;
	}
	char tmp6[] = "IDD=256;Timezones=1;Reserved=0";
	iBack = m_MaxReader.AddTableItem(FEDM_MYAXXESS_ACCESS_TABLE, tmp6);
	if (iBack)
	{
		printf("5: %s\n", m_MaxReader.GetErrorText(iBack));
		return iBack;
	}
	char tmp7[] = "IDD=9;Timezones=1;Reserved=0";
	iBack = m_MaxReader.AddTableItem(FEDM_MYAXXESS_ACCESS_TABLE, tmp7);
	if (iBack)
	{
		printf("6: %s\n", m_MaxReader.GetErrorText(iBack));
		return iBack;
	}

	// establish a connection to the Reader
	iBack = m_MaxReader.GetReaderObject()->ConnectTCP("192.168.3.159", 10001);
	if(iBack)
	{
		printf("7: (192.168.3.159) %s\n", m_MaxReader.GetErrorText(iBack));
		return iBack;
	}

	// read all reader info and set reader type for internal use
	FEDM_ISC_READER_INFO* pInfo = m_MaxReader.GetReaderObject()->ReadReaderInfo();
	if(pInfo ==NULL)
	{
		printf("8: %s\n", m_MaxReader.GetErrorText(m_MaxReader.GetLastError()));
		m_MaxReader.GetReaderObject()->DisConnect();
		return iBack;
	}

	FedmIscReport_ReaderInfo report(m_MaxReader.GetReaderObject());
	string sReport;
	report.BuildReport(0xFF, sReport);

	printf("OBID Reader detected. Type is %s\n", m_MaxReader.GetReaderObject()->GetReaderName());
	printf("Reader Info:\n%s\n", sReport.c_str());

	// write all tables
	iBack = m_MaxReader.WriteTables();
	if (iBack)
	{
		printf("9: %s\n", m_MaxReader.GetErrorText(iBack));
		m_MaxReader.GetReaderObject()->DisConnect();
		return iBack;
	}


	// read tables from reader
	iBack = m_MaxReader.ReadTable(FEDM_MYAXXESS_CRC_LIST);
	if (iBack)
	{
		printf("10: %s\n", m_MaxReader.GetErrorText(iBack));
		m_MaxReader.GetReaderObject()->DisConnect();
		return iBack;
	}

	iBack = m_MaxReader.ReadTable(FEDM_MYAXXESS_METADATA);
	if (iBack)
	{
		printf("11: %s\n", m_MaxReader.GetErrorText(iBack));
		m_MaxReader.GetReaderObject()->DisConnect();
		return iBack;
	}

	
	iBack = m_MaxReader.ReadTable(FEDM_MYAXXESS_TIMEZONE_TABLE);
	if (iBack)
	{
		printf("12: %s\n", m_MaxReader.GetErrorText(iBack));
		m_MaxReader.GetReaderObject()->DisConnect();
		return iBack;
	}

	iBack = m_MaxReader.ReadTable(FEDM_MYAXXESS_HOLIDAY_TABLE);
	if (iBack)
	{
		printf("13: %s\n", m_MaxReader.GetErrorText(iBack));
		m_MaxReader.GetReaderObject()->DisConnect();
		return iBack;
	}

	iBack = m_MaxReader.ReadTable(FEDM_MYAXXESS_ACCESS_TABLE);
	if (iBack)
	{
		printf("14: %s\n", m_MaxReader.GetErrorText(iBack));
		m_MaxReader.GetReaderObject()->DisConnect();
		return iBack;
	}

	// save access table in CSV-File
	iBack = m_MaxReader.SerializeTableOut(FEDM_MYAXXESS_ACCESS_TABLE, "Tabelle.csv", FEDM_MYAXXESS_FILETYPE_CSV);
	if (iBack)
	{
		printf("15: %s\n", m_MaxReader.GetErrorText(iBack));
		m_MaxReader.GetReaderObject()->DisConnect();
		return iBack;
	}

	// clear access table
	iBack = m_MaxReader.ClearTable(FEDM_MYAXXESS_ACCESS_TABLE);
	if (iBack)
	{
		printf("16: %s\n", m_MaxReader.GetErrorText(iBack));
		m_MaxReader.GetReaderObject()->DisConnect();
		return iBack;
	}

	// read access table from file
	iBack = m_MaxReader.SerializeTableIn("Tabelle.csv", FEDM_MYAXXESS_FILETYPE_CSV);
	if (iBack)
	{
		printf("17: %s\n", m_MaxReader.GetErrorText(iBack));
		m_MaxReader.GetReaderObject()->DisConnect();
		return iBack;
	}

	// write all tables
	iBack = m_MaxReader.WriteTables();
	if (iBack)
	{
		printf("18: %s\n", m_MaxReader.GetErrorText(iBack));
		m_MaxReader.GetReaderObject()->DisConnect();
		return iBack;
	}

	// read access table from Reader
	iBack = m_MaxReader.ReadTable(FEDM_MYAXXESS_ACCESS_TABLE);
	if (iBack)
	{
		printf("19: %s\n", m_MaxReader.GetErrorText(iBack));
		m_MaxReader.GetReaderObject()->DisConnect();
		return iBack;
	}

	// save access table in CSV-File
	iBack = m_MaxReader.SerializeTableOut(FEDM_MYAXXESS_ACCESS_TABLE, "Tabelle2.csv", FEDM_MYAXXESS_FILETYPE_CSV);
	if (iBack)
	{
		printf("20: %s\n", m_MaxReader.GetErrorText(iBack));
		m_MaxReader.GetReaderObject()->DisConnect();
		return iBack;
	}
		
	printf("OK\n");
	m_MaxReader.GetReaderObject()->DisConnect();
	return TRUE;
}
