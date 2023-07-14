// Shared Line Sample for Packet Serial
// Copyright (c) 2012 Dimension Engineering LLC
// See license.txt for license details.

#include <Sabertooth.h>

// Up to 8 Sabertooth/SyRen motor drivers can share the same S1 line.
// This sample uses three: address 128 and 129 on ST1[0] and ST1[2],
// and address 130 on ST2.
Sabertooth ST1[2] = { Sabertooth(128), Sabertooth(129) };
Sabertooth ST2(130);

void setup()
{
  SabertoothTXPinSerial.begin(9600);
  Sabertooth::autobaud(SabertoothTXPinSerial); // Autobaud is for the whole serial line -- you don't need to do
                                               // it for each individual motor driver. This is the version of
                                               // the autobaud command that is not tied to a particular
                                               // Sabertooth object.
                                               // See the examples in 1.Basics for information on whether you
                                               // need this line at all.
}

void loop()
{
  // ST1[0] (address 128) has power 50 (of 127 max) on M1,
  // ST1[1] (address 129) has power 60 (of 127 max) on M2, and
  // ST2    (address 130) we'll do tank-style and have it drive 20 and turn right 50.
  // Do this for 5 seconds.
  ST1[0].motor(1, 50);
  ST1[1].motor(2, 60);
  ST2.drive(20);
  ST2.turn(50);
  delay(5000);
  
  // And now let's stop for 5 seconds, except address 130 -- we'll let it stop and turn left...
  ST1[0].motor(1, 0);
  ST1[1].motor(2, 0);
  ST2.drive(0);
  ST2.turn(-40);  
  delay(5000);
}

