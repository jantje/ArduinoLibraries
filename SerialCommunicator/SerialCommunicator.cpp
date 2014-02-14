/*
 * MessageHandler.cpp
 *
 *  Created on: Nov 28, 2012
 *      Author: BE04258
 */

#include "SerialCommunicator.h"
#include "SerialStringReader.h"

const char LOG_VALUE[] PROGMEM ="LOG VALUE";
const char LOG_HEADER[] PROGMEM ="LOG HEADER";
const char DUMP[] PROGMEM ="DUMP";
const char NAME_NOT_FOUND[] PROGMEM="Field not found ";
const char NAME_TO_LONG[] PROGMEM ="Name to long ";
const char RESET[] PROGMEM ="RESET";

const char DONE[] PROGMEM="done";
const char SET[] PROGMEM ="SET ";
const char GET[] PROGMEM ="GET ";


uint8_t SerialCommunicator::myLogLevel;         // The Log Level used
uint16_t SerialCommunicator::myArduinoResetCount; // the number of times Arduino is reset
SerialStringReader SerialCommunicator::myStringSerial; //the class to read string from the serial monitor
uint16_t SerialCommunicator::myLogDelay;        //The time to wait after a log has been done

uint32_t SerialCommunicator::myLoopCounter;  //Counts the number of times loop has been called
uint16_t SerialCommunicator::myAveragebetweenLoops; //The average millis between loop counts
uint16_t SerialCommunicator::myMaxbetweenLoops; //The maximum millis between loop counts


void logValueVisitor(FieldData& fieldData)
{
	SerialOutput->print(fieldData.getValue(commonlyUsedBuffer, commonlyUsedBuffersize));
	SerialOutput->print(FIELDSEPERATOR);
}
void logHeaderVisitor(FieldData& fieldData)
{
	SerialOutput->print(fieldData.myClassName);
	SerialOutput->print(CLASSSEPERATOR);
	SerialOutput->print(fieldData.myFieldName);
	SerialOutput->print(FIELDSEPERATOR);
}
void dumpVisitor(FieldData& fieldData)
{
	fieldData.dump();
}

void DumpWritableData(FieldData& fieldData)
{
	SerialOutput->print((__FlashStringHelper *) SET);
	SerialOutput->print(fieldData.myClassName);
	SerialOutput->print(CLASSSEPERATOR);
	SerialOutput->print(fieldData.myFieldName);
	SerialOutput->print('=');
	SerialOutput->println(fieldData.getValue(commonlyUsedBuffer, commonlyUsedBuffersize));
}

void dumpCommands()
{
//Serial.println(F("Following commands are supported"));
//Serial.println(F("? to show this info"));
//Serial.println(F("DUMP full memory dump"));
//Serial.println(F("GET [Name] 1 field memory dump"));
//Serial.println(F("SET dump all the set commands"));
//Serial.println(F("SET [Field]=[value] set value of field"));
//Serial.println(F("LOG_VALUE LOG all the values"));
//Serial.println(F("LOG HEADER LOG all the names"));
//Serial.println(F("SAVE save the values"));
//Serial.println(F("LOAD load the values"));
//Serial.println(F("RESET reset arduino"));
}

#ifdef I_USE_RESET
void SerialCommunicator::ForceHardReset()
{
	delay(myResetDelay);
	pinMode(myResetPin, OUTPUT);
	digitalWrite(myResetPin, HIGH);
	while (true)
	;
}
#endif

void SerialCommunicator::logValue()
{
	SerialOutput->print((__FlashStringHelper *) LOG_VALUE);
	SerialOutput->print(FIELDSEPERATOR);
	FieldData::visitAllFields(logValueVisitor,true);
	SerialOutput->println();
}
/*
 * This method does the repetitive task of the class.
 * This method should be called from the loop method.
 */
void SerialCommunicator::loop()
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

