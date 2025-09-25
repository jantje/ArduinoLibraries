/*
 * AmpMeter.cpp
 *
 *  Created on: Jul 5, 2014
 *      Author: jan
 */

#include <CurrentSensorAnalog.h>

CurrentSensorAnalog::CurrentSensorAnalog(uint8_t analogPin)
	{
		myAnalogPin = analogPin;
		myMultiplyerValue = 0;
		myCurrentPinValue = 0;
		myOffset = 505;
	}

CurrentSensorAnalog::CurrentSensorAnalog(uint8_t analogPin, int16_t MultiplyerValue, int16_t Offset)
	{
		myAnalogPin = analogPin;
		myMultiplyerValue = MultiplyerValue;
		myCurrentPinValue = 0;
		myOffset = Offset;
	}

void CurrentSensorAnalog::loop(void)
	{
#ifndef USE_MAIN_LOOP_MILLIS
    uint32_t loopMillis = millis();
#endif
		uint32_t myLastRead = millis();
		if (myAnalogPin != 255)
			{
				if ((loopMillis - myLastRead) > 50)
					{
						myLastRead=loopMillis;
						myCurrentPinValue = (analogRead(myAnalogPin) / 5) * 5;
						myMedianPinValue.addValue(myCurrentPinValue);
						int32_t tmpCurrentPinValue = myCurrentPinValue;
						int32_t tmpOffset = myOffset;
						int32_t tmpMultiplyerValue = myMultiplyerValue;
						int32_t tmpDevisor = 1000;
						int32_t tmpCentiAmp = myCentiAmp;
						tmpCentiAmp = abs(((tmpCurrentPinValue - tmpOffset) * tmpMultiplyerValue) / tmpDevisor);
						myCentiAmp = abs(tmpCentiAmp);
						tmpCurrentPinValue = myMedianPinValue.getMedian();
						tmpCentiAmp = abs(((tmpCurrentPinValue - tmpOffset) * tmpMultiplyerValue) / tmpDevisor);
						myAvgCentiAmp = tmpCentiAmp;
					}
			}
	}

void CurrentSensorAnalog::serialRegister(const __FlashStringHelper* Name)
	{
		FieldData::set(Name, F("Current_cA"), MOD_NONE, &myCentiAmp);
#ifdef DETAILED_SERIAL_LOGGING
	FieldData::set(Name, F("Avg_cA"),MOD_NONE,&myAvgCentiAmp);
	FieldData::setNext( F("PinValue"),MOD_NONE,&myCurrentPinValue);
#endif
		FieldData::setNext(F("MultiplyerValue_1000"), MOD_WRITE | MOD_SAVE, &myMultiplyerValue);
		FieldData::setNext(F("PinValue_Offset"), MOD_NONE, &myOffset);
#ifdef DETAILED_SERIAL_LOGGING
	FieldData::setNext( F("Pin"),MOD_NONE,&myAnalogPin);
#endif
	}

void CurrentSensorAnalog::setZeroAmp()
	{
		// myOffset = myMedianPinValue.getMedian();
	}
