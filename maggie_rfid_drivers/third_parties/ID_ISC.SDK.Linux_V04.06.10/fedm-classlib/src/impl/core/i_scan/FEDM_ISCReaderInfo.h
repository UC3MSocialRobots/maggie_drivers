/*-------------------------------------------------------
|                                                       |
|                  FEDM_ISCReaderInfo.h                 |
|                                                       |
---------------------------------------------------------

Copyright © 2007-2014	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	19.06.2007

Version       		:	04.06.08 / 13.10.2014 / M. Hultsch
						- additional LAN link information with mode=0x50

						04.02.06 / 25.07.2012 / M. Hultsch
						- new mode 0x08 (with firmware supported tag drivers)

						04.00.09 / 01.02.2012 / M. Hultsch
						- mode 0x04: modification for CPR-Reader

						03.03.03 / 09.05.2011 / M. Hultsch
						- new elements: ucReaderClass, ucReaderClassGen, ucReaderTypeOfClass

						03.02.07 / 29.10.2010 / M. Hultsch

Operation Systems	:	independent

Function			:	class for collecting all reader information


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_READER_INFO_)
#define _FEDM_ISC_READER_INFO_

#if defined(_FEDM_LINUX)
	#include <string.h>
#endif



//#####################################################################################
// structure
//#####################################################################################

/// Structure for collecting all reader information
typedef struct _FEDM_ISC_READER_INFO
{
	// information from RF-Controller
	// [0x66] and Mode=0x00

	unsigned char	ucRfcSwVer[2];				///< [0x66] and Mode=0x00: major and minor version number of RFC
	unsigned char	ucRfcDevVer;				///< [0x66] and Mode=0x00: developer version number of RFC
	unsigned char	ucRfcHwType;				///< [0x66] and Mode=0x00: flag field with hardware options of RFC
	
	union
	{
		unsigned char	ucReaderType;			///< [0x66] and Mode=0x00: Reader Type (Bit 7 = 0)
		unsigned char	ucReaderClass;			///< [0x66] and Mode=0x00: Reader Class (Bit 7 = 1)
	};
	
	unsigned char	ucRfcTrType[2];				///< [0x66] and Mode=0x00: flag field with supported Transponder types
	unsigned char	ucRfcRxBufferSize[2];		///< [0x66] and Mode=0x00: size of Reader's receive buffer
	unsigned char	ucRfcTxBufferSize[2];		///< [0x66] and Mode=0x00: size of Reader's transmit buffer
	unsigned char	ucRfcUhfTrType[2];			///< [0x66] and Mode=0x00: additional flag field with supported UHF Transponder types
	unsigned char	ucReaderClassGen;			///< [0x66] and Mode=0x00: generation of Reader Class
	unsigned char	ucReaderTypeOfClass;		///< [0x66] and Mode=0x00: Reader Type of Class, which is defined in ucReaderClass
	unsigned char	ucReserved3;				///< rfu

	// information from AC-Controller
	// [0x66] and Mode=0x01
	unsigned char	ucAccSwVer[2];				///< [0x66] and Mode=0x01: major and minor version number of ACC
	unsigned char	ucAccDevVer;				///< [0x66] and Mode=0x01: developer version number of ACC
	unsigned char	ucAccHwType;				///< [0x66] and Mode=0x01: flag field with hardware options of ACC
	unsigned char	ucAccEmbAppSwVer[2];		///< [0x66] and Mode=0x01: major and minor version number of 2nd embedded application inside ACC
	unsigned char	ucAccEmbAppDevVer;			///< [0x66] and Mode=0x01: developer version number of 2nd embedded application inside ACC

	// information from USB-Controller
	// [0x66] and Mode=0x02
	unsigned char	ucUsbSwVer[2];				///< [0x66] and Mode=0x02: major and minor version number of USB controller
	unsigned char	ucUsbDevVer;				///< [0x66] and Mode=0x02: developer version number of USB controller
	unsigned char	ucUsbHwType;				///< [0x66] and Mode=0x02: flag field with hardware options of USB controller

	// information from FPGA
	// [0x66] and Mode=0x03
	union
	{
		struct // FPGA Info
		{
			unsigned char	ucFpgaSwVer[2];			///< [0x66] and Mode=0x03: major and minor version number of FPGA/RF-Decoder
			unsigned char	ucFpgaDevVer;			///< [0x66] and Mode=0x03: developer version number of FPGA/RF-Decoder
			unsigned char	ucFpgaHwType;			///< [0x66] and Mode=0x03: flag field with hardware options of FPGA/RF-Decoder
		};

		struct // RF-Decoder Info
		{
			unsigned char	ucDecoderType[5];		///< [0x66] and Mode=0x03: decoder information
			unsigned char	ucSelfTest;				///< [0x66] and Mode=0x03: selt test result
		};
	};

	// additional firmware functionality
	// [0x66] and Mode=0x04
	union
	{
		struct	// CPR-Reader
		{
			unsigned char	ucCprFctTemplate;					///< [0x66] and Mode=0x04: template
			union
			{
				struct // CPRM02, CPR02, CPR04
				{
					unsigned char	ucCprFctList;				///< [0x66] and Mode=0x04: flag field with function list
					unsigned char	ucFwIdentifierA[9];			///< [0x66] and Mode=0x04: firmware identifier
				};

				struct // all modern CPR-Reader
				{
					unsigned char	ucCprFctList0;				///< [0x66] and Mode=0x04: flag field with function list
					unsigned char	ucCprFctList1;				///< [0x66] and Mode=0x04: flag field with function list
					unsigned char	ucCprFctList2;				///< [0x66] and Mode=0x04: flag field with function list
					unsigned char	ucCprRfu0;					///< [0x66] and Mode=0x04: reserved
					unsigned char	ucCprRfu1;					///< [0x66] and Mode=0x04: reserved
					unsigned char	ucCprRfu2;					///< [0x66] and Mode=0x04: reserved
					unsigned char	ucCprRfu3;					///< [0x66] and Mode=0x04: reserved
					unsigned char	ucCprRfu4;					///< [0x66] and Mode=0x04: reserved
					unsigned char	ucFwIdentifierC[2];			///< [0x66] and Mode=0x04: firmware identifier
				};
			};
		};

		struct // MU02/LR/LRU-Reader
		{
			unsigned char	ucReserved1;						///< [0x66] and Mode=0x04: reserved byte
			unsigned char	ucFwIdentifierB[10];				///< [0x66] and Mode=0x04: firmware identifier
		};
	};
	

	// information from bootloader
	// [0x66] and Mode=0x05
	unsigned char	ucBootSwVer[2];				///< [0x66] and Mode=0x05: major and minor version number of Bootloader

	// information from keyboard unit
	// [0x66] and Mode=0x06
	unsigned char	ucKeybStatus[2];			///< [0x66] and Mode=0x06: 2 byte with status flags of Keyboard
	unsigned char	ucKeybSwVer[2];				///< [0x66] and Mode=0x06: major and minor version number of Keyboard
	unsigned char	ucKeybHwVer[4];				///< [0x66] and Mode=0x06: hardware version of Keyboard

	// information from CPRIO unit
	// [0x66] and Mode=0x07
	unsigned char	ucCprioSwVer[2];			///< [0x66] and Mode=0x07: major and minor version number of CPRIO
	unsigned char	ucCprioHwVer;				///< [0x66] and Mode=0x07: hardware revison number of CPRIO
	unsigned char	ucCprioNoOfInputs;			///< [0x66] and Mode=0x07: number of inputs of CPRIO
	unsigned char	ucCprioNoOfOutputs;			///< [0x66] and Mode=0x07: number of outputs of CPRIO
	unsigned char	ucCprioNoOfRelays;			///< [0x66] and Mode=0x07: number of relays of CPRIO
	unsigned char	ucCprioNoOfSignalers;		///< [0x66] and Mode=0x07: number of signalers of CPRIO

	// information with supported tag drivers
	// [0x66] and Mode=0x08
	unsigned char	ucRfcSupportedTagDriver[14];	///< [0x66] and Mode=0x08: with firmware supported tag drivers

	// information from WLAN-Module
	// [0x66] and Mode=0x09
	union
	{
		struct // CPRM02, CPR02, CPR04
		{
			unsigned char	ucWlanVer[14];			///< [0x66] and Mode=0x09: version info
		};

		struct // PRH200
		{
			//unsigned int	
		};
	};

	// hardware information of RF-Controller
	// [0x66] and Mode=0x10
	unsigned char	ucRfcHwInfo[2];				///< [0x66] and Mode=0x10: flag field with hardware info
	unsigned char	ucRfcDHw[2];				///< [0x66] and Mode=0x10: for internal use
	unsigned char	ucRfcAHw[2];				///< [0x66] and Mode=0x10: for internal use
	unsigned char	ucRfcFrequency;				///< [0x66] and Mode=0x10: frequency information (HF, UHF, .., EU, FCC)
	unsigned char	ucRfcPortTypes;				///< [0x66] and Mode=0x10: flag field with supported port types
	unsigned char	ucReserved2;				///< [0x66] and Mode=0x10: reserved byte
	unsigned char	ucRfcInfo;					///< [0x66] and Mode=0x10: RFC info
	unsigned char	ucBatteryStatus;			///< [0x66] and Mode=0x10: battery status in percentage (only MLC.COM)

	// SAM information of RF-Controller
	// [0x66] and Mode=0x11
	unsigned char	ucNoOfSlots;				///< [0x66] and Mode=0x11: number of installed SAM slots
	unsigned char	ucSlotInfo[4];				///< [0x66] and Mode=0x11: slot info for each SAM

	// general CPU information of RF-Controller
	// [0x66] and Mode=0x12
	unsigned char	ucRfcCpuManufacturer;		///< [0x66] and Mode=0x12: manufacturer id
	unsigned char	ucRfcCpuType[2];			///< [0x66] and Mode=0x12: cpu type id
	unsigned char	ucRfcCpuRevision;			///< [0x66] and Mode=0x12: cpu revision code
	unsigned char	ucRfcCpuFlashSize[4];		///< [0x66] and Mode=0x12: size in KB of flash

	// CFG information with read and write permission
	// [0x66] and Mode=0x40 and Mode=0x41
	unsigned char	ucNoOfCfgPages[2];			///< [0x66] and Mode=0x40 or 0x41: number of configuration pages
	unsigned char	ucCfgReadPermissions[16];	///< [0x66] and Mode=0x40 or 0x41: flag field with readable CFG pages
	unsigned char	ucCfgWritePermissions[16];	///< [0x66] and Mode=0x40 or 0x41: flag field with writeable CFG pages

	// information with MAC for LAN
	// [0x66] and Mode=0x50
	unsigned char	ucLanFlags;					///< [0x66] and Mode=0x50: flags
	unsigned char	ucLanMac[6];				///< [0x66] and Mode=0x50: media access control
	unsigned char	ucLanLinkInfo;				///< [0x66] and Mode=0x50: additional link information

	// information with IP Address for LAN
	// [0x66] and Mode=0x51
	unsigned char	ucLanIpV4[4];				///< [0x66] and Mode=0x51: IP Address V4
	unsigned char	ucLanIpV6[16];				///< [0x66] and Mode=0x51: IP Address V6

	// information with netmask for LAN
	// [0x66] and Mode=0x52
	unsigned char	ucLanNetmaskV4[4];			///< [0x66] and Mode=0x52: netmask V4
	unsigned char	ucLanNetmaskV6[16];			///< [0x66] and Mode=0x52: netmask V6

	// information with Gateway Address for LAN
	// [0x66] and Mode=0x53
	unsigned char	ucLanGatewayV4[4];			///< [0x66] and Mode=0x53: Gateway Address V4
	unsigned char	ucLanGatewayV6[16];			///< [0x66] and Mode=0x53: Gateway Address V6

	// information with MAC for WLAN
	// [0x66] and Mode=0x54
	unsigned char	ucWlanFlags;				///< [0x66] and Mode=0x54: flags
	unsigned char	ucWlanMac[6];				///< [0x66] and Mode=0x54: media access control

	// information with IP Address for WLAN
	// [0x66] and Mode=0x55
	unsigned char	ucWlanIpV4[4];				///< [0x66] and Mode=0x55: IP Address V4
	unsigned char	ucWlanIpV6[16];				///< [0x66] and Mode=0x55: IP Address V6

	// information with netmask for WLAN
	// [0x66] and Mode=0x56
	unsigned char	ucWlanNetmaskV4[4];			///< [0x66] and Mode=0x56: netmask V4
	unsigned char	ucWlanNetmaskV6[16];		///< [0x66] and Mode=0x56: netmask V6

	// information with Gateway Address for LAN
	// [0x66] and Mode=0x57
	unsigned char	ucWlanGatewayV4[4];			///< [0x66] and Mode=0x57: Gateway Address V4
	unsigned char	ucWlanGatewayV6[16];		///< [0x66] and Mode=0x57: Gateway Address V6

	// input/output capabilities information
	// [0x66] and Mode=0x60
	unsigned char	ucNoOfInputs;				///< [0x66] and Mode=0x60: number of inputs
	unsigned char	ucNoOfOutputs;				///< [0x66] and Mode=0x60: number of outputs
	unsigned char	ucNoOfRelays;				///< [0x66] and Mode=0x60: number of relays

	// external peripheral devices information
	// [0x66] and Mode=0x61
	unsigned char	ucNoOfPeripheralDevices;	///< [0x66] and Mode=0x61: number of detected devices

	/// Structure collecting information about connected Peripheral Devices (e.g. People-Counter)
	struct PERIPHERAL_DEVICE
	{
		unsigned char	ucPDType;				///< [0x66] and Mode=0x61: device type
		unsigned char	ucPDBusAdr;				///< [0x66] and Mode=0x61: bus addresses

	} aPeripheralDevice[3];

	// device information of RF-Controller
	// [0x66] and Mode=0x80
	unsigned char	ucRfcDevID[4];				///< [0x66] and Mode=0x80: Device-IF (serial number)
	unsigned char	ucRfcLicenseCustom[4];		///< [0x66] and Mode=0x80: licensed customer version
	unsigned char	ucRfcLicenseFw[2];			///< [0x66] and Mode=0x80: licensed firmware version
	unsigned char	ucRfcLicenseTrType[2];		///< [0x66] and Mode=0x80: licensed transponder driver
	unsigned char	ucRfcLicenseFct[2];			///< [0x66] and Mode=0x80: licensed functions
	unsigned char	ucRfcLicenseUhfTrType[2];	///< [0x66] and Mode=0x80: licensed uhf transponder driver (for dual frequency reader)
	unsigned char	ucRfcLicenseUhfFct[2];		///< [0x66] and Mode=0x80: licensed uhf functions (for dual frequency reader)

	bool bIsInfoRead;		///< indicates, if structure contains valid data
	bool bIsMode0x00Read;	///< indicates, if data with Mode 0x00 is read
	bool bIsMode0x01Read;	///< indicates, if data with Mode 0x01 is read
	bool bIsMode0x02Read;	///< indicates, if data with Mode 0x02 is read
	bool bIsMode0x03Read;	///< indicates, if data with Mode 0x03 is read
	bool bIsMode0x04Read;	///< indicates, if data with Mode 0x04 is read
	bool bIsMode0x05Read;	///< indicates, if data with Mode 0x05 is read
	bool bIsMode0x06Read;	///< indicates, if data with Mode 0x06 is read
	bool bIsMode0x07Read;	///< indicates, if data with Mode 0x07 is read
	bool bIsMode0x08Read;	///< indicates, if data with Mode 0x08 is read
	bool bIsMode0x09Read;	///< indicates, if data with Mode 0x09 is read
	bool bIsMode0x10Read;	///< indicates, if data with Mode 0x10 is read
	bool bIsMode0x11Read;	///< indicates, if data with Mode 0x11 is read
	bool bIsMode0x12Read;	///< indicates, if data with Mode 0x12 is read
	bool bIsMode0x40Read;	///< indicates, if data with Mode 0x40 is read
	bool bIsMode0x41Read;	///< indicates, if data with Mode 0x41 is read
	bool bIsMode0x50Read;	///< indicates, if data with Mode 0x50 is read
	bool bIsMode0x51Read;	///< indicates, if data with Mode 0x51 is read
	bool bIsMode0x52Read;	///< indicates, if data with Mode 0x52 is read
	bool bIsMode0x53Read;	///< indicates, if data with Mode 0x53 is read
	bool bIsMode0x54Read;	///< indicates, if data with Mode 0x54 is read
	bool bIsMode0x55Read;	///< indicates, if data with Mode 0x55 is read
	bool bIsMode0x56Read;	///< indicates, if data with Mode 0x56 is read
	bool bIsMode0x57Read;	///< indicates, if data with Mode 0x57 is read
	bool bIsMode0x60Read;	///< indicates, if data with Mode 0x60 is read
	bool bIsMode0x61Read;	///< indicates, if data with Mode 0x61 is read
	bool bIsMode0x80Read;	///< indicates, if data with Mode 0x80 is read
	bool bIsRfcFrequency;	///< indicates, if RFC Frequency is read

	/// initialization
	void Init()
	{
		memset(&ucRfcSwVer[0], 0, sizeof(_FEDM_ISC_READER_INFO));
		memset(&ucRfcSupportedTagDriver[0], 0xFF, 14);
	}

	/// Request of Reader Type Number
	/// All Reader Type Numbers are listed in OBID::FEDM::Core::Const::ReaderType::i_scan
	unsigned int GetReaderType()
	{
		if (!bIsMode0x00Read)
			return 0;

		if(ucReaderType & 0x80)
		{
			// class information
			return	((unsigned int)ucReserved3<<24) +
					((unsigned int)ucReaderTypeOfClass<<16) +
					((unsigned int)ucReaderClassGen<<8)  +
					 (unsigned int)ucReaderClass;
		}
		else
		{
			// single type number
			return (unsigned int)ucReaderClass;
		}
	}

	/// Returns size of RF-Controller receive buffer
	unsigned int GetRfcRxBufferSize()
	{
		if (!bIsMode0x01Read)
			return 0;

		return ((unsigned int)ucRfcRxBufferSize[0] << 8) + (unsigned int)ucRfcRxBufferSize[1];
	}

	/// Returns size of RF-Controller transmit buffer
	unsigned int GetRfcTxBufferSize()
	{
		if (!bIsMode0x01Read)
			return 0;

		return ((unsigned int)ucRfcTxBufferSize[0] << 8) + (unsigned int)ucRfcTxBufferSize[1];
	}

	/// Return of Device-ID (decimal)
	unsigned int GetDeviceID()
	{
		if(!bIsMode0x80Read)
			return 0;
			
		return	((unsigned int)ucRfcDevID[0]<<24) + 
				((unsigned int)ucRfcDevID[1]<<16) + 
				((unsigned int)ucRfcDevID[2]<<8)  + 
				 (unsigned int)ucRfcDevID[3];
	}

	/// Return the number of Reader's configuration pages
	unsigned int GetNoOfCfgPages()
	{
		if(!bIsMode0x40Read && !bIsMode0x41Read)
			return 0;
			
		return	((unsigned int)ucNoOfCfgPages[0]<<8)  + 
				 (unsigned int)ucNoOfCfgPages[1];
	}

} FEDM_ISC_READER_INFO;


#endif // !defined(_FEDM_ISC_READER_INFO_)
