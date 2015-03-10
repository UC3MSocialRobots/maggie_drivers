/**
 * @file driverMCD3006S.c
 * @brief Software implementation of the Aplication Program Interface for the drive model faulhaberMCDC3006S
 * @author Víctor Gonzalez vgonzale@ing.uc3m.es
 * @date 22 January 2009
 */

#include "mcdc3006s/MCDC3006Slib/MCDC3006S.h"

int initFromFile(char * configurationFile)
{
    driverConf_t dc;
    // set dc from configurationFile

    // init(dc)
    return 0;
}

int setDriverConf(driverConf_t dc)
{
    if (setDriverMinPos(dc.RSd, dc.sem.semID, dc.minPos) < ERR_NOERR) {
        perror("setDriverConf() --> Error setting the minimum position\n\r");
        return ERR_CONF;
    }
    if (setDriverMaxPos(dc.RSd, dc.sem.semID, dc.maxPos) < ERR_NOERR) {
        perror("setDriverConf() --> Error setting the minimum position\n\r");
        return ERR_CONF;
    }
    if (setDriverMaxVel(dc.RSd, dc.sem.semID, dc.maxVel) < ERR_NOERR) {
        perror("setDriverConf() --> Error setting the minimum position\n\r");
        return ERR_CONF;
    }
    if (setDriverMaxAcc(dc.RSd, dc.sem.semID, dc.maxAcc) < ERR_NOERR) {
        perror("setDriverConf() --> Error setting the minimum position\n\r");
        return ERR_CONF;
    }
    if (setDriverMaxDec(dc.RSd, dc.sem.semID, dc.maxDec) < ERR_NOERR) {
        perror("setDriverConf() --> Error setting the minimum position\n\r");
        return ERR_CONF;
    }

    return ERR_NOERR;
}

int getDriverConf(int RSd, int semID, driverConf_t * dc)
{
    (*dc).maxPos = getDriverMaxPos(RSd, semID);
    if (dc->maxPos == ERR_COM) {
        perror("getDriverConf() --> Error reading the configuration from the driver\n\r");
        return ERR_COM;
    }

    (*dc).minPos = getDriverMinPos(RSd, semID);
    if (dc->minPos == ERR_COM) {
        perror("getDriverConf() --> Error reading the configuration from the driver\n\r");
        return ERR_COM;
    }

    (*dc).maxVel = getDriverMaxVel(RSd, semID);
    if (dc->maxVel == ERR_COM) {
        perror("getDriverConf() --> Error reading the configuration from the driver\n\r");
        return ERR_COM;
    }

    (*dc).maxAcc = getDriverMaxAcc(RSd, semID);
    if (dc->maxAcc == ERR_COM) {
        perror("getDriverConf() --> Error reading the configuration from the driver\n\r");
        return ERR_COM;
    }

    (*dc).maxDec = getDriverMaxDec(RSd, semID);
    if (dc->maxDec == ERR_COM) {
        perror("getDriverConf() --> Error reading the configuration from the driver\n\r");
        return ERR_COM;
    }

    (*dc).cCLimit = getDriverCurLim(RSd, semID);
    if (dc->maxAcc == ERR_COM) {
        perror("getDriverConf() --> Error reading the configuration from the driver\n\r");
        return ERR_COM;
    }

    return ERR_NOERR;
}

int saveToFlash(int RSd, int semID)
{
    char command[SP_MSG_SIZE], response[SP_MSG_SIZE];

    sprintf(command, "EEPSAV\n\r\0");
    if (askToRS232(RSd, semID, command, sizeofchar(command), response) < ERR_NOERR) {
        perror("saveToFlash() --> Error. cannot communicate with the driver\n\r");
        return ERR_COM;
    }
    fprintf(stderr, "driverMCDC30065S - saveToFlash : %s\n", response);
    return ERR_NOERR;
}

