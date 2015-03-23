#ifndef DRIVERMOTORSENSOR_H_
#define DRIVERMOTORSENSOR_H_

/**
 * \file DriverMotorSensor.h
 *
 * \date 11/05/2010
 * \author Victor Gonzalez Pacheco mailto: vgonzale@ing.uc3m.es
 */

class DriverMotorSensor {
        friend class DriverInterface;

    public:
        /**
         * @brief Empty constructor.
         */
        DriverMotorSensor();

        /**
         * @brief Destructor.
         */
        virtual ~DriverMotorSensor();

        // Access Functions

        /**
         * @brief returns the position
         * @return DriverMotorSensor::position the last stored motorSensor position. Units: pulses
         */
        virtual long int getPosition() const;

        /**
         * @brief returns the velocity
         * @return DriverMotorSensor::velocity the last stored motorSensor velocity. Units: rpm (revolutions per minute)
         */
        virtual long int getVelocity() const;

        /**
         * @brief returns the instant current
         * @return DriverMotorSensor::instantCurrent the last stored motorSensor instant current. Units: milli amperes (mA)
         */
        virtual int getInstantCurrent() const;

        /**
         * @brief sets the instant current in the motorSensor data
         * @param DriverMotorSensor::instantCurrent sets the instant current in the motorSensor data. Units: milli amperes (mA)
         */
        void setInstantCurrent(int instantCurrent);

        /**
         * @brief sets the position in the motorSensor data
         * @param DriverMotorSensor::position sets the position in the motorSensor data. Units: pulses
         */
        void setPosition(long int position);

        /**
         * @brief sets the velocity in the motorSensor data
         * @param DriverMotorSensor::velocity sets the velocity in the motorSensor data. Units: rpm (revolutions per minute)
         */
        void setVelocity(long int velocity);

    protected:
        /** Current position of the motor. It's in pulses. **/
        long int position;

        /** Current velocity of the motor. It's in rpms **/
        long int velocity;

        /** Current Instantaneous current of the motor. It's in mA (milli Amperes). **/
        int instantCurrent;
};

#endif
