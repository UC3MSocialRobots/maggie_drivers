#ifndef DRIVERINTERFACE_H_
#define DRIVERINTERFACE_H_

/**
 * \file DriverInterface.h
 *
 * \date 13/05/2010
 * \author Victor Gonzalez (mailto:vgonzale@ing.uc3m.es)
 */

#include <string>
#include <iostream>
#include "driver_interface/DriverMotorSensor.h"
#include "driver_interface/DriverStatus.h"
#include "driver_interface/DriverConfiguration.h"
#include "definitions/ADerrors.h"

#define PI 3.14159265

using namespace std;

class DriverInterface {
    public:
        /**
         * @brief Empty constructor.
         */
        DriverInterface();

        /**
         * @brief Destructor.
         */
        virtual ~DriverInterface();

        // Pure virtual funcions. Derivate classes must implement this ones.

        /**
         * @brief Calibrate Driver. Each derived class must implement it with the necessary methods to calibrate a
         *        specific driver.
         * @param calibrationSensorPosition is the position (in pulses) of the calibration sensor measured from the
         *        desired Home position (0). For instance, If we want to have the 0 in the sensor, this parameter
         *        will be 0.
         * @param calibrationCurrentLimit max current the motor is not allowed to exceed (in mA) This limit is
         *        directly related to torque of the motor.
         * @param calibrationSpeed The speed of the motor during the calibration (in rpm)
         * @param calibrationTimeOut maximum time to perform the calibration. If this time is exceeded the function
         *        will return an error
         * @return error control. (0 means no errors)
         */
        virtual int calibrate(long int calibrationSensorPosition,
                              long int calibrationCurrentLimit,
                              long int calibrationSpeed,
                              long int calibrationTimeOut) = 0;

        /**
         * @brief Calibrate Driver. Each derived class must implement it with the necessary methods to calibrate a
         *        specific driver.
         * @param calibrate flag to set calibration to true or false. If the calibration flag is set to true, the
         *        calibration process must be called. If the calibration flag is set to false, the driver must update
         *        its calibration flag to false
         * @return error control. (0 means no errors)
         */
        virtual int calibrate(bool calibrate) = 0;

        /**
         * @brief Sends to the driver a movement by velocity command. Each derived class must be implemented with the
         *        necessary methods to send a move by velocity a specific driver.
         * @param vel the velocity at which the motor should move. The param MUST BE entered in the motor units.
         * @return error control. (0 means no errors)
         */
        virtual int moveDriverByVelocity(long int vel) = 0;

        /**
         * @brief Sends to the driver a command to move to an absolute position the motor. Each derived class must be
         *        implemented with the necessary methods to send a move by velocity a specific driver.
         * @param absolutePos the position at which the motor should move. The param MUST BE entered in the motor units.
         * @return error control. (0 means no errors)
         *
         * @warning DEPRECATED function
         */
        virtual int moveDriverToAbsolutePosition(long int absolutePos) = 0;

        /**
         * @brief Sends to the driver a command to move to a relative position the motor. Each derived class must be
         *        implemented with the necessary methods to send a move by velocity a specific driver.
         * @param relativePos the position at which the motor should move. The param MUST BE entered in rad. The
         *        developer is in charge to transform this unit to the units used by the driver.
         * @return error control. (0 means no errors)
         * @warning DEPRECATED function
         */
        virtual int moveDriverToRelativePosition(long int relativePos) = 0;

        /**
         * @brief Enables the driver
         * @return ERR_NOERR if everything works correctly
         */
        virtual int enable() = 0;

        /**
         * @brief Disables the driver
         * @return ERR_NOERR if everything works correctly
         */
        virtual int disable() = 0;

        // Status Related Functions (Getters)

        /**
         * @brief Checks if the current is being limitated.
         * @return DriverSatus::currentLimitated. True: the current is being limited.
         */
        virtual bool isCurrentLimitated() const;

