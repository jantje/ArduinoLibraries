/*
 * SerialEEPROMCommunicator.h
 *
 *  Created on: Jan 23, 2014
 *      Author: jan
 */

#ifndef SERIALEEPROMCOMMUNICATOR_H_
#define SERIALEEPROMCOMMUNICATOR_H_
#include "Arduino.h"
#include <SerialCommunicator.h>

class SerialEEPROMCommunicator: public SerialCommunicator
{
	private:
		virtual void setReceivedMessage(const char* newMessage);
	public:
		static void saveData(); //Reads data in the variables and stores it in the EEPROM memory
		static void readData(); //Reads data stored in the EEPROM and sets its values to the variables
		static void setup();
		static void serialRegister(const __FlashStringHelper* Name){SerialCommunicator::serialRegister( Name);};

};

#endif /* SERIALEEPROMCOMMUNICATOR_H_ */
