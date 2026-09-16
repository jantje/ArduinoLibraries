/*
 * ModBusSiemensV20.h
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
#include "Arduino.h"
#include "MotorInterface.h"
#include "CurrentSensorInterface.h"
#define RESPONSE_DELAY_TIME 4
#define BUSINESSEVENTSIZE 300
#define STATE_SIZE 60
const uint8_t STX= 2;
const int maximumPermissibleResponseDelayTime = 1000;

#define MESSAGETYPE_STOP_MOTOR 0
#define MESSAGETYPE_START_MOTOR 1
#define MESSAGETYPE_GET_STATUS 2
#define MESSAGETYPE_START_V20_ENCHANTMENT 3
#define MESSAGETYPE_CLEAR_ERROR_FLAGS 4
#define MESSAGETYPE_SET_MOTOR_SPEED 5
#define MESSAGETYPE_GET_CURRENT_USAGE 6
#define MESSAGETYPE_NONE 10

#define LOG_LEVEL_DETAIL_DEBUG  1
#define LOG_LEVEL_DEBUG  2
#define LOG_LEVEL_OPERATIONAL  3

#define MINIMUM_SPEED 10



class MotorModBusSiemensV20: public MotorInterface, public  CurrentSensorInterface
	{
	private:
		uint8_t mySlaveAddress=0;
		uint8_t myLastMessageType =MESSAGETYPE_NONE;
		uint32_t myKeepAliveInterval=600;
		uint32_t myCurrentInterval =500;
		uint32_t myResponseDelayTime=400;

		bool myEnchantmentNeedsToBeSend=true;
		bool myErrorFlagsNeedToBeCleared=true;
		bool myMotorNeedsToTurnOff=true;
		bool myMotorNeedsToTurnOn=false;
		bool myIsWaitingForResponse=false;
		uint32_t myLastMessageSendTime=0;
		uint32_t myLastKeepAliveMessageSendTime=0;
		uint32_t myLastCurentRequestMessageSendTime=0;
		uint32_t myLastMessageRecievedTime=0;
		uint16_t myRPMSpeed=0;

		Stream &mySerial;

		uint8_t responseFromV20[255]; //TODO check Doc; 255 is probably way to big
		int myAlreadyRead=0;


		//log stuff
		char myBusinessEvent[BUSINESSEVENTSIZE];

	public :
		MotorModBusSiemensV20(uint8_t slaveAddress,Stream &serial);
	//	virtual ~MotorModBusSiemensV20(){};
		void loop();
		void setup();
		void motorOn();
		void motorOff();
		bool emergencyBreak();
		virtual bool isAtSpeed() const;
		virtual bool isStopped() const;


#ifdef I_USE_SERIAL_REGISTER
		void serialRegister(const __FlashStringHelper* Name);
#endif
private:
		void tryToParseRecievedMesage();
		void parseCurrentResponse();
		void addBusinessEventInfo(uint8_t level,const __FlashStringHelper * info,bool newLine =true);
		void addBusinessEventInfo(uint8_t level,const char * info,bool newLine=true);
		void sendWriteMultipleRegisters(uint16_t address,uint16_t value1);
		void sendWriteMultipleRegisters(uint16_t address,uint16_t value1,uint16_t value2);
		void sendWriteMultipleRegisters(uint16_t address,
				uint16_t value1, uint16_t value2,uint16_t value3, uint16_t value4,uint16_t value5);
		void sendReadMultipleRegisters(uint16_t address,uint16_t numberOfValuesToRead);
		void sendWriteSingleRegisters(uint16_t address,uint16_t value1);
		void sendKeepAliveRequest();
		void sendStopRequest();
		void sendStartRequest();
		void sendSpeedChangeRequest();
		void sendStartV20EnchantmentRequest1();
		void sendClearErrorFlagsRequest();
		void sendCurrentRequest();
	};
