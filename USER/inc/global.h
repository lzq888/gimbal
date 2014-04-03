#ifndef __global_H
#define __global_H
#include "alientek.h"

typedef struct {
	float 		x;
	float 		y;
	float       z;
}sensor_acc;

typedef struct {
	float 		x;
	float 		y;
	float       z;
}sensor_gyro;

typedef struct {
	float 		x;
	float 		y;
	float       z;
}sensor_magne;

typedef struct {
	float 		x;
	float 		y;
	float       z;
}sensor_gps;

typedef struct {
	float     	latitude_value;
	float     	longtitude_value;
	float     	height_value;
}gps_data;

/*joystick data*/
extern  u16  			Mean_filter_output[4];
extern  u16  			ADC1_Buf[20][4];
extern  u8      		Mean_filter_BuF_string_checksum[29];
extern  u8      		Mean_filter_BuF_string_checksum_buf[5][29];
extern  u8      		timer_buf_flag;
extern  u8      		timer_buf;
extern  u16     		difference[4];
/*for gps*/
extern  u8  			gps_buf;
/*for imu*/
extern  u8      		correction_flag;
extern  sensor_acc		acc;
extern  sensor_gyro		gyr;
extern  sensor_magne    mag;
extern  sensor_gps		gps;
extern  gps_data 		gga;









#endif

