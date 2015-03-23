#ifndef MCDC3006SDRIVER_H_
#define MCDC3006SDRIVER_H_

/**
 * @file        MCDC3006SDriver.h
 * @brief       Node for controlling the mcdc3006s motors (neck and arms).
 *
 * @author      Raul Perula-Martinez <raul.perula@uc3m.es>
 * @date        2015-03
 * @author      Victor Gonzalez Pacheco <vgonzale@ing.uc3m.es>
 * @date        2010-05
 *
 * @copyright   Copyright (C) 2015 University Carlos III of Madrid.
 *              All rights reserved.
 * @license     LEUC3M v1.0, see LICENSE.txt
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the Licencia Educativa UC3M as published by
 * the University Carlos III of Madrid, either version 1.0, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY. See the Licencia Educativa UC3M
 * version 1.0 or any later version for more details.
 *
 * A copy of the Licencia Educativa UC3M is in the LICENSE file.
 */

#include <mxml.h>   /* XML parsing library. Used to access to the xml Driver config files */
#include <sstream>
#include <ros/package.h>
#include "definitions/ADerrors.h"
#include "driver_interface/DriverInterface.h"
#include "mcdc3006s/MCDC3006SMotorSensor.h"
#include "mcdc3006s/MCDC3006SStatus.h"
#include "mcdc3006s/MCDC3006SConfiguration.h"
#include "mcdc3006s/MCDC3006Slib/MCDC3006S.h"
#include "xml/mxmlUtils.h"

class MCDC3006SDriver : public DriverInterface {
    public:
        /**
         * @brief Empty constructor.
         */
        MCDC3006SDriver();

        /**
         * @brief Parametrized constructor.
         */
        MCDC3006SDriver(std::string driverConfigFile);

        /**
         * @brief Destructor.
         */
        virtual ~MCDC3006SDriver();

        /**
         * @brief Moves an MCDC3006S driver to a relative position.
         * @param relativePos the relative position to move the driver (in pulses).
         * @warning Deprectated function. Used only to provide backwards compatibility.
         * @TODO error control here
         */
        virtual int moveDriverToRelativePosition(long int relativePos);

        /**
         * @brief Moves an MCDC3006S driver to an absolute position.
         * @param absolutePos the absolute position to move the driver (in pulses).
         * @warning Deprectated function. Used only to provide backwards compatibility.
         * @return ERR_COM in case of error
         * @return ERR_NOERR otherwise
         */
        virtual int moveDriverToAbsolutePosition(long int absolutePos);

        /**
         * @brief Moves an MCDC3006S driver by a velocity command.
         * @param vel the velocity to move the driver (in rpm).
         * @return ERR_COM in case of error, ERR_NOERR otherwise
         */
        virtual int moveDriverByVelocity(long int vel);

        /**
         * @brief Calibrate Driver.
         * @param calibrationSensorPosition is the position (in pulses) of the calibration sensor measured from the
         *           desired Home position (0). For instance, If we want to have the 0 in the sensor, this parameter
         *           will be 0.
         * @param calibrationCurrentLimit max current the motor is not allowed to exceed (in mA) This limit is
         *          directly related to torque of the motor.
         * @param calibrationSpeed The speed of the motor during the calibration (in rpm).
         * @param calibrationTimeOut maximum time to perform the calibration. If this time is exceeded the function
         *          will return an error.
         *
         * @return ERR_COM in case of error, ERR_NOERR otherwise
         */
        virtual int calibrate(long int calibrationSensorPosition,
                              long int calibrationCurrentLimit,
                              long int calibrationSpeed,
                              long int calibrationTimeOut);

        /**
         * @brief If the parameter entered ( calibrated) is true, this function calibrates the driver.
         *        If it is false, the function sets the MCDC3006SStatus::calibrated to false to indicate the driver
         *        must be calibrated.
         * @param calibrated the boolean to set the calibration flag to true or false.
         * @warning If calibrated is false, this function does not calibrate the driver. It only sets the
         *          MCDC3006SStatus::calibrated flag to false.
         * @see MCDC3006SDriver::calibrate(long int calibrationSensorPosition, long int calibrationCurrentLimit,
         *      long int calibrationSpeed, long int calibrationTimeOut)
         * @return ERR_COM in case of error, ERR_NOERR otherwise
         **/
        virtual int calibrate(bool calibrated);

