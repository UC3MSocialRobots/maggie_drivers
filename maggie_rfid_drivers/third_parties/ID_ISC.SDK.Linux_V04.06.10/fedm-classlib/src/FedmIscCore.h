/*-------------------------------------------------------
|                                                       |
|                       FedmIscCore.h                   |
|                                                       |
---------------------------------------------------------

Copyright © 2008-2014	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	21.04.2008
Version       		:	04.06.08 / 27.10.2014 / M. Hultsch

Operation Systems	:	independent


This file includes all dependencies for the component FEDM


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#if !defined(_FEDM_ISC_CORE_H_INCLUDED_)
#define _FEDM_ISC_CORE_H_INCLUDED_


#if defined(__APPLE__)
	// limitations for Mac OSX
	#define _FEDM_NO_COM_SUPPORT
	#define _FEDM_NO_FU_SUPPORT
	#define _FEDM_NO_PD_SUPPORT
#endif

#if defined(__ANDROID__)
	// limitations for Android
	#define _FEDM_NO_FU_SUPPORT
	#define _FEDM_NO_PD_SUPPORT
#endif

#if !defined(_FEDM_NO_XML_SUPPORT)
  #if !defined(_FEDM_XML_SUPPORT)
	#define _FEDM_XML_SUPPORT
  #endif
#endif

#if !defined(_FEDM_NO_DLL)
	#define _FEDM_DLL
#endif

#if !defined(_FEDM_NO_COM_SUPPORT)
  #if !defined(_FEDM_COM_SUPPORT)
	#define _FEDM_COM_SUPPORT
  #endif
#endif

#if !defined(_FEDM_NO_USB_SUPPORT)
  #if !defined(_FEDM_USB_SUPPORT)
	#define _FEDM_USB_SUPPORT
  #endif
#endif

#if !defined(_FEDM_NO_TCP_SUPPORT)
  #if !defined(_FEDM_TCP_SUPPORT)
	#define _FEDM_TCP_SUPPORT
  #endif
#endif

#if !defined(_FEDM_NO_TAG_HANDLER)
  #if !defined(_FEDM_TAG_HANDLER)
	#define _FEDM_TAG_HANDLER
  #endif
#endif

#if !defined(_FEDM_NO_FU_SUPPORT)
  #if !defined(_FEDM_FU_SUPPORT)
	#define _FEDM_FU_SUPPORT
  #endif
#endif

#if defined(_MSC_VER)
  #if !defined(_FEDM_WINDOWS)
	#define _FEDM_WINDOWS
  #endif
  #if !defined(_FEDM_NO_MFC_SUPPORT)
	#if !defined(_FEDM_MFC_SUPPORT)
	  #define _FEDM_MFC_SUPPORT
	#endif
  #endif
#elif  defined(__BORLANDC__)
  #if !defined(_FEDM_WINDOWS)
	#define _FEDM_WINDOWS
  #endif
#else
  #if !defined(_FEDM_LINUX)
	#define _FEDM_LINUX
  #endif
#endif


// header files of implementation (and old API)
#include "impl/core/FEDM.h"
#include "impl/core/FEDM_Base.h"
#include "impl/core/FEDM_DataBase.h"
#include "impl/core/FEDM_Logger.h"
#include "impl/core/FEDM_Functions.h"
#include "impl/core/FEDM_Xml.h"
#include "impl/core/FEDM_XmlBase.h"
#include "impl/core/FEDM_Xml.h"
#include "impl/core/FEDM_XmlReaderCfgDataModul.h"
#include "impl/core/FEDM_XmlReaderCfgProfileModul.h"
#include "impl/core/i_scan/FEDM_ISC.h"
#include "impl/core/i_scan/FEDM_ISCReaderInfo.h"
#include "impl/core/i_scan/FEDM_ISCReaderDiagnostic.h"
#include "impl/core/i_scan/FEDM_ISCReaderID.h"
#include "impl/core/i_scan/FEDM_ISOTabItem.h"
#include "impl/core/i_scan/FEDM_BRMTabItem.h"
#include "impl/core/i_scan/FEDM_ISCReader.h"
#include "impl/core/i_scan/FEDM_ISCReaderModule.h"
#include "impl/core/i_scan/FEDM_ISCReaderConfig.h"
#include "impl/core/i_scan/FEDM_ISO_IEC_7816_6_ICManufacturerRegistration.h"
#include "impl/core/i_scan/utility/FedmIscReport_ReaderInfo.h"
#include "impl/core/i_scan/utility/FedmIscReport_ReaderDiagnostic.h"
#if !defined(_FEDM_NO_PD_SUPPORT)
	#include "impl/core/i_scan/peripheral_devices/FedmIscPeripheralDevice.h"
	#include "impl/core/i_scan/peripheral_devices/FedmIscPeopleCounter.h"
	#include "impl/core/i_scan/peripheral_devices/FedmIscExternalIO.h"
#endif
#if !defined(_FEDM_NO_FU_SUPPORT)
	#include "impl/core/i_scan/function_unit/FEDM_ISCFunctionUnit.h"
	#include "impl/core/i_scan/function_unit/FEDM_ISCFunctionUnitID.h"
#endif
#if !defined(_FEDM_NO_TAG_HANDLER)
	#include "impl/core/i_scan/tag_handler/FedmIscTagHandler_Includes.h"
#endif // #if !defined(_FEDM_NO_TAG_HANDLER)

#if !defined(_FEDM_NEW_API)
	using namespace CoreImpl;
#endif

#endif // _FEDM_ISC_CORE_H_INCLUDED_
