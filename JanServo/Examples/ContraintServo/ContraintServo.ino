/**
 * A sample program demonstrating the behavior of the JanConstraintServo class
 * 3 Servos are sweeping with exactly the same values in the code but with different physical movement.
 * 1 servo uses the Arduino Servo class and will use the full sweeping range
 * 2 servos use the JanConstraintServo class and will not use the full sweeping range. One is constraint in the max value the other in the min value.
 * The 3th servo uses the standard Servo class
 * The 2 servo's do behave in sync. With which I mean that the servo with the smaller range does not stop moving when nearing the ned of its range.
 */
#include "Arduino.h"
#include "JanServo.h"


/* defines for positioning Servo's*/
#define STOP_POSITIE 90

#define SERVO_1_MIN_POSITIE 45
#define SERVO_2_MIN_POSITIE 0

#define SERVO_1_MAX_POSITIE 180
#define SERVO_2_MAX_POSITIE 135


/* defines for the pins */
#define SERVO_1_PIN 20
#define SERVO_2_PIN 21
#define SERVO_3_PIN 22



JanConstraintServo servo1(SERVO_1_PIN,SERVO_1_MIN_POSITIE,SERVO_1_MAX_POSITIE,STOP_POSITIE);
JanConstraintServo servo2(SERVO_2_PIN,SERVO_2_MIN_POSITIE,SERVO_2_MAX_POSITIE,STOP_POSITIE);
Servo servo3;



void setup()
{
  servo1.setup();
  servo2.setup();
  servo3.attach(SERVO_3_PIN);
  servo3.write(STOP_POSITIE);

}

void loop() {
	static int curPosition=0;
	static int step=1;
	servo1.SetPosition(curPosition);
	servo2.SetPosition(curPosition);
	servo3.write(curPosition);
	servo1.loop();
	servo2.loop();
	//calculate the new position
	curPosition=curPosition+step;
	if (curPosition>180)
	{
		curPosition=179;
		step=-1;
	}
	if(curPosition<0)
	{
		curPosition=1;
		step=1;
	}
	delay(500); //Only use delay is samples to make the code more readable
}

