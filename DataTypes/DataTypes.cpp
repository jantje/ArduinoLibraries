/*
 * DataTypes.cpp

 *
 *  Created on: Feb 5, 2014
 *      Author: jan
 */
#include "DataTypes.h"
/**
 * this buffer can be used to not have to allocate buffers at multiple locations
 */
char commonlyUsedBuffer[commonlyUsedBuffersize];

DateTime::DateTime (uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec) {
    if (year >= 2000)
        year -= 2000;
    yOff = year;
    m = month;
    d = day;
    hh = hour;
    mm = min;
    ss = sec;
}

char * DateTime::ToString(char * buffer,int buffersize) const
{
	 //sprintf(DateString,"%u4-%h2-%h2 %h2:%h2:%h2",dt.year(),dt.month(),dt.day(),dt.hour(),dt.minute(),dt.second());
	 snprintf_P(buffer,buffersize,(char *)F("%d-%d-%d %d:%d::%d"),(int)year(),(int)m,(int)d,(int)hh,(int)mm,(int)ss);
	 return buffer;
}


