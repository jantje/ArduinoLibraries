/*
 * TemperatureSensetiveResistorTempMeter.h
 *
 * Implementation of the temperature interface with a temperature sensitive resistor
 *
 *  Created on: Jun 13, 2013
 *      Author: jan
 */

#include "Arduino.h"
#include "TempMeterInterface.h"
#include "FastRunningMedian.h"
#ifdef I_USE_SERIAL_REGISTER
#include "SerialDataInterface.h"
#endif
extern uint32_t loopMillis;

class TempMeterTemperatureSensetiveResistor:public TempMeterInterface
{

	private:
		uint8_t myPin;
		int16_t myMultiplyerValue;
		uint16_t myActualReadValue;
//		uint16_t myAveragedReadValue;
		FastRunningMedian<uint16_t, 5, 0> myReadValues;
		int16_t myOffset;
		uint32_t myLastRead;
		boolean myIgnore;

	public:
		TempMeterTemperatureSensetiveResistor( uint8_t Pin);
		TempMeterTemperatureSensetiveResistor( uint8_t Pin,int16_t multiplyerValue,int16_t offset);
		void serialRegister(const __FlashStringHelper* Name);
		void setup();
		void loop();
};

#pragma once
