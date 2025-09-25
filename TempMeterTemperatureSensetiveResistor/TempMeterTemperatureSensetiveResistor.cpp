/*
 * TempMeter.cpp
 *
 *  Created on: Jun 13, 2013
 *      Author: jan
 */

#include <TempMeterTemperatureSensetiveResistor.h>

TempMeterTemperatureSensetiveResistor::TempMeterTemperatureSensetiveResistor(
		uint8_t Pin) {
	myPin = Pin;
	myMilliCelsius = 0;
	myMultiplyerValue = 1000;
	myActualReadValue = 0;
	myOffset = 0;
	myLastRead = 0;
	myIgnore = false;
}

TempMeterTemperatureSensetiveResistor::TempMeterTemperatureSensetiveResistor(uint8_t Pin, int16_t multiplyerValue, int16_t offset)
	{
		myPin = Pin;
		myMilliCelsius = 0;
		myMultiplyerValue = multiplyerValue;
		myActualReadValue = 0;
		myOffset = offset;
		myLastRead = 0;
		myIgnore = false;
	}


#ifdef I_USE_SERIAL_REGISTER
#define DETAILED_SERIAL_LOGGING
void TempMeterTemperatureSensetiveResistor::serialRegister(
		const __FlashStringHelper* Name)
{
	if(myPin<NUM_DIGITAL_PINS)
	{
		TempMeterInterface::serialRegister(Name);
		FieldData::set(Name,FP(MULTIPLIER_DIV_10),MOD_WRITE|MOD_SAVE,&myMultiplyerValue);
		FieldData::set(Name,F("Offset"),MOD_WRITE|MOD_SAVE,&myOffset);
		FieldData::set(Name,F("Ignore"),MOD_WRITE,&myIgnore);

#ifdef DETAILED_SERIAL_LOGGING
		FieldData::set(Name,F("ReadValue"),0,&myActualReadValue);
		FieldData::set(Name,(const __FlashStringHelper*)PIN,0,&myPin);
#endif
	}

}
#endif

void TempMeterTemperatureSensetiveResistor::setup() {
	if (myPin < NUM_DIGITAL_PINS) {
		pinMode(myPin, INPUT);
		myActualReadValue = analogRead(myPin);
		for (int i = 0; i < 4; i++) {
			myReadValues.addValue(myActualReadValue);
		}
		loop();
	}
}
int16_t convertLinearADReadToCentiCelsius(int32_t inputValue,int32_t multiplyerValue,int32_t offset){
    //for speed purposes I force this to be a non-float calculation
    return (int16_t)( (inputValue * multiplyerValue) / 10L + offset);

}
int16_t convertLogarithmicADReadToCentiCelsius(int32_t inputValue,int32_t multiplyerValue,int32_t offset){
    if(inputValue==0){
        //log will return error with 0
        inputValue++;
    }
    inputValue=log(inputValue)*1000;
    //for speed purposes I force this to be a non-float calculation
    return (int16_t)( (inputValue * multiplyerValue) / 10000L + offset);

}
#define USE_LN
void TempMeterTemperatureSensetiveResistor::loop() {
#ifndef USE_MAIN_LOOP_MILLIS
    uint32_t loopMillis = millis();
#endif
	if (myPin < NUM_DIGITAL_PINS) {
		if (loopMillis - myLastRead > 1000) {
			myLastRead = loopMillis;
			myActualReadValue = analogRead(myPin);
			myReadValues.addValue(myActualReadValue);
			int32_t inputValue= myReadValues.getMedian();

#ifdef USE_LN
			//logarithmic scale works better for heat sensitive resistors
			myMilliCelsius=convertLogarithmicADReadToCentiCelsius(inputValue,myMultiplyerValue,myOffset)*10;

#else
            myCentiCelsius=convertLinearADReadToCentiCelsius(inputValue,myMultiplyerValue,myOffset);
#endif

			myIsError = (myActualReadValue < 5 || myActualReadValue > 1020);
			if (myIgnore) {
				myMilliCelsius = 20000;
				myIsError = false;
			}
		}
	}
	}

