/*-------------------------------------------------------
|                                                       |
|	FedmIscMyAxxess_XmlParser.h  			            |
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
Version       		:	03.00.08 / 25.06.2009 / M. Hultsch
Operation Systems	:	independent
Function			:	parser for myAxxess Data Files

OBID® and OBID i-scan® are registered Trademarks of FEIG ELECTRONIC GmbH.
Linux® is a registered trademark of Linus Torvalds.
Microsoft® and Windows® are registered trademarks of Microsoft Corporation.
*/


//###########################
//      D E F I N E S
//###########################
#ifndef _FEDM_ISC_MYAXXESS_XMLPARSER_FILE_H
#define _FEDM_ISC_MYAXXESS_XMLPARSER_FILE_H

#if defined(_FEDM_MYAXXESS_XML_SUPPORT)


#ifdef _MSC_VER
	// The (microsoft) debugger can't handle symbols more than 255 characters long.
	// STL often creates symbols longer than that.
	// When symbols are longer than 255 characters, the warning is disabled.
	#pragma warning(disable:4786)	// for MFC
#endif


//#############################
//      I N C L U D E S
//#############################
#include <map>
#include "FEDM_XmlParser_Base.h"	// base class
#include "FedmIscMyAxxessReader.h"
#include "FedmIscMyAxxess_XmlParser.h"
#include "FedmIscMyAxxess_CsvParser.h"

class FedmIscMyAxxessReader;

//#######################################################################################
//	class FedmIscMyAxxess_XmlParser
//#######################################################################################
class FedmIscMyAxxess_XmlParser : public FEDM_XmlParser_Base
{
public:

	friend class FedmIscMyAxxessReader;

	FedmIscMyAxxess_XmlParser(FedmIscMyAxxessReader* pAxxessReader);
	virtual ~FedmIscMyAxxess_XmlParser(void);

	// serialization functions
	int SerializeIn(	char* szXmlFileName,
						char* szAccessCode,
						unsigned int uiAccessCodeLen );

	int SerializeOut(	unsigned int uiTableID,
						char* szXmlFileName,
						char* szAccessCode,
						unsigned int uiAccessCodeLen );

protected:
	int SerializeIn_AccessTable(FEDM_XML_TREEITEM* root);
	int SerializeIn_HolidayTable(FEDM_XML_TREEITEM* root);
	int SerializeIn_TimezoneTable(FEDM_XML_TREEITEM* root);
	int SerializeIn_EventTable(FEDM_XML_TREEITEM* root);

	int SerializeOut_AccessTable(FEDM_XML_TREEITEM* root);
	int SerializeOut_HolidayTable(FEDM_XML_TREEITEM* root);
	int SerializeOut_TimezoneTable(FEDM_XML_TREEITEM* root);
	int SerializeOut_EventTable(FEDM_XML_TREEITEM* root, unsigned int& uiCrc);

	unsigned int CalcChecksum_AccessTable();
	unsigned int CalcChecksum_HolidayTable();
	unsigned int CalcChecksum_TimezoneTable();

	void SetComment();

protected:
	FedmIscMyAxxessReader* m_pAxxessReader;
};

#endif // #if defined(_FEDM_MYAXXESS_XML_SUPPORT)

#endif // #ifndef _FEDM_ISC_MYAXXESS_XMLPARSER_FILE_H

