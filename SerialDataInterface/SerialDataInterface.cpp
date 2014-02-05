/*
 * DataInterface.cpp
 *
 *  Created on: Nov 28, 2012
 *      Author: BE04258
 */

#include "SerialDataInterface.h"
#ifdef I_USE_DATETIME
#endif

static char __loopduration[] PROGMEM ="Loopduration";
static char __Multiplier[] PROGMEM ="Multiplier";
static char __PinValue[] PROGMEM ="PinValue";
static char __Pin[] PROGMEM = "Pin";
const __FlashStringHelper *LOOPDURATION = (const __FlashStringHelper *) __loopduration;
const __FlashStringHelper *MULTIPLIER = (const __FlashStringHelper *) __Multiplier;
const __FlashStringHelper *PINVALUE = (const __FlashStringHelper *) __PinValue;
const __FlashStringHelper *PIN = (const __FlashStringHelper *) __Pin;

//const char UNSUPPORTED_ACTION[] = "Unsupported action MOD_ERASE_ON_DUMP";
const char ERROR[] = "ERROR: ";

/**
 * returns the field data.
 * The advantage of this method to directly accessing the array is that here a test is done wether
 * the field is in the range of the array.
 * If the field is not in the range of the array the first field is returned
 */
FieldData SerialDataInterface::getFieldData(const uint8_t field) const
{
	if (!isInFieldRange(field)) return myFields[0];
	return myFields[field];
}

/**
 * Sets the value of a field based on string input.
 * This method does a range check off the array
 */
void SerialDataInterface::setField(const uint8_t field, const char *strValue)
{
//	MYSERIAL.print("DEBUG Setting: ");
//	MYSERIAL.print(myFields[field].FieldName);
//	MYSERIAL.print(" to: ");
//	MYSERIAL.println(strValue);
	if (!isInFieldRange(field)) return;
	myFields[field].setValue(strValue);
	myIsDirty = true;

}

/**
 * This method does a range check for the field and the field array
 * If the field is not in range a message is send to MYSERIAL
 * It returns true if the field is in range (and dumps nothing to MYSERIAL)
 * Returns false if not in range and a message to serial is dumped.
 */
bool SerialDataInterface::isInFieldRange(const uint8_t field) const
{
	if (myNumFields == 0)
	{
		Serial.println(ERROR);
		return false;
	}
	if (field >= myNumFields)
	{
//		Serial.print(F("Class "));
//		Serial.print(myFields[field].myClassName);
//		Serial.print(F(" has: "));
//		Serial.print(myNumFields);
//		Serial.print(F(" fields; and you requested field: "));
//		Serial.println(field);
		return false;
	}
	return true;
}

//		/**
//		 * Goes through all fields and call the method on the field.
//		 * If the visitor function returns true the method stops and returns true
//		 * This method is recursive. This eans for each child it cals itself.
//		 * if all calls return false the method returns false
//		 */
//bool SerialDataInterface::visitAllFields(FieldDataVisitor visitorFunc) const
//{
//	for (int CurField = 0; CurField < myNumFields; CurField++)
//	{
//		if (visitorFunc(myFields[CurField])) return true;
//	}
//	for (int CurChild = 0; CurChild < myNumChildren; CurChild++)
//	{
//		if (myChildren[CurChild]->visitAllFields(visitorFunc)) return true;
//	}
//	return false;
//}

/**
 * same as above but with different visitor function
 */
bool SerialDataInterface::visitAllFields(FieldDataVisitor3 visitorFunc)
{
	for (int CurField = 0; CurField < myNumFields; CurField++)
	{
		if (visitorFunc(myFields[CurField], CurField, this)) return true;
	}
	for (int CurChild = 0; CurChild < myNumChildren; CurChild++)
	{
		if (myChildren[CurChild]->visitAllFields(visitorFunc)) return true;
	}
	return false;
}

