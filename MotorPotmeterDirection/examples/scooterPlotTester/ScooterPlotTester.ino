/**
 * very simple program to plot the behavior of the scooter motor
 * I'm using http://www.dfrobot.com/wiki/index.php?title=50A_Current_Sensor%28SKU:SEN0098%29 to sense the current
 * and as digital pot http://floris.cc/shop/en/ic/533-digital-potentiometer-10k.html
 *
 * This code is written for a Mega but should be easily adjusted for other arduino compatible boards
 */
#include "ScooterMotor.h"
#include "simplot.h"
#include "MCP413.h"
#define clockwisePin  48

#define  ampPin A7
uint8_t slavePin =49;
MCP413 digitalPot(127, slavePin,true);
//AmpMeter myAmpMeter(ampPin);
AmpMeter myAmpMeter(ampPin,10000,0);


class plotMotor:public ScooterMotor
{
	public:
		void plot()
		{
			plot4(Serial,myRequestedSpeed,abs(myActualSpeed),(mySpinsClockWise?LOW:HIGH)*255,myCurrentUsage_cA);
		};
		plotMotor():ScooterMotor(clockwisePin,  myAmpMeter,digitalPot)
		{
			//nothing to do here
		};
};

plotMotor myMotor;
void setup()
{
	SPI.begin();
	Serial.begin(115200);
	Serial.println("Scooter motor plotter tester");
	Serial.println("Open scope to see the plot data");
	myMotor.setup();
	myMotor.setRequestedSpeed(0);
	Serial.println("Speed = 0");
}

#define DEFAULTDELAY 2000
void loop()
{
	static int state = 0;
	static uint32_t lastAction = millis();
	switch (state)
	{
		case 0:
			if (millis() - lastAction > 3000)
			{
				lastAction = millis();
				state++;
				myMotor.setRequestedSpeed(255);
				Serial.println("Speed = 255");
			}
			break;
		case 1:
			if (millis() - lastAction > DEFAULTDELAY)
			{
				lastAction = millis();
				state++;
				myMotor.setRequestedSpeed(-255);
				Serial.println("Speed = -255");
			}
			break;
		case 2:
			if (millis() - lastAction > DEFAULTDELAY)
			{
				lastAction = millis();
				state++;
				myMotor.setRequestedSpeed(255);
				Serial.println("Speed = 255");
			}
			break;
		case 3:
			if (millis() - lastAction > DEFAULTDELAY)
			{
				lastAction = millis();
				state++;
				myMotor.setRequestedSpeed(0);
				Serial.println("Speed = 0");
			}
			break;
		case 4:
			if (millis() - lastAction > DEFAULTDELAY)
			{
				lastAction = millis();
				state++;
				myMotor.setRequestedSpeed(255);
				Serial.println("Speed = 255");
			}
			break;
		case 5:
			if (millis() - lastAction > DEFAULTDELAY)
			{
				lastAction = millis();
				state++;
				myMotor.setRequestedSpeed(0);
				Serial.println("Speed = 0");
			}
			break;
		case 6:
			if (millis() - lastAction > DEFAULTDELAY)
			{
				lastAction = millis();
				state++;
				myMotor.setRequestedSpeed(-255);
				Serial.println("Speed = -255");
			}
			break;
		case 7:
			if (millis() - lastAction > DEFAULTDELAY)
			{
				lastAction = millis();
				state++;
				myMotor.setRequestedSpeed(0);
				Serial.println("Speed = 0");
			}
			break;
		default:
			state=0;
//			if (millis() - lastAction > DEFAULTDELAY)
//			{
//				while(true);//stop the arduino
//			}
			break;
	}
	myMotor.loop();
	myMotor.plot();
	delay(20);
}
