/*
 * SerialBridgeCommunicator.cpp
 *
 *  Created on: Jan 24, 2014
 *      Author: jan
 */

#include "SerialBridgeCommunicator.h"
#ifdef I_USE_MINI_BRIDGE
#include "MiniProcess.h"
#else
#include "Bridge.h"
#define BridgeSerial p
#endif

const char SAVE[] PROGMEM = "SAVE";
const char LOAD[] PROGMEM = "LOAD";
const char scriptlocation[] PROGMEM = "/www/cgi-bin/jantje/";
const char keyvaluefile[] PROGMEM = "/mnt/sda1/jantje/keyvalue";
const char setkeyValue[] PROGMEM = "setkeyValue ";
const char getkeyValue[] PROGMEM = "getkeyValue ";




void SerialBridgeCommunicator::setReceivedMessage(const char* newMessage)
{
	if (strcmp_P(newMessage, SAVE) == 0)
	{
		SerialOutput->println((const __FlashStringHelper *)SAVE);
		SaveData();
		SerialOutput->println((const __FlashStringHelper *)DONE);
		return;
	}
	if (strcmp_P(newMessage, LOAD) == 0)
	{
		SerialOutput->println((const __FlashStringHelper *)LOAD);
		ReadData();
		SerialOutput->println((const __FlashStringHelper *)DONE);
		return;
	}

	SerialCommunicator::setReceivedMessage(newMessage);
}

void saveAfield(FieldData& fieldData)
{
	//snprintf_P(commonlyUsedBuffer, commonlyUsedBuffersize, "%ssetkeyValue %s %s.", scriptlocation, keyvaluefile, fieldData.myClassName);
	strlcpy_P(commonlyUsedBuffer, scriptlocation,commonlyUsedBuffersize);
	strlcat_P(commonlyUsedBuffer, setkeyValue, commonlyUsedBuffersize);
	strlcat_P(commonlyUsedBuffer, keyvaluefile, commonlyUsedBuffersize);
	strlcat(commonlyUsedBuffer, " ", commonlyUsedBuffersize);
	strlcat_P(commonlyUsedBuffer, (const prog_char *)fieldData.myClassName, commonlyUsedBuffersize);
	strlcat(commonlyUsedBuffer, ".", commonlyUsedBuffersize);

	strlcat_P(commonlyUsedBuffer, (const char *) fieldData.myFieldName, commonlyUsedBuffersize);
	strlcat(commonlyUsedBuffer, " \"", commonlyUsedBuffersize);
	char valuebuffer[20];
	strlcat(commonlyUsedBuffer, fieldData.getValue(valuebuffer,20), commonlyUsedBuffersize);
	strlcat(commonlyUsedBuffer, "\"", commonlyUsedBuffersize);
	//Serial.println(commonlyUsedBuffer );
	//Serial.println(commonlyUsedBuffer + 20);
	runShellCommand(commonlyUsedBuffer, commonlyUsedBuffer, commonlyUsedBuffersize);

}

void SerialBridgeCommunicator::SaveData()
{
	FieldData::visitAllFields(saveAfield,false);
}


void readAfield(FieldData& fieldData)
{
	strlcpy_P(commonlyUsedBuffer, scriptlocation,commonlyUsedBuffersize);
	strlcat_P(commonlyUsedBuffer, getkeyValue, commonlyUsedBuffersize);
	strlcat_P(commonlyUsedBuffer, keyvaluefile, commonlyUsedBuffersize);
	strlcat(commonlyUsedBuffer, " ", commonlyUsedBuffersize);
	strlcat_P(commonlyUsedBuffer, (const prog_char *)fieldData.myClassName, commonlyUsedBuffersize);
	strlcat(commonlyUsedBuffer, ".", commonlyUsedBuffersize);


	//snprintf(commonlyUsedBuffer, commonlyUsedBuffersize, "%sgetkeyValue %s  %s.", scriptlocation, keyvaluefile, fieldData.myClassName);
	//Serial.println(commonlyUsedBuffer);
	strlcat_P(commonlyUsedBuffer, (const char *) fieldData.myFieldName, commonlyUsedBuffersize);
	//Serial.println(commonlyUsedBuffer + 20);
	runShellCommand(commonlyUsedBuffer, commonlyUsedBuffer, commonlyUsedBuffersize);
	//Serial.print('"');
	//Serial.print(commonlyUsedBuffer);
	//Serial.println('"');
	fieldData.setValue(commonlyUsedBuffer);
}

void SerialBridgeCommunicator::ReadData()
{
	FieldData::visitAllFields(readAfield,false);
}

void SerialBridgeCommunicator::setup()
{
	ReadData();
	myArduinoResetCount++;
	SaveData();
	myStringSerial.setup();
}
void SerialBridgeCommunicator::loop()
{
	static uint32_t myFirstLoop = millis();
	static uint32_t myPrefLoop = 0;
	uint32_t myCurTime = millis();
	if (myLoopCounter != 0)
	{
		myAveragebetweenLoops = (myCurTime - myFirstLoop) / (myLoopCounter);
		myMaxbetweenLoops = max(myMaxbetweenLoops,myCurTime-myPrefLoop);
	}
	myLoopCounter++;
	myPrefLoop = myCurTime;

	myStringSerial.loop();
	if (myStringSerial.messageReceived())
	{
		setReceivedMessage(myStringSerial.getMessage());
	}
	if ((myLogLevel & 1) == 1)
	{
		static unsigned long lastLog = 0;
		if ((millis() - lastLog) >= myLogDelay)
		{
			logValue();
			lastLog = millis();
		}
	}

}
