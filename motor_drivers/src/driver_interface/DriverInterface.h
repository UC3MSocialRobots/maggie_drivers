/**
 * \file DriverInterface.h
 *
 * \date 13/05/2010
 * \author Victor Gonzalez (mailto:vgonzale@ing.uc3m.es)
 */

#ifndef DRIVERINTERFACE_H_
#define DRIVERINTERFACE_H_

#define PI 3.14159265

#include <string>
#include <iostream>
#include "driver_interface/DriverMotorSensor.h"
#include "driver_interface/DriverStatus.h"
#include "driver_interface/DriverConfiguration.h"
#include "definitions/ADerrors.h"

using namespace std;

/*
 * Abstract class that is used as a base for all the specific drivers of the system.
 */
class DriverInterface {
    //	friend class actuator::Actuator;
public:
    DriverInterface(); // Default constructor.
    virtual ~DriverInterface();

    // Pure virtual funcions. Derivate classes must implement this ones.
    /**
         * @brief Calibrate Driver. Each derived class must implement it with the necessary methods to calibrate a specific driver.
         * @param calibrationSensorPosition is the position (in pulses) of the calibration sensor measured from the desired Home position (0). For instance, If we want
         * 				to have the 0 in the sensor, this parameter will be 0.
         * @param calibrationCurrentLimit max current the motor is not allowed to exceed (in mA) This limit is directly related to torque of the motor.
         * @param calibrationSpeed The speed of the motor during the calibration (in rpm)
         * @param calibrationTimeOut maximum time to perform the calibration. If this time is exceeded the function will return an error
         *
         * @return error control. (0 means no errors)
         * */
    virtual int calibrate(long int calibrationSensorPosition, long int calibrationCurrentLimit, long int calibrationSpeed, long int calibrationTimeOut) =0;

    /**
         * @brief Calibrate Driver. Each derived class must implement it with the necessary methods to calibrate a specific driver.
         * @param calibrate flag to set calibration to true or false. If the calibration flag is set to true, the calibration process must be called. If the calibration flag is set to false, the driver must update its calibration flag to false
         *
         * @return error control. (0 means no errors)
         * */
    virtual int calibrate (bool calibrate) =0;

    /**
         * Sends to the driver a movement by velocity command. Each derived class must be implemented with the necessary methods to send a move by velocity a specific driver.
         * @param vel the velocity at which the motor should move. The param MUST BE entered in the motor units.
         * @return error control. (0 means no errors)
         * */
    virtual int moveDriverByVelocity(long int vel) =0;

    /**
         * Sends to the driver a command to move to an absolute position the motor.
         * Each derived class must be implemented with the necessary methods to send a move by velocity a specific driver.
         * \param absolutePos the position at which the motor should move. The param MUST BE entered in the motor units.
         * \return error control. (0 means no errors)
         *
         * \warning DEPRECATED function
         * */
    virtual int moveDriverToAbsolutePosition(long int absolutePos) =0;

    /**
         * Sends to the driver a command to move to a relative position the motor.
         * Each derived class must be implemented with the necessary methods to send a move by velocity a specific driver.
         *
         * @param relativePos the position at which the motor should move. The param MUST BE entered in rad. The developer is in charge to transform this unit to the units used by the driver.
         * @return error control. (0 means no errors)
         *
         * @warning DEPRECATED function
         * */
    virtual int moveDriverToRelativePosition(long int relativePos) =0;

    /**
         * Enables the driver
         * @return ERR_NOERR if everything works correctly
         */
    virtual int enable() =0;

    /**
         * Disnables the driver
         * @return ERR_NOERR if everything works correctly
         */
    virtual int disable() =0;

    // Status Related Functions (Getters)
    virtual bool isCurrentLimitated() const;
    virtual bool isEnabled() const;
    virtual bool isLimitSensorReached() const;
    virtual bool isOverTemperatureWarning() const;
    virtual bool isOverVoltageWarning() const;
    virtual bool isCalibrated() const;
    virtual bool isConnected () const;

