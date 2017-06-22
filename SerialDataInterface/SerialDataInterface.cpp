/*
 * FieldData.cpp
 *
 *  Created on: Feb 8, 2014
 *      Author: jan
 */

#include "SerialDataInterface.h"
const char LOOPDURATION[] PROGMEM ="Loopduration";
const char STATENAME[] PROGMEM="StateName";
const char STATE[] PROGMEM="State";
const char MULTIPLIER[] PROGMEM ="Multiplier";
const char MULTIPLIER_DIV_10[] PROGMEM ="Multiplier/10";
const char MULTIPLIER_DIV_1000[] PROGMEM ="Multiplier/1000";

const char PINVALUE[] PROGMEM ="PinValue";
const char PIN[] PROGMEM = "Pin";
const char ERROR[] PROGMEM = "ERROR: ";

uint8_t lastFieldIndex = 0;

/**
 * This method dumps the content of the FieldData object to the stream provided
 */
void FieldData::dump() const
{
	FieldInfo::dump();
	SerialOutput.print(F("Value\t"));
	SerialOutput.print(getValue(commonlyUsedBuffer, commonlyUsedBuffersize));
	SerialOutput.println();
	SerialOutput.println();
}

/**
 * This method sets the value of the FieldData object based on the content of a string.
 * The string must contain a valid string representation of the type the field is
 */
bool FieldData::setValue(const char* strValue)
{
	if (!(myModFlag & MOD_WRITE))
	{
		SerialError.print(ERROR);
		return false;
	}
	uint32_t uint32Val = (uint32_t) strtoul(strValue, NULL, 10);
	int32_t int32Val = (int32_t) strtol(strValue, NULL, 10);

	switch (myType)
	{
		case _bool:
			*(myValue.pbool) = (bool) int32Val;
			break;
		case _uint8_t:
			*(myValue.puint8_t) = (uint8_t) uint32Val;
			break;
		case _int8_t:
			*(myValue.pint8_t) = (int8_t) int32Val;
			break;
		case _uint16_t:
			*(myValue.puint16_t) = (uint16_t) uint32Val;
			break;
		case _int16_t:
			*(myValue.pint16_t) = int32Val;
			break;
		case _uint32_t:
			*(myValue.puint32_t) = uint32Val;
			break;
		case _int32_t:
			*(myValue.pint32_t) = int32Val;
			break;
#ifdef I_USE_STRING
			case _String:
			*( myValue.pString)=strValue;
			break;
#endif
#ifdef I_USE_GPS
		case _GPSLocation:
		{
			const char * plat=strValue+5;
			char * pComma=(char * )strchr(plat,',');
			if (pComma==0) return false;
			pComma[0]=0;
			const char * plong=pComma+8;
			myValue.pGPSLocation->myLatitude=atol(plat);
			myValue.pGPSLocation->myLongitude=atol(plong);

			//snprintf(buffer,buffersize,"Lat: %li, long: %li",myLatitude,myLongitude
		}
			break;
#endif
#ifdef I_USE_DATETIME
		case _DateTime:
#endif
		case _ppchar:
		case _FlashStringHelper:
			break;
			return false;
	}
	return true;
}



/**getValue
 * returns the value as a string.
 * buffer a buffer that can be used to create the string. May or may not contain the value
 * bufferSize the size of the buffer
 *
 * returns a pointer to a string that represents the value.
 */
const char* FieldData::getValue(char * buffer, int bufferSize) const
{
	uint32_t val_uint32_t = 0;
	int32_t val_int32_t = 0;
	bool done = true;

	switch (myType)
	{
		case _bool:
			return "false\0true" + 6 * (*myValue.pbool);
			break;
		case _uint16_t:
			val_uint32_t = *(myValue.puint16_t);
			done = false;
			break;
		case _int16_t:
			val_int32_t = *(myValue.pint16_t);
			done = false;
			break;
		case _uint8_t:
			val_uint32_t = *(myValue.puint8_t);
			done = false;
			break;
		case _int8_t:
			val_int32_t = *(myValue.pint8_t);
			done = false;
			break;
		case _uint32_t:
			val_uint32_t = *(myValue.puint32_t);
			done = false;
			break;
		case _int32_t:
			val_int32_t = *(myValue.pint32_t);
			done = false;
			break;
		case _ppchar:
			if (snprintf(buffer, bufferSize, "\"%s\"", myValue.ppchar) >=bufferSize)
			{
				//not everything fitted in the buffer make sure we have a double quote at the end
				buffer[bufferSize-2]='"';
			}
			if ((myModFlag&MOD_ERASE_ON_DUMP)==MOD_ERASE_ON_DUMP)
			{
				myValue.ppchar[0]=0;
			}
			break;
#ifdef I_USE_PROGMEM
		case _FlashStringHelper:
			strlcpy_P(buffer, (const char *) *myValue.ppFlashStringHelper, bufferSize);
			break;
#endif
#ifdef I_USE_GPS
		case _GPSLocation:
			myValue.pGPSLocation->getValue(buffer, bufferSize);
			break;
#endif
#ifdef I_USE_STRING
			case _String:
			myValue.pString->getBytes((unsigned char *)buffer,bufferSize,0);
			if ((myModFlag&MOD_ERASE_ON_DUMP)==MOD_ERASE_ON_DUMP)
			{
				(*myValue.pString)="";
			}
			break;
#endif
#ifdef I_USE_DATETIME
		case _DateTime:
			snprintf(buffer, bufferSize, "%d-%d-%d %d:%d::%d", (int) myValue.pDateTime->year(), (int) myValue.pDateTime->month(), (int) myValue.pDateTime->day(), myValue.pDateTime->hour(), myValue.pDateTime->minute(), myValue.pDateTime->second());
			break;
#endif
	}
	if (!done)
	{
		if (val_uint32_t != 0)
		{
			snprintf(buffer, bufferSize, "%lu", (long unsigned int)val_uint32_t);
		} else
		{
			snprintf(buffer, bufferSize, "%li", (long int) val_int32_t);
		}
	}
	return buffer;
}

