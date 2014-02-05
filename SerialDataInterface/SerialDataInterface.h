/*
 * SerialDataInterface.h
 *
 *  Created on: Nov 28, 2012
 *      Author: BE04258
 */

#ifndef SERIALDATAINTERFACE_H_
#define SERIALDATAINTERFACE_H_
#include "Arduino.h"
#ifdef I_USE_GPS
#include "DataTypes.h"
#endif




extern Stream *SerialInput;
extern Stream *SerialOutput;
const char * makeChildName(const char* Name, const char* subName);


extern const __FlashStringHelper *LOOPDURATION;
extern const __FlashStringHelper *MULTIPLIER;
extern const __FlashStringHelper *PINVALUE;
extern const __FlashStringHelper *PIN;
extern const char ERROR[];
extern const char NAME_NOT_FOUND[];
extern const char NAME_TO_LONG[];
extern const char DONE[];

typedef enum
{
	_uint8_t, _int8_t, _uint16_t, _int16_t, _uint32_t, _int32_t, _char, _pcchar,	_bool, _FlashStringHelper,
#ifdef I_USE_GPS
	_GPSLocation,
#endif
#ifdef I_USE_STRING
	_String,
#endif
#ifdef I_USE_DATETIME
	_DateTime,
#endif
	_LastDataType
} DataTypes;
//extern const char* DataTypesNames[LastDataType];
#define    MOD_WRITE 			1  //Allow writes to this field
#define    MOD_SAVE  			2  //Allow this field to be saved
#define	   MOD_ERASE_ON_DUMP	4  //When a dump is done this field is erased String="" integer values=0 pchar=/0
class FieldInfo
{
	public:
		const char * myClassName;
		const __FlashStringHelper * myFieldName;
		DataTypes myType;
		uint8_t myModFlag;
		const FieldInfo & operator=(const FieldInfo &fieldInfo);
		bool operator==(const FieldInfo &fieldInfo) const;
		void dump() const;

};

class FieldData: public FieldInfo
{
	public:
		union
		{
				uint8_t* puint8_t;
				int8_t* pint8_t;
				uint16_t* puint16_t;
				uint32_t* puint32_t;
				int32_t* pint32_t;
				int16_t* pint16_t;
				char* pchar;
				const char ** ppcchar;
				bool* pbool;
				const __FlashStringHelper** ppFlashStringHelper;
#ifdef I_USE_STRING
				String* pString;
#endif
#ifdef I_USE_GPS
				GPSLocation* pGPSLocation;
#endif
#ifdef I_USE_DATETIME
				DateTime* pDateTime;
#endif

		} myValue;
		void dump() const;  //dumps the content to a stream
		//void dumpValue() const; //dumps the value to a stream
		bool setValue(const char * strValue);
		const char * getValue(char * buffer,uint8_t bufferSize)const; // get a string representation of the value

