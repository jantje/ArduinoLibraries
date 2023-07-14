/*
 * HalfBridge.h
 *
 *  Created on: Mar 31, 2013
 *      Author: jan
 */

#ifndef HALFBRIDGE_H_
#define HALFBRIDGE_H_
#include "Arduino.h"
#include "SerialDataInterface.h"
#include "FieldInfo.h"
#include "PwmFrequency.h"

#define MINPWM 120
#define DRIVE_MOTER_MAX_STEP  10
#define DRIVE_MOTOR_DELAY      5
//100 is way to slow reaction
// with 10 delay step 30 he is pretty aggressive and the bumper sensors get triggered
// with 20 delay step 30 less aggressive but the bumper still gets triggered from time to time
// with 30 delay step 30 /way to slow cra
// trying 10 delay step 10  fixed bumper being loose seems to help a lot
// was on 30 30 moved to 10 5 for doga motors as mega moto goes in safety
class HalfBridge
{
	private:
		//pins
		uint8_t myPWMPin;

		uint8_t myRequestedSpeed;
		uint8_t myActualSpeed;  //The PWM value on the myPWMPin
		uint32_t MyLastactionTime;
		bool myIsDirty;

		uint8_t myCurrentPin;
		uint16_t myCurrentPinValue;
		uint16_t myCurrent_cA;
		uint16_t myMultiplyerValue; //multiplier (factor 1000) for current calc



		void powerEngine()
		{
			analogWrite(myPWMPin, myActualSpeed);
			MyLastactionTime = millis();
		}
		void CalculatenewPWMValuee();

	public:
		HalfBridge(		uint8_t PWMPin,	uint8_t CurrentPin);
		void serialRegister(const __FlashStringHelper* Name);

		uint8_t getRequestedSpeed() const
		{
			return myRequestedSpeed;
		}
		uint8_t getActualSpeed() const
		{
			return myActualSpeed;
		}

		uint16_t getCurrentUsage_cA() const
		{
			return myCurrent_cA;
		}

		bool isDirty() const
		{
			return myIsDirty;
		}

		uint16_t calculatePower(uint8_t batteryVoltage) const;

#define PRESCALER 1
		void setup(Stream &Error)
		{
			MyLastactionTime = millis();
			pinMode(myPWMPin, OUTPUT);
			if (!setPwmFrequency(myPWMPin, PRESCALER))
			{
				Error.print( F("failed to set PWM frequency; pin:"));
				Error.print(myPWMPin);
				Error.print(F(" prescaler : "));
				Error.println(PRESCALER);
			}

		}
		void loop();
		void setRequestedSpeed(uint8_t Speed);
		/**
		 * Slow down the engine as much as possible to be still save
		 * return true if the engine is stalled
		 * return false if the engine is not yet stalled.
		 * call this method in a loop untill the method returns true to be sure the engine stopped
		 *
		 */
		bool emergencyBreak()
		{
			myIsDirty=false; /*make sure loop does not interfere */
			if ( (myActualSpeed > 0 ) && (millis() -MyLastactionTime >10)  )
			{
			myRequestedSpeed=myActualSpeed= constrain(myActualSpeed - 30, 0, 255);
			powerEngine();
			}
			return (myActualSpeed ==0) && ((millis() - MyLastactionTime >200) );
		}

};

#endif /* HALFBRIDGE_H_ */
