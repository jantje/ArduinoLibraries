/*
 * ICurrentSensor.h
 *
 *  Created on: Jul 14, 2014
 *      Author: jan
 */

#ifndef ICURRENTSENSOR_H_
#define ICURRENTSENSOR_H_
#include "Arduino.h"

class CurrentSensorInterface
{
	protected:
		uint16_t myCentiAmp;  //current in centi amps
		uint16_t myAvgCentiAmp; //average current in centi amps
	public:
		CurrentSensorInterface(){		myCentiAmp=0;		myAvgCentiAmp=0;}
		virtual void loop()=0;
		virtual void setup()=0;
		uint16_t getCurrent_cA() {return myCentiAmp;}
		uint16_t getCurrent_dA() {return (myCentiAmp+5)/10;}
		uint16_t getCurrent_A() {return (myCentiAmp+50)/100;}
		uint16_t getAvgCurrent_cA() {return myAvgCentiAmp;}
		uint16_t getAvgCurrent_dA() {return (myAvgCentiAmp+5)/10;}
		uint16_t getAvgCurrent_A() {return (myAvgCentiAmp+50)/100;}
		virtual void setZeroAmp()=0;
};

#endif /* ICURRENTSENSOR_H_ */
