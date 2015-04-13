/*-------------------------------------------------------
|                                                       |
|                  FEDM_ISOTabItem.h                    |
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
Begin        		:	07.06.2000

Version       		:	04.03.01 / 25.09.2012 / M. Hultsch
						- support for new tag types: ISO 15693 STM M24LRxxE-R, STM LRIS64K

						04.02.01 / 31.05.2012 / M. Hultsch
						- support for new tag types: Innovatron and ASK CTx

						04.00.00 / 21.07.2011 / M. Hultsch
						- max. UID length increased to 96 Bytes

						03.03.01 / 31.03.2011 / M. Hultsch

Operation Systems	:	independent

Function			:	class for host mode of OBID i-scan® and 
						OBID® classic-pro reader family


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISOTABITEM_H_INCLUDED_)
#define _FEDM_ISOTABITEM_H_INCLUDED_



#include "FEDM_ISC.h"
#include "../FEDM_DataBase.h"
#include "FEDM_TabItem.h"

class FEDM_ISCReader;
class FEDM_ISCReaderModule;
#ifdef _FEDM_TAG_HANDLER
	class FedmIscTagHandler;
#endif


//#####################################################################################
// constants
//#####################################################################################

// sizes of memories in ISO-table
#define FEDM_ISC_ISO_TABLE_SEC_STATUS_SIZE	 256	// 256 blocks
#define FEDM_ISC_ISO_TABLE_TxDB_SIZE		8192	// 256 blocks * 32 bytes = 8192 byte
#define FEDM_ISC_ISO_TABLE_RxDB_SIZE		8192	// 256 blocks * 32 bytes = 8192 byte

// only for EPCglobal Class1, Generation2 Transponder
#define FEDM_ISC_ISO_TABLE_EPC_BANK_SIZE	 256	// 128 blocks * 2 bytes
#define FEDM_ISC_ISO_TABLE_TID_BANK_SIZE	 256	// 128 blocks * 2 bytes
#define FEDM_ISC_ISO_TABLE_RES_BANK_SIZE	 256	// 128 blocks * 2 bytes

#define FEDM_ISC_ISO_TABLE_MAX_ANTENNA		   8	// max 8 antenna values for each transponder




//####################################################################
// class FEDM_ISOTabItem (only for use inside FEDM_ISCReader-class)
// >> supports ISO Host Commands <<
//####################################################################

/// @brief Class represents one record of requested Host-Mode data with command [0xB0][0x01] Inventory
///
/// <b>Description</b><br>
/// This class contains all record members, which are potentially part of the Reader&apos;s response of the command [0xB0][0x01] Inventory.<br>
/// An item can be requested after the command was executed successfully with the method OBID::FEDM::Core::IHmTableGroup::GetItemByIndex().<br>
/// <br>
/// The [example](@ref OBID::FEDM::Core::IHmTableGroup::GetItemByIndex) demonstrate the direct table access method. 
/// For more efficient and modern coding, the concept of TagHandlers should be preferred.
/// See [TagInventory](@ref OBID::FEDM::Core::ITagGroup::TagInventory) for a simple example.<br>
/// 
class _FEDM_ISC_CORE_EXT_CLASS FEDM_ISOTabItem : public FEDM_TabItem
{
public:

	// friend classes have access rights to protected members
	friend class FEDM_ISCReader;
	friend class FEDM_ISCReaderModule;
#ifdef _FEDM_TAG_HANDLER
	friend class FedmIscTagHandler;
#if !defined(_FEDM_NO_TAG_HANDLER_EPC_C1_G2)
	friend class FedmIscTagHandler_EPC_Class1_Gen2;
	friend class FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A;
#endif
#if !defined(_FEDM_NO_TAG_HANDLER_ISO18000_3M3)
	friend class FedmIscTagHandler_ISO18000_3M3;
#endif
#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
	friend class FedmIscTagHandler_ISO14443;
	friend class FedmIscTagHandler_ISO14443_Innovatron;
	friend class FedmIscTagHandler_ISO14443_2;
	friend class FedmIscTagHandler_ISO14443_2_Innovision_Jewel;
	friend class FedmIscTagHandler_ISO14443_2_STM_SR176;
	friend class FedmIscTagHandler_ISO14443_2_STM_SRIxxx;
	friend class FedmIscTagHandler_ISO14443_3;
	friend class FedmIscTagHandler_ISO14443_3_ASK_CTx;
	friend class FedmIscTagHandler_ISO14443_3_Infineon_my_d;
	friend class FedmIscTagHandler_ISO14443_3_Infineon_my_d_move;
	friend class FedmIscTagHandler_ISO14443_3_MIFARE_Classic;
	friend class FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL1;
	friend class FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL2;
	friend class FedmIscTagHandler_ISO14443_3_MIFARE_Ultralight;
	friend class FedmIscTagHandler_ISO14443_4;
	friend class FedmIscTagHandler_ISO14443_4_Maxim;
	friend class FedmIscTagHandler_ISO14443_4_Maxim_MAX66000;
	friend class FedmIscTagHandler_ISO14443_4_Maxim_MAX66020;
	friend class FedmIscTagHandler_ISO14443_4_Maxim_MAX66040;
	friend class FedmIscTagHandler_ISO14443_4_MIFARE_DESFire;
	friend class FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1;
	friend class FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3;
	friend class FedmIscTagHandler_ISO14443_4_MIFARE_Plus;
	friend class FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL1;
	friend class FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL2;
	friend class FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3;
#endif
#if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)
	friend class FedmIscTagHandler_ISO15693;
	friend class FedmIscTagHandler_ISO15693_STM;
	friend class FedmIscTagHandler_ISO15693_STM_LRI2K;
	friend class FedmIscTagHandler_ISO15693_STM_LRIS2K;
	friend class FedmIscTagHandler_ISO15693_STM_LRIS64K;
	friend class FedmIscTagHandler_ISO15693_STM_M24LR64R;
	friend class FedmIscTagHandler_ISO15693_STM_M24LRxxER;
	friend class FedmIscTagHandler_ISO15693_NXP;
	friend class FedmIscTagHandler_ISO15693_NXP_ICODE_SLI;
	friend class FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_L;
	friend class FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_S;
	friend class FedmIscTagHandler_ISO15693_NXP_ICODE_SLIX;
	friend class FedmIscTagHandler_ISO15693_NXP_ICODE_SLIX_L;
	friend class FedmIscTagHandler_ISO15693_NXP_ICODE_SLIX_S;
	friend class FedmIscTagHandler_ISO15693_Infineon;
	friend class FedmIscTagHandler_ISO15693_Infineon_my_d;
	friend class FedmIscTagHandler_ISO15693_TI;
	friend class FedmIscTagHandler_ISO15693_TI_Tag_it_HFI_Pro;
	friend class FedmIscTagHandler_ISO15693_TI_Tag_it_HFI_Plus;
	friend class FedmIscTagHandler_ISO15693_Fujitsu;
	friend class FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx;
	friend class FedmIscTagHandler_ISO15693_EM;
	friend class FedmIscTagHandler_ISO15693_EM_4034;
	friend class FedmIscTagHandler_ISO15693_KSW;
	friend class FedmIscTagHandler_ISO15693_Maxim;
	friend class FedmIscTagHandler_ISO15693_Maxim_MAX66100;
	friend class FedmIscTagHandler_ISO15693_Maxim_MAX66120;
	friend class FedmIscTagHandler_ISO15693_Maxim_MAX66140;
	friend class FedmIscTagHandler_ISO15693_IDS_SL13A;
#endif

#endif

	bool HasTagHandler();

	FEDM_ISOTabItem();							// default constructor
	FEDM_ISOTabItem(unsigned int uiTabIndex, FEDM_ISCReader* pReader);	// main constructor
	/*virtual*/ ~FEDM_ISOTabItem();					// destructor

	void Init();					// set all data members to 0
	void Init(	int iRxDB_BlockCount, int iRxDB_BlockSize, int iTxDB_BlockCount, int iTxDB_BlockSize);
	
	/// @brief Method to query the actual sizes of Transponder data buffers
	///
	/// @param[out] iRxDB_BlockCount Maximal number of data blocks in Receive Buffer
	/// @param[out] iRxDB_BlockSize Number of bytes in each data block in Receive Buffer
	/// @param[out] iTxDB_BlockCount Maximal number of data blocks in Transmit Buffer
	/// @param[out] iTxDB_BlockSize Number of bytes in each data block in Transmit Buffer
	///
	/// <b>Description</b><br>
	void GetTableSizes(	int& iRxDB_BlockCount, int& iRxDB_BlockSize, int& iTxDB_BlockCount, int& iTxDB_BlockSize);

	/// @brief Method to query the actual sizes of Transponder data buffers
	///
	/// @param[out] sSnr Buffer for Unified Identifier (UID) resp. Serial Number (SNR) resp. Electronic Product Code (EPC)
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	///
	/// <b>Description</b><br>
	int		GetSnr(std::string& sSnr);	// serial number as STL-string

	__int64 GetSnr();				// get lower serial number as 64-bit integer
	__int64 GetSnr(int i64BitPart);	// get part (1..4) of serial number as 64-bit integer

	int		GetEpc(char* cEpc, int iBufLen);
	int		GetEpcRaw(unsigned char* cEpc, int iBufLen);
	int		GetEpcHeader();
	__int64 GetEpcDomainManager();
	__int64 GetEpcObjectClass();
	__int64 GetEpcSnr();

