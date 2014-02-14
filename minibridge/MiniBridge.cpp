/*
 * MiniBridge.cpp
 *
 *  Created on: Jan 27, 2014
 *      Author: jan
 */
#ifdef I_USE_MINI_BRIDGE
#include "MiniBridge.h"

#define stream BridgeSerial
#define BridgeClass MiniBridge
MiniBridge Bridge;

//BridgeClass::MiniBridge(){max_retries=0;index=0; started=false;CRC=0;bridgeVersion=0;};
uint8_t BridgeClass::index;
uint16_t BridgeClass::CRC;
const char BridgeClass::CTRL_C = 3;
//#define CTRL_C  3
bool BridgeClass::started;
uint8_t BridgeClass::max_retries;
void BridgeClass::begin()
{
	if (started) return;
	started = true;
	int tt=0;
	Serial.print(tt++);
	stream.begin(250000);
	Serial.print(tt++);
  // Wait for U-boot to finish startup
  do {
  	Serial.println();
  	Serial.print("stream.available()=");
  	Serial.println(stream.available());
    dropAll();
    delay(1000);
  } while (stream.available() > 0);
  Serial.print(tt++);

  while (true) {
    // Bridge interrupt:
    // - Ask the bridge to close itself
    uint8_t quit_cmd[] = {'X', 'X', 'X', 'X', 'X'};
    Serial.print(tt++);
    max_retries = 1;
    transfer(quit_cmd, 5,0,0,0,0,0,0);
    Serial.print(tt++);

    // Bridge startup:
    // - If the bridge is not running starts it safely
    stream.print(CTRL_C);
    Serial.print(tt++);
    delay(250);
    Serial.print(tt++);
    stream.print(F("\n"));
    Serial.print(tt++);
    delay(250);
    Serial.print(tt++);
    stream.print(F("\n"));
    Serial.print(tt++);
    delay(500);
    Serial.print(tt++);
    // Wait for OpenWRT message
    // "Press enter to activate console"
    stream.print(F("run-bridge\n"));
    Serial.print(tt++);
    delay(500);
    Serial.print(tt++);
    dropAll();
    Serial.print(tt++);

    // Reset the brigde to check if it is running
    uint8_t cmd[] = {'X', 'X', '1', '0', '0'};
    Serial.print(tt++);
    uint8_t res[4];
    Serial.print(tt++);
    max_retries = 50;
    Serial.print(tt++);
    uint16_t l = transfer(cmd, 5,0,0,0,0, res, 4);
    Serial.print(tt++);
    if (l == TRANSFER_TIMEOUT) {
      // Bridge didn't start...
      // Maybe the board is starting-up?

      // Wait and retry
    	Serial.print(F("TRANSFER_TIMEOUT"));
      delay(1000);
      continue;
    }
    if (res[0] != 0)
    {
    	Serial.print(F("while (true)"));
      while (true);
    }

//    // Detect bridge version
//    if (l == 4) {
//      bridgeVersion = (res[1]-'0')*100 + (res[2]-'0')*10 + (res[3]-'0');
//    } else {
//      // Bridge v1.0.0 didn't send any version info
//      bridgeVersion = 100;
//    }

    max_retries = 50;
    return;
  }
}


#if defined(ARDUINO_ARCH_AVR)
// AVR use an optimized implementation of CRC
#include <util/crc16.h>
#else
// Generic implementation for non-AVR architectures
uint16_t _crc_ccitt_update(uint16_t crc, uint8_t data)
{
	data ^= crc & 0xff;
	data ^= data << 4;
	return ((((uint16_t)data << 8) | ((crc >> 8) & 0xff)) ^
			(uint8_t)(data >> 4) ^
			((uint16_t)data << 3));
}
#endif

void BridgeClass::crcUpdate(uint8_t c)
{
	CRC = _crc_ccitt_update(CRC, c);
}

//void BridgeClass::crcReset()
//{
//	CRC = 0xFFFF;
//}

void BridgeClass::crcWrite()
{
	stream.write((char) (CRC >> 8));
	stream.write((char) (CRC & 0xFF));
}

//bool BridgeClass::crcCheck(uint16_t _CRC)
//{
//	return CRC == _CRC;
//}

uint16_t BridgeClass::transfer(const uint8_t *buff1, uint16_t len1, const uint8_t *buff2, uint16_t len2, const uint8_t *buff3, uint16_t len3, uint8_t *rxbuff, uint16_t rxlen)
{
	uint16_t len = len1 + len2 + len3;
	uint8_t retries = 0;
	for (; retries < max_retries; retries++, delay(100), dropAll() /* Delay for retransmission */)
	{
		while(stream.read()!=-1); //clear the read buffer
		// Send packet
		crcReset();
		stream.write((char) 0xFF);                // Start of packet (0xFF)
		crcUpdate(0xFF);
		stream.write((char) index);               // Message index
		crcUpdate(index);
		stream.write((char) ((len >> 8) & 0xFF)); // Message length (hi)
		crcUpdate((len >> 8) & 0xFF);
		stream.write((char) (len & 0xFF));        // Message length (lo)
		crcUpdate(len & 0xFF);
		for (uint16_t i = 0; i < len1; i++)
		{ // Payload
			stream.write((char) buff1[i]);
			crcUpdate(buff1[i]);
		}
		for (uint16_t i = 0; i < len2; i++)
		{ // Payload
			stream.write((char) buff2[i]);
			crcUpdate(buff2[i]);
		}
		for (uint16_t i = 0; i < len3; i++)
		{ // Payload
			stream.write((char) buff3[i]);
			crcUpdate(buff3[i]);
		}
		crcWrite();                     // CRC

		// Wait for ACK in 100ms
		if (timedRead(100) != 0xFF) continue;
		crcReset();
		crcUpdate(0xFF);

		// Check packet index
		if (timedRead(5) != index) continue;
		crcUpdate(index);

		// Recv len
		int lh = timedRead(10);
		if (lh < 0) continue;
		crcUpdate(lh);
		int ll = timedRead(10);
		if (ll < 0) continue;
		crcUpdate(ll);
		uint16_t l = lh;
		l <<= 8;
		l += ll;

		// Recv data
		for (uint16_t i = 0; i < l; i++)
		{
			int c = timedRead(5);
			if (c < 0) continue;
			// Cut received data if rxbuffer is too small
			if (i < rxlen) rxbuff[i] = c;
			crcUpdate(c);
		}

		// Check CRC
		int crc_hi = timedRead(5);
		if (crc_hi < 0) continue;
		int crc_lo = timedRead(5);
		if (crc_lo < 0) continue;
		if (!crcCheck((crc_hi << 8) + crc_lo)) continue;

		// Increase index
		index++;

		// Return bytes received
		if (l > rxlen) return rxlen;
		return l;
	}

	// Max retries exceeded
	return TRANSFER_TIMEOUT;
}

int BridgeClass::timedRead(unsigned int timeout)
{
	int c;
	unsigned long _startMillis = millis();
	do
	{
		c = stream.read();
		if (c >= 0) return c;
	} while (millis() - _startMillis < timeout);
	return -1;     // -1 indicates timeout
}

void BridgeClass::dropAll()
{
	while (stream.available() > 0)
	{
		stream.read();
	}
}

#endif
