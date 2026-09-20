/*
 * ModBusSiemensV20.cpp
 *
 *  Created on: 14 jul. 2023
 *      Author: jan
 */

#include "MotorModBusSiemensV20.h"
#ifdef USE_MAIN_LOOP_MILLIS
extern uint32_t loopMillis;
#else
   uint32_t loopMillis = millis();
#endif

//The 3 function codes
#define FUNCTION_CODE_WRITE_MULTIPLE_REGISTERS 16
#define FUNCTION_CODE_WRITE_SINGLE_REGISTER 6
#define FUNCTION_CODE_READ_HOLDING_REGISTERS 3

//Important addresses
#define ADDRESS_STW	99	//read write;
#define ADDRESS_HSW	100 //read write; speed wanted in % (multiply by 100) of max frequency
#define ADDRESS_ZSW	109	//read only
#define ADDRESS_HIW	110  //read only; actual speed in % (multiply by 100) of max frequency
#define ACTUAL_POWER 27 //actual power * 100 kW

//#define DEBUG
#ifdef DEBUG
#define PRINT_MESSAGE(intro,message,messageLength) 	\
		Serial.print(intro);\
	for (int curchar = 0; curchar < messageLength; curchar++) {\
		Serial.printf("%02x ", message[curchar]);\
	}\
	Serial.println();
#else
#define PRINT_MESSAGE(intro,message,messageLength)
#endif

