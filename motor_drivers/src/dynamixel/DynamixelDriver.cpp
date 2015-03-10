/**
 * DynamixelDriver.cpp
 *      Created on: 01/09/2010
 *      Author: Paulina Vélez Núñez (mailto: pvelez@ing.uc3m.es)
 */

#include "dynamixel/DynamixelDriver.h"
//#include "long_term_memory/ltm_path.h"

#include "xml/mxmlUtils.h"
#include "definitions/ADerrors.h"
#include "debug/debug_utils.h"
#include <ros/package.h>

using namespace std;

const int DynamixelDriver::MAX_PORTNAME_SIZE = 200;
const int DynamixelDriver::MAX_ALLOWED_VELOCITY = 1023;


DynamixelDriver::DynamixelDriver (string driverConfigFile)
    : DriverInterface() {

    dynamixelConfig = new DynamixelConfiguration(driverConfigFile);
    dynamixelMotorSens = new DynamixelMotorSensor();
    dynamixelStatus = new DynamixelStatus();

    //	this->updateStatus(); 			// Reading the status from the driver
    dynamixelStatus->isConnected(false);	// At this moment the driver is not yet opened
    this->calibrate(false);			// This value it should be already in this state, but we do it again for safety

    // The base class will need this pointers
    drvConf = dynamixelConfig;
    drvMotorSensor = dynamixelMotorSens;
    drvStatus = dynamixelStatus;
}

DynamixelDriver::DynamixelDriver ()
    : DriverInterface::DriverInterface () {

    dynamixelConfig = new DynamixelConfiguration ();
    dynamixelMotorSens = new DynamixelMotorSensor();
    dynamixelStatus = new DynamixelStatus();

    dynamixelStatus->isConnected(false);	// At this moment the driver is not yet opened
    this->calibrate(false);		// This value it should be already in this state, but we do it again for safety

    // The base class will need this pointers
    drvConf = dynamixelConfig;
    drvMotorSensor = dynamixelMotorSens;
    drvStatus = dynamixelStatus;
}

DynamixelDriver::~DynamixelDriver() {
    disconnect();
    delete dynamixelConfig;
    delete dynamixelMotorSens;
    delete dynamixelStatus;
    drvConf = 0;
    drvMotorSensor = 0;
    drvStatus = 0;
}


/*!
 * Move the driver to maximum limit position at the speed. Also set
 * the maximum speed of Dynamixel driver.
 * @param vel the velocity to move the driver (between 0 - 1023 values).
 * @return ERR_OUTOFBOUNDS
 * 		   ERR_NOERR otherwise
 * \warning This function will change the maximum
 * Example: dynamixelConfig.maxVelocity=500
 * 			you set the new velocity to move the driver at 200 : vel = 200
 * 			the register in the driver will be write, and change the maxVelocity
 * 			now maxVelocity= 200 because you are wrote in the velocity register
 */
int DynamixelDriver::moveDriverByVelocity(long int vel){

    int vel_s; // this variable will save the velocity

    if ((vel<= MAX_ALLOWED_VELOCITY)&&(vel>0))
    {
        //dxl_write_word(dynamixelConfig->getID(), P_GOAL_SPEED_L, vel );
        setMaxVelocity(vel);
        //dynamixelConfig->maxVelocity= vel;
        moveDriverToAbsolutePosition(getMaxPosition());
        return ERR_NOERR;

    }
    else if (vel == 0){
        vel_s= dynamixelConfig->maxVelocity;
        setMaxVelocity(vel);
        moveDriverToAbsolutePosition(getPosition());
        setMaxVelocity(vel_s);
        return ERR_NOERR;
    }
    else
        if (vel < 0){
            vel_s= vel*(-1);
            setMaxVelocity(vel_s);
            dynamixelConfig->maxVelocity= vel_s;
            moveDriverToAbsolutePosition(getMinPosition());
            return ERR_NOERR;
        }

    return ERR_NOERR;
}


/*!
 * Moves an Dynamixel driver to relative position
 * @param relativePos the relative position to move the driver (in pulses)
 * @return ERR_POSLIMIT in case of error
 *         ERR_NOERR otherwise
 */
int DynamixelDriver::moveDriverToRelativePosition(long int relativePos){
    long int goalPosition;

    if (dynamixelStatus->isEnabled()== true){

        goalPosition = relativePos + dynamixelMotorSens->getPosition();

        if ((goalPosition>= getMinPosition()) && (goalPosition<= getMaxPosition())){
            moveDriverToAbsolutePosition(goalPosition);
            //return ERR_NOERR;
        }
    }
    else
    {
        maggieDebug2("Driver is disable");
        //return ERR_NOERR;
    }
    return ERR_NOERR;
}


/*!
 * Move the driver to an absolute position
 * If set position exceed the limits, the driver will move until the limit.
 * @param absolutePos the absolute position to move the driver (in pulses)
 * @return ERR_POSLIMIT in case of error
 *         ERR_NOERR otherwise
 */
int DynamixelDriver::moveDriverToAbsolutePosition(long int absolutePos) {

    if (dynamixelStatus->isEnabled()== true){
        // checking if out of bounds
        if (absolutePos < getMinPosition()){
            absolutePos = getMinPosition();
        }
        else if (absolutePos > getMaxPosition()){
            absolutePos = getMaxPosition();
        }
        // moving the driver
        dxl_write_word( dynamixelConfig->getID(), P_GOAL_POSITION_L, absolutePos );
        return ERR_NOERR;
    }
    else{
        maggieDebug2("Driver is disabled");
        return ERR_NOERR;
    }
}


