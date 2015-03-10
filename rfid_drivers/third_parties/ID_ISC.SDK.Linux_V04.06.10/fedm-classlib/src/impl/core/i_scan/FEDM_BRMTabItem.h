/*-------------------------------------------------------
|                                                       |
|                  FEDM_BRMTabItem.h                    |
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

Version       		:	04.03.01 / 09.11.2012 / M. Hultsch
						- new method: GetISO15693Manufacturer()

						04.02.04 / 19.06.2012 / M. Hultsch
						- support for direction information (in combination with Gate People Counter)

						04.00.00 / 21.07.2011 / M. Hultsch
						- max. UID length increased to 96 Bytes

						03.03.00 / 21.02.2011 / M. Hultsch

Operation Systems	:	independent

Function			:	class for buffered read mode of OBID i-scan® and 
						OBID® classic-pro reader family


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_BRMTABITEM_H_INCLUDED_)
#define _FEDM_BRMTABITEM_H_INCLUDED_


#include "FEDM_ISC.h"
#include "../FEDM_DataBase.h"
#include "FEDM_TabItem.h"


class FEDM_ISCReader;



//#####################################################################################
// constants
//#####################################################################################
		
// sizes of memories in BRM-table
#define FEDM_ISC_BRM_TABLE_RxDB_SIZE		1024	// 256 blocks * 4 bytes = 1024 byte

#define FEDM_ISC_BRM_TABLE_MAX_ANTENNA		  32	// max 32 antenna values for each transponder




//####################################################################
// class FEDM_BRMTabItem (only for use inside FEDM_ISCReader-class)
// >> supports Buffered Read Mode <<
//####################################################################

/// @brief Class represents one record of requested Buffered-Read-Mode data with command [0x22] Read Buffer
///
/// <b>Description</b><br>
/// This class contains all record members, which are potentially part of the Reader&apos;s response of the command [0x22] Read Buffer.<br>
/// An item can be requested after the command was executed successfully with the method OBID::FEDM::Core::IBrmTableGroup::GetItemByIndex().<br>
/// <br>
/// [Example](@ref OBID::FEDM::Core::IBrmTableGroup::GetItemByIndex)
class _FEDM_ISC_CORE_EXT_CLASS FEDM_BRMTabItem : public FEDM_TabItem
{
public:

	// friend classes have access rights to protected members
	friend class FEDM_ISCReader;

	/// @brief Method to query the manufacturer name for an Transponder of ISO 15693 standard
	///
	/// @return String with manufacturer name
	///
	/// <b>Description</b><br>
	const char* GetISO15693Manufacturer();

	/// @brief Method to query the actual sizes of Transponder data buffers
	///
	/// @param[out] iRxDB_BlockCount Maximal number of data blocks
	/// @param[out] iRxDB_BlockSize Number of bytes in each data block
	///
	/// <b>Description</b><br>
	void GetTableSizes(	int& iRxDB_BlockCount, int& iRxDB_BlockSize);
	
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

protected:
	FEDM_BRMTabItem();			// default constructor
	FEDM_BRMTabItem(unsigned int uiTabIndex);	// main constructor
	/*virtual*/ ~FEDM_BRMTabItem();	// destructor

	void Init();			// set all data members to 0
	void Init(int iRxDB_BlockCount, int iRxDB_BlockSize);

	__int64 GetSnr();		// get lower serial number as 64-bit integer

	int		GetEpc(char* cEpc, int iBufLen);
	int		GetEpcRaw(unsigned char* cEpc, int iBufLen);
	int		GetEpcHeader();
	__int64 GetEpcDomainManager();
	__int64 GetEpcObjectClass();
	__int64 GetEpcSnr();

	// sets table values (used only in class FEDM_ISCReader)
	int SetData(unsigned char ucCmd, unsigned char* ucData, unsigned char* ucTrData, unsigned int& uiByteCnt);

	// sets serial numbers into table
	int SetSnr_0x21(unsigned char* ucData, unsigned int& iByteCnt);
	int SetSnr_0x22(unsigned char* ucTrData, unsigned char* ucData, unsigned int& iByteCnt);

	void ClearAntennaValues();

	// data elements
