#include "global.h"
#include <stdlib.h>
#include <string.h>
 
/*joystick data*/
u16 			Mean_filter_output[4]={0,0,0,0};
u16  			ADC1_Buf[20][4];
u8				Mean_filter_BuF_string_checksum[29];
u8      		Mean_filter_BuF_string_checksum_buf[5][29];
u8      		timer_buf_flag=0;
u16     		difference[4] = {0,0,0,0};
Remote_signal   joystick;


u8      		timer_buf=0;
/*for gps*/
u8  			gps_buf;
sensor_gps		gps;
/*for imu*/
u8      		correction_flag=0;
sensor_acc		acc;
sensor_gyro		gyr;
sensor_magne    mag;


/*for ahrs*/
EulerAngle 		ang;
Quaternion      qua;




/*initial*/
u8   			initial_flag=0;
