/*!
 * Calibrate Driver to calibrate position
 * @param calibrationSensorPosition is the position (in pulses) of calibration the driver at the calibration sensor position
 * @param calibrationCurrentLimit is not used for this driver
 * @param calibrationSpeed is not used for this driver
 * @param calibrationTimeOut is not used for this driver
 * @return ERR_NOERR when the driver is calibrated or when the driver is disable, in this case
 * 		   show a message that is disable
 */
int DynamixelDriver:: calibrate(long int calibrationSensorPosition,long int calibrationCurrentLimit, long int calibrationSpeed, long int calibrationTimeOut){
    //long int AmpPos;
    //int deviceID;
    //deviceID= dynamixelConfig->driverID;
    //AmpPos= calibrationSensorPosition;

    if (dynamixelStatus->isEnabled()== true){
        //synchronize(dynamixelConfig->driverID,calibrationSensorPosition);
        maggieDebug2("Driver calibrated ");
        dynamixelStatus->isCalibrated(true);

        return ERR_NOERR;
    }
    else {
        maggieDebug2("Driver is disable");
        return ERR_NOERR;
    }
}


/*!
 * If the parameter entered is true, this function calibrates the driver.
 * If it is false, the function sets the DynamixelStatus::calibrated to false to indicate the driver must be calibrated.
 * @param calibrated is boolean that indicate if driver is or not calibrate
 * @return ERR_COM in case of error
 *         ERR_NOERR otherwise
 */
int DynamixelDriver::calibrate (bool calibrated){
    //long int AmpPos;
    //int deviceID;
    //deviceID= dynamixelConfig->id;
    //AmpPos= dynamixelConfig->calibrationSensorPosition;
    if (calibrated == true){
        if (dynamixelStatus->isEnabled()== true){
            //synchronize(deviceID,AmpPos);
            calibrated = true;

        }
        //else {
        //printf("Driver is disable");
        //return ERR_NOERR;
        //}
    }
    //else{
    //if ((dynamixelMotorSens->position <=(dynamixelConfig->calibrationSensorPosition+2))||(dynamixelMotorSens->position>=(dynamixelConfig->calibrationSensorPosition-2)))
    //	calibrated = false;

    //return ERR_COM;
    //}
    return ERR_NOERR;
}


/*!
 * Enable the driver
 * Register TORQUE(24) must be set to 1
 * @return ERR_OUTOFRANGE in case of error
 *         ERR_NOERR otherwise
 */
int DynamixelDriver::enable() {

    dxl_write_byte ( dynamixelConfig->getID(), TORQUE_ENABLE, 1);

    if(dxl_get_rxpacket_error(ERRBIT_OVERLOAD) == 1) {
        return  ERR_OUTOFRANGE;
    }
    dynamixelStatus->isEnabled(true);
    return ERR_NOERR;
}


/*!
 * Disable the driver
 * Register TORQUE(24) must be set to 0
 * @return ERR_OUTOFRANGE in case of error
 *         ERR_NOERR otherwise
 */
int DynamixelDriver::disable() {

    dxl_write_byte(dynamixelConfig->getID(),TORQUE_ENABLE,0);

    if(dxl_get_rxpacket_error(ERRBIT_OVERLOAD) == 1){
        return  ERR_OUTOFRANGE;
    }
    dynamixelStatus->isEnabled(false);
    return ERR_NOERR;
}

//--------------------------------Status Section------------------------------------
//Status Getters

/*!
 * Asks the driver if the current is being limitated
 * \warning NOT AVAILABLE FOR THIS DYNAMIXEL DRIVER
 * \see updateStatus()
 * \see DriverInterface.h \see DriverInterface.cpp
 * @return ERR_NA (Data is not available for this driver)
 * \see DynamixelConfiguration.h
 * \see ADerrors.h
*/
bool DynamixelDriver::isCurrentLimitated() { 
    updateStatus();
    return (ERR_NA);
}


/*!
 * This function asks if the driver is enabled or disabled. True = driver enabled. False = driver disabled
 * @return DynamixelStatus::enabled True if it is enabled
 * \warning This function calls the driver using the function updateStatus() If you don't really need to call the driver it's better to use DriverInterface::isEnabled()
 * \see updateStatus()
 */
bool DynamixelDriver::isEnabled() { 
    updateStatus();
    return dynamixelStatus->isEnabled();
}


/*!
 * This function asks the driver if the limit sensor is has been reached. True the sensor has been reached.
 * @return DynamixelStatus::limitSensorReached.. True: the limit sensor is producing an output. Thus the limit sensor has been reached
 * \warning This function calls the driver using the function updateStatus()
 * \see updateStatus()
 * \see DriverInterface::isLimitSensorReached()
 * \see DriverInterface.h \see DriverInterface.cpp
 */
bool DynamixelDriver::isLimitSensorReached() {
    updateStatus();
    return dynamixelStatus->isLimitSensorReached();
}


/*!
 * This function  Asks the driver if the overTemperatureWarning has been triggered (there is an over Temperature error in the driver). True = overTemperatureWarning.
 * @return dynamixelStatus->isOverTemperatureWarning();
 * \warning This function calls the driver using the function updateStatus() If you don't really need to call the driver it's better to use DriverInterface::isOverTemperatureWarning()
 * \warning The valid range of temperature is between -5 ºC and 85 ºC, if the temperature is not in this range, this function will be return true.
 * \see updateStatus()
 * \see DriverInterface::isOverTemperatureWarning()
 * \see DriverInterface.h \see DriverInterface.cpp
 */
bool DynamixelDriver::isOverTemperatureWarning() {

    updateStatus();
    return dynamixelStatus->isOverTemperatureWarning();

}


/*!
 * This function checks if the overTemperatureWarning has been triggered (there is an overVoltage error in the driver). True = overVoltageWarning.
 * @return DynamixelStatus::overVoltageWarning. True: warning.
 * \warning This function calls the driver using the function updateStatus() If you don't really need to call the driver it's better to use DriverInterface::isOverVoltageWarning()
 * \see updateStatus()
 * \see DriverInterface::isOverVoltageWarning
 * \see DriverInterface.h \seeDriverInterface.cpp
 */
