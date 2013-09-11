/*
 * MyBlinkLed.cpp
 *
 *  Created on: Sep 11, 2013
 *      Author: jan
 */

#include "MyBlinkLed.h"

MyBlinkLed::MyBlinkLed(const char* name, uint8_t ledPin,uint32_t onInterval ,uint32_t offInterval)
:BlinkLed( ledPin, onInterval ,offInterval)
{

	myDataFields[0].set(name, F("myOnInterval"),MOD_WRITE|MOD_SAVE,&myOnInterval);
	myDataFields[1].set(myDataFields[0].myClassName, F("myOffInterval"),MOD_WRITE|MOD_SAVE,&myOffInterval);
	myDataFields[2].set(myDataFields[0].myClassName, F("myLedState"),0,&myLedState);
	myDataFields[3].set(myDataFields[0].myClassName, F("myPreviousMillis"),0,&myPreviousMillis);
	myDataFields[4].set(myDataFields[0].myClassName, F("myLedPin"),0,&myLedPin);

	SerialDataInterface::myFields = myDataFields;
	myNumFields = NUMFields;
	myNumChildren = NUMChildren;
	myChildren = myDataChildren;
}

