#ifndef MCDC3006SCONFIGURATION_H_
#define MCDC3006SCONFIGURATION_H_

/**
 * @file        MCDC3006SConfiguration.h
 * @brief       Node for controlling the mcdc3006s motors (neck and arms).
 *
 * @author      Raul Perula-Martinez <raul.perula@uc3m.es>
 * @date        2015-02
 * @author      Victor Gonzalez Pacheco <vgonzale@ing.uc3m.es>
 * @date        2010-05
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

#include <iostream>
#include <string>
#include "driver_interface/DriverConfiguration.h"
#include "mcdc3006s/MCDC3006Slib/MCDC3006S.h"
#include "communications/serialPort/RS232.h"

using namespace std;
using std::cout;
using std::string;
using std::ios;
using std::endl;

class MCDC3006SConfiguration : public DriverConfiguration {
        friend class MCDC3006SDriver;
    public:
        /**
         * @brief Empty constructor.
         */
        MCDC3006SConfiguration();

        /**
         * @brief Parametrized constructor.
         */
        MCDC3006SConfiguration(std::string confFile);

        /**
         * @brief Desctructor.
         */
        virtual ~MCDC3006SConfiguration();

        /**
         * \brief gets the rs232_t of the mcdc3006sconfiguration
         * \return rs232 an rs232_t instance where the information of the serial port is stored
         */
        rs232_t getRS232() const;

        /**
         * \brief gets the semaphore of the mcdc3006sconfiguration
         * \return semaphore an instance of the semaphore where the information of the semaphore is stored
         */
        semaphore_t getSemaphore() const;

        /**
         * \brief sets the rs232_t of the mcdc3006sconfiguration
         * \param rs232 an rs232_t instance where the information of the serial port is stored
         */
        void setRS232(rs232_t rs232);

        /**
         * \brief sets the semaphore of the mcdc3006sconfiguration
         * \param sem an instance of the semaphore where the information of the semaphore is stored
         */
        void setSemaphore(semaphore_t sem);

    private:
        rs232_t rs232;
        semaphore_t semaphore;

        /** Default configuration file. */
        static const std::string defaultConfigFile;
};

#endif
