#ifndef DATATYPES_H_
#define DATATYPES_H_
#include "Arduino.h"

extern int SerialQueueSize; // The size of the serial SendQueue initialize it with SerialQueueSize=ser.availableInSendQueue() after you Serial.begin()
//#define DUMP_AND_WAIT(ser,msg,action) 		ser.println(msg);	while(ser.availableForWrite()<SerialQueueSize){action;};
//#define DUMP_AND_WAIT(ser,msg)

bool setPwmFrequency(uint8_t pin, uint8_t divisor);

#define SIGN(v)  (( (v) > 0) - ((v) < 0))
#define BIGGER(v,w) ( ( (v) > (w)) - ((v) < (w)))

#define DIRECTION_TYPE int8_t  		//-90 to 90
#define SPEED_TYPE     int16_t     // -255 255

#define GPSDIRECTION 	int16_t  		// 0 to 360 0= North 180 = south
#define GPSSPEED     int16_t     // in meters per hour
#define MAXGPSDEVIATION 0 // The maximum deviation to call it a hit 10 is default but to keep the robot at the same time I put it to 0 for testing
//TOFIX SET this value to 10
const float rEarth_cm = 637100000.0; //this number is in cm


class GPSLocation
{
public:
	long myLatitude; //Latitude Hours * 1.000.000
	long myLongitude; //Longitude Hours * 1.000.000
	//operator String() {return "Lat: " + String(myLatitude) + ", Long: " +String(myLongitude) ;};
	bool operator ==(const GPSLocation otherLoc) const { return ( sqrt(pow(otherLoc.myLatitude - myLatitude,2) + pow(otherLoc.myLongitude-myLongitude,2 ) ) <= MAXGPSDEVIATION);}
	//Serial.print (((String)otherLoc) + String("=") + this->operator String() + String(" is ") +ret);
	//return ret;}
	bool operator !=(GPSLocation otherLoc){return !(this->operator ==(otherLoc));}
	GPSLocation operator = (GPSLocation otherLoc) {myLatitude = otherLoc.myLatitude; myLongitude = otherLoc.myLongitude; return otherLoc;}
	char * getValue(char *buffer,size_t buffersize)const{snprintf(buffer,buffersize,"Lat: %li, Long: %li",myLatitude,myLongitude);return buffer;}
	float getLatitude(){return ((float)myLatitude)/1000000.0;}
	float getLongitude(){return ((float)myLongitude)/1000000.0;}
	float getLatitudeRad(){return radians(((float)myLatitude)/1000000.0);} //the latitude in radians
	float getLongitudeRad(){return radians(((float)myLongitude)/1000000.0);} //the longitude in radians

	/*Calculates the distance of 2 locations in cm.
	 *
	 */
	uint32_t distance(GPSLocation point);

	/* to convert gps coordiates to standard degrees to be able to calculate.
	 *
	 */
	void convertDegreesToDeci();

	/*
	 * Calculates the compass value you need to go from this to targetpoint
	 * returns the compass value in centirad
	 */
	uint32_t forwardAzimuth(GPSLocation targetPoint);
};



class DateTime {
public:
    //DateTime (uint32_t t =0);
		DateTime(){yOff=0; m=0; d=0; hh=0; mm=0; ss=0;};
    DateTime (uint16_t year, uint8_t month, uint8_t day,
                uint8_t hour =0, uint8_t min =0, uint8_t sec =0);

    DateTime (const char* date, const char* time);
    uint16_t year() const { return 2000 + yOff; }
    uint8_t month() const { return m; }
    uint8_t day() const { return d; }
    uint8_t hour() const { return hh; }
    uint8_t minute() const { return mm; }
    uint8_t second() const { return ss; }
    uint8_t dayOfWeek() const;

    // 32-bit times as seconds since 1/1/2000
    long secondstime() const;
    // 32-bit times as seconds since 1/1/1970
    uint32_t unixtime(void) const;
    char * toString(char * buffer,int buffersize)const;


protected:
    uint8_t yOff, m, d, hh, mm, ss;
};



#endif //DATATYPES_H_
