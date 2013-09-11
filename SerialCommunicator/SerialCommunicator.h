/*
 * MessageHandler.h
 *
 *  Created on: Nov 28, 2012
 *      Author: BE04258
 */

#ifndef SERIALCOMMUNICATOR_H_
#define SERIALCOMMUNICATOR_H_
#include "Arduino.h"

#include "SerialDataInterface.h"
#include "SerialStringReader.h"

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

#define MAXFIELDNAME 30

class SerialCommunicator: public SerialDataInterface
{
	private:
		SerialDataInterface* myData[2];  //The objects containing the data elements to modify
		SerialStringReader myStringSerial; //the class to read string from the serial monitor
		FieldData myFields[7];        //object to describe the the fields for serial modification

    uint8_t myResetPin;  //The pin used to rest Arduino
		uint16_t myLogDelay;        //The time to wait after a log has been done
		uint16_t myForceRestartDelay; //The delay before a force restart is actioned
		uint32_t myLoopCounter;  //Counts the number of times loop has been called
		uint16_t myAveragebetweenLoops; //The average millis between loop counts
		uint16_t myMaxbetweenLoops; //The maximum millis between loop counts

		void SaveDataToEEPROM(); //Reads data in the variables and stores it in the EEPROM memory
		void ReadDataFromEEPROM(); //Reads data stored in the EEPROM and sets its values to the variables
		void DumpDataInEEPROM(Stream& stream);  //dumps all the set commands
		void ForceHardReset();       //Forces the Arduino to restart so the boot loader can load a new sketch

		void dumpAllFields(); // Dump all the values and settings of all the fields in a readable format

		//Below are some visitor methods
		bool visitAllFields(FieldDataVisitor visitorFunc) const;
		bool visitAllFields(FieldDataVisitor2 visitorFunc, Stream& stream) const;
		bool visitAllFields(FieldDataVisitor3 visitorFunc);
		bool visitAllClasses(ClassDataVisitor visitorFunc) const;

	protected:
		void logValue(Stream& stream);          //log all the values
		void logHeader(Stream& stream);
		uint8_t myLogLevel;         // The Log Level used
		uint16_t myArduinoResetCount; // the number of times Arduino is reset

	public:

		SerialCommunicator(uint8_t resetPin);
		virtual ~SerialCommunicator(){;}
		/**
		 * Initializes the class.
		 * Call this method in your setup()
		 */
		void setup(SerialDataInterface* TheData);
		/**
		 * Add the Loop() in your loop();
		 */
		void loop();
		/**
		 * Call this method when a message has been received.
		 * The message will be parsed in the next call to Loop.
		 */
		virtual void setReceivedMessage(const char* newMessage,bool first);

};

//extern const __FlashStringHelper * mySketchName;
extern const char * mySketchName;

extern SerialCommunicator serialCommunicator;
#endif /* SERIALCOMMUNICATOR_H_ */
