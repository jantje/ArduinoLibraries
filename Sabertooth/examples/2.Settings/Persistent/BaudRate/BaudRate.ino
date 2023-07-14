// Set Baud Rate Sample for Packet Serial
// Copyright (c) 2012 Dimension Engineering LLC
// See license.txt for license details.

// WARNING: This sample makes changes that will persist between restarts.
// NOTE: The setBaudRate function will only have an effect on V2 controllers (2x12, 2x25 V2, 2x60, SyRen 50).
//       Earlier controllers automatically detect the baud rate you choose in Serial.begin
//       when you call the autobaud function. Autobaud was replaced in V2 controllers for reliability
//       in the event that the Sabertooth lost power.
#include <Sabertooth.h>

Sabertooth ST(128);

void setup()
{
  // This sample will tell the Sabertooth *at 9600 baud* to *switch to 2400 baud*.
  // Keep in mind you must send the command to change the baud rate *at the baud rate
  // the Sabertooth is listening at* (factory default is 9600). After that, if it works,
  // you will be able to communicate using the new baud rate.
  //
  // Options are:
  //   2400
  //   9600
  //   19200
  //   38400
  //   115200 (only supported by some devices such as 2X60 -- check the device's datasheet)
  //
  // WARNING: The Sabertooth remembers this command between restarts.
  // To change your Sabertooth back to its default, you must *be at the baud rate you've
  // set the Sabertooth to*, and then call ST.setBaudRate(9600)
  SabertoothTXPinSerial.begin(9600);
  ST.setBaudRate(2400);
  SabertoothTXPinSerial.end();

  // OK, we're at 2400. Let's talk to the Sabertooth at that speed.
  SabertoothTXPinSerial.begin(2400);  
}

void loop()
{
  ST.drive(0);
  ST.turn(20);
  delay(2000);
  
  ST.turn(-20);
  delay(2000);
}

