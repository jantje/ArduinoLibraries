/*
 * sample controlling one motor controlled via V20 inverter via modbus protocol
 * This program "bit bangs" the V20
 * Select 1 define only to select the bits to bang
 * This code assumes a factory reset V20 with the connection macro cn011 enabled (see page 81 of the v20 instruction manual)
 * You need to increase the watchdog time (p2014) or the V2 will fault (At least I think so
 * And following connections between your RS485 breakout (I used grove) and V20
 * gnd to gnd
 * A to P+
 * B to P-
 * (No extra resistor needed for short lines in a "lab" environment)
 * If there are problems:
 * Have a short as possible wires between your RS485 and V20
 * Check parameters r2023 to r2031
 *
 * Note
 * 1) the documentation is inconsistent in regards to the high and low bit of the crc
 * All values are high low but crc is low high.
 * 2) the registers you can read/write are not the same as the parameters.
 * You can find the registers on page 191 use the values in the converter column not the MODBUS column:-(
 * 3)if you do not get response messages from the sV20 Check parameters r2023 to r2031 on the converter
 * 4) If you use sloeber and created a project based on a library delete the library folder as this code is not using the library
 * 5) adjust the serial port and device address below to your setup
 */

#define DEVICE_ADDRESS 1 //The number you told the V20 is on the bus (default 1)
#define SIEMENS_SERIAL Serial5 //Set the serial port used to communicate with Siemens V20
//The 3 function codes
#define FUNCTION_CODE_WRITE_MULTIPLE_REGISTERS 0x10
#define FUNCTION_CODE_WRITE_SINGLE_REGISTER 0x06
#define FUNCTION_CODE_READ_HOLDING_REGISTERS 0x03

//Important addresses
#define ADDRESS_STW	99	//read write;
#define ADDRESS_HSW	100 //read write; speed wanted in % (multiply by 100) of max frequency
#define ADDRESS_ZSW	109	//read only
#define ADDRESS_HIW	110  //read only; actual speed in % (multiply by 100) of max frequency
#define ACTUAL_POWER 27 //actual power * 100 kW

#include "Arduino.h"
const char mySketchName[] =
		"Serial control Siemens V20 inverter starting and stopping";
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
uint8_t responseFromV20[255];
/** send a message to V20 and wait delayBeforRead for a response
 * Write out the send message and the recieved response to serial
 * return true if we got an all ok response message
 *
 */
bool sendMessage(uint8_t *message, uint8_t messageLength,
		uint16_t delayBeforRead) {
	//send the message
	SIEMENS_SERIAL.write(message, messageLength);
	static uint32_t prefMilis=millis();
	uint32_t curMillis=millis();
	if(curMillis-prefMilis>600){
		//6700 ms between calls may be excessive
		Serial.println("EXCESSIVE DELAY BETWEEN SENDS -------");
		Serial.println(curMillis-prefMilis);
	}
	prefMilis=curMillis;


	//wait for a response
	delay(delayBeforRead);
	//read the response and write it to serial
	uint8_t readindex = 0;
	while (SIEMENS_SERIAL.available() > 0) {
		SIEMENS_SERIAL.readBytes(responseFromV20 + readindex, 1);
		readindex++;
	}

	//check the validity of the received response and stop if ok
	bool ret = (readindex >= 2) && (message[0] == responseFromV20[0])
			&& (message[1] == responseFromV20[1]);
	//Disabling the test below makes there will be a log for all send messages
	//not only for the failed ones
	if (ret) {
		return true;
	}

	//something went wrong so log the send and recieved message
	Serial.println("");
	if (ret) {
		Serial.print("send :");
	} else {
		Serial.print("send failed :");
	}
	for (int curchar = 0; curchar < messageLength; curchar++) {
		Serial.printf("%02x ", message[curchar]);
	}
	Serial.println("");
	Serial.print("Received :");
	for (int curchar = 0; curchar < readindex; curchar++) {
		Serial.printf("%02x ", responseFromV20[curchar]);
	}
	Serial.println("");
	return ret;

}

uint16_t getRecievedValueAtBit(uint8_t bit) {
	return (((uint16_t) responseFromV20[bit]) << 8)
			+ ((uint16_t) responseFromV20[bit + 1]);
}

