/*
 * SerialBridgeCommunicator.h
 *
 *  Created on: Jan 24, 2014
 *      Author: jan
 */

#ifndef SERIALBRIDGECOMMUNICATOR_H_
#define SERIALBRIDGECOMMUNICATOR_H_
#include "SerialCommunicator.h"

class SerialBridgeCommunicator: public SerialCommunicator
{
	protected:
		static void setReceivedMessage(const char* newMessage);
	public:
		static void SaveData(); //Reads data in the variables and stores it in the EEPROM memory
		static void ReadData(); //Reads data stored in the EEPROM and sets its values to the variables
		static void setup();
		static void loop();
#ifdef I_USE_RESET
		SerialBridgeCommunicator(uint8_t resetPin):
			SerialCommunicator(resetPin)
	{
	};
#endif
};

#endif /* SERIALBRIDGECOMMUNICATOR_H_ */
