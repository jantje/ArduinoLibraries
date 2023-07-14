// Tank-Style Sweep Sample for Packet Serial
// Copyright (c) 2012 Dimension Engineering LLC
// See license.txt for license details.

#include <Sabertooth.h>

// Mixed mode is for tank-style diff-drive robots.

Sabertooth ST(128); // The Sabertooth is on address 128. We'll name its object ST.
                    // If you've set up your Sabertooth on a different address, of course change
                    // that here. For how to configure address, etc. see the DIP Switch Wizard for
                    //   Sabertooth - http://www.dimensionengineering.com/datasheets/SabertoothDIPWizard/start.htm
                    //   SyRen      - http://www.dimensionengineering.com/datasheets/SyrenDIPWizard/start.htm
                    // Be sure to select Packetized Serial Mode for use with this library.
                    //
                    // This sample uses the mixed mode (diff-drive) commands, which require two motors
                    // and hence do not work on a SyRen.
                    //
                    // In this sample, hardware serial TX connects to S1.
                    // See the SoftwareSerial example in 3.Advanced for how to use other pins.

void setup()
{
  SabertoothTXPinSerial.begin(9600); // 9600 is the default baud rate for Sabertooth packet serial.
  ST.autobaud(); // Send the autobaud command to the Sabertooth controller(s).
                 // NOTE: *Not all* Sabertooth controllers need this command.
                 //       It doesn't hurt anything, but V2 controllers use an
                 //       EEPROM setting (changeable with the function setBaudRate) to set
                 //       the baud rate instead of detecting with autobaud.
                 //
                 //       If you have a 2x12, 2x25 V2, 2x60 or SyRen 50, you can remove
                 //       the autobaud line and save yourself two seconds of startup delay.
             
  ST.drive(0); // The Sabertooth won't act on mixed mode packet serial commands until
  ST.turn(0);  // it has received power levels for BOTH throttle and turning, since it
               // mixes the two together to get diff-drive power levels for both motors.
}

// The SLOW ramp here is turning, and the FAST ramp is throttle.
// If that's the opposite of what you're seeing, swap M2A and M2B.
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
  // This way, our turning will have a radius.
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