#define PRINT(x) 	\
	uint16_t x = getRecievedValueAtBit(index);\
	index+=2;\
	Serial.print(#x);\
	Serial.print(" : ");\
	Serial.println(x);

void dumpStatusMessage1() {
	uint16_t index = 3;

	PRINT(watchdogTime);
	PRINT(watchdogAction);
	PRINT(frequencySetpoint);
	PRINT(runEnable);
	PRINT(turnDirection);
	PRINT(startCommand);
	PRINT(faultAcknowledgement);
	PRINT(PIDSetpointReference);
	PRINT(PIDEnable);
	PRINT(CurrentLimit);
	PRINT(accelerationTime);
	PRINT(decelerationTime);
}

void dumpStatusMessage2() {
	uint16_t index = 3;

	PRINT(digitalOutput1);
	PRINT(digitalOutput2);
	PRINT(referenceFrequency);
	PRINT(PIDUpperLimt);
	PRINT(PIDLowerLimit);
	PRINT(proportionalGain);
	PRINT(integralGain);
	PRINT(differenceGain);
	PRINT(feedbackGain);
	PRINT(lowPass);
	PRINT(frequencyOutput);
	PRINT(speed);
	PRINT(currentFiltered);
	PRINT(torque);
	PRINT(actualPower);
	PRINT(totalkwatt);
	PRINT(dcbusVoltage);
	PRINT(reference);
	PRINT(ratedPower);
	PRINT(voltageOutput);
	PRINT(forwardReverse);
	PRINT(stopRun);
	PRINT(runAtmaxFrequency);


}

void dumpStatusMessage3() {
	uint16_t index = 3;

	PRINT(controlMode);
	PRINT(enable);
	PRINT(readyToRun);
	PRINT(analogInput1);
	PRINT(analogInput2);
	PRINT(analogOuput1);
	PRINT(actualFrequency);
	PRINT(PIDSetpoitOutput);
	PRINT(PIDOutput);
	PRINT(PIDFeedback);
	PRINT(digitalInput1);
	PRINT(digitalInput2);
	PRINT(digitalInput3);
	PRINT(digitalInput4);
	PRINT(fault);
	PRINT(fault1);
	PRINT(fault2);
	PRINT(fault3);
	PRINT(warning);
	PRINT(lastWarning);
	PRINT(converterVersion);
	PRINT(converterModel);
}

void dumpPowerMessage() {
	uint16_t power = getRecievedValueAtBit(3);
	Serial.print("power consumption : ");
	Serial.print(power);
	Serial.println(" deci watt");
}

void setup() {
	delay(500); //cause the V20 to timeout to test F72 study
	Serial.begin(115200);
	Serial.println(mySketchName);
	SIEMENS_SERIAL.begin(9600, SERIAL_8E1);

}

void loop() {
	// all this are constants but repeated for readability
	int delayBeforeRead = 100;


	uint16_t runFrequency = 5000;
	uint16_t runEnable = 1;
	uint16_t runDisable = 0;
	uint16_t turnDirectionForward = 0;
	uint16_t startCommand = 1;
	uint16_t stopCommand = 0;
	uint16_t faultAcknowledgement = 1;
	uint16_t noFaultAcknowledgement = 0;
	uint16_t stopFrequency = 0;

	uint16_t numberOfValues = 5;
	uint8_t numberOfbytes = numberOfValues * 2;
	uint8_t stopMotorMessageLength = 9 + numberOfbytes;
	uint8_t stopMotorMessage[stopMotorMessageLength];
	stopMotorMessage[0] = DEVICE_ADDRESS;
	stopMotorMessage[1] = FUNCTION_CODE_WRITE_MULTIPLE_REGISTERS;
	stopMotorMessage[2] = highByte(2);
	stopMotorMessage[3] = lowByte(2);
	stopMotorMessage[4] = highByte(numberOfValues);
	stopMotorMessage[5] = lowByte(numberOfValues);
	stopMotorMessage[6] = numberOfbytes;
	stopMotorMessage[7] = highByte(stopFrequency);
	stopMotorMessage[8] = lowByte(stopFrequency);
	stopMotorMessage[9] = highByte(runDisable);
	stopMotorMessage[10] = lowByte(runDisable);
	stopMotorMessage[11] = highByte(turnDirectionForward);
	stopMotorMessage[12] = lowByte(turnDirectionForward);
	stopMotorMessage[13] = highByte(stopCommand);
	stopMotorMessage[14] = lowByte(stopCommand);
	stopMotorMessage[15] = highByte(noFaultAcknowledgement);
	stopMotorMessage[16] = lowByte(noFaultAcknowledgement);
	unsigned int crc = crc_16(stopMotorMessageLength - 2, stopMotorMessage);
	stopMotorMessage[stopMotorMessageLength - 2] = lowByte(crc);
	stopMotorMessage[stopMotorMessageLength - 1] = highByte(crc);




	numberOfValues = 1;
	numberOfbytes = numberOfValues * 2;
	uint8_t erazeFaultMessage2Length = 9 + numberOfbytes;
	uint8_t erazeFaultMessage2[erazeFaultMessage2Length];
	erazeFaultMessage2[0] = DEVICE_ADDRESS;
	erazeFaultMessage2[1] = FUNCTION_CODE_WRITE_MULTIPLE_REGISTERS;
	erazeFaultMessage2[2] = highByte(6);
	erazeFaultMessage2[3] = lowByte(6);
	erazeFaultMessage2[4] = highByte(numberOfValues);
	erazeFaultMessage2[5] = lowByte(numberOfValues);
	erazeFaultMessage2[6] = numberOfbytes;
	erazeFaultMessage2[7] = highByte(faultAcknowledgement);
	erazeFaultMessage2[8] = lowByte(faultAcknowledgement);
	crc = crc_16(erazeFaultMessage2Length - 2, erazeFaultMessage2);
	erazeFaultMessage2[erazeFaultMessage2Length - 2] = lowByte(crc);
	erazeFaultMessage2[erazeFaultMessage2Length - 1] = highByte(crc);


	numberOfValues = 5;
	numberOfbytes = numberOfValues * 2;
	uint8_t StartMotorMessageLength = 9 + numberOfbytes;
	uint8_t StartMotorMessage[StartMotorMessageLength];
	StartMotorMessage[0] = DEVICE_ADDRESS;
	StartMotorMessage[1] = FUNCTION_CODE_WRITE_MULTIPLE_REGISTERS;
	StartMotorMessage[2] = highByte(2);
	StartMotorMessage[3] = lowByte(2);
	StartMotorMessage[4] = highByte(numberOfValues);
	StartMotorMessage[5] = lowByte(numberOfValues);
	StartMotorMessage[6] = numberOfbytes;
	StartMotorMessage[7] = highByte(runFrequency);
	StartMotorMessage[8] = lowByte(runFrequency);
	StartMotorMessage[9] = highByte(runEnable);
	StartMotorMessage[10] = lowByte(runEnable);
	StartMotorMessage[11] = highByte(turnDirectionForward);
	StartMotorMessage[12] = lowByte(turnDirectionForward);
	StartMotorMessage[13] = highByte(startCommand);
	StartMotorMessage[14] = lowByte(startCommand);
	StartMotorMessage[15] = highByte(noFaultAcknowledgement);
	StartMotorMessage[16] = lowByte(noFaultAcknowledgement);
	crc = crc_16(StartMotorMessageLength - 2, StartMotorMessage);
	StartMotorMessage[StartMotorMessageLength - 2] = lowByte(crc);
	StartMotorMessage[StartMotorMessageLength - 1] = highByte(crc);

	uint16_t numberOfValuesToRead = 12;
	uint8_t readV20StatusMessageLength1 = 8;
	uint8_t readV20StatusMessage1[readV20StatusMessageLength1];
	readV20StatusMessage1[0] = DEVICE_ADDRESS;
	readV20StatusMessage1[1] = FUNCTION_CODE_READ_HOLDING_REGISTERS;
	readV20StatusMessage1[2] = highByte(0);
	readV20StatusMessage1[3] = lowByte(0);
	readV20StatusMessage1[4] = highByte(numberOfValuesToRead);
	readV20StatusMessage1[5] = lowByte(numberOfValuesToRead);
	crc = crc_16(readV20StatusMessageLength1 - 2, readV20StatusMessage1);
	readV20StatusMessage1[readV20StatusMessageLength1 - 2] = lowByte(crc);
	readV20StatusMessage1[readV20StatusMessageLength1 - 1] = highByte(crc);

	numberOfValuesToRead = 24;
	uint8_t readV20StatusMessageLength2 = 8;
	uint8_t readV20StatusMessage2[readV20StatusMessageLength2];
	readV20StatusMessage2[0] = DEVICE_ADDRESS;
	readV20StatusMessage2[1] = FUNCTION_CODE_READ_HOLDING_REGISTERS;
	readV20StatusMessage2[2] = highByte(13);
	readV20StatusMessage2[3] = lowByte(13);
	readV20StatusMessage2[4] = highByte(numberOfValuesToRead);
	readV20StatusMessage2[5] = lowByte(numberOfValuesToRead);
	crc = crc_16(readV20StatusMessageLength2 - 2, readV20StatusMessage2);
	readV20StatusMessage2[readV20StatusMessageLength2 - 2] = lowByte(crc);
	readV20StatusMessage2[readV20StatusMessageLength2 - 1] = highByte(crc);

	numberOfValuesToRead = 24;
	uint8_t readV20StatusMessageLength3 = 8;
	uint8_t readV20StatusMessage3[readV20StatusMessageLength3];
	readV20StatusMessage3[0] = DEVICE_ADDRESS;
	readV20StatusMessage3[1] = FUNCTION_CODE_READ_HOLDING_REGISTERS;
	readV20StatusMessage3[2] = highByte(36);
	readV20StatusMessage3[3] = lowByte(36);
	readV20StatusMessage3[4] = highByte(numberOfValuesToRead);
	readV20StatusMessage3[5] = lowByte(numberOfValuesToRead);
	crc = crc_16(readV20StatusMessageLength3 - 2, readV20StatusMessage3);
	readV20StatusMessage3[readV20StatusMessageLength3 - 2] = lowByte(crc);
	readV20StatusMessage3[readV20StatusMessageLength3 - 1] = highByte(crc);

	numberOfValuesToRead = 1;
	uint8_t readPowerConsumptionMessageLength = 8;
	uint8_t readPowerConsumptionMessage[readPowerConsumptionMessageLength];
	readPowerConsumptionMessage[0] = DEVICE_ADDRESS;
	readPowerConsumptionMessage[1] = FUNCTION_CODE_READ_HOLDING_REGISTERS;
	readPowerConsumptionMessage[2] = highByte(ACTUAL_POWER);
	readPowerConsumptionMessage[3] = lowByte(ACTUAL_POWER);
	readPowerConsumptionMessage[4] = highByte(numberOfValuesToRead);
	readPowerConsumptionMessage[5] = lowByte(numberOfValuesToRead);
	crc = crc_16(readPowerConsumptionMessageLength - 2,
			readPowerConsumptionMessage);
	readPowerConsumptionMessage[readPowerConsumptionMessageLength - 2] =
			lowByte(crc);
	readPowerConsumptionMessage[readPowerConsumptionMessageLength - 1] =
			highByte(crc);


	numberOfValues=2;
	numberOfbytes = numberOfValues * 2;
	uint16_t stopMotor = 0x047E;
	uint8_t enchantMessageLength = 9 + numberOfbytes;
	uint8_t enchantMessage[enchantMessageLength];
	enchantMessage[0] = DEVICE_ADDRESS;
	enchantMessage[1] = FUNCTION_CODE_WRITE_MULTIPLE_REGISTERS;
	enchantMessage[2] = highByte(ADDRESS_STW);
	enchantMessage[3] = lowByte(ADDRESS_STW);
	enchantMessage[4] = highByte(numberOfValues);
	enchantMessage[5] = lowByte(numberOfValues);
	enchantMessage[6] = numberOfbytes;
	enchantMessage[7] = highByte(stopMotor);
	enchantMessage[8] = lowByte(stopMotor);
	enchantMessage[9] = highByte(stopFrequency);
	enchantMessage[10] = lowByte(stopFrequency);
	crc = crc_16(enchantMessageLength - 2, enchantMessage);
	enchantMessage[enchantMessageLength - 2] = lowByte(crc);
	enchantMessage[enchantMessageLength - 1] = highByte(crc);



	//end of constant initialization part

	static uint32_t lastActionTime = millis();
	static uint32_t lastPowerRead = millis();
	static uint8_t status=0;
	uint32_t loopMillis=millis();

	if (status == 0) {
		{
			status = 1;
			Serial.println("------>magical enchantment 1 to get V20 to work");
			sendMessage(enchantMessage, enchantMessageLength,
								delayBeforeRead);
			sendMessage(erazeFaultMessage2, erazeFaultMessage2Length,
					delayBeforeRead);
			sendMessage(stopMotorMessage, stopMotorMessageLength,
					delayBeforeRead);
		}
	}

	if (status == 1) {
		if (loopMillis-lastActionTime > 10000) { // switch to run after some time
			lastActionTime=loopMillis;
			status = 2;
			Serial.println(	"---------------->Starting the motor");
			sendMessage(StartMotorMessage, StartMotorMessageLength,
					delayBeforeRead);
		}
	}
	if (status == 2) {
		if (loopMillis-lastActionTime > 10000) { // switch to stopped after some time
			lastActionTime=loopMillis;
			status = 1;
			Serial.println(	"----------------->Stopping the motor");
			sendMessage(stopMotorMessage, stopMotorMessageLength,
					delayBeforeRead);
		}
	}

	if ( loopMillis-lastPowerRead > 500) {
		lastPowerRead=loopMillis;
		sendMessage(readV20StatusMessage1, readV20StatusMessageLength1,
				delayBeforeRead);
//		dumpStatusMessage1();
//		sendMessage(readV20StatusMessage2, readV20StatusMessageLength2,
//				delayBeforRead);
//		dumpStatusMessage2();
//		sendMessage(readV20StatusMessage3, readV20StatusMessageLength3,
//				delayBeforRead);
//		dumpStatusMessage3();
		sendMessage(readPowerConsumptionMessage, readPowerConsumptionMessageLength,
				delayBeforeRead);
		dumpPowerMessage();
	}


}
