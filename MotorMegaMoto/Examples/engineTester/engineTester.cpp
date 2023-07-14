// Do not remove the include below
#include "engineTester.h"
#include "MotorMegaMoto.h"
#include "TempMeterTemperatureSensetiveResistor.h"
#include "simplot.h"
#include "brains.h"
#include "SerialCommunicator.h"
#include "PwmFrequency.h"

#define MAXSAMPLES 500
#define MAXSETUPS 5
int samples[MAXSAMPLES + 1][MAXSETUPS];
#define REPORTDELAY 5000  //delay when reporting state in non active states like coooling down
#define PLOTDELAY 1000  //The time between 2 plots
#define EXTRACOOLTIME   (300UL * 1000UL) //the time to cool after the engine is cold enough
#define Testing 0
#define CoolingDown 1
#define gotoTesting 2
#define GotoCoolingDown 3
#define GotoEndState 4
#define EndState 5
int state = gotoTesting;



#define MYENABLEPIN 8
#define PWMAPin 9
#define PWMBPin 10
#define CurrentPinA A1
#define CurrentPinB A2
#define HeatPin A3


const char  mySketchName[] PROGMEM= "engineTester";

MotorMegaMoto myTestMotor( MYENABLEPIN, PWMAPin, PWMBPin, CurrentPinA, CurrentPinB);
TempMeterTemperatureSensetiveResistor heatSensor( HeatPin);
TempMeterInterface & myHeatSensor=heatSensor;
brains mybrains;
Stream &SerialInput = Serial;
Stream &SerialOutput = Serial;
Stream &SerialError = Serial;
uint32_t loopMillis;

static int curMotorSpeed = 0;
int setMotorSpeed(int MotorSpeed)
{
	myTestMotor.motorOn();

	int prefSpeed = curMotorSpeed;
	curMotorSpeed = MotorSpeed;
	Serial.print("Set MotorSpeed ");
	Serial.println(MotorSpeed);
	myTestMotor.setRequestedSpeed(MotorSpeed);
	return prefSpeed;
}
static int myCurrentSetup = 0;
int setPrescaler(int currentSetup)
{
	int prefCurrentSetup=myCurrentSetup;
	myCurrentSetup=		currentSetup;
	Serial.print("Setting PWM frequency prescaler to :");
	Serial.println(myCurrentSetup+1);
	if (!setPwmFrequency(PWMAPin, myCurrentSetup+1))
	{
		Serial.println("failed to set PWMAPIN");
	}
	if (!setPwmFrequency(PWMBPin, myCurrentSetup+1))
	{
		Serial.println("failed to set PWMBPIN");
	}
	return prefCurrentSetup;
}

//The setup function is called once at startup of the sketch
void setup()
{
	delay(2000);
	Serial.begin(115200);
	Serial.println("DCPWM motor tester.");
	myTestMotor.setup();
	myHeatSensor.setup();
	mybrains.setup();
	myHeatSensor.loop();
	int temp=myHeatSensor.getCelsius();
	Serial.println("Start motor temp: ");
	Serial.println(temp);
	if ( temp > 25)
	{
		Serial.println("Motor is hot. Cool down first");
		state = GotoCoolingDown;
	} else
	{
		Serial.println("Motor is cold just start testing.");
		state = gotoTesting;
	}

	setPrescaler(myCurrentSetup);

	setMotorSpeed(160);

}

void loop()
{
	static int currentSample = 0;

	static unsigned long starttime = millis();
	loopMillis=millis();

	myTestMotor.loop();
	myHeatSensor.loop();
	mybrains.loop();


	switch (state)
	{
		case gotoTesting:
			setMotorSpeed(160);
			state = Testing;
			Serial.println("gotoTesting");
			break;
		case Testing:
		{

			samples[currentSample][myCurrentSetup] = myHeatSensor.getCelsius();
			if (myHeatSensor.getCelsius() > 50)
			{
				currentSample = MAXSAMPLES;//this is bad finish this part of the test
			}

			static unsigned long prefPlot = millis();
			if (millis() - prefPlot >= PLOTDELAY  )
			{
				plot5(Serial, samples[currentSample][0], samples[currentSample][1], samples[currentSample][2], samples[currentSample][3], samples[currentSample][4]);
				prefPlot = millis();
				currentSample++;
				Serial.print('.');
			}

			if (currentSample > MAXSAMPLES)
			{
				currentSample = 0;
				myCurrentSetup += 1;
				if (myCurrentSetup<MAXSETUPS)
				{
				setPrescaler(myCurrentSetup);
				state = GotoCoolingDown;
				}
				else
				{
					state = GotoEndState;
				}
			}
			break;
		}
		case GotoCoolingDown:
			Serial.println("GotoCoolingDown");
			setMotorSpeed(0);
			starttime = millis();
			state = CoolingDown;
			break;
		case CoolingDown:
		{
			if (myHeatSensor.getCelsius() > 25)
			{
				static unsigned long prefPlot = millis();
				if (millis() - prefPlot >= REPORTDELAY )
				{
					Serial.print("Motor is cooling down to 25 Celsius and is now :");
					Serial.print(myHeatSensor.getCelsius());
					Serial.println(" Celsius");
					prefPlot = millis();
				}
				starttime = millis();
			} else
			{
				if ((millis() -starttime ) >=  EXTRACOOLTIME)
				{
					static unsigned long prefPlot = millis();
					if (millis() - prefPlot >= REPORTDELAY )
					{
						Serial.print("Waiting for motor to be really cold seconds remaining :");
						Serial.println(((starttime + EXTRACOOLTIME)-millis() ) / 1000);
						Serial.print(myHeatSensor.getCelsius());
						Serial.println(" Celsius");
						prefPlot = millis();
					}
				} else
				{
					state = gotoTesting;
				}
			}
			break;
		}
		case GotoEndState:
		{
			Serial.println("GotoEndState");
			setMotorSpeed(0);
			Serial.print("Test is done Motor is now :");
			Serial.print(myHeatSensor.getCelsius());
			Serial.println(" Celsius");
			state = EndState;
      break;
		}
		case EndState:
		{
			//nothing to do here
			break;
		}
	}

}
