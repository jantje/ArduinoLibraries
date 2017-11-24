
#include "simplot.h"

const int ledPin = BUILTIN_LED;

void setup() {
	  Serial.begin(115200);
	pinMode(ledPin, OUTPUT);
}

void loop() {
	const uint32_t currentMillis = millis();

// calculate the state of the led
	int ledValue1 = getBlinkLedValue(currentMillis, 1000, 2000);
	int ledValue2 = getFadeLedValue(currentMillis, 1000, 2000);

//do the physical action
	digitalWrite(ledPin, ledValue1);
	  plot2(Serial, ledValue1,ledValue2);

	  //show we are still alive
	  print something to serial every x millis

}
/**
 * Based on the parameters tells you whether the led should
 * be  on or off
 */
int getBlinkLedValue(const uint32_t currentMillis, const uint32_t onInterval,
		const uint32_t offInterval) {
	uint32_t currentFrame = currentMillis % (onInterval + offInterval);
	return currentFrame > onInterval;
}
/**
 * Based on the parameters tells you how hard the led should shine
 * return value between 0 255
 */
int getFadeLedValue(const uint32_t currentMillis, const uint32_t onInterval,
		const uint32_t offInterval) {
	uint32_t currentFrame = currentMillis % (onInterval + offInterval);
	if( currentFrame > onInterval){
		return 255-map(currentFrame-onInterval,0,offInterval,0,255);
	};
	return map(currentFrame,0,onInterval,0,255);
}
