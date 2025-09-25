/*
 * SimpleSwitch.h
 *
 *  Created on: 29 sep. 2022
 *      Author: jan
 */
#pragma once
#include "Arduino.h"
#include "FastRunningMedian.h"
#define NUM_HIT_READS 4

class SimpleSwitch
	{
	public:
		SimpleSwitch(uint8_t pin, uint8_t closedState);
		void setup();
		void loop();
		bool isClosed()
			{
				return myIsClosed;
			}
	private:
		FastRunningMedian<bool, NUM_HIT_READS * 2 + 1, false> myReads;
		uint8_t myPin;
		uint8_t myClosedState;
		bool myIsClosed;
		uint32_t last_Switchread;
	};

