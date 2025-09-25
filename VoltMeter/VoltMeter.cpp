/*
 * VoltMeter.cpp
 *
 *  Created on: Mar 28, 2013
 *      Author: jan
 */

#include "VoltMeter.h"

#ifdef USE_SERIAl_LOGGING
void VoltMeter::serialRegister(const __FlashStringHelper* Name)
{
    FieldData::set(Name,F("CentiVolt"),0,&myCentiVolt);
    FieldData::setNext((__FlashStringHelper *)MULTIPLIER_DIV_1000,MOD_WRITE|MOD_SAVE,&myMultiplyerValue);

#ifdef DETAILED_SERIAL_LOGGING
    FieldData::setNext(F("Value"),0,&myActualReadValue);
    FieldData::setNext(F("AvgValue"),0,&myAveragedReadValue);
    FieldData::setNext((__FlashStringHelper *)PIN,0,&myPin);
    //          FieldData::setNext( (__FlashStringHelper *)LOOPDURATION,0,&myLoopduration);
#endif

}
VoltMeter::VoltMeter(uint8_t Pin) {
    myPin = Pin;
    myCentiVolt = 0;
    myMultiplyerValue = 1000;
    myActualReadValue = 0;
    myAveragedReadValue = 0;
    my_last_read=0;
}
#else
VoltMeter::VoltMeter(uint8_t Pin, uint16_t multiplyer) {
    myPin = Pin;
    myCentiVolt = 0;
    myMultiplyerValue = multiplyer;
    myActualReadValue = 0;
    myAveragedReadValue = 0;
}
#endif

#ifdef PRECISE_VOLTAGE
long readVcc() {
    long result;
    // Read 1.1V reference against AVcc
    //ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
    ADCSRB &= ~_BV(MUX5);
    delay(2);// Wait for Vref to settle
    ADCSRA |= _BV(ADSC);// Convert
    while (bit_is_set(ADCSRA,ADSC));
    result = ADCL;
    result |= ADCH<<8;
    result = 1125300L / result;// Back-calculate AVcc in mV
    return result;
}
#endif //PRECISE_VOLTAGE

void VoltMeter::loop() {
#ifndef USE_MAIN_LOOP_MILLIS
    uint32_t loopMillis = millis();
#endif
    //only reed every 100 ms
    if (loopMillis - my_last_read > 100) {
        my_last_read = loopMillis;
#ifdef PRECISE_VOLTAGE
        long vcc=readVcc();
        myActualReadValue= (analogRead(myPin) *vcc) / 1023;
        //as we do precise reading there is no need to average
        myAveragedReadValue=myActualReadValue;
#else
        myActualReadValue = analogRead(myPin);
        //As we do not read precisely we average the result
        myAveragedReadValue = (myAveragedReadValue * 8 + (2 * myActualReadValue)) / 10;
#endif

        myCentiVolt = (uint16_t)(((uint32_t) myAveragedReadValue * (uint32_t) myMultiplyerValue) / 1000UL);
    }
}

