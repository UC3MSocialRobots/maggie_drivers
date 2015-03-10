/*-------------------------------------------------------
|                                                       |
|                  FEDM_ISC.h							|
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
Begin        		:	20.09.2007

Version       		:	04.06.06 / 24.09.2014 / M. Hultsch
						- new reader: ID CPR60
						
						04.06.03 / 24.06.2014 / B. Tschunko
						- new reader: ID MAX.U1002
						
						04.06.01 / 03.03.2014 / M. Hultsch
						- TMP data memory (FEDM_ISC_MAX_TMPDATA_MEM) increased from 5K to 6K

						04.05.05 / 12.11.2013 / M. Hultsch
						- new reader: ID ISC.PRH200
						
						04.05.04 / 03.09.2013 / M. Hultsch
						- new reader: ID ISC.LRU1002

						04.05.03 / 15.07.2013 / M. Hultsch
						- new reader: OBID myAXXESS onTop

						04.04.02 / 18.01.2013 / M. Hultsch
						- new reader: ID CPR47

						04.02.01 / 31.05.2012 / M. Hultsch
						- support for new tag types: Innovatron and ASK CTx

						04.02.00 / 18.04.2012 / M. Hultsch
						- new reader: ID CPR46.XX

						04.00.09 / 23.01.2012 / M. Hultsch
						- new reader: ID ISC.LR1002

						04.00.06 / 22.11.2011 / M. Hultsch
						- new nPA reader: OBID myAXXESS standard, OBID myAXXESS comfort

						04.00.00 / 21.07.2011 / M. Hultsch
						- max. UID length increased to 96 Bytes

						03.03.01 / 07.03.2011 / M. Hultsch
						- new reader ID ISC.MRU102 added

						03.03.00 / 01.02.2011 / M. Hultsch
						- new reader ID ISC.LR2500-A added

						03.02.10 / 21.12.2010 / M. Hultsch
						- FEDM_ISC_MAX_RFC_EEDATA_MEM and FEDM_ISC_MAX_RFC_RAMDATA_MEM increased from 1KB to 4KB

						03.02.05 / 08.09.2010 / M. Hultsch
						- FEDM_ISC_MAX_TMPDATA_MEM increased from 4KB to 5KB

						03.02.03 / 27.07.2010 / M. Hultsch
						- new reader ID ISC.LR2500-B added

						03.01.05 / 23.04.2010 / M. Hultsch
						- new reader ID ISC.LR2500 added

						03.01.04 / 16.04.2010 / M. Hultsch
						- new reader ID ISC.MR102 added

						03.01.02 / 22.01.2010 / M. Hultsch
						- new Manufacturer for [0xB1] ISO-15693 Host Commands

						03.01.00 / 22.01.2010 / M. Hultsch
						- new Manufacturer Codes for [0xB1] ISO-15693 Host Commands
						- new FU-Type: ID ISC.ANT.GPC

						03.00.18 / 02.12.2009 / M. Hultsch
						- ID MLC.COM and ID MLC.EMPS added

						03.00.13 / 02.10.2009 / M. Hultsch
						- ID MAX50.xx added
						- new Manufacturer Codes for [0xB1] ISO-15693 Host Commands

Operation Systems	:	independent

Function			:	constants for OBID i-scan® and OBID® classic-pro reader family


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_H_INCLUDED_)
#define _FEDM_ISC_H_INCLUDED_

#include "../FEDM.h"


//#####################################################################################
// constants
//#####################################################################################
		
// size of memories in bytes
#define FEDM_ISC_MAX_RFC_EEDATA_MEM		4096	// memory for parameters in readers EEPROM
#define FEDM_ISC_MAX_RFC_RAMDATA_MEM	4096	// memory for parameters in readers RAM
#define FEDM_ISC_MAX_ACC_EEDATA_MEM		8192	// memory for parameters in readers EEPROM
#define FEDM_ISC_MAX_ACC_RAMDATA_MEM	8192	// memory for parameters in readers RAM
#define FEDM_ISC_MAX_TMPDATA_MEM		6144	// 192 x 32Byte = 6KByte memory for temporary data

#define FEDM_ISC_MAX_UID_LENGTH			  96

// table types 
// (uiTableID in functions GetTableData, SetTableData, FindTableIndex,
//	GetTableSize, ResetTable)
#define FEDM_ISC_BRM_TABLE			    1
#define FEDM_ISC_ISO_TABLE			    2
#define FEDM_ISC_CAD_TABLE			    3


// access flags for the tables BRM and ISO
// (uiDataID in functions GetTableData, SetTableData, FindTableIndex, ResetTable)
#define FEDM_ISC_DATA_TRTYPE				0x08000001	// BRM and ISO
#define FEDM_ISC_DATA_SNR					0x08000002	// BRM and ISO
#define FEDM_ISC_DATA_SNR_LEN				0x08000003	// BRM and ISO
#define FEDM_ISC_DATA_EPC					0x08000004	// only ISO
#define FEDM_ISC_DATA_EPC_HEADER			0x08000005	// only ISO
#define FEDM_ISC_DATA_EPC_DOMAIN			0x08000006	// only ISO
#define FEDM_ISC_DATA_EPC_OBJECT			0x08000007	// only ISO
#define FEDM_ISC_DATA_EPC_SNR				0x08000008	// only ISO
#define FEDM_ISC_DATA_EPC_TYPE				0x0800000A	// only ISO
#define FEDM_ISC_DATA_AFI					0x0800000B	// BRM and Host Mode
#define FEDM_ISC_DATA_DSFID					0x0800000C	// BRM and Host Mode
#define FEDM_ISC_DATA_MEM_SIZE				0x0800000D	// only ISO
#define FEDM_ISC_DATA_IC_REF				0x0800000E	// only ISO
#define FEDM_ISC_DATA_TRINFO				0x0800000F	// only ISO14443A
#define FEDM_ISC_DATA_OPTINFO				0x08000010	// only ISO14443A
#define FEDM_ISC_DATA_FSCI					0x08000011	// only ISO14443-4
#define FEDM_ISC_DATA_FWI					0x08000012	// only ISO14443-4
#define FEDM_ISC_DATA_DSI					0x08000013	// only ISO14443-4
#define FEDM_ISC_DATA_DRI					0x08000014	// only ISO14443-4
#define FEDM_ISC_DATA_NAD					0x08000015	// only ISO14443-4
#define FEDM_ISC_DATA_CID					0x08000016	// only ISO14443-4
#define FEDM_ISC_DATA_PROTOINFO				0x08000017	// only ISO14443B
#define FEDM_ISC_DATA_EPC_CL1_GEN2_PC		0x08000018	// BRM and ISO
#define FEDM_ISC_DATA_MEM_SIZE_LEN			0x08000019	// only ISO
#define FEDM_ISC_DATA_CHIP_ID				0x08000020	// only ISO
#define FEDM_ISC_DATA_RxDB					0x04000001	// BRM and ISO
#define FEDM_ISC_DATA_TxDB					0x04000002	// only ISO
#define FEDM_ISC_DATA_SEC_STATUS			0x04000003	// only ISO
#define FEDM_ISC_DATA_BLOCK_SIZE			0x04000004	// BRM and ISO
#define FEDM_ISC_DATA_VALUE					0x04000005	// only ISO
#define FEDM_ISC_DATA_RxDB_PLAIN			0x04000006	// only ISO
#define FEDM_ISC_DATA_TxDB_PLAIN			0x04000007	// only ISO
#define FEDM_ISC_DATA_ANT_COUNT				0x04000008	// BRM and Host Mode
#define FEDM_ISC_DATA_ANT_STATUS			0x04000009	// BRM and Host Mode
#define FEDM_ISC_DATA_ANT_RSSI				0x0400000A	// BRM and Host Mode
#define FEDM_ISC_DATA_RxCB					0x02000001	// only ISO
#define FEDM_ISC_DATA_TxCB					0x02000002	// only ISO
#define FEDM_ISC_DATA_TIMER					0x01000001	// only BRM
#define FEDM_ISC_DATA_DB_ADR				0x01000002	// only BRM
#define FEDM_ISC_DATA_DBN					0x01000003	// only BRM
#define FEDM_ISC_DATA_ANT_NR				0x01000004	// only BRM
#define FEDM_ISC_DATA_DATE					0x01000005	// only BRM
#define FEDM_ISC_DATA_INPUT					0x01000006	// only BRM
#define FEDM_ISC_DATA_STATE					0x01000007	// only BRM
#define FEDM_ISC_DATA_MAC_ADR				0x01000008	// only BRM
#define FEDM_ISC_DATA_IS_BLOCK_SIZE_SET		0x80000001	// only ISO
#define FEDM_ISC_DATA_IS_SELECTED			0x80000002	// only ISO
#define FEDM_ISC_DATA_IS_SNR				0x80000003	// BRM and ISO
#define FEDM_ISC_DATA_IS_EPC				0x80000004	// only ISO
#define FEDM_ISC_DATA_IS_AFI				0x80000005	// only ISO
#define FEDM_ISC_DATA_IS_DSFID				0x80000006	// only ISO
#define FEDM_ISC_DATA_IS_TRINFO				0x80000007	// only ISO14443A
#define FEDM_ISC_DATA_IS_SYSINFO			0x80000008	// only ISO
#define FEDM_ISC_DATA_IS_DB					0x80000009	// only BRM
#define FEDM_ISC_DATA_IS_TIMER				0x8000000A	// only BRM
#define FEDM_ISC_DATA_IS_ANT_NR				0x8000000B	// only BRM
#define FEDM_ISC_DATA_IS_DATE				0x8000000C	// only BRM
#define FEDM_ISC_DATA_IS_ISO14443_4_INFO	0x8000000D	// only ISO14443-4
#define FEDM_ISC_DATA_IS_INPUT				0x8000000E	// only BRM
#define FEDM_ISC_DATA_IS_RSSI				0x8000000F	// BRM and ISO
#define FEDM_ISC_DATA_IS_MAC_ADR			0x80000010	// only BRM
#define FEDM_ISC_DATA_ALL					0xFFFF0000	// for initialize all with ResetTable

// only for EPCglobal Class1, Generation2 Transponder
// the user memory bank can be accessed with FEDM_ISC_DATA_RxDB and FEDM_ISC_DATA_TxDB
#define FEDM_ISC_DATA_RxDB_EPC_BANK			0x04001001	// only ISO
#define FEDM_ISC_DATA_TxDB_EPC_BANK			0x04001002	// only ISO
#define FEDM_ISC_DATA_RxDB_TID_BANK			0x04001003	// only ISO
#define FEDM_ISC_DATA_TxDB_TID_BANK			0x04001004	// only ISO
#define FEDM_ISC_DATA_RxDB_RES_BANK			0x04001005	// only ISO
#define FEDM_ISC_DATA_TxDB_RES_BANK			0x04001006	// only ISO


// access flags for the table CAD
// (uiDataID in functions GetTableData, ResetTable)
#define FEDM_ISC_DATA_POWER_AVG				0x00000001	// average power value
#define FEDM_ISC_DATA_POWER_PEAK			0x00000002	// peak power value
#define FEDM_ISC_DATA_HISTORY_ALLOC			0x00000004	// history record: allocation of channels
#define FEDM_ISC_DATA_HISTORY_TX			0x00000008	// history record: transmit channel
#define FEDM_ISC_DATA_HISTORY_RX			0x00000010	// history record: no. of receive channels
#define FEDM_ISC_DATA_MISC					0x00000020	// miscellaneous data

		
// ISO-Command mode flags
#define FEDM_ISC_ISO_MODE_NONADR	    		0x00
#define FEDM_ISC_ISO_MODE_ADR		    		0x01
#define FEDM_ISC_ISO_MODE_SEL		    		0x02
#define FEDM_ISC_ISO_MODE_SEC		    		0x08
#define FEDM_ISC_ISO_MODE_WR_NE		    		0x08	// only for jewel transponder
#define FEDM_ISC_ISO_MODE_RECOM		    		0x08	// only for EPC C1 G2/ISO 18000-3M3 with [0xB3][0x18] Kill
#define FEDM_ISC_ISO_MODE_UID_LF	    		0x10
#define FEDM_ISC_ISO_MODE_ANT					0x10	// only for [0xB0][0x01] Inventory
#define FEDM_ISC_ISO_MODE_CINF					0x20	// only for ISO14443-Transponder and [0xB0][0x25] Select
#define FEDM_ISC_ISO_MODE_EXT_ADR	    		0x20
#define FEDM_ISC_ISO_MODE_PRESC		    		0x20	// only for ISO14443-Transponder and [0xB0][0x01] Inventory
#define FEDM_ISC_ISO_MODE_DRV_SEL	    		0x40	// only for ISO14443 transponder and [0xB0][0x25] Select
#define FEDM_ISC_ISO_MODE_NOTIFY	    		0x40	// only for ISO14443-Transponder and [0xB0][0x01] Inventory
#define FEDM_ISC_ISO_MODE_READ_COMPLETE_BANK	0x40	// only for EPC C1G2 Transponder and [0xB0][0x23] Read Multiple Blocks
#define FEDM_ISC_ISO_MODE_MORE					0x80

		
// ISO-Command bank identifier for:
// [0xB0][0x23] Read Multiple Blocks
// [0xB0][0x24] Write Multiple Blocks
#define FEDM_ISC_ISO_BANK_RESERVED	    0x00
#define FEDM_ISC_ISO_BANK_EPC_MEMORY    0x01
#define FEDM_ISC_ISO_BANK_TID_MEMORY    0x02
#define FEDM_ISC_ISO_BANK_USER_MEMORY   0x03


// Manufacturer Code for [0xB1] ISO-15693 Host Commands
#define FEDM_ISC_ISO_MFR_MOTOROLA			0x01
#define FEDM_ISC_ISO_MFR_STM				0x02
#define FEDM_ISC_ISO_MFR_HITACHI			0x03
#define FEDM_ISC_ISO_MFR_PHILIPS			0x04
#define FEDM_ISC_ISO_MFR_NXP				0x04
#define FEDM_ISC_ISO_MFR_INFINEON			0x05
#define FEDM_ISC_ISO_MFR_CYLINK				0x06
#define FEDM_ISC_ISO_MFR_TI					0x07
#define FEDM_ISC_ISO_MFR_FUJITSU			0x08
#define FEDM_ISC_ISO_MFR_MATSUSHITA			0x09
#define FEDM_ISC_ISO_MFR_NEC				0x0A
#define FEDM_ISC_ISO_MFR_OKI				0x0B
#define FEDM_ISC_ISO_MFR_TOSHIBA			0x0C
#define FEDM_ISC_ISO_MFR_MITSUBISHI			0x0D
#define FEDM_ISC_ISO_MFR_SAMSUNG			0x0E
#define FEDM_ISC_ISO_MFR_HYUNDAI			0x0F
#define FEDM_ISC_ISO_MFR_LG					0x10
#define FEDM_ISC_ISO_MFR_EMOSYN_EM			0x11
#define FEDM_ISC_ISO_MFR_INSIDE				0x12
#define FEDM_ISC_ISO_MFR_ORGA				0x13
#define FEDM_ISC_ISO_MFR_SHARP				0x14
#define FEDM_ISC_ISO_MFR_ATMEL				0x15
#define FEDM_ISC_ISO_MFR_EM					0x16
#define FEDM_ISC_ISO_MFR_KSW				0x17
#define FEDM_ISC_ISO_MFR_ZMD				0x18
#define FEDM_ISC_ISO_MFR_XICOR				0x19
#define FEDM_ISC_ISO_MFR_SONY				0x1A
#define FEDM_ISC_ISO_MFR_MALAYSIA			0x1B
#define FEDM_ISC_ISO_MFR_EMOSYN				0x1C
#define FEDM_ISC_ISO_MFR_SHANGHAI_FUDAN		0x1D
#define FEDM_ISC_ISO_MFR_MAGELLAN			0x1E
#define FEDM_ISC_ISO_MFR_MELEXIS			0x1F
#define FEDM_ISC_ISO_MFR_RENESAS_TECH		0x20
#define FEDM_ISC_ISO_MFR_TAGSYS				0x21
#define FEDM_ISC_ISO_MFR_TRANSCORE			0x22
#define FEDM_ISC_ISO_MFR_SHANGHAI_BELLING	0x23
#define FEDM_ISC_ISO_MFR_MASKTECH			0x24
#define FEDM_ISC_ISO_MFR_INNOVISION			0x25
#define FEDM_ISC_ISO_MFR_HITACHI_ULSI		0x26
#define FEDM_ISC_ISO_MFR_CYPAK				0x27
#define FEDM_ISC_ISO_MFR_RICOH				0x28
#define FEDM_ISC_ISO_MFR_ASK				0x29
#define FEDM_ISC_ISO_MFR_UNICORE			0x2A
#define FEDM_ISC_ISO_MFR_MAXIM				0x2B
#define FEDM_ISC_ISO_MFR_IMPINJ				0x2C
#define FEDM_ISC_ISO_MFR_RIGHTPLUG			0x2D
#define FEDM_ISC_ISO_MFR_BROADCOM			0x2E
#define FEDM_ISC_ISO_MFR_MSTAR				0x2F
#define FEDM_ISC_ISO_MFR_EEDAR				0x30
#define FEDM_ISC_ISO_MFR_RFIDSEC			0x31
#define FEDM_ISC_ISO_MFR_SCHWEIZER			0x32
#define FEDM_ISC_ISO_MFR_AMIC				0x33
#define FEDM_ISC_ISO_MFR_MIKRON_JSC			0x34
#define FEDM_ISC_ISO_MFR_FRAUNHOFER_IPMS	0x35
#define FEDM_ISC_ISO_MFR_IDS				0x36
#define FEDM_ISC_ISO_MFR_KOVIO				0x37
#define FEDM_ISC_ISO_MFR_HMT				0x38
#define FEDM_ISC_ISO_MFR_SILICON_CRAFT		0x39
#define FEDM_ISC_ISO_MFR_ADV_FILM_DEV		0x3A
#define FEDM_ISC_ISO_MFR_NITECREST			0x3B
#define FEDM_ISC_ISO_MFR_VERAYO				0x3C
#define FEDM_ISC_ISO_MFR_HID				0x3D
#define FEDM_ISC_ISO_MFR_PROD_ENG			0x3E
#define FEDM_ISC_ISO_MFR_AUSTRIA_MICRO		0x3F
#define FEDM_ISC_ISO_MFR_GEMALTO			0x40
#define FEDM_ISC_ISO_MFR_RENESAS_ELECTR		0x41
#define FEDM_ISC_ISO_MFR_3ALOGICS			0x42
#define FEDM_ISC_ISO_MFR_TOP_TRONIQ			0x43
#define FEDM_ISC_ISO_MFR_GENTAG				0x44

// HF/UHF function unit types
#define FEDM_ISC_TYPE_ANT_DAT			11	// HF Dynamic Antenna Tuner
#define FEDM_ISC_TYPE_ANT_UMUX			12	// UHF Antenna Multiplexer
#define FEDM_ISC_TYPE_ANT_GPC			13	// Gate People Counter

// LF classic reader types
//#define FEDM_RW_TYPE_RW40_XX			20: // ID RW40.30-U
		
// HF reader types
#define FEDM_ISC_TYPE_ISCM01			30	// listed, but unsupported
#define FEDM_ISC_TYPE_ISCM02			31
#define FEDM_ISC_TYPE_ISCM02_M8			33
#define FEDM_ISC_TYPE_ISCLR100			40	// listed, but unsupported
#define FEDM_ISC_TYPE_ISCLR200			41
#define FEDM_ISC_TYPE_ISCLR2000			42
#define FEDM_ISC_TYPE_ISCLR2500_B		43
#define FEDM_ISC_TYPE_ISCLR2500_A		44
#define FEDM_ISC_TYPE_ISCLR1002			45
#define FEDM_ISC_TYPE_ISCPRH101			60
#define FEDM_ISC_TYPE_ISCPRH101_U		61
#define FEDM_ISC_TYPE_ISCPRHD102		62
#define FEDM_ISC_TYPE_ISCPRH102			63
#define FEDM_ISC_TYPE_ISCPRH200			64
#define FEDM_ISC_TYPE_ISCPRH100_U		71
#define FEDM_ISC_TYPE_ISCPRH100			72
#define FEDM_ISC_TYPE_ISCMR100_U		73
#define FEDM_ISC_TYPE_ISCMR100			74
#define FEDM_ISC_TYPE_ISCPR100			74
#define FEDM_ISC_TYPE_ISCMR200			75
#define FEDM_ISC_TYPE_ISCMR101			76
#define FEDM_ISC_TYPE_ISCMR102			77
#define FEDM_ISC_TYPE_ISCMR101_U		78

// experimental reader, identified by reader class, class generation and reader type
#define FEDM_ISC_TYPE_ISCMR10X			0x00010094

// HF classsic-pro reader types
#define FEDM_ISC_TYPE_CPRM02			80
#define FEDM_ISC_TYPE_CPR02				81
#define FEDM_ISC_TYPE_CPR40_XX_U		82
#define FEDM_ISC_TYPE_CPR40_XX			83
#define FEDM_ISC_TYPE_CPR50_XX			84
#define FEDM_ISC_TYPE_CPR44_XX			85
#define FEDM_ISC_TYPE_CPR30_XX			86
#define FEDM_ISC_TYPE_CPR52_XX			87
#define FEDM_ISC_TYPE_CPR04_U			88
#define FEDM_ISC_TYPE_CPR46_XX			89
#define FEDM_ISC_TYPE_CPR20_XX		   110
#define FEDM_ISC_TYPE_CPR47_XX		   111
#define FEDM_ISC_TYPE_CPR60_XX		   113

// OBID myAXXESS reader types
#define FEDM_ISC_TYPE_MAX50_XX		   100
#define FEDM_ISC_TYPE_MAX_STANDARD	   102
#define FEDM_ISC_TYPE_MAX_COMFORT	   103
#define FEDM_ISC_TYPE_MAX_ONTOP		   112
#define FEDM_ISC_TYPE_MAXU1002		   101

// UHF reader types
#define FEDM_ISC_TYPE_ISCMU02			50
#define FEDM_ISC_TYPE_ISCMU95			51
#define FEDM_ISC_TYPE_ISCMRU102			54
#define FEDM_ISC_TYPE_ISCMRU200			55
#define FEDM_ISC_TYPE_ISCLRU1002		91
#define FEDM_ISC_TYPE_ISCLRU1000		92
#define FEDM_ISC_TYPE_ISCLRU2000		93
#define FEDM_ISC_TYPE_ISCLRU3000		94

// OBID megalock C reader types
#define FEDM_ISC_TYPE_MLC_COM		   120
#define FEDM_ISC_TYPE_MLC_EMPS		   121



// name of readers
#define FEDM_ISC_NAME_ANT_DAT			"ID ANT.DAT"	// HF Dynamic Antenna Tuner
#define FEDM_ISC_NAME_ANT_UMUX			"ID ANT.UMUX"	// UHF Antenna Multiplexer
#define FEDM_ISC_NAME_ANT_GPC			"ID ANT.GPC"	// Gate People Counter
//#define FEDM_ISC_NAME_ISCM01			"ID ISC.M01"	// unsupported
#define FEDM_ISC_NAME_ISCM02			"ID ISC.M02"
#define FEDM_ISC_NAME_ISCM02_M8			"ID ISC.M02.M8"
//#define FEDM_ISC_NAME_ISCLR100		"ID ISC.LR100"	// unsupported
#define FEDM_ISC_NAME_ISCLR200			"ID ISC.LR200"
#define FEDM_ISC_NAME_ISCLR2000			"ID ISC.LR2000"
#define FEDM_ISC_NAME_ISCLRM2000		"ID ISC.LRM2000"
#define FEDM_ISC_NAME_ISCLR2500_A		"ID ISC.LR2500-A"
#define FEDM_ISC_NAME_ISCLR2500_B		"ID ISC.LR2500-B"
#define FEDM_ISC_NAME_ISCLR1002			"ID ISC.LR1002"
#define FEDM_ISC_NAME_ISCPRH101			"ID ISC.PRH101"
#define FEDM_ISC_NAME_ISCPRH101_U		"ID ISC.PRH101-U"
#define FEDM_ISC_NAME_ISCPRH102			"ID ISC.PRH102"
#define FEDM_ISC_NAME_ISCPRH200			"ID ISC.PRH200"
#define FEDM_ISC_NAME_ISCPRHD102		"ID ISC.PRHD102"
#define FEDM_ISC_NAME_ISCPRH100_U		"ID ISC.PRH100-U"
#define FEDM_ISC_NAME_ISCPRH100			"ID ISC.PRH100"
#define FEDM_ISC_NAME_ISCMR100_U		"ID ISC.MR100-U"
#define FEDM_ISC_NAME_ISCMR100			"ID ISC.MR100"
#define FEDM_ISC_NAME_ISCPR100			"ID ISC.PR100"
#define FEDM_ISC_NAME_ISCMR200			"ID ISC.MR200"
#define FEDM_ISC_NAME_ISCMR101			"ID ISC.MR101"
#define FEDM_ISC_NAME_ISCMR101_U		"ID ISC.MR101-U"
#define FEDM_ISC_NAME_ISCMR102			"ID ISC.MR102"

// experimental reader, identified by reader class, class generation and reader type
#define FEDM_ISC_NAME_ISCMR10X			"OBID i-scan HF Mid-Range Type 1"

#define FEDM_ISC_NAME_CPRM02			"ID CPR.M02"
#define FEDM_ISC_NAME_CPR02				"ID CPR.02"
#define FEDM_ISC_NAME_CPR04_U			"ID CPR.04-USB"
#define FEDM_ISC_NAME_CPR20_XX			"ID CPR20.xx"
#define FEDM_ISC_NAME_CPR40_XX_U		"ID CPR40.xx-U"
#define FEDM_ISC_NAME_CPR40_XX			"ID CPR40.xx"
#define FEDM_ISC_NAME_CPR50_XX			"ID CPR50.xx"
#define FEDM_ISC_NAME_CPR30_XX			"ID CPR30.xx"
#define FEDM_ISC_NAME_CPR52_XX			"ID CPR52.xx"
#define FEDM_ISC_NAME_CPR44_XX			"ID CPR44.xx"
#define FEDM_ISC_NAME_CPR46_XX			"ID CPR46.xx"
#define FEDM_ISC_NAME_CPR47_XX			"ID CPR47.xx"
#define FEDM_ISC_NAME_CPR60_XX			"ID CPR60.xx"

#define FEDM_ISC_NAME_MAX50_XX			"ID MAX50.xx"
#define FEDM_ISC_NAME_MAX_STANDARD		"OBID myAXXESS standard"
#define FEDM_ISC_NAME_MAX_COMFORT		"OBID myAXXESS comfort"
#define FEDM_ISC_NAME_MAX_ONTOP			"OBID myAXXESS onTop"
#define FEDM_ISC_NAME_MAXU1002			"ID MAX.U1002"

#define FEDM_ISC_NAME_ISCMU02			"ID ISC.MU02"			// UHF
#define FEDM_ISC_NAME_ISCMU95			"ID ISC.MU95"			// UHF
#define FEDM_ISC_NAME_ISCMRU102			"ID ISC.MRU102"			// UHF
#define FEDM_ISC_NAME_ISCMRU200			"ID ISC.MRU200"			// UHF
#define FEDM_ISC_NAME_ISCLRU1002		"ID ISC.LRU1002"		// UHF
#define FEDM_ISC_NAME_ISCLRU1000		"ID ISC.LRU1000"		// UHF
#define FEDM_ISC_NAME_ISCLRMU1000		"ID ISC.LRMU1000"		// UHF
#define FEDM_ISC_NAME_ISCLRU2000		"ID ISC.LRU2000"		// UHF
#define FEDM_ISC_NAME_ISCLRMU2000		"ID ISC.LRMU2000"		// UHF
#define FEDM_ISC_NAME_ISCLRU3000		"ID ISC.LRU3000"		// UHF
#define FEDM_ISC_NAME_ISCLRU3500		"ID ISC.LRU3500"		// UHF

#define FEDM_ISC_NAME_MLC_COM			"ID MLC.COM"
#define FEDM_ISC_NAME_MLC_EMPS			"ID MLC.EMPS"


// name of readers in UNICODE
#define FEDM_ISC_NAME_ANT_DAT_UC		L"ID ANT.DAT"	// HF Dynamic Antenna Tuner
#define FEDM_ISC_NAME_ANT_UMUX_UC		L"ID ANT.UMUX"	// UHF Antenna Multiplexer
#define FEDM_ISC_NAME_ANT_GPC_UC		L"ID ANT.GPC"	// Gate People Counter
//#define FEDM_ISC_NAME_ISCM01_UC		L"ID ISC.M01"
#define FEDM_ISC_NAME_ISCM02_UC			L"ID ISC.M02"
#define FEDM_ISC_NAME_ISCM02_M8_UC		L"ID ISC.M02.M8"
//#define FEDM_ISC_NAME_ISCLR100_UC		L"ID ISC.LR100"
#define FEDM_ISC_NAME_ISCLR200_UC		L"ID ISC.LR200"
#define FEDM_ISC_NAME_ISCLR2000_UC		L"ID ISC.LR2000"
#define FEDM_ISC_NAME_ISCLRM2000_UC		L"ID ISC.LRM2000"
#define FEDM_ISC_NAME_ISCLR2500_A_UC	L"ID ISC.LR2500-A"
#define FEDM_ISC_NAME_ISCLR2500_B_UC	L"ID ISC.LR2500-B"
#define FEDM_ISC_NAME_ISCLR1002_UC		L"ID ISC.LR1002"
#define FEDM_ISC_NAME_ISCPRH101_UC		L"ID ISC.PRH101"
#define FEDM_ISC_NAME_ISCPRH101_U_UC	L"ID ISC.PRH101-U"
#define FEDM_ISC_NAME_ISCPRH102_UC		L"ID ISC.PRH102"
#define FEDM_ISC_NAME_ISCPRH200_UC		L"ID ISC.PRH200"
#define FEDM_ISC_NAME_ISCPRHD102_UC		L"ID ISC.PRHD102"
#define FEDM_ISC_NAME_ISCPRH100_U_UC	L"ID ISC.PRH100-U"
#define FEDM_ISC_NAME_ISCPRH100_UC		L"ID ISC.PRH100"
#define FEDM_ISC_NAME_ISCMR100_U_UC		L"ID ISC.MR100-U"
#define FEDM_ISC_NAME_ISCMR100_UC		L"ID ISC.MR100"
#define FEDM_ISC_NAME_ISCPR100_UC		L"ID ISC.PR100"
#define FEDM_ISC_NAME_ISCMR200_UC		L"ID ISC.MR200"
#define FEDM_ISC_NAME_ISCMR101_UC		L"ID ISC.MR101"
#define FEDM_ISC_NAME_ISCMR101_U_UC		L"ID ISC.MR101-U"
#define FEDM_ISC_NAME_ISCMR102_UC		L"ID ISC.MR102"

#define FEDM_ISC_NAME_CPRM02_UC			L"ID CPR.M02"
#define FEDM_ISC_NAME_CPR02_UC			L"ID CPR.02"
#define FEDM_ISC_NAME_CPR04_U_UC		L"ID CPR.04-USB"
#define FEDM_ISC_NAME_CPR20_XX_UC		L"ID CPR20.xx"
#define FEDM_ISC_NAME_CPR40_XX_U_UC		L"ID CPR40.xx-U"
#define FEDM_ISC_NAME_CPR40_XX_UC		L"ID CPR40.xx"
#define FEDM_ISC_NAME_CPR50_XX_UC		L"ID CPR50.xx"
#define FEDM_ISC_NAME_CPR30_XX_UC		L"ID CPR30.xx"
#define FEDM_ISC_NAME_CPR52_XX_UC		L"ID CPR52.xx"
#define FEDM_ISC_NAME_CPR44_XX_UC		L"ID CPR44.xx"
#define FEDM_ISC_NAME_CPR46_XX_UC		L"ID CPR46.xx"
#define FEDM_ISC_NAME_CPR47_XX_UC		L"ID CPR47.xx"
#define FEDM_ISC_NAME_CPR60_XX_UC		L"ID CPR60.xx"

#define FEDM_ISC_NAME_MAX50_XX_UC		L"ID MAX50.xx"
#define FEDM_ISC_NAME_MAX_STANDARD_UC	L"OBID myAXXESS standard"
#define FEDM_ISC_NAME_MAX_COMFORT_UC	L"OBID myAXXESS comfort"
#define FEDM_ISC_NAME_MAX_ONTOP_UC		L"OBID myAXXESS onTop"
#define FEDM_ISC_NAME_MAXU1002_UC		L"ID MAX.U1002"

#define FEDM_ISC_NAME_ISCMU02_UC		L"ID ISC.MU02"			// UHF
#define FEDM_ISC_NAME_ISCMU95_UC		L"ID ISC.MU95"			// UHF
#define FEDM_ISC_NAME_ISCMRU102_UC		L"ID ISC.MRU102"		// UHF
#define FEDM_ISC_NAME_ISCMRU200_UC		L"ID ISC.MRU200"		// UHF
#define FEDM_ISC_NAME_ISCLRU1002_UC		L"ID ISC.LRU1002"		// UHF
#define FEDM_ISC_NAME_ISCLRU1000_UC		L"ID ISC.LRU1000"		// UHF
#define FEDM_ISC_NAME_ISCLRMU1000_UC	L"ID ISC.LRMU1000"		// UHF
#define FEDM_ISC_NAME_ISCLRU2000_UC		L"ID ISC.LRU2000"		// UHF
#define FEDM_ISC_NAME_ISCLRMU2000_UC	L"ID ISC.LRMU2000"		// UHF
#define FEDM_ISC_NAME_ISCLRU3000_UC		L"ID ISC.LRU3000"		// UHF

#define FEDM_ISC_NAME_MLC_COM_UC		L"ID MLC.COM"
#define FEDM_ISC_NAME_MLC_EMPS_UC		L"ID MLC.EMPS"



// HF transponder types
#define FEDM_ISC_TR_TYPE_ICODE1				0x00
#define FEDM_ISC_TR_TYPE_TAGIT				0x01
#define FEDM_ISC_TR_TYPE_ISO15693			0x03
#define FEDM_ISC_TR_TYPE_ISO14443A			0x04
#define FEDM_ISC_TR_TYPE_ISO14443B			0x05
#define FEDM_ISC_TR_TYPE_EPC				0x06
#define FEDM_ISC_TR_TYPE_ICODE_UID			0x07
#define FEDM_ISC_TR_TYPE_JEWEL				0x08
#define FEDM_ISC_TR_TYPE_ISO18000_3M3		0x09
#define FEDM_ISC_TR_TYPE_STM_SR176			0x0A
#define FEDM_ISC_TR_TYPE_STM_SRIxx			0x0B
#define FEDM_ISC_TR_TYPE_MCRFxxx			0x0C
#define FEDM_ISC_TR_TYPE_KEYBOARD			0x0E // it's not a transponder, but handles so
#define FEDM_ISC_TR_TYPE_INNOVATRON			0x10
#define FEDM_ISC_TR_TYPE_ASK_CTx			0x11
//#define FEDM_ISC_TR_TYPE_0x64				0x64

// UHF transponder types
#define FEDM_ISC_TR_TYPE_ISO18000_6_A		0x80
#define FEDM_ISC_TR_TYPE_ISO18000_6_B		0x81
#define FEDM_ISC_TR_TYPE_EM4222				0x83
#define FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN2	0x84
#define FEDM_ISC_TR_TYPE_EPC_CLASS0			0x88
#define FEDM_ISC_TR_TYPE_EPC_CLASS1_GEN1	0x89


// HF transponder names
#define FEDM_ISC_TR_NAME_ICODE1				"NXP I-Code1"
#define FEDM_ISC_TR_NAME_TAGIT				"Texas Instruments Tag-it HF"
#define FEDM_ISC_TR_NAME_ISO15693			"ISO 15693"
#define FEDM_ISC_TR_NAME_ISO14443A			"ISO 14443-A"
#define FEDM_ISC_TR_NAME_ISO14443B			"ISO 14443-B"
#define FEDM_ISC_TR_NAME_EPC				"I-CODE EPC"
#define FEDM_ISC_TR_NAME_ICODE_UID			"I-CODE UID"
#define FEDM_ISC_TR_NAME_JEWEL				"Jewel"
#define FEDM_ISC_TR_NAME_ISO18000_3M3		"ISO 18000-3M3"
#define FEDM_ISC_TR_NAME_STM_SR176			"STMicroelectronics SR176" // ISO 14443-B
#define FEDM_ISC_TR_NAME_STM_SRIxx			"STMicroelectronics SRIxx" // ISO 14443-B
#define FEDM_ISC_TR_NAME_MCRFxxx			"Microchip MCRFxxx"
//#define FEDM_ISC_TR_NAME_0x64				"ISO 15693"
#define FEDM_ISC_TR_NAME_INNOVATRON			"Innovatron (ISO 14443B')"
#define FEDM_ISC_TR_NAME_ASK_CTx			"ASK CTx"

// UHF transponder names
#define FEDM_ISC_TR_NAME_ISO18000_6_A		"ISO 18000-6-A"
#define FEDM_ISC_TR_NAME_ISO18000_6_B		"ISO 18000-6-B"
#define FEDM_ISC_TR_NAME_EM4222				"EM4222"
#define FEDM_ISC_TR_NAME_EPC_CLASS0			"EPC Class 0"
#define FEDM_ISC_TR_NAME_EPC_CLASS1_GEN1	"EPC Class 1 Gen 1"
#define FEDM_ISC_TR_NAME_EPC_CLASS1_GEN2	"EPC Class 1 Gen 2"



// EPC types (EPC = Electronic Product Code)
#define FEDM_ISC_EPC_TYPE_1					0x00	// 96 bit EPC
#define FEDM_ISC_EPC_TYPE_2					0x01	// 64 bit EPC Type I
#define FEDM_ISC_EPC_TYPE_3					0x02	// 64 bit EPC Type II
#define FEDM_ISC_EPC_TYPE_4					0x03	// 64 bit EPC Type III


#endif // !defined(_FEDM_ISC_H_INCLUDED_)
