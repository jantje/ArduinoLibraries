/*
 * ModBusClientInterface.h
 *
 *  Created on: Oct 17, 2026
 *      Author: jan
 */

#pragma once
#include "Arduino.h"
#define MAX_NUMBER_OF_REGISTERS 100


class ModBusSlaveInterface
{
private :
	uint8_t myChannelID;
	uint32_t myWatchDogTime=1000;
	public:
		ModBusSlaveInterface(uint8_t channelID){myChannelID=channelID;}
		virtual ~ModBusSlaveInterface(){}
		virtual void loop()=0;
		virtual void setup()=0;
		virtual void responseOnLastMessage(uint8_t numRegisters, uint16_t registerValues[])=0;
		virtual uint8_t getChannelID(){return myChannelID;};
		/**
		 * Return true if you want to send a FC3 to the client
		 * The parameters are for you to return the startAddress and the numberOfRegister to read
		 */
		virtual bool hasFC3ReadHoldingRegistersRequest(uint16_t *startAddress,uint16_t *numberOfRegister)=0;

		virtual bool hasFC6WriteSingleRegisterRequest(uint16_t *address,uint16_t *value)=0;

		virtual bool hasFC16WriteMultipleRegisterRequest(uint16_t *startAddress,uint16_t *NumberOfValues,uint16_t values[MAX_NUMBER_OF_REGISTERS])=0;

		/**
		 * The last send has failed.
		 * Do you want to retry?
		 * Do not always answer yes as that will block everything
		 */
		virtual bool retryFailedSend()=0;

		uint32_t getWatchDogTime(){return myWatchDogTime;};
};
