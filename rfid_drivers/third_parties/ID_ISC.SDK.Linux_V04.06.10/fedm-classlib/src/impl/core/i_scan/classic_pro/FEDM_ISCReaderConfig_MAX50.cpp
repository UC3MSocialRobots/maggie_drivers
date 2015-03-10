/*-------------------------------------------------------
|
|               FEDM_ISCReaderConfig_MAX50.cpp
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

Function			:	implementation of Access-ID map for OBID® myAXXESS reader family


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#include "../FEDM_ISCReader.h"


void FEDM_ISCReader::BuildAccessIdMap_MAX50()
{
	m_mapAccessID.clear();

	FEDM_ISCReader::BuildAccessIdMap_MAX50_Part0();
	FEDM_ISCReader::BuildAccessIdMap_MAX50_Part1();
}


void FEDM_ISCReader::BuildAccessIdMap_MAX50_Part0()
{
	m_mapAccessID.insert(make_pair((string)"AccessProtection.CryptoMode", (string)"03 03 00 13 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.TimeLimit", (string)"03 03 01 06 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"Clock.NetworkTimeProtocol.IPv4.IPAddress", (string)"03 03 47 03 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"Clock.NetworkTimeProtocol.Mode", (string)"03 03 47 00 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Clock.NetworkTimeProtocol.TriggerTime", (string)"03 03 47 02 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Input.No1.Mode", (string)"03 03 02 04 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Input.No2.Mode", (string)"03 03 02 04 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Miscellaneous.Enable_CPRIO", (string)"03 03 02 13 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Relay.No1.AccessMode.AccessActiveTime", (string)"03 03 14 10 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Relay.No1.OfflineState", (string)"03 03 02 07 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Relay.No1.OnlineState", (string)"03 03 02 02 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Relay.No1.TagDetectState", (string)"03 03 02 10 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.AlarmTimeout", (string)"03 03 14 05 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.Buzzer.AccessMode.AccessActiveTime", (string)"03 03 14 09 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.Buzzer.AccessMode.AccessDeniedActiveTime", (string)"03 03 14 13 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.Buzzer.AccessMode.AccessDeniedState", (string)"03 03 14 11 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.Buzzer.AccessMode.AccessState", (string)"03 03 14 07 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.Buzzer.AccessMode.AlarmState", (string)"03 03 14 06 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.Buzzer.OfflineState", (string)"03 03 02 08 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.Buzzer.OnlineState", (string)"03 03 02 03 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.Buzzer.TagDetectState", (string)"03 03 02 11 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Blue.AccessMode.AccessActiveTime", (string)"03 03 14 08 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Blue.AccessMode.AccessDeniedActiveTime", (string)"03 03 14 12 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Blue.AccessMode.AccessDeniedState", (string)"03 03 14 11 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Blue.AccessMode.AccessState", (string)"03 03 14 07 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Blue.AccessMode.AlarmState", (string)"03 03 14 06 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Blue.OfflineState", (string)"03 03 02 08 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Blue.OnlineState", (string)"03 03 02 03 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Blue.TagDetectState", (string)"03 03 02 11 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Green.AccessMode.AccessActiveTime", (string)"03 03 14 08 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Green.AccessMode.AccessDeniedActiveTime", (string)"03 03 14 12 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Green.AccessMode.AccessDeniedState", (string)"03 03 14 11 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Green.AccessMode.AccessState", (string)"03 03 14 07 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Green.AccessMode.AlarmState", (string)"03 03 14 06 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Green.OfflineState", (string)"03 03 02 08 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Green.OnlineState", (string)"03 03 02 03 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Green.TagDetectState", (string)"03 03 02 11 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Red.AccessMode.AccessActiveTime", (string)"03 03 14 08 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Red.AccessMode.AccessDeniedActiveTime", (string)"03 03 14 12 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Red.AccessMode.AccessDeniedState", (string)"03 03 14 11 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Red.AccessMode.AccessState", (string)"03 03 14 07 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Red.AccessMode.AlarmState", (string)"03 03 14 06 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Red.OfflineState", (string)"03 03 02 08 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Red.OnlineState", (string)"03 03 02 03 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Red.TagDetectState", (string)"03 03 02 11 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.OfflineDelay", (string)"03 03 02 09 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.TagDetectActiveTime", (string)"03 03 02 12 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Interfaces", (string)"03 03 01 12 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.IPv4.Enable_DHCP", (string)"03 03 41 04 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.IPv4.GatewayAddress", (string)"03 03 41 06 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.IPv4.IPAddress", (string)"03 03 40 00 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.IPv4.Speed", (string)"03 03 41 04 01 06 01"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.IPv4.SubnetMask", (string)"03 03 41 00 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.Keepalive.Enable", (string)"03 03 41 04 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.Keepalive.IntervalTime", (string)"03 03 41 12 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.Keepalive.RetransmissionCount", (string)"03 03 41 05 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.PortNumber", (string)"03 03 40 08 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.AccessControl.OnlineRequest", (string)"03 03 14 01 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.AccessControl.RequestTimeout", (string)"03 03 14 04 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSelector.Data", (string)"03 03 11 00 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSelector.UID", (string)"03 03 11 00 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.ByteOrderOfData", (string)"03 03 11 00 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.FirstByte", (string)"03 03 11 11 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.FirstDataBlock", (string)"03 03 11 04 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.Mifare.Classic.AppID", (string)"03 03 15 02 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.Mifare.Classic.KeyAddress", (string)"03 03 15 00 01 00 04"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.Mifare.Classic.KeyType", (string)"03 03 15 00 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.Mifare.DESFire.AppID", (string)"03 03 10 00 03 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.Mifare.DESFire.Enable_Authentication", (string)"03 03 11 03 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.Mifare.DESFire.FileCommSettings", (string)"03 03 10 06 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.Mifare.DESFire.FileLength", (string)"03 03 10 10 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.Mifare.DESFire.FileNumber", (string)"03 03 10 05 01 00 05"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.Mifare.DESFire.FileOffset", (string)"03 03 10 07 03 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.Mifare.DESFire.FileType", (string)"03 03 10 06 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.Mifare.DESFire.KeyNumber", (string)"03 03 10 03 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.Mifare.DESFire.ReaderKeyIndex", (string)"03 03 10 04 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.Mifare.Plus.SecurityLevel1.Enable_Authentication", (string)"03 03 11 03 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.Mifare.Plus.SecurityLevel1.ReaderKeyIndex", (string)"03 03 09 07 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.Mifare.Plus.SecurityLevel3.AppID", (string)"03 03 09 08 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.Mifare.Plus.SecurityLevel3.BlockNumber", (string)"03 03 09 05 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.Mifare.Plus.SecurityLevel3.Enable_Authentication", (string)"03 03 11 03 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.Mifare.Plus.SecurityLevel3.Ext", (string)"03 03 09 06 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.Mifare.Plus.SecurityLevel3.FirstByte", (string)"03 03 09 11 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.Mifare.Plus.SecurityLevel3.KeyType", (string)"03 03 09 01 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.Mifare.Plus.SecurityLevel3.NoOfBytes", (string)"03 03 09 12 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.Mifare.Plus.SecurityLevel3.ReadCommand", (string)"03 03 09 03 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.Mifare.Plus.SecurityLevel3.ReaderKeyIndex", (string)"03 03 09 02 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.Mifare.Ultralight.Enable_Authentication", (string)"03 03 11 03 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.Mifare.Ultralight.KeyAddress", (string)"03 03 15 01 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.Miscellaneous.ISO_14443.Part4.ByteOrderOfData", (string)"03 03 11 10 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.NoOfBytes", (string)"03 03 11 12 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.NoOfDataBlocks", (string)"03 03 11 08 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Filter.Enable_AccessEvent", (string)"03 03 14 02 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Filter.Enable_AlarmEvent", (string)"03 03 14 02 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Filter.Enable_EventNotification", (string)"03 03 14 01 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Filter.Enable_InputEvent", (string)"03 03 02 05 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Filter.TransponderValidTime", (string)"03 03 12 00 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Mode", (string)"03 03 14 00 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Transmission.DataLayout.IDD", (string)"03 03 14 03 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Transmission.DataLayout.Input", (string)"03 03 14 03 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Transmission.DataLayout.Status", (string)"03 03 14 03 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Transmission.DataLayout.TimeStamp", (string)"03 03 14 03 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Transmission.Destination.ConnectionHoldTime", (string)"03 03 49 13 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Transmission.Destination.IPv4.IPAddress", (string)"03 03 49 07 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Transmission.Destination.PortNumber", (string)"03 03 49 11 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Transmission.KeepAlive.Enable", (string)"03 03 49 04 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Transmission.KeepAlive.IntervalTime", (string)"03 03 49 05 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.HostMode.InventoryAbortCondition", (string)"03 03 05 11 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Miscellaneous.TransponderIdentification.DataBlockNo", (string)"03 03 12 03 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Miscellaneous.TransponderIdentification.NoOfDataBlocks", (string)"03 03 12 05 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Miscellaneous.TransponderIdentification.Source", (string)"03 03 12 02 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Mode", (string)"03 03 01 13 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSelector.Data", (string)"03 03 11 00 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSelector.MacAddress", (string)"03 03 11 01 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSelector.UID", (string)"03 03 11 00 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.ByteOrderOfData", (string)"03 03 11 00 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.FirstByte", (string)"03 03 11 11 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.FirstDataBlock", (string)"03 03 11 04 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.Mifare.Classic.AppID", (string)"03 03 15 02 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.Mifare.Classic.KeyAddress", (string)"03 03 15 00 01 00 04"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.Mifare.Classic.KeyType", (string)"03 03 15 00 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.Mifare.DESFire.AppID", (string)"03 03 10 00 03 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.Mifare.DESFire.Enable_Authentication", (string)"03 03 11 03 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.Mifare.DESFire.FileCommSettings", (string)"03 03 10 06 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.Mifare.DESFire.FileLength", (string)"03 03 10 10 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.Mifare.DESFire.FileNumber", (string)"03 03 10 05 01 00 05"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.Mifare.DESFire.FileOffset", (string)"03 03 10 07 03 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.Mifare.DESFire.FileType", (string)"03 03 10 06 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.Mifare.DESFire.KeyNumber", (string)"03 03 10 03 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.Mifare.DESFire.ReaderKeyIndex", (string)"03 03 10 04 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.Mifare.Plus.SecurityLevel1.Enable_Authentication", (string)"03 03 11 03 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.Mifare.Plus.SecurityLevel1.ReaderKeyIndex", (string)"03 03 09 07 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.Mifare.Plus.SecurityLevel3.AppID", (string)"03 03 09 08 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.Mifare.Plus.SecurityLevel3.BlockNumber", (string)"03 03 09 05 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.Mifare.Plus.SecurityLevel3.Enable_Authentication", (string)"03 03 11 03 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.Mifare.Plus.SecurityLevel3.Ext", (string)"03 03 09 06 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.Mifare.Plus.SecurityLevel3.FirstByte", (string)"03 03 09 11 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.Mifare.Plus.SecurityLevel3.KeyType", (string)"03 03 09 01 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.Mifare.Plus.SecurityLevel3.NoOfBytes", (string)"03 03 09 12 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.Mifare.Plus.SecurityLevel3.ReadCommand", (string)"03 03 09 03 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.Mifare.Plus.SecurityLevel3.ReaderKeyIndex", (string)"03 03 09 02 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.Mifare.Ultralight.Enable_Authentication", (string)"03 03 11 03 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.Mifare.Ultralight.KeyAddress", (string)"03 03 15 01 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.Miscellaneous.ISO_14443.Part4.ByteOrderOfData", (string)"03 03 11 10 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.NoOfBytes", (string)"03 03 11 12 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.NoOfDataBlocks", (string)"03 03 11 08 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Filter.TransponderValidTime", (string)"03 03 12 00 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.InputMessage", (string)"03 03 02 05 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Transmission.Destination.ConnectionHoldTime", (string)"03 03 49 13 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Transmission.Destination.IPv4.IPAddress", (string)"03 03 49 07 04 00 00"));
}


void FEDM_ISCReader::BuildAccessIdMap_MAX50_Part1()
{
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Transmission.Destination.PortNumber", (string)"03 03 49 11 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Transmission.KeepAlive.Enable", (string)"03 03 49 04 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Transmission.KeepAlive.IntervalTime", (string)"03 03 49 05 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Anticollision.Enable", (string)"03 03 05 11 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.Drivers", (string)"03 03 03 00 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_14443.Drivers", (string)"03 03 03 02 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_14443.TypeA.MifareStandard", (string)"03 03 03 03 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_14443.TypeA.MifareUltraLight", (string)"03 03 03 03 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_14443.TypeA.Part4", (string)"03 03 03 03 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_14443_TypeA", (string)"03 03 03 01 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_14443_TypeB", (string)"03 03 03 01 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_15693", (string)"03 03 03 01 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.Jewel", (string)"03 03 03 00 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.Keyboard", (string)"03 03 03 00 01 06 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.SR176", (string)"03 03 03 00 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.SRIxx", (string)"03 03 03 00 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.Bitrate.Receive", (string)"03 03 03 07 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.Bitrate.Transmit", (string)"03 03 03 07 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.Part4.Miscellaneous.ErrorRetry", (string)"03 03 03 13 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.Part4.Miscellaneous.PowerLevelCheck", (string)"03 03 03 13 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.TypeA.OptionalInventoryInfo", (string)"03 03 03 13 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.TypeA.UID_Order", (string)"03 03 03 13 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.TypeB.Miscellaneous.BitrateSelect", (string)"03 03 03 13 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.TypeB.SelectionMask.AFI", (string)"03 03 04 08 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.Anticollision.NoOfTimeslots", (string)"03 03 04 04 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.Miscellaneous.ReadOption.BlockSize", (string)"03 03 04 13 01 00 05"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.Miscellaneous.ReadOption.BlockSizeSelection", (string)"03 03 04 13 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.Miscellaneous.ReadOption.ReadMode", (string)"03 03 04 13 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.Miscellaneous.WriteOption", (string)"03 03 04 06 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.SelectionMask.AFI1", (string)"03 03 04 05 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.SelectionMask.Enable_AFI", (string)"03 03 04 04 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.PersistenceReset.Antenna.No1.PersistenceResetTime", (string)"03 03 16 02 02 00 00"));
}