bool DynamixelDriver::isOverVoltageWarning() {
    updateStatus();
    return dynamixelStatus->isOverVoltageWarning();
}


/*!
 * This function checks if the the calibration flag is calibrated or not.
 * @return DynamixelStatus::calibrated The calibration flag. true = the driver is calibrated. false = the driver is not calibrated
 * \warning This function calls the driver using the function updateStatus() If you don't really need to call the driver it's better to use DriverInterface::isCalibrated()
 * \see updateStatus()
 * \see DriverInterface::isCalibrated
 * \see DriverInterface.h \see DriverInterface.cpp
 */
bool DynamixelDriver::isCalibrated() {
    updateStatus();
    return dynamixelStatus->isCalibrated();
}


/*!
 * This function reads the status of the driver and stores it in a DynamixelStatus object
 * @return ERR_NOERR
 */
int DynamixelDriver::updateStatus(){

    if ((dxl_read_byte( dynamixelConfig->id,TORQUE_ENABLE)==1))
        dynamixelStatus->isEnabled(true);
    else
        dynamixelStatus->isEnabled(false);

    if ((getCurrentTemperature(dynamixelConfig->id)>dynamixelConfig->maxTemperature)||(getCurrentTemperature(dynamixelConfig->id)<dynamixelConfig->minTemperature))
        dynamixelStatus->isOverTemperatureWarning(true);
    else
        dynamixelStatus->isOverTemperatureWarning(false);

    if ((getCurrentVoltage(dynamixelConfig->id)>dynamixelConfig->maxVoltage)||(getCurrentVoltage(dynamixelConfig->id)<dynamixelConfig->minVoltage))
        dynamixelStatus->isOverVoltageWarning(true);
    else
        dynamixelStatus->isOverVoltageWarning(false);

    if ((getCurrentPosition(dynamixelConfig->id)<=1023)||(getCurrentPosition(dynamixelConfig->id)>=0))
        dynamixelStatus->isCalibrated(true);
    else
        dynamixelStatus->isCalibrated(false);

    if((getCurrentPosition(dynamixelConfig->id)<= dynamixelConfig->minPosition)||(getCurrentPosition(dynamixelConfig->id)>=dynamixelConfig->maxPosition))
        dynamixelStatus->isLimitSensorReached(true);
    else
        dynamixelStatus->isLimitSensorReached(false);

    return ERR_NOERR;
}


//--------------------------------MotorSensor Section------------------------------------
// MotorSensor Access Functions

/**
 * Getters only for Dynamixel
 * This sections are useful only for Dynamixel driver and return the currents values of voltage, temperature and load
 */
/*int DynamixelDriver::getDeviceID() const{
 return dxl_read_byte(id, DEVICE_ID);
}*/

/*!
 * This function return the drivers input voltage
 * @return getCurrentVoltage(dynamixelConfig->id)
 * \warning id is the identification of the physical Dynamixel driver
 */
float DynamixelDriver::getVoltage(){
    return getCurrentVoltage(dynamixelConfig->id);
}

/*!
 * This function return the current drivers temperature
 * @return getCurrentTemperature(dynamixelConfig->id)
 * \warning id is the identification of the physical Dynamixel driver
 */
int DynamixelDriver::getTemperature(){
    return getCurrentTemperature(dynamixelConfig->id);
}

/*!
 * This function return the current drivers load
 * @return getCurrentLoad(dynamixelConfig->id)
 * \warning id is the identification of the physical Dynamixel driver
 */
int DynamixelDriver::getLoad(){
    return getCurrentLoad(dynamixelConfig->id);
}

/**
 * General Getters
 */

/*!
 * This function returns  the last position stored in the MotorSensor
 * \return getCurrentPosition(dynamixelConfig->id)
 * \warning id is the identification of the physical Dynamixel driver
 */
long int DynamixelDriver::getPosition() const{

    return getCurrentPosition(dynamixelConfig->id);
}


/*!
 * This function returns  the current drivers velocity
 * \return getCurrentSpeed(dynamixelConfig->id)
 * \warning id is the identification of the physical Dynamixel driver
 */
long int DynamixelDriver::getVelocity() const{

    return getCurrentSpeed(dynamixelConfig->id);
}


/*!
 * This function returns  the last Instant Current data stored in the MotorSensor
 * \warning NOT AVAILABLE FOR THIS DYNAMIXEL DRIVER
 * \warning this instant Current may not be the current instantCurrent of the driver. If you want to retrieve the actual Instant Current of the driver use DynamixelDriver::updateInstantCurrent()
 * @return ERR_NA (Data is not available for this driver)
 * \see DynamixelConfiguration.h
 * \see ADerrors.h
 */
int DynamixelDriver::getInstantCurrent() const{
    //return dynamixelMotorSens->instantCurrent;
    maggieDebug2("Information about Current is not available for this device");
    return ERR_NOERR;
}

/**
 * Reads from the driver an update of the current position and stores it in DynamixelDriver::dynamixelMotorSens ( DynamixelMotorSensor::position )
 * \note if you want to access the last MotorSensor data without consulting the driver (although you can retrieve old MotorSensor data) you can use DriverInterface::getPosition()
 * \return ERR_COM in case of error
 * \return ERR_NOERR otherwise
 */

/**
 * Updates only for Dynamixel
 * This sections are useful only for Dynamixel driver and will update the values of voltage, temperature and load, to currents values
 */

/*!
 * This function update the voltage value, to the current input voltage value of the driver
 * @return ERR_NOERR;
 */
