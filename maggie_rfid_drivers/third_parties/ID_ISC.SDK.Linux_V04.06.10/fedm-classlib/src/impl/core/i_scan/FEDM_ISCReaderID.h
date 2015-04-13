/*-------------------------------------------------------
|                                                       |
|                  FEDM_ISCReaderID.h                   |
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
Begin        		:	28.05.2000

Version       		:	04.06.10 / 02.12.2014 / M. Hultsch
						- new constants in block 37, 40 and 46 for [0xB4] EM and NXP Custom Commands

						04.06.01 / 03.03.2014 / M. Hultsch
						- new constants in block 160...169 for receive data in notification mode:
						  FEDM_ISC_TMP_0x6E_NTF_RSP_REC_COUNT
						  FEDM_ISC_TMP_0x6E_NTF_RSP_DATA

						04.03.00 / 20.08.2012 / M. Hultsch
						- new constants for [0x77] command
						- DIRECTION in TR-DATA2

						04.02.04 / 21.06.2012 / M. Hultsch
						- new constants for [0xB3] commands

						04.00.08 / 19.12.2011 / M. Hultsch
						- bugfix in constant: FEDM_ISC_TMP_0xC3_ISO7816_FILE_ID

						04.00.02 / 10.08.2011 / M. Hultsch

Operation Systems	:	independent


This file contains the parameter id for the OBID® i-scan reader and transponder


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h



### construction of IDs

	FF - family type			0..99	(03 for OBID i-scan®)
	MM - memory type			0..99	(see FEDM.h)
	BL - block number			0..99
	BY - 1. byte in block		0..99
	NB - number of bytes		0..99
	BI - 1. bit in byte			0..08
	Nb - number of bits			0..99


### how we build the identifier names

	#define FEDM_ISCM_EE_COM_BUSADR					"03 03 03 00 00 01 00 00"
			 |	  |    |  |
			 |	  |    |  - the rest is a shortening of the parameter
			 |	  |    ---- the parameter is located in memory m_EEData
			 |	  --------- reader type ISCM
			 -------------- name of this component

*/

#if !defined(_FEDM_ISC_READERID_H_INCLUDED_)
#define _FEDM_ISC_READERID_H_INCLUDED_
			
			  


//#############################################################################
// TMPDATA_MEM: parameter id for temporary data (ISC.M01-, ISC.MR/PR/PRH- and ISC.LR-reader)
// in block  0...7 : miscellaneous data
// in block  8..10 : iso-protocol data
//#############################################################################

// block 0												 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_LAST_STATE							"03 10 00 00 01 00 00"	// last status byte of reader
#define FEDM_ISC_TMP_LAST_ERROR							"03 10 00 01 04 00 00"	// last error code
														
// block 1												 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_INP_STATE							"03 10 01 00 01 00 00"	// flag field INPUTS in [0x74]
#define FEDM_ISC_TMP_INP_STATE_IN1						"03 10 01 00 01 00 01"	// flag IN1 in INPUTS
#define FEDM_ISC_TMP_INP_STATE_IN2						"03 10 01 00 01 01 01"	// flag IN2 in INPUTS
#define FEDM_ISC_TMP_INP_STATE_IN3						"03 10 01 00 01 02 01"	// flag IN3 in INPUTS
#define FEDM_ISC_TMP_INP_STATE_IN4						"03 10 01 00 01 03 01"	// flag IN4 in INPUTS
#define FEDM_ISC_TMP_INP_STATE_IN5						"03 10 01 00 01 04 01"	// flag IN5 in INPUTS
#define FEDM_ISC_TMP_INP_STATE_DIP1						"03 10 01 00 01 04 01"	// flag DIP1 in INPUTS
#define FEDM_ISC_TMP_INP_STATE_DIP2						"03 10 01 00 01 05 01"	// flag DIP2 in INPUTS
#define FEDM_ISC_TMP_INP_STATE_DIP3						"03 10 01 00 01 06 01"	// flag DIP3 in INPUTS
#define FEDM_ISC_TMP_INP_STATE_DIP4						"03 10 01 00 01 07 01"	// flag DIP4 in INPUTS
#define FEDM_ISC_TMP_INP_STATE_IN1_CHG					"03 10 01 00 01 04 01"	// flag Input1 changed in INPUTS
#define FEDM_ISC_TMP_INP_STATE_IN2_CHG					"03 10 01 00 01 05 01"	// flag Input2 changed  in INPUTS
#define FEDM_ISC_TMP_INP_STATE_IN3_CHG					"03 10 01 00 01 06 01"	// flag Input3 changed  in INPUTS
#define FEDM_ISC_TMP_INP_STATE_IN4_CHG					"03 10 01 00 01 07 01"	// flag Input4 changed  in INPUTS
#define FEDM_ISC_TMP_RF_ONOFF							"03 10 01 01 01 00 00"	// RF value in [0x6A]
#define FEDM_ISC_TMP_RF_ONOFF_ANT_NR					"03 10 01 01 01 00 04"	// Antenna Number in RF value in [0x6A]
#define FEDM_ISC_TMP_RF_ONOFF_DC						"03 10 01 01 01 04 01"	// Flag 'DC' in RF value in [0x6A]
#define FEDM_ISC_TMP_RF_ONOFF_MAINTAIN_HOST_MODE		"03 10 01 01 01 07 01"	// Flag 'Maintain Host Mode' in RF value in [0x6A]
#define FEDM_ISC_TMP_READ_CFG							"03 10 01 02 01 00 00"	// CFG-ADR value in [0x80]
#define FEDM_ISC_TMP_READ_CFG_ADR						"03 10 01 02 01 00 06"	// sub parameter address in CFG-ADR value in [0x80]
#define FEDM_ISC_TMP_READ_CFG_MODE						"03 10 01 02 01 06 01"	// sub parameter mode (unused, should be always 0)
#define FEDM_ISC_TMP_READ_CFG_LOC						"03 10 01 02 01 07 01"	// sub parameter loc in CFG-ADR value in [0x80]
#define FEDM_ISC_TMP_WRITE_CFG							"03 10 01 03 01 00 00"	// CFG-ADR value in [0x81]
#define FEDM_ISC_TMP_WRITE_CFG_ADR						"03 10 01 03 01 00 06"	// sub parameter address in CFG-ADR value in [0x81]
#define FEDM_ISC_TMP_WRITE_CFG_MODE						"03 10 01 03 01 06 01"	// sub parameter mode (unused, should be always 0)
#define FEDM_ISC_TMP_WRITE_CFG_LOC						"03 10 01 03 01 07 01"	// sub parameter loc in CFG-ADR value in [0x81]
#define FEDM_ISC_TMP_SAVE_CFG							"03 10 01 04 01 00 00"	// CFG-ADR value in [0x82]
#define FEDM_ISC_TMP_SAVE_CFG_ADR						"03 10 01 04 01 00 06"	// sub parameter address in CFG-ADR value in [0x82]
#define FEDM_ISC_TMP_SAVE_CFG_MODE						"03 10 01 04 01 06 01"	// sub parameter mode in CFG-ADR value in [0x82]
#define FEDM_ISC_TMP_SAVE_CFG_LOC						"03 10 01 04 01 07 01"	// sub parameter loc (unused, should be always 0)
#define FEDM_ISC_TMP_RESET_CFG							"03 10 01 05 01 00 00"	// CFG-ADR value in [0x83]
#define FEDM_ISC_TMP_RESET_CFG_ADR						"03 10 01 05 01 00 06"	// sub parameter address in CFG-ADR value in [0x83]
#define FEDM_ISC_TMP_RESET_CFG_MODE						"03 10 01 05 01 06 01"	// sub parameter loc in CFG-ADR value in [0x83]
#define FEDM_ISC_TMP_RESET_CFG_LOC						"03 10 01 05 01 07 01"	// sub parameter mode in CFG-ADR value in [0x83]
#define FEDM_ISCM_TMP_ANTENNA_VALUE						"03 10 01 06 02 00 00"	// ANT_VALUE in [0x75]
// [0x8A] Read Configuration send data
#define FEDM_ISC_TMP_0x8A_READ_DEVICE					"03 10 01 08 01 00 00"	// device value in [0x8A]
#define FEDM_ISC_TMP_0x8A_READ_BANK						"03 10 01 09 01 00 00"	// memory bank value in [0x8A]
#define FEDM_ISC_TMP_0x8A_READ_MODE						"03 10 01 10 01 00 00"	// mode value in [0x8A]
#define FEDM_ISC_TMP_0x8A_READ_MODE_LOC					"03 10 01 10 01 00 01"	// location flag in mode value in [0x8A]
#define FEDM_ISC_TMP_0x8A_READ_MODE_ALL					"03 10 01 10 01 06 01"	// all flag in mode value in [0x8A]
#define FEDM_ISC_TMP_0x8A_READ_REQ_CFG_ADR				"03 10 01 11 02 00 00"	// 2 byte CFG-ADR value in [0x8A] request protocol
#define FEDM_ISC_TMP_0x8A_READ_REQ_CFG_N				"03 10 01 13 01 00 00"	// number of CFGs to be read with [0x8A] request protocol
// [0x8A] Read Configuration response data
#define FEDM_ISC_TMP_0x8A_READ_RSP_CFG_N				"03 10 01 14 01 00 00"	// number of CFGs read with [0x8A] response protocol
#define FEDM_ISC_TMP_0x8A_READ_RSP_CFG_SIZE				"03 10 01 15 01 00 00"	// blocksize of each CFG in [0x8A] response protocol
// [0x8B] Write Configuration send data
#define FEDM_ISC_TMP_0x8B_WRITE_DEVICE					"03 10 01 16 01 00 00"	// device value in [0x8B]
#define FEDM_ISC_TMP_0x8B_WRITE_BANK					"03 10 01 17 01 00 00"	// memory bank value in [0x8B]
#define FEDM_ISC_TMP_0x8B_WRITE_MODE					"03 10 01 18 01 00 00"	// mode value in [0x8B]
#define FEDM_ISC_TMP_0x8B_WRITE_MODE_LOC				"03 10 01 18 01 00 01"	// location flag mode value in [0x8B]
#define FEDM_ISC_TMP_0x8B_WRITE_CFG_N					"03 10 01 19 01 00 00"	// number of CFGs to be read with [0x8B]
#define FEDM_ISC_TMP_0x8B_WRITE_CFG_SIZE				"03 10 01 20 01 00 00"	// blocksize of each CFG in [0x8B]
// [0x8C] Reset Configuration send data
#define FEDM_ISC_TMP_0x8C_RESET_DEVICE					"03 10 01 21 01 00 00"	// device value in [0x8C]
#define FEDM_ISC_TMP_0x8C_RESET_BANK					"03 10 01 22 01 00 00"	// memory bank value in [0x8C]
#define FEDM_ISC_TMP_0x8C_RESET_MODE					"03 10 01 23 01 00 00"	// mode value in [0x8C]
#define FEDM_ISC_TMP_0x8C_RESET_MODE_LOC				"03 10 01 23 01 00 01"	// location flag in mode value in [0x8C]
#define FEDM_ISC_TMP_0x8C_RESET_MODE_ALL				"03 10 01 23 01 06 01"	// all flag in mode value in [0x8C]
#define FEDM_ISC_TMP_0x8C_RESET_CFG_ADR					"03 10 01 24 02 00 00"	// 2 byte CFG-ADR value in [0x8C]
#define FEDM_ISC_TMP_0x8C_RESET_CFG_N					"03 10 01 26 01 00 00"	// number of CFGs to be read with [0x8C]
														
