/*
 * MessageHandler.cpp
 *
 *  Created on: Nov 28, 2012
 *      Author: BE04258
 */
#include "SerialCommunicator.h"
#include "SerialStringReader.h"
#include "watchdog.h"

#ifdef USE_MAIN_LOOP_MILLIS
extern uint32_t loopMillis;
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

//#ifdef I_USE_RESET
//uint8_t SerialCommunicator::myResetPin = 0;  //The pin used to rest Arduino
//uint16_t SerialCommunicator::myResetDelay = 800;//The delay before a reset is actioned
//#endif

static void logValueVisitor(Stream &serial, FieldData &fieldData)
	{
		serial.print(fieldData.getValue(commonlyUsedBuffer, commonlyUsedBuffersize));
		serial.print(FIELDSEPERATOR);
	}
static void logHeaderVisitor(Stream &serial, FieldData &fieldData)
	{
		serial.print(fieldData.myClassName);
		serial.print(CLASSSEPERATOR);
		serial.print(fieldData.myFieldName);
		serial.print(FIELDSEPERATOR);
	}
static void dumpVisitor(Stream &serial, FieldData &fieldData)
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
		fieldData.dump(serial);
	}

static void dumpWritableData(Stream &serial, FieldData &fieldData)
	{
		if ((fieldData.myModFlag & MOD_WRITE) != MOD_WRITE) return;
		serial.print((__FlashStringHelper*) SET);
		serial.print(fieldData.myClassName);
		serial.print(CLASSSEPERATOR);
		serial.print(fieldData.myFieldName);
		serial.print('=');
		serial.println(fieldData.getValue(commonlyUsedBuffer, commonlyUsedBuffersize));
	}

void SerialCommunicator::dumpCommands()
	{
		/*
		 * If you run out of program space you can save by comenting this verboze proza
		 * That will make you can not query the commands but it also means you have more
		 * program space
		 * */
		mySerialStringReader.myStream.println(F("Following commands are supported"));
		mySerialStringReader.myStream.println(F("? to show this info"));
		mySerialStringReader.myStream.println(F("DUMP full memory dump"));
		mySerialStringReader.myStream.println(F("GET [Name] 1 field memory dump"));
		mySerialStringReader.myStream.println(F("SET dump all the set commands"));
		mySerialStringReader.myStream.println(F("SET [Field]=[value] set value of field"));
		mySerialStringReader.myStream.println(F("LOG_VALUE LOG all the values"));
		mySerialStringReader.myStream.println(F("LOG HEADER LOSerialOutput.he names"));
	}



void SerialCommunicator::logValue()
	{
		mySerialStringReader.myStream.print((__FlashStringHelper*) LOG_VALUE);
		mySerialStringReader.myStream.print(FIELDSEPERATOR);
		FieldData::visitAllFields(logValueVisitor, mySerialStringReader.myStream, true);
		mySerialStringReader.myStream.println();
	}
/*
 * This method does the repetitive task of the class.
 * This method should be called from the loop method.
 */
void SerialCommunicator::loop()
	{
		myLastLoopMillis = millis();
		static uint32_t previousLoopMillis = 0;
		static const uint32_t firstLoopMillis = millis();
		if (myLoopCounter != 0)
			{
				myAveragebetweenLoops = (myLastLoopMillis - firstLoopMillis) / (myLoopCounter);
				myMaxbetweenLoops = max(myMaxbetweenLoops, myLastLoopMillis - previousLoopMillis);
			}
		myLoopCounter++;
		previousLoopMillis = myLastLoopMillis;

		mySerialStringReader.loop();
		if (mySerialStringReader.messageReceived())
			{
				mySerialStringReader.myStream.println(mySerialStringReader.getMessage());
				setReceivedMessage(mySerialStringReader.getMessage());
			}
		if ((myLogLevel & 1) == 1)
			{

				if (((myLastLoopMillis - mylastLog) >= myLogDelay)
#ifdef MAX_MILLIS_IN_LOOP_TO_START
				        && (myLastLoopMillis - loopMillis < MAX_MILLIS_IN_LOOP_TO_START))
#else
					)
#endif
					{
						mylastLog = myLastLoopMillis;
						/*
						 * If you get an error below it means that the arduino stream class needs to be modified.
						 * add the following to stream.h after the Available method
						 * virtual int availableForWrite(void){return 0;};
						 */
						int availableForWrite = mySerialStringReader.myStream.availableForWrite();
						if (availableForWrite >= mySerialQueueSize) //only when the buffer is completely empty transmit data
							{
								uint32_t logstart = millis();
								logValue();
								myLogduration = millis() - logstart;
							} else
							{
								//The below output is tight for me
								// the reason is that you do not want to put load on the stream
								mySerialError.print(F("log Fail have:"));
								mySerialError.print(availableForWrite);
								mySerialError.print(F(" need:"));
								mySerialError.println(mySerialQueueSize);
							}
					}
			}
		myLoopduration = millis() - myLastLoopMillis;

	}
