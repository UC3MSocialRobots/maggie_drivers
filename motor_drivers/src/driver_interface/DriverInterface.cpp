/**
 * \file DriverInterface.cpp
 *
 * \date 13/05/2010
 * \author Victor Gonzalez Pacheco (mailto: vgonzale@ing.uc3m.es)
 */

#include "driver_interface/DriverInterface.h"

//////////////////////////////////////////////////

DriverInterface::DriverInterface()
{
    drvConf = 0;
    drvMotorSensor = 0;
    drvStatus = 0;
}

//////////////////////////////////////////////////

DriverInterface::~DriverInterface()
{
}

//////////////////////////////////////////////////

// Status Section

bool DriverInterface::isCurrentLimitated() const
{
    return drvStatus->currentLimitated;
}

//////////////////////////////////////////////////

bool DriverInterface::isEnabled() const
{
    return drvStatus->enabled;
}

//////////////////////////////////////////////////

bool DriverInterface::isLimitSensorReached() const
{
    return drvStatus->limitSensorReached;
}

//////////////////////////////////////////////////

bool DriverInterface::isOverTemperatureWarning() const
{
    return drvStatus->overTemperatureWarning;
}

//////////////////////////////////////////////////

bool DriverInterface::isOverVoltageWarning() const
{
    return drvStatus->overVoltageWarning;
}

//////////////////////////////////////////////////

bool DriverInterface::isCalibrated() const
{
    return drvStatus->calibrated;
}

//////////////////////////////////////////////////

bool DriverInterface::isConnected() const
{
    return drvStatus->connected;
}

//////////////////////////////////////////////////

// Status Setters

