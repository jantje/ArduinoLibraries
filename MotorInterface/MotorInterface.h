/*
 * MotorInterface.h
 *
 *  Created on: Jul 4, 2014
 *      Author: jan
 */
#pragma once
#include "SerialDataInterface.h"
#include "DataTypes.h"

#ifdef USE_MAIN_LOOP_MILLIS
   extern uint32_t loopMillis;
#endif


class MotorInterface
{
	protected:
		SPEED_TYPE myRequestedSpeed;
		SPEED_TYPE myNewRequestedSpeed;
		SPEED_TYPE myActualSpeed;
		uint16_t myMaxTurnCurrent_cA;
		bool myIsDirty;
		uint16_t myCurrentUsage_cA;
		uint32_t myLastSpeedChangeTime;

	public:
		virtual void loop()=0;
		virtual void setup()=0;
		virtual void motorOn()=0;
		virtual void motorOff()=0;

		/**
		 * EmergencyBreak stops the drive engines directly powering the coils
		 * To avoid powerspikes the power is gradually decreased
		 * This method waits untill the motor is stopped
		 * The method returns true when the attached motors are stalled
		 */
		virtual bool emergencyBreak()=0;

		/**
		 * The last time the speed has changed.
		 */
		uint64_t getLastSpeedChangeTime()
		{
			return myLastSpeedChangeTime;
		}


		/**
		 * returns wether the motor is running at the requested speed.
		 * As we do not have a speed meter we wqait a while after we requested the speed. (slow start)
		 */
		boolean isAtSpeed() const
		{
			return ((myRequestedSpeed == myActualSpeed) && (loopMillis - myLastSpeedChangeTime > 10000));
		}
		;

		SPEED_TYPE getActualSpeed() const
		{
			return myActualSpeed;
		}
		bool isDirty()
		{
			return myIsDirty;
		}

		/*
		 * SetSpeed changes the speed of the motor.
		 * The speed is changed in small steps.
		 */
		void setRequestedSpeed(int16_t Speed)
		{
			myNewRequestedSpeed = Speed;
			myIsDirty = true;
		}

		int16_t getRequestedSpeed() const
		{
			return myNewRequestedSpeed;
		}

		uint16_t getCurrentUsage_cA() const
		{
			return myCurrentUsage_cA;
		}

		virtual ~MotorInterface()=default;
#ifdef I_USE_SERIAL_REGISTER
		virtual void serialRegister(const __FlashStringHelper* Name)
		{
			FieldData::set(Name, F("RequestedSpeed"),MOD_OVERVIEW , &myRequestedSpeed);
			FieldData::setNext( F("NewRequestedSpeed"), MOD_WRITE, &myNewRequestedSpeed);
			FieldData::setNext( F("LastSpeedChangeTime"), MOD_NONE, &myLastSpeedChangeTime);

			FieldData::setNext( F("Max_current_to_switchDir_cA"), MOD_WRITE|MOD_SAVE, &myMaxTurnCurrent_cA);
#ifdef DETAILED_SERIAL_LOGGING
				FieldData::setNext( F("IsDirty"), 0, &myIsDirty);
				//	FieldData::setNext( F("IsAtSpeed"), 0, &myIsAtSpeed);
				FieldData::setNext( F("CurrentUsage_cA"), 0, &myCurrentUsage_cA);
#endif
			}
#endif
		};