int DynamixelDriver::updateVoltage(){
    dynamixelMotorSens->voltage=getCurrentVoltage(dynamixelConfig->id);
    return ERR_NOERR;
}

/**!
 * This function update the temperature value, to the current input temperature value of the driver
 * @return ERR_NOERR;
 */
int DynamixelDriver::updateTemperature(){
    dynamixelMotorSens->temperature=getCurrentTemperature(dynamixelConfig->id);
    return ERR_NOERR;
}

/*!
 * This function update the load value, to the current input load value of the driver
 * @return ERR_NOERR;
 */
int DynamixelDriver::updateLoad(){
    dynamixelMotorSens->load=getCurrentLoad(dynamixelConfig->id);
    return ERR_NOERR;
}

/**
 * General Getters
 */

/*!
 * This function set in MotorSensor the last position of the driver
 * \return ERR_OUTOFRANGE if the lecture of values from the drivers are inconsistent (out of bounds)
 *         ERR_NOERR otherwise
 */
int DynamixelDriver::updatePosition() {

    if ((getCurrentPosition(dynamixelConfig->id) > (getMaxPosition()))
            ||((getCurrentPosition(dynamixelConfig->id)) < (getMinPosition()))){

        perror("Error updating the position");
        return ERR_OUTOFRANGE;
    }

    else{
        dynamixelMotorSens->position=getCurrentPosition(dynamixelConfig->id);

	return ERR_NOERR;
    }
}

/*!
 * This function reads from the driver an update of the current velocity and stores it in DynamixelDriver::dynamixelMotorSens ( DynamixelMotorSensor::velocity )
 * \note if you want to access the last MotorSensor data without consulting the driver (although you can retrieve old MotorSensor data) you can use DriverInterface::getVelocity()
 * \return ERR_OUTOFRANGE in case of the value that return is out of the permitted range
 *         ERR_NOERR otherwise
 */
int DynamixelDriver::updateVelocity() {

    if (getCurrentSpeed(dynamixelConfig->id) > getMaxVelocity())
    {
        perror ("Error updating the velocity");
        return ERR_OUTOFRANGE;
    }
    else{
        dynamixelMotorSens->velocity=getCurrentSpeed(dynamixelConfig->id);
        return ERR_NOERR;
    }
}

/*!
 * Reads from the driver an update of the instant current and stores it in DynamixelDriver::dynamixelMotorSens ( DynamixelMotorSensor::instantCurrent )
 * \warning NOT AVAILABLE FOR THIS DYNAMIXEL DRIVER
 * \note if you want to access the last MotorSensor data without consulting the driver (although you can retrieve old MotorSensor data) you can use DriverInterface::getInstantCurrent()
 * @return ERR_NA (Data is not available for this driver)
 * \see DynamixelConfiguration.h
 * \see ADerrors.h
 */
int DynamixelDriver::updateInstantCurrent() {
    maggieDebug2("Instant Current is Not available for this device");
    return ERR_NA;
}

/*!
 * Reads from the driver an update of all the MotorSensor data and stores it in DynamixelDriver::dynamixelMotorSens
 * \warning This function calls the driver using the functions updatePosition() and updateVelocity
 * \return ERR_COM in case of error
 *         ERR_NOERR otherwise
 */
int DynamixelDriver::updateMotorSensorInfo() {
    if (updatePosition() != ERR_NOERR){
        return ERR_COM;
    }
    if (updateVelocity() != ERR_NOERR){
        return ERR_COM;
    }
    return ERR_NOERR;
}


//--------------------------------Configuration Section------------------------------------


/*!
 * This function write the configuration stored in dynamixelConfig to the driver
 * \return ERR_NOERR if success
 *         ERR_CONF if any error occurs
 */
int DynamixelDriver::writeConfigToDriver() {

    setMinPosition(dynamixelConfig->minPosition);
    if (setMinPosition(dynamixelConfig->minPosition)< ERR_NOERR){
        perror("writeConfigToDriver() --> Error setting the minimum position to the driver\n\r");
        return ERR_CONF;
    }else setMinPosition(dynamixelConfig->minPosition);


    setMaxPosition(dynamixelConfig->maxPosition);
    if (setMaxPosition(dynamixelConfig->maxPosition)<ERR_NOERR){
        perror("setDriverConf() --> Error setting the maximum position to the driver\n\r");
        return ERR_CONF;
    }else setMaxPosition(dynamixelConfig->maxPosition);


    setMaxVelocity(dynamixelConfig->maxVelocity);
    if (setMaxVelocity(dynamixelConfig->maxVelocity)<ERR_NOERR){
        perror("setDriverConf() --> Error setting the maximum velocity to the driver\n\r");
        return ERR_CONF;
    }else setMaxVelocity(dynamixelConfig->maxVelocity);


    setMaxVelocity(dynamixelConfig->maxVelocity);
    if (setMaxVelocity(dynamixelConfig->maxVelocity)<ERR_NOERR){
        perror("setDriverConf() --> Error setting the maximum velocity to the driver\n\r");
        return ERR_CONF;
    }else setMaxVelocity(dynamixelConfig->maxVelocity);

    return ERR_NOERR;
}


/*!
 * \brief function that loads the configuration from the driver and stores it in dynamixelConfig
 * \return ERR_NOERR if success
 * \return ERR_CONF if any error occurs
 * \note{ <b>Note for maintainers:</b> If you want to add more features you also have add this features to
 * 		 DynamixelDriver::writeConfigToDriver() }
 */
int DynamixelDriver::loadConfigFromDriver() {
    return ERR_NOERR;
}

