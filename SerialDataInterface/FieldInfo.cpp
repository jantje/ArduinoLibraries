/*
 * FieldInfo.cpp
 *
 *  Created on: Feb 8, 2014
 *      Author: jan
 */

#include "FieldInfo.h"

const FieldInfo& FieldInfo::operator =(const FieldInfo& fieldInfo)
{
	myClassName = fieldInfo.myClassName;
	myFieldName = fieldInfo.myFieldName;
	myType = fieldInfo.myType;
	myModFlag = fieldInfo.myModFlag;
	return fieldInfo;
}

bool FieldInfo::operator ==(const FieldInfo& fieldInfo) const
{
	return ((strcmp_P((const char*) fieldInfo.myClassName, (const char*) myClassName) == 0) && (strcmp_P((const char*) fieldInfo.myFieldName, (const char *) myFieldName) == 0) && (fieldInfo.myType == myType) && (fieldInfo.myModFlag == myModFlag));

}

/**
 * This method dumps the content of the FieldInfoobject to the stream provided
 */
void FieldInfo::dump() const
{
	SerialOutput->print(F("name\t"));
	SerialOutput->print(myClassName);
	SerialOutput->print('.');
	SerialOutput->println(myFieldName);
	SerialOutput->print(F("mod\t"));
	SerialOutput->println(myModFlag);
	SerialOutput->print(F("Type\t"));
	switch (myType)
	{
		case _uint8_t:
		case _uint16_t:
		case _uint32_t:
		SerialOutput->print('u');
		break;
		default:
		break;
	}
	switch (myType)
	{
		case _bool:
		SerialOutput->println(F("bool"));
		break;
		case _uint8_t:
		case _int8_t:
		SerialOutput->println(F("int8"));
		break;
		case _uint16_t:
		case _int16_t:
		SerialOutput->println(F("int16"));
		break;
		case _uint32_t:
		case _int32_t:
		SerialOutput->println(F("int32"));
		break;
		case _ppchar:
		SerialOutput->println(F("char*"));
		break;

#ifdef I_USE_GPS
			case _GPSLocation:
			SerialOutput->println(F("GPS"));
			break;
#endif
#ifdef I_USE_STRING
			case _String:
			SerialOutput->println(F("String"));
			break;
#endif
#ifdef I_USE_DATETIME
			case _DateTime:
			SerialOutput->println(F("DateTime"));
			break;
#endif
			case _FlashStringHelper:
			SerialOutput->println('F');
			break;

		}

	}
