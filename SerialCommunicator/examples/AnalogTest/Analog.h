/*
 * Analog.h
 *
 *  Created on: Nov 2, 2014
 *      Author: jan
 */

#ifndef ANALOG_H_
#define ANALOG_H_
#include "Arduino.h"
#include "SerialDataInterface.h"
class Analog
{
		private:
		uint16_t my_Pin_A0_value;
		uint16_t my_Pin_A1_value;
		uint16_t my_Pin_A2_value;
		uint16_t my_Pin_A3_value;
		uint16_t my_Pin_A4_value;
		uint16_t my_Pin_A5_value;
		uint16_t my_Pin_A6_value;
		uint16_t my_Pin_A7_value;
		uint16_t my_Pin_A8_value;
		uint16_t my_Pin_A9_value;
		uint16_t my_Pin_A10_value;
		uint16_t my_Pin_A11_value;
		uint16_t my_Pin_A12_value;
		uint16_t my_Pin_A13_value;
		uint16_t my_Pin_A14_value;
		uint16_t my_Pin_A15_value;
	public:
		Analog();
		void serialRegister(const __FlashStringHelper* Name);
		void loop();
		void setup();

};

#endif /* ANALOG_H_ */
