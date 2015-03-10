#ifndef __SERIAL_COMM_H__
#define __SERIAL_COMM_H__

/**
 * @file        serial_communication.h
 * @brief       Serial communication driver for servomotors.
 *
 * @author      Raul Perula-Martinez <raul.perula@uc3m.es>
 * @date        2015-02
 * @author      David Garcia Godoy <>
 * @date        2008-04
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

#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ros/ros.h>
#include "serial_communication_interface.h"

class SerialCommunication : public SerialCommunicationInterface {
    public:
        /**
         * @brief Empty constructor.
         */
        SerialCommunication();

        /**
         * @brief Destructor.
         */
        ~SerialCommunication();

        /**
         * @brief Method to set the serial device name.
         * @param serial_device, the serial device name.
         * @return
         */
        void set_serial_device(std::string serial_device);

        /**
         * @brief Method to open and configure the serial port.
         * @param oldtio, structure to configure the serial port.
         * @return error: -1 or successful: 0.
         */
        int open_port(struct termios *oldtio);

        /**
         * @brief Method to return the original config to the serial port and close it.
         * @param oldtio, structure to configure the serial port.
         * @return error: -1 or successful: 0.
         */
        int close_port(struct termios *oldtio);

        /**
         * @brief Method that send characters by the serial port for the at89c2051.
         * @param str, string to send by the serial port.
         * @return error: -1 or successful: 0.
         */
        int send_character(unsigned char *str);

    private:
        std::string _serial_device;

        struct termios _oldtio;
        int _file_descriptor;
};

#endif
