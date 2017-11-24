/*
 * makerfairRome2017Demo.h
 *
 *  Created on: 24 nov. 2017
 *      Author: jan
 */

#ifndef MAKERFAIRROME2017DEMO_H_
#define MAKERFAIRROME2017DEMO_H_


#include "Arduino.h"
#include "simplot.h"
int getBlinkLedValue(const uint32_t currentMillis, const uint32_t onInterval, 		const uint32_t offInterval) ;
int getFadeLedValue(const uint32_t currentMillis, const uint32_t onInterval, 		const uint32_t offInterval) ;
uint32_t numTriggers(const uint32_t currentMillis,const uint32_t triggerInterval) ;


#endif /* MAKERFAIRROME2017DEMO_H_ */
