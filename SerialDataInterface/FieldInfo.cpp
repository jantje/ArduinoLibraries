/*
 * FieldInfo.cpp
 *
 *  Created on: Feb 8, 2014
 *      Author: jan
 */

#include "FieldInfo.h"
/**
 * this buffer can be used to not have to allocate buffers at multiple locations
 */
char commonlyUsedBuffer[commonlyUsedBuffersize];

const FieldInfo& FieldInfo::operator =(const FieldInfo &fieldInfo)
	{
		myClassName = fieldInfo.myClassName;
		myFieldName = fieldInfo.myFieldName;
		myType = fieldInfo.myType;
		myModFlag = fieldInfo.myModFlag;
		return fieldInfo;
	}

bool FieldInfo::operator ==(const FieldInfo &fieldInfo) const
	{
		return ((strcmp_P((const char*) fieldInfo.myClassName, (const char*) myClassName) == 0)
		        && (strcmp_P((const char*) fieldInfo.myFieldName, (const char *) myFieldName) == 0) && (fieldInfo.myType == myType)
		        && (fieldInfo.myModFlag == myModFlag));

	}

/**
 * This method dumps the content of the FieldInfoobject to the stream provided
 */
void FieldInfo::dump(Stream &serial) const
	{
		serial.print(F("name\t"));
		serial.print(myClassName);
		serial.print('.');
		serial.println(myFieldName);
		serial.print(F("mod\t"));
		serial.println(myModFlag);
		serial.print(F("Type\t"));
		switch (myType)
			{
			case _uint8_t:
			case _uint16_t:
			case _uint32_t:
				serial.print('u');
				break;
			default:
				break;
			}
		switch (myType)
			{
			case _bool:
				serial.println(F("bool"));
				break;
			case _uint8_t:
			case _int8_t:
				serial.println(F("int8"));
				break;
			case _uint16_t:
			case _int16_t:
				serial.println(F("int16"));
				break;
			case _uint32_t:
			case _int32_t:
				serial.println(F("int32"));
				break;
			case _ppchar:
				serial.println(F("char*"));
				break;

#ifdef I_USE_GPS
			case _GPSLocation:
				serial.println(F("GPS"));
				break;
#endif
#ifdef I_USE_STRING
			case _String:
			SerialOutput.println(F("String"));
			break;
#endif
#ifdef I_USE_DATETIME
			case _DateTime:
				serial.println(F("DateTime"));
				break;
#endif
			case _FlashStringHelper:
				serial.println('F');
				break;

			}

	}
