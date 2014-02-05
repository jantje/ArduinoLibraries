#include "Arduino.h"
#include "I2CLiquidCrystal.h"
#include "Wire.h"


LiquidCrystal lcd(0x38);  // Set the LCD I2C address



void setup()
{
  Serial.begin(115200);
  Serial.println("LCDMaster");

  lcd.begin();               // initialize the lcd
  lcd.println("Hello, ARDUINO");
  lcd.setCursor ( 0, 1 );        // go to the next line
  Serial.println("Setup done");
}

void loop()
{
	static int count=0;
	lcd.setCursor ( 0, 1 );
	lcd.println(count);
	uint8_t red = 255 *(count%3==0); //((uint8_t)count) *(count%3==0);
	uint8_t green = 255 *(count%3==1);
	uint8_t blue = 255 *(count%3==2);
	lcd.setBackground(red,green,blue);
	Serial.print("Hello, ARDUINO ");
	Serial.println(count);
	count++;
   delay (500);
}
