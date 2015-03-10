/*-------------------------------------------------------
|                                                       |
|                  FedmIscMyAxxessReader.h              |
|                                                       |
---------------------------------------------------------

Copyright © 2009-2012	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	02.03.2009

Version       		:	04.03.01 / 27.08.2012 / M. Hultsch
						- declaration of event callbacks modified for multiple records per event
						- support for alarm event and request event

						04.00.02 / 29.08.2011 / B. Tschunko, M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID i-scan and OBID classic-pro reader family with myAxxess functionality

OBID and OBID i-scan are registered Trademarks of FEIG ELECTRONIC GmbH
Linux is a registered trademark of Linus Torvalds.
Windows is a registered trademark of Microsoft Corporation in the United States and other countries.
*/

#if !defined(_FEDM_ISC_MYAXXESS_READER_H_INCLUDED_)
#define _FEDM_ISC_MYAXXESS_READER_H_INCLUDED_



#include <vector>
#include <queue>
#include "FedmIscCore.h"
#include "FedmIscMyAxxessTables.h"


// STL - Standard Template Library
using namespace std;


#ifdef _MSC_VER
	#ifdef _FEDM_MYAXXESS_DLL
		#define _FEDM_MYAXXESS_EXT_CLASS __declspec(dllexport)
	#else
		#if !defined(_FEDM_DLL)
			#define	_FEDM_MYAXXESS_EXT_CLASS
		#else
			#define _FEDM_MYAXXESS_EXT_CLASS __declspec(dllimport)
		#endif
	#endif

//	#define _FEDM_ISC_CORE_EXT_CLASS //__declspec(dllimport)
//	#define _FEDM_ISC_CORE_EXT_FUNCT
#else
	#define _FEDM_MYAXXESS_EXT_CLASS
#endif


//#####################################################################################
// constants
//#####################################################################################
// table version
#define FEDM_MYAXXESS_TABLE_VERSION			0x00

// command types
#define FEDM_MYAXXESS_UPDATE_START			0x01
#define FEDM_MYAXXESS_UPDATE_END			0x02
#define FEDM_MYAXXESS_WRITE_TABLE			0x03
//#define FEDM_MYAXXESS_RESET_TABLE			0x04
#define FEDM_MYAXXESS_READ_TABLE			0x05
#define FEDM_MYAXXESS_NOTIFY				0x06

// table types
#define FEDM_MYAXXESS_CRC_LIST				0x00
#define FEDM_MYAXXESS_METADATA				0x01
#define FEDM_MYAXXESS_TIMEZONE_TABLE		0x02
#define FEDM_MYAXXESS_HOLIDAY_TABLE			0x03
#define FEDM_MYAXXESS_ACCESS_TABLE			0x04
#define FEDM_MYAXXESS_EVENT_TABLE			0x05

// event types
#define FEDM_MYAXXESS_EVENT_ACCESS			0x01
#define FEDM_MYAXXESS_EVENT_IO				0x02
#define FEDM_MYAXXESS_EVENT_ALARM			0x03
#define FEDM_MYAXXESS_EVENT_REQUEST			0x04

// idd format types
#define FEDM_MYAXXESS_IDD_FORMAT_ASCII		0x01
#define FEDM_MYAXXESS_IDD_FORMAT_NUM		0x02
#define FEDM_MYAXXESS_IDD_FORMAT_HEX		0x03

// date type types
#define FEDM_MYAXXESS_DATE_FORMAT_ISO8601	0x01	// separated by -
#define FEDM_MYAXXESS_DATE_FORMAT_GER		0x02	// separated by .
#define FEDM_MYAXXESS_DATE_FORMAT_US		0x03	// separated by /

// file types
#define FEDM_MYAXXESS_FILETYPE_XML			1
#define FEDM_MYAXXESS_FILETYPE_CSV			2

#define FEDM_MYAXXESS_MAX_BLOCKSIZE			32000
#define FEDM_MYAXXESS_SERIAL_BLOCKSIZE		 1024


//#####################################################################################
// types
//#####################################################################################

