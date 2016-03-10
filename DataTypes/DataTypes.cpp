/*
 * DataTypes.cpp

 *
 *  Created on: Feb 5, 2014
 *      Author: jan
 */
#include "DataTypes.h"

DateTime::DateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec)
{
	if (year >= 2000)
		year -= 2000;
	yOff = year;
	m = month;
	d = day;
	hh = hour;
	mm = min;
	ss = sec;
}

char * DateTime::toString(char * buffer, int buffersize) const
		{
	//sprintf(DateString,"%u4-%h2-%h2 %h2:%h2:%h2",dt.year(),dt.month(),dt.day(),dt.hour(),dt.minute(),dt.second());
	snprintf_P(buffer, buffersize, (char *) F("%d-%d-%d %d:%d::%d"), (int) year(), (int) m, (int) d, (int) hh, (int) mm, (int) ss);
	return buffer;
}

/*
 * Divides a given PWM pin frequency by a divisor.
 *
 * The resulting frequency is equal to the base frequency divided by
 * the given divisor:
 *   - Base frequencies:
 *      o The base frequency for pins 3, 9, 10, and 11 is 31250 Hz.
 *      o The base frequency for pins 5 and 6 is 62500 Hz.
 *   - Divisors:
 *      o The divisors available on pins 5, 6, 9 and 10 are: 1, 8, 64,
 *        256, and 1024.
 *      o The divisors available on pins 3 and 11 are: 1, 8, 32, 64,
 *        128, 256, and 1024.
 *
 * PWM frequencies are tied together in pairs of pins. If one in a
 * pair is changed, the other is also changed to match:
 *   - Pins 5 and 6 are paired (Timer0)
 *   - Pins 9 and 10 are paired (Timer1)
 *   - Pins 3 and 11 are paired (Timer2)
 *
 * Note that this function will have side effects on anything else
 * that uses timers:
 *   - Changes on pins 5, 6 may cause the delay() and
 *     millis() functions to stop working. Other timing-related
 *     functions may also be affected.
 *   - Changes on pins 9 or 10 will cause the Servo library to function
 *     incorrectly.
 *
 * Thanks to macegr of the Arduino forums for his documentation of the
 * PWM frequency divisors. His post can be viewed at:
 *   http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1235060559/0#4
 */

//@uml.annotationsderived_abstraction=platform:/resource/UML%20model%20Project/Blank%20Package.emx#_Xu2WwdVKEeC1VtUBsYxodw
//@generated "UML to C++ (com.ibm.xtools.transform.uml2.cpp.CPPTransformation)"
#ifdef ARDUINO_AVR_MEGA2560 //mega
/**
 * 	Implementation for the mega
 * 	prescales for all timers except for 0 (pins 2, 3, 5, 6, 7, 8, 9, 10, 11, 12 );
 * 	prescaler = 1 ---> PWM frequency is 31000 Hz
 *  prescaler = 2 ---> PWM frequency is 4000 Hz
 *  prescaler = 3 ---> PWM frequency is 490 Hz (default value)
 * 	prescaler = 4 ---> PWM frequency is 120 Hz
 * 	prescaler = 5 ---> PWM frequency is 30 Hz
 * 	prescaler = 6 ---> PWM frequency is <20 Hz
 *
 * 	Prescalers for timer 0 (pin 13 and 4)
 * 	prescaler = 1 ---> PWM frequency is 62000 Hz
 * 	prescaler = 2 ---> PWM frequency is 7800 Hz
 * 	prescaler = 3 ---> PWM frequency is 980 Hz (default value)
 * 	prescaler = 4 ---> PWM frequency is 250 Hz
 * 	prescaler = 5 ---> PWM frequency is 60 Hz
 * 	prescaler = 6 ---> PWM frequency is <20 Hz
 *
 *
 * 	 */
bool setPwmFrequency(uint8_t pin, uint8_t prescaler)
{
	if (prescaler < 1 || prescaler > 6)
	{
		return false;
	}
	int myEraser = 7;             // this is 111 in binary and is used as an eraser
	switch (pin)
	{
		case 13:
			case 4:
			//timer 0 (controls pin 13, 4);
//			TCCR0B &= ~myEraser;   // this operation (AND plus NOT),  set the three bits in TCCR2B to 0
//			TCCR0B |= prescaler;  //this operation (OR), replaces the last three bits in TCCR2B with our new value 011
//			return true;
			return false; //don't mess with timer 0
		case 12:
			case 11:
			//timer 1 (controls pin 12, 11);
			TCCR1B &= ~myEraser;   // this operation (AND plus NOT),  set the three bits in TCCR2B to 0
			TCCR1B |= prescaler;  //this operation (OR), replaces the last three bits in TCCR2B with our new value 011
			return true;
		case 10:
			case 9:
			//timer 2 (controls pin 10, 9);
			TCCR2B &= ~myEraser;   // this operation (AND plus NOT),  set the three bits in TCCR2B to 0
			TCCR2B |= prescaler;  //this operation (OR), replaces the last three bits in TCCR2B with our new value 011
			return true;
		case 5:
			case 3:
			case 2:
			//timer 3 (controls pin 5, 3, 2);
			TCCR3B &= ~myEraser;   // this operation (AND plus NOT),  set the three bits in TCCR2B to 0
			TCCR3B |= prescaler;  //this operation (OR), replaces the last three bits in TCCR2B with our new value 011
			return true;
		case 8:
			case 7:
			case 6:
			//timer 4 (controls pin 8, 7, 6);
			TCCR4B &= ~myEraser;   // this operation (AND plus NOT),  set the three bits in TCCR2B to 0
			TCCR4B |= prescaler;  //this operation (OR), replaces the last three bits in TCCR2B with our new value 011
			return true;
		default:
			return false;
	}
	return true;
}

