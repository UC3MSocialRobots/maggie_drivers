/*-------------------------------------------------------
|                                                       |
|                  FEDM_TabItem.h						|
|                                                       |
---------------------------------------------------------

Copyright © 2011-2012	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	27.10.2011

Version       		:	04.04.00 / 13.12.2012 / M. Hultsch
						- new method GetEpcC1G2MaskDesignerName()

						04.00.08 / 21.12.2011 / M. Hultsch
						- new manufacturer: 0x39 (Silicon Craft Technology) ... 0x44 (Gentag Inc)

						04.00.04 / 27.10.2011 / M. Hultsch

Operation Systems	:	independent

Function			:	base table class


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_TABITEM_H_INCLUDED_)
#define _FEDM_TABITEM_H_INCLUDED_



#include "FEDM_ISO_IEC_7816_6_ICManufacturerRegistration.h"



//#####################################################################################
// constants
//#####################################################################################




//####################################################################
// class FEDM_TabItem
//####################################################################

class /*_FEDM_ISC_CORE_EXT_CLASS*/ FEDM_TabItem
{
public:
	FEDM_TabItem()	{}							// default constructor
	/*virtual*/ ~FEDM_TabItem()	{}				// destructor

public:
	static const char* GetISO15693Manufacturer(unsigned char ucMfrByte)
	{
		switch(ucMfrByte)
		{
		case FEDM_ISC_ISO_MFR_MOTOROLA:			return FEDM_ISO_MOTOROLA;
		case FEDM_ISC_ISO_MFR_STM:				return FEDM_ISO_STM;
		case FEDM_ISC_ISO_MFR_HITACHI:			return FEDM_ISO_HITACHI;
		case FEDM_ISC_ISO_MFR_NXP:				return FEDM_ISO_NXP;
		case FEDM_ISC_ISO_MFR_INFINEON:			return FEDM_ISO_INFINEON;
		case FEDM_ISC_ISO_MFR_CYLINK:			return FEDM_ISO_CYLINK	;
		case FEDM_ISC_ISO_MFR_TI:				return FEDM_ISO_TI;
		case FEDM_ISC_ISO_MFR_FUJITSU:			return FEDM_ISO_FUJITSU;
		case FEDM_ISC_ISO_MFR_MATSUSHITA:		return FEDM_ISO_MATSUSHITA;
		case FEDM_ISC_ISO_MFR_NEC:				return FEDM_ISO_NEC;
		case FEDM_ISC_ISO_MFR_OKI:				return FEDM_ISO_OKI;
		case FEDM_ISC_ISO_MFR_TOSHIBA:			return FEDM_ISO_TOSHIBA;
		case FEDM_ISC_ISO_MFR_MITSUBISHI:		return FEDM_ISO_MITSUBISHI;
		case FEDM_ISC_ISO_MFR_SAMSUNG:			return FEDM_ISO_SAMSUNG;
		case FEDM_ISC_ISO_MFR_HYUNDAI:			return FEDM_ISO_HYUNDAI;
		case FEDM_ISC_ISO_MFR_LG:				return FEDM_ISO_LG;
		case FEDM_ISC_ISO_MFR_EMOSYN_EM:		return FEDM_ISO_EMOSYN_EM;
		case FEDM_ISC_ISO_MFR_INSIDE:			return FEDM_ISO_INSIDE;
		case FEDM_ISC_ISO_MFR_ORGA:				return FEDM_ISO_ORGA;
		case FEDM_ISC_ISO_MFR_SHARP:			return FEDM_ISO_SHARP;
		case FEDM_ISC_ISO_MFR_ATMEL:			return FEDM_ISO_ATMEL;
		case FEDM_ISC_ISO_MFR_EM:				return FEDM_ISO_EM;
		case FEDM_ISC_ISO_MFR_KSW:				return FEDM_ISO_KSW;
		case FEDM_ISC_ISO_MFR_ZMD:				return FEDM_ISO_ZMD;
		case FEDM_ISC_ISO_MFR_XICOR:			return FEDM_ISO_XICOR;
		case FEDM_ISC_ISO_MFR_SONY:				return FEDM_ISO_SONY;
		case FEDM_ISC_ISO_MFR_MALAYSIA:			return FEDM_ISO_MALAYSIA;
		case FEDM_ISC_ISO_MFR_EMOSYN:			return FEDM_ISO_EMOSYN;
		case FEDM_ISC_ISO_MFR_SHANGHAI_FUDAN:	return FEDM_ISO_SHANGHAI_FUDAN;
		case FEDM_ISC_ISO_MFR_MAGELLAN:			return FEDM_ISO_MAGELLAN;
		case FEDM_ISC_ISO_MFR_MELEXIS:			return FEDM_ISO_MELEXIS;
		case FEDM_ISC_ISO_MFR_RENESAS_TECH:		return FEDM_ISO_RENESAS_TECH;
		case FEDM_ISC_ISO_MFR_TAGSYS:			return FEDM_ISO_TAGSYS;
		case FEDM_ISC_ISO_MFR_TRANSCORE:		return FEDM_ISO_TRANSCORE;
		case FEDM_ISC_ISO_MFR_SHANGHAI_BELLING:	return FEDM_ISO_SHANHAI_BELLING;
		case FEDM_ISC_ISO_MFR_MASKTECH:			return FEDM_ISO_MASKTECH;
		case FEDM_ISC_ISO_MFR_INNOVISION:		return FEDM_ISO_INNOVISION;
		case FEDM_ISC_ISO_MFR_HITACHI_ULSI:		return FEDM_ISO_HITACHI_ULSI;
		case FEDM_ISC_ISO_MFR_CYPAK:			return FEDM_ISO_CYPAK;
		case FEDM_ISC_ISO_MFR_RICOH:			return FEDM_ISO_RICOH;
		case FEDM_ISC_ISO_MFR_ASK:				return FEDM_ISO_ASK;
		case FEDM_ISC_ISO_MFR_UNICORE:			return FEDM_ISO_UNICORE;
		case FEDM_ISC_ISO_MFR_MAXIM:			return FEDM_ISO_MAXIM;
		case FEDM_ISC_ISO_MFR_IMPINJ:			return FEDM_ISO_IMPINJ;
		case FEDM_ISC_ISO_MFR_RIGHTPLUG:		return FEDM_ISO_RIGHTPLUG;
		case FEDM_ISC_ISO_MFR_BROADCOM:			return FEDM_ISO_BROADCOM;
		case FEDM_ISC_ISO_MFR_MSTAR:			return FEDM_ISO_MSTAR;
		case FEDM_ISC_ISO_MFR_EEDAR:			return FEDM_ISO_EEDAR;
		case FEDM_ISC_ISO_MFR_RFIDSEC:			return FEDM_ISO_RFIDSEC;
		case FEDM_ISC_ISO_MFR_SCHWEIZER:		return FEDM_ISO_SCHWEIZER;
		case FEDM_ISC_ISO_MFR_MIKRON_JSC:		return FEDM_ISO_MIKRON_JSC;
		case FEDM_ISC_ISO_MFR_FRAUNHOFER_IPMS:	return FEDM_ISO_FRAUNHOFER_IPMS;
		case FEDM_ISC_ISO_MFR_AMIC:				return FEDM_ISO_AMIC;
		case FEDM_ISC_ISO_MFR_IDS:				return FEDM_ISO_IDS;
		case FEDM_ISC_ISO_MFR_KOVIO:			return FEDM_ISO_KOVIO;
		case FEDM_ISC_ISO_MFR_HMT:				return FEDM_ISO_HMT;
		case FEDM_ISC_ISO_MFR_SILICON_CRAFT:	return FEDM_ISO_SILICAN_CRAFT;
		case FEDM_ISC_ISO_MFR_ADV_FILM_DEV:		return FEDM_ISO_ADV_FILM_DEV;
		case FEDM_ISC_ISO_MFR_NITECREST:		return FEDM_ISO_NITECREST;
		case FEDM_ISC_ISO_MFR_VERAYO:			return FEDM_ISO_VERAYO;
		case FEDM_ISC_ISO_MFR_HID:				return FEDM_ISO_HID;
		case FEDM_ISC_ISO_MFR_PROD_ENG:			return FEDM_ISO_PROD_ENG;
		case FEDM_ISC_ISO_MFR_AUSTRIA_MICRO:	return FEDM_ISO_AUSTRIA_MICRO;
		case FEDM_ISC_ISO_MFR_GEMALTO:			return FEDM_ISO_GEMALTO;
		case FEDM_ISC_ISO_MFR_RENESAS_ELECTR:	return FEDM_ISO_RENESAS_ELECTR;
		case FEDM_ISC_ISO_MFR_3ALOGICS:			return FEDM_ISO_3ALOGICS;
		case FEDM_ISC_ISO_MFR_TOP_TRONIQ:		return FEDM_ISO_TOP_TRONIQ;
		case FEDM_ISC_ISO_MFR_GENTAG:			return FEDM_ISO_GENTAG;
		}

		return "";
	}

	static const char* GetEpcC1G2MaskDesignerName(unsigned int uiMDID)
	{
		switch(uiMDID)
		{
		case 0x001:	return "Impinj";
		case 0x002: return FEDM_ISO_TI;
		case 0x003: return "Alien Technology";
		case 0x004:	return "Intelleflex";
		case 0x005:	return FEDM_ISO_ATMEL;
		case 0x006:	return FEDM_ISO_NXP;
		case 0x007:	return FEDM_ISO_STM;
		case 0x008:	return "EP Microelectronics";
		case 0x009: return FEDM_ISO_MOTOROLA;
		case 0x00A:	return "";
		case 0x00B:	return FEDM_ISO_EM;
		case 0x00C:	return "Renesas Technology";
		case 0x00D:	return "MStar";
		case 0x00E: return "Tyco International";
		case 0x00F:	return "Quanray Electronics";
		case 0x010:	return FEDM_ISO_FUJITSU;
		}

		return "";
	}

};

#endif // !defined(_FEDM_TABITEM_H_INCLUDED_)