#ifdef _FEDM_TAG_HANDLER
	const char* GetTagName();
#endif

	/// @brief Method to query the manufacturer name for an Transponder of ISO 15693 standard
	///
	/// @return String with manufacturer name
	///
	/// <b>Description</b><br>
	const char* GetISO15693Manufacturer();

	/// @brief Method to query the manufacturer name for an Transponder of ISO 14443, Part A standard
	///
	/// @return String with manufacturer name
	///
	/// <b>Description</b><br>
	const char* GetISO14443AManufacturer();
	
	/// @brief Method to query the Chip-ID for an Transponder of EPC Class 1 Gen 2 or ISO 18000-3M3 standard
	///
	/// @return Chip-ID
	///
	/// <b>Description</b><br>
	unsigned int GetEpcC1G2TagModelNumber();

	/// @brief Method to query the Manufacturer-ID of EPC Class 1 Gen 2 or ISO 18000-3M3 standard
	///
	/// @return Manufacturer-ID
	///
	/// <b>Description</b><br>
	unsigned int GetEpcC1G2MaskDesignerID();

	/// @brief Method to query the manufacturer name for an Transponder of EPC Class 1 Gen 2 or ISO 18000-3M3 standard
	///
	/// @return String with manufacturer name
	///
	/// <b>Description</b><br>
	const char* GetEpcC1G2MaskDesignerName();

	/// @brief Method to query if the Protocol-Control (PC) is extended (only applicable with EPC Class 1 Gen 2 and ISO 18000-3M3)
	///
	/// @return true, if PC is extended, otherwise false
	///
	/// <b>Description</b><br>
	/// Method to query if the Protocol-Control (PC) of an EPC Class 1 Gen 2 Transponder or ISO 18000-3M3 is extended with 1 additional block of 2 bytes.
	bool IsExtendedPC_W1();

