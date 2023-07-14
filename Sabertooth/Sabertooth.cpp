/*
Arduino Library for SyRen/Sabertooth Packet Serial
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

#include "Sabertooth.h"

Sabertooth::Sabertooth(byte address)
  : _address(address), _port(SabertoothTXPinSerial)
{
  
}

Sabertooth::Sabertooth(byte address, SabertoothStream& port)
  : _address(address), _port(port)
{

}

void Sabertooth::autobaud(boolean dontWait) const
{
  autobaud(port(), dontWait);
}

void Sabertooth::autobaud(SabertoothStream& port, boolean dontWait)
{
  if (!dontWait) { delay(1500); }
  port.write(0xAA);
#if defined(ARDUINO) && ARDUINO >= 100
  port.flush();
#endif
  if (!dontWait) { delay(500); }
}

void Sabertooth::command(byte command, byte value) const
{
  port().write(address());
  port().write(command);
  port().write(value);
  port().write((address() + command + value) & B01111111);
}

void Sabertooth::throttleCommand(byte command, int power) const
{
  power = constrain(power, -126, 126);
  this->command(command, (byte)abs(power));
}

void Sabertooth::motor(int power) const
{
  motor(1, power);
}

void Sabertooth::motor(byte motor, int power) const
{
  if (motor < 1 || motor > 2) { return; }
  throttleCommand((motor == 2 ? 4 : 0) + (power < 0 ? 1 : 0), power);
}

void Sabertooth::drive(int power) const
{
  throttleCommand(power < 0 ? 9 : 8, power);
}

void Sabertooth::turn(int power) const
{
  throttleCommand(power < 0 ? 11 : 10, power);
}

void Sabertooth::stop() const
{
  motor(1, 0);
  motor(2, 0);
}

void Sabertooth::setMinVoltage(byte value) const
{
  command(2, (byte)min(value, 120));
}

void Sabertooth::setMaxVoltage(byte value) const
{
  command(3, (byte)min(value, 127));
}

void Sabertooth::setBaudRate(long baudRate) const
{
#if defined(ARDUINO) && ARDUINO >= 100
  port().flush();
#endif

  byte value;
  switch (baudRate)
  {
  case 2400:           value = 1; break;
  case 9600: default: value = 2; break;
  case 19200:          value = 3; break;
  case 38400:          value = 4; break;
  case 115200:         value = 5; break;
  }
  command(15, value);
  
#if defined(ARDUINO) && ARDUINO >= 100
  port().flush();
#endif
  
  // (1) flush() does not seem to wait until transmission is complete.
  //     As a result, a Serial.end() directly after this appears to
  //     not always transmit completely. So, we manually add a delay.
  // (2) Sabertooth takes about 200 ms after setting the baud rate to
  //     respond to commands again (it restarts).
  // So, this 500 ms delay should deal with this.
  delay(500);
}

void Sabertooth::setDeadband(byte value) const
{
  command(17, (byte)min(value, 127));
}

void Sabertooth::setRamping(byte value) const
{
  command(16, (byte)constrain(value, 0, 80));
}

void Sabertooth::setTimeout(int milliseconds) const
{
  command(14, (byte)((constrain(milliseconds, 0, 12700) + 99) / 100));
}
