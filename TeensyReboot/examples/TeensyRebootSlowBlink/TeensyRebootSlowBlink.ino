/** example program demonstrating how to use the reset teensy 4,1
 * based on the information provided by paul on the teensy forums
 * from https://forum.pjrc.com/index.php?threads/teensy-4-1-software-reset-code.74572/
 * connect pin 32 to 3,3V to reset the teensy
 * Blinks slowly
 */

#include "Arduino.h"
#include "TeensyReboot.h"
#include "BlinkLed.h"

#define REBOOT_PIN 32
#define REBOOT_DELAY_TIME 10000 //time for USB hubs/ports to detect disconnect Paul says 50
TeensyReboot teensyReboot(REBOOT_PIN,REBOOT_DELAY_TIME ,INPUT_PULLDOWN);
BlinkLed imAliveLed(LED_BUILTIN,500 ,1000);



void setup()
{
	imAliveLed.setup();
	teensyReboot.setup();
}

void loop()
{
	imAliveLed.loop();
	teensyReboot.loop();

}