static void dumpAllFields(Stream &serial)
	{
		serial.println(F("Dumping all fields"));
		serial.print(F("SketchName\t"));
		serial.println((__FlashStringHelper*) mySketchName);
		serial.print(F("CompileDate\t"));
		serial.println(F(__DATE__));
		FieldData::visitAllFields(dumpVisitor, serial, true);
	}

/*
 * This method parses a incoming string (the message is expected to be complete)
 * Depending on the instruction the action is undertaken.
 */
void SerialCommunicator::setReceivedMessage(const char *newMessage)
	{
		if (strcmp_P(newMessage, DUMP) == 0)
			{
				dumpAllFields(mySerialStringReader.myStream);
			} else if (strncmp_P(newMessage, GET, 4) == 0)
			{
				mySerialStringReader.myStream.println((__FlashStringHelper*) GET);
				FieldData *fieldData = FieldData::findField(newMessage + 4);
				if (fieldData != 0) fieldData->dump(mySerialStringReader.myStream);
			} else if (strncmp_P(newMessage, SET, 4) == 0)
			{
				mySerialStringReader.myStream.println((__FlashStringHelper*) SET);
				FieldData *fp = FieldData::findField(newMessage + 4);
				if (fp != 0)
					{
						fp->setValue(newMessage + 4 + strlen_P((const char* ) fp->myClassName) + strlen_P((const char* ) fp->myFieldName) + 2);
						fp->dump(mySerialStringReader.myStream);
					}
			} else if (strncmp_P(newMessage, SET, 3) == 0)
			{
				mySerialStringReader.myStream.println((__FlashStringHelper*) SET);
				FieldData::visitAllFields(dumpWritableData, mySerialStringReader.myStream, true);
			} else if (strcmp_P(newMessage, LOG_HEADER) == 0)
			{
				mySerialStringReader.myStream.print((__FlashStringHelper*) LOG_HEADER);
				mySerialStringReader.myStream.print(FIELDSEPERATOR);
				FieldData::visitAllFields(logHeaderVisitor, mySerialStringReader.myStream, true);
				mySerialStringReader.myStream.println();
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
						mySerialError.print((__FlashStringHelper*) ERROR);
						//Even though it is handy to see what has been send
						//The yun bootloader sends press ard to stop bootloader
						//echoing this means the bootloader receives ard
						//mySerialOutput.println(newMessage);
					}
				return;
			}
		mySerialStringReader.myStream.println((__FlashStringHelper*) DONE);
	}

//#ifdef I_USE_RESET
//SerialCommunicator::SerialCommunicator(uint8_t resetPin)
//{
//	myResetPin = resetPin;
//	myResetDelay = 300;
//}
//#else
SerialCommunicator::SerialCommunicator(Stream &bridgeStream,  Stream &errorStream)
				:
				mySerialStringReader(bridgeStream), mySerialOutput(bridgeStream), mySerialError(errorStream)
	{
	}

void SerialCommunicator::serialRegister(const __FlashStringHelper* Name)
	{
		FieldData::set(Name, F("LogLevel"), MOD_WRITE | MOD_SAVE, &myLogLevel);
		FieldData::setNext(F("DelayBetweenLogs"), MOD_WRITE | MOD_SAVE, &myLogDelay);
		FieldData::setNext(F("LoopCounter"), MOD_NONE, &myLoopCounter);
		FieldData::setNext(F("millis"), MOD_OVERVIEW, &myLastLoopMillis);
		FieldData::setNext(F("Avg_Loop"), MOD_NONE, &myAveragebetweenLoops);
		FieldData::setNext(F("Max_Loop"), MOD_NONE, &myMaxbetweenLoops);
		FieldData::setNext((__FlashStringHelper*) LOOPDURATION, MOD_NONE, &myLogduration);
		FieldData::setNext(F("last_log_duration"), MOD_NONE, &myLogduration);
//#ifdef I_USE_RESET
//	FieldData::set(Name, F("ResetPin"), 0, &myResetPin);
//	FieldData::set(Name, F("ResetDelay"),MOD_WRITE | MOD_SAVE, &myResetDelay);
//#endif
	}

void SerialCommunicator::setup()
	{
		mySerialQueueSize = 0;
		int16_t oldSerialQueueSize = 0;
		do
			{
				delay(200);			//wait 200 ms in that time interval the queue should have been processed a bit
				oldSerialQueueSize = mySerialQueueSize;
				mySerialQueueSize = mySerialStringReader.myStream.availableForWrite();
			} while (mySerialQueueSize != oldSerialQueueSize);
		mySerialOutput.print("SerialQueueSize =");
		mySerialOutput.println(mySerialQueueSize);

		mySerialStringReader.setup();
		mylastLog = millis();
	}

void waitForYunToBoot()
	{
		pinMode(7, INPUT_PULLUP);
		delay(20);
		while (digitalRead(7) == HIGH) //wait for yun to startup
			{
				// The pin is still high, so give the LED a quick flash to show we're waiting.
				digitalWrite(LED_BUILTIN, HIGH);
				delay(100);
				digitalWrite(LED_BUILTIN, LOW);
				delay(100);
			}
	}
