/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO14443_4_MIFARE_DESFire.h    |
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
Begin        		:	30.03.2009

Version       		:	03.02.07 / 10.11.2010 / M. Hultsch

Operation Systems	:	independent

Function			:	interface class for OBID® classic-pro transponder ISO14443-4 MIFARE DESFire

NOTE				:	this class supports only selected mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_4_MIFARE_DESFIRE_H_INCLUDED_)
#define _FEDM_ISC_TAG_HANDLER_ISO14443_4_MIFARE_DESFIRE_H_INCLUDED_

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "../../FEDM.h"
#include "FedmIscTagHandler_ISO14443_4.h"
#include "FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1.h"
#include "FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3.h"
#include "FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto.h"
#include "FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto.h"
#include "FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto.h"

class FEDM_ISOTabItem;



//#####################################################################################
// constants
//#####################################################################################





//####################################################################
// class FedmIscTagHandler_ISO14443_4_MIFARE_DESFire
// >> supports only ISO Host Commands <<
//####################################################################

/// @brief Transponder class providing manufacturer specific commands for ISO 14443-4 compliant Transponders of type MIFARE DESFire from manufacturer NXP Semiconductors
///
/// <b>Description</b><br>
/// High-level class for easy-to-use Transponder specific communication. See OBID::FEDM::Core::ITagGroup for additional information.<br>
/// The support for MIFARE DESFire is limited to some OBID<SUP>&reg;</SUP> <i>classic-pro</i> Readers and realized with three different implementations:<br>
/// - Soft-Crypto, using [0xC1] High integrated commands for DESFire Communication
/// - Flexible Soft-Crypto, using [0xC3] Flexible commands for DESFire Communication
/// - SAM-Crypto, using [0xC3] Flexible commands for DESFire Communication with SAM
/// 
/// For more information about the pros and cons of the different commands get in contact with FEIG ELECTRONIC.<br>
/// Additionally, you have to order the Product Datasheet from NXP Semiconductors to become familiar with the features of the DESFire.<br>
/// <br>
/// The method interface of this class is very wide and complex. Due to the three different DESFire APIs in the Reader, this class maps this APIs with three different nested classes, 
/// each implementing the specialized method interface:
/// - [ISoftCrypto](@ref ISoftCrypto) for Soft-Crypto commands
/// - [IFlexSoftCrypto](@ref IFlexSoftCrypto) for Flexible Soft-Crypto commands
/// - [ISamCrypto](@ref ISamCrypto) for SAM-Crypto commands
///
/// @note Programmers should also be familiar with the ISO 14443 and ISO/IEC 7816 standards
class _FEDM_ISC_CORE_EXT_CLASS FedmIscTagHandler_ISO14443_4_MIFARE_DESFire : public FedmIscTagHandler_ISO14443_4
{
public:

	// friend classes have access rights to protected members
	friend class FEDM_ISCReaderModule;
	friend class FEDM_ISCReader;
	friend class FEDM_ISOTabItem;
	friend class FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1;
	friend class FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3;
	friend class FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto;
	friend class FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto;
	friend class FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto;


	/// @brief Initialization method
	///
	/// <b>Description</b><br>
	/// Clears all data members
	virtual void Init();

public:
	/// @brief Constant for Authentication
	///
	///
	static const unsigned int PLAIN_COMMUNICATION;
	/// @brief Constant for Authentication
	///
	///
	static const unsigned int PLAIN_SECURED_BY_MAC;
	/// @brief Constant for Authentication
	///
	///
	static const unsigned int FULLY_ENCIPHERED;

	/// @brief Nested interface implementing the Soft-Crypto commands
	///
	/// <b>Description</b><br>
	/// See FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto for detailed information about the method interface.
	FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SoftCrypto		ISoftCrypto;
	
	/// @brief Nested interface implementing the Flexible Soft-Crypto commands
	///
	/// <b>Description</b><br>
	/// See FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto for detailed information about the method interface.
	FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_FlexSoftCrypto	IFlexSoftCrypto;
	
	/// @brief Nested interface implementing the SAM-Crypto commands
	///
	/// <b>Description</b><br>
	/// See FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto for detailed information about the method interface.
	FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_SamCrypto		ISamCrypto;

protected:
	// constructor
	FedmIscTagHandler_ISO14443_4_MIFARE_DESFire(FEDM_ISCReader* pReader, 
												FEDM_ISOTabItem* pTabItem);
	// 2. constructor
	FedmIscTagHandler_ISO14443_4_MIFARE_DESFire(FEDM_ISCReader* pReader, 
												unsigned int uiTagHandlerType, 
												FEDM_ISOTabItem* pTabItem);
	// destructor
	virtual ~FedmIscTagHandler_ISO14443_4_MIFARE_DESFire();

	FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C1* m_pImpl_C1;
	FedmIscTagHandler_ISO14443_4_MIFARE_DESFire_Impl_C3* m_pImpl_C3;

};

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#endif // !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_4_MIFARE_DESFIRE_H_INCLUDED_)