unsigned int crc_16(unsigned int length, unsigned char *buffer) {
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

void MotorModBusSiemensV20::sendWriteMultipleRegisters(uint16_t address,
		uint16_t value1) {
	uint16_t numberOfValues = 1;
	uint8_t numberOfbytes = numberOfValues * 2;
	uint8_t messageLength = 9 + numberOfbytes;
	uint8_t message[messageLength];
	message[0] = mySlaveAddress;
	message[1] = FUNCTION_CODE_WRITE_MULTIPLE_REGISTERS;
	message[2] = highByte(address);
	message[3] = lowByte(address);
	message[4] = highByte(numberOfValues);
	message[5] = lowByte(numberOfValues);
	message[6] = numberOfbytes;
	message[7] = highByte(value1);
	message[8] = lowByte(value1);
	uint16_t crc = crc_16(messageLength - 2, message);
	message[messageLength - 2] = lowByte(crc);
	message[messageLength - 1] = highByte(crc);
	mySerial.write(message, messageLength);
	myIsWaitingForResponse = true;
	myLastMessageSendTime = loopMillis;
	PRINT_MESSAGE("send :",message,messageLength);
}

void MotorModBusSiemensV20::sendWriteMultipleRegisters(uint16_t address,
		uint16_t value1, uint16_t value2) {
	uint16_t numberOfValues = 2;
	uint8_t numberOfbytes = numberOfValues * 2;
	uint8_t messageLength = 9 + numberOfbytes;
	uint8_t message[messageLength];
	message[0] = mySlaveAddress;
	message[1] = FUNCTION_CODE_WRITE_MULTIPLE_REGISTERS;
	message[2] = highByte(address);
	message[3] = lowByte(address);
	message[4] = highByte(numberOfValues);
	message[5] = lowByte(numberOfValues);
	message[6] = numberOfbytes;
	message[7] = highByte(value1);
	message[8] = lowByte(value1);
	message[9] = highByte(value2);
	message[10] = lowByte(value2);
	uint16_t crc = crc_16(messageLength - 2, message);
	message[messageLength - 2] = lowByte(crc);
	message[messageLength - 1] = highByte(crc);
	mySerial.write(message, messageLength);
	myIsWaitingForResponse = true;
	myLastMessageSendTime = loopMillis;
	PRINT_MESSAGE("send :",message,messageLength);
}

void MotorModBusSiemensV20::sendWriteMultipleRegisters(uint16_t address,
		uint16_t value1, uint16_t value2, uint16_t value3, uint16_t value4,
		uint16_t value5) {
	uint16_t numberOfValues = 5;
	uint8_t numberOfbytes = numberOfValues * 2;
	uint8_t messageLength = 9 + numberOfbytes;
	uint8_t message[messageLength];
	message[0] = mySlaveAddress;
	message[1] = FUNCTION_CODE_WRITE_MULTIPLE_REGISTERS;
	message[2] = highByte(address);
	message[3] = lowByte(address);
	message[4] = highByte(numberOfValues);
	message[5] = lowByte(numberOfValues);
	message[6] = numberOfbytes;
	message[7] = highByte(value1);
	message[8] = lowByte(value1);
	message[9] = highByte(value2);
	message[10] = lowByte(value2);
	message[11] = highByte(value3);
	message[12] = lowByte(value3);
	message[13] = highByte(value4);
	message[14] = lowByte(value4);
	message[15] = highByte(value5);
	message[16] = lowByte(value5);
	uint16_t crc = crc_16(messageLength - 2, message);
	message[messageLength - 2] = lowByte(crc);
	message[messageLength - 1] = highByte(crc);
	mySerial.write(message, messageLength);
	myIsWaitingForResponse = true;
	myLastMessageSendTime = loopMillis;
	PRINT_MESSAGE("send :",message,messageLength);
}

void MotorModBusSiemensV20::sendReadMultipleRegisters(uint16_t address,
		uint16_t numberOfValuesToRead) {
	uint8_t messageLength = 8;
	uint8_t message[messageLength];
	message[0] = mySlaveAddress;
	message[1] = FUNCTION_CODE_READ_HOLDING_REGISTERS;
	message[2] = highByte(address);
	message[3] = lowByte(address);
	message[4] = highByte(numberOfValuesToRead);
	message[5] = lowByte(numberOfValuesToRead);
	uint16_t crc = crc_16(messageLength - 2, message);
	message[messageLength - 2] = lowByte(crc);
	message[messageLength - 1] = highByte(crc);
	mySerial.write(message, messageLength);
	myIsWaitingForResponse = true;
	myLastMessageSendTime = loopMillis;
	PRINT_MESSAGE("send :",message,messageLength);
}

void MotorModBusSiemensV20::sendWriteSingleRegisters(uint16_t address,
		uint16_t value1) {
	//TODO implement this
//	uint16_t numberOfValues = 2;
//	uint8_t numberOfbytes = numberOfValues * 2;
//	uint8_t messageLength = 9 + numberOfbytes;
//	uint8_t message[messageLength];
//	message[0] = mySlaveAddress;
//	message[1] = FUNCTION_CODE_WRITE_SINGLE_REGISTER;
//	message[2] = highByte(address);
//	message[3] = lowByte(address);
//	message[4] = highByte(numberOfValues);
//	message[5] = lowByte(numberOfValues);
//	message[6] = numberOfbytes;
//	message[7] = highByte(value1);
//	message[8] = lowByte(value1);
//	message[9] = highByte(value2);
//	message[10] = lowByte(value2);
//	unsigned int crc = crc_16(messageLength - 2, message);
//	message[messageLength - 2] = lowByte(crc);
//	message[messageLength - 1] = highByte(crc);
//	mySerial.write(message, messageLength);
	myIsWaitingForResponse = true;
	myLastMessageSendTime = loopMillis;
}

MotorModBusSiemensV20::MotorModBusSiemensV20(uint8_t slaveAddress,
		Stream &serial) :
		mySerial(serial) {
	mySlaveAddress = slaveAddress;
}

void MotorModBusSiemensV20::loop() {
#ifndef USE_MAIN_LOOP_MILLIS
   loopMillis = millis();
#endif

	//first try to read read from the serial
	int numBytesAvailable = mySerial.available();
	if (numBytesAvailable > 0) {
		myLastMessageRecievedTime = loopMillis;
		mySerial.readBytes(responseFromV20 + myAlreadyRead, numBytesAvailable);
		myAlreadyRead += numBytesAvailable;
		//make it a null terminated string
		responseFromV20[myAlreadyRead] = 0;
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
			&& (loopMillis - myLastMessageRecievedTime > 5)) {
		//we waited long enough
		//lets assume the previous message was lost somehow
		addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
				F("node x did not respond on"), false);
		switch (myLastMessageType) {
		case MESSAGETYPE_STOP_MOTOR: {
			addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
					F("MESSAGETYPE_STOP_MOTOR"), true);
			break;
		}
		case MESSAGETYPE_START_MOTOR: {
			addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
					F("MESSAGETYPE_START_MOTOR"), true);
			break;
		}
		case MESSAGETYPE_GET_STATUS: {
			addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
					F("MESSAGETYPE_GET_STATUS"), true);
			break;
		}
		case MESSAGETYPE_START_V20_ENCHANTMENT: {
			addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
					F("MESSAGETYPE_START_V20_ENCHANTMENT"), true);
			break;
		}
		case MESSAGETYPE_CLEAR_ERROR_FLAGS: {
			addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
					F("MESSAGETYPE_CLEAR_ERROR_FLAGS"), true);
			break;
		}
		case MESSAGETYPE_SET_MOTOR_SPEED: {
			addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
					F("MESSAGETYPE_SET_MOTOR_SPEED"), true);
			break;
		}
		case MESSAGETYPE_GET_CURRENT_USAGE: {
			addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
					F("MESSAGETYPE_GET_CURRENT_USAGE"), true);
			break;
		}
		case MESSAGETYPE_NONE: {
			addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
					F("MESSAGETYPE_NONE"), true);
			break;
		}
		}

		myIsWaitingForResponse = false;
	}

	//now do the writing
	if ((!myIsWaitingForResponse)
			&& (loopMillis - myLastMessageSendTime > myResponseDelayTime)) {
		if (myEnchantmentNeedsToBeSend) {
			sendStartV20EnchantmentRequest1();
			return;
		}

		if (myErrorFlagsNeedToBeCleared) {
			sendClearErrorFlagsRequest();
			return;
		}

		if (myMotorNeedsToTurnOn) {
			sendStartRequest();
			return;
		}

		if (myMotorNeedsToTurnOff) {
			sendStopRequest();
			return;
		}

		if (myNewRequestedSpeed != myRequestedSpeed) {
			// the motor speed needs to change
			if (abs(myNewRequestedSpeed) < MINIMUM_SPEED) {
				myNewRequestedSpeed = 0;
				myRequestedSpeed = 0;
				myMotorNeedsToTurnOff=true;
			}else{
			sendSpeedChangeRequest();
			}
			return;
		}

		//send keep alive message if needed
		if (loopMillis - myLastKeepAliveMessageSendTime > myKeepAliveInterval) {
			myLastKeepAliveMessageSendTime = loopMillis;
			sendKeepAliveRequest();
			return;
		}

		if (loopMillis - myLastCurentRequestMessageSendTime
				> myCurrentInterval) {
			myLastCurentRequestMessageSendTime = loopMillis;
			sendCurrentRequest();
			return;
		}

	}

}

