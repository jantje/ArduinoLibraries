/*
 * ModBusMaster.cpp
 *
 *  Created on: 14 jul. 2023
 *      Author: jan
 */
#include <ModBusMaster.h>
#ifdef USE_LOOP_MILLIS
extern uint32_t loopMillis;
#else
static uint32_t loopMillis = millis();
#endif

//The 3 function codes
#define FUNCTION_CODE_WRITE_MULTIPLE_REGISTERS 16
#define FUNCTION_CODE_WRITE_SINGLE_REGISTER 6
#define FUNCTION_CODE_READ_HOLDING_REGISTERS 3

#ifdef DEBUG_MOD_BUS_MASTER
#define PRINT_MESSAGE(intro,message,messageLength) 	\
		Serial.print(intro);\
	for (int curchar = 0; curchar < messageLength; curchar++) {\
		Serial.print( message[curchar],HEX);\
		Serial.print( " ");\
	}\
	Serial.println();
#else
#define PRINT_MESSAGE(intro,message,messageLength)
#endif

static unsigned int crc_16(unsigned int length, unsigned char *buffer) {
	unsigned int i, j, temp_bit, temp_int, crc;
	crc = 0xFFFF;
	for (i = 0; i < length; i++) {
		temp_int = (unsigned char) *buffer++;
		crc ^= temp_int;
		for (j = 0; j < 8; j++) {
			temp_bit = crc & 0x0001;
			crc >>= 1;
			if (temp_bit != 0)
				crc ^= 0xA001;
		}
	}
	return crc;
}

void ModBusMaster::sendF16WriteMultipleRegisters(uint8_t slaveAddress,
		uint16_t address, uint8_t numberOfRegisters, uint16_t *values) {
	uint8_t numberOfbytes = numberOfRegisters * 2;
	uint8_t messageLength = 9 + numberOfbytes;
	uint8_t message[messageLength];
	myLastSendMessagefunctioncode = FUNCTION_CODE_WRITE_MULTIPLE_REGISTERS;
	message[0] = slaveAddress;
	message[1] = myLastSendMessagefunctioncode;
	message[2] = highByte(address);
	message[3] = lowByte(address);
	message[4] = highByte(numberOfRegisters);
	message[5] = lowByte(numberOfRegisters);
	message[6] = numberOfbytes;
	uint8_t pos = 7;
	for (uint8_t curvalue = 0; curvalue < numberOfRegisters; curvalue++) {
		uint16_t value = values [curvalue];
		message[pos++] = highByte(value);
		message[pos++] = lowByte(value);
	}
	uint16_t crc = crc_16(messageLength - 2, message);
	message[messageLength - 2] = lowByte(crc);
	message[messageLength - 1] = highByte(crc);
	sendMessage(message, messageLength);
}

void ModBusMaster::sendFC3ReadMultipleRegisters(uint8_t slaveAddress,
		uint16_t address, uint16_t numberOfValuesToRead) {
	uint8_t messageLength = 8;
	uint8_t message[messageLength];
	myLastSendMessagefunctioncode = FUNCTION_CODE_READ_HOLDING_REGISTERS;
	message[0] = slaveAddress;
	message[1] = myLastSendMessagefunctioncode;
	message[2] = highByte(address);
	message[3] = lowByte(address);
	message[4] = highByte(numberOfValuesToRead);
	message[5] = lowByte(numberOfValuesToRead);
	uint16_t crc = crc_16(messageLength - 2, message);
	message[messageLength - 2] = lowByte(crc);
	message[messageLength - 1] = highByte(crc);
	sendMessage(message, messageLength);
}

void ModBusMaster::sendF6WriteSingleRegister(uint8_t slaveAddress,
		uint16_t address, uint16_t value1) {
	uint8_t messageLength = 8;
	uint8_t message[messageLength];
	myLastSendMessagefunctioncode=FUNCTION_CODE_WRITE_SINGLE_REGISTER;
	message[0] = slaveAddress;
	message[1] = myLastSendMessagefunctioncode;
	message[2] = highByte(address);
	message[3] = lowByte(address);
	message[4] = highByte(value1);
	message[5] = lowByte(value1);
	unsigned int crc = crc_16(messageLength - 2, message);
	message[messageLength - 2] = lowByte(crc);
	message[messageLength - 1] = highByte(crc);
	sendMessage(message, messageLength);
}

