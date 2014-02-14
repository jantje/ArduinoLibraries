/*
 * MiniProcess.h
 *
 *  Created on: Jan 27, 2014
 *      Author: jan
 *       *      This is a mini implementation of the bridge process
 *      to reduce memory and program space usage
 *
 *      I want to stick as much as possible to the ariginal API but use less memory
 */
#ifdef I_USE_MINI_BRIDGE
#ifndef MINIPROCESS_H_
#define MINIPROCESS_H_
#include "Arduino.h"
#include "MiniBridge.h"
#include "DataTypes.h"

//Wrapper to run command on linux and get feedback
void runShellCommand(const char* command, char * retbuffer, uint8_t size);
void runShellCommand(const __FlashStringHelper * command, char * retbuffer, uint8_t size);

class Process
{
  public:
		Process(){started=false;readPos=0;buffered=0;handle=0;};

    unsigned int runShellCommand(const char *command)
    {
    	close();
      uint8_t cmd[] = {'R'};
      uint8_t res[2];
      Bridge.transfer(cmd, 1,(uint8_t*)"/bin/ash\xFE-c\xFE",12, (uint8_t*)command, strlen(command), res, 2);
      handle = res[1];


      if (res[0] == 0) // res[0] contains error code
        started = true;
    	  while (running())
    	    delay(100);
    	  return exitValue();
    	}
    ;




    boolean running()
        {
          uint8_t cmd[] = {'r', handle};
          uint8_t res[1];
          Bridge.transfer(cmd, 2,0,0,0,0, res, 1);
          return (res[0] == 1);
        }
    ;

    int available()
//    {
//      // Look if there is new data available
//      doBuffer();
//      return buffered;
//    }
    ;


    void doBuffer()
    {
      // If there are already char in buffer exit
      if (buffered > 0)
        return;

      // Try to buffer up to 32 characters
      readPos = 0;
      uint8_t cmd[] = {'O', handle, sizeof(buffer)};
      buffered = Bridge.transfer(cmd, 3,0,0,0,0, buffer, sizeof(buffer));
    }
;

    int read()
//    {
//      doBuffer();
//      if (buffered == 0)
//        return -1; // no chars available
//      else {
//        buffered--;
//        return buffer[readPos++];
//      }
//    }
;
  private:
    unsigned int handle;
    boolean started;
    void close()
    {
      if (started) {
        uint8_t cmd[] = {'w', handle};
        Bridge.transfer(cmd, 2,0,0,0,0,0,0);
      }
      started = false;
    }
    ;
    unsigned int exitValue()
    {
      uint8_t cmd[] = {'W', handle};
      uint8_t res[2];
      Bridge.transfer(cmd, 2,0,0,0,0, res, 2);
      return (res[0] << 8) + res[1];
    }
    ;


    uint8_t buffered;
    uint8_t readPos;
    static const int BUFFER_SIZE = 64;
    uint8_t buffer[BUFFER_SIZE];
};

#endif /* MINIPROCESS_H_ */
#endif //#ifdef I_USE_MINI_BRIDGE

