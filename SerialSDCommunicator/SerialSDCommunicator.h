/*
 * SerialEEPROMCommunicator.h
 *
 *  Created on: Jan 23, 2014
 *      Author: jan
 */

#pragma once

#include "Arduino.h"
#include <SerialCommunicator.h>
#include "SD.h"

class SerialSDCommunicator: public SerialCommunicator
{
	public:
		virtual void setReceivedMessage(const char* newMessage);
		SerialSDCommunicator(Stream &bridgeStream,  Stream &errorStream);
		void saveData(); //Reads data in the variables and stores it in the EEPROM memory
		void readData(); //Reads data stored in the EEPROM and sets its values to the variables
		void setup();
	//	static void serialRegister(const __FlashStringHelper* Name){SerialCommunicator::serialRegister( Name);};

};

