/*
 * SmartSerial.cpp
 *
 *  Created on: Nov 28, 2012
 *      Author: Jan baeyens aka Jantje
 */

#include "SerialStringReader.h"

//char SerialStringReader::myMessage[MAX_MESSAGE_LENGTH + 1];       //The message (or part of) that has been received
//uint8_t SerialStringReader::myMessageIndex = 0; //The length of the message (next character to write)
//boolean SerialStringReader::myHasMessage = false;

SerialStringReader::SerialStringReader(	Stream & theStream	)
:myStream(theStream)
	{	myMessage[0]=0;
	};

void SerialStringReader::flush()
{
#ifndef LOCAL_RUN //if we do simulations with files there is no need to flush out data
	myHasMessage = false;
	myMessageIndex = 0;
	boolean hasReadSomething = false;
	do
	{
		if (hasReadSomething) delay(200); //If data has been found wait a while for more data
		hasReadSomething = false;
		while (myStream.available())
		{
				myStream.read();
			hasReadSomething = true;
		}
	} while (hasReadSomething);
#endif
}

void SerialStringReader::loop()
{
	while ((!myHasMessage) && (myStream.available() > 0))
	{
		char aChar = myStream.read();
		if ((aChar == '\n') || (aChar == '\r'))
		{
			myHasMessage = (myMessageIndex!=0); //this way if \n and \r are send we do not get 2 messages
			myMessageIndex = 0;
		} else
		{
			myMessage[myMessageIndex] = aChar;
			myMessage[++myMessageIndex] = 0; // Keep the string NULL terminated
			if (myMessageIndex >= MAX_MESSAGE_LENGTH)
			{
				SerialError.print(F("Message to long. Max: "));
				SerialError.println(MAX_MESSAGE_LENGTH);
				SerialError.println(myMessage);
				myMessageIndex=0;
			}
		}
	}
}

