/*
 * This is a example sketch showing how to read potentiometer values
 *
 * This example sketch needs quite some libraries as you can see below with the includes
 * Make sure they are configured and linked to properly
 *
 * This example is a follow up of SerialCommunicatorStep5 which is bundled as an example in
 *  the SerialCommunicator library
 *
 *  This example we change the serial port so that we can use the bridge between
 *  the avr and the linux on the yun
 *  There is also a need to wait to start due to uboot
 *  For fun I also swapped the serial registers so we get a different web page
 *  All other actions are on the linux part.
 *
 *
 * author jantje
 *
 */


#include "BlinkLed.h"
#include "BlinkLedSerial.h"
#include "SerialDataInterface.h"
#include "SerialCommunicator.h"
#include "SerialEEPROMCommunicator.h"
#include "SerialStringReader.h"


const char  mySketchName[] PROGMEM="potentiometers";

SerialEEPROMCommunicator myCommunicator;
BlinkLedSerial led13(13,500,2000);


//Set the communication to Serial1
#define THESERIAL Serial1
Stream &SerialInput = THESERIAL;
Stream &SerialOutput = THESERIAL;
Stream &SerialError = THESERIAL;


#define MAXFIELDS 70
FieldData AllFields[MAXFIELDS];

//I would do this normally with a class but for fun lets use a simple array only
// the array index corresponds to the analog pin number
#define NUM_POTENTIO_METERS 3 //this must be minimal 2 as we use A0 and A1 for setting the led
uint16_t myPotentioMeters[NUM_POTENTIO_METERS];


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

	//led13.serialRegister(F("BuildInLed"));
	myCommunicator.serialRegister(F("Admin"));

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
	for (uint8_t curPotmeter=0;curPotmeter<NUM_POTENTIO_METERS;curPotmeter++)
	{
		//This code relies on the fact that A0 ->A5 are sequential pins
		myPotentioMeters[curPotmeter]=analogRead((uint8_t)A0+curPotmeter);
	}
	led13.setOnInterval(myPotentioMeters[0]);
	led13.setOffInterval(myPotentioMeters[1]);
}
