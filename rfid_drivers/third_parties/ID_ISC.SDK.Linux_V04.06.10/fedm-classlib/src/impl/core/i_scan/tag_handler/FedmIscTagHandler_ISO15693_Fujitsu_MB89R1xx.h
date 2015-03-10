/*-------------------------------------------------------
|                                                       |
|	   FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx.h    |
|                                                       |
---------------------------------------------------------

Copyright © 2009-2010	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	02.10.2009

Version       		:	03.01.00 / 10.02.2010 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID i-scan® HF transponder ISO15693 of manufacturer Fujitsu Limited
						specialized tag handler on top with custom specific commands

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_TAG_HANDLER_ISO15693_FUJITSU_MB89R1xx_H_INCLUDED_)
#define _FEDM_ISC_TAG_HANDLER_ISO15693_FUJITSU_MB89R1xx_H_INCLUDED_

#if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)

#include "../../FEDM.h"
#include "FedmIscTagHandler_ISO15693_Fujitsu.h"

class FEDM_ISOTabItem;



//#####################################################################################
// constants
//#####################################################################################





//####################################################################
// class FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx
// >> supports only ISO Host Commands <<
//####################################################################

/// @brief Transponder class providing manufacturer specific commands for ISO 15693 compliant Transponders of type MB89R1xx series from manufacturer Fujitsu Limited
///
/// <b>Description</b><br>
/// High-level class for easy-to-use Transponder specific communication. See OBID::FEDM::Core::ITagGroup for additional information.<br>
/// All methods are similiar to the specific commands of the MB89R1xx series. Thus, you must refer to the Technical Datasheet of the MB89R1xx to get detailed information about the methods and the parameters.<br>
/// Secondly, all methods of this class are Custom specific commands, realized with the Reader command [0xB1], which are described in the Application Note <i>Fujitsu Custom and Proprietary Commands.pdf</i>
///
/// @note Programmers should be familiar with the ISO 15693 standard
class _FEDM_ISC_CORE_EXT_CLASS FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx : public FedmIscTagHandler_ISO15693_Fujitsu
{
public:

	// friend classes have access rights to protected members
	friend class FEDM_ISCReaderModule;
	friend class FEDM_ISCReader;
	friend class FEDM_ISOTabItem;

	/// @brief [0xB1][0xA0] EAS
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
    /// This command is realized with the Reader command [0xB1] ISO Custom and Proprietary Commands.<br>
	/// Refer to the Application Note <i>Fujitsu Custom and Proprietary Commands.pdf</i>, where all parameters are described in detail.<br>
	int EAS();

	/// @brief [0xB1][0xA1] Write EAS
	///
	/// @param ucData One byte to specify the EAS command
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
    /// This command is realized with the Reader command [0xB1] ISO Custom and Proprietary Commands.<br>
	/// Refer to the Application Note <i>Fujitsu Custom and Proprietary Commands.pdf</i>, where all parameters are described in detail.<br>
	int WriteEAS(unsigned char ucData);

	/// @brief [0xB1][0xA2] Refresh
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
    /// This command is realized with the Reader command [0xB1] ISO Custom and Proprietary Commands.<br>
	/// Refer to the Application Note <i>Fujitsu Custom and Proprietary Commands.pdf</i>, where all parameters are described in detail.<br>
	int Refresh();

	/// @brief [0xB1][0xA6] Kill
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
    /// This command is realized with the Reader command [0xB1] ISO Custom and Proprietary Commands.<br>
	/// Refer to the Application Note <i>Fujitsu Custom and Proprietary Commands.pdf</i>, where all parameters are described in detail.<br>
	int Kill();

//	virtual void Init();					// set all data members to 0

protected:
	// constructor
	FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx(FEDM_ISCReader* pReader, FEDM_ISOTabItem* pTabItem);
	// 2. constructor
	FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx(FEDM_ISCReader* pReader, unsigned int uiTagHandlerType, FEDM_ISOTabItem* pTabItem);
	// destructor
	virtual ~FedmIscTagHandler_ISO15693_Fujitsu_MB89R1xx();

};

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)

#endif // !defined(_FEDM_ISC_TAG_HANDLER_ISO15693_FUJITSU_MB89R1xx_H_INCLUDED_)
