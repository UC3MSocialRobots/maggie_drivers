/**
 * \file MCDC3006SDriver.h
 *
 * \date 14/05/2010
 * \author Victor Gonzalez Pacheco (mailto: vgonzale@ing.uc3m.es)
 */

#ifndef MCDC3006SDRIVER_H_
#define MCDC3006SDRIVER_H_

/*
 *
 */

#include <mxml.h>			/* XML parsing library. Used to access to the xml Driver config files */
#include <sstream>
#include "definitions/ADerrors.h"
#include "driver_interface/DriverInterface.h"
#include "mcdc3006s/MCDC3006SMotorSensor.h"
#include "mcdc3006s/MCDC3006SStatus.h"
#include "mcdc3006s/MCDC3006SConfiguration.h"
#include "mcdc3006s/MCDC3006Slib/MCDC3006S.h"
#include "xml/mxmlUtils.h"


class MCDC3006SDriver : public DriverInterface {
public:
        MCDC3006SDriver(std::string driverConfigFile);
        MCDC3006SDriver ();
        virtual ~MCDC3006SDriver();

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


//	 // Status Related Functions (Setters) --> Not needed yet
//	virtual void isCurrentLimitated(bool curLim);
//	virtual void isEnabled(bool enabled);
//	virtual void isLimitSensorReached(bool limitSensorReached);
//	virtual void isOverTemperatureWarning(bool overTemperatureWarning);
//	virtual void isOverVoltageWarning(bool overVoltageWarning);
//	virtual void isCalibrated (bool calibrated);

        // Other Status Related Functions
        virtual int updateStatus();



        // Motor Sensor Functions
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
        virtual int setConfigFile(std::string confFile);

        // Specific configuration getters and setters (only used by MCDC3006S and not other drivers)
        string getSerialDevice ();
        int getBaudrate ();
        string getSemFile();

        void setSerialDevice(std::string serialDevice);
        void setBaudrate (int baud);
        void setSemFile(std::string semFile);

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
        virtual int writeConfigToFile(std::string filename);
        virtual int loadConfigFromFile(std::string filename);

        virtual void printLimits();
        virtual void printConfig();
        virtual void printMotorSensorInfo();
        virtual void printStatus();

private:
        MCDC3006SConfiguration * mcdc3006Config;
        MCDC3006SMotorSensor * mcdc3006MotorSens;
        MCDC3006SStatus * mcdc3006Status;

};

#endif /* MCDC3006SDRIVER_H_ */
