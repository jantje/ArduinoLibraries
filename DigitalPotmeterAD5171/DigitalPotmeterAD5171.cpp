/*
 * AD5171.cpp
 *
 *  Created on: Jul 11, 2014
 *      Author: jan
 */

#include "AD5171.h"

AD5171::AD5171(uint8_t maxPotValue, uint8_t I2CAdress)
{
	myI2CAdress = I2CAdress;
	myMaxPotValue = maxPotValue;
}

void AD5171::setup()
{
	//I assume Wire.begin() has been called before this method is called
}

void AD5171::loop()
{
	if (myNewPotValue != myPotValue)
	{
		Wire.beginTransmission(myI2CAdress);
		Wire.write(uint8_t(0x00));
		Wire.write((uint8_t) map( myNewPotValue,0,255,0,myMaxPotValue));
				Wire.endTransmission();
				myPotValue = myNewPotValue;
			}
		}
