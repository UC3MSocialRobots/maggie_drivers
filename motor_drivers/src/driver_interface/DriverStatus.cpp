/*
 * DriverStatus.cpp
 *
 *  Created on: 12/05/2010
 *      Author: vgonpa
 */

#include "driver_interface/DriverStatus.h"

//////////////////////////////////////////////////

DriverStatus::DriverStatus()
{
    this->calibrated = false;
    this->currentLimitated = false;
    this->enabled = false;
    this->limitSensorReached = false;
    this->overTemperatureWarning = false;
    this->overVoltageWarning = false;
}

//////////////////////////////////////////////////

DriverStatus::~DriverStatus()
{
}

//////////////////////////////////////////////////

bool DriverStatus::isCurrentLimitated() const
{
    return currentLimitated;
}

//////////////////////////////////////////////////

bool DriverStatus::isEnabled() const
{
    return enabled;
}

//////////////////////////////////////////////////

bool DriverStatus::isLimitSensorReached() const
{
    return limitSensorReached;
}

//////////////////////////////////////////////////

bool DriverStatus::isOverTemperatureWarning() const
{
    return overTemperatureWarning;
}

//////////////////////////////////////////////////

bool DriverStatus::isOverVoltageWarning() const
{
    return overVoltageWarning;
}

//////////////////////////////////////////////////

bool DriverStatus::isCalibrated() const
{
    return calibrated;
}

//////////////////////////////////////////////////

// Setters

void DriverStatus::isCurrentLimitated(bool curLim)
{
    this->currentLimitated = curLim;
}

//////////////////////////////////////////////////

void DriverStatus::isEnabled(bool enabled)
{
    this->enabled = enabled;
}

//////////////////////////////////////////////////

void DriverStatus::isLimitSensorReached(bool limitSensorReached)
{
    this->limitSensorReached = limitSensorReached;
}

//////////////////////////////////////////////////

void DriverStatus::isOverTemperatureWarning(bool overTemperatureWarning)
{
    this->overTemperatureWarning = overTemperatureWarning;
}

//////////////////////////////////////////////////

void DriverStatus::isOverVoltageWarning(bool overVoltageWarning)
{
    this->overVoltageWarning = overVoltageWarning;
}

//////////////////////////////////////////////////

void DriverStatus::isCalibrated(bool calibrated)
{
    this->calibrated = calibrated;
}

//////////////////////////////////////////////////

bool DriverStatus::isConnected() const
{
    return this->connected;
}

//////////////////////////////////////////////////

void DriverStatus::isConnected(bool connected)
{
    this->connected = connected;
}

//////////////////////////////////////////////////
