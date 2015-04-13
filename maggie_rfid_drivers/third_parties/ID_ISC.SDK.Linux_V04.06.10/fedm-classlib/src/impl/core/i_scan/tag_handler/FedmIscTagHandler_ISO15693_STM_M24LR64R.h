/*-------------------------------------------------------
|                                                       |
|	   FedmIscTagHandler_ISO15693_STM_M24LR64R.h        |
|                                                       |
---------------------------------------------------------

Copyright © 2009-2014	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	25.11.2009

Version       		:	04.06.01 / 07.03.2014 / M. Hultsch
						- new methods: LockMultipleBlocks and GetMultipleBlockSecurityStatus
						- methods ReadMultipleBlocks and WriteMultipleBlocks now virtual

						03.03.00 / 10.02.2011 / M. Hultsch
						
						Operation Systems	:	independent

Function			:	class for OBID i-scan® HF transponder ISO15693 of manufacturer STMicroelectronics SA
						specialized tag handler on top with custom specific commands

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_TAG_HANDLER_ISO15693_STM_M24LR64R_H_INCLUDED_)
#define _FEDM_ISC_TAG_HANDLER_ISO15693_STM_M24LR64R_H_INCLUDED_

#if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)

#include "../../FEDM.h"
#include "FedmIscTagHandler_ISO15693_STM.h"

class FEDM_ISOTabItem;



//#####################################################################################
// constants
//#####################################################################################





//####################################################################
// class FedmIscTagHandler_ISO15693_STM_M24LR64R
// >> supports only ISO Host Commands <<
//####################################################################

/// @brief Transponder class providing manufacturer specific commands for ISO 15693 compliant Transponders of type M24LR64-R from manufacturer STMicroelectronics SA
///
/// <b>Description</b><br>
/// High-level class for easy-to-use Transponder specific communication. See OBID::FEDM::Core::ITagGroup for additional information.<br>
/// All methods are similiar to specific commands of M24LR64-R. Thus, you must refer to the Technical Datasheet of the M24LR64-R to get detailed information about the methods and the parameters.<br>
/// Secondly, all methods of this class are Custom specific commands, realized with the Reader command [0xB1], which are described in the Application Note <i>STMicroelectronics Custom and Proprietary Commands.pdf</i>
///
/// @note Programmers should be familiar with the ISO 15693 standard
class _FEDM_ISC_CORE_EXT_CLASS FedmIscTagHandler_ISO15693_STM_M24LR64R : public FedmIscTagHandler_ISO15693_STM
{
public:

	// friend classes have access rights to protected members
	friend class FEDM_ISCReaderModule;
	friend class FEDM_ISCReader;
	friend class FEDM_ISOTabItem;

//	virtual void Init();					// set all data members to 0

	/// @brief Basic [0xB0][0x23] Read Multiple Blocks command with additional security status
	///
	/// @param[in] uiFirstDataBlock First data block address (0 ... 65535)
	/// @param[in] uiNoOfDataBlocks Number of requested data blocks
	/// @param[out] uiBlockSize Number of bytes in each data block
	/// @param[out] pucSecurityStatus Array with received status information, one byte for each data block. The array must have the dimension of uiNoOfDataBlocks
	/// @param[out] pucData Pointer to array for received data blocks. The array must have the dimension of uiNoOfDataBlocks x Expected Block Size
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Basic [0xB0][0x23] Read Multiple Blocks command with additional security status<br>
	/// - With support for extended address mode<br>
	virtual int ReadMultipleBlocks(	unsigned int uiFirstDataBlock,
									unsigned int uiNoOfDataBlocks,
									unsigned int& uiBlockSize,
									unsigned char* pucSecurityStatus,
									unsigned char* pucData );

	/// @brief Basic [0xB0][0x24] Write Multiple Blocks command
	///
	/// @param[in] uiFirstDataBlock First data block address (0 ... 65535)
	/// @param[in] uiNoOfDataBlocks Number of data blocks to transmit to
	/// @param[in] uiBlockSize Number of bytes in each data block
	/// @param[in] pucData Pointer to array with data blocks to transmit to
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Basic [0xB0][0x24] Write Multiple Blocks command<br>
	/// - With support for extended address mode<br>
	virtual int WriteMultipleBlocks(unsigned int uiFirstDataBlock,
									unsigned int uiNoOfDataBlocks,
									unsigned int uiBlockSize,
									unsigned char* pucData );


	/// @brief [0xB0][0x22] : Lock Multiple Blocks command
	///
	/// @param[in] uiFirstDataBlock First data block address
	/// @param[in] uiNoOfDataBlocks Number of data blocks to be locked to
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Command with support for extended address mode<br>
	/// See Reader&apos;s System Manual for detailed information about the command and the method parameters
	virtual int LockMultipleBlocks(	unsigned int uiFirstDataBlock,
									unsigned int uiNoOfDataBlocks);

	/// @brief [0xB0][0x2C] : Get Multiple Block Security Status command
	///
	/// @param[in] uiFirstDataBlock First data block address
	/// @param[in] uiNoOfDataBlocks Number of data blocks to be locked to
	/// @param[out] pucSecurityStatus Array with received status information, one byte for each data block. The array must have the dimension of uiNoOfDataBlocks.
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Command with support for extended address mode<br>
	/// See Reader&apos;s System Manual for detailed information about the command and the method parameters
	virtual int GetMultipleBlockSecurityStatus(	unsigned int uiFirstDataBlock,
												unsigned int uiNoOfDataBlocks,
												unsigned char* pucSecurityStatus );

	/// @brief [0xB1][0xB1] Write Sector Password
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>STMicroelectronics Custom and Proprietary Commands.pdf</i>, where all parameters are described in detail.
	int WriteSectorPassword(unsigned char ucPasswordNumber, string sPassword);

	/// @brief [0xB1][0xB2] Lock Sector Password
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>STMicroelectronics Custom and Proprietary Commands.pdf</i>, where all parameters are described in detail.
	int LockSectorPassword(unsigned char ucSectorNumber, unsigned char ucSectorSecurityStatus);	

	/// @brief [0xB1][0xB3] Present Sector Password
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>STMicroelectronics Custom and Proprietary Commands.pdf</i>, where all parameters are described in detail.
	int PresentSectorPassword(unsigned char ucPasswordNumber, string sPassword);

protected:
	// constructor
	FedmIscTagHandler_ISO15693_STM_M24LR64R(FEDM_ISCReader* pReader, FEDM_ISOTabItem* pTabItem);
	// 2. constructor
	FedmIscTagHandler_ISO15693_STM_M24LR64R(FEDM_ISCReader* pReader, unsigned int uiTagHandlerType, FEDM_ISOTabItem* pTabItem);
	// destructor
	virtual ~FedmIscTagHandler_ISO15693_STM_M24LR64R();

};

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)

#endif // !defined(_FEDM_ISC_TAG_HANDLER_ISO15693_STM_M24LR64R_H_INCLUDED_)
