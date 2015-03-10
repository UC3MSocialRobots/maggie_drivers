/**
 * \file MCDC3006SDriver.cpp
 *
 * \date 14/05/2010
 * \author Victor Gonzalez (mailto:vgonzale@ing.uc3m.es)
 */

#include "mcdc3006s/MCDC3006SDriver.h"
//#include "long_term_memory/ltm_path.h"
#include <ros/package.h>

MCDC3006SDriver::MCDC3006SDriver(std::string driverConfigFile)
    : DriverInterface() {

    mcdc3006Config = new MCDC3006SConfiguration (driverConfigFile);
    mcdc3006MotorSens = new MCDC3006SMotorSensor();
    mcdc3006Status = new MCDC3006SStatus();

    //	this->updateStatus(); 			// Reading the status from the driver
    mcdc3006Status->connected = false;	// At this moment the driver is not yet opened
    this->calibrate(false);			// This value it should be already in this state, but we do it again for safety

    // The base class will need this pointers
    drvConf = mcdc3006Config;
    drvMotorSensor = mcdc3006MotorSens;
    drvStatus = mcdc3006Status;
}

MCDC3006SDriver::MCDC3006SDriver ()
    : DriverInterface::DriverInterface () {

    mcdc3006Config = new MCDC3006SConfiguration ();
    mcdc3006MotorSens = new MCDC3006SMotorSensor();
    mcdc3006Status = new MCDC3006SStatus();

    mcdc3006Status->connected = false;	// At this moment the driver is not yet opened
    this->calibrate(false);		// This value it should be already in this state, but we do it again for safety

    // The base class will need this pointers
    drvConf = mcdc3006Config;
    drvMotorSensor = mcdc3006MotorSens;
    drvStatus = mcdc3006Status;
}

MCDC3006SDriver::~MCDC3006SDriver() {
    disconnect();
    delete mcdc3006Config;
    delete mcdc3006MotorSens;
    delete mcdc3006Status;
    drvConf = 0;
    drvMotorSensor = 0;
    drvStatus =0;
}

/**
 * @brief Moves an MCDC3006S driver by a velocity command.
 * @param vel the velocity to move the driver (in rpm).
 * \return ERR_COM in case of error
 * \return ERR_NOERR otherwise
 */
int MCDC3006SDriver::moveDriverByVelocity(long int vel) {
    if ( moveDriverVel(mcdc3006Config->getRS232().RSd, mcdc3006Config->getSemaphore().semID, vel) != ERR_NOERR){
        return ERR_COM;
    }
    return ERR_NOERR;
}

/**
 * @brief Moves an MCDC3006S driver to a relative position.
 * @param relativePos the relative position to move the driver (in pulses).
 * @warning Deprectated function. Used only to provide backwards compatibility.
 * @todo error control here
 */
int MCDC3006SDriver::moveDriverToRelativePosition(long int relativePos) {
    if (moveDriverRelPos(mcdc3006Config->getRS232().RSd, mcdc3006Config->getSemaphore().semID, relativePos) != ERR_NOERR){
        return ERR_COM;
    }
    return ERR_NOERR;
}

/**
 * @brief Moves an MCDC3006S driver to an absolute position.
 * @param absolutePos the absolute position to move the driver (in pulses).
 * @warning Deprectated function. Used only to provide backwards compatibility.
 * \return ERR_COM in case of error
 * \return ERR_NOERR otherwise
 */
int MCDC3006SDriver::moveDriverToAbsolutePosition(long int absolutePos) {
    if (moveDriverAbsPos(mcdc3006Config->getRS232().RSd, mcdc3006Config->getSemaphore().semID, absolutePos) != ERR_NOERR){
        return ERR_COM;
    }
    return ERR_NOERR;
}

/**
 * @brief Calibrate Driver.
 * @param calibrationSensorPosition is the position (in pulses) of the calibration sensor measured from the desired Home position (0). For instance, If we want
 * 				to have the 0 in the sensor, this parameter will be 0.
 * @param calibrationCurrentLimit max current the motor is not allowed to exceed (in mA) This limit is directly related to torque of the motor.
 * @param calibrationSpeed The speed of the motor during the calibration (in rpm)
 * @param calibrationTimeOut maximum time to perform the calibration. If this time is exceeded the function will return an error
 *
 * \return ERR_COM in case of error
 * \return ERR_NOERR otherwise
 * */
int MCDC3006SDriver::calibrate(long int calibrationSensorPosition, long int calibrationCurrentLimit, long int calibrationSpeed, long int calibrationTimeOut) {
    if (calibrateDriver(mcdc3006Config->getRS232().RSd,
                        mcdc3006Config->getSemaphore().semID,
                        calibrationSensorPosition,
                        calibrationCurrentLimit,
                        calibrationSpeed,
                        calibrationTimeOut) != ERR_NOERR)
    {
        mcdc3006Status->isCalibrated(false);
        return ERR_COM;
    }
    mcdc3006Status->isCalibrated(true);
    return ERR_NOERR;
}

/**
 * If the parameter entered ( calibrated) is true, this function calibrates the driver.
 * If it is false, the function sets the MCDC3006SStatus::calibrated to false to indicate the driver must be calibrated.
 * \param calibrated the boolean to set the calibration flag to true or false.
 *  \warning If calibrated is false, this function does not calibrate the driver. It only sets the MCDC3006SStatus::calibrated flag to false.
 * \see MCDC3006SDriver::calibrate(long int calibrationSensorPosition, long int calibrationCurrentLimit, long int calibrationSpeed, long int calibrationTimeOut)
 * \return ERR_COM in case of error
 * \return ERR_NOERR otherwise
 **/
int MCDC3006SDriver::calibrate (bool calibrated){
    if (calibrated == true){ 			// Means that received a command to calibrate the driver
        if (calibrateDriver(mcdc3006Config->rs232.RSd,
                            mcdc3006Config->semaphore.semID,
                            mcdc3006Config->calibrationSensorPosition,
                            mcdc3006Config->calibrationCurrentLimit,
                            mcdc3006Config->calibrationSpeed,
                            mcdc3006Config->calibrationTimeOut) != ERR_NOERR)
        {
            calibrated = false;
            return ERR_COM;  // It hasn't been possible to calibrate the driver
        }
    }
    return ERR_NOERR;
}

