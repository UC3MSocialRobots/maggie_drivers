/*-------------------------------------------------------
|                                                       |
|                  FedmIscMyAxxessTables.h              |
|                                                       |
---------------------------------------------------------

Copyright © 2009-2014	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	02.03.2009

Version       		:	04.06.03 / 05.06.2014 / M. Hultsch
						- initialization of struct _FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM with 0xFF, except ucDays

						03.01.00 / 09.02.2010 / M. Hultsch

Operation Systems	:	independent

Function			:	table structure declarations for OBID i-scan and OBID classic-pro reader family with myAxxess functionality

OBID® and OBID i-scan® are registered Trademarks of FEIG ELECTRONIC GmbH
Linux® is a registered trademark of Linus Torvalds.
Windows is a registered trademark of Microsoft Corporation in the United States and other countries.
*/

#if !defined(_FEDM_ISC_MYAXXESS_TABLES_)
#define _FEDM_ISC_MYAXXESS_TABLES_


#include <string>


// STL - Standard Template Library
using namespace std;



//#####################################################################################
// crc list structure
//#####################################################################################

typedef struct _FEDM_ISC_MYAXXESS_CRCLIST
{
	unsigned short int	uiCrcMetadata;			// 
	unsigned short int	uiCrcTimezone;			// 
	unsigned short int	uiCrcHoliday;			// 
	unsigned short int	uiCrcAccess;			// 

	// initialization
	void Init()
	{
		memset(&uiCrcMetadata, 0, sizeof(_FEDM_ISC_MYAXXESS_CRCLIST));
	}


} FEDM_ISC_MYAXXESS_CRCLIST;


//#####################################################################################
// metadata structure
//#####################################################################################

typedef struct _FEDM_ISC_MYAXXESS_METADATA
{
	unsigned short int	uiTableVersion;			// 
	unsigned char		ucTimezoneEntries;		// 
	unsigned char		ucHolidayEntries;		// 
	unsigned short int	uiAccessEntries;	    // 
	unsigned char		ucIDDLength;			// 
	unsigned char		ucIDDFormat;			// 
	unsigned char		ucReserved[8];			// 

	// initialization
	void Init()
	{
		memset(&uiTableVersion, 0, sizeof(_FEDM_ISC_MYAXXESS_METADATA));
	}


} FEDM_ISC_MYAXXESS_METADATA;


//#####################################################################################
// timezone structure
//#####################################################################################

typedef struct _FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM
{
	unsigned char	ucStartDate_Day;			// 
	unsigned char	ucStartDate_Month;			// 
	unsigned char	ucStartDate_Year;			// 
	unsigned char	ucEndDate_Day;				// 
	unsigned char	ucEndDate_Month;			// 
	unsigned char	ucEndDate_Year;				// 
	unsigned char	ucDays;						// 
	unsigned char	ucStartTime_Hour;			// 
	unsigned char	ucStartTime_Minute;			// 
	unsigned char	ucEndTime_Hour;				// 
	unsigned char	ucEndTime_Minute;			// 

	// initialization
	void Init()
	{
		memset(&ucStartDate_Day, 0xFF, sizeof(_FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM));
		ucDays = 0x00;
	}


} FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM;


//#####################################################################################
// holiday structure
//#####################################################################################

typedef struct _FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM
{
	unsigned char	ucHoliday_Day;				//
	unsigned char	ucHoliday_Month;			//
	unsigned char	ucHoliday_Year;				//

	// initialization
	void Init()
	{
		memset(&ucHoliday_Day, 0, sizeof(_FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM));
	}


} FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM;


//#####################################################################################
// access data structure
//#####################################################################################

typedef struct _FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM
{
	unsigned char		ucIDD[128];				//
	unsigned short int	uiTimezones;			//
	unsigned char		ucReserved;				//

	// initialization
	void Init()
	{
		memset(&ucIDD[0], 0, sizeof(_FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM));
	}


} FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM;


//#####################################################################################
// logdata structure
//#####################################################################################

typedef struct _FEDM_ISC_MYAXXESS_EVENT_TABLE_ITEM
{
	unsigned char		ucDataLayout;			// flagfield with flag for each valid record member
												// IDD is signaled with bit 0
												// Timestamp is signaled with bit 1
												// Status is signaled with bit 2
												// Input is signaled with bit 3
												// Ant is signaled with bit 4

	unsigned char		ucEventID;				//
	unsigned char		ucIDDLength;			// number of bytes
	unsigned char		ucIDD[128];				// IDD buffer
	unsigned char		ucDay;					// 
	unsigned char		ucMonth;				//
	unsigned char		ucYear;					//
	unsigned char		ucHour;					// 
	unsigned char		ucMinute;				// 
	unsigned char		ucSecond;				// 
	unsigned char		ucAction;				// 
	unsigned short int	uiError;				// 
	unsigned char		ucInput;				// 
	unsigned char		ucAnt;					// 
	string				sSourceIP;				// IP-Address of source

	// initialization
	void Init()
	{
		ucDataLayout= 0;
		ucEventID	= 0;
		ucIDDLength	= 0;
		memset(ucIDD, 0, 128);
		ucDay		= 0;
		ucYear		= 0;
		ucMonth		= 0;
		ucHour		= 0;
		ucMinute	= 0;
		ucSecond	= 0;
		ucAction	= 0;
		uiError		= 0;
		ucInput		= 0;
		ucAnt		= 0;
		sSourceIP.clear();
	}


} FEDM_ISC_MYAXXESS_EVENT_TABLE_ITEM;


#endif // !defined(_FEDM_ISC_MYAXXESS_TABLES_)
