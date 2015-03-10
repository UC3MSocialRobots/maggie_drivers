/**
 * \file DriverConfiguration.cpp
 *
 *  \date 12/05/2010
 *  \author: Victor Gonzalez (vgonzale@ing.uc3m.es)
 */

#include "driver_interface/DriverConfiguration.h"

DriverConfiguration::DriverConfiguration (){
	driverID = 0;
	driverName = "NoName";
	driverType = "NoType";
	configFile.clear();

	// Calibration Parameters
	calibrationSensorPosition = 0;
	calibrationCurrentLimit = 0;
	calibrationSpeed = 0;
	calibrationTimeOut = 0;

	// Default parameters
	maxPosition = 0;					// In pulses of the driver
	minPosition = 0;					// In pulses of the driver
	maxVelocity = 0;					// In rpm
	maxAcceleration = 0;
	maxDecceleration = 0;
	continousCurrentLimit = 0;
	peakCurrentLimit = 0;

	setEncoderPulsesPerRevolution(DEFAULT_ENCODER_PULSES_PER_REV);
	setDriverMultiplicationFactor(DEFAULT_DRIVER_MULTIPLICATION_FACTOR);
	setReductionFactor(DEFAULT_REDUCTION_FACTOR);
	setPulsesPerRevolution();
//	setPulsesToRad();
}

DriverConfiguration::DriverConfiguration(std::string configurationFile){
	setConfigFile(configurationFile);
	DriverConfiguration();

}

DriverConfiguration::~DriverConfiguration() {
	// TODO Auto-generated destructor stub
}


// Getters
/**
 * \brief returns the maxPosition of the driver.
 * \return maxPosition the maximum position the driver can/is_allowed_to reach. Units: pulses
 */
long int DriverConfiguration::getMaxPosition() const {
    return maxPosition;
}

/**
 * \brief returns the minPosition of the driver.
 * \return minPosition the minimum position the driver can/is_allowed_to reach. Units: pulses
 */
long int DriverConfiguration::getMinPosition() const {
    return minPosition;
}

/**
 * \brief returns the maxVelocity of the driver.
 * \return maxVelocity the maximum velocity the driver can/is_allowed_to reach. Units: rpm
 */
long int DriverConfiguration::getMaxVelocity() const {
    return maxVelocity;
}

/**
 * \brief returns the maxAcceleration of the driver.
 * \return maxPosition the maximum Acceleration the driver can/is_allowed_to reach. Units: revolutions/sec^2
 */
long int DriverConfiguration::getMaxAcceleration() const {
    return maxAcceleration;
}

/**
 * \brief returns the maxDecceleration of the driver.
 * \return maxDecceleration the maximum Deceleration the driver can/is_allowed_to reach. Units: revolutions/sec^2
 */
long int DriverConfiguration::getMaxDecceleration() const {
     return maxDecceleration;
 }

/**
 * \brief returns the Continuous current limit  of the driver.
 * \return continousCurrentLimit the limit in the continous limit the driver can/is_allowed_to reach. Units: mA
 */
int DriverConfiguration::getContinousCurrentLimit() const {
	return continousCurrentLimit;
}

/**
 * \brief returns the Peak current limit of the driver.
 * \return peakCurrentLimit the peak current limit the driver can/is_allowed_to reach. Units: mA
 */
int DriverConfiguration::getPeakCurrentLimit() const {
     return peakCurrentLimit;
 }


/**
 * \brief returns current limit for the calibration process
 * \return calibrationCurrentLimit The current limit wich the  driver can/is_allowed_to reach during the cailbration process. Units: mA
 */
int DriverConfiguration::getCalibrationCurrentLimit() const {
    return calibrationCurrentLimit;
}

/**
 * This function returns the position of the calibration sensor, which is used to establish the Home (0) position
 * of the driver afther it is calibrated
 *
 * \brief returns the position of the limit sensor used in the calibration process
 * \return calibrationSensorPosition The position of the calibration sensor measured in pulses from the desired Home (0) position
 */
long int DriverConfiguration::getCalibrationSensorPosition() const {
    return calibrationSensorPosition;
}

/**
 * \brief returns the maximum speed of the motor during the calibration process. Units rpm
 * \return calibration speed The maximum speed (in rpm) of the motor during the calibration process
 */
long int DriverConfiguration::getCalibrationSpeed() const {
    return calibrationSpeed;
}

/**
 * \brief returns the maximum time allowed to perform the calibration process. Units: ms
 * \return calibrationTimeOut the maximum allowed time to perform the calibration process. Units: ms
 */
int DriverConfiguration::getCalibrationTimeOut() const {
    return calibrationTimeOut;
}

/**
 * \brief returns the driver configuration file
 * \return configFile The configuration file of the driver
 */
string DriverConfiguration::getConfigFile() const {
	return configFile;
}

