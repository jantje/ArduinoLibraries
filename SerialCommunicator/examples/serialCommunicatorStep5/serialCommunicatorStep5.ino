/*
 * This is a example sketch showing how to read potentiometer values
 *
 * This example sketch needs quite some libraries as you can see below with the includes
 * Make sure they are configured and linked to properly
 *
 * This example is a follow up of SerialCommunicatorStep4 which is bundled as an example in
 *  the SerialCommunicator library
 *
 *  This example we add potentiometers and change the blinking of the led
 *  based on the settings of the potentiometer
 *
 *
 *
 *  verify and upload the code; connect with the serial monitor and send following
 *  lines to the Arduino (do not foreget to add a CR or LF or both)
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


const char  mySketchName[] PROGMEM="Serial Communicator step 5";

SerialEEPROMCommunicator myCommunicator;
BlinkLedSerial led13(13,500,2000);



#define THESERIAL Serial
Stream &SerialInput = THESERIAL;
Stream &SerialOutput = THESERIAL;
Stream &SerialError = THESERIAL;


//I would do this normally with a class but for fun lets use a simple array only
// the array index corresponds to the analog pin number
#define NUM_POTENTIO_METERS 3 //this must be minimal 2 as we use A0 and A1 for setting the led
uint16_t myPotentioMeters[NUM_POTENTIO_METERS];


void setup()
{
	delay(2000);
	THESERIAL.begin(115200);
	myCommunicator.serialRegister(F("Admin"));
	led13.serialRegister(F("BuildInLed"));
	//Do the serial register directly
	//it is a difficult construction with if as each field needs its own string
	FieldData::set(F("PotMeters"), F("A0"),0,&myPotentioMeters[0]);
#if NUM_POTENTIO_METERS>1
	FieldData::set(F("PotMeters"), F("A1"),0,&myPotentioMeters[1]);
#if NUM_POTENTIO_METERS>2
	FieldData::set(F("PotMeters"), F("A2"),0,&myPotentioMeters[2]);
#if NUM_POTENTIO_METERS>3
	FieldData::set(F("PotMeters"), F("A3"),0,&myPotentioMeters[3]);
#if NUM_POTENTIO_METERS>4
	FieldData::set(F("PotMeters"), F("A4"),0,&myPotentioMeters[4]);
#if NUM_POTENTIO_METERS>5
	FieldData::set(F("PotMeters"), F("A5"),0,&myPotentioMeters[5]);
#endif
#endif
#endif
#endif
#endif

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
	for (int curPotmeter=0;curPotmeter<NUM_POTENTIO_METERS;curPotmeter++)
	{
		//This code relies on the fact that A0 ->A5 are sequential pins
		myPotentioMeters[curPotmeter]=analogRead(A0+curPotmeter);
	}
	led13.setOnInterval(myPotentioMeters[0]);
	led13.setOffInterval(myPotentioMeters[1]);
}
