/*
 * SerialBridgeCommunicator.cpp
 *
 *  Created on: Jan 24, 2014
 *      Author: jan
 */

#include "SerialBridgeCommunicator.h"
#include <avr/wdt.h>
#define BUFFERSIZE 100

const char SAVE[] PROGMEM = "SAVE";
const char LOAD[] PROGMEM = "LOAD";
const char scriptlocation[] PROGMEM = "/www/cgi-bin/jantje/";
const char keyvaluefile[] PROGMEM = "/mnt/sda1/jantje/keyvalue";
const char setkeyValue[] PROGMEM = "setkeyValueCommand ";
const char getkeyValue[] PROGMEM = "getkeyValueCommand ";




void SerialBridgeCommunicator::setReceivedMessage(const char* newMessage)
{
	if (strcmp_P(newMessage, SAVE) == 0)
	{
		SerialOutput.println((const __FlashStringHelper *)SAVE);
		saveData();
		SerialOutput.println((const __FlashStringHelper *)DONE);
		return;
	}
	else if (strcmp_P(newMessage, LOAD) == 0)
	{
		SerialOutput.println((const __FlashStringHelper *)LOAD);
		readData();
		SerialOutput.println((const __FlashStringHelper *)DONE);
		return;
	}	else if ('?' == newMessage[0])
	{
		SerialCommunicator::setReceivedMessage( newMessage);
		SerialOutput.println(F("SAVE                    save the settings to eeprom"));
		SerialOutput.println(F("LOAD 			              load the settings from eeprom"));
		return;
	}

	SerialCommunicator::setReceivedMessage(newMessage);
}

//static void saveAfield(FieldData& fieldData)
//{
//	//snprintf_P(commonlyUsedBuffer, commonlyUsedBuffersize, "%ssetkeyValue %s %s.", scriptlocation, keyvaluefile, fieldData.myClassName);
//	strlcpy_P(commonlyUsedBuffer, scriptlocation,commonlyUsedBuffersize);
//	strlcat_P(commonlyUsedBuffer, setkeyValue, commonlyUsedBuffersize);
//	strlcat_P(commonlyUsedBuffer, keyvaluefile, commonlyUsedBuffersize);
//	strlcat(commonlyUsedBuffer, " ", commonlyUsedBuffersize);
//	strlcat_P(commonlyUsedBuffer, (const prog_char *)fieldData.myClassName, commonlyUsedBuffersize);
//	strlcat(commonlyUsedBuffer, ".", commonlyUsedBuffersize);
//
//	strlcat_P(commonlyUsedBuffer, (const char *) fieldData.myFieldName, commonlyUsedBuffersize);
//	strlcat(commonlyUsedBuffer, " \"", commonlyUsedBuffersize);
//	char valuebuffer[20];
//	strlcat(commonlyUsedBuffer, fieldData.getValue(valuebuffer,20), commonlyUsedBuffersize);
//	strlcat(commonlyUsedBuffer, "\"", commonlyUsedBuffersize);
//	//Serial.println(commonlyUsedBuffer );
//	//Serial.println(commonlyUsedBuffer + 20);
//	runShellCommand(commonlyUsedBuffer, commonlyUsedBuffer, commonlyUsedBuffersize);
//}




//static void readAfield(FieldData& fieldData)
//{
//	strlcpy_P(commonlyUsedBuffer, scriptlocation,commonlyUsedBuffersize);
//	strlcat_P(commonlyUsedBuffer, getkeyValue, commonlyUsedBuffersize);
//	strlcat_P(commonlyUsedBuffer, keyvaluefile, commonlyUsedBuffersize);
//	strlcat(commonlyUsedBuffer, " ", commonlyUsedBuffersize);
//	strlcat_P(commonlyUsedBuffer, (const prog_char *)fieldData.myClassName, commonlyUsedBuffersize);
//	strlcat(commonlyUsedBuffer, ".", commonlyUsedBuffersize);
//
//
//	//snprintf(commonlyUsedBuffer, commonlyUsedBuffersize, "%sgetkeyValue %s  %s.", scriptlocation, keyvaluefile, fieldData.myClassName);
//	//Serial.println(commonlyUsedBuffer);
//	strlcat_P(commonlyUsedBuffer, (const char *) fieldData.myFieldName, commonlyUsedBuffersize);
//	//Serial.println(commonlyUsedBuffer + 20);
//	runShellCommand(commonlyUsedBuffer, commonlyUsedBuffer, commonlyUsedBuffersize);
//	//Serial.print('"');
//	//Serial.print(commonlyUsedBuffer);
//	//Serial.println('"');
//	fieldData.setValue(commonlyUsedBuffer);
//}



