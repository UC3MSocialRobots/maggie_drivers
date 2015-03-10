/**
 * DynamixelStatus.cpp
 *      Created on: 01/09/2010
 *      Author: Paulina Vélez Núñez (mailto: pvelez@ing.uc3m.es)
 */

#include "DynamixelStatus.h"
//#include "dynamixel.h"

/**
 * Default constructor.
 */
DynamixelStatus::DynamixelStatus() : DriverStatus() {

	connected = false;
	enabled = false;
	calibrated = false;
	currentLimitated = false;
	overTemperatureWarning = false;
	overVoltageWarning = false;
	limitSensorReached = false;


}

DynamixelStatus::~DynamixelStatus() {
	// TODO Auto-generated destructor stub
}

