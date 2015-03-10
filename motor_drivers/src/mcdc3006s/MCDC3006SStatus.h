/**
 * \file MCDC3006SStatus.h
 * \date 17/05/2010
 * \author: Victor Gonzalez Pacheco (mailto: vgonzale@ing.uc3m.es)
 */

#ifndef MCDC3006SSTATUS_H_
#define MCDC3006SSTATUS_H_

/*
 *
 */
#include "driver_interface/DriverStatus.h"
#include "mcdc3006s/MCDC3006Slib/MCDC3006S.h"

class MCDC3006SStatus : public DriverStatus {
        friend class MCDC3006SDriver;
public:
        MCDC3006SStatus();
        virtual ~MCDC3006SStatus();

private:
        driverStatus_t  statusData;				/** Struct used to store the status of the driver. **/
};

#endif /* MCDC3006SSTATUS_H_ */
