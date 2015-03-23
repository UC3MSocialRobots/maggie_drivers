/**
 * \file DriverInterface.cpp
 *
 * \date 13/05/2010
 * \author Victor Gonzalez Pacheco (mailto: vgonzale@ing.uc3m.es)
 */

#include "driver_interface/DriverInterface.h"

//////////////////////////////////////////////////

/**
 * Default constructor.
 */
DriverInterface::DriverInterface()
{
    drvConf = 0;
    drvMotorSensor = 0;
    drvStatus = 0;
}

//////////////////////////////////////////////////

DriverInterface::~DriverInterface()
{
    //	delete drvConf;
    //	delete drvMotorSensor;
    //	delete drvStatus;
}

//////////////////////////////////////////////////

// Status Section -------------------------------------------------------------------------------------------------------------------------------------------------
// Status Getters
/**
 * @brief Checks if the current is being limitated.
 * @return DriverSatus::currentLimitated. True: the current is being limited.
 */
bool DriverInterface::isCurrentLimitated() const
{
    return drvStatus->currentLimitated;
}

//////////////////////////////////////////////////

/**
 * @brief Checks if the driver is enabled or disabled. True = driver enabled. False = driver disabled.
 * @return DriverSatus::enabled. True if it is enabled.
 */
bool DriverInterface::isEnabled() const
{
    return drvStatus->enabled;
}

//////////////////////////////////////////////////

/**
 * @brief Checks if the limit sensor has been reached. True: the sensor has been reached. False: the limit sensor has not been reached (yet :-)
 * @return DriverSatus::limitSensorReached True: the limit sensor is producing an output. Thus the limit sensor has been reached.
 */
bool DriverInterface::isLimitSensorReached() const
{
    return drvStatus->limitSensorReached;
}

//////////////////////////////////////////////////

/**
 * @brief Checks if the overTemperatureWarning has been triggered (there is an over Temperature error in the driver). True = overTemperatureWarning.
 * @return DriverSatus::overTemperatureWarning True: warning.
 */
bool DriverInterface::isOverTemperatureWarning() const
{
    return drvStatus->overTemperatureWarning;
}

//////////////////////////////////////////////////

/**
 * @brief Checks if the overTemperatureWarning has been triggered (there is an overvoltage error in the driver). True = overVoltageWarning.
 * @return DriverSatus::overVoltageWarning True: warning.
 */
bool DriverInterface::isOverVoltageWarning() const
{
    return drvStatus->overVoltageWarning;
}

//////////////////////////////////////////////////

/**
 * @brief checks if the the calibration flag is calibrated or not.
 * @return DriverSatus::calibrated The calibration flag. true = the driver is calibrated. false = the driver is not calibrated
 */
bool DriverInterface::isCalibrated() const
{
    return drvStatus->calibrated;
}

//////////////////////////////////////////////////

/**
 * @brief returns the open flag
 * @return DriverSatus::open The open flag. true = the driver is open. false = the driver is closed
 * \note open means that the driver there is an open active communication channel between the computer and the driver
 */
bool DriverInterface::isConnected() const
{
    return drvStatus->connected;
}

//////////////////////////////////////////////////

// Status Setters
/**
 * @brief Sets the DriverSatus::currentLimitated to the value entered in the parameter
 * @param curLim The value to store in DriverSatus::currentLimitated
 */
