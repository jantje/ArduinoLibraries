/*
A library to provide basic support for the simplot oscilloscope

 */
#ifndef simplot_H_
#define simplot_H_
#include "Arduino.h"


/* use the general plot method or one of the wrapping macros*/
void _simple_plot_(Stream &output, uint8_t numData, int16_t data1, int16_t data2, int16_t data3, int16_t data4, int16_t data5, int16_t data6);



#define plot6(output,  data1,  data2,  data3,  data4,  data5,  data6) _simple_plot_(output, 6, data1, data2, data3,  data4,  data5,  data6)

#define plot5(output,  data1,  data2,  data3,  data4,  data5 ) _simple_plot_(output, 5, data1, data2, data3,  data4,  data5,  0)

#define plot4(output,  data1,  data2,  data3,  data4) _simple_plot_(output, 4, data1, data2, data3,  data4,  0,  0)

#define plot3(output,  data1,  data2,  data3) _simple_plot_(output, 3, data1, data2, data3,  0,  0,  0)

#define plot2(output,  data1,  data2) _simple_plot_(output, 2, data1, data2, 0,  0,  0,  0)

#define plot1(output,  data1) _simple_plot_(output, 1, data1, 0, 0,  0,  0,  0)

#endif //simplot_H_
