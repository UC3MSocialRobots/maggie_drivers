/**
 * \file DynamixelConfiguration.cpp
 * \date 01/09/2010
 * \author Paulina Vélez Núñez (mailto: pvelez@ing.uc3m.es)
 *
 * This file provides several functions to control the Dynamixel driver...
 */


#include "dynamixel/DynamixelConfiguration.h"
#include "dynamixel/lib/dxl_hal.h"
#include "debug/debug_utils.h"

const std::string DynamixelConfiguration::defaultConfigFile = "/conf/ojo";

DynamixelConfiguration::DynamixelConfiguration ()
    : DriverConfiguration::DriverConfiguration () {
    this->configFile = defaultConfigFile;
     maggieDebug2("Driver Configuration File is:'%s'", this->configFile.c_str());

}

DynamixelConfiguration::DynamixelConfiguration (string confFile ) : DriverConfiguration::DriverConfiguration () {
    this->configFile = confFile;
     maggieDebug2("Driver Configuration File is:'%s'", this->configFile.c_str());
}


DynamixelConfiguration :: ~DynamixelConfiguration(){

}

/*!
 * This function return the id parameter, that is the unique ID for each Dynamixel driver
 * @return id
 */
int DynamixelConfiguration::getID(){
    return this->id;
}

/*!
 * This function return the minimum voltage that is set by the user for the driver
 * @return minVoltage
 */
float DynamixelConfiguration::getMinVoltage(){
    return this->minVoltage;
}

/*!
 * This function return the maximum voltage that is set by the user for the driver
 * @return maxVoltage
 */
float DynamixelConfiguration::getMaxVoltage(){
    return this->maxVoltage;
}
/*!
 * This function return the maximum temperature that is set by the user for the driver
 * @return maxTemperature
 */
int DynamixelConfiguration::getMaxTemperature(){
    return this->maxTemperature;
}

/*!
 * This function return the minimum temperature that is set by the user for the driver
 * @return minTemperature
 */
int DynamixelConfiguration::getMinTemperature(){
    return this->minTemperature;
}

/*!
 * This function set the minimum voltage for the driver
 * @param minVoltage
 * \warning If the user don't set the minimum voltage, it will be set by default to the minimum
 *          value that device need (7 Volts)
 */
void DynamixelConfiguration::setMinVoltage (float minVoltage){
    if (minVoltage >= 7){
        this->minVoltage = minVoltage;
    } else{
        this->minVoltage = defaultMinVoltage;
    }
}

/*!
 * This function set the maximum voltage for the driver
 * @param maxVoltage
 * \warning If the user don't set the maximum voltage, it will be set by default to the maximum
 *          value that device support (12.1 Volts)
 */
void DynamixelConfiguration::setMaxVoltage (float maxVoltage){
    if (maxVoltage >= 12.1){
        this->maxVoltage = maxVoltage;
    } else{
        this->maxVoltage = defaultMaxVoltage;
    }
}

/*!
 * This function set the maximum temperature that driver support
 * @param maxTemperature
 * \warning If the user don't set the maximum temperature, it will be set by default to the maximum
 *          value that device support (85 º C)
 */void DynamixelConfiguration::setMaxTemperature (int maxTemperature){
    if (maxTemperature >= 85){
        this->maxTemperature = maxTemperature;
    } else{
        this->maxTemperature = defaultMaxTemperature;
    }
}

/*!
 *  This function set the minimum temperature that driver support
 * @param minTemperature
 * \warning If the user don't set the temperature temperature, it will be set by default to the minimum
 *          value that device support (-5 º C)
 */
void DynamixelConfiguration::setMinTemperature (int minTemperature){
    if (minTemperature >= -5){
        this->minTemperature = minTemperature;
    } else{
        this->minTemperature = defaultMinTemperature;
    }
}

/*!
 * This function set the ID for each Dynamixel driver connected to the net
 * @param id
 */
void DynamixelConfiguration::setID(int id){
    this->id= id;
}
