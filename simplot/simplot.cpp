#include "simplot.h"


void _simple_plot_(Stream &output, byte numData, int data1, int data2, int data3, int data4, int data5, int data6)
{
	int plotBuffer[8];
  int pktSize;

  plotBuffer[0] = 0xCDAB;             //SimPlot packet header. Indicates start of data packet
  plotBuffer[1] = numData*sizeof(int);      //Size of data in bytes. Does not include the header and size fields
  plotBuffer[2] = data1;
  plotBuffer[3] = data2;
  plotBuffer[4] = data3;
  plotBuffer[5] = data4;
  plotBuffer[6] = data5;
  plotBuffer[7] = data6;

  pktSize =  (numData+2)*sizeof(int); //Header bytes + size field bytes + data

  output.write((uint8_t * )plotBuffer, pktSize);
  output.println();
}

