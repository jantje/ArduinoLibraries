/* 
 Controlling a servo position using serial communication
*/

#include "JanServo.h"
#include "SerialDataInterface.h"
#include "SerialCommunicator.h"
#include "SerialStringReader.h"


int servoLock = 75; // angle (deg) of "locked" servo
int servoUnlock = 20; // angle (deg) of "unlocked" servo

//This is a global variable used by the communicator which should hold
// a nice name to recognize the program
// The name is outputted as part of the DUMP command
const char  mySketchName[] PROGMEM="servo";
SerialCommunicator myCommunicator;

#define THESERIAL Serial
Stream &SerialInput = THESERIAL;
Stream &SerialOutput = THESERIAL;
Stream &SerialError = THESERIAL;  //This channel has been added for error logging

JanConstraintServo servoLatch(9, 0, 180, servoUnlock);

//This is where the memory objects are saved for the communicator
// It is vital this is big enough. If this is not big enough you will get wierd results
#define MAXFIELDS 70
FieldData AllFields[MAXFIELDS];


int val;    // variable to read the value from the analog pin

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
	servoLatch.serialRegister(F("Servo"));
	//As MAXFIELDS needs to be big enough it is not a bad idea to know how big it is now
	SerialOutput.print(F("Current memory fields "));
	SerialOutput.println(lastFieldIndex); //yes I do use global variables
	SerialOutput.println(F(" from "));
	SerialOutput.println(MAXFIELDS); //13 of 70 that is way more than enough

	servoLatch.setup();
	myCommunicator.setup();
}

void loop() 
{ 
  servoLatch.loop();
  myCommunicator.loop();

} 