/**
 * Enables the driver
 * \return ERR_COM in case of error
 * \return ERR_NOERR otherwise
 */
int MCDC3006SDriver::enable() {
    if (enableDriver(mcdc3006Config->getRS232().RSd, mcdc3006Config->getSemaphore().semID) != ERR_NOERR){
        mcdc3006Status->isEnabled(false);
        return ERR_COM;
    }
    mcdc3006Status->isEnabled(true);
    return ERR_NOERR;
}

/**
 * Disables the driver
 * \return ERR_COM in case of error
 * \return ERR_NOERR otherwise
 */
int MCDC3006SDriver::disable() {
    if (disableDriver(mcdc3006Config->getRS232().RSd, mcdc3006Config->getSemaphore().semID) != ERR_NOERR){
        return ERR_COM;
    }
    mcdc3006Status->isEnabled(false);
    return ERR_NOERR;
}



// Status Section -------------------------------------------------------------------------------------------------------------------------------------------------
// Status Getters
/**
 * @brief Asks the driver if the current is being limitated.
 * @return MCDC3006SStatus::currentLimitated True: the current is being limited.
 *
 * \warning This function calls the driver using the function updateStatus() If you don't really need to call the driver it's better to use DriverInterface::isCurrentLimitated()
 * \see updateStatus()
 * \see DriverInterface.h \see DriverInterface.cpp
 * * @todo Error control here
 */
bool MCDC3006SDriver::isCurrentLimitated() {
    updateStatus();
    return mcdc3006Status->currentLimitated;
}

/**
 * @brief Asks if the driver is enabled or disabled. True = driver enabled. False = driver disabled.
 * @return MCDC3006SStatus::enabled True if it is enabled.
 * \warning This function calls the driver using the function updateStatus() If you don't really need to call the driver it's better to use DriverInterface::isEnabled()
 * \see updateStatus()
 * \see DriverInterface.h \see DriverInterface.cpp
 * * \todo Error control here
 */
bool MCDC3006SDriver::isEnabled() {
    updateStatus();
    return mcdc3006Status->enabled;
}

/**
 * @brief Asks the driver if the limit sensor is has been reached. True the sensor has been reached.
 * @return MCDC3006SStatus::limitSensorReached.. True: the limit sensor is producing an output. Thus the limit sensor has been reached.
 * \warning This function calls the driver using the function updateStatus() If you don't really need to call the driver it's better to use DriverInterface::isLimitSensorReached()
 * \see updateStatus()
 * \see DriverInterface::isLimitSensorReached()
 * \see DriverInterface.h \see DriverInterface.cpp
 * * \todo Error control here
 */
bool MCDC3006SDriver::isLimitSensorReached() {
    updateStatus();
    return mcdc3006Status->limitSensorReached;
}

/**
 * @brief Asks the driver if the overTemperatureWarning has been triggered (there is an over Temperature error in the driver). True = overTemperatureWarning.
 * @return drvStatus->overTemperatureWarning. True: warning.
 *
 * \warning This function calls the driver using the function updateStatus() If you don't really need to call the driver it's better to use DriverInterface::isOverTemperatureWarning()
 * \see updateStatus()
 * \see DriverInterface::isOverTemperatureWarning()
 * \see DriverInterface.h \see DriverInterface.cpp
 * * @todo Error control here
 */
bool MCDC3006SDriver::isOverTemperatureWarning() {
    updateStatus();
    return mcdc3006Status->overTemperatureWarning;
}

/**
 * @brief Checks if the overTemperatureWarning has been triggered (there is an overvoltage error in the driver). True = overVoltageWarning.
 * @return MCDC3006SStatus::overVoltageWarning. True: warning.
 *
 * \warning This function calls the driver using the function updateStatus() If you don't really need to call the driver it's better to use DriverInterface::isOverVoltageWarning()
 * \see updateStatus()
 * \see DriverInterface::isOverVoltageWarning
 * \see DriverInterface.h \see DriverInterface.cpp
 *
 * * @todo Error control here
 */
bool MCDC3006SDriver::isOverVoltageWarning() {
    updateStatus();
    return mcdc3006Status->overVoltageWarning;
}

/**
 * @brief checks if the the calibration flag is calibrated or not.
 * @return MCDC3006SStatus::calibrated The calibration flag. true = the driver is calibrated. false = the driver is not calibrated
 *
 * \warning This function calls the driver using the function updateStatus() If you don't really need to call the driver it's better to use DriverInterface::isCalibrated()
 * \see updateStatus()
 * \see DriverInterface::isCalibrated
 * \see DriverInterface.h \see DriverInterface.cpp
 */
bool MCDC3006SDriver::isCalibrated() {
    updateStatus();
    return mcdc3006Status->calibrated;
}

/**
 * Reads the status of the driver and stores it in a MCDC3006SStatus object
 * \see MCDC3006SStatus
 * \return ERR_COM in case of error
 * \return ERR_NOERR otherwise
 */
int MCDC3006SDriver::updateStatus(){
    if (getDriverStatus(mcdc3006Config->rs232.RSd,
                        mcdc3006Config->semaphore.semID,
                        &mcdc3006Status->statusData) != ERR_NOERR)
    {
        return ERR_COM;
    }
    mcdc3006Status->statusData.curLimiting = mcdc3006Status->currentLimitated;
    mcdc3006Status->statusData.disabled = !(mcdc3006Status->enabled);
    mcdc3006Status->statusData.overTemperature = mcdc3006Status->overTemperatureWarning;
    mcdc3006Status->statusData.overVoltage = mcdc3006Status->overVoltageWarning;
    mcdc3006Status->statusData.sensorReached = mcdc3006Status->limitSensorReached;
    return ERR_NOERR;
}


// Motor Sensor Section -------------------------------------------------------------------------------------------------------------------------------------------------
// Motor Sensor Access Functions

/**
 * \brief returns  the last position stored in the Motor Sensor
 * \return MCDC3006S::position the last stored position in the MCDC3006SMotorSensor::position
 * \warning this position may not be the current position of the driver. If you want to retrieve the actual position of the driver use MCDC3006SDriver::updatePosition()
 */
long int MCDC3006SDriver::getPosition() const{
    return mcdc3006MotorSens->position;
}