public:
	// sets table values (used only in class FEDM_ISCReader)
	int SetData(unsigned char ucCmd, unsigned char* ucData, int& iByteCnt, unsigned char ucBank=0, unsigned int uiAdr=0);
	// gets table values (used only in class FEDM_ISCReader)
	int GetData(unsigned char ucCmd, unsigned char* ucData, int& iByteCnt, unsigned char ucBank, unsigned int uiAdr);

protected:
	// sets serial numbers into table
	int SetSnr(unsigned char* ucData, int& iByteCnt);
	int SetSnr_wo_Antenna(unsigned char* ucData, int& iByteCnt);
	int SetSnr_with_Antenna(unsigned char* ucData, int& iByteCnt);

	void ClearAntennaValues();

	// for friend class FEDM_ISCReader
#ifdef _FEDM_TAG_HANDLER
	int  CreateTagHandler();
#if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)
	int  CreateTagHandler_ISO15693();
#endif
	void RemoveTagHandler();
#endif


	// data elements
public:
	// for extended inventory command with request of antenna values
	unsigned char	m_ucFlags;				// flag field indicating UID and/or antenna values valid

	unsigned char	m_ucAFI;				///< Application Family Identifier
	union
	{
		unsigned char	m_ucDsfID;			///< Data Storage Family Identifier (only applicable with ISO 15693 compliant Transponder)
		unsigned char	m_ucChipID;			///< Chip Identifier for SR176 and SRIxx Transponder og manufacturer STMicroelectronics SA (only applicable with ISO 14443 compliant Transponder)
		unsigned char	m_ucTrInfo;			///< Additional transponder info (only applicable with ISO 14443A compliant Transponder)
		unsigned char	m_ucProtoInfo;		///< Protocol info byte (only applicable with ISO 14443B compliant Transponder)
		unsigned char	m_ucIDDT;			///< Identifier Data Type (IDDT) (only applicable with EPC Class 1 Gen 2 and ISO 18000-3M3 compliant Transponder)

		// for ISO 14443 ASK CTx Tag
		struct
		{
			unsigned char m_ucProductCode;	///< Product Code (only applicable with ISO 14443 ASK CTx Tag)
			unsigned char m_ucFabCode;		///< Fab Code (only applicable with ISO 14443 ASK CTx Tag)
			unsigned char m_ucAppCode;		///< App Code (only applicable with ISO 14443 ASK CTx Tag)
			unsigned char m_ucEmbedderCode;	///< Embedder Code (only applicable with ISO 14443 ASK CTx Tag)
		};

		// for ISO 14443-B Innovatron Tag
		struct
		{
			unsigned char m_ucVerlog;		///< Verlog (only applicable with ISO 14443-B' Innovatron Tag)
			unsigned char m_ucConfig;		///< Config (only applicable with ISO 14443-B' Innovatron Tag)
			unsigned char m_ucAtrLen;		///< ATR Length (only applicable with ISO 14443-B' Innovatron Tag)
			unsigned char m_ucAtr[33];		///< Array with ATR (only applicable with ISO 14443-B' Innovatron Tag)
		};
	};

	unsigned char	m_ucOptInfo;			///< Optional informations (only applicable with ISO 14443A compliant Transponder)
	unsigned char	m_ucMemSize[3];			///< Array with complete Memory Size (only applicable with ISO 15693 compliant Transponder)
	unsigned char	m_ucICRef;				///< IC reference (only applicable with ISO 15693 compliant Transponder)
	unsigned char	m_ucTrType;				///< Transponder type
	unsigned char	m_ucTrSubType;			///< Transponder sub-type of generic transponder type
	unsigned char	m_ucEpcType;			// EPC type
	unsigned char	m_ucSnrLen;				///< Length of UID or Serial Number in bytes
	unsigned char	m_ucSnr[FEDM_ISC_MAX_UID_LENGTH];			///< Buffer for UID or Serial Number
	unsigned char	m_ucClass1Gen2_PC[2];	///< Protocol-Control (PC) of EPC Class 1 Gen 2 or ISO 18000-3M- compliant Transponder
	unsigned char	m_ucClass1Gen2_XPC_W1[2];	///< Extended Protocol-Control (XPC W1) of EPC Class 1 Gen 2 or ISO 18000-3M- compliant Transponder
	FEDM_BYTE_ARRAY	m_ucSecStatus;			///< Array for received block security status

	/// @brief Data buffer for Transponder data to be sent to
	///
	/// <b>Description</b><br>
	/// Initial size is:<br>
	/// <table border = "1">
	/// <tr>
	/// <td><center>Buffer Size in bytes</center></td>
	/// <td><center>No. of Data Blocks</center></td>
	/// <td><center>Block Size in bytes</center></td>
	/// </tr>
	/// <tr>
	/// <td><center>8192</center></td>
	/// <td><center>256</center></td>
	/// <td><center>32</center></td>
	/// </tr>
	/// </table>
	FEDM_BYTE_ARRAY	m_ucTxDB;

	/// @brief Data buffer for received Transponder data
	///
	/// <b>Description</b><br>
	/// Initial size is:<br>
	/// <table border = "1">
	/// <tr>
	/// <td><center>Buffer Size in bytes</center></td>
	/// <td><center>No. of Data Blocks</center></td>
	/// <td><center>Block Size in bytes</center></td>
	/// </tr>
	/// <tr>
	/// <td><center>8192</center></td>
	/// <td><center>256</center></td>
	/// <td><center>32</center></td>
	/// </tr>
	/// </table>
	FEDM_BYTE_ARRAY	m_ucRxDB;

	unsigned char	m_ucTxCB[4][4];			// transmit configuration block 0..3 a 4 Byte
	unsigned char	m_ucRxCB[4][4];			// receive configuration block 0..3 a 4 Byte

	// ISO14443-4 Transponder Info
	unsigned char	m_ucFSCI;				///< Max. Frame Size (part of ISO14443-4 Transponder Info)
	unsigned char	m_ucFWI;				///< Frame Waiting Time (part of ISO14443-4 Transponder Info)
	unsigned char	m_ucDSI;				///< Divisor Send Integer (part of ISO14443-4 Transponder Info)
	unsigned char	m_ucDRI;				///< Divisor Receive Integer (part of ISO14443-4 Transponder Info)
	unsigned char	m_ucNad;				///< Node Access (part of ISO14443-4 Transponder Info)
	unsigned char	m_ucCid;				///< Card Identifier (part of ISO14443-4 Transponder Info)

	unsigned char	m_ucAntCount;			///< number of antenna reads
	unsigned char	m_ucAntNumber[FEDM_ISC_ISO_TABLE_MAX_ANTENNA];	///< Array with antenna numbers
	unsigned char	m_ucAntStatus[FEDM_ISC_ISO_TABLE_MAX_ANTENNA];	///< Array with antenna status
	unsigned char	m_ucAntRSSI[FEDM_ISC_ISO_TABLE_MAX_ANTENNA];	///< Array with antenna Received Signal Strength Indication (RSSI)

	unsigned int	m_uiValue;				///< Value field for MIFARE DESFire

	FEDM_BYTE_ARRAY	m_ucTxDB_EpcBank;	///< transmit data block with block size 2 (only for EPC Class 1, Gen 2 and ISO 18000-3M3 compliant Transponder)
	FEDM_BYTE_ARRAY	m_ucRxDB_EpcBank;	///< receive data block with block size 2 (only for EPC Class 1, Gen 2 and ISO 18000-3M3 compliant Transponder)
	FEDM_BYTE_ARRAY	m_ucTxDB_TidBank;	///< transmit data block with block size 2 (only for EPC Class 1, Gen 2 and ISO 18000-3M3 compliant Transponder)
	FEDM_BYTE_ARRAY	m_ucRxDB_TidBank;	///< receive data block with block size 2 (only for EPC Class 1, Gen 2 and ISO 18000-3M3 compliant Transponder)
	FEDM_BYTE_ARRAY	m_ucTxDB_ResBank;	///< transmit data block with block size 2 (only for EPC Class 1, Gen 2 and ISO 18000-3M3 compliant Transponder)
	FEDM_BYTE_ARRAY	m_ucRxDB_ResBank;	///< receive data block with block size 2 (only for EPC Class 1, Gen 2 and ISO 18000-3M3 compliant Transponder)


	unsigned char	m_ucMemSizeLen;	///< Length of [Memory-Size](@ref m_ucMemSize) from ISO Command [0xB0][0x2B] Get System Information (normally 2)
	
	/// @brief Block size for data blocks in m_ucRxDB and m_ucTxDB
	///
	/// The block size is normally set after a read of Transponder data.<br>
	/// Use OBID::FEDM::Core::IHmTableGroup::SetSize(unsigned int,unsigned int,unsigned int,unsigned int,unsigned int) to change from application side
	unsigned char	m_ucBlockSize;

	bool	m_bIsBlockSizeSet;	///< read-only flag indicates if blocksize is set after read command

	/// @brief Read-only flag indicates if this transponder is selected for the next ISO-Host-Command
	///
	/// - a [0x25] Select sets this flag<br>
	/// - a [0x26] Reset to Ready or next [0x25] Select with other UID clears this flag<br>
	bool	m_bIsSelected;

	bool	m_bIsSnr;				///< Flag indicates, if item contains valid data for UID, TrType and IDDT
	bool	m_bIsEpc;
	bool	m_bIsAFI;				///< Flag indicates, if item contains valid data for DSFID and AFI
	bool	m_bIsSysInfo;			///< Flag indicates, if item contains valid ISO 15693 Transponder Info
	bool	m_bIsISO14443_4Info;	///< Flag indicates, if item contains valid 14443-4 Transponder Info
	bool	m_bIsRSSI;				///< Flag indicates, if item contains valid data for RSSI values for each antenna


protected:

	int m_iRxDB_BlockCount;
	int m_iRxDB_BlockSize;
	int m_iTxDB_BlockCount;
	int m_iTxDB_BlockSize;

#ifdef _FEDM_TAG_HANDLER
	FedmIscTagHandler*	m_pTagHandler;
#endif
	FEDM_ISCReader*		m_pReader;		// pointer to basic reader class

	unsigned int		m_uiTabIndex;
};

#endif // !defined(_FEDM_ISOTABITEM_H_INCLUDED_)
