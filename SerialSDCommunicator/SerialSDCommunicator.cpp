/*
 * SerialEEPROMCommunicator.cpp
 *
 *  Created on: Jan 23, 2014
 *      Author: jan
 */

#include "SerialSDCommunicator.h"

//enable the line below to get extensive debug info on Serial
//#define DEBUG

#define BUFFERSIZE 100


const char SAVE[] PROGMEM = "SAVE";
const char LOAD[] PROGMEM = "LOAD";
const char SAVE_FILE[] PROGMEM = "values.txt";

String valuesContent;


static void readAfield(FieldData& fieldData)
{
	//put the expected start of the result in expectedResultStart
	char expectedResultStart[BUFFERSIZE];
	strlcpy_P(expectedResultStart, (const char *) fieldData.myClassName, BUFFERSIZE);
	strlcat(expectedResultStart, ".", BUFFERSIZE);
	strlcat_P(expectedResultStart, (const char *) fieldData.myFieldName, BUFFERSIZE);
	strlcat(expectedResultStart, "=", BUFFERSIZE);


	int resultIndex=valuesContent.indexOf(expectedResultStart);
	if(resultIndex==-1){
#ifdef DEBUG
		Serial.print("No result found for: ");
		Serial.println(expectedResultStart);
		Serial.print("in: ");
		Serial.println(valuesContent);
		Serial.println("End of no result found.");
#endif
		return;
	}
	int resultEndIndex=valuesContent.indexOf("\n",resultIndex);
	if(resultEndIndex==-1){
#ifdef DEBUG
		Serial.print("Result was found but no line end found: ");
		Serial.println(expectedResultStart);
		Serial.println(valuesContent);
		Serial.println("End of result found nut no line end.");
#endif
		return;
	}
	String stringValue=valuesContent.substring(resultIndex, resultEndIndex);
#ifdef DEBUG
	Serial.print("searching: ");
	Serial.println(expectedResultStart);
	Serial.print("found at: ");
	Serial.print(resultIndex);
	Serial.print(" to: ");
	Serial.print(resultEndIndex);
	Serial.print(" is: ");
	Serial.println(stringValue);
#endif
	stringValue.toCharArray(expectedResultStart, BUFFERSIZE, strlen(expectedResultStart));
#ifdef DEBUG
	Serial.print("Setting value: ");
	Serial.println(expectedResultStart);
#endif
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

#ifdef DEBUG
	Serial.print("saved field ");
	Serial.println(valueLine);
#endif
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
#ifdef DEBUG
	Serial.println("start reading the fields");
#endif
	FieldData::visitAllFields(saveAfield,false);
#ifdef DEBUG
	Serial.println("Done reading the fields");
#endif

	// make a backup copyu of the file
	if (SD.exists(SAVE_FILE)) {
		SD.remove(SAVE_FILE);
		mySerialError.println("Removed the original file");
	}
#ifdef DEBUG
	Serial.println("Opening new values.txt for write");
#endif
	File valuesFile = SD.open(SAVE_FILE,FILE_WRITE);
	if (!valuesFile) {
		mySerialError.print("error opening file:");
		mySerialError.println(SAVE_FILE);
		return;
	}
#ifdef DEBUG
	Serial.println(valuesContent);
#endif
	valuesFile.println(valuesContent);
	valuesFile.close();
#ifdef DEBUG
	Serial.println("File values.txt is closed");
#endif
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
#ifdef DEBUG
	Serial.println(valuesContent);
	Serial.println();
	Serial.print("Done dumping the file: ");
	Serial.println(SAVE_FILE);

	Serial.println("reading the fields");
#endif
	FieldData::visitAllFields(readAfield,false);
#ifdef DEBUG
	Serial.println("File values.txt is closed");
#endif
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








