/**
 * \file MCDC3006SConfiguration.cpp
 *
 *  \date 14/05/2010
 *  \author Victor Gonzalez Pacheco (mailto: vgonzale@ing.uc3m.es)
 */

#include "mcdc3006s/MCDC3006SConfiguration.h"

const std::string MCDC3006SConfiguration::defaultConfigFile = "$/conf/brazo_derecho";

MCDC3006SConfiguration::MCDC3006SConfiguration ()
    : DriverConfiguration::DriverConfiguration () {
    this->configFile = defaultConfigFile;
    maggieDebug2("Driver Configuration File is:'%s'", this->configFile.c_str());
}

MCDC3006SConfiguration::MCDC3006SConfiguration(std::string confFile) : DriverConfiguration::DriverConfiguration () {
    this->configFile = confFile;
    maggieDebug2("Driver Configuration File is:'%s'", this->configFile.c_str());
}

MCDC3006SConfiguration::~MCDC3006SConfiguration() {
    endCommunication (rs232.RSd,semaphore.semID);			// Closing the serial port and the semaphore
}

// Getters
/**
 * \brief gets the rs232_t of the mcdc3006sconfiguration
 * \return rs232 an rs232_t instance where the information of the serial port is stored
 */
rs232_t MCDC3006SConfiguration::getRS232() const {
    return this->rs232;
}

/**
 * \brief gets the semaphore of the mcdc3006sconfiguration
 * \return semaphore an instance of the semaphore where the information of the semaphore is stored
 */
semaphore_t MCDC3006SConfiguration::getSemaphore() const {
    return this->semaphore;
}

// Setters
/**
 * \brief sets the semaphore of the mcdc3006sconfiguration
 * \param sem an instance of the semaphore where the information of the semaphore is stored
 */
void MCDC3006SConfiguration::setSemaphore(semaphore_t sem) {
    this->semaphore = sem;
}

/**
 * \brief sets the rs232_t of the mcdc3006sconfiguration
 * \param rs232 an rs232_t instance where the information of the serial port is stored
 */
void MCDC3006SConfiguration::setRS232(rs232_t rs232) {
    this->rs232 = rs232;
}