        // Enable and disable the driver

        /**
         * @brief Enables the driver.
         * @return ERR_COM in case of error, ERR_NOERR otherwise
         */
        virtual int enable();

        /**
         * @brief Disables the driver
         * @return ERR_COM in case of error, ERR_NOERR otherwise
         */
        virtual int disable();

        // Status Related Functions (Getters)

        /**
         * @brief Asks the driver if the current is being limitated.
         * @return MCDC3006SStatus::currentLimitated True: the current is being limited.
         *
         * @warning This function calls the driver using the function updateStatus() If you don't really need to call
         *          the driver it's better to use DriverInterface::isCurrentLimitated()
         * @see updateStatus()
         * @see DriverInterface.h @see DriverInterface.cpp
         * @TODO Error control here
         */
        virtual bool isCurrentLimitated();

        /**
         * @brief Asks if the driver is enabled or disabled. True = driver enabled. False = driver disabled.
         * @return MCDC3006SStatus::enabled True if it is enabled.
         * @warning This function calls the driver using the function updateStatus() If you don't really need to call
         *          the driver it's better to use DriverInterface::isEnabled()
         * @see updateStatus()
         * @see DriverInterface.h @see DriverInterface.cpp
         * @TODO Error control here
         */
        virtual bool isEnabled();

        /**
         * @brief Asks the driver if the limit sensor is has been reached. True the sensor has been reached.
         * @return MCDC3006SStatus::limitSensorReached.. True: the limit sensor is producing an output. Thus the limit
         *         sensor has been reached.
         * @warning This function calls the driver using the function updateStatus() If you don't really need to call
         *          the driver it's better to use DriverInterface::isLimitSensorReached()
         * @see updateStatus()
         * @see DriverInterface::isLimitSensorReached()
         * @see DriverInterface.h @see DriverInterface.cpp
         * @TODO Error control here
         */
        virtual bool isLimitSensorReached();

        /**
         * @brief Asks the driver if the overTemperatureWarning has been triggered (there is an over Temperature error
         *        in the driver). True = overTemperatureWarning.
         * @return drvStatus->overTemperatureWarning. True: warning.
         *
         * @warning This function calls the driver using the function updateStatus() If you don't really need to call
         *          the driver it's better to use DriverInterface::isOverTemperatureWarning()
         * @see updateStatus()
         * @see DriverInterface::isOverTemperatureWarning()
         * @see DriverInterface.h @see DriverInterface.cpp
         * @TODO Error control here
         */
        virtual bool isOverTemperatureWarning();

        /**
         * @brief Checks if the overTemperatureWarning has been triggered (there is an overvoltage error in the
         *        driver). True = overVoltageWarning.
         * @return MCDC3006SStatus::overVoltageWarning. True: warning.
         *
         * @warning This function calls the driver using the function updateStatus() If you don't really need to call
         *          the driver it's better to use DriverInterface::isOverVoltageWarning()
         * @see updateStatus()
         * @see DriverInterface::isOverVoltageWarning
         * @see DriverInterface.h @see DriverInterface.cpp
         *
         * @TODO Error control here
         */
        virtual bool isOverVoltageWarning();

        /**
         * @brief checks if the the calibration flag is calibrated or not.
         * @return MCDC3006SStatus::calibrated The calibration flag. true = the driver is calibrated. false = the
         *         driver is not calibrated
         *
         * @warning This function calls the driver using the function updateStatus() If you don't really need to call
         *          the driver it's better to use DriverInterface::isCalibrated()
         * @see updateStatus()
         * @see DriverInterface::isCalibrated
         * @see DriverInterface.h @see DriverInterface.cpp
         */
        virtual bool isCalibrated();

        // Other Status Related Functions

        /**
         * @brief Reads the status of the driver and stores it in a MCDC3006SStatus object
         * @see MCDC3006SStatus
         * @return ERR_COM in case of error
         * @return ERR_NOERR otherwise
         */
        virtual int updateStatus();

        // Motor Sensor Functions

