#pragma once
/**
 * This library controls a motor based on PWM signal.
 * This can be a continuous rotation servo or a brushless motor run by a motor driver controlled by PWM
 */

#include "Arduino.h"
#include "DataTypes.h"
#include "SerialDataInterface.h"
#include "IMotor.h"
#ifdef I_USE_PLOT
#include "simplot.h"
#endif

/**
 * Use this class to control a motor that only needs PWM as input for speed.
 */
class MotorPWM: public IMotor
{
	private:
		uint8_t myPWMPin;					//The pin the motor driver is attached to.
		uint8_t myStopSpeed;   //The speed at which the motor is standing still.
													 // for a continuous rotation servo that should be around 180
													 // for a one directional motor this is 0
		uint8_t myDirectionPin;  //the pin for reversed direction. Use 255 when you do not have a direction pin
		uint8_t myDirectionPinValue; // The Value of the directionPin
		boolean isMotorOn;  //flag wether motor is on or off
		boolean myReversed; //high is low is high for the speed
	public:

		//PWMMotor( uint8_t PWMPin, uint8_t stopSpeed, uint8_t directionPin);
		/**
		 * creates a PWMmotor object to control a motor based on PWM.
		 * PWMPin the pin connected to the motor(controller)
		 * stopSpeed the speed at which the motor is standing still
		 * directionPin the pin to use to change the direction of the motor.
		 * reversed when high is off and low is on or full speed 255 should be 0 on the pin
		 *
		 */
		MotorPWM(uint8_t PWMPin, uint8_t stopSpeed, uint8_t directionPin, boolean reversed = false);
		void serialRegister(const __FlashStringHelper* Name);

		void setup();
		void loop();

		boolean emergencyBreak( );

		/*
		 * If you use simplot or the arduino eclipse plugin you can use
		 * plotting functionality to better understand this code
		 * the last value gives a tooth signal with 1 second interval
		 */
#ifdef I_USE_PLOT
				void plot()
				{	plot3(Serial,myRequestedSpeed,myActualSpeed,myStopSpeed);}
#endif

				//for code compatibility
				void motorOff()
				{	setRequestedSpeed(0);isMotorOn=false;};
				void motorOn()
				{	isMotorOn=true;};
//		boolean motorUsesToMuchPower(){return false;};
//		boolean areKnifesToLoaded(){return false;};
//		boolean OKToIncreaseLoad(){return isAtSpeed();};

			};
