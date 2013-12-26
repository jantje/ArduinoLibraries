#include "I2CLiquidCrystal.h"




#define WIRE_WRAPPER0(command) \
		Wire.beginTransmission (myAddress);\
		Wire.println(command);\
		Wire.endTransmission()


#define WIRE_WRAPPER(command, info) \
		Wire.beginTransmission (myAddress);\
		Wire.print(command);\
		Wire.print(sepperator);\
  	size_t ret = Wire.println(info);\
  	 Wire.endTransmission();\
    return ret

#define WIRE_WRAPPER2(command,info1, info2) \
		Wire.beginTransmission (myAddress);\
		Wire.print(command);\
		Wire.print(sepperator);\
		Wire.print(info1);\
    Wire.print(sepperator);\
    Wire.println( info2);\
  	Wire.endTransmission()

#define WIRE_WRAPPER3(command,info1, info2, info3) \
		Wire.beginTransmission (myAddress);\
		Wire.print(command);\
		Wire.print(sepperator);\
		Wire.print(info1);\
    Wire.print(sepperator);\
    Wire.print( info2);\
    Wire.print(sepperator);\
    Wire.println( info3);\
  	Wire.endTransmission()

#define WIRE_WRAPPER_PRINT_NUM(command,info1, info2) \
		Wire.beginTransmission (myAddress);\
		Wire.print(command);\
		Wire.print(sepperator);\
  	size_t ret = Wire.println(info1,info2);\
  	Wire.endTransmission();\
  	return ret


const char sepperator=';';
const char printCall[]="print";
const char printlnCall[]="println";


#define WIRE_WRAPPER0(command) \
		Wire.beginTransmission (myAddress);\
		Wire.println(command);\
		Wire.endTransmission()


#define WIRE_WRAPPER(command, info) \
		Wire.beginTransmission (myAddress);\
		Wire.print(command);\
		Wire.print(sepperator);\
  	size_t ret = Wire.println(info);\
  	 Wire.endTransmission();\
    return ret

#define WIRE_WRAPPER2(command,info1, info2) \
		Wire.beginTransmission (myAddress);\
		Wire.print(command);\
		Wire.print(sepperator);\
		Wire.print(info1);\
    Wire.print(sepperator);\
    Wire.println( info2);\
  	Wire.endTransmission()

#define WIRE_WRAPPER3(command,info1, info2, info3) \
		Wire.beginTransmission (myAddress);\
		Wire.print(command);\
		Wire.print(sepperator);\
		Wire.print(info1);\
    Wire.print(sepperator);\
    Wire.print( info2);\
    Wire.print(sepperator);\
    Wire.println( info3);\
  	Wire.endTransmission()

#define WIRE_WRAPPER_PRINT_NUM(command,info1, info2) \
		Wire.beginTransmission (myAddress);\
		Wire.print(command);\
		Wire.print(sepperator);\
  	size_t ret = Wire.println(info1,info2);\
  	Wire.endTransmission();\
  	return ret


