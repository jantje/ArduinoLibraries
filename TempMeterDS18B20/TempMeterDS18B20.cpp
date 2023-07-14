#include <TempMeterDS18B20.h>
#ifndef MAX_MILLIS_IN_LOOP_TO_START
#define MAX_MILLIS_IN_LOOP_TO_START 5
#endif
TempMeterDS18B20::TempMeterDS18B20(uint8_t pin)
				:
				mySensors(new OneWire(pin))
	{
		myCentiCelsius = 0;
	}

#ifdef I_USE_SERIAL_REGISTER
void TempMeterDS18B20::serialRegister(const __FlashStringHelper* Name)
	{
		TempMeterInterface::serialRegister(Name);
		FieldData::set(Name, F("Ignore"), MOD_WRITE, &myIgnore);
	}
#endif

void TempMeterDS18B20::setup()
	{
		mySensors.begin();
		mySensors.setWaitForConversion(false);
		mySensors.requestTemperatures();
		myLastAction = millis();
	}

void TempMeterDS18B20::loop()
	{
		if (myIgnore == true)
			{
				myCentiCelsius = 2000;
				myIsError = false;
				return;
			}
		if (loopMillis - myLastAction < myReadInterval)
			{
				//not yet time to read
				return;
			}
		if (millis() - loopMillis > MAX_MILLIS_IN_LOOP_TO_START)
			{
				//this is low priority; someone already did something in this loop call
				//be nice and wait
				return;
			}
		myLastAction = loopMillis;
		//uint32_t startTime = millis();

		float temp = mySensors.getTempCByIndex(0);
		mySensors.requestTemperatures(); // Send the command to get temperature readings
		if (temp == DEVICE_DISCONNECTED_C)
			{
				myIsError = true;
				myCentiCelsius = 10000;
			} else
			{
				myCentiCelsius = temp * 100;
				myIsError = false;
			}

		// don't write to serial as that will fill up the serial buffer and stop logging
		//Serial.print("Reading temp sensor took ");
		//	Serial.println(millis() - startTime);
	}

