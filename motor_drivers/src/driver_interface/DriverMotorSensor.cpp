/**
 * \file DriverMotorSensor.cpp
 *
 * \date 11/05/2010
 * \author Victor Gonzalez Pacheco (mailto:vgonzale@ing.uc3m.es)
 *
 */
#include "driver_interface/DriverMotorSensor.h"

DriverMotorSensor::DriverMotorSensor() {
	setPosition(0);
	setVelocity(0);
	setInstantCurrent(0);
}

DriverMotorSensor::~DriverMotorSensor(){
}

// Access Functions
/**
 * \brief returns the position
 * \return DriverMotorSensor::position the last stored motorSensor position. Units: pulses
 */
long int DriverMotorSensor::getPosition() const {
	return position;
}

/**
 * \brief returns the velocity
 * \return DriverMotorSensor::velocity the last stored motorSensor velocity. Units: rpm (revolutions per minute)
 */
long int DriverMotorSensor::getVelocity() const {
	return velocity;
}


/**
 * \brief returns the instant current
 * \return DriverMotorSensor::instantCurrent the last stored motorSensor instant current. Units: milli amperes (mA)
 */
int DriverMotorSensor::getInstantCurrent() const {
	return instantCurrent;
}

/**
 * \brief sets the instant current in the motorSensor data
 * \param DriverMotorSensor::instantCurrent sets the instant current in the motorSensor data. Units: milli amperes (mA)
 */
void DriverMotorSensor::setInstantCurrent(int instantCurrent) {
	this->instantCurrent = instantCurrent;
}

/**
 * \brief sets the position in the motorSensor data
 * \param DriverMotorSensor::position sets the position in the motorSensor data. Units: pulses
 */
void DriverMotorSensor::setPosition(long int position) {
	this->position = position;
}

/**
 * \brief sets the velocity in the motorSensor data
 * \param DriverMotorSensor::velocity sets the velocity in the motorSensor data. Units: rpm (revolutions per minute)
 */
void DriverMotorSensor::setVelocity(long int velocity) {
	this->velocity = velocity;
}
