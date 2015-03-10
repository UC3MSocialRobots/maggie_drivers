/**
 * DynamixelDriver.h
 *      Created on: 01/09/2010
 *      Author: Paulina Vélez Núñez (mailto: pvelez@ing.uc3m.es)
 */

#ifndef DYNAMIXELDRIVER_H_
#define DYNAMIXELDRIVER_H_

#include <sstream>
#include <string>

#include "driver_interface/DriverInterface.h"

#include "dynamixel/DynamixelMotorSensor.h"
#include "dynamixel/DynamixelStatus.h"
#include "dynamixel/DynamixelConfiguration.h"

#include "dynamixel/lib/DatosDynamixel.h"
#include "dynamixel/lib/dynamixel.h"

class DynamixelDriver : public DriverInterface {
  
  public:
	DynamixelDriver(string driverConfigFile);
	DynamixelDriver ();
	virtual ~DynamixelDriver();

		virtual int moveDriverToRelativePosition(long int relativePos);
		virtual int moveDriverToAbsolutePosition(long int absolutePos);
		virtual int moveDriverByVelocity(long int vel);
		virtual int calibrate(long int calibrationSensorPosition, long int calibrationCurrentLimit, long int calibrationSpeed, long int calibrationTimeOut);
		virtual int calibrate (bool calibrated);

		// Enable and disable the driver
		virtual int enable();
		virtual int disable();

		// Status Related Functions (Getters)
		virtual bool isCurrentLimitated();
		virtual bool isEnabled();
		virtual bool isLimitSensorReached();
		virtual bool isOverTemperatureWarning();
		virtual bool isOverVoltageWarning();
		virtual bool isCalibrated();


	//	Status Related Functions (Setters) --> Not needed yet
	//	virtual void isCurrentLimitated(bool curLim);
	//	virtual void isEnabled(bool enabled);
	//	virtual void isLimitSensorReached(bool limitSensorReached);
	//	virtual void isOverTemperatureWarning(bool overTemperatureWarning);
	//	virtual void isOverVoltageWarning(bool overVoltageWarning);
	//	virtual void isCalibrated (bool calibrated);

		// Other Status Related Functions
		virtual int updateStatus();

		// Motor Sensor Functions
		//int getDeviceID() const;
		virtual long int getPosition() const;
		virtual long int getVelocity() const;
		virtual int getInstantCurrent() const;
		// Motor Sensor Update Functions. Used to retrieve the Motor Sensor data from the drivers.
		virtual int updatePosition();
		virtual int updateVelocity();
		virtual int updateInstantCurrent() ;
		virtual int updateMotorSensorInfo();


		// Configuration Access Functions (Getters)
		virtual long int getMaxPosition();
		virtual long int getMinPosition();
		virtual long int getMaxVelocity();
		virtual long int getMaxAcceleration();
		virtual long int getMaxDecceleration();
		virtual int getContinousCurrentLimit();
		virtual int getPeakCurrentLimit();
		string getConfigFile();

		// Configuration Access Functions (Setters)
		virtual int setMaxPosition(long int maxPosition);
		virtual int setMinPosition(long int minPosition);
		virtual int setMaxVelocity(long int maxVelocity);
		virtual int setMaxAcceleration(long int maxAcceleration);
		virtual int setMaxDecceleration(long int maxDecceleration);
		virtual int setContinousCurrentLimit(int continousCurrentLimit);
		virtual int setPeakCurrentLimit(int peakCurrentLimit);
		virtual int setConfigFile (string confFile);

		// Other Configuration Access functions
		/*
		 * Function to start the communication with the driver
		 */
		virtual int connect();
		/*
		 * Function to close the driver and end the communication with it
		 */
		virtual int disconnect();

		virtual int writeConfigToDriver();
		virtual int loadConfigFromDriver();
		virtual int writeConfigToFile(string filename);
		virtual int loadConfigFromFile(string filename);

		virtual void printLimits();
		virtual void printConfig();
		virtual void printMotorSensorInfo();
		virtual void printStatus();

		//only for dynamixel
		 float getVoltage();
		 int getTemperature();
		 int getLoad();
		 int updateVoltage();
		 int updateTemperature();
		 int updateLoad();


private:
	  DynamixelConfiguration *dynamixelConfig;
      DynamixelMotorSensor *dynamixelMotorSens;
      DynamixelStatus *dynamixelStatus;
      static const int MAX_PORTNAME_SIZE;
      static const int MAX_ALLOWED_VELOCITY;



};

#endif /* DYNAMIXELDRIVER_H_ */

