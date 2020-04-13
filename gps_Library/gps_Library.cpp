/*
 * gps_library.cpp
 *
 *  Created on: 14-nov.-2011
 *      Author: Jan Baeyens
 *
 */

#include "gps_Library.h"

Stream *GPSModule::m_gpsSerial;	// the serial connection to the gps
uint16_t GPSModule::m_gpsRate;	// the serial baud rate used by the gps module
DateTime GPSModule::m_LastMessageTimeStamp;
GPSLocation GPSModule::m_LastMessageLocation; // The last received fix message
GPSLocation GPSModule::m_LastButOneMessageLocation;
GPSDIRECTION GPSModule::m_LastMessageDirection = 0; // The  direction on the last received GPRMC message
GPSSPEED GPSModule::m_LastMessageSpeed = 0;	// The  speed on the last received GPRMC message in Meters/hour
bool GPSModule::m_LastMessageIsfix = false;		// Whether the last gprc message was a fix message or not
uint8_t GPSModule::bufferidx = 0;
char GPSModule::buffer[BUFFSIZE];
uint32_t GPSModule::LastFixMessage = 0; //The millis of the last fix message
uint32_t GPSModule::myLoopduration = 0;
uint32_t GPSModule::myDistanceBetweenLastFixes_cm=0;

//#define DETAILED_LOGGING
#ifdef I_USE_SERIAL_REGISTER
void GPSModule::serialRegister(const __FlashStringHelper* Name)
{
#ifndef  I_USE_DATETIME
#error "you need to define I_USE_DATETIME to get the following code to compile"
#endif
	FieldData::set( Name,F("TimeStamp"), MOD_NONE,&m_LastMessageTimeStamp);
#ifndef  I_USE_GPS
#error "you need to define I_USE_GPS to get the following code to compile"
#endif
	FieldData::set( Name, F("Location"),MOD_NONE|MOD_OVERVIEW, &m_LastMessageLocation);
	FieldData::set( Name, F("Isfix"),MOD_NONE, &m_LastMessageIsfix);
	FieldData::setNext( F("LastFixMessage"),MOD_NONE, &LastFixMessage);
	FieldData::set( Name, F("distance between last 2 fixes cm"),MOD_NONE, &myDistanceBetweenLastFixes_cm);

#ifdef DETAILED_LOGGING
	FieldData::set( Name,F("Direction"),MOD_NONE, &m_LastMessageDirection);
	FieldData::set( Name,F("Speed"), MOD_NONE, &m_LastMessageSpeed);
	FieldData::setNext( F("bufferidx"),MOD_NONE, &bufferidx);
	FieldData::set( Name, F("buffer"),MOD_NONE,buffer);
	FieldData::setNext( (__FlashStringHelper *)LOOPDURATION,MOD_NONE,&myLoopduration);
	FieldData::setNext( F("gpsRate"),MOD_NONE,&m_gpsRate);
#endif
}
#endif

void GPSModule::setup()
{
	bufferidx = 0;
	// connect to the GPS at the desired rate
//	(*m_gpsSerial).begin(m_gpsRate);
	//tell the gps module we are only interested in GPRC messages
	//(*m_gpsSerial).println(F("$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"));
	if (m_gpsRate == 9600) (*m_gpsSerial).print("$PSRF100,1,9600,8,1,0*0D\r\n");
	if (m_gpsRate == 4800) (*m_gpsSerial).print("$PSRF100,1,4800,8,1,0*0E\r\n");
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
		//Serial.print(ReceivedChar);
		if (ReceivedChar == '\r') ReceivedChar = ' ';
		buffer[bufferidx++] = ReceivedChar;
		buffer[bufferidx] = 0; //terminate with 0
		if ((ReceivedChar == '$') & (bufferidx != 1))
		{
			//Serial.println();
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
				bufferidx = 0;
				return;
			}
			// got good data!

			if (strstr(buffer, "GPRMC"))
			{
				parseGPRMCMessage();
				if (m_LastMessageIsfix)
				{
					LastFixMessage =  loopMillis;
					myDistanceBetweenLastFixes_cm = m_LastMessageLocation.distance(m_LastButOneMessageLocation);
					m_LastButOneMessageLocation = m_LastMessageLocation;
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

boolean GPSModule::hasReception(uint32_t since)
{
	return (((loopMillis - LastFixMessage) < since) && ((m_LastMessageLocation.myLatitude + m_LastMessageLocation.myLongitude) != 0));
}

void GPSModule::parseGPRMCMessage()
{
//	Serial.println("Parse Message");
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
		//Serial.println(p);
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
	}

//	Serial.println("done");
//	Serial.print("Latitude :");
//	Serial.print(m_LastMessageLocation.myLatitude);
//	Serial.print(" Longitude :");
//	Serial.println(m_LastMessageLocation.myLongitude);
//	Serial.print("isFix :");
//	Serial.println(m_LastMessageIsfix);
//	Serial.print("LastFixMessage :");
//	Serial.println(LastFixMessage);
	m_LastMessageLocation.convertDegreesToDeci();
//	Serial.print("Latitude :");
//	Serial.print(m_LastMessageLocation.myLatitude);
//	Serial.print(" Longitude :");
//	Serial.println(m_LastMessageLocation.myLongitude);

}

