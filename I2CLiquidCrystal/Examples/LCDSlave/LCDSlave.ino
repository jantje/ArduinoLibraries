#include "Arduino.h"
#include "Wire.h"
#include <LiquidCrystal.h>
#include "mapper.h"

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
#define redPin 11
#define greenPin 10
#define bluePin 9

void receiveEvent(int howMany);

void setup()
{
	Serial.begin(115200);
	Serial.println("LCDSlave");
	// set up the LCD's number of columns and rows:
	lcd.begin(16, 2);
	lcd.setCursor(0, 1);
	lcd.println(F("listening for twi"));

	Wire.begin(0x38);                // join i2c bus with address 0x38
	Wire.onReceive(receiveEvent); // register event
	pinMode(redPin, OUTPUT);
	pinMode(greenPin, OUTPUT);
	pinMode(bluePin, OUTPUT);
	analogWrite(redPin, 255);
	analogWrite(greenPin, 255);
	analogWrite(bluePin, 255);

}
#define BUFFERSIZE 50
char commandBuffer[BUFFERSIZE + 1];
byte bufferPos = 0;
void processTWIReceivedData();
void loop()
{
	if (Wire.available())
	{
		char c = Wire.read();
		//lcd.print(c);
		Serial.print(c);
		if (c == '\n') c = 0;
		if (c == '\r') c = 0;
		if (bufferPos < BUFFERSIZE)
		{
			commandBuffer[bufferPos++] = c; //Put the value in the buffer
			commandBuffer[bufferPos] = 0; //terminate with 0 so all C/C++ string functions work
		} else
		{
			Serial.println();
			Serial.println("ERROR: buffer overflow");
			bufferPos = 0;
		}
		if (c == 0)
		{
			if (bufferPos > 2)
			{
				processTWIReceivedData();
			}
			bufferPos = 0;
			commandBuffer[bufferPos] = 0;
		}

	}

}

void processTWIReceivedData()
{
	switch (commandBuffer[0])
	{
		case _clear:
			lcd.clear();
			return;
		case _home:
			lcd.home();
			return;
		case _noDisplay:
			lcd.noDisplay();
			return;
		case _display:
			lcd.display();
			return;
		case _blink:
			lcd.blink();
			return;
		case _noBlink:
			lcd.noBlink();
			return;
		case _noCursor:
			lcd.noCursor();
			return;
		case _cursor:
			lcd.cursor();
			return;

		case _scrollDisplayLeft:
			lcd.scrollDisplayLeft();
			return;
		case _scrollDisplayRight:
			lcd.scrollDisplayRight();
			return;
		case _leftToRight:
			lcd.leftToRight();
			return;
		case _rightToLeft:
			lcd.rightToLeft();
			return;
		case _autoscroll:
			lcd.autoscroll();
			return;
		case _noAutoscroll:
			lcd.noAutoscroll();
			return;
		case _print:
			char * dataPointer=commandBuffer+1;
			lcd.print(dataPointer);
			return;
		case _println:
			char * dataPointer=commandBuffer+1;
			lcd.println(dataPointer);
			return;
		case _setCursor:
			char * dataPointer=commandBuffer+1;
			char * sep= strchr(dataPointer,sepperator);
			if (sep==0)
			{
				Serial.print(F("ERROR: Incomplete command "));
				return;
			}
			*sep=0;
			uint8_t col = (uint8_t) atoi(dataPointer);
			uint8_t row = (uint8_t) atoi(++sep);
			lcd.setCursor(col,row);
			return;

		case _setBackground:
			char * dataPointer=commandBuffer+1;
			char * sep1= strchr(dataPointer,';');
			if (sep1==0)
			{
				Serial.print(F("ERROR: Incomplete command 1"));
				return;
			}
			char * sep2= strchr(sep1+1,';');
			if (sep2==0)
			{
				Serial.print(F("ERROR: Incomplete command 2"));
				return;
			}
			*sep2=0;
			uint8_t red = (uint8_t) atoi(dataPointer);
			uint8_t green = (uint8_t) atoi(++sep1);
			uint8_t blue = (uint8_t) atoi(++sep2);
			Serial.print("setBackground (");
			Serial.print(red);
			Serial.print(" , ");
			Serial.print(green);
			Serial.print(" , ");
			Serial.print(blue);
			Serial.println(" )");

			analogWrite(redPin,red);
			analogWrite(greenPin,green);
			analogWrite(bluePin,blue);
			return;
		}
	 Serial.print(F("Unknown command. : "));

	}

void receiveEvent(int howMany)
{

}
