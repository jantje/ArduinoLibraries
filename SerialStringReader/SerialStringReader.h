/*
 * SmartSerial.h
 *
 *  Created on: Nov 28, 2012
 *      Author: jan baeyens aka Jantje
 */

#ifndef SERIALSTRINGREADER_H_
#define SERIALSTRINGREADER_H_
#include "Arduino.h"

/* modify the line below if you want less memory used by the lib
 * and increase if you need a longer command line.
 */
#define MAX_MESSAGE_LENGTH (uint8_t)50
/**
 * A class to read strings from the serial communication.
 * This class reads line by line.
 * If a message is read (meaning a end of line is read) no more characters
 * are read until the message has been received (by calling get message)
 * The string returned is the actual memory buffer used to read the serial monitor.
 * if you want to save the value you should make a copy.
 *
 * look at the example on how to use this
 */
		extern Stream *SerialInput;
		extern Stream *SerialOutput;

class SerialStringReader
{
	private:
		char myMessage[MAX_MESSAGE_LENGTH +1];       //The message (or part of) that has been received
		uint8_t myMessageIndex; //The length of the message (next character to write)
		boolean myHasMessage;   //Is the message complete?


	public:
		SerialStringReader(		);
		/**
		 * Initializes the class. It does not initialize the serial communication
		 * This is something you will have to do before calling this method
		 * Call this method in your setup() after setting up the serial monitor
		 */
		void setup(){};
		/**
		 * Add the Loop() in your loop();
		 */
		void loop();
		/**
		 * Call this method to know whether a full message has arrived.
		 */
		boolean messageReceived(){return myHasMessage;};
		/**
		 * GetMessage should only be called when MessageReceived returned true.
		 * It resets the MessageReceived flag.
		 * WARNING The value returned points to the same buffer as used by the class.
		 * If you want to keep the value for later reference you need to make a copy.
		 */
		char * getMessage(){myHasMessage=false; return myMessage;};
};


#endif /* SERIALSTRINGREADER_H_ */
