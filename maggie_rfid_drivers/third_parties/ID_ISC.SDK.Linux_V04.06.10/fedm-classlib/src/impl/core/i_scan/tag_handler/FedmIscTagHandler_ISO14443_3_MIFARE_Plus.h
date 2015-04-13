/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO14443_3_MIFARE_Plus.h       |
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
Begin        		:	25.08.2009

Version       		:	03.01.00 / 10.02.2010 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID® classic-pro transponder ISO14443-3 MIFARE Plus

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_3_MIFARE_PLUS_H_INCLUDED_)
#define _FEDM_ISC_TAG_HANDLER_ISO14443_3_MIFARE_PLUS_H_INCLUDED_

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "../../FEDM.h"
#include "FedmIscTagHandler_ISO14443_3_MIFARE_Classic.h"

class FEDM_ISOTabItem;



//#####################################################################################
// constants
//#####################################################################################





//####################################################################
// class FedmIscTagHandler_ISO14443_3_MIFARE_Plus
// >> supports only ISO Host Commands <<
//####################################################################

/// @brief Transponder class providing manufacturer specific commands for ISO 14443-3 compliant Transponders of type MIFARE Plus from manufacturer NXP Semiconductors
///
/// <b>Description</b><br>
/// High-level class for easy-to-use Transponder specific communication. See OBID::FEDM::Core::ITagGroup for additional information.<br>
///
/// @note Programmers should be familiar with the ISO 14443 standards
class _FEDM_ISC_CORE_EXT_CLASS FedmIscTagHandler_ISO14443_3_MIFARE_Plus : public FedmIscTagHandler_ISO14443_3_MIFARE_Classic
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


protected:
	// constructor not necessary, as this class represents not a real tag type
	//FedmIscTagHandler_ISO14443_3_MIFARE_Plus(FEDM_ISCReader* pReader, FEDM_ISOTabItem* pTabItem);
	// 2. constructor
	FedmIscTagHandler_ISO14443_3_MIFARE_Plus(FEDM_ISCReader* pReader, unsigned int uiTagHandlerType, FEDM_ISOTabItem* pTabItem);
	// destructor
	virtual ~FedmIscTagHandler_ISO14443_3_MIFARE_Plus();

	unsigned char m_ucErrorSource;
	unsigned int m_uiErrorCode;

};

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#endif // !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_3_MIFARE_PLUS_H_INCLUDED_)
