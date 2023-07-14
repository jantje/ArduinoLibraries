/*
 * LemCurrentSensor.cpp
 *
 *  Created on: Jun 13, 2013
 *      Author: jan
 */

#include "LemCurrentSensor.h"
#include "SerialDataInterface.h"


int myAnalogRead(uint8_t pin)
{
	analogRead(pin);
	//delay(1);
	return analogRead(pin);
}
void LemCurrentSensor::loop()
{
	unsigned long loopstart = millis();
	myCurrentPinValue=myAnalogRead(myCurrentPin);
	myRefPinValue=myAnalogRead(myRefPin);
	myCentiAmp=abs((((int32_t)myCurrentPinValue-(int32_t)myRefPinValue)*myMultiplier)/100);
	myAvgCentiAmp=((uint32_t)myAvgCentiAmp*8ul+(uint32_t)myCentiAmp*2ul)/10ul;
	myLoopduration = millis() - loopstart;
}

void LemCurrentSensor::serialRegister(const __FlashStringHelper* Name)
{

	FieldData::set(Name, F("Cur_cA"),0,&myCentiAmp);
	FieldData::set(Name, F("avg_cA"),0,&myAvgCentiAmp);
	FieldData::setNext( (__FlashStringHelper *)PINVALUE,0,&myCurrentPinValue);
	FieldData::setNext( F("RefPinValue"),0,&myRefPinValue);
	FieldData::setNext( (__FlashStringHelper *)PIN,(uint8_t)0,&myCurrentPin);
	FieldData::set(Name, F("RefPin"),0,&myRefPin);
	FieldData::set(Name, (__FlashStringHelper *)MULTIPLIER,MOD_SAVE|MOD_WRITE,&myMultiplier);
	FieldData::setNext( (__FlashStringHelper *)LOOPDURATION,0,&myLoopduration);
}