        /**
         * @brief Checks if the driver is enabled or disabled. True = driver enabled. False = driver disabled.
         * @return DriverSatus::enabled. True if it is enabled.
         */
        virtual bool isEnabled() const;

        /**
         * @brief Checks if the limit sensor has been reached. True: the sensor has been reached. False: the
         *        limit sensor has not been reached (yet :-)
         * @return DriverSatus::limitSensorReached True: the limit sensor is producing an output. Thus the limit
         *         sensor has been reached.
         */
        virtual bool isLimitSensorReached() const;

        /**
         * @brief Checks if the overTemperatureWarning has been triggered (there is an over Temperature error in the
         *        driver). True = overTemperatureWarning.
         * @return DriverSatus::overTemperatureWarning True: warning.
         */
        virtual bool isOverTemperatureWarning() const;

        /**
         * @brief Checks if the overTemperatureWarning has been triggered (there is an overvoltage error in the
         *        driver). True = overVoltageWarning.
         * @return DriverSatus::overVoltageWarning True: warning.
         */
        virtual bool isOverVoltageWarning() const;

        /**
         * @brief checks if the the calibration flag is calibrated or not.
         * @return DriverSatus::calibrated The calibration flag. true = the driver is calibrated. false = the driver
         *         is not calibrated
         */
        virtual bool isCalibrated() const;

        /**
         * @brief returns the open flag
         * @return DriverSatus::open The open flag. true = the driver is open. false = the driver is closed
         * @note open means that the driver there is an open active communication channel between the computer and
         *       the driver
         */
        virtual bool isConnected() const;

        // Status Related Functions (Setters)

        /**
         * @brief Sets the DriverSatus::currentLimitated to the value entered in the parameter
         * @param curLim The value to store in DriverSatus::currentLimitated
         */
        virtual int isCurrentLimitated(bool curLim);

        /**
         * @brief Sets the DriverSatus::enabled to the value entered in the parameter enabled. True = device is
         *        enabled. False = device is disabled.
         * @param enabled The value to store in DriverSatus::enabled
         */
        virtual int isEnabled(bool enabled);

        /**
         * @brief Sets the DriverSatus::limitSensorReached to true or false depending if the Limit sensor has been
         *        reached (true) or not (false)
         * @param limitSensorReached The value to store in DriverSatus::limitSensorReached
         */
        virtual int isLimitSensorReached(bool limitSensorReached);

        /**
         * @brief Sets the DriverSatus::overTemperatureWarning to true or false depending on the status provided by
         *        the driver.
         * @param overTemperatureWarning The value to store in DriverSatus::overTemperatureWarning
         */
        virtual int isOverTemperatureWarning(bool overTemperatureWarning);

        /**
         * @brief Sets the DriverSatus::overVoltageWarning to the value entered in the parameter
         * @param overVoltageWarning The value to store in DriverSatus::overVoltageWarning
         */
        virtual int isOverVoltageWarning(bool overVoltageWarning);

        /**
         * @brief sets the calibration flag to the value of the entered parameter
         * @param calibrated The value to set the calibration flag. true = the driver is calibrated. false = the driver
         *        is not calibrated. The value is stored in DriverSatus::calibrated
         */
        virtual int isCalibrated(bool calibrated);

        /**
         * @brief sets the open flag to the value of the entered parameter
         * @param connected the value to set the open flag. true = The driver is open. false = the driver is closed
         * @note open means that the driver there is an open active communication channel between the computer and
         *       the driver
         */
        virtual int isConnected(bool open);

        // Other Status Related Functions

        /**
         * @brief
         */
        virtual int updateStatus() = 0;

        // Motor Sensor Access Functions

        /**
         * @brief Returns the position stored in the Motor Sensor.
         * @return DriverMotorSensor::position. Units: pulses
         * @warning this position may not be the current position of the driver. If you want to retrieve the actual
         *          position of the driver use DriverInterface::updatePosition()
         * @see DriverMotorSensor.h
         */
        virtual long int getPosition() const;

