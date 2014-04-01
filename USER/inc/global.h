#ifndef __global_H
#define __global_H
#include "alientek.h"

/*joystick data*/
extern  u16  		Mean_filter_output[4];
extern  u16  		ADC1_Buf[20][4];
extern  u8      	Mean_filter_BuF_string_checksum[29];
extern  u8      	Mean_filter_BuF_string_checksum_buf[5][29];
extern  u8      	timer_buf_flag;
extern  u8      	timer_buf;
extern  u8      	gps_buf;
extern  u16     	difference[4];
extern  u8      	correlation_flag;






#endif

