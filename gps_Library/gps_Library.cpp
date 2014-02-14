/*
 * gps_library.cpp
 *
 *  Created on: 14-nov.-2011
 *      Author: Jan Baeyens
 *
 */

#include "gps_Library.h"

/* EXAMPLE

 $PSRF103,<msg>,<mode>,<rate>,<cksumEnable>*CKSUM<CR><LF>

 <msg> 00=GGA,01=GLL,02=GSA,03=GSV,04=RMC,05=VTG
 <mode> 00=SetRate,01=Query
 <rate> Output every <rate>seconds, off=00,max=255
 <cksumEnable> 00=disable Checksum,01=Enable checksum for specified message
 Note: checksum is required

 Example 1: Query the GGA message with checksum enabled
 $PSRF103,00,01,00,01*25

 Example 2: Enable VTG message for a 1Hz constant output with checksum enabled
 $PSRF103,05,00,01,01*20

 Example 3: Disable VTG message
 $PSRF103,05,00,00,01*21

 */

#define SERIAL_SET F("$PSRF100,01,4800,08,01,00*0E\r\n")

// GGA-Global Positioning System Fixed Data, message 103,00

#define GGA_ON F("$PSRF103,00,00,01,01*25\r\n")
#define GGA_OFF F("$PSRF103,00,00,00,01*24\r\n")

// GLL-Geographic Position-Latitude/Longitude, message 103,01

#define GLL_ON F("$PSRF103,01,00,01,01*26\r\n")
#define GLL_OFF F("$PSRF103,01,00,00,01*27\r\n")

// GSA-GNSS DOP and Active Satellites, message 103,02

#define GSA_ON F("$PSRF103,02,00,01,01*27\r\n")
#define GSA_OFF F("$PSRF103,02,00,00,01*26\r\n")

// GSV-GNSS Satellites in View, message 103,03

#define GSV_ON F("$PSRF103,03,00,01,01*26\r\n")
#define GSV_OFF F("$PSRF103,03,00,00,01*27\r\n")

// RMC-Recommended Minimum Specific GNSS Data, message 103,04

#define RMC_ON F("$PSRF103,04,00,01,01*21\r\n")
#define RMC_OFF F("$PSRF103,04,00,00,01*20\r\n")

// VTG-Course Over Ground and Ground Speed, message 103,05

#define VTG_ON F("$PSRF103,05,00,01,01*20\r\n")
#define VTG_OFF F("$PSRF103,05,00,00,01*21\r\n")

// Switch Development Data Messages On/Off, message 105
#define LOG_DDM 1
#define DDM_ON F("$PSRF105,01*3E\r\n")
#define DDM_OFF F("$PSRF105,00*3F\r\n")

#define USE_WAAS 0 // useful in US, but slower fix
#define WAAS_ON F("$PSRF151,01*3F\r\n") // the command for turning on WAAS
#define WAAS_OFF F("$PSRF151,00*3E\r\n") // the command for turning off WAAS


	SERIALTYPE *GPSModule::m_gpsSerial;	// the serial connection to the gps
	uint16_t GPSModule::m_gpsRate;	// the serial baud rate used by the gps module
	DateTime GPSModule::m_LastMessageTimeStamp;
	GPSLocation GPSModule::m_LastMessageLocation; // The last received fix message
	GPSDIRECTION GPSModule::m_LastMessageDirection=0; // The  direction on the last received GPRMC message
	GPSSPEED GPSModule::m_LastMessageSpeed=0;	// The  speed on the last received GPRMC message in Meters/hour
	bool GPSModule::m_LastMessageIsfix=false;		// Whether the last gprc message was a fix message or not
	uint8_t GPSModule::bufferidx=0;
	char GPSModule::buffer[BUFFSIZE];
	uint32_t GPSModule::LastFixMessage=0; //The millis of the last fix message

#ifdef I_USE_SERIAL_COMMUNICATOR
	uint32_t GPSModule::myLoopduration=0;
#endif



void GPSModule::serialRegister(const __FlashStringHelper* Name)
{
#ifdef I_USE_SERIAL_COMMUNICATOR

	FieldData::set( Name,F("TimeStamp"), 0,&m_LastMessageTimeStamp);
	FieldData::set( Name, F("Location"),0, &m_LastMessageLocation);
	FieldData::set( Name,F("Direction"),0, &m_LastMessageDirection);
	FieldData::set( Name,F("Speed"), 0, &m_LastMessageSpeed);
	FieldData::set( Name, F("Isfix"),0, &m_LastMessageIsfix);
	FieldData::setNext(  F("bufferidx"),0, &bufferidx);
	FieldData::set( Name, F("buffer"),0,buffer);
	FieldData::setNext(  F("LastFixMessage"),0, &LastFixMessage);
	FieldData::setNext(  (__FlashStringHelper *)LOOPDURATION,0,&myLoopduration);
	FieldData::setNext(  F("gpsRate"),0,&m_gpsRate);

#endif
}

