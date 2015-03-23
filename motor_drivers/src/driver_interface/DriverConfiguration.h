#ifndef DRIVERCONFIGURATION_H_
#define DRIVERCONFIGURATION_H_

/**
 *  \file DriverConfiguration.h
 *
 *  \date 12/05/2010
 *  \author: Victor Gonzalez (vgonzale@ing.uc3m.es)
 */

#include <string>

#define DEFAULT_ENCODER_PULSES_PER_REV 2048
#define DEFAULT_DRIVER_MULTIPLICATION_FACTOR 4
#define DEFAULT_REDUCTION_FACTOR 1

using std::string;

class DriverConfiguration {
        friend class DriverInterface;

    public:
        DriverConfiguration(); /** Default constructor. **/
        DriverConfiguration(std::string configurationFile);
        virtual ~DriverConfiguration(); // Default destructor

        // Access Functions (getters and setters)
        long int getMaxPosition() const;
        long int getMinPosition() const;
        long int getMaxVelocity() const;
        long int getMaxAcceleration() const;
        long int getMaxDecceleration() const;
        int getContinousCurrentLimit() const;
        int getPeakCurrentLimit() const;
        int getCalibrationCurrentLimit() const;
        long int getCalibrationSensorPosition() const;
        long int getCalibrationSpeed() const;
        int getCalibrationTimeOut() const;
        string getConfigFile() const;
        int getDriverID() const;
        string getDriverName() const;
        string getDriverType() const;

        void setMaxPosition(long int maxPosition);
        void setMinPosition(long int minPosition);
        void setMaxVelocity(long int maxVelocity);
        void setMaxAcceleration(long int maxAcceleration);
        void setMaxDecceleration(long int maxDecceleration);
        void setContinousCurrentLimit(int continousCurrentLimit);
        void setPeakCurrentLimit(int peakCurrentLimit);
        void setCalibrationCurrentLimit(int calibrationCurrentLimit);
        void setCalibrationSensorPosition(long int calibrationSensorPosition);
        void setCalibrationSpeed(long int calibrationSpeed);
        void setCalibrationTimeOut(int calibrationTimeOut);
        void setConfigFile(std::string confFile);
        void setDriverID(int driverID);
        void setDriverName(std::string driverName);
        void setDriverType(std::string driverType);

        // Measure units and conversion data
        long int getEncoderPulsesPerRevolution() const;
        int getDriverMultiplicationFactor() const;
        int getReductionFactor() const;
        long int getPulsesPerRevolution() const;
        double getPulsesPerRad() const;
        void setEncoderPulsesPerRevolution(long int pulsesPerRevolution);
        void setDriverMultiplicationFactor(int driverMultiplicationFactor);
        void setReductionFactor(int reductionFactor);
        void setPulsesPerRevolution();
        void setPulsesPerRevolution(long int pulsesPerRevolution);
        void setPulsesToRad();

    protected:
        static const double pi = 3.14159265;

        // Limits:
        /** Maximum position of the motor. The units are pulses */
        long int maxPosition;

        /** Minimum position of the motor. The units are pulses*/
        long int minPosition;

        /** Maximum velocity of the motor. The units are rpm */
        long int maxVelocity;

        /** Maximum acceleration of the motor. The units are rad/s^2 */
        long int maxAcceleration;

        /** Maximum deceleration of the motor. The units are rad/s^2 */
        long int maxDecceleration;

        /** Maximum continuous current of the motor. The units are mA (milli Amperes) */
        int continousCurrentLimit;

        /** Maximum peak current of the motor. The units are mA (milli Amperes) */
        int peakCurrentLimit;

        /** File where the configuration is stored */
        string configFile;

        // Calibration parameters

        /** Position (in pulses) of the limit sensor used for calibrate each motor **/
        long int calibrationSensorPosition;

        /** The current limit in mA for the calibration process **/
        int calibrationCurrentLimit;

        /** The velocity at the motor should move durinng the calibration process (units: rpm) */
        long int calibrationSpeed;
        /** Time limit for the calibration process (units: ms) */
        int calibrationTimeOut;

        // Measure Units and conversion

        /** The number of pulses per revolution of the motor's encoder */
        long int encoderPulsesPerRevolution;

        /** The multiplication factor of the driver. This value is a multiplier of the encoderPulsesPerRevolution of the enconder */
        int driverMultiplicationFactor;

        /** The reduction factor of the motor in case it uses a gear **/
        int reductionFactor;

        /**
         * The number of pulses that form a single revolution of the motor.
         *	This value is the Used to calculate the pulsesPerRad factor and is the product of:
         *	encoderPulsesPerRevolution * driverMultiplicationFactor * reductionFactor
         */
        long int pulsesPerRevolution;

        // Driver Specific data

        /** The driver Name */
        string driverName;

        /** the driver type */
        string driverType;

        /** The driver unique Identificator */
        int driverID;
};

#endif
