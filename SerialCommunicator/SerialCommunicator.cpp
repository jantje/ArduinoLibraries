/*
 * MessageHandler.cpp
 *
 *  Created on: Nov 28, 2012
 *      Author: BE04258
 */
#include "SerialCommunicator.h"
#include "SerialStringReader.h"
#include "watchdog.h"

#if ARDUINO<158
#error This code needs Arduino IDE 1.5.8 or later
#endif

const char LOG_VALUE[] PROGMEM ="LOG VALUE";
const char LOG_HEADER[] PROGMEM ="LOG HEADER";
const char DUMP[] PROGMEM ="DUMP";
const char NAME_NOT_FOUND[] PROGMEM="Field not found ";
const char NAME_TO_LONG[] PROGMEM ="Name to long ";
const char RESET[] PROGMEM ="RESET";

const char DONE[] PROGMEM="done";
const char SET[] PROGMEM ="SET ";
const char GET[] PROGMEM ="GET ";

const char setkeyValue[] PROGMEM = "setkeyValueCommand ";
const char getkeyValue[] PROGMEM = "getkeyValueCommand ";

uint8_t SerialCommunicator::myLogLevel;         // The Log Level used
uint32_t SerialCommunicator::mylastLog;
SerialStringReader SerialCommunicator::myStringSerial; //the class to read string from the serial monitor
uint16_t SerialCommunicator::myLogDelay;        //The time to wait after a log has been done

uint32_t SerialCommunicator::myLoopCounter;  //Counts the number of times loop has been called
uint32_t SerialCommunicator::myMillis; //to show the millis since startup
uint16_t SerialCommunicator::myAveragebetweenLoops; //The average millis between loop counts
uint16_t SerialCommunicator::myMaxbetweenLoops; //The maximum millis between loop counts
uint32_t SerialCommunicator::myLoopduration; //the duration of the loop
uint32_t SerialCommunicator::myLogduration; //the duration of the last log
int16_t SerialCommunicator::mySerialQueueSize; //The size of the stream queue
//#ifdef I_USE_RESET
//uint8_t SerialCommunicator::myResetPin = 0;  //The pin used to rest Arduino
//uint16_t SerialCommunicator::myResetDelay = 800;//The delay before a reset is actioned
//#endif

static void logValueVisitor(FieldData& fieldData)
{
	SerialOutput.print(fieldData.getValue(commonlyUsedBuffer, commonlyUsedBuffersize));
	SerialOutput.print(FIELDSEPERATOR);
}
static void logHeaderVisitor(FieldData& fieldData)
{
	SerialOutput.print(fieldData.myClassName);
	SerialOutput.print(CLASSSEPERATOR);
	SerialOutput.print(fieldData.myFieldName);
	SerialOutput.print(FIELDSEPERATOR);
}
static void dumpVisitor(FieldData& fieldData)
{
	/*
	 * If you get an error here about wdt_reset not defined and you are building for teensy
	 * add the following to .../Arduino/hardware/teensy/avr/cores/teensyXX/avr/wdt.h
	 * where XX is your teensy version
	 *
	 * #ifndef __WDT_H__
   * #define __WDT_H__
   * void wdt_reset()
   * {
   * #warning "wdt_reset is not implemented"
   * }
   * #endif
   *
	 */
	wdt_reset(); //make sure the watch dog does not trigger
	fieldData.dump();
}

static void dumpWritableData(FieldData& fieldData)
{
	if ((fieldData.myModFlag&MOD_WRITE)!=MOD_WRITE)return;
	SerialOutput.print((__FlashStringHelper *) SET);
	SerialOutput.print(fieldData.myClassName);
	SerialOutput.print(CLASSSEPERATOR);
	SerialOutput.print(fieldData.myFieldName);
	SerialOutput.print('=');
	SerialOutput.println(fieldData.getValue(commonlyUsedBuffer, commonlyUsedBuffersize));
}