int activateLimits(int RSd, int semID, int action)
{
    char command[SP_MSG_SIZE];

    if (action == ACTIVATE || action == DEACTIVATE) {
        sprintf(command, "APL%d\n\r", action);

        if (writeToRS232(RSd, semID, command, sizeofchar(command)) < ERR_NOERR) {
            perror("activateLimits() --> Error. cannot write to the driver\n\r");
            return ERR_WRI;
        }
        return ERR_NOERR;
    }
    perror("activateLimits() --> Error Out of range");
    return ERR_OUTOFRANGE;
}

int setDriverMaxPos(int RSd, int semID, long int maxPos)
{
    char command[SP_MSG_SIZE];

    if (maxPos <= 0) {
        fprintf(stderr, "setDriveMaxPos --> ERROR. Out of range: "
            "maxPos must be higher than 0 (You entered %ld).\n\r", maxPos);
        return ERR_OUTOFRANGE;
    }
    sprintf(command, "LL%ld\n\r\0", maxPos);

    if (writeToRS232(RSd, semID, command, sizeofchar(command)) < ERR_NOERR) {
        perror("setDriverMaxPos() --> Error writing to the driver\n\r");
        return ERR_WRI;
    }

    if (activateLimits(RSd, semID, ACTIVATE) < ERR_NOERR) {
        perror("setDriverMaxPos() --> Error activating the driver\n\r");
        return ERR_POSLIMIT;
    }
    return ERR_NOERR;
}

int setDriverMinPos(int RSd, int semID, long int minPos)
{
    char command[SP_MSG_SIZE];

    if (minPos >= 0) {
        fprintf(stderr, "setDriveMinPos --> ERROR. minPos must be lower than 0 (You entered %ld).\n\r", minPos);
        return ERR_OUTOFRANGE;
    }

    sprintf(command, "LL%ld\n\r\0", minPos);

    if (writeToRS232(RSd, semID, command, sizeofchar(command)) < ERR_NOERR) {
        perror("setDriverMinPos() --> Error writing to the driver\n\r");
        return ERR_WRI;
    }

    if (activateLimits(RSd, semID, ACTIVATE) < ERR_NOERR) {
        perror("setDriverMaxPos() --> Error activating the driver\n\r");
        return ERR_POSLIMIT;
    }

    return ERR_NOERR;
}

int setDriverMaxVel(int RSd, int semID, long int maxVel)
{

    char command[SP_MSG_SIZE];

    sprintf(command, "SP%ld\n\r\0", maxVel);

    if (writeToRS232(RSd, semID, command, sizeofchar(command)) < ERR_NOERR) {
        perror("setDriverMaxVel() --> Error writing to the driver\n\r");
        return ERR_WRI;
    }

    return ERR_NOERR;

}

int setDriverMaxAcc(int RSd, int semID, long int maxAcc)
{
    char command[SP_MSG_SIZE];

    sprintf(command, "AC%ld\n\r\0", maxAcc);

    if (writeToRS232(RSd, semID, command, sizeofchar(command)) < ERR_NOERR) {
        perror("setDriverMaxAcc() --> Error writing to the driver\n\r");
        return ERR_WRI;
    }

    return ERR_NOERR;

}

int setDriverMaxDec(int RSd, int semID, long int maxDec)
{
    char command[SP_MSG_SIZE];

    sprintf(command, "DEC%ld\n\r\0", maxDec);

    if (writeToRS232(RSd, semID, command, sizeofchar(command)) < ERR_NOERR) {
        perror("setDriverMaxDec() --> Error writing to the driver\n\r");
        return ERR_WRI;
    }

    return ERR_NOERR;

}

int setDriverCurLim(int RSd, int semID, int cl)
{
    char command[SP_MSG_SIZE];

    sprintf(command, "LCC%d\n\r\0", cl);

    if (writeToRS232(RSd, semID, command, sizeofchar(command)) < ERR_NOERR) {
        perror("setDriverCurLim() --> Error writing to the driver\n\r");
        return ERR_WRI;
    }

    return ERR_NOERR;
}

