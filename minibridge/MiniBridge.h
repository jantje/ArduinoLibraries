/*
 * MiniBridge.h
 *
 *  Created on: Jan 27, 2014
 *      Author: jan
 */
#ifdef I_USE_MINI_BRIDGE
#ifndef MINIBRIDGE_H_
#define MINIBRIDGE_H_
#include "Arduino.h"


extern HardwareSerial &BridgeSerial;

class MiniBridge
{
		  public:
		MiniBridge(){max_retries=0;index=0; started=false;CRC=0;};
    static void begin();


    //void put(const char *key, const char *value);

    //unsigned int get(const char *key, uint8_t *buff, unsigned int size);

    static uint16_t transfer(const uint8_t *buff1, uint16_t len1,
                      const uint8_t *buff2 , uint16_t len2,
                      const uint8_t *buff3 , uint16_t len3,
                      uint8_t *rxbuff , uint16_t rxlen   ) ;


    static const unsigned int TRANSFER_TIMEOUT = 0xFFFF;

  private:
    static uint8_t index;
    static uint16_t CRC;
    static const char CTRL_C;
    static bool started;
    static uint8_t max_retries;
    static int timedRead(unsigned int timeout);
    static void dropAll();



    static void crcUpdate(uint8_t c);
    static void crcReset(){CRC = 0xFFFF;};
    static void crcWrite();
    static bool crcCheck(uint16_t _CRC){
    	return CRC == _CRC;
    };


};

extern MiniBridge Bridge;
#endif /* MINIBRIDGE_H_ */
#endif // I_USE_MINI_BRIDGE