void ModBusMaster::sendMessage(const uint8_t *message, uint8_t messageLength) {
	myLastSendMessageLength = messageLength;
	memcpy(myLastSendMessage, message, myLastSendMessageLength);
	mySerial.write(myLastSendMessage, myLastSendMessageLength);
	myIsWaitingForResponse = true;
	myLastMessageSendTime = loopMillis;
	PRINT_MESSAGE("send :",message,messageLength);
}

void ModBusMaster::resendLastMessage() {
	mySerial.write(myLastSendMessage, myLastSendMessageLength);
	myIsWaitingForResponse = true;
	myLastMessageSendTime = loopMillis;
	PRINT_MESSAGE("send again :",myLastSendMessage,myLastSendMessageLength);
}

ModBusMaster::ModBusMaster(Stream &serial, ModBusSlaveInterface &client1) :
		mySerial(serial), myModbudClients(client1, &myModbudClients) {
	myNumSlaves = 1;
	setBaudRate(9600);
}

ModBusMaster::ModBusMaster(Stream &serial, ModBusSlaveInterface &client1,
		ModBusSlaveInterface &client2) :
		mySerial(serial), myModbudClients(client1,
				new linkedClients(client2, &myModbudClients)) {
	myNumSlaves = 2;
	setBaudRate(9600);
}

ModBusMaster::ModBusMaster(Stream &serial, ModBusSlaveInterface &client1,
		ModBusSlaveInterface &client2, ModBusSlaveInterface &client3) :
		mySerial(serial), myModbudClients(client1,
				new linkedClients(client2,
						new linkedClients(client3, &myModbudClients))) {
	myNumSlaves = 3;
	setBaudRate(9600);
}

void ModBusMaster::loop() {
#ifndef USE_MAIN_LOOP_MILLIS
	loopMillis = millis();
#endif

	//first try to read read from the serial
	int numBytesAvailable = mySerial.available();
	if (numBytesAvailable > 0) {
		myLastMessageRecievedTime = loopMillis;
		mySerial.readBytes(myResponseFromV20 + myAlreadyRead, numBytesAvailable);
		myAlreadyRead += numBytesAvailable;
		//make it a null terminated string
		myResponseFromV20[myAlreadyRead] = 0;
		PRINT_MESSAGE("recieving :",myResponseFromV20,myAlreadyRead);
#ifdef DEBUG_SIEMENS_V20_SERIAL
		Serial.println("recieved from Siemens V20");
		for (int i = 0; i < myAlreadyRead; i++) {
			Serial.print((char) mySerialInBuffer[i]);
		}
		Serial.println();
		for (int i = 0; i < myAlreadyRead; i++) {
			Serial.print(mySerialInBuffer[i]);
			Serial.print(',');
		}
		Serial.println();
#endif
		tryToParseRecievedMesage();
		//we have recieved data so nothing else to do
		return;

	}

	/*
	 * As to the part (loopMillis - myLastMessageRecievedTime > 5) below
	 * The MODBUS spec tells there should be a start pauze and an end pause of >=3,5 character run time.
	 * Above that it states:
	 * > The following fixed framing characters in a message cannot
	 * > be altered: 8 data bits, 1 parity bit, and 1 or 2 stop bits.
	 * Which makes me believe the "character run time" is the time to send 1 byte with all the additional bit which
	 * is maximum 8+1+2=11 bits. Multiplied by 3,5 = 38.5 bits so take 39 bits.
	 * Assuming a 9600 baudrate (39/9,6) =4,0625 miliseconds so I took 5
	 */
	if (myIsWaitingForResponse
			&& (loopMillis - myLastMessageSendTime > myResponseDelayTime)
			&& (loopMillis - myLastMessageRecievedTime > myDelayBetweenMesages)) {
		//we waited long enough
		myAlreadyRead=0;
		//lets assume the previous message was lost somehow
		if (myCurModbudClient->myNode.retryFailedSend()) {
			resendLastMessage();
			return;
		}
		myIsWaitingForResponse = false;
		myAlreadyRead=0;
	}

	//now do the writing
	if ((!myIsWaitingForResponse)
			&& (loopMillis - myLastMessageRecievedTime > myDelayBetweenMesages)) {

		//we can send a message => check to see who wants to send a mesage
		int numSlavesProsessed = 0;
		do {
			//Serial.print("querying channel ");
			//Serial.println(myCurModbudClient->myNode.getChannelID());
			if (sendMessageToSlave(myCurModbudClient->myNode)) {
				return;
			}
			myCurModbudClient = myCurModbudClient->myNext;
			numSlavesProsessed++;
		} while (numSlavesProsessed < myNumSlaves);
	}

}

