/*-------------------------------------------------------
|                                                       |
|			FedmIscExternalIO.h							|
|                                                       |
---------------------------------------------------------

Copyright © 2009-2010	FEIG ELECTRONIC GmbH, All Rights Reserved.
						Lange Strasse 4
						D-35781 Weilburg
						Federal Republic of Germany
						phone    : +49 6471 31090
						fax      : +49 6471 310999
						e-mail   : obid-support@feig.de
						Internet : http://www.feig.de
					
Author         		:	Manuel Sahm
Begin        		:	18.02.2010

Version       		:	03.02.00 / 18.05.2010 / M. Sahm

Operation Systems	:	independent

Function			:	this class supports the peripheral device called External IO


Trademarks:
-----------
OBID®, OBID i-scan® and OBID myAXXESS® are registered Trademarks of FEIG ELECTRONIC GmbH
Other Trademarks: see FEDM.h
*/


#if !defined(_FEDM_ISC_EXTERNAL_IO_H_INCLUDED_)
#define _FEDM_ISC_EXTERNAL_IO_H_INCLUDED_

#if !defined(_FEDM_NO_PD_SUPPORT)

#include "FedmIscPeripheralDevice.h"

//#####################################################################################
// constants
//#####################################################################################


//####################################################################
// class FedmIscExternalIO
//####################################################################

class _FEDM_ISC_CORE_EXT_CLASS FedmIscExternalIO : public FedmIscPeripheralDevice
{
public:
	// constructor
	FedmIscExternalIO(FEDM_ISCReader* pReader, unsigned char ucBusAdr);
	// destructor
	virtual ~FedmIscExternalIO();

	//main access functions

	int SetOutput( FEISC_GPC_OUTPUT* pOutput1, FEISC_GPC_OUTPUT* pOutput2, FEISC_GPC_OUTPUT* pOutput3);

protected:

};

#endif // #if !defined(_FEDM_NO_PD_SUPPORT)

#endif // !defined(_FEDM_ISC_EXTERNAL_IO_H_INCLUDED_)