        /**
         * @brief returns  the last position stored in the Motor Sensor
         * @return MCDC3006S::position the last stored position in the MCDC3006SMotorSensor::position
         * @warning this position may not be the current position of the driver. If you want to retrieve the actual
         *          position of the driver use MCDC3006SDriver::updatePosition()
         */
        virtual long int getPosition() const;

        /**
         * @brief returns  the last velocity stored in the Motor Sensor
         * @return MCDC3006S::velocity the last stored velocity in the MCDC3006SMotorSensor::velocity
         * @warning this velocity may not be the current velocity of the driver. If you want to retrieve the actual
         *          velocity of the driver use MCDC3006SDriver::updateVelocity()
         */
        virtual long int getVelocity() const;

        /**
         * @brief returns  the last Instant Current data stored in the Motor Sensor
         * @return MCDC3006S::instantCurrent the last stored Instant Current in the MCDC3006SMotorSensor::instantCurrent
         * @warning this instant Current may not be the current instantCurrent of the driver. If you want to retrieve
         *          the actual Instant Current of the driver use MCDC3006SDriver::updateInstantCurrent()
         */
        virtual int getInstantCurrent() const;

        // Motor Sensor Update Functions. Used to retrieve the Motor Sensor data from the drivers.

        /**
         * @brief Reads from the driver an update of the current position and stores it in
         *        MCDC3006SDriver::mcdc3006MotorSens ( MCDC3006SMotorSensor::position )
         * @note if you want to access the last Motor Sensor data without consulting the driver (although you can
         *       retrieve old Motor Sensor data) you can use DriverInterface::getPosition()
         * @return ERR_COM in case of error
         * @return ERR_NOERR otherwise
         */
        virtual int updatePosition();

        /**
         * @brief Reads from the driver an update of the current velocity and stores it in
         *        MCDC3006SDriver::mcdc3006MotorSens ( MCDC3006SMotorSensor::velocity )
         * @note if you want to access the last Motor Sensor data without consulting the driver (although you can
         *       retrieve old Motor Sensor data) you can use DriverInterface::getVelocity()
         * @return ERR_COM in case of error
         * @return ERR_NOERR otherwise
         */
        virtual int updateVelocity();

        /**
         * @brief Reads from the driver an update of the instant current and stores it in
         *        MCDC3006SDriver::mcdc3006MotorSens ( MCDC3006SMotorSensor::instantCurrent )
         * @note if you want to access the last Motor Sensor data without consulting the driver (although you can
         *       retrieve old Motor Sensor data) you can use DriverInterface::getInstantCurrent()
         * @return ERR_COM in case of error
         * @return ERR_NOERR otherwise
         */
        virtual int updateInstantCurrent();

        /**
         * @brief Reads from the driver an update of all the Motor Sensor data and stores it in
         *       MCDC3006SDriver::mcdc3006MotorSens
         * @return ERR_COM in case of error
         * @return ERR_NOERR otherwise
         */
        virtual int updateMotorSensorInfo();

        /**
         * @brief function to write the configuration stored in mcdc3006SConfig to the driver
         * @return ERR_NOERR if success
         * @return ERR_CONF if any error occurs
         * @TODO error control
         */
        virtual int writeConfigToDriver();

        /**
         * @brief function that loads the configuration from the driver and stores it in mcdc3006Config
         * @return ERR_NOERR if success
         * @return ERR_CONF if any error occurs
         * @note{ <b>Note for maintainers:</b> If you want to add more features you also have add this features to
         *              MCDC3006SDriver::writeConfigToDriver() }
         *
         * @TODO the function itself
         */
        virtual int loadConfigFromDriver();

        /**
         * @brief function that loads the configuration from mcdc3006Config and stores it in an xml file
         * @param filename filename of the xml file where to store the configuration
         * @return ERR_NOERR if success
         * @return ERR_FILE_ACCESS
         * @return ERR_CONF if any error occurs
         * @note{ <b>Note for maintainers:</b> If you want to add more features you also have add this features to
         *              MCDC3006SDriver::loadConfigFromDriver() }
         * @warning { This function saves to a file the driver configuration stored in the program memory. Before you use
         *                  this function you may want to load the configuration from the driver to the program memory }
         * @see MCDC3006SDriver::writeConfigToDriver()
         * @see MCDC3006SDriver::loadConfigFromDriver()
         */
        virtual int writeConfigToFile(std::string filename);