bool SerialDataInterface::visitAllClasses(ClassDataVisitor visitorFunc) const
{
//	MYSERIAL.println("Processing DataInterface::VisitAllClasses ClassDataVisitor this  ");
	if (visitorFunc(*this)) return true;
	for (int CurChild = 0; CurChild < myNumChildren; CurChild++)
	{
//  	MYSERIAL.print("Processing DataInterface::VisitAllClasses ClassDataVisitor CurChild  ");
//  	MYSERIAL.println(CurChild);
		if (myChildren[CurChild]->visitAllClasses(visitorFunc)) return true;
	}
	return false;
}

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
		case _bool:
		SerialOutput->println(F("bool"));
		break;
		case _uint8_t:
		SerialOutput->println("uint8");
		break;
		case _int8_t:
		SerialOutput->println(F("int8"));
		break;
		case _uint16_t:
		SerialOutput->println("uint16");
		break;
		case _int16_t:
		SerialOutput->println(F("int16"));
		break;
		case _uint32_t:
		SerialOutput->println("uint32");
		break;
		case _int32_t:
		SerialOutput->println(F("int32"));
		break;
		case _char:
		SerialOutput->println(F("char"));
		break;
		case _pcchar:
		SerialOutput->println(F("c char*"));
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

	/**
	 * This method dumps the content of the FieldData object to the stream provided
	 */
void FieldData::dump() const
{
	FieldInfo::dump();
	SerialOutput->print(F("Value\t"));
	SerialOutput->print(getValue(commonlyUsedBuffer,commonlyUsedBuffersize));
	SerialOutput->println();
	SerialOutput->println();
}

/**
 * This method sets the value of the FieldData object based on the content of a string.
 * The string must contain a valid string representation of the type the field is
 */
bool FieldData::setValue(const char* strValue)
{
	if (!(myModFlag & MOD_WRITE))
	{
//		Serial.print(F("object "));
//		Serial.print(myFieldName);
//		Serial.print(F(" from class "));
//		Serial.print(myClassName);
//		Serial.print(F(" is not writable!!"));
		Serial.print(ERROR);
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
		case _char:
			strcpy((char *) myValue.pchar, strValue); /*TODO add code to limit the length of the char array*/
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
#endif
#ifdef I_USE_DATETIME
			case _DateTime:
#endif
		case _pcchar:
		case _FlashStringHelper:
			break;
			return false;
	}
	return true;
}

const char * makeChildName(const char* Name, const char* subName)
{
	int theSize = strlen(Name) + strlen(subName) + 2;
	char * ret = (char*) malloc(theSize);
	snprintf(ret, theSize, "%s.%s", Name, subName);
	return ret;
}

//void FieldData::set(const char* ClassName, const __FlashStringHelper* FieldName, uint8_t modFlag, uint8_t* data)
//{
//	myClassName=ClassName;
//	myFieldName = FieldName;
//	myType = _uint8_t;
//	myModFlag = modFlag;
//	myValue.puint8_t = data;
//}

//void FieldData::set(const char* ClassName, const __FlashStringHelper* FieldName, uint8_t modFlag, int8_t* data)
//{
//	myClassName=ClassName;
//	myFieldName = FieldName;
//	myType = _int8_t;
//	myModFlag = modFlag;
//	myValue.pint8_t = data;
//}

//void FieldData::set(const char* ClassName, const __FlashStringHelper* FieldName, uint8_t modFlag, uint16_t* data)
//{
//	myClassName=ClassName;
//	myFieldName = FieldName;
//	myType = _uint16_t;
//	myModFlag = modFlag;
//	myValue.puint16_t = data;
//}

//void FieldData::set(const char* ClassName, const __FlashStringHelper* FieldName, uint8_t modFlag, uint32_t* data)
//{
//	myClassName=ClassName;
//	myFieldName = FieldName;
//	myType = _uint32_t;
//	myModFlag = modFlag;
//	myValue.puint32_t = data;
//}

//void FieldData::set(const char* ClassName, const __FlashStringHelper* FieldName, uint8_t modFlag, int32_t* data)
//{
//	myClassName=ClassName;
//	myFieldName = FieldName;
//	myType = _int32_t;
//	myModFlag = modFlag;
//	myValue.pint32_t = data;
//}

//void FieldData::set(const char* ClassName, const __FlashStringHelper* FieldName, uint8_t modFlag, int16_t* data)
//{
//	myClassName=ClassName;
//	myFieldName = FieldName;
//	myType = _int16_t;
//	myModFlag = modFlag;
//	myValue.pint16_t = data;
//}

