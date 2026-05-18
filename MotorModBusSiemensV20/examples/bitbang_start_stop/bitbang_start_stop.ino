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
 */


#define DEVICE_ADDRESS 1 //The number you told the V20 is on the bus (default 1)

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


#include "Arduino.h"
const char mySketchName[] = "Serial control Siemens V20 inverter starting and stopping";
unsigned int crc_16(unsigned int length, unsigned char *buffer)
	{
		unsigned int i, j, temp_bit, temp_int, crc;
		crc = 0xFFFF;
		for (i = 0; i < length; i++)
			{
				temp_int = (unsigned char) *buffer++;
				crc ^= temp_int;
				for (j = 0; j < 8; j++)
					{
						temp_bit = crc & 0x0001;
						crc >>= 1;
						if (temp_bit != 0) crc ^= 0xA001;
					}
			}
		return crc;
	}
/** send a message to V20 and wait delayBeforRead for a response
 * Write out the send message and the recieved response to serial
 * return true if we got an all ok response message
 *
 */
bool sendMessage(uint8_t *message, uint8_t messageLength, uint16_t delayBeforRead)
	{
		//send the message
		Serial1.write(message, messageLength);
		//write to serial
		Serial.println("");
		Serial.print("send :");
		for (int curchar = 0; curchar < messageLength; curchar++)
			{
				Serial.print(message[curchar], HEX);
				Serial.print(' ');
			}
		Serial.println("");
		//wait for a response
		delay(delayBeforRead);
		Serial.print("Received :");
		//read the response and write it to serial
		uint8_t response[3];
		uint8_t readindex = 0;
		while (Serial1.available() > 0)
			{
				Serial1.readBytes(response + readindex, 1);
				Serial.print(response[readindex], HEX);
				Serial.print(' ');
				readindex++;
				if (readindex > 2)
					{
						readindex = 2;
					}
			}

		Serial.println("");
		//check the validity of the received response
		return (readindex == 2) && (message[0] == response[0]) && (message[1] == response[1]);
	}
void setup()
	{
		delay(500); //allow some time for me to control the serial monitor and handle the V20 error state
		Serial.begin(115200);
		Serial.println(mySketchName);
		Serial1.begin(9600, SERIAL_8E1);

	}

