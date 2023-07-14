#include "SkidDriverMegaMoto.h"
#include "Bounce2.h"

void SkidDriverMegaMoto::motorOn()
{
	myLeftMotor.motorOn();
	myRightMotor.motorOn();
}
void SkidDriverMegaMoto::motorOff()
{
	//digitalWrite(MyEnablePin, LOW);
	setSpeedAndDirection(0, 0);
	myLeftMotor.motorOff();
	myRightMotor.motorOff();
}
//#define STANDARDVOLTAGE ,((14 * 255)/24)
//#define MAXVOLTAGE STANDARDVOLTAGE
//#define DURATIONATMAXVOLTAGE  ,0
//#define CURRENTTRIGGER  ,20000

#define STANDARDVOLTAGE
#define MAXVOLTAGE
#define DURATIONATMAXVOLTAGE
#define CURRENTTRIGGER
//#ifdef SHARED_ENABLEPIN
SkidDriverMegaMoto::SkidDriverMegaMoto( ENABLEPINPARAM uint8_t RightPWMAPin, uint8_t RightPWMBPin, ENABLEPINPARAMLEFT uint8_t LeftPWMAPin, uint8_t LeftPWMBPin, uint8_t RightCurrentPin, uint8_t LeftCurrentPin)
: myLeftMotor( ENABLEPINATTRIBUTE RightPWMAPin, RightPWMBPin, RightCurrentPin, RightCurrentPin STANDARDVOLTAGE MAXVOLTAGE DURATIONATMAXVOLTAGE CURRENTTRIGGER), myRightMotor( ENABLEPINATTRIBUTELEFT LeftPWMAPin, LeftPWMBPin, LeftCurrentPin,
		LeftCurrentPin STANDARDVOLTAGE MAXVOLTAGE DURATIONATMAXVOLTAGE CURRENTTRIGGER)
{
	myRequestedSpeed = 0;
	myRequestedDirection = 0;
	myLoopduration = 0;
	myMaxCurrent = 0;
	myMaxCurrentDuration = 0;
	myUseWheelCorrection = false;
	myCurrentDirectionCorrection = 0;
	myPrefLeftCountPinState = LOW;
	myPrefRightCountPinState = LOW;
	myLeftCount = 0;
	myRightCount = 0;
	myLeftCountPin = 31;
	myRightCountPin = 33;
	myLastDirectionModification = millis();
}

#define DETAILED_SERIAL_LOGGING
#define SUPERDETAILED
void SkidDriverMegaMoto::serialRegister(const __FlashStringHelper* Name,const __FlashStringHelper* LeftDriverName,const __FlashStringHelper* LeftBrigeAName,const __FlashStringHelper* LeftBrigeBName,const __FlashStringHelper* RightDriverName,const __FlashStringHelper* RightBrigeAName,const __FlashStringHelper* RightBrigeBName)
{

	FieldData::set(Name, F("RequestedSpeed"),MOD_WRITE|MOD_OVERVIEW,&myRequestedSpeed);
	FieldData::set(Name, F("RequestedDirection"),MOD_WRITE|MOD_OVERVIEW,&myRequestedDirection);
	//FieldData::set(Name, F("EngineState"),0,(uint16_t*)&myWheelsState);
	FieldData::set(Name, F("MaxCurrent"),MOD_WRITE|MOD_SAVE,&myMaxCurrent);
	FieldData::set(Name, F("MaxCurrentDuration"),MOD_WRITE|MOD_SAVE,&myMaxCurrentDuration);
	FieldData::set(Name, F("UseWheelCorrection"),MOD_WRITE|MOD_SAVE|MOD_OVERVIEW,&myUseWheelCorrection);
	FieldData::set(Name, F("myCurrentDirectionCorrection"),MOD_OVERVIEW,&myCurrentDirectionCorrection);

#ifdef DETAILED_SERIAL_LOGGING

	FieldData::set(Name, F("myLeftCount"),MOD_NONE,&myLeftCount);
	FieldData::set(Name, F("myRightCount"),MOD_NONE,&myRightCount);
	FieldData::set(Name, F("myLastDirectionModification"),MOD_NONE,&myLastDirectionModification);
#ifdef SUPERDETAILED
	FieldData::set(Name, F("myPrefLeftCountPinState"),MOD_NONE,&myPrefLeftCountPinState);
	FieldData::set(Name, F("myPrefRightCountPinState"),MOD_NONE,&myPrefRightCountPinState);
	FieldData::set(Name, F("myLeftCountPin"),MOD_NONE,&myLeftCountPin);
	FieldData::set(Name, F("myRightCountPin"),MOD_NONE,&myRightCountPin);
#endif
#endif

	myLeftMotor.serialRegister( LeftDriverName, LeftBrigeAName, LeftBrigeBName);
	myRightMotor.serialRegister( RightDriverName, RightBrigeAName, RightBrigeBName);

}

#define NUMPOSITIVES 1

