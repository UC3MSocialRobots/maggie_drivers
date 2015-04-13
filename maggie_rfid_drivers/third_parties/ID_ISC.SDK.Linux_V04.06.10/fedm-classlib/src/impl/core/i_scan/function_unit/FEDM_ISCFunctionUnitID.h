/*-------------------------------------------------------
|                                                       |
|                 FEDM_ISCFunctionUnitID.h              |
|                                                       |
---------------------------------------------------------

Copyright © 2005-2008	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	05.01.2005
Version       		:	03.00.00 / 04.04.2008 / M. Hultsch

Operation Systems	:	Windows 9x/ME/NT/2000/XP/Linux/WindowsCE


This file contains the parameter id for the OBID® i-scan function units


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h



### construction of IDs

	FC - family code			0..99
	MM - memory type			0..99	(see FEDM.h)
	BL - block number			0..99
	BY - 1. byte in block		0..99
	NB - number of bytes		0..99
	BI - 1. bit in byte			0..08
	Nb - number of bits			0..99


### how we build the identifier names

	#define FEDM_ISC_FU_TMP_					"03 10 00 00 01 00 00"
			  |    |  |  |  |
			  |    |  |  |  ----- the rest is a shortening of the parameter
			  |    |  |  -------- the parameter is located in memory TmpData
			  |    |  ----------- identifies function unit
			  |    -------------- reader family
		      ------------------- name of this component

*/

#if !defined(_FEDM_ISC_FUNCTION_UNIT_ID_H_INCLUDED_)
#define _FEDM_ISC_FUNCTION_UNIT_ID_H_INCLUDED_



				
//#############################################################################
// TMPDATA_MEM: parameter id for temporary data
//#############################################################################

// block 0												 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_FU_TMP_MUX_ADR							"03 10 00 00 01 00 00"	// multiplexer address
#define FEDM_ISC_FU_TMP_DAT_ADR							"03 10 00 01 01 00 00"	// dynamic antenna tuner address
#define FEDM_ISC_FU_TMP_FLAGS							"03 10 00 02 01 00 00"	// flags for UHF multiplexer
														
// block 1												 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_FU_TMP_SOFTVER							"03 10 01 00 07 00 00"	// all bytes of software version
#define FEDM_ISC_FU_TMP_SOFTVER_SW_REV					"03 10 01 00 02 00 00"	// SW-REV value
#define FEDM_ISC_FU_TMP_SOFTVER_SW_REV_MAIN				"03 10 01 00 01 00 00"	// SW-REV main revision number
#define FEDM_ISC_FU_TMP_SOFTVER_SW_REV_SUB				"03 10 01 01 01 00 00"	// SW-REV sub revision number
#define FEDM_ISC_FU_TMP_SOFTVER_SW_TYPE					"03 10 01 02 01 00 00"	// SW-TYPE value
#define FEDM_ISC_FU_TMP_SOFTVER_HW_REV					"03 10 01 03 01 00 00"	// HW-REV value
#define FEDM_ISC_FU_TMP_SOFTVER_DIP						"03 10 01 04 01 00 00"	// DIP value
#define FEDM_ISC_FU_TMP_SOFTVER_RES1					"03 10 01 05 01 00 00"	// reserved
#define FEDM_ISC_FU_TMP_SOFTVER_RES2					"03 10 01 06 01 00 00"	// reserved

// block 2												 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_FU_TMP_MUX_OUT_CH1						"03 10 02 00 01 00 00"	// output channel value for input 1 in [0xDD]
#define FEDM_ISC_FU_TMP_MUX_OUT_CH2						"03 10 02 01 01 00 00"	// output channel value for input 2 in [0xDD]

// block 3												 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_FU_TMP_DAT_ANT_VAL						"03 10 03 00 06 00 00"	// antenna values in [0xC3]
#define FEDM_ISC_FU_TMP_DAT_ANT_VAL_C1					"03 10 03 00 01 00 00"	// value of C1 in [0xC3]
#define FEDM_ISC_FU_TMP_DAT_ANT_VAL_C2					"03 10 03 01 01 00 00"	// value of C1 in [0xC3]
#define FEDM_ISC_FU_TMP_DAT_ANT_VAL_R					"03 10 03 02 02 00 00"	// value of R in [0xC3]
#define FEDM_ISC_FU_TMP_DAT_ANT_VAL_PHI					"03 10 03 04 01 00 00"	// value of PHI in [0xC3]
#define FEDM_ISC_FU_TMP_DAT_ANT_VAL_STATUS				"03 10 03 05 01 00 00"	// value of TU-ST in [0xC3]

// block 4												 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_FU_TMP_DAT_OUT							"03 10 04 00 01 00 00"	// value of OUT in [0xC4]
#define FEDM_ISC_FU_TMP_DAT_NEW_ADR						"03 10 04 01 01 00 00"	// value of ADDRESS in [0xCA]
#define FEDM_ISC_FU_TMP_DAT_MODE						"03 10 04 02 01 00 00"	// value of MODE in [0xCB]

// block 5												 FF-MM-BN-BY-NB-BI-Nb
#define FEDM_ISC_FU_TMP_UMUX_POWER						"03 10 05 00 05 00 00"	// all bytes of get power protocol for UHF mux
#define FEDM_ISC_FU_TMP_UMUX_POWER_FWD					"03 10 05 00 02 00 00"	// value of POWER_FWD for UHF mux
#define FEDM_ISC_FU_TMP_UMUX_POWER_REV					"03 10 05 02 02 00 00"	// value of POWER_REV for UHF mux
#define FEDM_ISC_FU_TMP_UMUX_ERROR_FLAGS				"03 10 05 04 01 00 00"	// error flags of UHF mux
#define FEDM_ISC_FU_TMP_UMUX_RESERVED					"03 10 05 05 01 00 00"	// reserved byte of UHF mux get power protocol

// block 6
#define FEDM_ISC_FU_TMP_UMUX_LAST_STATE					"03 10 06 00 01 00 00" // status byte from multiplexer

#endif // !defined(_FEDM_ISC_FUNCTION_UNIT_ID_H_INCLUDED_)
