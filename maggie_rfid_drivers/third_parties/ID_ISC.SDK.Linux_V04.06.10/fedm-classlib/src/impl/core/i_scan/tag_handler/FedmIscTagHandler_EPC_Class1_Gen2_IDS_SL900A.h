/*-------------------------------------------------------
|                                                       |
|	   FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A.h   |
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
Begin        		:	03.03.01 / 06.04.2011

Version       		:	03.03.01 / 12.04.2011 / M. Hultsch

Operation Systems	:	independent

Function			:	class for UHF transponder EPC Class 1 Generation 2 of manufacturer IDS Microchip 
						specialized tag handler on top with custom specific commands

NOTE				:	this class supports only non-addressed or addressed mode in Host-Mode


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_TAG_HANDLER_EPC_CLASS1_GEN2_IDS_SL900A_H_INCLUDED_)
#define _FEDM_ISC_TAG_HANDLER_EPC_CLASS1_GEN2_IDS_SL900A_H_INCLUDED_

#if !defined(_FEDM_NO_TAG_HANDLER_EPC_C1_G2)

#include "../../FEDM.h"
#include "FedmIscTagHandler_EPC_Class1_Gen2.h"

class FEDM_ISOTabItem;


//#####################################################################################
// types
//#####################################################################################




//####################################################################
// class FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A
// >> supports only ISO Host Commands <<
//####################################################################

/// @brief Transponder class providing manufacturer specific commands for EPC Class 1 Generation 2 compliant Transponders of type SL900A from manufacturer IDS Microchip AG
///
/// <b>Description</b><br>
/// High-level class for easy-to-use Transponder specific communication. See OBID::FEDM::Core::ITagGroup for additional information, how to work with this class.<br>
/// All methods are similiar to the specific commands of the IDS SL900A. Thus, you must refer to the Technical Datasheet of the SL900A to get detailed information about the methods and the parameters.<br>
/// Secondly, all methods of this class are Custom specific commands, realized with the Reader command [0xB4], which are described in the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>
class _FEDM_ISC_CORE_EXT_CLASS FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A : public FedmIscTagHandler_EPC_Class1_Gen2
{
public:

	// friend classes have access rights to protected members
	friend class FEDM_ISCReaderModule;
	friend class FEDM_ISCReader;
	friend class FEDM_ISOTabItem;

	/// @brief Structure with time information
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all members are described in detail.
	struct START_TIME
	{
		unsigned char ucYear;
		unsigned char ucMonth;
		unsigned char ucDay;
		unsigned char ucHour;
		unsigned char ucMinute;
		unsigned char ucSecond;

		// initialization
		void Init()
		{
			memset(&ucYear, 0, sizeof(START_TIME));
		}

		// initialization
		START_TIME() :	ucYear(0),
						ucMonth(0),
						ucDay(0),
						ucHour(0),
						ucMinute(0),
						ucSecond(0) {}

	};

	/// @brief Structure with calibration information
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all members are described in detail.
	struct CALIBRATION
	{
		unsigned char ucRefTc;
		unsigned char ucOffInt;
		unsigned char ucRingCall;
		unsigned char ucIRLev;
		unsigned char ucSelp22;
		unsigned char ucDf;
		unsigned char ucAdf;
		unsigned char ucSelp12;
		unsigned char ucCoars2;
		unsigned char ucAd2;
		unsigned char ucCoars1;
		unsigned char ucAd1;
		bool bSwExtEn;
		bool bGndSwitch;

		// initialization
		void Init()
		{
			memset(&ucRefTc, 0, sizeof(CALIBRATION));
		}

		// initialization
		CALIBRATION() :	ucRefTc(0),
						ucOffInt(0),
						ucRingCall(0),
						ucIRLev(0),
						ucSelp22(0),
						ucDf(0),
						ucAdf(0),
						ucSelp12(0),
						ucCoars2(0),
						ucAd2(0),
						ucCoars1(0),
						ucAd1(0),
						bSwExtEn(false),
						bGndSwitch(false) {}

	};

	/// @brief Structure with Sensor Frontend data
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all members are described in detail.
	struct SFE_PARAMETERS
	{
		unsigned char ucVerifySensorID;
		unsigned char ucSext2;
		unsigned char ucSext1;
		unsigned char ucSeti;
		unsigned char ucRang;
		bool bAutoRangePreset;

		/// initialization
		void Init()
		{
			memset(&ucVerifySensorID, 0, sizeof(SFE_PARAMETERS));
		}

		/// initialization
		SFE_PARAMETERS() :	ucVerifySensorID(0),
							ucSext2(0),
							ucSext1(0),
							ucSeti(0),
							ucRang(0),
							bAutoRangePreset(false) {}

	};

	/// @brief Structure with Limits data
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all members are described in detail.
	struct LIMITS
	{
		unsigned int uiExtremeUpperLimit;
		unsigned int uiUpperLimit;
		unsigned int uiLowerLimit;
		unsigned int uiExtremeLowerLimit;

		/// initialization
		void Init()
		{
			memset(&uiExtremeUpperLimit, 0, sizeof(LIMITS));
		}

		/// initialization
		LIMITS() :	uiExtremeUpperLimit(0),
					uiUpperLimit(0),
					uiLowerLimit(0),
					uiExtremeLowerLimit(0) {}

	};

	/// @brief Structure with Limits Counter data
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all members are described in detail.
	struct LIMITS_COUNTER
	{
		unsigned char ucExtremeUpperLimitsCounter;
		unsigned char ucUpperLimitsCounter;
		unsigned char ucLowerLimitsCounter;
		unsigned char ucExtremeLowerLimitsCounter;

		/// initialization
		void Init()
		{
			memset(&ucExtremeUpperLimitsCounter, 0, sizeof(LIMITS_COUNTER));
		}

		/// initialization
		LIMITS_COUNTER() :	ucExtremeUpperLimitsCounter(0),
							ucUpperLimitsCounter(0),
							ucLowerLimitsCounter(0),
							ucExtremeLowerLimitsCounter(0) {}

	};

	/// @brief Structure with System Status information
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all members are described in detail.
	struct SYSTEM_STATUS
	{
		bool bActive;
		unsigned int uiNoOfMeasurements;
		unsigned char ucNoOfMemoryReplacements;
		unsigned int uiMeasurementAddressPointer;

		/// initialization
		void Init()
		{
			memset(&bActive, 0, sizeof(SYSTEM_STATUS));
		}

		/// initialization
		SYSTEM_STATUS() :	bActive(false),
							uiNoOfMeasurements(0),
							ucNoOfMemoryReplacements(0),
							uiMeasurementAddressPointer(0) {}

	};

	/// @brief Structure with Log Mode data
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all members are described in detail.
	struct LOG_MODE
	{
		bool bBatteryCheck;
		bool bTemperatureSensor;
		bool bExternalSensor1;
		bool bExternalSensor2;
		bool bStorageRule;
		unsigned char ucLogginForm;

		/// initialization
		void Init()
		{
			memset(&bBatteryCheck, 0, sizeof(LOG_MODE));
		}

		/// initialization
		LOG_MODE() :	bBatteryCheck(false),
						bTemperatureSensor(false),
						bExternalSensor1(false),
						bExternalSensor2(false),
						bStorageRule(false),
						ucLogginForm(0) {}

	};

	/// @brief Structure with Log Interval information
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all members are described in detail.
	struct LOG_INTERVAL
	{
		unsigned int uiLogInterval;

		/// initialization
		void Init()
		{
			memset(&uiLogInterval, 0, sizeof(LOG_INTERVAL));
		}

		/// initialization
		LOG_INTERVAL() :	uiLogInterval(0) {}

	};

	/// @brief Structure with Delay Time data
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all members are described in detail.
	struct DELAY_TIME
	{
		bool bTimer;
		bool bDelayMode_External;
		unsigned int uiDelayTime;

		/// initialization
		void Init()
		{
			memset(&bTimer, 0, sizeof(DELAY_TIME));
		}

		/// initialization
		DELAY_TIME() :	bTimer(false),
						bDelayMode_External(false),
						uiDelayTime(0) {}

	};

	/// @brief Structure with App Data
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all members are described in detail.
	struct APP_DATA
	{
		unsigned char ucBrokenWordPointer;
		unsigned int uiNoOfWordsForAppData;

		/// initialization
		void Init()
		{
			memset(&ucBrokenWordPointer, 0, sizeof(APP_DATA));
		}

		/// initialization
		APP_DATA() :	ucBrokenWordPointer(0),
						uiNoOfWordsForAppData(0) {}

	};

	/// @brief Structure with Shelf Live Block 0
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all members are described in detail.
	struct SHELF_LIFE_0
	{
		unsigned char ucActivationEnergy;
		unsigned char ucNormalTemperature;
		unsigned char ucMinimalTemperature;
		unsigned char ucMaximalTemperature;

		/// initialization
		void Init()
		{
			memset(&ucActivationEnergy, 0, sizeof(SHELF_LIFE_0));
		}

		/// initialization
		SHELF_LIFE_0() :	ucActivationEnergy(0),
							ucNormalTemperature(0),
							ucMinimalTemperature(0),
							ucMaximalTemperature(0) {}

	};

	/// @brief Structure with Shelf Live Block 1
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all members are described in detail.
	struct SHELF_LIFE_1
	{
		unsigned char ucSkipLog;
		bool bEnableAlgorithm;
		bool bEnableNegativeShelfLife;
		unsigned char ucSensorID;
		unsigned int uiTinit;
		unsigned int uiSLinit;

		/// initialization
		void Init()
		{
			memset(&ucSkipLog, 0, sizeof(SHELF_LIFE_1));
		}

		/// initialization
		SHELF_LIFE_1() :	ucSkipLog(0),
							bEnableAlgorithm(false),
							bEnableNegativeShelfLife(false),
							ucSensorID(0),
							uiTinit(0),
							uiSLinit(0) {}

	};



	/// @brief [0xB4][0xA0] : Set the password in the Transponder
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all parameters are described in detail.
	int SetPassword(	string sAccessPassword,
						unsigned char ucPasswordLevel,
						string sPassword );

	/// @brief [0xB4][0xA1] : writes parameters to System Area Blocks 0x0026..0x028
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all parameters are described in detail.
	int SetLogMode(		string sAccessPassword,
						LOG_MODE* pLogMode,
						LOG_INTERVAL* pLogInterval );

	/// @brief [0xB4][0xA2] : writes parameters to System Area Blocks 0x019..0x01D
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all parameters are described in detail.
	int SetLogLimits(	string sAccessPassword,
						LIMITS* pLogLimits);

	/// @brief [0xB4][0xA3] : reads parameters from System Area Blocks 0x00C..0x00F, 0x019..0x01D, 0x026..0x02C
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all parameters are described in detail.
	int GetMeasurementSetup(string sAccessPassword,
							START_TIME* pStartTime,
							LIMITS* pLogLimits,
							LOG_MODE* pLogMode,
							LOG_INTERVAL* pLogInterval,
							DELAY_TIME* pDelayTime,
							APP_DATA* pUserData );

	/// @brief [0xB4][0xA4] : writes Sensor Front End parameters to System Area Blocks 0x017..0x018
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all parameters are described in detail.
	int SetSFEData(	string sAccessPassword,
					SFE_PARAMETERS* pSfeData );

	/// @brief [0xB4][0xA5] : writes internal calibration data to System Area Blocks 0x010..0x016
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all parameters are described in detail.
	int SetCalibrationData(	string sAccessPassword,
							CALIBRATION* pCalData );

	/// @brief [0xB4][0xA6] : stops logging process, stops timer and sets tag in standby mode
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all parameters are described in detail.
	int EndLog(	string sAccessPassword );

	/// @brief [0xB4][0xA7] : start logging process and sets the start time in UTC format
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all parameters are described in detail.
	int StartLog(	string sAccessPassword,
					START_TIME* pStartTime );

	/// @brief [0xB4][0xA8] : returns the logging state
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all parameters are described in detail.
	int GetLogState(string sAccessPassword,
					LIMITS_COUNTER* pLimitsCounter,
					SYSTEM_STATUS* pSystemStatus,
					SHELF_LIFE_0* pSF0,
					SHELF_LIFE_1* pSF1,
					unsigned int& uiCurrentShelfLife,
					unsigned char& ucStatusFlags );

	/// @brief [0xB4][0xA9] : reads internal and external calibration data from System Area Blocks 0x010..0x016, 0x030..0x037
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all parameters are described in detail.
	int GetCalibrationData(	string sAccessPassword,
							CALIBRATION* pCalData,
							SFE_PARAMETERS* pSfeData );

	/// @brief [0xB4][0xAA] : returns the actual battery level
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all parameters are described in detail.
	int GetBatteryLevel(string sAccessPassword,
						unsigned char ucBatteryRetrigger,
						bool& bError,
						unsigned char& ucBatteryType,
						unsigned int& uiBatteryLevel );

	/// @brief [0xB4][0xAB] : programs parameters for the dynamic shelf-life algorithm
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all parameters are described in detail.
	int SetShelfLife(	string sAccessPassword,
						SHELF_LIFE_0* pSF0,
						SHELF_LIFE_1* pSF1 );

	/// @brief [0xB4][0xAC] : writes parameters to System Area Blocks 0x029..0x02C
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all parameters are described in detail.
	int Initialize(	string sAccessPassword,
					DELAY_TIME* pDelayTime,
					APP_DATA* pUserData );

	/// @brief [0xB4][0xAD] : starts the AD conversion on the specified sensor and returns the measured value
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all parameters are described in detail.
	int GetSensorValue(	string sAccessPassword,
						unsigned char ucSensorType,
						bool& bError,
						unsigned char& ucRangeOrLimit,
						unsigned int& uiSensorValue );

	/// @brief [0xB4][0xAE]
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all parameters are described in detail.
	int OpenArea(	string sAccessPassword,
					unsigned char ucPasswordLevel,
					string sPassword );

	/// @brief [0xB4][0xAF] Access FIFO : can read and write data from the FIFO and can also read the FIFO status register.
	///
	/// @return 
	/// -   0 if OK
	/// - < 0 [error code](@ref OBID::FEDM::Core::Error)
	/// - > 0 status byte of the responded protocol, if the Reader signals a problem. See Reader&apos;s System Manual for the full list with all status bytes.
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all parameters are described in detail.
	int AccessFIFO(	string sAccessPassword,
					unsigned char ucSubCommand,
					unsigned char ucNoOfBytes,
					unsigned char* pucBuffer );

	/// @brief returns the last tag error
	///
	/// <b>Description</b><br>
	/// Refer to the Application Note <i>IDS Custom and Proprietary Commands_UHF.pdf</i>, where all parameters are described in detail.
	unsigned char GetTagError();

	/// @brief Initialization method
	///
	/// <b>Description</b><br>
	/// Clears all data members
	virtual void Init();					// set all data members to 0

#if _MSC_VER > 1200
	/// @brief constant for [0xA0] Set Password and [0xAE] Open Area
	///
	/// 
	static const unsigned char PW_LEVEL_SYSTEM			= 0x01;
	/// @brief constant for [0xA0] Set Password and [0xAE] Open Area
	///
	/// 
	static const unsigned char PW_LEVEL_USER			= 0x02;
	/// @brief constant for [0xA0] Set Password and [0xAE] Open Area
	///
	/// 
	static const unsigned char PW_LEVEL_MEASUREMENT		= 0x03;

	/// @brief constant for [0xA1] Set Log Mode
	///
	/// 
	static const unsigned char LOG_MODE_DENSE						= 0x00;
	/// @brief constant for [0xA1] Set Log Mode
	///
	/// 
	static const unsigned char LOG_MODE_ALL_VALUES_OUT_OF_LIMITS	= 0x01;
	/// @brief constant for [0xA1] Set Log Mode
	///
	/// 
	static const unsigned char LOG_MODE_LIMITS_CROSSING				= 0x03;
	/// @brief constant for [0xA1] Set Log Mode
	///
	/// 
	static const unsigned char LOG_MODE_IRQ_EXT1					= 0x05;
	/// @brief constant for [0xA1] Set Log Mode
	///
	/// 
	static const unsigned char LOG_MODE_IRQ_EXT2					= 0x06;
	/// @brief constant for [0xA1] Set Log Mode
	///
	/// 
	static const unsigned char LOG_MODE_IRQ_EXT1_EXT2				= 0x07;

	/// @brief constant for [0xAF] Access FIFO
	///
	/// 
	static const unsigned char SUB_CMD_READ_FIFO		= 0x04;
	/// @brief constant for [0xAF] Access FIFO
	///
	/// 
	static const unsigned char SUB_CMD_WRITE_FIFO		= 0x05;
	/// @brief constant for [0xAF] Access FIFO
	///
	/// 
	static const unsigned char SUB_CMD_READ_STATUS		= 0x06;
#else // for Visual Studio 6
	// constants for [0xA0] Set Password and [0xAE] Open Area
	static const unsigned char PW_LEVEL_SYSTEM;
	static const unsigned char PW_LEVEL_USER;
	static const unsigned char PW_LEVEL_MEASUREMENT;
	// constants for [0xA1] Set Log Mode
	static const unsigned char LOG_MODE_DENSE;
	static const unsigned char LOG_MODE_ALL_VALUES_OUT_OF_LIMITS;
	static const unsigned char LOG_MODE_LIMITS_CROSSING;
	static const unsigned char LOG_MODE_IRQ_EXT1;
	static const unsigned char LOG_MODE_IRQ_EXT2;
	static const unsigned char LOG_MODE_IRQ_EXT1_EXT2;
	// constants for [0xAF] Access FIFO
	static const unsigned char SUB_CMD_READ_FIFO;
	static const unsigned char SUB_CMD_WRITE_FIFO;
	static const unsigned char SUB_CMD_READ_STATUS;
#endif

protected:
	// constructor
	FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A(FEDM_ISCReader* pReader, FEDM_ISOTabItem* pTabItem);
	// 2. constructor
	FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A(FEDM_ISCReader* pReader, unsigned int uiTagHandlerType, FEDM_ISOTabItem* pTabItem);
	// destructor
	virtual ~FedmIscTagHandler_EPC_Class1_Gen2_IDS_SL900A();

	unsigned char	m_ucTagError;

};

#endif // #if !defined(_FEDM_NO_TAG_HANDLER_EPC_C1_G2)

#endif // !defined(_FEDM_ISC_TAG_HANDLER_EPC_CLASS1_GEN2_IDS_SL900A_H_INCLUDED_)