int DriverInterface::isCurrentLimitated(bool curLim)
{
    drvStatus->currentLimitated = curLim;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int DriverInterface::isEnabled(bool enabled)
{
    drvStatus->enabled = enabled;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int DriverInterface::isLimitSensorReached(bool limitSensorReached)
{
    drvStatus->limitSensorReached = limitSensorReached;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int DriverInterface::isOverTemperatureWarning(bool overTemperatureWarning)
{
    drvStatus->overTemperatureWarning = overTemperatureWarning;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int DriverInterface::isOverVoltageWarning(bool overVoltageWarning)
{
    drvStatus->overVoltageWarning = overVoltageWarning;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int DriverInterface::isCalibrated(bool calibrated)
{
    drvStatus->calibrated = calibrated;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int DriverInterface::isConnected(bool connected)
{
    drvStatus->connected = connected;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

// Motor Sensor Section

long int DriverInterface::getPosition() const
{
    return drvMotorSensor->position;
}

//////////////////////////////////////////////////

long int DriverInterface::getVelocity() const
{
    return drvMotorSensor->velocity;
}

//////////////////////////////////////////////////

int DriverInterface::getInstantCurrent() const
{
    return drvMotorSensor->instantCurrent;
}

//////////////////////////////////////////////////

// Configuration Section

long int DriverInterface::getMaxPosition() const
{
    return drvConf->maxPosition;
}

//////////////////////////////////////////////////

long int DriverInterface::getMinPosition() const
{
    return drvConf->minPosition;
}

//////////////////////////////////////////////////

long int DriverInterface::getMaxVelocity() const
{
    return drvConf->getMaxVelocity();
}

//////////////////////////////////////////////////

long int DriverInterface::getMaxAcceleration() const
{
    return drvConf->getMaxAcceleration();
}

//////////////////////////////////////////////////

long int DriverInterface::getMaxDecceleration() const
{
    return drvConf->getMaxDecceleration();
}

//////////////////////////////////////////////////

int DriverInterface::getContinousCurrentLimit() const
{
    return drvConf->getContinousCurrentLimit();
}

//////////////////////////////////////////////////

int DriverInterface::getPeakCurrentLimit() const
{
    return drvConf->getPeakCurrentLimit();
}

//////////////////////////////////////////////////

string DriverInterface::getConfigFile() const
{
    return drvConf->configFile;
}

//////////////////////////////////////////////////

int DriverInterface::getDriverID() const
{
    return drvConf->driverID;
}

//////////////////////////////////////////////////

string DriverInterface::getDriverName() const
{
    return drvConf->driverName;
}

//////////////////////////////////////////////////

string DriverInterface::getDriverType() const
{
    return drvConf->driverType;
}

//////////////////////////////////////////////////

// Configuration Setters

int DriverInterface::setMaxPosition(long int maxPosition)
{
    drvConf->maxPosition = maxPosition;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int DriverInterface::setMinPosition(long int minPosition)
{
    drvConf->minPosition = minPosition;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int DriverInterface::setMaxVelocity(long int maxVelocity)
{
    drvConf->maxVelocity = maxVelocity;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int DriverInterface::setMaxAcceleration(long int maxAcceleration)
{
    drvConf->maxAcceleration = maxAcceleration;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int DriverInterface::setMaxDecceleration(long int maxDecceleration)
{
    drvConf->maxDecceleration = maxDecceleration;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int DriverInterface::setContinousCurrentLimit(int continousCurrentLimit)
{
    drvConf->continousCurrentLimit = continousCurrentLimit;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int DriverInterface::setPeakCurrentLimit(int peakCurrentLimit)
{
    drvConf->peakCurrentLimit = peakCurrentLimit;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int DriverInterface::setConfigFile(std::string confFile)
{
    drvConf->configFile = confFile;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int DriverInterface::setDriverID(int driverID)
{
    drvConf->driverID = driverID;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int DriverInterface::setDriverName(std::string driverName)
{
    drvConf->driverName = driverName;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int DriverInterface::setDriverType(std::string driverType)
{
    drvConf->driverType = driverType;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

//double DriverInterface::getPulsesPerRad() const {
//	return drvConf->getPulsesPerRad();
//}

//////////////////////////////////////////////////

long int DriverInterface::getPulsesPerRevolution() const
{
    return drvConf->getPulsesPerRevolution();
}

//////////////////////////////////////////////////

double DriverInterface::pulsesToRad(long int pulses)
{
    return (pulses * ((2 * PI) / getPulsesPerRevolution()));
}

//////////////////////////////////////////////////

long int DriverInterface::radToPulses(double rad)
{
    return (rad * (getPulsesPerRevolution() / (2 * PI)));
}

//////////////////////////////////////////////////

long int DriverInterface::getEncoderPulsesPerRevolution() const
{
    return drvConf->getEncoderPulsesPerRevolution();
}

//////////////////////////////////////////////////

int DriverInterface::getDriverMultiplicationFactor() const
{
    return drvConf->getDriverMultiplicationFactor();
}

//////////////////////////////////////////////////

int DriverInterface::getReductionFactor() const
{
    return drvConf->getReductionFactor();
}

//////////////////////////////////////////////////

void DriverInterface::setEncoderPulsesPerRevolution(long int pulsesPerRevolution)
{
    drvConf->setEncoderPulsesPerRevolution(pulsesPerRevolution);
}

//////////////////////////////////////////////////

void DriverInterface::setDriverMultiplicationFactor(int driverMultiplicationFactor)
{
    drvConf->setDriverMultiplicationFactor(driverMultiplicationFactor);
}

//////////////////////////////////////////////////

void DriverInterface::setReductionFactor(int reductionFactor)
{
    drvConf->setReductionFactor(reductionFactor);
}

//////////////////////////////////////////////////

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
    cout << endl;
}

//////////////////////////////////////////////////

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

void DriverInterface::setCalibrationCurrentLimit(int cLimit)
{
    drvConf->setCalibrationCurrentLimit(cLimit);
}

//////////////////////////////////////////////////

void DriverInterface::setCalibrationSensorPosition(long int sensorPosition)
{
    drvConf->setCalibrationSensorPosition(sensorPosition);
}

//////////////////////////////////////////////////

void DriverInterface::setCalibrationSpeed(long int calibrationSpeed)
{
    drvConf->setCalibrationSpeed(calibrationSpeed);
}

//////////////////////////////////////////////////

void DriverInterface::setCalibrationTimeOut(int timeOut)
{
    drvConf->setCalibrationTimeOut(timeOut);
}

//////////////////////////////////////////////////

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
