#ifndef _MEGA_MOTO_SKID_DRIVER_H_
#define _MEGA_MOTO_SKID_DRIVER_H_
#include <SkidDriverinterface.h>
#include "MotorMegaMoto.h"
#include "MotorMegaMotoHBridge.h"

class SkidDriverMegaMoto: public SkidDriverInterface {
private:
//		MegaMotoLoadHBridgeDriver myLeftEngines;
//		MegaMotoLoadHBridgeDriver myRightEngines;
#ifdef UNITTEST
public:
#endif
	MotorMegaMotoHBridge myLeftMotor;
	MotorMegaMotoHBridge myRightMotor;

	uint16_t myMaxCurrent;  //The maximum current to be "normal"
	uint16_t myMaxCurrentDuration; //The time we allow the current to be highr than the max current in millies
	uint32_t myLoopduration;

	boolean myUseWheelCorrection;
	int myCurrentDirectionCorrection;
	int8_t myPrefLeftCountPinState;
	int8_t myPrefRightCountPinState;
	int16_t myLeftCount;
	int16_t myRightCount;
	uint8_t myLeftCountPin;
	uint8_t myRightCountPin;
	uint32_t myLastDirectionModification;

	void calculateSpeedAndDirection();

public:

	virtual void motorOn();
	virtual void motorOff();
	SkidDriverMegaMoto( ENABLEPINPARAM uint8_t RightPWMAPin,
			uint8_t RightPWMBPin, ENABLEPINPARAMLEFT uint8_t LeftPWMAPin,
			uint8_t LeftPWMBPin, uint8_t ACurrentPin, uint8_t BCurrentPin);
	void serialRegister(const __FlashStringHelper* Name,const __FlashStringHelper*LeftDriverName,const __FlashStringHelper* LeftBrigeAName,const __FlashStringHelper* LeftBrigeBName,const __FlashStringHelper* RightDriverName,const __FlashStringHelper* RightBrigeAName,const __FlashStringHelper* RightBrigeBName);

		virtual void setup();
		virtual void loop();

		bool emergencyBreak( bool immediatlyReturn = false);

		virtual const __FlashStringHelper *          getOverloadedWheelName();

		virtual void turnOnTheSpot(SPEED_TYPE Speed);


		virtual SPEED_TYPE getActualSpeed() const{return (myLeftMotor.getActualSpeed()+myRightMotor.getActualSpeed())/2;};

};

#endif

