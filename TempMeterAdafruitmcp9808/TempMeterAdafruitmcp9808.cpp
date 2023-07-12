/*
 * adafruitmcp9808.cpp
 *
 *  Created on: May 12, 2015
 *      Author: jan
 */

#include <TempMeterAdafruitmcp9808.h>
#include "Wire.h"
#define MCP9808_I2CADDR_DEFAULT        0x18
#define MCP9808_REG_CONFIG             0x01

#define MCP9808_REG_CONFIG_SHUTDOWN    0x0100
#define MCP9808_REG_CONFIG_CRITLOCKED  0x0080
#define MCP9808_REG_CONFIG_WINLOCKED   0x0040
#define MCP9808_REG_CONFIG_INTCLR      0x0020
#define MCP9808_REG_CONFIG_ALERTSTAT   0x0010
#define MCP9808_REG_CONFIG_ALERTCTRL   0x0008
#define MCP9808_REG_CONFIG_ALERTSEL    0x0002
#define MCP9808_REG_CONFIG_ALERTPOL    0x0002
#define MCP9808_REG_CONFIG_ALERTMODE   0x0001

#define MCP9808_REG_UPPER_TEMP         0x02
#define MCP9808_REG_LOWER_TEMP         0x03
#define MCP9808_REG_CRIT_TEMP          0x04
#define MCP9808_REG_AMBIENT_TEMP       0x05
#define MCP9808_REG_MANUF_ID           0x06
#define MCP9808_REG_DEVICE_ID          0x07


uint16_t TempMeterAdafruitmcp9808::read16(uint8_t reg) {
  uint16_t val;

  Wire.beginTransmission(myAddress);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();

  Wire.requestFrom((uint8_t)myAddress, (uint8_t)2);
  val = Wire.read();
  val <<= 8;
  val |= Wire.read();
  return val;
}


void TempMeterAdafruitmcp9808::setup()
{
  Wire.begin();
  myIsError =(read16(MCP9808_REG_MANUF_ID) != 0x0054);
  myIsError &= (read16(MCP9808_REG_DEVICE_ID) != 0x0400);
  if(myIsError)
  {
  	myCentiCelsius=ERROR_CENTI_CELSIUS;
  }
}

void TempMeterAdafruitmcp9808::loop()
{
	if(!myIsError)
	{
	  uint16_t t = read16(MCP9808_REG_AMBIENT_TEMP);

	  float temp = t & 0x0FFF;
	  temp /=  16.0;
	  if (t & 0x1000) temp -= 256;

	  myCentiCelsius= temp*100;
	}
}
