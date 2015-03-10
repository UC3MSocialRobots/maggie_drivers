/**
 * DynamixelMotorSensor.cpp
 *      Created on: 01/09/2010
 *      Author: Paulina Vélez Núñez (mailto: pvelez@ing.uc3m.es)
 */

#include "DynamixelMotorSensor.h"


DynamixelMotorSensor::DynamixelMotorSensor() : DriverMotorSensor() {

}

DynamixelMotorSensor::~DynamixelMotorSensor() {
	/// \todo Auto-generated destructor stub
}

//GETTERS

int DynamixelMotorSensor::getLoad(){
	return load;
}

int DynamixelMotorSensor::getLoadDirection(){
	return loadDirection;
}

float DynamixelMotorSensor::getVoltage(){
	return voltage;
}

int DynamixelMotorSensor::getTemperature(){
	return temperature;
}

//SETTERS

void DynamixelMotorSensor::setLoad(int load){
	this->load=load;
}

void DynamixelMotorSensor::setLoadDirection(int loadDirection){
	this->loadDirection= loadDirection;
}

void DynamixelMotorSensor::setVoltage(float voltage){
	this->voltage= voltage;
}

void DynamixelMotorSensor::setTemperature(int Temperature){
	this->temperature= temperature;
}
