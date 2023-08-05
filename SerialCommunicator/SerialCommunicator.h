/*
 * MessageHandler.h
 *
 *  Created on: Nov 28, 2012
 *      Author: BE04258
 */

#pragma once
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
		 Stream &SerialOutput;
		 Stream &SerialError;
		 uint8_t myLogLevel=1;         // The Log Level used
		 uint32_t mylastLog=0;         //the last time we logged
		 SerialStringReader mySerialStringReader; //the class to read string from the serial monitor
		 uint16_t myLogDelay=1000;        //The time to wait after a log has been done

		 uint32_t myLoopCounter=0;  //Counts the number of times loop has been called
		 uint32_t myLastLoopMillis=0;
		 uint16_t myAveragebetweenLoops=0; //The average millis between loop counts
		 uint16_t myMaxbetweenLoops=0; //The maximum millis between loop counts
		 uint32_t myLoopduration=0; //the duration of the loop
		 uint32_t myLogduration=0; //the duration of the last log
		 int16_t mySerialQueueSize=0; //The size of the serial queue
		/**
		 * Call this method when a message has been received.
		 * The message will be parsed in the next call to Loop.
		 */
		virtual void setReceivedMessage(const char* newMessage);
		virtual ~SerialCommunicator(){};

		 void logValue();
		 void dumpCommands();

//#ifdef I_USE_RESET
//		static uint8_t myResetPin;  //The pin used to rest Arduino
//		static void ForceHardReset();//Forces the Arduino to restart so the boot loader can load a new sketch
//		static uint16_t myResetDelay;//The delay before a reset is actioned
//#endif

	public:
		SerialCommunicator(Stream &commStream,Stream & outputStream,Stream & errorStream);

		/**
		 * Initializes the class.
		 * Call this method in your setup()
		 */
		void serialRegister(const __FlashStringHelper* Name);
		void setup();

		/**
		 * Add the Loop() in your loop();
		 */
		void loop();

	};


extern const char mySketchName[] PROGMEM;
void waitForYunToBoot();