        /**
         * @brief Returns the velocity stored in the Motor Sensor.
         * @return DriverMotorSensor::velocity. Units: rpms
         * @warning this velocity may not be the current velocity of the driver. If you want to retrieve the actual
         *          velocity of the driver use DriverInterface::updateVelocity()
         * @see DriverMotorSensor.h
         */
        virtual long int getVelocity() const;

        /**
         * @brief Returns the instantaneous current stored in the Motor Sensor.
         * @return DriverMotorSensor::instantCurrent. Units: milliAmperes
         * @warning this instant Current may not be the current instantCurrent of the driver. If you want to retrieve
         *          the actual Instant Current of the driver use DriverInterface::updateInstantCurrent()
         * @see DriverMotorSensor.h
         */
        virtual int getInstantCurrent() const;

        // Motor Sensor Update Functions. Used to retrieve the Motor Sensor data from the drivers.

        /**
         * @brief
         */
        virtual int updatePosition() = 0;

        /**
         * @brief
         */
        virtual int updateVelocity() = 0;

        /**
         * @brief
         */
        virtual int updateInstantCurrent() = 0;

        /**
         * @brief
         */
        virtual int updateMotorSensorInfo() = 0;

        // Configuration Access Functions (Getters)

        /**
         * @brief Returns Max Position stored in the configuration.
         * @return DriverConfiguration::maxPosition. Units: pulses
         * @see DriverConfiguration.h
         */
        virtual long int getMaxPosition() const;

        /**
         * @brief Returns Min Position stored in the configuration.
         * @return DriverConfiguration::minPosition. Units: pulses
         * @see DriverConfiguration.h
         */
        virtual long int getMinPosition() const;

        /**
         * @brief Returns Max Velocity stored in the configuration.
         * @return DriverConfiguration::maxVelocity. Units: rpm
         * @see DriverConfiguration.h
         */
        virtual long int getMaxVelocity() const;

        /**
         * @brief Returns Max Acceleration stored in the configuration.
         * @return DriverConfiguration::maxAcceleration. Units:
         * @see DriverConfiguration.h
         */
        virtual long int getMaxAcceleration() const;

        /**
         * @brief Returns Max Decceleration stored in the configuration.
         * @return DriverConfiguration::maxDecceleration. Units:
         * @see DriverConfiguration.h
         */
        virtual long int getMaxDecceleration() const;

        /**
         * @brief Returns the Continuous Current Limit stored in the configuration.
         * @return DriverConfiguration::continousCurrentLimit. Units: milli amperes
         * @see DriverConfiguration.h
         */
        virtual int getContinousCurrentLimit() const;

        /**
         * @brief Returns the Peak Current Limit stored in the configuration.
         * @return DriverConfiguration::peakCurrentLimit. Units: milliAmperes
         * @see DriverConfiguration.h
         */
        virtual int getPeakCurrentLimit() const;

        /**
         * @brief Returns configuration filename stored in the configuration.
         * @return DriverConfiguration::configFile The configuration filename
         * @see DriverConfiguration.h
         */
        string getConfigFile() const;

        /**
         * @brief returns the Unique identificator of the driver
         * @return DriverConfiguration::driverID the identificator of the driver
         * @see DriverConfiguration
         */
        int getDriverID() const;

        /**
         * @brief returns the Driver Name
         * @return DriverConfiguration::driverName the name of the driver
         * @see DriverConfiguration
         */
        string getDriverName() const;

        /**
         * @brief returns the driver type
         * @return DriverConfiguration::driverType the type of the driver
         * @see DriverConfiguration
         */
        string getDriverType() const;

        // Configuration Access Functions (Setters)

        /**
         * @brief Sets the DriverConfiguration::maxPosition in the configuration data area.
         * @param maxPosition The value to set DriverConfiguration::maxPosition
         * @TODO error control
         */
        virtual int setMaxPosition(long int maxPosition);

        /**
         * @brief Sets the DriverConfiguration::minPosition in the configuration data area.
         * @param minPosition The value to set DriverConfiguration::minPosition
         * @TODO error control
         */
        virtual int setMinPosition(long int minPosition);

