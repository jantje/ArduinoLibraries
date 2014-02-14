/*
 * VoltMeter.cpp
 *
 *  Created on: Mar 28, 2013
 *      Author: jan
 */

#include "VoltMeter.h"

VoltMeter::VoltMeter(uint8_t Pin)
{
	myPin=Pin;
	myMilivolt=0;
	myMultiplyerValue=8333;
	myActualReadValue=0;
	myAveragedReadValue=0;
	myLoopduration=0;


}


//void VoltMeter::setup()
//{
//	pinMode(myPin,INPUT);
//	loop(); //initialize the average
//	myAveragedReadValue=myActualReadValue;
//}

#ifdef PRECISE_VOLTAGE
long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  //ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  ADCSRB &= ~_BV(MUX5) ;
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1126400L / result; // Back-calculate AVcc in mV
  return result;
}
#endif //PRECISE_VOLTAGE

void VoltMeter::loop()
{
	unsigned long loopstart = millis();
#ifdef PRECISE_VOLTAGE
	long vcc=readVcc();
	myActualReadValue= (analogRead(myPin) *vcc) / 1023;
	//as we do precise reading there is no need to average
	myAveragedReadValue=myActualReadValue;
#else
	myActualReadValue= ((uint32_t) analogRead(myPin) * 3000ul) / 1023ul;
	//As we do not read precisely we average the result
	myAveragedReadValue=(myAveragedReadValue*9+myActualReadValue)/10;
#endif

	myMilivolt=(uint16_t)(((uint32_t)myAveragedReadValue*(uint32_t)myMultiplyerValue)/5000UL);
	myLoopduration = millis() - loopstart;
}

#ifdef I_USE_SERIAL_COMMUNICATOR
		void VoltMeter::serialRegister(const __FlashStringHelper* Name)
		{
			FieldData::set(Name,F("MiliVolt"),0,&myMilivolt);
			FieldData::setNext(F("Value"),0,&myActualReadValue);
			FieldData::setNext(F("AvgValue"),0,&myAveragedReadValue);
			FieldData::setNext((__FlashStringHelper *)MULTIPLIER,MOD_WRITE|MOD_SAVE,&myMultiplyerValue);
			FieldData::setNext((__FlashStringHelper *)PIN,0,&myPin);
			FieldData::setNext( (__FlashStringHelper *)LOOPDURATION,0,&myLoopduration);
		}
#endif
