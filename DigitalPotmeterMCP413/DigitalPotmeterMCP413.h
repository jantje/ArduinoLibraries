/*
 * MCP413.h
 * This is a library for a digital pot meter MCP413
 *
 *  Created on: Jul 11, 2014
 *      Author: jan
 */

#ifndef MCP413_H_
#define MCP413_H_
#include "SPI.h"
#include "DigitalPotmeterInterface.h"
#include "SerialDataInterface.h"

class DigitalPotmeterMCP413 :public DigitalPotmeterInterface
{
		uint8_t mySlavePin; //the slave pin used to communicate with the MCP413
		uint8_t myActualSendValue; //The actual value send over SPI
		bool myInverted;
	public:
		DigitalPotmeterMCP413(uint8_t myMaxPotValue,uint8_t slavePin,bool inverted);
		virtual void setup();
		virtual void loop();
		void serialRegister(const __FlashStringHelper* Name);
};

#endif /* MCP413_H_ */