void MotorModBusSiemensV20::setup() {
}

void MotorModBusSiemensV20::motorOn() {
	if(myMotorNeedsToTurnOff){
	addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
			"Received motorOn request when off is also requested");
	}
	else{
		addBusinessEventInfo(LOG_LEVEL_DEBUG,
				"Received motorOn request");
	}

	myMotorNeedsToTurnOn = true;
}

void MotorModBusSiemensV20::motorOff() {
	if(myMotorNeedsToTurnOn){
	addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
			"Received motorOff request when on is also requested");
	}
	else{
		addBusinessEventInfo(LOG_LEVEL_DEBUG,
				"Received motorOff request");
	}
	myMotorNeedsToTurnOff = true;
}

bool MotorModBusSiemensV20::emergencyBreak() {
	addBusinessEventInfo(LOG_LEVEL_DEBUG,
					"Received emergencyBreak request");
	motorOff();
	return getActualSpeed() == 0;
}

void MotorModBusSiemensV20::tryToParseRecievedMesage() {
	if (myAlreadyRead < 2) {
		//not enough data; wait for more data to arrive
		return;
	}
	uint8_t functioncode = responseFromV20[1];
	switch (functioncode) {
	case FUNCTION_CODE_WRITE_MULTIPLE_REGISTERS:
	case FUNCTION_CODE_WRITE_SINGLE_REGISTER: {
		if (myAlreadyRead < 8) {
			//message has not yet completely arrived
			return;
		}
		uint16_t crc = crc_16(6, responseFromV20);
		if ((responseFromV20[6] != lowByte(crc))
				|| (responseFromV20[7] != highByte(crc))) {
			//Wrong cxrc code
			addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
					"Received message with wrong crc code");
			return;
		}

		break;
	}
	case FUNCTION_CODE_READ_HOLDING_REGISTERS: {

		uint8_t numberOfByte = responseFromV20[2];
		uint8_t totalMessageSize = numberOfByte + 5;
		if (myAlreadyRead < totalMessageSize) {
			//message has not yet completely arrived
			return;
		}
		uint16_t crc = crc_16(totalMessageSize - 2, responseFromV20);
		if ((responseFromV20[totalMessageSize - 2] != lowByte(crc))
				|| (responseFromV20[totalMessageSize - 1] != highByte(crc))) {
			//Wrong cxrc code
			addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
					"Received message with wrong crc code");
			return;
		}
	}
		break;
	default:
		//an error code has been recieved
		addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
				"Received error message from V20", true);
		return;
	}

	//addBusinessEventInfo("Received message ");
	myIsWaitingForResponse = false;
	PRINT_MESSAGE("response :", responseFromV20,myAlreadyRead);
	myAlreadyRead = 0;



	//Based on which message we had send set the correct flags
	switch (myLastMessageType) {
	case MESSAGETYPE_STOP_MOTOR:
		myMotorNeedsToTurnOff = false;
		break;
	case MESSAGETYPE_START_MOTOR:
		myMotorNeedsToTurnOn = false;
		break;
	case MESSAGETYPE_GET_STATUS:
		break;
	case MESSAGETYPE_START_V20_ENCHANTMENT:
		myEnchantmentNeedsToBeSend = false;
		break;
	case MESSAGETYPE_CLEAR_ERROR_FLAGS:
		myErrorFlagsNeedToBeCleared = false;
		break;
	case MESSAGETYPE_SET_MOTOR_SPEED:
		break;
	case MESSAGETYPE_GET_CURRENT_USAGE:
		parseCurrentResponse();
		break;
	case MESSAGETYPE_NONE:
		break;
	}
}