// block 2												 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISCLR_TMP_BRM_SETS							"03 10 02 00 01 00 00"	// DATA-SETS value in [0x21]
#define FEDM_ISCLR_TMP_BRM_TRDATA						"03 10 02 01 01 00 00"	// flag field TR-DATA in [0x21]
#define FEDM_ISCLR_TMP_BRM_TRDATA_SNR					"03 10 02 01 01 00 01"	// flag SNR in TR-DATA in [0x21]
#define FEDM_ISCLR_TMP_BRM_TRDATA_DB					"03 10 02 01 01 01 01"	// flag DB in TR-DATA in [0x21]
#define FEDM_ISCLR_TMP_BRM_TRDATA_ANT					"03 10 02 01 01 04 01"	// flag ANT in TR-DATA in [0x21]
#define FEDM_ISCLR_TMP_BRM_TRDATA_TIME					"03 10 02 01 01 05 01"	// flag TIME in TR-DATA in [0x21]
#define FEDM_ISCLR_TMP_BRM_RECSETS						"03 10 02 02 01 00 00"	// received data sets in [0x21]
#define FEDM_ISCLR_TMP_NOISE_LEVEL						"03 10 02 03 06 00 00"	// NOISE-LEVEL value in [0x6C] and [0x6D]
#define FEDM_ISCLR_TMP_NOISE_LEVEL_MIN					"03 10 02 03 02 00 00"	// sub parameter min in NOISE-LEVEL
#define FEDM_ISCLR_TMP_NOISE_LEVEL_AVG					"03 10 02 05 02 00 00"	// sub parameter avg in NOISE-LEVEL
#define FEDM_ISCLR_TMP_NOISE_LEVEL_MAX					"03 10 02 07 02 00 00"	// sub parameter max in NOISE-LEVEL
#define FEDM_ISCLR_TMP_MEM_ADR							"03 10 02 09 01 00 00"	// MEM-ADR value in [0x84]
#define FEDM_ISCLR_TMP_READER_PW						"03 10 02 10 04 00 00"	// password for [0xA0] reader login
#define FEDM_ISCLR_TMP_READER_PW0						"03 10 02 10 01 00 00"
#define FEDM_ISCLR_TMP_READER_PW1						"03 10 02 11 01 00 00"
#define FEDM_ISCLR_TMP_READER_PW2						"03 10 02 12 01 00 00"
#define FEDM_ISCLR_TMP_READER_PW3						"03 10 02 13 01 00 00"
#define FEDM_ISC_TMP_0x76_MODE							"03 10 02 14 01 00 00"	// mode in [0x76]
#define FEDM_ISC_TMP_ANTENNA_OUT						"03 10 02 15 05 00 00"	// ANT_OUT_xxx in [0x76]
#define FEDM_ISC_TMP_ANTENNA_OUT_INT					"03 10 02 15 01 00 00"	// ANT_OUT_xxx in [0x76]
#define FEDM_ISC_TMP_ANTENNA_OUT_EXT_1					"03 10 02 16 01 00 00"	// ANT_OUT_xxx in [0x76]
#define FEDM_ISC_TMP_ANTENNA_OUT_EXT_2					"03 10 02 17 01 00 00"	// ANT_OUT_xxx in [0x76]
#define FEDM_ISC_TMP_ANTENNA_OUT_EXT_3					"03 10 02 18 01 00 00"	// ANT_OUT_xxx in [0x76]
#define FEDM_ISC_TMP_ANTENNA_OUT_EXT_4					"03 10 02 19 01 00 00"	// ANT_OUT_xxx in [0x76]
														
// block 3												 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISCLR_TMP_TIME								"03 10 03 00 04 00 00"	// TIME value in [0x85] and [0x86]
#define FEDM_ISCLR_TMP_TIME_H							"03 10 03 00 01 00 00"	// sub parameter hour in TIME
#define FEDM_ISCLR_TMP_TIME_M							"03 10 03 01 01 00 00"	// sub parameter minute in TIME
#define FEDM_ISCLR_TMP_TIME_MS							"03 10 03 02 02 00 00"	// sub parameter milli-second in TIME
#define FEDM_ISCLR_TMP_TAB_SIZE							"03 10 03 04 02 00 00"	// TAB-SIZE value in [0x31]
#define FEDM_ISCLR_TMP_TAB_START						"03 10 03 06 02 00 00"	// TAB-START value in [0x31]
#define FEDM_ISCLR_TMP_TAB_LEN							"03 10 03 08 02 00 00"	// TAB-LEN value in [0x31]
#define FEDM_ISC_TMP_SYSTEM_RESET_MODE					"03 10 03 10 01 00 00"	// mode for [0x64] system reset
#define FEDM_ISC_TMP_0x35_MODE							"03 10 03 11 01 00 00"	// MODE parameter in [0x35] Software Trigger
#define FEDM_ISC_TMP_0x35_OPTION						"03 10 03 12 01 00 00"	// OPTION parameter in [0x35] Software Trigger
#define FEDM_ISC_TMP_0x35_CYCLE							"03 10 03 13 01 00 00"	// CYCLE parameter in [0x35] Software Trigger
#define FEDM_ISC_TMP_0x35_CYCLE_TIMEOUT					"03 10 03 14 02 00 00"	// CYCLE-TIMEOUT parameter in [0x35] Software Trigger
#define FEDM_ISC_TMP_0x34_MODE							"03 10 03 16 01 00 00"	// MODE parameter in [0x34] Force Notify Trigger
														
// block 4												 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_OUT_OS								"03 10 04 01 02 00 00"	// OS value in [0x71]
#define FEDM_ISC_TMP_OUT_OSF							"03 10 04 03 02 00 00"	// OSF value in [0x71]
#define FEDM_ISC_TMP_OUT_OSTIME							"03 10 04 05 02 00 00"	// OS-TIME value in [0x71]
#define FEDM_ISC_TMP_OUT_OUTTIME						"03 10 04 07 02 00 00"	// OUT-TIME value in [0x71]
#define FEDM_ISC_TMP_OUT_OS_OUT1						"03 10 04 01 01 00 02"	// sub parameter OUT1 in OS
#define FEDM_ISC_TMP_OUT_OS_OUT2						"03 10 04 01 01 02 02"	// sub parameter OUT2 in OS
#define FEDM_ISC_TMP_OUT_OS_LED_G						"03 10 04 02 01 00 02"	// sub parameter LED GREEN in OS
#define FEDM_ISC_TMP_OUT_OS_LED_R						"03 10 04 02 01 02 02"	// sub parameter LED RED in OS
#define FEDM_ISC_TMP_OUT_OS_BEEPER						"03 10 04 02 01 04 02"	// sub parameter BUZZER in OS
#define FEDM_ISC_TMP_OUT_OSF_OUT1						"03 10 04 03 01 00 02"	// sub parameter OUT1 in OSF
#define FEDM_ISC_TMP_OUT_OSF_OUT2						"03 10 04 03 01 02 02"	// sub parameter OUT2 in OSF
#define FEDM_ISC_TMP_OUT_OSF_LED_G						"03 10 04 04 01 00 02"	// sub parameter LED GREEN in OSF
#define FEDM_ISC_TMP_OUT_OSF_LED_R						"03 10 04 04 01 02 02"	// sub parameter LED RED in OSF
#define FEDM_ISC_TMP_OUT_OSF_BEEPER						"03 10 04 04 01 04 02"	// sub parameter BUZZER in OSF

// old subfields for ISC.M01 (do not use for new projects)
#define FEDM_ISCM_TMP_OUT_OS_OUT1						"03 10 04 01 01 00 02"	// sub parameter OUT1 in OS for ISC.M01
#define FEDM_ISCM_TMP_OUT_OS_OUT2						"03 10 04 01 01 02 02"	// sub parameter OUT2 in OS for ISC.M01
#define FEDM_ISCM_TMP_OUT_OS_LED_G						"03 10 04 02 01 00 02"	// sub parameter LED GREEN in OS for ISC.M01
#define FEDM_ISCM_TMP_OUT_OS_LED_R						"03 10 04 02 01 02 02"	// sub parameter LED RED in OS for ISC.M01
#define FEDM_ISCM_TMP_OUT_OS_BEEPER						"03 10 04 02 01 04 02"	// sub parameter BEEPER in OS for ISC.M01
#define FEDM_ISCM_TMP_OUT_OSF_OUT1						"03 10 04 03 01 00 02"	// sub parameter OUT1 in OSF for ISC.M01
#define FEDM_ISCM_TMP_OUT_OSF_OUT2						"03 10 04 03 01 02 02"	// sub parameter OUT2 in OSF for ISC.M01
#define FEDM_ISCM_TMP_OUT_OSF_LED_G						"03 10 04 04 01 00 02"	// sub parameter LED GREEN in OSF for ISC.M01
#define FEDM_ISCM_TMP_OUT_OSF_LED_R						"03 10 04 04 01 02 02"	// sub parameter LED RED in OSF for ISC.M01
#define FEDM_ISCM_TMP_OUT_OSF_BEEPER					"03 10 04 04 01 04 02"	// sub parameter BEEPER in OSF for ISC.M01

// old subfields for ISC.MR100/PR100/PRH100 (do not use for new projects)
#define FEDM_ISCPRH_TMP_OUT_OS_LED_G					"03 10 04 02 01 00 02"	// sub parameter LED GREEN in OS for ISC.MR/PR/PRH
#define FEDM_ISCPRH_TMP_OUT_OS_LED_R					"03 10 04 02 01 02 02"	// sub parameter LED RED in OS for ISC.MR/PR/PRH
#define FEDM_ISCPRH_TMP_OUT_OS_BEEPER					"03 10 04 02 01 04 02"	// sub parameter BEEPER in OS for ISC.MR/PR/PRH
#define FEDM_ISCPRH_TMP_OUT_OSF_LED_G					"03 10 04 04 01 00 02"	// sub parameter LED GREEN in OSF for ISC.MR/PR/PRH
#define FEDM_ISCPRH_TMP_OUT_OSF_LED_R					"03 10 04 04 01 02 02"	// sub parameter LED RED in OSF for ISC.MR/PR/PRH
#define FEDM_ISCPRH_TMP_OUT_OSF_BEEPER					"03 10 04 04 01 04 02"	// sub parameter BEEPER in OSF for ISC.MR/PR/PRH

