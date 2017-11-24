
#include "makerfairRome2017Demo.h"
const int ledPin1 = LED_BUILTIN;

void setup() {
	Serial.begin(115200);
	pinMode(ledPin1, OUTPUT);
}

void loop() {
	const uint32_t currentMillis = millis();

// calculate the state of the leds
	int ledValue1 = getBlinkLedValue(currentMillis, 1000, 2000);
	int ledValue2 = getFadeLedValue(currentMillis, 1000, 2000);

//calculate if we need to print a keep alive message
	static int prefNumtriggers = 0;
	int curNumTriggers = numTriggers(currentMillis, 2000);
	bool printAliveMessage = curNumTriggers != prefNumtriggers;
	prefNumtriggers = curNumTriggers;

//do the physical actions
	digitalWrite(ledPin1, ledValue1);
	plot3(Serial, ledValue1, ledValue2, curNumTriggers);

	//show we are still alive
	if (printAliveMessage) {
		Serial.println("I'm alive");
	}

}
/**
 * Based on the parameters tells you whether the led should
 * be  on or off
 */
int getBlinkLedValue(const uint32_t currentMillis, const uint32_t onInterval,
		const uint32_t offInterval) {
	uint32_t currentFrame = currentMillis % (onInterval + offInterval);
	int ret = currentFrame < onInterval;
	return ret;
}
/**
 * Based on the parameters tells you how hard the led should shine
 * return value between 0 255
 */
int getFadeLedValue(const uint32_t currentMillis, const uint32_t onInterval,
		const uint32_t offInterval) {
	uint32_t currentFrame = currentMillis % (onInterval + offInterval);
	if (currentFrame > onInterval) {
		return 255 - map(currentFrame - onInterval, 0, offInterval, 0, 255);
	};
	int ret= map(currentFrame, 0, onInterval, 0, 255);
	return ret;
}

/**
 *Tells you how many times there should have been triggered since the start of the cpu
 */
uint32_t numTriggers(const uint32_t currentMillis,
		const uint32_t triggerInterval) {
	uint32_t ret= currentMillis / triggerInterval;
	return ret;
}
