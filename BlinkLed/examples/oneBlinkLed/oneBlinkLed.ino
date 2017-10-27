#include "BlinkLed.h"

//use the default blinkled to blink 1s on 2s off
BlinkLed buildInled(LED_BUILTIN,1000 ,2000);
void setup()
{
	buildInled.setup();
}

// The loop function is called in an endless loop
void loop()
{
	buildInled.loop();
}