    // Status Related Functions (Setters)
    virtual int isCurrentLimitated(bool curLim);
    virtual int isEnabled(bool enabled);
    virtual int isLimitSensorReached(bool limitSensorReached);
    virtual int isOverTemperatureWarning(bool overTemperatureWarning);
    virtual int isOverVoltageWarning(bool overVoltageWarning);
    virtual int isCalibrated (bool calibrated);
    virtual int isConnected (bool open);

    // Other Status Related Functions
    virtual int updateStatus() =0;

    // Motor Sensor Access Functions.
    virtual long int getPosition() const;
    virtual long int getVelocity() const;
    virtual int getInstantCurrent() const;

    // Motor Sensor Update Functions. Used to retrieve the Motor Sensor data from the drivers.
    virtual int updatePosition() =0;
    virtual int updateVelocity() =0;
    virtual int updateInstantCurrent() =0;
    virtual int updateMotorSensorInfo() =0;


    // Configuration Access Functions (Getters)
    virtual long int getMaxPosition() const;
    virtual long int getMinPosition() const;
    virtual long int getMaxVelocity() const;
    virtual long int getMaxAcceleration() const;
    virtual long int getMaxDecceleration() const;
    virtual int getContinousCurrentLimit() const;
    virtual int getPeakCurrentLimit() const;
    string getConfigFile() const;
    int getDriverID() const ;
    string getDriverName() const ;
    string getDriverType() const ;

    // Configuration Access Functions (Setters)
    virtual int setMaxPosition(long int maxPosition);
    virtual int setMinPosition(long int minPosition);
    virtual int setMaxVelocity(long int maxVelocity);
    virtual int setMaxAcceleration(long int maxAcceleration);
    virtual int setMaxDecceleration(long int maxDecceleration);
    virtual int setContinousCurrentLimit(int continousCurrentLimit);
    virtual int setPeakCurrentLimit(int peakCurrentLimit);
    virtual int setConfigFile(std::string confFile);
    int setDriverID(int driverID) ;
    int setDriverName(std::string driverName) ;
    int setDriverType(std::string driverType) ;

    long int getEncoderPulsesPerRevolution () const;
    int getDriverMultiplicationFactor () const;
    int getReductionFactor() const;

    void setEncoderPulsesPerRevolution(long int pulsesPerRevolution);
    void setDriverMultiplicationFactor (int driverMultiplicationFactor);
    void setReductionFactor(int reductionFactor);

    //	virtual double getPulsesPerRad() const;
    virtual long int getPulsesPerRevolution() const;
    double pulsesToRad (long int pulses);
    long int radToPulses (double rad);

    // Other Configuration Access functions

    /*
         * Function to open and to start the communication with the driver
         */
    virtual int connect() =0;
    /*
         * Function to close the driver and end the communication with it
         */
    virtual int disconnect() =0;

    /**
         * Writes the configuration from DriverConfiguration inherited object
         * and stores it in the driver
         */
    virtual int writeConfigToDriver()=0;
    /**
         * Loads the configuration from the driver and stores in a DriverConfiguration inherited object
         */
    virtual int loadConfigFromDriver()=0;

    /**
         * Writes the configuration stored in a DriverConfiguration inherited object to a file
         * \param filename the path to the file
         */
    virtual int writeConfigToFile(std::string filename)=0;
    /**
         * Loads the configuration from a file into a DriverConfiguration object
         */
    virtual int loadConfigFromFile(std::string filename)=0;

    virtual void printLimits() const;
    virtual void printConfiguration() const;
    virtual void printMotorSensor() const;
    virtual void printStatus() const;

    virtual void setCalibrationCurrentLimit(int cLimit);
    virtual void setCalibrationSensorPosition(long int sensorPosition);
    virtual void setCalibrationSpeed(long int calibrationSpeed);
    virtual void setCalibrationTimeOut(int timeOut);

protected:

    DriverMotorSensor * drvMotorSensor;
    DriverConfiguration * drvConf;
    DriverStatus * drvStatus;

    string boolToString (bool b) const;

};

#endif /* DRIVERINTERFACE_H_ */