// subfields for ISC.LR200
#define FEDM_ISCLR_TMP_OUT_OS_OUT1						"03 10 04 01 01 02 02"	// sub parameter OUT1 MODE in OS for ISC.LR
#define FEDM_ISCLR_TMP_OUT_OS_OUT2						"03 10 04 01 01 04 02"	// sub parameter OUT2 MODE in OS for ISC.LR
#define FEDM_ISCLR_TMP_OUT_OS_REL						"03 10 04 01 01 06 02"	// sub parameter REL MODE in OS for ISC.LR
#define FEDM_ISCLR_TMP_OUT_OSF_OUT1						"03 10 04 03 01 02 02"	// sub parameter OUT1 FRQ in OSF for ISC.LR
#define FEDM_ISCLR_TMP_OUT_OSF_OUT2						"03 10 04 03 01 04 02"	// sub parameter OUT2 FRQ in OSF for ISC.LR
#define FEDM_ISCLR_TMP_OUT_OSF_REL						"03 10 04 03 01 06 02"	// sub parameter REL FRQ in OSF for ISC.LR
														
// block 5												 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_SOFTVER							"03 10 05 00 16 00 00"	// readers software version from [0x65]
#define FEDM_ISC_TMP_SOFTVER_SW_REV						"03 10 05 00 02 00 00"	// SW-REV value in [0x65]
#define FEDM_ISC_TMP_SOFTVER_SW_REV_HIGH				"03 10 05 00 01 00 00"	// SW-REV main revision number in [0x65]
#define FEDM_ISC_TMP_SOFTVER_SW_REV_LOW					"03 10 05 01 01 00 00"	// SW-REV sub revision number in [0x65]
#define FEDM_ISC_TMP_SOFTVER_D_REV						"03 10 05 02 01 00 00"	// D-REV value in [0x65]
#define FEDM_ISC_TMP_SOFTVER_HW_TYPE					"03 10 05 03 01 00 00"	// HW-TYPE value in [0x65]
#define FEDM_ISC_TMP_SOFTVER_SW_TYPE					"03 10 05 04 01 00 00"	// SW-TYPE value in [0x65]: reader type, if Bit 7 = 0 or reader class, if Bit 7 = 1
#define FEDM_ISC_TMP_SOFTVER_TR_TYPE					"03 10 05 05 02 00 00"	// TR-TYPE value in [0x65]
#define FEDM_ISC_TMP_SOFTVER_RX_BUF						"03 10 05 07 02 00 00"	// RX-BUF value in [0x65]
#define FEDM_ISC_TMP_SOFTVER_TX_BUF						"03 10 05 09 02 00 00"	// TX-BUF value in [0x65]
#define FEDM_ISC_TMP_SOFTVER_TR_TYPE_UHF				"03 10 05 11 02 00 00"	// UHF TR-TYPE value in [0x65]
#define FEDM_ISC_TMP_SOFTVER_READER_CLASS_GEN			"03 10 05 13 01 00 00"	// reader class generation value in [0x66], Mode 0
#define FEDM_ISC_TMP_SOFTVER_READER_TYPE_OF_CLASS		"03 10 05 14 01 00 00"	// reader type of reader class value in [0x66], Mode 0
														
// block 6												 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_ISO14443A_KEY_TYPE					"03 10 06 00 01 00 00"
#define FEDM_ISC_TMP_ISO14443A_KEY_ADR					"03 10 06 01 01 00 00"
#define FEDM_ISC_TMP_ISO14443A_KEY						"03 10 06 02 06 00 00"
#define FEDM_ISC_TMP_ISO14443A_KEY0						"03 10 06 02 01 00 00"
#define FEDM_ISC_TMP_ISO14443A_KEY1						"03 10 06 03 01 00 00"
#define FEDM_ISC_TMP_ISO14443A_KEY2						"03 10 06 04 01 00 00"
#define FEDM_ISC_TMP_ISO14443A_KEY3						"03 10 06 05 01 00 00"
#define FEDM_ISC_TMP_ISO14443A_KEY4						"03 10 06 06 01 00 00"
#define FEDM_ISC_TMP_ISO14443A_KEY5						"03 10 06 07 01 00 00"
														
// block 7												 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_DIAG_MODE							"03 10 07 00 01 00 00"	// mode for [0x6E] reader diagnostic
#define FEDM_ISC_TMP_DIAG_DATA							"03 10 07 01 31 00 00"	// receive data with [0x6E]
														
// block 8: [0xB0] send data							 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_B0_CMD								"03 10 08 00 01 00 00"
#define FEDM_ISC_TMP_B0_MODE							"03 10 08 01 01 00 00"
#define FEDM_ISC_TMP_B0_MODE_MORE						"03 10 08 01 01 07 01"
#define FEDM_ISC_TMP_B0_MODE_NOTIFY						"03 10 08 01 01 06 01" // only for ISO14443 transponder and [0xB0][0x01] Inventory
#define FEDM_ISC_TMP_B0_MODE_DRV_SEL					"03 10 08 01 01 06 01" // only for ISO14443 transponder and [0xB0][0x25] Select
#define FEDM_ISC_TMP_B0_MODE_READ_COMPLETE_BANK			"03 10 08 01 01 06 01" // only for EPC C1G2 Transponder and [0xB0][0x23] Read Multiple Blocks
#define FEDM_ISC_TMP_B0_MODE_EXT_ADR					"03 10 08 01 01 05 01"
#define FEDM_ISC_TMP_B0_MODE_CINF						"03 10 08 01 01 05 01" // only for ISO14443 transponder and [0xB0][0x25] Select
#define FEDM_ISC_TMP_B0_MODE_PRESC						"03 10 08 01 01 05 01" // only for ISO14443 transponder and [0xB0][0x01] Inventory
#define FEDM_ISC_TMP_B0_MODE_UID_LF						"03 10 08 01 01 04 01"
#define FEDM_ISC_TMP_B0_MODE_ANT						"03 10 08 01 01 04 01" // only for ISC.M02.M8
#define FEDM_ISC_TMP_B0_MODE_SEC						"03 10 08 01 01 03 01"
#define FEDM_ISC_TMP_B0_MODE_WR_NE						"03 10 08 01 01 03 01" // only for jewel transponder
#define FEDM_ISC_TMP_B0_MODE_ADR						"03 10 08 01 01 00 03"
#define FEDM_ISC_TMP_B0_NOTIFY_TIMEOUT					"03 10 08 16 01 00 00"
														
// block 9: [0xB0] send data							 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_B0_REQ_UID_LEN						"03 10 09 00 01 00 00"
#define FEDM_ISC_TMP_B0_REQ_BANK						"03 10 09 01 01 00 00"
#define FEDM_ISC_TMP_B0_REQ_BANK_ACCESS_FLAG			"03 10 09 01 01 07 01"
#define FEDM_ISC_TMP_B0_REQ_BANK_BANK_NR				"03 10 09 01 01 00 02"
#define FEDM_ISC_TMP_B0_REQ_DB_ADR_EXT					"03 10 09 02 02 00 00"
#define FEDM_ISC_TMP_B0_REQ_DB_ADR						"03 10 09 03 01 00 00"
#define FEDM_ISC_TMP_B0_REQ_DBN							"03 10 09 04 01 00 00"
#define FEDM_ISC_TMP_B0_REQ_DB_SIZE						"03 10 09 05 01 00 00"
#define FEDM_ISC_TMP_B0_REQ_CB_ADR						"03 10 09 06 01 00 00"
#define FEDM_ISC_TMP_B0_REQ_ANT_SEL						"03 10 09 07 01 00 00"
#define FEDM_ISC_TMP_B0_REQ_TR_DRIVER					"03 10 09 08 01 00 00" // only for ISO14443 transponder and [0xB0][0x25] Select
#define FEDM_ISC_TMP_B0_REQ_AFI							"03 10 09 09 01 00 00"
#define FEDM_ISC_TMP_B0_REQ_DSFID						"03 10 09 10 01 00 00"

// block 10: [0xB0] receive data						 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_B0_RSP_DATASETS					"03 10 10 01 01 00 00"
#define FEDM_ISC_TMP_B0_RSP_DBN							"03 10 10 02 01 00 00"
#define FEDM_ISC_TMP_B0_RSP_DB_SIZE						"03 10 10 03 01 00 00"
#define FEDM_ISC_TMP_B0_RSP_DB_EXT_ADR_E				"03 10 10 04 02 00 00"
#define FEDM_ISC_TMP_B0_RSP_DB_ADR_E					"03 10 10 05 01 00 00"
#define FEDM_ISC_TMP_B0_RSP_FORMAT						"03 10 10 06 01 00 00" // only for ISO14443 transponder and [0xB0][0x25] Select
														
// block 11: [0xB1] send data							 FF-MM-BN-BY-NB-BI-Nb									
#define FEDM_ISC_TMP_B1_CMD								"03 10 11 00 01 00 00"
#define FEDM_ISC_TMP_B1_MODE							"03 10 11 01 01 00 00"
#define FEDM_ISC_TMP_B1_MODE_ADR						"03 10 11 01 01 00 03"
#define FEDM_ISC_TMP_B1_MFR								"03 10 11 02 01 00 00"
#define FEDM_ISC_TMP_B1_REQ_DB_ADR						"03 10 11 04 01 00 00"
#define FEDM_ISC_TMP_B1_REQ_DBN							"03 10 11 05 01 00 00"
#define FEDM_ISC_TMP_B1_REQ_UID							"03 10 11 06 08 00 00"
#define FEDM_ISC_TMP_B1_REQ_TI_DB_ADR					"03 10 11 14 01 00 00" // for TI
#define FEDM_ISC_TMP_B1_REQ_TI_DB						"03 10 11 15 04 00 00" // for TI
//received [0xBx] ISO Error
#define FEDM_ISC_TMP_B0_ISO_ERROR						"03 10 11 25 01 00 00"	// last 0xB0 iso-error code
#define FEDM_ISC_TMP_B1_ISO_ERROR						"03 10 11 26 01 00 00"	// last 0xB1 iso-error code
#define FEDM_ISC_TMP_B2_ISO_ERROR						"03 10 11 27 01 00 00"	// last 0xB2 iso-error code
#define FEDM_ISC_TMP_B3_TAG_ERROR						"03 10 11 28 01 00 00"	// last 0xB3 tag-error code
#define FEDM_ISC_TMP_B4_ISO_ERROR						"03 10 11 29 01 00 00"	// last 0xB4 iso-error code
													
