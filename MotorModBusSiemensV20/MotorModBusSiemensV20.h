/*
 * ModBusSiemensV20.h
 * License type: This code is AS IS and you can do whatever you want with it
 *
 *  What was I trying to do
 *  Communicate with a Siemens V20 inverter to steer a motor
 *  I connected the inverter to my Arduino via a RS485 connector breakout board
 *  When I'm writing this I don't even know I will get this to work .. ever
 *
 *  Created on: 14 jul. 2023
 *      Author: jan
 */

#pragma once
#include "Arduino.h"
#include "MotorInterface.h"
class Motor_ModBus_Siemens_V20: public MotorInterface
	{
	private:
		uint8_t slaveAddress=0;
	public:


	};

