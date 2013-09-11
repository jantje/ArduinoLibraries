/*
 * DataInterface.cpp
 *
 *  Created on: Nov 28, 2012
 *      Author: BE04258
 */

#include "SerialDataInterface.h"
#ifdef DATETIME
#include "JanHelpers.h"
#include "JanDateTime.h"
#endif

/**
 * returns the field data.
 * The advantage of this method to directy accessing the array is that here a test is done wether
 * the field is in the range of the array.
 * If the field is not in the range of the array the first field is returned
 */
const FieldData SerialDataInterface::getFieldData(const uint8_t field) const
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
		Serial.println(F("This class is empty"));
		return false;
	}
	if (field>=myNumFields)
	{
		Serial.print(F("Class "));
		Serial.print(myFields[field].myClassName);
		Serial.print(F(" has: "));
		Serial.print(myNumFields);
		Serial.print(F(" fields; and you requested field: "));
		Serial.println(field);
		return false;
	}
	return true;
}

		/**
		 * Goes through all fields and call the method on the field.
		 * If the visitor function returns true the method stops and returns true
		 * This method is recursive. This eans for each child it cals itself.
		 * if all calls return false the method returns false
		 */
bool SerialDataInterface::visitAllFields(FieldDataVisitor visitorFunc) const
{
	for (int CurField = 0; CurField < myNumFields; CurField++)
	{
		if (visitorFunc(myFields[CurField])) return true;
	}
	for (int CurChild = 0; CurChild < myNumChildren; CurChild++)
	{
		if (myChildren[CurChild]->visitAllFields(visitorFunc)) return true;
	}
	return false;
}

bool SerialDataInterface::visitAllFields(FieldDataVisitor2 visitorFunc, Stream& stream) const
{
	for (int CurField = 0; CurField < myNumFields; CurField++)
	{
		if (visitorFunc(myFields[CurField], stream)) return true;
	}
	for (int CurChild = 0; CurChild < myNumChildren; CurChild++)
	{
		if (myChildren[CurChild]->visitAllFields(visitorFunc, stream)) return true;
	}
	return false;
}

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
	return ((strcmp_P((const char*) fieldInfo.myClassName, (const char*) myClassName) == 0)
			&& (strcmp_P((const char*) fieldInfo.myFieldName, (const char *) myFieldName) == 0) && (fieldInfo.myType == myType) && (fieldInfo.myModFlag == myModFlag));

}

/**
 * This method dumps the content of the FieldInfoobject to the stream provided
 */
void FieldInfo::dump(Stream& stream) const
{
	stream.print(F("name    \t"));
	stream.print(myClassName);
	stream.print('.');
	stream.println(myFieldName);
	stream.print(F("modflag \t"));
	stream.println(myModFlag);
	stream.print(F("Type     \t"));
	switch (myType)
	{
		case _bool:
		stream.println(F("bool"));
		break;
		case _uint8_t:
		stream.println(F("uint8_t"));
		break;
		case _int8_t:
		stream.println(F("int8_t"));
		break;
		case _uint16_t:
		stream.println(F("uint16_t"));
		break;
		case _int16_t:
		stream.println(F("int16_t"));
		break;
		case _char:
		stream.println(F("char string"));
		break;
		case _pcchar:
		stream.println(F("const char string pointer"));
		break;
		case _uint32_t:
		stream.println(F("unsigned long"));
		break;
		case _int32_t:
		stream.println(F("signed long"));
		break;
		case _GPSLocation:
		stream.println(F("GPSLocation"));
		break;
		case _String:
		stream.println(F("String"));
		break;
		case _DateTime:
		stream.println(F("DateTime"));
		break;
		case _FlashStringHelper:
		stream.println(F("FlashStringHelper"));
		break;
		default:
		stream.println(F("type not supported by FieldInfo::dump"));
		break;

	}

}

	/**
	 * This method dumps the value of the FieldData object to the stream provided
	 * There is no newline here.
	 */
