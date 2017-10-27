#include "BlinkLed.h"

//use the default blinkled to blink 1s on 2s off
BlinkLed buildInled(LED_BUILTIN,1000 ,2000);
//change the pin number of the second led
BlinkLed anotherInled(12,2000 ,1000);
void setup()
{
	buildInled.setup();
	anotherInled.setup();
}

// The loop function is called in an endless loop
void loop()
{
	buildInled.loop();
	anotherInled.loop();
}
