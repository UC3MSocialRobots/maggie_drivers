/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL1.h   |
|                                                       |
---------------------------------------------------------

Copyright � 2009-2010	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	24.08.2009

Version       		:	03.01.00 / 10.02.2010 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID� classic-pro transponder ISO14443-3 MIFARE Plus in Security Level 1

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID�, OBID i-scan� and OBID myAXXESS� are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_3_MIFARE_PLUS_SL1_H_INCLUDED_)
#define _FEDM_ISC_TAG_HANDLER_ISO14443_3_MIFARE_PLUS_SL1_H_INCLUDED_

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "../../FEDM.h"
#include "FedmIscTagHandler_ISO14443_3_MIFARE_Plus.h"

class FEDM_ISOTabItem;



//#####################################################################################
// constants
//#####################################################################################





//####################################################################
// class FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL1
// >> supports only ISO Host Commands <<
//####################################################################

/// @brief Transponder class providing manufacturer specific commands for ISO 14443-3 compliant Transponders of type MIFARE Plus in Security Level 1 from manufacturer NXP Semiconductors
///
/// <b>Description</b><br>
/// High-level class for easy-to-use Transponder specific communication. See OBID::FEDM::Core::ITagGroup for additional information.<br>
/// This class implements the communication with [0xC2] Reader commands.<br>
/// Detailed description about the command interface can be found in:
/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
///
/// @note Programmers should be familiar with the ISO 14443 standards
class _FEDM_ISC_CORE_EXT_CLASS FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL1 : public FedmIscTagHandler_ISO14443_3_MIFARE_Plus
{
public:

	// friend classes have access rights to protected members
	friend class FEDM_ISCReaderModule;
	friend class FEDM_ISCReader;
	friend class FEDM_ISOTabItem;

//	virtual void Init();					// set all data members to 0


	/// @brief MIFARE Plus Command [0x77], realized with Reader command [0xC2][0x77] MFP_SL1_AESAuthent
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is a Security Level 1 command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int AESAuthent(	unsigned char ucReaderKeyIndex );


protected:
	// constructor
	FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL1(FEDM_ISCReader* pReader, FEDM_ISOTabItem* pTabItem);
	// 2. constructor
	FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL1(FEDM_ISCReader* pReader, unsigned int uiTagHandlerType, FEDM_ISOTabItem* pTabItem);
	// destructor
	virtual ~FedmIscTagHandler_ISO14443_3_MIFARE_Plus_SL1();

};

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#endif // !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_3_MIFARE_PLUS_SL1_H_INCLUDED_)