static const char EVENT_SEPARATOR[] = "<br>";
#ifdef I_USE_SERIAL_REGISTER
//TODO reactivate line below debug activity
//#define LOG_VIA_SERIAL_REGISTER
#endif
//#define LOG_LEVEL_TO_REPORT LOG_LEVEL_OPERATIONAL
#define LOG_LEVEL_TO_REPORT 0
void MotorModBusSiemensV20::addBusinessEventInfo(uint8_t level,
		const __FlashStringHelper *info, bool newLine) {
	if (level < LOG_LEVEL_TO_REPORT) {
		return;
	}
#ifdef LOG_VIA_SERIAL_REGISTER
	strlcat_P(myBusinessEvent, (char* ) info, BUSINESSEVENTSIZE);
	if (newLine) {
		strlcat(myBusinessEvent, EVENT_SEPARATOR, BUSINESSEVENTSIZE);
	}
#else
	if (newLine) {
		Serial.print(loopMillis);
		Serial.print("  ");
		Serial.println(info);
	} else {
		Serial.print(info);
	}
#endif
}
void MotorModBusSiemensV20::addBusinessEventInfo(uint8_t level,
		const char *info, bool newLine) {
	if (level < LOG_LEVEL_TO_REPORT) {
		return;
	}
#ifdef LOG_VIA_SERIAL_REGISTER
	strlcat(myBusinessEvent, info, BUSINESSEVENTSIZE);
	if (newLine) {
		strlcat(myBusinessEvent, EVENT_SEPARATOR, BUSINESSEVENTSIZE);
	}
#else
	if (newLine) {
		Serial.print(loopMillis);
		Serial.print("  ");
		Serial.println(info);
	} else {
		Serial.print(info);
	}
#endif
}

#define FREQREGISTER (uint16_t)2
#define FAULTREGISTER (uint16_t)6

#define RUN_ENABLE_FLAG (uint16_t) 1
#define RUN_DISABLE_FLAG (uint16_t) 0
#define START_COMMAND_FLAG (uint16_t)1
#define STOP_COMMAND_FLAG (uint16_t)0
#define FAULT_ACKNOWELDGEMENT_FLAG (uint16_t)1
#define NO_FAULT_ACKNOWELDGEMENT_FLAG (uint16_t)0

uint16_t getSpeedInCentiHerz(SPEED_TYPE requestedSpeed) {
	if (abs(requestedSpeed) < MINIMUM_SPEED) {
		return 0;
	}
	uint16_t ret = map(abs(requestedSpeed), 0, 255, 0, 10000);
	return ret;
}
uint16_t getdirection(SPEED_TYPE requestedSpeed) {
	return requestedSpeed > 0 ? 1 : 0;
}

void MotorModBusSiemensV20::sendStopRequest() {
	addBusinessEventInfo(LOG_LEVEL_DEBUG, F("Sending stop request"), true);
	myLastMessageType = MESSAGETYPE_STOP_MOTOR;
	myActualSpeed=0;
	sendWriteMultipleRegisters(FREQREGISTER, 0,
	RUN_DISABLE_FLAG, getdirection(myRequestedSpeed),
	STOP_COMMAND_FLAG,
	NO_FAULT_ACKNOWELDGEMENT_FLAG);
}

