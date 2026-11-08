/*
 * SkidDriversaberTooth.cpp
 *
 *  Created on: 11-mei-2016
 *      Author: jan
 */
#include "SkidDriverModBusSiemensV20.h"

SkidDriverModBusSiemensV20::SkidDriverModBusSiemensV20()
{
	myIsMotorOn = false;
	mySabertoothspeed = 0;
	mySabertoothDirection = 0;
}

SkidDriverModBusSiemensV20::~SkidDriverModBusSiemensV20()
{
}

void SkidDriverModBusSiemensV20::motorOn()
{
	myIsMotorOn = true;
}

void SkidDriverModBusSiemensV20::motorOff()
{
	myIsMotorOn = false;
}

void SkidDriverModBusSiemensV20::setup()
{
}

void SkidDriverModBusSiemensV20::loop()
{
	if ((myNewRequestedDirection != myRequestedDirection) || (myNewRequestedSpeed != myRequestedSpeed))
	{
		myRequestedDirection = myNewRequestedDirection * SIGN(myNewRequestedSpeed);
		myRequestedSpeed = myNewRequestedSpeed;
		mySabertoothspeed = myRequestedSpeed / 2;  //maps {-255, 255} to  {-127, 127}
		mySabertoothDirection = map(myRequestedDirection, -90, 90, -127, 127);
//		myPSabertooth->drive(mySabertoothspeed);
//		myPSabertooth->turn(mySabertoothDirection);

	}
}

bool SkidDriverModBusSiemensV20::emergencyBreak(bool immediatlyReturn)
{
//	myPSabertooth->stop();
	myNewRequestedDirection = myRequestedDirection = myNewRequestedSpeed = myRequestedSpeed = 0;
	return true;
}

void SkidDriverModBusSiemensV20::turnOnTheSpot(SPEED_TYPE Speed)
{
//	myPSabertooth->drive(0);
//	myPSabertooth->turn(map(myRequestedDirection, -90, 90, -255, 255));
}

SPEED_TYPE SkidDriverModBusSiemensV20::getActualSpeed() const
{
	return myRequestedSpeed;
}

#ifdef I_USE_SERIAL_REGISTER
void SkidDriverModBusSiemensV20::serialRegister(const __FlashStringHelper* Name)
{

	FieldData::set(Name, F("RequestedSpeed"),MOD_OVERVIEW,&myRequestedSpeed);
	FieldData::set(Name, F("RequestedDirection"),MOD_OVERVIEW,&myRequestedDirection);
	FieldData::set(Name, F("mySabertoothspeed"),MOD_NONE,&mySabertoothspeed);
	FieldData::set(Name, F("mySabertoothDirection"),MOD_NONE,&mySabertoothDirection);

}
#endif