public:
	// For extended inventory command with request of antenna values
	unsigned char	m_ucFlags;

	unsigned char	m_ucTrType;				///< Transponder type
	unsigned char	m_ucEpcType;			// EPC type
	unsigned char	m_ucIDDT;				///< Identifier Data Type (IDDT). Only applicable with EPC Class 1 Gen 2 and ISO 18000-3M3
	unsigned char	m_ucAFI;				///< Application Family Identifier
	unsigned char	m_ucDsfID;				///< Data Storage Family Identifier. Only applicable with ISO 15693 standard
	unsigned char	m_ucSnrLen;				///< Length of UID or Serial Number in bytes
	unsigned char	m_ucSnr[FEDM_ISC_MAX_UID_LENGTH];	///< Buffer for UID or Serial Number
	unsigned char	m_ucClass1Gen2_PC[2];	///< Protocol-Control (PC) of EPC Class 1 Gen 2 or ISO 18000-3M- compliant Transponder
	unsigned char	m_ucClass1Gen2_XPC_W1[2];	///< Extended Protocol-Control (XPC W1) of EPC Class 1 Gen 2 or ISO 18000-3M- compliant Transponder
	FEDM_BYTE_ARRAY	m_ucRxDB;				///< Received data blocks
	unsigned char	m_ucDBAdr;				///< Data block address
	unsigned int	m_uiDBN;				///< Number of datablocks
	unsigned char	m_ucTimer[4];			///< Timer
	unsigned char	m_ucDate[5];			///< Date and timezone
	unsigned char	m_ucAntNr;				///< Antenna number
	unsigned char	m_ucInput;				///< Input states
	unsigned char	m_ucState;				///< Status flags, comming with input states
	unsigned char	m_ucDirection;			///< Direction information (in combination with Gate People Counter)
	unsigned char	m_ucMacAddress[6];		///< MAC-Address (only for Notification-Mode)

	unsigned char	m_ucBlockSize;			///< Block size for data blocks in m_ucRxDB

	unsigned char	m_ucAntCount;			///< Number of antenna reads
	unsigned char	m_ucAntNumber[FEDM_ISC_BRM_TABLE_MAX_ANTENNA];	///< Array with antenna numbers
	unsigned char	m_ucAntRSSI[FEDM_ISC_BRM_TABLE_MAX_ANTENNA];	///< Array with antenna Received Signal Strength Indication (RSSI)
	unsigned char	m_ucAntRes1[FEDM_ISC_BRM_TABLE_MAX_ANTENNA];	///< Array with antenna reserved bytes

	bool	m_bIsSnr;			///< Flag indicates, if item contains valid data for UID, TrType and IDDT
	bool	m_bIsEpc;
	bool	m_bIsAFI;			///< Flag indicates, if item contains valid data for DSFID and AFI
	bool	m_bIsDB;			///< Flag indicates, if item contains valid data for Transponder data
	bool	m_bIsTimer;			///< Flag indicates, if item contains valid data for Timer value
	bool	m_bIsDate;			///< Flag indicates, if item contains valid data for Date and Time
	bool	m_bIsAntNr;			///< Flag indicates, if item contains valid data for flag field with Antennas
	bool	m_bIsInput;			///< Flag indicates, if item contains valid data for Input and Status information
	bool	m_bIsRSSI;			///< Flag indicates, if item contains valid data for RSSI values for each antenna
	bool	m_bIsMacAddr;		///< Flag indicates, if item contains valid data for MAC-Address
	bool	m_bIsDirection;		///< Flag indicates, if item contains valid data for direction information from People-Counter


protected:

	unsigned char m_ucRxDB_TidBank[4];

	int m_iRxDB_BlockCount;
	int m_iRxDB_BlockSize;

	unsigned int		m_uiTabIndex;
};

#endif // !defined(_FEDM_BRMTABITEM_H_INCLUDED_)
