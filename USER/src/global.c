#include "global.h"
#include <stdlib.h>
#include <string.h>
 
/*joystick data*/
u16 		Mean_filter_output[4]={0,0,0,0};
u16  		ADC1_Buf[20][4];
u8			Mean_filter_BuF_string_checksum[29];
u8      	Mean_filter_BuF_string_checksum_buf[5][29];
u8      	timer_buf_flag=0;
u8      	timer_buf=0;
u8      	gps_buf=0;
u16     	difference[4] = {0,0,0,0};
u8      	correlation_flag=0;










