// Set Minimum Voltage Sample for Packet Serial
// Copyright (c) 2012 Dimension Engineering LLC
// See license.txt for license details.

// The values in this sample are specifically for the Sabertooth 2x25, and may
// not have the same effect on other models.
#include <Sabertooth.h>

Sabertooth ST(128);

void setup()
{
  SabertoothTXPinSerial.begin(9600);
  ST.autobaud();

  // This setting does not persist between power cycles.
  // See the Packet Serial section of the documentation for what values to use
  // for the minimum voltage command. It may vary between Sabertooth models
  // (2x25, 2x60, etc.).
  //
  // On a Sabertooth 2x25, the value is (Desired Volts - 6) X 5.
  // So, in this sample, we'll make the low battery cutoff 12V: (12 - 6) X 5 = 30.
  ST.setMinVoltage(30);
}

void loop()
{
  ST.motor(1, 50);
  delay(5000);
  
  ST.motor(1, -50);
  delay(5000);
}