/*!
 * \brief function that loads the configuration from dynamixelConfig and stores it in an xml file
 * \param filename filename of the xml file where to store the configuration
 * \return ERR_NOERR if success
 * \return ERR_FILE_ACCESS
 * \return ERR_CONF if any error occurs
 * \note{ <b>Note for maintainers:</b> If you want to add more features you also have add this features to
 * 				DynamixelDriver::loadConfigFromDriver() }
 * \warning { This function saves to a file the driver configuration stored in the program memory. Before you use
 * 					this function you may want to load the configuration from the driver to the program memory }
 * \see DynamixelDriver::writeConfigToDriver()
 * \see DynamixelDriver::loadConfigFromDriver()
 */
int DynamixelDriver::writeConfigToFile(string filename) {
    mxml_node_t *xml;    					/* <?xml ... ?> */
    mxml_node_t *driver;   					/* <driver> */
    mxml_node_t *driverName;   				/* <driverName> */
    mxml_node_t *driverID;   				/* <driverID> */
    mxml_node_t *driverType;   				/* <driverType> */
    mxml_node_t *portName;					/*<portName>*/
    mxml_node_t *deviceID;					/*<deviceID>*/
    mxml_node_t *dynamixel;  				/* <dynamixel> */
    mxml_node_t *maxPos;					/* <maxPos> */
    mxml_node_t *minPos;					/* <minPos> */
    mxml_node_t *maxVel;					/* <maxVel> */
    mxml_node_t *calibrationSensorPosition;	/* <calibrationSensorPosition> */
    mxml_node_t *minVolt;					/* <minVolt> */
    mxml_node_t *maxVolt;					/* <maxVolt> */
    mxml_node_t *minTemp;					/* <minTemp> */
    mxml_node_t *maxTemp;					/* <maxTemp> */

    mxml_node_t *encPulsesPerRev;			/* <encPulsesPerRev> */
    mxml_node_t *driverMultipFactor;		/* <driverMultipFactor> */
    mxml_node_t *reductionFactor;			/* <reductionFactor> */
    //mxml_node_t *rs232;						/** <rs232> */
    //mxml_node_t *serialDevice;				/** <serialDevice */
    //mxml_node_t *baudRate;					/** <baudRate> */

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
    mxmlNewText(driverName, 0, dynamixelConfig->driverName.data());
    /*</driverName>*/

    /*<driverID>*/
    driverID = mxmlNewElement(driver, "driverID");
    auxString = XmlUtils::intToString (dynamixelConfig->driverID);	// converting the integer to ascii
    mxmlNewText(driverID, 0,  auxString.data());
    /*</driverID>*/

    /*<driverType>*/
    driverType = mxmlNewElement(driver, "driverType");
    mxmlNewText(driverType, 0, dynamixelConfig->driverType.data());
    /*</driverType>*/

    /*<portName>*/
    portName= mxmlNewElement(driver, "portName");
    mxmlNewText(portName, 0, dynamixelConfig->namePort.data());
    /*</portName>*/


    /*<deviceID>*/
    deviceID = NULL;
    deviceID= mxmlNewElement (deviceID, "deviceID");
    auxString = XmlUtils::intToString (dynamixelConfig->id);
    mxmlNewText(deviceID, 0, auxString.data());
    /*</deviceID>*/

    /*<dynamixel>*/
    dynamixel = mxmlNewElement(driver, "dynamixel");

    maxPos = mxmlNewElement(dynamixel, "maxPos");
    auxString = XmlUtils::intToString (dynamixelConfig->maxPosition);	// converting the integer to ascii
    mxmlNewText(maxPos,0,auxString.data());

    minPos = mxmlNewElement(dynamixel, "minPos");
    auxString = XmlUtils::intToString (dynamixelConfig->minPosition);	// converting the integer to ascii
    mxmlNewText(minPos,0,auxString.data());

    maxVel = mxmlNewElement(dynamixel, "maxVel");
    auxString = XmlUtils::intToString (dynamixelConfig->maxVelocity);	// converting the integer to ascii
    mxmlNewText(maxVel,0,auxString.data());

    calibrationSensorPosition= mxmlNewElement(dynamixel, "calibrationSensorPosition");
    auxString = XmlUtils::intToString (dynamixelConfig->calibrationSensorPosition);// converting the integer to ascii
    mxmlNewText(calibrationSensorPosition,0,auxString.data());

    minVolt = mxmlNewElement(dynamixel, "minVolt");
    auxString = XmlUtils::intToString (dynamixelConfig->minVoltage );	// converting the integer to ascii
    mxmlNewText(minVolt,0,auxString.data());

    maxVolt = mxmlNewElement(dynamixel, "maxVolt");
    auxString = XmlUtils::intToString (dynamixelConfig->maxVoltage);	// converting the integer to ascii
    mxmlNewText(maxVolt,0,auxString.data());

    minTemp = mxmlNewElement(dynamixel, "minTemp");
    auxString = XmlUtils::intToString (dynamixelConfig->minTemperature);	// converting the integer to ascii
    mxmlNewText(minTemp,0,auxString.data());

    maxTemp = mxmlNewElement(dynamixel, "maxTemp");
    auxString = XmlUtils::intToString (dynamixelConfig->maxTemperature);	// converting the integer to ascii
    mxmlNewText(maxTemp,0,auxString.data());

    encPulsesPerRev = mxmlNewElement(dynamixel, "encPulsesPerRev");
    auxString = XmlUtils::intToString (dynamixelConfig->getEncoderPulsesPerRevolution());	// converting the integer to ascii
    mxmlNewText(encPulsesPerRev,0,auxString.data());

    driverMultipFactor = mxmlNewElement(dynamixel, "driverMultipFactor");
    auxString = XmlUtils::intToString (dynamixelConfig->getDriverMultiplicationFactor());	// converting the integer to ascii
    mxmlNewText(driverMultipFactor,0,auxString.data());

    reductionFactor = mxmlNewElement(dynamixel, "reductionFactor");
    auxString = XmlUtils::intToString (dynamixelConfig->getReductionFactor());			// converting the integer to ascii
    mxmlNewText(reductionFactor,0,auxString.data());

    // Saving the configuration to a file
    if (mxmlSaveFile(xml,configFile,XmlUtils::tabbed_cb) != 0){
        fclose(configFile);
        return (ERR_CONF);
    }

    fclose(configFile);
    return (ERR_NOERR);
}