        /**
         * @brief Sets the DriverConfiguration::maxVelocity in the configuration data area.
         * @param maxVelocity The value to set DriverConfiguration::maxVelocity
         * @TODO error control
         */
        virtual int setMaxVelocity(long int maxVelocity);

        /**
         * @brief Sets the DriverConfiguration::maxAcceleration in the configuration data area.
         * @param maxAcceleration The value to set DriverConfiguration::maxAcceleration
         * @TODO error control
         */
        virtual int setMaxAcceleration(long int maxAcceleration);

        /**
         * @brief Sets the DriverConfiguration::maxDecceleration in the configuration data area.
         * @param maxDecceleration The value to set DriverConfiguration::maxDecceleration
         * @TODO error control
         */
        virtual int setMaxDecceleration(long int maxDecceleration);

        /**
         * @brief Sets the DriverConfiguration::continousCurrentLimit in the configuration data area.
         * @param continousCurrentLimit The value to set DriverConfiguration::continousCurrentLimit
         * @TODO error control
         */
        virtual int setContinousCurrentLimit(int continousCurrentLimit);

        /**
         * @brief Sets the DriverConfiguration::peakCurrentLimit in the configuration data area.
         * @param peakCurrentLimit The value to set DriverConfiguration::peakCurrentLimit
         * @TODO error control
         */
        virtual int setPeakCurrentLimit(int peakCurrentLimit);

        /**
         * @brief stores the driver configuration filename in DriverConfiguration::configFile
         * @param confFile the filename of the configuration filename. Is stored in DriverConfiguration::configFile
         * @TODO Error control here. Check the confFile is accessible and readable.
         */
        virtual int setConfigFile(std::string confFile);

        /**
         * @brief sets an ID to the driver
         * @param driverID the identificator to set to the driver
         * @TODO error control: Make sure the identificator is unique
         */
        int setDriverID(int driverID);

        /**
         * @brief sets the driver Name
         * @param driverName the name of the driver
         * @TODO error control
         */
        int setDriverName(std::string driverName);

        /**
         * @brief sets the type of the driver
         * @param driverType the type of the driver. A string defining the name of the type of the driver
         * @TODO error control
         */
        int setDriverType(std::string driverType);

        /**
         * @brief Returns ratio between pulses and rad (the number of pulses per 1 rad) of the driver stored in the
         *        DriverConfiguration area
         * @return DriverConfiguration::pulsesPerRad
         */
        //  virtual double getPulsesPerRad() const;
        /**
         * @brief Returns the number of pulses per revolution of the driver
         * @return DriverConfiguration::pulsesPerRevolution
         */
        virtual long int getPulsesPerRevolution() const;

        /**
         * @brief Transforms the given number of pulses to rads
         * @param pulses the number of pulses to transform
         * @return the pulses transformed to radians (rad)
         * @returns 0 in case of DriverInterface::pulsesToRad is 0 (to avoid a division per 0 error)
         */
        double pulsesToRad(long int pulses);

        /**
         * @brief Trasnforms from radians to pulses
         * @param rad the radians to be transformed to pulses
         * @return the radians transformed to pulses
         */
        long int radToPulses(double rad);

        /**
         * @brief Returns the encoder of pulses per revolution
         * @return DriverConfiguration::encoderPulsesPerRevolution the number of pulses per revolution of the motor's
         *         encoder
         */
        long int getEncoderPulsesPerRevolution() const;

        /**
         * @brief Returns the multiplication factor of the driver. This multiplication factor is a multiplier added
         *        to the DriverConfiguration::encoderPulsesPerRevolution used to count the number of pulses per
         *        revolution as seen by the driver
         * @return DriverConfiguration::driverMultiplicationFactor the multiplication factor of the driver
         */
        int getDriverMultiplicationFactor() const;

