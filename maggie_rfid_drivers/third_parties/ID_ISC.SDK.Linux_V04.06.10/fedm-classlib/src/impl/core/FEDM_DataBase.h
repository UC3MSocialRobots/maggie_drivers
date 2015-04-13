/*-------------------------------------------------------
|                                                       |
|                  FEDM_DataBase.h                      |
|                                                       |
---------------------------------------------------------

Copyright © 2000-2012	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	13.05.2000

Version       		:	04.00.10 / 06.02.2012 / M. Hultsch
						- support for external logmanager (FELOGMNG.dll), when #define _FELOGMNG_SUPPORT is set

						03.03.03 / 09.05.2011 / M. Hultsch
						- parameter m_ucReaderType moved from unsigned char to unsigned int

						03.01.06 / 10.05.2010 / M. Hultsch

Operation Systems	:	independent

Function			:	abstract class with data memories


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/



#if !defined(_FEDM_DATABASE_H_INCLUDED_)
#define _FEDM_DATABASE_H_INCLUDED_



#include "FEDM_Base.h"
#include "FEDM_Xml.h"
#include "FEDM_Logger.h"

class FEDM_XMLBase;
class FEDM_XMLReaderCfgDataModul;
class FEDM_XMLReaderCfgProfileModul;
#ifndef _FEDM_XML_SUPPORT
	struct FEDM_XML_TREEITEM;
#endif





// type definition for callback function for method FindBaudRate(..)
// void* pAny				- pointer to anything (e.g. 'this' pointer), which is transferred as last parameter in FindBaudRate
// int iPortNr				- serial port number
// unsigned char ucBusAdr	- bus address
// char* cBaud				- actual selected baudrate ("230400", "115200", "57600", "38400", "19200", "9600", "4800")
// char* cFrame				- actual selected frame ("8E1", "8N1", "8O1")
// int iPrtFrame			- actual selected OBID protocol frame (FEDM_PRT_FRAME_STANDARD, FEDM_PRT_FRAME_ADVANCED)
typedef void(*FEDM_CB_FIND_BAUDRATE)(void*, int, unsigned char, char*, char*, int);




class _FEDM_ISC_CORE_EXT_CLASS FEDM_DataBase : public FEDM_Base
{
// #################
// ### functions ###
// #################
public:
	FEDM_DataBase();
	virtual ~FEDM_DataBase();


	// check version numbers of dependent libraries
	virtual int EvalLibDependencies(string& sMessage)=0;



	// sub-function for XML file-I/O
	virtual int Serialize(FEDM_XMLReaderCfgDataModul* xml, 
						  bool bRead, 
						  char* sFileName);

	// sub-function for XML based reader customization
	virtual int SerializeProfile(FEDM_XMLReaderCfgProfileModul* xml, 
								  bool bRead, 
								  char* sFileName);


	// sub-function for XML based reader customization
	virtual int SerializeProfileIn( FEDM_XMLBase* xml,
									FEDM_XML_TREEITEM* root);

	virtual int SerializeProfileOut( FEDM_XMLBase* xml,
									 FEDM_XML_TREEITEM* root);

	// set the reader handle
	// must be done before use of all other functions
	void SetReaderHnd(int iReaderHnd);
	int  GetReaderHnd();

	// set the busaddress for asynchrone communication
	// default: 0xFF
	void  SetBusAddress(unsigned char ucBusAdr);
	unsigned char GetBusAddress();

	// query functions
	char* GetFamilyCode();
	char* GetReaderName();
	unsigned int GetReaderType();

	// functions for management support
	void SetModuleHnd(int iHnd);
	int  GetModuleHnd();

	// log support
	FEDM_Logger* GetLogger()	{ return m_pLogger; }

	// main access functions
	virtual int GetData( const char* ID, bool* Data );
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	virtual int GetData( const char* ID, BOOL* Data );
#endif
	virtual int GetData( const char* ID, unsigned char* Data );
	virtual int GetData( const char* ID, unsigned char* Data, int Cnt );
	virtual int GetData( const char* ID, unsigned int* Data );
	virtual int GetData( const char* ID, __int64* Data);
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	virtual int GetData( const char* ID, CString& Data );
#endif
	virtual int GetData( const char* ID, string& Data );
	virtual int GetData( const char* ID, char* Data, int DataLen );
	
	virtual int SetData( const char* ID, bool Data );
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	virtual int SetData( const char* ID, BOOL Data );
#endif
	virtual int SetData( const char* ID, unsigned char Data );
	virtual int SetData( const char* ID, unsigned char* Data, int Cnt );
	virtual int SetData( const char* ID, unsigned int Data );
	virtual int SetData( const char* ID, __int64 Data);
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	virtual int SetData( const char* ID, CString Data );
#endif
	virtual int SetData( const char* ID, string Data );
	virtual int SetData( const char* ID, char* Data, int DataLen );

	// for direct access with address (MemID is defined in FEDM.h)
	virtual int GetData( int Adr, unsigned char* Data, int MemID );
	virtual int GetData( int Adr, unsigned char* Data, int Cnt, int MemID );
	virtual int GetData( int Adr, unsigned int* Data, int MemID );
	virtual int GetData( int Adr, __int64* Data, int MemID );
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	virtual int GetData( int Adr, CString& Data, int Cnt, int MemID );
#endif

	virtual int SetData( int Adr, unsigned char Data, int MemID );
	virtual int SetData( int Adr, unsigned char* Data, int Cnt, int MemID );
	virtual int SetData( int Adr, unsigned int Data, int MemID );
	virtual int SetData( int Adr, __int64 Data, int MemID );
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	virtual int SetData( int Adr, CString Data, int MemID );
#endif


	int GetAccessID(string sParaName, string& sAccessID, bool bEEPROM);


// ##########################
// ### abstract functions ###
// ##########################
public:
	// abstract communication functions
	virtual int SendProtocol(unsigned char ucStByte)=0;

	// function to find the baudrate and frame of reader
	virtual int FindBaudRate(bool* pStop = NULL, int iTimeout = 300, int iFrameSupport = 0, FEDM_CB_FIND_BAUDRATE cbFct=NULL, void* pAny=NULL)=0;

	virtual int GetLastProt(char* sID, char* sProt)=0;

	// abstract functions for communication channel
	virtual int SetPortHnd(int iPortHnd)=0;
	virtual int GetPortHnd()=0;

	// change the protocol type (STANDARD or ADVANCED)
	// STANDARD is preset
	virtual int SetProtocolFrameSupport(unsigned int uiType)=0;
	virtual int GetProtocolFrameSupport()=0;

	virtual int SetReaderType(unsigned int uiReaderType)=0;

	// abstract main function for XML serialization
	virtual int Serialize(bool bRead, char* sFileName)=0;

	// abstract sub-functions for XML serialization
	virtual int SerializeIn(FEDM_XMLBase* xml, FEDM_XML_TREEITEM* root)=0;
	virtual int SerializeOut(FEDM_XMLBase* xml, FEDM_XML_TREEITEM* root)=0;

#ifdef _FEDM_MFC_SUPPORT
	// abstract function for proprietary file-I/O
	virtual int Serialize(CArchive& ar, int iMemType)=0;
#endif

	// abstract miscellaneous functions
	virtual int		GetLastError()=0;
	virtual int		GetLastStatus()=0;
	virtual int		GetErrorText(char* cErrorText, int iErrorCode)=0;
	virtual char*	GetErrorText(int iErrorCode)=0;
	virtual int		GetStatusText(char* cStatusText, unsigned char ucStatus)=0;
	virtual char*	GetStatusText(unsigned char ucStatus)=0;

	// abstract functions for access of one attribute from table
	virtual int GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, bool* bData)=0;
	virtual int GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, unsigned char* ucData)=0;
	virtual int GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, unsigned char* ucData, int iDataBufLen)=0;
	virtual int GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, unsigned int* uiData)=0;
	virtual int GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, __int64* i64Data)=0;
	virtual int GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, char* Data, int iDataBufLen)=0;
	virtual int GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, string& sData)=0;
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	virtual int GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, CString& sData)=0;
#endif

	virtual int SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, bool bData)=0;
	virtual int SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, unsigned char ucData)=0;
	virtual int SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, unsigned int uiData)=0;
	virtual int SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, __int64 i64Data)=0;
	virtual int SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, string sData)=0;
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	virtual int SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, CString sData)=0;
#endif
	
	// abstract functions for access of one datablock from table
	virtual int GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID,
							 int iBlockNr, unsigned char* ucData, int iDataBufLen)=0;
	virtual int GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID,
							 int iBlockNr, string& sData)=0;
	
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	virtual int GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID,
							 int iBlockNr, CString& sData)=0;
#endif

	virtual int SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID,
							 int iBlockNr, unsigned char* ucData, int iDataBufLen)=0;
	virtual int SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID,
							 int iBlockNr, string sData)=0;

#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	virtual int SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID,
							 int iBlockNr, CString sData)=0;
#endif
	
	// abstract function for verify read/write-data block(s) in table
	virtual int VerifyTableData(int iIdx, unsigned int uiTableID, int iBlockNr, int iBlockCnt)=0;
	
	// abstract functions for search of index of one table data
	virtual int FindTableIndex(int iStartIdx, unsigned int uiTableID, unsigned int uiDataID, bool bData)=0;
	virtual int FindTableIndex(int iStartIdx, unsigned int uiTableID, unsigned int uiDataID, unsigned char ucData)=0;
	virtual int FindTableIndex(int iStartIdx, unsigned int uiTableID, unsigned int uiDataID, unsigned int uiData)=0;
	virtual int FindTableIndex(int iStartIdx, unsigned int uiTableID, unsigned int uiDataID, __int64 i64Data)=0;
	virtual int FindTableIndex(int iStartIdx, unsigned int uiTableID, unsigned int uiDataID, string sData)=0;
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	virtual int FindTableIndex(int iStartIdx, unsigned int uiTableID, unsigned int uiDataID, CString sData)=0;
#endif

	// abstract function for setting the size of a table
	virtual int SetTableSize(unsigned int uiTableID, int iSize)=0;
	// abstract function for query the size of a table
	virtual int GetTableSize(unsigned int uiTableID)=0;
	// abstract function for query the length of valid data in a table
	virtual int GetTableLength(unsigned int uiTableID)=0;
	// abstract function to reset the table (the content of the table)
	virtual int ResetTable(unsigned int uiTableID, unsigned int uiDataFlags)=0;


// ###############################
// ### serialization functions ###
// ###############################
protected:
	// sub-function for XML serialization
	virtual int SerializeIn(int iMemType,
							FEDM_XMLBase* xml,
							FEDM_XML_TREEITEM* root);

	virtual int SerializeOut(int iMemType,
							 FEDM_XMLBase* xml,
							 FEDM_XML_TREEITEM* root);

	void OptionalSetDataAction( const char* ID, __int64 Data );
	void OptionalSetDataAction( const char* ID, string Data );
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	void OptionalSetDataAction( const char* ID, CString Data );
#endif



// ############################
// ### public datacontainer ###
// ############################
public:
	// memories (block-organized; data in Motorolas "Big Endian" format)
	// ### for reader
	FEDM_BYTE_ARRAY m_RFC_EEData;	// EEPROM parameter for RF-Controller
	FEDM_BYTE_ARRAY m_RFC_RAMData;	// RAM parameter for RF-Controller

	FEDM_BYTE_ARRAY m_ACC_EEData;	// EEPROM parameter for AC-Controller
	FEDM_BYTE_ARRAY m_ACC_RAMData;	// RAM parameter for AC-Controller

	// ### for transponder
	FEDM_BYTE_ARRAY m_SN_Mem;		// serial number
	FEDM_BYTE_ARRAY m_ID_Mem;		// ID number
	FEDM_BYTE_ARRAY m_AC_Mem;		// account
	FEDM_BYTE_ARRAY m_PubMem;		// public memory
	FEDM_BYTE_ARRAY m_SecMem;		// secret memory
	FEDM_BYTE_ARRAY m_ConfMem;		// configuration memory
	FEDM_BYTE_ARRAY m_DateMem;		// date memory

	// ### for temporary Data
	FEDM_BYTE_ARRAY m_TmpData;
	FEDM_BYTE_ARRAY m_MjpData;


	// memories (block-organized; data in Motorolas "Big Endian" format)
	// ### for Profiling masks
	FEDM_BYTE_ARRAY m_RFC_CfgProfileMask;	// profiling mask for EEPROM parameter in RF-Controller
	FEDM_BYTE_ARRAY m_ACC_CfgProfileMask;	// profiling mask for EEPROM parameter in RF-Controller

// ############################
// ### protected attributes ###
// ############################
protected:
	int					m_iReaderHnd;		// for FExxx.DLL functions
	unsigned char		m_ucBusAdr;			// bus address for reader

	char				m_sFamCode[32];		// reader family code

//	unsigned char		m_ucReaderType;		// reader type number
	char				m_sReaderName[32];	// name of reader type
	unsigned int		m_uiReaderType;		// extended reader type number
											// bit	0...7	ReaderType/ReaderClass (SW-TYPE) according system manual from [0x65] or [0x66] with Mode 0
											//				if bit7 == 0 >> value is ReaderType
											//				if bit7 == 1 >> value is ReaderClass
											//		8..15	ReaderType, if bit7 == 1; otherwise 0
											//	   16..23	reserved
											//	   24..31	reserved
											//

	// map for namespace to access-id map
	FEDM_MAP_ACCESS_ID	m_mapAccessID;

	// the following parameters are for internal use and
	// depend on reader families
	// they are set in specialized reader class
	int m_iUsedEEDataBlocks;
	int m_iUsedRAMDataBlocks;
	int m_iUsedAccEEDataBlocks;
	int m_iUsedAccRAMDataBlocks;
	int m_iUsedTmpDataBlocks;
	int m_iUsedMjpDataBlocks;
	int m_iUsedSNMemBlocks;
	int m_iUsedIDMemBlocks;
	int m_iUsedACMemBlocks;
	int m_iUsedPubMemBlocks;
	int m_iUsedSecMemBlocks;
	int m_iUsedConfMemBlocks;
	int m_iUsedDateMemBlocks;

	int m_iRFC_DataBlockSize;
	int m_iACC_DataBlockSize;
	int m_iPubMemBlockSize;

	int m_iModuleHnd;	// handle for manager class FEDM_ReaderManager

	// for logmanager support
	FEDM_Logger* m_pLogger;
};

#endif // !defined(_FEDM_DATABASE_H_INCLUDED_)
