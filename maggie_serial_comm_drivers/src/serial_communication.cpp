/**
 * @file        serial_communication.cpp
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

#include "maggie_serial_comm_drivers/serial_communication.h"

//////////////////////////////////////////////////

SerialCommunication::SerialCommunication() :
    _file_descriptor(-1)
{
}

//////////////////////////////////////////////////

SerialCommunication::~SerialCommunication()
{
}

//////////////////////////////////////////////////

void SerialCommunication::set_serial_device(std::string serial_device)
{
    _serial_device = serial_device;
}

//////////////////////////////////////////////////

int SerialCommunication::open_port(struct termios *oldtio)
{
    struct termios newtio;
    int error = 0;

    // open the serial port
    _file_descriptor = open(_serial_device.c_str(), O_RDWR | O_NOCTTY);

    if (_file_descriptor < 0) {
        ROS_ERROR("[EYELIDS] Error: opening serial device %s", _serial_device.c_str());
        exit(-1);
    }

    // save current port settings
    if (tcgetattr(_file_descriptor, oldtio) != 0) {
        error = -1;
    }

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    newtio.c_cflag &= ~PARENB;
    newtio.c_cflag &= ~CSTOPB;
    newtio.c_cflag &= ~CSIZE;
    newtio.c_cflag |= CS8;
    newtio.c_oflag = 0;

    // set input mode (non-canonical, no echo,...)
    newtio.c_lflag = 0;

    newtio.c_cc[VTIME] = 0; // inter-character timer unused
    newtio.c_cc[VMIN] = 1;  // blocking read until 1 chars received

    if (tcflush(_file_descriptor, TCIFLUSH) != 0) {
        error = -1;
    }
    if (tcsetattr(_file_descriptor, TCSANOW, &newtio) != 0) {
        error = -1;
    }

    return error;
}

//////////////////////////////////////////////////

int SerialCommunication::close_port(struct termios *oldtio)
{
    int error = 0;

    if (tcsetattr(_file_descriptor, TCSANOW, oldtio) != 0) {
        error = -1;
    }
    if (close(_file_descriptor) != 0) {
        error = -1;
    }

    return error;
}

//////////////////////////////////////////////////

int SerialCommunication::send_character(unsigned char *str)
{
    int error = 0;

    if (write(_file_descriptor, str, 1) == -1) {
        ROS_ERROR("[EYELIDS] Error: sending character by the serial port");
        error = -1;
    }
    ROS_DEBUG("[EYELIDS] Character sent: %u", *str);

    return error;
}

//////////////////////////////////////////////////
