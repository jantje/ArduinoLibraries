/*
 * VoltMeter.h
 *
 *  Created on: Mar 28, 2013
 *      Author: jan
 */

#pragma once


#include "Arduino.h"
#ifdef USE_SERIAl_LOGGING
#include "SerialDataInterface.h"
#endif

class VoltMeter
{
	private:
		uint8_t myPin;
		uint16_t myCentiVolt;
		uint16_t myMultiplyerValue;
		uint16_t myActualReadValue;
		uint16_t myAveragedReadValue;
		uint32_t my_last_read;


	public:
#ifdef USE_SERIAl_LOGGING
		void serialRegister(const __FlashStringHelper* Name);
		VoltMeter( uint8_t Pin);
#else
		/*
		 * Constructor class if you do not use the serial logger.
		 * This class allows you to initialize some constants that are otherwise
		 * set via serial communication and remembered in FI eeprom
		 * multiplyer is used to get from the adc cnverter (1 to 1023) to centivolt as follows
		 * centivolt=adcvalue*multiplyer/1000
		 * Assume you have a 10volt source and you uses a 50/50 deviding bridge on a uno
		 * this means that 10 volt on source is 5 volt on the pin or 1023 adcvalue
		 * the formula is the
		 * 1000(10Volt=1000centivolt)=1023*multiplyer/1000
		 * this mean multiplyer=1000*1000/1023=978
		 */
		VoltMeter(uint8_t Pin,uint16_t multiplyer);
#endif

		void setup()
		{
			pinMode(myPin,INPUT); // This may not be needed
			myAveragedReadValue=myActualReadValue= analogRead(myPin);
		}

		void loop();
		uint16_t getCentiVoltage() const	{	return myCentiVolt;}
		uint16_t get_dV() const	{	return (myCentiVolt+5)/10;}

};

