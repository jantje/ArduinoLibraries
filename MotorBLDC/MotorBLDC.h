/*
 * BLDC1Motor.h
 * This is a class that handles the BLDC motor I bought from topbrand (I do not advice the brand nor the motor !!!)
 * The motor needs a pull down resistor (no more then 800 ohms) on the PWM pin (no connection will make the motor spin)
 * The direction pin can not handle +5V so it needs a PNP resistor to.
 *
 * Once these connections are done the motor is controlled by PWM (255= full speed) for speed
 * and the direction pin for the direction.
 *
 * This class changes the PWM slowly based on experiments (I'm not sure it is needed)
 *
 * engine on and engine off are not powering the motors as should because I did not use a mosfet or simular to switch the motors on/off
 *
 *  Created on: May 9, 2014
 *      Author: jan
 */

#ifndef BLDC1_MOTOR_H_
#define BLDC1_MOTOR_H_
#include "DataTypes.h"
#include "MotorInterface.h"

#ifdef I_USE_PLOT
#include "simplot.h"
#endif

class MotorBLDC: public MotorInterface
{

	private:
		SPEED_TYPE myRequestedSpeed;
		SPEED_TYPE myActualSpeed;
		uint8_t myActualSpinDirection;
		uint32_t myLoopduration;
		uint8_t myForwardBackwardPin;
		uint8_t myPWMPin;


	public:
#ifdef I_USE_PLOT
		void plotdata()
		{
			plot4(Serial,myRequestedSpeed,myActualSpeed,(myActualSpinDirection*500)-255,(millis() % 1000) / 4);

		}
#endif
		void motorOn(){};
		void motorOff(){setRequestedSpeed(0);};
		MotorBLDC( uint8_t forwardBackwardPin, uint8_t PWMPin);
		void serialRegister(const __FlashStringHelper* Name);

		void setup();
		void loop();
		void setRequestedSpeed(SPEED_TYPE speed){myRequestedSpeed=speed;};

		 bool emergencyBreak(){emergencyBreak(false);};
		bool emergencyBreak( bool immediatlyReturn = false);

		SPEED_TYPE getRequestedSpeed() const
		{	return myRequestedSpeed;}

		SPEED_TYPE getActualSpeed() const
		{	return myActualSpeed;};

	};


#endif /* BLDC1_MOTOR_H_ */
