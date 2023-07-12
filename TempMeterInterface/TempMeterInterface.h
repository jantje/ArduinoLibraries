/*
 * ITempMeter.h
 *
 *  Created on: May 12, 2015
 *      Author: jan
 */

#ifndef LIBRARIES_TEMPMETER_ITEMPMETER_H_
#define LIBRARIES_TEMPMETER_ITEMPMETER_H_
#include "Arduino.h"

#define ERROR_CENTI_CELSIUS (-9999)
#ifdef I_USE_SERIAL_REGISTER
#include "SerialDataInterface.h"
#endif

class TempMeterInterface
	{

	protected:
		int16_t myCentiCelsius;
		boolean myIsError;
	public:

#ifdef I_USE_SERIAL_REGISTER
        void serialRegister(const __FlashStringHelper* Name)
        {
            FieldData::set(Name,F("CentiCelsius"),MOD_OVERVIEW,&myCentiCelsius);
        }
#endif
		virtual void setup()=0;
		virtual void loop()=0;
		int16_t getCelsius() const
			{
				return myCentiCelsius / 100;
			}
		int16_t getCentiCelsius() const
			{
				return myCentiCelsius;
			}
		bool isError()
			{
				return myIsError;
			}
	};

#endif /* LIBRARIES_TEMPMETER_ITEMPMETER_H_ */
