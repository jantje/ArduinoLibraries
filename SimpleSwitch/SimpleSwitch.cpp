/*
 * SimpleSwitch.cpp
 *
 *  Created on: 29 sep. 2022
 *      Author: jan
 */

#include "SimpleSwitch.h"

SimpleSwitch::SimpleSwitch(uint8_t pin, uint8_t closedState)
	{
		myPin=pin;
		myClosedState=closedState;
		myIsClosed=false;
		last_Switchread=0;
	}

void SimpleSwitch::setup()
	{
		pinMode(myPin, INPUT_PULLUP);
	}

void SimpleSwitch::loop()
	{
		if (loopMillis - last_Switchread > 15)
			{
				last_Switchread = loopMillis;
				myReads.addValue(digitalRead(myPin) == myClosedState);
				myIsClosed=myReads.getMedian();
			}
	}
