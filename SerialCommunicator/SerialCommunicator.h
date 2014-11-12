/*
 * MessageHandler.h
 *
 *  Created on: Nov 28, 2012
 *      Author: BE04258
 */

#ifndef SERIALCOMMUNICATOR_H_
#define SERIALCOMMUNICATOR_H_
#include "Arduino.h"
#include "SerialStringReader.h"
#include "SerialDataInterface.h"

/**
 * Messages to Arduino are
 * DUMP 							shows all variables with their type and value
 * SET [name] [value] set a variable with name [name] equal to [value]
 * GET [name]         get the value of the variable with name [name]
 * RESET							to reset the Arduino
 * LOG HEADER					to print the LOG Header
 * LOG VALUE					to print the LOG values
 *
 * Messages From Arduino ARE
 * HTMLGET [data]		Request to do a HTML GET
 * HTMLPOST [data] 	Request to do a HTML POST
 * LOG VALUE;[data]	Request to do log the data to disk
 */

class SerialCommunicator
{
	protected:
		static uint8_t myLogLevel;         // The Log Level used
		static uint32_t mylastLog;         //the last time we logged
		static SerialStringReader myStringSerial; //the class to read string from the serial monitor
		static uint16_t myLogDelay;        //The time to wait after a log has been done

		static uint32_t myLoopCounter;  //Counts the number of times loop has been called
		static uint32_t myMillis;
		static uint16_t myAveragebetweenLoops; //The average millis between loop counts
		static uint16_t myMaxbetweenLoops; //The maximum millis between loop counts
		static uint32_t myLoopduration; //the duration of the loop
		static uint32_t myLogduration; //the duration of the last log
		static int16_t mySerialQueueSize; //The size of the serial queue
		/**
		 * Call this method when a message has been received.
		 * The message will be parsed in the next call to Loop.
		 */
		virtual void setReceivedMessage(const char* newMessage);

		static void logValue();
		static void dumpAllFields(); // Dump all the values and settings of all the fields in a readable format

//#ifdef I_USE_RESET
//		static uint8_t myResetPin;  //The pin used to rest Arduino
//		static void ForceHardReset();//Forces the Arduino to restart so the boot loader can load a new sketch
//		static uint16_t myResetDelay;//The delay before a reset is actioned
//#endif

	public:
		SerialCommunicator();

		/**
		 * Initializes the class.
		 * Call this method in your setup()
		 */
		static void serialRegister(const __FlashStringHelper* Name);
		static void setup();

		/**
		 * Add the Loop() in your loop();
		 */
		void loop();

	};


extern const char mySketchName[] PROGMEM;


#endif /* SERIALCOMMUNICATOR_H_ */