void loop()
	{
		// all this are constants but repeated for readability
		int delayBeforRead = 100;
		uint16_t resetV20Key = 0x4728;
		uint16_t startMotor = 0x047F;
		uint16_t stopMotor = 0x047E;

		uint16_t numberOfValues = 2;
		uint8_t numberOfbytes = numberOfValues * 2;

		uint16_t stopFrequency = 0;
		uint8_t stopMotorMessageLength = 9 + numberOfbytes;
		uint8_t stopMotorMessage[stopMotorMessageLength];
		stopMotorMessage[0] = DEVICE_ADDRESS;
		stopMotorMessage[1] = FUNCTION_CODE_WRITE_MULTIPLE_REGISTERS;
		stopMotorMessage[2] = highByte(ADDRESS_STW);
		stopMotorMessage[3] = lowByte(ADDRESS_STW);
		stopMotorMessage[4] = highByte(numberOfValues);
		stopMotorMessage[5] = lowByte(numberOfValues);
		stopMotorMessage[6] = numberOfbytes;
		stopMotorMessage[7] = highByte(stopMotor);
		stopMotorMessage[8] = lowByte(stopMotor);
		stopMotorMessage[9] = highByte(stopFrequency);
		stopMotorMessage[10] = lowByte(stopFrequency);
		unsigned int crc = crc_16(stopMotorMessageLength - 2, stopMotorMessage);
		stopMotorMessage[stopMotorMessageLength - 2] = lowByte(crc);
		stopMotorMessage[stopMotorMessageLength - 1] = highByte(crc);

		uint8_t StartMotorMessageLength = 9 + numberOfbytes;
		uint8_t StartMotorMessage[StartMotorMessageLength];
		StartMotorMessage[0] = 1;
		StartMotorMessage[1] = FUNCTION_CODE_WRITE_MULTIPLE_REGISTERS;
		StartMotorMessage[2] = highByte(ADDRESS_STW);
		StartMotorMessage[3] = lowByte(ADDRESS_STW);
		StartMotorMessage[4] = highByte(numberOfValues);
		StartMotorMessage[5] = lowByte(numberOfValues);
		StartMotorMessage[6] = numberOfbytes;
		StartMotorMessage[7] = highByte(startMotor);
		StartMotorMessage[8] = lowByte(startMotor);
		StartMotorMessage[9] = highByte(runFrequency);
		StartMotorMessage[10] = lowByte(runFrequency);
		crc = crc_16(StartMotorMessageLength - 2, StartMotorMessage);
		StartMotorMessage[StartMotorMessageLength - 2] = lowByte(crc);
		StartMotorMessage[StartMotorMessageLength - 1] = highByte(crc);

		uint16_t numberOfValuesToRead = 2;
		uint8_t readV20StatusMessageLength = 8;
		uint8_t readV20StatusMessage[readV20StatusMessageLength];
		readV20StatusMessage[0] = DEVICE_ADDRESS;
		readV20StatusMessage[1] = FUNCTION_CODE_READ_HOLDING_REGISTERS;
		readV20StatusMessage[2] = highByte(ADDRESS_ZSW);
		readV20StatusMessage[3] = lowByte(ADDRESS_ZSW);
		readV20StatusMessage[4] = highByte(numberOfValuesToRead);
		readV20StatusMessage[5] = lowByte(numberOfValuesToRead);
		crc = crc_16(readV20StatusMessageLength - 2, readV20StatusMessage);
		readV20StatusMessage[readV20StatusMessageLength - 2] = lowByte(crc);
		readV20StatusMessage[readV20StatusMessageLength - 1] = highByte(crc);


		numberOfValuesToRead = 1;
		uint8_t readPowerConsumptionMessageLength = 8;
		uint8_t readPowerConsumptionMessage[readPowerConsumptionMessageLength];
		readPowerConsumptionMessage[0] = DEVICE_ADDRESS;
		readPowerConsumptionMessage[1] = FUNCTION_CODE_READ_HOLDING_REGISTERS;
		readPowerConsumptionMessage[2] = highByte(ACTUAL_POWER);
		readPowerConsumptionMessage[3] = lowByte(ACTUAL_POWER);
		readPowerConsumptionMessage[4] = highByte(numberOfValuesToRead);
		readPowerConsumptionMessage[5] = lowByte(numberOfValuesToRead);
		crc = crc_16(readPowerConsumptionMessageLength - 2, readPowerConsumptionMessage);
		readPowerConsumptionMessage[readPowerConsumptionMessageLength - 2] = lowByte(crc);
		readPowerConsumptionMessage[readPowerConsumptionMessageLength - 1] = highByte(crc);


		//end of constant initialization part

		static uint16_t counter = 0;
		static uint16_t status = 0;
		sendMessage(readV20StatusMessage, readV20StatusMessageLength, delayBeforRead);
		sendMessage(readPowerConsumptionMessage, readPowerConsumptionMessageLength, delayBeforRead);
		if (status == 0)
			{ //reset V20 to clear error caused by uploading sketch while already connected to V20
				if (sendMessage(resetV20Message, resetV20MessageLength, 3000))
					{ // switch to stopping after successful reset
						status = 1;
						Serial.println("-------------------------------->Stopping the motor");
						sendMessage(stopMotorMessage, stopMotorMessageLength, delayBeforRead);
					}
			}
		if (status == 1)
			{
				counter++;
				if (counter > 100)
					{ // switch to run after some time
						counter = 0;
						status = 2;
						Serial.println("-------------------------------->Starting the motor");
						sendMessage(StartMotorMessage, StartMotorMessageLength, delayBeforRead);
					}
			}
		if (status == 2)
			{
				counter++;
				if (counter > 100)
					{ // switch to run after some time
						counter = 0;
						status = 1;
						Serial.println("--------------------------------->Stopping the motor");
						sendMessage(stopMotorMessage, stopMotorMessageLength, delayBeforRead);
					}
			}

	}