/**
 * \brief returns  the last velocity stored in the Motor Sensor
 * \return MCDC3006S::velocity the last stored velocity in the MCDC3006SMotorSensor::velocity
 * \warning this velocity may not be the current velocity of the driver. If you want to retrieve the actual velocity of the driver use MCDC3006SDriver::updateVelocity()
 */
long int MCDC3006SDriver::getVelocity() const{
    return mcdc3006MotorSens->velocity;
}

/**
 * \brief returns  the last Instant Current data stored in the Motor Sensor
 * \return MCDC3006S::instantCurrent the last stored Instant Current in the MCDC3006SMotorSensor::instantCurrent
 * \warning this instant Current may not be the current instantCurrent of the driver. If you want to retrieve the actual Instant Current of the driver use MCDC3006SDriver::updateInstantCurrent()
 */
int MCDC3006SDriver::getInstantCurrent() const{
    return mcdc3006MotorSens->instantCurrent;
}

/**
 * Reads from the driver an update of the current position and stores it in MCDC3006SDriver::mcdc3006MotorSens ( MCDC3006SMotorSensor::position )
 * \note if you want to access the last Motor Sensor data without consulting the driver (although you can retrieve old Motor Sensor data) you can use DriverInterface::getPosition()
 * \return ERR_COM in case of error
 * \return ERR_NOERR otherwise
 */
int MCDC3006SDriver::updatePosition() {
    if (getDriverInstantPos(mcdc3006Config->rs232.RSd,
                            mcdc3006Config->semaphore.semID, &mcdc3006MotorSens->position) != ERR_NOERR)
    {
        perror ("Error updating the position\n");
        return ERR_COM;
    }
    return ERR_NOERR;
}

/**
 * Reads from the driver an update of the current velocity and stores it in MCDC3006SDriver::mcdc3006MotorSens ( MCDC3006SMotorSensor::velocity )
 * \note if you want to access the last Motor Sensor data without consulting the driver (although you can retrieve old Motor Sensor data) you can use DriverInterface::getVelocity()
 * \return ERR_COM in case of error
 * \return ERR_NOERR otherwise
 */
int MCDC3006SDriver::updateVelocity() {
    if (getDriverInstantVel(mcdc3006Config->rs232.RSd,
                            mcdc3006Config->semaphore.semID, &mcdc3006MotorSens->velocity) != ERR_NOERR)
    {
        perror ("Error updating the velocity\n");
        return ERR_COM;
    }
    return ERR_NOERR;
}

/**
 * Reads from the driver an update of the instant current and stores it in MCDC3006SDriver::mcdc3006MotorSens ( MCDC3006SMotorSensor::instantCurrent )
 * \note if you want to access the last Motor Sensor data without consulting the driver (although you can retrieve old Motor Sensor data) you can use DriverInterface::getInstantCurrent()
 * \return ERR_COM in case of error
 * \return ERR_NOERR otherwise
 */
int MCDC3006SDriver::updateInstantCurrent() {
    if (getDriverInstantCurrent(mcdc3006Config->rs232.RSd,
                                mcdc3006Config->semaphore.semID,
                                &mcdc3006MotorSens->instantCurrent) != ERR_NOERR)
    {
        perror ("Error updating the Instant Current");
        return ERR_COM;
    }
    return ERR_NOERR;
}

/**
 * Reads from the driver an update of all the Motor Sensor data and stores it in MCDC3006SDriver::mcdc3006MotorSens
 * \return ERR_COM in case of error
 * \return ERR_NOERR otherwise
 */
int MCDC3006SDriver::updateMotorSensorInfo() {
    if (updatePosition() != ERR_NOERR){
        return ERR_COM;
    }
    if (updateVelocity() != ERR_NOERR){
        return ERR_COM;
    }
    if (updateInstantCurrent() != ERR_NOERR){
        return ERR_COM;
    }
    return ERR_NOERR;
}


// Configuration Section -------------------------------------------------------------------------------------------------------------------------------------------------

/**
 * \brief function to write the configuration stored in mcdc3006SConfig to the driver
 * \return ERR_NOERR if success
 * \return ERR_CONF if any error occurs
 * \todo error control
 */
int MCDC3006SDriver::writeConfigToDriver() {
    if (setDriverMinPos( mcdc3006Config->rs232.RSd,mcdc3006Config->semaphore.semID,
                         mcdc3006Config->minPosition ) < ERR_NOERR) {
        perror("writeConfigToDriver() --> Error setting the minimum position to the driver\n\r");
        return ERR_CONF;
    }
    if (setDriverMaxPos(mcdc3006Config->rs232.RSd,mcdc3006Config->semaphore.semID,
                        mcdc3006Config->maxPosition) < ERR_NOERR) {
        perror("setDriverConf() --> Error setting the maximum position to the driver\n\r");
        return ERR_CONF;
    }
    if ( setDriverMaxVel (mcdc3006Config->rs232.RSd,mcdc3006Config->semaphore.semID,
                          mcdc3006Config->maxVelocity) < ERR_NOERR) {
        perror("setDriverConf() --> Error setting the maximum velocity to the driver\n\r");
        return ERR_CONF;
    }
    if ( setDriverMaxAcc (mcdc3006Config->rs232.RSd,mcdc3006Config->semaphore.semID,
                          mcdc3006Config->maxAcceleration) < ERR_NOERR) {
        perror("setDriverConf() --> Error writing the maximum acceleration to the driver\n\r");
        return ERR_CONF;
    }
    if ( setDriverMaxDec (mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID,
                          mcdc3006Config->maxDecceleration) < ERR_NOERR){
        perror("setDriverConf() --> Error writing the maximum deceleration to the driver\n\r");
        return ERR_CONF;
    }
    if ( setDriverCurLim(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID,
                         mcdc3006Config->continousCurrentLimit) < ERR_NOERR){
        perror("setDriverConf() --> Error writing the continuous current limit to the driver\n\r");
        return ERR_CONF;
    }

    if ( setDriverPCurLim(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID,
                          mcdc3006Config->peakCurrentLimit) < ERR_NOERR){
        perror("setDriverConf() --> Error writing the peak current limit to the driver\n\r");
        return ERR_CONF;
    }
    return ERR_NOERR;
}


