/*
 * TeensyReboot.h
 *
 *  Created on: 1 May 2026
 *      Author: jan
 */

#pragma once
#include "Arduino.h"

class TeensyReboot {
public:
	void setup();
	void loop();
	TeensyReboot(uint8_t rebootPin, uint16_t delayForUSB, uint8_t pinMode);
	virtual ~TeensyReboot();

private:
	uint8_t myRebootPin;
	uint16_t myDelayForUSB;
	uint8_t myPinMode;
	boolean myPinHasBeenFalse=false;
};

