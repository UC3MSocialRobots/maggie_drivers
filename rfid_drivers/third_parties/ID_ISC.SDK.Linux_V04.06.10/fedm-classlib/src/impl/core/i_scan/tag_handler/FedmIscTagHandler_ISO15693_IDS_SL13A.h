/*-------------------------------------------------------
|                                                       |
|	   FedmIscTagHandler_ISO15693_IDS_SL13A.h           |
|                                                       |
---------------------------------------------------------

Copyright © 2011		FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	03.03.01 / 28.03.2011

Version       		:	03.03.01 / 01.04.2011 / M. Hultsch

Operation Systems	:	independent

Function			:	class for OBID i-scan® HF transponder ISO15693 of manufacturer IDS Microchip 
						specialized tag handler on top with custom specific commands

NOTE				:	this class supports only non-addressed or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_TAG_HANDLER_ISO15693_IDS_SL13A_H_INCLUDED_)
#define _FEDM_ISC_TAG_HANDLER_ISO15693_IDS_SL13A_H_INCLUDED_

#if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)

#include "../../FEDM.h"
#include "FedmIscTagHandler_ISO15693.h"

class FEDM_ISOTabItem;



//#####################################################################################
// constants
//#####################################################################################





//####################################################################
// class FedmIscTagHandler_ISO15693_IDS_SL13A
// >> supports only ISO Host Commands <<
//####################################################################

/// @brief Transponder class providing manufacturer specific commands for ISO 15693 compliant Transponders of type SLA13A from manufacturer IDS Microchip AG
///
/// <b>Description</b><br>
/// High-level class for easy-to-use Transponder specific communication. See OBID::FEDM::Core::ITagGroup for additional information.<br>
/// All methods are similiar to the specific commands of the IDS SL13A. Thus, you must refer to the Technical Datasheet of the SL13A to get detailed information about the methods and the parameters.<br>
/// Secondly, all methods of this class are Custom specific commands, realized with the Reader command [0xB1], which are described in the Application Note <i>IDS Custom and Proprietary Commands_HF.pdf</i>
///
/// @note Programmers should be familiar with the ISO 15693 standard
class _FEDM_ISC_CORE_EXT_CLASS FedmIscTagHandler_ISO15693_IDS_SL13A : public FedmIscTagHandler_ISO15693
{
public:

	// friend classes have access rights to protected members
	friend class FEDM_ISCReaderModule;
	friend class FEDM_ISCReader;
	friend class FEDM_ISOTabItem;

	/// @brief [0xB1][0xA0] : Set the password in the Transponder
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_HF.pdf</i>, where all parameters are described in detail.
	int SetPassword(	unsigned char ucPasswordLevel,
						string sPassword);

	/// @brief [0xB1][0xA1] : Writes parameters to System Area Block 0x0A
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Command with composed parameters<br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_HF.pdf</i>, where all parameters are described in detail.
	int SetLogMode(		unsigned int uiLogMode );
	
	/// @brief [0xB1][0xA1] : Writes parameters to System Area Block 0x0A
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Command with single parameters (composed internal)<br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_HF.pdf</i>, where all parameters are described in detail.
	int SetLogMode(		bool bExternalSensor,
						unsigned char ucLogFormat,
						bool bBatteryCheck,
						bool bStorageRule,
						unsigned int uiLogInterval,
						unsigned int uiExtremeUpperLimit );

	/// @brief [0xB1][0xA2] : Writes parameters to System Area Block 0x09
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_HF.pdf</i>, where all parameters are described in detail.
	int SetLogLimits(	unsigned int uiExtremeLowerLimit,
						unsigned int uiLowerLimit,
						unsigned int uiUpperLimit );

	/// @brief [0xB1][0xA3] : Reads parameters from System Area Blocks 0x08..0x0B
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_HF.pdf</i>, where all parameters are described in detail.
	int GetMeasurementSetup(	unsigned int& uiStartTime,
								unsigned int& uiLogLimits,
								unsigned int& uiLogMode,
								unsigned int& uiDelayTime );

	/// @brief [0xB1][0xA4] : Writes external calibration data to System Area Block 0x06
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_HF.pdf</i>, where all parameters are described in detail.
	int SetExternalCalibrationData(	unsigned int uiCalibrationData );

	/// @brief [0xB1][0xA5] : Writes internal calibration data to System Area Block 0x05
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_HF.pdf</i>, where all parameters are described in detail.
	int SetInternalCalibrationData(	unsigned int uiCalibrationData );

	/// @brief [0xB1][0xA6] : Stops logging process, stops timer and sets tag in standby mode
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_HF.pdf</i>, where all parameters are described in detail.
	int SetPassive();

	/// @brief [0xB1][0xA7] : Start logging process and sets the start time in UTC format
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Command with composed parameters<br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_HF.pdf</i>, where all parameters are described in detail.
	int StartLog(	unsigned int uiStartTime );
	
	/// @brief [0xB1][0xA7] : Start logging process and sets the start time in UTC format
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Command with single parameters (composed internal)<br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_HF.pdf</i>, where all parameters are described in detail.
	int StartLog(	unsigned char ucYear,
					unsigned char ucMonth,
					unsigned char ucDay,
					unsigned char ucHour,
					unsigned char ucMinute,
					unsigned char ucSecond );

	/// @brief [0xB1][0xA8] : Returns the logging state
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Command with single status parameters<br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_HF.pdf</i>, where all parameters are described in detail.
	int GetLogState(	bool& bActiveFlag,
						unsigned int&  uiErrorsAndEvents,
						unsigned char& ucNoOfMemReplacements,
						unsigned int&  uiNoOfMeasurements,
						unsigned char& ucMeasurementAddressPointer,
						unsigned char& ucExtremeLowerLimitCounter,
						unsigned char& ucLowerLimitCounter,
						unsigned char& ucHigherLimitCounter,
						unsigned char& ucExtremeHigherLimitCounter );
	
	/// @brief [0xB1][0xA8] : Returns the logging state
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Command with composed different parameters<br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_HF.pdf</i>, where all parameters are described in detail.
	int GetLogState(	unsigned int& uiMeasurementStatus,
						unsigned int& uiLimitCounters );

	/// @brief [0xB1][0xA9] : Reads internal and external calibration data from System Area Blocks 0x05 and 0x06
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_HF.pdf</i>, where all parameters are described in detail.
	int GetCalibrationData(	unsigned int& uiInternalCalibrationData,
							unsigned int& uiExternalCalibrationData );

	/// @brief [0xB1][0xAA] : Returns the actual battery level
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_HF.pdf</i>, where all parameters are described in detail.
	int GetBatteryLevel(	unsigned char& ucBatteryLevel );

	/// @brief [0xB1][0xAB] : Compares the password of the specified password level
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_HF.pdf</i>, where all parameters are described in detail.
	int VerifyPassword(	unsigned char ucPasswordLevel );

	/// @brief [0xB1][0xAC] : Writes parameters to System Area Block 0x0B
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_HF.pdf</i>, where all parameters are described in detail.
	int Initialize(	unsigned char ucNoOfUserBlocks,
					unsigned int uiDelayTime,
					bool bSingleUse );

	//[0xB1][0xAD] : starts the AD conversion and returns the measured temperature
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_HF.pdf</i>, where all parameters are described in detail.
	int GetTemperature(	unsigned int& uiTemperature );

	/// @brief returns the last tag error
	///
	/// <b>Description</b><br>
	unsigned char GetTagError();

	/// @brief Set all data members to 0
	///
	/// <b>Description</b><br>
	virtual void Init();

#if _MSC_VER > 1200
	/// @brief constant for [0xA0] Set Password
	///
	/// <b>Description</b><br>
	static const unsigned char PW_LEVEL_SYSTEM			= 0x01;

	/// @brief constant for [0xA0] Set Password
	///
	/// <b>Description</b><br>
	static const unsigned char PW_LEVEL_USER			= 0x02;

	/// @brief constant for [0xA0] Set Password
	///
	/// <b>Description</b><br>
	static const unsigned char PW_LEVEL_MEASUREMENT		= 0x03;

	/// @brief constant for [0xA0] Set Password
	///
	/// <b>Description</b><br>
	static const unsigned char PW_LEVEL_STORAGE_EEPROM	= 0x80;

	/// @brief constant for [0xA1] Set Log Mode
	///
	/// <b>Description</b><br>
	static const unsigned char LOG_MODE_DENSE						= 0x00;

	/// @brief constant for [0xA1] Set Log Mode
	///
	/// <b>Description</b><br>
	static const unsigned char LOG_MODE_ALL_VALUES_OUT_OF_LIMITS	= 0x01;

	/// @brief constant for [0xA1] Set Log Mode
	///
	/// <b>Description</b><br>
	static const unsigned char LOG_MODE_LIMITS_CROSSING				= 0x03;
#else // for Visual Studio 6
	// constants for [0xA0] Set Password
	static const unsigned char PW_LEVEL_SYSTEM;
	static const unsigned char PW_LEVEL_USER;
	static const unsigned char PW_LEVEL_MEASUREMENT;
	static const unsigned char PW_LEVEL_STORAGE_EEPROM;
	// constants for [0xA1] Set Log Mode
	static const unsigned char LOG_MODE_DENSE;
	static const unsigned char LOG_MODE_ALL_VALUES_OUT_OF_LIMITS;
	static const unsigned char LOG_MODE_LIMITS_CROSSING;
#endif

protected:
	// constructor
	FedmIscTagHandler_ISO15693_IDS_SL13A(FEDM_ISCReader* pReader, FEDM_ISOTabItem* pTabItem);
	// 2. constructor
	FedmIscTagHandler_ISO15693_IDS_SL13A(FEDM_ISCReader* pReader, unsigned int uiTagHandlerType, FEDM_ISOTabItem* pTabItem);
	// destructor
	virtual ~FedmIscTagHandler_ISO15693_IDS_SL13A();

	unsigned char	m_ucTagError;

};

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_ISO15693)

#endif // !defined(_FEDM_ISC_TAG_HANDLER_ISO15693_IDS_SL13A_H_INCLUDED_)
