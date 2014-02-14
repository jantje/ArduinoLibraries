#include "I2CLiquidCrystal.h"
#include "mapper.h"



#define WIRE_WRAPPER0(command) \
		Wire.beginTransmission (myAddress);\
		Wire.print(command);\
		Wire.endTransmission()

#define WIRE_WRAPPER(command, info) \
		Wire.beginTransmission (myAddress);\
		Wire.print(command);\
  	size_t ret = Wire.print(info);\
  	 Wire.endTransmission();\
    return ret

#define WIRE_WRAPPER2(command,info1, info2) \
		Wire.beginTransmission (myAddress);\
		Wire.print(command);\
		Wire.print(info1);\
    Wire.print(sepperator);\
    Wire.print( info2);\
  	Wire.endTransmission()

#define WIRE_WRAPPER3(command,info1, info2, info3) \
		Wire.beginTransmission (myAddress);\
		Wire.print(command);\
		Wire.print(info1);\
    Wire.print(sepperator);\
    Wire.print( info2);\
    Wire.print(sepperator);\
    Wire.print( info3);\
  	Wire.endTransmission()

#define WIRE_WRAPPER_PRINT_NUM(command,info1, info2) \
		Wire.beginTransmission (myAddress);\
		Wire.print(command);\
  	size_t ret = Wire.print(info1,info2);\
  	Wire.endTransmission();\
  	return ret


//LiquidCrystal::LiquidCrystal(uint8_t address) {	myAddress=address;};
//	  void LiquidCrystal::begin() { Wire.begin(myAddress); clear(); };
	  //void LiquidCrystal::init(){};
uint8_t LiquidCrystal::myAddress;

	  void LiquidCrystal::clear() {WIRE_WRAPPER0(_clear);};
	  void LiquidCrystal::home(){WIRE_WRAPPER0(_home);};

	  void LiquidCrystal::noDisplay(){WIRE_WRAPPER0(_noDisplay);};
	  void LiquidCrystal::display(){WIRE_WRAPPER0(_display);};
	  void LiquidCrystal::noBlink(){WIRE_WRAPPER0(_noBlink);};
	  void LiquidCrystal::blink(){WIRE_WRAPPER0(_blink);};
	  void LiquidCrystal::noCursor(){WIRE_WRAPPER0(_noCursor);};
	  void LiquidCrystal::cursor(){WIRE_WRAPPER0(_cursor);};
	  void LiquidCrystal::scrollDisplayLeft(){WIRE_WRAPPER0(_scrollDisplayLeft);};
	  void LiquidCrystal::scrollDisplayRight(){WIRE_WRAPPER0(_scrollDisplayRight);};
	  void LiquidCrystal::leftToRight(){WIRE_WRAPPER0(_leftToRight);};
	  void LiquidCrystal::rightToLeft(){WIRE_WRAPPER0(_rightToLeft);};
	  void LiquidCrystal::autoscroll(){WIRE_WRAPPER0(_autoscroll);};
	  void LiquidCrystal::noAutoscroll(){WIRE_WRAPPER0(_noAutoscroll);};

	  void LiquidCrystal::createChar(uint8_t, uint8_t[]){};
	  void LiquidCrystal::setCursor(uint8_t col, uint8_t row){WIRE_WRAPPER2(_setCursor,col,row);};
	  void LiquidCrystal::command(uint8_t){};

    size_t LiquidCrystal::print(const __FlashStringHelper * info) {WIRE_WRAPPER(_print,info);};
#ifdef I_USE_STRING
    size_t LiquidCrystal::print(const String &info) {WIRE_WRAPPER(_print,info);};
#endif
    size_t LiquidCrystal::print(const char info[]){WIRE_WRAPPER(_print,info);};
    size_t LiquidCrystal::print(char info){WIRE_WRAPPER(_print,info);};
    size_t LiquidCrystal::print(unsigned char info1, int info2 ) {WIRE_WRAPPER_PRINT_NUM(_print,info1,info2);};
    size_t LiquidCrystal::print(int info1, int info2){WIRE_WRAPPER_PRINT_NUM(_print,info1,info2);};
    size_t LiquidCrystal::print(unsigned int info1, int info2){WIRE_WRAPPER_PRINT_NUM(_print,info1,info2);};
    size_t LiquidCrystal::print(long info1, int info2){WIRE_WRAPPER_PRINT_NUM(_print,info1,info2);};
    size_t LiquidCrystal::print(unsigned long info1, int info2){WIRE_WRAPPER_PRINT_NUM(_print,info1,info2);};
    size_t LiquidCrystal::print(double info1, int info2){WIRE_WRAPPER_PRINT_NUM(_print,info1,info2);};
    size_t LiquidCrystal::print(const Printable& info){WIRE_WRAPPER(_print,info);};

//    size_t LiquidCrystal::println(const __FlashStringHelper *info){WIRE_WRAPPER(_println,info);};
//#ifdef I_USE_STRING
//    size_t LiquidCrystal::println(const String &info){WIRE_WRAPPER(_println,info);};
//#endif
//    size_t LiquidCrystal::println(const char info[] ){WIRE_WRAPPER(_println,info);};
//    size_t LiquidCrystal::println(char info){WIRE_WRAPPER(_println,info);};
//    size_t LiquidCrystal::println(unsigned char info1, int info2){WIRE_WRAPPER_PRINT_NUM(_println,info1,info2);};
//    size_t LiquidCrystal::println(int info1, int info2){WIRE_WRAPPER_PRINT_NUM(_println,info1,info2);};
//    size_t LiquidCrystal::println(unsigned int info1, int info2){WIRE_WRAPPER_PRINT_NUM(_println,info1,info2);};
//    size_t LiquidCrystal::println(long info1, int info2){WIRE_WRAPPER_PRINT_NUM(_println,info1,info2);};
//    size_t LiquidCrystal::println(unsigned long info1, int info2){WIRE_WRAPPER_PRINT_NUM(_println,info1,info2);};
//    size_t LiquidCrystal::println(double info1, int info2){WIRE_WRAPPER_PRINT_NUM(_println,info1,info2);};
//    size_t LiquidCrystal::println(const Printable& info){WIRE_WRAPPER(_println,info);};
//    size_t LiquidCrystal::println(void){WIRE_WRAPPER0(_println);return 0;};

    void LiquidCrystal::setBackground(uint8_t red,uint8_t green, uint8_t blue){WIRE_WRAPPER3(_setBackground,red,green,blue);};


