// Set Maximum Voltage Sample for Packet Serial
// Copyright (c) 2012 Dimension Engineering LLC
// See license.txt for license details.

// WARNING: This sample makes changes that will persist between restarts AND in all modes.
//          The values in this sample are specifically for the Sabertooth 2x25, and may
//          not have the same effect on other models.
#include <Sabertooth.h>

Sabertooth ST(128);

void setup()
{
  SabertoothTXPinSerial.begin(9600);
  ST.autobaud();

  // See the Packet Serial section of the documentation for what values to use
  // for the maximum voltage command. It may vary between Sabertooth models
  // (2x25, 2x60, etc.).
  //
  // On a Sabertooth 2x25, the value is (Desired Volts) X 5.12.
  // In this sample, we'll cap the max voltage before the motor driver does
  // a hard brake at 14V. For a 12V ATX power supply this might be reasonable --
  // at 16V they tend to shut off. Here, if the voltage climbs above
  // 14V due to regenerative braking, the Sabertooth will go into hard brake instead.
  // While this is occuring, the red Error LED will turn on.
  //
  // 14 X 5.12 = 71.68, so we'll go with 71, cutting off slightly below 14V.
  //
  // WARNING: This setting persists between power cycles.
  ST.setMaxVoltage(71);
}

void loop()
{
  ST.motor(1, 50);
  delay(5000);
  
  ST.motor(1, -50);
  delay(5000);
}
