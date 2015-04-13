/*-------------------------------------------------------
|
|               FEDM_ISCReaderConfig_MAXU1002.cpp
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

Function			:	implementation of Access-ID map for OBID® myAXXESS reader ID MAX.U1002


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#include "FEDM_ISCReader.h"


void FEDM_ISCReader::BuildAccessIdMap_MAXU1002()
{
	m_mapAccessID.clear();

	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG0", (string)"03 03 00 08 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG1", (string)"03 03 00 08 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG10", (string)"03 03 00 09 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG11", (string)"03 03 00 09 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG12", (string)"03 03 00 09 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG13", (string)"03 03 00 09 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG14", (string)"03 03 00 09 01 06 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG15", (string)"03 03 00 09 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG16", (string)"03 03 00 10 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG17", (string)"03 03 00 10 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG18", (string)"03 03 00 10 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG19", (string)"03 03 00 10 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG2", (string)"03 03 00 08 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG20", (string)"03 03 00 10 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG21", (string)"03 03 00 10 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG22_29", (string)"03 03 00 10 01 06 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG3", (string)"03 03 00 08 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG4", (string)"03 03 00 08 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG5", (string)"03 03 00 08 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG6", (string)"03 03 00 08 01 06 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG7", (string)"03 03 00 08 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG8", (string)"03 03 00 09 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG9", (string)"03 03 00 09 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Password", (string)"03 03 00 00 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Antenna.UHF.No1.OutputPower", (string)"03 03 03 02 01 00 06"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Antenna.UHF.No2.OutputPower", (string)"03 03 20 10 01 00 06"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Antenna.UHF.No3.OutputPower", (string)"03 03 20 11 01 00 06"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Antenna.UHF.No4.OutputPower", (string)"03 03 20 12 01 00 06"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Multiplexer.Enable", (string)"03 03 15 00 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Multiplexer.UHF.Internal.AntennaSelectionMode", (string)"03 03 15 01 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Multiplexer.UHF.Internal.NoOfAntennas", (string)"03 03 15 01 01 00 03"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Multiplexer.UHF.Internal.SelectedAntennas", (string)"03 03 15 01 01 03 04"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Region.UHF.EU.Channel.EN302208_4_ChannelPlan.PreferredChannels.ChannelNo1", (string)"03 03 03 12 01 04 04"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Region.UHF.EU.Channel.EN302208_4_ChannelPlan.PreferredChannels.ChannelNo2", (string)"03 03 03 12 01 00 04"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Region.UHF.EU.Channel.EN302208_4_ChannelPlan.PreferredChannels.ChannelNo3", (string)"03 03 03 13 01 04 04"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Region.UHF.EU.Channel.EN302208_4_ChannelPlan.PreferredChannels.ChannelNo4", (string)"03 03 03 13 01 00 04"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Region.UHF.EU.Channel.EN302208_4_ChannelPlan.PreferredChannels.NoOfChannels", (string)"03 03 03 11 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Region.UHF.FCC.Channel.LowerChannel", (string)"03 03 03 09 01 00 06"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Region.UHF.FCC.Channel.UpperChannel", (string)"03 03 03 08 01 00 06"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Region.UHF.Regulation", (string)"03 03 03 03 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.TimeLimit", (string)"03 03 01 06 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Input.No1.Mode", (string)"03 03 02 04 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Output.No1.AccessMode.AccessActiveTime", (string)"03 03 14 10 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Output.No1.ActiveState", (string)"03 03 02 02 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Output.No1.IdleMode", (string)"03 03 02 00 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Output.No1.ReadEventActivation.AntennaNo", (string)"03 03 09 00 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Output.No1.SettlingTime", (string)"03 03 02 08 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Output.No2.AccessMode.AccessActiveTime", (string)"03 03 14 11 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Output.No2.ActiveState", (string)"03 03 02 02 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Output.No2.IdleMode", (string)"03 03 02 00 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Output.No2.ReadEventActivation.AntennaNo", (string)"03 03 09 01 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Output.No2.SettlingTime", (string)"03 03 02 13 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Relay.No1.ActiveState", (string)"03 03 02 02 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Relay.No1.IdleMode", (string)"03 03 02 00 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Relay.No1.ReadEventActivation.AntennaNo", (string)"03 03 09 07 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Relay.No1.SettlingTime", (string)"03 03 02 06 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.DataClock.Format", (string)"03 03 01 11 01 03 03"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Interfaces", (string)"03 03 01 12 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.Autonegotiation.Disable", (string)"03 03 41 04 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.Autonegotiation.Duplex", (string)"03 03 41 04 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.Autonegotiation.Speed", (string)"03 03 41 04 01 06 01"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.Hostname.Enable", (string)"03 03 41 04 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.Hostname.Length", (string)"03 03 33 00 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.Hostname.Name", (string)"03 03 33 01 27 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.IPv4.Enable_DHCP", (string)"03 03 41 04 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.IPv4.GatewayAddress", (string)"03 03 41 06 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.IPv4.IPAddress", (string)"03 03 40 00 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.IPv4.SubnetMask", (string)"03 03 41 00 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.Keepalive.Enable", (string)"03 03 41 04 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.Keepalive.IdleTime", (string)"03 03 41 10 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.Keepalive.IntervalTime", (string)"03 03 41 12 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.Keepalive.RetransmissionCount", (string)"03 03 41 05 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.PortNumber", (string)"03 03 40 08 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Baudrate", (string)"03 03 01 02 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.BusAddress", (string)"03 03 01 00 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Databits", (string)"03 03 01 03 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Parity", (string)"03 03 01 03 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Stopbits", (string)"03 03 01 03 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSelector.Data", (string)"03 03 11 00 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSelector.UID", (string)"03 03 11 00 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.BankNo", (string)"03 03 11 03 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.ByteOrderOfData", (string)"03 03 11 00 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.FirstByte", (string)"03 03 11 11 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.FirstDataBlock", (string)"03 03 11 04 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.NoOfBytes", (string)"03 03 11 12 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.DataSource.NoOfDataBlocks", (string)"03 03 11 08 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Filter.Enable_EventNotification", (string)"03 03 14 01 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Filter.TransponderValidTime", (string)"03 03 12 00 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Transmission.DataLayout.AntennaNo", (string)"03 03 14 03 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Transmission.DataLayout.IDD", (string)"03 03 14 03 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Transmission.DataLayout.Status", (string)"03 03 14 03 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Transmission.DataLayout.TimeStamp", (string)"03 03 14 03 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Transmission.Destination.ConnectionHoldTime", (string)"03 03 49 13 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Transmission.Destination.IPv4.IPAddress", (string)"03 03 49 07 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Transmission.Destination.PortNumber", (string)"03 03 49 11 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Transmission.KeepAlive.Enable", (string)"03 03 49 04 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Transmission.KeepAlive.IntervalTime", (string)"03 03 49 05 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Trigger.Condition", (string)"03 03 10 00 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Trigger.Enable", (string)"03 03 10 00 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Trigger.Enable_UnlimitTransponderValidTime", (string)"03 03 10 00 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Trigger.Source.Input.No1.HoldTime", (string)"03 03 10 02 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.AccessMode.Trigger.Source.Input.No1.TriggerUse", (string)"03 03 10 01 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Miscellaneous.TransponderIdentification.DataBlockNo", (string)"03 03 12 03 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Miscellaneous.TransponderIdentification.NoOfDataBlocks", (string)"03 03 12 05 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Miscellaneous.TransponderIdentification.Source", (string)"03 03 12 02 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Mode", (string)"03 03 01 13 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Anticollision.Enable", (string)"03 03 05 11 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.UHF.Drivers", (string)"03 03 03 00 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.UHF.EPC_Class1Gen2", (string)"03 03 03 01 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Miscellaneous.IdentifierInterpretationMode", (string)"03 03 04 12 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Miscellaneous.TIDLength", (string)"03 03 04 13 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.PersistenceReset.Antenna.No1.PersistenceResetTime", (string)"03 03 16 02 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.PersistenceReset.Antenna.No2.PersistenceResetTime", (string)"03 03 16 04 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.PersistenceReset.Antenna.No3.PersistenceResetTime", (string)"03 03 16 06 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.PersistenceReset.Antenna.No4.PersistenceResetTime", (string)"03 03 16 08 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.PersistenceReset.Mode", (string)"03 03 16 00 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.UHF.EPC_Class1Gen2.Anticollision.Session", (string)"03 03 05 10 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"Transponder.UHF.EPC_Class1Gen2.Miscellaneous.WriteOption", (string)"03 03 04 11 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.UHF.EPC_Class1Gen2.SelectionMask.No1.BankNo", (string)"03 03 22 01 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"Transponder.UHF.EPC_Class1Gen2.SelectionMask.No1.FirstBit", (string)"03 03 22 02 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.UHF.EPC_Class1Gen2.SelectionMask.No1.Mask", (string)"03 03 22 04 24 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.UHF.EPC_Class1Gen2.SelectionMask.No1.MaskLength", (string)"03 03 22 00 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.UHF.EPC_Class1Gen2.SelectionMask.No1.Negation", (string)"03 03 22 01 01 02 01"));
}
