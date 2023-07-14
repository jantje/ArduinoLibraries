// Set Deadband Sample for Packet Serial
// Copyright (c) 2012 Dimension Engineering LLC
// See license.txt for license details.

// WARNING: This sample makes changes that will persist between restarts AND in all modes.
#include <Sabertooth.h>

Sabertooth ST(128);

void setup()
{
  SabertoothTXPinSerial.begin(9600);
  ST.autobaud();
  
  // This makes the deadband from -20 to 20 (of 127).
  // If your commands for a motor stay entirely within the deadband for more than
  // a second, the motor driver will stop the motor.
  // WARNING: The Sabertooth remembers this command between restarts AND in all modes.
  // To change your Sabertooth back to its default, call ST.setDeadband(0)
  ST.setDeadband(20);
}

void loop()
{
  // 50 is greater than 20, so the motor moves.
  ST.motor(1, 50);
  delay(5000);
  
  // 10 is NOT, so the motor does not move.
  ST.motor(1, 10);
  delay(5000);
}