		//All typed generators to easily describe the fields
		void set(const char * ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, uint8_t* data)
		{
			myClassName=ClassName;
			myFieldName = FieldName;
			myType = _uint8_t;
			myModFlag = modFlag;
			myValue.puint8_t = data;
		}
		void set(const char * ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, int8_t* data)
		{
			myClassName=ClassName;
			myFieldName = FieldName;
			myType = _int8_t;
			myModFlag = modFlag;
			myValue.pint8_t = data;
		}
		void set(const char * ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, uint16_t* data)
		{
			myClassName=ClassName;
			myFieldName = FieldName;
			myType = _uint16_t;
			myModFlag = modFlag;
			myValue.puint16_t = data;
		}
		void set(const char * ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, uint32_t* data)
		{
			myClassName=ClassName;
			myFieldName = FieldName;
			myType = _uint32_t;
			myModFlag = modFlag;
			myValue.puint32_t = data;
		}
		void set(const char * ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, int32_t* data)
		{
			myClassName=ClassName;
			myFieldName = FieldName;
			myType = _int32_t;
			myModFlag = modFlag;
			myValue.pint32_t = data;
		}
		void set(const char * ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, int16_t* data)
		{
			myClassName=ClassName;
			myFieldName = FieldName;
			myType = _int16_t;
			myModFlag = modFlag;
			myValue.pint16_t = data;
		}
		void set(const char * ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, char* data)
		{
			myClassName=ClassName;
			myFieldName = FieldName;
			myType = _char;
			myModFlag = modFlag;
			myValue.pchar = data;
		}
		void set(const char * ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, const char ** data)
		{
			myClassName=ClassName;
			myFieldName = FieldName;
			myType = _pcchar;
			myModFlag = modFlag;
			myValue.ppcchar = data;
		}
#ifdef I_USE_STRING
		void set(const char * ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, String* data)
		{
			myClassName=ClassName;
			myFieldName = FieldName;
			myType = _String;
			myModFlag = modFlag;
			myValue.pString = data;
		}
#endif
		void set(const char * ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, bool* data)
		{
			myClassName=ClassName;
			myFieldName = FieldName;
			myType = _bool;
			myModFlag = modFlag;
			myValue.pbool = data;
		}
		void set(const char * ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, const __FlashStringHelper ** data)
			{
				myClassName=ClassName;
				myFieldName = FieldName;
				myType = _FlashStringHelper;
				myModFlag = modFlag;
				myValue.ppFlashStringHelper = data;
			}



#ifdef I_USE_GPS
		void set(const char * ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, GPSLocation* data)
		{
			myClassName=ClassName;
			myFieldName = FieldName;
			myType = _GPSLocation;
			myModFlag = modFlag;
			myValue.pGPSLocation = data;
		}
#endif
#ifdef I_USE_DATETIME
		void set(const char * ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, DateTime* data)
		{
			myClassName=ClassName;
			myFieldName = FieldName;
			myType = _DateTime;
			myModFlag = modFlag;
			myValue.pDateTime = data;
		}
#endif

};

class SerialDataInterface;
typedef bool (*FieldDataVisitor3)( FieldData& fieldData, const uint8_t field, SerialDataInterface* theClass);
typedef bool (*ClassDataVisitor)(const SerialDataInterface& data);

class SerialDataInterface
{
	protected:
		FieldData * myFields;
		SerialDataInterface** myChildren;
		uint8_t myNumFields;      //The number of fields myFields points to
		uint8_t myNumChildren;    //The number of children myChildren points to
		bool myIsDirty;        //Tells you whether one of the fields has been touched since the flag has been set to false
													 //Touching a child does not set the flag
		bool isInFieldRange(const uint8_t item) const;

	public:
		bool isDirty() const
		{
			return myIsDirty;
		}

		SerialDataInterface()
		{
			myFields = 0;
			myChildren = 0;
			myNumChildren = 0;
			myNumFields = 0;
			myIsDirty = false;
		}
		SerialDataInterface( FieldData * Fields, SerialDataInterface** Children, uint8_t NumFields, uint8_t NumChildren)
		{
			myFields = Fields;
			myChildren = Children;
			myNumChildren = NumChildren;
			myNumFields = NumFields;
			myIsDirty = false;
		}

		uint8_t getNumFields() const
		{
			return myNumFields;
		}
		;
		FieldData getFieldData(const uint8_t item) const;

		/**
		 * Overload this messsage if you want to do something smart when a value is set.
		 * It is not necessary to overload this method is you do not want to be alerted immediately
		 * Use the isDirty flag in your Loop Method to see if a value has changed
		 * I get problems when I enable virtual so I advice not to use it
		 */
		//virtual void SetField(const uint8_t field, const char *strValue);
		void setField(const uint8_t field, const char *strValue);

		//visiting methods
		bool visitAllFields(FieldDataVisitor3 visitorFunc);
		bool visitAllClasses(ClassDataVisitor visitorFunc) const;


};



#endif /* SERIALDATAINTERFACE_H_ */
