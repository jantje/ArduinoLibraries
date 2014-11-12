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
		virtual void setReceivedMessage(const char* newMessage);
	public:
		static void saveData(); //Reads data in the variables and stores it on the linux part of the yun
		static void readData(); //Reads data on the linux part of the yun

		static void setup();
		void runShellCommand(const char * command)
		{	SerialOutput.print("EXEC:");SerialOutput.println(command);};
		void runShellCommand(const __FlashStringHelper* command)
		{	SerialOutput.print("EXEC:");SerialOutput.println(command);};
		void runSynchronousShellCommand(const char * command,char * returnBuffer,uint8_t ReturnBuffersize);

};


extern SerialBridgeCommunicator myCommunicator;

#endif /* SERIALBRIDGECOMMUNICATOR_H_ */
