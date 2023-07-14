/*
 * MegaMotoLoadHBridgeDriver.h
 *
 *  Created on: May 24, 2012
 *      Author: BE04258
 */

#ifndef MEGAMOTOLOADHBRIDGEDRIVER_H_
#define MEGAMOTOLOADHBRIDGEDRIVER_H_
#include "MegaMotoHBridgeDriver.h"
#include "SerialDataInterface.h"
#define BUSINESSACTIONSIZE 30

class MegaMotoLoadHBridgeDriver
{
	private:
		MegaMotoHBridgeDriver myMegaMotoHBridgeDriver;
		uint8_t myStandardPWM; //The pwm used when we get 255 to run
		uint8_t myMaxPWM;      // the power used when we see more than current power is used
		uint16_t myMaxDurationAtMaxPWM; // The maximum time we use MyMaxPower
		uint16_t myCurrentTriggerToGoToMaxPWM;         // The current we use to trigger (current is more than this) to go MyMaxPower
		uint32_t myMaxPWMStartTime;
		uint32_t myLoopDuration;
		int16_t myRequestedSpeed;
		char myBusinessAction[BUSINESSACTIONSIZE];



	public:
		virtual ~MegaMotoLoadHBridgeDriver(){};
		MegaMotoLoadHBridgeDriver( ENABLEPINPARAM uint8_t PWMAPin, uint8_t PWMBPin,uint8_t CurrentPin , uint8_t StandardVoltage, uint8_t MaxVoltage,uint16_t MaxDurationAtMaxVoltage,int CurrentTrigger);
		void serialRegister(const __FlashStringHelper* Name,const __FlashStringHelper* DriverName,const __FlashStringHelper* BrigeAName,const __FlashStringHelper* BrigeBName);
		void loop();
		virtual void setSpeed(int16_t Speed);
		virtual int16_t getRequestedSpeed();

		void setStandardPWM( uint8_t newStandardPWM)
		{	myStandardPWM=newStandardPWM;};
		uint8_t getStandardPWM()
		{	return myStandardPWM;};
		bool emergencyBreak(bool immediatlyReturn)
		{	return myMegaMotoHBridgeDriver.emergencyBreak(immediatlyReturn);};
//		void setSuppliedVoltage(uint8_t Voltage)
//		{	myMegaMotoHBridgeDriver.setSuppliedVoltage( Voltage);}
		void engineOn(){myMegaMotoHBridgeDriver.engineOn();};
		void engineOff(){myMegaMotoHBridgeDriver.engineOff();};
	};

#endif /* MEGAMOTOLOADHBRIDGEDRIVER_H_ */
