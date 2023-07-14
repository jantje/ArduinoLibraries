/*
 * BLDC1SkidDriver.cpp
 *
 *  Created on: May 9, 2014
 *      Author: jan
 */

#include "SkidDriverBLDC.h"
#include "JanHelpers.h"
#ifdef I_USE_PLOT
#include "simplot.h"
#endif

void SkidDriverBLDC::motorOn()
{
	myLeftMotor.motorOn();
	myRightMotor.motorOn();
}

void SkidDriverBLDC::motorOff()
{
	myLeftMotor.motorOff();
	myRightMotor.motorOff();
}

SkidDriverBLDC::SkidDriverBLDC(uint8_t LeftForwardBackwardPin, uint8_t RightForwardBackwardPin, uint8_t LeftPWMPin, uint8_t RightPWMPin)
		: myLeftMotor(LeftForwardBackwardPin, LeftPWMPin), myRightMotor(RightForwardBackwardPin, RightPWMPin)
{
	 myRequestedSpeed=0;
	 myRequestedDirection=0;


}

void SkidDriverBLDC::serialRegister(const __FlashStringHelper* Name, const __FlashStringHelper* LeftDriverName, const __FlashStringHelper* RightDriverName)
{

	FieldData::set(Name, F("RequestedSpeed"),MOD_WRITE , &myRequestedSpeed);
	FieldData::setNext( F("RequestedDirection"), MOD_WRITE, &myRequestedDirection);

		myLeftMotor.serialRegister(LeftDriverName);
		myRightMotor.serialRegister(RightDriverName);

	}

void SkidDriverBLDC::setup()
{
	myLeftMotor.setup();
	myRightMotor.setup();
}

void SkidDriverBLDC::loop()
{
	static SPEED_TYPE myPreviousSpeed=0;
	static DIRECTION_TYPE mPreviousDirection=0;
	if ((myPreviousSpeed != myRequestedSpeed) || (mPreviousDirection != myRequestedDirection))
	{
		SPEED_TYPE myRightRequestedSpeed=0;
		SPEED_TYPE myLeftRequestedSpeed=0;
		mPreviousDirection=myRequestedDirection;
		myPreviousSpeed=myRequestedSpeed;
		if ((myRequestedSpeed == 0) && (myRequestedDirection == 0))
		{
			myRightRequestedSpeed=0;
			myLeftRequestedSpeed=0;

		} else
		{
			myRightRequestedSpeed=constrain( myRequestedSpeed -(myRequestedDirection ),-255,255);
			myLeftRequestedSpeed=-1*constrain( myRequestedSpeed + (myRequestedDirection ),-255,255);
		}
		myRightMotor.setRequestedSpeed(myRightRequestedSpeed);
		myLeftMotor.setRequestedSpeed(myLeftRequestedSpeed);
	}
	myLeftMotor.loop();
	myRightMotor.loop();
}

void SkidDriverBLDC::setSpeedAndDirection(SPEED_TYPE Speed, DIRECTION_TYPE Direction)
{
	myRequestedSpeed = Speed;
	myRequestedDirection = Direction;
}

bool SkidDriverBLDC::emergencyBreak(bool immediatlyReturn)
{
	while (!(myLeftMotor.emergencyBreak(true) && myRightMotor.emergencyBreak(true)))
	{
		if (immediatlyReturn) return false;
	};
	myRequestedDirection = 0;
	myRequestedSpeed = 0;
	return true;
}

void SkidDriverBLDC::turn(DIRECTION_TYPE Corner)
{
	myRequestedSpeed = 200;
	myRequestedDirection = 100; //impossible value so any set speed will change the speed.
	SPEED_TYPE LeftTurnSpeed = myRequestedSpeed * SIGN(Corner );
	SPEED_TYPE RightTurnSpeed = -1 * LeftTurnSpeed;
//	SPEED_TYPE LeftTurnSpeed = max(-200 +(int)Corner,-255);
//	SPEED_TYPE RightTurnSpeed =max(-200 -(int)Corner,-255);
	myRightMotor.setRequestedSpeed(RightTurnSpeed);
	myLeftMotor.setRequestedSpeed(LeftTurnSpeed);
}


void SkidDriverBLDC::plot()
{
#ifdef I_USE_PLOT
	plot4(Serial,  myRequestedSpeed,   myRequestedDirection,myRightMotor.getRequestedSpeed(),  myLeftMotor.getRequestedSpeed()) ;

#endif
}

