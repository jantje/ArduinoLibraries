/*
 * This is a example sketch showing the basic functionality of reading line by line from
 * Serial (or any other stream)
 *
 * no rights reserved
 */
#include "SerialStringReader.h"
//for the stream you want to read info from you need to declare a SerialStringReader
//you can only have one
SerialStringReader myCommunicator;

//Define that the USB serial (pin 0 and 1 on the uno) is to be used by the mySerialReader
//If you want to use the yun with the bridge use Serial1
#define THESERIAL Serial
Stream &SerialInput = THESERIAL;
Stream &SerialOutput = THESERIAL;



void setup()
{
	delay(2000);
	THESERIAL.begin(115200);
	SerialOutput.println("serialStringReaderDemo2");
	myCommunicator.setup();
}

void loop()
{
	//always add the following line to your loop
	myCommunicator.loop();
	//mySerialReader.messageReceived() will only return true when a line of data has arrived
	if (myCommunicator.messageReceived())
	{
		SerialOutput.println("You have send a message to Arduino.");
		SerialOutput.println("And the message is:");
		// with mySerialReader.getMessage() we get the actual message
		// for the demo we simply play it back.
		SerialOutput.println(myCommunicator.getMessage());
	}
}
