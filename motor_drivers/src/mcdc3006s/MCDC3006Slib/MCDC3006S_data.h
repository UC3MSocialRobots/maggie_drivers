#ifndef MCD3006S_DATA_H
#define MCD3006S_DATA_H

/**
 * @file MCD3006S_data.h
 * @author Raúl Pérula (mailto:rperula@ing.uc3m.es)
 * @date 2014-10
 */

/**
 * \todo This two defines must be moved to a primitive level
 */

//#define MCDC3006S_PULSES_PER_REV 2048         /// Warning
//#define MCDC3006S_REDUCTION_FACTOR 123000 /* motorValue = FACTOR * dofValue */
#define CALIBRATION_TIMEOUT 5000            /**< Calibration timeout in milliseconds */

#define ACTIVATE 1
#define DEACTIVATE 0

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

// Driver Masks used to extract information from GST and GFS (Get Status and Get Fault Status) commands
#define ENABLED_MASK            8           /**< Mask used to check if the driver is enabled or not. Masks the bit 3 of the GST command response */
#define CURRENT_LIMITING_MASK   16          /**< Mask used to check if the current limiting is active. Masks the bit 4 of the OST command response */
#define OVERVOLTAGE_MASK        64          /**< Mask used to check if the overvoltage bit is active. Masks the bit 6 of the OST command response */
#define OVERTEMPERATURE_MASK    128         /**< Mask used to check if the overtemperature is active. Masks the bit 7 of the OST command response*/
#define DRIVER_INPUT_4_MASK     2048        /**< Mask used to check if the input 4 of the driver is active. I have this input connected to the limit sensor.
                                                                                                        Masks the bit 11 of the OST command response */

/**
 * @brief Structure where the configuration of the driver is stored
 *
 */
typedef struct driverConf {
        int RSd; /**< Serial Port File descriptor */
        semaphore_t sem; /**< Semaphore related data */

        long int maxPos; /**< Driver maximum position (in pulses) */
        long int minPos; /**< Driver minimum position (in pulses) */
        long int maxVel; /**< Driver maximum velocity (in r.p.m) */
        long int maxAcc; /**< Driver maximum acceleration (in revolutions/sec^2) */
        long int maxDec; /**< Driver maximum deceleration (in revolutions/sec^2) */
        long int cCLimit; /**< Driver Continuous Current Limit (LLC) in mA. */
} driverConf_t;

/**
 * @brief Structure that contains the information of the sensors of the driver (Position, Velocity and Current)
 */
typedef struct driverMotorSensor {
        long int p; /**< current position in pulses */
        long int v; /**< current velocity in rpm */
        long int i; /**< instant current in mA */
} driverSensor_t;

/**
 * @brief Structure where the status of the driver is stored
 *
 */
typedef struct driverStatus {
        int disabled; /**< Stores if the driver is disabled or not TRUE = enabled; FALSE = disabled */
        int overTemperature; /**< Stores if there is an overTemperature error or not TRUE = error overtemperature; FALSE = Temp is OK */
        int curLimiting; /**< Stores if limit current is active or not TRUE = current limit is active; FALSE = actualCurrent < currentLimt */
        int overVoltage; /**< Stores the status of overVoltage: TRUE = error overvoltage; FALSE = no error, voltage is lower than the minimum*/
        int sensorReached; /**< Stores if the limit sensor is reached or not. I've connected this sensor to the input 4 of the driver. */
} driverStatus_t;

#endif