void MotorModBusSiemensV20::sendStartRequest() {
	addBusinessEventInfo(LOG_LEVEL_DEBUG, F("Sending start request"), true);
	myLastMessageType = MESSAGETYPE_START_MOTOR;
	myActualSpeed=myRequestedSpeed = myNewRequestedSpeed;
	sendWriteMultipleRegisters(FREQREGISTER, getSpeedInCentiHerz(myRequestedSpeed),
	RUN_ENABLE_FLAG, getdirection(myRequestedSpeed),
	START_COMMAND_FLAG,
	FAULT_ACKNOWELDGEMENT_FLAG);
}

void MotorModBusSiemensV20::sendSpeedChangeRequest() {
	addBusinessEventInfo(LOG_LEVEL_DEBUG, F("Sending speed change request"),
			true);
	myLastMessageType = MESSAGETYPE_SET_MOTOR_SPEED;
	myRequestedSpeed = myNewRequestedSpeed;
	myActualSpeed=myRequestedSpeed;
	sendWriteMultipleRegisters(FREQREGISTER, getSpeedInCentiHerz(myRequestedSpeed),
	RUN_ENABLE_FLAG, getdirection(myRequestedSpeed), START_COMMAND_FLAG,
	FAULT_ACKNOWELDGEMENT_FLAG);
}

/**
 * I have no clue why I need to send this message but if I do not do so
 * This message only neds to be send once at the beginning
 * of the communication.
 * Which means that a start/stop of the V20 requires this message to be send
 * The V20 simply does not start the motor
 */
void MotorModBusSiemensV20::sendStartV20EnchantmentRequest1() {
	addBusinessEventInfo(LOG_LEVEL_DEBUG,
			F("Sending start v20 enchantment request"), true);
	myLastMessageType = MESSAGETYPE_START_V20_ENCHANTMENT;
	uint16_t stopMotor = 0x047E;
	sendWriteMultipleRegisters(ADDRESS_STW, stopMotor);
}
void MotorModBusSiemensV20::sendClearErrorFlagsRequest() {
	addBusinessEventInfo(LOG_LEVEL_DEBUG, F("Sending clear flags request"),
			true);
	myLastMessageType = MESSAGETYPE_CLEAR_ERROR_FLAGS;
	sendWriteMultipleRegisters(FAULTREGISTER, FAULT_ACKNOWELDGEMENT_FLAG);
}

/**
 * send a request to keep the communication alive
 */
void MotorModBusSiemensV20::sendKeepAliveRequest() {
	addBusinessEventInfo(LOG_LEVEL_DEBUG, F("Sending state request"), true);
	myLastMessageType = MESSAGETYPE_GET_STATUS;
	sendReadMultipleRegisters(0, 12);
}

bool MotorModBusSiemensV20::isStopped() const {
	if(myMotorNeedsToTurnOff){
		return false;
	}
	if (myRPMSpeed==0){
		return true;
	}
	return (myRequestedSpeed==0);
}

/**
 * send a request to get the currentusage
 */
void MotorModBusSiemensV20::sendCurrentRequest() {
	addBusinessEventInfo(LOG_LEVEL_DEBUG, F("Sending current request"), true);
	myLastMessageType = MESSAGETYPE_GET_CURRENT_USAGE;
	sendReadMultipleRegisters(24, 2);
}

/**
 * we assume the motor is at speed when the reported RPM is
 * bigger than 90% of the requested frequency multiuplied by
 * standard 3000 rpm for the motor
 */
bool MotorModBusSiemensV20::isAtSpeed() const {
	//TODO I divided the speed into 2 because the Siemens
	//does not go to the herz requested
	//still need to investigate why that is
	return myRPMSpeed>=1700;//(getSpeedInCentiHerz(myRequestedSpeed/2)*54)/100;
}

#ifdef I_USE_SERIAL_REGISTER
void MotorModBusSiemensV20::serialRegister(const __FlashStringHelper* Name) {
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

void MotorModBusSiemensV20::parseCurrentResponse() {
	myRPMSpeed = (((uint16_t) responseFromV20[3]) << 8)
			+ ((uint16_t) responseFromV20[4]);
	myCentiAmp = (((uint16_t) responseFromV20[5]) << 8)
				+ ((uint16_t) responseFromV20[6]);
	if(myRPMSpeed==0){
		myCentiAmp=0;
	}
	myAvgCentiAmp=myCentiAmp;
}