/*
 * This method parses a incoming string (the message is expected to be complete)
 * Depending on the instruction the action is undertaken.
 */
void SerialCommunicator::setReceivedMessage(const char* newMessage)
{
	if (strcmp_P(newMessage, DUMP) == 0)
	{
		dumpAllFields();
	} else if (strncmp_P(newMessage, GET, 4) == 0)
	{
		Serial.println((__FlashStringHelper *) GET);
		FieldData *fieldData = FieldData::findField(newMessage + 4);
		if (fieldData != 0) fieldData->dump();
	} else if (strncmp_P(newMessage, SET, 4) == 0)
	{
		Serial.println((__FlashStringHelper *) SET);
		FieldData *fp = FieldData::findField(newMessage + 4);
		if (fp != 0)
		{
			fp->setValue(newMessage + 4 + strlen_P((const char *) fp->myClassName) + strlen_P((const char *) fp->myFieldName) + 2);
			fp->dump();
		}
	} else if (strncmp_P(newMessage, SET, 3) == 0)
	{
		Serial.println((__FlashStringHelper *) SET);
		FieldData::visitAllFields(DumpWritableData, false);
	} else if (strcmp_P(newMessage, LOG_HEADER) == 0)
	{
		SerialOutput->print((__FlashStringHelper *) LOG_HEADER);
		SerialOutput->print(FIELDSEPERATOR);
		FieldData::visitAllFields(logHeaderVisitor,true);
		SerialOutput->println();
		return;
	} else if (strcmp_P(newMessage, LOG_VALUE) == 0)
	{
		logValue();
		return;
#ifdef I_USE_RESET
	} else if (strcmp_P(newMessage, RESET) == 0)
	{
		ForceHardReset();
#endif
	} else
	{
		dumpCommands();
		if ('?' != newMessage[0])
		{
			Serial.print((__FlashStringHelper *) ERROR);
			Serial.println(newMessage);
		}
		return;
	}
	Serial.println((__FlashStringHelper *) DONE);
}

void SerialCommunicator::dumpAllFields()
{
	Serial.println(F("Dumping all fields"));
	Serial.print(F("SketchName\t"));
	Serial.println((__FlashStringHelper *)mySketchName);
	Serial.print(F("CompileDate\t"));
	Serial.println(F(__DATE__));
	FieldData::visitAllFields(dumpVisitor,true);
}
#ifdef I_USE_RESET
	SerialCommunicator::SerialCommunicator(uint8_t resetPin)
#else
SerialCommunicator::SerialCommunicator()
#endif
{
#ifdef I_USE_RESET
	myResetPin = resetPin;
	myResetDelay = 300;
#endif

	myLogLevel = 1;
	myLogDelay = 2000;
	myArduinoResetCount = 0;
	myLoopCounter = 0; /*Counts the number of times loop has been called*/
	myAveragebetweenLoops = 0;/*The average millis between loop counts*/
	myMaxbetweenLoops = 0;/*The maximum millis between loop counts*/

}

void SerialCommunicator::serialRegister(const __FlashStringHelper* Name)
{

	FieldData::set(Name, F("logLevel"),MOD_WRITE | MOD_SAVE, &myLogLevel);

	FieldData::setNext( F("DelaybetweenLogs"), MOD_WRITE | MOD_SAVE, &myLogDelay);
	FieldData::setNext( F("LoopCounter"), 0, &myLoopCounter);
	FieldData::setNext( F("Avg Loop"), 0, &myAveragebetweenLoops);
	FieldData::setNext( F("Max Loop"), 0, &myMaxbetweenLoops);
	FieldData::setNext( F("ResetCount"),MOD_SAVE | MOD_WRITE,&myArduinoResetCount);
#ifdef I_USE_RESET
		FieldData::set(Name, F("ResetPin"), 0, &myResetPin);
		FieldData::set(Name, F("ResetDelay"),MOD_WRITE | MOD_SAVE, &myResetDelay);
#endif
	}
