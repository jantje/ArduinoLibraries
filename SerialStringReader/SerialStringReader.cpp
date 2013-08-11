/*
 * SmartSerial.cpp
 *
 *  Created on: Nov 28, 2012
 *      Author: Jan baeyens aka Jantje
 */

#include "SerialStringReader.h"



void SerialStringReader::setup()
{
	myMessageIndex=0;
	myMessage[0]=0;
	myHasMessage=false;

}

void SerialStringReader::loop()
{
   while( (!myHasMessage) && (Serial.available() > 0))
   {

      char aChar = Serial.read();
      if((aChar == '\n') || (aChar == '\r'))
      {
      	myHasMessage=myMessageIndex; //this way if \n and \r are send we do not get 2 messages
      	myMessageIndex = 0;

      }
      else
      {
      	myMessage[myMessageIndex] = aChar;
      	myMessage[++myMessageIndex] = 0; // Keep the string NULL terminated
      	if (myMessageIndex>= MAX_MESSAGE_LENGTH )
      		{
      		Serial.print(F("Message to long. Longest allowed message is "));
      		Serial.println(sizeof(myMessage)-1);
      		}
      }
   }
}
