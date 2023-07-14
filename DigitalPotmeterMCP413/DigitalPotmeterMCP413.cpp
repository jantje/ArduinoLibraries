/*
 * MCP413.cpp
 *
 *  Created on: Jul 11, 2014
 *      Author: jan
 */

#include <DigitalPotmeterMCP413.h>


DigitalPotmeterMCP413::DigitalPotmeterMCP413(uint8_t maxPotValue, uint8_t slavePin,bool inverted)
{
	myMaxPotValue=maxPotValue;
	mySlavePin=slavePin;
	myInverted=inverted;
}

void DigitalPotmeterMCP413::setup()
{
	// I assume SPI.begin(); has been called before this method is called
  pinMode(mySlavePin,OUTPUT);
  myNewPotValue=0;
  myPotValue=1;
  loop();
}

void DigitalPotmeterMCP413::loop()
{
	if(myNewPotValue!=myPotValue)
	{
	  digitalWrite(mySlavePin,LOW);
	  SPI.transfer(0);
	  if(myInverted)
	  {
	  	myActualSendValue=map( 255-myNewPotValue,0,255,0,myMaxPotValue);
	  }
	  else
	  {
	  	myActualSendValue= map( myNewPotValue,0,255,0,myMaxPotValue);
	  }
	  SPI.transfer(myActualSendValue);
	  digitalWrite(mySlavePin,HIGH);
	  myPotValue=myNewPotValue;
	}
}

void DigitalPotmeterMCP413::serialRegister(const __FlashStringHelper* Name)
{
	FieldData::set( Name,F("myNewPotValue"), MOD_WRITE, &myNewPotValue);
	FieldData::setNext( F("myPotValue"), 0, &myPotValue);
	FieldData::setNext( F("myMaxPotValue"), 0, &myMaxPotValue);
	FieldData::setNext( F("myInverted"), 0, &myInverted);
	FieldData::setNext( F("myActualSendValue"),0,&myActualSendValue);
	FieldData::setNext( F("SlavePin"), 0, &mySlavePin);
}