// callback functions for C++
typedef int		(FEDM_CALL*
				 LPFN_FEDM_MYAXXESS_EVENT_CB)(				void*,				// pAny
															int,				// error value
															vector<FEDM_ISC_MYAXXESS_EVENT_TABLE_ITEM*>, // event record
															unsigned char&,		// reference to action
															char*,				// IP-Address
															int );				// IP-Port

typedef void	(FEDM_CALL*
                 LPFN_FEDM_MYAXXESS_KEEPALIVE_CB)(			void*,				// pAny
															int,				// error value
															unsigned int,		// error flags from my axxess Reader
															unsigned int,		// event table size
															unsigned int,		// event table length
															char*,				// IP-Address
															int );				// IP-Port

// callback functions for .NET-Wrapper
typedef int		(CALLBACK* 
				 LPFN_FEDM_MYAXXESS_EVENT_DOTNET_CB)(		int,				// error value
															vector<FEDM_ISC_MYAXXESS_EVENT_TABLE_ITEM*>, // event record
															unsigned char&,		// reference to action
															char*,				// IP-Address
															int );				// IP-Port

typedef void	(CALLBACK* 
				 LPFN_FEDM_MYAXXESS_KEEPALIVE_DOTNET_CB)(	int,				// error value
															unsigned int,		// error flags from my axxess Reader
															unsigned int,		// event table size
															unsigned int,		// event table length
															char*,				// IP-Address
															int );				// IP-Port


class FedmIscMyAxxess_XmlParser;
class FedmIscMyAxxess_CsvParser;


//#####################################################################################
// class FedmIscMyAxxessReader
//#####################################################################################

class FedmIscMyAxxessReader
{

#if defined(_FEDM_MYAXXESS_XML_SUPPORT)
	friend class FedmIscMyAxxess_XmlParser;
#endif
	friend class FedmIscMyAxxess_CsvParser;

public:
	// main constructor
	_FEDM_MYAXXESS_EXT_CLASS FedmIscMyAxxessReader(	unsigned char ucIDDLength, 
							unsigned char ucIDDFormat = FEDM_MYAXXESS_IDD_FORMAT_NUM);
	
	// constructor for wrapper projects (.NET, JNI)
	_FEDM_MYAXXESS_EXT_CLASS FedmIscMyAxxessReader(	FEDM_ISCReaderModule* pReaderModule, 
							unsigned char ucIDDLength, 
							unsigned char ucIDDFormat = FEDM_MYAXXESS_IDD_FORMAT_NUM);

	_FEDM_MYAXXESS_EXT_CLASS ~FedmIscMyAxxessReader();

	_FEDM_MYAXXESS_EXT_CLASS FEDM_ISCReaderModule* GetReaderObject()	{ return m_pReaderModule; }

	_FEDM_MYAXXESS_EXT_CLASS unsigned char GetIDDLength(void)			{ return m_Metadata.ucIDDLength; }
	_FEDM_MYAXXESS_EXT_CLASS unsigned char GetIDDFormat(void)			{ return m_Metadata.ucIDDFormat; }

	_FEDM_MYAXXESS_EXT_CLASS int SetDateFormat (unsigned char ucDateType);
	_FEDM_MYAXXESS_EXT_CLASS unsigned char GetDateFormat(void)			{ return m_ucDateFormat; }

	_FEDM_MYAXXESS_EXT_CLASS int   GetLastError();
	_FEDM_MYAXXESS_EXT_CLASS char* GetErrorText(int iErrorCode);

	// TODO_M - GetTableInfo: definieren, was Methode zurückgeben könnte
	//const char* GetTableInfo();

	// public API for table management
	_FEDM_MYAXXESS_EXT_CLASS int ClearTable(unsigned int uiTableID);
	
	_FEDM_MYAXXESS_EXT_CLASS int SerializeTableIn(	char* szFilename,
							unsigned int uiFileType=FEDM_MYAXXESS_FILETYPE_XML );
	
	_FEDM_MYAXXESS_EXT_CLASS int SerializeTableOut(	unsigned int uiTableID,
							char* szFilename,
							unsigned int uiFileType=FEDM_MYAXXESS_FILETYPE_XML );

