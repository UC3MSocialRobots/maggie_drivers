/*-------------------------------------------------------
|                                                       |
|      FedmIscTagHandler_ISO14443_4.h                   |
|                                                       |
---------------------------------------------------------

Copyright © 2009-2011	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	01.04.2009

Version       		:	03.03.02 / 28.04.2011 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID® classic-pro transponder ISO14443-4

NOTE				:	this class supports only selected or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_4_H_INCLUDED_)
#define _FEDM_ISC_TAG_HANDLER_ISO14443_4_H_INCLUDED_

#if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#include "../../FEDM.h"
#include "FedmIscTagHandler_ISO14443_3.h"


#define FEDM_ISO14443_4_APDU_BUFFER_SIZE		2048


class FEDM_ISOTabItem;



//#####################################################################################
// constants
//#####################################################################################





//####################################################################
// class FedmIscTagHandler_ISO14443_4
// >> supports only ISO Host Commands <<
//####################################################################

/// @brief Transponder class providing commands for all ISO 14443-4 compliant Transponders
///
/// <b>Description</b><br>
/// High-level class for easy-to-use Transponder specific communication. See OBID::FEDM::Core::ITagGroup for additional information.<br>
///
/// @note Programmers should be familiar with the ISO 14443 and ISO/IEC 7816 standards
class _FEDM_ISC_CORE_EXT_CLASS FedmIscTagHandler_ISO14443_4 : public FedmIscTagHandler_ISO14443_3
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

	/// @brief Set CID for next use, if enabled
	///
	/// <b>Description</b><br>
	///
	void SetCID(unsigned char ucCID);

	/// @brief Enable CID usage
	///
	/// @return
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	///
	/// <b>Description</b><br>
	int EnableCID(bool bEnable);

	/// @brief Get CID, actually in use
	///
	/// @return CID
	///
	/// <b>Description</b><br>
	unsigned char GetCID();

	/// @brief Query, if CID usage is enabled
	///
	/// @return true, if CID is enabled
	///
	/// <b>Description</b><br>
	bool IsCIDEnabled();

	/// @brief Set NAD for next use, if enabled
	///
	/// <b>Description</b><br>
	///
	void SetNAD(unsigned char ucNAD);

	/// @brief Enable CID usage
	///
	/// @return
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	///
	/// <b>Description</b><br>
	int EnableNAD(bool bEnable);

	/// @brief Get NAD, actually in use
	///
	/// @return NAD
	///
	/// <b>Description</b><br>
	unsigned char GetNAD();

	/// @brief Query, if NAD usage is enabled
	///
	/// @return true, if NAD is enabled
	///
	/// <b>Description</b><br>
	bool IsNADEnabled();

	
	/// @brief [0xB2][0xBE] : T=CL %APDU for synchronous call
	///
	/// @param[in] pucRequestData Pointer to array with request %APDU
	/// @param[in] iRequestLen Length (number of bytes) of request %APDU
	/// @param[out] pucResponseData Pointer to array for responded %APDU
	/// @param[in] uiResponseBufferLen Buffer size for responded %APDU
	/// @param[out] uiResponseLen Length (number of bytes) of responded %APDU
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// See Reader&apos;s System Manual for detailed information about the command and the method parameters<br>
	/// The structure of an APDU is defined by ISO/IEC 7816-4.<br>
	///
	/// @note An %APDU exchange can be a long term function call. The necessary timeout for the communication port is adapted internally and set to the previous value 
	/// when the method returns. If an application cannot wait such a long time, use the overloaded APDU(unsigned char*,unsigned int,void*,void (*)(void*,int,int,int)) 
	/// method for asynchronous execution.
	int Apdu(				unsigned char*	pucRequestData,
							unsigned int	iRequestLen,
							unsigned char*	pucResponseData,
							unsigned int	uiResponseBufferLen,
							unsigned int&	uiResponseLen	);

	/// @brief [0xB2][0xBE] : T=CL %APDU for asynchronous call
	///
	/// @param[in] pucRequestData Pointer to array with request %APDU
	/// @param[in] iRequestLen Length (number of bytes) of request %APDU
	/// @param[in] pAny Pointer to anything, which is reflected as the first parameter in the callback function
	/// @param[in] cbFct function to be called, if %APDU is received or an error must be notified
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// See Reader&apos;s System Manual for detailed information about the command and the method parameters.<br>
	/// The structure of an APDU is defined by ISO/IEC 7816-4.<br>
	/// The responded %APDU must be requested with the method GetResponseData() inside the callback function, if the 3rd parameter iError is 0.
	int Apdu(				unsigned char*	pucRequestData,
							unsigned int	iRequestLen,
							void* pAny,
							void (*cbFct)(void*, int, int, int)	);

	/// @brief [0xB2][0xBE] : T=CL Ping command
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// See Reader&apos;s System Manual for detailed information about the command
	int Ping();

	/// @brief [0xB2][0xBE] : T=CL Deselect command
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// See Reader&apos;s System Manual for detailed information about the command
	int Deselect();

	/// @brief [0xB2][0x2B] : Get Transponder Info command
	///
	/// @param[out] ucFSCI
	/// @param[out] ucFWI
	/// @param[out] ucDSI
	/// @param[out] ucDRI
	/// @param[out] ucNADSupport
	/// @param[out] ucCIDSupport
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// See Reader&apos;s System Manual for detailed information about the command and the method parameters.<br>
	int GetTransponderInfo(	unsigned char& ucFSCI,
							unsigned char& ucFWI,
							unsigned char& ucDSI,
							unsigned char& ucDRI,
							unsigned char& ucNADSupport,
							unsigned char& ucCIDSupport );

	// for internal use
	static void ApduCallback(void* pAny, int iApduHnd, int iError, int iDataLength);

	/// @brief Return of additional, Transponder specific error code
	///
	/// @return Error code
	///
	/// <b>Description</b><br>
	unsigned char GetLastISOErrorCode();

	/// @brief Returns the responded %APDU
	///
	/// @param[out] pucResponseData Pointer to array for responded %APDU
	/// @param[in] uiResponseBufferLen Buffer size for responded %APDU
	/// @param[out] uiResponseLen Length (number of bytes) of responded %APDU
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	///
	/// <b>Description</b><br>
	/// The structure of an APDU is defined by ISO/IEC 7816-4.<br>
	int GetResponseData(	unsigned char*	pucResponseData,
							unsigned int	uiResponseBufferLen,
							unsigned int&	uiResponseLen );


protected:
	// constructor
	FedmIscTagHandler_ISO14443_4(FEDM_ISCReader* pReader, FEDM_ISOTabItem* pTabItem);
	// 2. constructor
	FedmIscTagHandler_ISO14443_4(FEDM_ISCReader* pReader, unsigned int uiTagHandlerType, FEDM_ISOTabItem* pTabItem);
	// destructor
	virtual ~FedmIscTagHandler_ISO14443_4();

	unsigned char m_ucCID;
	unsigned char m_ucNAD;
	bool m_bUseCID;
	bool m_bUseNAD;

	int m_iFetclHnd;		// transponder handle in FETCL library

	// some temporary values for asynchronous APDU invoke
	void (*m_cbFct)(void*, int, int, int);
	void* m_pAny;

	unsigned char m_ucISO14443ErrorCode;

	// buffer for APDU
	unsigned char m_ucRequestData[FEDM_ISO14443_4_APDU_BUFFER_SIZE];
	unsigned char m_ucResponseData[FEDM_ISO14443_4_APDU_BUFFER_SIZE];
	unsigned int  m_uiResponseLen;
};

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO14443)

#endif // !defined(_FEDM_ISC_TAG_HANDLER_ISO14443_4_H_INCLUDED_)
