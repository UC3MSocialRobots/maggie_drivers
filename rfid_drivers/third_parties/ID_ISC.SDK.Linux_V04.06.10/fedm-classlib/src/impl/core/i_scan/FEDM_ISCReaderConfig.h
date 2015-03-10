/*-------------------------------------------------------
|
|		FEDM_ISCReaderConfig.h
|
---------------------------------------------------------

Copyright © 2000-2014	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : info@feig.de
						Internet : http://www.feig.de
					
Version       		:	04.06.08 / 20.10.2014

Operation Systems	:	independent
						This file is auto-generated

Function			:	This file contains all configuration namespaces


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#if !defined(_FEDM_READER_CONFIG_PARA_H_INCLUDED_)
#define _FEDM_READER_CONFIG_PARA_H_INCLUDED_

#include <string>
#include "../FEDM.h"

using namespace std;

namespace CoreImpl
{
	namespace ReaderConfig
	{
		/// Configuration parameters for security and access protection
		namespace AccessProtection
		{
			_FEDM_ISC_CORE_EXT_CLASS extern const string Password;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG0;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG1;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG2;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG3;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG4;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG5;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG6;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG7;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG8;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG9;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG10;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG11;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG12;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG13;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG14;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG15;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG16;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG17;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG18;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG19;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG20;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG21;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG36;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG37;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG38;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG39;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG48;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG63;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG22_29;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG30_35;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG30_39;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG40_49;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG50_59;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG60_62;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Lock_CFG64_86;
			_FEDM_ISC_CORE_EXT_CLASS extern const string CryptoMode;
		}

		/// Configuration parameters for communication interfaces
		namespace HostInterface
		{
			_FEDM_ISC_CORE_EXT_CLASS extern const string Interfaces;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_RS232;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_RS4xx;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_LAN;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_WLAN;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_USB;
			_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_Discovery;

			/// %Serial Port specific parameters
			namespace Serial
			{
				_FEDM_ISC_CORE_EXT_CLASS extern const string BusAddress;
				_FEDM_ISC_CORE_EXT_CLASS extern const string Baudrate;
				_FEDM_ISC_CORE_EXT_CLASS extern const string Frame;
				_FEDM_ISC_CORE_EXT_CLASS extern const string Parity;
				_FEDM_ISC_CORE_EXT_CLASS extern const string Databits;
				_FEDM_ISC_CORE_EXT_CLASS extern const string Stopbits;
				_FEDM_ISC_CORE_EXT_CLASS extern const string ProtocolStartSynchronizationTime;
				_FEDM_ISC_CORE_EXT_CLASS extern const string FlashloaderBaudrate;

				/// RS485 or RS422 specific parameters
				namespace RS4xx
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_TerminationResistors;
				}
			}

			/// %USB specific parameters
			namespace USB
			{
				_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_UniformDeviceID;
			}

			/// %LAN specific parameters
			namespace LAN
			{
				_FEDM_ISC_CORE_EXT_CLASS extern const string PortNumber;

				/// 
				namespace Hostname
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Length;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Name;
				}

				/// Keep-Alive specific parameters (important when network gets broken)
				namespace Keepalive
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable;
					_FEDM_ISC_CORE_EXT_CLASS extern const string RetransmissionCount;
					_FEDM_ISC_CORE_EXT_CLASS extern const string IdleTime;
					_FEDM_ISC_CORE_EXT_CLASS extern const string IntervalTime;
				}

				/// Connection parameters for Localhost (embedded Application)
				namespace LocalHost
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string PortNumber;
				}

				/// %IPv4 specific connection parameters
				namespace IPv4
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_IPv4;
					_FEDM_ISC_CORE_EXT_CLASS extern const string IPAddress;
					_FEDM_ISC_CORE_EXT_CLASS extern const string PortNumber;
					_FEDM_ISC_CORE_EXT_CLASS extern const string SubnetMask;
					_FEDM_ISC_CORE_EXT_CLASS extern const string GatewayAddress;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_DHCP;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Speed;
					_FEDM_ISC_CORE_EXT_CLASS extern const string TCP_UserTimeout;
				}

				/// %IPv6 specific connection parameters
				namespace IPv6
				{
				}

				/// 
				namespace Autonegotiation
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Disable;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Duplex;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Speed;
				}
			}

			/// %WLAN specific parameters
			namespace WLAN
			{
				_FEDM_ISC_CORE_EXT_CLASS extern const string PortNumber;
				_FEDM_ISC_CORE_EXT_CLASS extern const string NetworkType;
				_FEDM_ISC_CORE_EXT_CLASS extern const string AdHocChannel;

				/// 
				namespace Hostname
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Length;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Name;
				}

				/// Keep-Alive specific parameters (important when network gets broken)
				namespace Keepalive
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable;
					_FEDM_ISC_CORE_EXT_CLASS extern const string RetransmissionCount;
					_FEDM_ISC_CORE_EXT_CLASS extern const string IdleTime;
					_FEDM_ISC_CORE_EXT_CLASS extern const string IntervalTime;
				}

				/// 
				namespace AccessPoint
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Channel;
					_FEDM_ISC_CORE_EXT_CLASS extern const string SecurityMode;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_HTTPServer;
				}

				/// Connection parameters for secured transmission
				namespace Security
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string AuthenticationType;
					_FEDM_ISC_CORE_EXT_CLASS extern const string EncryptionType;

					/// SSID parameters
					namespace ServiceSetIdentifier
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string Enable;
						_FEDM_ISC_CORE_EXT_CLASS extern const string Length;
						_FEDM_ISC_CORE_EXT_CLASS extern const string SSID;
					}

					/// %WEP Key
					namespace WEP
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string KeyLength;
						_FEDM_ISC_CORE_EXT_CLASS extern const string Key;
					}

					/// %WPA Key
					namespace WPA
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string KeyLength;
						_FEDM_ISC_CORE_EXT_CLASS extern const string Key;
					}

					/// %WPA2 Key
					namespace WPA2
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string KeyLength;
						_FEDM_ISC_CORE_EXT_CLASS extern const string Key;
					}
				}

				/// %IPv4 specific connection parameters
				namespace IPv4
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_IPv4;
					_FEDM_ISC_CORE_EXT_CLASS extern const string IPAddress;
					_FEDM_ISC_CORE_EXT_CLASS extern const string PortNumber;
					_FEDM_ISC_CORE_EXT_CLASS extern const string SubnetMask;
					_FEDM_ISC_CORE_EXT_CLASS extern const string GatewayAddress;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_DHCP;
					_FEDM_ISC_CORE_EXT_CLASS extern const string TCP_UserTimeout;
				}

				/// %IPv6 specific connection parameters
				namespace IPv6
				{
				}
			}

			/// %Bluetooth specific parameters
			namespace Bluetooth
			{
				_FEDM_ISC_CORE_EXT_CLASS extern const string NameSuffix;
				_FEDM_ISC_CORE_EXT_CLASS extern const string Key;
				_FEDM_ISC_CORE_EXT_CLASS extern const string PowerOnTime;
			}

			/// Data-Clock Port specific parameters
			namespace DataClock
			{
				_FEDM_ISC_CORE_EXT_CLASS extern const string Format;
			}

			/// 
			namespace MultiDropBus
			{
				_FEDM_ISC_CORE_EXT_CLASS extern const string DeviceAddress;
				_FEDM_ISC_CORE_EXT_CLASS extern const string TerminalID;
			}

			/// 
			namespace IrDA
			{

				/// 
				namespace AccessPassword
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Password;
				}
			}

			/// %Miscellaneous configuration parameters
			namespace Miscellaneous
			{
				_FEDM_ISC_CORE_EXT_CLASS extern const string ProtocolSelection;
			}
		}

		/// Configuration parameters for configuring the different working modes
		namespace OperatingMode
		{
			_FEDM_ISC_CORE_EXT_CLASS extern const string Mode;
			_FEDM_ISC_CORE_EXT_CLASS extern const string EMVco;
			_FEDM_ISC_CORE_EXT_CLASS extern const string EMV;

			/// Configuration parameters for adjusting the Host-Mode (polling mode)
			namespace HostMode
			{
				_FEDM_ISC_CORE_EXT_CLASS extern const string InventoryAbortCondition;

				/// %Filter parameters
				namespace Filter
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string ResponseMode;
				}
			}

			/// Configuration parameters for adjusting the Buffered-Read-Mode (auto-read mode)
			namespace BufferedReadMode
			{

				/// Parameters for selecting data items
				namespace DataSelector
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Selector;
					_FEDM_ISC_CORE_EXT_CLASS extern const string UID;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Data;
					_FEDM_ISC_CORE_EXT_CLASS extern const string EAS;
					_FEDM_ISC_CORE_EXT_CLASS extern const string AntennaNo;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Time;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Date;
					_FEDM_ISC_CORE_EXT_CLASS extern const string InputEvents;
					_FEDM_ISC_CORE_EXT_CLASS extern const string AFI;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Direction;
					_FEDM_ISC_CORE_EXT_CLASS extern const string RSSI;

					/// Parameters for adjusting the selected data items
					namespace Mode
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_AntennaPool;
						_FEDM_ISC_CORE_EXT_CLASS extern const string ReadCompleteBank;
						_FEDM_ISC_CORE_EXT_CLASS extern const string SeekFunction;
						_FEDM_ISC_CORE_EXT_CLASS extern const string ChangeAFI;
						_FEDM_ISC_CORE_EXT_CLASS extern const string ActionOnEPC;
						_FEDM_ISC_CORE_EXT_CLASS extern const string ChangeEAS;
					}
				}

				/// Parameters for defining the sources of Transponder data
				namespace DataSource
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string BankNo;
					_FEDM_ISC_CORE_EXT_CLASS extern const string FirstDataBlock;
					_FEDM_ISC_CORE_EXT_CLASS extern const string NoOfDataBlocks;
					_FEDM_ISC_CORE_EXT_CLASS extern const string ByteOrderOfData;
				}

				/// Parameters for filtering event data
				namespace Filter
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string TransponderValidTime;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_Input1Event;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_Input2Event;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_Input3Event;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_Input4Event;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_Input5Event;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_TriggerEvent;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_TimeoutEvent;
				}

				/// Parameters for defining trigger conditions
				namespace Trigger
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Condition;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_UnlimitTransponderValidTime;

					/// Parameters for defining the trigger source
					namespace Source
					{

						/// Parameters for trigger source of type %Input
						namespace Input
						{

							/// 1st Input
							namespace No1
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string TriggerUse;
								_FEDM_ISC_CORE_EXT_CLASS extern const string HoldTime;
							}

							/// 2nd Input
							namespace No2
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string TriggerUse;
								_FEDM_ISC_CORE_EXT_CLASS extern const string HoldTime;
							}

							/// 3rd Input
							namespace No3
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string TriggerUse;
								_FEDM_ISC_CORE_EXT_CLASS extern const string HoldTime;
							}

							/// 4th Input
							namespace No4
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string TriggerUse;
								_FEDM_ISC_CORE_EXT_CLASS extern const string HoldTime;
							}

							/// 5th Input
							namespace No5
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string TriggerUse;
								_FEDM_ISC_CORE_EXT_CLASS extern const string HoldTime;
							}
						}
					}
				}

				/// Parameters for %EAS (Electronic Article Surveillance)
				namespace EAS
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string AlarmMode;
					_FEDM_ISC_CORE_EXT_CLASS extern const string TagType;
				}

				/// 
				namespace SeekFunction
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string MaskStartAddress;
					_FEDM_ISC_CORE_EXT_CLASS extern const string MaskLength;
					_FEDM_ISC_CORE_EXT_CLASS extern const string NewAFI;
					_FEDM_ISC_CORE_EXT_CLASS extern const string NewEAS;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_Password;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Password;

					/// 
					namespace Mask
					{
					}
				}

				/// %Miscellaneous configuration parameters
				namespace Miscellaneous
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string NoRfReset;
				}
			}

			/// Configuration parameters for adjusting the Notification-Mode (auto-read mode)
			namespace NotificationMode
			{
				_FEDM_ISC_CORE_EXT_CLASS extern const string InputMessage;

				/// Parameters for selecting data items
				namespace DataSelector
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Selector;
					_FEDM_ISC_CORE_EXT_CLASS extern const string UID;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Data;
					_FEDM_ISC_CORE_EXT_CLASS extern const string EAS;
					_FEDM_ISC_CORE_EXT_CLASS extern const string AntennaNo;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Time;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Date;
					_FEDM_ISC_CORE_EXT_CLASS extern const string InputEvents;
					_FEDM_ISC_CORE_EXT_CLASS extern const string MacAddress;
					_FEDM_ISC_CORE_EXT_CLASS extern const string AFI;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Direction;
					_FEDM_ISC_CORE_EXT_CLASS extern const string RSSI;

					/// Parameters for adjusting the selected data items
					namespace Mode
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_AntennaPool;
						_FEDM_ISC_CORE_EXT_CLASS extern const string ReadCompleteBank;
						_FEDM_ISC_CORE_EXT_CLASS extern const string SeekFunction;
						_FEDM_ISC_CORE_EXT_CLASS extern const string ChangeAFI;
						_FEDM_ISC_CORE_EXT_CLASS extern const string ActionOnEPC;
						_FEDM_ISC_CORE_EXT_CLASS extern const string ChangeEAS;
					}
				}

				/// Parameters for defining the sources of Transponder data
				namespace DataSource
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string BankNo;
					_FEDM_ISC_CORE_EXT_CLASS extern const string FirstDataBlock;
					_FEDM_ISC_CORE_EXT_CLASS extern const string NoOfDataBlocks;
					_FEDM_ISC_CORE_EXT_CLASS extern const string ByteOrderOfData;
					_FEDM_ISC_CORE_EXT_CLASS extern const string FirstByte;
					_FEDM_ISC_CORE_EXT_CLASS extern const string NoOfBytes;

					/// 
					namespace Miscellaneous
					{

						/// 
						namespace ISO_14443
						{

							/// 
							namespace Part4
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string ByteOrderOfData;
							}
						}
					}

					/// Access parameters for %Mifare Transponders
					namespace Mifare
					{

						/// Access parameters for Mifare %Classic Transponders
						namespace Classic
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string AppID;
							_FEDM_ISC_CORE_EXT_CLASS extern const string KeyAddress;
							_FEDM_ISC_CORE_EXT_CLASS extern const string KeyType;
						}

						/// Access parameters for Mifare %Ultralight Transponders
						namespace Ultralight
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_Authentication;
							_FEDM_ISC_CORE_EXT_CLASS extern const string KeyAddress;
						}

						/// Access parameters for Mifare %DESFire Transponders
						namespace DESFire
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_Authentication;
							_FEDM_ISC_CORE_EXT_CLASS extern const string AppID;
							_FEDM_ISC_CORE_EXT_CLASS extern const string KeyNumber;
							_FEDM_ISC_CORE_EXT_CLASS extern const string ReaderKeyIndex;
							_FEDM_ISC_CORE_EXT_CLASS extern const string FileType;
							_FEDM_ISC_CORE_EXT_CLASS extern const string FileNumber;
							_FEDM_ISC_CORE_EXT_CLASS extern const string FileCommSettings;
							_FEDM_ISC_CORE_EXT_CLASS extern const string FileOffset;
							_FEDM_ISC_CORE_EXT_CLASS extern const string FileLength;
						}

						/// 
						namespace Plus
						{

							/// 
							namespace SecurityLevel1
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_Authentication;
								_FEDM_ISC_CORE_EXT_CLASS extern const string ReaderKeyIndex;
							}

							/// 
							namespace SecurityLevel3
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_Authentication;
								_FEDM_ISC_CORE_EXT_CLASS extern const string KeyType;
								_FEDM_ISC_CORE_EXT_CLASS extern const string ReaderKeyIndex;
								_FEDM_ISC_CORE_EXT_CLASS extern const string ReadCommand;
								_FEDM_ISC_CORE_EXT_CLASS extern const string BlockNumber;
								_FEDM_ISC_CORE_EXT_CLASS extern const string Ext;
								_FEDM_ISC_CORE_EXT_CLASS extern const string AppID;
								_FEDM_ISC_CORE_EXT_CLASS extern const string FirstByte;
								_FEDM_ISC_CORE_EXT_CLASS extern const string NoOfBytes;
							}
						}
					}
				}

				/// Parameters for filtering event data
				namespace Filter
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string TransponderValidTime;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_Input1Event;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_Input2Event;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_Input3Event;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_Input4Event;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_Input5Event;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_TriggerEvent;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_TimeoutEvent;
				}

				/// Parameters for defining trigger conditions
				namespace Trigger
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Condition;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_UnlimitTransponderValidTime;

					/// Parameters for defining the trigger source
					namespace Source
					{

						/// Parameters for trigger source of type %Input
						namespace Input
						{

							/// 1st Input
							namespace No1
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string TriggerUse;
								_FEDM_ISC_CORE_EXT_CLASS extern const string HoldTime;
							}

							/// 2nd Input
							namespace No2
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string TriggerUse;
								_FEDM_ISC_CORE_EXT_CLASS extern const string HoldTime;
							}

							/// 3rd Input
							namespace No3
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string TriggerUse;
								_FEDM_ISC_CORE_EXT_CLASS extern const string HoldTime;
							}

							/// 4th Input
							namespace No4
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string TriggerUse;
								_FEDM_ISC_CORE_EXT_CLASS extern const string HoldTime;
							}

							/// 5th Input
							namespace No5
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string TriggerUse;
								_FEDM_ISC_CORE_EXT_CLASS extern const string HoldTime;
							}
						}
					}
				}

				/// Parameters for %EAS (Electronic Article Surveillance)
				namespace EAS
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string AlarmMode;
					_FEDM_ISC_CORE_EXT_CLASS extern const string TagType;
				}

				/// Parameters for defining transmission parameters
				namespace Transmission
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string NotifyTrigger;
					_FEDM_ISC_CORE_EXT_CLASS extern const string TimeTriggeredTime;
					_FEDM_ISC_CORE_EXT_CLASS extern const string DataSetsLimit;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_Acknowledge;

					/// Parameters for defining the transmission destination
					namespace Destination
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string PortNumber;
						_FEDM_ISC_CORE_EXT_CLASS extern const string ConnectionHoldTime;

						/// %IPv4 specific connection parameters
						namespace IPv4
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string IPAddress;
							_FEDM_ISC_CORE_EXT_CLASS extern const string TCP_UserTimeout;
						}

						/// %IPv6 specific connection parameters
						namespace IPv6
						{
						}
					}

					/// Keep-Alive specific parameters (important when network gets broken)
					namespace KeepAlive
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string Enable;
						_FEDM_ISC_CORE_EXT_CLASS extern const string IntervalTime;
					}
				}

				/// 
				namespace SeekFunction
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string MaskStartAddress;
					_FEDM_ISC_CORE_EXT_CLASS extern const string MaskLength;
					_FEDM_ISC_CORE_EXT_CLASS extern const string NewAFI;
					_FEDM_ISC_CORE_EXT_CLASS extern const string NewEAS;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_Password;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Password;

					/// 
					namespace Mask
					{
					}
				}

				/// Parameters for defining event parameters of Gate-People-Counter
				namespace GatePeopleCounter
				{

					/// Parameters for defining transmission parameters
					namespace Transmission
					{

						/// Parameters for defining the transmission destination
						namespace Destination
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string PortNumber;

							/// %IPv4 specific connection parameters
							namespace IPv4
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string IPAddress;
							}
						}
					}
				}
			}

			/// Configuration parameters for adjusting the Scan-Mode (auto-read mode)
			namespace ScanMode
			{
				_FEDM_ISC_CORE_EXT_CLASS extern const string Interface;

				/// Parameters for selecting data items
				namespace DataSelector
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Selector;
					_FEDM_ISC_CORE_EXT_CLASS extern const string UID;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Data;
					_FEDM_ISC_CORE_EXT_CLASS extern const string EAS;
					_FEDM_ISC_CORE_EXT_CLASS extern const string AntennaNo;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Time;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Date;
					_FEDM_ISC_CORE_EXT_CLASS extern const string InputEvents;
					_FEDM_ISC_CORE_EXT_CLASS extern const string MifareAppID;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Direction;
					_FEDM_ISC_CORE_EXT_CLASS extern const string RSSI;

					/// Parameters for adjusting the selected data items
					namespace Mode
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_AntennaPool;
						_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_IndependentTransmission;
						_FEDM_ISC_CORE_EXT_CLASS extern const string ReadCompleteBank;
					}
				}

				/// Parameters for defining the sources of Transponder data
				namespace DataSource
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string BankNo;
					_FEDM_ISC_CORE_EXT_CLASS extern const string FirstDataBlock;
					_FEDM_ISC_CORE_EXT_CLASS extern const string NoOfDataBlocks;
					_FEDM_ISC_CORE_EXT_CLASS extern const string ByteOrderOfData;
					_FEDM_ISC_CORE_EXT_CLASS extern const string FirstByte;
					_FEDM_ISC_CORE_EXT_CLASS extern const string NoOfBytes;

					/// Access parameters for %Mifare Transponders
					namespace Mifare
					{

						/// Access parameters for Mifare %Classic Transponders
						namespace Classic
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string AppID;
							_FEDM_ISC_CORE_EXT_CLASS extern const string KeyAddress;
							_FEDM_ISC_CORE_EXT_CLASS extern const string KeyType;
						}

						/// Access parameters for Mifare %DESFire Transponders
						namespace DESFire
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string AppID;
							_FEDM_ISC_CORE_EXT_CLASS extern const string KeyNumber;
							_FEDM_ISC_CORE_EXT_CLASS extern const string ReaderKeyIndex;
							_FEDM_ISC_CORE_EXT_CLASS extern const string FileType;
							_FEDM_ISC_CORE_EXT_CLASS extern const string FileNumber;
							_FEDM_ISC_CORE_EXT_CLASS extern const string FileCommSettings;
							_FEDM_ISC_CORE_EXT_CLASS extern const string FileOffset;
							_FEDM_ISC_CORE_EXT_CLASS extern const string FileLength;
						}
					}
				}

				/// Parameters for formatting the data output
				namespace DataFormat
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string BusAddressPrefix;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Format;
					_FEDM_ISC_CORE_EXT_CLASS extern const string NoOfUserHeaderChars;
					_FEDM_ISC_CORE_EXT_CLASS extern const string UserHeaderChar1;
					_FEDM_ISC_CORE_EXT_CLASS extern const string UserHeaderChar2;
					_FEDM_ISC_CORE_EXT_CLASS extern const string UserHeaderChar3;
					_FEDM_ISC_CORE_EXT_CLASS extern const string UserHeaderChar4;
					_FEDM_ISC_CORE_EXT_CLASS extern const string SeparationChar;
					_FEDM_ISC_CORE_EXT_CLASS extern const string UserSeparationChar;
					_FEDM_ISC_CORE_EXT_CLASS extern const string EndChar;
					_FEDM_ISC_CORE_EXT_CLASS extern const string NoOfUserEndChars;
					_FEDM_ISC_CORE_EXT_CLASS extern const string UserEndChar;
					_FEDM_ISC_CORE_EXT_CLASS extern const string UserEndChar1;
					_FEDM_ISC_CORE_EXT_CLASS extern const string UserEndChar2;
					_FEDM_ISC_CORE_EXT_CLASS extern const string UserEndChar3;
					_FEDM_ISC_CORE_EXT_CLASS extern const string UidInBcd;
				}

				/// Parameters for filtering event data
				namespace Filter
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string ResponseMode;
					_FEDM_ISC_CORE_EXT_CLASS extern const string TransponderValidTime;
				}

				/// Parameters for defining trigger conditions
				namespace Trigger
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Mode;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Condition;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_UnlimitTransponderValidTime;

					/// Parameters for defining the trigger source
					namespace Source
					{

						/// Parameters for trigger source of type %Input
						namespace Input
						{

							/// 1st Input
							namespace No1
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string TriggerUse;
								_FEDM_ISC_CORE_EXT_CLASS extern const string HoldTime;
							}

							/// 2nd Input
							namespace No2
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string TriggerUse;
								_FEDM_ISC_CORE_EXT_CLASS extern const string HoldTime;
							}

							/// 3rd Input
							namespace No3
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string TriggerUse;
								_FEDM_ISC_CORE_EXT_CLASS extern const string HoldTime;
							}

							/// 4th Input
							namespace No4
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string TriggerUse;
								_FEDM_ISC_CORE_EXT_CLASS extern const string HoldTime;
							}

							/// 5th Input
							namespace No5
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string TriggerUse;
								_FEDM_ISC_CORE_EXT_CLASS extern const string HoldTime;
							}
						}
					}
				}

				/// Parameters for %EAS (Electronic Article Surveillance)
				namespace EAS
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string AlarmMode;
					_FEDM_ISC_CORE_EXT_CLASS extern const string TagType;
				}
			}

			/// Configuration parameters for adjusting the Access-Mode (auto-read mode)
			namespace AccessMode
			{
				_FEDM_ISC_CORE_EXT_CLASS extern const string Mode;

				/// 
				namespace AccessControl
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string OnlineRequest;
					_FEDM_ISC_CORE_EXT_CLASS extern const string RequestTimeout;
				}

				/// Parameters for selecting data items
				namespace DataSelector
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string UID;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Data;
				}

				/// Parameters for defining the sources of Transponder data
				namespace DataSource
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string BankNo;
					_FEDM_ISC_CORE_EXT_CLASS extern const string FirstDataBlock;
					_FEDM_ISC_CORE_EXT_CLASS extern const string NoOfDataBlocks;
					_FEDM_ISC_CORE_EXT_CLASS extern const string ByteOrderOfData;
					_FEDM_ISC_CORE_EXT_CLASS extern const string FirstByte;
					_FEDM_ISC_CORE_EXT_CLASS extern const string NoOfBytes;

					/// 
					namespace Miscellaneous
					{

						/// 
						namespace ISO_14443
						{

							/// 
							namespace Part4
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string ByteOrderOfData;
							}
						}
					}

					/// 
					namespace Mifare
					{

						/// 
						namespace Classic
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string AppID;
							_FEDM_ISC_CORE_EXT_CLASS extern const string KeyAddress;
							_FEDM_ISC_CORE_EXT_CLASS extern const string KeyType;
						}

						/// 
						namespace Ultralight
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_Authentication;
							_FEDM_ISC_CORE_EXT_CLASS extern const string KeyAddress;
						}

						/// Access parameters for Mifare %DESFire Transponders
						namespace DESFire
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_Authentication;
							_FEDM_ISC_CORE_EXT_CLASS extern const string AppID;
							_FEDM_ISC_CORE_EXT_CLASS extern const string KeyNumber;
							_FEDM_ISC_CORE_EXT_CLASS extern const string ReaderKeyIndex;
							_FEDM_ISC_CORE_EXT_CLASS extern const string FileType;
							_FEDM_ISC_CORE_EXT_CLASS extern const string FileNumber;
							_FEDM_ISC_CORE_EXT_CLASS extern const string FileCommSettings;
							_FEDM_ISC_CORE_EXT_CLASS extern const string FileOffset;
							_FEDM_ISC_CORE_EXT_CLASS extern const string FileLength;
						}

						/// 
						namespace Plus
						{

							/// 
							namespace SecurityLevel1
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_Authentication;
								_FEDM_ISC_CORE_EXT_CLASS extern const string ReaderKeyIndex;
							}

							/// 
							namespace SecurityLevel3
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_Authentication;
								_FEDM_ISC_CORE_EXT_CLASS extern const string KeyType;
								_FEDM_ISC_CORE_EXT_CLASS extern const string ReaderKeyIndex;
								_FEDM_ISC_CORE_EXT_CLASS extern const string ReadCommand;
								_FEDM_ISC_CORE_EXT_CLASS extern const string BlockNumber;
								_FEDM_ISC_CORE_EXT_CLASS extern const string Ext;
								_FEDM_ISC_CORE_EXT_CLASS extern const string AppID;
								_FEDM_ISC_CORE_EXT_CLASS extern const string FirstByte;
								_FEDM_ISC_CORE_EXT_CLASS extern const string NoOfBytes;
							}
						}
					}
				}

				/// Parameters for filtering event data
				namespace Filter
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string TransponderValidTime;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_EventNotification;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_AccessEvent;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_InputEvent;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_AlarmEvent;
				}

				/// Parameters for defining trigger conditions
				namespace Trigger
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Condition;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_UnlimitTransponderValidTime;

					/// Parameters for defining the trigger source
					namespace Source
					{

						/// 
						namespace Input
						{

							/// 
							namespace No1
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string TriggerUse;
								_FEDM_ISC_CORE_EXT_CLASS extern const string HoldTime;
							}
						}
					}
				}

				/// Parameters for defining transmission parameters
				namespace Transmission
				{

					/// Parameters for defining the transmission destination
					namespace Destination
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string PortNumber;
						_FEDM_ISC_CORE_EXT_CLASS extern const string ConnectionHoldTime;

						/// %IPv4 specific connection parameters
						namespace IPv4
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string IPAddress;
						}
					}

					/// Keep-Alive specific parameters (important when network gets broken)
					namespace KeepAlive
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string Enable;
						_FEDM_ISC_CORE_EXT_CLASS extern const string IntervalTime;
					}

					/// Parameters for formatting the data output
					namespace DataLayout
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string IDD;
						_FEDM_ISC_CORE_EXT_CLASS extern const string TimeStamp;
						_FEDM_ISC_CORE_EXT_CLASS extern const string Status;
						_FEDM_ISC_CORE_EXT_CLASS extern const string Input;
						_FEDM_ISC_CORE_EXT_CLASS extern const string AntennaNo;
					}
				}
			}

			/// %Miscellaneous configuration parameters
			namespace Miscellaneous
			{

				/// %HF specific parameters
				namespace HF
				{

					/// 
					namespace ReaderSynchronization
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_Synchronization;
						_FEDM_ISC_CORE_EXT_CLASS extern const string Is_Master;
						_FEDM_ISC_CORE_EXT_CLASS extern const string SwitchCondition;
						_FEDM_ISC_CORE_EXT_CLASS extern const string RfOffCondition;
						_FEDM_ISC_CORE_EXT_CLASS extern const string SynchronizationTimeout;
					}
				}

				/// 
				namespace TransponderIdentification
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Source;
					_FEDM_ISC_CORE_EXT_CLASS extern const string DataBlockNo;
					_FEDM_ISC_CORE_EXT_CLASS extern const string NoOfDataBlocks;
				}

				/// 
				namespace Logging
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Enable;
				}
			}

			/// 
			namespace Payment
			{
				_FEDM_ISC_CORE_EXT_CLASS extern const string Interface;

				/// 
				namespace girogo
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Functions;
					_FEDM_ISC_CORE_EXT_CLASS extern const string CreditDisplay;
					_FEDM_ISC_CORE_EXT_CLASS extern const string PaymentAck;
					_FEDM_ISC_CORE_EXT_CLASS extern const string ConfirmPayment;
					_FEDM_ISC_CORE_EXT_CLASS extern const string MerchantReferenceNo;

					/// 
					namespace AutomaticAccountReconciliation
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string TransactionLimit;
					}

					/// 
					namespace SmartCard
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string Enable;
					}
				}
			}
		}

		/// Configuration parameters for adjusting the RF-Interface
		namespace AirInterface
		{
			_FEDM_ISC_CORE_EXT_CLASS extern const string TimeLimit;

			/// Configuration parameters for adjusting the %Antennas
			namespace Antenna
			{

				/// %HF specific parameters
				namespace HF
				{

					/// Antenna output 1
					namespace No1
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string OutputPower;
						_FEDM_ISC_CORE_EXT_CLASS extern const string PowerOnTime;

						/// 
						namespace Filter
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string SensitivityLevel;
						}
					}

					/// %Miscellaneous configuration parameters
					namespace Miscellaneous
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_PowerUpTuning;
						_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_DCPower;
						_FEDM_ISC_CORE_EXT_CLASS extern const string MinimumLevelMode;
						_FEDM_ISC_CORE_EXT_CLASS extern const string MinimumLevel;
						_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_TagDetect;
					}
				}

				/// %UHF specific parameters
				namespace UHF
				{

					/// Antenna output 1
					namespace No1
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string OutputPower;
					}

					/// Antenna output 2
					namespace No2
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string OutputPower;
					}

					/// Antenna output 3
					namespace No3
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string OutputPower;
					}

					/// Antenna output 4
					namespace No4
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string OutputPower;
					}

					/// %Miscellaneous configuration parameters
					namespace Miscellaneous
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string Gain;
						_FEDM_ISC_CORE_EXT_CLASS extern const string Type;
						_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_DCPower;
					}
				}
			}

			/// %Regional settings
			namespace Region
			{

				/// %UHF specific parameters
				namespace UHF
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Regulation;

					/// Settings for European Union regulation
					namespace EU
					{

						/// %Channel parameters
						namespace Channel
						{

							/// Settings for EN302208 with LBT
							namespace EN302208_with_LBT
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_865MHz1;
								_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_865MHz3;
								_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_865MHz5;
								_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_865MHz7;
								_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_865MHz9;
								_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_866MHz1;
								_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_866MHz3;
								_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_866MHz5;
								_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_866MHz7;
								_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_866MHz9;
								_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_867MHz1;
								_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_867MHz3;
								_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_867MHz5;
								_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_867MHz7;
								_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_867MHz9;
							}

							/// Settings for EN302208 4-Channel-Plan
							namespace EN302208_4_ChannelPlan
							{

								/// Preferred channels for EN302208 4-Channel-Plan
								namespace PreferredChannels
								{
									_FEDM_ISC_CORE_EXT_CLASS extern const string NoOfChannels;
									_FEDM_ISC_CORE_EXT_CLASS extern const string ChannelNo1;
									_FEDM_ISC_CORE_EXT_CLASS extern const string ChannelNo2;
									_FEDM_ISC_CORE_EXT_CLASS extern const string ChannelNo3;
									_FEDM_ISC_CORE_EXT_CLASS extern const string ChannelNo4;
								}
							}

							/// Settings for %EN300220
							namespace EN300220
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string Disable_869MHz_FallbackChannel;
							}
						}
					}

					/// Settings for %FCC regulation
					namespace FCC
					{

						/// %Channel parameters
						namespace Channel
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string ChannelAllocationMode;
							_FEDM_ISC_CORE_EXT_CLASS extern const string UpperChannel;
							_FEDM_ISC_CORE_EXT_CLASS extern const string LowerChannel;
						}
					}

					/// Settings for %Korea regulation
					namespace Korea
					{

						/// %Channel parameters
						namespace Channel
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string LowerChannel;
							_FEDM_ISC_CORE_EXT_CLASS extern const string UpperChannel;
							_FEDM_ISC_CORE_EXT_CLASS extern const string ChannelAllocationMode;
						}
					}
				}
			}

			/// Configuration parameters for internal or external %Multiplexer
			namespace Multiplexer
			{
				_FEDM_ISC_CORE_EXT_CLASS extern const string Enable;

				/// %HF specific settings
				namespace HF
				{

					/// %External Multiplexer
					namespace External
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string SwitchCondition;
						_FEDM_ISC_CORE_EXT_CLASS extern const string InputChannelMode;
						_FEDM_ISC_CORE_EXT_CLASS extern const string NoOfOutputChannels;

						/// Settings for %Antenna Outputs
						namespace Antenna
						{

							/// Output 1
							namespace No1
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string ActiveTime;
							}

							/// Output 2
							namespace No2
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string ActiveTime;
							}

							/// Output 3
							namespace No3
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string ActiveTime;
							}

							/// Output 4
							namespace No4
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string ActiveTime;
							}

							/// Output 5
							namespace No5
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string ActiveTime;
							}

							/// Output 6
							namespace No6
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string ActiveTime;
							}

							/// Output 7
							namespace No7
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string ActiveTime;
							}

							/// Output 8
							namespace No8
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string ActiveTime;
							}
						}
					}
				}

				/// %UHF specific settings
				namespace UHF
				{

					/// %Internal Multiplexer
					namespace Internal
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string SwitchCondition;
						_FEDM_ISC_CORE_EXT_CLASS extern const string AntennaSelectionMode;
						_FEDM_ISC_CORE_EXT_CLASS extern const string NoOfAntennas;
						_FEDM_ISC_CORE_EXT_CLASS extern const string SelectedAntennas;

						/// Settings for %Antenna Outputs
						namespace Antenna
						{

							/// Output 1
							namespace No1
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string ActiveTime;
							}

							/// Output 2
							namespace No2
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string ActiveTime;
							}

							/// Output 3
							namespace No3
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string ActiveTime;
							}

							/// Output 4
							namespace No4
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string ActiveTime;
							}
						}
					}

					/// %External Multiplexer
					namespace External
					{

						/// Settings for Antenna %Outputs
						namespace Output
						{

							/// Output 1
							namespace No1
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string SelectedAntennas;
							}

							/// Output 2
							namespace No2
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string SelectedAntennas;
							}

							/// Output 3
							namespace No3
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string SelectedAntennas;
							}

							/// Output 4
							namespace No4
							{
								_FEDM_ISC_CORE_EXT_CLASS extern const string SelectedAntennas;
							}
						}
					}
				}
			}

			/// %Miscellaneous configuration parameters
			namespace Miscellaneous
			{

				/// %HF specific parameters
				namespace HF
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Modulation;
					_FEDM_ISC_CORE_EXT_CLASS extern const string ReceiverChannel;
					_FEDM_ISC_CORE_EXT_CLASS extern const string ReceiverSensitivity;

					/// Settings for ISO 15693 and/or ICode1
					namespace ISO_15693_ICode1
					{

						/// Settings for error handling
						namespace ErrorHandling
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string WeakCollision;
						}
					}

					/// Settings for ICode UID and ICode EPC
					namespace ICodeUID_ICodeEPC
					{

						/// Settings for error handling
						namespace ErrorHandling
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string WeakCollision;
						}
					}

					/// Settings for ISO 15693
					namespace ISO_15693
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string Modulation;
						_FEDM_ISC_CORE_EXT_CLASS extern const string SubCarrier;
					}

					/// Settings for error handling
					namespace ISO_18000_3M3
					{

						/// Settings for ISO 18000-3M3
						namespace ErrorHandling
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string WeakCollision;
						}
					}

					/// 
					namespace ASK_Modulation
					{

						/// Settings for error handling
						namespace ErrorHandling
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string WeakCollision;
						}
					}

					/// 
					namespace NoiseLevel
					{
					}
				}
			}
		}

		/// Configuration parameters for %Transponder drivers
		namespace Transponder
		{

			/// Transponder %Driver
			namespace Driver
			{
				_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_HF;
				_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_UHF;

				/// %HF Drivers (in fixed execution order)
				namespace HF
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Selector;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Drivers;
					_FEDM_ISC_CORE_EXT_CLASS extern const string ICode1;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Tagit_HF;
					_FEDM_ISC_CORE_EXT_CLASS extern const string ISO_15693;
					_FEDM_ISC_CORE_EXT_CLASS extern const string ISO_14443_TypeA;
					_FEDM_ISC_CORE_EXT_CLASS extern const string ISO_14443_TypeB;
					_FEDM_ISC_CORE_EXT_CLASS extern const string ICodeEPC;
					_FEDM_ISC_CORE_EXT_CLASS extern const string ICodeUID;
					_FEDM_ISC_CORE_EXT_CLASS extern const string EPC_Class1Gen2;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Jewel;
					_FEDM_ISC_CORE_EXT_CLASS extern const string SR176;
					_FEDM_ISC_CORE_EXT_CLASS extern const string SRIxx;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Microchip_MCRF45x;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Keyboard;
					_FEDM_ISC_CORE_EXT_CLASS extern const string ISO_18000_3M3;

					/// Drivers, in execution order defined by user
					namespace UserSequenced
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string No1;
						_FEDM_ISC_CORE_EXT_CLASS extern const string No2;
						_FEDM_ISC_CORE_EXT_CLASS extern const string No3;
						_FEDM_ISC_CORE_EXT_CLASS extern const string No4;
						_FEDM_ISC_CORE_EXT_CLASS extern const string No5;
						_FEDM_ISC_CORE_EXT_CLASS extern const string No6;
						_FEDM_ISC_CORE_EXT_CLASS extern const string No7;
						_FEDM_ISC_CORE_EXT_CLASS extern const string No8;
						_FEDM_ISC_CORE_EXT_CLASS extern const string No9;
						_FEDM_ISC_CORE_EXT_CLASS extern const string No10;
						_FEDM_ISC_CORE_EXT_CLASS extern const string No11;
						_FEDM_ISC_CORE_EXT_CLASS extern const string No12;
						_FEDM_ISC_CORE_EXT_CLASS extern const string No13;
						_FEDM_ISC_CORE_EXT_CLASS extern const string No14;
					}

					/// Sub-Drivers for ISO 14443
					namespace ISO_14443
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string Drivers;

						/// Sub-Drivers for ISO 14443, Part A
						namespace TypeA
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string MifareStandard;
							_FEDM_ISC_CORE_EXT_CLASS extern const string my_d_Proximity_SLE55Rxx;
							_FEDM_ISC_CORE_EXT_CLASS extern const string MifareUltraLight;
							_FEDM_ISC_CORE_EXT_CLASS extern const string Part4;
						}
					}
				}

				/// %UHF Driver
				namespace UHF
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Drivers;
					_FEDM_ISC_CORE_EXT_CLASS extern const string ISO_18000_6_B;
					_FEDM_ISC_CORE_EXT_CLASS extern const string EM4222_EM4444;
					_FEDM_ISC_CORE_EXT_CLASS extern const string EPC_Class1Gen1;
					_FEDM_ISC_CORE_EXT_CLASS extern const string EPC_Class1Gen2;
				}
			}

			/// Anticollission settings
			namespace Anticollision
			{
				_FEDM_ISC_CORE_EXT_CLASS extern const string Enable;
			}

			/// Persistence Reset settings
			namespace PersistenceReset
			{
				_FEDM_ISC_CORE_EXT_CLASS extern const string Mode;

				/// Settings for %Antenna outputs
				namespace Antenna
				{

					/// Output 1
					namespace No1
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string PersistenceResetTime;
					}

					/// Output 2
					namespace No2
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string PersistenceResetTime;
					}

					/// Output 3
					namespace No3
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string PersistenceResetTime;
					}

					/// Output 4
					namespace No4
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string PersistenceResetTime;
					}
				}
			}

			/// Configuration parameters for %HF Transponders
			namespace HF
			{

				/// I-Code1 specific settings
				namespace ICode1
				{

					/// %Anticollision settings
					namespace Anticollision
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string MaximalTimeslots;
						_FEDM_ISC_CORE_EXT_CLASS extern const string MinimalTimeslots;
						_FEDM_ISC_CORE_EXT_CLASS extern const string Timeslots;
					}

					/// Link settings
					namespace LinkRate
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string FastMode;
					}

					/// Mask settings
					namespace SelectionMask
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string FamilyCode;
						_FEDM_ISC_CORE_EXT_CLASS extern const string ApplicationIdentifier;
					}

					/// %Miscellaneous configuration parameters
					namespace Miscellaneous
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string MemoryMapping;
						_FEDM_ISC_CORE_EXT_CLASS extern const string EASLevel;
					}
				}

				/// ISO 15693 specific settings
				namespace ISO_15693
				{

					/// %Anticollision settings
					namespace Anticollision
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string NoOfTimeslots;
					}

					/// Link settings
					namespace LinkRate
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string DataCoding;
						_FEDM_ISC_CORE_EXT_CLASS extern const string DataRate;
					}

					/// Mask settings
					namespace SelectionMask
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_AFI;
						_FEDM_ISC_CORE_EXT_CLASS extern const string AFI1;
						_FEDM_ISC_CORE_EXT_CLASS extern const string AFI2;
						_FEDM_ISC_CORE_EXT_CLASS extern const string AFI3;
						_FEDM_ISC_CORE_EXT_CLASS extern const string AFI4;
					}

					/// %Miscellaneous configuration parameters
					namespace Miscellaneous
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string WriteOption;
						_FEDM_ISC_CORE_EXT_CLASS extern const string CommandBreak;

						/// Settings for reading Transponder data
						namespace ReadOption
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string ReadMode;
							_FEDM_ISC_CORE_EXT_CLASS extern const string BlockSizeSelection;
							_FEDM_ISC_CORE_EXT_CLASS extern const string BlockSize;
						}
					}
				}

				/// ICode UID and ICode EPC specific settings
				namespace ICodeUID_ICodeEPC
				{

					/// %Anticollision settings
					namespace Anticollision
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string MinimalTimeslots;
						_FEDM_ISC_CORE_EXT_CLASS extern const string MaximalTimeslots;
					}

					/// Mask settings
					namespace SelectionMask
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string MaskLength;
						_FEDM_ISC_CORE_EXT_CLASS extern const string Mask;
					}
				}

				/// >EPC Class1 Gen2 specific settings
				namespace EPC_Class1Gen2
				{

					/// %Anticollision settings
					namespace Anticollision
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string Session;
					}
				}

				/// ISO 14443 specific settings
				namespace ISO_14443
				{

					/// Link settings
					namespace Bitrate
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string Transmit;
						_FEDM_ISC_CORE_EXT_CLASS extern const string Receive;
					}

					/// ISO 14443, Part 4 specific settings
					namespace Part4
					{

						/// %Miscellaneous configuration parameters
						namespace Miscellaneous
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string PowerLevelCheck;
							_FEDM_ISC_CORE_EXT_CLASS extern const string ErrorRetry;
						}
					}

					/// ISO 14443, Type A specific settings
					namespace TypeA
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string OptionalInventoryInfo;
						_FEDM_ISC_CORE_EXT_CLASS extern const string UID_Order;
					}

					/// ISO 14443, Type B specific settings
					namespace TypeB
					{

						/// Mask settings
						namespace SelectionMask
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string AFI;
						}

						/// %Miscellaneous configuration parameters
						namespace Miscellaneous
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string BitrateSelect;
						}
					}

					/// %Miscellaneous configuration parameters
					namespace Miscellaneous
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string StartupTime;
					}
				}

				/// ISO 18000-3M3 specific settings
				namespace ISO_18000_3M3
				{

					/// Mask settings
					namespace SelectionMask
					{

						/// 1st Mask
						namespace No1
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string Bank;
							_FEDM_ISC_CORE_EXT_CLASS extern const string MaskLength;
							_FEDM_ISC_CORE_EXT_CLASS extern const string FirstBit;
							_FEDM_ISC_CORE_EXT_CLASS extern const string Mask;
						}
					}

					/// 
					namespace Anticollision
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string Session;
					}
				}

				/// Configuration parameters for Transponder-specific Custom-Commands
				namespace CustomerCommandOptions
				{

					/// EM Microelectronic-Marin SA
					namespace EM_Microelectronic
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string StorageQuiet;
					}

					/// %Fujitsu Limited
					namespace Fujitsu
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string FastInventory;
						_FEDM_ISC_CORE_EXT_CLASS extern const string FastCommands;
					}

					/// %Infineon Technologies AG
					namespace Infineon
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string ReadWrite_Commands;
					}

					/// %NXP Semiconductors
					namespace NXP
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string Disable;
						_FEDM_ISC_CORE_EXT_CLASS extern const string FastReturnLink;
						_FEDM_ISC_CORE_EXT_CLASS extern const string InventoryReadPage;
						_FEDM_ISC_CORE_EXT_CLASS extern const string InventoryReadBlock;
						_FEDM_ISC_CORE_EXT_CLASS extern const string LongRangeCommand;
					}

					/// %STMicroelectronics SA
					namespace STMicroelectronics
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string InitiateInventory;
						_FEDM_ISC_CORE_EXT_CLASS extern const string FastCommands;
					}

					/// Texas Instruments
					namespace Texas_Instruments
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string InventoryReadBlock;
						_FEDM_ISC_CORE_EXT_CLASS extern const string FastSID;
					}
				}
			}

			/// Configuration parameters for %UHF Transponders
			namespace UHF
			{

				/// ISO 18000-6B specific settings
				namespace ISO_18000_6_B
				{

					/// Link settings
					namespace LinkRate
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string ReturnLinkBitrate;
						_FEDM_ISC_CORE_EXT_CLASS extern const string ForwardLinkModulation;
						_FEDM_ISC_CORE_EXT_CLASS extern const string ForwardLinkBitrate;
					}

					/// Mask settings
					namespace SelectionMask
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string MaskLength;
						_FEDM_ISC_CORE_EXT_CLASS extern const string FirstByte;
						_FEDM_ISC_CORE_EXT_CLASS extern const string Operation;
						_FEDM_ISC_CORE_EXT_CLASS extern const string Mask;
					}
				}

				/// EPC Class1 Gen1 specific settings
				namespace EPC_Class1Gen1
				{

					/// Link settings
					namespace LinkRate
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string ReturnLinkBitrate;
						_FEDM_ISC_CORE_EXT_CLASS extern const string UidLength;
					}

					/// Mask settings
					namespace SelectionMask
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string MaskLength;
						_FEDM_ISC_CORE_EXT_CLASS extern const string FirstBit;
						_FEDM_ISC_CORE_EXT_CLASS extern const string Mask;
					}
				}

				/// EPC Class1 Gen2 specific settings
				namespace EPC_Class1Gen2
				{

					/// %Anticollision settings
					namespace Anticollision
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string AnticollisionStartValue;
						_FEDM_ISC_CORE_EXT_CLASS extern const string Session;
					}

					/// Link settings
					namespace LinkRate
					{
					}

					/// Mask settings
					namespace SelectionMask
					{

						/// 1st Mask
						namespace No1
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string BankNo;
							_FEDM_ISC_CORE_EXT_CLASS extern const string MaskLength;
							_FEDM_ISC_CORE_EXT_CLASS extern const string FirstBit;
							_FEDM_ISC_CORE_EXT_CLASS extern const string Mask;
							_FEDM_ISC_CORE_EXT_CLASS extern const string Negation;
							_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_EPCTruncation;
						}

						/// 2nd Mask
						namespace No2
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string BankNo;
							_FEDM_ISC_CORE_EXT_CLASS extern const string MaskLength;
							_FEDM_ISC_CORE_EXT_CLASS extern const string FirstBit;
							_FEDM_ISC_CORE_EXT_CLASS extern const string Mask;
							_FEDM_ISC_CORE_EXT_CLASS extern const string Negation;
							_FEDM_ISC_CORE_EXT_CLASS extern const string Logic;
							_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_EPCTruncation;
						}

						/// 3rd Mask
						namespace No3
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string BankNo;
							_FEDM_ISC_CORE_EXT_CLASS extern const string MaskLength;
							_FEDM_ISC_CORE_EXT_CLASS extern const string FirstBit;
							_FEDM_ISC_CORE_EXT_CLASS extern const string Mask;
							_FEDM_ISC_CORE_EXT_CLASS extern const string Negation;
							_FEDM_ISC_CORE_EXT_CLASS extern const string Logic;
							_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_EPCTruncation;
						}
					}

					/// %Miscellaneous configuration parameters
					namespace Miscellaneous
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string WriteOption;
					}
				}
			}

			/// %Miscellaneous configuration parameters
			namespace Miscellaneous
			{
				_FEDM_ISC_CORE_EXT_CLASS extern const string IdentifierInterpretationMode;
				_FEDM_ISC_CORE_EXT_CLASS extern const string TIDLength;
			}
		}

		/// Configuration parameters for digital inputs and outputs, Relays and LEDs
		namespace DigitalIO
		{

			/// Settings for digital inputs
			namespace Input
			{

				/// No 1
				namespace No1
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Mode;
				}

				/// No 2
				namespace No2
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Mode;
				}

				/// No 3
				namespace No3
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Mode;
				}

				/// No 4
				namespace No4
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Mode;
				}

				/// No 5
				namespace No5
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string Mode;
				}
			}

			/// Settings for digital outputs
			namespace Output
			{
				_FEDM_ISC_CORE_EXT_CLASS extern const string OfflineDelay;
				_FEDM_ISC_CORE_EXT_CLASS extern const string TagDetectActiveTime;

				/// No 1
				namespace No1
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string IdleMode;
					_FEDM_ISC_CORE_EXT_CLASS extern const string IdleFlashMode;
					_FEDM_ISC_CORE_EXT_CLASS extern const string SettlingTime;
					_FEDM_ISC_CORE_EXT_CLASS extern const string OnlineState;
					_FEDM_ISC_CORE_EXT_CLASS extern const string OfflineState;
					_FEDM_ISC_CORE_EXT_CLASS extern const string TagDetectState;
					_FEDM_ISC_CORE_EXT_CLASS extern const string ActiveState;

					/// Event link
					namespace ReadEventActivation
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string AntennaNo;
					}

					/// Settings for Access-Mode
					namespace AccessMode
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string AccessActiveTime;
					}
				}

				/// No 2
				namespace No2
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string IdleMode;
					_FEDM_ISC_CORE_EXT_CLASS extern const string IdleFlashMode;
					_FEDM_ISC_CORE_EXT_CLASS extern const string SettlingTime;
					_FEDM_ISC_CORE_EXT_CLASS extern const string OnlineState;
					_FEDM_ISC_CORE_EXT_CLASS extern const string OfflineState;
					_FEDM_ISC_CORE_EXT_CLASS extern const string TagDetectState;
					_FEDM_ISC_CORE_EXT_CLASS extern const string ActiveState;

					/// Event link
					namespace ReadEventActivation
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string AntennaNo;
					}

					/// 
					namespace AccessMode
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string AccessActiveTime;
					}
				}

				/// No 3
				namespace No3
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string OnlineState;
					_FEDM_ISC_CORE_EXT_CLASS extern const string OfflineState;
					_FEDM_ISC_CORE_EXT_CLASS extern const string TagDetectState;
				}
			}

			/// Settings for %Relays
			namespace Relay
			{

				/// No 1
				namespace No1
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string IdleMode;
					_FEDM_ISC_CORE_EXT_CLASS extern const string OfflineState;
					_FEDM_ISC_CORE_EXT_CLASS extern const string OnlineState;
					_FEDM_ISC_CORE_EXT_CLASS extern const string TagDetectState;
					_FEDM_ISC_CORE_EXT_CLASS extern const string IdleFlashMode;
					_FEDM_ISC_CORE_EXT_CLASS extern const string SettlingTime;
					_FEDM_ISC_CORE_EXT_CLASS extern const string ActiveState;

					/// Event link
					namespace ReadEventActivation
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string AntennaNo;
					}

					/// Settings for Access-Mode
					namespace AccessMode
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string AccessActiveTime;
					}
				}

				/// No 2
				namespace No2
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string IdleMode;
					_FEDM_ISC_CORE_EXT_CLASS extern const string IdleFlashMode;
					_FEDM_ISC_CORE_EXT_CLASS extern const string SettlingTime;
					_FEDM_ISC_CORE_EXT_CLASS extern const string ActiveState;

					/// Event link
					namespace ReadEventActivation
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string AntennaNo;
					}
				}

				/// No 3
				namespace No3
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string IdleMode;
					_FEDM_ISC_CORE_EXT_CLASS extern const string IdleFlashMode;
					_FEDM_ISC_CORE_EXT_CLASS extern const string SettlingTime;
					_FEDM_ISC_CORE_EXT_CLASS extern const string ActiveState;

					/// Event link
					namespace ReadEventActivation
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string AntennaNo;
					}
				}

				/// No 4
				namespace No4
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string IdleMode;
					_FEDM_ISC_CORE_EXT_CLASS extern const string IdleFlashMode;
					_FEDM_ISC_CORE_EXT_CLASS extern const string SettlingTime;

					/// Event link
					namespace ReadEventActivation
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string AntennaNo;
					}
				}
			}

			/// Settings for %Signalers/Buzzers
			namespace Signaler
			{
				_FEDM_ISC_CORE_EXT_CLASS extern const string OfflineDelay;
				_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_StartupSignal;
				_FEDM_ISC_CORE_EXT_CLASS extern const string TagDetectActiveTime;
				_FEDM_ISC_CORE_EXT_CLASS extern const string AlarmTimeout;

				/// Automatic activation by sources
				namespace EventSource
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string OfflineDetect;
					_FEDM_ISC_CORE_EXT_CLASS extern const string TagDetect;
					_FEDM_ISC_CORE_EXT_CLASS extern const string StartupSignal;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Input1;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Input2;
					_FEDM_ISC_CORE_EXT_CLASS extern const string Input3;
				}

				/// Settings for %Buzzers
				namespace Buzzer
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string IdleState;
					_FEDM_ISC_CORE_EXT_CLASS extern const string ActiveState;
					_FEDM_ISC_CORE_EXT_CLASS extern const string OfflineState;
					_FEDM_ISC_CORE_EXT_CLASS extern const string OnlineState;
					_FEDM_ISC_CORE_EXT_CLASS extern const string TagDetectState;
					_FEDM_ISC_CORE_EXT_CLASS extern const string IdleFlashFrequency;
					_FEDM_ISC_CORE_EXT_CLASS extern const string ActiveFlashFrequency;
					_FEDM_ISC_CORE_EXT_CLASS extern const string InputFlashFrequency;
					_FEDM_ISC_CORE_EXT_CLASS extern const string OfflineFlashFrequency;
					_FEDM_ISC_CORE_EXT_CLASS extern const string ActivationTime;
					_FEDM_ISC_CORE_EXT_CLASS extern const string ActivationSources;

					/// Settings for Access-Mode
					namespace AccessMode
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string AccessState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string AccessDeniedState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string AccessActiveTime;
						_FEDM_ISC_CORE_EXT_CLASS extern const string AccessDeniedActiveTime;
						_FEDM_ISC_CORE_EXT_CLASS extern const string AlarmState;
					}
				}

				/// Settings for %LEDs
				namespace LED
				{

					/// %Blue LED
					namespace Blue
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string IdleState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string ActiveState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string OfflineState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string OnlineState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string TagDetectState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string IdleFlashFrequency;
						_FEDM_ISC_CORE_EXT_CLASS extern const string ActivationTime;
						_FEDM_ISC_CORE_EXT_CLASS extern const string ActiveFlashFrequency;

						/// Settings for Access-Mode
						namespace AccessMode
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string AccessState;
							_FEDM_ISC_CORE_EXT_CLASS extern const string AccessDeniedState;
							_FEDM_ISC_CORE_EXT_CLASS extern const string AccessActiveTime;
							_FEDM_ISC_CORE_EXT_CLASS extern const string AccessDeniedActiveTime;
							_FEDM_ISC_CORE_EXT_CLASS extern const string AlarmState;
						}
					}

					/// %Green LED
					namespace Green
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string IdleState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string ActiveState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string OfflineState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string OnlineState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string TagDetectState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string IdleFlashFrequency;
						_FEDM_ISC_CORE_EXT_CLASS extern const string ActiveFlashFrequency;
						_FEDM_ISC_CORE_EXT_CLASS extern const string InputFlashFrequency;
						_FEDM_ISC_CORE_EXT_CLASS extern const string OfflineFlashFrequency;
						_FEDM_ISC_CORE_EXT_CLASS extern const string ActivationTime;

						/// Settings for Access-Mode
						namespace AccessMode
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string AccessState;
							_FEDM_ISC_CORE_EXT_CLASS extern const string AccessDeniedState;
							_FEDM_ISC_CORE_EXT_CLASS extern const string AccessActiveTime;
							_FEDM_ISC_CORE_EXT_CLASS extern const string AccessDeniedActiveTime;
							_FEDM_ISC_CORE_EXT_CLASS extern const string AlarmState;
						}
					}

					/// %Yellow LED
					namespace Yellow
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string OfflineState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string OnlineState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string TagDetectState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string InputEventActivation;
						_FEDM_ISC_CORE_EXT_CLASS extern const string OutputEventActivation;
					}

					/// %Red LED
					namespace Red
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string IdleState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string ActiveState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string OfflineState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string OnlineState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string TagDetectState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string IdleFlashFrequency;
						_FEDM_ISC_CORE_EXT_CLASS extern const string ActiveFlashFrequency;
						_FEDM_ISC_CORE_EXT_CLASS extern const string InputFlashFrequency;
						_FEDM_ISC_CORE_EXT_CLASS extern const string OfflineFlashFrequency;
						_FEDM_ISC_CORE_EXT_CLASS extern const string ActivationTime;

						/// Settings for Access-Mode
						namespace AccessMode
						{
							_FEDM_ISC_CORE_EXT_CLASS extern const string AccessState;
							_FEDM_ISC_CORE_EXT_CLASS extern const string AccessDeniedState;
							_FEDM_ISC_CORE_EXT_CLASS extern const string AccessActiveTime;
							_FEDM_ISC_CORE_EXT_CLASS extern const string AccessDeniedActiveTime;
							_FEDM_ISC_CORE_EXT_CLASS extern const string AlarmState;
						}
					}

					/// LED No 1
					namespace No1
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string OnlineState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string OfflineState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string TagDetectState;
					}

					/// LED No 2
					namespace No2
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string OnlineState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string OfflineState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string TagDetectState;
					}

					/// LED No 3
					namespace No3
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string OnlineState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string OfflineState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string TagDetectState;
					}

					/// LED No 4
					namespace No4
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string OnlineState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string OfflineState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string TagDetectState;
					}

					/// LED No 5
					namespace No5
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string OnlineState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string OfflineState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string TagDetectState;
					}

					/// LED No 6
					namespace No6
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string OnlineState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string OfflineState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string TagDetectState;
					}

					/// LED No 7
					namespace No7
					{
						_FEDM_ISC_CORE_EXT_CLASS extern const string OnlineState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string OfflineState;
						_FEDM_ISC_CORE_EXT_CLASS extern const string TagDetectState;
					}
				}
			}

			/// %Miscellaneous configuration parameters
			namespace Miscellaneous
			{
				_FEDM_ISC_CORE_EXT_CLASS extern const string AFIAssignment;
				_FEDM_ISC_CORE_EXT_CLASS extern const string Enable_CPRIO;
			}
		}

		/// Configuration parameters for adjusting the power consumption
		namespace PowerManagement
		{
			_FEDM_ISC_CORE_EXT_CLASS extern const string Mode;
			_FEDM_ISC_CORE_EXT_CLASS extern const string WakeUpPulse;
			_FEDM_ISC_CORE_EXT_CLASS extern const string PowerOnTime;
			_FEDM_ISC_CORE_EXT_CLASS extern const string RFOnMode;
			_FEDM_ISC_CORE_EXT_CLASS extern const string RFOnTime_Min;
			_FEDM_ISC_CORE_EXT_CLASS extern const string RFOnTime_Max;
		}

		/// Configuration parameters for internal clock or timer
		namespace Clock
		{

			/// Settings for Network Time Protocol
			namespace NetworkTimeProtocol
			{
				_FEDM_ISC_CORE_EXT_CLASS extern const string Mode;
				_FEDM_ISC_CORE_EXT_CLASS extern const string TriggerTime;

				/// %IPv4 specific settings
				namespace IPv4
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string IPAddress;
				}

				/// %IPv6 specific settings
				namespace IPv6
				{
				}
			}
		}

		/// Configuration parameters for Peripheral Devices, like People-Counter
		namespace PeripheralDevice
		{
			_FEDM_ISC_CORE_EXT_CLASS extern const string Mode;
			_FEDM_ISC_CORE_EXT_CLASS extern const string DetectMode;
			_FEDM_ISC_CORE_EXT_CLASS extern const string DirectionMode;

			/// Settings for digital inputs and outputs
			namespace DigitalIO
			{

				/// Settings for digital outputs
				namespace Output
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string SettlingTime;
				}
			}

			/// %RS485-Bus devices
			namespace RS485
			{

				/// Device with bus address 1
				namespace BusAdr1
				{

					/// Settings for digital inputs and outputs
					namespace DigitalIO
					{

						/// Settings for digital outputs
						namespace Output
						{

							/// N0 1
							namespace No1
							{

								/// Event link
								namespace ReadEventActivation
								{
									_FEDM_ISC_CORE_EXT_CLASS extern const string AntennaNo;
								}
							}

							/// No 2
							namespace No2
							{

								/// Event link
								namespace ReadEventActivation
								{
									_FEDM_ISC_CORE_EXT_CLASS extern const string AntennaNo;
								}
							}

							/// No 3
							namespace No3
							{

								/// Event link
								namespace ReadEventActivation
								{
									_FEDM_ISC_CORE_EXT_CLASS extern const string AntennaNo;
								}
							}
						}
					}
				}

				/// Device with bus address 2
				namespace BusAdr2
				{

					/// Settings for digital inputs and outputs
					namespace DigitalIO
					{

						/// Settings for digital outputs
						namespace Output
						{

							/// No 1
							namespace No1
							{

								/// Event link
								namespace ReadEventActivation
								{
									_FEDM_ISC_CORE_EXT_CLASS extern const string AntennaNo;
								}
							}

							/// No 2
							namespace No2
							{

								/// Event link
								namespace ReadEventActivation
								{
									_FEDM_ISC_CORE_EXT_CLASS extern const string AntennaNo;
								}
							}

							/// No 3
							namespace No3
							{

								/// Event link
								namespace ReadEventActivation
								{
									_FEDM_ISC_CORE_EXT_CLASS extern const string AntennaNo;
								}
							}
						}
					}
				}

				/// Device with bus address 3
				namespace BusAdr3
				{

					/// Settings for digital inputs and outputs
					namespace DigitalIO
					{

						/// Settings for digital outputs
						namespace Output
						{

							/// No 1
							namespace No1
							{

								/// Event link
								namespace ReadEventActivation
								{
									_FEDM_ISC_CORE_EXT_CLASS extern const string AntennaNo;
								}
							}

							/// No 2
							namespace No2
							{

								/// Event link
								namespace ReadEventActivation
								{
									_FEDM_ISC_CORE_EXT_CLASS extern const string AntennaNo;
								}
							}

							/// No 3
							namespace No3
							{

								/// Event link
								namespace ReadEventActivation
								{
									_FEDM_ISC_CORE_EXT_CLASS extern const string AntennaNo;
								}
							}
						}
					}
				}
			}

			/// 
			namespace GatePeopleCounter
			{

				/// 
				namespace Filter
				{
					_FEDM_ISC_CORE_EXT_CLASS extern const string TimeLimit_BeforeTagDetection;
					_FEDM_ISC_CORE_EXT_CLASS extern const string TimeLimit_AfterTagDetection;
				}
			}
		}

		/// Additional tools
		namespace SystemTools
		{

			/// %Linux specific tools
			namespace Linux
			{
				_FEDM_ISC_CORE_EXT_CLASS extern const string Telnet;
				_FEDM_ISC_CORE_EXT_CLASS extern const string ssh;
				_FEDM_ISC_CORE_EXT_CLASS extern const string WebServer;
				_FEDM_ISC_CORE_EXT_CLASS extern const string FTPServer;
			}
		}

	}
}

#endif // _FEDM_READER_CONFIG_PARA_H_INCLUDED_
