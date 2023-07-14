#include "Arduino.h"
#include "SerialCommunicator.h"

#include "SkidDriverBLDC.h"
SkidDriverBLDC driver(7,5, 6,4);

Stream &SerialOutput = Serial;
Stream &SerialInput = Serial;

const char mySketchName[] PROGMEM= "BLDC1DriverTester";

SerialCommunicator myCommunicator;

void setup()
{
	Serial.begin(115200);
	Serial.println("Start sketch BLDC1DriveTester");
	myCommunicator.serialRegister(F("Admin"));

	driver.serialRegister(F("driver"),F("driver.left"),F("driver.right"));

	Serial.print(F("Current Datas "));
	Serial.println(lastFieldIndex);
	Serial.println(F(" from "));
	Serial.println(MAXFIELDS);
	driver.setup();
	myCommunicator.setup();
}

// The loop function is called in an endless loop
void loop()
{
	driver.loop();
	myCommunicator.loop();
	driver.plot();
	delay(50);

}