// block 12: [0xB1] send data							 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_B1_REQ_TI_PASSWORD					"03 10 12 00 04 00 00" // for TI
#define FEDM_ISC_TMP_B1_REQ_NXP_PAGE_NO					"03 10 12 04 01 00 00" // for NXP
#define FEDM_ISC_TMP_B1_REQ_NXP_PROTECTION_STATUS		"03 10 12 05 01 00 00" // for NXP
#define FEDM_ISC_TMP_B1_REQ_NXP_64BIT_PWD_PARAMETER		"03 10 12 06 01 00 00" // for NXP
#define FEDM_ISC_TMP_B1_REQ_NXP_EAS_ID					"03 10 12 07 02 00 00" // for NXP
#define FEDM_ISC_TMP_B1_REQ_NXP_PASSWORD_ID				"03 10 12 09 01 00 00" // for NXP
#define FEDM_ISC_TMP_B1_REQ_NXP_PASSWORD				"03 10 12 10 04 00 00" // for NXP
#define FEDM_ISC_TMP_B1_REQ_STM_PASSWORD_NO				"03 10 12 14 01 00 00" // for STM
#define FEDM_ISC_TMP_B1_REQ_STM_PASSWORD				"03 10 12 15 04 00 00" // for STM
#define FEDM_ISC_TMP_B1_REQ_STM_KILL_CODE				"03 10 12 15 04 00 00" // for STM
#define FEDM_ISC_TMP_B1_REQ_STM_INFO_BYTE				"03 10 12 19 01 00 00" // for STM
#define FEDM_ISC_TMP_B1_REQ_STM_BLOCK_NO				"03 10 12 20 01 00 00" // for STM
#define FEDM_ISC_TMP_B1_REQ_STM_PROTECT_STATUS			"03 10 12 21 01 00 00" // for STM
#define FEDM_ISC_TMP_B1_REQ_EM_PASSWORD					"03 10 12 22 04 00 00" // for EM
#define FEDM_ISC_TMP_B1_REQ_NXP_DB_ADR					"03 10 12 26 01 00 00" // for NXP
#define FEDM_ISC_TMP_B1_REQ_NXP_DB_N					"03 10 12 27 01 00 00" // for NXP
#define FEDM_ISC_TMP_B1_REQ_INFINEON_DB_ADR				"03 10 12 28 01 00 00" // for INFINEON
#define FEDM_ISC_TMP_B1_REQ_INFINEON_DB_N				"03 10 12 29 01 00 00" // for INFINEON
														
// block 13: [0xB1] send data							 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_B1_REQ_INFINEON_DB_SIZE			"03 10 13 00 01 00 00" // for INFINEON
#define FEDM_ISC_TMP_B1_REQ_INFINEON_AQM				"03 10 13 01 01 00 00" // for INFINEON
#define FEDM_ISC_TMP_B1_REQ_TI_DB_SIZE					"03 10 13 02 01 00 00" // for TI
#define FEDM_ISC_TMP_B1_REQ_TI_EVEN_DB					"03 10 13 03 04 00 00" // for TI
#define FEDM_ISC_TMP_B1_REQ_TI_ODD_DB					"03 10 13 07 04 00 00" // for TI
#define FEDM_ISC_TMP_B1_REQ_FUJITSU_DATA				"03 10 13 11 01 00 00" // for Fujitsu
#define FEDM_ISC_TMP_B1_REQ_KSW_PASSWORD				"03 10 13 12 06 00 00" // for KSW
#define FEDM_ISC_TMP_B1_REQ_KSW_LOGFLAGS				"03 10 13 18 01 00 00" // for KSW
#define FEDM_ISC_TMP_B1_REQ_KSW_LOGPOINTER				"03 10 13 19 01 00 00" // for KSW
#define FEDM_ISC_TMP_B1_REQ_KSW_LOGLIMITLO				"03 10 13 20 02 00 00" // for KSW
#define FEDM_ISC_TMP_B1_REQ_KSW_LOGLIMITHI				"03 10 13 22 02 00 00" // for KSW
#define FEDM_ISC_TMP_B1_REQ_KSW_LOGPERIODE				"03 10 13 24 02 00 00" // for KSW
#define FEDM_ISC_TMP_B1_REQ_KSW_TIMERTICKS				"03 10 13 26 02 00 00" // for KSW
#define FEDM_ISC_TMP_B1_REQ_STM_DATA					"03 10 13 28 01 00 00" // for STM
#define FEDM_ISC_TMP_B1_RSP_STM_DATA					"03 10 13 29 01 00 00" // for STM

// block 14: [0xB2] send data							 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_B2_CMD								"03 10 14 00 01 00 00"
#define FEDM_ISC_TMP_B2_MODE							"03 10 14 01 01 00 00"
#define FEDM_ISC_TMP_B2_MODE_ADR						"03 10 14 01 01 00 03"
#define FEDM_ISC_TMP_B2_MODE_KL							"03 10 14 01 01 03 01"

// block 15: [0xB2] send data							 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_B2_REQ_UID							"03 10 15 02 08 00 00"
#define FEDM_ISC_TMP_B2_REQ_DB_ADR						"03 10 15 10 01 00 00"
#define FEDM_ISC_TMP_B2_REQ_KEY_ADR						"03 10 15 11 01 00 00"
#define FEDM_ISC_TMP_B2_REQ_KEY_TYPE					"03 10 15 12 01 00 00"
#define FEDM_ISC_TMP_B2_REQ_DEST_ADR					"03 10 15 13 01 00 00"
#define FEDM_ISC_TMP_B2_REQ_MF_CMD						"03 10 15 14 01 00 00"
#define FEDM_ISC_TMP_B2_REQ_OP_VALUE					"03 10 15 15 04 00 00"
#define FEDM_ISC_TMP_B2_REQ_KEY_INDEX					"03 10 15 19 01 00 00"
														
// block 16: [0xB2] send data							 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_B2_REQ_KEY_ADR_TAG					"03 10 16 00 01 00 00"
#define FEDM_ISC_TMP_B2_REQ_KEY_ADR_SAM					"03 10 16 01 01 00 00"
#define FEDM_ISC_TMP_B2_REQ_AUTH_COUNTER_ADR			"03 10 16 02 01 00 00"
#define FEDM_ISC_TMP_B2_REQ_KEY_AUTH_SEQUENCE			"03 10 16 03 01 00 00"
														
// block 17: [0xB1] receive data						 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_B1_RSP_DBN							"03 10 17 00 01 00 00"
#define FEDM_ISC_TMP_B1_RSP_NXP_RANDOM_NUMBER			"03 10 17 01 02 00 00"
#define FEDM_ISC_TMP_B1_RSP_NXP_EPC_DATA				"03 10 17 03 12 00 00"
#define FEDM_ISC_TMP_B1_RSP_EM_EAS_BIT					"03 10 17 15 01 00 00"
#define FEDM_ISC_TMP_B1_RSP_INFINEON_DB_ADR_E			"03 10 17 16 01 00 00"
#define FEDM_ISC_TMP_B1_RSP_TI_DB_ADR_E					"03 10 17 16 01 00 00"
#define FEDM_ISC_TMP_B1_RSP_KSW_LOGFLAGS				"03 10 17 17 01 00 00"
#define FEDM_ISC_TMP_B1_RSP_KSW_LOGPOINTER				"03 10 17 18 01 00 00"
#define FEDM_ISC_TMP_B1_RSP_KSW_LOGLIMITLO				"03 10 17 19 02 00 00"
#define FEDM_ISC_TMP_B1_RSP_KSW_LOGLIMITHI				"03 10 17 21 02 00 00"
#define FEDM_ISC_TMP_B1_RSP_KSW_LOGPERIODE				"03 10 17 23 02 00 00"
#define FEDM_ISC_TMP_B1_RSP_KSW_TIMERTICKS				"03 10 17 25 02 00 00"
#define FEDM_ISC_TMP_B1_RSP_DB_ADR_E					"03 10 17 27 01 00 00"
#define FEDM_ISC_TMP_B1_RSP_INFINEON_DB_N				"03 10 17 28 01 00 00" // for INFINEON
#define FEDM_ISC_TMP_B1_RSP_INFINEON_DB_SIZE			"03 10 17 29 01 00 00" // for INFINEON

// block 18: Advanced Buffered Read Mode [0x22]			 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_ADV_BRM_SETS						"03 10 18 00 02 00 00"	// DATA-SETS value in [0x22]
#define FEDM_ISC_TMP_ADV_BRM_TRDATA1					"03 10 18 02 01 00 00"	// flag field TR-DATA1 in [0x22]
#define FEDM_ISC_TMP_ADV_BRM_TRDATA1_SNR				"03 10 18 02 01 00 01"	// flag SNR in TR-DATA1 in [0x22]
#define FEDM_ISC_TMP_ADV_BRM_TRDATA1_DB					"03 10 18 02 01 01 01"	// flag DB in TR-DATA1 in [0x22]
#define FEDM_ISC_TMP_ADV_BRM_TRDATA1_EAS				"03 10 18 02 01 02 01"	// flag EAS in TR-DATA1 in [0x22]
#define FEDM_ISC_TMP_ADV_BRM_TRDATA1_DB_MSB				"03 10 18 02 01 03 01"	// flag Byte Order in TR-DATA1 in [0x22]
#define FEDM_ISC_TMP_ADV_BRM_TRDATA1_ANT				"03 10 18 02 01 04 01"	// flag ANT in TR-DATA1 in [0x22]
#define FEDM_ISC_TMP_ADV_BRM_TRDATA1_TIME				"03 10 18 02 01 05 01"	// flag TIME in TR-DATA1 in [0x22]
#define FEDM_ISC_TMP_ADV_BRM_TRDATA1_DATE				"03 10 18 02 01 06 01"	// flag DATE in TR-DATA1 in [0x22]
#define FEDM_ISC_TMP_ADV_BRM_TRDATA1_EXT				"03 10 18 02 01 07 01"	// flag EXTENSION in TR-DATA1 in [0x22]
#define FEDM_ISC_TMP_ADV_BRM_TRDATA2					"03 10 18 03 01 00 00"	// flag field TR-DATA2 in [0x22]
#define FEDM_ISC_TMP_ADV_BRM_TRDATA2_IN					"03 10 18 03 01 00 01"	// flag IN in TR-DATA2 in [0x22]
#define FEDM_ISC_TMP_ADV_BRM_TRDATA2_MAC_ADR			"03 10 18 03 01 01 01"	// flag MAC in TR-DATA2 in [0x22]
#define FEDM_ISC_TMP_ADV_BRM_TRDATA2_AFI				"03 10 18 03 01 02 01"	// flag AFI/DSFID in TR-DATA2 in [0x22]
#define FEDM_ISC_TMP_ADV_BRM_TRDATA2_ANT_EXT			"03 10 18 03 01 04 01"	// flag ANT_EXT in TR-DATA2 in [0x22]
#define FEDM_ISC_TMP_ADV_BRM_TRDATA2_DIR				"03 10 18 03 01 05 01"	// flag DIRECTION in TR-DATA2 in [0x22]
#define FEDM_ISC_TMP_ADV_BRM_TRDATA2_EXT				"03 10 18 03 01 07 01"	// flag EXTENSION in TR-DATA2 in [0x22]
#define FEDM_ISC_TMP_ADV_BRM_RECSETS					"03 10 18 05 02 00 00"	// received data sets in [0x22]

