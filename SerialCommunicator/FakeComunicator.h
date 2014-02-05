/*
 * FakeComunicator.h
 *
 *  Created on: Jan 26, 2014
 *      Author: jan
 */

#ifndef FAKECOMUNICATOR_H_
#define FAKECOMUNICATOR_H_

class FakeComunicator
{
	public:

		FakeComunicator(uint8_t resetPin){};
		virtual ~FakeComunicator()
		{
		}
		/**
		 * Initializes the class.
		 * Call this method in your setup()
		 */
		void setup(SerialDataInterface* TheData){};
		/**
		 * Add the Loop() in your loop();
		 */
		void loop(){};
};

#endif /* FAKECOMUNICATOR_H_ */

