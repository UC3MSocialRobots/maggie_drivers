/**
 * @file        MCDC3006SDriver.cpp
 * @brief       Node for controlling the mcdc3006s motors (neck and arms).
 *
 * @author      Raul Perula-Martinez <raul.perula@uc3m.es>
 * @date        2015-03
 * @author      Victor Gonzalez Pacheco <vgonzale@ing.uc3m.es>
 * @date        2010-05
 *
 * @copyright   Copyright (C) 2015 University Carlos III of Madrid.
 *              All rights reserved.
 * @license     LEUC3M v1.0, see LICENSE.txt
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the Licencia Educativa UC3M as published by
 * the University Carlos III of Madrid, either version 1.0, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY. See the Licencia Educativa UC3M
 * version 1.0 or any later version for more details.
 *
 * A copy of the Licencia Educativa UC3M is in the LICENSE file.
 */
#include "mcdc3006s/MCDC3006SDriver.h"

//////////////////////////////////////////////////

MCDC3006SDriver::MCDC3006SDriver(std::string driverConfigFile) :
    DriverInterface()
{

    mcdc3006Config = new MCDC3006SConfiguration(driverConfigFile);
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

//////////////////////////////////////////////////

MCDC3006SDriver::MCDC3006SDriver() :
    DriverInterface::DriverInterface()
{
    mcdc3006Config = new MCDC3006SConfiguration();
    mcdc3006MotorSens = new MCDC3006SMotorSensor();
    mcdc3006Status = new MCDC3006SStatus();

    mcdc3006Status->connected = false;	// At this moment the driver is not yet opened
    this->calibrate(false);		    // This value it should be already in this state, but we do it again for safety

    // The base class will need this pointers
    drvConf = mcdc3006Config;
    drvMotorSensor = mcdc3006MotorSens;
    drvStatus = mcdc3006Status;
}

//////////////////////////////////////////////////

MCDC3006SDriver::~MCDC3006SDriver()
{
    disconnect();

    delete mcdc3006Config;
    delete mcdc3006MotorSens;
    delete mcdc3006Status;

    drvConf = 0;
    drvMotorSensor = 0;
    drvStatus = 0;
}

//////////////////////////////////////////////////

int MCDC3006SDriver::moveDriverToRelativePosition(long int relativePos)
{
    if (moveDriverRelPos(mcdc3006Config->getRS232().RSd, mcdc3006Config->getSemaphore().semID, relativePos) != ERR_NOERR) {
        return ERR_COM;
    }
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int MCDC3006SDriver::moveDriverToAbsolutePosition(long int absolutePos)
{
    if (moveDriverAbsPos(mcdc3006Config->getRS232().RSd, mcdc3006Config->getSemaphore().semID, absolutePos) != ERR_NOERR) {
        return ERR_COM;
    }
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int MCDC3006SDriver::moveDriverByVelocity(long int vel)
{
    if (moveDriverVel(mcdc3006Config->getRS232().RSd, mcdc3006Config->getSemaphore().semID, vel) != ERR_NOERR) {
        return ERR_COM;
    }
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int MCDC3006SDriver::calibrate(long int calibrationSensorPosition,
                               long int calibrationCurrentLimit,
                               long int calibrationSpeed,
                               long int calibrationTimeOut)
{
    if (calibrateDriver(mcdc3006Config->getRS232().RSd, mcdc3006Config->getSemaphore().semID, calibrationSensorPosition, calibrationCurrentLimit, calibrationSpeed, calibrationTimeOut) != ERR_NOERR) {
        mcdc3006Status->isCalibrated(false);
        return ERR_COM;
    }
    mcdc3006Status->isCalibrated(true);
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int MCDC3006SDriver::calibrate(bool calibrated)
{
    if (calibrated == true) { 			// Means that received a command to calibrate the driver
        if (calibrateDriver(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID, mcdc3006Config->calibrationSensorPosition, mcdc3006Config->calibrationCurrentLimit, mcdc3006Config->calibrationSpeed, mcdc3006Config->calibrationTimeOut) != ERR_NOERR) {
            calibrated = false;
            return ERR_COM;  // It hasn't been possible to calibrate the driver
        }
    }
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int MCDC3006SDriver::enable()
{
    if (enableDriver(mcdc3006Config->getRS232().RSd, mcdc3006Config->getSemaphore().semID) != ERR_NOERR) {
        mcdc3006Status->isEnabled(false);
        return ERR_COM;
    }
    mcdc3006Status->isEnabled(true);
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int MCDC3006SDriver::disable()
{
    if (disableDriver(mcdc3006Config->getRS232().RSd, mcdc3006Config->getSemaphore().semID) != ERR_NOERR) {
        return ERR_COM;
    }
    mcdc3006Status->isEnabled(false);
    return ERR_NOERR;
}

//////////////////////////////////////////////////

// Status Section

bool MCDC3006SDriver::isCurrentLimitated()
{
    updateStatus();
    return mcdc3006Status->currentLimitated;
}

//////////////////////////////////////////////////

bool MCDC3006SDriver::isEnabled()
{
    updateStatus();
    return mcdc3006Status->enabled;
}

//////////////////////////////////////////////////

bool MCDC3006SDriver::isLimitSensorReached()
{
    updateStatus();
    return mcdc3006Status->limitSensorReached;
}

//////////////////////////////////////////////////

bool MCDC3006SDriver::isOverTemperatureWarning()
{
    updateStatus();
    return mcdc3006Status->overTemperatureWarning;
}

//////////////////////////////////////////////////

bool MCDC3006SDriver::isOverVoltageWarning()
{
    updateStatus();
    return mcdc3006Status->overVoltageWarning;
}

//////////////////////////////////////////////////

bool MCDC3006SDriver::isCalibrated()
{
    updateStatus();
    return mcdc3006Status->calibrated;
}

//////////////////////////////////////////////////

int MCDC3006SDriver::updateStatus()
{
    if (getDriverStatus(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID, &mcdc3006Status->statusData) != ERR_NOERR) {
        return ERR_COM;
    }
    mcdc3006Status->statusData.curLimiting = mcdc3006Status->currentLimitated;
    mcdc3006Status->statusData.disabled = !(mcdc3006Status->enabled);
    mcdc3006Status->statusData.overTemperature = mcdc3006Status->overTemperatureWarning;
    mcdc3006Status->statusData.overVoltage = mcdc3006Status->overVoltageWarning;
    mcdc3006Status->statusData.sensorReached = mcdc3006Status->limitSensorReached;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

// Motor Sensor Section

long int MCDC3006SDriver::getPosition() const
{
    return mcdc3006MotorSens->position;
}

//////////////////////////////////////////////////

long int MCDC3006SDriver::getVelocity() const
{
    return mcdc3006MotorSens->velocity;
}

//////////////////////////////////////////////////

int MCDC3006SDriver::getInstantCurrent() const
{
    return mcdc3006MotorSens->instantCurrent;
}

//////////////////////////////////////////////////

int MCDC3006SDriver::updatePosition()
{
    if (getDriverInstantPos(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID, &mcdc3006MotorSens->position) != ERR_NOERR) {
        perror("Error updating the position\n");
        return ERR_COM;
    }
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int MCDC3006SDriver::updateVelocity()
{
    if (getDriverInstantVel(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID, &mcdc3006MotorSens->velocity) != ERR_NOERR) {
        perror("Error updating the velocity\n");
        return ERR_COM;
    }
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int MCDC3006SDriver::updateInstantCurrent()
{
    if (getDriverInstantCurrent(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID, &mcdc3006MotorSens->instantCurrent) != ERR_NOERR) {
        perror("Error updating the Instant Current");
        return ERR_COM;
    }
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int MCDC3006SDriver::updateMotorSensorInfo()
{
    if (updatePosition() != ERR_NOERR) {
        return ERR_COM;
    }
    if (updateVelocity() != ERR_NOERR) {
        return ERR_COM;
    }
    if (updateInstantCurrent() != ERR_NOERR) {
        return ERR_COM;
    }
    return ERR_NOERR;
}

//////////////////////////////////////////////////

// Configuration Section

int MCDC3006SDriver::writeConfigToDriver()
{
    if (setDriverMinPos(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID, mcdc3006Config->minPosition) < ERR_NOERR) {
        perror("writeConfigToDriver() --> Error setting the minimum position to the driver\n\r");
        return ERR_CONF;
    }
    if (setDriverMaxPos(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID, mcdc3006Config->maxPosition) < ERR_NOERR) {
        perror("setDriverConf() --> Error setting the maximum position to the driver\n\r");
        return ERR_CONF;
    }
    if (setDriverMaxVel(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID, mcdc3006Config->maxVelocity) < ERR_NOERR) {
        perror("setDriverConf() --> Error setting the maximum velocity to the driver\n\r");
        return ERR_CONF;
    }
    if (setDriverMaxAcc(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID, mcdc3006Config->maxAcceleration) < ERR_NOERR) {
        perror("setDriverConf() --> Error writing the maximum acceleration to the driver\n\r");
        return ERR_CONF;
    }
    if (setDriverMaxDec(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID, mcdc3006Config->maxDecceleration) < ERR_NOERR) {
        perror("setDriverConf() --> Error writing the maximum deceleration to the driver\n\r");
        return ERR_CONF;
    }
    if (setDriverCurLim(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID, mcdc3006Config->continousCurrentLimit) < ERR_NOERR) {
        perror("setDriverConf() --> Error writing the continuous current limit to the driver\n\r");
        return ERR_CONF;
    }

    if (setDriverPCurLim(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID, mcdc3006Config->peakCurrentLimit) < ERR_NOERR) {
        perror("setDriverConf() --> Error writing the peak current limit to the driver\n\r");
        return ERR_CONF;
    }
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int MCDC3006SDriver::loadConfigFromDriver()
{
    maggiePrint("Not implemented");
    return 0;
}

//////////////////////////////////////////////////

int MCDC3006SDriver::writeConfigToFile(std::string filename)
{
    mxml_node_t *xml; /* <?xml ... ?> */
    mxml_node_t *driver; /* <driver> */
    mxml_node_t *driverName; /* <driverName> */
    mxml_node_t *driverID; /* <driverID> */
    mxml_node_t *driverType; /* <driverType> */
    mxml_node_t *mcdc3006s; /* <mcdc3006s> */
    mxml_node_t *maxPos; /* <maxPos> */
    mxml_node_t *minPos; /* <minPos> */
    mxml_node_t *maxVel; /* <maxVel> */
    mxml_node_t *maxAcc; /* <maxAcc> */
    mxml_node_t *maxDec; /* <maxDecc> */
    mxml_node_t *contCurrent; /* <contCurrent> */
    mxml_node_t *peakCurrent; /* <peakCurrent> */
    mxml_node_t *calibrationSensorPosition; /* <calibrationSensorPosition> */
    mxml_node_t *calibrationCurrentLimit; /* <calibrationCurrentLimit> */
    mxml_node_t *calibrationSpeed; /* <calibrationSpeed> */
    mxml_node_t *calibrationTimeOut; /* <calibrationTimeOut> */
    mxml_node_t *encPulsesPerRev; /* <encPulsesPerRev> */
    mxml_node_t *driverMultipFactor; /* <driverMultipFactor> */
    mxml_node_t *reductionFactor; /* <reductionFactor> */
    mxml_node_t *rs232; /** <rs232> */
    mxml_node_t *serialDevice; /** <serialDevice */
    mxml_node_t *baudRate; /** <baudRate> */
    mxml_node_t *semaphore; /** <semaphore>*/
    mxml_node_t *semFile; /** <semFile> */

    FILE *configFile;
    configFile = fopen(filename.data(), "w");
    if (configFile == NULL) {
        fprintf(stderr, "Error accessing the file %s. ", filename.data());
        perror("Can't open the file");
        return ERR_FILE_ACCESS;
    }

    string auxString;
    /*<?xml version="1.0"?> */
    xml = mxmlNewXML("1.0");
    /*<driver>*/
    driver = mxmlNewElement(xml, "driver");
    /*<driverName>*/
    driverName = mxmlNewElement(driver, "driverName");
    mxmlNewText(driverName, 0, mcdc3006Config->driverName.data());
    /*</driverName>*/
    /*<driverID>*/
    driverID = mxmlNewElement(driver, "driverID");
    auxString = XmlUtils::intToString(mcdc3006Config->driverID);			// converting the integer to ascii
    mxmlNewText(driverID, 0, auxString.data());
    /*</driverID>*/
    /*<driverType>*/
    driverType = mxmlNewElement(driver, "driverType");
    mxmlNewText(driverType, 0, mcdc3006Config->driverType.data());
    /*</driverType>*/
    /*<mcdc3006s>*/
    mcdc3006s = mxmlNewElement(driver, "mcdc3006s");

    maxPos = mxmlNewElement(mcdc3006s, "maxPos");
    auxString = XmlUtils::intToString(mcdc3006Config->maxPosition);	// converting the integer to ascii
    mxmlNewText(maxPos, 0, auxString.data());

    minPos = mxmlNewElement(mcdc3006s, "minPos");
    auxString = XmlUtils::intToString(mcdc3006Config->minPosition);	// converting the integer to ascii
    mxmlNewText(minPos, 0, auxString.data());

    maxVel = mxmlNewElement(mcdc3006s, "maxVel");
    auxString = XmlUtils::intToString(mcdc3006Config->maxVelocity);	// converting the integer to ascii
    mxmlNewText(maxVel, 0, auxString.data());

    maxAcc = mxmlNewElement(mcdc3006s, "maxAcc");
    auxString = XmlUtils::intToString(mcdc3006Config->maxAcceleration);		// converting the integer to ascii
    mxmlNewText(maxAcc, 0, auxString.data());

    maxDec = mxmlNewElement(mcdc3006s, "maxDec");
    auxString = XmlUtils::intToString(mcdc3006Config->maxDecceleration);		// converting the integer to ascii
    mxmlNewText(maxDec, 0, auxString.data());

    contCurrent = mxmlNewElement(mcdc3006s, "contCurrent");
    auxString = XmlUtils::intToString(mcdc3006Config->continousCurrentLimit);		// converting the integer to ascii
    mxmlNewText(contCurrent, 0, auxString.data());

    peakCurrent = mxmlNewElement(mcdc3006s, "peakCurrent");
    auxString = XmlUtils::intToString(mcdc3006Config->peakCurrentLimit);		// converting the integer to ascii
    mxmlNewText(peakCurrent, 0, auxString.data());

    calibrationCurrentLimit = mxmlNewElement(mcdc3006s, "calibrationCurrentLimit");
    auxString = XmlUtils::intToString(mcdc3006Config->calibrationCurrentLimit);	// converting the integer to ascii
    mxmlNewText(calibrationCurrentLimit, 0, auxString.data());

    calibrationSensorPosition = mxmlNewElement(mcdc3006s, "calibrationSensorPosition");
    auxString = XmlUtils::intToString(mcdc3006Config->calibrationSensorPosition);	// converting the integer to ascii
    mxmlNewText(calibrationSensorPosition, 0, auxString.data());

    calibrationSpeed = mxmlNewElement(mcdc3006s, "calibrationSpeed");
    auxString = XmlUtils::intToString(mcdc3006Config->calibrationSpeed);			// converting the integer to ascii
    mxmlNewText(calibrationSpeed, 0, auxString.data());

    calibrationTimeOut = mxmlNewElement(mcdc3006s, "calibrationTimeOut");
    auxString = XmlUtils::intToString(mcdc3006Config->calibrationTimeOut);	// converting the integer to ascii
    mxmlNewText(calibrationTimeOut, 0, auxString.data());

    encPulsesPerRev = mxmlNewElement(mcdc3006s, "encPulsesPerRev");
    auxString = XmlUtils::intToString(mcdc3006Config->getEncoderPulsesPerRevolution());	// converting the integer to ascii
    mxmlNewText(encPulsesPerRev, 0, auxString.data());

    driverMultipFactor = mxmlNewElement(mcdc3006s, "driverMultipFactor");
    auxString = XmlUtils::intToString(mcdc3006Config->getDriverMultiplicationFactor());	// converting the integer to ascii
    mxmlNewText(driverMultipFactor, 0, auxString.data());

    reductionFactor = mxmlNewElement(mcdc3006s, "reductionFactor");
    auxString = XmlUtils::intToString(mcdc3006Config->getReductionFactor());		// converting the integer to ascii
    mxmlNewText(reductionFactor, 0, auxString.data());

    /*<rs232> */
    rs232 = mxmlNewElement(mcdc3006s, "rs232");
    serialDevice = mxmlNewElement(rs232, "serialDevice");
    mxmlNewText(serialDevice, 0, mcdc3006Config->rs232.serialDevice);

    baudRate = mxmlNewElement(rs232, "baudRate");
    auxString = XmlUtils::intToString(mcdc3006Config->rs232.baudrate);	// converting the integer to ascii
    mxmlNewText(baudRate, 0, auxString.data());
    /*<rs232> */

    /*<semaphore>*/
    semaphore = mxmlNewElement(mcdc3006s, "semaphore");
    /*<semFile>*/
    semFile = mxmlNewElement(semaphore, "semFile");
    mxmlNewText(semFile, 0, mcdc3006Config->semaphore.semFile);
    /*</semFile>*/
    /*</semaphore>*/

    /*</mcdc3006s>*/
    /*</driver>*/

    //	cout << "\n\nPrinting the file in the screen" << endl;
    //	mxmlSaveFile(xml,stdout,tabbed_cb);
    // Saving the configuration to a file
    if (mxmlSaveFile(xml, configFile, XmlUtils::tabbed_cb) != 0) {
        fclose(configFile);
        return (ERR_CONF);
    }

    fclose(configFile);
    return (ERR_NOERR);
}

//////////////////////////////////////////////////

int MCDC3006SDriver::loadConfigFromFile(std::string filename)
{
    FILE *configFile;
    mxml_node_t *xmlConfig; // Used to store the xml tree from the config file
    mxml_index_t *ind;      // Index of the configuration tree
    string parameterName;   // used to store the name of each Xml tag
    string parameterData;   // used to store the data of the parameters as we are reading them
    bool error = false;    // used to monitor if there has been any errors

    // add LONG_TERM_MEMORY_DIR to filename
    std::string PATH_MOTOR_DRIVERS_PROJECT = ros::package::getPath("motor_drivers");
    filename = std::string(PATH_MOTOR_DRIVERS_PROJECT + "/data/motor_configs/") + filename;

    configFile = fopen(filename.data(), "r");
    if (configFile == NULL) {
        fprintf(stderr, "Error accessing the file %s. ", filename.data());
        perror("Can't open the file");
        return ERR_FILE_ACCESS;
    }

    xmlConfig = mxmlLoadFile(NULL, configFile, MXML_TEXT_CALLBACK); // Loading from file
    ind = mxmlIndexNew(xmlConfig, NULL, NULL);                      // Generating the index

    // Reading the generic parameters:
    parameterName = "driverName";
    if (XmlUtils::XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR) {
        perror("Error reading driverName parameter");
        error = true;
    }
    mcdc3006Config->driverName = parameterData.data();

    parameterName = "driverID";
    if (XmlUtils::XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR) {
        perror("Error reading driverID parameter");
        error = true;
    }
    mcdc3006Config->driverID = atoi(parameterData.data());

    parameterName = "driverType";
    if (XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR) {
        perror("Error reading driverType parameter");
        error = true;
    }
    mcdc3006Config->driverType = parameterData.data();

    // Reading the RS232 related Parameters
    parameterName = "serialDevice";
    if (XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR) {
        perror("Error reading serialDevice parameter");
        error = true;
    }
    sprintf(mcdc3006Config->rs232.serialDevice, "%s", parameterData.data());

    parameterName = "baudRate";
    if (XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR) {
        perror("Error reading baudRate parameter");
        error = true;
    }
    mcdc3006Config->rs232.baudrate = atoi(parameterData.data());

    // Now reading the semaphore related parameters
    parameterName = "semFile";
    if (XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR) {
        perror("Error reading semFile parameter");
        error = true;
    }
    sprintf(mcdc3006Config->semaphore.semFile, "%s", parameterData.data());

    // The limits
    parameterName = "maxPos";
    if (XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR) {
        perror("Error reading MaxPos parameter");
        error = true;
    }
    mcdc3006Config->maxPosition = atol(parameterData.data());

    parameterName = "minPos";
    if (XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR) {
        perror("Error reading MinPos parameter");
        error = true;
    }
    mcdc3006Config->minPosition = atol(parameterData.data());

    parameterName = "maxVel";
    if (XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR) {
        perror("Error reading MaxVel parameter");
        error = true;
    }
    mcdc3006Config->maxVelocity = atol(parameterData.data());

    parameterName = "maxAcc";
    if (XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR) {
        perror("Error reading MaxAcc parameter");
        error = true;
    }
    mcdc3006Config->maxAcceleration = atol(parameterData.data());

    parameterName = "maxDec";
    if (XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR) {
        perror("Error reading MaxDec parameter");
        error = true;
    }
    mcdc3006Config->maxDecceleration = atol(parameterData.data());

    parameterName = "contCurrent";
    if (XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR) {
        perror("Error reading contCurrent parameter");
        error = true;
    }
    mcdc3006Config->continousCurrentLimit = atol(parameterData.data());

    parameterName = "peakCurrent";
    if (XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR) {
        perror("Error reading peakCurrent parameter");
        error = true;
    }
    mcdc3006Config->peakCurrentLimit = atol(parameterData.data());

    // Calibration parameters
    parameterName = "calibrationSensorPosition";
    if (XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR) {
        perror("Error reading peakCurrent parameter");
        error = true;
    }
    mcdc3006Config->calibrationSensorPosition = atol(parameterData.data());

    parameterName = "calibrationCurrentLimit";
    if (XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR) {
        perror("Error reading peakCurrent parameter");
        error = true;
    }
    mcdc3006Config->calibrationCurrentLimit = atol(parameterData.data());

    parameterName = "calibrationSpeed";
    if (XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR) {
        perror("Error reading peakCurrent parameter");
        error = true;
    }
    mcdc3006Config->calibrationSpeed = atol(parameterData.data());

    parameterName = "calibrationTimeOut";
    if (XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR) {
        perror("Error reading peakCurrent parameter");
        error = true;
    }
    mcdc3006Config->calibrationTimeOut = atol(parameterData.data());

    // Measure Units
    parameterName = "encPulsesPerRev";
    if (XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR) {
        perror("Error reading encPulsesPerRev parameter");
        error = true;
    }
    mcdc3006Config->setEncoderPulsesPerRevolution(atol(parameterData.data()));

    parameterName = "driverMultipFactor";
    if (XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR) {
        perror("Error reading driverMultipFactor parameter");
        error = true;
    }
    mcdc3006Config->setDriverMultiplicationFactor(atoi(parameterData.data()));

    parameterName = "reductionFactor";
    if (XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR) {
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
    if (error == true) {			// We notify if there have been errors
        return (ERR_CONF);
    }
    return (ERR_NOERR);
}

//////////////////////////////////////////////////

// Configuration Access Functions (Getters)

long int MCDC3006SDriver::getMaxPosition()
{
    mcdc3006Config->maxPosition = getDriverMaxPos(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID);
    if (mcdc3006Config->maxPosition != ERR_NOERR) {
        /** Todo Throw exception here*/
    }

    return mcdc3006Config->maxPosition;
}

//////////////////////////////////////////////////

long int MCDC3006SDriver::getMinPosition()
{
    mcdc3006Config->minPosition = getDriverMinPos(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID);
    if (mcdc3006Config->minPosition != ERR_NOERR) {
        /** Todo Throw exception here*/
    }
    return mcdc3006Config->minPosition;
}

//////////////////////////////////////////////////

long int MCDC3006SDriver::getMaxVelocity()
{
    mcdc3006Config->maxVelocity = getDriverMaxVel(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID);
    if (mcdc3006Config->maxVelocity != ERR_NOERR) {
        /** Todo Throw exception here*/
    }
    return mcdc3006Config->maxVelocity;
}

//////////////////////////////////////////////////

long int MCDC3006SDriver::getMaxAcceleration()
{
    mcdc3006Config->maxAcceleration = getDriverMaxAcc(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID);
    if (mcdc3006Config->maxAcceleration != ERR_NOERR) {
        /** Todo Throw exception here*/
    }
    return mcdc3006Config->maxAcceleration;
}

//////////////////////////////////////////////////

long int MCDC3006SDriver::getMaxDecceleration()
{
    mcdc3006Config->maxDecceleration = getDriverMaxDec(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID);
    if (mcdc3006Config->maxDecceleration != ERR_NOERR) {
        /** Todo Throw exception here*/
    }
    return mcdc3006Config->maxDecceleration;
}

//////////////////////////////////////////////////

int MCDC3006SDriver::getContinousCurrentLimit()
{
    mcdc3006Config->continousCurrentLimit = getDriverCurLim(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID);
    if (mcdc3006Config->continousCurrentLimit != ERR_NOERR) {
        /** Todo Throw exception here*/
    }
    return mcdc3006Config->continousCurrentLimit;
}

//////////////////////////////////////////////////

int MCDC3006SDriver::getPeakCurrentLimit()
{
    mcdc3006Config->peakCurrentLimit = getDriverPCurLim(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID);
    if (mcdc3006Config->peakCurrentLimit != ERR_NOERR) {
        /** Todo Throw exception here*/
    }
    return mcdc3006Config->peakCurrentLimit;
}

//////////////////////////////////////////////////

string MCDC3006SDriver::getConfigFile()
{
    return mcdc3006Config->configFile;
}

//////////////////////////////////////////////////

// Configuration Access Functions (Setters)

int MCDC3006SDriver::setMaxPosition(long int maxPosition)
{
    if (setDriverMaxPos(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID, maxPosition) != ERR_NOERR) {
        return ERR_CONF;
    }
    DriverInterface::setMaxPosition(maxPosition);
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int MCDC3006SDriver::setMinPosition(long int minPosition)
{
    if (setDriverMinPos(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID, minPosition) != ERR_NOERR) {
        return ERR_CONF;
    }
    DriverInterface::setMinPosition(minPosition);
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int MCDC3006SDriver::setMaxVelocity(long int maxVelocity)
{
    if (setDriverMaxVel(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID, maxVelocity) != ERR_NOERR) {
        return ERR_CONF;
    }
    DriverInterface::setMaxVelocity(maxVelocity);
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int MCDC3006SDriver::setMaxAcceleration(long int maxAcceleration)
{
    if (setDriverMaxAcc(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID, maxAcceleration) != ERR_NOERR) {
        return ERR_CONF;
    }
    DriverInterface::setMaxAcceleration(maxAcceleration);
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int MCDC3006SDriver::setMaxDecceleration(long int maxDecceleration)
{
    if (setDriverMaxDec(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID, maxDecceleration) != ERR_NOERR) {
        return ERR_CONF;
    }
    DriverInterface::setMaxDecceleration(maxDecceleration);
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int MCDC3006SDriver::setContinousCurrentLimit(int continousCurrentLimit)
{
    if (setDriverCurLim(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID, continousCurrentLimit) != ERR_NOERR) {
        return ERR_CONF;
    }
    DriverInterface::setContinousCurrentLimit(continousCurrentLimit);
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int MCDC3006SDriver::setPeakCurrentLimit(int peakCurrentLimit)
{
    if (setDriverPCurLim(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID, peakCurrentLimit) != ERR_NOERR) {
        return ERR_CONF;
    }
    DriverInterface::setPeakCurrentLimit(peakCurrentLimit);
    return ERR_NOERR;
}

//////////////////////////////////////////////////

int MCDC3006SDriver::setConfigFile(std::string confFile)
{
    mcdc3006Config->configFile = confFile;
    return 0;
}

//////////////////////////////////////////////////

// Specific configuration getters and setters (only used by MCDC3006S and not other drivers)

string MCDC3006SDriver::getSerialDevice()
{
    string deviceName;
    deviceName = mcdc3006Config->getRS232().serialDevice;
    return deviceName;
}

//////////////////////////////////////////////////

int MCDC3006SDriver::getBaudrate()
{
    return mcdc3006Config->getRS232().baudrate;
}

//////////////////////////////////////////////////

string MCDC3006SDriver::getSemFile()
{
    string semName;
    semName = mcdc3006Config->getSemaphore().semFile;
    return semName;
}

//////////////////////////////////////////////////

void MCDC3006SDriver::setSerialDevice(std::string serialDevice)
{
    for (int i = 0; i < SP_MAX_FILENAME_SIZE; i++) {
        mcdc3006Config->rs232.serialDevice[i] = serialDevice[i];
    }
}

//////////////////////////////////////////////////

void MCDC3006SDriver::setBaudrate(int baud)
{
    mcdc3006Config->rs232.baudrate = baud;
}

//////////////////////////////////////////////////

void MCDC3006SDriver::setSemFile(std::string semFile)
{
    for (int i = 0; i < MAX_SEMFILENAME_SIZE; i++) {
        mcdc3006Config->semaphore.semFile[i] = semFile[i];
    }
}

//////////////////////////////////////////////////

int MCDC3006SDriver::connect()
{
    if (initCommunication(mcdc3006Config->rs232.baudrate, mcdc3006Config->rs232.serialDevice, &mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semFile, &mcdc3006Config->semaphore.semID) != ERR_NOERR) {
        mcdc3006Status->connected = false;
        return ERR_FILE_ACCESS;
    }
    mcdc3006Status->connected = true;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

/*
 * Function to close the driver and end the communication with it
 * \return ERR_FILE_ACCESS in case of failing the communication with the driver. It also sets MCDC3006SDriver::mcdc3006Status (MCDC3006SStatus::connected) to false
 * \return ERR_NOERR otherwise. It also sets MCDC3006SStatus::connected to true
 */
int MCDC3006SDriver::disconnect()
{
    if (endCommunication(mcdc3006Config->rs232.RSd, mcdc3006Config->semaphore.semID) != ERR_NOERR) {
        return ERR_FILE_ACCESS;
    }
    mcdc3006Status->connected = false;
    return ERR_NOERR;
}

//////////////////////////////////////////////////

void MCDC3006SDriver::printLimits()
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

void MCDC3006SDriver::printConfig()
{
    DriverInterface::printConfiguration();
    cout << "Driver Serial Device: "
         << getSerialDevice()
         << endl;
    cout << "Driver BaudRate: "
         << getBaudrate()
         << endl;
    cout << "Driver SemFile: "
         << getSemFile()
         << endl;
    cout << endl;
}

//////////////////////////////////////////////////

void MCDC3006SDriver::printMotorSensorInfo()
{
    cout << endl;
    cout << "-- Driver Motor Sensor --"
         << endl;
    if (updateMotorSensorInfo() != ERR_NOERR) {
        perror("Could not update the Motor Sensor\n");
    }
    else {
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
}

//////////////////////////////////////////////////

void MCDC3006SDriver::printStatus()
{
    cout << endl;
    cout << "-- Driver Status -- "
         << endl;
    cout << "True = yes. False = no"
         << endl;
    cout << "Driver is enabled: "
         << boolToString(isEnabled())
         << endl;
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
