/*
 * ModBusSiemensV20.cpp
 *
 *  Created on: 14 jul. 2023
 *      Author: jan
 */

#include "MotorModBusSiemensV20.h"

MotorModBusSiemensV20::MotorModBusSiemensV20(uint8_t slaveAddress, Stream &serial)
				:
				mySerial(serial)
	{
		mySlaveAddress = slaveAddress;
		setConverterStateDescription();
	}

void MotorModBusSiemensV20::loop()
	{
		uint32_t loopMillis = millis();
		if (!myIsWaitingForResponse)
			{
				//check to see for rubbisch on the channel
				if (loopMillis - myLastMessageSendTime > mySendInterval)
					{
						addBusinessEventInfo(F("requestInverterState"), true);
						int sendResponse = requestInverterState(mySlaveAddress);
						switch (sendResponse)
							{
							case 0:
								addBusinessEventInfo(F("InverterState request send"), true);
								break;
							case 1:
								addBusinessEventInfo(F("InverterState request failed to send as we are waiting for response"), true);
								break;
							default:
								addBusinessEventInfo(F("InverterState request returned unknown error"), true);
								break;
							}
					}
				return;
			}

		if (millis() - myLastMessageSendTime > myResponseDelayTime)
			{
				//we waited long enough
				//lets assume the previous message was lost somehow
				addBusinessEventInfo(F("node x does not transmit"), true);
				myIsWaitingForResponse = false;
			}
		int numBytesAvailable = mySerial.available();
		if (numBytesAvailable > 0)
			{
				mySerial.readBytes(mySerialInBuffer + myAlreadyRead, numBytesAvailable);
				myAlreadyRead += numBytesAvailable;
				Serial.println("recieved from Siemens V20");
				for (int i = 0; i < myAlreadyRead; i++)
					{
						Serial.print((char) mySerialInBuffer[i]);
					}
				Serial.println();
				for (int i = 0; i < myAlreadyRead; i++)
					{
						Serial.print(mySerialInBuffer[i]);
						Serial.print(',');
					}
				Serial.println();

			}
		tryToParseRecievedMesage();

		setConverterStateDescription();
	}

void MotorModBusSiemensV20::setup()
	{
	}

void MotorModBusSiemensV20::motorOn()
	{
	}

void MotorModBusSiemensV20::motorOff()
	{
	}

bool MotorModBusSiemensV20::emergencyBreak()
	{
	}

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

/** @ingroup util_crc16
 Processor-independent CRC-16 calculation.

 Polynomial: x^16 + x^15 + x^2 + 1 (0xA001)<br>
 Initial value: 0xFFFF

 This CRC is normally used in disk-drive controllers.

 @param uint16_t crc (0x0000..0xFFFF)
 @param uint8_t a (0x00..0xFF)
 @return calculated CRC (0x0000..0xFFFF)
 */
static uint16_t crc16_update(uint16_t crc, uint8_t a)
	{
		int i;

		crc ^= a;
		for (i = 0; i < 8; ++i)
			{
				if (crc & 1)
					crc = (crc >> 1) ^ 0xA001;
				else
					crc = (crc >> 1);
			}

		return crc;
	}

/**
 * try to send a telegram to a slave
 * returns
 * 0 if message has been send successfully
 * 1 when we are waiting for a response and thus we should not send a message
 */
int MotorModBusSiemensV20::sendTelegram(uint8_t address, uint8_t length, uint8_t message[])
	{
		myLastUsedAddress = address;
		uint32_t curMillis = millis();
		if (myIsWaitingForResponse)
			{
				//We are waiting for a response
				Serial.println("sendTelegram waiting for response");
				return 1;
			}
		//at least RESPONSE_DELAY_TIME ms silence is needed on the bus before sending
		if (curMillis - myLastMessageRecievedTime <= RESPONSE_DELAY_TIME)
			{
				delay(RESPONSE_DELAY_TIME);
			}
//		mySerial.write(STX);
//		mySerial.write(length);
//		uint8_t bcc = length;
//		mySerial.write(address);
//		uint16_t bcc = 0xFFFF;
//		bcc = crc16_update(bcc, address);
//		bcc ^= address;
	//	Serial.println("Sending Siemens message");
		for (uint8_t index = 0; index < length; index++)
			{
				mySerial.write(message[index]);
				Serial.print(message[index]);
				Serial.print(",");
//				bcc = crc16_update(bcc, message[index]);
//				bcc ^= message[index];
			}
		// = crc_16(length, message);
		//	mySerial.write(lowByte(bcc));
//		mySerial.write(highByte(bcc));
		mySerial.write((uint8_t) 0);
		mySerial.write((uint8_t) 0);
		Serial.println();
		//mySerial.write(bcc);
		myIsWaitingForResponse = true;
		myLastMessageSendTime = millis(); //This is factually incorrect as the message may still be in the send queue
		return 0;
	}

int MotorModBusSiemensV20::sendMirrorTelegram(uint8_t address)
	{
	}

bool MotorModBusSiemensV20::hasTelegramResponse()
	{
	}

void MotorModBusSiemensV20::getTelegramResponse()
	{
	}

