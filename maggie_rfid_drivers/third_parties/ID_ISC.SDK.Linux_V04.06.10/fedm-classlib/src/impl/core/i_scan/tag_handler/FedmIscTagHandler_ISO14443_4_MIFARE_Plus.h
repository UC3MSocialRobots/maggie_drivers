/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO14443_4_MIFARE_Plus.h       |
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
Begin        		:	24.08.2009

Version       		:	03.01.00 / 10.02.2010 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID® classic-pro transponder ISO14443-4 MIFARE Plus

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_4_MIFARE_PLUS_H_INCLUDED_)
#define _FEDM_ISC_TAG_HANDLER_ISO14443_4_MIFARE_PLUS_H_INCLUDED_

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "../../FEDM.h"
#include "FedmIscTagHandler_ISO14443_4.h"

class FEDM_ISOTabItem;



//#####################################################################################
// constants
//#####################################################################################





//####################################################################
// class FedmIscTagHandler_ISO14443_4_MIFARE_Plus
// >> supports only ISO Host Commands <<
//####################################################################

/// @brief Transponder class providing manufacturer specific commands for ISO 14443-4 compliant Transponders of type MIFARE Plus from manufacturer NXP Semiconductors
///
/// <b>Description</b><br>
/// High-level class for easy-to-use Transponder specific communication. See OBID::FEDM::Core::ITagGroup for additional information.<br>
/// This class implements the communication with [0xC2] Reader commands.<br>
/// Detailed description about the command interface can be found in:
/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
///
/// @note Programmers should be familiar with the ISO 14443 standards
class _FEDM_ISC_CORE_EXT_CLASS FedmIscTagHandler_ISO14443_4_MIFARE_Plus : public FedmIscTagHandler_ISO14443_4
{
public:

	// friend classes have access rights to protected members
	friend class FEDM_ISCReaderModule;
	friend class FEDM_ISCReader;
	friend class FEDM_ISOTabItem;

	/// @brief Initialization method
	///
	/// <b>Description</b><br>
	/// Clears all data members
	virtual void Init();

	/// @brief Method to query the last Error Source
	///
	/// @return Error Source
	///
	/// <b>Description</b><br>
	/// 
	unsigned char	GetErrorSource();

	/// @brief Method to query the last Error Code
	///
	/// @return Error Code
	///
	/// <b>Description</b><br>
	/// 
	unsigned int	GetErrorCode();


	// commands for all security levels

	/// @brief MIFARE Plus Command [0x70], realized with Reader command [0xC2][0x70] First Authentication
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is an Authentication command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int FirstAuthent(		unsigned int uiAesKeyBlockNumber,
							unsigned char ucPcdCapLength,
							unsigned char* pucPcdCap2,
							unsigned char ucReaderKeyIndex );

	/// @brief MIFARE Plus Command [0x76], realized with Reader command [0xC2][0x76] Following Authentication
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// This method is an Authentication command.<br>
	/// Detailed description about the command and the parameters can be found in:
	/// - Product Datasheet for MIFARE Plus from NXP Semiconductors
	/// - System Manual <i>SoftCrypto Functions</i> from FEIG ELECTRONIC
	int FollowingAuthent(	unsigned int uiAesKeyBlockNumber,
							unsigned char ucReaderKeyIndex );


protected:
	// constructor not necessary, as this class represents not a real tag type
	FedmIscTagHandler_ISO14443_4_MIFARE_Plus(FEDM_ISCReader* pReader, FEDM_ISOTabItem* pTabItem);
	// 2. constructor
	FedmIscTagHandler_ISO14443_4_MIFARE_Plus(FEDM_ISCReader* pReader, unsigned int uiTagHandlerType, FEDM_ISOTabItem* pTabItem);
	// destructor
	virtual ~FedmIscTagHandler_ISO14443_4_MIFARE_Plus();

	unsigned char m_ucErrorSource;
	unsigned int m_uiErrorCode;

};

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#endif // !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_4_MIFARE_PLUS_H_INCLUDED_)