// block 19: [0x68] Channel Analyze	(send data)			 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_0x68_CA_MODE						"03 10 19 00 01 00 00"	// MODE value in [0x68]
#define FEDM_ISC_TMP_0x68_CA_MODE_CH_HISTORY			"03 10 19 00 01 02 01"	// flag CH_HISTORY in MODE byte in [0x68]
#define FEDM_ISC_TMP_0x68_CA_MODE_CH_POWER				"03 10 19 00 01 03 01"	// flag CH_POWER in MODE byte in [0x68]
#define FEDM_ISC_TMP_0x68_CA_MODE_MORE					"03 10 19 00 01 07 01"	// flag MORE in MODE byte in [0x68]
#define FEDM_ISC_TMP_0x68_CA_RES1						"03 10 19 01 01 00 00"	// reserved
#define FEDM_ISC_TMP_0x68_CA_RES2						"03 10 19 02 01 00 00"	// reserved
#define FEDM_ISC_TMP_0x68_CA_RES3						"03 10 19 03 01 00 00"	// reserved
#define FEDM_ISC_TMP_0x68_CA_RES4						"03 10 19 04 02 00 00"	// reserved
// [0x6B] Centralized RF Synchronization (send data)
#define FEDM_ISC_TMP_0x6B_MODE							"03 10 19 16 01 00 00"	// MODE value in [0x6B]
#define FEDM_ISC_TMP_0x6B_RES1							"03 10 19 17 01 00 00"	// reserved
#define FEDM_ISC_TMP_0x6B_RES2							"03 10 19 18 01 00 00"	// reserved
#define FEDM_ISC_TMP_0x6B_TX_CHANNEL					"03 10 19 19 01 00 00"	// RF transmit channel
#define FEDM_ISC_TMP_0x6B_TX_PERIOD						"03 10 19 20 02 00 00"	// RF transmit period

// block 20: [0x66] Reader Info							 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_READER_INFO_MODE					"03 10 20 00 01 00 00"	// mode value in [0x66]
#define FEDM_ISC_TMP_READER_INFO						"03 10 20 01 31 00 00"	// reader info in [0x66]
#define FEDM_ISC_TMP_READER_INFO_SW_MAJOR				"03 10 20 01 01 00 00"	// SW-MAJOR main version number in [0x66]
#define FEDM_ISC_TMP_READER_INFO_SW_MINOR				"03 10 20 02 01 00 00"	// SW-MINOR sub version number in [0x66]
#define FEDM_ISC_TMP_READER_INFO_SW_DEV					"03 10 20 03 01 00 00"	// SW-DEV developer number in [0x66]
#define FEDM_ISC_TMP_READER_INFO_HW_TYPE				"03 10 20 04 01 00 00"	// HW-TYPE in [0x66]
#define FEDM_ISC_TMP_READER_INFO_SW_MAJOR_EMB_APP		"03 10 20 05 01 00 00"	// SW-MAJOR main version number for 2nd embedded app in [0x66], mode 0x01
#define FEDM_ISC_TMP_READER_INFO_SW_MINOR_EMB_APP		"03 10 20 06 01 00 00"	// SW-MINOR sub version number for 2nd embedded app in [0x66], mode 0x01
#define FEDM_ISC_TMP_READER_INFO_SW_DEV_EMB_APP			"03 10 20 07 01 00 00"	// SW-DEV developer number for 2nd embedded app in [0x66], mode 0x01

// block 21: [0xA3] Write DES/AES Reader Key			 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_0xA3_MODE							"03 10 21 00 01 00 00"
#define FEDM_ISC_TMP_0xA3_MODE_LOC						"03 10 21 00 01 00 01"
#define FEDM_ISC_TMP_0xA3_KEY_INDEX						"03 10 21 01 01 00 00"
#define FEDM_ISC_TMP_0xA3_AUTHENTICATE_MODE				"03 10 21 02 01 00 00"
#define FEDM_ISC_TMP_0xA3_KEY_LEN						"03 10 21 03 01 00 00"
#define FEDM_ISC_TMP_0xA3_KEY							"03 10 21 04 24 00 00"

// block 22: [0x87], [0x88] Set/Get System Date			 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_DATE_CENTURY						"03 10 22 00 01 00 00"	// CENTURY value in [0x87] and [0x88]
#define FEDM_ISC_TMP_DATE_YEAR							"03 10 22 01 01 00 00"	// YEAR value in [0x87] and [0x88]
#define FEDM_ISC_TMP_DATE_MONTH							"03 10 22 02 01 00 00"	// MONTH value in [0x87] and [0x88]
#define FEDM_ISC_TMP_DATE_DAY							"03 10 22 03 01 00 00"	// DAY value in [0x87] and [0x88]
#define FEDM_ISC_TMP_DATE_TIMEZONE						"03 10 22 04 01 00 00"	// TIMEZONE value in [0x87] and [0x88]
#define FEDM_ISC_TMP_DATE_HOUR							"03 10 22 05 01 00 00"	// HOUR value in [0x87] and [0x88]
#define FEDM_ISC_TMP_DATE_MINUTE						"03 10 22 06 01 00 00"	// MINUTE value in [0x87] and [0x88]
#define FEDM_ISC_TMP_DATE_MILLISECOND					"03 10 22 07 02 00 00"	// MILLISECOND value in [0x87] and [0x88]
														
// block 23: [0x6E] send data							 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_DIAG_REQ_DATA						"03 10 23 00 01 00 00"	// data value in [0x6E]
														
// block 24: [0xB0] send data							 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_B0_ACCESS_PW_LENGTH				"03 10 24 00 01 00 00"
#define FEDM_ISC_TMP_B0_ACCESS_PW						"03 10 24 01 31 00 00"
														
// block 25: [0x18] Destroy								 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_EPC_DESTROY_MODE					"03 10 25 00 01 00 00"	// Mode in [0x18]
#define FEDM_ISC_TMP_EPC_DESTROY_PASSWORD				"03 10 25 02 03 00 00"	// Password in [0x18]
														
// block 26: [0x18] Destroy								 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_DESTROY_EPC						"03 10 26 00 32 00 00"	// EPC in [0x18]
														
// block 27: [0xB3] send data							 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_B3_CMD								"03 10 27 00 01 00 00"
#define FEDM_ISC_TMP_B3_MODE							"03 10 27 01 01 00 00"
#define FEDM_ISC_TMP_B3_MODE_ADR						"03 10 27 01 01 00 03"
#define FEDM_ISC_TMP_B3_MODE_RECOM						"03 10 27 01 01 03 01"
#define FEDM_ISC_TMP_B3_MODE_EPC_LF						"03 10 27 01 01 04 01"
#define FEDM_ISC_TMP_B3_MODE_EXT_ADR					"03 10 27 01 01 05 01"
														
// block 28: [0xB3] send data							 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_B3_REQ_EPC_LEN						"03 10 28 00 01 00 00"
#define FEDM_ISC_TMP_B3_REQ_BANK						"03 10 28 01 01 00 00"
#define FEDM_ISC_TMP_B3_REQ_BANK_ACCESS_FLAG			"03 10 28 01 01 07 01"
#define FEDM_ISC_TMP_B3_REQ_BANK_BANK_NR				"03 10 28 01 01 00 02"
#define FEDM_ISC_TMP_B3_REQ_DB_ADR_EXT					"03 10 28 02 02 00 00"
#define FEDM_ISC_TMP_B3_REQ_DB_ADR						"03 10 28 03 01 00 00"
#define FEDM_ISC_TMP_B3_REQ_DBN							"03 10 28 04 01 00 00"
#define FEDM_ISC_TMP_B3_REQ_DB_SIZE						"03 10 28 05 01 00 00"
#define FEDM_ISC_TMP_B3_REQ_TR_TYPE						"03 10 28 06 01 00 00"
#define FEDM_ISC_TMP_B3_REQ_BLOCK_PTR					"03 10 28 07 02 00 00"
#define FEDM_ISC_TMP_B3_REQ_BLOCK_RANGE					"03 10 28 09 01 00 00"
#define FEDM_ISC_TMP_B3_REQ_MASK						"03 10 28 10 08 00 00"
#define FEDM_ISC_TMP_B3_RSP_MASK						"03 10 28 18 08 00 00"
#define FEDM_ISC_TMP_B3_REQ_RECOM_BITS					"03 10 28 26 01 00 00"
														
// block 29: [0xB3] send data							 FF-MM-BN-BY-NB-BI-Nb
//#define FEDM_ISC_TMP_B3_REQ_EPC							"03 10 29 00 32 00 00"
														
// block 30: [0xB3] send data							 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_B3_KILL_PW_LENGTH					"03 10 30 00 01 00 00"
#define FEDM_ISC_TMP_B3_KILL_PW							"03 10 30 01 31 00 00"
														
// block 31: [0xB3] send data							 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_B3_LOCK_DATA_LENGTH				"03 10 31 00 01 00 00"
#define FEDM_ISC_TMP_B3_LOCK_DATA						"03 10 31 01 31 00 00"
														
// block 32: [0xB3] send data							 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_B3_ACCESS_PW_LENGTH				"03 10 32 00 01 00 00"
#define FEDM_ISC_TMP_B3_ACCESS_PW						"03 10 32 01 31 00 00"
														
// block 33: [0xB3] receive data						 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_B3_RSP_DB_EXT_ADR_E				"03 10 33 00 02 00 00"
#define FEDM_ISC_TMP_B3_RSP_DB_ADR_E					"03 10 33 01 01 00 00"
														
