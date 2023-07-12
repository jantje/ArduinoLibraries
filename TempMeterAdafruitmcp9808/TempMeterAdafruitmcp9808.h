/*
 * adafruitmcp9808.h
 *
 *  Created on: May 12, 2015
 *      Author: jan
 */

#ifndef LIBRARIES_TEMPMETER_ADAFRUITMCP9808_H_
#define LIBRARIES_TEMPMETER_ADAFRUITMCP9808_H_

#include "TempMeterInterface.h"

class TempMeterAdafruitmcp9808: public TempMeterInterface
{
	private:
		uint8_t myAddress;
		uint16_t read16(uint8_t reg);

	public:
		TempMeterAdafruitmcp9808(uint8_t address)
		{
			myAddress = address;
		}
		;
		void setup();
		void loop();
};

#endif /* LIBRARIES_TEMPMETER_ADAFRUITMCP9808_H_ */
