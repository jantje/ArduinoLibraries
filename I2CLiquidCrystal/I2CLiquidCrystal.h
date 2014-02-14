/*
 * I2C_LiquidCrystal.h
 *
 *  Created on: Dec 21, 2013
 *      Author: jan
 */

#ifndef I2C_LIQUIDCRYSTAL_H_
#define I2C_LIQUIDCRYSTAL_H_
#include "Arduino.h"
#include "Wire.h"

class LiquidCrystal
{
		private:
		static uint8_t myAddress;
		public:
		LiquidCrystal(uint8_t address) {	myAddress=address;};
		static void begin() { Wire.begin(myAddress); clear(); };
		static void init(){};

		static void clear();
		static void home();

		static void noDisplay();
		static void display();
		static void noBlink();
		static void blink();
	  static void noCursor();
	  static void cursor();
	  static void scrollDisplayLeft();
	  static void scrollDisplayRight();
	  static void leftToRight();
	  static void rightToLeft();
	  static void autoscroll();
	  static void noAutoscroll();

	  static void createChar(uint8_t, uint8_t[]);
	  static void setCursor(uint8_t col, uint8_t row);
	  static void command(uint8_t);

	  static size_t print(const __FlashStringHelper * info) ;
	  static size_t print(const String &info) ;
	  static size_t print(const char info[]);
	  static size_t print(char info);
	  static size_t print(unsigned char info1, int info2 = DEC);
	  static size_t print(int info1, int info2= DEC);
	  static size_t print(unsigned int info1, int info2= DEC);
	  static size_t print(long info1, int info2= DEC);
	  static size_t print(unsigned long info1, int info2= DEC);
	  static size_t print(double info1, int info2= 2);
	  static size_t print(const Printable& info);

//    size_t println(const __FlashStringHelper *info);
//    size_t println(const String &info);
//    size_t println(const char info[] );
//    size_t println(char info);
//    size_t println(unsigned char info1, int info2= DEC);
//    size_t println(int info1, int info2= DEC);
//    size_t println(unsigned int info1, int info2= DEC);
//    size_t println(long info1, int info2= DEC);
//    size_t println(unsigned long info1, int info2= DEC);
//    size_t println(double info1, int info2= 2);
//    size_t println(const Printable& info);
//    size_t println(void);

    static void setBackground(uint8_t red,uint8_t green, uint8_t blue);

};

#endif /* I2C_LIQUIDCRYSTAL_H_ */