// block 34: [0x72] send data							 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_0x72_OUT_MODE						"03 10 34 00 01 00 00"
#define FEDM_ISC_TMP_0x72_OUT_N							"03 10 34 01 01 00 00"
#define FEDM_ISC_TMP_0x72_OUT_TYPE_1					"03 10 34 02 01 05 03"
#define FEDM_ISC_TMP_0x72_OUT_NR_1						"03 10 34 02 01 00 04"
#define FEDM_ISC_TMP_0x72_OUT_S_1						"03 10 34 03 01 00 00"
#define FEDM_ISC_TMP_0x72_OUT_MODE_1					"03 10 34 03 01 00 02"
#define FEDM_ISC_TMP_0x72_OUT_FREQ_1					"03 10 34 03 01 02 02"
#define FEDM_ISC_TMP_0x72_OUT_REP_1						"03 10 34 03 01 04 04"
#define FEDM_ISC_TMP_0x72_OUT_TIME_1					"03 10 34 04 02 00 00"
#define FEDM_ISC_TMP_0x72_OUT_TYPE_2					"03 10 34 06 01 05 03"
#define FEDM_ISC_TMP_0x72_OUT_NR_2						"03 10 34 06 01 00 04"
#define FEDM_ISC_TMP_0x72_OUT_S_2						"03 10 34 07 01 00 00"
#define FEDM_ISC_TMP_0x72_OUT_MODE_2					"03 10 34 07 01 00 02"
#define FEDM_ISC_TMP_0x72_OUT_FREQ_2					"03 10 34 07 01 02 02"
#define FEDM_ISC_TMP_0x72_OUT_REP_2						"03 10 34 07 01 04 04"
#define FEDM_ISC_TMP_0x72_OUT_TIME_2					"03 10 34 08 02 00 00"
#define FEDM_ISC_TMP_0x72_OUT_TYPE_3					"03 10 34 10 01 05 03"
#define FEDM_ISC_TMP_0x72_OUT_NR_3						"03 10 34 10 01 00 04"
#define FEDM_ISC_TMP_0x72_OUT_S_3						"03 10 34 11 01 00 00"
#define FEDM_ISC_TMP_0x72_OUT_MODE_3					"03 10 34 11 01 00 02"
#define FEDM_ISC_TMP_0x72_OUT_FREQ_3					"03 10 34 11 01 02 02"
#define FEDM_ISC_TMP_0x72_OUT_REP_3						"03 10 34 11 01 04 04"
#define FEDM_ISC_TMP_0x72_OUT_TIME_3					"03 10 34 12 02 00 00"
#define FEDM_ISC_TMP_0x72_OUT_TYPE_4					"03 10 34 14 01 05 03"
#define FEDM_ISC_TMP_0x72_OUT_NR_4						"03 10 34 14 01 00 04"
#define FEDM_ISC_TMP_0x72_OUT_S_4						"03 10 34 15 01 00 00"
#define FEDM_ISC_TMP_0x72_OUT_MODE_4					"03 10 34 15 01 00 02"
#define FEDM_ISC_TMP_0x72_OUT_FREQ_4					"03 10 34 15 01 02 02"
#define FEDM_ISC_TMP_0x72_OUT_REP_4						"03 10 34 15 01 04 04"
#define FEDM_ISC_TMP_0x72_OUT_TIME_4					"03 10 34 16 02 00 00"
#define FEDM_ISC_TMP_0x72_OUT_TYPE_5					"03 10 34 18 01 05 03"
#define FEDM_ISC_TMP_0x72_OUT_NR_5						"03 10 34 18 01 00 04"
#define FEDM_ISC_TMP_0x72_OUT_S_5						"03 10 34 19 01 00 00"
#define FEDM_ISC_TMP_0x72_OUT_MODE_5					"03 10 34 19 01 00 02"
#define FEDM_ISC_TMP_0x72_OUT_FREQ_5					"03 10 34 19 01 02 02"
#define FEDM_ISC_TMP_0x72_OUT_REP_5						"03 10 34 19 01 04 04"
#define FEDM_ISC_TMP_0x72_OUT_TIME_5					"03 10 34 20 02 00 00"
#define FEDM_ISC_TMP_0x72_OUT_TYPE_6					"03 10 34 22 01 05 03"
#define FEDM_ISC_TMP_0x72_OUT_NR_6						"03 10 34 22 01 00 04"
#define FEDM_ISC_TMP_0x72_OUT_S_6						"03 10 34 23 01 00 00"
#define FEDM_ISC_TMP_0x72_OUT_MODE_6					"03 10 34 23 01 00 02"
#define FEDM_ISC_TMP_0x72_OUT_FREQ_6					"03 10 34 23 01 02 02"
#define FEDM_ISC_TMP_0x72_OUT_REP_6						"03 10 34 23 01 04 04"
#define FEDM_ISC_TMP_0x72_OUT_TIME_6					"03 10 34 24 02 00 00"
#define FEDM_ISC_TMP_0x72_OUT_TYPE_7					"03 10 34 26 01 05 03"
#define FEDM_ISC_TMP_0x72_OUT_NR_7						"03 10 34 26 01 00 04"
#define FEDM_ISC_TMP_0x72_OUT_S_7						"03 10 34 27 01 00 00"
#define FEDM_ISC_TMP_0x72_OUT_MODE_7					"03 10 34 27 01 00 02"
#define FEDM_ISC_TMP_0x72_OUT_FREQ_7					"03 10 34 27 01 02 02"
#define FEDM_ISC_TMP_0x72_OUT_REP_7						"03 10 34 27 01 04 04"
#define FEDM_ISC_TMP_0x72_OUT_TIME_7					"03 10 34 28 02 00 00"
#define FEDM_ISC_TMP_0x72_OUT_TYPE_8					"03 10 34 30 01 05 03"
#define FEDM_ISC_TMP_0x72_OUT_NR_8						"03 10 34 30 01 00 04"
#define FEDM_ISC_TMP_0x72_OUT_S_8						"03 10 34 31 01 00 00"
#define FEDM_ISC_TMP_0x72_OUT_MODE_8					"03 10 34 31 01 00 02"
#define FEDM_ISC_TMP_0x72_OUT_FREQ_8					"03 10 34 31 01 02 02"
#define FEDM_ISC_TMP_0x72_OUT_REP_8						"03 10 34 31 01 04 04"
														
// block 35: [0x72] send data							 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_0x72_OUT_TIME_8					"03 10 35 00 02 00 00"
//			 [0xB4] send data
#define FEDM_ISC_TMP_B4_REQ_NXP_KEY						"03 10 35 02 16 00 00"

// block 36: for notification mode						 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_NTF_REMOTE_IP_ADR					"03 10 36 00 04 00 00"
#define FEDM_ISC_TMP_NTF_IP_PORT_NR						"03 10 36 04 02 00 00"
														
// block 37: [0xB4] send data							 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_B4_CMD								"03 10 37 00 01 00 00"
#define FEDM_ISC_TMP_B4_MFR								"03 10 37 01 01 00 00"
#define FEDM_ISC_TMP_B4_MODE							"03 10 37 02 01 00 00"
#define FEDM_ISC_TMP_B4_MODE_EXT_ADR					"03 10 37 02 01 05 01"
#define FEDM_ISC_TMP_B4_MODE_EPC_LF						"03 10 37 02 01 04 01"
#define FEDM_ISC_TMP_B4_MODE_ADR						"03 10 37 02 01 00 03"
#define FEDM_ISC_TMP_B4_REQ_EPC_LEN						"03 10 37 03 01 00 00"
#define FEDM_ISC_TMP_B4_REQ_BANK						"03 10 37 04 01 00 00"
#define FEDM_ISC_TMP_B4_REQ_BANK_ACCESS_FLAG			"03 10 37 04 01 07 01"
#define FEDM_ISC_TMP_B4_REQ_BANK_BANK_NR				"03 10 37 04 01 00 02"
#define FEDM_ISC_TMP_B4_REQ_DB_ADR_EXT					"03 10 37 05 02 00 00"
#define FEDM_ISC_TMP_B4_REQ_DB_ADR						"03 10 37 06 01 00 00"
#define FEDM_ISC_TMP_B4_REQ_DBN							"03 10 37 07 01 00 00"
#define FEDM_ISC_TMP_B4_REQ_NXP_EAS						"03 10 37 08 01 00 00"
#define FEDM_ISC_TMP_B4_REQ_NXP_QUIET_BIT				"03 10 37 09 01 00 00"
#define FEDM_ISC_TMP_B4_REQ_NXP_CONFIG_WORD				"03 10 37 10 02 00 00"
/*#define FEDM_ISC_TMP_B4_REQ_NXP_UFLAG					"03 10 37 12 01 00 00"
#define FEDM_ISC_TMP_B4_REQ_NXP_HIDE_NEW_EPC_LEN		"03 10 37 13 01 00 05"
#define FEDM_ISC_TMP_B4_REQ_NXP_HIDE_EPC				"03 10 37 13 01 05 01"
#define FEDM_ISC_TMP_B4_REQ_NXP_HIDE					"03 10 37 14 02 00 00"
#define FEDM_ISC_TMP_B4_REQ_NXP_HIDE_USER				"03 10 37 14 01 00 01"
#define FEDM_ISC_TMP_B4_REQ_NXP_HIDE_TID				"03 10 37 14 01 01 02"
#define FEDM_ISC_TMP_B4_REQ_NXP_RANGE					"03 10 37 15 01 00 00"
#define FEDM_ISC_TMP_B4_REQ_NXP_RESP_FLAG				"03 10 37 16 01 00 00"
#define FEDM_ISC_TMP_B4_REQ_NXP_RESP_FLAG_INC_REP_LEN	"03 10 37 16 01 00 01"
#define FEDM_ISC_TMP_B4_REQ_NXP_RESP_FLAG_SEN_REP		"03 10 37 16 01 01 01"
#define FEDM_ISC_TMP_B4_REQ_NXP_TAM						"03 10 37 17 01 00 00"
#define FEDM_ISC_TMP_B4_REQ_NXP_KEY_ID					"03 10 37 18 01 00 00"
#define FEDM_ISC_TMP_B4_REQ_NXP_PROFILE					"03 10 37 19 01 00 00"
#define FEDM_ISC_TMP_B4_REQ_NXP_OFFSET					"03 10 37 20 02 00 00"
#define FEDM_ISC_TMP_B4_REQ_NXP_BLOCK_COUNT				"03 10 37 22 01 00 00"
#define FEDM_ISC_TMP_B4_REQ_NXP_PROT_MODE				"03 10 37 23 01 00 00"*/
#define FEDM_ISC_TMP_B4_REQ_NXP_KEY_TYPE				"03 10 37 24 01 00 00"
														
// block 38: [0x77] receive data						 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_77_COUNTER1						"03 10 38 00 04 00 00"
#define FEDM_ISC_TMP_77_COUNTER2						"03 10 38 04 04 00 00"
#define FEDM_ISC_TMP_77_COUNTER3						"03 10 38 08 04 00 00"
#define FEDM_ISC_TMP_77_COUNTER4						"03 10 38 12 04 00 00"
#define FEDM_ISC_TMP_77_BUS_ADDR						"03 10 38 16 01 00 00"

// block 39: [0xB4] send data							 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_B4_ACCESS_PW_LENGTH				"03 10 39 00 01 00 00"
#define FEDM_ISC_TMP_B4_ACCESS_PW						"03 10 39 01 31 00 00"
														
// block 40: [0xB4] receive data						 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_B4_RSP_DB_EXT_ADR_E				"03 10 40 00 02 00 00"
#define FEDM_ISC_TMP_B4_RSP_DB_ADR_E					"03 10 40 01 01 00 00"
#define FEDM_ISC_TMP_B4_RSP_NXP_CONFIG_WORD				"03 10 40 02 02 00 00"
#define FEDM_ISC_TMP_B4_RSP_EM_SENSOR_DATA				"03 10 40 04 04 00 00"
#define FEDM_ISC_TMP_B4_RSP_EM_UTC_TIMESTAMP			"03 10 40 08 04 00 00"
#define FEDM_ISC_TMP_B4_RSP_EM_SPI_DATA					"03 10 40 12 07 00 00"
#define FEDM_ISC_TMP_B4_RSP_EM_UID						"03 10 40 19 12 00 00"
														
