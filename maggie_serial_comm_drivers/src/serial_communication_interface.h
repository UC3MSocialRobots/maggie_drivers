#ifndef __SERIAL_COMM_INTERFACE_H__
#define __SERIAL_COMM_INTERFACE_H__

/**
 * @file        serial_communication_interface.h
 * @brief       Serial communication interface for servomotors.
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

class SerialCommunicationInterface {
    public:
        /**
         * @brief Constructor.
         */
        SerialCommunicationInterface()
        {
        }

        /**
         * @brief Abstract destructor.
         */
        virtual ~SerialCommunicationInterface()
        {
        }

        /**
         * @brief Method to set the serial device name.
         * @param serial_device, the serial device name.
         * @return
         */
        virtual void set_serial_device(std::string serial_device) = 0;

        /**
         * @brief Method to open and configure the serial port.
         * @param oldtio, structure to configure the serial port.
         * @return error: -1 or successful: 0.
         */
        virtual int open_port(struct termios *oldtio) = 0;

        /**
         * @brief Method to return the original config to the serial port and close it.
         * @param oldtio, structure to configure the serial port.
         * @return error: -1 or successful: 0.
         */
        virtual int close_port(struct termios *oldtio) = 0;

        /**
         * @brief Method that send characters by the serial port for the at89c2051.
         * @param str, string to send by the serial port.
         * @return error: -1 or successful: 0.
         */
        virtual int send_character(unsigned char *str) = 0;
};

#endif
