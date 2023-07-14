/*
 * BLDC1SkidDriver.h
 *
 * This is the skid driver implementation for the BLDC1 motor.
 *
 *  Created on: May 9, 2014
 *      Author: jan
 */

#ifndef _BLDC_SKID_DRIVER_H_
#define _BLDC_SKID_DRIVER_H_
#include "SkidDriverInterface.h"
#include "MotorBLDC.h"

//typedef enum
//{
//	WHEELS_AT_SPEED = 0, WHEELS_DIRTY, WHEELS_OVERLOADED, WHEELS_NUM_STATES
//} WHEELS_STATE;

class SkidDriverBLDC: public SkidDriverInterface
{

	private:

		MotorBLDC myLeftMotor;
		MotorBLDC myRightMotor;

	public:
		void plot();
		void motorOn();
		void motorOff();
		SkidDriverBLDC( uint8_t LeftForwardBackwardPin,uint8_t RightForwardBackwardPin, uint8_t LeftPWMPin,uint8_t RightPWMPin);
		void serialRegister(const __FlashStringHelper* Name, const __FlashStringHelper* LeftDriverName,  const __FlashStringHelper* RightDriverName);

		void setup();
		void loop();
		void setSpeedAndDirection(SPEED_TYPE Speed, DIRECTION_TYPE Direction);

		bool emergencyBreak();
		bool emergencyBreak( bool immediatlyReturn = false);
		void turn(DIRECTION_TYPE Corner);
		virtual void turnOnTheSpot(SPEED_TYPE Speed){};//TODO implement this method

		const __FlashStringHelper *          getOverloadedWheelName(){return F("no");};


		virtual SPEED_TYPE getActualSpeed() const {return (myLeftMotor.getActualSpeed()+myRightMotor.getActualSpeed())/2;};


	};

#endif /* _BLDC_SKID_DRIVER_H_ */
