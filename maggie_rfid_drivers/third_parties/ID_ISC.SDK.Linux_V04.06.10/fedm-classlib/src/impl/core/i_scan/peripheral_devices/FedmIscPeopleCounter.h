/*-------------------------------------------------------
|                                                       |
|			FedmIscPeopleCounter.h						|
|                                                       |
---------------------------------------------------------

Copyright © 2010	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Manuel Sahm
Begin        		:	18.02.2010

Version       		:	03.02.00 / 15.06.2010 / M. Sahm, M. Hultsch

Operation Systems	:	independent

Function			:	this class supports the peripheral device called People Counter


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#if !defined(_FEDM_ISC_PEOPLE_COUNTER_H_INCLUDED_)
#define _FEDM_ISC_PEOPLE_COUNTER_H_INCLUDED_

#if !defined(_FEDM_NO_PD_SUPPORT)

#include "FedmIscPeripheralDevice.h"

//#####################################################################################
// type definitions
//#####################################################################################


//#####################################################################################
// constants
//#####################################################################################


//####################################################################
// class FedmIscPeopleCounter
//####################################################################

class _FEDM_ISC_CORE_EXT_CLASS FedmIscPeopleCounter : public FedmIscPeripheralDevice
{
public:
	// constructor
	FedmIscPeopleCounter(FEDM_ISCReader* pReader, unsigned char ucBusAdr);
	// destructor
	virtual ~FedmIscPeopleCounter();

	//main access functions
	int SetOutput(FEISC_GPC_OUTPUT* pOutput1, FEISC_GPC_OUTPUT* pOutput2, FEISC_GPC_OUTPUT* pOutput3);
	int GetCounter(unsigned int& uiRadar1_Cnt1, unsigned int& uiRadar1_Cnt2, unsigned int& uiRadar2_Cnt1, unsigned int& uiRadar2_Cnt2);
	int SetCounter(unsigned int uiRadar1_Cnt1, unsigned int uiRadar1_Cnt2, unsigned int uiRadar2_Cnt1, unsigned int uiRadar2_Cnt2);


protected:

};

#endif // #if !defined(_FEDM_NO_PD_SUPPORT)

#endif // !defined(_FEDM_ISC_PEOPLE_COUNTER_H_INCLUDED_)
