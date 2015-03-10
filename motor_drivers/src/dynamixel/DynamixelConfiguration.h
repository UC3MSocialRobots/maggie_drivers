/**
 * DynamixelConfiguration.h
 *      Created on: 01/09/2010
 *      Author: Paulina Vélez Núñez (mailto: pvelez@ing.uc3m.es)
 */

#ifndef DYNAMIXELCONFIGURATION_H_
#define DYNAMIXELCONFIGURATION_H_


#include "driver_interface/DriverConfiguration.h"
#include "dynamixel/lib/dynamixel.h"
#include <iostream>
#include <string>


class DynamixelConfiguration : public DriverConfiguration {
	friend class DynamixelDriver;
public:
	DynamixelConfiguration();
	DynamixelConfiguration(string confFile);		/** Constructor in which the configuration file is passed */
	virtual ~DynamixelConfiguration();

	int getID();
	float getMaxVoltage();
	float getMinVoltage();
	int getMaxTemperature();
	int getMinTemperature();

	void setID(int id);
	void setMaxVoltage(float maxVoltage);
	void setMinVoltage (float minVoltage);
	void setMaxTemperature(int maxTemperature);
	void setMinTemperature(int minTemperature);

private:
	string namePort;
	int id;
	float minVoltage;
	float maxVoltage;
	int minTemperature;
	int maxTemperature;

	static const string defaultConfigFile;

	// Getters
	static const int defaultID= 1;					// 1 is the default ID for devices
	static const float defaultMinVoltage = 7;  		// 7 Volts is the minimum voltage operating
	static const float defaultMaxVoltage = 12.1;  	// 12.1 Volts is the minimum voltage operating
	static const int defaultMinTemperature = -5;	// -5 ºC is the minimum operation temperature
	static const int defaultMaxTemperature = 85;	// 85 ºC is the maximum operation temperature
};

#endif /* DYNAMIXELCONFIGURATION_H_ */