int setDriverPCurLim(int RSd, int semID, int pcl)
{
    char command[SP_MSG_SIZE];

    sprintf(command, "LPC%d\n\r\0", pcl);

    if (writeToRS232(RSd, semID, command, sizeofchar(command)) < ERR_NOERR) {
        perror("setDriverPCurLim() --> Error writing to the driver\n\r");
        return ERR_WRI;
    }

    return ERR_NOERR;
}

long int getDriverMaxVel(int RSd, int semID)
{
//	long int vmax;
    char command[SP_MSG_SIZE]; // "GSP\n\r\0";
    char response[SP_MSG_SIZE];

    sprintf(command, "GSP\n\r\0");

    if (askToRS232(RSd, semID, command, sizeofchar(command), response) < ERR_NOERR) {
        perror("getDriverMaxVel() --> Error communicating with driver\n\r");
        return ERR_COM;
    }

    return (atol(response)); // Converting the response of the driver to a long int.

}

long int getDriverMaxAcc(int RSd, int semID)
{
    //  long int maxAcc = 0.0;
    char command[SP_MSG_SIZE];
    char response[SP_MSG_SIZE];

    strcpy(command, "GAC\n\r\0");
    if (askToRS232(RSd, semID, command, sizeofchar(command), response) < ERR_NOERR) {
        perror("getDriverMaxAcc() --> Error communicating with driver\n\r");
        return ERR_COM;
    }

    return (atol(response)); // Converting the response of the driver to a long int.
}

long int getDriverMaxDec(int RSd, int semID)
{

    char command[SP_MSG_SIZE];
    char response[SP_MSG_SIZE];

    strcpy(command, "GDEC\n\r\0");

    if (askToRS232(RSd, semID, command, sizeofchar(command), response) < ERR_NOERR) {
        perror("getDriverMaxDec() --> Error communicating with driver\n\r");
        return ERR_COM;
    }

    return (atol(response)); // Converting the response of the driver to a long int.
}

long int getDriverMaxPos(int RSd, int semID)
{

    char command[SP_MSG_SIZE], response[SP_MSG_SIZE];

    sprintf(command, "GPL\r");

    if (askToRS232(RSd, semID, command, sizeofchar(command), response) < ERR_NOERR) {
        perror("getDriverMaxPos() --> Error communicating with driver\n\r");
        return ERR_COM;
    }

    return atol(response); // Converting the response of the driver to a long int.
}

long int getDriverMinPos(int RSd, int semID)
{
    static char command[SP_MSG_SIZE];
    char response[SP_MSG_SIZE];
    long int minPos;

    sprintf(command, "GNL\r");

    if (askToRS232(RSd, semID, command, sizeofchar(command), response) < ERR_NOERR) {
        perror("getDriverMinPos() --> Error when communicating with the driver\n\r");
        return ERR_COM;
    }

    minPos = atol(response);

    return minPos;
}

int getDriverCurLim(int RSd, int semID)
{
    static char command[SP_MSG_SIZE];
    char response[SP_MSG_SIZE];

    sprintf(command, "GCC\n\r\0");

    if (askToRS232(RSd, semID, command, sizeofchar(command), response) < ERR_NOERR) {
        perror("getDriverCurLim() --> Error communicating with the driver\n\r");
        return ERR_COM;
    }

    return (atoi(response));
}

int getDriverPCurLim(int RSd, int semID)
{
    static char command[SP_MSG_SIZE];
    char response[SP_MSG_SIZE];

    sprintf(command, "GPC\n\r\0");

    if (askToRS232(RSd, semID, command, sizeofchar(command), response) < ERR_NOERR) {
        perror("getDriverPCurLim() --> Error communicating with the driver\n\r");
        return ERR_COM;
    }

    return (atoi(response));
}

