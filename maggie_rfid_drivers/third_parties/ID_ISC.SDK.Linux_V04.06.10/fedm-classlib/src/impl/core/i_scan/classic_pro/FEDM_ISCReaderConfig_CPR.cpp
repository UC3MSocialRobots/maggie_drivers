/*-------------------------------------------------------
|
|               FEDM_ISCReaderConfig_CPR.cpp
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

Function			:	implementation of Access-ID map for OBID® classic-pro reader family


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#include "../FEDM_ISCReader.h"


void FEDM_ISCReader::BuildAccessIdMap_CPR()
{
	m_mapAccessID.clear();

	m_mapAccessID.insert(make_pair((string)"AirInterface.Antenna.HF.Miscellaneous.MinimumLevel", (string)"03 03 03 06 01 00 04"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Antenna.HF.Miscellaneous.MinimumLevelMode", (string)"03 03 03 06 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Miscellaneous.HF.ISO_15693.Modulation", (string)"03 03 04 04 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.TimeLimit", (string)"03 03 01 06 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.Buzzer.IdleFlashFrequency", (string)"03 03 02 04 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.Buzzer.IdleState", (string)"03 03 02 03 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.Buzzer.InputFlashFrequency", (string)"03 03 02 06 01 03 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.Buzzer.OfflineFlashFrequency", (string)"03 03 02 08 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.Buzzer.OfflineState", (string)"03 03 02 07 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.EventSource.Input1", (string)"03 03 02 05 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.EventSource.Input2", (string)"03 03 02 05 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.EventSource.Input3", (string)"03 03 02 05 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.EventSource.OfflineDetect", (string)"03 03 02 01 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.EventSource.StartupSignal", (string)"03 03 02 01 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.EventSource.TagDetect", (string)"03 03 02 01 01 06 01"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Green.IdleFlashFrequency", (string)"03 03 02 04 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Green.IdleState", (string)"03 03 02 03 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Green.InputFlashFrequency", (string)"03 03 02 06 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Green.OfflineFlashFrequency", (string)"03 03 02 08 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Green.OfflineState", (string)"03 03 02 07 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Red.IdleFlashFrequency", (string)"03 03 02 04 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Red.IdleState", (string)"03 03 02 03 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Red.InputFlashFrequency", (string)"03 03 02 06 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Red.OfflineFlashFrequency", (string)"03 03 02 08 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.Red.OfflineState", (string)"03 03 02 07 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.OfflineDelay", (string)"03 03 02 09 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.DataClock.Format", (string)"03 03 01 13 01 02 03"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Baudrate", (string)"03 03 01 02 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.BusAddress", (string)"03 03 01 00 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Databits", (string)"03 03 01 03 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Parity", (string)"03 03 01 03 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.ProtocolStartSynchronizationTime", (string)"03 03 01 05 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Stopbits", (string)"03 03 01 03 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.HostMode.Filter.ResponseMode", (string)"03 03 05 11 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.HostMode.InventoryAbortCondition", (string)"03 03 05 11 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Mode", (string)"03 03 01 13 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.InputMessage", (string)"03 03 02 05 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.BusAddressPrefix", (string)"03 03 06 03 01 06 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.EndChar", (string)"03 03 07 03 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.Format", (string)"03 03 07 00 01 00 04"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.UidInBcd", (string)"03 03 06 03 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.UserEndChar1", (string)"03 03 07 04 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSelector.Data", (string)"03 03 06 03 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSelector.MifareAppID", (string)"03 03 06 03 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSelector.Mode.Enable_IndependentTransmission", (string)"03 03 06 04 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSelector.Selector", (string)"03 03 06 03 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSelector.UID", (string)"03 03 06 03 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSource.ByteOrderOfData", (string)"03 03 06 03 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSource.FirstByte", (string)"03 03 06 13 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSource.FirstDataBlock", (string)"03 03 06 11 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSource.Mifare.Classic.AppID", (string)"03 03 06 08 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSource.Mifare.Classic.KeyAddress", (string)"03 03 06 10 01 00 04"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSource.Mifare.Classic.KeyType", (string)"03 03 06 10 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSource.NoOfBytes", (string)"03 03 06 12 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.Filter.ResponseMode", (string)"03 03 05 11 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.Filter.TransponderValidTime", (string)"03 03 06 06 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.Interface", (string)"03 03 01 13 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Anticollision.Enable", (string)"03 03 05 11 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.Drivers", (string)"03 03 03 00 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_14443.Drivers", (string)"03 03 03 02 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_14443.TypeA.MifareStandard", (string)"03 03 03 03 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_14443.TypeA.MifareUltraLight", (string)"03 03 03 03 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_14443.TypeA.Part4", (string)"03 03 03 03 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_14443.TypeA.my_d_Proximity_SLE55Rxx", (string)"03 03 03 03 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_14443_TypeA", (string)"03 03 03 01 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_14443_TypeB", (string)"03 03 03 01 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_15693", (string)"03 03 03 01 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.Jewel", (string)"03 03 03 00 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.Bitrate.Receive", (string)"03 03 03 07 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.Bitrate.Transmit", (string)"03 03 03 07 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.Miscellaneous.StartupTime", (string)"03 03 03 12 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.Part4.Miscellaneous.ErrorRetry", (string)"03 03 03 13 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.Part4.Miscellaneous.PowerLevelCheck", (string)"03 03 03 13 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.TypeA.OptionalInventoryInfo", (string)"03 03 03 13 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.TypeB.Miscellaneous.BitrateSelect", (string)"03 03 03 13 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.TypeB.SelectionMask.AFI", (string)"03 03 04 08 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.Anticollision.NoOfTimeslots", (string)"03 03 04 04 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.Miscellaneous.ReadOption.BlockSize", (string)"03 03 04 13 01 00 05"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.Miscellaneous.WriteOption", (string)"03 03 04 06 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.SelectionMask.AFI1", (string)"03 03 04 05 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.SelectionMask.Enable_AFI", (string)"03 03 04 04 01 05 01"));
}