	// for native C/C++ interface
	_FEDM_MYAXXESS_EXT_CLASS int AddTableItem(FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM* pItem);
	_FEDM_MYAXXESS_EXT_CLASS int AddTableItem(FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM* pItem);
	_FEDM_MYAXXESS_EXT_CLASS int AddTableItem(FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM* pItem);
	// for 'naturally speaking' interface; ready for VB, Delphi, and other stupid languages
	_FEDM_MYAXXESS_EXT_CLASS int AddTableItem(unsigned int uiTableID, char* sItem);

	// for native C/C++ interface and
	_FEDM_MYAXXESS_EXT_CLASS int GetTableItem(unsigned int uiIndex, FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM* pItem);
	_FEDM_MYAXXESS_EXT_CLASS int GetTableItem(unsigned int uiIndex, FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM* pItem);
	_FEDM_MYAXXESS_EXT_CLASS int GetTableItem(unsigned int uiIndex, FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM* pItem);
	_FEDM_MYAXXESS_EXT_CLASS int GetTableItem(FEDM_ISC_MYAXXESS_EVENT_TABLE_ITEM* pItem);
	// for 'naturally speaking' interface; ready for VB, Delphi, and other stupid languages
	_FEDM_MYAXXESS_EXT_CLASS int GetTableItem(unsigned int uiTableID, unsigned int uiIndex, char* sItem);

	// for native C/C++ interface and for 'naturally speaking' interface; ready for VB, Delphi, and other stupid languages
	_FEDM_MYAXXESS_EXT_CLASS int RemoveTableItem(unsigned int uiTableID, unsigned int uiIndex);

	// for native C/C++ interface and
	_FEDM_MYAXXESS_EXT_CLASS int ModifyTableItem(unsigned int uiIndex, FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM* pItem);
	_FEDM_MYAXXESS_EXT_CLASS int ModifyTableItem(unsigned int uiIndex, FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM* pItem);
	_FEDM_MYAXXESS_EXT_CLASS int ModifyTableItem(unsigned int uiIndex, FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM* pItem);
	// for 'naturally speaking' interface; ready for VB, Delphi, and other stupid languages
	_FEDM_MYAXXESS_EXT_CLASS int ModifyTableItem(unsigned int uiTableID, unsigned int uiIndex, char* sItem);

	// public API for tabel update
	_FEDM_MYAXXESS_EXT_CLASS int WriteTables();
	_FEDM_MYAXXESS_EXT_CLASS int ReadTable(unsigned int uiTableID);

	_FEDM_MYAXXESS_EXT_CLASS int StartEventHandler(	unsigned int uiPortNr,
													void* pAny,
													LPFN_FEDM_MYAXXESS_EVENT_CB cbEvent,
													LPFN_FEDM_MYAXXESS_KEEPALIVE_CB cbKeepAlive,
													int iAuthentType=-1,
													char* sAuthentKey=NULL );

	// only for .NET wrapper library OBIDISC4NETnative
	_FEDM_MYAXXESS_EXT_CLASS int StartEventHandler4DotNet(	unsigned int uiPortNr,
															void* pAny,
															LPFN_FEDM_MYAXXESS_EVENT_DOTNET_CB cbEvent,
															LPFN_FEDM_MYAXXESS_KEEPALIVE_DOTNET_CB cbKeepAlive,
															int iAuthentType=-1,
															char* sAuthentKey=NULL );
	
	_FEDM_MYAXXESS_EXT_CLASS int StopEventHandler();

	// callback function, invoked by asynchronous task
	_FEDM_MYAXXESS_EXT_CLASS static int	cbsTaskRsp3(void* pAny, 
													int iReaderHnd, 
													int iTaskID, 
													int iError, 
													unsigned char ucCmd,
													unsigned char* ucRspData, 
													int iRspLen,
													char* cIPAdr,
													int iPortNr,
													unsigned char& ucAction );

// protected methods
protected:
	int SetIDDLength (unsigned char ucIDDLength);
	int SetIDDFormat (unsigned char ucIDDFormat);

