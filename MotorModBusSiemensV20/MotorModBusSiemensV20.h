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
#define RESPONSE_DELAY_TIME 4
#define BUSINESSEVENTSIZE 300
#define STATE_SIZE 60
const uint8_t STX= 2;
const int maximumPermissibleResponseDelayTime = 1000;




class MotorModBusSiemensV20: public MotorInterface
	{
	private:
		uint8_t mySlaveAddress=0;
		uint8_t myLastUsedAddress=0;
		uint32_t myLastMessageSendTime=0;
		uint32_t myLastMessageRecievedTime=0;
		uint32_t mySendInterval=3000;
		uint32_t myResponseDelayTime=2000;
		uint16_t myInverterState=3;
		Stream &mySerial;

		//log stuff
		char myBusinessEvent[BUSINESSEVENTSIZE];
		char myInverterStateDescription[STATE_SIZE];
		//Serial stuff
		bool myIsWaitingForResponse=false;
		uint8_t mySerialInBuffer[255]; //TODO check Doc; 255 is probably way to big
		int myAlreadyRead=0;

	public :
		MotorModBusSiemensV20(uint8_t slaveAddress,Stream &serial);
		void loop();
		void setup();
		void motorOn();
		void motorOff();
		bool emergencyBreak();

		/** send a telegram message
		 * returns an error code
		 * 0 is all ok
		 * 1 waiting for response (half duplex does not allow sending while waiting for response)
		 */
		int sendTelegram(uint8_t address,uint8_t length,uint8_t message[]);
		int sendMirrorTelegram(uint8_t address);
		bool hasTelegramResponse();
		void getTelegramResponse();
		int requestInverterState(uint8_t address);
		int readHoldingRegisters(uint8_t address,uint32_t adress,uint32_t numregisters);
#ifdef I_USE_SERIAL_REGISTER
		void serialRegister(const __FlashStringHelper* Name,const __FlashStringHelper* MotorName);
#endif
private:
		void setConverterStateDescription() const;
		void tryToParseRecievedMesage();
		void addBusinessEventInfo(const __FlashStringHelper * info,bool newLine =true);
		void addBusinessEventInfo(const char * info,bool newLine=true);

	};


//THe stuff below is for USS protocol
//
//#define MASTER_ID_NO_REQUEST 0 //No request
//#define MASTER_ID_PARAM_VALUE 1 // Request parameter value
//#define MASTER_ID_MOD_PARAM_VALUE_WORD 2 //Modify parameter value (word)
//#define MASTER_ID_MOD_PARAM_VALUEDOUBLE_WORD 3 // Modify parameter value (double word)
//#define MASTER_ID_REQUEST_DESCRITIVE_VALUE 4 //Request descriptive element
//#define MASTER_ID_REQUEST_PARAM_ARRAY_VALUE 6 //Request parameter value (array)
//#define MASTER_ID_MOD_PARAM_ARRAY_VALUE_WORD 7 //Modify parameter value (array, word)
//#define MASTER_ID_MOD_PARAM_ARRAY_VALUE_DOUBLE_WORD 8 //Modify parameter value (array, double word)
//#define MASTER_ID_REQUEST_NUMBER_OF_ARRAY_ELEMENTS 9 //Request number of array elements
//#define MASTER_ID_MOD_PARAM_ARRAY_VALUE_DOUBLE_WORD_EEPROM 11 //Modify parameter value (array, double word) and store in EEPROM
//#define MASTER_ID_MOD_PARAM_ARRAY_VALUE_WORD_EEPROM 12 //Modify parameter value (array, word) and store in EEPROM
//#define MASTER_ID_MOD_PARAM_VALUE_DOUBLE_WORD_EEPROM 13 //Modify parameter value (double word) and store in EEPROM
//#define MASTER_ID_MOD_PARAM_VALUE_WORD_EEPROM 14 //Modify parameter value (word) and store in EEPROM
//
//#define SLAVE_ID_NO_RESPONSE 0 //No response
//#define SLAVE_ID_TRANSFER_PARAMETER_VALUE 1 //Transfer parameter value (word)
//#define SLAVE_ID_TRANSFER_PARAMETER_VALUE_DOUBLE_WORD 2 //Transfer parameter value (double word)
//#define SLAVE_ID_TRANSFER_DESCRIPTIVE_ELEMENT 3 //Transfer descriptive element
//#define SLAVE_ID_TRANSFER_PARAMETER_ARRAY 4 //Transfer parameter value (array, word)
//#define SLAVE_ID_TRANSFER_PARAMETER_ARRAY_DOUBLE_WORD 5 //Transfer parameter value (array, double word)
//#define SLAVE_ID_TRANSFER_NUMBER_OF_ARRAY_ELEMENTS 6 //Transfer number of array elements
//#define SLAVE_ID_REQUEST_CAN_NOT_BE_PROCESSED 7 //Request cannot be processed, task cannot be executed (with error number)
//#define SLAVE_ID_NO_MASTER_CONTROLLER_STATUS 8 //No master controller status/no parameter change rights for PKW interface

