
#include "simplot.h"
int plotBuffer[20];
float deltaAngle = 3.14/51; //Arbitrary angle increment size
float angle = 0;
int amplitude = 100;

void setup()
{
  Serial.begin(115200);

}



void loop()
{

  int data1;
  int data2;
  int data3;
  int data4;

  //Generating data that will be plotted
  data1 = amplitude * sin(angle);
  data2 = amplitude * cos(angle);

  data3 = (amplitude/2) * sin(angle);
  data4 = (amplitude/2) * cos(angle);

  angle = angle + deltaAngle;

  plot4(data1,data2,data3,data4);

  delay(100); //Need some delay else the program gets swamped with data

}

