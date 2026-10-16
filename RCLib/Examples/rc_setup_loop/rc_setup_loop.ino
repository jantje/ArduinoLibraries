/**
 * An example for using the RC lib library in the setup loop way
 * That is call rcLib.setup() in setup and rcLib.loop() in loop
 *
 * Note RC lib is different to other libs from Jantje because
 * you can only create one RCLib instance
 * You have to define NUM_RC_CHANNELS and create RC_Channel_Pin
 * before including RCLib.h
 *
 * this is because RCLib uses global variables based on these defines
 * I basically didn't find a way around this because
 * you need the hardware and
 */

#include "Arduino.h"

#ifdef ARDUINO_AVR_MEGA2560
#define NUM_RC_CHANNELS 6 //You need to specify how many pins you want to use
#include "PinChangeInt.h"  //If you need pinchangeint you need to include this header
const uint8_t RC_Channel_Pin[NUM_RC_CHANNELS]={
  A8,A9,A10,A11,A12,A13};//Here I specify I want to listen to pins A8 to A13 of my mega
#elif NUM_DIGITAL_PINS ==55 //this is Teensy 4.1 all pins are ok
#define NUM_RC_CHANNELS 6 //You need to specify how many pins you want to use
const uint8_t RC_Channel_Pin[NUM_RC_CHANNELS] =
{ 41, 37, 36, 40,38 ,39 };//Connect the rc module pwm pins to these pins
#endif

#include "RCLib.h"

RCLib rcLib;


void setup(){
	Serial.begin(11500);
	rcLib.setup();
	rcLib.setDelayBetweenReads(500);
}

void loop(){
	if(rcLib.loop()){
		//new data has arrived
		//uint16_t* channelValues =rcLib.getChannelValues();
		for(int curChannel=0;curChannel<rcLib.getNumChannels();curChannel++){
		  Serial.print(RC_Channel_Value[curChannel]);
		  Serial.print(" ");
		}
		  Serial.println();
	}
}