bool ModBusMaster::sendMessageToSlave(ModBusSlaveInterface &node) {
	uint16_t startAddress;
	uint16_t numberOfRegisters;
	if (node.hasFC3ReadHoldingRegistersRequest(&startAddress,
			&numberOfRegisters)) {
		//send single write request
		sendFC3ReadMultipleRegisters(myCurModbudClient->myNode.getChannelID(),
				startAddress, numberOfRegisters);
		return true;
	}
	uint16_t value;
	if (node.hasFC6WriteSingleRegisterRequest(&startAddress, &value)) {
		//send single write request
		sendF6WriteSingleRegister(myCurModbudClient->myNode.getChannelID(),
				startAddress, value);
		return true;
	}
	if (node.hasFC16WriteMultipleRegisterRequest(&startAddress,
			&numberOfRegisters, mySharedRegisterValues)) {
		//send single write request
		sendF16WriteMultipleRegisters(myCurModbudClient->myNode.getChannelID(),
				startAddress, numberOfRegisters, mySharedRegisterValues);
		return true;
	}
	return false;
}

void ModBusMaster::setup() {
}

void ModBusMaster::tryToParseRecievedMesage() {
	if (myAlreadyRead < 2) {
		//not enough data; wait for more data to arrive
		return;
	}
	uint8_t functioncode = myResponseFromV20[1];
	switch (functioncode) {
	case FUNCTION_CODE_WRITE_MULTIPLE_REGISTERS:
	case FUNCTION_CODE_WRITE_SINGLE_REGISTER: {
		if (myAlreadyRead < 8) {
			//message has not yet completely arrived
			return;
		}
		PRINT_MESSAGE("response :", myResponseFromV20,myAlreadyRead);
		uint16_t crc = crc_16(6, myResponseFromV20);
		if ((myResponseFromV20[6] != lowByte(crc))
				|| (myResponseFromV20[7] != highByte(crc))) {
			//Wrong cxrc code
			addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
					"Received message with wrong crc code");
			myAlreadyRead=0;
			myIsWaitingForResponse = false;
			return;
		}
		myCurModbudClient->myNode.responseOnLastMessage(0,
				mySharedRegisterValues);
		break;
	}
	case FUNCTION_CODE_READ_HOLDING_REGISTERS: {

		uint8_t numberOfByte = myResponseFromV20[2];
		uint8_t totalMessageSize = numberOfByte + 5;
		if (myAlreadyRead < totalMessageSize) {
			//message has not yet completely arrived
			return;
		}
		PRINT_MESSAGE("response :", myResponseFromV20,myAlreadyRead);
		uint16_t crc = crc_16(totalMessageSize - 2, myResponseFromV20);
		if ((myResponseFromV20[totalMessageSize - 2] != lowByte(crc))
				|| (myResponseFromV20[totalMessageSize - 1] != highByte(crc))) {
			//Wrong cxrc code
			addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
					"Received message with wrong crc code");
			myAlreadyRead=0;
			myIsWaitingForResponse = false;
			return;
		}
		if(numberOfByte %2==1){
			addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
					"Received uneven number of bytes from read request");
			myAlreadyRead=0;
			myIsWaitingForResponse = false;
			return;
		}
		uint8_t numberOfRegisters = numberOfByte/2;
		for (int curRegister=0;curRegister<numberOfRegisters;curRegister++){
			uint16_t registervalue = (((uint16_t) myResponseFromV20[3+(curRegister*2)]) << 8)
						+ ((uint16_t) myResponseFromV20[4+(curRegister*2)]);
			mySharedRegisterValues[curRegister]=registervalue;
		}
		//delay(50);
		myCurModbudClient->myNode.responseOnLastMessage(numberOfRegisters,
				mySharedRegisterValues);
		break;
	}

	default:
		//an error code has been received
		PRINT_MESSAGE("response :", myResponseFromV20,myAlreadyRead);
		addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
				"Received error message from V20", true);
	}

	//addBusinessEventInfo("Received message ");
	myIsWaitingForResponse = false;
	myAlreadyRead = 0;

}

