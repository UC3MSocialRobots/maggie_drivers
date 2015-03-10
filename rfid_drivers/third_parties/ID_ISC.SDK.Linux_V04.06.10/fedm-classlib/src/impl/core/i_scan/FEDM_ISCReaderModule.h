/*-------------------------------------------------------
|                                                       |
|                  FEDM_ISCReaderModule.h               |
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
Begin        		:	06.11.2003

Version       		:	04.02.08 / 13.08.2012 / M. Hultsch
						- support for events from People Counter
		 
						04.02.00 / 04.04.2012 / M. Hultsch
						- renamed method: GetNonAddressedTagHandler to CreateNonAddressedTagHandler
		 
						04.00.00 / 21.07.2011 / M. Hultsch
						- new method: GetTcpConnectionState()

						03.02.07 / 25.10.2010 / M. Hultsch
						- non-addressed with taghandler

						03.01.04 / 30.03.2010 / M. Hultsch

Operation Systems	:	independent

Function			:	classes for OBID i-scan and OBID classic-pro reader family


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISCREADER_MODULE_H_INCLUDED_)
#define _FEDM_ISCREADER_MODULE_H_INCLUDED_



#include "FEDM_ISCReader.h"
#include "../FEDM_XmlReaderCfgProfileModul.h"

class FEDM_ISOTabItem;				// class for iso host mode
class FedmIscTagHandler;
class FedmIscMyAxxessReader;


//#####################################################################################
// constants
//#####################################################################################


// defines for uiFlag in FEDM_TASK_INIT
#define FEDM_TASKCB1						1
#define FEDM_TASKCB2						2
#define FEDM_TASKCB3						3
#define FEDM_TASKCB_NET1				  101 // not applicable

// defines for task identifier
#define FEDM_TASKID_FIRST_NEW_TAG			1
#define FEDM_TASKID_EVERY_NEW_TAG			2
#define FEDM_TASKID_NOTIFICATION			3
#define FEDM_TASKID_QUEUE_RESPONSE			4 // not applicable
#define FEDM_TASKID_APDU_RESPONSE			5 // not applicable
#define FEDM_TASKID_SAM_RESPONSE			6 // not applicable
#define FEDM_TASKID_MAX_EVENT				7 // not applicable



//#####################################################################################
// types
//#####################################################################################


typedef map<string, FedmIscTagHandler*>				FEDM_ISC_TAG_LIST;
typedef map<string, FedmIscTagHandler*>::iterator	FEDM_ISC_TAG_LIST_ITOR;


// structure for initializing task event handlers
typedef struct _FEDM_TASK_INIT
{
	unsigned int	uiUse;			// defines the task (e.g. FEDM_TASKID_FIRST_NEW_TAG)
	unsigned int	uiFlag;			// specifies the use of the union (e.g. FEDM_TASKCB1)
	void*			pAny;			// pointer to anything, which is reflected as the first parameter 
									// in the callback function (e.g. can be used to pass the object pointer)
	int				iConnectByHost;	// if 0: TCP/IP connection is initiated by reader. otherwise by host
	char			cIPAdr[16];		// server ip address
	int				iPortNr;		// server or host port address
	unsigned int	uiTimeout;		// timeout for asynchronous task:
									// FEDM_TASKID_FIRST_NEW_TAG		: in steps of 100ms 
									// FEDM_TASKID_EVERY_NEW_TAG		: in steps of 100ms
									// FEDM_TASKID_QUEUE_RESPONSE		: in steps of 100ms
									// FEDM_TASKID_APDU_RESPONSE		: in steps of 100ms
									// FEDM_TASKID_SAM_RESPONSE			: in steps of 100ms
									// FEDM_TASKID_NOTIFICATION			: in s
									// FEDM_TASKID_MAX_EVENT			: in s
									// FEDM_TASKID_PEOPLE_COUNTER_EVENT	: in s

	//  only for authentication in notification mode
	bool			bCryptoMode;		// security mode on/off
	unsigned int	uiAuthentKeyLength;	// authent key length
	unsigned char	ucAuthentKey[32];	// authent key

	// only for notification tasks
	bool			bKeepAlive;					// if true, keep alive option will be enabled (recommended)
	unsigned int	uiKeepAliveIdleTime;		// wait time in ms for first probe after connection is dropped down
												// for Linux: time is rounded up to seconds
	unsigned int	uiKeepAliveProbeCount;		// only for Linux: number of probes
												// for Windows Server 2003, 2000 and XP it is fixed to 5 by Microsoft
												// for Windows Vista and later it is fixed to 10 by Microsoft
	unsigned int	uiKeepAliveIntervalTime;	// wait time in ms between probes

	// list with different function pointers
	union
	{
		// for C++ callback; for inventory and notification task
		void	(FEDM_CALL* cbFct1)(void* pAny, int iError, unsigned char ucCmd);
		// for C++; only for notification task
		void	(FEDM_CALL* cbFct2)(void* pAny, int iError, unsigned char ucCmd, char* cIPAdr, int iPortNr);
		// for C++; only for my axxess Reader events handled by class FedmIscMyAxxessReader
		//int		(*cbFct3)(void* pAny, int iError, unsigned char ucCmd, char* cIPAdr, int iPortNr, unsigned char& ucAction);
#ifdef _FEDM_WINDOWS
		// for .NET delegates; for inventory and notification task
		void	(CALLBACK* cbFctNET1)(int iError, unsigned char ucCmd);
#endif
	};

	// list with different additional options
	union
	{
		int iNotifyWithAck;	// 0: notification without acknowledge
							// 1: notification with acknowledge
	};

} FEDM_TASK_INIT;



//#####################################################################################
// class FEDM_ISCReaderModule
//#####################################################################################

class _FEDM_ISC_CORE_EXT_CLASS FEDM_ISCReaderModule : public FEDM_ISCReader
{
public:

	// friend classes have access rights to protected members
	friend class FEDM_ISOTabItem;
	friend class FedmIscMyAxxessReader;

	FEDM_ISCReaderModule();
	virtual ~FEDM_ISCReaderModule();


	// serial port
	int ConnectCOMM(int iPortNr);
	
	// LAN, WLAN (TCP/IP)
	int ConnectTCP(char* cHostAdr, int iPortNr);
	
	// USB (optional: nDeviceID=0 >> ScanAndOpen)
	int ConnectUSB(unsigned long dwDeviceID);
	
	int DisConnect();								// for all port types
	bool IsConnected();								// for all port types
	int GetTcpConnectionState();					// for TCP/IP connection

	// for Reader in crypto-mode
	int ReaderAuthentication(unsigned char ucAuthentType, string sAuthentKey);
	int ReaderAuthentication(unsigned char ucAuthentType, unsigned int uiAuthentKeyLength, unsigned char* ucAuthentKey);

	// main transponder communication methods
#ifdef _FEDM_TAG_HANDLER
	// method executes synchronous Inventory (for all i-scan and classic-pro Readers)
	FEDM_ISC_TAG_LIST*	TagInventory(	bool bAll=true,
										unsigned char ucMode=0x00,
										unsigned char ucAntennas=1 );

	FedmIscTagHandler*  TagSelect(		FedmIscTagHandler* pTagHandler,
										unsigned int uiTagDriver=0 );

	FEDM_ISC_TAG_LIST*	GetTagList();
  #ifdef _WIN32_WCE
	FedmIscTagHandler*  GetTagHandler(char* sSnr);
  #else
	FedmIscTagHandler*  GetTagHandler(string sSnr);
  #endif
	FedmIscTagHandler*  CreateNonAddressedTagHandler(unsigned int uiTagHandlerType);
	FedmIscTagHandler*  GetSelectedTagHandler();
	FedmIscTagHandler*  Convert_EPC_C1_G2_TagHandler(FedmIscTagHandler* pTagHandler, unsigned char ucMfrCode, unsigned int uiTagHandlerType);
#endif

	// methods for executing asynchronous tasks
	int StartAsyncTask(FEDM_TASK_INIT* pInit);
	int CancelAsyncTask();
	int TriggerAsyncTask();

	// callback functions, invoked by asynchronous task
	static void cbsTaskRsp1(void* pAny, 
							int iReaderHnd, 
							int iTaskID, 
							int iError, 
							unsigned char ucCmd,
							unsigned char* ucRspData, 
							int iRspLen );

	static void cbsTaskRsp2(void* pAny, 
							int iReaderHnd, 
							int iTaskID, 
							int iError, 
							unsigned char ucCmd,
							unsigned char* ucRspData, 
							int iRspLen,
							char* cIPAdr,
							int iPortNr );

	// setting port parameters
	int SetPortPara(char* cPara, char* cValue);		// for all port types
	int SetPortPara(char* cPara, int iValue);		// for all port types

	// getting port parameters
	int GetPortPara(char* cPara, char* cValue);		// for all port types
	int GetPortPara(char* cPara, int* iValue);		// for all port types

	int SetPortHnd(int iPortHnd);

	// complex communication methods
	int ReadCompleteConfiguration(bool bEEPROM);	// reads complete reader configuration
	int WriteCompleteConfiguration(bool bEEPROM);	// writes complete reader configuration
	int ResetCompleteConfiguration(bool bEEPROM);	// resets complete configuration to factory default
	int ApplyConfiguration(bool bEEPROM);			// writes modified configuration pages to reader
	unsigned char* GetModifiedCfgPageFlags();		// returns a 16 byte flag-field
	
	// main method for XML based reader profiling
	int SerializeProfile(bool bRead, char* sFileName);

	int TransferXmlFileToReaderCfg(char* sFileName);	// transfer configuration from XML file to reader
	int TransferReaderCfgToXmlFile(char* sFileName);	// transfer configuration from reader to XML file

	// reads readers software information
	int ReadSoftVersion(unsigned char* ucSWRev,	
						unsigned char* ucDevRev, 
						unsigned char* ucHWType, 
						unsigned char* ucSWType, 
						unsigned char* ucTrp);

	// read complete reader info
	FEDM_ISC_READER_INFO*		ReadReaderInfo(unsigned int uiProtocolFrame = FEDM_PRT_FRAME_STANDARD);

	// read the complete reader diagnostic
	FEDM_ISC_READER_DIAGNOSTIC* ReadReaderDiagnostic();


protected:
#ifdef _FEDM_TAG_HANDLER
	FedmIscTagHandler* TagSelect_CreateNewTagHandler(int iIdx, unsigned int uiTagDriver, FedmIscTagHandler* pTagHandler);
#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
	FedmIscTagHandler* TagSelect_EvaFormat1(int iIdx, FedmIscTagHandler* pTagHandler);
	FedmIscTagHandler* TagSelect_EvaFormat2(int iIdx, FedmIscTagHandler* pTagHandler);
	FedmIscTagHandler* TagSelect_EvaFormat3(int iIdx, FedmIscTagHandler* pTagHandler);
#endif
#endif

	// callback function for asynchronous inventory, invoked by static callback member
	virtual void cbNewTag(	int iReaderHnd, 
							int iTaskID, 
							int iError, 
							unsigned char* ucRspData, 
							int iRspLen );

	// callback function for notifications from a reader, invoked by static callback member
	virtual void cbNewNotify(	int iReaderHnd, 
								int iTaskID, 
								int iError, 
								unsigned char* ucRspData, 
								int iRspLen,
								char* cIPAdr,
								int iPortNr );

	// callback function for keep alive message with protocol [0x6E] from a reader, invoked by static callback member
	virtual void cbNewReaderDiagnostic(	int iReaderHnd, 
										int iTaskID, 
										int iError, 
										unsigned char* ucRspData, 
										int iRspLen,
										char* cIPAdr,
										int iPortNr );
	
	virtual void cbNewInputEvent(	int iReaderHnd, 
									int iTaskID, 
									int iError, 
									unsigned char* ucRspData, 
									int iRspLen,
									char* cIPAdr,
									int iPortNr );

	// callback function for notifications from a People Counter, invoked by static callback member
	virtual void cbNewNotifyFromPeopleCounter(	int iReaderHnd, 
												int iTaskID, 
												int iError, 
												unsigned char* ucRspData, 
												int iRspLen,
												char* cIPAdr,
												int iPortNr );


public:
#ifdef _FEDM_XML_SUPPORT
	FEDM_XMLReaderCfgProfileModul	m_ProfileModule;
#endif

protected:
	int m_iCommMode;	// see constants in fedm.h

	bool m_bActiveAsyncTask;

	FEDM_TASK_INIT		m_TaskInit;

#ifdef _FEDM_TAG_HANDLER
	FEDM_ISC_TAG_LIST	m_mapTagHandler;
#endif

	unsigned char		m_ucModifiedCfgPages[16];	// flag field with modified CFG pages is valid after ApplyConfiguration

};

#endif // !defined(_FEDM_ISCREADER_MODULE_H_INCLUDED_)
