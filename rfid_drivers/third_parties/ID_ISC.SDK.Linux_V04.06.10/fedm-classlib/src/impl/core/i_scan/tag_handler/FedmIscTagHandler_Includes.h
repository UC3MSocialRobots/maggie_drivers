/*-------------------------------------------------------
|                                                       |
|	   FedmIscTagHandler_Includes.h                     |
|                                                       |
---------------------------------------------------------

Copyright © 2009-2013	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	02.10.2009
Version       		:	04.05.02 / 02.07.2013 / M. Hultsch

Operation Systems	:	independent


This file includes all dependencies for the component FEDM with support for TagHandler

Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#if !defined(_FEDM_ISC_TAG_HANDLER_INCLUDES_H_INCLUDED_)
#define _FEDM_ISC_TAG_HANDLER_INCLUDES_H_INCLUDED_

#if !defined(_FEDM_NO_TAG_HANDLER)
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler.h"
#if !defined(_FEDM_NO_TAG_HANDLER_EPC_C1_G2)
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_EPC_Class1_Gen2.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A.h"
#endif
#if !defined(_FEDM_NO_TAG_HANDLER_ISO18000_3M3)
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO18000_3M3.h"
#endif
#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO14443.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO14443_Innovatron.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO14443_2.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO14443_2_Innovision_Jewel.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO14443_2_STM_SR176.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO14443_2_STM_SRIxxx.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO14443_3.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO14443_3_ASK_CTx.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO14443_3_Infineon_my_d.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO14443_3_Infineon_my_d_move.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO14443_3_MIFARE_Classic.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO14443_3_MIFARE_Ultralight.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO14443_3_MIFARE_Plus.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL1.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL2.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO14443_4.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO14443_4_Maxim.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO14443_4_Maxim_MAX66000.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO14443_4_Maxim_MAX66020.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO14443_4_Maxim_MAX66040.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO14443_4_MIFARE_Plus.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL1.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL2.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO14443_4_MIFARE_Plus_SL3.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO14443_4_MIFARE_DESFire.h"
#endif
#if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_STM.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_STM_LRI2K.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_STM_LRIS2K.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_STM_LRIS64K.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_STM_M24LR64R.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_STM_M24LRxxER.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_NXP.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_NXP_ICODE_SLI.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_NXP_ICODE_SLIX.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_L.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_NXP_ICODE_SLIX_L.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_NXP_ICODE_SLI_S.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_NXP_ICODE_SLIX_S.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_Infineon.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_Infineon_my_d.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_TI.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_TI_Tag_it_HFI_Pro.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_TI_Tag_it_HFI_Plus.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_Fujitsu.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_EM.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_EM_4034.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_KSW.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_Maxim.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_Maxim_MAX66100.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_Maxim_MAX66120.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_Maxim_MAX66140.h"
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_ISO15693_IDS_SL13A.h"
#endif
#endif // #if !defined(_FEDM_NO_TAG_HANDLER)

#endif // _FEDM_ISC_TAG_HANDLER_INCLUDES_H_INCLUDED_

