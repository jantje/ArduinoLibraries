/*
 * MessageHandler.cpp
 *
 *  Created on: Nov 28, 2012
 *      Author: BE04258
 */

#include "SerialCommunicator.h"
#include "SerialDataInterface.h"
#include <avr/wdt.h>
#include <avr/eeprom.h>



#define CLASSSEPERATOR '.'
#define FIELDSEPERATOR ';'

#define RESET_VECTOR 0x0000

void SerialCommunicator::ForceHardReset()
{
	Serial.println(F("triggering restart"));
	while (true)
	{
		if (Serial.available())
		{
			if(Serial.read()=='N')
			{
				delay(myForceRestartDelay);
				pinMode(myResetPin,OUTPUT);
				digitalWrite(myResetPin,LOW);
				digitalWrite(myResetPin,HIGH);
				while(true);
			}
		}
	}
}

	/*
	 * This method initializes the class.
	 * This method should be called from the setup method.
	 */
void SerialCommunicator::setup(SerialDataInterface* TheData)
{
	wdt_disable();
	//Is only needed when running without bootloader
	myData[0] = this; //This should be first so we do not lose the values when we change TheData
	myData[1] = TheData;

	ReadDataFromEEPROM();
	myArduinoResetCount++;
	SaveDataToEEPROM();
	myStringSerial.setup();
}

/*
 * This method does the repetitive task of the class.
 * This method should be called from the loop method.
 */
void SerialCommunicator::loop()
{
	static uint32_t myFirstLoop = millis();
	static uint32_t myPrefLoop =  0;
	uint32_t myCurTime = millis();
	if (myLoopCounter != 0)
	{
		myAveragebetweenLoops = (myCurTime - myFirstLoop) / (myLoopCounter );
		myMaxbetweenLoops = max(myMaxbetweenLoops,myCurTime-myPrefLoop);
	}
	myLoopCounter++;
	myPrefLoop = myCurTime;

	myStringSerial.loop();
	if (myStringSerial.messageReceived())
	{
		setReceivedMessage(myStringSerial.getMessage(), true);
	}
	if ((myLogLevel & 1) == 1)
	{
		static unsigned long lastLog = 0;
		if ((lastLog + myLogDelay) < millis())
		{
			logValue(Serial);
			lastLog = millis();
		}
	}
	myIsDirty=false;

}

/**
 * This method is a visitor method and is used to visit the DataInterface classes
 * This method will dump to serial: classname.fieldname;
 */
bool LOGHEADERDataVisitor(const FieldData& fieldData, Stream& stream)
{
	stream.print(fieldData.myClassName);
	stream.print(CLASSSEPERATOR);
	stream.print(fieldData.myFieldName);
	stream.print(FIELDSEPERATOR);
	return false;
}

/**
 * This method is a visitor method and is used to visit the DataInterface classes
 * This method will dump to serial the value of the fieldData
 */
bool LOGVALUEDataVisitor(const FieldData& fieldData, Stream& stream)
{
	fieldData.dumpValue(stream);
	stream.print(FIELDSEPERATOR);
	return false;
}

/**
 * This method is a visitor method and is used to visit the DataInterface classes
 * This method will dump to serial the full content of the FieldData object
 */
bool DumpDataVisitor(const FieldData& fieldData)
{
	fieldData.dump(Serial);
	return false;
}

const char * MyMessage;

/**
 * This method uses the global Variable MyMessage to see wether it matches the classname.fieldname
 * as available in the fieldData Object.
 * It returns true if equal else it returns false
 */
