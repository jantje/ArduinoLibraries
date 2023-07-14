#include "MCP413.h"
#include "AD5171.h"
uint8_t slavePin =49;
uint8_t I2CAdress=44;


// Activate the pot meter you have below
MCP413 digitalPot(127, slavePin,true);
//MCP413 digitalPot(255, slavePin);
//AD5171 digitalPot( 64, I2CAdress);


void setup()
{
	Serial.begin(115200);
	Serial.println("digital potmeter");

  SPI.begin();
  digitalPot.setup();
  Serial.println("setup done");
}

uint8_t val = 0;

void loop()
{
  val++;        // increment value;no need to do anything. vall will overflow and become zero
  if (val<100) val=130; //For my motor anything under 130 doesn't do anything

  digitalPot.setPotValue(val);
  digitalPot.loop();

  Serial.print("val =");
  Serial.println(val);
  delay(1000);
}