void FieldData::dumpValue(Stream& stream) const
{
	boolean erase = ((this->myModFlag & MOD_ERASE_ON_DUMP) == MOD_ERASE_ON_DUMP);
	switch (myType)
	{
		case _bool:
			stream.print(*(myValue.pbool));
			if (erase) *myValue.pbool = 0;
			break;
		case _uint16_t:
			stream.print(*(myValue.puint16_t));
			if (erase) *myValue.puint16_t = 0;
			break;
		case _int16_t:
			stream.print(*(myValue.pint16_t));
			if (erase) *myValue.pint16_t = 0;
			break;
		case _uint8_t:
			stream.print(*(myValue.puint8_t));
			if (erase) *myValue.puint8_t = 0;
			break;
		case _int8_t:
			stream.print(*(myValue.pint8_t));
			if (erase) *myValue.pint8_t = 0;
			break;
		case _pcchar:
			stream.print("\"");
			stream.print(*myValue.ppcchar);
			stream.print("\"");
			break;
		case _char:
			stream.print(myValue.pchar);
			if (erase) *myValue.pchar = 0;
			break;
		case _uint32_t:
			stream.print(*myValue.puint32_t);
			if (erase) *myValue.puint32_t = 0;
			break;
		case _int32_t:
			stream.print(*myValue.pint32_t);
			if (erase) *myValue.pint32_t = 0;
			break;
#ifdef GPSLOCATION
		case _GPSLocation:
			stream.print(*myValue.pGPSLocation);
			if (erase) stream.println(F("Unsupported action MOD_ERASE_ON_DUMP on GPSLocation"));
			break;
#endif
		case _String:
			stream.print("\"");
			stream.print(*myValue.pString);
			stream.print("\"");
			if (erase) *myValue.pString = "";
			break;
		case _FlashStringHelper:
			stream.print("\"");
			stream.print(*myValue.ppFlashStringHelper);
			stream.print("\"");
			if (erase) stream.println(F("Unsupported action MOD_ERASE_ON_DUMP on GPSLocation"));
			break;
#ifdef DATETIME
		case _DateTime:
			stream.print(DateTimeToString(*myValue.pDateTime));
			if (erase) stream.println(F("Unsupported action MOD_ERASE_ON_DUMP on DateTime"));
			break;
#endif
		default:
			stream.println(F("the function FieldData::dumpValue does not know this type"));
		}
	}

			/**
			 * This method dumps the content of the FieldData object to the stream provided
			 */
void FieldData::dump(Stream& stream) const
{
	FieldInfo::dump(stream);
	stream.print(F("Value    \t"));
	dumpValue(stream);
	stream.println();
	stream.println();
}

/**
 * This method sets the value of the FieldData object based on the content of a string.
 * The string must contain a valid string representation of the type the field is
 */
