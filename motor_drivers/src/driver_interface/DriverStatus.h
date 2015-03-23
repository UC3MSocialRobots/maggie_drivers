#ifndef DRIVERSTATUS_H_
#define DRIVERSTATUS_H_

/**
 * \file DriverStatus.h
 *
 * \date 12/05/2010
 * \author: Victor Gonzalez Pacheco (mailto: vgonzale@ing.uc3m.es)
 */

class DriverStatus {
        friend class DriverInterface;

    public:
        /**
         * @brief Empty contructor.
         */
        DriverStatus();

        /**
         * @brief Destructor.
         */
        virtual ~DriverStatus();

        // Getters

        /**
         * @brief Checks if the current is being limitated.
         * @return currentLimitated. True: the current is being limited.
         */
        virtual bool isCurrentLimitated() const;

        /**
         * @brief Checks if the driver is enabled or disabled. True = driver enabled. False = driver disabled.
         * @return enabled. True if it is enabled.
         */
        virtual bool isEnabled() const;

        /**
         * @brief Checks if the limit sensoris has been reached. True the sensor has been reached.
         * @return limitSensorReached.. True: the limit sensor is producing an output. Thus the limit sensor has been
         *         reached.
         */
        virtual bool isLimitSensorReached() const;

        /**
         * @brief Checks if the overTemperatureWarning has been triggered (there is an over Temperature error in the
         *        driver). True = overTemperatureWarning.
         * @return overTemperatureWarning. True: warning.
         */
        virtual bool isOverTemperatureWarning() const;

        /**
         * @brief Checks if the overTemperatureWarning has been triggered (there is an overvoltage error in the driver).
         *        True = overVoltageWarning.
         * @return overVoltageWarning. True: warning.
         */
        virtual bool isOverVoltageWarning() const;

        /**
         * @brief checks if the the calibration flag is calibrated or not.
         * @return calibrate The calibration flag. true = the driver is calibrated. false = the driver is not calibrated
         */
        virtual bool isCalibrated() const;

        // Setters

        /**
         * @brief Sets the DriverStatus::currentLimitated to the value entered in the parameter
         * @param currentLimitated. The value to set DriverStatus::currentLimitated
         */
        virtual void isCurrentLimitated(bool curLim);

        /**
         * @brief Sets the DriverStatus::enabled to the value entered in the parameter enabled. True = device is
         *        enabled. False = device is disabled.
         * @param enabled. The value to set DriverStatus::enabled
         */
        virtual void isEnabled(bool enabled);

        /**
         * @brief Sets the DriverStatus::limitSensorReached to true or false depending if the Limit sensor has been
         *        reached (true) or not (false)
         * @param limitSensorReached. The value to set DriverStatus::limitSensorReached
         */
        virtual void isLimitSensorReached(bool limitSensorReached);

        /**
         * @brief Sets the DriverStatus::overTemperatureWarning to true or false depending on the status provided by
         *        the driver.
         * @param overTemperatureWarning. The value to set DriverStatus::overTemperatureWarning
         */
        virtual void isOverTemperatureWarning(bool overTemperatureWarning);

        /**
         * @brief Sets the DriverStatus::overVoltageWarning to the value entered in the parameter
         * @param overVoltageWarning. The value to set DriverStatus::overVoltageWarning
         */
        virtual void isOverVoltageWarning(bool overVoltageWarning);

        /**
         * @brief sets the DriverStatus::calibrated calibration flag to the value of the entered parameter
         * @param calibrated The value to set the calibration flag. true = the driver is calibrated. false = the driver
         *        is not calibrated
         */
        virtual void isCalibrated(bool calibrated);

        /**
         * @brief checks the status of the boolean DriverStatus::open When the driver is open this boolean is set to
         *        true. Otherwise is false
         * @return DriverStatus::open the boolean that keeps the status of the driver
         * @note open means that there is communication between the computer and the driver
         */
        virtual bool isConnected() const;

        /**
         * @brief Sets the DriverStatus::open flag to the entered parameter (true or false).
         * @param connected the boolean variable to specify that the driver is open or closed. True = open.
         *        False = closed
         */
        virtual void isConnected(bool open);

    protected:
        bool connected;
        bool enabled;
        bool calibrated;
        bool overTemperatureWarning;
        bool overVoltageWarning;
        bool currentLimitated;
        bool limitSensorReached;
};

#endif
