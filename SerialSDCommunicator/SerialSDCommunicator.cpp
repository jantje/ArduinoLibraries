/*
 * SerialEEPROMCommunicator.cpp
 *
 *  Created on: Jan 23, 2014
 *      Author: jan
 */

#include "SerialSDCommunicator.h"


#define BUFFERSIZE 100


const char SAVE[] PROGMEM = "SAVE";
const char LOAD[] PROGMEM = "LOAD";
const char SAVE_FILE[] PROGMEM = "values.txt";

String valuesContent;


static void readAfield(FieldData& fieldData)
{
	//put the expected start of the result in expectedResultStart
	char expectedResultStart[BUFFERSIZE];
	strlcpy_P(expectedResultStart, (const char *) F("SET "), BUFFERSIZE);
	strlcat_P(expectedResultStart, (const char *) fieldData.myClassName, BUFFERSIZE);
	strlcat(expectedResultStart, ".", BUFFERSIZE);
	strlcat_P(expectedResultStart, (const char *) fieldData.myFieldName, BUFFERSIZE);
	strlcat(expectedResultStart, "=", BUFFERSIZE);


	int resultIndex=valuesContent.indexOf(expectedResultStart);
	if(resultIndex==0){
		Serial.print("No result found for: ");
		Serial.println(expectedResultStart);
		return;
	}
	int resultEndIndex=valuesContent.indexOf("\n",resultIndex);
	if(resultEndIndex==0){
		Serial.print("Result was found but no line end found: ");
		Serial.println(expectedResultStart);
		return;
	}
	String stringValue=valuesContent.substring(resultIndex, resultEndIndex);
	stringValue.toCharArray(expectedResultStart, BUFFERSIZE, strlen(expectedResultStart));
	fieldData.setValue(expectedResultStart);

	}



static void saveAfield(FieldData& fieldData)
{

	char valueLine[200];
	char valuebuffer[20];

	strcpy(valueLine,(const char*)fieldData.myClassName);
	strncat(valueLine,".",200-strlen(valueLine));
	strncat(valueLine,(const char*)fieldData.myFieldName,200-strlen(valueLine));
	strncat(valueLine,"=",200-strlen(valueLine));
	strncat(valueLine,(const char*)fieldData.getValue(valuebuffer, 20),200-strlen(valueLine));
	valuesContent.concat(valueLine);
	valuesContent.concat('\n');
	//dataFile.println(valueLine);

	//debug log
	Serial.print("saved field ");
	Serial.println(valueLine);
}





void SerialSDCommunicator::setReceivedMessage(const char* newMessage)
{
	if (strcmp_P(newMessage, SAVE) == 0)
	{
		mySerialOutput.println((const __FlashStringHelper *)SAVE);
		saveData();
		mySerialOutput.println((const __FlashStringHelper *)DONE);
		return;
	}
	if (strcmp_P(newMessage, LOAD) == 0)
	{
		mySerialOutput.println((const __FlashStringHelper *)LOAD);
		readData();
		mySerialOutput.println((const __FlashStringHelper *)DONE);
		return;
	}
	if ('?' == newMessage[0])
	{
		SerialCommunicator::setReceivedMessage( newMessage);
		mySerialOutput.println(F("SAVE  save the settings to SD"));
		mySerialOutput.println(F("LOAD 	load the settings from SD"));
		return;
	}

	SerialCommunicator::setReceivedMessage(newMessage);
}

void SerialSDCommunicator::saveData()
{


	valuesContent="";
	Serial.println("start reading the fields");
	FieldData::visitAllFields(saveAfield,false);
	Serial.println("Done reading the fields");

	// make a backup copyu of the file
	if (SD.exists(SAVE_FILE)) {
		SD.remove(SAVE_FILE);
		Serial.println("Removed the original file");
	}
	Serial.println("Opening new values.txt for write");
	File valuesFile = SD.open(SAVE_FILE,FILE_WRITE);
	if (!valuesFile) {
		mySerialError.print("error opening file:");
		mySerialError.println(SAVE_FILE);
		return;
	}
	Serial.println(valuesContent);
	valuesFile.println(valuesContent);
	valuesFile.close();
	Serial.println("File values.txt is closed");
}


void SerialSDCommunicator::readData()
{
	File dataFile = SD.open(SAVE_FILE);

	// if the file is available, write to it:
	if (!dataFile) {
		mySerialError.print("error opening file:");
		mySerialError.println(SAVE_FILE);
		return;
	}
	valuesContent="";
	while (dataFile.available()) {
		valuesContent.concat((char)dataFile.read());
	  }
	Serial.println("Done reading fields");
	dataFile.close();
	Serial.println(valuesContent);
	Serial.println();
	Serial.print("Done dumping the file: ");
	Serial.println(SAVE_FILE);

	Serial.println("reading the fields");
	FieldData::visitAllFields(readAfield,false);

	Serial.println("File values.txt is closed");
}

SerialSDCommunicator::SerialSDCommunicator(Stream &bridgeStream, Stream &errorStream):
						SerialCommunicator(bridgeStream,  errorStream)
{
}

void SerialSDCommunicator::setup()
{
	SerialCommunicator::setup();
	mySerialError.print("Initializing SD card...");

	// see if the card is present and can be initialized:
	if (!SD.begin(BUILTIN_SDCARD)) {
		mySerialError.println("Card failed, or not present");
	  // don't do anything more:
	  return;
	}
	mySerialError.println("SD card initialized.");
	readData();

}








