/*
 * HalfBridge.cpp
 *
 *  Created on: Mar 31, 2013
 *      Author: jan
 */

#include "HalfBridge.h"
#include "FastRunningMedian.h"


//FastRunningMedian<int, 30, 0> medianCurrent;


/**
 * calculatePower calculates the power usage in watt.
 * batteryVoltage is the voltage in volt
 */
uint16_t HalfBridge::calculatePower(uint8_t batteryVoltage) const
{
	if (myCurrentPin != 255)
	{
		return (uint16_t) (((uint32_t) myCurrent_cA * (uint32_t) batteryVoltage) / 100UL); //in watts
	}
	return 0;
}

void HalfBridge::loop()
{
	if (myIsDirty)
	{
		if ((millis() - MyLastactionTime) >= DRIVE_MOTOR_DELAY )
		{
			myIsDirty = (myRequestedSpeed != myActualSpeed);
			CalculatenewPWMValuee();
			powerEngine();
		}
	}
	if (myCurrentPin != 255)
	{
//		medianCurrent.addValue(analogRead(myCurrentPin));
//		myCurrentPinValue = medianCurrent.getMedian();
		myCurrentPinValue=analogRead(myCurrentPin);
		myCurrent_cA = ((uint32_t) myCurrentPinValue * (uint32_t) myMultiplyerValue) / 1000UL; //* (uint32_t) myActualSpeed) / 255000UL;
	}
}

void HalfBridge::CalculatenewPWMValuee()//int ReqValue, int CurValue) myRequestedSpeed, myActualSpeed
{
	uint8_t ret=0;
	if (myRequestedSpeed > myActualSpeed)
	{
		ret=  min( myActualSpeed + DRIVE_MOTER_MAX_STEP , myRequestedSpeed);
		myActualSpeed=(ret<MINPWM)?MINPWM:ret;
	} else
	{
		ret= max( myActualSpeed - DRIVE_MOTER_MAX_STEP ,myRequestedSpeed);
		myActualSpeed=(ret<MINPWM)?0:ret;
	}

}

void HalfBridge::setRequestedSpeed(uint8_t Speed)
{
	if(Speed!=0)
	{
		Speed=max(MINPWM,Speed);
	}
	if (myRequestedSpeed != Speed)
	{
		myIsDirty = true;
		myRequestedSpeed = Speed;
	}
}

HalfBridge::HalfBridge( uint8_t PWMPin, uint8_t CurrentPin)
{
	myPWMPin = PWMPin;
	myCurrentPin = CurrentPin;
	myActualSpeed = 0;
	myIsDirty = true;
	myRequestedSpeed = 0;
	MyLastactionTime = millis();
	myCurrent_cA = 0;
	myCurrentPinValue = 0;
	myMultiplyerValue=100;

}

void HalfBridge::serialRegister(const __FlashStringHelper* Name)
{

	FieldData::set(Name, F("RequestedSpeed"),0,&myRequestedSpeed);
	FieldData::setNext( F("ActualSpeed"),0,&myActualSpeed);
	FieldData::setNext( F("Current(cA)"),0,&myCurrent_cA);
	FieldData::setNext( (const __FlashStringHelper *)MULTIPLIER,MOD_WRITE|MOD_SAVE,&myMultiplyerValue);

#ifdef DETAILED_SERIAL_LOGGING
	FieldData::setNext( F("isDirty"),0,&myIsDirty);
	FieldData::setNext( F("CurrentPinValue"),0,&myCurrentPinValue);
	FieldData::set(Name, F("PWMPin"),0,&myPWMPin);
	FieldData::set(Name, F("CurrentPin"),0,&myCurrentPin);
	FieldData::set(Name, F("LastactionTime"),0,&MyLastactionTime);
#endif

}
