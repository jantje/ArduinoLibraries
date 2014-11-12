/*
 * BlinkLedSerial.cpp
 *
 *  Created on: Nov 7, 2014
 *      Author: jan
 */

#include <BlinkLedSerial.h>

void BlinkLedSerial::serialRegister(const __FlashStringHelper* Name)
{
	FieldData::set(Name, F("OnInterval"),MOD_WRITE|MOD_SAVE,&myOnInterval);
	FieldData::set(Name, F("OffInterval"),MOD_WRITE|MOD_SAVE,&myOffInterval);
	FieldData::set(Name, F("LedState"),0,&myLedState);
	FieldData::set(Name, F("PreviousMillis"),0,&myPreviousMillis);
	FieldData::set(Name, F("LedPin"),0,&myLedPin);

}