#elif defined( ARDUINO_AVR_YUN) //other
#warning setPwmFrequency not implemented for yun
#elif TEENSYDUINO
#warning setPwmFrequency not implemented for Teensy
#else
bool setPwmFrequency(uint8_t pin, uint8_t divisor)
{
	byte mode;
	if (pin == 3 || pin == 11)
	{
		switch (divisor)
		{
			case 1:
			mode = 0x01;
			break;
			case 8:
			mode = 0x02;
			break;
			case 32:
			mode = 0x03;
			break;
			case 64:
			mode = 0x04;
			break;
			case 128:
			mode = 0x05;
			break;
			case 256:
			mode = 0x06;
			break;
			case 1024:
			mode = 0x7;
			break;
			default:
			return false;
		}
	} else
	switch (divisor)
	{
		case 1:
		mode = 0x01;
		break;
		case 8:
		mode = 0x02;
		break;
		case 64:
		mode = 0x03;
		break;
		case 256:
		mode = 0x04;
		break;
		case 1024:
		mode = 0x05;
		break;
		default:
		return false;
	}
	switch (pin)
	{
		case 5:
		case 6:
		TCCR0B = (TCCR0B & 0b11111000) | mode; // Timer0
		break;
		case 9:
		case 10:
		TCCR1B = (TCCR1B & 0b11111000) | mode;// Timer1
		break;
	}
	TCCR2B = (TCCR2B & 0b11111000) | mode; // Timer2
	return true;
}
#endif

/*
 * Returns the distance between this and the provided point in cm
 */
uint32_t GPSLocation::distance(GPSLocation point)
{
//	        float distLat = abs(gpsLat0-gpsLat)*111194.9;
//	        float distLong = 111194.9*abs(gpsLong0-gpsLong)*cos(radians((gpsLat0+gpsLat)/2));
//	        float distance = sqrt(pow(distLat,2)+pow(distLong,2));
	//TODO my longitude and latitude is not in real numbers but in integer
	//so a recalculation is needed
//    float distLat = abs(point.getLatitude()-getLatitude())*111194.9;
//    float distLong = 111194.9*abs((point.getLongitude()-getLongitude())*cos(radians((point.getLatitude()+getLatitude())/2));
//    float distance = sqrt(pow(distLat,2)+pow(distLong,2));
//    return distance;
	//haversine
	// returns the great-circle distance between two points (radians) on a sphere
	float a = sq((sin((point.getLatitudeRad() - getLatitudeRad()) / 2.0))) + (cos(point.getLatitudeRad()) * cos(getLatitudeRad()) * sq((sin((point.getLongitudeRad() - getLongitudeRad()) / 2.0))));
	float c=2.0*atan2(sqrt(a),sqrt(1-a));
	float d = c * rEarth_cm ;
	//Serial.println(d);
	return d;
}

void GPSLocation::convertDegreesToDeci()
{
	long lat = myLatitude;
	long degree = lat / 1000000L;
	long deci = lat % 1000000L;
	lat = degree * 1000000L + (deci * 10L) / 6L;

	long longi = myLongitude;
	degree = longi / 1000000L;
	deci = longi % 1000000L;
	longi = degree * 1000000L + (deci * 10L) / 6L;

	myLatitude = lat;
	myLongitude = longi;
}

/*
 * Calculates the compass value you need to go from this to targetpoint
 * based on http://www.movable-type.co.uk/scripts/latlong.html section bearing
 * returns the compass value in centirad
 */
uint32_t GPSLocation::forwardAzimuth(GPSLocation targetPoint)
{
	float y = sin(targetPoint.getLongitudeRad() - getLongitudeRad()) * cos(targetPoint.getLatitudeRad());
	float x = cos(getLatitudeRad()) * sin(targetPoint.getLatitudeRad()) -
			sin(getLatitudeRad()) * cos(targetPoint.getLatitudeRad()) * cos(targetPoint.getLongitudeRad() - getLongitudeRad());
	return atan2(y, x) * 100;
}