#ifdef I_USE_SERIAL_REGISTER
//TODO reactivate line below debug activity
//#define LOG_VIA_SERIAL_REGISTER
#endif
#ifndef MODBUS_LOG_LEVEL_TO_REPORT
#define MODBUS_LOG_LEVEL_TO_REPORT LOG_LEVEL_OPERATIONAL
#endif
//#define MODBUS_LOG_LEVEL_TO_REPORT 0
void ModBusMaster::addBusinessEventInfo(uint8_t level,
		const __FlashStringHelper *info, bool intro, bool newLine) {
	if (level < MODBUS_LOG_LEVEL_TO_REPORT) {
		return;
	}
#ifdef LOG_VIA_SERIAL_REGISTER
	strlcat_P(myBusinessEvent, (char* ) info, BUSINESSEVENTSIZE);
	if (newLine) {
		strlcat(myBusinessEvent, EVENT_SEPARATOR, BUSINESSEVENTSIZE);
	}
#else
	if(intro){
	Serial.print(loopMillis);
	Serial.print(" modbusMaster ");
	}
	Serial.print(info);
	if (newLine) {
		Serial.println("");
	}
#endif
}
void ModBusMaster::addBusinessEventInfo(uint8_t level, const char *info,
		bool intro, bool newLine) {
	if (level < MODBUS_LOG_LEVEL_TO_REPORT) {
		return;
	}
#ifdef LOG_VIA_SERIAL_REGISTER
	strlcat(myBusinessEvent, info, BUSINESSEVENTSIZE);
	if (newLine) {
		strlcat(myBusinessEvent, EVENT_SEPARATOR, BUSINESSEVENTSIZE);
	}
#else
	if(intro){
		Serial.print(loopMillis);
		Serial.print(" modbusMaster ");
		}
		Serial.print(info);
		if (newLine) {
			Serial.println("");
		}
#endif
}

#ifdef I_USE_SERIAL_REGISTER
void ModBusMaster::serialRegister(const __FlashStringHelper* Name) {
	MotorInterface::serialRegister(Name);
	FieldData::set(Name, F("Current_cA"), MOD_NONE, &myCentiAmp);
	FieldData::set(Name, F("BusinessEvent"), MOD_ERASE_ON_DUMP | MOD_OVERVIEW,
			myBusinessEvent);
	FieldData::set(Name, F("myRPMSpeed"), MOD_NONE,
			&myRPMSpeed);
#define DETAILED_LOGGING
#ifdef DETAILED_LOGGING
	FieldData::set(Name, F("myActualSpeed"), MOD_NONE, &myActualSpeed);
	FieldData::set(Name, F("IsWaitingForResponse"), MOD_NONE,
			&myIsWaitingForResponse);
	FieldData::set(Name, F("LastMessageSendTime"), MOD_NONE,
			&myLastMessageSendTime);
	FieldData::set(Name, F("LastMessageRecievedTime"), MOD_NONE,
			&myLastMessageRecievedTime);
	FieldData::set(Name, F("SlaveAddress"), MOD_WRITE | MOD_SAVE,
			&mySlaveAddress);
	FieldData::set(Name, F("KeepAliveInterval"), MOD_WRITE | MOD_SAVE,
			&myKeepAliveInterval);
	FieldData::set(Name, F("ResponseDelayTime"), MOD_WRITE | MOD_SAVE,
			&myResponseDelayTime);
#endif
}
#endif