// block 41: [0xC1] and [0xC3] send data				 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_0xC1_CMD							"03 10 41 00 01 00 00"
#define FEDM_ISC_TMP_0xC1_MODE							"03 10 41 01 01 00 00"
#define FEDM_ISC_TMP_0xC1_APP_ID						"03 10 41 02 03 00 00"
#define FEDM_ISC_TMP_0xC3_APP_ID						"03 10 41 02 03 00 00"
#define FEDM_ISC_TMP_0xC1_KEY_INDEX						"03 10 41 05 01 00 00"
#define FEDM_ISC_TMP_0xC3_KEY_INDEX						"03 10 41 05 01 00 00"
#define FEDM_ISC_TMP_0xC1_KEY_NO						"03 10 41 06 01 00 00"
#define FEDM_ISC_TMP_0xC3_KEY_NO						"03 10 41 06 01 00 00"
#define FEDM_ISC_TMP_0xC1_FILE_NO						"03 10 41 07 01 00 00"
#define FEDM_ISC_TMP_0xC3_FILE_NO						"03 10 41 07 01 00 00"
#define FEDM_ISC_TMP_0xC1_FILE_COMM_SETTINGS			"03 10 41 08 01 00 00"
#define FEDM_ISC_TMP_0xC3_FILE_COMM_SETTINGS			"03 10 41 08 01 00 00"
#define FEDM_ISC_TMP_0xC1_FILE_OFFSET					"03 10 41 09 03 00 00"
#define FEDM_ISC_TMP_0xC3_FILE_OFFSET					"03 10 41 09 03 00 00"
#define FEDM_ISC_TMP_0xC1_FILE_DATA_LEN					"03 10 41 12 03 00 00"
#define FEDM_ISC_TMP_0xC3_FILE_DATA_LEN					"03 10 41 12 03 00 00"
#define FEDM_ISC_TMP_0xC1_RECORD_OFFSET					"03 10 41 15 03 00 00"
#define FEDM_ISC_TMP_0xC3_RECORD_OFFSET					"03 10 41 15 03 00 00"
#define FEDM_ISC_TMP_0xC1_NO_OF_RECORDS					"03 10 41 18 03 00 00"
#define FEDM_ISC_TMP_0xC3_NO_OF_RECORDS					"03 10 41 18 03 00 00"
#define FEDM_ISC_TMP_0xC1_RECORD_SIZE					"03 10 41 21 03 00 00"
#define FEDM_ISC_TMP_0xC3_RECORD_SIZE					"03 10 41 21 03 00 00"
#define FEDM_ISC_TMP_0xC1_CREDIT_VALUE					"03 10 41 24 04 00 00"
#define FEDM_ISC_TMP_0xC3_CREDIT_VALUE					"03 10 41 24 04 00 00"
#define FEDM_ISC_TMP_0xC1_DEBIT_VALUE					"03 10 41 28 04 00 00"
#define FEDM_ISC_TMP_0xC3_DEBIT_VALUE					"03 10 41 28 04 00 00"
														
// block 42: [0xC1] and [0xC3] send data				 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_0xC1_FILE_ACCESS_RIGHTS			"03 10 42 00 02 00 00"
#define FEDM_ISC_TMP_0xC3_FILE_ACCESS_RIGHTS			"03 10 42 00 02 00 00"
#define FEDM_ISC_TMP_0xC1_NEW_KEY_SETTINGS				"03 10 42 02 01 00 00"
#define FEDM_ISC_TMP_0xC3_NEW_KEY_SETTINGS				"03 10 42 02 01 00 00"
#define FEDM_ISC_TMP_0xC1_KEY_NO_TB_CHANGED				"03 10 42 03 01 00 00"
#define FEDM_ISC_TMP_0xC3_KEY_NO_TB_CHANGED				"03 10 42 03 01 00 00"
#define FEDM_ISC_TMP_0xC1_NEW_KEY_VERSION				"03 10 42 04 01 00 00"
#define FEDM_ISC_TMP_0xC3_NEW_KEY_VERSION				"03 10 42 04 01 00 00"
#define FEDM_ISC_TMP_0xC1_KEY_LEN						"03 10 42 05 01 00 00"
#define FEDM_ISC_TMP_0xC3_KEY_LEN						"03 10 42 05 01 00 00"
#define FEDM_ISC_TMP_0xC1_OLD_KEY						"03 10 42 06 24 00 00"
#define FEDM_ISC_TMP_0xC3_OLD_KEY						"03 10 42 06 24 00 00"
														
// block 43: [0xC1] and [0xC3] send data				 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_0xC1_NEW_KEY						"03 10 43 00 24 00 00"
#define FEDM_ISC_TMP_0xC3_NEW_KEY						"03 10 43 00 24 00 00"
														
// block 44: [0xC1] and [0xC3] receive data				 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_0xC1_ERROR_SOURCE					"03 10 44 00 01 00 00"
#define FEDM_ISC_TMP_0xC1_ERROR_CODE					"03 10 44 01 02 00 00"
#define FEDM_ISC_TMP_0xC3_ERROR_SOURCE					"03 10 44 03 01 00 00"
#define FEDM_ISC_TMP_0xC3_ERROR_CODE					"03 10 44 04 02 00 00"
														
// block 45: [0xC2] send data							 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_0xC2_CMD							"03 10 45 00 01 00 00"
#define FEDM_ISC_TMP_0xC2_MODE							"03 10 45 01 01 00 00"
#define FEDM_ISC_TMP_0xC2_KEY_INDEX						"03 10 45 02 01 00 00"
#define FEDM_ISC_TMP_0xC2_AES_KEY_BLOCK_NO				"03 10 45 03 02 00 00"
#define FEDM_ISC_TMP_0xC2_PCD_CAP_LEN					"03 10 45 05 01 00 00"
#define FEDM_ISC_TMP_0xC2_PCD_CAP						"03 10 45 06 06 00 00"
#define FEDM_ISC_TMP_0xC2_BLOCK_NO						"03 10 45 12 02 00 00"
#define FEDM_ISC_TMP_0xC2_EXT							"03 10 45 14 01 00 00"
#define FEDM_ISC_TMP_0xC2_LENGTH						"03 10 45 14 01 00 00" // same location as EXT !!
#define FEDM_ISC_TMP_0xC2_SRC_BLOCK_NO					"03 10 45 15 02 00 00"
#define FEDM_ISC_TMP_0xC2_DST_BLOCK_NO					"03 10 45 17 02 00 00"
#define FEDM_ISC_TMP_0xC2_DB_ADR						"03 10 45 19 01 00 00"
#define FEDM_ISC_TMP_0xC2_KEY_TYPE						"03 10 45 20 01 00 00"
#define FEDM_ISC_TMP_0xC2_KEY_NO						"03 10 45 21 01 00 00"
#define FEDM_ISC_TMP_0xC2_VALUE							"03 10 45 22 04 00 00"
														
// block 46: [0xB4] send data for EM Custom Commands	 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_B4_REQ_EM_OPTION					"03 10 46 00 01 00 00"
#define FEDM_ISC_TMP_B4_REQ_EM_OPTION_NEW_SAMPLE		"03 10 46 00 01 00 01"
#define FEDM_ISC_TMP_B4_REQ_EM_OPTION_SEND_UID			"03 10 46 00 01 01 01"
#define FEDM_ISC_TMP_B4_REQ_EM_SPI_CMD_SIZE				"03 10 46 01 01 00 00"
#define FEDM_ISC_TMP_B4_REQ_EM_SPI_RESP_SIZE			"03 10 46 02 01 00 00"
#define FEDM_ISC_TMP_B4_REQ_EM_SPI_SCLK					"03 10 46 03 01 00 00"
#define FEDM_ISC_TMP_B4_REQ_EM_SPI_DELAY_TIME			"03 10 46 04 01 00 00"
#define FEDM_ISC_TMP_B4_REQ_EM_SPI_CMD					"03 10 46 05 08 00 00"
#define FEDM_ISC_TMP_B4_REQ_EM_UTC_TIME					"03 10 46 13 04 00 00"
														
// block 47: [0xC2] receive data						 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_0xC2_ERROR_SOURCE					"03 10 47 00 01 00 00"
#define FEDM_ISC_TMP_0xC2_ERROR_CODE					"03 10 47 01 02 00 00"
														
// block 48: [0xAD] send data							FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_0xAD_MODE							"03 10 48 00 01 00 00"
#define FEDM_ISC_TMP_0xAD_KEY_TYPE						"03 10 48 01 01 00 00"
#define FEDM_ISC_TMP_0xAD_KEY_LEN						"03 10 48 02 01 00 00"
#define FEDM_ISC_TMP_0xAD_KEY							"03 10 48 03 32 00 00"
														
// block 49: [0xAD] send data							FF-MM-BN-BY-NB-BI-Nb
// used partial by key
														
// block 50: [0xAE] send data							FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_0xAE_MODE							"03 10 50 00 01 00 00"
#define FEDM_ISC_TMP_0xAE_KEY_TYPE						"03 10 50 01 01 00 00"
#define FEDM_ISC_TMP_0xAE_KEY_LEN						"03 10 50 02 01 00 00"
#define FEDM_ISC_TMP_0xAE_KEY							"03 10 50 03 32 00 00"
														
// block 51: [0xAE] send data							FF-MM-BN-BY-NB-BI-Nb
// used partial by key

// block 54..63: reserved

// block 64..73: [0xB0][0x25] receive data
// card information from 14443 Transponder,
// as described in system manual of ID CPR-Family

// block 64..: [0x8B] Write Configuration: array with CFG-ADR
#define FEDM_ISC_TMP_0x8B_WRITE_CFG_ADR					"03 10 64 00 02 00 00"	// first CFG address to be written with [0x8B]

// block 74: free										 FF-MM-BN-BY-NB-BI-Nb
														
// block 75: [0xC3] send data							 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_0xC3_CMD							"03 10 75 00 01 00 00"
#define FEDM_ISC_TMP_0xC3_MODE							"03 10 75 01 01 00 00"
#define FEDM_ISC_TMP_0xC3_SLOT_NO						"03 10 75 02 01 00 00"
#define FEDM_ISC_TMP_0xC3_TIMEOUT						"03 10 75 03 01 00 00"
#define FEDM_ISC_TMP_0xC3_DESFIRE_AUTH_MODE				"03 10 75 04 01 00 00"
#define FEDM_ISC_TMP_0xC3_SAM_AUTH_MODE					"03 10 75 05 01 00 00"
#define FEDM_ISC_TMP_0xC3_SAM_KEY_NO					"03 10 75 06 01 00 00"
#define FEDM_ISC_TMP_0xC3_SAM_KEY_VERSION				"03 10 75 07 01 00 00"
#define FEDM_ISC_TMP_0xC3_SAM_DIV_INPUT_LEN				"03 10 75 08 01 00 00"
#define FEDM_ISC_TMP_0xC3_SAM_DIV_INPUT					"03 10 75 09 16 00 00"
#define FEDM_ISC_TMP_0xC3_SAM_KEY_COMP_METHOD			"03 10 75 25 01 00 00"
#define FEDM_ISC_TMP_0xC3_SAM_CFG						"03 10 75 26 01 00 00"
#define FEDM_ISC_TMP_0xC3_SAM_KEY_NO_CURRENT_KEY		"03 10 75 27 01 00 00"
#define FEDM_ISC_TMP_0xC3_SAM_KEY_VERSION_CURRENT_KEY	"03 10 75 28 01 00 00"
#define FEDM_ISC_TMP_0xC3_SAM_KEY_NO_NEW_KEY			"03 10 75 29 01 00 00"
#define FEDM_ISC_TMP_0xC3_SAM_KEY_VERSION_NEW_KEY		"03 10 75 30 01 00 00"

