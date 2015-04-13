/*-------------------------------------------------------
|
|               FEDM_ISCReaderConfig_LR200.cpp
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

Function			:	implementation of Access-ID map for OBID i-scan® HF reader ID ISC.LR200


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#include "FEDM_ISCReader.h"


void FEDM_ISCReader::BuildAccessIdMap_LR200()
{
	m_mapAccessID.clear();

	m_mapAccessID.insert(make_pair((string)"AccessProtection.Password", (string)"03 03 00 00 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Antenna.HF.No1.OutputPower", (string)"03 03 03 02 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Miscellaneous.HF.ASK_Modulation.ErrorHandling.WeakCollision", (string)"03 03 04 07 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Miscellaneous.HF.ISO_15693.Modulation", (string)"03 03 08 04 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Miscellaneous.HF.ISO_15693.SubCarrier", (string)"03 03 08 04 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Miscellaneous.HF.Modulation", (string)"03 03 05 06 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Miscellaneous.HF.ReceiverChannel", (string)"03 03 03 04 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Multiplexer.Enable", (string)"03 03 15 00 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Multiplexer.HF.External.Antenna.No1.ActiveTime", (string)"03 03 15 02 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Multiplexer.HF.External.InputChannelMode", (string)"03 03 15 04 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Multiplexer.HF.External.NoOfOutputChannels", (string)"03 03 15 05 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Multiplexer.HF.External.SwitchCondition", (string)"03 03 15 00 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.TimeLimit", (string)"03 03 02 06 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Input.No1.Mode", (string)"03 03 01 04 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Input.No2.Mode", (string)"03 03 01 04 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Output.No1.IdleFlashMode", (string)"03 03 01 02 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Output.No1.IdleMode", (string)"03 03 01 00 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Output.No2.IdleFlashMode", (string)"03 03 01 02 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Output.No2.IdleMode", (string)"03 03 01 00 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Output.No2.SettlingTime", (string)"03 03 01 08 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Relay.No1.IdleFlashMode", (string)"03 03 01 02 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Relay.No1.IdleMode", (string)"03 03 01 00 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Relay.No1.SettlingTime", (string)"03 03 01 06 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.DataClock.Format", (string)"03 03 12 01 01 02 03"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Baudrate", (string)"03 03 02 02 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.BusAddress", (string)"03 03 02 00 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Databits", (string)"03 03 02 03 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.FlashloaderBaudrate", (string)"03 03 02 04 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Parity", (string)"03 03 02 03 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Stopbits", (string)"03 03 02 03 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.AntennaNo", (string)"03 03 11 00 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.Data", (string)"03 03 11 00 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.EAS", (string)"03 03 11 00 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.Mode.Enable_AntennaPool", (string)"03 03 10 13 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.Time", (string)"03 03 11 00 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.UID", (string)"03 03 11 00 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSource.ByteOrderOfData", (string)"03 03 11 00 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSource.FirstDataBlock", (string)"03 03 11 01 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSource.NoOfDataBlocks", (string)"03 03 11 02 01 00 06"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.EAS.AlarmMode", (string)"03 03 10 13 01 06 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.Filter.TransponderValidTime", (string)"03 03 11 04 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.Miscellaneous.NoRfReset", (string)"03 03 10 13 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.HostMode.Filter.ResponseMode", (string)"03 03 13 11 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Miscellaneous.HF.ReaderSynchronization.Enable_Synchronization", (string)"03 03 11 07 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Miscellaneous.HF.ReaderSynchronization.Is_Master", (string)"03 03 11 07 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Miscellaneous.HF.ReaderSynchronization.RfOffCondition", (string)"03 03 11 07 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Miscellaneous.HF.ReaderSynchronization.SwitchCondition", (string)"03 03 11 07 01 06 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Miscellaneous.HF.ReaderSynchronization.SynchronizationTimeout", (string)"03 03 11 08 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Miscellaneous.TransponderIdentification.DataBlockNo", (string)"03 03 11 03 01 00 07"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Miscellaneous.TransponderIdentification.Source", (string)"03 03 11 03 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Mode", (string)"03 03 10 00 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.BusAddressPrefix", (string)"03 03 12 03 01 06 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.EndChar", (string)"03 03 12 12 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.Format", (string)"03 03 12 09 01 00 04"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.SeparationChar", (string)"03 03 12 01 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.UserEndChar", (string)"03 03 12 13 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.UserSeparationChar", (string)"03 03 12 11 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSelector.AntennaNo", (string)"03 03 12 03 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSelector.Data", (string)"03 03 12 03 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSelector.Time", (string)"03 03 12 03 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSelector.UID", (string)"03 03 12 03 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSource.ByteOrderOfData", (string)"03 03 12 03 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSource.FirstByte", (string)"03 03 12 08 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSource.FirstDataBlock", (string)"03 03 12 04 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSource.NoOfBytes", (string)"03 03 12 05 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.Filter.TransponderValidTime", (string)"03 03 12 06 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.Interface", (string)"03 03 12 01 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.Trigger.Enable", (string)"03 03 12 00 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.Trigger.Mode", (string)"03 03 12 00 01 00 03"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Anticollision.Enable", (string)"03 03 10 00 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.Drivers", (string)"03 03 03 00 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ICode1", (string)"03 03 03 01 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ICodeEPC", (string)"03 03 03 01 01 06 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ICodeUID", (string)"03 03 03 01 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_15693", (string)"03 03 03 01 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.Tagit_HF", (string)"03 03 03 01 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ICode1.Anticollision.MaximalTimeslots", (string)"03 03 13 00 01 04 03"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ICode1.Anticollision.MinimalTimeslots", (string)"03 03 13 00 01 00 03"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ICode1.LinkRate.FastMode", (string)"03 03 08 00 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ICode1.Miscellaneous.EASLevel", (string)"03 03 03 03 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ICode1.Miscellaneous.MemoryMapping", (string)"03 03 08 00 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ICode1.SelectionMask.ApplicationIdentifier", (string)"03 03 08 02 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ICode1.SelectionMask.FamilyCode", (string)"03 03 08 01 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ICodeUID_ICodeEPC.Anticollision.MaximalTimeslots", (string)"03 03 13 00 01 04 03"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ICodeUID_ICodeEPC.Anticollision.MinimalTimeslots", (string)"03 03 13 00 01 00 03"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ICodeUID_ICodeEPC.SelectionMask.Mask", (string)"03 03 06 01 19 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ICodeUID_ICodeEPC.SelectionMask.MaskLength", (string)"03 03 06 00 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.Anticollision.NoOfTimeslots", (string)"03 03 08 04 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.LinkRate.DataCoding", (string)"03 03 08 04 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.LinkRate.DataRate", (string)"03 03 08 04 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.Miscellaneous.CommandBreak", (string)"03 03 08 07 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.Miscellaneous.ReadOption.BlockSize", (string)"03 03 08 13 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.Miscellaneous.WriteOption", (string)"03 03 08 06 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.SelectionMask.AFI1", (string)"03 03 08 05 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.SelectionMask.Enable_AFI", (string)"03 03 08 04 01 05 01"));
}