int DriverInterface::isCurrentLimitated(bool curLim)
{
    drvStatus->currentLimitated = curLim;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

/**
 * @brief Sets the DriverSatus::enabled to the value entered in the parameter enabled. True = device is enabled. False = device is disabled.
 * @param enabled The value to store in DriverSatus::enabled
 */
int DriverInterface::isEnabled(bool enabled)
{
    drvStatus->enabled = enabled;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

/**
 * @brief Sets the DriverSatus::limitSensorReached to true or false depending if the Limit sensor has been reached (true) or not (false)
 * @param limitSensorReached The value to store in DriverSatus::limitSensorReached
 */
int DriverInterface::isLimitSensorReached(bool limitSensorReached)
{
    drvStatus->limitSensorReached = limitSensorReached;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

/**
 * @brief Sets the DriverSatus::overTemperatureWarning to true or false depending on the status provided by the driver.
 * @param overTemperatureWarning The value to store in DriverSatus::overTemperatureWarning
 */
int DriverInterface::isOverTemperatureWarning(bool overTemperatureWarning)
{
    drvStatus->overTemperatureWarning = overTemperatureWarning;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

/**
 * @brief Sets the DriverSatus::overVoltageWarning to the value entered in the parameter
 * @param overVoltageWarning The value to store in DriverSatus::overVoltageWarning
 */
int DriverInterface::isOverVoltageWarning(bool overVoltageWarning)
{
    drvStatus->overVoltageWarning = overVoltageWarning;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

/**
 * @brief sets the calibration flag to the value of the entered parameter
 * @param calibrated The value to set the calibration flag. true = the driver is calibrated. false = the driver is not calibrated. The value is stored in DriverSatus::calibrated
 */
int DriverInterface::isCalibrated(bool calibrated)
{
    drvStatus->calibrated = calibrated;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

/**
 * \brief sets the open flag to the value of the entered parameter
 * \param connected the value to set the open flag. true = The driver is open. false = the driver is closed
 * \note open means that the driver there is an open active communication channel between the computer and the driver
 */
int DriverInterface::isConnected(bool connected)
{
    drvStatus->connected = connected;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

// Motor Sensor Section -------------------------------------------------------------------------------------------------------------------------------------------------
// Motor Sensor Access Functions
/**
 * @brief Returns the position stored in the Motor Sensor.
 * @return DriverMotorSensor::position. Units: pulses
 *  \warning this position may not be the current position of the driver. If you want to retrieve the actual position of the driver use DriverInterface::updatePosition()
 * \see DriverMotorSensor.h
 */
long int DriverInterface::getPosition() const
{
    return drvMotorSensor->position;
}

//////////////////////////////////////////////////

/**
 * @brief Returns the velocity stored in the Motor Sensor.
 * @return DriverMotorSensor::velocity. Units: rpms
 * \warning this velocity may not be the current velocity of the driver. If you want to retrieve the actual velocity of the driver use DriverInterface::updateVelocity()
 * \see DriverMotorSensor.h
 */
long int DriverInterface::getVelocity() const
{
    return drvMotorSensor->velocity;
}

//////////////////////////////////////////////////

/**
 * @brief Returns the instantaneous current stored in the Motor Sensor.
 * @return DriverMotorSensor::instantCurrent. Units: milliAmperes
 *  \warning this instant Current may not be the current instantCurrent of the driver. If you want to retrieve the actual Instant Current of the driver use DriverInterface::updateInstantCurrent()
 * \see DriverMotorSensor.h
 */
int DriverInterface::getInstantCurrent() const
{
    return drvMotorSensor->instantCurrent;
}

//////////////////////////////////////////////////

// Configuration Section -------------------------------------------------------------------------------------------------------------------------------------------------
// Configuration Getters
/**
 * @brief Returns Max Position stored in the configuration.
 * @return DriverConfiguration::maxPosition. Units: pulses
 * \see DriverConfiguration.h
 */
long int DriverInterface::getMaxPosition() const
{
    return drvConf->maxPosition;
}

//////////////////////////////////////////////////

/**
 * @brief Returns Min Position stored in the configuration.
 * @return DriverConfiguration::minPosition. Units: pulses
 * \see DriverConfiguration.h
 */
long int DriverInterface::getMinPosition() const
{
    return drvConf->minPosition;
}

//////////////////////////////////////////////////

/**
 * @brief Returns Max Velocity stored in the configuration.
 * @return DriverConfiguration::maxVelocity. Units: rpm
 * \see DriverConfiguration.h
 */
long int DriverInterface::getMaxVelocity() const
{
    return drvConf->getMaxVelocity();
}

//////////////////////////////////////////////////

/**
 * @brief Returns Max Acceleration stored in the configuration.
 * @return DriverConfiguration::maxAcceleration. Units:
 * \see DriverConfiguration.h
 */
long int DriverInterface::getMaxAcceleration() const
{
    return drvConf->getMaxAcceleration();
}

//////////////////////////////////////////////////

/**
 * @brief Returns Max Decceleration stored in the configuration.
 * @return DriverConfiguration::maxDecceleration. Units:
 * \see DriverConfiguration.h
 */
long int DriverInterface::getMaxDecceleration() const
{
    return drvConf->getMaxDecceleration();
}

//////////////////////////////////////////////////

/**
 * @brief Returns the Continuous Current Limit stored in the configuration.
 * @return DriverConfiguration::continousCurrentLimit. Units: milli amperes
 * \see DriverConfiguration.h
 */
int DriverInterface::getContinousCurrentLimit() const
{
    return drvConf->getContinousCurrentLimit();
}

//////////////////////////////////////////////////

/**
 * @brief Returns the Peak Current Limit stored in the configuration.
 * @return DriverConfiguration::peakCurrentLimit. Units: milliAmperes
 * \see DriverConfiguration.h
 */
int DriverInterface::getPeakCurrentLimit() const
{
    return drvConf->getPeakCurrentLimit();
}

//////////////////////////////////////////////////

/**
 * @brief Returns configuration filename stored in the configuration.
 * @return DriverConfiguration::configFile The configuration filename
 * \see DriverConfiguration.h
 */
string DriverInterface::getConfigFile() const
{
    return drvConf->configFile;
}

//////////////////////////////////////////////////

/**
 * \brief returns the Unique identificator of the driver
 * \return DriverConfiguration::driverID the identificator of the driver
 * \see DriverConfiguration
 */
int DriverInterface::getDriverID() const
{
    return drvConf->driverID;
}

//////////////////////////////////////////////////

/**
 * \brief returns the Driver Name
 * \return DriverConfiguration::driverName the name of the driver
 * \see DriverConfiguration
 */
string DriverInterface::getDriverName() const
{
    return drvConf->driverName;
}

//////////////////////////////////////////////////

/**
 * \brief returns the driver type
 * \return DriverConfiguration::driverType the type of the driver
 * \see DriverConfiguration
 */
string DriverInterface::getDriverType() const
{
    return drvConf->driverType;
}

//////////////////////////////////////////////////

// Configuration Setters
/**
 * @brief Sets the DriverConfiguration::maxPosition in the configuration data area.
 * @param maxPosition The value to set DriverConfiguration::maxPosition
 * \todo error control
 */
int DriverInterface::setMaxPosition(long int maxPosition)
{
    drvConf->maxPosition = maxPosition;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

/**
 * @brief Sets the DriverConfiguration::minPosition in the configuration data area.
 * @param minPosition The value to set DriverConfiguration::minPosition
 * \todo error control
 */
int DriverInterface::setMinPosition(long int minPosition)
{
    drvConf->minPosition = minPosition;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

/**
 * @brief Sets the DriverConfiguration::maxVelocity in the configuration data area.
 * @param maxVelocity The value to set DriverConfiguration::maxVelocity
 * \todo error control
 */
int DriverInterface::setMaxVelocity(long int maxVelocity)
{
    drvConf->maxVelocity = maxVelocity;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

/**
 * @brief Sets the DriverConfiguration::maxAcceleration in the configuration data area.
 * @param maxAcceleration The value to set DriverConfiguration::maxAcceleration
 * \todo error control
 */
int DriverInterface::setMaxAcceleration(long int maxAcceleration)
{
    drvConf->maxAcceleration = maxAcceleration;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

/**
 * @brief Sets the DriverConfiguration::maxDecceleration in the configuration data area.
 * @param maxDecceleration The value to set DriverConfiguration::maxDecceleration
 * \todo error control
 */
int DriverInterface::setMaxDecceleration(long int maxDecceleration)
{
    drvConf->maxDecceleration = maxDecceleration;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

/**
 * @brief Sets the DriverConfiguration::continousCurrentLimit in the configuration data area.
 * @param continousCurrentLimit The value to set DriverConfiguration::continousCurrentLimit
 * \todo error control
 */
int DriverInterface::setContinousCurrentLimit(int continousCurrentLimit)
{
    drvConf->continousCurrentLimit = continousCurrentLimit;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

/**
 * @brief Sets the DriverConfiguration::peakCurrentLimit in the configuration data area.
 * @param peakCurrentLimit The value to set DriverConfiguration::peakCurrentLimit
 * \todo error control
 */
int DriverInterface::setPeakCurrentLimit(int peakCurrentLimit)
{
    drvConf->peakCurrentLimit = peakCurrentLimit;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

/**
 * @brief stores the driver configuration filename in DriverConfiguration::configFile
 * @param confFile the filename of the configuration filename. Is stored in DriverConfiguration::configFile
 * \todo Error control here. Check the confFile is accessible and readable.
 */
int DriverInterface::setConfigFile(std::string confFile)
{
    drvConf->configFile = confFile;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

/**
 * \brief sets an ID to the driver
 * \param driverID the identificator to set to the driver
 * \todo error control: Make sure the identificator is unique
 */
int DriverInterface::setDriverID(int driverID)
{
    drvConf->driverID = driverID;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

/**
 * \brief sets the driver Name
 * \param driverName the name of the driver
 * \todo error control
 */
int DriverInterface::setDriverName(std::string driverName)
{
    drvConf->driverName = driverName;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

/**
 * \brief sets the type of the driver
 * \param driverType the type of the driver. A string defining the name of the type of the driver
 * \todo error control
 */
int DriverInterface::setDriverType(std::string driverType)
{
    drvConf->driverType = driverType;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

///**
// * Returns ratio between pulses and rad (the number of pulses per 1 rad) of the driver stored in the DriverConfiguration area
// * \return DriverConfiguration::pulsesPerRad
// */
//double DriverInterface::getPulsesPerRad() const {
//	return drvConf->getPulsesPerRad();
//}
/**
 * Returns the number of pulses per revolution of the driver
 * \return DriverConfiguration::pulsesPerRevolution
 */
long int DriverInterface::getPulsesPerRevolution() const
{
    return drvConf->getPulsesPerRevolution();
}

//////////////////////////////////////////////////

/**
 * Transforms the given number of pulses to rads
 * \param pulses the number of pulses to transform
 * \return the pulses transformed to radians (rad)
 * \returns 0 in case of DriverInterface::pulsesToRad is 0 (to avoid a division per 0 error)
 */
double DriverInterface::pulsesToRad(long int pulses)
{
    return (pulses * ((2 * PI) / getPulsesPerRevolution()));
}

//////////////////////////////////////////////////

/**
 * Trasnforms from radians to pulses
 * \param rad the radians to be transformed to pulses
 * \return the radians transformed to pulses
 */
long int DriverInterface::radToPulses(double rad)
{
    return (rad * (getPulsesPerRevolution() / (2 * PI)));
}

//////////////////////////////////////////////////

/**
 * Returns the encoder of pulses per revolution
 * \return DriverConfiguration::encoderPulsesPerRevolution the number of pulses per revolution of the motor's encoder
 */
long int DriverInterface::getEncoderPulsesPerRevolution() const
{
    return drvConf->getEncoderPulsesPerRevolution();
}

//////////////////////////////////////////////////

/**
 * Returns the multiplication factor of the driver.
 * This multiplication factor is a multiplier added to the DriverConfiguration::encoderPulsesPerRevolution used to
 * count the number of pulses per revolution as seen by the driver
 * \return DriverConfiguration::driverMultiplicationFactor the multiplication factor of the driver
 */
int DriverInterface::getDriverMultiplicationFactor() const
{
    return drvConf->getDriverMultiplicationFactor();
}

//////////////////////////////////////////////////

/**
 * Returns reduction factor of the motor of the driver.
 * If the motor uses a gear, the value of the gear should be multiplied to the DriverConfiguration::driverMultiplicationFactor
 * \return DriverConfiguration::reductionFactor the reduction factor of the motor's driver. Units: --
 */
int DriverInterface::getReductionFactor() const
{
    return drvConf->getReductionFactor();
}

//////////////////////////////////////////////////

/**
 * sets the encoder of pulses per revolution
 * \param pulsesPerRevolution the number of pulses per revolution of the motor's encoder
 */
void DriverInterface::setEncoderPulsesPerRevolution(long int pulsesPerRevolution)
{
    drvConf->setEncoderPulsesPerRevolution(pulsesPerRevolution);
}

//////////////////////////////////////////////////

/**
 * Sets the multiplication factor of the driver.
 * This multiplication factor is a multiplier added to the DriverConfiguration::encoderPulsesPerRevolution used to
 * count the number of pulses per revoulution as seen by the driver
 * \param driverMultiplicationFactor the multiplication factor of the driver
 */
void DriverInterface::setDriverMultiplicationFactor(int driverMultiplicationFactor)
{
    drvConf->setDriverMultiplicationFactor(driverMultiplicationFactor);
}

//////////////////////////////////////////////////

/**
 * sets the reduction factor of the driver's motor.
 * If the motor uses a gear, the value of the gear should be multiplied to the DriverConfiguration::driverMultiplicationFactor
 * \param reductionFactor the reduction factor of the motor's driver. Units: --
 */
void DriverInterface::setReductionFactor(int reductionFactor)
{
    drvConf->setReductionFactor(reductionFactor);
}

//////////////////////////////////////////////////

/**
 * Prints in stdout all the limits from DriverConfiguration
 */
void DriverInterface::printLimits() const
{
    cout << endl;
    cout << "Max Position: "
         << getMaxPosition()
         << endl;
    cout << "Min Position: "
         << getMinPosition()
         << endl;
    cout << "Max Velocity: "
         << getMaxVelocity()
         << endl;
    cout << "Max Acceleration: "
         << getMaxAcceleration()
         << endl;
    cout << "Max Decceleration: "
         << getMaxDecceleration()
         << endl;
    cout << "Max Continous Current: "
         << getContinousCurrentLimit()
         << endl;
    cout << "Max Peak Current: "
         << getPeakCurrentLimit()
         << endl;
    cout << endl;
}

//////////////////////////////////////////////////

/**
 * Prints in stdout all the non-limits configuration from DriverConfiguration
 */
void DriverInterface::printConfiguration() const
{
    cout << endl;
    cout << "-- Driver Configuration --"
         << endl;
    cout << "Driver Configuration File: "
         << getConfigFile()
         << endl;
    cout << "Driver Name: "
         << getDriverName()
         << endl;
    cout << "Driver Type: "
         << getDriverType()
         << endl;
    cout << "Driver ID: "
         << getDriverID()
         << endl;

    cout << "Encoder pulses revolution: "
         << getEncoderPulsesPerRevolution()
         << endl;
    cout << "Driver Multiplication Factor: "
         << getDriverMultiplicationFactor()
         << endl;
    cout << "Reduction Factor: "
         << getReductionFactor()
         << endl;
    cout << "Pulses per revolution: "
         << getPulsesPerRevolution()
         << endl;
    //	cout << "Pulses per rad: " << getPulsesPerRad() << endl;
    cout << endl;
}

//////////////////////////////////////////////////

/**
 * Prints in stdout all the Motor Sensor data from DriverMotorSensor
 */
void DriverInterface::printMotorSensor() const
{
    cout << endl;
    cout << "-- Driver Motor Sensor --"
         << endl;
    cout << "Current Position: "
         << getPosition()
         << endl;
    cout << "Current Speed: "
         << getVelocity()
         << endl;
    cout << "Current Instant Current: "
         << getInstantCurrent()
         << endl;
    cout << endl;
}

//////////////////////////////////////////////////

/**
 * Prints in stdout all the status data from DriverStatus
 */
void DriverInterface::printStatus() const
{
    cout << endl;
    cout << "-- Driver Status -- "
         << endl;
    cout << "True = yes. False = no"
         << endl;
    cout << "Driver is enabled: "
         << boolToString(this->isEnabled())
         << endl;
    ;
    cout << "Driver is calibrated: "
         << boolToString(isCalibrated())
         << endl;
    cout << "Current is Limited: "
         << boolToString(isCurrentLimitated())
         << endl;
    cout << "Limit Sensor is reached: "
         << boolToString(isLimitSensorReached())
         << endl;
    cout << "Overtemperature warning: "
         << boolToString(isOverTemperatureWarning())
         << endl;
    cout << "Overvoltage warning: "
         << boolToString(isOverVoltageWarning())
         << endl;
    cout << "Driver is connected: "
         << boolToString(isConnected())
         << endl;
    cout << endl;
}

//////////////////////////////////////////////////

/**
 * sets the DriverConfiguration::calibrationCurrentLimit (the maximum current delivered to the motor during the calibration process
 * \param cLimit the currentLimit during the calibration process (units mA)
 */
void DriverInterface::setCalibrationCurrentLimit(int cLimit)
{
    drvConf->setCalibrationCurrentLimit(cLimit);
}

//////////////////////////////////////////////////

/**
 * sets the DriverConfiguration::calibrationSensorPosition to the entered value
 * \param sensorPosition the position at which sensor position is located mesured from the 0 position (units pulses)
 */
void DriverInterface::setCalibrationSensorPosition(long int sensorPosition)
{
    drvConf->setCalibrationSensorPosition(sensorPosition);
}

//////////////////////////////////////////////////

/**
 * sets the DriverConfiguration::calibrationSpeed (units:rpm)
 * \param calibrationSpeed the maximum speed at which the calibration must be carried out (units rpm)
 */
void DriverInterface::setCalibrationSpeed(long int calibrationSpeed)
{
    drvConf->setCalibrationSpeed(calibrationSpeed);
}

//////////////////////////////////////////////////

/**
 * sets the DriverConfiguration::calibrationTimeOut (units ms)
 * \param timeOut maxium time in which the calibration must be performed (units ms)
 */
void DriverInterface::setCalibrationTimeOut(int timeOut)
{
    drvConf->setCalibrationTimeOut(timeOut);
}

//////////////////////////////////////////////////

/**
 * \brief Function to convert a bool to a string
 * \param b the boolean to convert
 * \return "true" if the boolean is true
 * \return "false" if the boolean is false
 */
string DriverInterface::boolToString(bool b) const
{
    string sBool = "false";
    if (b == true) {
        sBool = "true";
        return sBool;
    }
    return sBool;
}

//////////////////////////////////////////////////
