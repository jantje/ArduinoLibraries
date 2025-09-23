/*
 * BlinkLed.cpp
 *
 *  Created on: Sep 11, 2013
 *      Author: jan
 */

#include "BlinkLed.h"

BlinkLed::BlinkLed(uint8_t ledPin, uint16_t onInterval, uint16_t offInterval) {

    myLedPin = ledPin;      // the number of the LED pin
    myOnInterval = onInterval;
    myOffInterval = offInterval;
#ifdef SOFTPWM
    myPwmValue=100;
#endif
}

void BlinkLed::setup() {
    // set the digital pin as output:
    pinMode(myLedPin, OUTPUT);

#ifdef SOFTPWM
    SoftPWMSet(myLedPin, 0);
    SoftPWMSetFadeTime(myLedPin, 1000, 1000);
#else
    digitalWrite(myLedPin, LOW);
#endif
}

void BlinkLed::loop() {
#ifdef LOOPMILLIS
    extern uint32_t LOOPMILLIS;
#else
#define LOOPMILLIS curMillis
    uint32_t LOOPMILLIS = millis();
#endif
    uint8_t newPinState = myPrefPinState;
    switch (myLedState) {
        case blinking: {
            //this is a expensive calculation
            if (LOOPMILLIS - myPrefLoopMillis > 10) {
                myPrefLoopMillis = LOOPMILLIS;
                if ((LOOPMILLIS % (myOnInterval + myOffInterval)) >= myOffInterval) {
                    newPinState = HIGH;
                } else {
                    newPinState = LOW;
                }
            } else {
                return;
            }
            break;
        }
        case on:
            newPinState = HIGH;
            break;
        case off:
            newPinState = LOW;
    }
    if (myPrefPinState != newPinState) {
#ifdef SOFTPWM
        SoftPWMSet(myLedPin, newPinState*myPwmValue);
#else
        //Serial.println("led changed state");
        digitalWrite(myLedPin, newPinState);
#endif
        myPrefPinState = newPinState;
    }
}