//void FieldData::set(const char* ClassName, const __FlashStringHelper* FieldName, uint8_t modFlag, char* data)
//{
//	myClassName=ClassName;
//	myFieldName = FieldName;
//	myType = _char;
//	myModFlag = modFlag;
//	myValue.pchar = data;
//}

//void FieldData::set(const char* ClassName, const __FlashStringHelper* FieldName, uint8_t modFlag, const char** data)
//{
//	myClassName=ClassName;
//	myFieldName = FieldName;
//	myType = _pcchar;
//	myModFlag = modFlag;
//	myValue.ppcchar = data;
//}

//#ifdef I_USE_GPS
//void FieldData::set(const char* ClassName, const __FlashStringHelper* FieldName, uint8_t modFlag, GPSLocation* data)
//{
//	myClassName=ClassName;
//	myFieldName = FieldName;
//	myType = _GPSLocation;
//	myModFlag = modFlag;
//	myValue.pGPSLocation = data;
//}
//#endif

//#ifdef I_USE_DATETIME
//void FieldData::set(const char* ClassName, const __FlashStringHelper* FieldName, uint8_t modFlag, DateTime* data)
//{
//	myClassName=ClassName;
//	myFieldName = FieldName;
//	myType = _DateTime;
//	myModFlag = modFlag;
//	myValue.pDateTime = data;
//}
//#endif

//#ifdef I_USE_STRING
//void FieldData::set(const char* ClassName, const __FlashStringHelper* FieldName, uint8_t modFlag, String* data)
//{
//	myClassName=ClassName;
//	myFieldName = FieldName;
//	myType = _String;
//	myModFlag = modFlag;
//	myValue.pString = data;
//}
//#endif

//void FieldData::set(const char* ClassName, const __FlashStringHelper* FieldName, uint8_t modFlag, bool* data)
//{
//	myClassName=ClassName;
//	myFieldName = FieldName;
//	myType = _bool;
//	myModFlag = modFlag;
//	myValue.pbool = data;
//}

//void FieldData::set(const char* ClassName, const __FlashStringHelper* FieldName, uint8_t modFlag, const __FlashStringHelper** data)
//{
//	myClassName=ClassName;
//	myFieldName = FieldName;
//	myType = _FlashStringHelper;
//	myModFlag = modFlag;
//	myValue.ppFlashStringHelper = data;
//}

/**getValue
 * returns the value as a string.
 * buffer a buffer that can be used to create the string. May or may not contain the value
 * bufferSize the size of the buffer
 *
 * returns a pointer to a string that represents the value.
 */
const char* FieldData::getValue(char * buffer,uint8_t bufferSize) const
{
	uint32_t val_uint32_t=0;
	int32_t val_int32_t=0;
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
		case _pcchar:
			snprintf(buffer, bufferSize, "\"%s\"", *(myValue.ppcchar));
			break;
		case _char:
			snprintf(buffer, bufferSize, "%c", *(myValue.pchar));
			break;
		case _FlashStringHelper:
			strlcpy_P(buffer, (const prog_char *) *myValue.ppFlashStringHelper, bufferSize);
			break;
#ifdef I_USE_GPS
			case _GPSLocation:
			myValue.pGPSLocation->getValue(buffer,bufferSize);
			break;
#endif
#ifdef I_USE_STRING
			case _String:
			myValue.pString->getBytes((unsigned char *)buffer,bufferSize,0);
			break;
#endif
#ifdef I_USE_DATETIME
			case _DateTime:
				 snprintf(buffer,bufferSize,"%d-%d-%d %d:%d::%d",(int)myValue.pDateTime->year(),(int)myValue.pDateTime->month(),(int)myValue.pDateTime->day(),myValue.pDateTime->hour(),myValue.pDateTime->minute(),myValue.pDateTime->second());
			break;
#endif
	}
	if (!done)
	{
		if (val_uint32_t != 0)
		{
			snprintf(buffer, bufferSize, "%lu", val_uint32_t);
		} else
		{
			snprintf(buffer, bufferSize, "%li", val_int32_t);
		}
	}
	return buffer;
}

