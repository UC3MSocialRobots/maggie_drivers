/**
 * @file        ljacklm_mock.h
 * @brief       Mock for labjact driver.
 *
 * @author      Raul Perula-Martinez <raul.perula@uc3m.es>
 * @date        2015-02
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

#include <gmock/gmock.h>
#include "labjack_driver_interface.h"

class MockLjacklmWrapper : public LabjackDriverInterface {
    public:
        MOCK_METHOD0(config, long());
        MOCK_METHOD0(reset, long());
        
        MOCK_METHOD1(disableWatchdog, long(int activeD));
        MOCK_METHOD1(readDirectionDs, long(long * directions));
        
        MOCK_METHOD2(readIO, long(long channel, long * state));
        MOCK_METHOD2(writeIO, long(long channel, long state));
        MOCK_METHOD2(readDIOs, long(long * stateD, long * stateIO));
        MOCK_METHOD2(writeDIOsCONFIG, long(long * stateD, long * stateIO));
        MOCK_METHOD2(readD, long(long channel, long * state));
        MOCK_METHOD2(writeD, long(long channel, long state));
        MOCK_METHOD2(writeDCONFIG, long(long channel, long state));
        MOCK_METHOD2(readAI, long(long channel, float * voltage));
        MOCK_METHOD2(readDifferentialAI, long(long channel, float * voltage));
        MOCK_METHOD2(writeAO, long(int channel, float voltage));
        MOCK_METHOD2(writeAOs, long(float voltageAO0, float voltageAO1));
        
        MOCK_METHOD3(readAIs, long(long num_channels, long * channels, float * voltages));
        MOCK_METHOD3(enableWatchdog, long(long timeout, int activeD, long state));
        
        MOCK_METHOD4(writeDIOs, long(long * direcD, long * direcIO, long * stateD, long * stateIO));
        MOCK_METHOD4(writePulse, long(long pulse_level, long line, long pulse_time, long line_type));
};
