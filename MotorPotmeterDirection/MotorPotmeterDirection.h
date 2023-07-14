/*
 * ScooterMotor.h
 *
 *  Created on: Jul 5, 2014
 *      Author: jan
 */

#ifndef SCOOTERMOTOR_H_
#define SCOOTERMOTOR_H_

#include "MotorInterface.h"
#include "SerialDataInterface.h"
#include "CurrentSensorInterface.h"
#include "DigitalPotmeterInterface.h"


/**
 * This class is to control a scooter motor that can go clockwise and counterclockwise
 * The motor is controled by a digital potmeter AD5171
 * and a pin to switch between clockwise and counterclockwise
 * The load of the motor is measused with a ampere meter connected to a analog pin
 * The switch between clockwise and counterclockwise is only done when the motor is standing still.
 */
class MotorPotmeterDirection: public MotorInterface
{
	protected :
		bool	mySpinsClockWise;
		uint8_t myClockwisePin;
		uint32_t MyLastactionTime;
		CurrentSensorInterface &myCurrentSensor;
		DigitalPotmeterInterface &myPotmeter;
		void setPotValue();
	public:
#ifdef I_USE_SERIAL_REGISTER
		void serialRegister(const __FlashStringHelper* Name);
#endif
		/**
		 * The constructor of the scooter motor
		 *
		 * clockwisePin the digital pin to control clockwise or counter clockwise
		 * ampPin the analog pin giving the amps used by the motor
		 * I2CAdress the I2C adress to talk to the digital pot
		 */
		MotorPotmeterDirection(uint8_t clockwisePin,CurrentSensorInterface &currentSensor,DigitalPotmeterInterface &poMeter);
		virtual void loop();
		virtual void setup();
		virtual void motorOn();
		virtual void motorOff();
		bool emergencyBreak(){emergencyBreak(false);};
		virtual bool emergencyBreak(bool immediatlyReturn);

};

#endif /* SCOOTERMOTOR_H_ */
