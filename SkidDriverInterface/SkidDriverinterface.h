/*
 * ISkidDriver.h
 *
 *  Created on: Jul 4, 2014
 *      Author: jan
 */

#ifndef ISKIDDRIVER_H_
#define ISKIDDRIVER_H_
#include "DataTypes.h"

//typedef enum
//{
//	WHEELS_AT_SPEED = 0, WHEELS_DIRTY, WHEELS_OVERLOADED, WHEELS_NUM_STATES
//} WHEELS_STATE;

class SkidDriverInterface
{
	protected:
		SPEED_TYPE myRequestedSpeed;
		DIRECTION_TYPE myRequestedDirection;
		SPEED_TYPE myNewRequestedSpeed;
		DIRECTION_TYPE myNewRequestedDirection;
//		WHEELS_STATE myWheelsState;


	public:

		virtual void motorOn()=0;
		virtual void motorOff()=0;
		virtual void setup()=0;
		virtual void loop()=0;
		void setSpeedAndDirection(SPEED_TYPE Speed, DIRECTION_TYPE Direction)
		{
			myNewRequestedSpeed=Speed;
			myNewRequestedDirection=Direction;
		};

		virtual bool emergencyBreak(bool immediatlyReturn = false)=0;
		virtual void turnOnTheSpot(SPEED_TYPE Speed)=0;

//		WHEELS_STATE getState() const {return myWheelsState;};
		virtual const __FlashStringHelper * getOverloadedWheelName(){	return F("no");};

		virtual SPEED_TYPE getActualSpeed() const=0;

		SPEED_TYPE getRequestedSpeed() const
		{
			return myNewRequestedSpeed;
		}
		DIRECTION_TYPE getRequestedDirection() const
		{
			return myNewRequestedDirection;
		}


};

#endif /* ISKIDDRIVER_H_ */
