/*
 * TempMeterDS18B20.h
 *
 * Implementation of the temperature interface with a DS18B20
 * This class assumes that there is only 1 sensor on the one wire
 * This is acceptable to me because I want to identify the sensor based on the pin
 * not on the ID of the sensor
 *
 *  Created on: Jul 13, 2023
 *      Author: jan
 */

#include "Arduino.h"
#include "TempMeterInterface.h"
#include "DallasTemperature.h"
#ifdef I_USE_SERIAL_REGISTER
#include "SerialDataInterface.h"
#endif
extern uint32_t loopMillis;

class TempMeterDS18B20:public TempMeterInterface
{

	private:
		uint16_t myReadInterval=1000;
		boolean myIgnore=false;

    uint32_t myLastAction=0;
	public:
    //I have made the DallasTemperature public so the programmer has full access to its functionality
    DallasTemperature mySensors;

	public:

		TempMeterDS18B20( uint8_t Pin);

		void setup();
		void loop();
#ifdef I_USE_SERIAL_REGISTER
		void serialRegister(const __FlashStringHelper* Name);
#endif
};

#pragma once
