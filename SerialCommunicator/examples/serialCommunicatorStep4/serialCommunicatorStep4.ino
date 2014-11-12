/*
 * This is a example sketch showing the use of EEPROM to save values
 *
 * This example sketch needs quite some libraries as you can see below with the includes
 * Make sure they are configured and linked to properly
 *
 * This example is a follow up of SerialCommunicatorStep3 which is bundled as an example in
 *  the SerialCommunicator library
 *
 *  This example is slightly different from the previous in code and allows saving values in
 *  EEPROM
 *
 *  In the previous example we could change the blink rhythm of the led on pin 13
 *  and the logging parameters but each time the Arduino is reset these object values
 *  are lost.
 *  In this example we will add a the possibility to save the object values that are
 *  tagged as possible to save.
 *
 *
 *  verify and upload the code; connect with the serial monitor and send following
 *  lines to the Arduino (do not forget to add a CR or LF or both)
 *  ?
 *  ->note that a SAVE and LOAD have been added
 *  SET Admin.DelaybetweenLogs=5000
 *  -> reboot the arduino
 *  SET
 *  -> Admin.DelaybetweenLogs is set to the default value (probably garbage)
 *   *  SET Admin.DelaybetweenLogs=5000
 *  SAVE
 *  -> reboot the arduino
 *  SET
 *  -> Admin.DelaybetweenLogs is still set on 5000
 *  SET Admin.DelaybetweenLogs=800
 *  LOAD
 *  -> Admin.DelaybetweenLogs is reset to 5000
 *
 * no rights reserved
 */


#include "BlinkLed.h"
#include "BlinkLedSerial.h"
#include "SerialDataInterface.h"
#include "SerialCommunicator.h"
#include "SerialEEPROMCommunicator.h"
#include "SerialStringReader.h"


const char  mySketchName[] PROGMEM="Serial Communicator step4";

//We replaced the SerialCommunicator by the SerialEEPROMCommunicator,
// with no impact on the code
SerialEEPROMCommunicator myCommunicator;
BlinkLedSerial led13(13,500,2000);


#define THESERIAL Serial
Stream &SerialInput = THESERIAL;
Stream &SerialOutput = THESERIAL;
Stream &SerialError = THESERIAL;


#define MAXFIELDS 70
FieldData AllFields[MAXFIELDS];


void setup()
{
	delay(2000);
	THESERIAL.begin(115200);
	myCommunicator.serialRegister(F("Admin"));
	led13.serialRegister(F("BuildInLed"));
	SerialOutput.print(F("Current memory fields "));
	SerialOutput.println(lastFieldIndex);
	SerialOutput.println(F(" from "));
	SerialOutput.println(MAXFIELDS);

	led13.setup();
	myCommunicator.setup();
}


void loop()
{
	myCommunicator.loop();
	led13.loop();
}
