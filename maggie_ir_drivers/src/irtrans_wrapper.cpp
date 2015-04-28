/**
 * @file        irtrans_wrapper.cpp
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

#include "maggie_ir_drivers/irtrans_wrapper.h"

//////////////////////////////////////////////////

IrTransWrapper::IrTransWrapper() :
    _irt_server()
{
}

//////////////////////////////////////////////////

IrTransWrapper::~IrTransWrapper()
{
}

//////////////////////////////////////////////////

int IrTransWrapper::connect()
{
    int res, error = 0;
    char * host = (char*) "127.0.0.1";

    if ((res = ConnectIRTransServer(host, &_irt_server)) != 0) {
        ROS_ERROR("[IRTRANS_WRAPPER] Error: %i connecting to IR controller server", res);
        error = -1;
    }
    else {
        ROS_INFO("[IRTRANS_WRAPPER] Successful connection with IR controller server");
    }

    return error;
}

//////////////////////////////////////////////////

void IrTransWrapper::disconnect()
{
    DisconnectIRTransServer(_irt_server);
}

//////////////////////////////////////////////////

int IrTransWrapper::send_remote_command(std::string remote, std::string command)
{
    NETWORKSTATUS *stat;
    int error = 0;

    stat = SendRemoteCommand(_irt_server, &remote[0], &command[0], 0, 0, 0);
    if (stat) {
        ROS_WARN("[IRTRANS_WRAPPER] Warning: sending IR command: '%s'", stat->message);
        error = -1;
    }
    else {
        ROS_INFO("[IRTRANS_WRAPPER] IR command sent: '%s'", remote.c_str());
    }

    return error;
}

//////////////////////////////////////////////////
