/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO14443_3_ASK_CTx.h           |
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

Function			:	class for OBID® classic-pro transponder ISO14443-3 ASK CTx

NOTE				:	this class supports only selected mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_3_ASK_CTX_H_INCLUDED_)
#define _FEDM_ISC_TAG_HANDLER_ISO14443_3_ASK_CTX_H_INCLUDED_

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "../../FEDM.h"
#include "FedmIscTagHandler_ISO14443_3.h"

class FEDM_ISOTabItem;



//#####################################################################################
// constants
//#####################################################################################





//####################################################################
// class FedmIscTagHandler_ISO14443_3_ASK_CTx
// >> supports only ISO Host Commands <<
//####################################################################

/// @brief Transponder class providing manufacturer specific commands for ISO 14443-3 compliant CTx Transponder of type CTx series from manufacturer ASK
///
/// <b>Description</b><br>
/// High-level class for easy-to-use Transponder specific communication. See OBID::FEDM::Core::ITagGroup for additional information.<br>
/// See Reader&apos;s System Manual for detailed information about the command abd parameters.<br>
///
/// @note Programmers should be familiar with the ISO 14443 standards
class _FEDM_ISC_CORE_EXT_CLASS FedmIscTagHandler_ISO14443_3_ASK_CTx : public FedmIscTagHandler_ISO14443_3
{
public:

	// friend classes have access rights to protected members
	friend class FEDM_ISCReaderModule;
	friend class FEDM_ISCReader;
	friend class FEDM_ISOTabItem;

	/// @brief Method to query the Product Code
	///
	/// @return Product Code
	///
	/// <b>Description</b><br>
	/// The Product Code is part of the Inventory responsse.
	unsigned char GetProductCode();

	/// @brief Method to query the Fab Code
	///
	/// @return Fab Code
	///
	/// <b>Description</b><br>
	/// The Fab Code is part of the Inventory responsse.
	unsigned char GetFabCode();

	/// @brief Method to query the App Code
	///
	/// @return App Code
	///
	/// <b>Description</b><br>
	/// The App Code is part of the Inventory responsse.
	unsigned char GetAppCode();

	/// @brief Method to query the Embedder Code
	///
	/// @return Embedder Code
	///
	/// <b>Description</b><br>
	/// The Embedder Code is part of the Inventory responsse.
	unsigned char GetEmbedderCode();


protected:
	// constructor
	FedmIscTagHandler_ISO14443_3_ASK_CTx(FEDM_ISCReader* pReader, FEDM_ISOTabItem* pTabItem);
	// 2. constructor
	FedmIscTagHandler_ISO14443_3_ASK_CTx(FEDM_ISCReader* pReader, unsigned int uiTagHandlerType, FEDM_ISOTabItem* pTabItem);
	// destructor
	virtual ~FedmIscTagHandler_ISO14443_3_ASK_CTx();

};

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#endif // !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_3_ASK_CTX_H_INCLUDED_)
