/*
 * MCDC3006SMotorSensor.h
 *
 *  Created on: 17/05/2010
 *      Author: Victor Gonzalez Pacheco (mailto: vgonzale@ing.uc3m.es)
 */

#ifndef MCDC3006SMOTORSENSOR_H_
#define MCDC3006SMOTORSENSOR_H_

/*
 *
 */
#include "driver_interface/DriverMotorSensor.h"
#include "mcdc3006s/MCDC3006Slib/MCDC3006S.h"

class MCDC3006SMotorSensor: public DriverMotorSensor {
        friend class MCDC3006SDriver;
public:
        MCDC3006SMotorSensor();

        virtual ~MCDC3006SMotorSensor();


private:

};

#endif /* MCDC3006SODOMETRY_H_ */