        /**
         * @brief Returns reduction factor of the motor of the driver. If the motor uses a gear, the value of the gear
         *        should be multiplied to the DriverConfiguration::driverMultiplicationFactor
         * @return DriverConfiguration::reductionFactor the reduction factor of the motor's driver. Units: --
         */
        int getReductionFactor() const;

        /**
         * @brief sets the encoder of pulses per revolution
         * @param pulsesPerRevolution the number of pulses per revolution of the motor's encoder
         */
        void setEncoderPulsesPerRevolution(long int pulsesPerRevolution);

        /**
         * @brief Sets the multiplication factor of the driver. This multiplication factor is a multiplier added
         *        to the DriverConfiguration::encoderPulsesPerRevolution used to
         * count the number of pulses per revoulution as seen by the driver
         * @param driverMultiplicationFactor the multiplication factor of the driver
         */
        void setDriverMultiplicationFactor(int driverMultiplicationFactor);

        /**
         * @brief Sets the reduction factor of the driver's motor. If the motor uses a gear, the value of the gear
         *        should be multiplied to the DriverConfiguration::driverMultiplicationFactor
         * @param reductionFactor the reduction factor of the motor's driver. Units: --
         */
        void setReductionFactor(int reductionFactor);

        // Other Configuration Access functions

        /*
         * @brief Function to open and to start the communication with the driver
         */
        virtual int connect() = 0;

        /*
         * @brief Function to close the driver and end the communication with it
         */
        virtual int disconnect() = 0;

        /**
         * @brief Writes the configuration from DriverConfiguration inherited object
         * and stores it in the driver
         */
        virtual int writeConfigToDriver() = 0;

        /**
         * @brief Loads the configuration from the driver and stores in a DriverConfiguration inherited object
         */
        virtual int loadConfigFromDriver() = 0;

        /**
         * @brief Writes the configuration stored in a DriverConfiguration inherited object to a file
         * @param filename the path to the file
         */
        virtual int writeConfigToFile(std::string filename) = 0;

        /**
         * @brief Loads the configuration from a file into a DriverConfiguration object
         */
        virtual int loadConfigFromFile(std::string filename) = 0;

        /**
         * @brief Prints in stdout all the limits from DriverConfiguration
         */
        virtual void printLimits() const;

        /**
         * @brief Prints in stdout all the non-limits configuration from DriverConfiguration
         */
        virtual void printConfiguration() const;

        /**
         * @brief Prints in stdout all the Motor Sensor data from DriverMotorSensor
         */
        virtual void printMotorSensor() const;

        /**
         * @brief Prints in stdout all the status data from DriverStatus
         */
        virtual void printStatus() const;

        /**
         * @brief Sets the DriverConfiguration::calibrationCurrentLimit (the maximum current delivered to the motor
         *        during the calibration process
         * @param cLimit the currentLimit during the calibration process (units mA)
         */
        virtual void setCalibrationCurrentLimit(int cLimit);

        /**
         * @brief Sets the DriverConfiguration::calibrationSensorPosition to the entered value
         * @param sensorPosition the position at which sensor position is located mesured from the 0 position
         *        (units pulses)
         */
        virtual void setCalibrationSensorPosition(long int sensorPosition);

        /**
         * @brief Sets the DriverConfiguration::calibrationSpeed (units:rpm)
         * @param calibrationSpeed the maximum speed at which the calibration must be carried out (units rpm)
         */
        virtual void setCalibrationSpeed(long int calibrationSpeed);

        /**
         * @brief Sets the DriverConfiguration::calibrationTimeOut (units ms)
         * @param timeOut maxium time in which the calibration must be performed (units ms)
         */
        virtual void setCalibrationTimeOut(int timeOut);

    protected:
        DriverMotorSensor * drvMotorSensor;
        DriverConfiguration * drvConf;
        DriverStatus * drvStatus;

        /**
         * @brief Function to convert a bool to a string
         * @param b the boolean to convert
         * @return "true" if the boolean is true, "false" if the boolean is false
         */
        string boolToString(bool b) const;
};

#endif
