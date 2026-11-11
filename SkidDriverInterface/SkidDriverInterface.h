/*
 * SkidDriverInterface.h
 *
 *  Created on: Jul 4, 2014
 *      Author: jan
 */

#pragma once
#include "DataTypes.h"

class SkidDriverInterface {
protected:
	SPEED_TYPE myRequestedSpeed = 0;
	DIRECTION_TYPE myRequestedDirection = 0;
	SPEED_TYPE myNewRequestedSpeed = 0;
	DIRECTION_TYPE myNewRequestedDirection = 0;

public:

	virtual void motorOn()=0;
	virtual void motorOff()=0;
	virtual void setup()=0;
	virtual void loop()=0;
	void setSpeedAndDirection(SPEED_TYPE Speed, DIRECTION_TYPE Direction) {
		myNewRequestedSpeed = Speed;
		myNewRequestedDirection = Direction;
	}

	virtual bool emergencyBreak(bool immediatlyReturn = false)=0;
	virtual void turnOnTheSpot(SPEED_TYPE Speed)=0;

	virtual const __FlashStringHelper* getOverloadedWheelName() {
		return F("no");
	}
	;

	virtual SPEED_TYPE getActualSpeed() const =0;

	SPEED_TYPE getRequestedSpeed() const {
		return myNewRequestedSpeed;
	}
	DIRECTION_TYPE getRequestedDirection() const {
		return myNewRequestedDirection;
	}

	virtual ~SkidDriverInterface() {
	}
	;
};

