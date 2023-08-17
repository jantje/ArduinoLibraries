/*
 * BlinkLed.h
 *
 *  Created on: Sep 11, 2013
 *      Author: jan
 *      This is the blink without delay example put in a class
 *      I made the on interval different from the off interval
 */

#pragma once

#include "Arduino.h"
#ifdef SOFTPWM
#include "SoftPWM.h"
#else
#warning If the leds are to bright define SOFTPWM
#endif


class BlinkLed
{
    public:
        enum ledState{blinking,on,off};
        BlinkLed(uint8_t ledPin,uint16_t onInterval ,uint16_t offInterval);
    void setOnInterval(uint16_t onInterval){myOnInterval=onInterval;};
    void setOffInterval(uint16_t offInterval){myOffInterval=offInterval;};
        void setup();
        void loop();
        ledState getLedState(){return myLedState;};
        void setLedState(ledState newLedState){myLedState=newLedState;};
	protected:
		// constants won't change. Used here to
		// set pin numbers:
		uint8_t myLedPin ;      // the number of the LED pin
		ledState myLedState=blinking;

		// the follow variables is a long because the time, measured in miliseconds,
		// will quickly become a bigger number than can be stored in an int.
		uint16_t myOnInterval ;           // interval at which to blink (milliseconds)
		uint16_t myOffInterval ;           // interval at which to blink (milliseconds)
		uint8_t myPrefPinState=LOW;
		uint32_t myPrefLoopMillis=0;
#ifdef SOFTPWM
		//the usage of softpwm allows to coorect the brightness of the leds
	public:
		void setPwmValue(uint8_t newPwmValue){myPwmValue=newPwmValue;};
	protected:
		uint8_t myPwmValue;
#endif

};

