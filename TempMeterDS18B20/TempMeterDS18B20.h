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
#include "OneWire.h"
#ifdef I_USE_SERIAL_REGISTER
#include "SerialDataInterface.h"
#endif
extern uint32_t loopMillis;

class TempMeterDS18B20: public TempMeterInterface {

private:

	uint16_t myReadResponseTime = calculateReadResponseTime(10);
	int8_t myIndex=-1; //using negative value to know index is not used
	int8_t myResolution=10; //The bit resolution used by the sensor (9->12)
	boolean myIgnore = false;
	boolean myValueRequested = true;
	DeviceAddress mySensorAddress;

	uint32_t myLastAction = 0;
public:
	//I have made the DallasTemperature public so the programmer can have access to its functionality
	DallasTemperature mySensors;

public:

	TempMeterDS18B20(OneWire &wire, DeviceAddress sensorAddress);
	TempMeterDS18B20(OneWire &wire, int sensorIndex);
	virtual ~TempMeterDS18B20() = default;
	void setResolution(int8_t newResolution){
		if (newResolution>8 && newResolution<13 ){
			myResolution=newResolution;
			myReadResponseTime=calculateReadResponseTime(myResolution);
		}
	}

	void setup();
	void loop();
	//For the calculation below I use resolution 10 and the formula provided
		//by robtillaart (thanks robtillaart)
		//https://forum.arduino.cc/t/dallas-temperature-libary-non-blocking-question/153430/2
	static uint16_t calculateReadResponseTime(uint8_t bitResolution){
		uint16_t correctResolution=10;
		if (bitResolution>8 && bitResolution<13 ){
			correctResolution=bitResolution;
		}
		return 750 / (1 << (12 - bitResolution));
	}
#ifdef I_USE_SERIAL_REGISTER
	void serialRegister(const __FlashStringHelper* Name);
#endif
};

#pragma once
