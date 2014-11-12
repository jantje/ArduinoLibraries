/*
 * SerialEEPROMCommunicator.cpp
 *
 *  Created on: Jan 23, 2014
 *      Author: jan
 */

#include "SerialEEPROMCommunicator.h"
#include <avr/eeprom.h>


int fieldoffset = 0;



static void readAfield(FieldData& fieldData)
{

		switch (fieldData.myType)
		{
			case _bool:
			case _int8_t:
			case _uint8_t:
				*(fieldData.myValue.pint8_t) = eeprom_read_byte((const uint8_t *) ( fieldoffset));
				fieldoffset += 1;
				break;
			case _uint16_t:

				*(fieldData.myValue.pint16_t) = eeprom_read_word((const uint16_t*) ( fieldoffset));
				fieldoffset += 2;
				break;
			case _int16_t:
				*(fieldData.myValue.puint16_t) = eeprom_read_word((const uint16_t*) ( fieldoffset));
				fieldoffset += 2;
				break;
			case _uint32_t:
			case _int32_t:
				*(fieldData.myValue.puint32_t) = eeprom_read_dword((const uint32_t *) ( fieldoffset));
				fieldoffset += 4;
				break;

#ifdef I_USE_GPS
				case _GPSLocation:
				fieldData.myValue.pGPSLocation->myLatitude = eeprom_read_dword((const uint32_t *) ( fieldoffset));
				fieldData.myValue.pGPSLocation->myLongitude = eeprom_read_dword((const uint32_t *) ( fieldoffset + 4));
				fieldoffset += 8;
				break;
#endif
#ifdef I_USE_STRING
			case _String:
#endif
#ifdef I_USE_DATETIME
				case _DateTime:
#endif
				break;
		}
	}



static void saveAfield(FieldData& fieldData)
{
		switch (fieldData.myType)
		{
			case _bool:
			case _int8_t:
			case _uint8_t:
				eeprom_write_byte((uint8_t *) ( fieldoffset), *(fieldData.myValue.pbool));
				fieldoffset += 1;
				break;
			case _uint16_t:
				eeprom_write_word((uint16_t *) ( fieldoffset), *(fieldData.myValue.puint16_t));
				fieldoffset += 2;
				break;
			case _int16_t:
				eeprom_write_word((uint16_t *) ( fieldoffset), *(fieldData.myValue.pint16_t));
				fieldoffset += 2;
				break;
			case _int32_t:
			case _uint32_t:
				eeprom_write_dword((uint32_t *) ( fieldoffset), *(fieldData.myValue.pint32_t));
				fieldoffset += 4;
				break;
#ifdef I_USE_GPS
				case _GPSLocation:
				eeprom_write_dword((uint32_t *) ( fieldoffset), fieldData.myValue.pGPSLocation->myLatitude);
				eeprom_write_dword((uint32_t *) ( fieldoffset + 4), fieldData.myValue.pGPSLocation->myLongitude);
				fieldoffset += 8;
				break;
#endif
#ifdef I_USE_STRING
			case _String:
#endif
#ifdef I_USE_DATETIME
				case _DateTime:
#endif
				break;
		}
}


const char SAVE[] PROGMEM = "SAVE";
const char LOAD[] PROGMEM = "LOAD";




void SerialEEPROMCommunicator::setReceivedMessage(const char* newMessage)
{
	if (strcmp_P(newMessage, SAVE) == 0)
	{
		SerialOutput.println((const __FlashStringHelper *)SAVE);
		saveData();
		SerialOutput.println((const __FlashStringHelper *)DONE);
		return;
	}
	if (strcmp_P(newMessage, LOAD) == 0)
	{
		SerialOutput.println((const __FlashStringHelper *)LOAD);
		readData();
		SerialOutput.println((const __FlashStringHelper *)DONE);
		return;
	}
	if ('?' == newMessage[0])
	{
		SerialCommunicator::setReceivedMessage( newMessage);
		SerialOutput.println(F("SAVE  save the settings to eeprom"));
		SerialOutput.println(F("LOAD 	load the settings from eeprom"));
		return;
	}

	SerialCommunicator::setReceivedMessage(newMessage);
}

void SerialEEPROMCommunicator::saveData()
{
	fieldoffset = 0;
	FieldData::visitAllFields(saveAfield,false);
}

void SerialEEPROMCommunicator::readData()
{
	fieldoffset = 0;
	FieldData::visitAllFields(readAfield,false);
}

void SerialEEPROMCommunicator::setup()
{
	readData();
	SerialCommunicator::setup();

}


