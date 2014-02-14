/*
 * FieldInfo.h
 *
 *  Created on: Feb 8, 2014
 *      Author: jan
 */

#ifndef FIELDINFO_H_
#define FIELDINFO_H_
#include "Arduino.h"

#ifdef I_USE_GPS
#include "DataTypes.h"
#endif


extern Stream *SerialInput;
extern Stream *SerialOutput;

typedef enum
{
	_uint8_t, _int8_t, _uint16_t, _int16_t, _uint32_t, _int32_t, _ppchar,	_bool, _FlashStringHelper,
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


#define    MOD_WRITE 			1  //Allow writes to this field
#define    MOD_SAVE  			2  //Allow this field to be saved
#define	   MOD_ERASE_ON_DUMP	4  //When a dump is done this field is erased String="" integer values=0 pchar=/0
class FieldInfo
{
	public:
		const  __FlashStringHelper *  myClassName;
		const __FlashStringHelper * myFieldName;
		//DataTypes myType;
		uint8_t myType;
		uint8_t myModFlag;
		const FieldInfo & operator=(const FieldInfo &fieldInfo);
		bool operator==(const FieldInfo &fieldInfo) const;
		void dump() const;

		#ifdef I_USE_GPS
			static DataTypes getType(GPSLocation){return  _GPSLocation;};
		#endif
		#ifdef I_USE_STRING
			static DataTypes getType(String){return  _String;};
		#endif
		#ifdef I_USE_DATETIME
			static DataTypes getType(DateTime){return  _DateTime;};
		#endif
};
#endif /* FIELDINFO_H_ */
