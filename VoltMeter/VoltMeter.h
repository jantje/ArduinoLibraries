/*
 * VoltMeter.h
 *
 *  Created on: Mar 28, 2013
 *      Author: jan
 */

#ifndef VOLTMETER_H_
#define VOLTMETER_H_


#include "Arduino.h"
#ifdef I_USE_SERIAL_COMMUNICATOR
#include "FieldData.h"
#endif
class VoltMeter
{
	private:
		uint8_t myPin;
		uint16_t myMilivolt;
		uint16_t myMultiplyerValue;
		uint16_t myActualReadValue;
		uint16_t myAveragedReadValue;
		uint32_t myLoopduration;

	public:
		VoltMeter( uint8_t Pin);

		void setup()
		{
			pinMode(myPin,INPUT);
			loop(); //initialize the average
			myAveragedReadValue=myActualReadValue;
		}

		void loop();
		uint16_t getVoltage() const	{	return myMilivolt;}

#ifdef I_USE_SERIAL_COMMUNICATOR
		void serialRegister(const __FlashStringHelper* Name);
#endif
};

#endif /* VOLTMETER_H_ */
