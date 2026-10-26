/*
 * SkidDriversaberTooth.h
 *
 *  Created on: 11-mei-2016
 *      Author: jan
 */
#pragma once
#include "SkidDriverinterface.h"
#include "MotorModBusSiemensV20.h"
#include "FieldInfo.h"
#ifdef I_USE_SERIAL_REGISTER
#include "SerialDataInterface.h"
#endif

class SkidDriverModBusSiemensV20: public SkidDriverInterface {
	uint8_t myIsMotorOn;
	SPEED_TYPE mySabertoothspeed;
	DIRECTION_TYPE mySabertoothDirection;
public:
	SkidDriverModBusSiemensV20();
	virtual ~SkidDriverModBusSiemensV20();
	virtual void motorOn();
	virtual void motorOff();
	virtual void setup();
	virtual void loop();

	virtual bool emergencyBreak(bool immediatlyReturn = false);
	virtual void turnOnTheSpot(SPEED_TYPE Speed);

	virtual SPEED_TYPE getActualSpeed() const;
#ifdef I_USE_SERIAL_REGISTER
	void serialRegister(const __FlashStringHelper* Name);
#endif


};

