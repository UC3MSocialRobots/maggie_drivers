/**
 * \file DriverMotorSensor.cpp
 *
 * \date 11/05/2010
 * \author Victor Gonzalez Pacheco (mailto:vgonzale@ing.uc3m.es)
 *
 */
#include "driver_interface/DriverMotorSensor.h"

//////////////////////////////////////////////////

DriverMotorSensor::DriverMotorSensor()
{
    setPosition(0);
    setVelocity(0);
    setInstantCurrent(0);
}

//////////////////////////////////////////////////

DriverMotorSensor::~DriverMotorSensor()
{
}

//////////////////////////////////////////////////

// Access Functions

long int DriverMotorSensor::getPosition() const
{
    return position;
}

//////////////////////////////////////////////////

long int DriverMotorSensor::getVelocity() const
{
    return velocity;
}

//////////////////////////////////////////////////

int DriverMotorSensor::getInstantCurrent() const
{
    return instantCurrent;
}

//////////////////////////////////////////////////

void DriverMotorSensor::setInstantCurrent(int instantCurrent)
{
    this->instantCurrent = instantCurrent;
}

//////////////////////////////////////////////////

void DriverMotorSensor::setPosition(long int position)
{
    this->position = position;
}

//////////////////////////////////////////////////

void DriverMotorSensor::setVelocity(long int velocity)
{
    this->velocity = velocity;
}

//////////////////////////////////////////////////
