/*
 * SerialBridgeCommunicator.h
 *
 *  Created on: Jan 24, 2014
 *      Author: jan
 */

#pragma once
#include "SerialCommunicator.h"

class SerialBridgeCommunicator: public SerialCommunicator
	{

	public:

		SerialBridgeCommunicator(Stream &bridgeStream,  Stream &errorStream);
		virtual void setReceivedMessage(const char *newMessage);
		void saveData(); //Reads data in the variables and stores it on the linux part of the yun
		void readData(); //Reads data on the linux part of the yun

		void setup();
		void runShellCommand(const char *command)
			{
				mySerialOutput.print("EXEC:");
				mySerialOutput.println(command);
			}
		;
		void runShellCommand(const __FlashStringHelper* command)
			{
				mySerialOutput.print("EXEC:");
				mySerialOutput.println(command);
			}
		void runSynchronousShellCommand(const char *command, char *returnBuffer, uint8_t ReturnBuffersize);
		virtual ~SerialBridgeCommunicator(){};
	};

