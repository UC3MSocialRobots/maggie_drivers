/*-------------------------------------------------------
|
|               FEDM_ISCReaderConfig_LR2500-B.cpp
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

Function			:	implementation of Access-ID map for OBID i-scan® HF reader ID ISC.LR2500-B


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#include "FEDM_ISCReader.h"


void FEDM_ISCReader::BuildAccessIdMap_LR2500_B()
{
	m_mapAccessID.clear();

	FEDM_ISCReader::BuildAccessIdMap_LR2500_B_Part0();
	FEDM_ISCReader::BuildAccessIdMap_LR2500_B_Part1();
}


void FEDM_ISCReader::BuildAccessIdMap_LR2500_B_Part0()
{
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
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG30_39", (string)"03 03 00 10 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG4", (string)"03 03 00 08 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG5", (string)"03 03 00 08 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG6", (string)"03 03 00 08 01 06 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG63", (string)"03 03 00 11 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG64_86", (string)"03 03 00 11 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG7", (string)"03 03 00 08 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG8", (string)"03 03 00 09 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG9", (string)"03 03 00 09 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Password", (string)"03 03 00 00 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Antenna.HF.Miscellaneous.Enable_DCPower", (string)"03 03 03 13 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Antenna.HF.Miscellaneous.Enable_PowerUpTuning", (string)"03 03 03 13 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Antenna.HF.No1.OutputPower", (string)"03 03 03 02 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Miscellaneous.HF.ISO_15693_ICode1.ErrorHandling.WeakCollision", (string)"03 03 20 07 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Miscellaneous.HF.Modulation", (string)"03 03 20 00 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Miscellaneous.HF.ReceiverChannel", (string)"03 03 20 08 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Multiplexer.Enable", (string)"03 03 15 00 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Multiplexer.HF.External.Antenna.No1.ActiveTime", (string)"03 03 15 02 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Multiplexer.HF.External.Antenna.No2.ActiveTime", (string)"03 03 15 04 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Multiplexer.HF.External.Antenna.No3.ActiveTime", (string)"03 03 15 06 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Multiplexer.HF.External.Antenna.No4.ActiveTime", (string)"03 03 15 08 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Multiplexer.HF.External.Antenna.No5.ActiveTime", (string)"03 03 15 10 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Multiplexer.HF.External.Antenna.No6.ActiveTime", (string)"03 03 17 00 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Multiplexer.HF.External.Antenna.No7.ActiveTime", (string)"03 03 17 02 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Multiplexer.HF.External.Antenna.No8.ActiveTime", (string)"03 03 17 04 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Multiplexer.HF.External.InputChannelMode", (string)"03 03 15 13 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Multiplexer.HF.External.NoOfOutputChannels", (string)"03 03 15 01 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.TimeLimit", (string)"03 03 01 06 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Input.No1.Mode", (string)"03 03 02 04 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Input.No2.Mode", (string)"03 03 02 04 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Input.No3.Mode", (string)"03 03 02 04 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Miscellaneous.AFIAssignment", (string)"03 03 09 13 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Output.No1.ActiveState", (string)"03 03 02 02 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Output.No1.IdleMode", (string)"03 03 02 00 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Output.No1.ReadEventActivation.AntennaNo", (string)"03 03 09 00 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Output.No1.SettlingTime", (string)"03 03 02 08 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Output.No2.ActiveState", (string)"03 03 02 02 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Output.No2.IdleMode", (string)"03 03 02 00 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Output.No2.ReadEventActivation.AntennaNo", (string)"03 03 09 01 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Output.No2.SettlingTime", (string)"03 03 02 05 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Relay.No1.ActiveState", (string)"03 03 02 02 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Relay.No1.IdleMode", (string)"03 03 02 00 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Relay.No1.ReadEventActivation.AntennaNo", (string)"03 03 09 07 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Relay.No1.SettlingTime", (string)"03 03 02 06 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Relay.No2.ActiveState", (string)"03 03 02 03 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Relay.No2.IdleMode", (string)"03 03 02 01 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Relay.No2.ReadEventActivation.AntennaNo", (string)"03 03 09 08 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Relay.No2.SettlingTime", (string)"03 03 02 10 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Relay.No3.ActiveState", (string)"03 03 02 03 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Relay.No3.IdleMode", (string)"03 03 02 01 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Relay.No3.ReadEventActivation.AntennaNo", (string)"03 03 09 09 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Relay.No3.SettlingTime", (string)"03 03 02 12 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Yellow.InputEventActivation", (string)"03 03 09 12 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Yellow.OutputEventActivation", (string)"03 03 09 11 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.DataClock.Format", (string)"03 03 01 11 01 03 03"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Interfaces", (string)"03 03 01 12 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.IPv4.Enable_DHCP", (string)"03 03 69 00 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.IPv4.GatewayAddress", (string)"03 03 68 00 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.IPv4.IPAddress", (string)"03 03 66 00 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.IPv4.PortNumber", (string)"03 03 66 20 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.IPv4.SubnetMask", (string)"03 03 67 00 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.Keepalive.Enable", (string)"03 03 69 04 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.Keepalive.IntervalTime", (string)"03 03 69 08 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.Keepalive.RetransmissionCount", (string)"03 03 69 05 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Baudrate", (string)"03 03 01 02 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.BusAddress", (string)"03 03 01 00 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Databits", (string)"03 03 01 03 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Parity", (string)"03 03 01 03 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.RS4xx.Enable_TerminationResistors", (string)"03 03 01 04 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Stopbits", (string)"03 03 01 03 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.AFI", (string)"03 03 11 01 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.AntennaNo", (string)"03 03 11 00 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.Data", (string)"03 03 11 00 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.Date", (string)"03 03 11 00 01 06 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.Direction", (string)"03 03 11 01 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.EAS", (string)"03 03 11 00 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.InputEvents", (string)"03 03 11 01 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.Mode.Enable_AntennaPool", (string)"03 03 11 02 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.RSSI", (string)"03 03 11 01 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.Selector", (string)"03 03 11 00 03 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.Time", (string)"03 03 11 00 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.UID", (string)"03 03 11 00 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSource.ByteOrderOfData", (string)"03 03 11 00 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSource.FirstDataBlock", (string)"03 03 11 04 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSource.NoOfDataBlocks", (string)"03 03 11 08 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.EAS.AlarmMode", (string)"03 03 11 10 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.EAS.TagType", (string)"03 03 11 10 01 00 04"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.Filter.Enable_Input1Event", (string)"03 03 12 06 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.Filter.Enable_Input2Event", (string)"03 03 12 06 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.Filter.Enable_Input3Event", (string)"03 03 12 06 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.Filter.Enable_TimeoutEvent", (string)"03 03 12 07 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.Filter.Enable_TriggerEvent", (string)"03 03 12 07 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.Filter.TransponderValidTime", (string)"03 03 12 00 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.Trigger.Condition", (string)"03 03 10 00 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.Trigger.Enable", (string)"03 03 10 00 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.Trigger.Enable_UnlimitTransponderValidTime", (string)"03 03 10 00 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.Trigger.Source.Input.No1.HoldTime", (string)"03 03 10 02 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.Trigger.Source.Input.No1.TriggerUse", (string)"03 03 10 01 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.Trigger.Source.Input.No2.HoldTime", (string)"03 03 10 04 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.Trigger.Source.Input.No2.TriggerUse", (string)"03 03 10 01 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.Trigger.Source.Input.No3.HoldTime", (string)"03 03 10 06 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.Trigger.Source.Input.No3.TriggerUse", (string)"03 03 10 01 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Miscellaneous.HF.ReaderSynchronization.Enable_Synchronization", (string)"03 03 18 00 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Miscellaneous.HF.ReaderSynchronization.Is_Master", (string)"03 03 18 00 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Miscellaneous.HF.ReaderSynchronization.RfOffCondition", (string)"03 03 18 00 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Miscellaneous.HF.ReaderSynchronization.SynchronizationTimeout", (string)"03 03 18 02 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Miscellaneous.TransponderIdentification.DataBlockNo", (string)"03 03 12 03 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Miscellaneous.TransponderIdentification.NoOfDataBlocks", (string)"03 03 12 05 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Miscellaneous.TransponderIdentification.Source", (string)"03 03 12 02 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Mode", (string)"03 03 01 13 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSelector.AFI", (string)"03 03 11 01 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSelector.AntennaNo", (string)"03 03 11 00 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSelector.Data", (string)"03 03 11 00 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSelector.Date", (string)"03 03 11 00 01 06 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSelector.Direction", (string)"03 03 11 01 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSelector.EAS", (string)"03 03 11 00 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSelector.InputEvents", (string)"03 03 11 01 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSelector.Mode.Enable_AntennaPool", (string)"03 03 11 02 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSelector.RSSI", (string)"03 03 11 01 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSelector.Selector", (string)"03 03 11 00 03 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSelector.Time", (string)"03 03 11 00 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSelector.UID", (string)"03 03 11 00 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.ByteOrderOfData", (string)"03 03 11 00 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.FirstDataBlock", (string)"03 03 11 04 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.NoOfDataBlocks", (string)"03 03 11 08 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.EAS.AlarmMode", (string)"03 03 11 10 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.EAS.TagType", (string)"03 03 11 10 01 00 04"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Filter.Enable_Input1Event", (string)"03 03 12 06 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Filter.Enable_Input2Event", (string)"03 03 12 06 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Filter.Enable_Input3Event", (string)"03 03 12 06 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Filter.Enable_TimeoutEvent", (string)"03 03 12 07 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Filter.Enable_TriggerEvent", (string)"03 03 12 07 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Filter.TransponderValidTime", (string)"03 03 12 00 02 00 00"));
}


void FEDM_ISCReader::BuildAccessIdMap_LR2500_B_Part1()
{
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Transmission.Destination.ConnectionHoldTime", (string)"03 03 83 22 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Transmission.Destination.IPv4.IPAddress", (string)"03 03 83 00 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Transmission.Destination.PortNumber", (string)"03 03 83 20 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Transmission.Enable_Acknowledge", (string)"03 03 82 00 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Transmission.KeepAlive.Enable", (string)"03 03 82 04 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Transmission.KeepAlive.IntervalTime", (string)"03 03 82 05 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Trigger.Condition", (string)"03 03 10 00 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Trigger.Enable", (string)"03 03 10 00 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Trigger.Enable_UnlimitTransponderValidTime", (string)"03 03 10 00 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Trigger.Source.Input.No1.HoldTime", (string)"03 03 10 02 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Trigger.Source.Input.No1.TriggerUse", (string)"03 03 10 01 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Trigger.Source.Input.No2.HoldTime", (string)"03 03 10 04 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Trigger.Source.Input.No2.TriggerUse", (string)"03 03 10 01 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Trigger.Source.Input.No3.HoldTime", (string)"03 03 10 06 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Trigger.Source.Input.No3.TriggerUse", (string)"03 03 10 01 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.BusAddressPrefix", (string)"03 03 11 02 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.EndChar", (string)"03 03 13 03 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.Format", (string)"03 03 13 00 01 00 04"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.NoOfUserEndChars", (string)"03 03 13 13 01 00 04"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.NoOfUserHeaderChars", (string)"03 03 13 13 01 04 04"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.SeparationChar", (string)"03 03 13 01 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.UserEndChar1", (string)"03 03 13 04 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.UserEndChar2", (string)"03 03 13 05 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.UserEndChar3", (string)"03 03 13 06 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.UserHeaderChar1", (string)"03 03 13 08 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.UserHeaderChar2", (string)"03 03 13 09 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.UserHeaderChar3", (string)"03 03 13 10 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.UserHeaderChar4", (string)"03 03 13 11 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.UserSeparationChar", (string)"03 03 13 02 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSelector.AntennaNo", (string)"03 03 11 00 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSelector.Data", (string)"03 03 11 00 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSelector.Date", (string)"03 03 11 00 01 06 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSelector.Direction", (string)"03 03 11 01 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSelector.EAS", (string)"03 03 11 00 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSelector.InputEvents", (string)"03 03 11 01 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSelector.Mode.Enable_AntennaPool", (string)"03 03 11 02 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSelector.Selector", (string)"03 03 11 00 03 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSelector.Time", (string)"03 03 11 00 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSelector.UID", (string)"03 03 11 00 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSource.ByteOrderOfData", (string)"03 03 11 00 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSource.FirstByte", (string)"03 03 11 11 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSource.FirstDataBlock", (string)"03 03 11 04 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSource.NoOfBytes", (string)"03 03 11 12 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSource.NoOfDataBlocks", (string)"03 03 11 08 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.EAS.AlarmMode", (string)"03 03 11 10 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.EAS.TagType", (string)"03 03 11 10 01 00 04"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.Filter.TransponderValidTime", (string)"03 03 12 00 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.Interface", (string)"03 03 01 11 01 00 03"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.Trigger.Condition", (string)"03 03 10 00 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.Trigger.Enable", (string)"03 03 10 00 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.Trigger.Enable_UnlimitTransponderValidTime", (string)"03 03 10 00 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.Trigger.Source.Input.No1.HoldTime", (string)"03 03 10 02 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.Trigger.Source.Input.No1.TriggerUse", (string)"03 03 10 01 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.Trigger.Source.Input.No2.HoldTime", (string)"03 03 10 04 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.Trigger.Source.Input.No2.TriggerUse", (string)"03 03 10 01 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.Trigger.Source.Input.No3.HoldTime", (string)"03 03 10 06 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.Trigger.Source.Input.No3.TriggerUse", (string)"03 03 10 01 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"PeripheralDevice.DetectMode", (string)"03 03 08 13 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"PeripheralDevice.DigitalIO.Output.SettlingTime", (string)"03 03 08 01 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"PeripheralDevice.DirectionMode", (string)"03 03 08 12 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"PeripheralDevice.GatePeopleCounter.Filter.TimeLimit_AfterTagDetection", (string)"03 03 10 12 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"PeripheralDevice.GatePeopleCounter.Filter.TimeLimit_BeforeTagDetection", (string)"03 03 10 10 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"PeripheralDevice.Mode", (string)"03 03 08 00 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"PeripheralDevice.RS485.BusAdr1.DigitalIO.Output.No1.ReadEventActivation.AntennaNo", (string)"03 03 08 03 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"PeripheralDevice.RS485.BusAdr1.DigitalIO.Output.No2.ReadEventActivation.AntennaNo", (string)"03 03 08 04 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"PeripheralDevice.RS485.BusAdr1.DigitalIO.Output.No3.ReadEventActivation.AntennaNo", (string)"03 03 08 05 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"PeripheralDevice.RS485.BusAdr2.DigitalIO.Output.No1.ReadEventActivation.AntennaNo", (string)"03 03 08 06 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"PeripheralDevice.RS485.BusAdr2.DigitalIO.Output.No2.ReadEventActivation.AntennaNo", (string)"03 03 08 07 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"PeripheralDevice.RS485.BusAdr2.DigitalIO.Output.No3.ReadEventActivation.AntennaNo", (string)"03 03 08 08 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"PeripheralDevice.RS485.BusAdr3.DigitalIO.Output.No1.ReadEventActivation.AntennaNo", (string)"03 03 08 09 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"PeripheralDevice.RS485.BusAdr3.DigitalIO.Output.No2.ReadEventActivation.AntennaNo", (string)"03 03 08 10 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"PeripheralDevice.RS485.BusAdr3.DigitalIO.Output.No3.ReadEventActivation.AntennaNo", (string)"03 03 08 11 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Anticollision.Enable", (string)"03 03 05 11 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.Drivers", (string)"03 03 03 00 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ICode1", (string)"03 03 03 01 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_15693", (string)"03 03 03 01 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.CustomerCommandOptions.EM_Microelectronic.StorageQuiet", (string)"03 03 06 01 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.CustomerCommandOptions.Fujitsu.FastCommands", (string)"03 03 06 02 01 01 02"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.CustomerCommandOptions.Fujitsu.FastInventory", (string)"03 03 06 02 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.CustomerCommandOptions.Infineon.ReadWrite_Commands", (string)"03 03 06 03 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.CustomerCommandOptions.NXP.FastReturnLink", (string)"03 03 06 06 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.CustomerCommandOptions.NXP.InventoryReadBlock", (string)"03 03 06 06 01 06 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.CustomerCommandOptions.NXP.InventoryReadPage", (string)"03 03 06 06 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.CustomerCommandOptions.NXP.LongRangeCommand", (string)"03 03 06 06 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.CustomerCommandOptions.STMicroelectronics.FastCommands", (string)"03 03 06 07 01 01 02"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.CustomerCommandOptions.STMicroelectronics.InitiateInventory", (string)"03 03 06 07 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.CustomerCommandOptions.Texas_Instruments.FastSID", (string)"03 03 06 09 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.CustomerCommandOptions.Texas_Instruments.InventoryReadBlock", (string)"03 03 06 09 01 06 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ICode1.Anticollision.MaximalTimeslots", (string)"03 03 05 00 01 04 04"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ICode1.Anticollision.MinimalTimeslots", (string)"03 03 05 00 01 00 04"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ICode1.LinkRate.FastMode", (string)"03 03 04 00 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ICode1.Miscellaneous.EASLevel", (string)"03 03 03 03 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ICode1.SelectionMask.ApplicationIdentifier", (string)"03 03 04 02 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ICode1.SelectionMask.FamilyCode", (string)"03 03 04 01 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.Anticollision.NoOfTimeslots", (string)"03 03 04 04 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.LinkRate.DataCoding", (string)"03 03 04 04 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.Miscellaneous.ReadOption.BlockSize", (string)"03 03 04 13 01 00 05"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.Miscellaneous.ReadOption.BlockSizeSelection", (string)"03 03 04 13 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.Miscellaneous.ReadOption.ReadMode", (string)"03 03 04 13 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.Miscellaneous.WriteOption", (string)"03 03 04 06 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.SelectionMask.AFI1", (string)"03 03 04 05 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.SelectionMask.AFI2", (string)"03 03 04 08 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.SelectionMask.AFI3", (string)"03 03 04 09 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.SelectionMask.AFI4", (string)"03 03 04 10 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.SelectionMask.Enable_AFI", (string)"03 03 04 04 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.PersistenceReset.Antenna.No1.PersistenceResetTime", (string)"03 03 16 02 02 00 00"));
}
