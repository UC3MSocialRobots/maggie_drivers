/*-------------------------------------------------------
|
|               FEDM_ISCReaderConfig_PRH200.cpp
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

Function			:	implementation of Access-ID map for OBID i-scan® HF reader ID ISC.PRH200


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#include "FEDM_ISCReader.h"


void FEDM_ISCReader::BuildAccessIdMap_PRH200()
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
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG30_39", (string)"03 03 00 10 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG4", (string)"03 03 00 08 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG40_49", (string)"03 03 00 11 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG5", (string)"03 03 00 08 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG6", (string)"03 03 00 08 01 06 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG63", (string)"03 03 00 11 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG7", (string)"03 03 00 08 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG8", (string)"03 03 00 09 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Lock_CFG9", (string)"03 03 00 09 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"AccessProtection.Password", (string)"03 03 00 00 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Antenna.HF.No1.Filter.SensitivityLevel", (string)"03 03 20 00 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Miscellaneous.HF.ISO_15693_ICode1.ErrorHandling.WeakCollision", (string)"03 03 20 07 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Miscellaneous.HF.ISO_18000_3M3.ErrorHandling.WeakCollision", (string)"03 03 20 07 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.TimeLimit", (string)"03 03 01 06 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.Buzzer.ActivationSources", (string)"03 03 02 13 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"DigitalIO.Signaler.Buzzer.ActivationTime", (string)"03 03 02 12 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.WLAN.AccessPoint.Channel", (string)"03 03 42 11 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.WLAN.AccessPoint.Enable_HTTPServer", (string)"03 03 42 10 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.WLAN.AccessPoint.SecurityMode", (string)"03 03 42 12 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.WLAN.Hostname.Enable", (string)"03 03 43 04 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.WLAN.Hostname.Length", (string)"03 03 33 00 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.WLAN.Hostname.Name", (string)"03 03 33 01 27 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.WLAN.IPv4.Enable_DHCP", (string)"03 03 43 04 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.WLAN.IPv4.GatewayAddress", (string)"03 03 43 06 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.WLAN.IPv4.IPAddress", (string)"03 03 42 00 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.WLAN.IPv4.SubnetMask", (string)"03 03 43 00 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.WLAN.NetworkType", (string)"03 03 42 13 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.WLAN.PortNumber", (string)"03 03 42 08 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.WLAN.Security.ServiceSetIdentifier.Length", (string)"03 03 44 01 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.WLAN.Security.ServiceSetIdentifier.SSID", (string)"03 03 44 02 26 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.WLAN.Security.WPA2.Key", (string)"03 03 35 01 63 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.WLAN.Security.WPA2.KeyLength", (string)"03 03 35 00 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.AFI", (string)"03 03 11 01 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.Data", (string)"03 03 11 00 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.EAS", (string)"03 03 11 00 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.InputEvents", (string)"03 03 11 01 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.Mode.ChangeAFI", (string)"03 03 11 02 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.Mode.ChangeEAS", (string)"03 03 11 02 01 06 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.Mode.ReadCompleteBank", (string)"03 03 11 02 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.Mode.SeekFunction", (string)"03 03 11 02 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.RSSI", (string)"03 03 11 01 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSelector.UID", (string)"03 03 11 00 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSource.BankNo", (string)"03 03 11 03 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSource.ByteOrderOfData", (string)"03 03 11 00 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSource.FirstDataBlock", (string)"03 03 11 04 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.DataSource.NoOfDataBlocks", (string)"03 03 11 08 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.Filter.TransponderValidTime", (string)"03 03 12 00 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.SeekFunction.Enable_Password", (string)"03 03 12 09 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.SeekFunction.MaskLength", (string)"03 03 11 12 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.SeekFunction.MaskStartAddress", (string)"03 03 11 11 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.SeekFunction.NewAFI", (string)"03 03 12 07 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.SeekFunction.NewEAS", (string)"03 03 12 08 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.BufferedReadMode.SeekFunction.Password", (string)"03 03 12 10 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Miscellaneous.TransponderIdentification.DataBlockNo", (string)"03 03 12 03 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Miscellaneous.TransponderIdentification.NoOfDataBlocks", (string)"03 03 12 05 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Miscellaneous.TransponderIdentification.Source", (string)"03 03 12 02 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Mode", (string)"03 03 01 13 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSelector.AFI", (string)"03 03 11 01 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSelector.Data", (string)"03 03 11 00 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSelector.EAS", (string)"03 03 11 00 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSelector.InputEvents", (string)"03 03 11 01 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSelector.Mode.ChangeAFI", (string)"03 03 11 02 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSelector.Mode.ChangeEAS", (string)"03 03 11 02 01 06 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSelector.Mode.ReadCompleteBank", (string)"03 03 11 02 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSelector.Mode.SeekFunction", (string)"03 03 11 02 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSelector.RSSI", (string)"03 03 11 01 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSelector.Selector", (string)"03 03 11 00 03 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSelector.UID", (string)"03 03 11 00 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.BankNo", (string)"03 03 11 03 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.ByteOrderOfData", (string)"03 03 11 00 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.FirstDataBlock", (string)"03 03 11 04 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.DataSource.NoOfDataBlocks", (string)"03 03 11 08 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Filter.TransponderValidTime", (string)"03 03 12 00 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.SeekFunction.Enable_Password", (string)"03 03 12 09 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.SeekFunction.MaskLength", (string)"03 03 11 12 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.SeekFunction.MaskStartAddress", (string)"03 03 11 11 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.SeekFunction.NewAFI", (string)"03 03 12 07 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.SeekFunction.NewEAS", (string)"03 03 12 08 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.SeekFunction.Password", (string)"03 03 12 10 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Transmission.Destination.IPv4.IPAddress", (string)"03 03 49 07 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Transmission.Destination.PortNumber", (string)"03 03 49 11 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Transmission.KeepAlive.Enable", (string)"03 03 49 04 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.NotificationMode.Transmission.KeepAlive.IntervalTime", (string)"03 03 49 05 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"PowerManagement.PowerOnTime", (string)"03 03 21 04 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"PowerManagement.RFOnMode", (string)"03 03 21 13 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"PowerManagement.RFOnTime_Max", (string)"03 03 21 00 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"PowerManagement.RFOnTime_Min", (string)"03 03 21 02 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Anticollision.Enable", (string)"03 03 05 11 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.Drivers", (string)"03 03 03 00 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_15693", (string)"03 03 03 01 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_18000_3M3", (string)"03 03 03 00 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.Miscellaneous.ReadOption.BlockSize", (string)"03 03 04 13 01 00 05"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.Miscellaneous.ReadOption.BlockSizeSelection", (string)"03 03 04 13 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.Miscellaneous.ReadOption.ReadMode", (string)"03 03 04 13 01 06 02"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.Miscellaneous.WriteOption", (string)"03 03 04 06 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.SelectionMask.AFI1", (string)"03 03 04 05 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.SelectionMask.AFI2", (string)"03 03 04 08 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.SelectionMask.AFI3", (string)"03 03 04 09 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.SelectionMask.AFI4", (string)"03 03 04 10 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_15693.SelectionMask.Enable_AFI", (string)"03 03 04 04 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_18000_3M3.Anticollision.Session", (string)"03 03 05 10 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_18000_3M3.SelectionMask.No1.Bank", (string)"03 03 22 01 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_18000_3M3.SelectionMask.No1.FirstBit", (string)"03 03 22 02 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_18000_3M3.SelectionMask.No1.Mask", (string)"03 03 22 04 24 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_18000_3M3.SelectionMask.No1.MaskLength", (string)"03 03 22 00 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Miscellaneous.IdentifierInterpretationMode", (string)"03 03 04 12 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Miscellaneous.TIDLength", (string)"03 03 04 11 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.PersistenceReset.Antenna.No1.PersistenceResetTime", (string)"03 03 16 02 02 00 00"));
}
