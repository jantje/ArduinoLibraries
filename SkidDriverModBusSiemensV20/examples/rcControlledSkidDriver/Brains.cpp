/*
 * Brains.cpp
 *
 *  Created on: Oct 23, 2026
 *      Author: jan
 */

#include "Brains.h"
extern ModBusSlaveSiemensV20 modBusMotor_1;
#if NUM_MOTORS>1
extern ModBusSlaveSiemensV20 modBusMotor_2;
#if NUM_MOTORS>2
extern ModBusSlaveSiemensV20 modBusMotor_3;
#endif
#endif
void Brains::setup(){
	myMotor1LastAction=myMotor2LastAction=myMotor3LastAction=millis();


}
void motorChange(uint32_t loopmilis,ModBusSlaveSiemensV20 &motor,uint32_t *lastChange){
	static int status = 1;
	if (loopmilis - *lastChange > 5000) {
		*lastChange = loopmilis;
		switch (status) {
		case 1:
			Serial.println("Setting speed to 100");
			motor.motorOn();
			motor.setRequestedSpeed(100);
			status = 2;
			break;
		case 2:
			Serial.println("Stopping motor");
			motor.motorOff();
			status = 3;
			break;
		case 3:
			Serial.println("Setting speed to -100");
			motor.setRequestedSpeed(-100);
			motor.motorOn();
			status = 4;
			break;
		case 4:
			Serial.println("Stopping motor");
			motor.motorOff();
			status = 1;
			break;
		}
	}
}

MOTOR_STATE reportMotorStateChange(ModBusSlaveSiemensV20 &motor,MOTOR_STATE prefState){
	MOTOR_STATE curstate = motor.getState();
	if(curstate!=prefState){
		Serial.print(motor.getName());
		Serial.print(" changed from state ");
		Serial.print(getMotorStateName(prefState));
		Serial.print(" to state ");
		Serial.println(getMotorStateName(curstate));
		prefState=curstate;
	}
	return curstate;
}
void Brains::loop(){
	uint32_t loopMillis=millis();
	motorChange(loopMillis,modBusMotor_1,&myMotor1LastAction);
#if NUM_MOTORS>1
	motorChange(loopMillis,modBusMotor_2,&myMotor2LastAction);
#if NUM_MOTORS>2
	motorChange(loopMillis,modBusMotor_3,&myMotor3LastAction);
#endif
#endif
	myMotor1State = reportMotorStateChange(modBusMotor_1,myMotor1State);
#if NUM_MOTORS>1
	reportMotorStateChange(loopMillis,modBusMotor_2,&myMotor2LastAction);
#if NUM_MOTORS>2
	reportMotorStateChange(loopMillis,modBusMotor_3,&myMotor3LastAction);
#endif
#endif

}

