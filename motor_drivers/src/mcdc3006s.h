#ifndef __MCD3006S_H__
#define __MCD3006S_H__

/**
 * @file        mcdc3006s.h
 * @brief       Software implementation of the Application Program Interface for the drive model faulhaberMCDC3006S.
 *
 * @author      Raul Perula-Martinez <raul.perula@uc3m.es>
 * @date        2015-03
 * @author      Victor Gonzalez <vgonzale@ing.uc3m.es>
 * @date        2010-01
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

#include "motor_driver_interface.h"
#include "communications/serial_port/rs232.h"

class Mcdc3006s : public MotorDriverInterface {
    public:
        /**
         * @brief Empty constructor.
         */
        Mcdc3006s();

        /**
         * @brief Destructor.
         */
        ~Mcdc3006s();

        /**
         * @brief high level functions
         */
        int init(int baudrate, char *dev, char *sem);

        /**
         * @brief Enables the driver
         * @return ERR_NOERR if everything works correctly
         * @return ERR_WRI in case of error when writing in the driver descriptor file
         */
        int enableDriver();

        /**
         * @brief Disables the driver
         * @return ERR_NOERR if everything works correctly
         * @return ERR_WRI in case of error when writing in the driver descriptor file
         */
        int disableDriver();

        /**
         * @brief Function that gets the current driver configuration as is stored in the
         *        flash memory of the driver and writes it in a driverConf_t type format
         *
         * @param dc is the driverConf_t type where the driver configuration is stored
         *
         * @return ERR_NOERR if there are no problems
         * @return ERR_COM if it is not possible to read (any parameter) information from
         *         the driver. When this happens at least one of the parameters of the driverConf_t
         *         structure could be incomplete.
         */
        int getDriverConf(driverConf_t *dc);

        /**
         * @brief Function that configures the driver from a driverConf_t type
         * @param dc is where dc configuration is read in order to write it into the driver
         * @return ERR_NOERR if success
         * @return ERR_CONF if any error occurs
         */
        int setDriverConf(driverConf_t dc);

        /**
         * @brief Save current configuartion params to FLASH memory of the drive, so the nest time
         *        the drive is turned on it loads thas configuration.
         * WARNING!!! It should not be used more than 10,000 times because the FLASH memory could
         *            get damaged!!!
         *
         * @return ERR_COM if an error communicating with driver occurs
         * @return ERR_NOERR if the save is done correctly
         *
         * The EEPSAV command always responds with the character string “EEPROM writing done”
         * after successful saving of the current settings in the data Flash memory,
         * or with “Flash defect”, if the save has failed.
         */
        int saveToFlash();

        /* set configuration to the driver */

        /**
         * @brief Activates/Deactivates the driver Limits. If the limits aren't activated the driver
         *        will NOT respect the Maximum or Minimum positions.
         * @param action it only has two allowed values: ACTIVATE to activate de driver or DEACTIVATE
         *        to disºable de driver
         *
         * @return ERR_NOERR if everything works correctly
         * @return ERR_WRI in case of error when writing in the driver descriptor file
         * @return ERR_OUTOFRANGE if action is not in allowed values set
         */
        int activateLimits(int action);

        /* get configuration from the driver */

        /**
         * @brief returns the maximum position allowed by the driver. To set this value see
         * @see SetDriverMaxPos
         *
         * @return ERR_NOERR if there aren't any problems
         * @return ERR_COM if could not communicate with the driver (either write or read)
         * \todo change the way I save the parameter. It has to be saved by parameter by reference.
         * Return only must return Error control
         *
         */
        long int getDriverMaxPos();

        /**
         * @brief This function reads from the driver min position from its flash memory
         *
         * @return returns the value of the min position in pulses
         * @return ERR_COM if could not communicate with the driver (either write or read)
         * <b>Warning</b>, it is possible to recevie a min pos which the same as ERR_COM. If this happens the function will wrongly understand that an error has occured
         * * \todo change the way I save the parameter. It has to be saved by parameter by reference. Return only must return Error control
         */
        long int getDriverMinPos();

        /**
         * @brief Returns the maximum speed allowed by the driver which is set with setDriverMaxVel()
         *        function
         *
         * @return maxVel configurated in the drive in r.p.m.
         * @return ERR_COM if could not communicate with the driver (either write or read)
         * \todo change the way I save the parameter. It has to be saved by parameter by reference.
         *  Return only must return Error control
         */
        long int getDriverMaxVel();

        /**
         * @brief returns the maximum acceleration allowed by the driver. To set this value see @see SetDriverMaxAcc
         *
         * @return maxAcc Returns the maximum acceleration revolutions/sec²
         * @return ERR_COM if could not communicate with the driver (either write or read)
         * \todo change the way I save the parameter. It has to be saved by parameter by reference.
         * Return only must return Error control
         */
        long int getDriverMaxAcc();

        /**
         * @brief returns the maximum decceleration allowed by the driver. To set this value see
         * @see SetDriverMaxDec
         *
         * @return maxDec Returns the maximum decceleration revolutions/sec²
         * @return ERR_COM if could not communicate with the driver (either write or read)
         * \todo change the way I save the parameter. It has to be saved by parameter by reference.
         * Return only must return Error control
         *
         */
        long int getDriverMaxDec();

        /**
         * @brief get the continous current limit (CCL) in mA.
         *
         * @return returns the value of the continous current limit (CCL) in mA.
         * @return ERR_COM if could not communicate with the driver (either write or read)
         *
         * WARNING: Untested Function @TODO
         * \todo change the way I save the parameter. It has to be saved by parameter by reference.
         * Return only must return Error control
         */
        int getDriverCurLim();

        /**
         * @brief get the Peak Current Limit (PCL) in mA.
         *
         * @return returns the value of the peak current limit (PCL) in mA.
         * @return ERR_COM if could not communicate with the driver (either write or read)
         *
         * WARNING: Untested Function @TODO
         * \todo change the way I save the parameter. It has to be saved by parameter by reference.
         * Return only must return Error control
         */
        int getDriverPCurLim();

        /* get status from the driver */

        /**
         * @brief Asks to the driver its current status
         * @param drvStatus is a structure where the current driver status is stored. Each of the parameters of the drvStatus are set to 1 if true or 0 if false
         * @return ERR_NOERR if the status could be returned
         * @return ERR_COM if it is not possible to check the driver status
         */
        int getDriverStatus(driverStatus_t * drvStatus);

        /* get motorSensor from the driver */

        /**
         * @brief asks for sensor data of the driver
         * @param sensor is the sensor structure where we want to store the sensor data from the driver.
         * @return ERR_NOERR if no error (operation is succesful)
         * @return ERR_COM if there is an error communicating whit the driver. In this case some (of all) of the sensor parameters could not be up to date.
         */
        int getDriverSensor(driverSensor_t *sensor);

        /**
         * @brief asks for the instant position from sensor data of the driver.
         * @param position is a pointer pointing to where the position sensor data from the driver will be stored. The units from the driver are in [pulses]
         * @return ERR_NOERR if no error (operation is succesful)
         * @return ERR_COM if there is an error communicating whit the driver.
         */
        int getDriverInstantPos(long int *positon);

        /**
         * @brief asks for the instant velocity from sensor data of the driver.
         * @param velocity is where the velocity sensor data from the driver will be stored. The units from the driver are [rpm]
         * @return ERR_NOERR if no error (operation is succesful)
         * @return ERR_COM if there is an error communicating whit the driver.
         */
        int getDriverInstantVel(long int *velocity);

        /**
         * @brief asks for the instant velocity from sensor data of the driver.
         * @param current is where the instant current data from the driver will be stored.
         *        The units from the driver are [rpm]
         * @return ERR_NOERR if no error (operation is succesful)
         * @return ERR_COM if there is an error communicating whit the driver.
         */
        int getDriverInstantCurrent(int *current);

        /**
         * @brief Sets the maximum absolute position in pulses
         * @param maxPos in pulses
         * @return ERR_NOERR if everything has gone correctly
         * @return ERR_OUTOFRANGE if maxPos is below or equal 0 (maxPos must be higher than 0).
         * @return ERR_WRI if could not write to the driver
         * @return ERR_POSLIMIT if it is not possible to activate the limits
         */
        int setDriverMaxPos(long int maxPos);

        /**
         * @brief Sets the minimum absolute position in pulses
         * @param minPos is the minimum absolute position in pulses
         * @return ERR_NOERR if everything has gone correctly
         * @return ERR_OUTOFRANGE if minPos is higher or equal than 0 (minPos must be lower than 0).
         * @return ERR_WRI if could not write to the driver
         * @return ERR_POSLIMIT if it is not possible to activate the limits
         */
        int setDriverMinPos(long int minPos);

        /**
         * @brief Sets the maximum speed allowed by the driver in rpm
         * @param maxVel in r.p.m.
         * @return ERR_NOERR if everything goes correct
         * @return ERR_WRI if could not write to the driver
         */
        int setDriverMaxVel(long int maxVel);

        /**
         * @brief sets the maximum acceleration allowed by the driver in revolutions/s^2
         * @param maxAcc maximum acceleration in revolutions/s^2
         * @return ERR_NOERR if everything goes correct
         * @return ERR_WRI if could not write to the driver
         */
        int setDriverMaxAcc(long int maxAcc);

        /**
         * @brief sets the maximum deceleration allowed by the driver in revolutions/s^2
         * @param maxDec maximum deceleration in r/sec²
         * @return ERR_NOERR if everything goes correct
         * @return ERR_WRI if could not write to the driver
         *
         */
        int setDriverMaxDec(long int maxDec);

        /**
         * @brief loads continous current limit
         * @param cl current limit in mA Range = [0 - 12000mA]
         * @return ERR_NOERR if everything goes correct
         * @return ERR_WRI if could not write to the driver
         */
        int setDriverCurLim(int cl);

        /**
         * @brief loads peak current limit
         * @param pcl current limit in mA Range = [0 - 12000mA]
         * @return ERR_NOERR if everything goes correct
         * @return ERR_WRI if could not write to the driver
         */
        int setDriverPCurLim(int pcl);

        /**
         * @brief tells to the driver at which baudrate we want to operate
         * @param baud is the baudrate of the communication.
         * @return ERR_NOERR if the configuration is succesful
         * @return ERR_OUTOFRANGE if the parameter baud is not in the expected values
         * @return ERR_WRI if it is not possible to write to the driver the configuration
         */
        int setDriverBaud(int baud);

        /* direct movement */

        /**
         * @brief Sends to the driver the order to move to an absolute position in pulses
         * @param p absolute position in pulses
         * @return ERR_NOERR if no error sending the command to the driver
         * @return ERR_WRI if it isn't possible to communicate with the driver
         */
        int moveDriverAbsPos(long int pos);

        /**
         * @brief Sends to the driver the order to move to a relative position in pulses
         * @param p relative position in pulses
         * @return ERR_NOERR if no error sending the command to the driver
         * @return ERR_WRI if it isn't possible to communicate with the driver
         */
        int moveDriverRelPos(long int pos);

        /**
         * @brief Sends to the driver the order to move at a determined speed (in rpm)
         * @param v velocity in rpm (revolutions per minute)
         * @return ERR_NOERR if no error sending the command to the driver
         * @return ERR_WRI if it isn't possible to communicate with the driver
         */
        int moveDriverVel(long int vel);

        /* Calibrate Driver */

        /**
         * @brief Sets the current positon as the given value. NOTE: Use only during the calibration phase.
         *        This function sends to the driver the Home command. The home commands tells to the driver which
         *        is the current position. For example If we send home = 0
         *        we are telling to the driver that the current position is 0 position (and it should used as a
         *        reference).
         * @param home is the given position in pulses. This parameter specifies which is the current position.
         * @return ERR_NOERR if everything works correctly
         * @return ERR_WRI in case of error when writing in the driver descriptor file
         */
        int setDriverHomePosition(long int home);

        /**
         * @brief starts the calibration sequence of the drive
         * @param limit sensor position in pulses measured from the desired 0 position
         * @param currentLimit max current the motor is not allowed to exceed (in mA) This limit is
         *        directly related to torque of the motor.
         * @param calibrationSpeed The speed of the motor during the calibration
         * @param timeOut maximum time (in milli seconds) to perform the calibration. If this time is
         *        exceeded the function will return an error.
         * @return ERR_NOERR If the calibration is done successfully
         * @return ERR_CURLIM if the current limit is reached during the calibration. I.E. an obstacle
         *         is blocking the motor.
         * @return ERR_TIMEOUT if the calibration process lasts more than the timeOut value
         * @return ERR_NOHOME if it is not possible to establish the home position
         */
        int calibrateDriver(long int limit, int current_limit, int calibration_speed, int time_out);

    private:
        Rs232 _comm;
};

#endif