/**
 * \brief function that loads the configuration from the driver and stores it in mcdc3006Config
 * \return ERR_NOERR if success
 * \return ERR_CONF if any error occurs
 * \note{ <b>Note for maintainers:</b> If you want to add more features you also have add this features to
 * 				MCDC3006SDriver::writeConfigToDriver() }
 *
 * \todo the function itself
 */
int MCDC3006SDriver::loadConfigFromDriver() {
    maggiePrint("Not implemented");
    return 0;
}

/**
 * \brief function that loads the configuration from mcdc3006Config and stores it in an xml file
 * \param filename filename of the xml file where to store the configuration
 * \return ERR_NOERR if success
 * \return ERR_FILE_ACCESS
 * \return ERR_CONF if any error occurs
 * \note{ <b>Note for maintainers:</b> If you want to add more features you also have add this features to
 * 				MCDC3006SDriver::loadConfigFromDriver() }
 * \warning { This function saves to a file the driver configuration stored in the program memory. Before you use
 * 					this function you may want to load the configuration from the driver to the program memory }
 * \see MCDC3006SDriver::writeConfigToDriver()
 * \see MCDC3006SDriver::loadConfigFromDriver()
 */
int MCDC3006SDriver::writeConfigToFile(std::string filename) {
    mxml_node_t *xml;    					/* <?xml ... ?> */
    mxml_node_t *driver;   					/* <driver> */
    mxml_node_t *driverName;   				/* <driverName> */
    mxml_node_t *driverID;   				/* <driverID> */
    mxml_node_t *driverType;   				/* <driverType> */
    mxml_node_t *mcdc3006s;  				/* <mcdc3006s> */
    mxml_node_t *maxPos;					/* <maxPos> */
    mxml_node_t *minPos;					/* <minPos> */
    mxml_node_t *maxVel;					/* <maxVel> */
    mxml_node_t *maxAcc;					/* <maxAcc> */
    mxml_node_t *maxDec;					/* <maxDecc> */
    mxml_node_t *contCurrent;				/* <contCurrent> */
    mxml_node_t *peakCurrent;				/* <peakCurrent> */
    mxml_node_t *calibrationSensorPosition;	/* <calibrationSensorPosition> */
    mxml_node_t *calibrationCurrentLimit;	/* <calibrationCurrentLimit> */
    mxml_node_t *calibrationSpeed;			/* <calibrationSpeed> */
    mxml_node_t *calibrationTimeOut;		/* <calibrationTimeOut> */
    mxml_node_t *encPulsesPerRev;			/* <encPulsesPerRev> */
    mxml_node_t *driverMultipFactor;		/* <driverMultipFactor> */
    mxml_node_t *reductionFactor;			/* <reductionFactor> */
    mxml_node_t *rs232;						/** <rs232> */
    mxml_node_t *serialDevice;				/** <serialDevice */
    mxml_node_t *baudRate;					/** <baudRate> */
    mxml_node_t *semaphore;				/** <semaphore>*/
    mxml_node_t *semFile;					/** <semFile> */

    FILE *configFile;
    configFile = fopen(filename.data(),"w");
    if (configFile == NULL)	{
        fprintf(stderr, "Error accessing the file %s. ",filename.data() );
        perror("Can't open the file");
        return ERR_FILE_ACCESS;
    }

    string auxString;
    /*<?xml version="1.0"?> */
    xml = mxmlNewXML("1.0");
    /*<driver>*/
    driver = mxmlNewElement(xml, "driver");
    /*<driverName>*/
    driverName = mxmlNewElement (driver, "driverName");
    mxmlNewText(driverName, 0, mcdc3006Config->driverName.data());
    /*</driverName>*/
    /*<driverID>*/
    driverID = mxmlNewElement(driver, "driverID");
    auxString = XmlUtils::intToString (mcdc3006Config->driverID);			// converting the integer to ascii
    mxmlNewText(driverID, 0,  auxString.data());
    /*</driverID>*/
    /*<driverType>*/
    driverType = mxmlNewElement(driver, "driverType");
    mxmlNewText(driverType, 0, mcdc3006Config->driverType.data());
    /*</driverType>*/
    /*<mcdc3006s>*/
    mcdc3006s = mxmlNewElement(driver, "mcdc3006s");

    maxPos = mxmlNewElement(mcdc3006s, "maxPos");
    auxString = XmlUtils::intToString (mcdc3006Config->maxPosition);	// converting the integer to ascii
    mxmlNewText(maxPos,0,auxString.data());

    minPos = mxmlNewElement(mcdc3006s, "minPos");
    auxString = XmlUtils::intToString (mcdc3006Config->minPosition);	// converting the integer to ascii
    mxmlNewText(minPos,0,auxString.data());

    maxVel = mxmlNewElement(mcdc3006s, "maxVel");
    auxString = XmlUtils::intToString (mcdc3006Config->maxVelocity);	// converting the integer to ascii
    mxmlNewText(maxVel,0,auxString.data());

    maxAcc = mxmlNewElement(mcdc3006s, "maxAcc");
    auxString = XmlUtils::intToString (mcdc3006Config->maxAcceleration);		// converting the integer to ascii
    mxmlNewText(maxAcc,0,auxString.data());

    maxDec = mxmlNewElement(mcdc3006s, "maxDec");
    auxString = XmlUtils::intToString (mcdc3006Config->maxDecceleration);		// converting the integer to ascii
    mxmlNewText(maxDec,0,auxString.data());

    contCurrent = mxmlNewElement(mcdc3006s, "contCurrent");
    auxString = XmlUtils::intToString (mcdc3006Config->continousCurrentLimit);// converting the integer to ascii
    mxmlNewText(contCurrent,0,auxString.data());

    peakCurrent = mxmlNewElement(mcdc3006s, "peakCurrent");
    auxString = XmlUtils::intToString (mcdc3006Config->peakCurrentLimit);		// converting the integer to ascii
    mxmlNewText(peakCurrent,0,auxString.data());

    calibrationCurrentLimit = mxmlNewElement(mcdc3006s, "calibrationCurrentLimit");
    auxString = XmlUtils::intToString (mcdc3006Config->calibrationCurrentLimit);	// converting the integer to ascii
    mxmlNewText(calibrationCurrentLimit,0,auxString.data());

    calibrationSensorPosition= mxmlNewElement(mcdc3006s, "calibrationSensorPosition");
    auxString = XmlUtils::intToString (mcdc3006Config->calibrationSensorPosition);// converting the integer to ascii
    mxmlNewText(calibrationSensorPosition,0,auxString.data());

    calibrationSpeed = mxmlNewElement(mcdc3006s, "calibrationSpeed");
    auxString = XmlUtils::intToString (mcdc3006Config->calibrationSpeed);			// converting the integer to ascii
    mxmlNewText(calibrationSpeed,0,auxString.data());

    calibrationTimeOut = mxmlNewElement(mcdc3006s, "calibrationTimeOut");
    auxString = XmlUtils::intToString (mcdc3006Config->calibrationTimeOut);	// converting the integer to ascii
    mxmlNewText(calibrationTimeOut,0,auxString.data());

    encPulsesPerRev = mxmlNewElement(mcdc3006s, "encPulsesPerRev");
    auxString = XmlUtils::intToString (mcdc3006Config->getEncoderPulsesPerRevolution());	// converting the integer to ascii
    mxmlNewText(encPulsesPerRev,0,auxString.data());

    driverMultipFactor = mxmlNewElement(mcdc3006s, "driverMultipFactor");
    auxString = XmlUtils::intToString (mcdc3006Config->getDriverMultiplicationFactor());	// converting the integer to ascii
    mxmlNewText(driverMultipFactor,0,auxString.data());

    reductionFactor = mxmlNewElement(mcdc3006s, "reductionFactor");
    auxString = XmlUtils::intToString (mcdc3006Config->getReductionFactor());			// converting the integer to ascii
    mxmlNewText(reductionFactor,0,auxString.data());


    /*<rs232> */
    rs232 = mxmlNewElement(mcdc3006s, "rs232");
    serialDevice = mxmlNewElement(rs232,"serialDevice");
    mxmlNewText(serialDevice,0,mcdc3006Config->rs232.serialDevice);

    baudRate = mxmlNewElement(rs232,"baudRate");
    auxString = XmlUtils::intToString (mcdc3006Config->rs232.baudrate);	// converting the integer to ascii
    mxmlNewText(baudRate,0,auxString.data());
    /*<rs232> */

    /*<semaphore>*/
    semaphore = mxmlNewElement(mcdc3006s, "semaphore");
    /*<semFile>*/
    semFile = mxmlNewElement(semaphore, "semFile");
    mxmlNewText(semFile,0,mcdc3006Config->semaphore.semFile);
    /*</semFile>*/
    /*</semaphore>*/

    /*</mcdc3006s>*/
    /*</driver>*/

    //	cout << "\n\nPrinting the file in the screen" << endl;
    //	mxmlSaveFile(xml,stdout,tabbed_cb);

    // Saving the configuration to a file
    if (mxmlSaveFile(xml,configFile,XmlUtils::tabbed_cb) != 0){
        fclose(configFile);
        return (ERR_CONF);
    }

    fclose(configFile);
    return (ERR_NOERR);
}