// what to log
#define LOG_RMC 1 // RMC-Recommended Minimum Specific GNSS Data, message 103,04
#define LOG_GGA 0 // GGA-Global Positioning System Fixed Data, message 103,00
#define LOG_GLL 0 // GLL-Geographic Position-Latitude/Longitude, message 103,01
#define LOG_GSA 0 // GSA-GNSS DOP and Active Satellites, message 103,02
#define LOG_GSV 0 // GSV-GNSS Satellites in View, message 103,03
#define LOG_VTG 0 // VTG-Course Over Ground and Ground Speed, message 103,05
void GPSModule::setup()
{
	bufferidx = 0;
	WDTCSR |= (1 << WDCE) | (1 << WDE);
	WDTCSR = 0;

	// connect to the GPS at the desired rate
	(*m_gpsSerial).begin(m_gpsRate);
//	pinMode(m_powerPin, OUTPUT);
//	digitalWrite(m_powerPin, LOW);
//	//delay(250);
	(*m_gpsSerial).print(SERIAL_SET);
	delay(250);

#if (LOG_DDM == 1)
	(*m_gpsSerial).print(DDM_ON);
#else
	(*m_gpsSerial).print(DDM_OFF);
#endif
	delay(250);
#if (LOG_GGA == 1)
	(*m_gpsSerial).print(GGA_ON);
#else
	(*m_gpsSerial).print(GGA_OFF);
#endif
	delay(250);
#if (LOG_GLL == 1)
	(*m_gpsSerial).print(GLL_ON);
#else
	(*m_gpsSerial).print(GLL_OFF);
#endif
	delay(250);
#if (LOG_GSA == 1)
	(*m_gpsSerial).print(GSA_ON);
#else
	(*m_gpsSerial).print(GSA_OFF);
#endif
	delay(250);
#if (LOG_GSV == 1)
	(*m_gpsSerial).print(GSV_ON);
#else
	(*m_gpsSerial).print(GSV_OFF);
#endif
	delay(250);
#if (LOG_RMC == 1)
	(*m_gpsSerial).print(RMC_ON);
#else
	(*m_gpsSerial).print(RMC_OFF);
#endif
	delay(250);

#if (LOG_VTG == 1)
	(*m_gpsSerial).print(VTG_ON);
#else
	(*m_gpsSerial).print(VTG_OFF);
#endif
	delay(250);

#if (USE_WAAS == 1)
	(*m_gpsSerial).print(WAAS_ON);
#else
	(*m_gpsSerial).print(WAAS_OFF);
#endif
	//SHOW_DEBUG_INFO_LN(DEBUGLEVEL_GPS_BUSINESS, "GPS initialized");
}

// read a Hex value and return the decimal equivalent
uint8_t parseHex(char c)
{
	if (c < '0') return 0;
	if (c <= '9') return c - '0';
	if (c < 'A') return 0;
	if (c <= 'F') return (c - 'A') + 10;
	return 0;
}

void GPSModule::loop()
{
	char ReceivedChar;
	uint8_t sum;
	int InBuffer;

	while ((InBuffer = (*m_gpsSerial).available()) > 0)
	{
		ReceivedChar = (*m_gpsSerial).read();
		if (ReceivedChar=='\r') ReceivedChar=' ';
		buffer[bufferidx++] = ReceivedChar;
		buffer[bufferidx] = 0; //terminate with 0
		if ((ReceivedChar == '$') & (bufferidx != 1))
		{
			//SHOW_DEBUG_INFO_VAR_LN(DEBUGLEVEL_GPS_TRACE, buffer);
			//SHOW_DEBUG_INFO_STRING_LN(DEBUGLEVEL_GPS_TRACE, "ERROR: start of new line received in message");
			bufferidx = 0;
			buffer[bufferidx++] = ReceivedChar;
			buffer[bufferidx] = 0; //terminate with 0
		}

		if (ReceivedChar == '\n') // the message is complete
		{
			buffer[bufferidx - 1] = 0;
			//SHOW_DEBUG_INFO_VAR_LN(DEBUGLEVEL_GPS_TRACE, buffer);
			if (buffer[bufferidx - 5] != '*')
			{
				// no checksum?
				//SHOW_DEBUG_INFO_STRING_LN(DEBUGLEVEL_GPS_TRACE, "ERROR: there is no checksum in the message");
				bufferidx = 0;
				return;
			}
			// get received checksum
			sum = parseHex(buffer[bufferidx - 4]) * 16;
			sum += parseHex(buffer[bufferidx - 3]);

			// check checksum
			for (int i = 1; i < (bufferidx - 5); i++)
			{
				sum ^= buffer[i];
			}
			if (sum != 0)
			{
				//SHOW_DEBUG_INFO_STRING_LN(DEBUGLEVEL_GPS_TRACE, "ERROR: received checksum is wrong");
				bufferidx = 0;
				return;
			}
			// got good data!

			if (strstr(buffer, "GPRMC"))
			{
				parseGPRCMessage();
				if (m_LastMessageIsfix)
				{
					LastFixMessage = millis();
				}
#ifdef I_USE_processGPRMCMessage
				processGPRMCMessage();
#endif
			}
			bufferidx = 0;
		}

		if (bufferidx == BUFFSIZE - 1)
		{
			//SHOW_DEBUG_INFO_STRING_LN(DEBUGLEVEL_GPS_TRACE, "ERROR: received message that is longer than the buffer");
			bufferidx = 0;
		}

	}

}

