// Do not remove the include below
#include "Arduino.h"
#include "SerialDataInterface.h"
#include "SerialCommunicator.h"
#include "Analog.h"

#define THESERIAL Serial2
//#define THESERIAL Serial
Stream &SerialInput = THESERIAL;
Stream &SerialOutput = THESERIAL;
Stream &SerialError = THESERIAL;


const char mySketchName[] PROGMEM= "AnalogTester";

//Here we will store all the pointers to data and names
#define MAXFIELDS 160
FieldData AllFields[MAXFIELDS];

//the communication channel
SerialCommunicator myCommunicator;

//The analog pin reader
Analog myAnalog;

void setup()
{

//	analogReference(INTERNAL2V56);

	delay(5000);
	THESERIAL.begin(115200);

  // Wait for U-boot to finish startup.  Consume all bytes until we are done.
 do {
    while (SerialOutput.available() > 0) {
    	SerialOutput.read();
       }

   delay(1000);
 } while (SerialOutput.available()>0);

	SerialOutput.println ( (const __FlashStringHelper *)mySketchName);

	myCommunicator.serialRegister(F("Admin"));
	myAnalog.serialRegister(F("Analog"));

	SerialOutput.print(F("Current Datas "));
	SerialOutput.println(lastFieldIndex);
	SerialOutput.println(F(" from "));
	SerialOutput.println(MAXFIELDS);

	myCommunicator.setup();

	myAnalog.setup();

}

void loop()
{

	myAnalog.loop();
	myCommunicator.loop();
}
