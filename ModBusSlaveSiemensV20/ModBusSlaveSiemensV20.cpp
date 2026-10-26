/*
 * ModBusSiemensV20.cpp
 *
 *  Created on: 14 jul. 2023
 *      Author: jan
 */

#include <ModBusSlaveSiemensV20.h>

#ifdef USE_LOOP_MILLIS
extern uint32_t loopMillis;
#else
static uint32_t loopMillis = millis();
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

ModBusSlaveSiemensV20::ModBusSlaveSiemensV20(uint8_t slaveAddress,
		const char *name) :
		MotorInterface(name), ModBusSlaveInterface(slaveAddress) {
	mySlaveAddress = slaveAddress;
}

void ModBusSlaveSiemensV20::switchToState(MOTOR_STATE newState) {
	switch (newState) {
	case MOTOR_STOPPED:
	case MOTOR_RUNNING:
		break;
	case MOTOR_BOOTING:
		//this should not happen
	case MOTOR_ERROR:
		 myEnchantmentNeedsToBeSend = true;
		 myErrorFlagsNeedToBeCleared = true;
		 myMotorNeedsToTurnOff = true;
		 myMotorNeedsToTurnOn = false;
		 myIsWaitingForResponse = false;
		 myMotorNeedsSpeedChange = false;
		 myCurentRequestNeeded = false;
		 myWatchdogRequestNeeded = false;
		break;
	}
	addBusinessEventInfo(LOG_LEVEL_OPERATIONAL, "switching from state \"", true,
			false);
	addBusinessEventInfo(LOG_LEVEL_OPERATIONAL, getMotorStateName(myState),
			false, false);
	addBusinessEventInfo(LOG_LEVEL_OPERATIONAL, "\" to state \"", false, false);
	addBusinessEventInfo(LOG_LEVEL_OPERATIONAL, getMotorStateName(newState),
			false, false);
	addBusinessEventInfo(LOG_LEVEL_OPERATIONAL, "\"", false, true);
	myState = newState;
}

void ModBusSlaveSiemensV20::loop() {
#ifndef USE_MAIN_LOOP_MILLIS
	loopMillis = millis();
#endif
	if ((myState == MOTOR_ERROR) || (myState == MOTOR_BOOTING)) {
		// do nothing
		return;
	}
	if (loopMillis - myLastResponseRecieved > getWatchDogTime()) {
		// the siemens controller must have timed out by now
		addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
				"watchdog time has reached");
		switchToState(MOTOR_ERROR);
	}

	if ((myNewRequestedSpeed != myRequestedSpeed)
			& (myState == MOTOR_RUNNING)) {
		// the motor speed needs to change
		if (abs(myNewRequestedSpeed) < MINIMUM_SPEED) {
			myNewRequestedSpeed = 0;
			myRequestedSpeed = 0;
			myMotorNeedsToTurnOff = true;
		} else {
			myMotorNeedsSpeedChange = true;
		}
	}

	if (loopMillis - myLastCurentRequestMessageSendTime
			> myCurentRequestInterval) {
		myCurentRequestNeeded = true;
		return;
	}

	if (loopMillis - myLastWatchdogSendTime > getWatchDogTime()) {
		myWatchdogRequestNeeded = true;
		return;
	}

}

void ModBusSlaveSiemensV20::setup() {
}

void ModBusSlaveSiemensV20::motorOn() {
	if ((myState == MOTOR_ERROR) || (myState == MOTOR_BOOTING)) {
		addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
				"Received motorOn request while in error/booting state");
		return;
	}
	if (myMotorNeedsToTurnOff) {
		addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
				"Received motorOn request when off is also requested");
	} else {
		addBusinessEventInfo(LOG_LEVEL_DEBUG, "Received motorOn request");
	}
	myMotorNeedsToTurnOff = false;
	myMotorNeedsToTurnOn = true;
}

void ModBusSlaveSiemensV20::motorOff() {
	if ((myState == MOTOR_ERROR) || (myState == MOTOR_BOOTING)) {
		addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
				"Received motorOff request while in error state");
		return;
	}
	if (myMotorNeedsToTurnOn) {
		addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
				"Received motorOff request when on is also requested");
	} else {
		addBusinessEventInfo(LOG_LEVEL_DEBUG, "Received motorOff request");
	}
	myMotorNeedsToTurnOn = false;
	myMotorNeedsToTurnOff = true;
}

bool ModBusSlaveSiemensV20::emergencyBreak() {
	addBusinessEventInfo(LOG_LEVEL_DEBUG, "Received emergencyBreak request");
	motorOff();
	return getActualSpeed() == 0;
}

