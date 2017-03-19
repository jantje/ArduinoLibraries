/*
 * blink.cpp
 *
 *  Created on: Nov 27, 2015
 *      Author: jan
 */

#include "blink.h"
#include "Arduino.h"

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
	static uint8_t ledState = LOW;
	ledState = calcLedState(millis(), ledState);
	digitalWrite(LED_BUILTIN, ledState);
}

uint8_t calcLedState(uint32_t currentMillis, uint8_t ledState) {
	static uint32_t previousMillis = 0;
	if (currentMillis - previousMillis >= INTERVAL) {
		previousMillis = currentMillis;

		if (ledState == LOW)
			return HIGH;
		else
			return LOW;
	}
	return ledState;
}
