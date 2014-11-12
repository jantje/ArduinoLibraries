/*
 * SmartSerial.cpp
 *
 *  Created on: Nov 28, 2012
 *      Author: Jan baeyens aka Jantje
 */

#include "SerialStringReader.h"

char SerialStringReader::myMessage[MAX_MESSAGE_LENGTH + 1];       //The message (or part of) that has been received
uint8_t SerialStringReader::myMessageIndex = 0; //The length of the message (next character to write)
boolean SerialStringReader::myHasMessage = false;

void SerialStringReader::flush()
{
	myHasMessage = false;
	myMessageIndex = 0;
	boolean hasReadSomething = false;
	do
	{
		if (hasReadSomething) delay(200); //If data has been found wait a while for more data
		hasReadSomething = false;
		while (SerialInput.available())
		{
			SerialInput.read();
			hasReadSomething = true;
		}
	} while (hasReadSomething);

}

void SerialStringReader::loop()
{
	while ((!myHasMessage) && (SerialInput.available() > 0))
	{
		char aChar = SerialInput.read();
		if ((aChar == '\n') || (aChar == '\r'))
		{
			myHasMessage = myMessageIndex; //this way if \n and \r are send we do not get 2 messages
			myMessageIndex = 0;
		} else
		{
			myMessage[myMessageIndex] = aChar;
			myMessage[++myMessageIndex] = 0; // Keep the string NULL terminated
			if (myMessageIndex >= MAX_MESSAGE_LENGTH)
			{
				SerialOutput.print(F("Message to long. Max: "));
				SerialOutput.println(MAX_MESSAGE_LENGTH);
				SerialOutput.println(myMessage);
				myMessageIndex=0;
			}
		}
	}
}