void MotorModBusSiemensV20::setConverterStateDescription() const
	{
		switch (myInverterState)
			{
			case 0:
				strlcpy_P((char*) myInverterStateDescription, (const char*) F("Commissioning mode (P0010 ≠ 0)"), STATE_SIZE);
				break;
			case 1:
				strlcpy_P((char*) myInverterStateDescription, (const char*) F("Inverter ready"), STATE_SIZE);
				break;
			case 2:
				strlcpy_P((char*) myInverterStateDescription, (const char*) F("Inverter fault active"), STATE_SIZE);
				break;
			case 3:
				strlcpy_P((char*) myInverterStateDescription, (const char*) F("Inverter starting (visible only while pre-charging DC link)"), STATE_SIZE);
				break;
			case 4:
				strlcpy_P((char*) myInverterStateDescription, (const char*) F("Inverter running"), STATE_SIZE);
				break;
			case 5:
				strlcpy_P((char*) myInverterStateDescription, (const char*) F("Stopping (ramping down)"), STATE_SIZE);
				break;
			case 6:
				strlcpy_P((char*) myInverterStateDescription, (const char*) F("Inverter inhibited"), STATE_SIZE);
				break;
			default:
				strlcpy_P((char*) myInverterStateDescription, (const char*) F("unKnownState"), STATE_SIZE);
			}
	}

/**
 * send a request to get the inverter state
 * returns the sendTelegram return
 */
int MotorModBusSiemensV20::requestInverterState(uint8_t address)
	{
		return readHoldingRegisters(address, 2, 1);
	}
int MotorModBusSiemensV20::readHoldingRegisters(uint8_t deviceAddress, uint32_t address, uint32_t numregisters)
	{
		//static uint16_t crc = 0;
		uint8_t message[8];
		message[0] = deviceAddress;
		message[1] = 0x03;
		message[2] = highByte(address);
		message[3] = lowByte(address);
		message[4] = highByte(numregisters);
		message[5] = lowByte(numregisters);
		unsigned int crc= crc_16(6,message);
		message[6] = highByte(crc);
		message[7] = lowByte(crc);
		//crc += 1;
		return sendTelegram(deviceAddress, 8, message);
	}

void MotorModBusSiemensV20::tryToParseRecievedMesage()
	{
		if (myAlreadyRead < 2)
			{
				//not enough data; wait for more data to arrive
				return;
			}
		int startOfPackage = 0;
		while (startOfPackage < myAlreadyRead && !mySerialInBuffer[startOfPackage] != STX)
			{
				startOfPackage++;
			}
		if (startOfPackage == myAlreadyRead || mySerialInBuffer[startOfPackage] != STX)
			{
				//startof message has not yet been found
				//or no  telegram size info read yet
				return;
			}
		int telegramSize = mySerialInBuffer[startOfPackage + 1];
		if (myAlreadyRead < (startOfPackage + telegramSize) + 2)
			{
				//the full message has not yet been received
				return;
			}
		//The full message has arrived; try to parse it
		//Check the parity code
		uint8_t parity = 0;
		for (int curbyte = startOfPackage + 1; curbyte < startOfPackage + telegramSize + 2; curbyte++)
			{
				parity ^= mySerialInBuffer[curbyte];
			}
		if (parity != mySerialInBuffer[telegramSize + 2])
			{
				//parity error
				addBusinessEventInfo(F("Parity error in received message"));
				return;
			}
		addBusinessEventInfo(F("message arrived but parsing code not yet implemented"));
	}

static const char EVENT_SEPARATOR[] = "<br>";
void MotorModBusSiemensV20::addBusinessEventInfo(const __FlashStringHelper *info, bool newLine)
	{
		strlcat_P(myBusinessEvent, (char*) info, BUSINESSEVENTSIZE);
		if (newLine)
			{
				strlcat(myBusinessEvent, EVENT_SEPARATOR, BUSINESSEVENTSIZE);
			}
	}
void MotorModBusSiemensV20::addBusinessEventInfo(const char *info, bool newLine)
	{
		strlcat(myBusinessEvent, info, BUSINESSEVENTSIZE);
		if (newLine)
			{
				strlcat(myBusinessEvent, EVENT_SEPARATOR, BUSINESSEVENTSIZE);
			}
	}

#ifdef I_USE_SERIAL_REGISTER
void MotorModBusSiemensV20::serialRegister(const __FlashStringHelper* Name, const __FlashStringHelper* MotorName)
	{
		MotorInterface::serialRegister(MotorName);
		FieldData::set(Name, F("BusinessEvent"), MOD_ERASE_ON_DUMP | MOD_OVERVIEW, myBusinessEvent);
		FieldData::set(Name, F("SlaveAddress"), MOD_WRITE | MOD_SAVE, &mySlaveAddress);
		FieldData::set(Name, F("SendInterval"), MOD_WRITE | MOD_SAVE, &mySendInterval);
		FieldData::set(Name, F("ResponseDelayTime"), MOD_WRITE | MOD_SAVE, &myResponseDelayTime);
		FieldData::set(Name, F("InverterState"), MOD_NONE, &myInverterState);
		FieldData::set(Name, F("InverterStateDescription"), MOD_OVERVIEW, myInverterStateDescription);
		FieldData::set(Name, F("IsWaitingForResponse"), MOD_OVERVIEW, &myIsWaitingForResponse);
		FieldData::set(Name, F("LastMessageSendTime"), MOD_NONE, &myLastMessageSendTime);
		FieldData::set(Name, F("LastMessageRecievedTime"), MOD_NONE, &myLastMessageRecievedTime);
		FieldData::set(Name, F("LastUsedAddress"), MOD_NONE, &myLastUsedAddress);
		FieldData::set(Name, F("SerialBuffer"), MOD_NONE, mySerialInBuffer);
		FieldData::set(Name, F("AlreadyRead"), MOD_NONE, &myAlreadyRead);

	}
#endif
