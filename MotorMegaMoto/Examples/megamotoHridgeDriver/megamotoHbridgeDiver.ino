

#include "MotorMegaMoto.h"

const char mySketchName[] = "MegaMoto tester";
Stream &SerialInput=Serial;
Stream &SerialOutput=Serial;
Stream &SerialError=Serial;

#define ENGINE_ENABLE_PIN  8

#define PWMA_PIN  6
#define PWMB_PIN  5
#define CURRENT_SENSOR_PIN A4

//DriveEngine MyDriveEngine(RIGHT_ENGINE_ENABLE_PIN, RIGHT_ENGINE_PWMA_PIN, RIGHT_ENGINE_PWMB_PIN, LEFT_ENGINE_ENABLE_PIN, LEFT_ENGINE_PWMA_PIN, LEFT_ENGINE_PWMB_PIN);

//Mower MyMower(MOWER_ENABLE_PIN, LEFT_MOW_ENGINE_PIN, RIGHT_MOW_ENGINE_PIN);

MotorMegaMoto MyEngine( ENGINE_ENABLE_PIN, PWMA_PIN, PWMB_PIN, CURRENT_SENSOR_PIN, 255);

#define OutputFile Serial




void setup()
{
	analogReference(DEFAULT);
	//delay(2000);
	Serial.begin(115200);
	Serial.println( "MegaMoto tester Starting Up");
	MyEngine.motorOn();

}

void loop()
{
	static int CurMowSpeed = 0;
	MyEngine.loop();

	if (Serial.available()) //data received see what we need to do
	{
		char incomingByte = Serial.read(); // read the incoming byte:
		switch (toupper(incomingByte))
		{

			case 'Z':
				Serial.println( "Received Z Full ahead");
				MyEngine.setRequestedSpeed(255);
				break;
			case 'X':
				Serial.println( "Received X full backwards");
				MyEngine.setRequestedSpeed(-255);
				break;
			case 'U':
				Serial.println( "Received speed Up");
				MyEngine.setRequestedSpeed(constrain( MyEngine.getRequestedSpeed()+1,-255,255));
				break;

			case 'D':
				Serial.println( "Received speed Down");
				MyEngine.setRequestedSpeed(constrain( MyEngine.getRequestedSpeed()-1,-255,255));
				break;
			case 'S':
				Serial.println( "Received S stopping");
				MyEngine.setRequestedSpeed(0);
				break;
			default:
				Serial.print( "Arduino received: '");
				Serial.print( incomingByte);
				Serial.println("'. This is not supported. Send ");
				Serial.println( "Z/z to go Full ahead ");
				Serial.println( "X/x to go Full backwards ");
				Serial.println( "U/u speed up");
				Serial.println( "D/d speed Down");
				Serial.println( "S/s to stop ");

				break;
		}

	}


}
