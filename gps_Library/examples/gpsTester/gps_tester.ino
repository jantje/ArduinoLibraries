#include "gps_Library.h"
#define GPSBAUTRATE 9600


#if defined(I_USE_SOFTWARESERIAL) || defined(I_USE_ALTSOFTSERIAL)
#define GPSTX_PIN 13
#define GPSRX_PIN 5
SERIALTYPE myGPSserial(GPSTX_PIN, GPSRX_PIN);
#else
#define myGPSserial Serial1
#endif
#define MYSERIAL Serial1

GPSModule MyGpsModule(GPSBAUTRATE,&myGPSserial);

//The setup function is called once at startup of the sketch
void setup()
{
	delay(3000); //needed to be able to reboot both linino and leonardo.
	MYSERIAL.begin(115200); // Set the baud.

   // Wait for U-boot to finish startup.  Consume all bytes until we are done.
  do {
     while (MYSERIAL.available() > 0) {
    	 MYSERIAL.read();
        }

    delay(1000);
  } while (MYSERIAL.available()>0);

  MYSERIAL.println("\r\nGPSlogger");

  MYSERIAL.println("initializing gps");
	MyGpsModule.setup();
	MYSERIAL.println("initializing gps is done ");
}


char buffer[100];
void loop()
{
	static uint32_t lastLog=0;
	static DateTime lastTime;
	DateTime CurTime;
	MyGpsModule.loop();
	CurTime = MyGpsModule.getLastMessageTimeStamp();
	if (millis()-lastLog>30000)
	{
		lastTime = CurTime;
		MYSERIAL.println(CurTime.toString(buffer,100));
		MYSERIAL.print("Has reception ");
		MYSERIAL.println(MyGpsModule.hasReception());
		MYSERIAL.print("Latitude ");
		MYSERIAL.print(MyGpsModule.getLastMessageLocation().myLatitude);
		MYSERIAL.print(" Longitude ");
		MYSERIAL.println(MyGpsModule.getLastMessageLocation().myLongitude);
		MYSERIAL.print("Speed ");
		MYSERIAL.println(MyGpsModule.getLastMessageSpeed());
		MYSERIAL.print("Direction ");
		MYSERIAL.println(MyGpsModule.getLastMessageDirection());
		MYSERIAL.print("Buffer ");
		MYSERIAL.println(MyGpsModule.getBuffer());
		lastLog=millis();
	}


}