int setDriverBaud(int RSd, int semID, int baud)
{
    int c = -1;
    int n;
    char command[SP_MSG_SIZE];
    for (n = 1; n < 10; n++) {
        if (baud == 600 * n) {
            c = 1;
            break;
        }
    }
    if (c) {
        sprintf(command, "BAUD%d\n\r", baud);
    }
    else {
        fprintf(stderr, "driverMCDC3006S - setDriverBaud ERROR %d not supported\n\r", baud);
        return ERR_OUTOFRANGE;
    }
    if (writeToRS232(RSd, semID, command, sizeofchar(command)) < ERR_NOERR) {
        perror("setDriverCurLim() --> Error writing to the driver\n\r");
        return ERR_WRI;
    }

    return ERR_NOERR;
}

int getDriverSensor(int RSd, int semID, driverSensor_t * sensor)
{
    char command[SP_MSG_SIZE];
    char response[SP_MSG_SIZE];

    int status = ERR_NOERR;

    bzero((void *) response, sizeof(response));

    sprintf(command, "POS\n\r\0");			// Get current Position
    if (askToRS232(RSd, semID, command, sizeofchar(command), response) < ERR_NOERR) {
        perror("getDriverOdometry() --> Error when communicating with the driver\n\rCould not establish current position\n\r");
        return (ERR_COM);
    }
    (*sensor).p = atol(response);

    sprintf(command, "GV\n\r\0");			// Get current velocity --> Current target velocity in rpm
    if (askToRS232(RSd, semID, command, sizeofchar(command), response) < ERR_NOERR) {
        perror("getDriverOdometry() --> Error when communicating with the driver\n\rCould not establish current velocity\n\r");
        return (ERR_COM);
    }
    (*sensor).v = atol(response);

    sprintf(command, "GRC\n\r\0");			// Get current instant current in mA
    if (askToRS232(RSd, semID, command, sizeofchar(command), response) < ERR_NOERR) {
        perror("getDriverOdometry() --> Error when communicating with the driver\n\rCould not establish current instant current\n\r");
        return (ERR_COM);
    }
    (*sensor).i = atol(response);

    return (ERR_NOERR);
}

int getDriverInstantPos(int RSd, int semID, long int * position)
{
    char command[SP_MSG_SIZE];
    char response[SP_MSG_SIZE];
    int status = ERR_NOERR;

    bzero((void *) response, sizeof(response));
    sprintf(command, "POS\n\r\0");			// Get current Position
    if (askToRS232(RSd, semID, command, sizeofchar(command), response) < ERR_NOERR) {
        perror("getDriverInstantPos() --> Error when communicating with the driver\n\rCould not establish current position\n\r");
        return (ERR_COM);
    }
    *position = atol(response);
    return ERR_NOERR;
}

int getDriverInstantVel(int RSd, int semID, long int * velocity)
{
    char command[SP_MSG_SIZE];
    char response[SP_MSG_SIZE];
    int status = ERR_NOERR;

    bzero((void *) response, sizeof(response));
    sprintf(command, "GV\n\r\0");			// Get Velocity
    if (askToRS232(RSd, semID, command, sizeofchar(command), response) < ERR_NOERR) {
        perror("getDriverInstantVel() --> Error when communicating with the driver\n\rCould not establish current instant current\n\r");
        return (ERR_COM);
    }
    *velocity = atol(response);
    return (ERR_NOERR);
}

int getDriverInstantCurrent(int RSd, int semID, int * current)
{
    char command[SP_MSG_SIZE];
    char response[SP_MSG_SIZE];
    int status = ERR_NOERR;

    bzero((void *) response, sizeof(response));
    sprintf(command, "GRC\n\r\0");			// Get current instant current in mA
    if (askToRS232(RSd, semID, command, sizeofchar(command), response) < ERR_NOERR) {
        perror("getDriverInstantCurrent() --> Error when communicating with the driver\n\rCould not establish current instant current\n\r");
        return (ERR_COM);
    }
    *current = atol(response);
    return (ERR_NOERR);
}

