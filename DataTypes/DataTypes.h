#ifndef DATATYPES_H_
#define DATATYPES_H_
#include "Arduino.h"


#define DIRECTION_TYPE int8_t  		//-90 to 90
#define SPEED_TYPE     int16_t     // -255 255

#define GPSDIRECTION 	int16_t  		// 0 to 360 0= North 180 = south
#define GPSSPEED     int16_t     // in meters per hour
#define MAXGPSDEVIATION 0 // The maximum deviation to call it a hit 10 is default but to keep the robot at the same time I put it to 0 for testing
//TOFIX SET this value to 10
#define commonlyUsedBuffersize 100
extern char commonlyUsedBuffer[commonlyUsedBuffersize]; //a buffer you use for temporary storage.
           //as this is a shared buffer don'texpect the data to be available after a while

class GPSLocation
{
public:
	long myLatitude; //Latitude Hours * 1.000.000
	long myLongitude; //Longitude Hours * 1.000.000
	//operator String() {return "Lat: " + String(myLatitude) + ", Long: " +String(myLongitude) ;};
	bool operator ==(GPSLocation otherLoc) { return ( sqrt(square(otherLoc.myLatitude - myLatitude) + square(otherLoc.myLongitude-myLongitude ) ) < MAXGPSDEVIATION);}
	//Serial.print (((String)otherLoc) + String("=") + this->operator String() + String(" is ") +ret);
	//return ret;}
	bool operator !=(GPSLocation otherLoc){return !(this->operator ==(otherLoc));}
	GPSLocation operator = (GPSLocation otherLoc) {myLatitude = otherLoc.myLatitude; myLongitude = otherLoc.myLongitude; return otherLoc;}
	char * getValue(char *buffer,byte buffersize)const{snprintf(buffer,buffersize,"Lat: %lu, long: %lu",myLatitude,myLongitude);return buffer;}
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
    char * ToString(char * buffer,int buffersize)const;


protected:
    uint8_t yOff, m, d, hh, mm, ss;
};



#endif //DATATYPES_H_
