//#include "MaaiRobot.h"
#include "MotorPWM.h"

void MotorPWM::serialRegister(const __FlashStringHelper* Name)
{
	FieldData::set(Name, F("isOn"),MOD_WRITE,&isMotorOn);
	FieldData::setNext(F("ActualSpeed"),MOD_NONE,&myActualSpeed);

	IMotor::serialRegister(Name);
#ifdef DETAILED_SERIAL_REGISTER
	FieldData::setNext(F("PWMPin"),0,&myPWMPin);
	FieldData::setNext(F("StopSpeed"),MOD_WRITE,&myStopSpeed);
	FieldData::setNext(F("DirectionPinValue"),MOD_NONE,&myDirectionPinValue);
#endif

}

#define defaultDelay 10
#define STEP 10
/**
 * The Loop method must be called directly or indirectly as part of your main Loop
 *  This does the major bulk of the work.
 *  The other  methods are getters and setters.
 */
void MotorPWM::loop()
{
	if (!isMotorOn) myNewRequestedSpeed = 0;
	myRequestedSpeed = myNewRequestedSpeed;
	if (myActualSpeed != myRequestedSpeed)
	{
		static unsigned long cur_delay = defaultDelay;
		if (millis() - myLastSpeedChangeTime > cur_delay)
		{
			myLastSpeedChangeTime = millis();
			SPEED_TYPE newActualSpeed = myActualSpeed;
			if (myActualSpeed < myRequestedSpeed)
			{
				newActualSpeed = min(newActualSpeed + STEP, myRequestedSpeed);
			} else
			{
				newActualSpeed = max(newActualSpeed - STEP, myRequestedSpeed);
			}

			cur_delay = defaultDelay;
			//If we pass the stopping point wait a little longer
			if ((SIGN(newActualSpeed - myStopSpeed) * SIGN(myActualSpeed - myStopSpeed)) < 0)
			{
				cur_delay = 10000;
				newActualSpeed = myStopSpeed;
			}

			/*
			 * Only change the direction if we are currently stopped and we start again.
			 */
			if ((myDirectionPin < 255) && (newActualSpeed != myStopSpeed) && (myActualSpeed == myStopSpeed))
			{
				myDirectionPinValue = SIGN(newActualSpeed - myStopSpeed) < 0 ? HIGH : LOW;
				digitalWrite(myDirectionPin, myDirectionPinValue);
			}
			myActualSpeed = newActualSpeed;
			analogWrite(myPWMPin, myReversed ? (255 - abs(myActualSpeed)) : abs(myActualSpeed));

		}
	}
}

MotorPWM::MotorPWM(uint8_t PWMPin, uint8_t stopSpeed, uint8_t directionPin, boolean reversed)
{
	myRequestedSpeed = stopSpeed;
	myActualSpeed = stopSpeed;
	myPWMPin = PWMPin;
	myStopSpeed = stopSpeed;
	isMotorOn = false;
	myDirectionPin = directionPin;
	myDirectionPinValue = LOW;
	myReversed = reversed;
}

/**
 * If immediatlyReturn is set to true this method will immediately return and thrust on a good sequence of calling Loop
 * to properly stop the engine
 * Otherwise this method will only return after the engine coils have no more power
 * Note the motor may not be in a stand still after the return of this message.
 */
boolean MotorPWM::emergencyBreak()
{
	myActualSpeed = myStopSpeed;
	myRequestedSpeed = myStopSpeed;
	analogWrite(myPWMPin, myReversed ? (255 - myActualSpeed) : myActualSpeed); //I assume the motor does a decent stop
	return true;
}

/**
 * To be called as part of the setup of your sketch
 */
void MotorPWM::setup()
{

	pinMode(myPWMPin, OUTPUT);
	analogWrite(myPWMPin, myReversed ? (255 - myActualSpeed) : myActualSpeed);
	if (myDirectionPin < 255)
	{
		pinMode(myDirectionPin, OUTPUT);
		digitalWrite(myDirectionPin, LOW);
	}

}