int moveDriverAbsPos(int RSd, int semID, long int p)
{
    long int pos = p;
    char command[SP_MSG_SIZE]; // = "LA";

    sprintf(command, "LA%ld\n\r\0", pos);
    if (writeToRS232(RSd, semID, command, sizeofchar(command)) < ERR_NOERR) {
        perror("moveDriverAbsPos() --> Error writing to the driver\n\r");
        return ERR_WRI;
    }

    sprintf(command, "M\n\r\0");
    if (writeToRS232(RSd, semID, command, sizeofchar(command)) < ERR_NOERR) {
        perror("moveDriverAbsPos() --> Error writing to the driver\n\r");
        return ERR_WRI;
    }
    return ERR_NOERR;
}

int moveDriverRelPos(int RSd, int semID, long int p)
{
    long int pos = p;
    char command[SP_MSG_SIZE]; // = "LR";

    sprintf(command, "LR%ld\n\r\0", pos);
    if (writeToRS232(RSd, semID, command, sizeofchar(command)) < ERR_NOERR) {
        perror("moveDriverRelPos() --> Error writing to the driver\n\r");
        return ERR_WRI;
    }

    sprintf(command, "M\n\r\0");
    if (writeToRS232(RSd, semID, command, sizeofchar(command)) < ERR_NOERR) {
        perror("moveDriverRelPos() --> Error writing to the driver\n\r");
        return ERR_WRI;
    }
    return ERR_NOERR;
}

int moveDriverVel(int RSd, int semID, long int v)
{
    char command[SP_MSG_SIZE]; //

    sprintf(command, "V%ld\n\r\0", v);
    if (writeToRS232(RSd, semID, command, sizeofchar(command)) < ERR_NOERR) {
        perror("moveDriverVel() --> Error writing to the driver\n\r");
        return ERR_WRI;
    }
    return ERR_NOERR;

}

int enableDriver(int RSd, int semID)
{
    char command[] = "EN\n\r\0";

    if (writeToRS232(RSd, semID, command, sizeofchar(command)) < ERR_NOERR) {
        perror("EnableDriver() --> Error\n\r");
        return ERR_WRI;
    }
    return ERR_NOERR;
}

int disableDriver(int RSd, int semID)
{
    char command[] = "DI\n\r\0";

    if (writeToRS232(RSd, semID, command, sizeofchar(command)) < ERR_NOERR) {
        perror("DisableDriver() --> Error\n\r");
        return ERR_WRI;
    }
    return ERR_NOERR;
}

int setDriverHomePosition(int RSd, int semID, long int home)
{
    char command[SP_MSG_SIZE];
    sprintf(command, "HO%ld\n\r", home);

    if (writeToRS232(RSd, semID, command, sizeofchar(command))) {
        perror("setDriverHomePosition() --> Error\n\r");
        return ERR_WRI;
    }

    return ERR_NOERR;
}