/**
 * \brief Returns the identificator of the driver
 * \return DriverConfiguration::driverID the identificator of the driver
 */
int DriverConfiguration::getDriverID() const {
	 return driverID;
}

/**
 * \brief Returns the driver Name
 * \return DriverConfiguration::driverName the driver name
 */
string DriverConfiguration::getDriverName() const {
	 return driverName;
}

/**
 * \brief Returns the driver Type.
 * \return DriverConfiguration::driverType the driver type
 */
string DriverConfiguration::getDriverType() const {
	 return driverType;
}

// Measure units and conversion data

/**
 * Returns the encoder of pulses per revolution
 * \return DriverConfiguration::encoderPulsesPerRevolution the number of pulses per revolution of the motor's encoder
 */
long int DriverConfiguration::getEncoderPulsesPerRevolution () const{
	return this->encoderPulsesPerRevolution;
}

/**
 * Returns the multiplication factor of the driver.
 * This multiplication factor is a multiplier added to the DriverConfiguration::encoderPulsesPerRevolution used to
 * count the number of pulses per revoulution as seen by the driver
 * \return DriverConfiguration::driverMultiplicationFactor the multiplication factor of the driver
 */
int DriverConfiguration::getDriverMultiplicationFactor () const{
	return this->driverMultiplicationFactor;
}

/**
 * Returns reduction factor of the motor of the driver.
 * If the motor uses a gear, the value of the gear should be multiplied to the DriverConfiguration::driverMultiplicationFactor
 * \return DriverConfiguration::reductionFactor the reduction factor of the motor's driver. Units: --
 */
 int DriverConfiguration::getReductionFactor() const {
    return reductionFactor;
}

/**
 * Returns the number of pulses per revolution of the driver.
 * Its the result of
 * DriverConfiguration::encoderPulsesPerRevolution * DriverConfiguration::driverMultiplicationFactor * DriverConfiguration::reductionFactor
 * \return DriverConfiguration::pulsesPerRevolution the number of pulses the motor does to perform a single revolution. Units: pulses
 */
long int DriverConfiguration::getPulsesPerRevolution() const {
    return pulsesPerRevolution;
}

///**
// * returns the ratio between pulses and rads of the motor
// * \return ActuatorConfiguration::pulsesPerRad the ratio between pulses and rad of the acutator's linked motor
// */
//double DriverConfiguration::getPulsesPerRad() const{
//	return this->pulsesPerRad;
//}


// Setters
/**
 * \brief sets the maxPosition of the driver.
 * \param maxPosition the maximum position the driver can/is_allowed_to reach. Units: pulses
 */
void DriverConfiguration::setMaxPosition(long int maxPosition) {
    this->maxPosition = maxPosition;
}

/**
 * \brief sets the minPosition of the driver.
 * \param minPosition the minimum position the driver can/is_allowed_to reach. Units: pulses
 */
void DriverConfiguration::setMinPosition(long int minPosition)  {
     this->minPosition = minPosition;
 }

/**
 * \brief sets the maxVelocity of the driver.
 * \param maxVelocity the maximum velocity the driver can/is_allowed_to reach. Units: rpm
 */
void DriverConfiguration::setMaxVelocity(long int maxVelocity) {
    this->maxVelocity = maxVelocity;
}

/**
 * \brief sets the maxAcceleration of the driver.
 * \param maxPosition the maximum Acceleration the driver can/is_allowed_to reach. Units: revolutions/sec^2
 */
void DriverConfiguration::setMaxAcceleration(long int maxAcceleration) {
    this->maxAcceleration = maxAcceleration;
}

/**
 * \brief sets the maxDecceleration of the driver.
 * \param maxDecceleration the maximum Deceleration the driver can/is_allowed_to reach. Units: revolutions/sec^2
 */
void DriverConfiguration::setMaxDecceleration(long int maxDecceleration)  {
     this->maxDecceleration = maxDecceleration;
 }

/**
 * \brief sets the Continuous current limit  of the driver.
 * \param continousCurrentLimit the limit in the continous limit the driver can/is_allowed_to reach. Units: mA
 */
void DriverConfiguration::setContinousCurrentLimit(int continousCurrentLimit) {
    this->continousCurrentLimit = continousCurrentLimit;
}

