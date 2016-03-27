/*
 * FieldData.h
 *
 *  Created on: Feb 8, 2014
 *      Author: jan
 */

#ifndef FIELDDATA_H_
#define FIELDDATA_H_
#include "FieldInfo.h"

#if defined(I_USE_GPS) || defined(I_USE_DATETIME)
#include "DataTypes.h"
#endif
#include "SerialStringReader.h" //needed for the streams




//Below are some common used string defined in progmem to reduce memory usage
extern const char LOOPDURATION[] PROGMEM;
extern const char MULTIPLIER[] PROGMEM;
extern const char PINVALUE[] PROGMEM;
extern const char PIN[] PROGMEM;
extern const char ERROR[] PROGMEM;
extern const char NAME_NOT_FOUND[]PROGMEM;
extern const char NAME_TO_LONG[]PROGMEM;
extern const char DONE[]PROGMEM;
extern const char STATENAME[] PROGMEM;
#define CLASSSEPERATOR '.'
#define FIELDSEPERATOR ';'

class FieldData;
extern FieldData AllFields[];
extern uint8_t lastFieldIndex;
typedef void (*FieldVisitor)( FieldData& fieldData);

class FieldData: public FieldInfo
{

	public:

		union
		{
			void* pvoid;
				uint8_t* puint8_t;
				int8_t* pint8_t;
				uint16_t* puint16_t;
				uint32_t* puint32_t;
				int32_t* pint32_t;
				int16_t* pint16_t;
				char * ppchar;
				bool* pbool;
#ifdef I_USE_PROGMEM
				const __FlashStringHelper** ppFlashStringHelper;
#endif
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
		bool setValue(const char * strValue);
		const char * getValue(char * buffer,int bufferSize)const; // get a string representation of the value

		static FieldData* findField(const char * fieldName);
		static void visitAllFields(FieldVisitor visitorfunc,uint8_t all);


		static void setNext( const __FlashStringHelper * FieldName, uint8_t modFlag, uint16_t* data)
		{
			set(AllFields[lastFieldIndex-1].myClassName,FieldName,modFlag,data);
		}
		static void setNext( const __FlashStringHelper * FieldName, uint8_t modFlag, int16_t* data)
		{
			set(AllFields[lastFieldIndex-1].myClassName,FieldName,modFlag,data);
		}
		static void setNext( const __FlashStringHelper * FieldName, uint8_t modFlag, uint32_t* data)
		{
			set(AllFields[lastFieldIndex-1].myClassName,FieldName,modFlag,data);
		}
		static void setNext( const __FlashStringHelper * FieldName, uint8_t modFlag, int32_t* data)
		{
			set(AllFields[lastFieldIndex-1].myClassName,FieldName,modFlag,data);
		}
		static void setNext( const __FlashStringHelper * FieldName, uint8_t modFlag, uint8_t* data)
		{
			set(AllFields[lastFieldIndex-1].myClassName,FieldName,modFlag,data);
		}
		static void setNext( const __FlashStringHelper * FieldName, uint8_t modFlag, int8_t* data)
		{
			set(AllFields[lastFieldIndex-1].myClassName,FieldName,modFlag,data);
		}
		static void setNext( const __FlashStringHelper * FieldName, uint8_t modFlag, bool* data)
		{
			set(AllFields[lastFieldIndex-1].myClassName,FieldName,modFlag,data);
		}
		static void setNext( const __FlashStringHelper * FieldName, uint8_t modFlag,  char * data)
		{
			set(AllFields[lastFieldIndex-1].myClassName,FieldName,modFlag,data);
		}
		static void set(const __FlashStringHelper *  ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, void* data,DataTypes dataType)
		{
			AllFields[lastFieldIndex].myClassName=ClassName;
			AllFields[lastFieldIndex].myFieldName = FieldName;
			AllFields[lastFieldIndex].myType = dataType;
			AllFields[lastFieldIndex].myModFlag = modFlag;
			AllFields[lastFieldIndex++].myValue.pvoid = data;
		}

				//All typed generators to easily describe the fields
				static void set(const __FlashStringHelper *  ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, uint8_t* data)
				{
					set(  ClassName,   FieldName,  modFlag,  data,_uint8_t);
				}

				static void set(const __FlashStringHelper *  ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, int8_t* data)
				{
					set(  ClassName,   FieldName,  modFlag,  data,_int8_t);
				}
				static void set(const __FlashStringHelper *  ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, uint16_t* data)
				{
					set(  ClassName,   FieldName,  modFlag,  data,_uint16_t);
				}
				static void set(const __FlashStringHelper *  ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, uint32_t* data)
				{
					set(  ClassName,   FieldName,  modFlag,  data,_uint32_t);
				}
				static void set(const __FlashStringHelper *  ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, int32_t* data)
				{
					set(  ClassName,   FieldName,  modFlag,  data,_int32_t);
				}
				static void set(const __FlashStringHelper *  ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, int16_t* data)
				{
					set(  ClassName,   FieldName,  modFlag,  data,_int16_t);
				}

				static void set(const __FlashStringHelper *  ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag,  char * data)
				{
					set(  ClassName,   FieldName,  modFlag,  data,_ppchar);
				}
		#ifdef I_USE_STRING
				static void set(const __FlashStringHelper *  ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, String* data)
				{
					AllFields[lastFieldIndex].myClassName=ClassName;
					AllFields[lastFieldIndex].myFieldName = FieldName;
					AllFields[lastFieldIndex].myType = _String;
					AllFields[lastFieldIndex].myModFlag = modFlag;
					AllFields[lastFieldIndex++].myValue.pString = data;
					//set(  ClassName,   FieldName,  modFlag,  data,_String);
				}
		#endif
				static void set(const __FlashStringHelper *  ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, bool* data)
				{
					set(  ClassName,   FieldName,  modFlag,  data,_bool);
				}

				static void set(const __FlashStringHelper *  ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, const __FlashStringHelper ** data)
					{
					set(  ClassName,   FieldName,  modFlag,  data,_FlashStringHelper);
					}

		#ifdef I_USE_GPS
				static void set(const __FlashStringHelper *  ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, GPSLocation* data)
				{
					set(  ClassName,   FieldName,  modFlag,  data,_GPSLocation);
				}
		#endif
		#ifdef I_USE_DATETIME
				static void set(const __FlashStringHelper *  ClassName, const __FlashStringHelper * FieldName, uint8_t modFlag, DateTime* data)
				{
					set(  ClassName,   FieldName,  modFlag,  data,_DateTime);
				}
		#endif


};



#endif /* FIELDDATA_H_ */
