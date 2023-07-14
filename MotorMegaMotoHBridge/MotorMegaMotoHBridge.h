/*
 * MegaMotoHBridgeDriver.h
 *
 *  Created on: May 19, 2012
 *      Author: BE04258
 */

#ifndef MEGAMOTOHBRIDGEDRIVER_H_
#define MEGAMOTOHBRIDGEDRIVER_H_
#include "MotorMegaMoto.h"

class MotorMegaMotoHBridge: public MotorInterface
{
	protected:
		MotorMegaMoto myMegaMotoDriver;
		int16_t myCurrentRequestedSpeed;
		uint32_t myLoopDuration;

		/*
		 * changeSpeed changes the speed of the HBridge driven engine.
		 * The speed is changes in small steps.
		 * If the force flag is used the speed if forced immediately to the engine if the new speed is in the same
		 * direction as the current speed direction
		 * If the force flag is true and the speed direction is different from the current speed direction the
		 * engine is forced to stop (PWM 0 on both halves) and will go to the speed requested in small steps
		 *
		 */
		void changeSpeed(int16_t Speed);

	public:
		MotorMegaMotoHBridge( ENABLEPINPARAM uint8_t PWMAPin, uint8_t PWMBPin,uint8_t CurrentPinA,uint8_t CurrentPinB );
		void serialRegister(const __FlashStringHelper* DriverName,const __FlashStringHelper* BrigeAName,const __FlashStringHelper* BrigeBName);

		virtual void loop();
		virtual bool emergencyBreak()
		{	myCurrentRequestedSpeed=0; return myMegaMotoDriver.emergencyBreak();};
		//uint16_t GetCurrentUsage() const {return MyMegaMotoDriver.GetACurrentUsage()+MyMegaMotoDriver.GetBCurrentUsage();};
				uint16_t getCurrentUsage() const
				{	return (myMegaMotoDriver.myHalfBridgeA.getCurrentUsage_cA()+myMegaMotoDriver.myHalfBridgeB.getCurrentUsage_cA());};
				uint16_t getCurrentUsageA() const
				{	return myMegaMotoDriver.myHalfBridgeA.getCurrentUsage_cA();};
				uint16_t getCurrentUsageB() const
				{	return myMegaMotoDriver.myHalfBridgeB.getCurrentUsage_cA();};
				uint16_t getPowerConsumptionA(uint8_t suppliedVoltage) const
				{	return myMegaMotoDriver.getPowerConsumptionA(suppliedVoltage);};
				uint16_t getPowerConsumptionB(uint8_t suppliedVoltage) const
				{	return myMegaMotoDriver.getPowerConsumptionB(suppliedVoltage);};

				virtual void motorOn()
				{	myMegaMotoDriver.motorOn();};
				virtual void motorOff()
				{	myMegaMotoDriver.motorOff();};
				virtual void setup()
				{	myMegaMotoDriver.setup();};
			};

#endif /* MEGAMOTOHBRIDGEDRIVER_H_ */
