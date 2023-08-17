/*
 * ScooterMotor.cpp
 *
 *  Created on: Jul 5, 2014
 *      Author: jan
 */

#include "MotorPotmeterDirection.h"



MotorPotmeterDirection::MotorPotmeterDirection(uint8_t clockwisePin, CurrentSensorInterface &currentSensor,DigitalPotmeterInterface &potMeter)
		: myCurrentSensor(currentSensor),myPotmeter(potMeter)
{
	myRequestedSpeed = 0;
	myActualSpeed = 0;
	mySpinsClockWise = true;
	myClockwisePin = clockwisePin;
	//myI2CAdress = I2CAdress;
	MyLastactionTime = millis();


}

void MotorPotmeterDirection::setPotValue()
{
	uint8_t newActualSpeed=abs(myActualSpeed);
	if (newActualSpeed<30)
	{
		myPotmeter.setPotValue(0);
	}
	else
	{
	  myPotmeter.setPotValue(map(constrain(newActualSpeed,0,240),0,240,120,255));
	}


}

#define defaultDelay 10
#define STEP 10
void MotorPotmeterDirection::loop()
{
	myRequestedSpeed = myNewRequestedSpeed;

	static unsigned long last_action_Time = millis();
	static unsigned long cur_delay = defaultDelay;
	static bool waitForMotorToStop = false;
	if (millis() - last_action_Time > cur_delay)
	{
		last_action_Time = millis();
		cur_delay = defaultDelay;
		if (waitForMotorToStop)
		{

			if ((myActualSpeed == 0)&&(myCurrentSensor.getAvgCurrent_cA()<=myMaxTurnCurrent_cA))
			{
				mySpinsClockWise = (myRequestedSpeed >= 0);
				digitalWrite(myClockwisePin, mySpinsClockWise ? LOW : HIGH);
				cur_delay = 100;
				waitForMotorToStop=false;
			}
		} else
		{
			if (myActualSpeed != myRequestedSpeed)
			{
				SPEED_TYPE newActualSpeed = myActualSpeed;
				if (myActualSpeed < myRequestedSpeed)
				{
					newActualSpeed = min(newActualSpeed+STEP,myRequestedSpeed);
				} else
				{
					newActualSpeed = max(newActualSpeed-STEP,myRequestedSpeed);
				}

				if ( (SIGN( newActualSpeed)!=SIGN( myActualSpeed) ) && (mySpinsClockWise != (newActualSpeed >= 0)))
				{
					newActualSpeed = 0;
					waitForMotorToStop = true;
					cur_delay = 100;
				}

				myActualSpeed = newActualSpeed;
				setPotValue();

				MyLastactionTime = millis();
			}
		}
	}
	myIsDirty=(myActualSpeed!=myRequestedSpeed);
	myPotmeter.loop();
	myCurrentSensor.loop();
	myCurrentUsage_cA=abs(myCurrentSensor.getAvgCurrent_cA());

}

void MotorPotmeterDirection::setup()
{
	pinMode(myClockwisePin, OUTPUT);
	digitalWrite(myClockwisePin, LOW);
	myPotmeter.setup();
	myCurrentSensor.setup();
}

void MotorPotmeterDirection::motorOn()
{
//Nothing to do here
}

void MotorPotmeterDirection::motorOff()
{
	myNewRequestedSpeed = 0;
}

bool MotorPotmeterDirection::emergencyBreak(bool immediatlyReturn)
{
	if ((abs(myActualSpeed) > 0) && (millis() - MyLastactionTime > 10))
	{

		myNewRequestedSpeed = myRequestedSpeed = myActualSpeed = SIGN(myActualSpeed) * constrain(abs(myActualSpeed) - 30, 0, 255);
		MyLastactionTime = millis();
		setPotValue();


	}
	return (myActualSpeed == 0) && ((millis() - MyLastactionTime > 200));
}

#ifdef I_USE_SERIAL_REGISTER
void MotorPotmeterDirection::serialRegister(const __FlashStringHelper* Name)
{
	IMotor::serialRegister(Name);
	FieldData::set( Name,F("ActualSpeed"), 0, &myActualSpeed);
#ifdef DETAILED_SERIAL_LOGGING
	FieldData::setNext( F("SpinsClockWise"), 0, &mySpinsClockWise);
	FieldData::setNext( F("ClockwisePin"), 0, &myClockwisePin);
#endif
}
#endif