#ifdef I_USE_SERIAL_REGISTER
//TODO reactivate line below debug activity
//#define LOG_VIA_SERIAL_REGISTER
#endif
#ifndef MODBUS_LOG_LEVEL_TO_REPORT
#define MODBUS_LOG_LEVEL_TO_REPORT LOG_LEVEL_OPERATIONAL
#endif
void ModBusSlaveSiemensV20::addBusinessEventInfo(uint8_t level,
		const __FlashStringHelper *info, bool intro, bool newLine) {
	if (level < MODBUS_LOG_LEVEL_TO_REPORT) {
		return;
	}
#ifdef LOG_VIA_SERIAL_REGISTER
	static const char EVENT_SEPARATOR[] = "<br>";
	strlcat_P(myBusinessEvent, (char* ) info, BUSINESSEVENTSIZE);
	if (newLine) {
		strlcat(myBusinessEvent, EVENT_SEPARATOR, BUSINESSEVENTSIZE);
	}
#else
	if (intro) {
		Serial.print(loopMillis);
		Serial.print(" modbusSlave \"");
		Serial.print(myName);
		Serial.print("\" ");
	}
	Serial.print(info);
	if (newLine) {
		Serial.println();
	}
#endif
}
void ModBusSlaveSiemensV20::addBusinessEventInfo(uint8_t level,
		const char *info, bool intro, bool newLine) {
	if (level < MODBUS_LOG_LEVEL_TO_REPORT) {
		return;
	}
#ifdef LOG_VIA_SERIAL_REGISTER
	strlcat(myBusinessEvent, info, BUSINESSEVENTSIZE);
	if (newLine) {
		strlcat(myBusinessEvent, EVENT_SEPARATOR, BUSINESSEVENTSIZE);
	}
#else
	if (intro) {
		Serial.print(loopMillis);
		Serial.print(" modbusSlave \"");
		Serial.print(myName);
		Serial.print("\" ");
	}
	Serial.print(info);
	if (newLine) {
		Serial.println();
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
bool ModBusSlaveSiemensV20::isStopped() const {
	if (myMotorNeedsToTurnOff) {
		return false;
	}
	if (myRPMSpeed == 0) {
		return true;
	}
	return (myRequestedSpeed == 0);
}

/**
 * we assume the motor is at speed when the reported RPM is
 * bigger than 90% of the requested frequency multiuplied by
 * standard 3000 rpm for the motor
 */
bool ModBusSlaveSiemensV20::isAtSpeed() const {
	//TODO I divided the speed into 2 because the Siemens
	//does not go to the herz requested
	//still need to investigate why that is
	return myRPMSpeed >= 1700;//(getSpeedInCentiHerz(myRequestedSpeed/2)*54)/100;
}

#ifdef I_USE_SERIAL_REGISTER
void ModBusSlaveSiemensV20::serialRegister(const __FlashStringHelper* Name) {
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

void ModBusSlaveSiemensV20::responseOnLastMessage(uint8_t numRegisters,
		uint16_t registerValues[]) {
	myLastResponseRecieved = loopMillis;
	myFailedSends = 0;
	//Based on which message we had send set the correct flags
	switch (myLastMessageType) {
	case MESSAGETYPE_STOP_MOTOR:
		myMotorNeedsToTurnOff = false;
		switchToState(MOTOR_STOPPED);
		addBusinessEventInfo(LOG_LEVEL_OPERATIONAL, myName, false);
		addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
				" stop response message processed");

		break;
	case MESSAGETYPE_START_MOTOR:
		myMotorNeedsToTurnOn = false;
		switchToState(MOTOR_RUNNING);
		addBusinessEventInfo(LOG_LEVEL_OPERATIONAL, myName, false);
		addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
				" start running response message processed");
		break;
	case MESSAGETYPE_WATCHDOG:
		myWatchdogRequestNeeded = false;
		myLastWatchdogSendTime = loopMillis;
		addBusinessEventInfo(LOG_LEVEL_OPERATIONAL, myName, false);
		addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
				" get status response message processed");
		break;
	case MESSAGETYPE_START_V20_ENCHANTMENT:
		myEnchantmentNeedsToBeSend = false;
		addBusinessEventInfo(LOG_LEVEL_OPERATIONAL, myName, false);
		addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
				" enchantement response message processed");
		break;
	case MESSAGETYPE_CLEAR_ERROR_FLAGS:
		myErrorFlagsNeedToBeCleared = false;
		addBusinessEventInfo(LOG_LEVEL_OPERATIONAL, myName, false);
		addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
				" clear error flags message processed");
		break;
	case MESSAGETYPE_SET_MOTOR_SPEED:
		myMotorNeedsSpeedChange = false;
		addBusinessEventInfo(LOG_LEVEL_OPERATIONAL, myName, false);
		addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
				" set speed response message processed");
		break;
	case MESSAGETYPE_GET_CURRENT_USAGE:
		myCurentRequestNeeded = false;
		myLastCurentRequestMessageSendTime = loopMillis;
		if (numRegisters != 2) {
			addBusinessEventInfo(LOG_LEVEL_OPERATIONAL,
					F("Current request returned wrong number of values"));
			Serial.println(numRegisters);
		} else {
			myRPMSpeed = registerValues[0];
			myCentiAmp = registerValues[1];
			if (myRPMSpeed == 0) {
				myCentiAmp = 0;
			}
			myAvgCentiAmp = myCentiAmp;
		}
		addBusinessEventInfo(LOG_LEVEL_DEBUG,
				" get current usage response message processed");
		break;
	case MESSAGETYPE_NONE:
		break;
	}
}

