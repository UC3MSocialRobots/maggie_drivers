/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO14443_Innovatron.h          |
|                                                       |
---------------------------------------------------------

Copyright © 2012		FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	06.08.2012

Version       		:	04.02.06 / 06.08.2012 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID® classic-pro transponder ISO14443 Innovatron (ISO 14443B')

NOTE				:	this class supports only selected mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_INNOVATRON_H_INCLUDED_)
#define _FEDM_ISC_TAG_HANDLER_ISO14443_INNOVATRON_H_INCLUDED_

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "../../FEDM.h"
#include "FedmIscTagHandler_ISO14443.h"


#define FEDM_ISO14443_INNOVATRON_APDU_BUFFER_SIZE		2048


class FEDM_ISOTabItem;



//#####################################################################################
// constants
//#####################################################################################





//####################################################################
// class FedmIscTagHandler_ISO14443_Innovatron
// >> supports only ISO Host Commands <<
//####################################################################

/// @brief Transponder class providing manufacturer specific commands for ISO 14443-4 compliant Transponders of type Innovatron (ISO 14443B')
///
/// <b>Description</b><br>
/// High-level class for easy-to-use Transponder specific communication. See OBID::FEDM::Core::ITagGroup for additional information.<br>
///
/// @note Programmers should be familiar with the ISO 14443 standards
class _FEDM_ISC_CORE_EXT_CLASS FedmIscTagHandler_ISO14443_Innovatron : public FedmIscTagHandler_ISO14443
{
public:

	// friend classes have access rights to protected members
	friend class FEDM_ISCReaderModule;
	friend class FEDM_ISCReader;
	friend class FEDM_ISOTabItem;

	/// @brief Return of Card Info read with last TagSelect call
	///
	/// @param[out] ucCardInfoFormat Card Info Format value
	/// @param[out] pucCardInfo Pointer to buffer with Card Info
	/// @param[in] uiCardInfoBufferSize Size of Card Info buffer (Number of bytes)
	/// @return
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	///
	/// <b>Description</b><br>
	/// See command [0xB0][0x25] Select in Reader&apos;s System Manual.<br>
	int GetCardInfo(	unsigned char& ucCardInfoFormat,
						unsigned char* pucCardInfo,
						unsigned int   uiCardInfoBufferSize );

	/// @brief Return of Verlog value read with last Inventory call
	///
	/// @return Verlog value
	///
	/// <b>Description</b><br>
	/// See command [0xB0][0x01] Inventory in Reader&apos;s System Manual.<br>
	unsigned char GetVerlog();

	/// @brief Return of Config value read with last Inventory call
	///
	/// @return Config value
	///
	/// <b>Description</b><br>
	/// See command [0xB0][0x01] Inventory in Reader&apos;s System Manual.<br>
	unsigned char GetConfig();

	/// @brief Return of ATR value read with last Inventory call
	///
	/// @param[out] pucAtr Pointer to buffer with ATR
	/// @param[in] uiAtrBufferLen Size of ATR buffer (Number of bytes)
	/// @param[out] uiAtrLen Number of bytes in ATR buffer
	/// @return
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	///
	/// <b>Description</b><br>
	/// See command [0xB0][0x01] Inventory in Reader&apos;s System Manual.<br>
	int GetAtr(unsigned char* pucAtr, unsigned int uiAtrBufferLen, unsigned int& uiAtrLen);

	/// @brief APDU command
	///
	/// @param[in] pucRequestData Buffer with APDU request
	/// @param[in] iRequestLen Length of APDU (number of bytes)
	/// @param[out] pucResponseData Buffer for APDU response
	/// @param[in] uiResponseBufferLen Length of response bufer
	/// @param[out] uiResponseLen Length of APDU response (number of bytes)
	/// @return
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// See command [0xB2][0xBB] ISO 14443-B’ (Innovatron) Data Exchange in Reader&apos;s System Manual.<br>
	int Apdu(	unsigned char*	pucRequestData,
				unsigned int	iRequestLen,
				unsigned char*	pucResponseData,
				unsigned int	uiResponseBufferLen,
				unsigned int&	uiResponseLen	);

	/// @brief Discard command
	///
	/// @return
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// See command [0xB2][0xBB] ISO 14443-B’ (Innovatron) Data Exchange in Reader&apos;s System Manual.<br>
	int Discard();

protected:
	// constructor
	FedmIscTagHandler_ISO14443_Innovatron(FEDM_ISCReader* pReader, FEDM_ISOTabItem* pTabItem);
	// 2. constructor
	FedmIscTagHandler_ISO14443_Innovatron(FEDM_ISCReader* pReader, unsigned int uiTagHandlerType, FEDM_ISOTabItem* pTabItem);
	// destructor
	virtual ~FedmIscTagHandler_ISO14443_Innovatron();

	// buffer for APDU
	unsigned char m_ucRequestData[FEDM_ISO14443_INNOVATRON_APDU_BUFFER_SIZE];
	unsigned char m_ucResponseData[FEDM_ISO14443_INNOVATRON_APDU_BUFFER_SIZE];
	unsigned int  m_uiResponseLen;

};

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#endif // !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_INNOVATRON_H_INCLUDED_)