LiquidCrystal::LiquidCrystal(uint8_t address) {	myAddress=address;};
	  void LiquidCrystal::begin() { Wire.begin(myAddress); clear(); };
	  void LiquidCrystal::init(){};

	  void LiquidCrystal::clear() {WIRE_WRAPPER0(F("clear"));};
	  void LiquidCrystal::home(){WIRE_WRAPPER0(F("home"));};

	  void LiquidCrystal::noDisplay(){WIRE_WRAPPER0(F("noDisplay"));};
	  void LiquidCrystal::display(){WIRE_WRAPPER0(F("display"));};
	  void LiquidCrystal::noBlink(){WIRE_WRAPPER0(F("noBlink"));};
	  void LiquidCrystal::blink(){WIRE_WRAPPER0(F("blink"));};
	  void LiquidCrystal::noCursor(){WIRE_WRAPPER0(F("noCursor"));};
	  void LiquidCrystal::cursor(){WIRE_WRAPPER0(F("cursor"));};
	  void LiquidCrystal::scrollDisplayLeft(){WIRE_WRAPPER0(F("scrollDisplayLeft"));};
	  void LiquidCrystal::scrollDisplayRight(){WIRE_WRAPPER0(F("scrollDisplayRight"));};
	  void LiquidCrystal::leftToRight(){WIRE_WRAPPER0(F("leftToRight"));};
	  void LiquidCrystal::rightToLeft(){WIRE_WRAPPER0(F("rightToLeft"));};
	  void LiquidCrystal::autoscroll(){WIRE_WRAPPER0(F("autoscroll"));};
	  void LiquidCrystal::noAutoscroll(){WIRE_WRAPPER0(F("noAutoscroll"));};

	  void LiquidCrystal::createChar(uint8_t, uint8_t[]){};
	  void LiquidCrystal::setCursor(uint8_t col, uint8_t row){WIRE_WRAPPER2(F("setCursor"),col,row);};
	  void LiquidCrystal::command(uint8_t){};

    size_t LiquidCrystal::print(const __FlashStringHelper * info) {WIRE_WRAPPER(printCall,info);};
    size_t LiquidCrystal::print(const String &info) {WIRE_WRAPPER(printCall,info);};
    size_t LiquidCrystal::print(const char info[]){WIRE_WRAPPER(printCall,info);};
    size_t LiquidCrystal::print(char info){WIRE_WRAPPER(printCall,info);};
    size_t LiquidCrystal::print(unsigned char info1, int info2 ) {WIRE_WRAPPER_PRINT_NUM(printCall,info1,info2);};
    size_t LiquidCrystal::print(int info1, int info2){WIRE_WRAPPER_PRINT_NUM(printCall,info1,info2);};
    size_t LiquidCrystal::print(unsigned int info1, int info2){WIRE_WRAPPER_PRINT_NUM(printCall,info1,info2);};
    size_t LiquidCrystal::print(long info1, int info2){WIRE_WRAPPER_PRINT_NUM(printCall,info1,info2);};
    size_t LiquidCrystal::print(unsigned long info1, int info2){WIRE_WRAPPER_PRINT_NUM(printCall,info1,info2);};
    size_t LiquidCrystal::print(double info1, int info2){WIRE_WRAPPER_PRINT_NUM(printCall,info1,info2);};
    size_t LiquidCrystal::print(const Printable& info){WIRE_WRAPPER(printCall,info);};

    size_t LiquidCrystal::println(const __FlashStringHelper *info){WIRE_WRAPPER(printlnCall,info);};
    size_t LiquidCrystal::println(const String &info){WIRE_WRAPPER(printlnCall,info);};
    size_t LiquidCrystal::println(const char info[] ){WIRE_WRAPPER(printlnCall,info);};
    size_t LiquidCrystal::println(char info){WIRE_WRAPPER(printlnCall,info);};
    size_t LiquidCrystal::println(unsigned char info1, int info2){WIRE_WRAPPER_PRINT_NUM(printlnCall,info1,info2);};
    size_t LiquidCrystal::println(int info1, int info2){WIRE_WRAPPER_PRINT_NUM(printlnCall,info1,info2);};
    size_t LiquidCrystal::println(unsigned int info1, int info2){WIRE_WRAPPER_PRINT_NUM(printlnCall,info1,info2);};
    size_t LiquidCrystal::println(long info1, int info2){WIRE_WRAPPER_PRINT_NUM(printlnCall,info1,info2);};
    size_t LiquidCrystal::println(unsigned long info1, int info2){WIRE_WRAPPER_PRINT_NUM(printlnCall,info1,info2);};
    size_t LiquidCrystal::println(double info1, int info2){WIRE_WRAPPER_PRINT_NUM(printlnCall,info1,info2);};
    size_t LiquidCrystal::println(const Printable& info){WIRE_WRAPPER(printlnCall,info);};
    size_t LiquidCrystal::println(void){WIRE_WRAPPER0(printlnCall);return 0;};

    void LiquidCrystal::setBackground(uint8_t red,uint8_t green, uint8_t blue){WIRE_WRAPPER3(F("setBackground"),red,green,blue);};


