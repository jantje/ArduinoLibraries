/*
 * gps_library.h
 * Ladyada's logger modified by Bill Greiman to use the SdFat library
 * converted to a real library with only gps functionality by jan baeyens
 * this is a generic gps libary that does checksum testing so the data written should be always good
 * Assumes a sirf III chipset logger
 *
 *  Created on: 14-nov.-2011
 *      Author: Jan Baeyens
 */

#ifndef GPS_LIBRARY_H_
#define GPS_LIBRARY_H_

#include "Arduino.h"

#include "DataTypes.h"
#ifdef I_USE_SERIAL_REGISTER
#include "SerialDataInterface.h"
#endif

extern uint32_t loopMillis;

#define BUFFSIZE 85 // the maximum length of a gps message string
                    // According to NMEA spec no longer than 81 chars
class GPSModule
{

	protected:
		//protected so that the overloaded methods can access the data
		static Stream *m_gpsSerial;	// the serial connection to the gps
		static uint16_t m_gpsRate;	// the serial baud rate used by the gps module
		static DateTime m_LastMessageTimeStamp;
		static GPSLocation m_LastMessageLocation; // The last received fix message
		static GPSLocation m_LastButOneMessageLocation; // The last received fix message
		static GPSDIRECTION m_LastMessageDirection; // The  direction on the last received GPRMC message
		static GPSSPEED m_LastMessageSpeed;	// The  speed on the last received GPRMC message in Meters/hour
		static bool m_LastMessageIsfix;		// Whether the last gprc message was a fix message or not
		static uint8_t bufferidx;
		static char buffer[BUFFSIZE];
		static uint32_t LastFixMessage; //The millis of the last fix message
		static uint32_t myLoopduration;
		static uint32_t myDistanceBetweenLastFixes_cm;

		/**
		 * The method that parses a GPRMC Message
		 * This method assumes buffer contains the message and based on the content of this message
		 * fills in the m_LastMessageXX member values
		 */
		static void parseGPRMCMessage();
	public:

		/**
		 * Has reception tells whether there has been a fix in the time given
		 * default is 2000 milliseconds
		 */
		static boolean hasReception(uint32_t since=2000);
		//GPSLocation GetCurrentLocation();
		/**
		 * The only constructor of the GPSModule class.
		 * This method creates the serial communication but does not yet power on the gps module
		 *
		 * long gpsRate the baud rate to communicate to the gps module if not specified 4800
		 * byte powerPin the pin used to power on the gps module. If not specified 4
		 * byte txPin the TX pin used to communicate with the gps module. default :2
		 * byte rxPin the rx pin used to communicate with the gps module. default : 3
		 */
		GPSModule(uint16_t gpsRate, Stream *serial)
		{
			m_gpsSerial = serial;
			m_gpsRate = gpsRate;
		}
#ifdef I_USE_SERIAL_REGISTER
		/*
		 * This method registers the content of the class instance to the communicator.
		 * You need to add I_USE_SERIAL_REGISTER to your defines and add the communicator libraries
		 * before this will work
		 */
		static void serialRegister(const __FlashStringHelper* Name);
#endif
		/**
		 * powers on the gps module and starts the serial communication
		 * This method can only be called once
		 */
		static void setup();
		/**
		 * to be called in the loop of the sketch
		 * reads and processes the serial communication
		 */
		static void loop();

		static GPSDIRECTION getLastMessageDirection()
		{	return m_LastMessageDirection;};

		static GPSSPEED getLastMessageSpeed()
		{	return m_LastMessageSpeed;};

		/**
		 * returns the date and time stamp of the last received GPRMC message
		 */
		static DateTime getLastMessageTimeStamp()
		{	return m_LastMessageTimeStamp;}

		/**
		 * returns the GPS locations is the last received "fixed" GPRMC message
		 */
		static GPSLocation getLastMessageLocation()
		{	return m_LastMessageLocation;};
		/**
		 * Implement this method in your part of the code if you want to add personal processing
		 * when GPRMC Message arrives.
		 * this method is called after the message has been parsed.
		 * Therfore all m_LastMessageXX members contain values in the message
		 *
		 */
#ifdef I_USE_processGPRMCMessage
				static void processGPRMCMessage();
#endif
				static const char * getBuffer()
				{	return buffer;}


			};


#endif /* GPS_LIBRARY_H_ */
