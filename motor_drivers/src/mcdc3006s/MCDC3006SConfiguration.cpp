/**
 * @file        MCDC3006SConfiguration.cpp
 * @brief       Node for controlling the mcdc3006s motors (neck and arms).
 *
 * @author      Raul Perula-Martinez <raul.perula@uc3m.es>
 * @date        2015-02
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

#include "mcdc3006s/MCDC3006SConfiguration.h"

const std::string MCDC3006SConfiguration::defaultConfigFile = "$/conf/brazo_derecho";

//////////////////////////////////////////////////

MCDC3006SConfiguration::MCDC3006SConfiguration() :
    DriverConfiguration::DriverConfiguration()
{
    this->configFile = defaultConfigFile;
    maggieDebug2("Driver Configuration File is:'%s'", this->configFile.c_str());
}

//////////////////////////////////////////////////

MCDC3006SConfiguration::MCDC3006SConfiguration(std::string confFile) :
    DriverConfiguration::DriverConfiguration()
{
    this->configFile = confFile;
    maggieDebug2("Driver Configuration File is:'%s'", this->configFile.c_str());
}

//////////////////////////////////////////////////

MCDC3006SConfiguration::~MCDC3006SConfiguration()
{
    // Closing the serial port and the semaphore
    endCommunication(rs232.RSd, semaphore.semID);
}

//////////////////////////////////////////////////

rs232_t MCDC3006SConfiguration::getRS232() const
{
    return this->rs232;
}

//////////////////////////////////////////////////

semaphore_t MCDC3006SConfiguration::getSemaphore() const
{
    return this->semaphore;
}

//////////////////////////////////////////////////

void MCDC3006SConfiguration::setRS232(rs232_t rs232)
{
    this->rs232 = rs232;
}

//////////////////////////////////////////////////

void MCDC3006SConfiguration::setSemaphore(semaphore_t sem)
{
    this->semaphore = sem;
}

//////////////////////////////////////////////////
