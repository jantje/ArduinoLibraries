// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include "SerialCommunicator.h"

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            A0

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      5

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);

int delayval = 500; // delay for half a second

//If you are not using yun use Serial instead of Serial1 in the line below
#define THESERIAL Serial1
//The stream used for reading standard input
Stream &SerialInput = THESERIAL;
//The stream for writing standard output
Stream &SerialOutput = THESERIAL;
//The stream for writing errors
Stream &SerialError = THESERIAL;

//The name of the program. This name is visible in the html page
const char mySketchName[] PROGMEM= "WebPixels";

//the communication channel
SerialCommunicator myCommunicator;

class WebPixel
{
	private:
		uint8_t R;
		uint8_t G;
		uint8_t B;
	public:
		uint32_t getColor()
		{
			return pixels.Color(R, G, B);
		}
		void serialRegister(const __FlashStringHelper* Name)
		{
			FieldData::set(Name, F("Red"),MOD_WRITE | MOD_SAVE, &R);
			FieldData::setNext( F("Green"),MOD_WRITE | MOD_SAVE, &G);
			FieldData::setNext( F("Blue"),MOD_WRITE | MOD_SAVE, &B);
		}
	};
WebPixel webPixel[NUMPIXELS];

void setup()
{
	// This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
	if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
	// End of trinket special code

	pixels.begin(); // This initializes the NeoPixel library.

	delay(5000);
	THESERIAL.begin(115200);
	//THESERIAL.begin(250000);
	// Wait for U-boot to finish startup.  Consume all bytes until we are done.
	do
	{
		while (SerialInput.available() > 0)
		{
			SerialInput.read();
		}
		delay(1000);
	} while (SerialInput.available() > 0);

	myCommunicator.serialRegister(F("Admin"));
	webPixel[0].serialRegister(F("Pixels.Pixel1"));
	webPixel[1].serialRegister(F("Pixels.Pixel2"));
	webPixel[2].serialRegister(F("Pixels.Pixel3"));
	webPixel[3].serialRegister(F("Pixels.Pixel4"));
	webPixel[4].serialRegister(F("Pixels.Pixel5"));

	SerialOutput.print(F("Current memory fields "));
	SerialOutput.println(lastFieldIndex); //yes I do use global variables
	SerialOutput.println(F(" from "));
	SerialOutput.println(MAXFIELDS);
	myCommunicator.setup();
}

void loop()
{

	// For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.

	for (int i = 0; i < NUMPIXELS; i++)
	{

		// pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
		pixels.setPixelColor(i, webPixel[i].getColor());
	}
	pixels.show(); // This sends the updated pixel color to the hardware.
	delay(delayval);
	myCommunicator.loop();
}

