/*
 * DriverStatus.cpp
 *
 *  Created on: 12/05/2010
 *      Author: vgonpa
 */

#include "driver_interface/DriverStatus.h"

/**
 * Default constructor initialised to false
 */
DriverStatus::DriverStatus() {
	this->calibrated = false;
	this->currentLimitated = false;
	this->enabled = false;
	this->limitSensorReached = false;
	this->overTemperatureWarning = false;
	this->overVoltageWarning = false;
}

DriverStatus::~DriverStatus() {
	// TODO Auto-generated destructor stub
}

// Getters
/**
 * @brief Checks if the current is being limitated.
 * @return currentLimitated. True: the current is being limited.
 */
bool DriverStatus::isCurrentLimitated() const {
     return currentLimitated;
 }

/**
 * @brief Checks if the driver is enabled or disabled. True = driver enabled. False = driver disabled.
 * @return enabled. True if it is enabled.
 */
 bool DriverStatus::isEnabled() const {
     return enabled;
 }

 /**
  * @brief Checks if the limit sensoris has been reached. True the sensor has been reached.
  * @return limitSensorReached.. True: the limit sensor is producing an output. Thus the limit sensor has been reached.
  */
 bool DriverStatus::isLimitSensorReached() const {
     return limitSensorReached;
 }

 /**
  * @brief Checks if the overTemperatureWarning has been triggered (there is an over Temperature error in the driver). True = overTemperatureWarning.
  * @return overTemperatureWarning. True: warning.
  */
 bool DriverStatus::isOverTemperatureWarning() const {
     return overTemperatureWarning;
 }

 /**
   * @brief Checks if the overTemperatureWarning has been triggered (there is an overvoltage error in the driver). True = overVoltageWarning.
   * @return overVoltageWarning. True: warning.
   */
bool DriverStatus::isOverVoltageWarning() const {
     return overVoltageWarning;
 }

 /**
   * @brief checks if the the calibration flag is calibrated or not.
   * @return calibrate The calibration flag. true = the driver is calibrated. false = the driver is not calibrated
   */
 bool DriverStatus::isCalibrated() const {
	 return calibrated;
 }

 // Setters

 /**
   * @brief Sets the DriverStatus::currentLimitated to the value entered in the parameter
   * @param currentLimitated. The value to set DriverStatus::currentLimitated
   */
 void DriverStatus::isCurrentLimitated(bool curLim) {
     this->currentLimitated = curLim;
 }

 /**
    * @brief Sets the DriverStatus::enabled to the value entered in the parameter enabled. True = device is enabled. False = device is disabled.
    * @param enabled. The value to set DriverStatus::enabled
    */
 void DriverStatus::isEnabled(bool enabled) {
     this->enabled = enabled;
 }

 /**
    * @brief Sets the DriverStatus::limitSensorReached to true or false depending if the Limit sensor has been reached (true) or not (false)
    * @param limitSensorReached. The value to set DriverStatus::limitSensorReached
    */
 void DriverStatus::isLimitSensorReached(bool limitSensorReached) {
     this->limitSensorReached = limitSensorReached;
 }

 /**
    * @brief Sets the DriverStatus::overTemperatureWarning to true or false depending on the status provided by the driver.
    * @param overTemperatureWarning. The value to set DriverStatus::overTemperatureWarning
    */
void DriverStatus::isOverTemperatureWarning(bool overTemperatureWarning) {
     this->overTemperatureWarning = overTemperatureWarning;
 }

 /**
    * @brief Sets the DriverStatus::overVoltageWarning to the value entered in the parameter
    * @param overVoltageWarning. The value to set DriverStatus::overVoltageWarning
    */
 void DriverStatus::isOverVoltageWarning(bool overVoltageWarning) {
     this->overVoltageWarning = overVoltageWarning;
 }

 /**
  * @brief sets the DriverStatus::calibrated calibration flag to the value of the entered parameter
  * @param calibrated The value to set the calibration flag. true = the driver is calibrated. false = the driver is not calibrated
  */
void DriverStatus::isCalibrated (bool calibrated) {
	this->calibrated = calibrated;
}

/**
 * \brief checks the status of the boolean DriverStatus::open When the driver is open this boolean is set to true. Otherwise is false
 * \return DriverStatus::open the boolean that keeps the status of the driver
 * \note open means that there is communication between the computer and the driver
 */
bool DriverStatus::isConnected() const {
	return this->connected;
}


/**
 * \brief Sets the DriverStatus::open flag to the entered parameter (true or false).
 * \param connected the boolean variable to specify that the driver is open or closed. True = open. False = closed
 */
void DriverStatus::isConnected(bool connected) {
	this->connected = connected;
}
