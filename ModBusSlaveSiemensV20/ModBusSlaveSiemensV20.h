/*
 * ModBusSlaveSiemensV20.h
 * License type: This code is AS IS and you can do whatever you want with it
 *
 *  What was I trying to do
 *  Communicate with one Siemens V20 inverter to steer a motor
 *  I connected the inverter to my Arduino via a RS485 connector breakout board
 *  I configured my siemens V20 with connection macro Cn011
 *  The siemens bautrate can be set in parameter P2010 and is by default 38400 but cn011 sets it to 9600
 *  The default slave adress is 1. As you can oinly connect one V20 with the current code there is no reason to change that
 *  You have to call SerialXXX.begin with the correct baudrate for the serial provided at the constructor
 *
 *
 *  When I'm writing this I don't even know I will get this to work .. ever
 *
 *  Created on: 14 jul. 2023
 *      Author: jan
 */

#pragma once
#include <ModBusSlaveInterface.h>
#include "Arduino.h"
#include "MotorInterface.h"
#include "CurrentSensorInterface.h"
#define RESPONSE_DELAY_TIME 4
#define BUSINESSEVENTSIZE 300
#define STATE_SIZE 60

#define MESSAGETYPE_STOP_MOTOR 0
#define MESSAGETYPE_START_MOTOR 1
#define MESSAGETYPE_WATCHDOG 2
#define MESSAGETYPE_START_V20_ENCHANTMENT 3
#define MESSAGETYPE_CLEAR_ERROR_FLAGS 4
#define MESSAGETYPE_SET_MOTOR_SPEED 5
#define MESSAGETYPE_GET_CURRENT_USAGE 6
#define MESSAGETYPE_NONE 10

#define LOG_LEVEL_DETAIL_DEBUG  1
#define LOG_LEVEL_DEBUG  2
#define LOG_LEVEL_OPERATIONAL  3

#define MINIMUM_SPEED 10
typedef enum
{
	MOTOR_STOPPED, MOTOR_RUNNING, MOTOR_ERROR , MOTOR_BOOTING
} MOTOR_STATE;

const char * getMotorStateName(MOTOR_STATE);


class ModBusSlaveSiemensV20: public MotorInterface, public  CurrentSensorInterface, public ModBusSlaveInterface
	{
	private:
		uint8_t myLastMessageType =MESSAGETYPE_NONE;
		uint8_t mySlaveAddress;

		uint16_t myRPMSpeed=0;

		/**
		 * I have no clue why I need to send myEnchantmentNeedsToBeSend but if I do not send it; the motor does not run
		 *
		 * This message only needs to be send once when the V20 started
		 *
		 * Which means that a start/stop of the V20 requires this message to be send
		 * otherwise the V20 simply does not start the motor
		 */
		bool myEnchantmentNeedsToBeSend=true;
		bool myErrorFlagsNeedToBeCleared=true;
		bool myMotorNeedsToTurnOff=true;
		bool myMotorNeedsToTurnOn=false;
		bool myIsWaitingForResponse=false;
		bool myMotorNeedsSpeedChange=false;
		bool myCurentRequestNeeded=false;
		bool myWatchdogRequestNeeded=false;

		MOTOR_STATE myState=MOTOR_BOOTING;
		//Number of failed sends since last successful Send
		uint16_t myFailedSends=0;
		//last time a successful response was received
		uint32_t myLastResponseRecieved=0;

		//log stuff
		char myBusinessEvent[BUSINESSEVENTSIZE];

		//timing stuff
		uint32_t myLastCurentRequestMessageSendTime=0;
		uint32_t myLastWatchdogSendTime=0;
		uint32_t myCurentRequestInterval=100;

	public :
		ModBusSlaveSiemensV20(uint8_t slaveAddress,const char * name);
	//	virtual ~MotorModBusSiemensV20(){};
		void loop();
		void setup();
		void motorOn();
		void motorOff();
		bool emergencyBreak();
		MOTOR_STATE getState(){return myState;};
		virtual bool isAtSpeed() const;
		virtual bool isStopped() const;
		virtual void responseOnLastMessage(uint8_t numRegisters,uint16_t registerValues[]);


#ifdef I_USE_SERIAL_REGISTER
		void serialRegister(const __FlashStringHelper* Name);
#endif

	virtual uint8_t getChannelID(){return mySlaveAddress;};
	virtual bool hasFC3ReadHoldingRegistersRequest(uint16_t *startAddress,
			uint16_t *numberOfRegister);
	virtual bool hasFC16WriteMultipleRegisterRequest(uint16_t *startAddress,
			uint16_t *NumberOfValues, uint16_t *values);
	virtual bool hasFC6WriteSingleRegisterRequest(uint16_t *address,
			uint16_t *value);
	virtual bool retryFailedSend();

private:
	void switchToState(MOTOR_STATE newState);
		void addBusinessEventInfo(uint8_t level,const __FlashStringHelper * info,bool intro = true,bool newLine =true);
		void addBusinessEventInfo(uint8_t level,const char * info,bool intro = true,bool newLine=true);

	};
