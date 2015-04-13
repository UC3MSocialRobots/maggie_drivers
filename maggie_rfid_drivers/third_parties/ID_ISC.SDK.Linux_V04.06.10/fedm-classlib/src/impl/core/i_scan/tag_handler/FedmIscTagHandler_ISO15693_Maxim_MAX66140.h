/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO15693_Maxim_MAX66140.h		|
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
Begin        		:	20.09.2010

Version       		:	03.03.02 / 29.04.2011 / M. Raner, M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID i-scan® HF transponder ISO15693 of manufacturer MAXIM

NOTE				:	this class supports selected, addressed and Nonaddressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_TAG_HANDLER_ISO15693_MAXIM_MAX66140_H_INCLUDED_)
#define _FEDM_ISC_TAG_HANDLER_ISO15693_MAXIM_MAX66140_H_INCLUDED_

#if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)

#include "../../FEDM.h"
#include "FedmIscTagHandler_ISO15693_Maxim.h"

class FEDM_ISOTabItem;

/// @brief Transponder class providing manufacturer specific commands for ISO 15693 compliant Transponders of type MAX66140 from manufacturer Maxim
///
/// <b>Description</b><br>
/// High-level class for easy-to-use Transponder specific communication. See OBID::FEDM::Core::ITagGroup for additional information.<br>
/// All methods are similiar to specific commands of MAX66140. Thus, you must refer to the Technical Datasheets of the MAX66140 to get detailed information about the methods and the parameters.<br>
///
/// @note Programmers should be familiar with the ISO 15693 standard
class _FEDM_ISC_CORE_EXT_CLASS FedmIscTagHandler_ISO15693_Maxim_MAX66140 : public FedmIscTagHandler_ISO15693_Maxim
{
public:

	// friend classes have access rights to protected members
	friend class FEDM_ISCReaderModule;
	friend class FEDM_ISCReader;
	friend class FEDM_ISOTabItem;

	// set all data members to 0
	//virtual void Init();

	int UserComputePageMAC(			unsigned char* pucSecret, 
									unsigned char* pucMemoryPageData, 
									unsigned char ucPageNumber, 
									unsigned char* pucUID,
									unsigned char* pucMAC);

	int	UserComputeSecret(			unsigned char* pucSecret, 
									unsigned char* pucMemoryPageData, 
									unsigned char* pucPartialSecret, 
									unsigned char* pucNewSecret);

	int UserComputeCopyBufferMAC(	unsigned char* pucSecret, 
									unsigned char* pucMemoryPageData, 
									unsigned char* pucReadWriteBufferData, 
									unsigned char* pucIntegrityBytes,
									unsigned char pucPageNumber, 
									unsigned char* pucUID,
									unsigned char* pucMAC);

	int GetMemoryPageData(			unsigned char ucRequestFlags, 
									unsigned char ucPageNumber, 
									unsigned char& ucResponseFlags, 
									unsigned char* pucMemoryPageData);

	int UpdateChallenge();

	/// @brief [0xBF][0xA0]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This command is realized with the Reader command [0xBF] ISO Transparent Command.<br>
	int GenerateSecret(				unsigned char ucRequestFlags,
									unsigned char ucControlByte,
									unsigned char* pucPartialSecret,
									unsigned char& ucResponseFlags,
									unsigned char& ucPageCRCStatus);

	/// @brief [0xBF][0xA1]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This command is realized with the Reader command [0xBF] ISO Transparent Command.<br>
	int WriteBuffer(				unsigned char ucRequestFlags,
									unsigned char ucBlockNumber,
									unsigned char* pucNewBlockData,
									unsigned char& ucResponseFlags);

	/// @brief [0xBF][0xA2]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This command is realized with the Reader command [0xBF] ISO Transparent Command.<br>
	int ReadBuffer(					unsigned char ucRequestFlags,
									unsigned char& pucResponseFlags,
									unsigned char& ucBlockNumber,
									unsigned char* pucReadWriteBufferData);

	/// @brief [0xBF][0xA3]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This command is realized with the Reader command [0xBF] ISO Transparent Command.<br>
	int CopyBuffer(					unsigned char ucRequestFlags,
									unsigned char ucBlockNumber,
									unsigned char* pucMAC,
									unsigned char& ucResponseFlags,
									unsigned char& ucPageCRCStatus);

	/// @brief [0xBF][0x20]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This command is realized with the Reader command [0xBF] ISO Transparent Command.<br>
	int ReadSingleBlock(			unsigned char ucRequestFlags,
									unsigned char ucBlockNumber,
									unsigned char& ucResponseFlags,
									unsigned char& ucSecurityStatus,
									unsigned char* pucMemoryData);

	/// @brief [0xBF][0xA4]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This command is realized with the Reader command [0xBF] ISO Transparent Command.<br>
	int CustomReadBlock(			unsigned char ucRequestFlags,
									unsigned char ucBlockNumber,
									unsigned char& ucResponseFlags,
									unsigned char& ucSecurityStatus,
									unsigned char* pucMemoryData,
									unsigned char* pucIntegrityBytes);

	/// @brief [0xBF][0xA5]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This command is realized with the Reader command [0xBF] ISO Transparent Command.<br>
	int ComputePageMAC(				unsigned char ucRequestFlags,
									unsigned char ucPageNumber,
									unsigned char& ucResponseFlags,
									unsigned char& ucPageCRCStatus,
									unsigned char* pucMAC);

protected:
	// constructor
	FedmIscTagHandler_ISO15693_Maxim_MAX66140(FEDM_ISCReader* pReader, FEDM_ISOTabItem* pTabItem);
	// 2. constructor
	FedmIscTagHandler_ISO15693_Maxim_MAX66140(FEDM_ISCReader* pReader, unsigned int uiTagHandlerType, FEDM_ISOTabItem* pTabItem);
	// destructor
	virtual ~FedmIscTagHandler_ISO15693_Maxim_MAX66140();

	int ComputeMAC(unsigned char* ucM, unsigned int* uiMAC);

	unsigned char m_ucRequestData[64];
	unsigned char m_ucResponseData[256];

	unsigned char ucChallenge[5];

};

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)

#endif // !defined(_FEDM_ISC_TAG_HANDLER_ISO15693_MAXIM_MAX66140_H_INCLUDED_)