/**
 * \brief function that loads the configuration from a file and stores it in mcdc3006Config
 * \param filename the file where to get the configuration
 * \return ERR_NOERR if success
 * \return ERR_FILE_ACCESS if couldn't access to the file
 * \return ERR_CONF if any error occurs
 * \warning {This function loads the configuration from the file into the program memory.
 * 				This function DOES NOT write the loaded configuration into the driver. If you want to store the loaded
 * 					configuration in the driver you must call MCDC3006SDriver::writeConfigToDriver() after loading the
 * 					configuration from the file}
 * \see MCDC3006SDriver::writeConfigToDriver()
 */
int MCDC3006SDriver::loadConfigFromFile(std::string filename){
    FILE *configFile;
    mxml_node_t *xmlConfig;			// Used to store the xml tree from the config file
    mxml_index_t *ind;						// Index of the configuration tree
    string parameterName;				// used to store the name of each Xml tag
    string parameterData;					// used to store the data of the parameters as we are reading them
    bool error = false;					// used to monitor if there has been any errors

    // add LONG_TERM_MEMORY_DIR to filename
    std::string PATH_MOTOR_DRIVERS_PROJECT = ros::package::getPath("motor_drivers");
    filename = std::string(PATH_MOTOR_DRIVERS_PROJECT + "/data/motor_configs/") + filename;

    configFile = fopen(filename.data(),"r");
    if (configFile==NULL)	{
        fprintf(stderr, "Error accessing the file %s. ",filename.data() );
        perror("Can't open the file");
        return ERR_FILE_ACCESS;
    }

    xmlConfig = mxmlLoadFile(NULL, configFile, MXML_TEXT_CALLBACK); // Loading from file
    ind = mxmlIndexNew(xmlConfig, NULL, NULL);									// Generating the index

    // Reading the generic parameters:
    parameterName = "driverName";
    if ( XmlUtils::XmlUtils::xmlFindParam (ind,parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading driverName parameter");
        error = true;
    }
    mcdc3006Config->driverName = parameterData.data();

    parameterName = "driverID";
    if ( XmlUtils::XmlUtils::xmlFindParam (ind, parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading driverID parameter");
        error = true;
    }
    mcdc3006Config->driverID = atoi (parameterData.data());

    parameterName = "driverType";
    if ( XmlUtils::xmlFindParam (ind,parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading driverType parameter");
        error = true;
    }
    mcdc3006Config->driverType = parameterData.data();


    // Reading the RS232 related Parameters
    parameterName = "serialDevice";
    if ( XmlUtils::xmlFindParam (ind,parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading serialDevice parameter");
        error = true;
    }
    sprintf(mcdc3006Config->rs232.serialDevice,"%s",parameterData.data());


    parameterName = "baudRate";
    if ( XmlUtils::xmlFindParam (ind,parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading baudRate parameter");
        error = true;
    }
    mcdc3006Config->rs232.baudrate = atoi(parameterData.data());

    // Now reading the semaphore related parameters
    parameterName = "semFile";
    if ( XmlUtils::xmlFindParam (ind,parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading semFile parameter");
        error = true;
    }
    sprintf(mcdc3006Config->semaphore.semFile, "%s",parameterData.data());

    // The limits
    parameterName = "maxPos";
    if ( XmlUtils::xmlFindParam (ind,parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading MaxPos parameter");
        error = true;
    }
    mcdc3006Config->maxPosition = atol(parameterData.data());

    parameterName = "minPos";
    if ( XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading MinPos parameter");
        error = true;
    }
    mcdc3006Config->minPosition = atol(parameterData.data());

    parameterName = "maxVel";
    if ( XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading MaxVel parameter");
        error = true;
    }
    mcdc3006Config->maxVelocity = atol(parameterData.data());

    parameterName = "maxAcc";
    if ( XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading MaxAcc parameter");
        error = true;
    }
    mcdc3006Config->maxAcceleration = atol(parameterData.data());

    parameterName = "maxDec";
    if ( XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading MaxDec parameter");
        error = true;
    }
    mcdc3006Config->maxDecceleration = atol(parameterData.data());

    parameterName = "contCurrent";
    if ( XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading contCurrent parameter");
        error = true;
    }
    mcdc3006Config->continousCurrentLimit = atol(parameterData.data());

    parameterName = "peakCurrent";
    if ( XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading peakCurrent parameter");
        error = true;
    }
    mcdc3006Config->peakCurrentLimit = atol(parameterData.data());


    // Calibration parameters
    parameterName = "calibrationSensorPosition";
    if ( XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading peakCurrent parameter");
        error = true;
    }
    mcdc3006Config->calibrationSensorPosition = atol(parameterData.data());

    parameterName = "calibrationCurrentLimit";
    if ( XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading peakCurrent parameter");
        error = true;
    }
    mcdc3006Config->calibrationCurrentLimit = atol(parameterData.data());

    parameterName = "calibrationSpeed";
    if ( XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading peakCurrent parameter");
        error = true;
    }
    mcdc3006Config->calibrationSpeed = atol(parameterData.data());

    parameterName = "calibrationTimeOut";
    if ( XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading peakCurrent parameter");
        error = true;
    }
    mcdc3006Config->calibrationTimeOut = atol(parameterData.data());

    // Measure Units
    parameterName = "encPulsesPerRev";
    if ( XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading encPulsesPerRev parameter");
        error = true;
    }
    mcdc3006Config->setEncoderPulsesPerRevolution(atol(parameterData.data()));

    parameterName = "driverMultipFactor";
    if ( XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading driverMultipFactor parameter");
        error = true;
    }
    mcdc3006Config->setDriverMultiplicationFactor(atoi(parameterData.data()));

    parameterName = "reductionFactor";
    if ( XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading reductionFactor parameter");
        error = true;
    }
    mcdc3006Config->setReductionFactor(atoi(parameterData.data()));

    // With the measure units we have enough information to set the pulsesPerRevolution parameter
    // and to set the conversion units
    mcdc3006Config->setPulsesPerRevolution();
    //	mcdc3006Config->setPulsesToRad();


    //	mxmlSaveFile(xmlConfig,stdout,tabbed_cb);	// Used for debbuging

    fclose(configFile);
    if (error == true){			// We notify if there have been errors
        return (ERR_CONF);
    }
    return (ERR_NOERR);
}


// Configuration Access Functions (Getters)
/**
 * @brief Retrieves from the driver the MaxPosition parameter
 * @return MCDC3006SConfiguration::maxPosition Units: pulses
 * \see MCDC3006SConfiguration.h
 * \todo error control
 */
long int MCDC3006SDriver::getMaxPosition(){
    mcdc3006Config->maxPosition = getDriverMaxPos(mcdc3006Config->rs232.RSd,mcdc3006Config->semaphore.semID);
    if (mcdc3006Config->maxPosition != ERR_NOERR){
        /** Todo Throw exception here*/
    }
    return mcdc3006Config->maxPosition;
}

/**
 * @brief Retrieves from the driver the MinPosition parameter
 * @return MCDC3006SConfiguration::minPosition Units: pulses
 * \see MCDC3006SConfiguration.h
 * \todo error control
 */
long int MCDC3006SDriver::getMinPosition(){
    mcdc3006Config->minPosition = getDriverMinPos(mcdc3006Config->rs232.RSd,mcdc3006Config->semaphore.semID);
    if (mcdc3006Config->minPosition != ERR_NOERR){
        /** Todo Throw exception here*/
    }
    return mcdc3006Config->minPosition;
}

/**
 * @brief Retrieves from the driver the MaxVelocity parameter
 * @return MCDC3006SConfiguration::maxVelocity Units: rpm
 * \see MCDC3006SConfiguration.h
 * \todo error control
 */
long int MCDC3006SDriver::getMaxVelocity(){
    mcdc3006Config->maxVelocity = getDriverMaxVel(mcdc3006Config->rs232.RSd,
                                                  mcdc3006Config->semaphore.semID);
    if (mcdc3006Config->maxVelocity != ERR_NOERR){
        /** Todo Throw exception here*/
    }
    return mcdc3006Config->maxVelocity;
}

/**
 * @brief Retrieves from the driver the MaxAcceleration parameter
 * @return MCDC3006SConfiguration::maxAcceleration Units:
 * \see MCDC3006SConfiguration.h
 * \todo error control
 */
long int MCDC3006SDriver::getMaxAcceleration(){
    mcdc3006Config->maxAcceleration = getDriverMaxAcc(mcdc3006Config->rs232.RSd,
                                                      mcdc3006Config->semaphore.semID);
    if (mcdc3006Config->maxAcceleration != ERR_NOERR){
        /** Todo Throw exception here*/
    }
    return mcdc3006Config->maxAcceleration;
}

/**
 * @brief Retrieves from the driver the MaxDecceleration
 * @return MCDC3006SConfiguration::maxDecceleration Units:
 * \see MCDC3006SConfiguration.h
 * \todo error control
 */
long int MCDC3006SDriver::getMaxDecceleration(){
    mcdc3006Config->maxDecceleration = getDriverMaxDec(mcdc3006Config->rs232.RSd,
                                                       mcdc3006Config->semaphore.semID);
    if (mcdc3006Config->maxDecceleration != ERR_NOERR){
        /** Todo Throw exception here*/
    }
    return mcdc3006Config->maxDecceleration;
}

/**
 * @brief Retrieves from the driver the ContinuousCurrentLimit parameter
 * @return MCDC3006SConfiguration::continousCurrentLimit Units: mA
 * \see MCDC3006SConfiguration.h
 * \todo error control
 */
int MCDC3006SDriver::getContinousCurrentLimit(){
    mcdc3006Config->continousCurrentLimit = getDriverCurLim (mcdc3006Config->rs232.RSd,
                                                             mcdc3006Config->semaphore.semID);
    if (mcdc3006Config->continousCurrentLimit != ERR_NOERR){
        /** Todo Throw exception here*/
    }
    return mcdc3006Config->continousCurrentLimit;
}

/**
 * @brief Retrieves from the driver the MaxPosition parameter
 * @return MCDC3006SConfiguration::peakCurrentLimit Units: mA
 * \see MCDC3006SConfiguration.h
 * \todo error control
 */
int MCDC3006SDriver::getPeakCurrentLimit(){
    mcdc3006Config->peakCurrentLimit = getDriverPCurLim(mcdc3006Config->rs232.RSd,
                                                        mcdc3006Config->semaphore.semID);
    if (mcdc3006Config->peakCurrentLimit != ERR_NOERR){
        /** Todo Throw exception here*/
    }
    return mcdc3006Config->peakCurrentLimit;
}

/**
 * @brief Returns configuration filename stored in the configuration.
 * @return MCDC3006SConfiguration::configFile The configuration filename
 * \see MCDC3006SConfiguration.h
 */
string MCDC3006SDriver::getConfigFile(){
    return mcdc3006Config->configFile;
}

// Configuration Access Functions (Setters)

/**
 * \brief Sets the driver maxPosition and if succeeds updates the driverConfiguration::maxPosition with the entered parameter
 * \param maxPosition The value to set the maximum position in the driver. Units: pulses
 * \return ERR_CONF in case of failing to set the parameter into the driver
 * \return ERR_NOERR otherwise
 */
int MCDC3006SDriver::setMaxPosition(long int maxPosition){
    if (setDriverMaxPos(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID,
                        maxPosition) != ERR_NOERR){
        return ERR_CONF;
    }
    DriverInterface::setMaxPosition(maxPosition);
    return ERR_NOERR;
}

/**
 * \brief Sets the driver minPosition and if succeeds updates the driverConfiguration::minPosition with the entered parameter
 * \param minPosition The value to set the minimum position in the driver. Units: pulses
 * \return ERR_CONF in case of failing to set the parameter into the driver
 * \return ERR_NOERR otherwise
 */
int MCDC3006SDriver::setMinPosition(long int minPosition){
    if (setDriverMinPos(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID,
                        minPosition) != ERR_NOERR){
        return ERR_CONF;
    }
    DriverInterface::setMinPosition(minPosition);
    return ERR_NOERR;
}

/**
 * \brief Sets the driver maxVelocity and if succeeds updates the driverConfiguration::maxVelocity with the entered parameter
 * \param maxVelocity The value to set the maximum velocity in the driver. Units: rpm
 * \return ERR_CONF in case of failing to set the parameter into the driver
 * \return ERR_NOERR otherwise
 */
int MCDC3006SDriver::setMaxVelocity(long int maxVelocity){
    if (setDriverMaxVel(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID,
                        maxVelocity) != ERR_NOERR){
        return ERR_CONF;
    }
    DriverInterface::setMaxVelocity(maxVelocity);
    return ERR_NOERR;
}

/**
 * \brief Sets the driver maxAcceleration and if succeeds updates the driverConfiguration::maxAcceleration with the entered parameter
 * \param maxAcceleration The value to set the maximum acceleration in the driver. Units:
 * \return ERR_CONF in case of failing to set the parameter into the driver
 * \return ERR_NOERR otherwise
 */
int MCDC3006SDriver::setMaxAcceleration(long int maxAcceleration){
    if (setDriverMaxAcc(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID,
                        maxAcceleration) != ERR_NOERR){
        return ERR_CONF;
    }
    DriverInterface::setMaxAcceleration(maxAcceleration);
    return ERR_NOERR;
}

/**
 * \brief Sets the driver maxDecceleration and if succeeds updates the driverConfiguration::maxDecceleration with the entered parameter
 * \param maxDecceleration The value to set the maximum decceleration in the driver. Units:
* \return ERR_CONF in case of failing to set the parameter into the driver
 * \return ERR_NOERR otherwise
 */
int MCDC3006SDriver::setMaxDecceleration(long int maxDecceleration){
    if (setDriverMaxDec(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID,
                        maxDecceleration) != ERR_NOERR){
        return ERR_CONF;
    }
    DriverInterface::setMaxDecceleration(maxDecceleration);
    return ERR_NOERR;
}

/**
 * \brief Sets the driver continousCurrentLimit and if succeeds updates the DriverConfiguration::continousCurrentLimit with the entered parameter
 * \param continousCurrentLimit The value to set the continous current limit in the driver. Units: mA
 * \return ERR_CONF in case of failing to set the parameter into the driver
 * \return ERR_NOERR otherwise
 */
int MCDC3006SDriver::setContinousCurrentLimit(int continousCurrentLimit){
    if (setDriverCurLim(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID,
                        continousCurrentLimit) != ERR_NOERR){
        return ERR_CONF;
    }
    DriverInterface::setContinousCurrentLimit(continousCurrentLimit);
    return ERR_NOERR;
}

/**
 * \brief Sets the driver peakCurrentLimit and if succeeds updates the DriverConfiguration::PeakCurrentLimit with the entered parameter
 * \param peakCurrentLimit The value to set the peak current Limit in the driver. Units: mA
 * \return ERR_CONF in case of failing to set the parameter into the driver
 * \return ERR_NOERR otherwise
 */
int MCDC3006SDriver::setPeakCurrentLimit(int peakCurrentLimit){
    if (setDriverPCurLim(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID,
                         peakCurrentLimit) != ERR_NOERR){
        return ERR_CONF;
    }
    DriverInterface::setPeakCurrentLimit(peakCurrentLimit);
    return ERR_NOERR;
}

/**
 * \brief sets the configuration file
 * \param confFile the cofiguration file name
 * \todo error control
 */
int MCDC3006SDriver::setConfigFile(std::string confFile){
    mcdc3006Config->configFile = confFile;
    return 0;
}


// Specific configuration getters and setters (only used by MCDC3006S and not other drivers)
/**
 * \brief gets the serial device filename
 * \return The file name of the serial port where the driver is attached
 */
string MCDC3006SDriver::getSerialDevice (){
    string deviceName;
    deviceName = mcdc3006Config->getRS232().serialDevice;
    return deviceName;
}

/**
 * \brief Returns the baudrate of the serial port at which the driver is connected
 * \return the baudrate of the serial port at which the driver is connected
 */
int MCDC3006SDriver::getBaudrate (){
    return mcdc3006Config->getRS232().baudrate;
}

/**
 * \brief gets the filename used by the driver's semaphore to generate the unique identificator of the driver's semaphore
 * \return the semaphore file name
 */
string MCDC3006SDriver::getSemFile(){
    string semName;
    semName = mcdc3006Config->getSemaphore().semFile;
    return semName;
}

/**
 * \brief sets the filename of the serial Port at wich the driver will connect
 * \param serialDevice the filename of the serial port
 * \note if the entered filename is longer than SP_MAX_FILENAME_SIZE (the maximum serial port filename size) the
 * 				entered string will be truncated
 */
void MCDC3006SDriver::setSerialDevice(std::string serialDevice){
    for (int i = 0; i < SP_MAX_FILENAME_SIZE; i++){
        mcdc3006Config->rs232.serialDevice[i] = serialDevice[i];
    }
}

/**
 * \brief sets the baudrate of the serial port of the driver
 * \param baud the baudrate
 * \todo error control here: The baudrate must be a real baudrate supported by the serial port
 */
void MCDC3006SDriver::setBaudrate (int baud){
    mcdc3006Config->rs232.baudrate = baud;
}

/**
 * \brief sets the filename of the semaphore file which will be used by the driver
 * \param semFile the filename of the semaphore
 * \note if the entered filename is longer than MAX_SEMFILENAME_SIZE (the maximum semaphore filename size) the
 * 				entered string will be truncated
 */
void MCDC3006SDriver::setSemFile(std::string semFile){
    for (int i=0; i < MAX_SEMFILENAME_SIZE; i++){
        mcdc3006Config->semaphore.semFile[i] = semFile[i];
    }
}


/*
 * Function to open and to start the communication with the driver.
 * \return ERR_FILE_ACCESS in case of failing the communication with the driver. It also sets MCDC3006SDriver::mcdc3006Status (MCDC3006SStatus::connected) to false
 * \return ERR_NOERR otherwise. It also sets MCDC3006SStatus::connected to true
 */
int MCDC3006SDriver::connect() {
    if (initCommunication(mcdc3006Config->rs232.baudrate,
                          mcdc3006Config->rs232.serialDevice,
                          &mcdc3006Config->rs232.RSd,
                          mcdc3006Config->semaphore.semFile,
                          &mcdc3006Config->semaphore.semID) != ERR_NOERR)
    {
        mcdc3006Status->connected = false;
        return ERR_FILE_ACCESS;
    }
    mcdc3006Status->connected = true;
    return ERR_NOERR;
}

/*
 * Function to close the driver and end the communication with it
 * \return ERR_FILE_ACCESS in case of failing the communication with the driver. It also sets MCDC3006SDriver::mcdc3006Status (MCDC3006SStatus::connected) to false
 * \return ERR_NOERR otherwise. It also sets MCDC3006SStatus::connected to true
 */
int MCDC3006SDriver::disconnect() {
    if (endCommunication(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID) != ERR_NOERR){
        return ERR_FILE_ACCESS;
    }
    mcdc3006Status->connected = false;
    return ERR_NOERR;
}


/**
 * Prints in stdout all the limits from DriverConfiguration
 */
void MCDC3006SDriver::printLimits () {
    cout << endl;
    cout << "Max Position: " << getMaxPosition() << endl;
    cout << "Min Position: " << getMinPosition() << endl;
    cout << "Max Velocity: " << getMaxVelocity() << endl;
    cout << "Max Acceleration: " << getMaxAcceleration()<< endl;
    cout << "Max Decceleration: " << getMaxDecceleration() << endl;
    cout << "Max Continous Current: " << getContinousCurrentLimit() << endl;
    cout << "Max Peak Current: " << getPeakCurrentLimit() << endl;
    cout << endl;
}


/**
 * Prints in stdout all the non-limits configuration from DriverConfiguration
 */
void MCDC3006SDriver::printConfig() {
    DriverInterface::printConfiguration();
    cout << "Driver Serial Device: " << getSerialDevice() << endl;
    cout << "Driver BaudRate: " << getBaudrate() << endl;
    cout << "Driver SemFile: " << getSemFile() << endl;
    cout << endl;
}


/**
 * Prints in stdout all the Motor Sensor data from DriverMotorSensor
 */
void MCDC3006SDriver::printMotorSensorInfo () {
    cout << endl;
    cout << "-- Driver Motor Sensor --" << endl;
    if (updateMotorSensorInfo() != ERR_NOERR){
        perror ("Could not update the Motor Sensor\n");
    }else {
        cout << "Current Position: " << getPosition() << endl;
        cout << "Current Speed: " << getVelocity() << endl;
        cout << "Current Instant Current: " << getInstantCurrent() << endl;
        cout << endl;
    }
}

/**
 * Prints in stdout all the status data from DriverStatus
 */
void MCDC3006SDriver::printStatus () {
    cout << endl;
    cout << "-- Driver Status -- " << endl;
    cout << "True = yes. False = no" << endl;
    cout << "Driver is enabled: " << boolToString(isEnabled()) << endl;
    cout << "Driver is calibrated: " << boolToString( isCalibrated()) << endl;
    cout << "Current is Limited: " << boolToString(isCurrentLimitated()) << endl;
    cout << "Limit Sensor is reached: " << boolToString(isLimitSensorReached()) << endl;
    cout << "Overtemperature warning: " << boolToString(isOverTemperatureWarning()) << endl;
    cout << "Overvoltage warning: " << boolToString(isOverVoltageWarning()) << endl;
    cout << "Driver is connected: " << boolToString(isConnected()) << endl;
    cout << endl;
}