Bounce leftBounce;
Bounce rightBounce;
void SkidDriverMegaMoto::loop()
{
	leftBounce.update();
	rightBounce.update();
	unsigned long loopstart = millis();
	if ((myRequestedDirection != myNewRequestedDirection) || (myRequestedSpeed != myNewRequestedSpeed))
	{
		myRequestedDirection = myNewRequestedDirection;
		myRequestedSpeed = myNewRequestedSpeed;
		calculateSpeedAndDirection();
		myLastDirectionModification = millis();
	} else
	{

		/*
		 * We only try to drive straight if we drive straight
		 */
		if ((myRequestedDirection == 0))
		{
			/*
			 * If we stand still we start from scratch
			 */
			if (myRequestedSpeed == 0)
			{
				myLeftCount = 0;
				myRightCount = 0;
				myCurrentDirectionCorrection = 0;
			}
			/*Make sure we do not overflow
			 */
			if (min(myLeftCount, myRightCount) > 1000)
			{
				myLeftCount -= 1000;
				myRightCount -= 1000;
			}

			myLeftCount += leftBounce.rose() ? 1 : 0;
			myRightCount += rightBounce.rose() ? 1 : 0;

			int diff = abs(myLeftCount - myRightCount);
			if ((diff > 3) && (millis() - myLastDirectionModification > 500) && myUseWheelCorrection)
			{
				/* There is to much difference. Try to correct. */
				if (myLeftCount < myRightCount)
					myCurrentDirectionCorrection = min(45, diff);
				else
					myCurrentDirectionCorrection = max(-45, -1 * diff);

				myLastDirectionModification = millis();
				myRequestedDirection = myCurrentDirectionCorrection;
				calculateSpeedAndDirection();
				myRequestedDirection = 0;
			}
		} else
		{
			myCurrentDirectionCorrection = 0;
			myLeftCount = 0;
			myRightCount = 0;
			myPrefLeftCountPinState = LOW;
			myPrefRightCountPinState = LOW;
			myLastDirectionModification = millis();
		}
	}
	myLeftMotor.loop();
	myRightMotor.loop();

	myLoopduration = millis() - loopstart;

}

const __FlashStringHelper * SkidDriverMegaMoto::getOverloadedWheelName()
{
	if ((myLeftMotor.getCurrentUsage() > myMaxCurrent) && (myRightMotor.getCurrentUsage() > myMaxCurrent)) return F("both");
	if (myLeftMotor.getCurrentUsage() > myMaxCurrent) return F("left");
	if (myRightMotor.getCurrentUsage() > myMaxCurrent) return F("right");
	return F("no");

}

void SkidDriverMegaMoto::calculateSpeedAndDirection()
{
	if ((myRequestedSpeed == 0) && (myRequestedDirection == 0))
	{
		myRightMotor.setRequestedSpeed(0);
		myLeftMotor.setRequestedSpeed(0);
	} else if (abs(myRequestedSpeed) < 5)
	{
		int16_t speed = -1 * map(constrain(myRequestedDirection, -90, 90), -90, 90, -255, 255);
		myRightMotor.setRequestedSpeed(speed);
		myLeftMotor.setRequestedSpeed(speed);
	} else
	{
		myRightMotor.setRequestedSpeed(constrain(myRequestedSpeed - (myRequestedDirection * SIGN(myRequestedSpeed)), -255, 255));
		myLeftMotor.setRequestedSpeed(constrain(myRequestedSpeed + (myRequestedDirection * SIGN(myRequestedSpeed)), -255, 255) * -1);
	}
}

void SkidDriverMegaMoto::turnOnTheSpot(SPEED_TYPE Speed)
{
	myRequestedSpeed = myRequestedDirection = myNewRequestedSpeed = myNewRequestedDirection = Speed;
	myRightMotor.setRequestedSpeed(Speed);
	myLeftMotor.setRequestedSpeed(Speed);
}

/**
 * EmergencyBreak stops the drive engines directly powering the coils
 * To avoid powerspikes the power is gradually decreased
 * If immediatlyReturn is true this method should be called in a tight loop without significant delay.
 * This is needed when more than one engine needs to be stopped immediately
 * The method returns true when the attached engines are stalled
 * The method returns false when emergencyBreak needs to be called again.
 */
bool SkidDriverMegaMoto::emergencyBreak(bool immediatlyReturn)
{

	myRequestedDirection = 0;
	myRequestedSpeed = 0;
	myNewRequestedDirection = 0;
	myNewRequestedSpeed = 0;
	return (myLeftMotor.emergencyBreak() && myRightMotor.emergencyBreak());
}

void SkidDriverMegaMoto::setup()
{
	pinMode(myLeftCountPin, INPUT_PULLUP);
	pinMode(myRightCountPin, INPUT_PULLUP);
	leftBounce.attach(myLeftCountPin);
	rightBounce.attach(myRightCountPin);
	leftBounce.interval(100);
	rightBounce.interval(100);
	myRightMotor.setup();
	myLeftMotor.setup();
}

