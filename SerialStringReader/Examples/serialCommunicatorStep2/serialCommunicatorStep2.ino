/*
 * This is a example sketch showing the basic functionality of reading line by line from
 * Serial (or any other stream)
 *
 * This example skecth needs 2 libraries SerialStringReader and BlinkLed
 * Make sure they are configured and linked to properly
 *
 * after compiling and uploading send following commands to the arduino
 * LED SLOW
 * LED FAST
 * LED ASYNC
 * All the commands should change the blinking of led 13
 *
 * no rights reserved
 */
#include "SerialStringReader.h"
#include "BlinkLed.h"


//for the stream you want to read info from you need to declare a SerialStringReader
//you can only have one
SerialStringReader myCommunicator;

//Define that the USB serial (pin 0 and 1 on the uno) is to be used by the mySerialReader
//If you want to use the yun with the bridge use Serial1
#define THESERIAL Serial
Stream &SerialInput = THESERIAL;
Stream &SerialOutput = THESERIAL;

BlinkLed led13(13,500,2000);


//The setup function is called once at startup of the sketch
void setup()
{
	delay(2000);
	THESERIAL.begin(115200);
	SerialOutput.println("serialStringReaderDemo2");
	led13.setup();
	myCommunicator.setup();
}

// The loop function is called in an endless loop
void loop()
{
	myCommunicator.loop();
	led13.loop();
	if (myCommunicator.messageReceived())
	{
		SerialOutput.println("You have send a message to Arduino.");
		SerialOutput.println("And the message is:");
		char * message=myCommunicator.getMessage();
		SerialOutput.println(message);
		if (strcasecmp(message,"LED SLOW")==0)
		{
			led13.setOnInterval(5000);
			led13.setOffInterval(5000);
			SerialOutput.println("changed to slow");
		}
		if (strcasecmp(message,"LED FAST")==0)
		{
			led13.setOnInterval(100);
			led13.setOffInterval(100);
			SerialOutput.println("changed to fast");
		}
		if (strcasecmp(message,"LED ASYNC")==0)
		{
			led13.setOnInterval(1000);
			led13.setOffInterval(200);
			SerialOutput.println("changed to async");
		}
	}
}
