/*-------------------------------------------------------
|                                                       |
|	FedmIscMyAxxess_CsvParser.h  			            |
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
Version       		:	03.01.00 / 09.02.2010 / B. Tschunko
Operation Systems	:	independent
Function			:	parser for myAxxess Data Files

OBID® and OBID i-scan® are registered Trademarks of FEIG ELECTRONIC GmbH.
Linux® is a registered trademark of Linus Torvalds.
Microsoft® and Windows® are registered trademarks of Microsoft Corporation.
*/


//###########################
//      D E F I N E S
//###########################
#ifndef _FEDM_ISC_MYAXXESS_CSVPARSER_FILE_H
#define _FEDM_ISC_MYAXXESS_CSVPARSER_FILE_H



//#############################
//      I N C L U D E S
//#############################
#include "FedmIscMyAxxessReader.h"


//#####################################################################################
// constants
//#####################################################################################

#define FEDM_CSV_MAX_FILENAME_SIZE		512

//#####################################################################################
// types
//#####################################################################################

class FedmIscMyAxxessReader;


//#######################################################################################
//	class FedmIscMyAxxess_CsvParser
//#######################################################################################
class FedmIscMyAxxess_CsvParser
{
public:

	friend class FedmIscMyAxxessReader;

	FedmIscMyAxxess_CsvParser(FedmIscMyAxxessReader* pAxxessReader);
	virtual ~FedmIscMyAxxess_CsvParser(void);

	// serialization functions
	int SerializeIn(	char* szCsvFileName );

	int SerializeOut(	unsigned int uiTableID,
						char* szCsvFileName );

protected:
	int SerializeIn_HolidayTable(	FILE* hCvsFile,
									char* pLine);
	int SerializeIn_TimezoneTable(	FILE* hCvsFile,
									char* pLine);
	int SerializeIn_AccessTable(	FILE* hCvsFile,
									char* pLine);
	int SerializeIn_EventTable(		FILE* hCvsFile,
									char* pLine);

	int SerializeOut_TimezoneTable(	FILE* hCvsFile);
	int SerializeOut_HolidayTable(	FILE* hCvsFile);
	int SerializeOut_AccessTable(	FILE* hCvsFile);
	int SerializeOut_EventTable(	FILE* hCvsFile);

protected:
	FedmIscMyAxxessReader* m_pAxxessReader;
};

#endif // #ifndef _FEDM_ISC_MYAXXESS_CSVPARSER_FILE_H

