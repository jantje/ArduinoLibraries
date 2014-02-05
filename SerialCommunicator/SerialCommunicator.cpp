/*
 * MessageHandler.cpp
 *
 *  Created on: Nov 28, 2012
 *      Author: BE04258
 */

#include "SerialCommunicator.h"
#include "SerialDataInterface.h"
#include "SerialStringReader.h"

#define CLASSSEPERATOR '.'
#define FIELDSEPERATOR ';'
const char LOG_VALUE[]="LOG VALUE";
const char LOG_HEADER[]="LOG HEADER";
const char DUMP[]="DUMP";
const char NAME_NOT_FOUND[]="Field not found ";
const char NAME_TO_LONG[]="Name to long ";

const char DONE[]="done";
const char SET[]="SET ";
const char GET[]="GET ";



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




void SerialCommunicator::ForceHardReset()
{
	delay(myForceRestartDelay);
	pinMode(myResetPin, OUTPUT);
	digitalWrite(myResetPin, LOW);
	digitalWrite(myResetPin, HIGH);
	while (true)
		;
}

/*
 * This method initializes the class.
 * This method should be called from the setup method.
 */
void SerialCommunicator::setup(SerialDataInterface* TheData)
{
	myData[0] = this; //This should be first so we do not lose the values when we change TheData
	myData[1] = TheData;

	myStringSerial.setup();
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
		if ((millis()- lastLog) >= myLogDelay )
		{
			logValue();
			lastLog = millis();
		}
	}
	myIsDirty = false;

}

/**
 * This method is a visitor method and is used to visit the DataInterface classes
 * This method will dump to serial: classname.fieldname;
 */
bool LOGHEADERDataVisitor( FieldData& fieldData,const uint8_t field, SerialDataInterface* theClass)
{
	SerialOutput->print(fieldData.myClassName);
	SerialOutput->print(CLASSSEPERATOR);
	SerialOutput->print(fieldData.myFieldName);
	SerialOutput->print(FIELDSEPERATOR);
	return false;
}

/**
 * This method is a visitor method and is used to visit the DataInterface classes
 * This method will dump to serial the value of the fieldData
 */
bool LOGVALUEDataVisitor( FieldData& fieldData,const uint8_t field, SerialDataInterface* theClass)
{
	//fieldData.dumpValue();
	SerialOutput->print(fieldData.getValue(commonlyUsedBuffer,commonlyUsedBuffersize));
	SerialOutput->print(FIELDSEPERATOR);
	return false;
}

/**
 * This method is a visitor method and is used to visit the DataInterface classes
 * This method will dump to serial the full content of the FieldData object
 */
bool DumpDataVisitor(FieldData& fieldData,const uint8_t field, SerialDataInterface* theClass)
{
	fieldData.dump();
	delay(30); //I added this as the dump does not arrive properly at linux
	return false;
}

const char * MyMessage;

/**
 * This method uses the global Variable MyMessage to see whether it matches the classname.fieldname
 * as available in the fieldData Object.
 * It returns true if equal else it returns false
 */
bool isFieldMatch(const FieldInfo& fieldData)
{
	size_t ClassNameLength = strlen(fieldData.myClassName);
	size_t FieldNameLength = strlen_P((char*) fieldData.myFieldName);

	if (FieldNameLength > MAXFIELDNAME)
	{
		Serial.print(NAME_TO_LONG);
		Serial.println (fieldData.myFieldName);
		return false;
	}
	char CopyFieldName[MAXFIELDNAME + 1]; //TOFIX remove MAXFIELDNAME and do direct compare in progmem
	strcpy_P(CopyFieldName, (char*) fieldData.myFieldName);

	if (strncmp(fieldData.myClassName, MyMessage, ClassNameLength) != 0) return false; /*Class name does not match*/
	if (MyMessage[ClassNameLength] != CLASSSEPERATOR) return false;/*The Classname is to short*/
	/*The class name is a match*/
	if (strncmp(CopyFieldName, MyMessage + ClassNameLength + 1, FieldNameLength) != 0) return false;/*field name does not match*/
	char LastChar = MyMessage[ClassNameLength + 1 + FieldNameLength];
	if ((LastChar == 0) || (LastChar == ' ') || (LastChar == '=')) return true;
	return false;/*The Fieldname is to short*/
}

/**
 * This method is a visitor method and is used to visit the DataInterface classes
 * This method will dump to content of fieldData that matches the classname and fieldname as in the
 * global variable MyMessage
 */
bool GETDataVisitor( FieldData& fieldData,const uint8_t field, SerialDataInterface* theClass)
{
	if (isFieldMatch(fieldData))
	{
		fieldData.dump();
		return true;
	}
	return false;
}

/**
 * This method is a visitor method and is used to visit the DataInterface classes
 * This method will set to content of value field of fieldData that matches the classname and fieldname
 * as in the global variable MyMessage
 * If the fieldData is not writable a error is dumped to serial.
 */

bool SETDataVisitor( FieldData& fieldData, const uint8_t field, SerialDataInterface* theClass)
{
	if (isFieldMatch(fieldData))
	{
		theClass->setField(field, MyMessage + strlen(fieldData.myClassName) + strlen_P((const char *) fieldData.myFieldName) + 2);
		fieldData.dump();
		return true;
	}
	return false;
}


void SerialCommunicator::logValue()
{
	SerialOutput->print(LOG_VALUE);
	SerialOutput->print(FIELDSEPERATOR);
	visitAllFields(&LOGVALUEDataVisitor);
	SerialOutput->println();
}
void SerialCommunicator::logHeader()
{
	SerialOutput->print(LOG_HEADER);
	SerialOutput->print(FIELDSEPERATOR);
	visitAllFields(&LOGHEADERDataVisitor);
	SerialOutput->println();
	return;
}
	/*
	 * This method parses a incoming string (the message is expected to be complete)
	 * Depending on the instruction the action is undertaken.
	 */
