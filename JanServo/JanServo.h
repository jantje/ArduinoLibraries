/* A library to handle 
 non aligned servo's
 */
#ifndef JANSERVO_H_
#define JANSERVO_H_
#include "Arduino.h"
#include <Servo.h>

class JanServo: private Servo
{
	public:
		JanServo( uint8_t Pin){	offset = 0;
		ServoControlPin = Pin;
		ServoPosition = 0;};

		void serialRegister(const __FlashStringHelper* Name);

		void setup()
		{
			attach(ServoControlPin); //this can not be done in the constructor for one reason or another
		}
		void loop()
		{
		}

		void SetPosition(uint8_t Position);


		void Move(uint8_t Movement)
		{
			SetPosition(ServoPosition + Movement);
		}
		;
		byte GetPosition()
		{
			return ServoPosition;
		}
		;
		byte GetCorrectedPosition()
		{
			return ServoPosition + offset;
		}
		;

	private:
		uint8_t offset; //the error on the servo
		uint8_t ServoControlPin; //the pin used to control this servo
		uint8_t ServoPosition; //The current Position of the servo
};



#endif //JANSERVO_H_