/*!
 * \brief function that loads the configuration from a file and stores it in dynamixelConfig
 * \param filename the file where to get the configuration
 * \return ERR_NOERR if success
 * \return ERR_FILE_ACCESS if couldn't access to the file
 * \return ERR_CONF if any error occurs
 * \warning {This function loads the configuration from the file into the program memory.
 * 				This function DOES NOT write the loaded configuration into the driver. If you want to store the loaded
 * 					configuration in the driver you must call DynamixelDriver::writeConfigToDriver() after loading the
 * 					configuration from the file}
 * \see DynamixelDriver::writeConfigToDriver()
 */
int DynamixelDriver::loadConfigFromFile(string filename){
    FILE *configFile;
    mxml_node_t *xmlConfig;			// Used to store the xml tree from the config file
    mxml_index_t *ind;				// Index of the configuration tree
    string parameterName;			// used to store the name of each Xml tag
    string parameterData;			// used to store the data of the parameters as we are reading them
    bool error = false;				// used to monitor if there has been any errors

    // add LONG_TERM_MEMORY_DIR to filename
    std::string PATH_MOTOR_DRIVERS_PROJECT = ros::package::getPath("motor_drivers");
    filename = std::string(PATH_MOTOR_DRIVERS_PROJECT + "/data/motor_configs/") + filename;

    configFile = fopen(filename.c_str(),"r");
    if (configFile==NULL)	{
        fprintf(stderr, "Error accessing the file %s. ",filename.c_str() );
        perror("Can't open the file");
        return ERR_FILE_ACCESS;
    }

    xmlConfig = mxmlLoadFile(NULL, configFile, MXML_TEXT_CALLBACK); // Loading from file
    ind = mxmlIndexNew(xmlConfig, NULL, NULL);						// Generating the index

    // Reading the generic parameters:
    parameterName = "driverName";
    if ( XmlUtils::xmlFindParam (ind,parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading driverName parameter");
        error = true;
    }
    dynamixelConfig->driverName = parameterData.data();

    parameterName = "driverID";
    if ( XmlUtils::xmlFindParam (ind, parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading driverID parameter");
        error = true;
    }
    dynamixelConfig->driverID = atoi (parameterData.data());

    parameterName = "driverType";
    if ( XmlUtils::xmlFindParam (ind,parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading driverType parameter");
        error = true;
    }
    dynamixelConfig->driverType = parameterData.data();

    parameterName= "portName";
    if (XmlUtils::xmlFindParam (ind, parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading portName parameter");
        error = true;
    }
    dynamixelConfig->namePort= parameterData.data();

    parameterName = "deviceID";
    if ( XmlUtils::xmlFindParam (ind,parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading driverType parameter");
        error = true;
    }
    dynamixelConfig->id = atol(parameterData.data());

    // The limits
    parameterName = "maxPos";
    if ( XmlUtils::xmlFindParam (ind,parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading MaxPos parameter");
        error = true;
    }
    dynamixelConfig->maxPosition = atol(parameterData.data());

    parameterName = "minPos";
    if ( XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading MinPos parameter");
        error = true;
    }
    dynamixelConfig->minPosition = atol(parameterData.data());

    parameterName = "maxVel";
    if ( XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading MaxVel parameter");
        error = true;
    }
    dynamixelConfig->maxVelocity = atol(parameterData.data());

    // Calibration parameters
    parameterName = "calibrationSensorPosition";
    if ( XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading peakCurrent parameter");
        error = true;
    }
    dynamixelConfig->calibrationSensorPosition = atol(parameterData.data());

    // Operating parameters
    parameterName = "minVolt";
    if ( XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading MinVolt parameter");
        error = true;
    }
    dynamixelConfig->minVoltage = atol(parameterData.data());

    parameterName = "maxVolt";
    if ( XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading MaxVolt parameter");
        error = true;
    }
    dynamixelConfig->maxVoltage = atol(parameterData.data());

    parameterName = "minTemp";
    if ( XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading MinTemp parameter");
        error = true;
    }
    dynamixelConfig->minTemperature = atol(parameterData.data());

    parameterName = "maxTemp";
    if ( XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading MaxVel parameter");
        error = true;
    }
    dynamixelConfig->maxTemperature = atol(parameterData.data());

    // Measure Units-------------------------------------------------------------------
    parameterName = "encPulsesPerRev";
    if ( XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading encPulsesPerRev parameter");
        error = true;
    }
    dynamixelConfig->setEncoderPulsesPerRevolution(atol(parameterData.data()));

    parameterName = "driverMultipFactor";
    if ( XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading driverMultipFactor parameter");
        error = true;
    }
    dynamixelConfig->setDriverMultiplicationFactor(atoi(parameterData.data()));

    parameterName = "reductionFactor";
    if ( XmlUtils::xmlFindParam(ind, parameterName, &parameterData) != ERR_NOERR){
        perror("Error reading reductionFactor parameter");
        error = true;
    }
    dynamixelConfig->setReductionFactor(atoi(parameterData.data()));

    // With the measure units we have enough information to set the pulsesPerRevolution parameter
    // and to set the conversion units
    dynamixelConfig->setPulsesPerRevolution();

    fclose(configFile);
    if (error == true){			// We notify if there have been errors
        return (ERR_CONF);
    }
    return (ERR_NOERR);
}

//----------------------------------------------------------------------------------------
// Configuration Access Functions (Getters)

/*!
 * This function retrieves from the driver the MaxPosition parameter (by default is 1023 pulses)
 * @return dynamixelConfig->maxPosition (in pulses)
 * \see DynamixelConfiguration.h
 */
long int DynamixelDriver::getMaxPosition(){
    return dynamixelConfig->maxPosition;

}


/*!
 * This function retrieves from the driver the MinPosition parameter (by default is 0 pulses)
 * @return dynamixelConfig->minPosition (in pulses)
 * \see DynamixelConfiguration.h
 */
long int DynamixelDriver::getMinPosition(){
    return dynamixelConfig->minPosition;

}


/*!
 * This function retrieves from the driver the MaxVelocity parameter (by default is 1023 pulses/sec)
 * @return dynamixelConfig->maxVelocity
 * \see DynamixelConfiguration.h
 */
long int DynamixelDriver::getMaxVelocity(){
    return dynamixelConfig->maxVelocity;
}


/*!
 * This function retrieves from the driver the MaxAcceleration parameter
 * \warning NOT AVAILABLE FOR THIS DYNAMIXEL DRIVER
 * @return ERR_NA (Data is not available for this driver)
 * \see DynamixelConfiguration.h
 * \see ADerrors.h
 */
long int DynamixelDriver::getMaxAcceleration(){
    maggieDebug2("Information about Max Acceleration is Not available for this device");
    return ERR_NA;
    //return dynamixelConfig->maxAcceleration;
}


/*!
 * This function returns the driver the MaxDecceleration
 * \warning NOT AVAILABLE FOR THIS DYNAMIXEL DRIVER
 * @return ERR_NA (Data is not available for this driver)
 * \see DynamixelConfiguration.h
 * \see ADerrors.h
 */
long int DynamixelDriver::getMaxDecceleration(){
    maggieDebug2("Information about Max Decceleration is Not available for this device");
    return (ERR_NA);
    //	return dynamixelConfig->maxDecceleration;
}


/*!
 * This function returns the driver the ContinuousCurrentLimit parameter
 * \warning NOT AVAILABLE FOR THIS DYNAMIXEL DRIVER
 * @return ERR_NA (Data is not available for this driver)
 * \see DynamixelConfiguration.h
 * \see ADerrors.h
 */
int DynamixelDriver::getContinousCurrentLimit(){
    maggieDebug2("Information about Continuous Current Limit is not available for this device");
    return (ERR_NA);
    //	return dynamixelConfig->continousCurrentLimit;
}


/*!
 * This function returns the driver the PeakCurrentLimit parameter
 * \warning NOT AVAILABLE FOR THIS DYNAMIXEL DRIVER
 * @return ERR_NA (Data is not available for this driver)
 * \see DynamixelConfiguration.h
 * \see ADerrors.h
 */
int DynamixelDriver::getPeakCurrentLimit(){
    maggieDebug2("Information about Peak Current Limit is not available for this device");
    return (ERR_NA);
    //	return dynamixelConfig->peakCurrentLimit;
}

/*!
 * This function returns configuration filename stored in the configuration
 * @return DynamixelConfiguration::configFile (dynamixelConfig->configFile)The configuration filename
 * \see DynamixelConfiguration.h
 */
string DynamixelDriver::getConfigFile(){
    return dynamixelConfig->configFile;
}

//------------------------ Configuration Access Functions (Setters)-------------------------------
/*!
 * This function sets the driver maxPosition and if succeeds updates the driverConfiguration::maxPosition with the entered parameter
 * @param maxPosition The value to set the maximum position in the driver. Units: pulses
 * @return ERR_NOERR
 */
int DynamixelDriver::setMaxPosition(long int maxPosition){
    dxl_write_word( dynamixelConfig->getID(),CCW_ANGLE_LIMIT_L, maxPosition);
    DriverInterface::setMaxPosition(maxPosition);
    return ERR_NOERR;
}

/*!
 * This function sets the driver minPosition and if succeeds updates the driverConfiguration::minPosition with the entered parameter
 * @param minPosition The value to set the minimum position in the driver. Units: pulses
 * @return ERR_NOERR
 */
int DynamixelDriver::setMinPosition(long int minPosition){
    dxl_write_word( dynamixelConfig->getID(),CW_ANGLE_LIMIT_L, minPosition);
    DriverInterface::setMinPosition(minPosition);
    return ERR_NOERR;
}


/*!
 * This function sets the driver maxVelocity and if succeeds updates the driverConfiguration::maxVelocity with the entered parameter
 * @param maxVelocity The value to set the maximum velocity in the driver. Units: pulses/sec
 * @return ERR_NOERR
 */
int DynamixelDriver::setMaxVelocity(long int maxVelocity){

    if (maxVelocity > MAX_ALLOWED_VELOCITY){
        maxVelocity = MAX_ALLOWED_VELOCITY;
    }
    else if (maxVelocity < 0){
        maxVelocity *= -1; // changing the sign of maxVelocity
    }

    dxl_write_word( dynamixelConfig->getID(),P_GOAL_SPEED_L, maxVelocity);
    DriverInterface::setMaxVelocity(maxVelocity);
    return ERR_NOERR;


}

/*!
 * This function sets the driver maxAcceleration and if succeeds updates the driverConfiguration::maxAcceleration with the entered parameter
 * \warning NOT AVAILABLE FOR THIS DYNAMIXEL DRIVER
 * @param maxAcceleration The value to set the maximum acceleration in the driver.
 * @return ERR_NA (Data is not available for this driver)
 * \see ADerrors.h
 */
int DynamixelDriver::setMaxAcceleration(long int maxAcceleration){
    maggieDebug2("Information about Max Acceleration is Not available for this device");
    return ERR_NOERR;
}

/*!
 * This function sets the driver maxDecceleration and if succeeds updates the driverConfiguration::maxDecceleration with the entered parameter
 * \warning NOT AVAILABLE FOR THIS DYNAMIXEL DRIVER
 * \param maxDecceleration The value to set the maximum decceleration in the driver. Units:
 * @return ERR_NA (Data is not available for this driver)
 * \see ADerrors.h
 */
int DynamixelDriver::setMaxDecceleration(long int maxDecceleration){
    maggieDebug2("Information about Max Decceleration is Not available for this device");
    return ERR_NOERR;
}

/*!
 * This function sets the driver continousCurrentLimit and if succeeds updates the DriverConfiguration::continousCurrentLimit with the entered parameter
 * \warning NOT AVAILABLE FOR THIS DYNAMIXEL DRIVER
 * \param continousCurrentLimit The value to set the continuous current limit in the driver. Units: mA
 * @return ERR_NA (Data is not available for this driver)
 * \see ADerrors.h
 */
int DynamixelDriver::setContinousCurrentLimit(int continousCurrentLimit){
    maggieDebug2("Information about Continuous Current Limit is Not available for this device");
    return ERR_NOERR;
}

/*!
 * This function sets the driver peakCurrentLimit and if succeeds updates the DriverConfiguration::PeakCurrentLimit with the entered parameter
 * \warning NOT AVAILABLE FOR THIS DYNAMIXEL DRIVER
 * \param peakCurrentLimit The value to set the peak current Limit in the driver. Units: mA
 * @return ERR_NA (Data is not available for this driver)
 * \see ADerrors.h
 */
int DynamixelDriver::setPeakCurrentLimit(int peakCurrentLimit){
    maggieDebug2("Information about Peak Current Limit is Not available for this device");
    return ERR_NOERR;
}


/*!
 * this function sets the configuration file
 * @param confFile the configuration file name
 * @return ERR_NOERR
 */
int DynamixelDriver::setConfigFile (string confFile){
    dynamixelConfig->configFile = confFile;
    return ERR_NOERR;
}


/*!
 * This function open and to start the communication with the driver.
 * @return ERR_COM iin case of failing the communication with the driver. It also sets DynamixelDriver::dynamixelStatus (DynamixelStatus::connected) to false
 *         ERR_NOERR otherwise. It also sets DynamixelStatus::connected to true
 */
int DynamixelDriver::connect() {

    char c_portName[MAX_PORTNAME_SIZE];

    if (dynamixelConfig->namePort.length() > MAX_PORTNAME_SIZE)
    {
        maggieDebug2("Error portName exceeds the allowable size");
        return ERR_OUTOFRANGE;
    }
    else
    {
        sprintf(c_portName, "%s", dynamixelConfig->namePort.data());
        if( dxl_initialize(c_portName, baudnum) == 0 )
        {
            maggieDebug2( "Failed to open USB2Dynamixel!" );
            dynamixelStatus->isConnected(false);
            return ERR_COM;
        }
        else
        {
            dynamixelStatus->isConnected(true);
            return ERR_NOERR;
        }
    }
}

/*!
 * This function end the communication with the driver
 * \return ERR_NOERR. It also set dynamixelStatus->isConnected to false
 */
int DynamixelDriver::disconnect() {
    dxl_terminate();
    dynamixelStatus->isConnected(false);
    return ERR_NOERR;
}


/*!
 * This function prints in stdout all the limits from DriverConfiguration and don't return any value
 */
void DynamixelDriver::printLimits () {
    cout << endl;
    cout << "Max Position: " << getMaxPosition() << endl;
    cout << "Min Position: " << getMinPosition() << endl;
    cout << "Max Velocity: " << getMaxVelocity() << endl;
    cout << endl;
}


/*!
 * This function prints in stdout all the non-limits configuration from DriverConfiguration
 * \warning NOT AVAILABLE FOR THIS DYNAMIXEL DRIVER
 */
void DynamixelDriver::printConfig() {
    /*DriverInterface::printConfiguration();
 cout << "Driver Serial Device: " << getSerialDevice() << endl;
 cout << "Driver BaudRate: " << getBaudrate() << endl;
 cout << "Driver SemFile: " << getSemFile() << endl;
 cout << endl;*/
}


/*!
 * This function prints in stdout all the MotorSensor data from DriverMotorSensor
 */
void DynamixelDriver::printMotorSensorInfo () {
    cout << endl;
    cout << "-- Driver MotorSensor --" << endl;
    if (updateMotorSensorInfo() != ERR_NOERR){
        perror ("Could not update the MotorSensor");
    }else {
        cout << "Current Position: " << getPosition() << endl;
        cout << "Current Speed: " << getVelocity() << endl;
        cout << "Current Temperature: " << getTemperature() << endl;
        cout << "Current Voltage: " << getVoltage() << endl;
        cout << "Current Load: " << getLoad()  << endl;
        dynamixelMotorSens->getLoadDirection();
        cout << endl;
    }
}

/*!
 * This function prints in stdout all the status data from DriverStatus
 */
void DynamixelDriver::printStatus () {
    cout << endl;
    cout << "-- Driver Status -- " << endl;
    cout << "True = yes. False = no" << endl;
    cout << "Driver is enabled: " << boolToString(isEnabled()) << endl;
    cout << "Driver is calibrated: " << boolToString( isCalibrated()) << endl;
    cout << "Limit Sensor is reached: " << boolToString(isLimitSensorReached()) << endl;
    cout << "OverTemperature warning: " << boolToString(isOverTemperatureWarning()) << endl;
    cout << "OverVoltage warning: " << boolToString(isOverVoltageWarning()) << endl;
    cout << "Driver is connected: " << boolToString(isConnected()) << endl;
    cout << endl;
}
