/*
 * AD5171.h
 *
 *  Created on: Jul 11, 2014
 *      Author: jan
 */

#ifndef AD5171_H_
#define AD5171_H_
#include "Arduino.h"
#include "Wire.h"
#include "IDigitalPotmeter.h"

class AD5171:public IDigitalPotmeter
{
	private:
		uint8_t myI2CAdress; //the I2CAdress of the I2CPotmeter
	public:
		AD5171(uint8_t myMaxPotValue,uint8_t I2CAdress);
		virtual void setup();
		virtual void loop();
};

#endif /* AD5171_H_ */
