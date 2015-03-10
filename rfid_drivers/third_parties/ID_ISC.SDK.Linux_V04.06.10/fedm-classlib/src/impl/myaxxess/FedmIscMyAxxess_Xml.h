/*-------------------------------------------------------
|                                                       |
|                  FedmIscMyAxxess_Xml.h	            |
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
Begin        		:	23.06.2009

Version       		:	03.00.08 / 29.06.2009 / M. Hultsch

Operation Systems	:	independent

Function			:	basic defines for myAxxess XML documents

OBID® and OBID i-scan® are registered Trademarks of FEIG ELECTRONIC GmbH
Linux® is a registered trademark of Linus Torvalds.
Windows is a registered trademark of Microsoft Corporation in the United States and other countries.
*/



#if !defined(_FEDM_ISC_MYAXXESS_XML_H_INCLUDED_)
#define _FEDM_ISC_MYAXXESS_XML_H_INCLUDED_




//#####################################################################################
// constants
//#####################################################################################

// UNICODE strings for file
#define FEDM_XML_TAG_ACCESS_TABLE_UC			L"access-table"
#define FEDM_XML_TAG_HOLIDAY_TABLE_UC			L"holiday-table"
#define FEDM_XML_TAG_TIMEZONE_TABLE_UC			L"timezone-table"
#define FEDM_XML_TAG_EVENT_TABLE_UC				L"event-table"
#define FEDM_XML_TAG_IDD_FORMAT_UC				L"idd-format"
#define FEDM_XML_TAG_IDD_LENGTH_UC				L"idd-length"
#define FEDM_XML_TAG_ACCESS_DATA_UC				L"access-data"
#define FEDM_XML_TAG_IDD_UC						L"idd"
#define FEDM_XML_TAG_TIMEZONES_UC				L"timezones"
#define FEDM_XML_TAG_TIMEZONE_UC				L"timezone"
#define FEDM_XML_TAG_HOLIDAY_UC					L"holiday"
#define FEDM_XML_TAG_DAYS_UC					L"days"
#define FEDM_XML_TAG_MONDAY_UC					L"monday"
#define FEDM_XML_TAG_TUESDAY_UC					L"tuesday"
#define FEDM_XML_TAG_WEDNESDAY_UC				L"wednesday"
#define FEDM_XML_TAG_THURSDAY_UC				L"thursday"
#define FEDM_XML_TAG_FRIDAY_UC					L"friday"
#define FEDM_XML_TAG_SATURDAY_UC				L"saturday"
#define FEDM_XML_TAG_SUNDAY_UC					L"sunday"
#define FEDM_XML_TAG_DATE_FROM_UC				L"date-from"
#define FEDM_XML_TAG_DATE_TO_UC					L"date-to"
#define FEDM_XML_TAG_TIME_FROM_UC				L"time-from"
#define FEDM_XML_TAG_TIME_TO_UC					L"time-to"
#define FEDM_XML_TAG_EVENT_UC					L"event"
#define FEDM_XML_TAG_EVENT_ID_UC				L"event-id"
#define FEDM_XML_TAG_ACTION_UC					L"action"
#define FEDM_XML_TAG_ERROR_UC					L"error"
#define FEDM_XML_TAG_SOURCE_UC					L"source"
												
// UNICODE strings for standard attributes


// UNICODE strings for standard attribute values


// UNICODE strings for file header
#define FEDM_XML_STRING_MAX_AT_DOC_TYPE_UC		L"myAxxess-Reader Access Data Table File"
#define FEDM_XML_STRING_MAX_TT_DOC_TYPE_UC		L"myAxxess-Reader Timezone Table File"
#define FEDM_XML_STRING_MAX_HT_DOC_TYPE_UC		L"myAxxess-Reader Holiday Table File"
#define FEDM_XML_STRING_MAX_ET_DOC_TYPE_UC		L"myAxxess-Reader Event Table File"
#define FEDM_XML_STRING_MAX_DOC_VERSION_UC		L"1.0"		// document version

// strings in file header
#define FEDM_XML_STRING_MAX_AT_DOC_TYPE			"myAxxess-Reader Access Data Table File"
#define FEDM_XML_STRING_MAX_TT_DOC_TYPE			"myAxxess-Reader Timezone Table File"
#define FEDM_XML_STRING_MAX_HT_DOC_TYPE			"myAxxess-Reader Holiday Table File"
#define FEDM_XML_STRING_MAX_ET_DOC_TYPE			"myAxxess-Reader Event Table File"
#define FEDM_XML_STRING_MAX_DOC_VERSION			"1.0"		// document version



#endif // !defined(_FEDM_ISC_MYAXXESS_XML_H_INCLUDED_)
