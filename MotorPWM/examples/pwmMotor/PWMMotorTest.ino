/**
 * This sketch demonstrates the usage of the PWMMotor library.
 *
 * We will start the motor and let you use the SET command to set the motor speed
 * send
 * "SET Motor.RequestedSpeed=200"+CR
 * to arduino to set the motor speed to 200
 */
#include "PWMMotor.h"
#include "SerialEEPROMCommunicator.h"

/**!!!!!!!!!!!!!!!!This section you may have to change!!!!!!!!!!! */
#define PWMPIN 10    //the pin you connect the motor to
#define STOPPWM  0   //the pwm frequency your motor is standing still (around 180) for bidirectional motors
#define DIRPIN 5     //The pin to switch direction. Use -1 if no direction pin is used
/**!!!!!!!!!!!!!!!!END OF This section you may have to change!!!!!!!!!!! */

Stream &SerialOutput = Serial;
Stream &SerialInput = Serial;
Stream &SerialError = Serial;


const char mySketchName[] PROGMEM= "PWMMotorTester";

SerialEEPROMCommunicator myCommunicator;
PWMMotor myMotor(PWMPIN,STOPPWM,DIRPIN); //This tests a one directional motor

void setup()
{
	Serial.begin(115200);
	myCommunicator.serialRegister(F("Admin"));
	myMotor.serialRegister(F("Motor"));
	myMotor.setup();
	myCommunicator.setup();

}

// The loop function is called in an endless loop
void loop()
{
	myMotor.loop();
	myCommunicator.loop();
#ifdef I_USE_PLOT
	myMotor.plot();
	delay(10);//Give the serial port some hope to catch up
#endif
}
