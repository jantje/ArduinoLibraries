/*
 * AmpMeter.h
 *
 *  Created on: Jul 5, 2014
 *      Author: jan
 */

#ifndef AMPMETER_H_
#define AMPMETER_H_
#include "SerialDataInterface.h"
#include "CurrentSensorInterface.h"
#include "FastRunningMedian.h"
extern uint32_t loopMillis;

class CurrentSensorAnalog:public CurrentSensorInterface
{
	private :
		uint8_t myAnalogPin;
		int32_t myMultiplyerValue;
		int16_t myOffset;
		uint16_t myCurrentPinValue;
		FastRunningMedian<uint16_t, 5, 505> myMedianPinValue;
	public:
		/**
		 * Creates the ampmeter. You need to use serialRegister and Jan baeyens his
		 * communication system to get the variables initialized.
		 */
		CurrentSensorAnalog(uint8_t analogPin);
		/*
		 * Creates and Initializes the amp meter.
		 */
		CurrentSensorAnalog(uint8_t analogPin,int16_t MultiplyerValue,int16_t Offset);
		virtual void setup(void){;};
		virtual void loop(void);
		void serialRegister(const __FlashStringHelper* Name);
		void setZeroAmp();
};

#endif /* AMPMETER_H_ */
