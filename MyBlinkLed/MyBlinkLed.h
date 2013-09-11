/*
 * MyBlinkLed.h
 *
 *  Created on: Sep 11, 2013
 *      Author: jan
 */

#ifndef MYBLINKLED_H_
#define MYBLINKLED_H_

#include <BlinkLed.h>
#include <SerialDataInterface.h>

class MyBlinkLed: public BlinkLed, public SerialDataInterface
{
	private:
		//For SerialDataInterface
		static const uint8_t NUMFields = 5;
		static const uint8_t NUMChildren = 0;
		FieldData myDataFields[NUMFields];
		SerialDataInterface * myDataChildren[NUMChildren];
	public:
		MyBlinkLed(const char* name,uint8_t ledPin,uint32_t onInterval ,uint32_t offInterval);
};

#endif /* MYBLINKLED_H_ */
