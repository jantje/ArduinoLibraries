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

/*
 * The first register is the watchdog value and corresponds with p2014[0]
 * If the watchdog is set to 100 the serial monitor should show
 * send :
 * 1 3 0 0 0 4 44 9
 * recieved :
 * 1 3 8 0 64 0 1 0 0 0 0 8D D1
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
const char mySketchName[] = "Serial control Siemens V20 inverter motor reading some registers";
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
				int delayBeforRead=1000;
		uint16_t address=0;
		uint16_t numberOfValuesToRead=4;
		uint8_t messageLength=8;
		uint8_t message[messageLength];
				message[0] = DEVICE_ADDRESS;
				message[1] = FUNCTION_CODE_READ_HOLDING_REGISTERS;
				message[2] = highByte(address);
				message[3] = lowByte(address);
				message[4] = highByte(numberOfValuesToRead);
				message[5] = lowByte(numberOfValuesToRead);
				unsigned int crc= crc_16(messageLength-2,message);
				message[6] = lowByte(crc);
				message[7] = highByte(crc);
				sendMessage(		 message, messageLength, delayBeforRead);
	}