// block 76: [0xC3] send data							 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_0xC3_PICC_LEVEL_FLAGS				"03 10 76 00 01 00 00"
#define FEDM_ISC_TMP_0xC3_APPLICATION_LEVEL_FLAGS		"03 10 76 01 01 00 00"
#define FEDM_ISC_TMP_0xC3_DATA_MANIPULATION_FLAGS		"03 10 76 02 01 00 00"
#define FEDM_ISC_TMP_0xC3_KEY_SETTINGS1					"03 10 76 03 01 00 00"
#define FEDM_ISC_TMP_0xC3_KEY_SETTINGS2					"03 10 76 04 01 00 00"
#define FEDM_ISC_TMP_0xC3_ISO7816_FILE_ID				"03 10 76 05 02 00 00"
#define FEDM_ISC_TMP_0xC3_ISO7816_DF_NAME_LEN			"03 10 76 07 01 00 00"
#define FEDM_ISC_TMP_0xC3_ISO7816_DF_NAME				"03 10 76 08 16 00 00"
#define FEDM_ISC_TMP_0xC3_0x6E_OPTION					"03 10 76 24 01 00 00"
#define FEDM_ISC_TMP_0xC3_0x6E_CONFIG					"03 10 76 25 01 00 00"
#define FEDM_ISC_TMP_0xC3_0x6E_DEFAULT_KEY_VERSION		"03 10 76 26 01 00 00"
#define FEDM_ISC_TMP_0xC3_LIMITED_CREDIT_ENABLED		"03 10 76 27 01 00 00"

// block 77: [0xC3] send data							 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_0xC3_FILE_SIZE						"03 10 77 00 03 00 00"
#define FEDM_ISC_TMP_0xC3_0x6E_DEFAULT_KEY				"03 10 77 03 24 00 00"

// block 78: [0xC3] send data							 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_0xC3_0x6E_USER_DEF_ATS				"03 10 78 00 20 00 00"
#define FEDM_ISC_TMP_0xC3_LOWER_LIMIT					"03 10 78 20 04 00 00"
#define FEDM_ISC_TMP_0xC3_UPPER_LIMIT					"03 10 78 24 04 00 00"
#define FEDM_ISC_TMP_0xC3_VALUE							"03 10 78 28 04 00 00"
										
// block 79: [0xB1] send data							 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_0xB1_REQ_IDS_PASSWORD_LEVEL		"03 10 79 00 01 00 00" // for IDS [0xA0], [0xAB]
#define FEDM_ISC_TMP_0xB1_REQ_IDS_PASSWORD				"03 10 79 01 04 00 00" // for IDS [0xA0]
#define FEDM_ISC_TMP_0xB1_REQ_IDS_LOG_MODE				"03 10 79 05 04 00 00" // for IDS [0xA1]
#define FEDM_ISC_TMP_0xB1_REQ_IDS_LOG_LIMITS			"03 10 79 09 04 00 00" // for IDS [0xA2]
#define FEDM_ISC_TMP_0xB1_REQ_IDS_EXT_CAL_DATA			"03 10 79 13 04 00 00" // for IDS [0xA4]
#define FEDM_ISC_TMP_0xB1_REQ_IDS_INT_CAL_DATA			"03 10 79 17 04 00 00" // for IDS [0xA5]
#define FEDM_ISC_TMP_0xB1_REQ_IDS_START_TIME			"03 10 79 21 04 00 00" // for IDS [0xA7]
#define FEDM_ISC_TMP_0xB1_REQ_IDS_INIT_DATA				"03 10 79 25 04 00 00" // for IDS [0xAC]
										
// block 80: [0xB1] receive data						 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_0xB1_RSP_IDS_START_TIME			"03 10 80 00 04 00 00" // for IDS [0xA3]
#define FEDM_ISC_TMP_0xB1_RSP_IDS_LOG_LIMITS			"03 10 80 04 04 00 00" // for IDS [0xA3]
#define FEDM_ISC_TMP_0xB1_RSP_IDS_LOG_MODE				"03 10 80 08 04 00 00" // for IDS [0xA3]
#define FEDM_ISC_TMP_0xB1_RSP_IDS_DELAY_TIME			"03 10 80 12 04 00 00" // for IDS [0xA3]
#define FEDM_ISC_TMP_0xB1_RSP_IDS_MEAS_STATE			"03 10 80 16 04 00 00" // for IDS [0xA8]
#define FEDM_ISC_TMP_0xB1_RSP_IDS_LIMITS_COUNTER		"03 10 80 20 04 00 00" // for IDS [0xA8]
#define FEDM_ISC_TMP_0xB1_RSP_IDS_INT_CAL_DATA			"03 10 80 24 04 00 00" // for IDS [0xA9]
#define FEDM_ISC_TMP_0xB1_RSP_IDS_EXT_CAL_DATA			"03 10 80 28 04 00 00" // for IDS [0xA9]
										
// block 81: [0xB1] receive data						 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_0xB1_RSP_IDS_BATTERY_LEVEL			"03 10 81 00 01 00 00" // for IDS [0xAA]
#define FEDM_ISC_TMP_0xB1_RSP_IDS_MEAS_TEMP				"03 10 81 01 02 00 00" // for IDS [0xAD]

// block 82: [0xB4] send data							 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_0xB4_REQ_IDS_PASSWORD_LEVEL		"03 10 82 00 01 00 00" // for IDS [0xA0], [0xAE]
#define FEDM_ISC_TMP_0xB4_REQ_IDS_PASSWORD				"03 10 82 01 04 00 00" // for IDS [0xA0]
#define FEDM_ISC_TMP_0xB4_REQ_IDS_LOG_MODE				"03 10 82 05 03 00 00" // for IDS [0xA1]
#define FEDM_ISC_TMP_0xB4_REQ_IDS_LOG_LIMITS			"03 10 82 08 05 00 00" // for IDS [0xA2]
#define FEDM_ISC_TMP_0xB4_REQ_IDS_SFE_PARA				"03 10 82 13 02 00 00" // for IDS [0xA4]
#define FEDM_ISC_TMP_0xB4_REQ_IDS_CAL_DATA				"03 10 82 15 07 00 00" // for IDS [0xA5]
#define FEDM_ISC_TMP_0xB4_REQ_IDS_START_TIME			"03 10 82 22 04 00 00" // for IDS [0xA7]
#define FEDM_ISC_TMP_0xB4_REQ_IDS_BATTERY_RETRIGGER		"03 10 82 26 01 00 00" // for IDS [0xAA]

// block 83: [0xB4] send data							 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_0xB4_REQ_IDS_SL_BLOCK_0			"03 10 83 00 04 00 00" // for IDS [0xAB]
#define FEDM_ISC_TMP_0xB4_REQ_IDS_SL_BLOCK_1			"03 10 83 04 04 00 00" // for IDS [0xAB]
#define FEDM_ISC_TMP_0xB4_REQ_IDS_DELAY_TIME			"03 10 83 08 02 00 00" // for IDS [0xAC]
#define FEDM_ISC_TMP_0xB4_REQ_IDS_APP_DATA				"03 10 83 10 02 00 00" // for IDS [0xAC]
#define FEDM_ISC_TMP_0xB4_REQ_IDS_SENSOR_VALUE			"03 10 83 12 01 00 00" // for IDS [0xAD]
#define FEDM_ISC_TMP_0xB4_REQ_IDS_SUB_COMMAND			"03 10 83 13 01 00 00" // for IDS [0xAF]
#define FEDM_ISC_TMP_0xB4_REQ_IDS_PAYLOAD				"03 10 83 14 08 00 00" // for IDS [0xAF]
										
// block 84: [0xB4] receive data						 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_0xB4_RSP_IDS_START_TIME			"03 10 84 00 04 00 00" // for IDS [0xA3]
#define FEDM_ISC_TMP_0xB4_RSP_IDS_LOG_LIMITS			"03 10 84 04 05 00 00" // for IDS [0xA3]
#define FEDM_ISC_TMP_0xB4_RSP_IDS_LOG_MODE				"03 10 84 09 01 00 00" // for IDS [0xA3]
#define FEDM_ISC_TMP_0xB4_RSP_IDS_LOG_INTERVAL			"03 10 84 10 02 00 00" // for IDS [0xA3]
#define FEDM_ISC_TMP_0xB4_RSP_IDS_DELAY_TIME			"03 10 84 12 02 00 00" // for IDS [0xA3]
#define FEDM_ISC_TMP_0xB4_RSP_IDS_APP_DATA				"03 10 84 14 02 00 00" // for IDS [0xA3]
#define FEDM_ISC_TMP_0xB4_RSP_IDS_LIMITS_COUNTER		"03 10 84 16 04 00 00" // for IDS [0xA8]
#define FEDM_ISC_TMP_0xB4_RSP_IDS_SYSTEM_STATUS			"03 10 84 20 04 00 00" // for IDS [0xA8]
#define FEDM_ISC_TMP_0xB4_RSP_IDS_SL_BLOCK_0			"03 10 84 24 04 00 00" // for IDS [0xA8]
#define FEDM_ISC_TMP_0xB4_RSP_IDS_SL_BLOCK_1			"03 10 84 28 04 00 00" // for IDS [0xA8]

// block 85: [0xB4] receive data						 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_TMP_0xB4_RSP_IDS_CURRENT_SHELF_LIFE	"03 10 85 00 03 00 00" // for IDS [0xA8]
#define FEDM_ISC_TMP_0xB4_RSP_IDS_STATUS_FLAG			"03 10 85 03 01 00 00" // for IDS [0xA8]
#define FEDM_ISC_TMP_0xB4_RSP_IDS_CAL_DATA				"03 10 85 04 07 00 00" // for IDS [0xA9]
#define FEDM_ISC_TMP_0xB4_RSP_IDS_SFE_PARA				"03 10 85 11 02 00 00" // for IDS [0xA9]
#define FEDM_ISC_TMP_0xB4_RSP_IDS_BATTERY_LEVEL			"03 10 85 13 02 00 00" // for IDS [0xAA]
#define FEDM_ISC_TMP_0xB4_RSP_IDS_SENSOR_INFO			"03 10 85 15 02 00 00" // for IDS [0xAD]
#define FEDM_ISC_TMP_0xB4_RSP_IDS_PAYLOAD				"03 10 85 17 08 00 00" // for IDS [0xAF]

// block 99..101: reserved

// block 110-112: [0xB0] send data						 FF-MM-BN -BY-NB-BI-Nb
#define FEDM_ISC_TMP_B0_REQ_UID							"03 10 110 00 96 00 00"
// block 113-115: [0xB3] send data						 FF-MM-BN -BY-NB-BI-Nb
#define FEDM_ISC_TMP_B3_REQ_EPC							"03 10 113 00 96 00 00"
// block 116-118: [0xB4] send data						 FF-MM-BN -BY-NB-BI-Nb
#define FEDM_ISC_TMP_B4_REQ_EPC							"03 10 116 00 96 00 00"

// block 129...159: [0xC3] receive data					 FF-MM-BN -BY-NB-BI-Nb
#define FEDM_ISC_TMP_0xC3_RSP_DATA_LEN					"03 10 129 00 02 00 00"
#define FEDM_ISC_TMP_0xC3_RSP_DATA						"03 10 129 02 01 00 00" // only one dummy byte length

// block 160...169: [0x6E] receive data	in NTF mode		 FF-MM-BN -BY-NB-BI-Nb
#define FEDM_ISC_TMP_0x6E_NTF_RSP_READER_TYPE			"03 10 160 00 04 00 00"
#define FEDM_ISC_TMP_0x6E_NTF_RSP_REC_COUNT				"03 10 160 04 01 00 00"
#define FEDM_ISC_TMP_0x6E_NTF_RSP_DATA					"03 10 160 05 01 00 00" // only one dummy byte length


#endif // !defined(_FEDM_ISC_READERID_H_INCLUDED_)
