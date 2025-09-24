/*
 * ITempMeter.h
 *
 *  Created on: May 12, 2015
 *      Author: jan
 */

#pragma once
#include "Arduino.h"

#ifdef I_USE_SERIAL_REGISTER
#include "SerialDataInterface.h"
#endif

class TempMeterInterface
	{

	protected:
		uint32_t myMilliCelsius=0;
		boolean myIsError=false;
		uint16_t myReadInterval =1000; //default read every second
	public:

#ifdef I_USE_SERIAL_REGISTER
		virtual void serialRegister(const __FlashStringHelper* Name)
        {
            FieldData::set(Name,F("MiliCelsius"),MOD_OVERVIEW,&myMilliCelsius);
            FieldData::set(Name,F("isError"),MOD_OVERVIEW,&myIsError);
            FieldData::set(Name,F("readEvery"),MOD_WRITE,&myReadInterval);
        }
#endif
		virtual void setup()=0;
		virtual void loop()=0;
		virtual void setReadInterval(uint16_t newReadInterval)
		{myReadInterval=newReadInterval;}
		virtual uint16_t getReadInterval(){
			return myReadInterval;
		}
		virtual ~TempMeterInterface()=default;
		int16_t getCelsius() const
			{
				return myMilliCelsius / 1000;
			}
		int16_t getCentiCelsius() const
			{
				return myMilliCelsius/10;
			}
		uint32_t getMiliCelsius() const
			{
				return myMilliCelsius;
			}
		bool isError()
			{
				return myIsError;
			}
	};

