/*
 * Analog.cpp
 *
 *  Created on: Nov 2, 2014
 *      Author: jan
 */

#include "Analog.h"

Analog::Analog()
{
	 my_Pin_A0_value=0;
	 my_Pin_A1_value=0;
	 my_Pin_A2_value=0;
	 my_Pin_A3_value=0;
	 my_Pin_A4_value=0;
	 my_Pin_A5_value=0;
	 my_Pin_A6_value=0;
	 my_Pin_A7_value=0;
	 my_Pin_A8_value=0;
	 my_Pin_A9_value=0;
	 my_Pin_A10_value=0;
	 my_Pin_A11_value=0;
	 my_Pin_A12_value=0;
	 my_Pin_A13_value=0;
	 my_Pin_A14_value=0;
	 my_Pin_A15_value=0;

}

void Analog::serialRegister(const __FlashStringHelper* Name)
{
	FieldData::set(Name, F("A0"), 0, &my_Pin_A0_value);
	FieldData::set(Name, F("A1"), 0, &my_Pin_A1_value);
	FieldData::set(Name, F("A2"), 0, &my_Pin_A2_value);
	FieldData::set(Name, F("A3"), 0, &my_Pin_A3_value);
	FieldData::set(Name, F("A4"), 0, &my_Pin_A4_value);
	FieldData::set(Name, F("A5"), 0, &my_Pin_A5_value);
	FieldData::set(Name, F("A6"), 0, &my_Pin_A6_value);
	FieldData::set(Name, F("A7"), 0, &my_Pin_A7_value);
	FieldData::set(Name, F("A8"), 0, &my_Pin_A8_value);
	FieldData::set(Name, F("A9"), 0, &my_Pin_A9_value);
	FieldData::set(Name, F("A10"), 0, &my_Pin_A10_value);
	FieldData::set(Name, F("A11"), 0, &my_Pin_A11_value);
	FieldData::set(Name, F("A12"), 0, &my_Pin_A12_value);
	FieldData::set(Name, F("A13"), 0, &my_Pin_A13_value);
	FieldData::set(Name, F("A14"), 0, &my_Pin_A14_value);
	FieldData::set(Name, F("A15"), 0, &my_Pin_A15_value);
}

void Analog::loop()
{
	my_Pin_A0_value=analogRead(A0);
	my_Pin_A1_value=analogRead(A1);
	my_Pin_A2_value=analogRead(A2);
	my_Pin_A3_value=analogRead(A3);
	my_Pin_A4_value=analogRead(A4);
	my_Pin_A5_value=analogRead(A5);
	my_Pin_A6_value=analogRead(A6);
	my_Pin_A7_value=analogRead(A7);
	my_Pin_A8_value=analogRead(A8);
	my_Pin_A9_value=analogRead(A9);
	my_Pin_A10_value=analogRead(A10);
	my_Pin_A11_value=analogRead(A11);
	my_Pin_A12_value=analogRead(A12);
	my_Pin_A13_value=analogRead(A13);
	my_Pin_A14_value=analogRead(A14);
	my_Pin_A15_value=analogRead(A15);

}

void Analog::setup()
{
}