void SerialBridgeCommunicator::setup()
{
	readData();
	SerialCommunicator::setup();
}


/**
 * Method EXECUTEs a command on the linux site and waits for a response
 * The response is a line of content in the serial connection
 * As a side effect of this method the serial queque is emptied. This is done so old info is not interpreted as the result of the command request.
 * After 500m the method returns anyway
 */
void SerialBridgeCommunicator::runSynchronousShellCommand(const char* command, char* returnBuffer, uint8_t ReturnBuffersize)
{
	SerialOutput.print("EXEC:");
	SerialOutput.println(command);
	unsigned long startTime = millis();

	myStringSerial.flush(); //All data already in the serial queue and still comming is disregarded.
	do
	{
		myStringSerial.loop();
		wdt_reset(); //make sure the watch dog does not trigger
	} while (!(myStringSerial.messageReceived() || ((millis() - startTime) > 5000)));
	strncpy(returnBuffer, myStringSerial.getMessage(), ReturnBuffersize);
}


/**
 * Asynchronously saves the value of a field on the linux site
 * This method will only return after the Linux command returns the expected result
 */
static void saveAfield(FieldData& fieldData)
{
	// construct the linux command and put it in commonlyUsedBuffer
	strlcpy_P(commonlyUsedBuffer, setkeyValue, commonlyUsedBuffersize);
	strlcat_P(commonlyUsedBuffer, (const char *) fieldData.myClassName, commonlyUsedBuffersize);
	strlcat(commonlyUsedBuffer, ".", commonlyUsedBuffersize);
	strlcat_P(commonlyUsedBuffer, (const char *) fieldData.myFieldName, commonlyUsedBuffersize);
	strlcat(commonlyUsedBuffer, " ", commonlyUsedBuffersize);
	char valuebuffer[20];
	strlcat(commonlyUsedBuffer, fieldData.getValue(valuebuffer, 20), commonlyUsedBuffersize);

	// put the expected result in expectedResult
	char expectedResult[BUFFERSIZE];
	strlcpy_P(expectedResult, setkeyValue, BUFFERSIZE);
	strlcat_P(expectedResult, (const char *) fieldData.myClassName, BUFFERSIZE);
	strlcat(expectedResult, ".", BUFFERSIZE);
	strlcat_P(expectedResult, (const char *) fieldData.myFieldName, BUFFERSIZE);
	strlcat(expectedResult, " OK", BUFFERSIZE);

	// run the linux command until we get the expected result
	char resultbuffer[BUFFERSIZE];
	do
	{
		myCommunicator.runSynchronousShellCommand(commonlyUsedBuffer, resultbuffer, BUFFERSIZE);
		SerialOutput.println(resultbuffer);
	} while (strcmp(expectedResult, resultbuffer) != 0);

}


/**
 * The save data visitor method
 */
void SerialBridgeCommunicator::saveData()
{
	FieldData::visitAllFields(saveAfield, false);
}


/*
 * Asynchronously reads a field from the linux side
 * This method will only return after the read has been successful
 */
static void readAfield(FieldData& fieldData)
{
	//Put the Linux read command in commonlyUsedBuffer
	strlcpy_P(commonlyUsedBuffer, getkeyValue, commonlyUsedBuffersize);
	strlcat_P(commonlyUsedBuffer, (const char *) fieldData.myClassName, commonlyUsedBuffersize);
	strlcat(commonlyUsedBuffer, ".", commonlyUsedBuffersize);
	strlcat_P(commonlyUsedBuffer, (const char *) fieldData.myFieldName, commonlyUsedBuffersize);

	//put the expected start of the result in expectedResultStart
	char expectedResultStart[BUFFERSIZE];
	strlcpy_P(expectedResultStart, (const char *) F("SET "), BUFFERSIZE);
	strlcat_P(expectedResultStart, (const char *) fieldData.myClassName, BUFFERSIZE);
	strlcat(expectedResultStart, ".", BUFFERSIZE);
	strlcat_P(expectedResultStart, (const char *) fieldData.myFieldName, BUFFERSIZE);
	strlcat(expectedResultStart, "=", BUFFERSIZE);

	// run the linux command until the result starts with expectedResultStart
	char resultbuffer[BUFFERSIZE];
	do
	{
		myCommunicator.runSynchronousShellCommand(commonlyUsedBuffer, resultbuffer, BUFFERSIZE);
		SerialOutput.println(resultbuffer);
	} while (strncmp(expectedResultStart, resultbuffer, strlen(expectedResultStart)) != 0);

	//Set the received value
	fieldData.setValue(resultbuffer + strlen(expectedResultStart));
}

/**
 * The read data visitor method
 */
void SerialBridgeCommunicator::readData()
{
	FieldData::visitAllFields(readAfield, false);
}
