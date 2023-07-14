// Tank-Style Sweep Sample
// Copyright (c) 2012 Dimension Engineering LLC
// See license.txt for license details.

#include <SabertoothSimplified.h>

// Mixed mode is for tank-style diff-drive robots.
// Only Packet Serial actually has mixed mode, so this Simplified Serial library
// emulates it (to allow easy switching between the two libraries).

SabertoothSimplified ST; // We'll name the Sabertooth object ST.
                         // For how to configure the Sabertooth, see the DIP Switch Wizard for
                         //   http://www.dimensionengineering.com/datasheets/SabertoothDIPWizard/start.htm
                         // Be sure to select Simplified Serial Mode for use with this library.
                         // This sample uses a baud rate of 9600.
                         //
                         // Connections to make:
                         //   Arduino TX->1  ->  Sabertooth S1
                         //   Arduino GND    ->  Sabertooth 0V
                         //   Arduino VIN    ->  Sabertooth 5V (OPTIONAL, if you want the Sabertooth to power the Arduino)
                         //
                         // If you want to use a pin other than TX->1, see the SoftwareSerial example.

void setup()
{
  SabertoothTXPinSerial.begin(9600); // This is the baud rate you chose with the DIP switches.             
  
  ST.drive(0); // The Sabertooth won't act on mixed mode until
  ST.turn(0);  // it has received power levels for BOTH throttle and turning, since it
               // mixes the two together to get diff-drive power levels for both motors.
               // So, we set both to zero initially.
}

// Mixed mode tips:
//   drive() should go forward and back, turn() should go right and left.
//     If this is reversed, swap M2A and M2B.
//   Positive on drive() should go forward, negative should go backward.
//     If this is reversed, swap A and B on both M1 and M2.
//   Positive on turn() should go right, negative should go left.
//     If this is reversed, swap M1 and M2.

// In this sample, the SLOW sweep (left-to-right) here is turning,
// and the FAST sweep (backwards-to-forwards) is throttle.
void loop()
{
  int power;
  
  // Don't turn. Ramp from going backwards to going forwards, waiting 20 ms (1/50th of a second) per value.
  for (power = -127; power <= 127; power ++)
  {
    ST.drive(power);
    delay(20);
  }
  
  // Now, let's use a power level of 20 (out of 127) forward.
  // This way, our turning will have a radius. Mostly, the command
  // is just to demonstrate you can use drive() and turn() at the same time.
  ST.drive(20);
  
  // Ramp turning from full left to full right SLOWLY by waiting 50 ms (1/20th of a second) per value.
  for (power = -127; power <= 127; power ++)
  {
    ST.turn(power);
    delay(50);
  }
  
  // Now stop turning, and stop driving.
  ST.turn(0);
  ST.drive(0);
  
  // Wait a bit. This is so you can catch your robot if you want to. :-)
  delay(5000);
}