//GPSDIRECTION GPSModule::getLastMessageDirection() const
//{
//	return m_LastMessageDirection;
//}

//GPSLocation GPSModule::getLastMessageLocation() const
//{
//	return m_LastMessageLocation;
//}

//GPSSPEED GPSModule::getLastMessageSpeed() const
//{
//	return m_LastMessageSpeed;
//}

//DateTime GPSModule::getLastMessageTimeStamp() const
//{
//
//	return m_LastMessageTimeStamp;
//}

boolean GPSModule::hasReception()
{
	unsigned long Curtime = millis();
	return (((LastFixMessage + 5000) > Curtime) && ((m_LastMessageLocation.myLatitude + m_LastMessageLocation.myLongitude) != 0));
}

void GPSModule::parseGPRCMessage()
{
	m_LastMessageLocation.myLatitude = 0;
	m_LastMessageLocation.myLongitude = 0;
	m_LastMessageSpeed = 0;
	m_LastMessageDirection = 0;

	//extract the time
	const char *p = buffer;
	p = strchr(p, ',') + 1;
	uint8_t hour = (p[0] - '0') * 10 + (p[1] - '0');
	p += 2;
	uint8_t minutes = (p[0] - '0') * 10 + (p[1] - '0');
	p += 2;
	uint8_t seconds = (p[0] - '0') * 10 + (p[1] - '0');
	// find out if we got a fix
	p = strchr(p, ',') + 1;
	if (p[0] != 'A')
	{
		m_LastMessageIsfix = false;
		//DateTime dt(0, 0, 0, hour, minutes, seconds);
	}
	else
	{
		m_LastMessageIsfix = true;
		//extract the latitude
		p = strchr(p, ',') + 1;
		m_LastMessageLocation.myLatitude = (p[0] - '0') * 10 + (p[1] - '0');
		p += 2;

		while (p[0] != ',')
		{
			if (p[0] != '.') m_LastMessageLocation.myLatitude = m_LastMessageLocation.myLatitude * 10 + (p[0] - '0');
			p++;
		}
		p++;
		if (p[0] == 'S') m_LastMessageLocation.myLatitude *= -1;
		//extract the longitude
		p += 2;
		m_LastMessageLocation.myLongitude = (p[0] - '0') * 100 + (p[1] - '0') * 10 + (p[2] - '0');
		p += 3;
		while (p[0] != ',')
		{
			if (p[0] != '.') m_LastMessageLocation.myLongitude = m_LastMessageLocation.myLongitude * 10 + (p[0] - '0');
			p++;
		}
		p += 2;
		if (p[0] == 'W') m_LastMessageLocation.myLongitude *= -1;
		p += 1;
		//extract speed
		int Power = 1000;
		int Count = 1;
		while (p[0] != ',')
		{
			if (p[0] != '.')
			{
				m_LastMessageSpeed = m_LastMessageSpeed * 10 + (p[0] - '0');
				Power /= (Count);
			} else
			{
				Count = 10;
			}
			p++;
		}
		//The speed is in knots (1.852 km per hour = 1852 m per hour) so calculate to m/hour
		m_LastMessageSpeed = m_LastMessageSpeed * 1852 * Power;
		p += 1;
		Power = 1000;
		Count = 1;
		while (p[0] != ',')
		{
			if (p[0] != '.')
			{
				m_LastMessageDirection = m_LastMessageDirection * 10 + (p[0] - '0');
				Power /= Count;
			} else
			{
				Count = 10;
			}
			p++;
		}
		m_LastMessageDirection = m_LastMessageDirection * Power;
		p += 1;
		uint8_t m_day = (p[0] - '0') * 10 + (p[1] - '0');
		uint8_t m_month = (p[2] - '0') * 10 + (p[3] - '0');
		uint8_t m_year = (p[4] - '0') * 10 + (p[5] - '0');
		DateTime dt(m_year, m_month, m_day, hour, minutes, seconds);
		m_LastMessageTimeStamp = dt;
//	Serial.println(buffer);
//	Serial.println(m_year);
//	Serial.println(m_month);
//	Serial.println(m_day);
//	Serial.println(hour);
//	Serial.println(minutes);
//	Serial.println(seconds);
	}




}

/* End code */

