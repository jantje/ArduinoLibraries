#include "JanServo.h"

void JanContiniousServo::setPosition(uint8_t Position)
{
	myServoPosition =  Position  ;
	write(constrain((int)myServoPosition + myOffset,0,180));
}


#ifdef I_USE_SERIAL_REGISTER
void JanContiniousServo::serialRegister(const __FlashStringHelper* Name)
{
	FieldData::set(Name, (__FlashStringHelper *) (PIN), 0, &myServoControlPin);
	FieldData::setNext( F("Position"),(uint8_t)0,&myServoPosition);
	FieldData::setNext( F("offset"),MOD_SAVE | MOD_WRITE,&myOffset);
}

void JanConstraintServo::serialRegister(const __FlashStringHelper* Name)
{
	FieldData::set(Name, (__FlashStringHelper *) (PIN), MOD_NONE, &myServoControlPin);
	FieldData::setNext( F("Position"),MOD_NONE,&myServoPosition);
	FieldData::setNext( F("newPosition"),MOD_WRITE,&myNewServoPosition);
	FieldData::setNext( F("MaxServoPositionset"),MOD_WRITE|MOD_SAVE,&myMaxServoPosition);
	FieldData::setNext( F("minServoPosition"),MOD_WRITE|MOD_SAVE,&myMinServoPosition);
	FieldData::setNext( F("DefaultServoPosition"),MOD_WRITE|MOD_SAVE,&myDefaultServoPosition);

}
#endif


