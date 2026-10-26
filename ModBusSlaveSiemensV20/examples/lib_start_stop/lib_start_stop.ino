/*
 * sample controlling one motor controlled via V20 inverter via modbus protocol
 * This program uses the library to communicate with the V20

 * This code assumes a factory reset V20 with the connection macro cn011 enabled (see page 81 of the v20 instruction manual)
 * You need to increase the watchdog time (p2014) or the V2 will fault (At least I think so
 * And following connections between your RS485 breakout (I used grove) and V20
 * gnd to gnd
 * A to P+
 * B to P-
 * (No extra resistor needed for short lines in a "lab" environment)
 * If there are problems:
 * Have a short as possible wires between your RS485 and V20
 * Check parameters r2023 to r2031
 *
 * Note
 * 1) the documentation is inconsistent in regards to the high and low bit of the crc
 * All values are high low but crc is low high.
 * 2) the registers you can read/write are not the same as the parameters.
 * You can find the registers on page 191 use the values in the converter column not the MODBUS column:-(
 * 3)if you do not get response messages from the sV20 Check parameters r2023 to r2031 on the converter
 * 4) If you use sloeber and created a project based on a library delete the library folder as this code is not using the library
 * 5) adjust the serial port and device address below to your setup
 */
#include "Arduino.h"
#include "MotorModBusSiemensV20.h"

#define DEVICE_ADDRESS 1 //The number you told the V20 is on the bus (default 1)

MotorModBusSiemensV20 myMotor(DEVICE_ADDRESS, Serial5);

const char mySketchName[] =
		"Serial control Siemens V20 inverter starting and stopping via lib";

void setup() {
	delay(500); //allow some time for me to control the serial monitor and handle the V20 error state
	Serial.begin(115200);
	Serial.println(mySketchName);
	Serial5.begin(9600, SERIAL_8E1);
	myMotor.setup();
}

void loop() {
	static int status = 1;
	static uint32_t LastChange = 0;
	myMotor.loop();
	if (millis() - LastChange > 5000) {
		LastChange = millis();
		switch (status) {
		case 1:
			Serial.println("Setting speed to 100");
			myMotor.motorOn();
			myMotor.setRequestedSpeed(100);
			status = 2;
			break;
		case 2:
			Serial.println("Stopping motor");
			myMotor.motorOff();
			status = 3;
			break;
		case 3:
			Serial.println("Setting speed to -100");
			myMotor.setRequestedSpeed(-100);
			myMotor.motorOn();
			status = 4;
			break;
		case 4:
			Serial.println("Stopping motor");
			myMotor.motorOff();
			status = 1;
			break;
		}
	}

}