	int ValidateIDDSettings();

	void SetLastError(int iError);

	unsigned int CalcCRC16CheckSum(unsigned short int uiCrcStart, unsigned short int uiCrcPoly, unsigned char* ucBuffer, unsigned int uiBufLen);

	int BuildTableItem(vector<string> aName, vector<string> aValue, FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM* pItem);
	int BuildTableItem(vector<string> aName, vector<string> aValue, FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM* pItem);
	int BuildTableItem(vector<string> aName, vector<string> aValue, FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM* pItem);

	int GetTableItem_Timezone(unsigned int uiIndex, char* sItem);
	int GetTableItem_Holiday(unsigned int uiIndex, char* sItem);
	int GetTableItem_Access(unsigned int uiIndex, char* sItem);
	int GetTableItem_Event(char* sItem);

	int WriteTables_CrcList();
	int WriteTables_Metadata();
	int WriteTables_Timezone();
	int WriteTables_Holiday();
	int WriteTables_Access();

	int ReadTable_CrcList();
	int ReadTable_Metadata();
	int ReadTable_Timezone();
	int ReadTable_Holiday();
	int ReadTable_Access();
	int ReadTable_Event();


	// callback function for notifications from a my axxess reader, invoked by static callback member
	int  cbNewMaxEvent(	int iReaderHnd, 
						int iTaskID, 
						int iError, 
						unsigned char* ucRspData, 
						int iRspLen,
						char* cIPAdr,
						int iPortNr,
						unsigned char& ucAction );

	// callback function for keep alive message with protocol [0x6E] from a reader, invoked by static callback member
	void cbNewReaderDiagnostic(	int iReaderHnd, 
								int iTaskID, 
								int iError, 
								unsigned char* ucRspData, 
								int iRspLen,
								char* cIPAdr,
								int iPortNr );

// public member
public:

// protected member
protected:
	// is set by main constructor to control the delete of m_pReaderModule
	bool	m_bInternalReaderModule;

	FEDM_ISCReaderModule*							m_pReaderModule;
	FEDM_ISC_MYAXXESS_CRCLIST						m_CrcList;
	FEDM_ISC_MYAXXESS_METADATA						m_Metadata;
	vector<FEDM_ISC_MYAXXESS_TIMEZONE_TABLE_ITEM*>	m_TimezoneTable;
	vector<FEDM_ISC_MYAXXESS_HOLIDAY_TABLE_ITEM*>	m_HolidayTable;
	vector<FEDM_ISC_MYAXXESS_ACCESS_TABLE_ITEM*>	m_AccessTable;
	queue<FEDM_ISC_MYAXXESS_EVENT_TABLE_ITEM*>		m_EventTable;

	// parser modules for XML and CSV file types
#if defined(_FEDM_MYAXXESS_XML_SUPPORT)
	FedmIscMyAxxess_XmlParser*	m_pXmlParser;
#endif
	FedmIscMyAxxess_CsvParser*	m_pCsvParser;

	unsigned char	m_ucDateFormat;			// date format
	unsigned int	m_uiTimeout;			// communication timeout
	unsigned int	m_uiPackageLen;			// length of a package to transfer
	unsigned int	m_uiEventCallerType;	// 0 for C++
											// 1 for .NET

	// buffer for I/O exchange with Reader
	unsigned char m_ucOut[FEDM_MYAXXESS_MAX_BLOCKSIZE + 20];
	unsigned char m_ucIn[FEDM_MYAXXESS_MAX_BLOCKSIZE + 20];

	LPFN_FEDM_MYAXXESS_EVENT_CB				m_cbEvent;
	LPFN_FEDM_MYAXXESS_KEEPALIVE_CB			m_cbKeepAlive;
	LPFN_FEDM_MYAXXESS_EVENT_DOTNET_CB		m_cbStdCallEvent;
	LPFN_FEDM_MYAXXESS_KEEPALIVE_DOTNET_CB	m_cbStdCallKeepAlive;
};

#endif // !defined(_FEDM_ISC_MYAXXESS_READER_H_INCLUDED_)
