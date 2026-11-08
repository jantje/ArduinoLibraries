#include "Brains.h"

Brains brains;

#ifdef ARDUINO_AVR_MEGA2560
#define NUM_RC_CHANNELS 6 //You need to specify how many pins you want to use
#include "PinChangeInt.h"  //If you need pinchangeint you need to include this header
const uint8_t RC_Channel_Pin[NUM_RC_CHANNELS]={
  A8,A9,A10,A11,A12,A13};//Here I specify I want to listen to pins A8 to A13 of my mega
#elif defined( ARDUINO_TEENSY41)
#define NUM_RC_CHANNELS 6 //You need to specify how many pins you want to use
const uint8_t RC_Channel_Pin[NUM_RC_CHANNELS] =
{ 36, 37, 38, 39,40 ,41 };//Connect the rc module pwm pins to these pins
#endif

//now the rc controll is defined lets define the modbus stuff
#include "ModBusSlaveSiemensV20.h"
#include "ModBusMaster.h"

ModBusSlaveSiemensV20 modBusMotor_1=   ModBusSlaveSiemensV20(1,"motor 1");
#if NUM_MOTORS>1
ModBusSlaveSiemensV20 modBusMotor_2=   ModBusSlaveSiemensV20(2,"Motor 2");
#if NUM_MOTORS>2
ModBusSlaveSiemensV20 modBusMotor_3=   ModBusSlaveSiemensV20(3,"Motor 3");
#endif
#endif

#if NUM_MOTORS==1
ModBusMaster modBusMaster(MOD_BUS_SERIAL,modBusMotor_1);
#endif
#if NUM_MOTORS==2
ModBusMaster modBusMaster(MOD_BUS_SERIAL,modBusMotor_1,modBusMotor_2);
#endif
#if NUM_MOTORS==3
ModBusMaster modBusMaster(MOD_BUS_SERIAL,modBusMotor_1,modBusMotor_2,modBusMotor_3);
#endif




void setup(){
	delay(1000);//give the computer/user some time to open the serial monitor
	Serial.begin(115200);
	Serial.println("starting modbus Motor demo program");
	brains.setup();
	Serial.println("brains are ready to roll");

	modBusMotor_1.setup();
	Serial.println("Motor 1 is ready to roll");
#if NUM_MOTORS>1
	modBusMotor_2.setup();
	Serial.println("Motor 2 is ready to roll");
#if NUM_MOTORS>2
	modBusMotor_3.setup();
	Serial.println("Motor 3 is ready to roll");
#endif
#endif

	//I have to do this here (and not in ModBusMaster.setup
	//because on Teensy the hardware and software serial
	//highest mutual class is Stream which does not know Begin
	MOD_BUS_SERIAL.begin(MOD_BUS_BAUD_RATE, SERIAL_8E1);
	//modBusMaster needs to know this because timing is sometimes
	// defined in "character run time"
	modBusMaster.setBaudRate(MOD_BUS_BAUD_RATE);
	modBusMaster.setup();
}

void loop(){
	brains.loop();

	modBusMotor_1.loop();
#if NUM_MOTORS>1
	modBusMotor_2.loop();
#if NUM_MOTORS>2
	modBusMotor_3.loop();
#endif
#endif
	modBusMaster.loop();

}
