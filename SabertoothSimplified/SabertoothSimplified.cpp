/*
Arduino Library for Sabertooth Simplified Serial
Copyright (c) 2012-2013 Dimension Engineering LLC
http://www.dimensionengineering.com/arduino

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE
USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "SabertoothSimplified.h"

SabertoothSimplified::SabertoothSimplified()
  : _port(SabertoothTXPinSerial)
{
  
}

SabertoothSimplified::SabertoothSimplified(Print& port)
  : _port(port)
{
  
}

void SabertoothSimplified::motor(int power)
{
  motor(1, power);
}

void SabertoothSimplified::motor(byte motor, int power)
{
  mixedMode(false);
  raw(motor, power);  
}

void SabertoothSimplified::drive(int power)
{
  mixedMode(true);
  _mixedDrive = constrain(power, -127, 127);
  _mixedDriveSet = true;
  mixedUpdate();
}

void SabertoothSimplified::turn(int power)
{
  mixedMode(true);
  _mixedTurn = constrain(power, -127, 127);
  _mixedTurnSet = true;
  mixedUpdate();
}

void SabertoothSimplified::stop()
{
  _port.write((uint8_t)0);
  _mixedDriveSet = false;
  _mixedTurnSet = false;
}

void SabertoothSimplified::mixedMode(boolean enable)
{
  if (_mixed == enable) { return; }
  
  stop();
  _mixed = enable;
}

void SabertoothSimplified::mixedUpdate()
{
  if (!_mixedDriveSet || !_mixedTurnSet) { return; }
  raw(1, _mixedDrive - _mixedTurn);
  raw(2, _mixedDrive + _mixedTurn);
}

void SabertoothSimplified::raw(byte motor, int power)
{
  byte command, magnitude;
  power = constrain(power, -127, 127);
  magnitude = abs(power) >> 1;
  
  if (motor == 1)
  {
    command = power < 0 ? 63 - magnitude : 64 + magnitude;
  }
  else if (motor == 2)
  {
    command = power < 0 ? 191 - magnitude : 192 + magnitude;
  }
  
  command = constrain(command, 1, 254);
  _port.write(command);
}
