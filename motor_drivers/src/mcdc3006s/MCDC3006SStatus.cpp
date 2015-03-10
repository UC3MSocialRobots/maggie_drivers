/*
 * \file MCDC3006SStatus.cpp
 *
 *  \date 17/05/2010
 *  \author Victor Gonzalez Pacheco (mailto: vgonzale@ing.uc3m.es)
 */

#include "mcdc3006s/MCDC3006SStatus.h"

/**
 * Default constructor.
 */
MCDC3006SStatus::MCDC3006SStatus() : DriverStatus() {
	connected = false;
	enabled = false;
	calibrated = false;
	currentLimitated = false;
	overTemperatureWarning = false;
	overVoltageWarning = false;
	limitSensorReached = false;
}

MCDC3006SStatus::~MCDC3006SStatus() {
	// TODO Auto-generated destructor stub
}

