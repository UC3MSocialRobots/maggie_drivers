/**
 * @file        MCDC3006SStatus.cpp
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

#include "mcdc3006s/MCDC3006SStatus.h"

//////////////////////////////////////////////////

MCDC3006SStatus::MCDC3006SStatus() :
    DriverStatus()
{
    connected = false;
    enabled = false;
    calibrated = false;
    currentLimitated = false;
    overTemperatureWarning = false;
    overVoltageWarning = false;
    limitSensorReached = false;
}

//////////////////////////////////////////////////

MCDC3006SStatus::~MCDC3006SStatus()
{
}

//////////////////////////////////////////////////
