/**
 * @file        ljacklm_wrapper.cpp
 * @brief       Ljacklm wrapper for the labjack controller.
 *
 *  Extra info: Labjack model U12
 *
 * @author      Raul Perula-Martinez <raul.perula@uc3m.es>
 * @date        2015-02
 * @author      Victor Gonzalez <victor.gonzalez@uc3m.es>
 * @date        2011-04
 * @author      Alvaro Castro-Gonzalez <acgonzal@ing.uc3m.es>
 * @date        2007-12
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

#include "maggie_labjack_drivers/ljacklm_wrapper.h"

//////////////////////////////////////////////////

long LjacklmWrapper::config()
{
    long error = 0;
    char errors[50];

    // D0 to 1 for not activate the watchdog, rest to 0
    long stateD = 0x0001;
    // output state of IO2 and IO3 to 0 for beginning in SLEEP state,
    // IO1 to 1 for not activate the emergency
    long stateIO = 0x0002;

    long trisD = CONFIG_D;
    long trisIO = CONFIG_IO;

    // when the watchdog is launched, the line goes to low
    error = writeDIOs(&trisD, &trisIO, &stateD, &stateIO);
    if (error != 0) {
        GetErrorString(error, errors);
        ROS_ERROR("[LJACKLM_WRAPPER] Error: %s", errors);
    }

    return error;
}

//////////////////////////////////////////////////

long LjacklmWrapper::reset()
{
    long error = 0;

    long idlj = ID_LJ;

    error = Reset(&idlj);

    return error;
}

//////////////////////////////////////////////////

long LjacklmWrapper::readIO(long channel, long * state)
{
    long error = 0;

    long idlj = ID_LJ;

    if (channel < 0 || channel > 3) {
        error = ERROR_PARAM;
        ROS_ERROR("[LJACKLM_WRAPPER] Error in readIO: channel out of range");
    }
    else {
        error = EDigitalIn(&idlj, DEMO_MODE, channel, 0, state);
    }

    return error;
}

//////////////////////////////////////////////////

long LjacklmWrapper::writeIO(long channel, long state)
{
    long error = 0;

    long idlj = ID_LJ;

    if (channel < 0 || channel > 3) {
        error = ERROR_PARAM;
        ROS_ERROR("[LJACKLM_WRAPPER] Error in writeIO: channel out of range");
    }
    else {
        error = EDigitalOut(&idlj, DEMO_MODE, channel, 0, state);
        ROS_DEBUG("[LJACKLM_WRAPPER] Channel: %ld, State: %ld, Error: %ld\n", channel, state, error);
    }

    return error;
}

//////////////////////////////////////////////////

long LjacklmWrapper::readDIOs(long * stateD, long * stateIO)
{
    long error = 0;

    long idlj = ID_LJ;
    long trisD = 0;     // necessary but not used
    long trisIO = 0;    // necessary but not used
    long outputD = 0;

    *stateD = *stateIO = 0;

    error = DigitalIO(&idlj, DEMO_MODE, &trisD, trisIO, stateD, stateIO, 0, &outputD);

    return error;
}

//////////////////////////////////////////////////

long LjacklmWrapper::writeDIOs(long * direcD, long * direcIO, long * stateD, long * stateIO)
{
    long error = 0;

    long idlj = ID_LJ;
    long outputD = 0;   // necessary but not used

    if (*direcD > 0xFFFF
        || *direcD < 0x0
        || *stateD > 0xFFFF
        || *stateD < 0x0
        || *direcIO > 0xF
        || *direcIO < 0x0
        || *stateIO > 0xFFFF
        || *stateIO < 0x0) {
        error = ERROR_PARAM;
    }
    else {
        error = DigitalIO(&idlj, DEMO_MODE, direcD, *direcIO, stateD, stateIO, 1, &outputD);
    }

    return error;
}

//////////////////////////////////////////////////

long LjacklmWrapper::writeDIOsCONFIG(long * stateD, long * stateIO)
{
    long error = 0;

    long idlj = ID_LJ;
    long trisD = CONFIG_D;
    long trisIO = CONFIG_IO;
    long outputD = 0;       // necessary but not used

    if (*stateD > 0xFFFF || *stateD < 0x0 || *stateIO > 0xFFFF || *stateIO < 0x0) {
        error = ERROR_PARAM;
    }
    else {
        error = DigitalIO(&idlj, DEMO_MODE, &trisD, trisIO, stateD, stateIO, 1, &outputD);
    }

    return error;
}

//////////////////////////////////////////////////

long LjacklmWrapper::readD(long channel, long * state)
{
    long error = 0;

    long idlj = ID_LJ;

    if (channel < 0 || channel > 15) {
        error = ERROR_PARAM;
        ROS_ERROR("[LJACKLM_WRAPPER] Error in readD: channel out of range");
    }
    else {
        error = EDigitalIn(&idlj, DEMO_MODE, channel, 1, state);
    }

    return error;
}

//////////////////////////////////////////////////

long LjacklmWrapper::writeD(long channel, long state)
{
    long error = 0;

    long idlj = ID_LJ;

    if (channel < 0 || channel > 15) {
        error = ERROR_PARAM;
        ROS_ERROR("[LJACKLM_WRAPPER] Error in writeD: channel out of range");
    }
    else {
        ROS_DEBUG("[LJACKLM_WRAPPER] channel=%ld , state=%ld\n", channel, state);

        error = EDigitalOut(&idlj, DEMO_MODE, channel, 1, state);
    }

    return error;
}

//////////////////////////////////////////////////

long LjacklmWrapper::writeDCONFIG(long channel, long state)
{
    long error = 0;

    long idlj = ID_LJ;
    long stateD = 0, stateIO = 0, outputD = 0;
    long trisD = CONFIG_D;
    long trisIO = CONFIG_IO;
    long mask = 0x0001; // 0x0001 = 0000 0000 0000 0001

    if (channel < 0 || channel > 15) {
        error = ERROR_PARAM;
        ROS_ERROR("[LJACKLM_WRAPPER] Error in writeDCONFIG: channel out of range");
    }
    else {
        ROS_DEBUG("[LJACKLM_WRAPPER] channel=%ld, state=%ld\n", channel, state);

        mask <<= channel; // change the mask to point to the line we want to write

        error = DigitalIO(&idlj, DEMO_MODE, &trisD, trisIO, &stateD, &stateIO, 0, &outputD);

        if (state) {
            // write '1'
            stateD = stateD | mask;
        }
        else {
            // write '0'
            stateD = stateD & (~mask);
        }

        error = DigitalIO(&idlj, DEMO_MODE, &trisD, trisIO, &stateD, &stateIO, 1, &outputD);
    }

    return error;
}

//////////////////////////////////////////////////

long LjacklmWrapper::readAI(long channel, float * voltage)
{
    long error = 0;

    long idlj = ID_LJ;
    long overVoltage = 0;

    if (channel >= 0 && channel <= 7) {
        error = EAnalogIn(&idlj, DEMO_MODE, channel, GAIN, &overVoltage, voltage);
    }

    return error;
}

//////////////////////////////////////////////////

long LjacklmWrapper::readAIs(long num_channels, long * channels, float * voltages)
{
    long error = 0;

    long idlj = ID_LJ;
    long stateIO = 0;   // necessary but not used
    long updateIO = 0;  // do a reading
    long gains[4] = {0,
                     0,
                     0,
                     0};
    long disableCal = 0;
    long overVoltage;

    // check parameters
    if (num_channels != 1 && num_channels != 2 && num_channels != 4) {
        error = ERROR_PARAM;
        ROS_ERROR("[LJACKLM_WRAPPER] Error in readAIs: channel different to 1, 2, 4");
    }
    else {
        for (long i = 0; i < num_channels; i++) {
            if (channels[i] < 0 || channels[i] > 7) {
                error = ERROR_PARAM;
                ROS_ERROR("[LJACKLM_WRAPPER] Error in readAIs: channel out of range");
                break;
            }
        }
    }

    if (!error) {
        // init array before using, necessary for AISample
        for (long i = 0; i < num_channels; i++) {
            voltages[i] = 0;
        }

        error = AISample(&idlj, DEMO_MODE, &stateIO, updateIO, 1, num_channels, channels, gains, disableCal,
                         &overVoltage, voltages);

        if (error) {
            ROS_ERROR("[LJACKLM_WRAPPER] Error in AISample:");
            ROS_ERROR("[LJACKLM_WRAPPER] AISample:");
        }
    }

    return error;
}

//////////////////////////////////////////////////

long LjacklmWrapper::readDifferentialAI(long channel, float * voltage)
{
    long error = 0;

    long idlj = ID_LJ;
    long overVoltage = 0;

    if (channel >= 8 && channel <= 11) {
        error = EAnalogIn(&idlj, DEMO_MODE, channel, 1, &overVoltage, voltage);
    }

    return error;
}

//////////////////////////////////////////////////

long LjacklmWrapper::writeAO(int channel, float voltage)
{
    long error = 0;

    long idlj = ID_LJ;
    float analogOut0 = -1.0, analogOut1 = -1.0;

    // check the channel
    if (channel == 0) {
        analogOut0 = voltage;
    }
    else if (channel == 1) {
        analogOut1 = voltage;
    }
    else if (channel == 2) {
        analogOut0 = analogOut1 = voltage;
    }
    else {
        error = ERROR_PARAM;
    }

    // check the voltage value
    if (voltage < 0.0 || voltage > 5.0) {
        error = ERROR_PARAM;
        ROS_ERROR("[LJACKLM_WRAPPER] Error in writeAO: voltage out of range");
    }
    else {
        error = EAnalogOut(&idlj, DEMO_MODE, analogOut0, analogOut1);
    }

    return error;
}

//////////////////////////////////////////////////

long LjacklmWrapper::writeAOs(float voltageAO0, float voltageAO1)
{
    long error = 0;

    long idlj = ID_LJ;

    // check the correct voltages
    if (voltageAO0 < 0.0 || voltageAO0 > 5.0 || voltageAO1 < 0.0 || voltageAO1 > 5.0) {
        error = ERROR_PARAM;
    }
    else {
        error = EAnalogOut(&idlj, DEMO_MODE, voltageAO0, voltageAO1);
    }

    return error;
}

//////////////////////////////////////////////////

long LjacklmWrapper::enableWatchdog(long timeout, int activeD, long state)
{
    long error = 0;

    long idlj = ID_LJ;
    long activeD0, activeD1, activeD8, stateD0, stateD1, stateD8;
    activeD0 = activeD1 = activeD8 = stateD0 = stateD1 = stateD8 = 0;

    if (timeout < 1 || timeout > 715 || state < 0 || (activeD != 0 && activeD != 1 && activeD != 8)) {
        error = ERROR_PARAM;
    }
    else {
        if (activeD == 0) {
            activeD0 = 1;
            stateD0 = state;
        }
        else if (activeD == 1) {
            activeD1 = 1;
            stateD1 = state;
        }
        // activeD = 8
        else {
            activeD8 = 1;
            stateD8 = state;
        }

        error = Watchdog(&idlj, DEMO_MODE, 1, timeout, 0, activeD0, activeD1, activeD8, stateD0, stateD1, stateD8);
    }

    return error;
}

//////////////////////////////////////////////////

long LjacklmWrapper::disableWatchdog(int activeD)
{
    long error = 0;

    long idlj = ID_LJ;
    long timeout = 5; // a whatever value
    long activeD0, activeD1, activeD8, stateD0, stateD1, stateD8;
    activeD0 = activeD1 = activeD8 = stateD0 = stateD1 = stateD8 = 0;

    ROS_DEBUG("[LJACKLM_WRAPPER] timeout=%ld , activeD%d=%ld\n", timeout, activeD, stateD0);

    if (activeD != 0 && activeD != 1 && activeD != 8) {
        error = ERROR_PARAM;
    }
    else {
        if (activeD == 0) {
            activeD0 = 1;
        }
        else if (activeD == 1) {
            activeD1 = 1;
        }
        else {
            // activeD = 8
            activeD8 = 1;
        }

        error = Watchdog(&idlj, DEMO_MODE, 0, timeout, 0, activeD0, activeD1, activeD8, stateD0, stateD1, stateD8);
    }

    return error;
}

//////////////////////////////////////////////////

long LjacklmWrapper::writePulse(long pulse_level, long line, long pulse_time, long line_type)
{
    long error = 0;

    // D lines
    if (line_type == 0) {
        if (line < 0 || line > 15 || pulse_time < 1 || pulse_time > 2000000) {
            error = ERROR_PARAM;
        }
        else {
            if (pulse_level > 0) {
                if ((error = writeD(line, 0)) != 0)
                    return error;
                if ((error = writeD(line, 1)) != 0)
                    return error;
                usleep(pulse_time);
                if ((error = writeD(line, 0)) != 0)
                    return error;
            }
            else { // pulse_level <= 0
                if ((error = writeD(line, 1)) != 0)
                    return error;
                if ((error = writeD(line, 0)) != 0)
                    return error;
                usleep(pulse_time);
                if ((error = writeD(line, 1)) != 0)
                    return error;
            }
        }
    }
    // IO lines
    else if (line_type == 1) {
        if (line < 0 || line > 3 || pulse_time < 1 || pulse_time > 2000000) {
            error = ERROR_PARAM;
        }
        else {
            if (pulse_level > 0) {
                if ((error = writeIO(line, 0)) != 0)
                    return error;
                if ((error = writeIO(line, 1)) != 0)
                    return error;
                usleep(pulse_time);
                if ((error = writeIO(line, 0)) != 0)
                    return error;
            }
            // pulse_level <= 0
            else {
                if ((error = writeIO(line, 1)) != 0)
                    return error;
                if ((error = writeIO(line, 0)) != 0)
                    return error;
                usleep(pulse_time);
                if ((error = writeIO(line, 1)) != 0)
                    return error;
            }
        }
    }
    else {
        error = ERROR_PARAM;
    }

    return error;
}

//////////////////////////////////////////////////

long LjacklmWrapper::readDirectionDs(long * directions)
{
    long error = 0;

    long idlj = ID_LJ;
    long trisIO = 0;        // necessary but not used
    long outputD = 0;       // necessary but not used
    long stateD = 0;        // necessary but not used
    long stateIO = 0;       // necessary but not used

    *directions = 0;

    error = DigitalIO(&idlj, DEMO_MODE, directions, trisIO, &stateD, &stateIO, 0, &outputD);

    return error;
}

//////////////////////////////////////////////////