int calibrateDriver(int RSd, int semID, long int limit, int currentLimit, int calibrationSpeed, int timeOut)
{
    struct timeb timer;
    char calibrationCommand[SP_MSG_SIZE];	// This is the input 4 of the driver. In this input is connected the sensor.
    char calibrationResponse[SP_MSG_SIZE];

    int status = 1;
    struct timeval before, now;

    // Configurating the driver parameters
    sprintf(calibrationCommand, "LCC%d\n\r", currentLimit); // LCC: Load Continous Current LPC: Load peak current
    if (writeToRS232(RSd, semID, calibrationCommand, sizeofchar(calibrationCommand)) < ERR_NOERR) {
        perror("calibrateDriver() --> Error\n\r");
        return ERR_NOHOME;
    }

    sprintf(calibrationCommand, "HL8\n\r", currentLimit);
    if (writeToRS232(RSd, semID, calibrationCommand, sizeofchar(calibrationCommand)) < ERR_NOERR) {
        perror("calibrateDriver() --> Error\n\r");
        return ERR_NOHOME;
    }

    sprintf(calibrationCommand, "V%d\n\r", calibrationSpeed);
    if (writeToRS232(RSd, semID, calibrationCommand, sizeofchar(calibrationCommand)) < ERR_NOERR) {
        perror("calibrateDriver() --> Error\n\r");
        return ERR_NOHOME;
    }

    gettimeofday(&before, 0);
    do {
        askToRS232(RSd, semID, "OST\n\r\0", sizeofchar("OST\n\r\0"), calibrationResponse); /// @ToDo Error control here
        gettimeofday(&now, 0);

        if (atoi(calibrationResponse) & DRIVER_INPUT_4_MASK) {
            status = ERR_NOERR; // Sensor Reached OK.
        }
        else if (atoi(calibrationResponse) & CURRENT_LIMITING_MASK) {
            perror("calibrateDriver() --> Error Calibrating the driver. Current Limit Reached Could not establish home position");
            status = ERR_CURLIM; // Error calibrating the driver (limit current reached)
        }
        else if (timeDifferenceMsec(&before, &now) > timeOut) {
            perror("calibrateDriver() --> Error Calibrating the driver. Timeout. Could not establish home position");
            status = ERR_TIMEOUT; // Timeout reached before arriving to the sensor
        }
    }
    while(status == 1);

    // Assuring that the driver stops at this point
    writeToRS232(RSd, semID, "V0\n\r\0", sizeofchar("V0\n\r\0"));

    // Moving the driver to 0.
    if (status == ERR_NOERR) {

        if (setDriverHomePosition(RSd, semID, limit) == 0) {
            moveDriverAbsPos(RSd, semID, 0); // Moving to 0 position.
        }
        else {
            perror("calibrateDriver() --> Error Calibrating the driver. Could not establish home position");
            status = ERR_NOHOME;
        }
    }
    return (status);
}

int getDriverStatus(int RSd, int semID, driverStatus_t * drvStatus)
{

    char command[SP_MSG_SIZE];
    char response[SP_MSG_SIZE];

    sprintf(command, "GST\n\r\0");			//
    if (askToRS232(RSd, semID, command, sizeofchar(command), response) < ERR_NOERR) {
        perror("getDriverStatus() --> Error when communicating with the driver\n\rCould not establish driver status (GST).\n\r");
        return (ERR_COM);
    } 	// Now response has the driver "status" information
    drvStatus->disabled = atoi(response) & ENABLED_MASK;

    sprintf(command, "OST\n\r\0");			//
    if (askToRS232(RSd, semID, command, sizeofchar(command), response) < ERR_NOERR) {
        perror("getDriverStatus() --> Error when communicating with the driver\n\rCould not establish driver Operative Status (OST).\n\r");
        return (ERR_COM);
    } // Now response has the driver "fault status" information

    drvStatus->curLimiting = atoi(response) & CURRENT_LIMITING_MASK;
    drvStatus->overVoltage = atoi(response) & OVERVOLTAGE_MASK;
    drvStatus->overTemperature = atoi(response) & OVERTEMPERATURE_MASK;
    drvStatus->sensorReached = atoi(response) & DRIVER_INPUT_4_MASK;

    drvStatus->curLimiting = (drvStatus->curLimiting > 0) ? TRUE : FALSE;
    drvStatus->disabled = (drvStatus->disabled > 0) ? TRUE : FALSE;
    drvStatus->overTemperature = (drvStatus->overTemperature > 0) ? TRUE : FALSE;
    drvStatus->overVoltage = (drvStatus->overVoltage > 0) ? FALSE : FALSE;
    drvStatus->sensorReached = (drvStatus->sensorReached > 0) ? FALSE : FALSE;

    return ERR_NOERR;
}