        /**
         * @brief function that loads the configuration from a file and stores it in mcdc3006Config
         * @param filename the file where to get the configuration
         * @return ERR_NOERR if success
         * @return ERR_FILE_ACCESS if couldn't access to the file
         * @return ERR_CONF if any error occurs
         * @warning {This function loads the configuration from the file into the program memory.
         *              This function DOES NOT write the loaded configuration into the driver. If you want to store the loaded
         *                  configuration in the driver you must call MCDC3006SDriver::writeConfigToDriver() after loading the
         *                  configuration from the file}
         * @see MCDC3006SDriver::writeConfigToDriver()
         */
        virtual int loadConfigFromFile(std::string filename);

        // Configuration Access Functions (Getters)

        /**
         * @brief Retrieves from the driver the MaxPosition parameter
         * @return MCDC3006SConfiguration::maxPosition Units: pulses
         * @see MCDC3006SConfiguration.h
         * @TODO error control
         */
        virtual long int getMaxPosition();

        /**
         * @brief Retrieves from the driver the MinPosition parameter
         * @return MCDC3006SConfiguration::minPosition Units: pulses
         * @see MCDC3006SConfiguration.h
         * @TODO error control
         */
        virtual long int getMinPosition();

        /**
         * @brief Retrieves from the driver the MaxVelocity parameter
         * @return MCDC3006SConfiguration::maxVelocity Units: rpm
         * @see MCDC3006SConfiguration.h
         * @TODO error control
         */
        virtual long int getMaxVelocity();

        /**
         * @brief Retrieves from the driver the MaxAcceleration parameter
         * @return MCDC3006SConfiguration::maxAcceleration Units:
         * @see MCDC3006SConfiguration.h
         * @TODO error control
         */
        virtual long int getMaxAcceleration();

        /**
         * @brief Retrieves from the driver the MaxDecceleration
         * @return MCDC3006SConfiguration::maxDecceleration Units:
         * @see MCDC3006SConfiguration.h
         * @TODO error control
         */
        virtual long int getMaxDecceleration();

        /**
         * @brief Retrieves from the driver the ContinuousCurrentLimit parameter
         * @return MCDC3006SConfiguration::continousCurrentLimit Units: mA
         * @see MCDC3006SConfiguration.h
         * @TODO error control
         */
        virtual int getContinousCurrentLimit();

        /**
         * @brief Retrieves from the driver the MaxPosition parameter
         * @return MCDC3006SConfiguration::peakCurrentLimit Units: mA
         * @see MCDC3006SConfiguration.h
         * @TODO error control
         */
        virtual int getPeakCurrentLimit();

        /**
         * @brief Returns configuration filename stored in the configuration.
         * @return MCDC3006SConfiguration::configFile The configuration filename
         * @see MCDC3006SConfiguration.h
         */
        string getConfigFile();

        // Configuration Access Functions (Setters)

        /**
         * @brief Sets the driver maxPosition and if succeeds updates the driverConfiguration::maxPosition with the
         *        entered parameter
         * @param maxPosition The value to set the maximum position in the driver. Units: pulses
         * @return ERR_CONF in case of failing to set the parameter into the driver
         * @return ERR_NOERR otherwise
         */
        virtual int setMaxPosition(long int maxPosition);

        /**
         * @brief Sets the driver minPosition and if succeeds updates the driverConfiguration::minPosition with the
         *        entered parameter
         * @param minPosition The value to set the minimum position in the driver. Units: pulses
         * @return ERR_CONF in case of failing to set the parameter into the driver
         * @return ERR_NOERR otherwise
         */
        virtual int setMinPosition(long int minPosition);

        /**
         * @brief Sets the driver maxVelocity and if succeeds updates the driverConfiguration::maxVelocity with the
         *        entered parameter
         * @param maxVelocity The value to set the maximum velocity in the driver. Units: rpm
         * @return ERR_CONF in case of failing to set the parameter into the driver
         * @return ERR_NOERR otherwise
         */
        virtual int setMaxVelocity(long int maxVelocity);

        /**
         * @brief Sets the driver maxAcceleration and if succeeds updates the driverConfiguration::maxAcceleration with
         *        the entered parameter
         * @param maxAcceleration The value to set the maximum acceleration in the driver. Units:
         * @return ERR_CONF in case of failing to set the parameter into the driver
         * @return ERR_NOERR otherwise
         */
        virtual int setMaxAcceleration(long int maxAcceleration);