/**
 * This method uses the global Variable MyMessage to see whether it matches the classname.fieldname
 * as available in the fieldData Object.
 * It returns true if equal else it returns false
 */
bool isFieldMatch(const FieldInfo& fieldData, const char * MyMessage)
{
	size_t ClassNameLength = strlen_P((char*) fieldData.myClassName);
	size_t FieldNameLength = strlen_P((char*) fieldData.myFieldName);

	if (strncmp_P(MyMessage, (const char *) fieldData.myClassName, ClassNameLength) != 0) return false; /*Class name does not match*/
	if (MyMessage[ClassNameLength] != CLASSSEPERATOR) return false;/*The Classname is to short*/
	/*The class name is a match*/
	if (strncmp_P(MyMessage + ClassNameLength + 1, (char*) fieldData.myFieldName, FieldNameLength) != 0) return false;/*field name does not match*/
	char LastChar = MyMessage[ClassNameLength + 1 + FieldNameLength];
	if ((LastChar == 0) || (LastChar == ' ') || (LastChar == '=')) return true;
	return false;/*The Fieldname is to short*/
}

FieldData* FieldData::findField(const char * fieldName)
{
	for (int curField = 0; curField < lastFieldIndex; curField++)
	{
		if (isFieldMatch(AllFields[curField], fieldName))
		{
			return &AllFields[curField];
		}
	}
	return 0;
}

void FieldData::visitAllFields(FieldVisitor visitorfunc, uint8_t all)
{
	for (int curField = 0; curField < lastFieldIndex; curField++)
	{
		if (all || ((AllFields[curField].myModFlag & MOD_SAVE) == MOD_SAVE))
		{
			(*visitorfunc)(AllFields[curField]);
		}
	}
}
//
//void FieldData::setNext( const __FlashStringHelper * FieldName, uint8_t modFlag, uint16_t* data)
//{
//	AllFields[lastFieldIndex].myClassName=AllFields[lastFieldIndex-1].myClassName;
//	AllFields[lastFieldIndex].myFieldName = FieldName;
//	AllFields[lastFieldIndex].myType = _uint16_t;
//	AllFields[lastFieldIndex].myModFlag = modFlag;
//	AllFields[lastFieldIndex++].myValue.puint16_t = data;
//}
//void FieldData::setNext( const __FlashStringHelper * FieldName, uint8_t modFlag, int16_t* data)
//{
//	AllFields[lastFieldIndex].myClassName=AllFields[lastFieldIndex-1].myClassName;
//	AllFields[lastFieldIndex].myFieldName = FieldName;
//	AllFields[lastFieldIndex].myType = _int16_t;
//	AllFields[lastFieldIndex].myModFlag = modFlag;
//	AllFields[lastFieldIndex++].myValue.pint16_t = data;
//}
//
//void FieldData::setNext( const __FlashStringHelper * FieldName, uint8_t modFlag, uint32_t* data)
//{
//	AllFields[lastFieldIndex].myClassName=AllFields[lastFieldIndex-1].myClassName;
//	AllFields[lastFieldIndex].myFieldName = FieldName;
//	AllFields[lastFieldIndex].myType = _uint32_t;
//	AllFields[lastFieldIndex].myModFlag = modFlag;
//	AllFields[lastFieldIndex++].myValue.puint32_t = data;
//}
//
//void FieldData::setNext( const __FlashStringHelper * FieldName, uint8_t modFlag, uint8_t* data)
//{
//	AllFields[lastFieldIndex].myClassName=AllFields[lastFieldIndex-1].myClassName;
//	AllFields[lastFieldIndex].myFieldName = FieldName;
//	AllFields[lastFieldIndex].myType = _uint8_t;
//	AllFields[lastFieldIndex].myModFlag = modFlag;
//	AllFields[lastFieldIndex++].myValue.puint8_t = data;
//}
//
//void FieldData::setNext( const __FlashStringHelper * FieldName, uint8_t modFlag, int8_t* data)
//{
//	AllFields[lastFieldIndex].myClassName=AllFields[lastFieldIndex-1].myClassName;
//	AllFields[lastFieldIndex].myFieldName = FieldName;
//	AllFields[lastFieldIndex].myType = _int8_t;
//	AllFields[lastFieldIndex].myModFlag = modFlag;
//	AllFields[lastFieldIndex++].myValue.pint8_t = data;
//}
//
//void FieldData::setNext( const __FlashStringHelper * FieldName, uint8_t modFlag, bool* data)
//{
//	AllFields[lastFieldIndex].myClassName=AllFields[lastFieldIndex-1].myClassName;
//	AllFields[lastFieldIndex].myFieldName = FieldName;
//	AllFields[lastFieldIndex].myType = _bool;
//	AllFields[lastFieldIndex].myModFlag = modFlag;
//	AllFields[lastFieldIndex++].myValue.pbool = data;
//}
//

