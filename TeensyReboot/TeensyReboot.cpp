/*
 * TeensyReboot.cpp
 *
 *  Created on: 1 May 2026
 *      Author: jan
 */

#include "TeensyReboot.h"

void TeensyReboot::setup() {
	pinMode(myRebootPin, myPinMode);
}

void TeensyReboot::loop() {
	if (digitalRead(myRebootPin) == HIGH) {
		//only reboot if the pin has changed value
		if (myPinHasBeenFalse) {
			digitalWrite(LED_BUILTIN, HIGH);
			// do reboot
			USB1_USBCMD = 0; // disconnect USB
			delay(myDelayForUSB); // enough time for USB hubs/ports to detect disconnect
			digitalWrite(LED_BUILTIN, LOW);
			SCB_AIRCR = 0x05FA0004; //do reboot
		}
	} else {
		myPinHasBeenFalse = true;
	}
}

TeensyReboot::TeensyReboot(uint8_t rebootPin, uint16_t delayForUSB,
		uint8_t pinMode) {
	myRebootPin = rebootPin;
	myDelayForUSB = delayForUSB;
	myPinMode = pinMode;
}

TeensyReboot::~TeensyReboot() {
	// TODO Auto-generated destructor stub
}

