/*-------------------------------------------------------
|
|               FEDM_ISCReaderConfig_MAX_ONTOP.cpp
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


void FEDM_ISCReader::BuildAccessIdMap_MAX_ONTOP()
{
	m_mapAccessID.clear();

	m_mapAccessID.insert(make_pair((string)"AccessProtection.CryptoMode", (string)"03 03 01 04 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.TimeLimit", (string)"03 03 01 06 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.IPv4.Enable_DHCP", (string)"03 03 41 04 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.IPv4.GatewayAddress", (string)"03 03 41 06 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.IPv4.IPAddress", (string)"03 03 40 00 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.IPv4.SubnetMask", (string)"03 03 41 00 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.Keepalive.Enable", (string)"03 03 41 04 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.Keepalive.IntervalTime", (string)"03 03 41 12 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.Keepalive.RetransmissionCount", (string)"03 03 41 05 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.LAN.PortNumber", (string)"03 03 40 08 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.MultiDropBus.TerminalID", (string)"03 03 16 00 04 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Baudrate", (string)"03 03 01 02 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.BusAddress", (string)"03 03 01 00 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Databits", (string)"03 03 01 03 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Parity", (string)"03 03 01 03 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Stopbits", (string)"03 03 01 03 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.EMV", (string)"03 03 12 01 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.EMVco", (string)"03 03 12 00 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.HostMode.Filter.ResponseMode", (string)"03 03 05 11 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.HostMode.InventoryAbortCondition", (string)"03 03 05 11 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Payment.girogo.AutomaticAccountReconciliation.TransactionLimit", (string)"03 03 17 00 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Payment.girogo.ConfirmPayment", (string)"03 03 16 04 01 06 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Payment.girogo.CreditDisplay", (string)"03 03 16 04 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Payment.girogo.MerchantReferenceNo", (string)"03 03 17 02 06 00 00"));
	m_mapAccessID.insert(make_pair((string)"OperatingMode.Payment.girogo.PaymentAck", (string)"03 03 16 04 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Anticollision.Enable", (string)"03 03 05 11 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_14443.Drivers", (string)"03 03 03 02 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_14443.TypeA.MifareStandard", (string)"03 03 03 03 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_14443.TypeA.MifareUltraLight", (string)"03 03 03 03 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_14443.TypeA.Part4", (string)"03 03 03 03 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_14443.TypeA.my_d_Proximity_SLE55Rxx", (string)"03 03 03 03 01 01 01"));
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
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.TypeA.UID_Order", (string)"03 03 03 13 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.TypeB.SelectionMask.AFI", (string)"03 03 04 08 01 00 00"));
}
