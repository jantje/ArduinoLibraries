/*
 * BlinkLedSerial.h
 *
 * This is the extension neededto get blinkled to be controlled by the serial comunication module
 *
 *  Created on: Nov 7, 2014
 *      Author: jan
 */

#ifndef LIBRARIES_BLINKLED_BLINKLEDSERIAL_H_
#define LIBRARIES_BLINKLED_BLINKLEDSERIAL_H_

#include <BlinkLed.h>
#include "SerialDataInterface.h"

class BlinkLedSerial: public BlinkLed
{
	public:
		BlinkLedSerial(uint8_t ledPin,uint32_t onInterval ,uint32_t offInterval):
			BlinkLed( ledPin, onInterval , offInterval){};
		void serialRegister(const __FlashStringHelper* Name);
};

#endif /* LIBRARIES_BLINKLED_BLINKLEDSERIAL_H_ */
