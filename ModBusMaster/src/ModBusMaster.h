/*
 * ModBusMaster.h
 * License type: This code is AS IS and you can do whatever you want with it
 *
 *  What was I trying to do
 *  Communicate with three Siemens V20 inverter to steer and drive the mow motor
 *  I connected the 3 inverters to my Arduino via a RS485 connector breakout board
 *  (just 3 pins in the inverter no extra resistor
 *  I configured my siemens V20 with connection macro Cn011
 *  The siemens baudrate can be set in parameter P2010 and is by default 38400 but cn011 sets it to 9600
 *  The default slave adress is 1.
 *  This lib can connect 3 V20 (should be easy to extend) so you will have to change those
 *  You have to call SerialXXX.begin with the correct baudrate for the serial provided at the constructor
 *
 *
 *  When I'm writing this I don't even know I will get this to work .. ever
 *
 *  Created on: november 2026
 *      Author: jantje
 */

#pragma once
#include <ModBusSlaveInterface.h>
#include "Arduino.h"
#define MAX_MESSAGE_LENGTH 255
#define RESPONSE_DELAY_TIME 4
#define BUSINESSEVENTSIZE 300
const int maximumPermissibleResponseDelayTime = 1000;

#define LOG_LEVEL_DETAIL_DEBUG  1
#define LOG_LEVEL_DEBUG  2
#define LOG_LEVEL_OPERATIONAL  3

class ModBusMaster {
private:
	Stream &mySerial;
	long myBaudRate;
	uint32_t myDelayBetweenMesages;
	uint32_t myResponseDelayTime = 400;
	class linkedClients {
	public:
		linkedClients(ModBusSlaveInterface &node, linkedClients *next) :
				myNode(node), myNext(next) {

		}
		ModBusSlaveInterface &myNode;
		linkedClients *myNext;
	};
	linkedClients myModbudClients;
	linkedClients *myCurModbudClient = &myModbudClients;
	int myNumSlaves;





public:
	ModBusMaster(Stream &serial, ModBusSlaveInterface &client1);
	ModBusMaster(Stream &serial, ModBusSlaveInterface &client1,
			ModBusSlaveInterface &client2);
	ModBusMaster(Stream &serial, ModBusSlaveInterface &client1,
			ModBusSlaveInterface &client2, ModBusSlaveInterface &client3);
	void loop();
	void setup();

#ifdef I_USE_SERIAL_REGISTER
		void serialRegister(const __FlashStringHelper* Name);
#endif

	long getBaudRate() const {
		return myBaudRate;
	}

	void setBaudRate(long baudRate ) {
		myBaudRate = baudRate;
		/*
		 * To calculate myDelayBetweenMesages I add the
		 * 3.5 (character run time) before the message
		 * and add 3.5 (character run time) from after the message
		 * and round that up to 10 (character run time)
		 * I assume 11 bits per character
		 * That results to 11 milliseconds for 9600 but 0 for 115200
		 * Therefore I add 2 extra millis
		 */
		myDelayBetweenMesages=2L+(10L * 11L * 1000L /baudRate);
	}

private:

	//log stuff
	char myBusinessEvent[BUSINESSEVENTSIZE];
	void addBusinessEventInfo(uint8_t level, const __FlashStringHelper *info,
			bool intro=true, bool newLine = true);
	void addBusinessEventInfo(uint8_t level, const char *info, bool intro=true, bool newLine =
			true);

	// Sending stuff
	void sendF16WriteMultipleRegisters(uint8_t slaveAddress, uint16_t address,
			uint8_t numberOfRegisters, uint16_t values[]);
	void sendFC3ReadMultipleRegisters(uint8_t slaveAddress, uint16_t address,
			uint16_t numberOfValuesToRead);
	void sendF6WriteSingleRegister(uint8_t slaveAddress, uint16_t address,
			uint16_t value1);
	void sendKeepAliveRequest(uint8_t slaveAddress);

	void resendLastMessage();
	void sendMessage(const uint8_t *message, uint8_t messageLength);
	bool sendMessageToSlave( ModBusSlaveInterface & node);
	uint8_t myLastSendMessageLength=0;
	uint8_t myLastSendMessage[MAX_MESSAGE_LENGTH];
	uint8_t myLastSendMessagefunctioncode=0;
	uint32_t myLastMessageSendTime = 0;
	uint32_t myLastKeepAliveMessageSendTime = 0;

	//Receiving stuff
	bool myIsWaitingForResponse = false;
	uint32_t myLastMessageRecievedTime = 0;
	void tryToParseRecievedMesage();

	//memory management
	uint16_t mySharedRegisterValues[MAX_NUMBER_OF_REGISTERS];
	uint8_t myResponseFromV20[510]; //I use 255 as size as I use uint8_t for number of registers
	int myAlreadyRead = 0;
};

