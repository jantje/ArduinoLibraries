// Do not remove the include below
#include "SerialStringReader.h"
SerialStringReader mySerialReader(&Serial,&Serial);


//The setup function is called once at startup of the sketch
void setup()
{
	Serial.begin(9600);
	mySerialReader.setup();
}

// The loop function is called in an endless loop
void loop()
{
	mySerialReader.loop();
	if (mySerialReader.messageReceived())
	{
		Serial.println("You have send a message to Arduino.");
		Serial.println("And the message is:");
		Serial.println(mySerialReader.getMessage());
	}
}
