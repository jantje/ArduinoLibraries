#include "Arduino.h"
#include "Wire.h"
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
#define redPin 11
#define greenPin 10
#define bluePin 9


void receiveEvent(int howMany);

void setup()
{
	Serial.begin(115200 );
	Serial.println("LCDSlave");
	// set up the LCD's number of columns and rows:
	lcd.begin(16, 2);
	lcd.setCursor(0, 1);
	lcd.println(F("listening for twi"));

	Wire.begin(0x38);                // join i2c bus with address 0x38
	Wire.onReceive(receiveEvent); // register event
	pinMode( redPin ,OUTPUT);
	pinMode( greenPin ,OUTPUT);
	pinMode( bluePin ,OUTPUT);
	analogWrite(redPin,255);
	analogWrite(greenPin,255);
	analogWrite(bluePin,255);

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
	if (strcmp_P(commandBuffer, (const char *) F( "clear")) ==0)
	{
		lcd.clear();
		return;
	}
	if (strcmp_P(commandBuffer,(const char *) F( "home")) ==0)
	{
		lcd.home();
		return;
	}
	if (strcmp_P(commandBuffer,(const char *) F( "noDisplay;")) ==0)
	{
		lcd.noDisplay();
		return;
	}
	if (strcmp_P(commandBuffer,(const char *) F( "display")) ==0)
	{
		lcd.display();
		return;
	}
	if (strcmp_P(commandBuffer,(const char *) F( "noBlink")) ==0)
	{
		lcd.noBlink();
		return;
	}
	if (strcmp_P(commandBuffer,(const char *) F( "blink")) ==0)
	{
		lcd.blink();
		return;
	}
	if (strcmp_P(commandBuffer,(const char *) F( "noCursor")) ==0)
	{
		lcd.noCursor();
		return;
	}
	if (strcmp_P(commandBuffer,(const char *) F( "cursor")) ==0)
	{
		lcd.cursor();
		return;
	}
	if (strcmp_P(commandBuffer,(const char *) F( "scrollDisplayLeft")) ==0)
	{
		lcd.scrollDisplayLeft();
		return;
	}
	if (strcmp_P(commandBuffer,(const char *) F( "scrollDisplayRight")) ==0)
	{
		lcd.scrollDisplayRight();
		return;
	}
	if (strcmp_P(commandBuffer,(const char *) F( "leftToRight")) ==0)
	{
		lcd.leftToRight();
		return;
	}
	if (strcmp_P(commandBuffer,(const char *) F( "rightToLeft")) ==0)
	{
		lcd.rightToLeft();
		return;
	}
	if (strcmp_P(commandBuffer,(const char *) F( "autoscroll")) ==0)
	{
		lcd.autoscroll();
		return;
	}
	if (strcmp_P(commandBuffer,(const char *) F( "noAutoscroll")) ==0)
	{
		lcd.noAutoscroll();
		return;
	}
	if (strncmp_P(commandBuffer,(const char *)F("print;"),6)==0)
	{
		char * dataPointer=commandBuffer+6;
		lcd.print(dataPointer);
		return;
	}
	if (strncmp_P(commandBuffer,(const char *)F("println;"),8)==0)
	{
		char * dataPointer=commandBuffer+8;
		lcd.print(dataPointer);
		return;
	}
	if (strncmp_P(commandBuffer,(const char *)F("setCursor;"),10)==0)
	{
		char * dataPointer=commandBuffer+10;
		char * sep= strchr(dataPointer,';');
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
	}
	if (strncmp_P(commandBuffer,(const char *)F("setBackground;"),14)==0)
	{
		char * dataPointer=commandBuffer+14;
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
