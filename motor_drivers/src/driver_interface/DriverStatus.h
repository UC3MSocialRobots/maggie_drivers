/**
 * \file DriverStatus.h
 *
 * \date 12/05/2010
 * \author: Victor Gonzalez Pacheco (mailto: vgonzale@ing.uc3m.es)
 */

#ifndef DRIVERSTATUS_H_
#define DRIVERSTATUS_H_

/*
 * Class that contains the data needed to monitor the status of a motor
 */
class DriverStatus {
	friend class DriverInterface;
//	friend class actuator::Actuator;
public:
	DriverStatus();
	virtual ~DriverStatus();


	// Getters
	virtual bool isCurrentLimitated() const;
	virtual bool isEnabled() const;
	virtual bool isLimitSensorReached() const;
	virtual bool isOverTemperatureWarning() const;
	virtual bool isOverVoltageWarning() const;
	virtual bool isCalibrated() const;
	virtual bool isConnected() const;

	 // Setters
	virtual void isCurrentLimitated(bool curLim);
	virtual void isEnabled(bool enabled);
	virtual void isLimitSensorReached(bool limitSensorReached);
	virtual void isOverTemperatureWarning(bool overTemperatureWarning);
	virtual void isOverVoltageWarning(bool overVoltageWarning);
	virtual void isCalibrated (bool calibrated);
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

#endif /* DRIVERSTATUS_H_ */
