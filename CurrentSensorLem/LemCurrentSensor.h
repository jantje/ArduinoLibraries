/*
 * LemCurrentSensor.h
 *
 *  Created on: Jun 13, 2013
 *      Author: jan
 */

#ifndef LEMCURRENTSENSOR_H_
#define LEMCURRENTSENSOR_H_


#include "Arduino.h"
#include "ICurrentSensor.h"
class LemCurrentSensor:public ICurrentSensor
{
	private:
		int32_t myMultiplier;
		uint16_t myRefPinValue;
		uint16_t myCurrentPinValue;
		uint8_t myCurrentPin;
		uint8_t myRefPin;
		uint32_t myLoopduration;

	public:
		LemCurrentSensor( uint8_t CurrentPin, uint8_t refPin){	myCurrentPin=CurrentPin;
		myRefPin=refPin;
		myCurrentPinValue=0;
		myRefPinValue=0;
		myMultiplier=1;
		myLoopduration=0;
};
		void loop();
		void serialRegister(const __FlashStringHelper* Name);
		void setup(){	}
		void setZeroAmp(){};

};

#endif /* LEMCURRENTSENSOR_H_ */
