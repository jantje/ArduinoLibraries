/*
 * BlinkLed.cpp
 *
 *  Created on: Sep 11, 2013
 *      Author: jan
 */

#include "BlinkLed.h"

BlinkLed::BlinkLed(uint8_t ledPin,uint32_t onInterval ,uint32_t offInterval )
{

	myLedPin =  ledPin;      // the number of the LED pin
	myLedState = LOW;             // ledState used to set the LED
	myPreviousMillis = millis();        // will store last time LED was updated
	myOnInterval = onInterval;
	myOffInterval=offInterval;
}




void BlinkLed::setup() {
  // set the digital pin as output:
  pinMode(myLedPin, OUTPUT);
  digitalWrite(myLedPin, myLedState);
}

void BlinkLed::loop()
{
  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the
  // difference between the current time and last time you blinked
  // the LED is bigger than the interval at which you want to
  // blink the LED.
	uint32_t myInterval =myOnInterval;
  if (myLedState == LOW)
  {
  	myInterval =myOffInterval;
  }


  unsigned long currentMillis = millis();

  if(currentMillis - myPreviousMillis > myInterval) {
    // save the last time you blinked the LED
  	myPreviousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (myLedState == LOW)
    {
    	myLedState = HIGH;
    	myInterval =myOnInterval;
    }
    else
    {
    	myLedState = LOW;
    	myInterval =myOffInterval;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(myLedPin, myLedState);
  }
}


