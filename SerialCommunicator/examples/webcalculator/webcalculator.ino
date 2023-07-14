#include "SerialCommunicator.h"

//If you are not using yun use Serial instead of Serial1 in the line below
#define THESERIAL Serial1
//The stream used for reading standard input
Stream &SerialInput = THESERIAL;
//The stream for writing standard output
Stream &SerialOutput = THESERIAL;
//The stream for writing errors
Stream &SerialError = THESERIAL;

//The name of the program. This name is visible in the html page
const char mySketchName[] PROGMEM= "Calculator";


//the communication channel
SerialCommunicator myCommunicator;

//The variables used to do the actual calculation
int value1;
int value2;
int sum;

void setup()
{
	delay(5000);
	THESERIAL.begin(115200);
	// Wait for U-boot to finish startup.  Consume all bytes until we are done.
	do
	{
		while (SerialInput.available() > 0)
		{
			SerialInput.read();
		}
		delay(1000);
	} while (SerialInput.available() > 0);


	FieldData::set(F("Sum"), F("adding"),MOD_WRITE, &value1);
	FieldData::set(F("Sum"), F("to"),MOD_WRITE, &value2);
	FieldData::set(F("Sum"), F("gives"),MOD_NONE, &sum);

	myCommunicator.setup();
}

void loop()
{
	sum=value1+value2;
	myCommunicator.loop();
}
