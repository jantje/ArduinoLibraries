/*
 * SerialDataInterface.h
 *
 *  Created on: Nov 28, 2012
 *      Author: BE04258
 */

#ifndef SERIALDATAINTERFACE_H_
#define SERIALDATAINTERFACE_H_
#include "Arduino.h"
#ifdef GPSLOCATION
#include "DataTypes.h"
#endif
#ifdef DATETIME
#include "RTClib.h"
#endif

typedef enum
{
	_uint8_t, _int8_t, _uint16_t, _int16_t, _uint32_t, _int32_t, _char, _pcchar, _GPSLocation, _String, _DateTime, _bool, _FlashStringHelper, _LastDataType
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
		void dump(Stream& stream) const;

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
				String* pString;
				bool* pbool;
				const __FlashStringHelper** ppFlashStringHelper;
#ifdef GPSLOCATION
				GPSLocation* pGPSLocation;
#endif
#ifdef DATETIME
				DateTime* pDateTime;
#endif

		} myValue;
		void dump(Stream& stream) const;  //dumps the content to a stream
		void dumpValue(Stream& stream) const; //dumps the value to a stream
		bool setValue(const char * strValue);

		//All typed generators to easily describe the fields
		void set(const char * ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, uint8_t* data);
		void set(const char * ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, int8_t* data);
		void set(const char * ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, uint16_t* data);
		void set(const char * ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, uint32_t* data);
		void set(const char * ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, int32_t* data);
		void set(const char * ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, int16_t* data);
		void set(const char * ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, char* data);
		void set(const char * ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, const char ** data);
		void set(const char * ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, String* data);
		void set(const char * ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, bool* data);
		void set(const char * ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, const __FlashStringHelper ** data);

#ifdef GPSLOCATION
		void set(const char * ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, GPSLocation* data);
#endif
#ifdef DATETIME
		void set(const char * ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, DateTime* data);
#endif



};

class SerialDataInterface;
typedef bool (*FieldDataVisitor)(const FieldData& fieldData);
typedef bool (*FieldDataVisitor2)(const FieldData& fieldData,Stream& stream);
typedef bool (*FieldDataVisitor3)(const FieldData& fieldData, const uint8_t field, SerialDataInterface* theClass);

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
		const FieldData getFieldData(const uint8_t item) const;

		/**
		 * Overload this messsage if you want to do something smart when a value is set.
		 * It is not necessary to overload this method is you do not want to be alerted immediately
		 * Use the isDirty flag in your Loop Method to see if a value has changed
		 * I get problems when I enable virtual so I advice not to use it
		 */
		//virtual void SetField(const uint8_t field, const char *strValue);
		void setField(const uint8_t field, const char *strValue);

		//visiting methods
		bool visitAllFields(FieldDataVisitor visitorFunc) const;
		bool visitAllFields(FieldDataVisitor2 visitorFunc,Stream& stream) const;
		bool visitAllFields(FieldDataVisitor3 visitorFunc);
		bool visitAllClasses(ClassDataVisitor visitorFunc) const;


};

const char * makeChildName(const char* Name, const char* subName);

#endif /* SERIALDATAINTERFACE_H_ */