/**
 * \brief sets the Peak current limit of the driver.
 * \param peakCurrentLimit the peak current limit the driver can/is_allowed_to reach. Units: mA
 */
 void DriverConfiguration::setPeakCurrentLimit(int peakCurrentLimit)  {
     this->peakCurrentLimit = peakCurrentLimit;
 }

 /**
  * \brief sets the current limit for the calibration process.
  * \param current limit for the calibration process. Units: mA
  * \todo error control here: check that the user is not entering impossible values like negative limits, etc.
  */
 void DriverConfiguration::setCalibrationCurrentLimit(int calibrationCurrentLimit) {
	 this->calibrationCurrentLimit = calibrationCurrentLimit;
 }

 /**
  * \brief sets the Calibration Sensor Position. This sensor is used as a reference to establish the Home (0) Position
  * \param calibrationSensorPosition the position of the Calibration Sensor measured from the desired Home (0) Position. Units: pulses
  */
 void DriverConfiguration::setCalibrationSensorPosition(long int calibrationSensorPosition) {
	 this->calibrationSensorPosition = calibrationSensorPosition;
 }

 /**
  * \brief sets the maximum speed the motor can reach during the calibration process. Units: rpm
  * \param calibrationSpeed the maximum speed the motor can reach during the calibration process. Units: rpm
  */
 void DriverConfiguration::setCalibrationSpeed(long int calibrationSpeed) {
	 this->calibrationSpeed = calibrationSpeed;
 }

 /**
  * \brief sets maximum time the calibration process should take. Units: ms
  * \param calibrationTimeOut the time limit for the cailbration process. Units: ms
  * \todo error control here: check that the user is not entering impossible values like negative times, etc.
  */
 void DriverConfiguration::setCalibrationTimeOut(int calibrationTimeOut) {
	 this->calibrationTimeOut = calibrationTimeOut;
 }

 /**
  * \brief sets the driver configuration file
  * \param configFile the configuration file that will be used by this driver
  * \todo Error control here. Check the confFile is accessible and readable.
  */
 void DriverConfiguration::setConfigFile(std::string confFile){
	 this->configFile = confFile;
 }


 /**
   * @brief sets the driver ID (driver unique identificator)
   * @param driverID the driver unique identificator
   * \todo In the future:Error control here. Check if this id is already given
   */
  void DriverConfiguration::setDriverID(int driverID) {
 	 this->driverID = driverID;
  }

  /**
   * @brief sets the driver Name. It should be a unique name
   * @param driverName the unique name of the driver
   * \todo Error control here
   */
  void DriverConfiguration::setDriverName(std::string driverName) {
 	 this->driverName = driverName;
  }

  /**
   * @brief sets the driver type. It must be one of the existing types
   * @param driverType the type of the driver (at \date 15 jun 2010 there is only one possible: mcdc3006s)
   * \todo Error control here. Check if the entered driver type is one of the existing ones
   */
  void DriverConfiguration::setDriverType(std::string driverType) {
 	 this->driverType = driverType;
  }

  /**
   * sets the encoder of pulses per revolution
   * \param pulsesPerRevolution the number of pulses per revolution of the motor's encoder
   */
  void DriverConfiguration::setEncoderPulsesPerRevolution(long int pulsesPerRevolution){
	  this->encoderPulsesPerRevolution = pulsesPerRevolution;
  }


  /**
   * Sets the multiplication factor of the driver.
   * This multiplication factor is a multiplier added to the DriverConfiguration::encoderPulsesPerRevolution used to
   * count the number of pulses per revoulution as seen by the driver
   * \param driverMultiplicationFactor the multiplication factor of the driver
   */
  void DriverConfiguration::setDriverMultiplicationFactor (int driverMultiplicationFactor){
	  this->driverMultiplicationFactor = driverMultiplicationFactor;
  }

  /**
   * sets the reduction factor of the driver's motor.
   * If the motor uses a gear, the value of the gearshould be multiplied to the DriverConfiguration::driverMultiplicationFactor
   * \param reductionFactor the reduction factor of the motor's driver. Units: --
   */
  void DriverConfiguration::setReductionFactor(int reductionFactor) {
	  this->reductionFactor = reductionFactor;
  }

  /**
   * sets the number of pulses per revolution of the driver.
   * Its the result of
   * DriverConfiguration::encoderPulsesPerRevolution * DriverConfiguration::driverMultiplicationFactor * DriverConfiguration::reductionFactor
   */
  void DriverConfiguration::setPulsesPerRevolution() {
	  this->pulsesPerRevolution = getEncoderPulsesPerRevolution()
			  * getDriverMultiplicationFactor()
			  * getReductionFactor();
  }

/**
 * Sets the pulses per revolution of the driver.
 * \param pulsesPerRevolution the number of pulses the motor does to perform a single revolution. Units: pulses
 */
  void DriverConfiguration::setPulsesPerRevolution(long int pulsesPerRevolution){
	  this->pulsesPerRevolution = pulsesPerRevolution;
  }

//  /**
//   * Sets the ratio between pulses and rads of the motor linked to the actuator.
//   */
//  void DriverConfiguration::setPulsesToRad (){
//	this->pulsesPerRad = ( getEncoderPulsesPerRevolution()
//			* getDriverMultiplicationFactor()
//			* getReductionFactor() )
//			/ (2 * pi);
//  }



