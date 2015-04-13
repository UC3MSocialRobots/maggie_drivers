/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO14443_4_Maxim_MAX66020.h    |
|                                                       |
---------------------------------------------------------

Copyright © 2010-2011	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Marcel Raner
Begin        		:	04.10.2010

Version       		:	03.03.02 / 29.04.2011 / M. Raner, M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID® classic-pro transponder ISO14443 of manufacturer MAXIM

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_4_MAXIM_MAX66020_H_INCLUDED_)
#define _FEDM_ISC_TAG_HANDLER_ISO14443_4_MAXIM_MAX66020_H_INCLUDED_

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "../../FEDM.h"
#include "FedmIscTagHandler_ISO14443_4_Maxim.h"

class FEDM_ISOTabItem;




/// @brief Transponder class providing manufacturer specific commands for ISO 14443-4 compliant Transponders of type MAX66020 from manufacturer Maxim
///
/// <b>Description</b><br>
/// High-level class for easy-to-use Transponder specific communication. See OBID::FEDM::Core::ITagGroup for additional information.<br>
/// Detailed description about the command interface can be found in:
/// - Product Datasheet for MAX66020 from MAXIM
///
/// @note Programmers should be familiar with the ISO 14443 standards
class _FEDM_ISC_CORE_EXT_CLASS FedmIscTagHandler_ISO14443_4_Maxim_MAX66020 : public FedmIscTagHandler_ISO14443_4_Maxim
{
public:

	// friend classes have access rights to protected members
	friend class FEDM_ISCReaderModule;
	friend class FEDM_ISCReader;
	friend class FEDM_ISOTabItem;

	// set all data members to 0
	//	virtual void Init();

	/// @brief Command [0x20]: Read Single Block
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is realized with Reader command [0xB2][0xBE] : T=CL %APDU for synchronous call.<br>
	/// Detailed description about the command and the parameters can be found in:<br>
	/// - Product Datasheet for MAX66020 from MAXIM
	int ReadSingleBlock(					unsigned char ucBlockNumber,
											unsigned char& ucIndicator,
											unsigned char* pucMemoryData);

	/// @brief Command [0xB0]: Read Single Block with Block Security
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is realized with Reader command [0xB2][0xBE] : T=CL %APDU for synchronous call.<br>
	/// Detailed description about the command and the parameters can be found in:<br>
	/// - Product Datasheet for MAX66020 from MAXIM
	int ReadSingleBlockWithBlockSecurity(	unsigned char ucBlockNumber,
											unsigned char& ucIndicator,
											unsigned char& ucSecurityStatus,
											unsigned char* pucMemoryData);

	/// @brief Command [0xA4]: Custom Read Block
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is realized with Reader command [0xB2][0xBE] : T=CL %APDU for synchronous call.<br>
	/// Detailed description about the command and the parameters can be found in:<br>
	/// - Product Datasheet for MAX66020 from MAXIM
	int CustomReadBlock(					unsigned char ucBlockNumber,
											unsigned char& ucIndicator,
											unsigned char* pucMemoryData,
											unsigned char* pucIntegrityBytes);

	/// @brief Command [0x27]: Write AFI
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is realized with Reader command [0xB2][0xBE] : T=CL %APDU for synchronous call.<br>
	/// Detailed description about the command and the parameters can be found in:<br>
	/// - Product Datasheet for MAX66020 from MAXIM
	int WriteAFI(							unsigned char ucAfiValue,
											unsigned char& ucIndicator);

	/// @brief Command [0x28]: Lock AFI
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is realized with Reader command [0xB2][0xBE] : T=CL %APDU for synchronous call.<br>
	/// Detailed description about the command and the parameters can be found in:<br>
	/// - Product Datasheet for MAX66020 from MAXIM
	int LockAFI(							unsigned char& ucIndicator);

	/// @brief Command [0x21]: Write Single Block
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is realized with Reader command [0xB2][0xBE] : T=CL %APDU for synchronous call.<br>
	/// Detailed description about the command and the parameters can be found in:<br>
	/// - Product Datasheet for MAX66020 from MAXIM
	int WriteSingleBlock(					unsigned char ucBlockNumber,
											unsigned char* pucNewBlockData,
											unsigned char& ucIndicator);

	/// @brief Command [0x22]: Lock Block
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is realized with Reader command [0xB2][0xBE] : T=CL %APDU for synchronous call.<br>
	/// Detailed description about the command and the parameters can be found in:<br>
	/// - Product Datasheet for MAX66020 from MAXIM
	int LockBlock(							unsigned char ucBlockNumber,
											unsigned char& ucIndicator);


protected:
	// constructor
	FedmIscTagHandler_ISO14443_4_Maxim_MAX66020(FEDM_ISCReader* pReader, FEDM_ISOTabItem* pTabItem);
	// 2. constructor
	FedmIscTagHandler_ISO14443_4_Maxim_MAX66020(FEDM_ISCReader* pReader, unsigned int uiTagHandlerType, FEDM_ISOTabItem* pTabItem);
	// destructor
	virtual ~FedmIscTagHandler_ISO14443_4_Maxim_MAX66020();

	unsigned char m_ucRequestData[64];
	unsigned char m_ucResponseData[256];

};

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#endif // !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_MAX66020_4_MAXIM_H_INCLUDED_)
