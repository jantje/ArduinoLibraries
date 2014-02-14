#include  "MiniProcess.h"

void runShellCommand(const __FlashStringHelper* command, char * retbuffer, uint8_t size)
{
strlcpy_P(commonlyUsedBuffer,(prog_char *)command,commonlyUsedBuffersize);
runShellCommand((char*) commonlyUsedBuffer, retbuffer,  size);
}

void runShellCommand(const char* command, char * retbuffer, uint8_t size)
{
	Process p;
	p.runShellCommand(command);
	int count = 0;
	while (p.available() && (count < (size - 1)))
	{
		char curchar = (char) p.read();
		if (curchar == '\n')
		{
			retbuffer[count++] = '\r';
			retbuffer[count++] = '\n';
		} else
		{
			retbuffer[count++] = curchar;
		}
	}
	if (count > 2) count -= 2;
	retbuffer[count] = 0;
//	size_t readSize = p.readBytes(retbuffer, size);
//	retbuffer[readSize+1]=0;

}


//		Process::Process(){started=false;readPos=0;};

//    unsigned int Process::runShellCommand(const char *command)
//    {
//    	close();
//      uint8_t cmd[] = {'R'};
//      uint8_t res[2];
//      Bridge.transfer(cmd, 1,(uint8_t*)"/bin/ash\xFE-c\xFE",12, (uint8_t*)command, strlen(command), res, 2);
//      handle = res[1];
//
//
//      if (res[0] == 0) // res[0] contains error code
//        started = true;
//    	  while (running())
//    	    delay(100);
//    	  return exitValue();
//    	}




//    boolean Process::running()
//    {
//      uint8_t cmd[] = {'r', handle};
//      uint8_t res[1];
//      Bridge.transfer(cmd, 2,0,0,0,0, res, 1);
//      return (res[0] == 1);
//    }

    int Process::available()
    {
      // Look if there is new data available
      doBuffer();
      return buffered;
    }


//    void Process::doBuffer() {
//      // If there are already char in buffer exit
//      if (buffered > 0)
//        return;
//
//      // Try to buffer up to 32 characters
//      readPos = 0;
//      uint8_t cmd[] = {'O', handle, sizeof(buffer)};
//      buffered = Bridge.transfer(cmd, 3,0,0,0,0, buffer, sizeof(buffer));
//    }

    int Process::read() {
      doBuffer();
      if (buffered == 0)
        return -1; // no chars available
      else {
        buffered--;
        return buffer[readPos++];
      }
    }


//    void Process::close() {
//      if (started) {
//        uint8_t cmd[] = {'w', handle};
//        Bridge.transfer(cmd, 2,0,0,0,0,0,0);
//      }
//      started = false;
//    }
//    unsigned int Process::exitValue() {
//      uint8_t cmd[] = {'W', handle};
//      uint8_t res[2];
//      Bridge.transfer(cmd, 2,0,0,0,0, res, 2);
//      return (res[0] << 8) + res[1];
//    }


