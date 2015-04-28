/**
 * @file        serial_communication_mock.h
 * @brief       Mock for serial communication.
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
#include "serial_communication_interface.h"

class MockSerialCommunication : public SerialCommunicationInterface {
    public:
        MOCK_METHOD1(set_serial_device, void(std::string serial_device));
        MOCK_METHOD1(open_port, int(struct termios *oldtio));
        MOCK_METHOD1(close_port, int(struct termios *oldtio));
        MOCK_METHOD1(send_character, int(unsigned char *str));
};
