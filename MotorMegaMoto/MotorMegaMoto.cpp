/*
 * MegaMotoDriver.cpp
 *
 *  Created on: 30-sep.-2011
 *      Author: be4258
 */

#include "MotorMegaMoto.h"

void MotorMegaMoto::motorOn()
{
#ifdef SHARED_ENABLEPIN
#else
	digitalWrite(ENABLEPINNAME, HIGH);
#endif
}
void MotorMegaMoto::motorOff()
{
#ifdef SHARED_ENABLEPIN
	myHalfBridgeA.setRequestedSpeed(0);
	myHalfBridgeB.setRequestedSpeed(0);
#else
	digitalWrite(ENABLEPINNAME, LOW);
	myHalfBridgeA.setRequestedSpeed(0);
	myHalfBridgeB.setRequestedSpeed(0);
#endif
}

MotorMegaMoto::MotorMegaMoto( ENABLEPINPARAM uint8_t PWMAPin, uint8_t PWMBPin, uint8_t ACurrentPin, uint8_t BCurrentPin)
: myHalfBridgeA( PWMAPin, ACurrentPin), myHalfBridgeB( PWMBPin, BCurrentPin)
{
#ifndef SHARED_ENABLEPIN
	this->ENABLEPINNAME = ENABLEPINNAME;
	pinMode(ENABLEPINNAME, OUTPUT);
#endif
	//mySuppliedVoltage = suppliedVoltage;
	// myLoopDuration = 0;
	//myMotorVoltage = MotorVoltage;
	myRequestedSpeedA = 0;
	myRequestedSpeedB = 0;
}

void MotorMegaMoto::serialRegister(const __FlashStringHelper* Name,const __FlashStringHelper* BrigeAName,const __FlashStringHelper* BrigeBName)
{
#ifdef DETAILED_SERIAL_LOGGING
//	FieldData::set(Name, F("MotorVoltage"),0,&myMotorVoltage);
//	FieldData::set(Name, F("SuppliedVoltage"),0,&mySuppliedVoltage);
	FieldData::set(Name, F("RequestedSpeedA"),0,&myRequestedSpeedA);
	FieldData::set(Name, F("RequestedSpeedB"),0,&myRequestedSpeedB);
	//FieldData::set(Name, F("LoopDuration"),0,&myLoopDuration);
#endif
	myHalfBridgeA.serialRegister(BrigeAName);
	myHalfBridgeB.serialRegister(BrigeBName);
}

void MotorMegaMoto::loop()
{
//	unsigned long loopStart = millis();
	myHalfBridgeA.loop();
	myHalfBridgeB.loop();

//	myLoopDuration = millis() - loopStart;
}

/**
 * This method stops the engine directly only when immediatlyReturn is not true
 * If immediatlyReturn is true this method should be called in a tight loop without delay.
 * This is needed when more than one engine needs to be stopped immediately
 * The method returns true when the engine is stalled
 */
bool MotorMegaMoto::emergencyBreak()
{
	myRequestedSpeedA = 0;
	myRequestedSpeedB = 0;
	return (myHalfBridgeA.emergencyBreak() && myHalfBridgeB.emergencyBreak());
}

