/*
 * SkidDriversaberTooth.h
 *
 *  Created on: 11-mei-2016
 *      Author: jan
 */

#ifndef LIBRARIES_SKIDDRIVER_SABERTOOTH_SKIDDRIVERSABERTOOTH_H_
#define LIBRARIES_SKIDDRIVER_SABERTOOTH_SKIDDRIVERSABERTOOTH_H_
#include "SkidDriverinterface.h"
#include <Sabertooth.h>
#include "SerialDataInterface.h"
#include "FieldInfo.h"

class SkidDriverSaberTooth: public SkidDriverInterface {
	uint8_t myIsMotorOn;
	Sabertooth *myPSabertooth;
	SPEED_TYPE mySabertoothspeed;
	DIRECTION_TYPE mySabertoothDirection;
public:
	SkidDriverSaberTooth(Sabertooth* pSabertooth);
	virtual ~SkidDriverSaberTooth();
	virtual void motorOn();
	virtual void motorOff();
	virtual void setup();
	virtual void loop();

	virtual bool emergencyBreak(bool immediatlyReturn = false);
	virtual void turnOnTheSpot(SPEED_TYPE Speed);

	virtual SPEED_TYPE getActualSpeed() const;
	void serialRegister(const __FlashStringHelper* Name);

};

#endif /* LIBRARIES_SKIDDRIVER_SABERTOOTH_SKIDDRIVERSABERTOOTH_H_ */
