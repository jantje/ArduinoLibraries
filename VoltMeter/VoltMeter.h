/*
 * VoltMeter.h
 *
 *  Created on: Mar 28, 2013
 *      Author: jan
 */

#ifndef VOLTMETER_H_
#define VOLTMETER_H_


#include "Arduino.h"
#include "SerialDataInterface.h"

class VoltMeter
{
	private:
		uint8_t myPin;
		uint16_t myCentiVolt;
		uint16_t myMultiplyerValue;
		uint16_t myActualReadValue;
		uint16_t myAveragedReadValue;
		//uint32_t myLoopduration;

	public:
		VoltMeter( uint8_t Pin);

		void setup()
		{
			//pinMode(myPin,INPUT); This is not needed
			loop(); //initialize the average
			myAveragedReadValue=myActualReadValue;
		}

		void loop();
		uint16_t getCentiVoltage() const	{	return myCentiVolt;}
		uint16_t get_dV() const	{	return (myCentiVolt+5)/10;}

		void serialRegister(const __FlashStringHelper* Name);
};

#endif /* VOLTMETER_H_ */
