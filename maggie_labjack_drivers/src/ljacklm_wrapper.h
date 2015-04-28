#ifndef __LJACKLM_WRAPPER_H__
#define __LJACKLM_WRAPPER_H__

/**
 * @file        ljacklm_wrapper.h
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

#include <ros/ros.h>
#include <ljacklm.h>
#include "labjack_driver_interface.h"

// set the I/O lines
// 0=input, 1=output

// D15 D14 D13 D12 D11 D10 D9 D8 D7 D6 D5 D4 D3 D2 D1 D0
// 0   0   0   0   0   1   0  0  0  0  0  0  0  0  0  1  = 0x401
#define CONFIG_D    0x401

// IO3 IO2 IO1 IO0
// 1   1   1   0   = 0xE
#define CONFIG_IO   0xE

// default values
#define DEMO_MODE   0
#define GAIN        0

// identify the labjack (-1 => first found)
#define ID_LJ       -1

// error code because a wrong parameter
#define ERROR_PARAM -1

class LjacklmWrapper : public LabjackDriverInterface {
    public:
        /**
         * @brief Configure input/output lines in labjack.
         * @return error code or 0 (no error).
         */
        long config();

        /**
         * @brief Reset the labjack
         * @return error code or 0 (no error).
         */
        long reset();

        /**
         * @brief Read the value of one IO line.
         * @param channel line to read: channel >= 0 & channel <= 3.
         * @param state value of the line state: state > 0 -> high level, state = 0 -> low level.
         * @return error code, 0 (no error) or <0 if any parameter is incorrect.
         */
        long readIO(long channel, long * state);

        /**
         * @brief Write the value of one IO line.
         * @param channel line to read: channel >= 0 & channel <= 3.
         * @param state value of the line state: state > 0 -> high level, state = 0 -> low level.
         * @return error code, 0 (no error) or <0 if any parameter is incorrect.
         */
        long writeIO(long channel, long state);

        /**
         * @brief Read all the D and IO input lines.
         * @param stateD D line states (D0-D15).
         * @param stateIO IO line states (IO0-IO3).
         * @return error code or 0 (no error).
         */
        long readDIOs(long * stateD, long * stateIO);

        /**
         * @brief Write and read all D and IO input lines.
         * @param direcD D lines direction (0=input, 1=output).
         * @param direcIO IO lines direction (0=input, 1=output).
         * @param stateD D lines level (D0-D15).
         * @param stateIO IO lines level (IO0-IO3).
         * @return error code, 0 (no error) or <0 if any parameter is incorrect.
         */
        long writeDIOs(long * direcD, long * direcIO, long * stateD, long * stateIO);

        /**
         * @brief Write and read all D and IO input lines depending on CONFIG_D and CONFIG_IO.
         * @param stateD D lines level (D0-D15).
         * @param stateIO IO lines level (IO0-IO3).
         * @return error code, 0 (no error) or <0 if any parameter is incorrect.
         */
        long writeDIOsCONFIG(long * stateD, long * stateIO);

        /**
         * @brief Read the value of one D line.
         * @param channel line to read: channel >= 0 & channel <= 3.
         * @param state value of the line state: state > 0 -> high level, state = 0 -> low level.
         * @return error code or 0 (no error).
         */
        long readD(long channel, long * state);

        /**
         * @brief Write the value of one D line.
         * @param channel line to read: channel >= 0 & channel <= 3.
         * @param state value of the line state: state > 0 -> high level, state = 0 -> low level.
         * @return error code or 0 (no error).
         */
        long writeD(long channel, long state);

        /**
         * @brief Write the value of one D line and configure the D lines.
         * @param channel line to read: channel >= 0 & channel <= 3.
         * @param state value of the line state: state > 0 -> high level, state = 0 -> low level.
         * @return error code or 0 (no error).
         */
        long writeDCONFIG(long channel, long state);

        /**
         * @brief Read the value of one AI line.
         * @param channel channel number to read: 0..7. (e.g. to channel=0 => read AI0).
         * @param voltage read voltage to return.
         * @return error code, 0 (no error) or <0 if any parameter is incorrect.
         */
        long readAI(long channel, float * voltage);

        /**
         * @brief Read the voltage of several AI lines.
         * @param num_channels number of channels to read: 1, 2 ó 4.
         * @param channels array with channels values: channels[num_channels] = {0,..,7}. (e.g. to channels={0,1} => read AI0 and AI1).
         * @param voltages array with read voltages to return.
         * @return error code, 0 (no error) or <0 if any parameter is incorrect.
         */
        long readAIs(long num_channels, long * channels, float * voltages);

        /**
         * @brief Read the voltage in a differential way of the AI lines.
         * @param channel channel number to read: 8..11. (e.g. to channel=8 => read difference between AI0 and AI1, and so on).
         * @param voltages array with read voltages to return.
         * @return error code, 0 (no error) or <0 if any parameter is incorrect.
         */
        long readDifferentialAI(long channel, float * voltage);

        /**
         * @brief Write the value of one AO line.
         * @param channel channel number to assign a voltage: channel=0->AO0, channel=1->AO1 and channel=2->AO0&AO1.
         * @param voltage value of the voltage to assign: voltage >= 0.0 & voltage <= 5.0.
         * @return error code, 0 (no error) or <0 if any parameter is incorrect.
         */
        long writeAO(int channel, float voltage);

        /**
         * @brief Write the voltage of AO0 and AO1 lines.
         * @param voltageAO0 voltage value to assign to AO0 (voltageAO0 >= 0.0 & voltageAO0 <= 5.0).
         * @param voltageAO1 voltage value to assign to AO1 (voltageAO1 >= 0.0 & voltageAO1 <= 5.0).
         * @return error code, 0 (no error) or <0 if any parameter is incorrect.
         */
        long writeAOs(float voltageAO0, float voltageAO1);

        /**
         * @brief Enable and configure the watchdog.
         * @param timeout time in seconds to reset (1-715).
         * @param activeD D line to enable (0,1,8).
         * @param state state of the D line (0-1).
         * @return error code, 0 (no error) or <0 if any parameter is incorrect.
         */
        long enableWatchdog(long timeout, int activeD, long state);

        /**
         * @brief Disable the watchdog for a D line.
         * @param activeD D line to disable (0,1,8).
         * @return error code, 0 (no error) or <0 if any parameter is incorrect.
         */
        long disableWatchdog(int activeD);

        /**
         * @brief Generate the pulse in one the lines D0..D15.
         * @param pulse_level pulse level: 0->high level, 0->low level.
         * @param line line to use (for lines D:0..15; for lines IO:0..3).
         * @param pulse_time time in microseconds for a pulse (1..2000000).
         * @param line_type type of line, 0=D line, 1=IO line
         * @return error code, 0 (no error) or <0 if any parameter is incorrect.
         */
        long writePulse(long pulse_level, long line, long pulse_time, long line_type);

        /**
         * @brief Read the D line directions (in binary, 0:input and 1:output)
         * @param directions directions of the D lines.
         * @return error code, 0 (no error) or <0 if any parameter is incorrect.
         */
        long readDirectionDs(long * directions);
};

#endif
