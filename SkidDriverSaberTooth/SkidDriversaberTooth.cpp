/*
 * SkidDriversaberTooth.cpp
 *
 *  Created on: 11-mei-2016
 *      Author: jan
 */

#include <SkidDriversaberTooth.h>

SkidDriverSaberTooth::SkidDriverSaberTooth(Sabertooth* pSabertooth)
{
	myIsMotorOn = false;
	myPSabertooth = pSabertooth;
	mySabertoothspeed = 0;
	mySabertoothDirection = 0;
}

SkidDriverSaberTooth::~SkidDriverSaberTooth()
{
}

void SkidDriverSaberTooth::motorOn()
{
	myIsMotorOn = true;
}

void SkidDriverSaberTooth::motorOff()
{
	myIsMotorOn = false;
}

void SkidDriverSaberTooth::setup()
{
	myPSabertooth->autobaud(); // Send the autobaud command to the Sabertooth controller(s).
	// NOTE: *Not all* Sabertooth controllers need this command.
	//       It doesn't hurt anything, but V2 controllers use an
	//       EEPROM setting (changeable with the function setBaudRate) to set
	//       the baud rate instead of detecting with autobaud.
	//
	//       If you have a 2x12, 2x25 V2, 2x60 or SyRen 50, you can remove
	//       the autobaud line and save yourself two seconds of startup delay.

	myPSabertooth->drive(0); // The Sabertooth won't act on mixed mode packet serial commands until
	myPSabertooth->turn(0); // it has received power levels for BOTH throttle and turning, since it
	// mixes the two together to get diff-drive power levels for both motors.
}

void SkidDriverSaberTooth::loop()
{
	if ((myNewRequestedDirection != myRequestedDirection) || (myNewRequestedSpeed != myRequestedSpeed))
	{
		myRequestedDirection = myNewRequestedDirection * SIGN(myNewRequestedSpeed);
		myRequestedSpeed = myNewRequestedSpeed;
		mySabertoothspeed = myRequestedSpeed / 2;  //maps {-255, 255} to  {-127, 127}
		mySabertoothDirection = map(myRequestedDirection, -90, 90, -127, 127);
		myPSabertooth->drive(mySabertoothspeed);
		myPSabertooth->turn(mySabertoothDirection);

	}
}

bool SkidDriverSaberTooth::emergencyBreak(bool immediatlyReturn)
{
	myPSabertooth->stop();
	myNewRequestedDirection = myRequestedDirection = myNewRequestedSpeed = myRequestedSpeed = 0;
	return true;
}

void SkidDriverSaberTooth::turnOnTheSpot(SPEED_TYPE Speed)
{
	myPSabertooth->drive(0);
	myPSabertooth->turn(map(myRequestedDirection, -90, 90, -255, 255));
}

SPEED_TYPE SkidDriverSaberTooth::getActualSpeed() const
{
	return myRequestedSpeed;
}

void SkidDriverSaberTooth::serialRegister(const __FlashStringHelper* Name)
{

	FieldData::set(Name, F("RequestedSpeed"),MOD_OVERVIEW,&myRequestedSpeed);
	FieldData::set(Name, F("RequestedDirection"),MOD_OVERVIEW,&myRequestedDirection);
	FieldData::set(Name, F("mySabertoothspeed"),MOD_NONE,&mySabertoothspeed);
	FieldData::set(Name, F("mySabertoothDirection"),MOD_NONE,&mySabertoothDirection);

}
