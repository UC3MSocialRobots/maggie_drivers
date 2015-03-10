/*-------------------------------------------------------
|
|               FEDM_ISCReaderConfig_CPR20.cpp
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

Function			:	implementation of Access-ID map for OBID® classic-pro reader ID CPR20.xx


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#include "../FEDM_ISCReader.h"


void FEDM_ISCReader::BuildAccessIdMap_CPR20()
{
	m_mapAccessID.clear();

	m_mapAccessID.insert(make_pair((string)"AirInterface.Antenna.HF.Miscellaneous.MinimumLevel", (string)"03 03 03 06 01 00 04"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.Antenna.HF.Miscellaneous.MinimumLevelMode", (string)"03 03 03 06 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"AirInterface.TimeLimit", (string)"03 03 01 06 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Baudrate", (string)"03 03 01 02 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.BusAddress", (string)"03 03 01 00 01 00 00"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Databits", (string)"03 03 01 03 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Parity", (string)"03 03 01 03 01 00 02"));
	m_mapAccessID.insert(make_pair((string)"HostInterface.Serial.Stopbits", (string)"03 03 01 03 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.Drivers", (string)"03 03 03 00 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_14443.Drivers", (string)"03 03 03 02 02 00 00"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_14443.TypeA.MifareStandard", (string)"03 03 03 03 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_14443.TypeA.MifareUltraLight", (string)"03 03 03 03 01 02 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_14443.TypeA.Part4", (string)"03 03 03 03 01 03 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_14443_TypeA", (string)"03 03 03 01 01 04 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.Driver.HF.ISO_14443_TypeB", (string)"03 03 03 01 01 05 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.Part4.Miscellaneous.ErrorRetry", (string)"03 03 03 13 01 02 02"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.Part4.Miscellaneous.PowerLevelCheck", (string)"03 03 03 13 01 01 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.TypeA.UID_Order", (string)"03 03 03 13 01 07 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.TypeB.Miscellaneous.BitrateSelect", (string)"03 03 03 13 01 00 01"));
	m_mapAccessID.insert(make_pair((string)"Transponder.HF.ISO_14443.TypeB.SelectionMask.AFI", (string)"03 03 04 08 01 00 00"));
}
