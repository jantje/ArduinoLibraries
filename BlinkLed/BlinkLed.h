/*
 * BlinkLed.h
 *
 *  Created on: Sep 11, 2013
 *      Author: jan
 *      This is the blink without delay example put in a class
 *      I made the on interval different from the off interval
 */

#ifndef BLINKLED_H_
#define BLINKLED_H_
#include "Arduino.h"


class BlinkLed
{
	protected:
		// constants won't change. Used here to
		// set pin numbers:
		uint8_t myLedPin ;      // the number of the LED pin

		// Variables will change:
		uint8_t myLedState ;             // ledState used to set the LED
		uint32_t myPreviousMillis ;        // will store last time LED was updated

		// the follow variables is a long because the time, measured in miliseconds,
		// will quickly become a bigger number than can be stored in an int.
		uint32_t myOnInterval ;           // interval at which to blink (milliseconds)
		uint32_t myOffInterval ;           // interval at which to blink (milliseconds)
	public:
		BlinkLed(uint8_t ledPin,uint32_t onInterval ,uint32_t offInterval);
    void setOnInterval(uint32_t onInterval){myOnInterval=onInterval;};
    void setOffInterval(uint32_t offInterval){myOffInterval=offInterval;};
		void setup();
		void loop();
};

#endif /* BLINKLED_H_ */
