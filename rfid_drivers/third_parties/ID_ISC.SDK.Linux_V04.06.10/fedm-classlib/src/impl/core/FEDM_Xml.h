/*-------------------------------------------------------
|                                                       |
|                       FEDM_Xml.h	                    |
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
Begin        		:	01.04.2003

Version       		:	04.02.04 / 20.06.2012 / M. Hultsch

Operation Systems	:	independent

Function			:	basic defines and types XML document


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/



#if !defined(_FEDM_XML_H_INCLUDED_)
#define _FEDM_XML_H_INCLUDED_

#ifdef _MSC_VER
	// The (microsoft) debugger can't handle symbols more than 255 characters long.
	// STL often creates symbols longer than that.
	// When symbols are longer than 255 characters, the warning is disabled.
	#pragma warning(disable:4786)	// for MFC
#endif


#ifdef _FEDM_XML_SUPPORT


#include <string>
#include <vector>
#include <map>


// STL - Standard Template Library
using namespace std;




//#####################################################################################
// constants
//#####################################################################################

#define FEDM_MAX_NAME_SIZE				   64 // maximal size of a C-string based name
#define FEDM_MAX_TEXT_SIZE				  512 // maximal size of a C-string based text


// UNICODE strings for XML header
#define FEDM_XML_STRING_XML_VERSION_UC			L"1.0"		// XML version
#define FEDM_XML_STRING_XML_ENCODING_UC			L"utf-8"	// pure ASCII chars
#define FEDM_XML_STRING_XML_STANDALONE_UC		L"yes"		// without external DTD
												
// UNICODE strings for file header				
#define FEDM_XML_TAG_VENDOR_UC					L"vendor"
#define FEDM_XML_TAG_DOC_TYPE_UC				L"document-type"
#define FEDM_XML_TAG_DOC_VERSION_UC				L"document-version"
#define FEDM_XML_TAG_READER_FAM_UC				L"reader-family"
#define FEDM_XML_TAG_READER_NAME_UC				L"reader-name"
#define FEDM_XML_TAG_READER_TYPE_UC				L"reader-type"
#define FEDM_XML_TAG_HARDWARE_TYPE_UC			L"hardware-type"
#define FEDM_XML_TAG_COMMENT_UC					L"comment"
#define FEDM_XML_TAG_PRG_NAME_UC				L"program-name"
#define FEDM_XML_TAG_PRG_VERSION_UC				L"program-version"
#define FEDM_XML_TAG_FEDM_VERSION_UC			L"fedm-version"
#define FEDM_XML_TAG_DATE_UC					L"date"
#define FEDM_XML_TAG_TIME_UC					L"time"
#define FEDM_XML_TAG_BUSADR_UC					L"bus-address"
#define FEDM_XML_TAG_COMPORT_UC					L"com-port"
#define FEDM_XML_TAG_DEVICE_ID_UC				L"device-id"
#define FEDM_XML_TAG_HOSTADR_UC					L"host-address"
#define FEDM_XML_TAG_PORTNR_UC					L"port-number"
#define FEDM_XML_TAG_PORT_TYPE_UC				L"port-type"
#define FEDM_XML_TAG_COMM_MODE_UC				L"communication-mode"
#define FEDM_XML_TAG_FRAME_SUPPORT				L"protocol-frame-support"
#define FEDM_XML_TAG_READER_UC					L"reader"
#define FEDM_XML_TAG_READER_INFO_UC				L"reader-info"
#define FEDM_XML_TAG_CUSTOMER_ID_UC				L"customer-id"
#define FEDM_XML_TAG_FIRMWARE_VERSION_UC		L"firmware-version"
#define FEDM_XML_TAG_READER_FUNCTIONS_UC		L"reader-functions"
#define FEDM_XML_TAG_HF_READER_FUNCTIONS_UC		L"hf-reader-functions"
#define FEDM_XML_TAG_UHF_READER_FUNCTIONS_UC	L"uhf-reader-functions"
#define FEDM_XML_TAG_INPUT_UC					L"input"
#define FEDM_XML_TAG_OUTPUT_UC					L"output"
#define FEDM_XML_TAG_TRANSPONDER_DRIVER_UC		L"transponder-driver"
#define FEDM_XML_TAG_HF_TRANSPONDER_DRIVER_UC	L"hf-transponder-driver"
#define FEDM_XML_TAG_UHF_TRANSPONDER_DRIVER_UC	L"uhf-transponder-driver"
#define FEDM_XML_TAG_FREQUENCY_UC				L"frequency"
#define FEDM_XML_TAG_REGION_UC					L"region"
#define FEDM_XML_TAG_FIRMWARE_IDENTIFIER_UC		L"firmware-identifier"
#define FEDM_XML_TAG_FIRMWARE_UPGRADE_UC		L"firmware-upgrade"
#define FEDM_XML_TAG_UNLOCK_KEY_UC				L"unlock-key"
#define FEDM_XML_TAG_CONTROLLER_UC				L"controller"
#define FEDM_XML_TAG_CONTROLLER_TYPE_UC			L"controller-type"
#define FEDM_XML_TAG_CONTROLLER_SUB_TYPE_UC		L"controller-sub-type"
#define FEDM_XML_TAG_BANK_NO_UC					L"bank-number"
#define FEDM_XML_TAG_MANUFACTURER_ID_UC			L"manufacturer-id"
#define FEDM_XML_TAG_CHIP_TYPE_ID_UC			L"chip-type-id"
#define FEDM_XML_TAG_REV_CODE_UC				L"revision-code"
#define FEDM_XML_TAG_FLASHSIZE_UC				L"flashsize"
#define FEDM_XML_TAG_CRC_UC						L"checksum"
#define FEDM_XML_TAG_OPTIONAL_UC				L"optional"
#define FEDM_XML_TAG_OPTIONAL_INFO_UC			L"optional-info"
#define FEDM_XML_TAG_DECODER_INFO_UC			L"decoder-info"
#define FEDM_XML_TAG_SELF_TEST_UC				L"self-test"
#define FEDM_XML_TAG_ADD_FIRMWARE_INFO_UC		L"additional-firmware-info"
#define FEDM_XML_TAG_ADD_HARDWARE_INFO_UC		L"additional-hardware-info"
#define FEDM_XML_TAG_TEMPLATE_UC				L"template"
#define FEDM_XML_TAG_FUNCTION_LIST_UC			L"function-list"
#define FEDM_XML_TAG_IDENTIFIER_UC				L"identifier"
#define FEDM_XML_TAG_HW_INFO_UC					L"hw-info"
#define FEDM_XML_TAG_AHW_INFO_UC				L"ahw-info"
#define FEDM_XML_TAG_DHW_INFO_UC				L"dhw-info"
#define FEDM_XML_TAG_DEVICE_INFO_UC				L"device-info"
#define FEDM_XML_TAG_RX_BUFFER_SIZE_UC			L"rx-buffer-size"
#define FEDM_XML_TAG_TX_BUFFER_SIZE_UC			L"tx-buffer-size"
#define FEDM_XML_TAG_CFG_RESET_UC				L"cfg-reset"
#define FEDM_XML_TAG_RFC_RESET_UC				L"rfc-reset"
#define FEDM_XML_TAG_ACC_RESET_UC				L"acc-reset"

												
// UNICODE strings for standard tags			
#define FEDM_XML_TAG_OBID_UC					L"OBID"
#define FEDM_XML_TAG_FILE_HDR_UC				L"file-header"
#define FEDM_XML_TAG_DATA_ARRAY_UC				L"data-array"
#define FEDM_XML_TAG_CFG_UC						L"CFG"
#define FEDM_XML_TAG_PARTITION_UC				L"partition"
#define FEDM_XML_TAG_DATA_UC					L"data"
#define FEDM_XML_TAG_DATA_LENGTH_UC				L"data-length"

// UNICODE strings for standard attributes
#define FEDM_XML_ATTRIB_NAME_UC					L"name"
#define FEDM_XML_ATTRIB_BLOCKS_UC				L"blocks"
#define FEDM_XML_ATTRIB_SIZE_UC					L"size"
#define FEDM_XML_ATTRIB_DEVICE_ID_UC			L"device-id"
#define FEDM_XML_ATTRIB_TYPE_UC					L"type"

// UNICODE strings for standard attribute values
#define FEDM_XML_STRING_EEPROM_DATA_UC			L"Reader EEPROM-Parameter"
#define FEDM_XML_STRING_RAM_DATA_UC				L"Reader RAM-Parameter"
#define FEDM_XML_STRING_ACC_EEPROM_DATA_UC		L"Reader EEPROM-Parameter for ACC"
#define FEDM_XML_STRING_ACC_RAM_DATA_UC			L"Reader RAM-Parameter for ACC"
#define FEDM_XML_STRING_TMP_DATA_UC				L"Temporary Protocol Data"
#define FEDM_XML_STRING_MJP_DATA_UC				L"Temporary Multijob Protocol Data"
#define FEDM_XML_STRING_SN_DATA_UC				L"Transponder Serial-Number"
#define FEDM_XML_STRING_ID_DATA_UC				L"Transponder ID-Number"
#define FEDM_XML_STRING_AC_DATA_UC				L"Transponder Account"
#define FEDM_XML_STRING_PUB_MEM_UC				L"Transponder Public Memory"
#define FEDM_XML_STRING_SEC_MEM_UC				L"Transponder Secret Memory"
#define FEDM_XML_STRING_CFG_MEM_UC				L"Transponder Configuration Memory"
#define FEDM_XML_STRING_DATE_MEM_UC				L"Transponder Date Memory"

// UNICODE strings for communication mode values
#define FEDM_XML_STRING_COMM_MODE_SERIAL_UC		L"Serial"
#define FEDM_XML_STRING_COMM_MODE_USB_UC		L"USB"
#define FEDM_XML_STRING_COMM_MODE_TCP_UC		L"TCP"

// UNICODE strings for file header
#define FEDM_XML_STRING_DOC_TYPE_1_UC			L"Reader Configuration File"
#define FEDM_XML_STRING_DOC_TYPE_2_UC			L"Reader Firmware Upgrade File"
#define FEDM_XML_STRING_DOC_TYPE_3_UC			L"Reader Firmware Update File"
#define FEDM_XML_STRING_DOC_TYPE_4_UC			L"Reader Profile File"
#define FEDM_XML_STRING_DOC_VERSION_UC				L"1.0"		// document version
#define FEDM_XML_STRING_PROFILE_DOC_VERSION_UC		L"1.0"		// document version
#define FEDM_XML_STRING_FWUG_DOC_VERSION_UC			L"2.0"		// document version
#define FEDM_XML_STRING_FWUPD_DOC_VERSION_UC		L"1.3"		// document version
#define FEDM_XML_STRING_FWUPD_REQ_DOC_VERSION_UC	L"1.2"		// required document version

// strings in file header
#define FEDM_XML_STRING_DOC_TYPE_1				"Reader Configuration File"
#define FEDM_XML_STRING_DOC_TYPE_2				"Reader Firmware Upgrade File"
#define FEDM_XML_STRING_DOC_TYPE_3				"Reader Firmware Update File"
#define FEDM_XML_STRING_DOC_TYPE_4				"Reader Profile File"
#define FEDM_XML_STRING_DOC_VERSION					"1.0"		// document version
#define FEDM_XML_STRING_PROFILE_DOC_VERSION			"1.0"		// document version
#define FEDM_XML_STRING_FWUG_DOC_VERSION			"2.0"		// document version
#define FEDM_XML_STRING_FWUPD_DOC_VERSION			"1.3"		// document version
#define FEDM_XML_STRING_FWUPD_REQ_DOC_VERSION		"1.2"		// required document version
#define FEDM_XML_STRING_COMM_MODE_SERIAL		"Serial"
#define FEDM_XML_STRING_COMM_MODE_USB			"USB"
#define FEDM_XML_STRING_COMM_MODE_TCP			"TCP"


//#####################################################################################
// structures
//#####################################################################################

typedef struct _FEDM_XML_ATTRIBUTE
{
	wstring sAttribute;					// tag's attribute name
	wstring sValue;						// tag's attribute value

} FEDM_XML_ATTRIBUTE;

		
typedef struct _FEDM_XML_TREEITEM
{
	wstring sTag;						// tag's name
	wstring sValue;						// tag's value
	vector<_FEDM_XML_ATTRIBUTE*> aAttribList;	// tag's attribute list

	vector<_FEDM_XML_TREEITEM*> aChildList;		// child list

	// options
	bool bNewLine;					// after start tag insert a new line L"\n"
	bool bEmpty;					// tag without value

} FEDM_XML_TREEITEM;



//#####################################################################################
// datatypes
//#####################################################################################
		
typedef vector<FEDM_XML_TREEITEM*>				FEDM_XML_TREE_LIST;
typedef vector<FEDM_XML_TREEITEM*>::iterator	FEDM_XML_TREE_LIST_ITOR;
typedef vector<FEDM_XML_ATTRIBUTE*>::iterator	FEDM_XML_ATTRIBUTE_LIST_ITOR;



#endif // #ifdef _FEDM_XML_SUPPORT

#endif // !defined(_FEDM_XML_H_INCLUDED_)
