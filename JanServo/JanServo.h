/* A library to handle 
 non aligned servo's
 */
#ifndef JANSERVO_H_
#define JANSERVO_H_
#include "Arduino.h"
#include <Servo.h>
#ifdef I_USE_SERIAL_REGISTER
#include "SerialDataInterface.h"
#endif

class iServo:public Servo
{
	public:
		virtual void setup()=0;
		virtual void setPosition(uint8_t Position)=0;
		virtual void loop()=0;
		virtual void move(int8_t Movement)=0;
		virtual uint8_t getPosition()=0;
		virtual void stop()=0;
};
/**
 * This is a servo class that can be typically used for rotational servo's
 * Set the offset to the value where the rotational servo does not rotate.
 * Setting the speed to 0 will make the rotational servo stop.
 */
class JanContiniousServo: public iServo
{
	public:
		JanContiniousServo(uint8_t Pin, uint8_t position = 0, uint8_t offset = 0)
		{
			myOffset = offset;
			myServoControlPin = Pin;
			myServoPosition = position;
		}
#ifdef I_USE_SERIAL_REGISTER
		void serialRegister(const __FlashStringHelper* Name);
#endif

		void setup()
		{
			attach(myServoControlPin); //this can not be done in the constructor for one reason or another
			setPosition(myOffset);
		}
		void loop()
		{
		}

		void setPosition(uint8_t Position);

		void move(int8_t Movement)
		{
			setPosition(myServoPosition + Movement);
		}

		byte getPosition()
		{
			return myServoPosition;
		}

		byte getCorrectedPosition()
		{
			return myServoPosition + myOffset;
		}

		void stop()
		{
			setPosition(myOffset);
		}

		private:
		int8_t myOffset; //the error on the servo
		uint8_t myServoControlPin;//the pin used to control this servo
		uint8_t myServoPosition;//The current Position of the servo
	};

	/**
	 * JanConstraintServo is a servo class that you can use if you do not want to use the full range (0 to 180) of the servo.
	 * In typical situations the extreme positions of the servo can be dangerous because it touches some other object applying force and using unneeded energy.
	 * To avoid this without having to care about this in the code you can use this class. In the contructor the 2 maximum values are given.
	 * This way the class can make sure the servo never mover over this position
	 */
class JanConstraintServo: public iServo
{
	public:
		/**
		 * pin the pin the servo is attached to
		 * minimumPosition the smallest value that can be send safely to the servo. Must be bigger or equal to 0
		 * MaximumPosition the biggest value that can be send safely to the servo.
		 * defaultPosition the position the servo will go to when stop is called.
		 */
		JanConstraintServo(uint8_t Pin, uint8_t minimumPosition, uint8_t MaximumPosition, uint8_t defaultPosition)
		{
			myServoControlPin = Pin;
			myMinServoPosition = minimumPosition;
			myMaxServoPosition = MaximumPosition;
			myDefaultServoPosition = defaultPosition;
			myCurrentPosition= defaultPosition;
			myNewServoPosition = defaultPosition;
			myServoPosition = defaultPosition;
		}

#ifdef I_USE_SERIAL_REGISTER
		void serialRegister(const __FlashStringHelper* Name);
#endif

		void setup()
		{
			attach(myServoControlPin); //this can not be done in the constructor for one reason or another
			myCurrentPosition=myNewServoPosition+2;
			loop();

			}
			void loop()
			{
				if(myNewServoPosition!=myCurrentPosition)
				{
					if (myMinServoPosition<myMaxServoPosition)
					{
						myServoPosition=map(myNewServoPosition,0,180,(int)myMinServoPosition,(int)myMaxServoPosition);
					}
					else
					{
						myServoPosition=map(180-myNewServoPosition,0,180,(int)myMaxServoPosition,(int)myMinServoPosition);
					}
					Serial.print("pos =");
					Serial.println(myServoPosition);
					write((int)myServoPosition);
					myCurrentPosition=myNewServoPosition;
				}
			}

			void setPosition(uint8_t Position)
			{
				myNewServoPosition=Position;

			}

			void move(int8_t Movement)
			{
				setPosition(myServoPosition + Movement);
			}

			byte getPosition()
			{
				return myCurrentPosition;
			}

			void stop()
			{
				setPosition(myDefaultServoPosition);
			}

			private:
			uint8_t myServoControlPin; //the pin used to control this servo
			uint8_t myServoPosition;//The current constrained Position of the servo
			uint8_t myMaxServoPosition;//The maximum Position of the servo
			uint8_t myMinServoPosition;//The minimum Position of the servo
			uint8_t myNewServoPosition;//The Position the servo will move to on the next loop
			uint8_t myCurrentPosition;//The current position not constrained)
			uint8_t myDefaultServoPosition;//the position to go to in rest

		};
#endif //JANSERVO_H_
