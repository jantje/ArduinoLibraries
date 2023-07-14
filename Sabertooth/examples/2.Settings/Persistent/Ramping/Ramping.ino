// Set Ramping Sample for Packet Serial
// Copyright (c) 2012 Dimension Engineering LLC
// See license.txt for license details.

// WARNING: This sample makes changes that will persist between restarts AND in all modes.
#include <Sabertooth.h>

Sabertooth ST(128);

void setup()
{
  SabertoothTXPinSerial.begin(9600);
  ST.autobaud();
  
  // See the Sabertooth 2x60 documentation for information on ramping values.
  // There are three ranges: 1-10 (Fast), 11-20 (Slow), and 21-80 (Intermediate).
  // The ramping value 14 used here sets a ramp time of 4 seconds for full
  // forward-to-full reverse.
  //
  // 0 turns off ramping. Turning off ramping requires a power cycle.
  //
  // WARNING: The Sabertooth remembers this command between restarts AND in all modes.
  // To change your Sabertooth back to its default, call ST.setRamping(0)
  ST.setRamping(14);
}

void loop()
{
  // Full forward, both motors.
  ST.motor(1, 127);
  ST.motor(2, 127);
  delay(5000);
  
  // Full reverse
  ST.motor(1, -127);
  ST.motor(2, -127);
  delay(5000);
}

