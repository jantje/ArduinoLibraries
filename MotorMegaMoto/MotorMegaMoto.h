/*
 * MegaMotoDriver.h
 *
 *  Created on: 30-sep.-2011
 *      Author: be4258
 */

#ifndef MEGAMOTODRIVER_H_
#define MEGAMOTODRIVER_H_
#include "MotorInterface.h"
#include "HalfBridge.h"

#ifdef SHARED_ENABLEPIN
#define ENABLEPINPARAM
#define ENABLEPINNAME
#define ENABLEPIN
#define ENABLEPINATTRIBUTE
#define ENABLEPINNAMELEFT
#define ENABLEPINPARAMLEFT
#define ENABLEPINATTRIBUTELEFT
#define ENABLEPINLEFT

#else
#define ENABLEPINNAME EnablePin
#define ENABLEPINPARAM uint8_t EnablePin ,
#define ENABLEPINATTRIBUTE EnablePin ,
#define ENABLEPIN				uint8_t EnablePin;

#define ENABLEPINNAMELEFT EnablePinLeft
#define ENABLEPINPARAMLEFT uint8_t ENABLEPINNAMELEFT ,
#define ENABLEPINATTRIBUTELEFT ENABLEPINNAMELEFT ,
#define ENABLEPINLEFT				uint8_t ENABLEPINNAMELEFT;

#endif

// MegaMotoDriver class allows you to control a megamoto on the high level.
// Changes in speed are not done directly to avoid voltage peaks.
// Do not use this class if you want to use the mega moto as a H-Bridge but use MegaMotoHBridgeDriver
class MotorMegaMoto: public MotorInterface
{
	public:

		MotorMegaMoto( ENABLEPINPARAM uint8_t PWMAPin, uint8_t PWMBPin, uint8_t ACurrentPin, uint8_t BCurrentPin);
		void serialRegister(const __FlashStringHelper* Name,const __FlashStringHelper* BrigeAName,const __FlashStringHelper* BrigeBName);
		private:
		ENABLEPIN
		public:
		HalfBridge myHalfBridgeA;
		HalfBridge myHalfBridgeB;
		protected:
		uint8_t myRequestedSpeedA; //The speed requested for A bridge
				uint8_t myRequestedSpeedB;//The speed requested for B bridge

				public:
				virtual int16_t getActualSpeed()const
				{
					return max(myHalfBridgeA.getActualSpeed(),myHalfBridgeB.getActualSpeed());
				}

				virtual void loop(); // Add Loop to your main arduino sketch loop
				virtual void setup()
				{	myHalfBridgeA.setup(SerialError);myHalfBridgeB.setup(SerialError);};
				virtual void motorOn();
				virtual void motorOff();
				virtual bool emergencyBreak();

				void setRequestedSpeedA(uint8_t speed)
				{
					myRequestedSpeedA = speed;
					//myHalfBridgeA.setRequestedSpeed(min(255,((uint16_t) speed *10 * (uint16_t) myMotorVoltage) / ((uint16_t) (mySuppliedVoltage*10))));
					myHalfBridgeA.setRequestedSpeed( speed );
				}

				void setRequestedSpeedB(uint8_t speed)
				{
					myRequestedSpeedB =speed;
					//myHalfBridgeB.setRequestedSpeed(min(255,( (uint16_t)speed *10 *(uint16_t)myMotorVoltage) / ((uint16_t)mySuppliedVoltage*10)));
					myHalfBridgeB.setRequestedSpeed(speed );
				}

				uint8_t getRequestedSpeedA()
				{
					return myRequestedSpeedA;
				}
				uint8_t getRequestedSpeedB()
				{
					return myRequestedSpeedB;
				}
				boolean isDirty()
				{
					return myHalfBridgeA.isDirty() || myHalfBridgeB.isDirty();
				}

				/*
				 * Calculates the power used in watt
				 * suppliedVoltage is the voltage provided to the motor in volt
				 */
				uint16_t getPowerConsumptionA(uint8_t suppliedVoltage) const
				{
					return myHalfBridgeA.calculatePower(suppliedVoltage);
				}
				/*
				 * Calculates the power used in watt
				 * suppliedVoltage is the voltage provided to the motor in volt
				 */
				uint16_t getPowerConsumptionB(uint8_t suppliedVoltage) const
				{
					return myHalfBridgeB.calculatePower(suppliedVoltage);
				}

			};

#endif /* MEGAMOTODRIVER_H_ */