bool FieldData::setValue(const char* strValue)
{
	if (!(myModFlag & MOD_WRITE))
	{
		Serial.print(F("object "));
		Serial.print(myFieldName);
		Serial.print(F(" from class "));
		Serial.print(myClassName);
		Serial.print(F(" is not writable!!"));
		return false;
	}
	switch (myType)
	{
		case _bool:
		*(myValue.pbool)= (bool) atoi(strValue);
		break; case _uint8_t:
		*(myValue.puint8_t)= (uint8_t) atoi(strValue);
		break;
		case _int8_t:
		*(myValue.pint8_t)= (int8_t) atoi(strValue);
		break;
		case _uint16_t:
		*(myValue.puint16_t)= (uint16_t)atol(strValue);
		break;
		case _int16_t:
		*(myValue.pint16_t)=atoi(strValue);
		break;
		case _char:
		strcpy( (char *) myValue.pchar,strValue); /*TODO add code to limit the length of the char array*/
		break;
		case _uint32_t:
		*(myValue.puint32_t)= (uint32_t)strtoul(strValue,NULL,10);
		break;
		case _int32_t:
		*(myValue.pint32_t)= (int32_t)strtol(strValue,NULL,10);
		break;

		case _String:
		*( myValue.pString)=strValue;
		break;
		case _GPSLocation:
		case _DateTime:
		case _pcchar:
		case _FlashStringHelper:
		default:
		Serial.println(F("the function FieldData::setValue does not support this type"));
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

void FieldData::set(const char* ClassName, const __FlashStringHelper* FieldName, uint8_t modFlag, uint8_t* data)
{
	myClassName=ClassName;
	myFieldName = FieldName;
	myType = _uint8_t;
	myModFlag = modFlag;
	myValue.puint8_t = data;
}

void FieldData::set(const char* ClassName, const __FlashStringHelper* FieldName, uint8_t modFlag, int8_t* data)
{
	myClassName=ClassName;
	myFieldName = FieldName;
	myType = _int8_t;
	myModFlag = modFlag;
	myValue.pint8_t = data;
}

void FieldData::set(const char* ClassName, const __FlashStringHelper* FieldName, uint8_t modFlag, uint16_t* data)
{
	myClassName=ClassName;
	myFieldName = FieldName;
	myType = _uint16_t;
	myModFlag = modFlag;
	myValue.puint16_t = data;
}

void FieldData::set(const char* ClassName, const __FlashStringHelper* FieldName, uint8_t modFlag, uint32_t* data)
{
	myClassName=ClassName;
	myFieldName = FieldName;
	myType = _uint32_t;
	myModFlag = modFlag;
	myValue.puint32_t = data;
}

void FieldData::set(const char* ClassName, const __FlashStringHelper* FieldName, uint8_t modFlag, int32_t* data)
{
	myClassName=ClassName;
	myFieldName = FieldName;
	myType = _int32_t;
	myModFlag = modFlag;
	myValue.pint32_t = data;
}

void FieldData::set(const char* ClassName, const __FlashStringHelper* FieldName, uint8_t modFlag, int16_t* data)
{
	myClassName=ClassName;
	myFieldName = FieldName;
	myType = _int16_t;
	myModFlag = modFlag;
	myValue.pint16_t = data;
}

void FieldData::set(const char* ClassName, const __FlashStringHelper* FieldName, uint8_t modFlag, char* data)
{
	myClassName=ClassName;
	myFieldName = FieldName;
	myType = _char;
	myModFlag = modFlag;
	myValue.pchar = data;
}

void FieldData::set(const char* ClassName, const __FlashStringHelper* FieldName, uint8_t modFlag, const char** data)
{
	myClassName=ClassName;
	myFieldName = FieldName;
	myType = _pcchar;
	myModFlag = modFlag;
	myValue.ppcchar = data;
}

#ifdef GPSLOCATION
void FieldData::set(const char* ClassName, const __FlashStringHelper* FieldName, uint8_t modFlag, GPSLocation* data)
{
	myClassName=ClassName;
	myFieldName = FieldName;
	myType = _GPSLocation;
	myModFlag = modFlag;
	myValue.pGPSLocation = data;
}
#endif

#ifdef DATETIME
void FieldData::set(const char* ClassName, const __FlashStringHelper* FieldName, uint8_t modFlag, DateTime* data)
{
	myClassName=ClassName;
	myFieldName = FieldName;
	myType = _DateTime;
	myModFlag = modFlag;
	myValue.pDateTime = data;
}
#endif

void FieldData::set(const char* ClassName, const __FlashStringHelper* FieldName, uint8_t modFlag, String* data)
{
	myClassName=ClassName;
	myFieldName = FieldName;
	myType = _String;
	myModFlag = modFlag;
	myValue.pString = data;
}

void FieldData::set(const char* ClassName, const __FlashStringHelper* FieldName, uint8_t modFlag, bool* data)
{
	myClassName=ClassName;
	myFieldName = FieldName;
	myType = _bool;
	myModFlag = modFlag;
	myValue.pbool = data;
}

void FieldData::set(const char* ClassName, const __FlashStringHelper* FieldName, uint8_t modFlag, const __FlashStringHelper** data)
{
	myClassName=ClassName;
	myFieldName = FieldName;
	myType = _FlashStringHelper;
	myModFlag = modFlag;
	myValue.ppFlashStringHelper = data;
}
