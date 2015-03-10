#ifndef __IR_DRIVER_INTERFACE_H__
#define __IR_DRIVER_INTERFACE_H__

/**
 * @file        ir_driver_interface.h
 * @brief       Interface for ir controller.
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

class IrDriverInterface {
    public:
        /**
         * @brief Constructor.
         */
        IrDriverInterface()
        {
        }

        /**
         * @brief Abstract destructor.
         */
        virtual ~IrDriverInterface()
        {
        }

        /**
         * @brief Method to connect with a socket.
         * @param
         * @@return error: -1 or successful: 0.
         */
        virtual int connect() = 0;

        /**
         * @brief Method to disconnect from a socket.
         * @param
         * @return
         */
        virtual void disconnect() = 0;

        /**
         * @brief Method that send remote commands by a socket.
         * @param rem, the remote host.
         * @param com, the command to send.
         * @return error: -1 or successful: 0.
         */
        virtual int send_remote_command(std::string remote, std::string command) = 0;
};

#endif
