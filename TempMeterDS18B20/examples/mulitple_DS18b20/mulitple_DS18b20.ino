/*
 * Example showing how to read 3 DS18B20 temp sensors
 * The example is originally written for a mega and uses
 * pins 43 45 and 47 when the sensors are all on a
 * different pin.
 * Pin 43 is used when all sensors are on the same pin.
 * You will have to modify the code if you want to use diferent pins.
 *
 * If speed is important define USE_MAIN_LOOP_MILLIS on the command line.
 * When the define is set there will be less calls to millis() which
 * will speed up the code
 *
 * did not get the maximum time spend during a loop to read the 3 sensors
 * below 18 millis. If you hit numbers bigger than 20 consider to define
 * MAX_MILLIS_IN_LOOP_TO_START as 15 (this way there will never be 2 wire calls in the same loop)
 *
 */
#include "Arduino.h"
#include "TempMeterDS18B20.h"
#include "OneWire.h"
uint32_t loopMillis;
uint32_t lastLogMillis;

//remove define below to test 3 sensors on same pin
#define DIFFERENT_PINS

#ifdef DIFFERENT_PINS
OneWire oneWireTemp1(43);
TempMeterDS18B20 meter1(oneWireTemp1,0);

OneWire oneWireTemp2(45);
TempMeterDS18B20 meter2(oneWireTemp2,0);

OneWire oneWireTemp3(47);
TempMeterDS18B20 meter3(oneWireTemp3,0);
#else
OneWire oneWireTemp(43);
TempMeterDS18B20 meter1(oneWireTemp,0);

TempMeterDS18B20 meter2(oneWireTemp,1);

TempMeterDS18B20 meter3(oneWireTemp,2);

#endif


void setup()
{

	Serial.begin(115200);
	delay(2000);
	Serial.println("Example on using the TempMeterDS18B20 library ");
	Serial.println("Originally written for a Mega using pins 47 , 45 , 43");
	meter1.setup();
	meter2.setup();
	meter3.setup();
}

int16_t logValue(int16_t prefCelcius,int16_t curCelcius,char meterName[]){
	if(prefCelcius==curCelcius){
		return curCelcius;
	}
	Serial.print("new temp reading on meter : ");
	Serial.print(meterName);
	Serial.print(" : ");
	Serial.print(curCelcius);
	Serial.print(" centi Celcius at ");
	Serial.println(loopMillis);

	lastLogMillis=loopMillis;
	return curCelcius;
}

// The loop function is called in an endless loop
void loop()
{
	static int16_t prefCelcius1=0;
	static int16_t prefCelcius2=0;
	static int16_t prefCelcius3=0;
	loopMillis=millis();
	meter1.loop();
	meter2.loop();
	meter3.loop();


	unsigned long endMillis=millis();

	prefCelcius1 = logValue(prefCelcius1,meter1.getCentiCelsius(),"celcius1");
	prefCelcius2 = logValue(prefCelcius2,meter2.getCentiCelsius(),"celcius2");
	prefCelcius3 = logValue(prefCelcius3,meter3.getCentiCelsius(),"celcius3");



	if(endMillis-loopMillis>=15){
		Serial.print("getting the temp took: ");
		Serial.print(endMillis-loopMillis);
		Serial.println(" millis");
		lastLogMillis=loopMillis;
	}
	if(loopMillis-lastLogMillis>3000){
	//to long no serial output
		Serial.println("No meter provided a different value for 3 seconds ");
		lastLogMillis=loopMillis;
	}
}