static void dumpCommands()
{
	/*
	 * If you run out of program space you can save by comenting this verboze proza
	 * That will make you can not query the commands but it also means you have more
	 * program space
	 * */
SerialOutput.println(F("Following commands are supported"));
SerialOutput.println(F("? to show this info"));
SerialOutput.println(F("DUMP full memory dump"));
SerialOutput.println(F("GET [Name] 1 field memory dump"));
SerialOutput.println(F("SET dump all the set commands"));
SerialOutput.println(F("SET [Field]=[value] set value of field"));
SerialOutput.println(F("LOG_VALUE LOG all the values"));
SerialOutput.println(F("LOG HEADER LOSerialOutput.he names"));
}

//#ifdef I_USE_RESET
///*
// * Don't fiddle with this code as it is very likely things won't work anymore
// */
//void SerialCommunicator::ForceHardReset()
//{
//	delay(myResetDelay);
//	pinMode(myResetPin, OUTPUT); //this sets the pin to low one way or another
//	//digitalWrite(myResetPin,LOW);
//	digitalWrite(myResetPin,HIGH);
//	SerialOutput.print(F("Did you connect pin "));
//	SerialOutput.print(myResetPin);
//	SerialOutput.println(F(" to reset?"));
//	delay(10000);//needs some delay but if it fails you want to be able to try again
//
//}
//#endif

void SerialCommunicator::logValue()
{
	SerialOutput.print((__FlashStringHelper *) LOG_VALUE);
	SerialOutput.print(FIELDSEPERATOR);
	FieldData::visitAllFields(logValueVisitor, true);
	SerialOutput.println();
}
/*
 * This method does the repetitive task of the class.
 * This method should be called from the loop method.
 */
void SerialCommunicator::loop()
{
	myMillis = millis();
	static uint32_t myPrefLoop = 0;
	static uint32_t myStartTime = millis();
	uint32_t curloopTime = myMillis - myPrefLoop;
	if (myLoopCounter != 0)
	{
		myAveragebetweenLoops = (myMillis - myStartTime) / (myLoopCounter);
		myMaxbetweenLoops = max(myMaxbetweenLoops, curloopTime);
	}
	myLoopCounter++;
	myPrefLoop = myMillis;

	myStringSerial.loop();
	if (myStringSerial.messageReceived())
	{
		SerialOutput.println(myStringSerial.getMessage());
		setReceivedMessage(myStringSerial.getMessage());
	}
	if ((myLogLevel & 1) == 1)
	{


		if (((millis() - mylastLog) >= myLogDelay))
		{
			mylastLog = millis();
			/*
			 * If you get an error below it means that the arduino stream class needs to be modified.
			 * add the following to stream.h after the Available method
			 * virtual int availableForWrite(void){return 0;};
			 */
			int availableForWrite = SerialOutput.availableForWrite();
			if (availableForWrite == mySerialQueueSize) //only when the buffer is completely empty transmit data
			{
				uint32_t logstart = millis();
				logValue();
				myLogduration = millis() - logstart;
			} else
			{
        //The below output is tight for me
				// the reason is that you do not want to put load on the stream
				SerialOutput.print(F("log Fail have:"));
				SerialOutput.print(availableForWrite);
				SerialOutput.print(F(" need:"));
				SerialOutput.println(mySerialQueueSize);
			}
		}
	}
	myLoopduration = millis() - myMillis;

}

/*
 * This method parses a incoming string (the message is expected to be complete)
 * Depending on the instruction the action is undertaken.
 */
