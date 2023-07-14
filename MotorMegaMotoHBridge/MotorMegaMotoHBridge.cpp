/*
 * MegaMotoHBridgeDriver.cpp
 *
 *  Created on: May 19, 2012
 *      Author: BE04258
 */


#include "MotorMegaMotoHBridge.h"
#define MINIMUMPWM 120
#define MINIMUMSPEEDTORUN 10  //used to be 10

MotorMegaMotoHBridge::MotorMegaMotoHBridge( ENABLEPINPARAM uint8_t PWMAPin, uint8_t PWMBPin, uint8_t CurrentPinA, uint8_t CurrentPinB)
		: myMegaMotoDriver( ENABLEPINATTRIBUTE PWMAPin, PWMBPin, CurrentPinA, CurrentPinB)
{
	myCurrentRequestedSpeed = 0;
	myIsDirty = true;  /*Set this to true so we know for sure everything is properly initialised */
	myLoopDuration=0;

}
#define DETAILED_SERIAL_LOGGING
void MotorMegaMotoHBridge::serialRegister(const __FlashStringHelper* DriverName,const __FlashStringHelper* BrigeAName,const __FlashStringHelper* BrigeBName)
{

	FieldData::set(DriverName, F("RequestedSpeed"),MOD_WRITE,&myCurrentRequestedSpeed);
#ifdef DETAILED_SERIAL_LOGGING
	FieldData::set(DriverName, F("myNewRequestedSpeed"),MOD_WRITE,&myNewRequestedSpeed);
	FieldData::set(DriverName, F("IsDirty"),0,&myIsDirty);
	FieldData::set(DriverName, F("LoopDuration"),0,&myLoopDuration);
#endif

	myMegaMotoDriver.serialRegister( DriverName,  BrigeAName,  BrigeBName);

}



void MotorMegaMotoHBridge::changeSpeed(int16_t requestedSpeed)
{

	myCurrentRequestedSpeed = requestedSpeed;
	if (abs(requestedSpeed) < MINIMUMSPEEDTORUN) myCurrentRequestedSpeed = 0;

	uint8_t ReqSpeedA = (myCurrentRequestedSpeed > 0) * abs(myCurrentRequestedSpeed);
	uint8_t ReqSpeedB = (myCurrentRequestedSpeed < 0) * abs(myCurrentRequestedSpeed);
	if (((myMegaMotoDriver.myHalfBridgeA.getRequestedSpeed() == 0) && (myMegaMotoDriver.myHalfBridgeA.getActualSpeed() == 0) && (ReqSpeedA == 0))
			|| ((myMegaMotoDriver.myHalfBridgeB.getRequestedSpeed() == 0) && (myMegaMotoDriver.myHalfBridgeB.getActualSpeed() == 0) && (ReqSpeedB == 0)))
	{
		myMegaMotoDriver.setRequestedSpeedA(ReqSpeedA);
		myMegaMotoDriver.setRequestedSpeedB(ReqSpeedB);
		myIsDirty = false;
	} else
	{
		myMegaMotoDriver.setRequestedSpeedA(0);
		myMegaMotoDriver.setRequestedSpeedB(0);
		myIsDirty = true;
	}
}

void MotorMegaMotoHBridge::loop()
{
	unsigned long loopStart = millis();
	if ((myIsDirty) || (myCurrentRequestedSpeed!=myNewRequestedSpeed))
	{
		changeSpeed(myNewRequestedSpeed);
	}
	myMegaMotoDriver.loop();
	//myIsAtSpeed = !myMegaMotoDriver.isDirty();
	myCurrentUsage_cA=myMegaMotoDriver.getCurrentUsage_cA();
	myActualSpeed=myMegaMotoDriver.myHalfBridgeA.getActualSpeed()-myMegaMotoDriver.myHalfBridgeB.getActualSpeed();
	myLoopDuration = millis() - loopStart;
}

