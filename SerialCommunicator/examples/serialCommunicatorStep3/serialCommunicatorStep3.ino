/*
 * This is a example sketch showing the basic communication using line by line serial
 * communication
 *
 * This example skecth needs 5 libraries as you can see below with the includes
 * Make sure they are configured and linked to properly
 *
 * This example is a follow up of SerialCommunicatorStep2 which is bundled as an example in
 *  the SerialStringReader library
 *
 *  This example is seriously different from the previous both in code and as behavior
 *  as it goes on (or more) level(s) up
 *  In the previous example we used the line by line serial input of SerialStringReader
 *  here we still use it but indirectly and as such hidden.
 *
 *  In this example we still use BlinkLed but then indirectly with BlinkLedSerial.
 *  BlinkLedSerial derives from BlinkLed and adds the register function.
 *  The register is the secret to it all.
 *  Basically the register makes a copy (well it copies the pointers to) of the important
 *  objects in memory in such a way (read array) that the communicator can loop over it.
 *  Because the register also provides a name the communicator can output and modify
 *  any of the registered objects without any additional code.
 *
 *  It can also DUMP the memory objects it knows which allows for advanced control.
 *
 *  verify and upload the code; connect with the serial monitor and send following
 *  lines to the Arduino (do not forget to add a CR or LF or both)
 *  ?
 *  -> yes you can even see all commands note that
 *  DUMP
 *  -> all the fields with detailed technical info
 *  LOG HEADER
 *  -> all the field names the way they are logged in LOG VALUE
 *  SET
 *  -> Shows all the set commands to get back to how they are set now
 *  SET Admin.DelaybetweenLogs=5000
 *  ->notice the slower incoming LOG VALUE strings?
 *  SET Admin.DelaybetweenLogs=2000
 *  ->back to normal
 *  SET Admin.logLevel=0
 *  ->no more LOG VALUE messages
 *  SET BuildInLed.OffInterval=200
 *  ->Do you notice the change in led blinking?
 *  SET BuildInLed.OnInterval=1000
 *  ->You must see the led 13 blink different
 *
 *
 *  while playing around you may have noticed a string similar to the
 *  following in the serial monitor
 *  SET Admin.DelaybetweenLogs=2000
 *  Failed to log: availableForWrite =2 , SerialQueueSize =63
 *  This is a warning message that when the code wanted to log the values the serial queue
 *  was not empty. Therefore it will not log but log this error message
 *  Here it tells there were only 2 bytes available of 63.
 *
 *
 * no rights reserved
 */


#include "BlinkLed.h"
#include "BlinkLedSerial.h"
#include "SerialDataInterface.h"
#include "SerialCommunicator.h"
#include "SerialStringReader.h"



//This is a global variable used by the communicator which should hold
// a nice name to recognize the program
// The name is outputted as part of the DUMP command
const char  mySketchName[] PROGMEM="Serial Communicator step3";

//We replaced the SerialStringReader by the SerialCommunicator, with some impact
SerialCommunicator myCommunicator;
//we replace the BlinkLed by the BlinkLedSerial with hardly any impact
BlinkLedSerial led13(13,500,2000);

//Define that the USB serial (pin 0 and 1 on the uno) is to be used by the mySerialReader
//If you want to use the yun with the bridge use Serial1
#define THESERIAL Serial
Stream &SerialInput = THESERIAL;
Stream &SerialOutput = THESERIAL;
Stream &SerialError = THESERIAL;  //This channel has been added for error logging


//The setup function is called once at startup of the sketch
void setup()
{
	delay(2000);
	THESERIAL.begin(115200);
	//Here we register the communicator. This is not strictly needed but the communicator
	// contains some very handy features and setting so you probably want it.
	// Note that the name provided here is the name you will use to access it via the serial
	// monitor line commands
	// comment the line below and see what it does
	myCommunicator.serialRegister(F("Admin"));
	// We also register the led. Again we will access the led with the name provided here
	led13.serialRegister(F("BuildInLed"));
	//As MAXFIELDS needs to be big enough it is not a bad idea to know how big it is now
	SerialOutput.print(F("Current memory fields "));
	SerialOutput.println(lastFieldIndex); //yes I do use global variables
	SerialOutput.println(F(" from "));
	SerialOutput.println(MAXFIELDS); //13 of 70 that is way more than enough

	led13.setup();
	myCommunicator.setup();
}

// The loop function is called in an endless loop
void loop()
{
	// yes!!!! yes!!! That is all we need in the loop to change the blinking of the led
	myCommunicator.loop();
	led13.loop();
}
