/*-------------------------------------------------------
|                                                       |
|                  FEDM_ISCReaderDiagnostic.h           |
|                                                       |
---------------------------------------------------------

Copyright © 2008-2014	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Markus Hultsch
Begin        		:	08.12.2008

Version       		:	04.06.10 / 10.12.2014 / M. Hultsch
						- new mode 0x08

						04.06.01 / 03.03.2014 / M. Hultsch
						- new mode 0x07

						04.03.01 / 05.11.2012 / M. Hultsch
						- new mode 0x21

						03.03.00 / 10.02.2011 / M. Hultsch

Operation Systems	:	independent

Function			:	class for collecting all reader diagnostic status


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/

#if !defined(_FEDM_ISC_READER_DIAGNOSTIC_)
#define _FEDM_ISC_READER_DIAGNOSTIC_





//#####################################################################################
// structure
//#####################################################################################

/// Structure for collecting all reader diagnostic data
typedef struct _FEDM_ISC_READER_DIAGNOSTIC
{
	// Reader and Antenna Status
	// [0x6E] and Mode=0x01
	union
	{
		unsigned char	ucFlagsA;						///< [0x6E] and Mode=0x01: Flags byte A
		struct
		{
			unsigned char	bRfHardware:		1;		///< [0x6E] and Mode=0x01: Flags byte A, bit 0: RF-Hardware error
			unsigned char	bRfNoise:			1;		///< [0x6E] and Mode=0x01: Flags byte A, bit 1: RF-Noise problem
			unsigned char	bRfPhaseError_Less:	1;		///< [0x6E] and Mode=0x01: Flags byte A, bit 2: only HF Reader
			unsigned char	bRfPhaseError_More:	1;		///< [0x6E] and Mode=0x01: Flags byte A, bit 3: only HF Reader
			unsigned char	bRfPowerAmp:		1;		///< [0x6E] and Mode=0x01: Flags byte A, bit 4:
			unsigned char	bTempWarning:		1;		///< [0x6E] and Mode=0x01: Flags byte A, bit 5: Temperature Warning (only HF Reader)
			unsigned char	bSyncTimeout:		1;		///< [0x6E] and Mode=0x01: Flags byte A, bit 6:
			unsigned char	bTempAlarm:			1;		///< [0x6E] and Mode=0x01: Flags byte A, bit 7: Temperature Alarm
		};
		struct
		{
			unsigned char	bDummy1:			5;		///< [0x6E] and Mode=0x01: Flags byte A, unused bits
			unsigned char	bChannelAlloc:		1;		///< [0x6E] and Mode=0x01: Flags byte A, bit 5: UHF Reader
			unsigned char	bDummy2:			2;		///< [0x6E] and Mode=0x01: Flags byte A, unused bits
		};
	};

	union
	{
		unsigned char	ucFlagsB;						///< [0x6E] and Mode=0x01: Flags byte B
		struct
		{
			unsigned char	bRfPhaseError_Ant1:	1;		///< [0x6E] and Mode=0x01: Flags byte B, bit 0: Phase Error at internal Antenna Output 1 (only UHF Reader)
			unsigned char	bRfPhaseError_Ant2: 1;		///< [0x6E] and Mode=0x01: Flags byte B, bit 1: Phase Error at internal Antenna Output 2 (only UHF Reader)
			unsigned char	bRfPhaseError_Ant3: 1;		///< [0x6E] and Mode=0x01: Flags byte B, bit 2: Phase Error at internal Antenna Output 3 (only UHF Reader)
			unsigned char	bRfPhaseError_Ant4: 1;		///< [0x6E] and Mode=0x01: Flags byte B, bit 3: Phase Error at internal Antenna Output 4 (only UHF Reader)
			unsigned char	bDummy3:			4;		///< [0x6E] and Mode=0x01: Flags byte B, bits 4-7: unused bits
		};
		struct
		{
			unsigned char	bMuxStatus:			1;		///< [0x6E] and Mode=0x01: Flags byte B, bit 0: Multiplexer status (only HF Reader)
			unsigned char	bDatStatus:			1;		///< [0x6E] and Mode=0x01: Flags byte B, bit 1: Dynamic Antenna Tuner status (only HF Reader)
			unsigned char	bDCAntError:		1;		///< [0x6E] and Mode=0x01: Flags byte B, bit 2: DC-Power error at Antenna Output (only HF Reader)
			unsigned char	bDummy4:			5;		///< [0x6E] and Mode=0x01: Flags byte B, bits 3-7: unused bits
		};

	};

	// RF Power Amplifier Status
	// [0x6E] and Mode=0x02
	// only ISC.LR200, ISC.LR2000 and ISC.MR200
	union
	{
		struct
		{
			unsigned short int uiImpedance;		///< [0x6E] and Mode=0x02: Impedance-Status (only for ID ISC.MR200)
		};

		struct
		{
			unsigned char ucRfPower;			///< [0x6E] and Mode=0x02: RF-Power status (only for ID ISC.LR200, ID ISC.LR2000)
			unsigned char ucRfModulation;		///< [0x6E] and Mode=0x02: RF-Modulation status (only for ID ISC.LR2000)
			unsigned char ucAmpTemp;			///< [0x6E] and Mode=0x02: Amplifier-Temperature status (only for ID ISC.LR200, ID ISC.LR2000)
		};
	};

	// Multiplexer Status
	// [0x6E] and Mode=0x03
	unsigned char ucMuxStatusCh1;				///< [0x6E] and Mode=0x03: Impedance-Status of Output Channel 1 of external Multiplexer (only for HF Reader)
	unsigned char ucMuxStatusCh2;				///< [0x6E] and Mode=0x03: Impedance-Status of Output Channel 2 of external Multiplexer (only for HF Reader)
	unsigned char ucMuxStatusCh3;				///< [0x6E] and Mode=0x03: Impedance-Status of Output Channel 3 of external Multiplexer (only for HF Reader)
	unsigned char ucMuxStatusCh4;				///< [0x6E] and Mode=0x03: Impedance-Status of Output Channel 4 of external Multiplexer (only for HF Reader)
	unsigned char ucMuxStatusCh5;				///< [0x6E] and Mode=0x03: Impedance-Status of Output Channel 5 of external Multiplexer (only for HF Reader)
	unsigned char ucMuxStatusCh6;				///< [0x6E] and Mode=0x03: Impedance-Status of Output Channel 6 of external Multiplexer (only for HF Reader)
	unsigned char ucMuxStatusCh7;				///< [0x6E] and Mode=0x03: Impedance-Status of Output Channel 7 of external Multiplexer (only for HF Reader)
	unsigned char ucMuxStatusCh8;				///< [0x6E] and Mode=0x03: Impedance-Status of Output Channel 8 of external Multiplexer (only for HF Reader)

	// EEPROM and FPGA Status
	// [0x6E] and Mode=0x04
	unsigned char	ucIntErrorA;						///< [0x6E] and Mode=0x04: Internal Error flag field A

	union
	{
		unsigned char	ucIntErrorB;					///< [0x6E] and Mode=0x04: Internal Error flag field B
		struct
		{
			unsigned char	bEepromDevice1:		1;		///< [0x6E] and Mode=0x04: Internal Error flag field B, bit 0: EEPROM error in Device 1
			unsigned char	bEepromDevice2:		1;		///< [0x6E] and Mode=0x04: Internal Error flag field B, bit 1: EEPROM error in Device 2
			unsigned char	bHwFilter:			1;		///< [0x6E] and Mode=0x04: Internal Error flag field B, bit 2: Hardware Filter problem
			unsigned char	bRfDecoder:			1;		///< [0x6E] and Mode=0x04: Internal Error flag field B, bit 3: RF-Decoder problem
			unsigned char	bParaMismatch:		1;		///< [0x6E] and Mode=0x04: Internal Error flag field B, bit 4: Parameter mismatch
			unsigned char	bDummy5:			3;		///< [0x6E] and Mode=0x04: Internal Error flag field B, bits 5-7: unused bits
		};
	};

	// Multiplexer Impedance Status
	/// [0x6E] and Mode=0x05 (only HF/UHF LR-Reader): Status of external Multiplexer, connected at internal Antenna Output 1
#if defined(__BORLANDC__)
	union _CNTRL_OUT1
#else
	union CNTRL_OUT1
#endif
	{
		/// [0x6E] and Mode=0x05 (only HF/UHF LR-Reader): Power- or Impedance-Status of Antenna Outputs of external Multiplexer, connected at internal Antenna Output 1
		unsigned char	ucMux_PowerAmp;
		struct
		{
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 1 of external Multiplexer, connected at internal Antenna Output 1:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 1 of external Multiplexer, connected at internal Antenna Output 1:<br>
			unsigned char 	bMuxCh1_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 2 of external Multiplexer, connected at internal Antenna Output 1:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 2 of external Multiplexer, connected at internal Antenna Output 1:<br>
			unsigned char 	bMuxCh2_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 3 of external Multiplexer, connected at internal Antenna Output 1:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 3 of external Multiplexer, connected at internal Antenna Output 1:<br>
			unsigned char 	bMuxCh3_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 4 of external Multiplexer, connected at internal Antenna Output 1:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 4 of external Multiplexer, connected at internal Antenna Output 1:<br>
			unsigned char 	bMuxCh4_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 5 of external Multiplexer, connected at internal Antenna Output 1:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 5 of external Multiplexer, connected at internal Antenna Output 1:<br>
			unsigned char 	bMuxCh5_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 6 of external Multiplexer, connected at internal Antenna Output 1:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 6 of external Multiplexer, connected at internal Antenna Output 1:<br>
			unsigned char 	bMuxCh6_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 7 of external Multiplexer, connected at internal Antenna Output 1:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 7 of external Multiplexer, connected at internal Antenna Output 1:<br>
			unsigned char 	bMuxCh7_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 8 of external Multiplexer, connected at internal Antenna Output 1:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 8 of external Multiplexer, connected at internal Antenna Output 1:<br>
			unsigned char 	bMuxCh8_PowerAmp:			1;
		};
	} CNTRL_OUT1;

	/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Outputs of external Multiplexer, connected at internal Antenna Output 1
#if defined(__BORLANDC__)
	union _PHASE_LT50_OUT1
#else
	union PHASE_LT50_OUT1
#endif
	{
		/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Outputs of external Multiplexer, connected at internal Antenna Output 1
		unsigned char	ucMux_PhaseError_Less;
		struct
		{
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 1 of external Multiplexer, connected at internal Antenna Output 1
			unsigned char 	bMuxCh1_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 2 of external Multiplexer, connected at internal Antenna Output 1
			unsigned char 	bMuxCh2_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 3 of external Multiplexer, connected at internal Antenna Output 1
			unsigned char 	bMuxCh3_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 4 of external Multiplexer, connected at internal Antenna Output 1
			unsigned char 	bMuxCh4_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 5 of external Multiplexer, connected at internal Antenna Output 1
			unsigned char 	bMuxCh5_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 6 of external Multiplexer, connected at internal Antenna Output 1
			unsigned char 	bMuxCh6_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 7 of external Multiplexer, connected at internal Antenna Output 1
			unsigned char 	bMuxCh7_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 8 of external Multiplexer, connected at internal Antenna Output 1
			unsigned char 	bMuxCh8_PhaseError_Less:	1;
		};
	} PHASE_LT50_OUT1;

	/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Outputs of external Multiplexer, connected at internal Antenna Output 1
#if defined(__BORLANDC__)
	union _PHASE_GT50_OUT1
#else
	union PHASE_GT50_OUT1
#endif
	{
		/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Outputs of external Multiplexer, connected at internal Antenna Output 1
		unsigned char	ucMux_PhaseError_More;
		struct
		{
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 1 of external Multiplexer, connected at internal Antenna Output 1
			unsigned char 	bMuxCh1_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 2 of external Multiplexer, connected at internal Antenna Output 1
			unsigned char 	bMuxCh2_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 3 of external Multiplexer, connected at internal Antenna Output 1
			unsigned char 	bMuxCh3_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 4 of external Multiplexer, connected at internal Antenna Output 1
			unsigned char 	bMuxCh4_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 5 of external Multiplexer, connected at internal Antenna Output 1
			unsigned char 	bMuxCh5_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 6 of external Multiplexer, connected at internal Antenna Output 1
			unsigned char 	bMuxCh6_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 7 of external Multiplexer, connected at internal Antenna Output 1
			unsigned char 	bMuxCh7_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 8 of external Multiplexer, connected at internal Antenna Output 1
			unsigned char 	bMuxCh8_PhaseError_More:	1;
		};
	} PHASE_GT50_OUT1;
	
	/// [0x6E] and Mode=0x05 (only HF/UHF LR-Reader): Status of external Multiplexer, connected at internal Antenna Output 2
#if defined(__BORLANDC__)
	union _CNTRL_OUT2
#else
	union CNTRL_OUT2
#endif
	{
		/// [0x6E] and Mode=0x05 (only HF/UHF LR-Reader): Power- or Impedance-Status of Antenna Outputs of external Multiplexer, connected at internal Antenna Output 2
		unsigned char	ucMux_PowerAmp;
		struct
		{
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 1 of external Multiplexer, connected at internal Antenna Output 2:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 1 of external Multiplexer, connected at internal Antenna Output 2:<br>
			unsigned char 	bMuxCh1_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 2 of external Multiplexer, connected at internal Antenna Output 2:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 2 of external Multiplexer, connected at internal Antenna Output 2:<br>
			unsigned char 	bMuxCh2_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 3 of external Multiplexer, connected at internal Antenna Output 2:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 3 of external Multiplexer, connected at internal Antenna Output 2:<br>
			unsigned char 	bMuxCh3_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 4 of external Multiplexer, connected at internal Antenna Output 2:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 4 of external Multiplexer, connected at internal Antenna Output 2:<br>
			unsigned char 	bMuxCh4_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 5 of external Multiplexer, connected at internal Antenna Output 2:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 5 of external Multiplexer, connected at internal Antenna Output 2:<br>
			unsigned char 	bMuxCh5_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 6 of external Multiplexer, connected at internal Antenna Output 2:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 6 of external Multiplexer, connected at internal Antenna Output 2:<br>
			unsigned char 	bMuxCh6_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 7 of external Multiplexer, connected at internal Antenna Output 2:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 7 of external Multiplexer, connected at internal Antenna Output 2:<br>
			unsigned char 	bMuxCh7_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 8 of external Multiplexer, connected at internal Antenna Output 2:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 8 of external Multiplexer, connected at internal Antenna Output 2:<br>
			unsigned char 	bMuxCh8_PowerAmp:			1;
		};
	} CNTRL_OUT2;

	/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Outputs of external Multiplexer, connected at internal Antenna Output 2
#if defined(__BORLANDC__)
	union _PHASE_LT50_OUT2
#else
	union PHASE_LT50_OUT2
#endif
	{
		/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Outputs of external Multiplexer, connected at internal Antenna Output 2
		unsigned char	ucMux_PhaseError_Less;
		struct
		{
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 1 of external Multiplexer, connected at internal Antenna Output 2
			unsigned char 	bMuxCh1_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 2 of external Multiplexer, connected at internal Antenna Output 2
			unsigned char 	bMuxCh2_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 3 of external Multiplexer, connected at internal Antenna Output 2
			unsigned char 	bMuxCh3_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 4 of external Multiplexer, connected at internal Antenna Output 2
			unsigned char 	bMuxCh4_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 5 of external Multiplexer, connected at internal Antenna Output 2
			unsigned char 	bMuxCh5_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 6 of external Multiplexer, connected at internal Antenna Output 2
			unsigned char 	bMuxCh6_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 7 of external Multiplexer, connected at internal Antenna Output 2
			unsigned char 	bMuxCh7_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 8 of external Multiplexer, connected at internal Antenna Output 2
			unsigned char 	bMuxCh8_PhaseError_Less:	1;
		};
	} PHASE_LT50_OUT2;

	/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Outputs of external Multiplexer, connected at internal Antenna Output 2
#if defined(__BORLANDC__)
	union _PHASE_GT50_OUT2
#else
	union PHASE_GT50_OUT2
#endif
	{
		/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Outputs of external Multiplexer, connected at internal Antenna Output 2
		unsigned char	ucMux_PhaseError_More;
		struct
		{
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 1 of external Multiplexer, connected at internal Antenna Output 2
			unsigned char 	bMuxCh1_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 2 of external Multiplexer, connected at internal Antenna Output 2
			unsigned char 	bMuxCh2_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 3 of external Multiplexer, connected at internal Antenna Output 2
			unsigned char 	bMuxCh3_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 4 of external Multiplexer, connected at internal Antenna Output 2
			unsigned char 	bMuxCh4_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 5 of external Multiplexer, connected at internal Antenna Output 2
			unsigned char 	bMuxCh5_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 6 of external Multiplexer, connected at internal Antenna Output 2
			unsigned char 	bMuxCh6_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 7 of external Multiplexer, connected at internal Antenna Output 2
			unsigned char 	bMuxCh7_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 8 of external Multiplexer, connected at internal Antenna Output 2
			unsigned char 	bMuxCh8_PhaseError_More:	1;
		};
	} PHASE_GT50_OUT2;
	
	/// [0x6E] and Mode=0x05 (only HF/UHF LR-Reader): Status of external Multiplexer, connected at internal Antenna Output 3
#if defined(__BORLANDC__)
	union _CNTRL_OUT3
#else
	union CNTRL_OUT3
#endif
	{
		/// [0x6E] and Mode=0x05 (only HF/UHF LR-Reader): Power- or Impedance-Status of Antenna Outputs of external Multiplexer, connected at internal Antenna Output 3
		unsigned char	ucMux_PowerAmp;
		struct
		{
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 1 of external Multiplexer, connected at internal Antenna Output 3:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 1 of external Multiplexer, connected at internal Antenna Output 3:<br>
			unsigned char 	bMuxCh1_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 2 of external Multiplexer, connected at internal Antenna Output 3:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 2 of external Multiplexer, connected at internal Antenna Output 3:<br>
			unsigned char 	bMuxCh2_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 3 of external Multiplexer, connected at internal Antenna Output 3:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 3 of external Multiplexer, connected at internal Antenna Output 3:<br>
			unsigned char 	bMuxCh3_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 4 of external Multiplexer, connected at internal Antenna Output 3:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 4 of external Multiplexer, connected at internal Antenna Output 3:<br>
			unsigned char 	bMuxCh4_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 5 of external Multiplexer, connected at internal Antenna Output 3:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 5 of external Multiplexer, connected at internal Antenna Output 3:<br>
			unsigned char 	bMuxCh5_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 6 of external Multiplexer, connected at internal Antenna Output 3:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 6 of external Multiplexer, connected at internal Antenna Output 3:<br>
			unsigned char 	bMuxCh6_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 7 of external Multiplexer, connected at internal Antenna Output 3:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 7 of external Multiplexer, connected at internal Antenna Output 3:<br>
			unsigned char 	bMuxCh7_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 8 of external Multiplexer, connected at internal Antenna Output 3:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 8 of external Multiplexer, connected at internal Antenna Output 3:<br>
			unsigned char 	bMuxCh8_PowerAmp:			1;
		};
	} CNTRL_OUT3;

	/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Outputs of external Multiplexer, connected at internal Antenna Output 3
#if defined(__BORLANDC__)
	union _PHASE_LT50_OUT3
#else
	union PHASE_LT50_OUT3
#endif
	{
		/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Outputs of external Multiplexer, connected at internal Antenna Output 3
		unsigned char	ucMux_PhaseError_Less;
		struct
		{
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 1 of external Multiplexer, connected at internal Antenna Output 3
			unsigned char 	bMuxCh1_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 2 of external Multiplexer, connected at internal Antenna Output 3
			unsigned char 	bMuxCh2_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 3 of external Multiplexer, connected at internal Antenna Output 3
			unsigned char 	bMuxCh3_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 4 of external Multiplexer, connected at internal Antenna Output 3
			unsigned char 	bMuxCh4_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 5 of external Multiplexer, connected at internal Antenna Output 3
			unsigned char 	bMuxCh5_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 6 of external Multiplexer, connected at internal Antenna Output 3
			unsigned char 	bMuxCh6_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 7 of external Multiplexer, connected at internal Antenna Output 3
			unsigned char 	bMuxCh7_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 8 of external Multiplexer, connected at internal Antenna Output 3
			unsigned char 	bMuxCh8_PhaseError_Less:	1;
		};
	} PHASE_LT50_OUT3;

	/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Outputs of external Multiplexer, connected at internal Antenna Output 3
#if defined(__BORLANDC__)
	union _PHASE_GT50_OUT3
#else
	union PHASE_GT50_OUT3
#endif
	{
		/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Outputs of external Multiplexer, connected at internal Antenna Output 3
		unsigned char	ucMux_PhaseError_More;
		struct
		{
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 1 of external Multiplexer, connected at internal Antenna Output 3
			unsigned char 	bMuxCh1_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 2 of external Multiplexer, connected at internal Antenna Output 3
			unsigned char 	bMuxCh2_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 3 of external Multiplexer, connected at internal Antenna Output 3
			unsigned char 	bMuxCh3_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 4 of external Multiplexer, connected at internal Antenna Output 3
			unsigned char 	bMuxCh4_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 5 of external Multiplexer, connected at internal Antenna Output 3
			unsigned char 	bMuxCh5_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 6 of external Multiplexer, connected at internal Antenna Output 3
			unsigned char 	bMuxCh6_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 7 of external Multiplexer, connected at internal Antenna Output 3
			unsigned char 	bMuxCh7_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 8 of external Multiplexer, connected at internal Antenna Output 3
			unsigned char 	bMuxCh8_PhaseError_More:	1;
		};
	} PHASE_GT50_OUT3;
	
	/// [0x6E] and Mode=0x05 (only HF/UHF LR-Reader): Status of external Multiplexer, connected at internal Antenna Output 4
#if defined(__BORLANDC__)
	union _CNTRL_OUT4
#else
	union CNTRL_OUT4
#endif
	{
		/// [0x6E] and Mode=0x05 (only HF/UHF LR-Reader): Power- or Impedance-Status of Antenna Outputs of external Multiplexer, connected at internal Antenna Output 4
		unsigned char	ucMux_PowerAmp;
		struct
		{
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 1 of external Multiplexer, connected at internal Antenna Output 4:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 1 of external Multiplexer, connected at internal Antenna Output 4:<br>
			unsigned char 	bMuxCh1_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 2 of external Multiplexer, connected at internal Antenna Output 4:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 2 of external Multiplexer, connected at internal Antenna Output 4:<br>
			unsigned char 	bMuxCh2_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 3 of external Multiplexer, connected at internal Antenna Output 4:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 3 of external Multiplexer, connected at internal Antenna Output 4:<br>
			unsigned char 	bMuxCh3_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 4 of external Multiplexer, connected at internal Antenna Output 4:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 4 of external Multiplexer, connected at internal Antenna Output 4:<br>
			unsigned char 	bMuxCh4_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 5 of external Multiplexer, connected at internal Antenna Output 4:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 5 of external Multiplexer, connected at internal Antenna Output 4:<br>
			unsigned char 	bMuxCh5_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 6 of external Multiplexer, connected at internal Antenna Output 4:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 6 of external Multiplexer, connected at internal Antenna Output 4:<br>
			unsigned char 	bMuxCh6_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 7 of external Multiplexer, connected at internal Antenna Output 4:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 7 of external Multiplexer, connected at internal Antenna Output 4:<br>
			unsigned char 	bMuxCh7_PowerAmp:			1;
			/// [0x6E] and Mode=0x05:  HF LR-Reader: Power-Status of Antenna Output 8 of external Multiplexer, connected at internal Antenna Output 4:<br>
			/// [0x6E] and Mode=0x05: UHF LR-Reader: Impedance-Status of Antenna Output 8 of external Multiplexer, connected at internal Antenna Output 4:<br>
			unsigned char 	bMuxCh8_PowerAmp:			1;
		};
	} CNTRL_OUT4;

	/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Outputs of external Multiplexer, connected at internal Antenna Output 4
#if defined(__BORLANDC__)
	union _PHASE_LT50_OUT4
#else
	union PHASE_LT50_OUT4
#endif
	{
		/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Outputs of external Multiplexer, connected at internal Antenna Output 4
		unsigned char	ucMux_PhaseError_Less;
		struct
		{
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 1 of external Multiplexer, connected at internal Antenna Output 4
			unsigned char 	bMuxCh1_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 2 of external Multiplexer, connected at internal Antenna Output 4
			unsigned char 	bMuxCh2_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 3 of external Multiplexer, connected at internal Antenna Output 4
			unsigned char 	bMuxCh3_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 4 of external Multiplexer, connected at internal Antenna Output 4
			unsigned char 	bMuxCh4_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 5 of external Multiplexer, connected at internal Antenna Output 4
			unsigned char 	bMuxCh5_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 6 of external Multiplexer, connected at internal Antenna Output 4
			unsigned char 	bMuxCh6_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 7 of external Multiplexer, connected at internal Antenna Output 4
			unsigned char 	bMuxCh7_PhaseError_Less:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (<50R) of Antenna Output 8 of external Multiplexer, connected at internal Antenna Output 4
			unsigned char 	bMuxCh8_PhaseError_Less:	1;
		};
	} PHASE_LT50_OUT4;

	/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Outputs of external Multiplexer, connected at internal Antenna Output 4
#if defined(__BORLANDC__)
	union _PHASE_GT50_OUT4
#else
	union PHASE_GT50_OUT4
#endif
	{
		/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Outputs of external Multiplexer, connected at internal Antenna Output 4
		unsigned char	ucMux_PhaseError_More;
		struct
		{
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 1 of external Multiplexer, connected at internal Antenna Output 4
			unsigned char 	bMuxCh1_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 2 of external Multiplexer, connected at internal Antenna Output 4
			unsigned char 	bMuxCh2_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 3 of external Multiplexer, connected at internal Antenna Output 4
			unsigned char 	bMuxCh3_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 4 of external Multiplexer, connected at internal Antenna Output 4
			unsigned char 	bMuxCh4_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 5 of external Multiplexer, connected at internal Antenna Output 4
			unsigned char 	bMuxCh5_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 6 of external Multiplexer, connected at internal Antenna Output 4
			unsigned char 	bMuxCh6_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 7 of external Multiplexer, connected at internal Antenna Output 4
			unsigned char 	bMuxCh7_PhaseError_More:	1;
			/// [0x6E] and Mode=0x05 (only HF LR-Reader): Impedance-Status (>50R) of Antenna Output 8 of external Multiplexer, connected at internal Antenna Output 4
			unsigned char 	bMuxCh8_PhaseError_More:	1;
		};
	} PHASE_GT50_OUT4;

	// MAX Status
	// [0x6E] and Mode=0x06
	union
	{
		unsigned short int	usErrorFlags;						///< [0x6E] and Mode=0x06: 2 byte Error flag field
		struct
		{
			unsigned short int 	bRTC:					1;		///< [0x6E] and Mode=0x06: Error flag field, bit 0: real-time clock error
			unsigned short int 	bMetadata:				1;		///< [0x6E] and Mode=0x06: Error flag field, bit 1: metadata error
			unsigned short int 	bTimezoneTable:			1;		///< [0x6E] and Mode=0x06: Error flag field, bit 2: timezone table error
			unsigned short int 	bHolidayTable:			1;		///< [0x6E] and Mode=0x06: Error flag field, bit 3: holiday table error
			unsigned short int 	bAccessTable:			1;		///< [0x6E] and Mode=0x06: Error flag field, bit 4: access table error
			unsigned short int 	bRes1:					1;		///< [0x6E] and Mode=0x06: Error flag field, bit 5: reserved
			unsigned short int 	bReaderConfig:			1;		///< [0x6E] and Mode=0x06: Error flag field, bit 6: Reader configuration problem
			unsigned short int 	bTableUpdate:			1;		///< [0x6E] and Mode=0x06: Error flag field, bit 7: table update problem
			unsigned short int 	bRes2:					8;		///< [0x6E] and Mode=0x06: Error flag field, bit 8-15: reserved
		};
	};

	unsigned short int	usEventTableSize;						///< [0x6E] and Mode=0x06: Event table size
	unsigned short int	usEventTableLength;						///< [0x6E] and Mode=0x06: Event table length

	// ID ISC.PRH200 LED Status
	// [0x6E] and Mode=0x07
	unsigned char	ucNoOfLEDs;									///< [0x6E] and Mode=0x07: Number of following LED status bytes
	unsigned char	ucLEDStatus[29];							///< [0x6E] and Mode=0x07: LED status bytes for ID ISC.PRH200


	// ID ISC.LR2500-B Antenna Tuning Status
	// [0x6E] and Mode=0x08
	unsigned short int	usRes1;						///< [0x6E] and Mode=0x08: 
	unsigned short int	usVoltage;					///< [0x6E] and Mode=0x08: 
	unsigned short int	usRes2;						///< [0x6E] and Mode=0x08: 
	unsigned short int	usCurrent;					///< [0x6E] and Mode=0x08: 
	unsigned short int	usPhase;					///< [0x6E] and Mode=0x08: 
	short int			iPhase;						///< [0x6E] and Mode=0x08: 
	unsigned char		ucRes1;						///< [0x6E] and Mode=0x08: 
	unsigned char		ucRes2;						///< [0x6E] and Mode=0x08: 
	unsigned char		ucRes3;						///< [0x6E] and Mode=0x08: 

	// Firmware Status
	// [0x6E] and Mode=0x20
	unsigned char	ucFirmwareStatus[32];						///< [0x6E] and Mode=0x20: Firmware status

	// Config-Para Change
	// [0x6E] and Mode=0x21
	unsigned char	ucConfigParaChange;							///< [0x6E] and Mode=0x21: Status of Reader's configuration after last Firmware Update


	bool bIsDiagnosticRead;	///< indicates, if structure contains valid data
	bool bIsMode0x01Read;	///< indicates, if data with Mode 0x01 is read
	bool bIsMode0x02Read;	///< indicates, if data with Mode 0x02 is read
	bool bIsMode0x03Read;	///< indicates, if data with Mode 0x03 is read
	bool bIsMode0x04Read;	///< indicates, if data with Mode 0x04 is read
	bool bIsMode0x05Read;	///< indicates, if data with Mode 0x05 is read
	bool bIsMode0x06Read;	///< indicates, if data with Mode 0x06 is read
	bool bIsMode0x07Read;	///< indicates, if data with Mode 0x07 is read
	bool bIsMode0x08Read;	///< indicates, if data with Mode 0x08 is read
	bool bIsMode0x20Read;	///< indicates, if data with Mode 0x20 is read
	bool bIsMode0x21Read;	///< indicates, if data with Mode 0x21 is read
	
	// Reader information, only when Diagnostic Data sent with notification protocol
	union
	{
		unsigned char	ucReaderType;			///< Reader Type (if Bit 7 = 0)
		unsigned char	ucReaderClass;			///< Reader Class (if Bit 7 = 1)
	};
	unsigned char	ucReaderClassGen;			///< generation of Reader Class
	unsigned char	ucReaderTypeOfClass;		///< Reader Type of Class, which is defined in ucReaderClass
	unsigned char	ucReserved3;				///< rfu

	/// initialization
	void Init()
	{
		memset(&ucFlagsA, 0, sizeof(_FEDM_ISC_READER_DIAGNOSTIC));
	}


	/// Request of Reader Type Number
	/// All Reader Type Numbers are listed in FEDM_ISC.h
	unsigned int GetReaderType()
	{
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

} FEDM_ISC_READER_DIAGNOSTIC;


#endif // !defined(_FEDM_ISC_READER_DIAGNOSTIC_)
