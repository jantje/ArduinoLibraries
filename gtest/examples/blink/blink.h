/*
 * blink.h
 *
 *  Created on: Nov 27, 2015
 *      Author: jan
 */

#ifndef BLINK_H_
#define BLINK_H_
#include "Arduino.h"
#define INTERVAL 500 //The blinking interval

uint8_t calcLedState(uint32_t currentMillis, uint8_t ledState);


#endif /* BLINK_H_ */
