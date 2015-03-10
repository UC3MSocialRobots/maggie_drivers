/**
 * \file MCDC3006SConfiguration.h
 *
 *  \date 14/05/2010
 *  \author Victor Gonzalez (mailto: vgonzale@ing.uc3m.es)
 */

#ifndef MCDC3006SCONFIGURATION_H_
#define MCDC3006SCONFIGURATION_H_


#include "driver_interface/DriverConfiguration.h"
#include "mcdc3006s/MCDC3006Slib/MCDC3006S.h"
#include "communications/serialPort/RS232.h"
#include <iostream>
#include <string>

using namespace std;
using std::cout;
using std::string;
using std::ios;
using std::endl;

class MCDC3006SConfiguration : public DriverConfiguration {
        friend class MCDC3006SDriver;
public:
        MCDC3006SConfiguration();
        MCDC3006SConfiguration(std::string confFile);			/** Constructor in which the configuration file is passed */
        virtual ~MCDC3006SConfiguration();

        // Getters
        rs232_t getRS232 () const;
        semaphore_t getSemaphore () const;

        // Setters
        void setRS232 (rs232_t rs232);
        void setSemaphore (semaphore_t sem);

private:
                rs232_t rs232;
                semaphore_t semaphore;
                 /** Default configuration file. */
                static const std::string defaultConfigFile;
};

#endif /* MCDC3006SCONFIGURATION_H_ */
