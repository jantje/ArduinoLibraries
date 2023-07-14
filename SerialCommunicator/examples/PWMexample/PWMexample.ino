/*
 * This is an example how you can set values to PWM pins
 * That can be used for any other setting that needs code triggering
 */
#include "SerialCommunicator.h"
#include "SerialDataInterface.h"
#include "SerialStringReader.h"
SerialCommunicator myCommunicator;
Stream &SerialInput = Serial;
Stream &SerialOutput = Serial;
Stream &SerialError = Serial;

const char mySketchName[] PROGMEM="set PWM sample";

class pwmPin
{
	private:
		uint8_t myPWM;
		uint8_t newPWM;
		uint8_t mypin;
		public:
		pwmPin(uint8_t pin){mypin=pin;myPWM=newPWM=0;}
		void setup()
		{
			pinMode(mypin,OUTPUT);
			analogWrite(mypin,myPWM);
		};
		void loop()
		{
			if (myPWM!=newPWM)
			{
				myPWM=newPWM;
				analogWrite(mypin,myPWM);
			}
		}
		void serialRegister(const __FlashStringHelper* Name)
		{
			FieldData::set(Name, F("newPWMValue"),MOD_WRITE,  &newPWM);
			FieldData::setNext( F("curPWMValue"), 0, &myPWM);
			FieldData::setNext( F("pin"), 0, &mypin);
		}

};

pwmPin myPWMPin3(3);
pwmPin myPWMPin4(4);
pwmPin myPWMPin5(5);
pwmPin myPWMPin6(6);
void setup()
{
	Serial.begin(115200);
	myCommunicator.serialRegister(F("communicator"));
	myPWMPin3.serialRegister(F("pin3"));
	myPWMPin4.serialRegister(F("pin4"));
	myPWMPin5.serialRegister(F("pin5"));
	myPWMPin6.serialRegister(F("pin6"));
	myPWMPin3.setup();
	myPWMPin4.setup();
	myPWMPin5.setup();
	myPWMPin6.setup();
	myCommunicator.setup();
}


void loop()
{
	myPWMPin3.loop();
	myPWMPin4.loop();
	myPWMPin5.loop();
	myPWMPin6.loop();
	myCommunicator.loop();
}
