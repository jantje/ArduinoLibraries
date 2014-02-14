#include "JanServo.h"
#include "FieldData.h"



void JanServo::SetPosition(uint8_t Position)
{
	ServoPosition = Position;
	write(ServoPosition + offset);
}
void JanServo::serialRegister(const __FlashStringHelper* Name)
{
	FieldData::set(Name, (__FlashStringHelper *) (PIN), 0, &ServoControlPin);
	FieldData::setNext( F("Position"),0,&ServoPosition);
	FieldData::setNext( F("offset"),MOD_SAVE | MOD_WRITE,&offset);
}
//void JanServo::setup()
//{
//	attach(ServoControlPin); //this can not be done in the constructor for one reason or another
//}


// no code here as all code is inline
