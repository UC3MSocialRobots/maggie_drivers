/**
 * \file DriverMotorSensor.h
 *
 * \date 11/05/2010
 * \author Victor Gonzalez Pacheco mailto: vgonzale@ing.uc3m.es
 */

#ifndef DRIVERMOTORSENSOR_H_
#define DRIVERMOTORSENSOR_H_

/*
 * Class that contains the motor's sensor information of an actuator.
 * It also provides the appropriate access functions (getters and setters).
 */
class DriverMotorSensor {
	friend class DriverInterface;
//	friend class actuator::Actuator;
public:
	DriverMotorSensor();
	virtual ~DriverMotorSensor();

	// Access Functions.
	virtual long int getPosition() const;
	virtual long int getVelocity() const;
	virtual int getInstantCurrent() const;

	void setInstantCurrent(int instantCurrent);
	void setPosition(long int position);
	void setVelocity(long int velocity);

protected:
	/** Current position of the motor. It's in pulses. **/
	long int position;
	/** Current velocity of the motor. It's in rpms **/
	long int velocity;
	/** Current Instantaneous current of the motor. It's in mA (milli Amperes). **/
	int instantCurrent;
};

#endif /* DRIVERMOTORSENSOR_H_ */
