/**
 * DynamixelStatus.h
 *      Created on: 01/09/2010
 *      Author: Paulina Vélez Núñez (mailto: pvelez@ing.uc3m.es)
 */

#ifndef DINAMIXELSTATUS_H_
#define DINAMIXELSTATUS_H_

#include "driver_interface/DriverStatus.h"
#include "dynamixel/lib/dynamixel.h"

class DynamixelStatus : public DriverStatus {
	friend class DynamixelSDriver;
public:
	DynamixelStatus();
	virtual ~DynamixelStatus();
	int CommStatus;

private:
	//driverStatus_dynamixel statusData;				/** Struct used to store the status of the driver. **/


};

#endif /* DINAMIXELSTATUS_H_ */