void SerialCommunicator::setReceivedMessage(const char* newMessage)
{
	if (strcmp_P(newMessage, DUMP) == 0)
	{
		dumpAllFields();
	} else if (strncmp_P(newMessage, GET, 4) == 0)
	{
		SerialOutput.println((__FlashStringHelper *) GET);
		FieldData *fieldData = FieldData::findField(newMessage + 4);
		if (fieldData != 0) fieldData->dump();
	} else if (strncmp_P(newMessage, SET, 4) == 0)
	{
		SerialOutput.println((__FlashStringHelper *) SET);
		FieldData *fp = FieldData::findField(newMessage + 4);
		if (fp != 0)
		{
			fp->setValue(newMessage + 4 + strlen_P((const char *) fp->myClassName) + strlen_P((const char *) fp->myFieldName) + 2);
			fp->dump();
		}
	} else if (strncmp_P(newMessage, SET, 3) == 0)
	{
		SerialOutput.println((__FlashStringHelper *) SET);
		FieldData::visitAllFields(dumpWritableData, true);
	} else if (strcmp_P(newMessage, LOG_HEADER) == 0)
	{
		SerialOutput.print((__FlashStringHelper *) LOG_HEADER);
		SerialOutput.print(FIELDSEPERATOR);
		FieldData::visitAllFields(logHeaderVisitor, true);
		SerialOutput.println();
		return;
	} else if (strcmp_P(newMessage, LOG_VALUE) == 0)
	{
		logValue();
		return;
//#ifdef I_USE_RESET
//	} else if (strcmp_P(newMessage, RESET) == 0)
//	{
//		ForceHardReset();
//#endif
	} else
	{
		dumpCommands();
		if ('?' != newMessage[0])
		{
			SerialOutput.print((__FlashStringHelper *) ERROR);
			//Even though it is handy to see what has been send
			//The yun bootloader sends press ard to stop bootloader
			//echoing this means the bootloader receives ard
			//SerialOutput.println(newMessage);
		}
		return;
	}
	SerialOutput.println((__FlashStringHelper *) DONE);
}




void SerialCommunicator::dumpAllFields()
{
	SerialOutput.println(F("Dumping all fields"));
	SerialOutput.print(F("SketchName\t"));
	SerialOutput.println((__FlashStringHelper *) mySketchName);
	SerialOutput.print(F("CompileDate\t"));
	SerialOutput.println(F(__DATE__));
	FieldData::visitAllFields(dumpVisitor, true);
}
//#ifdef I_USE_RESET
//SerialCommunicator::SerialCommunicator(uint8_t resetPin)
//{
//	myResetPin = resetPin;
//	myResetDelay = 300;
//}
//#else
SerialCommunicator::SerialCommunicator()
{
//#endif
	myLogLevel = 1;
	myLogDelay = 1000;
	myLoopCounter = 0; /*Counts the number of times loop has been called*/
	myAveragebetweenLoops = 0;/*The average millis between loop counts*/
	myMaxbetweenLoops = 0;/*The maximum millis between loop counts*/

}

void SerialCommunicator::serialRegister(const __FlashStringHelper* Name)
{
	FieldData::set(Name, F("logLevel"),MOD_WRITE | MOD_SAVE, &myLogLevel);
	FieldData::setNext( F("DelaybetweenLogs"), MOD_WRITE | MOD_SAVE, &myLogDelay);
	FieldData::setNext( F("LoopCounter"), MOD_NONE, &myLoopCounter);
	FieldData::setNext( F("millis"), MOD_OVERVIEW, &myMillis);
	FieldData::setNext( F("Avg_Loop"), MOD_NONE, &myAveragebetweenLoops);
	FieldData::setNext( F("Max_Loop"), MOD_NONE, &myMaxbetweenLoops);
	FieldData::setNext((__FlashStringHelper *) LOOPDURATION, MOD_NONE, &myLogduration);
	FieldData::setNext( F("last_log_duration"), MOD_NONE, &myLogduration);
//#ifdef I_USE_RESET
//	FieldData::set(Name, F("ResetPin"), 0, &myResetPin);
//	FieldData::set(Name, F("ResetDelay"),MOD_WRITE | MOD_SAVE, &myResetDelay);
//#endif
}

void SerialCommunicator::setup()
{
	mySerialQueueSize=0;
	int16_t oldSerialQueueSize=0;
	do
		{
		delay(200);//wait 200 ms in that time interval the queue should have been processed a bit
		oldSerialQueueSize =mySerialQueueSize;
		mySerialQueueSize=	SerialOutput.availableForWrite();
		}
	while (mySerialQueueSize!=oldSerialQueueSize);
	SerialOutput.print("SerialQueueSize =");
	SerialOutput.println(mySerialQueueSize);

	myStringSerial.setup();
	mylastLog = millis();
}

void waitForYunToBoot()
{
	pinMode(7,INPUT_PULLUP);
	delay(20);
	while(digitalRead(7)==HIGH) //wait for yun to startup
		{
    // The pin is still high, so give the LED a quick flash to show we're waiting.
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN,LOW);
    delay(100);
		}
}
