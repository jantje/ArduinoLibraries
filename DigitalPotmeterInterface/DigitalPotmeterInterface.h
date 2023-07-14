/*
 * IDigitalPotmeter.h
 *
 *  Created on: Jul 11, 2014
 *      Author: jan
 */

#ifndef IDIGITALPOTMETER_H_
#define IDIGITALPOTMETER_H_

class DigitalPotmeterInterface
{
	protected :
		uint8_t myPotValue; //the value the potmeter is currently set to range 0 to 255
		uint8_t myNewPotValue; // the value the potmeter will be set to at the next loop range 0 to 255
		uint8_t myMaxPotValue; //the maximum value the potmeter can be set to

	public:
		/**
		 * Set the value of the pot position with a range of 0 to 255
		 */
		virtual void setPotValue(uint8_t newPotValue){myNewPotValue=newPotValue;};
		virtual void setup()=0;
		virtual void loop()=0;
};

#endif /* IDIGITALPOTMETER_H_ */
