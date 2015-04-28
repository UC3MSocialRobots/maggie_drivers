#ifndef __SERIAL_COMM_H__
#define __SERIAL_COMM_H__

/**
 * @file        irtrans_wrapper.h
 * @brief       IRTrans wrapper for ir controller.
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

#include <ros/ros.h>
#include "ir_driver_interface.h"

// third party driver headers
#define LINUX
extern "C" {
#include <IRTrans.h>
}

class IrTransWrapper : public IrDriverInterface {
    public:
        /**
         * @brief Empty constructor.
         */
        IrTransWrapper();

        /**
         * @brief Destructor.
         */
        ~IrTransWrapper();

        /**
         * @brief Method to connect with a socket.
         * @param
         * @return error: -1 or successful: 0.
         */
        int connect();

        /**
         * @brief Method to disconnect from a socket.
         * @param
         * @return
         */
        void disconnect();

        /**
         * @brief Method that send remote commands by a socket.
         * @param rem, the remote host.
         * @param com, the command to send.
         * @return error: -1 or successful: 0.
         */
        int send_remote_command(std::string remote, std::string command);

    private:
        SOCKET _irt_server;
};

#endif
