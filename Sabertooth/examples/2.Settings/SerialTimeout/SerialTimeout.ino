// Set Serial Timeout Sample for Packet Serial
// Copyright (c) 2012 Dimension Engineering LLC
// See license.txt for license details.

#include <Sabertooth.h>

Sabertooth ST(128);

void setup()
{
  SabertoothTXPinSerial.begin(9600);
  ST.autobaud();
  
  // setTimeout rounds up to the nearest 100 milliseconds, so this 950 will actually be 1 second.
  // A value of 0 disables the serial timeout.
  ST.setTimeout(950);
}

void loop()
{
  // Set motor 1 to reverse 20 (out of 127), and sleep for 5 seconds.
  // Notice how it cuts out after 1 second -- this is the serial timeout in action.
  // Since we configured it in setup() for 1 second, 1 second without any new
  // commands will cause the motors to stop.
  ST.motor(1, -20);
  delay(5000);
  
  // Why do this?
  // If the S1 wire gets cut for some reason, or if your program crashes,
  // the Sabertooth will stop receiving commands from the Arduino.
  // With a timeout, your robot will stop. So, it's a safety feature mostly.
}

