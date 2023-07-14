#include "Arduino.h"
#include "SerialEEPROMCommunicator.h"
#include "simplot.h"

#include "MotorBLDC.h"
MotorBLDC driver1(7, 6);
MotorBLDC driver2(5, 4);

Stream &SerialOutput = Serial;
Stream &SerialInput = Serial;

#define MAXFIELDS 50
FieldData AllFields[MAXFIELDS];
const char mySketchName[] PROGMEM= "BLDC1MotorTester";

SerialEEPROMCommunicator myCommunicator;


void setup()
{
	Serial.begin(115200);
	Serial.println("Start sketch BLDC1MotorTester");
	myCommunicator.serialRegister(F("Admin"));
	driver1.serialRegister(F("motor1"));
	driver2.serialRegister(F("motor2"));

	Serial.print(F("Current Datas "));
	Serial.println(lastFieldIndex);
	Serial.println(F(" from "));
	Serial.println(MAXFIELDS);
	driver1.setup();
	driver2.setup();
	myCommunicator.setup();
}


void loop()
{
	driver1.loop();
	driver2.loop();
	myCommunicator.loop();
	//driver2.plotdata();
	delay(20);

}
