#ifndef MCD3006S_H
#define MCD3006S_H

/**
 * @file MCD3006S.h
 * @author Víctor González (mailto:vgonzale@ing.uc3m.es)
 * @date 22 of January of 2010
 */

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>      	// Modos de apertura de dispositivos
#include <termios.h>   // struct termios
#include <stdio.h>     	// printf
#include <stdlib.h>
#include <string.h>    	// bzero, strerror
#include <math.h>      	//Funciones sin, cos
#include <errno.h>		// errno y perror
#include <unistd.h>
#include <sys/timeb.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include "communications/ipc/semaphores.h"
#include "communications/serialPort/RS232.h"

#include "MCDC3006S_data.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @brief high level functions
     */
    int init(int baudrate, char * dev, driverConf_t);

    /**
     * @TODO Make this function in future work: In the future we will check if the driver
     *       configuration (at the power on) is the same to the default
     *       configuration stored in a file. If there are differences. We have to send a warning
     *       and reconfigure the driver from the file
     *
     * @brief This function initializes some drive configuration params, like serial
     *        communication baudrate, Fault-Pin configuration, encoder resolution, PID
     *        control params, etc. This function opens the serial port and establishes the
     *        necesary communication with the drive.
     * @param configurationFile with full path. Each line of this file must have a pair variable value. Lines that begins with the character # are taken as comment lines.
     * @return 0 everything OK
     *    -1 could not establish communication with the serial port.
     *    -2 could not read the configuration file
     *
     *  Warning: not tested funcion
     */
    int initFromFile(char *); /** @todo */

    /**
     * @brief function that configures the driver from a driverConf_t type
     * @param dc is where dc configuration is read in order to write it into the driver
     * @return ERR_NOERR if success
     * @return ERR_CONF if any error occurs
     */
    int setDriverConf(driverConf_t);

    /**
     * @brief function that gets the current driver configuration as is stored in the
     *        flash memory of the driver and writes it in a driverConf_t type format
     *
     * @param RSd is the file descriptor of the serial port
     * @param semID is the semaphore identificator
     * @param dc is the driverConf_t type where the driver configuration is stored
     *
     * @return ERR_NOERR if there are no problems
     * @return ERR_COM if it is not possible to read (any parameter) information from
     *         the driver. When this happens at least one of the parameters of the driverConf_t
     *         structure could be incomplete.
     */
    int getDriverConf(int, int, driverConf_t *);

    /* set configuration to the driver */

    /**
     * @brief Activates/Deactivates the driver Limits. If the limits aren't activated the driver
     *        will NOT respect the Maximum or Minimum positions.
     * @param RSd the RS232 File descriptor
     * @param semID Semaphore Identifier
     * @param action it only has two allowed values: ACTIVATE to activate de driver or DEACTIVATE
     *        to disºable de driver
     *
     * @return ERR_NOERR if everything works correctly
     * @return ERR_WRI in case of error when writing in the driver descriptor file
     * @return ERR_OUTOFRANGE if action is not in allowed values set
     */
    int activateLimits(int RSd, int semID, int action);

    /**
     * @brief Sets the maximum absolute position in pulses
     *
     * @param RSd is the file descriptor of the serial port connected to the driver
     * @param semID is the semaphore identificator
     * @param maxPos in pulses
     * @return ERR_NOERR if everything has gone correctly
     * @return ERR_OUTOFRANGE if maxPos is below or equal 0 (maxPos must be higher than 0).
     * @return ERR_WRI if could not write to the driver
     * @return ERR_POSLIMIT if it is not possible to activate the limits
     */
    int setDriverMaxPos(int, int, long int);

    /**
     * @brief Sets the minimum absolute position in pulses
     *
     * @param RSd is the file descriptor of the serial port connected to the driver
     * @param semID is the semaphore identificator
     * @param minPos is the minimum absolute position in pulses
     *
     * @return ERR_NOERR if everything has gone correctly
     * @return ERR_OUTOFRANGE if minPos is higher or equal than 0 (minPos must be lower than 0).
     * @return ERR_WRI if could not write to the driver
     * @return ERR_POSLIMIT if it is not possible to activate the limits
     */
    int setDriverMinPos(int, int, long int);

    /**
     * @brief Sets the maximum speed allowed by the driver in rpm
     *
     * @param RSd is the file descriptor of the serial port connected to the driver
     * @param semID is the semaphore identificator
     * @param maxVel in r.p.m.
     *
     * @return ERR_NOERR if everything goes correct
     * @return ERR_WRI if could not write to the driver
     */
    int setDriverMaxVel(int, int, long int);

    /**
     * @brief sets the maximum acceleration allowed by the driver in revolutions/s^2
     *
     * @param RSd is the file descriptor of the serial port connected to the driver
     * @param semID is the semaphore identificator
     * @param maxAcc maximum acceleration in revolutions/s^2
     *
     * @return ERR_NOERR if everything goes correct
     * @return ERR_WRI if could not write to the driver
     */
    int setDriverMaxAcc(int, int, long int);

    /**
     * @brief sets the maximum deceleration allowed by the driver in revolutions/s^2
     *
     * @param RSd is the file descriptor of the serial port connected to the driver
     * @param semID is the semaphore identificator
     * @param maxDec maximum deceleration in r/sec²
     *
     * @return ERR_NOERR if everything goes correct
     * @return ERR_WRI if could not write to the driver
     *
     */
    int setDriverMaxDec(int, int, long int);

    /**
     * @brief loads continous current limit
     *
     * @param RSd is the file descriptor of the serial port connected to the driver
     * @param semID is the semaphore identificator
     * @param cl current limit in mA Range = [0 - 12000mA]
     *
     * @return ERR_NOERR if everything goes correct
     * @return ERR_WRI if could not write to the driver
     */
    int setDriverCurLim(int, int, int);

    /**
     * @brief loads peak current limit
     *
     * @param RSd is the file descriptor of the serial port connected to the driver
     * @param semID is the semaphore identificator
     * @param pcl current limit in mA Range = [0 - 12000mA]
     *
     * @return ERR_NOERR if everything goes correct
     * @return ERR_WRI if could not write to the driver
     */
    int setDriverPCurLim(int RSd, int semID, int pcl);

    /**
     * @brief tells to the driver at which baudrate we want to operate
     *
     * @param RSd is the file descriptor of the serial port connected to the driver
     * @param semID is the semaphore identificator
     * @param baud is the baudrate of the communication.
     *
     * @return ERR_NOERR if the configuration is succesful
     * @return ERR_OUTOFRANGE if the parameter baud is not in the expected values
     * @return ERR_WRI if it is not possible to write to the driver the configuration
     */
    int setDriverBaud(int, int, int);

    /**
     * @brief Save current configuartion params to FLASH memory of the drive, so the nest time
     *        the drive is turned on it loads thas configuration.
     * WARNING!!! It should not be used more than 10,000 times because the FLASH memory could
     *            get damaged!!!
     *
     * @param RSd is the file descriptor of the serial port
     * @param semID is the semaphore identificator
     *
     * @return ERR_COM if an error communicating with driver occurs
     * @return ERR_NOERR if the save is done correctly
     *
     * The EEPSAV command always responds with the character string “EEPROM writing done”
     * after successful saving of the current settings in the data Flash memory,
     * or with “Flash defect”, if the save has failed.
     */
    int saveToFlash(int, int);

    /* get configuration from the driver */

    /**
     * @brief returns the maximum position allowed by the driver. To set this value see
     * @see SetDriverMaxPos
     *
     * @param RSd is the file descriptor of the serial port connected to the driver
     * @param semID is the semaphore identificator
     *
     * @return ERR_NOERR if there aren't any problems
     * @return ERR_COM if could not communicate with the driver (either write or read)
     * \todo change the way I save the parameter. It has to be saved by parameter by reference.
     * Return only must return Error control
     *
     */
    long int getDriverMaxPos(int RSd, int semID);

    /**
     * @brief This function reads from the driver min position from its flash memory
     *
     * @param RSd RS232 file descriptor
     * @param semID semaphore ID
     *
     * @return returns the value of the min position in pulses
     * @return ERR_COM if could not communicate with the driver (either write or read)
     * <b>Warning</b>, it is possible to recevie a min pos which the same as ERR_COM. If this happens the function will wrongly understand that an error has occured
     * * \todo change the way I save the parameter. It has to be saved by parameter by reference. Return only must return Error control
     */
    long int getDriverMinPos(int RSd, int semID);

    /**
     * @brief Returns the maximum speed allowed by the driver which is set with setDriverMaxVel()
     *        function
     *
     * @param RSd is the file descriptor of the serial port connected to the driver
     * @param semID is the semaphore identificator
     *
     * @return maxVel configurated in the drive in r.p.m.
     * @return ERR_COM if could not communicate with the driver (either write or read)
     * \todo change the way I save the parameter. It has to be saved by parameter by reference.
     *  Return only must return Error control
     */
    long int getDriverMaxVel(int RSd, int semID);

    /**
     * @brief returns the maximum acceleration allowed by the driver. To set this value see @see SetDriverMaxAcc
     *
     * @param RSd is the file descriptor of the serial port connected to the driver
     * @param semID is the semaphore identificator
     *
     * @return maxAcc Returns the maximum acceleration revolutions/sec²
     * @return ERR_COM if could not communicate with the driver (either write or read)
     * \todo change the way I save the parameter. It has to be saved by parameter by reference.
     * Return only must return Error control
     */
    long int getDriverMaxAcc(int RSd, int semID);

    /**
     * @brief returns the maximum decceleration allowed by the driver. To set this value see
     * @see SetDriverMaxDec
     *
     * @param RSd is the file descriptor of the serial port connected to the driver
     * @param semID is the semaphore identificator
     *
     * @return maxDec Returns the maximum decceleration revolutions/sec²
     * @return ERR_COM if could not communicate with the driver (either write or read)
     * \todo change the way I save the parameter. It has to be saved by parameter by reference.
     * Return only must return Error control
     *
     */
    long int getDriverMaxDec(int RSd, int semID);

    /**
     * @brief get the continous current limit (CCL) in mA.
     *
     * @param RSd RS232 file descriptor
     * @param semID semaphore ID
     *
     * @return returns the value of the continous current limit (CCL) in mA.
     * @return ERR_COM if could not communicate with the driver (either write or read)
     *
     * WARNING: Untested Function @TODO
     * \todo change the way I save the parameter. It has to be saved by parameter by reference.
     * Return only must return Error control
     */
    int getDriverCurLim(int RSd, int semID);

    /**
     * @brief get the Peak Current Limit (PCL) in mA.
     *
     * @param RSd RS232 file descriptor
     * @param semID semaphore ID
     *
     * @return returns the value of the peak current limit (PCL) in mA.
     * @return ERR_COM if could not communicate with the driver (either write or read)
     *
     * WARNING: Untested Function @TODO
     * \todo change the way I save the parameter. It has to be saved by parameter by reference.
     * Return only must return Error control
     */
    int getDriverPCurLim(int RSd, int semID);

    /* get status from the driver */

    /**
     * @brief Asks to the driver its current status
     *
     * @param RSd is the file descriptor of the serial port connected to the driver
     * @param semID is the semaphore identificator
     * @param drvStatus is a structure where the current driver status is stored. Each of the parameters of the drvStatus are set to 1 if true or 0 if false
     *
     * @return ERR_NOERR if the status could be returned
     * @return ERR_COM if it is not possible to check the driver status
     */
    int getDriverStatus(int RSd, int semID, driverStatus_t * drvStatus);

    /* get motorSensor from the driver */

    /**
     * @brief asks for sensor data of the driver
     *
     * @param RSd is the file descriptor of the serial port connected to the driver
     * @param semID is the semaphore identificator
     * @param sensor is the sensor structure where we want to store the sensor data from the driver.
     *
     * @return ERR_NOERR if no error (operation is succesful)
     * @return ERR_COM if there is an error communicating whit the driver. In this case some (of all) of the sensor parameters could not be up to date.
     */
    int getDriverSensor(int, int, driverSensor_t *);

    /**
     * @brief asks for the instant position from sensor data of the driver.
     * @param RSd is the file descriptor of the serial port connected to the driver
     * @param semID is the semaphore identificator
     * @param position is a pointer pointing to where the position sensor data from the driver will be stored. The units from the driver are in [pulses]
     * @return ERR_NOERR if no error (operation is succesful)
     * @return ERR_COM if there is an error communicating whit the driver.
     */
    int getDriverInstantPos(int RSd, int semID, long int * positon);

    /**
     * @brief asks for the instant velocity from sensor data of the driver.
     * @param RSd is the file descriptor of the serial port connected to the driver
     * @param semID is the semaphore identificator
     * @param velocity is where the velocity sensor data from the driver will be stored. The units from the driver are [rpm]
     * @return ERR_NOERR if no error (operation is succesful)
     * @return ERR_COM if there is an error communicating whit the driver.
     */
    int getDriverInstantVel(int RSd, int semID, long int * velocity);

    /**
     * @brief asks for the instant velocity from sensor data of the driver.
     * @param RSd is the file descriptor of the serial port connected to the driver
     * @param semID is the semaphore identificator
     * @param current is where the instant current data from the driver will be stored.
     *        The units from the driver are [rpm]
     * @return ERR_NOERR if no error (operation is succesful)
     * @return ERR_COM if there is an error communicating whit the driver.
     */
    int getDriverInstantCurrent(int RSd, int semID, int * current);

    /* direct movement */

    /**
     * @brief Sends to the driver the order to move to an absolute position in pulses
     *
     * @param RSd is the file descriptor of the serial port connected to the driver
     * @param semID is the semaphore identificator
     * @param p absolute position in pulses
     *
     * @return ERR_NOERR if no error sending the command to the driver
     * @return ERR_WRI if it isn't possible to communicate with the driver
     */
    int moveDriverAbsPos(int RSd, int semID, long int p);

    /**
     * @brief Sends to the driver the order to move to a relative position in pulses
     *
     * @param RSd is the file descriptor of the serial port connected to the driver
     * @param semID is the semaphore identificator
     * @param p relative position in pulses
     *
     * @return ERR_NOERR if no error sending the command to the driver
     * @return ERR_WRI if it isn't possible to communicate with the driver
     */
    int moveDriverRelPos(int RSd, int semID, long int p);

    /**
     * @brief Sends to the driver the order to move at a determined speed (in rpm)
     *
     * @param RSd is the file descriptor of the serial port connected to the driver
     * @param semID is the semaphore identificator
     * @param v velocity in rpm (revolutions per minute)
     *
     * @return ERR_NOERR if no error sending the command to the driver
     * @return ERR_WRI if it isn't possible to communicate with the driver
     */
    int moveDriverVel(int RSd, int semID, long int v);

    /* enable/disable driver */

    /**
     * @brief Enables the driver
     * @param RSd the RS232 File descriptor
     * @param semID Semaphore Identifier
     * @return ERR_NOERR if everything works correctly
     * @return ERR_WRI in case of error when writing in the driver descriptor file
     */
    int enableDriver(int RSd, int);

    /**
     * @brief Disables the driver
     * @param RSd the RS232 File descriptor
     * @param semID Semaphore Identifier
     * @return ERR_NOERR if everything works correctly
     * @return ERR_WRI in case of error when writing in the driver descriptor file
     */
    int disableDriver(int RSd, int);

    /* Calibrate Driver */

    /**
     * @brief sets the current positon as the given value. NOTE: Use only during the calibration phase.
     *
     * This function sends to the driver the Home command. The home commands tells to the driver which
     * is the current position. For example If we send home = 0
     * we are telling to the driver that the current position is 0 position (and it should used as a
     * reference).
     *
     * @param RSd is the file descriptor of the serial port connected to the driver
     * @param semID is the semaphore identificator
     * @param home is the given position in pulses. This parameter specifies which is the current position.
     *
     * @return ERR_NOERR if everything works correctly
     * @return ERR_WRI in case of error when writing in the driver descriptor file
     */
    int setDriverHomePosition(int RSd, int semID, long int home);

    /**
     * @brief starts the calibration sequence of the drive
     *
     * @param RSd is the file descriptor of the serial port connected to the driver
     * @param semID is the semaphore identificator
     * @param limit sensor position in pulses measured from the desired 0 position
     * @param currentLimit max current the motor is not allowed to exceed (in mA) This limit is
     *        directly related to torque of the motor.
     * @param calibrationSpeed The speed of the motor during the calibration
     * @param timeOut maximum time (in milli seconds) to perform the calibration. If this time is
     *        exceeded the function will return an error.
     *
     * @return ERR_NOERR If the calibration is done successfully
     * @return ERR_CURLIM if the current limit is reached during the calibration. I.E. an obstacle
     *         is blocking the motor.
     * @return ERR_TIMEOUT if the calibration process lasts more than the timeOut value
     * @return ERR_NOHOME if it is not possible to establish the home position
     */
    int calibrateDriver(int RSd, int semID, long int limit, int currentLimit, int calibrationSpeed, int timeOut);

#ifdef __cplusplus
}
#endif

#endif

