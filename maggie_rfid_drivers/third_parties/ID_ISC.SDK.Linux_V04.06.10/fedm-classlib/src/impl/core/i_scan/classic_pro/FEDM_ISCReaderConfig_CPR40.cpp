/*-------------------------------------------------------
|
|               FEDM_ISCReaderConfig_CPR40.cpp
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

Function			:	implementation of Access-ID map for OBID® classic-pro reader ID CPR40.xx


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#include "../FEDM_ISCReader.h"


void FEDM_ISCReader::BuildAccessIdMap_CPR40()
{
	m_mapAccessID.clear();

	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG0", (string)"03 03 15 04 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG1", (string)"03 03 15 04 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG14", (string)"03 03 15 07 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG15", (string)"03 03 15 07 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG2", (string)"03 03 15 04 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG3", (string)"03 03 15 04 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG4", (string)"03 03 15 05 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG5", (string)"03 03 15 05 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG6", (string)"03 03 15 05 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG7", (string)"03 03 15 05 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Password", (string)"03 03 15 00 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Antenna.HF.Miscellaneous.MinimumLevel", (string)"03 03 03 06 01 00 04"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Antenna.HF.Miscellaneous.MinimumLevelMode", (string)"03 03 03 06 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.TimeLimit", (string)"03 03 01 06 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.No1.OfflineState", (string)"03 03 02 08 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.No1.OnlineState", (string)"03 03 02 03 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.No1.TagDetectState", (string)"03 03 02 12 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.No2.OfflineState", (string)"03 03 02 08 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.No2.OnlineState", (string)"03 03 02 03 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.No2.TagDetectState", (string)"03 03 02 10 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.No3.OfflineState", (string)"03 03 02 08 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.No3.OnlineState", (string)"03 03 02 03 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.LED.No3.TagDetectState", (string)"03 03 02 10 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.OfflineDelay", (string)"03 03 02 10 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.TagDetectActiveTime", (string)"03 03 02 13 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.DataClock.Format", (string)"03 03 01 13 01 02 03"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Baudrate", (string)"03 03 01 02 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.BusAddress", (string)"03 03 01 00 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Databits", (string)"03 03 01 03 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Parity", (string)"03 03 01 03 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.ProtocolStartSynchronizationTime", (string)"03 03 01 05 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Stopbits", (string)"03 03 01 03 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.USB.Enable_UniformDeviceID", (string)"03 03 01 04 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.HostMode.Filter.ResponseMode", (string)"03 03 05 11 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.HostMode.InventoryAbortCondition", (string)"03 03 05 11 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Mode", (string)"03 03 01 13 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.BusAddressPrefix", (string)"03 03 06 03 01 06 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.EndChar", (string)"03 03 07 03 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.Format", (string)"03 03 07 00 01 00 04"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.SeparationChar", (string)"03 03 07 01 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.UidInBcd", (string)"03 03 06 03 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.UserEndChar1", (string)"03 03 07 04 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataFormat.UserSeparationChar", (string)"03 03 07 02 01 00 00"));
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
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSource.Mifare.DESFire.AppID", (string)"03 03 10 00 03 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSource.Mifare.DESFire.FileCommSettings", (string)"03 03 10 06 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSource.Mifare.DESFire.FileLength", (string)"03 03 10 10 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSource.Mifare.DESFire.FileNumber", (string)"03 03 10 05 01 00 05"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSource.Mifare.DESFire.FileOffset", (string)"03 03 10 07 03 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSource.Mifare.DESFire.FileType", (string)"03 03 10 06 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSource.Mifare.DESFire.KeyNumber", (string)"03 03 10 03 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.ScanMode.DataSource.Mifare.DESFire.ReaderKeyIndex", (string)"03 03 10 04 01 00 00"));
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
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.Jewel", (string)"03 03 03 00 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.SR176", (string)"03 03 03 00 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.SRIxx", (string)"03 03 03 00 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.Selector", (string)"03 03 03 00 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.UserSequenced.No1", (string)"03 03 11 00 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.UserSequenced.No10", (string)"03 03 11 09 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.UserSequenced.No11", (string)"03 03 11 10 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.UserSequenced.No12", (string)"03 03 11 11 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.UserSequenced.No13", (string)"03 03 11 12 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.UserSequenced.No14", (string)"03 03 11 13 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.UserSequenced.No2", (string)"03 03 11 01 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.UserSequenced.No3", (string)"03 03 11 02 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.UserSequenced.No4", (string)"03 03 11 03 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.UserSequenced.No5", (string)"03 03 11 04 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.UserSequenced.No6", (string)"03 03 11 05 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.UserSequenced.No7", (string)"03 03 11 06 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.UserSequenced.No8", (string)"03 03 11 07 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.UserSequenced.No9", (string)"03 03 11 08 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.Bitrate.Receive", (string)"03 03 03 07 01 04 02"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.Bitrate.Transmit", (string)"03 03 03 07 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.Miscellaneous.StartupTime", (string)"03 03 03 12 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.Part4.Miscellaneous.ErrorRetry", (string)"03 03 03 13 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.Part4.Miscellaneous.PowerLevelCheck", (string)"03 03 03 13 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.TypeA.OptionalInventoryInfo", (string)"03 03 03 13 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.TypeA.UID_Order", (string)"03 03 03 13 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.TypeB.Miscellaneous.BitrateSelect", (string)"03 03 03 13 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.TypeB.SelectionMask.AFI", (string)"03 03 04 08 01 00 00"));
}
