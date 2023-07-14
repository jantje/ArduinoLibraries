/*
 * brains.h
 *
 *  Created on: Nov 11, 2013
 *      Author: jan
 */

#ifndef BRAINS_H_
#define BRAINS_H_
#include"MotorMegaMoto.h"
#include "TempMeterInterface.h"
#include "DataTypes.h"

extern MotorMegaMoto myTestMotor;
extern TempMeterInterface & myHeatSensor;

class brains
{
	protected:
		//For SerialDataInterface
		int field1;

	public:
		brains()
		{
			field1=0;
		}
		void setup(){};
		void loop(){};
};

#endif /* BRAINS_H_ */

//Added by Sloeber 
#pragma once
