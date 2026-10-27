/*
 * Brains.h
 *
 *  Created on: Oct 23, 2026
 *      Author: jan
 */
#pragma once
#include "Arduino.h"
#include "ModBusSlaveSiemensV20.h"
#define NUM_MOTORS 3
#define MOD_BUS_SERIAL Serial1
#define MOD_BUS_BAUD_RATE 9600 //115200 //P2010 on V20 =12

class Brains {
	private :
	uint32_t myMotor1LastAction=10000;
	uint32_t myMotor2LastAction=15000;
	uint32_t myMotor3LastAction=20000;
	MOTOR_STATE myMotor1State=MOTOR_BOOTING;
	MOTOR_STATE myMotor2State=MOTOR_BOOTING;
	MOTOR_STATE myMotor3State=MOTOR_BOOTING;
public:
	Brains(){};
	virtual ~Brains(){};
	void setup();
	void loop();
};

