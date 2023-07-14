/*
 * BLDC1Motor.cpp
 *
 *  Created on: May 9, 2014
 *      Author: jan
 */

#include "MotorBLDC.h"


#define STEP 10
MotorBLDC::MotorBLDC(uint8_t forwardBackwardPin, uint8_t PWMPin)
{
	myRequestedSpeed = 0;
	myActualSpeed = 0;
	myActualSpinDirection = 0;
	myLoopduration = 0;
	myForwardBackwardPin = forwardBackwardPin;
	myPWMPin = PWMPin;
}

void MotorBLDC::serialRegister(const __FlashStringHelper* Name)
{

		FieldData::set(Name, F("RequestedSpeed"),MOD_WRITE , &myRequestedSpeed);
		FieldData::setNext( F("ActualSpeed"), 0, &myActualSpeed);
		FieldData::setNext( F("ActualSpinDirection"), 0, &myActualSpinDirection);
		FieldData::setNext( F("ForwardBackwardPin"), 0, &myForwardBackwardPin);
		FieldData::setNext( F("PWMPin"), 0, &myPWMPin);
		FieldData::setNext((__FlashStringHelper *) LOOPDURATION, 0, &myLoopduration);
	}

void MotorBLDC::setup()
{
	pinMode(myForwardBackwardPin, OUTPUT);
	pinMode(myPWMPin, OUTPUT);
	digitalWrite(myForwardBackwardPin, LOW);
	analogWrite(myPWMPin, 0);
}

#define defaultDelay 10
void MotorBLDC::loop()
{
	if (myActualSpeed != myRequestedSpeed)
	{
		static unsigned long last_action_Time = millis();
		static unsigned long cur_delay=defaultDelay;
		if (millis() - last_action_Time > cur_delay)
		{
			last_action_Time = millis();
			SPEED_TYPE newActualSpeed = myActualSpeed;
			if (myActualSpeed < myRequestedSpeed)
			{
				newActualSpeed = min(newActualSpeed+STEP,myRequestedSpeed);
			} else
			{
				newActualSpeed = max(newActualSpeed-STEP,myRequestedSpeed);
			}

			cur_delay=defaultDelay;
			if (((int64_t)newActualSpeed * (int64_t)myActualSpeed) < 0)
			{
				cur_delay = 200;
				newActualSpeed = 0;
			}
			myActualSpeed = newActualSpeed;
			if (myActualSpeed < 0)
			{
				myActualSpinDirection = HIGH;
			} else
			{
				myActualSpinDirection = LOW;
			}
			digitalWrite(myForwardBackwardPin, myActualSpinDirection);
			uint8_t pwmValue=abs(myActualSpeed);
			analogWrite(myPWMPin,pwmValue );
//			Serial.print("pwm value=");
//			Serial.println(pwmValue);
		}
	}
}

bool MotorBLDC::emergencyBreak(bool immediatlyReturn)
{
	myActualSpeed=0;
	myRequestedSpeed=0;
	analogWrite(myPWMPin,myActualSpeed ); //I assume the motor does a decent stop
	return true;
}
