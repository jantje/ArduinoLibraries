#include <TempMeterDS18B20.h>
//Enable the line below to dump debug info to Serial
//#define DEBUG

#ifdef USE_MAIN_LOOP_MILLIS
extern uint32_t loopMillis;
#else
#ifdef MAX_MILLIS_IN_LOOP_TO_START
#error "MAX_MILLIS_IN_LOOP_TO_START is defined but USE_MAIN_LOOP_MILLIS is not"
#else
#warning "TempMeterDS18B20 is using it's own loop millis"
#endif
#endif

TempMeterDS18B20::TempMeterDS18B20(OneWire &wire, DeviceAddress sensorAddress) :
		mySensors(&wire) {
	for (int i = 0; i < 8; i++)
		mySensorAddress[i] = sensorAddress[i];
}

TempMeterDS18B20::TempMeterDS18B20(OneWire &wire, int sensorIndex) :
		mySensors(&wire) {
	myIndex = sensorIndex;

}

#ifdef I_USE_SERIAL_REGISTER
void TempMeterDS18B20::serialRegister(const __FlashStringHelper* Name)
	{
		TempMeterInterface::serialRegister(Name);
		FieldData::set(Name, F("Ignore"), MOD_WRITE, &myIgnore);
	}
#endif

void TempMeterDS18B20::setup() {
	if(mySensors.getDeviceCount()==0){
		mySensors.begin();
	}
	if (myIndex >= 0) {
		mySensors.getAddress(mySensorAddress, myIndex);
	}
	mySensors.setResolution(mySensorAddress, 10);
	mySensors.setWaitForConversion(false);
	mySensors.requestTemperaturesByAddress(mySensorAddress);
	myLastAction = millis();
}

void TempMeterDS18B20::loop() {
#ifndef USE_MAIN_LOOP_MILLIS
    uint32_t loopMillis = millis();
#endif
	if (myIgnore == true) {
		myMilliCelsius = 20000;
		myIsError = false;
		myValueRequested = false;
#ifdef DEBUG
		Serial.print("IgnoreFlag set");
#endif
		return;
	}
	/*
	 * MAX_MILLIS_IN_LOOP_TO_START allows to ignore the temp meter if there
	 *  have been to many miliseconds passed since the main loop has started.
	 *  To use it you need to define USE_MAIN_LOOP_MILLIS (no value needed
	 *  and define MAX_MILLIS_IN_LOOP_TO_START with the number of miliseconds
	 */
#ifdef MAX_MILLIS_IN_LOOP_TO_START
		if (millis() - loopMillis > MAX_MILLIS_IN_LOOP_TO_START)
			{
				//this is low priority; someone already did something in this loop call
				//be nice and wait
				return;
			}
#endif

	if (myValueRequested) {
		if (loopMillis - myLastAction > myReadResponseTime) {
			myValueRequested = false;
#ifdef DEBUG
			uint32_t startTime = millis();
#endif
			//Use address as that is faster
			float temp = mySensors.getTempC(mySensorAddress);
//		float temp = mySensors.getTempCByIndex(0);
#ifdef DEBUG
			uint32_t prefTime = millis();
			Serial.print("getTempCByIndex took ");
			Serial.println(prefTime - startTime);
#endif
			if (temp == DEVICE_DISCONNECTED_C) {
				myIsError = true;
				myMilliCelsius = 111111;
			} else {
				myMilliCelsius = temp * 1000;
				myIsError = false;
			}
#ifdef DEBUG
			if (myIsError) {
				Serial.print("A error during temp reading occured");
			}
			Serial.print("Temp in centi celcius is ");
			Serial.println(getCentiCelsius());

			Serial.print("calculated delay time ");
			Serial.println(myReadResponseTime);

			Serial.print("Reading temp sensor took ");
			Serial.println(millis() - startTime);
#endif
		}
		return;
	}
	if (loopMillis - myLastAction < myReadInterval) {
		//not yet time to read
//		Serial.print("loopMillis: ");
//		Serial.print(loopMillis);
//		Serial.print(" last Action: ");
//		Serial.print(myLastAction);
//
//		Serial.print(" myReadInterval: ");
//		Serial.println(myReadInterval);

		return;
	}

	myLastAction = loopMillis;
#ifdef DEBUG
	uint32_t startTime = millis();
#endif
	mySensors.requestTemperaturesByAddress(mySensorAddress); // Send the command to get temperature readings
	//mySensors.requestTemperaturesByIndex(0); // Send the command to get temperature readings
	myValueRequested = true;
#ifdef DEBUG
	Serial.print("requestTemperaturesByIndex took ");
	Serial.println(millis() - startTime);
#endif
}