        /**
         * @brief Sets the driver maxDecceleration and if succeeds updates the driverConfiguration::maxDecceleration
         *        with the entered parameter
         * @param maxDecceleration The value to set the maximum decceleration in the driver. Units:
         * @return ERR_CONF in case of failing to set the parameter into the driver
         * @return ERR_NOERR otherwise
         */
        virtual int setMaxDecceleration(long int maxDecceleration);

        /**
         * @brief Sets the driver continousCurrentLimit and if succeeds updates the
         *        DriverConfiguration::continousCurrentLimit with the entered parameter
         * @param continousCurrentLimit The value to set the continous current limit in the driver. Units: mA
         * @return ERR_CONF in case of failing to set the parameter into the driver
         * @return ERR_NOERR otherwise
         */
        virtual int setContinousCurrentLimit(int continousCurrentLimit);

        /**
         * @brief Sets the driver peakCurrentLimit and if succeeds updates the DriverConfiguration::PeakCurrentLimit
         *        with the entered parameter
         * @param peakCurrentLimit The value to set the peak current Limit in the driver. Units: mA
         * @return ERR_CONF in case of failing to set the parameter into the driver
         * @return ERR_NOERR otherwise
         */
        virtual int setPeakCurrentLimit(int peakCurrentLimit);

        /**
         * @brief sets the configuration file
         * @param confFile the cofiguration file name
         * @TODO error control
         */
        virtual int setConfigFile(std::string confFile);

        // Specific configuration getters and setters (only used by MCDC3006S and not other drivers)

        /**
         * @brief gets the serial device filename
         * @return The file name of the serial port where the driver is attached
         */
        string getSerialDevice();

        /**
         * @brief Returns the baudrate of the serial port at which the driver is connected
         * @return the baudrate of the serial port at which the driver is connected
         */
        int getBaudrate();

        /**
         * @brief gets the filename used by the driver's semaphore to generate the unique identificator of the driver's
         *        semaphore
         * @return the semaphore file name
         */
        string getSemFile();

        /**
         * @brief sets the filename of the serial Port at wich the driver will connect
         * @param serialDevice the filename of the serial port
         * @note if the entered filename is longer than SP_MAX_FILENAME_SIZE (the maximum serial port filename size) the
         *              entered string will be truncated
         */
        void setSerialDevice(std::string serialDevice);

        /**
         * @brief sets the baudrate of the serial port of the driver
         * @param baud the baudrate
         * @TODO error control here: The baudrate must be a real baudrate supported by the serial port
         */
        void setBaudrate(int baud);

        /**
         * @brief sets the filename of the semaphore file which will be used by the driver
         * @param semFile the filename of the semaphore
         * @note if the entered filename is longer than MAX_SEMFILENAME_SIZE (the maximum semaphore filename size) the
         *              entered string will be truncated
         */
        void setSemFile(std::string semFile);

        // Other Configuration Access functions

        /*
         * @brief Function to open and to start the communication with the driver.
         * @return ERR_FILE_ACCESS in case of failing the communication with the driver. It also sets
         *         MCDC3006SDriver::mcdc3006Status (MCDC3006SStatus::connected) to false
         * @return ERR_NOERR otherwise. It also sets MCDC3006SStatus::connected to true
         */
        virtual int connect();

        /*
         * Function to close the driver and end the communication with it
         */
        virtual int disconnect();

        /**
         * @brief Prints in stdout all the limits from DriverConfiguration
         */
        virtual void printLimits();

        /**
         * @brief Prints in stdout all the non-limits configuration from DriverConfiguration
         */
        virtual void printConfig();

        /**
         * @brief Prints in stdout all the Motor Sensor data from DriverMotorSensor
         */
        virtual void printMotorSensorInfo();

        /**
         * @brief Prints in stdout all the status data from DriverStatus
         */
        virtual void printStatus();

    private:
        MCDC3006SConfiguration *mcdc3006Config;
        MCDC3006SMotorSensor *mcdc3006MotorSens;
        MCDC3006SStatus *mcdc3006Status;
};

#endif