bool isFieldMatch(const FieldInfo& fieldData)
{
	size_t ClassNameLength = strlen(fieldData.myClassName);
	size_t FieldNameLength = strlen_P((char*) fieldData.myFieldName);

	if (FieldNameLength > MAXFIELDNAME)
	{
		Serial.print(F("Fieldname to long: "));
		Serial.println (fieldData.myFieldName);
		return false;
	}
	char CopyFieldName[MAXFIELDNAME + 1]; //TOFIX remove MAXFIELDNAME and do direct compare in progmem
	strcpy_P(CopyFieldName, (char*) fieldData.myFieldName);
//	Serial.print(F("Comparing "));
//	Serial.print(fieldData.myClassName);
//	Serial.print(CLASSSEPERATOR);
//	Serial.print(fieldData.myFieldName);
//	Serial.print(F(" to "));
//	Serial.println(MyMessage);

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
bool GETDataVisitor(const FieldData& fieldData)
{
	if (isFieldMatch(fieldData))
	{
		fieldData.dump(Serial);
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

bool SETDataVisitor(const FieldData& fieldData, const uint8_t field, SerialDataInterface* theClass)
{
	if (isFieldMatch(fieldData))
	{
		theClass->setField(field, MyMessage + strlen(fieldData.myClassName) + strlen_P((const char *) fieldData.myFieldName) + 2);
		fieldData.dump(Serial);
		return true;
	}
	return false;
}

bool LOGDirtyClasses(const SerialDataInterface& data)
{
	if (data.isDirty())
	{
		Serial.println(data.getFieldData(0).myClassName);
	}
	return false;
}

void SerialCommunicator::logValue(Stream& stream)
{
	stream.print(F("LOG VALUE;"));
	visitAllFields(&LOGVALUEDataVisitor,stream);
	stream.println();
}
void SerialCommunicator::logHeader(Stream& stream)
{
	stream.print(F("LOG HEADER;"));
	visitAllFields(&LOGHEADERDataVisitor,stream);
	stream.println();
	return;
}
	/*
	 * This method parses a incoming string (the message is expected to be complete)
	 * Depending on the instruction the action is undertaken.
	 */
void SerialCommunicator::setReceivedMessage(const char* newMessage, bool first)
{
	if ('?' == newMessage[0])
	{
		if (first)
		{
			Serial.println(F("Following commands are supported"));
			Serial.println(F("? to show this info"));
		}
		Serial.println(F("DUMP to show all variables and their values"));
		Serial.println(F("GET [FieldName] to show one variable value"));
		Serial.println(F("SET to dump all the set commands to get copy the eeprom info"));
		Serial.println(F("SET [FieldName]=[value] to set one variable value do not use spaces around the \"'s"));
		Serial.println(F("ISDIRTY to see all the classes with the dirty flag set"));
		Serial.println(F("LOG VALUE               to LOG the values of the fields"));
		Serial.println(F("LOG HEADER              to LOG the names of the fields"));
		Serial.println(F("SAVE                    to save the values of the fields with MOD_SAVE flag to eeprom"));
		Serial.println(F("LOAD                    to load the values of the fields with MOD_SAVE flag from eeprom"));
		Serial.println(F("RESET                   to initialize the reset of the arduino; Send N to reset Arduino after this command"));
		Serial.println(F("N                       only works after RESET. 60 ms later arduino will reset"));
		return;
	}
	if (strcmp_P(newMessage,(const char *) F( "DUMP")) ==0)
	{
		dumpAllFields();
		return;
	}
	if (strncmp_P(newMessage,(const char *)F("GET "),4)==0)
	{
		Serial.println(F("GET started"));
		MyMessage=newMessage+4;
		if (!visitAllFields(&GETDataVisitor))
		{
			Serial.println(F("GET Field not found"));
		}
		else
		{
			Serial.println(F("GET done"));
		}
		return;
	}
	if (strcmp_P(newMessage,(const char *) F( "SET")) ==0)
	{
		DumpDataInEEPROM(Serial);
		return;
	}
	if (strncmp_P(newMessage,(const char *)F("SET "),4)==0)
	{
		Serial.println(F("SET started"));
		MyMessage=newMessage+4;
		if (!visitAllFields(&SETDataVisitor))
		{
			Serial.print(F("Field not found"));
			Serial.println(newMessage);
		}
		else
		{
			Serial.println(F("SET done"));
		}
		return;
	}

	if (strcmp_P(newMessage,(const char *)F("LOG HEADER"))==0)
	{
		logHeader(Serial);
		return;
	}
	if (strcmp_P(newMessage,(const char *)F("LOG VALUE"))==0)
	{
		logValue(Serial);
		return;
	}
	if (strcmp_P(newMessage,(const char *)F("ISDIRTY"))==0)
	{
		Serial.println(F("Dirty Classes:"));
		visitAllClasses(&LOGDirtyClasses);
		return;
	}
	if (strcmp_P(newMessage,(const char *)F("SAVE"))==0)
	{
		SaveDataToEEPROM();
		Serial.println(F("SAVE done"));
		return;
	}
	if (strcmp_P(newMessage,(const char *)F("LOAD"))==0)
	{
		ReadDataFromEEPROM();
		Serial.println(F("LOAD done"));
		return;
	}
	if(strcmp_P(newMessage,(const char *)F("RESET"))==0)
	{
		/*MYSERIAL.println(F("Reseting Arduino"));*/
		ForceHardReset();
		Serial.println(F("This should not happen"));
		return;
	}
	Serial.print(F("Unknown command. use ? to get a list of all commands : "));
	Serial.println(newMessage);

}
void SerialCommunicator::dumpAllFields()
{
	Serial.println(F("Dumping all fields"));
	Serial.print(F("SketchName \t"));
	Serial.println(mySketchName);
	Serial.print(F("CompileDate \t"));
	Serial.println(F(__DATE__));
	visitAllFields(&DumpDataVisitor);
	Serial.println(F("Dump done"));
}

#define EEPROMCLASBUFFER  40 //number of bytes per class
int classOffset = -1;
int fieldoffset = 0;
const char * prefClassName = 0;
bool DumpEpromFields(const FieldData& fieldData,Stream& stream)
{

	if ((fieldData.myModFlag & MOD_SAVE) == MOD_SAVE)
	{
		stream.print(F("SET "));
		stream.print(fieldData.myClassName);
		stream.print('.');
		stream.print(fieldData.myFieldName);
		stream.print('=');
		fieldData.dumpValue(stream);
		stream.println();
	}
	return false;
}
bool LoadFields(const FieldData& fieldData)
{

	if (fieldData.myClassName != prefClassName)
	{
		prefClassName = fieldData.myClassName;
		if (fieldoffset > EEPROMCLASBUFFER)
		{
			Serial.println(F("ERROR EEPROMCLASBUFFER is not big enough"));
		}
		classOffset += EEPROMCLASBUFFER;
		fieldoffset = 0;
//		MYSERIAL.print(F("DEBUG Reading class "));
//		MYSERIAL.println(fieldData.myClassName);
	}
	if ((fieldData.myModFlag & MOD_SAVE) == MOD_SAVE)
	{
//		MYSERIAL.print(F("DEBUG Reading field "));
//		MYSERIAL.println(fieldData.myFieldName);
		switch (fieldData.myType)
		{
			case _int8_t:
				*(fieldData.myValue.pint8_t) = eeprom_read_byte((const uint8_t *) (classOffset + fieldoffset));
//			MYSERIAL.print(F("DEBUG Reading signed 8bit from "));
//			MYSERIAL.println((classOffset+fieldoffset));
				fieldoffset += 1;
				break;
			case _uint8_t:
				*(fieldData.myValue.puint8_t) = eeprom_read_byte((const uint8_t *) (classOffset + fieldoffset));
//			MYSERIAL.print(F("DEBUG Reading unsigned 8bit from "));
//			MYSERIAL.println((classOffset+fieldoffset));
				fieldoffset += 1;
				break;
			case _uint16_t:
				*(fieldData.myValue.pint16_t) = eeprom_read_word((const uint16_t*) (classOffset + fieldoffset));
//			MYSERIAL.print(F("DEBUG Reading unsigned 16bit from "));
//			MYSERIAL.println((classOffset+fieldoffset));
				fieldoffset += 2;
				break;
			case _int16_t:
				*(fieldData.myValue.puint16_t) = eeprom_read_word((const uint16_t*) (classOffset + fieldoffset));
//			MYSERIAL.print(F("DEBUG Reading signed 16bit from "));
//			MYSERIAL.println((classOffset+fieldoffset));
				fieldoffset += 2;
				break;
			case _uint32_t:
				*(fieldData.myValue.puint32_t) = eeprom_read_dword((const uint32_t *) (classOffset + fieldoffset));
//			MYSERIAL.print(F("DEBUG Reading unsigned 32bit from "));
//			MYSERIAL.println((classOffset+fieldoffset));
				fieldoffset += 4;
				break;
			case _int32_t:
				*(fieldData.myValue.pint32_t) = eeprom_read_dword((const uint32_t *) (classOffset + fieldoffset));
//			MYSERIAL.print(F("DEBUG Reading signed 32bit from "));
//			MYSERIAL.println((classOffset+fieldoffset));
				fieldoffset += 4;
				break;
#ifdef GPSLOCATION
			case _GPSLocation:
				fieldData.myValue.pGPSLocation->myLatitude = eeprom_read_dword((const uint32_t *) (classOffset + fieldoffset));
				fieldData.myValue.pGPSLocation->myLongitude = eeprom_read_dword((const uint32_t *) (classOffset + fieldoffset + 4));
//			MYSERIAL.print(F("DEBUG Reading GPSLocation from "));
//			MYSERIAL.println((classOffset+fieldoffset));
				fieldoffset += 8;
				break;
#endif
			case _char:
			case _String:
				Serial.println(F("ERROR pchar and String are not supported in LoadFields"));
				break;
				default:

				Serial.print(F("ERROR this type is not coded in LoadFields"));
				Serial.println(fieldData.myType);
			}
		}

	return false;
}

bool SaveFields(const FieldData& fieldData)
{

	if (fieldData.myClassName != prefClassName)
	{
		prefClassName = fieldData.myClassName;
		if (fieldoffset > EEPROMCLASBUFFER)
		{
			Serial.println(F("ERROR EEPROMCLASBUFFER is not big enough"));
		}
		classOffset += EEPROMCLASBUFFER;
		fieldoffset = 0;
	}
	if ((fieldData.myModFlag & MOD_SAVE) == MOD_SAVE)
	{
		switch (fieldData.myType)
		{
			case _bool:
				eeprom_write_byte((uint8_t *) (classOffset + fieldoffset), *(fieldData.myValue.pbool));
//				MYSERIAL.print(F("DEBUG writing bool to "));
//				MYSERIAL.println((classOffset+fieldoffset));
				fieldoffset += 1;
				break;
			case _int8_t:
				eeprom_write_byte((uint8_t *) (classOffset + fieldoffset), *(fieldData.myValue.pint8_t));
//				MYSERIAL.print(F("DEBUG writing signed 8bit to "));
//				MYSERIAL.println((classOffset+fieldoffset));
				fieldoffset += 1;
				break;
			case _uint8_t:
				eeprom_write_byte((uint8_t *) (classOffset + fieldoffset), *(fieldData.myValue.puint8_t));
//				MYSERIAL.print(F("DEBUG writing unsigned 8bit to "));
//				MYSERIAL.println((classOffset+fieldoffset));
				fieldoffset += 1;
				break;
			case _uint16_t:
				eeprom_write_word((uint16_t *) (classOffset + fieldoffset), *(fieldData.myValue.puint16_t));
//				MYSERIAL.print(F("DEBUG writing unsigned 16bit to "));
//				MYSERIAL.println((classOffset+fieldoffset));
				fieldoffset += 2;
				break;
			case _int16_t:
				eeprom_write_word((uint16_t *) (classOffset + fieldoffset), *(fieldData.myValue.pint16_t));
//				MYSERIAL.print(F("DEBUG writing signed 16bit to "));
//				MYSERIAL.println((classOffset+fieldoffset));
				fieldoffset += 2;
				break;
			case _uint32_t:
				eeprom_write_dword((uint32_t *) (classOffset + fieldoffset), *(fieldData.myValue.puint32_t));
//				MYSERIAL.print(F("DEBUG writing unsigned 32bit to "));
//				MYSERIAL.println((classOffset+fieldoffset));
				fieldoffset += 4;
				break;
			case _int32_t:
				eeprom_write_dword((uint32_t *) (classOffset + fieldoffset), *(fieldData.myValue.pint32_t));
//				MYSERIAL.print(F("DEBUG writing signed 32bit to "));
//				MYSERIAL.println((classOffset+fieldoffset));
				fieldoffset += 4;
				break;
#ifdef GPSLOCATION
			case _GPSLocation:
				eeprom_write_dword((uint32_t *) (classOffset + fieldoffset), fieldData.myValue.pGPSLocation->myLatitude);
				eeprom_write_dword((uint32_t *) (classOffset + fieldoffset + 4), fieldData.myValue.pGPSLocation->myLongitude);
//				MYSERIAL.print(F("DEBUG writing GPSLocation to "));
//				MYSERIAL.println((classOffset+fieldoffset));
				fieldoffset += 8;
				break;
#endif
			case _char:
			case _String:
			case _DateTime:
				Serial.println(F("ERROR pchar, String and DateTime are not supported in SaveFields"));
				break;
				default:
				Serial.print(F("ERROR this type is not coded in SaveFields"));
				Serial.println(fieldData.myType);
			}
		}

	return false;
}
void SerialCommunicator::SaveDataToEEPROM()
{
	classOffset = -1 * EEPROMCLASBUFFER;
	fieldoffset = 0;
	prefClassName = 0;
	visitAllFields(&SaveFields);
}
void SerialCommunicator::ReadDataFromEEPROM()
{
	classOffset = -1 * EEPROMCLASBUFFER;
	fieldoffset = 0;
	prefClassName = 0;
	visitAllFields(&LoadFields);
}

void SerialCommunicator::DumpDataInEEPROM(Stream& stream)
{
	visitAllFields(&DumpEpromFields,stream);
}

SerialCommunicator::SerialCommunicator(uint8_t resetPin)
{
	myResetPin=resetPin;
	myFields[0].set("Admin", F("ResetCount"),MOD_SAVE | MOD_WRITE,&myArduinoResetCount);
	myFields[1].set( myFields[0].myClassName, F("logLevel"),MOD_WRITE | MOD_SAVE, &myLogLevel);
	myFields[2].set( myFields[0].myClassName, F("RestartDelay"),MOD_WRITE | MOD_SAVE, &myForceRestartDelay);
	myFields[3].set( myFields[0].myClassName, F("DelaybetweenLogs"), MOD_WRITE | MOD_SAVE, &myLogDelay);
	myFields[4].set( myFields[0].myClassName, F("LoopCounter"), 0, &myLoopCounter);
	myFields[5].set( myFields[0].myClassName, F("AveragebetweenLoops"), 0, &myAveragebetweenLoops);
	myFields[6].set( myFields[0].myClassName, F("MaxbetweenLoops"), 0, &myMaxbetweenLoops);

	SerialDataInterface::myFields = myFields;
	myLogLevel = 0;
	myLogDelay = 500;
	myForceRestartDelay = 300;
	myArduinoResetCount = 0;
	myNumFields = 7;
	myNumChildren = 0;

	myLoopCounter=0; /*Counts the number of times loop has been called*/
	myAveragebetweenLoops=0;/*The average millis between loop counts*/
	myMaxbetweenLoops=0;/*The maximum millis between loop counts*/

}

#define NUMCHILDREN 2
bool SerialCommunicator::visitAllFields(FieldDataVisitor visitorFunc) const
{
	for (size_t CurChild = 0; CurChild < NUMCHILDREN; CurChild++)
	{
		if (myData[CurChild]->visitAllFields(visitorFunc)) return true;
	}
	return false;
}

bool SerialCommunicator::visitAllFields(FieldDataVisitor2 visitorFunc, Stream& stream) const
{
	for (size_t CurChild = 0; CurChild < NUMCHILDREN; CurChild++)
	{
		if (myData[CurChild]->visitAllFields(visitorFunc, stream)) return true;
	}
	return false;
}

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