void SerialCommunicator::setReceivedMessage(const char* newMessage)
{

	if (strcmp(newMessage,DUMP) ==0)
	{
		dumpAllFields();
		return;
	}
	if (strncmp(newMessage,GET,4)==0)
	{
		Serial.println(GET);
		MyMessage=newMessage+4;
		if (!visitAllFields(&GETDataVisitor))
		{
			Serial.println(NAME_NOT_FOUND);
		}
		Serial.println(DONE);
		return;
	}

	if (strncmp(newMessage,SET,4)==0)
	{
		Serial.println(SET);
		MyMessage=newMessage+4;
		if (!visitAllFields(&SETDataVisitor))
		{
			Serial.print(NAME_NOT_FOUND);
			Serial.println(newMessage);
		}
		else
		{
			Serial.println(DONE);
		}
		return;
	}
	else 	if (strncmp(newMessage,SET,3) ==0)
	{
		Serial.println(SET);
		DumpSavedData();
		Serial.println(DONE);
		return;
	}

	if (strcmp(newMessage,LOG_HEADER)==0)
	{
		logHeader();
		return;
	}
	if (strcmp(newMessage,LOG_VALUE)==0)
	{
		logValue();
		return;
	}

	if(strcmp_P(newMessage,(const char *)F("RESET"))==0)
	{
		ForceHardReset();
   	return;
	}
	dumpCommands();
	if ('?' != newMessage[0])
	{
		Serial.print(ERROR);
		Serial.println(newMessage);
	}


}
void SerialCommunicator::dumpAllFields()
{
	Serial.println(F("Dumping all fields"));
	Serial.print(F("SketchName\t"));
	Serial.println(mySketchName);
	Serial.print(F("CompileDate\t"));
	Serial.println(F(__DATE__));
	visitAllFields(&DumpDataVisitor);
	Serial.println(DONE);
}



bool DumpSavedFields( FieldData& fieldData,const uint8_t field, SerialDataInterface* theClass)
{

	if ((fieldData.myModFlag & MOD_SAVE) == MOD_SAVE)
	{
		SerialOutput->print(SET);
		SerialOutput->print(fieldData.myClassName);
		SerialOutput->print(CLASSSEPERATOR);
		SerialOutput->print(fieldData.myFieldName);
		SerialOutput->print('=');
		SerialOutput->println(fieldData.getValue(commonlyUsedBuffer,commonlyUsedBuffersize));
		//fieldData.dumpValue();
		//SerialOutput->println();
	}
	return false;
}


void SerialCommunicator::DumpSavedData()
{
	visitAllFields(&DumpSavedFields);
}

SerialCommunicator::SerialCommunicator(uint8_t resetPin)
		: myStringSerial()
{
	myResetPin = resetPin;
	myFields[0].set("Admin", F("ResetCount"),MOD_SAVE | MOD_WRITE,&myArduinoResetCount);
	myFields[1].set(myFields[0].myClassName, F("logLevel"),MOD_WRITE | MOD_SAVE, &myLogLevel);
	myFields[2].set(myFields[0].myClassName, F("RestartDelay"),MOD_WRITE | MOD_SAVE, &myForceRestartDelay);
	myFields[3].set(myFields[0].myClassName, F("DelaybetweenLogs"), MOD_WRITE | MOD_SAVE, &myLogDelay);
	myFields[4].set(myFields[0].myClassName, F("LoopCounter"), 0, &myLoopCounter);
	myFields[5].set(myFields[0].myClassName, F("Avg Loop"), 0, &myAveragebetweenLoops);
	myFields[6].set(myFields[0].myClassName, F("Max Loop"), 0, &myMaxbetweenLoops);
	myFields[7].set(myFields[0].myClassName, F("ResetPin"), 0, &myResetPin);

	SerialDataInterface::myFields = myFields;
	myLogLevel = 0;
	myLogDelay = 500;
	myForceRestartDelay = 300;
	myArduinoResetCount = 0;
	myNumFields = 8;
	myNumChildren = 0;

	myLoopCounter = 0; /*Counts the number of times loop has been called*/
	myAveragebetweenLoops = 0;/*The average millis between loop counts*/
	myMaxbetweenLoops = 0;/*The maximum millis between loop counts*/

}

#define NUMCHILDREN 2
//bool SerialCommunicator::visitAllFields(FieldDataVisitor visitorFunc) const
//{
//	for (size_t CurChild = 0; CurChild < NUMCHILDREN; CurChild++)
//	{
//		if (myData[CurChild]->visitAllFields(visitorFunc)) return true;
//	}
//	return false;
//}


bool SerialCommunicator::visitAllFields(FieldDataVisitor3 visitorFunc)
{
	for (size_t CurChild = 0; CurChild < NUMCHILDREN; CurChild++)
	{
		if (myData[CurChild]->visitAllFields(visitorFunc)) return true;
	}
	return false;
}

bool SerialCommunicator::visitAllClasses(ClassDataVisitor visitorFunc) const
{
	for (size_t CurChild = 0; CurChild < NUMCHILDREN; CurChild++)
	{
//  	MYSERIAL.print("Processing MessageHandler::VisitAllClasses visitorFunc child ");
//  	MYSERIAL.println(CurChild);
		if (myData[CurChild]->visitAllClasses(visitorFunc)) return true;
	}
	return false;
}
