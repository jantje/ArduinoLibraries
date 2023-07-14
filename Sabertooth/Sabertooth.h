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

#ifndef Sabertooth_h
#define Sabertooth_h   

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
typedef Stream SabertoothStream;
#else
#include <WProgram.h>
typedef Print SabertoothStream;
#endif

#if defined(USBCON)
#define SabertoothTXPinSerial Serial1 // Arduino Leonardo has TX->1 on Serial1, not Serial.
#else
#define SabertoothTXPinSerial Serial
#endif
#define SyRenTXPinSerial SabertoothTXPinSerial

/*!
\class Sabertooth
\brief Controls a %Sabertooth or %SyRen motor driver running in Packet Serial mode.
*/
class Sabertooth
{
public:
  /*!
  Initializes a new instance of the Sabertooth class.
  The driver address is set to the value given, and the Arduino TX serial port is used.
  \param address The driver address.
  */
  Sabertooth(byte address);
  
  /*!
  Initializes a new instance of the Sabertooth class.
  The driver address is set to the value given, and the specified serial port is used.
  \param address The driver address.
  \param port    The port to use.
  */
  Sabertooth(byte address, SabertoothStream& port);

public:
  /*!
  Gets the driver address.
  \return The driver address.
  */
  inline byte address() const { return _address; }
  
  /*!
  Gets the serial port.
  \return The serial port.
  */
  inline SabertoothStream& port() const { return _port; }

  /*!
  Sends the autobaud character.
  \param dontWait If false, a delay is added to give the driver time to start up.
  */
  void autobaud(boolean dontWait = false) const;
  
  /*!
  Sends the autobaud character.
  \param port     The port to use.
  \param dontWait If false, a delay is added to give the driver time to start up.
  */
  static void autobaud(SabertoothStream& port, boolean dontWait = false);
  
  /*!
  Sends a packet serial command to the motor driver.
  \param command The number of the command.
  \param value   The command's value.
  */
  void command(byte command, byte value) const;
  
public:
  /*!
  Sets the power of motor 1.
  \param power The power, between -127 and 127.
  */
  void motor(int power) const;
  
  /*!
  Sets the power of the specified motor.
  \param motor The motor number, 1 or 2.
  \param power The power, between -127 and 127.
  */
  void motor(byte motor, int power) const;
  
  /*!
  Sets the driving power.
  \param power The power, between -127 and 127.
  */
  void drive(int power) const;
  
  /*!
  Sets the turning power.
  \param power The power, between -127 and 127.
  */
  void turn(int power) const;
  
  /*!
  Stops.
  */
  void stop() const;
  
public:
  /*!
  Sets the minimum voltage.
  \param value The voltage. The units of this value are driver-specific and are specified in the Packet Serial chapter of the driver's user manual.
  */
  void setMinVoltage(byte value) const;
  
  /*!
  Sets the maximum voltage.
  Maximum voltage is stored in EEPROM, so changes persist between power cycles.
  \param value The voltage. The units of this value are driver-specific and are specified in the Packet Serial chapter of the driver's user manual.
  */
  void setMaxVoltage(byte value) const;
  
  /*!
  Sets the baud rate.
  Baud rate is stored in EEPROM, so changes persist between power cycles.
  \param baudRate The baud rate. This can be 2400, 9600, 19200, 38400, or on some drivers 115200.
  */
  void setBaudRate(long baudRate) const;
  
  /*!
  Sets the deadband.
  Deadband is stored in EEPROM, so changes persist between power cycles.
  \param value The deadband value.
               Motor powers in the range [-deadband, deadband] will be considered in the deadband, and will
               not prevent the driver from entering nor cause the driver to leave an idle brake state.
               0 resets to the default, which is 3.
  */
  void setDeadband(byte value) const;
  
  /*!
  Sets the ramping.
  Ramping is stored in EEPROM, so changes persist between power cycles.
  \param value The ramping value. Consult the user manual for possible values.
  */
  void setRamping(byte value) const;
  
  /*!
  Sets the serial timeout.
  \param milliseconds The maximum time in milliseconds between packets. If this time is exceeded,
                      the driver will stop the motors. This value is rounded up to the nearest 100 milliseconds.
                      This library assumes the command value is in units of 100 milliseconds. This is true for
                      most drivers, but not all. Check the packet serial chapter of the driver's user manual
                      to make sure.
  */
  void setTimeout(int milliseconds) const;
  
private:
  void throttleCommand(byte command, int power) const;
  
private:
  const byte        _address;
  SabertoothStream& _port; 
};

#endif
