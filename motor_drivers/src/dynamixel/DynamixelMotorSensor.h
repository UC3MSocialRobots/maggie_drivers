/**
 * DynamixelMotorSensor.h
 *      Created on: 01/09/2010
 *      Author: Paulina Vélez Núñez (mailto: pvelez@ing.uc3m.es)
 */

#ifndef DYNAMIXELMOTORSENSOR_H_  //MCDC3006SMOTORSENSOR_H_
#define DYNAMIXELMOTORSENSOR_H_  //MCDC3006SMOTORSENSOR_H_

/*
 *
 */
#include "driver_interface/DriverMotorSensor.h"
#include "dynamixel/lib/dynamixel.h"
#include "dynamixel/lib/DatosDynamixel.h"
#include <iostream>

class DynamixelMotorSensor: public DriverMotorSensor {
	friend class DynamixelDriver;
public:
	DynamixelMotorSensor();

	virtual ~DynamixelMotorSensor();

	int getLoad();
	int getLoadDirection();
	float getVoltage();
	int getTemperature();
	void setLoad(int load);
	void setLoadDirection(int loadDirection);
	void setVoltage(float voltage);
	void setTemperature(int temperature);

protected:
	int load;
	int loadDirection;
	float voltage;
	int temperature;

};
#endif /* DYNAMIXELMOTORSENSOR_H_ */
