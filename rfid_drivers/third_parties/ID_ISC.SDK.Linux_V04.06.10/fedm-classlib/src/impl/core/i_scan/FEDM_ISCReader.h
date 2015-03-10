/*-------------------------------------------------------
|                                                       |
|                  FEDM_ISCReader.h                     |
|                                                       |
---------------------------------------------------------

Copyright © 2000-2014	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	07.06.2000

Version       		:	04.06.01 / 03.03.2014 / M. Hultsch
						- new overload method Set_0x6E_ReaderDiagnosticData

						04.05.05 / 04.11.2013 / M. Hultsch
						- new reader: ID ISC.PRH200

						04.05.04 / 03.09.2013 / M. Hultsch
						- new reader: ID ISC.LRU1002

						04.04.03 / 05.03.2013 / M. Hultsch
						- new method: GetDependentLibVersions

						04.03.01 / 10.09.2012 / M. Hultsch
						- new methods: SetCommandPara/GetCommandPara

						04.02.05 / 05.07.2012 / M. Hultsch
						- new reader: ID CPR.20

						04.02.04 / 21.06.2012 / M. Hultsch
						- UHF Class1 Gen2: support for [0xB3][0x25] Block Permalock and [0xB3][0x26] Read Permalock Status

						04.00.09 / 23.01.2012 / M. Hultsch
						- new reader: ID ISC.LR1002

						03.03.01 / 08.04.2011 / M. Hultsch
						- new reader ID ISC.MRU102 added
						- new TagHandler classes for ISO 15693 IDS SL13A
						  and EPC Class1 Gen2 IDS SL900A

						03.02.07 / 25.10.2010 / M. Hultsch
						- non-addressed with taghandler

						03.02.05 / 08.09.2010 / M. Hultsch
						- support for [0xC3] DESFire Command

						03.02.04 / 17.08.2010 / M. Hultsch
						- ID CPR30.xx added

						03.01.06 / 10.05.2010 / M. Hultsch
						- FindBaudRate with new parameters

						03.01.02 / 18.03.2010 / M. Hultsch
						- handling of new USB enumeration after [0x64] System Reset

						03.01.01 / 25.02.2010 / M. Hultsch
						- support for peripheral devices

						03.00.13 / 07.10.2009 / M. Hultsch
						- ID MAX50.xx added
						- [0xBx] Host Commands in non-addressed mode from table index > 0

						03.00.12 / 24.08.2009 / M. Hultsch

Operation Systems	:	independent

Function			:	classes for OBID i-scan® and OBID® classic-pro reader family


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISCREADER_H_INCLUDED_)
#define _FEDM_ISCREADER_H_INCLUDED_


#include "../FEDM_DataBase.h"
#include "FEDM_ISC.h"
#include "FEDM_ISCReaderInfo.h"
#include "FEDM_ISCReaderDiagnostic.h"


class FEDM_BRMTabItem;				// class for buffered read mode
class FEDM_ISOTabItem;				// class for iso host mode
class FEDM_CADItem;					// class for channel analyze measurement data
class FEDM_ISCReader;				// main reader class
class FEDM_XMLReaderCfgDataModul;	// class for xml serialization
class FedmIscMyAxxessReader;
#if !defined(_FEDM_NO_PD_SUPPORT)
	class FedmIscPeripheralDevice;
#endif
#ifdef _FEDM_TAG_HANDLER
	class FedmIscTagHandler_ISO14443_4;
#endif


//#####################################################################################
// structure for measurement data
// (only for use inside FEDM_CADTable)
//#####################################################################################


#if !defined(__BORLANDC__)
typedef struct _FEDM_ISC_STRUCT_CAD_HISTORY
{
	unsigned int	uiAlloc;
	unsigned char	ucTxChannel;
	unsigned char	ucRxChannel;

	// initialization
	_FEDM_ISC_STRUCT_CAD_HISTORY() :	uiAlloc(0),
										ucTxChannel(0),
										ucRxChannel(0)
										{}

} FEDM_ISC_STRUCT_CAD_HISTORY;
#endif


//####################################################################
// type declarations
//####################################################################

typedef vector<FEDM_BRMTabItem*>						FEDM_BRMTAB_ARRAY;
typedef vector<FEDM_BRMTabItem*>::iterator				FEDM_BRMTAB_ARRAY_ITOR;
typedef vector<FEDM_ISOTabItem*>						FEDM_ISOTAB_ARRAY;
typedef vector<FEDM_ISOTabItem*>::iterator				FEDM_ISOTAB_ARRAY_ITOR;
#if !defined(__BORLANDC__)
typedef vector<FEDM_ISC_STRUCT_CAD_HISTORY*>			FEDM_CAD_HISTORY_ARRAY;
typedef vector<FEDM_ISC_STRUCT_CAD_HISTORY*>::iterator	FEDM_CAD_HISTORY_ARRAY_ITOR;
#endif

#if !defined(_FEDM_NO_PD_SUPPORT)
typedef map<unsigned char, FedmIscPeripheralDevice*>				FEDM_PD_MAP;
typedef map<unsigned char, FedmIscPeripheralDevice*>::iterator		FEDM_PD_MAP_ITOR;
#endif


//#####################################################################################
// class FEDM_CADTable (only for use inside FEDM_ISCReader-class)
// collects measurement data transfered with [0x68] Channel Analyze
//#####################################################################################

#if !defined(__BORLANDC__)
class _FEDM_ISC_CORE_EXT_CLASS FEDM_CADTable
{
public:
	FEDM_CADTable();
	~FEDM_CADTable();

	void Init();				// set all data members to 0
								// and clear allocated memory

	bool m_bValid;				// indicator, that table contains valid data
	unsigned char m_ucMode;				// mode byte of [0x68] Channel Analyze
								// CH_POWER flag or CH_HISTORY flag must be set

	// memory for power data
	unsigned char m_ucPowerAvg[15];		// average power value for each channel
	unsigned char m_ucPowerPeak[15];	// peak power value for each channel

	// memory for history data
	FEDM_CAD_HISTORY_ARRAY m_History;

	// memory for different data
	long m_nData[4];
};
#endif



//####################################################################
// class FEDM_ISCReader
//####################################################################

class _FEDM_ISC_CORE_EXT_CLASS FEDM_ISCReader : public FEDM_DataBase
{
// public methods
public:

	// friend classes have access rights to protected members
	friend class FEDM_ISOTabItem;
	friend class FedmIscMyAxxessReader;
#ifdef _FEDM_TAG_HANDLER
	friend class FedmIscTagHandler_ISO14443_4;
#endif
#if defined(_FEDM_ISC_NEW_CORE_DLL)
	//friend class OBID::FEDM::Core::IAsyncGroup;
#endif

	FEDM_ISCReader();			// constructor
	virtual ~FEDM_ISCReader();	// destructor

	// check version numbers of dependent libraries
	virtual int EvalLibDependencies(string& sMessage);
	string GetDependentLibVersions();

	// main function for XML serialization
	virtual int Serialize(bool bRead, char* sFileName);

#ifdef _FEDM_MFC_SUPPORT
	// for proprietary file-I/O
	virtual int Serialize(CArchive& ar, int iMemType);
#endif

	// main communication method
	virtual int SendProtocol(unsigned char ucCmdByte);

	FEDM_ISC_READER_INFO*		GetReaderInfo()				{ return &m_ReaderInfo; }
	FEDM_ISC_READER_DIAGNOSTIC* GetReaderDiagnostic()		{ return &m_ReaderDiagnostic; }

	// set the port handle in reader object of FEISC
	// must be done before use of all other methods
	virtual int SetPortHnd(int iPortHnd);
	virtual int GetPortHnd();

	virtual int SetReaderType(unsigned int uiReaderType);
	char* GetReaderName();
	char* GetReaderName(unsigned int uiReaderType);
	void SetOemReaderNames(	map<unsigned int, string> mapOemNames,
							map<unsigned int, string> mapOemNames4ReaderVariants );

	void DisableReadCfgBeforeWriteCfg(bool bFlag)		{ m_bDisableReadCfgBeforeWriteCfg = bFlag; }
	void EnableTagHandler(bool bFlag)					{ m_bEnableTagHandler = bFlag; }
#ifdef _FEDM_TAG_HANDLER
	void SetTagHandlerOption(string sParameter, string sValue);
#endif

	char* GetTagName(unsigned char ucTagType);

	// change the protocol type (STANDARD or ADVANCED)
	// STANDARD is preset
	virtual int SetProtocolFrameSupport(unsigned int uiType);
	virtual int GetProtocolFrameSupport();

	// get the last protocol string (frame and data)
	virtual int GetLastProt(char* sID, char* sProt);
	// get the last error code
	virtual int GetLastError();
	// get the last status byte
	virtual int GetLastStatus();
	
	// get error text
	int		GetErrorText(char* cErrorText, int iErrorCode);
	char*	GetErrorText(int iErrorCode);
	// get status text
	int		GetStatusText(char* cStatusText, unsigned char ucStatus);
	char*	GetStatusText(unsigned char ucStatus);

	// method to find the baudrate and frame of reader
	virtual int FindBaudRate(bool* pStop = NULL, int iTimeout = 300, int iFrameSupport = 3, FEDM_CB_FIND_BAUDRATE cbFct=NULL, void* pAny=NULL);

	// main access methods
	int TestConfigPara( string sParaName );
	
	// methods for getting command parameter values
	int GetCommandPara( string sParaName, bool* Data );
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	int GetCommandPara( string sParaName, BOOL* Data );
#endif
	int GetCommandPara( string sParaName, unsigned char* Data );
	int GetCommandPara( string sParaName, unsigned char* Data, int DataLen );
	int GetCommandPara( string sParaName, unsigned int* Data );
	int GetCommandPara( string sParaName, __int64* Data );
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	int GetCommandPara( string sParaName, CString Data );
#endif
	int GetCommandPara( string sParaName, string Data );
	int GetCommandPara( string sParaName, char* Data, int DataLen );
	
	// methods for setting command parameter values
	int SetCommandPara( string sParaName, bool Data );
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	int SetCommandPara( string sParaName, BOOL Data );
#endif
	int SetCommandPara( string sParaName, unsigned char Data );
	int SetCommandPara( string sParaName, unsigned char* Data, int DataLen );
	int SetCommandPara( string sParaName, unsigned int Data );
	int SetCommandPara( string sParaName, __int64 Data );
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	int SetCommandPara( string sParaName, CString Data );
#endif
	int SetCommandPara( string sParaName, string Data );
	int SetCommandPara( string sParaName, char* Data, int DataLen );

	// methods for getting configuration parameter values
	int GetConfigPara( string sParaName, bool* Data, bool bEEPROM=true );
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	int GetConfigPara( string sParaName, BOOL* Data, bool bEEPROM=true );
#endif
	int GetConfigPara( string sParaName, unsigned char* Data, bool bEEPROM=true );
	int GetConfigPara( string sParaName, unsigned char* Data, int ParaAnz, bool bEEPROM=true );
	int GetConfigPara( string sParaName, unsigned int* Data, bool bEEPROM=true );
	int GetConfigPara( string sParaName, __int64* Data, bool bEEPROM=true );
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	int GetConfigPara( string sParaName, CString& Data, bool bEEPROM=true );
#endif
	int GetConfigPara( string sParaName, string& Data, bool bEEPROM=true );
	int GetConfigPara( string sParaName, char* Data, int DataLen, bool bEEPROM=true );
	
	// methods for setting configuration parameter values
	int SetConfigPara( string sParaName, bool Data, bool bEEPROM=true );
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	int SetConfigPara( string sParaName, BOOL Data, bool bEEPROM=true );
#endif
	int SetConfigPara( string sParaName, unsigned char Data, bool bEEPROM=true );
	int SetConfigPara( string sParaName, unsigned char* Data, int ParaAnz, bool bEEPROM=true );
	int SetConfigPara( string sParaName, unsigned int Data, bool bEEPROM=true );
	int SetConfigPara( string sParaName, __int64 Data, bool bEEPROM=true );
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	int SetConfigPara( string sParaName, CString Data, bool bEEPROM=true );
#endif
	int SetConfigPara( string sParaName, string Data, bool bEEPROM=true );
	int SetConfigPara( string sParaName, char* Data, int DataLen, bool bEEPROM=true );

	// get specific data from table
	virtual int GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, bool* bData);
	virtual int GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, unsigned char* ucData);
	virtual int GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, unsigned char* ucData, int iDataBufLen);
	virtual int GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, unsigned int* uiData);
	virtual int GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, __int64* i64Data);
	virtual int GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, char* cData, int iDataBufLen);
	virtual int GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, string& sData);
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	virtual int GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, CString& sData);
#endif

	// set specific data in table
	virtual int SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, bool bData);
	virtual int SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, unsigned char ucData);
	virtual int SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, unsigned char* ucData, int iDataBufLen);
	virtual int SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, unsigned int uiData);
	virtual int SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, __int64 i64Data);
	virtual int SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, string sData);
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	virtual int SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, CString sData);
#endif
		
	// get data block from table
	virtual int GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, 
							 int iBlockNr, unsigned char* ucData, int iDataBufLen);
	virtual int GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, 
							 int iBlockNr, string& sData);
	
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	virtual int GetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, 
							 int iBlockNr, CString& sData);
#endif
	
	// set data block in table
	virtual int SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, 
							 int iBlockNr, unsigned char* ucData, int iDataBufLen);
	int SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, 
					 int iBlockNr, string sData);
	
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	virtual int SetTableData(int iIdx, unsigned int uiTableID, unsigned int uiDataID, 
							 int iBlockNr, CString sData);
#endif
	
	// verify read/write-data block(s) in table
	virtual int VerifyTableData(int iIdx, unsigned int uiTableID, int iBlockNr, int iBlockCnt);
	
	// copy complete byte arrays from source to destination inside a table
	// source/destination can be: FEDM_ISC_DATA_TxDB, FEDM_ISC_DATA_RxDB
	int CopyTableData(int iIdx, unsigned int uiTableID, int iSrcDataID, int iDstDataID);
	
	// get table-index of specific data from table
	virtual int FindTableIndex(int iStartIdx, unsigned int uiTableID, unsigned int uiDataID, bool bData);
	virtual int FindTableIndex(int iStartIdx, unsigned int uiTableID, unsigned int uiDataID, unsigned char ucData);
	virtual int FindTableIndex(int iStartIdx, unsigned int uiTableID, unsigned int uiDataID, unsigned int uiData);
	virtual int FindTableIndex(int iStartIdx, unsigned int uiTableID, unsigned int uiDataID, __int64 i64Data);
	virtual int FindTableIndex(int iStartIdx, unsigned int uiTableID, unsigned int uiDataID, string sData);
#if defined(_FEDM_MFC_SUPPORT) //|| defined(__BORLANDC__)
	virtual int FindTableIndex(int iStartIdx, unsigned int uiTableID, unsigned int uiDataID, CString sData);
#endif
	
	// set size of a table
	virtual int SetTableSize(unsigned int uiTableID, int iSize);
	virtual int SetTableSize(unsigned int uiTableID, 
							 int iSize, 
							 int iRxDB_BlockCount, 
							 int iRxDB_BlockSize, 
							 int iTxDB_BlockCount,
							 int iTxDB_BlockSize);
	
	// get size of a table
	virtual int GetTableSize(unsigned int uiTableID);
	// get length of valid data in a table
	virtual int GetTableLength(unsigned int uiTableID);
	// set length of valid data in a table if the application has built the table
	int SetTableLength(unsigned int uiTableID, unsigned int uiLength);
	// reset length of valid data in a table
	virtual int ResetTable(unsigned int uiTableID, unsigned int uiDataFlags=0);

	// for non-addresses mode: index in m_ISOTable
//	void		 SetNonAddressedTabIndex(unsigned int uiTabIndex)	{ m_uiNonAddressedTabIndex = uiTabIndex; }
//	unsigned int GetNonAddressedTabIndex()							{ return m_uiNonAddressedTabIndex; }

	FEDM_ISOTabItem* GetISOTableItem(unsigned int uiIdx);
	FEDM_BRMTabItem* GetBRMTableItem(unsigned int uiIdx);

#if !defined(_FEDM_NO_PD_SUPPORT)
	FEDM_PD_MAP* GetPeripheralDevices()								{ return &m_mapPeripheralDevices; }
#endif

	// only for internal use
	int Set_0x6E_ReaderDiagnosticData(unsigned char ucMode, unsigned char* ucData, FEDM_ISC_READER_DIAGNOSTIC* pReaderDiagnostic);

// protected methods
protected:
	// save the last error or status byte in memory
	void SetLastError(int iError);

	// methods auto-generated by ISOStartDev
	void BuildAccessIdMap_CPR();
	void BuildAccessIdMap_CPR20();
	void BuildAccessIdMap_CPR30();
	void BuildAccessIdMap_CPR40();
	void BuildAccessIdMap_CPR44();
	void BuildAccessIdMap_CPR46();
	void BuildAccessIdMap_CPR47();
	void BuildAccessIdMap_CPR50();
	void BuildAccessIdMap_CPR52();
	void BuildAccessIdMap_CPR60();
	void BuildAccessIdMap_MAX50();
	void BuildAccessIdMap_MAX50_Part0();
	void BuildAccessIdMap_MAX50_Part1();
	void BuildAccessIdMap_MAX_ONTOP();
	void BuildAccessIdMap_M02();
	void BuildAccessIdMap_MR101();
	void BuildAccessIdMap_MR102();
	void BuildAccessIdMap_PRH102();
	void BuildAccessIdMap_PRH200();
	void BuildAccessIdMap_MR200();
	void BuildAccessIdMap_MR200_Part0();
	void BuildAccessIdMap_MR200_Part1();
	void BuildAccessIdMap_LR200();
	void BuildAccessIdMap_LR1002();
	void BuildAccessIdMap_LR1002_Part0();
	void BuildAccessIdMap_LR1002_Part1();
	void BuildAccessIdMap_LR2000();
	void BuildAccessIdMap_LR2000_Part0();
	void BuildAccessIdMap_LR2000_Part1();
	void BuildAccessIdMap_LR2500_A();
	void BuildAccessIdMap_LR2500_A_Part0();
	void BuildAccessIdMap_LR2500_A_Part1();
	void BuildAccessIdMap_LR2500_A_Part2();
	void BuildAccessIdMap_LR2500_B();
	void BuildAccessIdMap_LR2500_B_Part0();
	void BuildAccessIdMap_LR2500_B_Part1();
	void BuildAccessIdMap_PRHD102();
	void BuildAccessIdMap_ISCMU02();
	void BuildAccessIdMap_MRU102();
	void BuildAccessIdMap_MRU102_Part0();
	void BuildAccessIdMap_MRU102_Part1();
	void BuildAccessIdMap_MRU200();
	void BuildAccessIdMap_MRU200_Part0();
	void BuildAccessIdMap_MRU200_Part1();
	void BuildAccessIdMap_LRU1000();
	void BuildAccessIdMap_LRU1000_Part0();
	void BuildAccessIdMap_LRU1000_Part1();
	void BuildAccessIdMap_MAXU1002();
	void BuildAccessIdMap_LRU1002();
	void BuildAccessIdMap_LRU1002_Part0();
	void BuildAccessIdMap_LRU1002_Part1();
	void BuildAccessIdMap_LRU2000();
	void BuildAccessIdMap_LRU2000_Part0();
	void BuildAccessIdMap_LRU2000_Part1();
	void BuildAccessIdMap_LRU3000();
	void BuildAccessIdMap_LRU3000_Part0();
	void BuildAccessIdMap_LRU3000_Part1();
#if defined(_FEDM_SUPPORT_READER_CLASS)
	void BuildAccessIdMap_Class_ISC_HF_MR1();
#endif

#ifdef _FEDM_USB_SUPPORT
	void HandleUsbEnumeration();
#endif
	
	int Set_0x66_ReaderInfoData(unsigned char ucMode, unsigned char* ucData);
	int Set_0x6E_ReaderDiagnosticData(unsigned char ucMode, unsigned char* ucData);
	int Get_0x72_OutputData(unsigned char ucOutN, unsigned char* pOutRec);

	// get data for [0xB0], [0xB2], [0xB3] iso command
	virtual int Get_0xB0_IsoTableData(unsigned char ucCmdByte, unsigned char* ucData);
	// get data for [0xB0][0x??] iso command
	virtual int Get_0xB0_0x24_IsoTableData(unsigned char* ucData);
	virtual int Get_0xB0_0x27_IsoTableData(unsigned char* ucData);
	virtual int Get_0xB0_0x29_IsoTableData(unsigned char* ucData);
	virtual int Get_0xB0_0xA1_IsoTableData(unsigned char* ucData);

	// save data in ISO-table after [0xB0] [0x??] iso command
	virtual int Set_0xB0_IsoTableData(unsigned char* ucData, int iLen, int iStatusByte);
	virtual int Set_0xB0_0x23_IsoTableData(unsigned char* ucData, int iLen);
	virtual int Set_0xB0_0x25_IsoTableData(unsigned char* ucData, int iLen);
	virtual int Set_0xB0_0x26_IsoTableData(unsigned char* ucData, int iLen);
	virtual int Set_0xB0_0x2B_IsoTableData(unsigned char* ucData, int iLen, int iStatusByte);
	virtual int Set_0xB0_0x2C_IsoTableData(unsigned char* ucData, int iLen);
	virtual int Set_0xB0_0xA0_IsoTableData(unsigned char* ucData, int iLen);

	// get data for [0xB1] iso command
	virtual int Get_0xB1_IsoTableData(unsigned char ucCmdByte, unsigned char* ucData);
	// get data for [0xB1][0x??] iso command
	int Get_0xB1_IsoTableData_STM(unsigned char ucCmdByte, unsigned char* ucData);
	int Get_0xB1_IsoTableData_NXP(unsigned char ucCmdByte, unsigned char* ucData);
	int Get_0xB1_IsoTableData_Infineon(unsigned char ucCmdByte, unsigned char* ucData);
	int Get_0xB1_IsoTableData_TI(unsigned char ucCmdByte, unsigned char* ucData);
	int Get_0xB1_IsoTableData_Fujitsu(unsigned char ucCmdByte, unsigned char* ucData);
	int Get_0xB1_IsoTableData_EM(unsigned char ucCmdByte, unsigned char* ucData);
	int Get_0xB1_IsoTableData_KSW(unsigned char ucCmdByte, unsigned char* ucData);
	int Get_0xB1_IsoTableData_IDS(unsigned char ucCmdByte, unsigned char* ucData);

	// save data in ISO-table after [0xB1][0x??] iso command
	int Set_0xB1_IsoTableData(unsigned char ucCmdByte, unsigned char* ucData, int iLen);
	int Set_0xB1_IsoTableData_NXP(unsigned char ucCmdByte, unsigned char* ucData, int iLen);
	int Set_0xB1_IsoTableData_STM(unsigned char ucCmdByte, unsigned char* ucData, int iLen);
	int Set_0xB1_IsoTableData_EM(unsigned char ucCmdByte, unsigned char* ucData, int iLen);
	int Set_0xB1_IsoTableData_Fujitsu(unsigned char ucCmdByte, unsigned char* ucData, int iLen);
	int Set_0xB1_IsoTableData_KSW(unsigned char ucCmdByte, unsigned char* ucData, int iLen);
	int Set_0xB1_IsoTableData_Infineon(unsigned char ucCmdByte, unsigned char* ucData, int iLen);
	int Set_0xB1_IsoTableData_TI(unsigned char ucCmdByte, unsigned char* ucData, int iLen);
	int Set_0xB1_IsoTableData_IDS(unsigned char ucCmdByte, unsigned char* ucData, int iLen);

	// get data for [0xB2], [0xB3] iso command
	virtual int Get_0xB2_IsoTableData(unsigned char ucCmdByte, unsigned char* ucData);

	// save data in ISO-table after [0xB2][0x??] iso command
	virtual int Set_0xB2_IsoTableData(unsigned char* ucData, int iLen, int iStatusByte);
	virtual int Set_0xB2_0x2B_IsoTableData(unsigned char* ucData, int iLen);

	// get data for [0xB3] iso command
	virtual int Get_0xB3_IsoTableData(unsigned char ucCmdByte, unsigned char* ucData);
	// get data for [0xB3][0x??] epc command
	virtual int Get_0xB3_0x24_IsoTableData(unsigned char* ucData);
	virtual int Get_0xB3_0x25_IsoTableData(unsigned char* ucData);
	virtual int Get_0xB3_0x26_IsoTableData(unsigned char* ucData);

	// save data after [0xB3][0x??] epc command
	virtual int Set_0xB3_IsoTableData(unsigned char* ucData, int iLen, int iStatusByte);

	// get data for [0xB4] iso command
	virtual int Get_0xB4_IsoTableData(unsigned char ucCmdByte, unsigned char* ucData);
	// get data for [0xB4][0x??] epc command
	int Get_0xB4_IsoTableData_NXP(unsigned char ucCmdByte, unsigned char* ucData);
	int Get_0xB4_IsoTableData_EM(unsigned char ucCmdByte, unsigned char* ucData);
	int Get_0xB4_IsoTableData_IDS(unsigned char ucCmdByte, unsigned char* ucData);

	// save data in ISO-table after [0xB4][0x??] epc command
	int Set_0xB4_IsoTableData(unsigned char ucCmdByte, unsigned char* ucData, int iLen);
	int Set_0xB4_IsoTableData_NXP(unsigned char ucCmdByte, unsigned char* ucData, int iLen);
	int Set_0xB4_IsoTableData_EM(unsigned char ucCmdByte, unsigned char* ucData, int iLen);
	int Set_0xB4_IsoTableData_IDS(unsigned char ucCmdByte, unsigned char* ucData, int iLen);

	int Get_0xC1_IsoTableData(unsigned char ucDESFireCmd, unsigned char* ucData);
	int Set_0xC1_IsoTableData(unsigned char ucDESFireCmd, unsigned char* ucData, int iLen);

	int Get_0xC2_IsoTableData(unsigned char ucMifarePlusCmd, unsigned char* ucData);
	int Set_0xC2_IsoTableData(unsigned char ucMifarePlusCmd, unsigned char* ucData, int iLen);

	int Get_0xC3_IsoTableData(unsigned char ucDESFireCmd, unsigned char ucMode, unsigned char* ucData);
	int Set_0xC3_IsoTableData(unsigned char ucDESFireCmd, unsigned char* ucData, int iLen);

	// save data in BRM-table after [0x21] or [0x22] read buffer
	virtual int SetBrmTableData(unsigned char ucMode, unsigned char* ucData);

	// save measurement data in CAD-table after [0x68]
#if !defined(__BORLANDC__)
	virtual int SetCADTableData(unsigned char ucCmd, long* ucData);
#endif

	virtual int SerializeIn(FEDM_XMLBase* xml, FEDM_XML_TREEITEM* root);
	virtual int SerializeOut(FEDM_XMLBase* xml, FEDM_XML_TREEITEM* root);

	void BuildPDMap();

// data members
protected:
	// if true, [0x81] Write Configuration allowed without previously
	// [0x80] Read Configuration
	bool m_bDisableReadCfgBeforeWriteCfg;

	// for classic-pro reader
	bool m_bSelectWithCinf;			// is set after detecting reader type
	bool m_bDisableSelectWithCinf;	// is a general setting

	// Java- and .NET-Wrapper need no tag handler
	bool m_bEnableTagHandler;

#ifdef _FEDM_TAG_HANDLER
	// option flag to keep a tag handler after Inventory alive, 
	// if the base class is identical, although the concrete type differs
	bool m_bTagHandlerOption_CreateCondition_Smart;
#endif

	// BRM-table for received data
	FEDM_BRMTAB_ARRAY m_BRMTable;

	// ISO-table for ISO Host Commands
	FEDM_ISOTAB_ARRAY m_ISOTable;

	// Channel Analyze Data table for received data
#if !defined(__BORLANDC__)
	FEDM_CADTable m_CADTable;
#endif

	// Reader Diagnostic information
	FEDM_ISC_READER_DIAGNOSTIC	m_ReaderDiagnostic;

	// general Reader information
	FEDM_ISC_READER_INFO		m_ReaderInfo;

	// OEM Reader names
	map<unsigned int, string> m_mapOemNames;

	// OEM Reader names for Reader variants
	map<unsigned int, string> m_mapOemNames4ReaderVariants;

	// all detected peripheral devices are collected in a map
	// the objects are created after a [0x66] with Mode=0x61
#if !defined(_FEDM_NO_PD_SUPPORT)
	FEDM_PD_MAP					m_mapPeripheralDevices;
#endif

	unsigned int m_uiBRMTableLength;		// length of valid data in BRM-table
	unsigned int m_uiISOTableLength;		// length of valid data in ISO-table
	unsigned int m_uiNonAddressedTabIndex;	// index in m_ISOTable (default: 0)

	// for temporary protocol data
	unsigned char	m_ucData[FEDM_MAX_PROTOCOL_SIZE+1];
	long			m_nData[516];
};

#endif // !defined(_FEDM_ISCREADER_H_INCLUDED_)
