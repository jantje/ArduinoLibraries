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
static uint32_t loopMillis;
void Brains::setup(){
}
int motorChange(ModBusSlaveSiemensV20 &motor,uint32_t *lastChange,int status){
	if(loopMillis<(*lastChange)){
		return status;
	}
	if (loopMillis - (*lastChange) > 5000) {
		(*lastChange) = loopMillis;
		switch (status) {
		case 1:
			Serial.print(motor.getName());
			Serial.println(" Setting speed to 100");
			motor.motorOn();
			motor.setRequestedSpeed(100);
			return 2;
		case 2:
			Serial.print(motor.getName());
			Serial.println("Stopping motor");
			motor.motorOff();
			return 3;
		case 3:
			Serial.print(motor.getName());
			Serial.println("Setting speed to -100");
			motor.setRequestedSpeed(-100);
			motor.motorOn();
			return 4;
		case 4:
			Serial.print(motor.getName());
			Serial.println("Stopping motor");
			motor.motorOff();
			return 1;
		}
	}
	return status;
}

MOTOR_STATE reportMotorStateChange(ModBusSlaveSiemensV20 &motor,MOTOR_STATE prefState){
	MOTOR_STATE curstate = motor.getState();
	if(curstate!=prefState){
		Serial.print("Brains ");
		Serial.print(motor.getName());
		Serial.print(" ");
		Serial.print(loopMillis);
		Serial.print(" changed from state ");
		Serial.print(getMotorStateName(prefState));
		Serial.print(" to state ");
		Serial.println(getMotorStateName(curstate));
		prefState=curstate;
	}
	return curstate;
}
void Brains::loop(){
	loopMillis=millis();
	static int motor1State=1;
	motor1State=motorChange(modBusMotor_1,&myMotor1LastAction,motor1State);
#if NUM_MOTORS>1
	static int motor2State=1;
	motor2State=motorChange(modBusMotor_2,&myMotor2LastAction,motor2State);
#if NUM_MOTORS>2
	static int motor3State=1;
	motor3State=motorChange(modBusMotor_3,&myMotor3LastAction,motor3State);
#endif
#endif
	myMotor1State = reportMotorStateChange(modBusMotor_1,myMotor1State);
#if NUM_MOTORS>1
	myMotor2State = reportMotorStateChange(modBusMotor_2,myMotor2State);
#if NUM_MOTORS>2
	myMotor3State = reportMotorStateChange(modBusMotor_3,myMotor3State);
#endif
#endif

}

