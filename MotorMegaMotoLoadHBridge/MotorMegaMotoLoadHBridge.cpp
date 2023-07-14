/*
 * MegaMotoLoadHBridgeDriver.cpp
 *
 *  Created on: May 24, 2012
 *      Author: BE04258
 */
#include "MegaMoto.h"
#include "Motor_MegaMoto.h"
#include "MegaMotoHBridgeDriver.h"
#include "MegaMotoLoadHBridgeDriver.h"

//#ifdef SHARED_ENABLEPIN
MegaMotoLoadHBridgeDriver::MegaMotoLoadHBridgeDriver( ENABLEPINPARAM uint8_t PWMAPin, uint8_t PWMBPin, uint8_t CurrentPin, uint8_t StandardVoltage,
		uint8_t MaxVoltage, uint16_t MaxDurationAtMaxVoltage, int CurrentTrigger)
		: myMegaMotoHBridgeDriver( ENABLEPINATTRIBUTE PWMAPin, PWMBPin, CurrentPin,CurrentPin)
//#else
//MegaMotoLoadHBridgeDriver::MegaMotoLoadHBridgeDriver(const __FlashStringHelper* Name, uint8_t EnablePin, uint8_t PWMAPin, uint8_t PWMBPin, uint8_t CurrentPin, uint8_t StandardVoltage, uint8_t MaxVoltage, uint16_t MaxDurationAtMaxVoltage, int CurrentTrigger,uint8_t Voltage)
//: MegaMotoHBridgeDriver(Name, EnablePin, PWMAPin, PWMBPin, CurrentPin, Voltage)
//#endif

{
	myRequestedSpeed = 0;
	myStandardPWM = StandardVoltage;
	myMaxPWM = MaxVoltage;
	myMaxDurationAtMaxPWM = MaxDurationAtMaxVoltage;
	myCurrentTriggerToGoToMaxPWM = CurrentTrigger;
	myMaxPWMStartTime = 0;
	myLoopDuration = 0;
	myBusinessAction[0] = 0;


}

void MegaMotoLoadHBridgeDriver::serialRegister(const __FlashStringHelper* Name,const __FlashStringHelper* DriverName,const __FlashStringHelper* BrigeAName,const __FlashStringHelper* BrigeBName)
{
	FieldData::set(Name, F("RequestedSpeed"), MOD_WRITE | MOD_SAVE, &myRequestedSpeed);
	FieldData::set(Name, F("StandardPWM"), MOD_WRITE | MOD_SAVE, &myStandardPWM);
	FieldData::set(Name, F("MaxPWM"), MOD_WRITE | MOD_SAVE, &myMaxPWM);
	FieldData::set(Name, F("MaxDurationAtMaxPWM"), MOD_WRITE | MOD_SAVE, &myMaxDurationAtMaxPWM);
	FieldData::set(Name, F("CurrentToGoToMaxPWM"), MOD_WRITE | MOD_SAVE, &myCurrentTriggerToGoToMaxPWM);
	FieldData::set(Name, F("MaxPWMStartTime"), 0, &myMaxPWMStartTime);
	FieldData::set(Name, F("LoopDuration"), 0,&myLoopDuration);
	FieldData::set(Name, F("BusinessAction"),MOD_ERASE_ON_DUMP, myBusinessAction);


	myMegaMotoHBridgeDriver.serialRegister(   DriverName,  BrigeAName,  BrigeBName);
}

void MegaMotoLoadHBridgeDriver::loop()
{
	unsigned long StartTime = millis();
	if (myStandardPWM < myMegaMotoHBridgeDriver.getActualSpeed()) //we are providing more than standard voltage
	{
		strlcat(myBusinessAction, "Elevate Power; ",BUSINESSACTIONSIZE);
		if ((millis() -myMaxPWMStartTime) >= myMaxDurationAtMaxPWM)  //we have been to long applying extra power
		{
			strlcat(myBusinessAction, "should end ",BUSINESSACTIONSIZE);
			if (myMegaMotoHBridgeDriver.getCurrentUsage() > myCurrentTriggerToGoToMaxPWM) // we still use to much power
			{
				strlcat(myBusinessAction, "doing EmergencyBreak ",BUSINESSACTIONSIZE);
				myMegaMotoHBridgeDriver.emergencyBreak(false); //stop the engine
				myMegaMotoHBridgeDriver.setRequestedSpeed(0); //make sure we stay still
			} else
			{
				strlcat( myBusinessAction, "Going to standard power ",BUSINESSACTIONSIZE);
				//MegaMotoHBridgeDriver::SetSpeed(myRequestedSpeed,true);
				setSpeed(myRequestedSpeed);
			}
		}
	} else  // we are providing standard power
	{
		if (myMegaMotoHBridgeDriver.getCurrentUsage() > myCurrentTriggerToGoToMaxPWM) //We use to much power
		{
			strlcat( myBusinessAction , "To much current ",BUSINESSACTIONSIZE);
			if (!myMegaMotoHBridgeDriver.isDirty()) // we are in stable mode
			{
				strlcat( myBusinessAction , "Going to max power ",BUSINESSACTIONSIZE);
				myMegaMotoHBridgeDriver.setRequestedSpeed(SIGN(myRequestedSpeed) * (int16_t) myMaxPWM);
				myMaxPWMStartTime = millis();
			} else
			{
				strlcat( myBusinessAction , " but wait because is dirty ",BUSINESSACTIONSIZE);
			}
		}
	}
	myMegaMotoHBridgeDriver.loop();

	myLoopDuration = millis() - StartTime;
}

void MegaMotoLoadHBridgeDriver::setSpeed(int16_t Speed)
{
	myRequestedSpeed = Speed;
	myMegaMotoHBridgeDriver.setRequestedSpeed(map(Speed, 0, 255, 0, myStandardPWM));
}

int16_t MegaMotoLoadHBridgeDriver::getRequestedSpeed()
{
	return map(myMegaMotoHBridgeDriver.getRequestedSpeed(), 0, myStandardPWM, 0, 255);
}