bool ModBusSlaveSiemensV20::hasFC3ReadHoldingRegistersRequest(
		uint16_t *startAddress, uint16_t *numberOfRegister) {
	if (myEnchantmentNeedsToBeSend || myErrorFlagsNeedToBeCleared) {
		return false;
	}
	if (myWatchdogRequestNeeded) {
		addBusinessEventInfo(LOG_LEVEL_DEBUG, F("Sending watchdog request"),
				true);
		myLastMessageType = MESSAGETYPE_WATCHDOG;
		*startAddress = 0;
		*numberOfRegister = 12;
		return true;
	}
	if (myCurentRequestNeeded) {
		addBusinessEventInfo(LOG_LEVEL_DEBUG, F("Sending current request"),
				true);
		myLastMessageType = MESSAGETYPE_GET_CURRENT_USAGE;
		*startAddress = 24;
		*numberOfRegister = 2;
		return true;
	}
	return false;
}

bool ModBusSlaveSiemensV20::hasFC16WriteMultipleRegisterRequest(
		uint16_t *startAddress, uint16_t *NumberOfValues,
		uint16_t values[MAX_NUMBER_OF_REGISTERS]) {
	if (myEnchantmentNeedsToBeSend || myErrorFlagsNeedToBeCleared) {
		return false;
	}
	if (myMotorNeedsToTurnOn) {
		addBusinessEventInfo(LOG_LEVEL_DEBUG, F("Sending start request"), true);
		myLastMessageType = MESSAGETYPE_START_MOTOR;
		myActualSpeed = myRequestedSpeed = myNewRequestedSpeed;
		*startAddress = FREQREGISTER;
		*NumberOfValues = 5;
		values[0] = getSpeedInCentiHerz(myRequestedSpeed);
		values[1] = RUN_ENABLE_FLAG;
		values[2] = getdirection(myRequestedSpeed);
		values[3] = START_COMMAND_FLAG;
		values[4] = FAULT_ACKNOWELDGEMENT_FLAG;
		return true;
	}
	if (myMotorNeedsToTurnOff) {
		addBusinessEventInfo(LOG_LEVEL_DEBUG, F("Sending stop request"), true);
		myLastMessageType = MESSAGETYPE_STOP_MOTOR;
		myActualSpeed = 0;
		*startAddress = FREQREGISTER;
		*NumberOfValues = 5;
		values[0] = 0;
		values[1] = RUN_DISABLE_FLAG;
		values[2] = getdirection(myRequestedSpeed);
		values[3] = STOP_COMMAND_FLAG;
		values[4] = NO_FAULT_ACKNOWELDGEMENT_FLAG;
		return true;
	}
	if (myMotorNeedsSpeedChange) {
		addBusinessEventInfo(LOG_LEVEL_DEBUG, F("Sending speed change request"),
				true);
		myLastMessageType = MESSAGETYPE_SET_MOTOR_SPEED;
		*startAddress = FREQREGISTER;
		*NumberOfValues = 5;
		values[0] = getSpeedInCentiHerz(myRequestedSpeed);
		values[1] = RUN_ENABLE_FLAG;
		values[2] = getdirection(myRequestedSpeed);
		values[3] = START_COMMAND_FLAG;
		values[4] = FAULT_ACKNOWELDGEMENT_FLAG;
		return true;
	}
	return false;
}

bool ModBusSlaveSiemensV20::hasFC6WriteSingleRegisterRequest(uint16_t *address,
		uint16_t *value) {
	if (myEnchantmentNeedsToBeSend) {
		/**
		 * I have no clue why I need to send this message but if I do not do so
		 * This message only neds to be send once at the beginning
		 * of the communication.
		 * Which means that a start/stop of the V20 requires this message to be send
		 * The V20 simply does not start the motor
		 */
		addBusinessEventInfo(LOG_LEVEL_DEBUG,
				F("Sending start v20 enchantment request"));
		myLastMessageType = MESSAGETYPE_START_V20_ENCHANTMENT;
		*address = ADDRESS_STW;
		*value = 0x047E; 	//		uint16_t stopMotor = 0x047E;
		return true;
	}
	if (myErrorFlagsNeedToBeCleared) {
		addBusinessEventInfo(LOG_LEVEL_DEBUG, F("Sending clear flags request"),
				true);
		myLastMessageType = MESSAGETYPE_CLEAR_ERROR_FLAGS;
		*address = FAULTREGISTER;
		*value = FAULT_ACKNOWELDGEMENT_FLAG;
		return true;
	}
	return false;
}

bool ModBusSlaveSiemensV20::retryFailedSend() {
	myFailedSends++;
	return false;
}

const char* getMotorStateName(MOTOR_STATE state) {
	switch (state) {
	case MOTOR_STOPPED:
		return "Motor stopped";
	case MOTOR_RUNNING:
		return "Motor running";
	case MOTOR_ERROR:
		return "Motor error";
	case MOTOR_BOOTING:
		return "motor booting";
	}
	return "Unknown motor state";

}
