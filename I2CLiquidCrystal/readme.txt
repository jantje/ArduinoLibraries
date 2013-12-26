A library that alows to save pins when using a liquid crystal LCD by using 2 arduinos communicating with TWI
As a have a RGB backlight I also added a RGB setting method.
Compile and upload LCDSlave to the arduino connected to the LCD (change the pins as needed)
Compile and upload the LCDMaster tot he Arduino 2
connect the 2 arduino's with the TWI pins (uno A4 and A5)
The LCD should say on line 1: "Hello, ARDUINO"
on lIne 2 you have a increasing number.
If you have a RGB backlight connected than the RGB color should change with each increment between red green blue.
