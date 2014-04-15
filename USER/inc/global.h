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
	float 		EllipseSita;
	float 		EllipseX0;
	float 		EllipseY0;
	float 		EllipseA;
	float 		EllipseB;
	float       magz_offset;
	float 		magz_range;
}sensor_magne;

typedef struct {
	float 		x;
	float 		y;
	float       z;
}sensor_gps;

typedef struct {
	int Remote_PWM_Pitch;
	int Remote_PWM_Roll;
	int Remote_PWM_Yaw;
	int Remote_on_off;
} Remote_signal;

typedef struct {
	float Pitch;
	float Roll;
	float Yaw;
} EulerAngle;

typedef struct {
	float q0;
	float q1;
	float q2;
	float q3;
} Quaternion;


/*joystick data*/
extern  u16  			Mean_filter_output[4];
extern  u16  			ADC1_Buf[20][4];
extern  u8      		Mean_filter_BuF_string_checksum[29];
extern  u8      		Mean_filter_BuF_string_checksum_buf[5][29];
extern  u8      		timer_buf_flag;
extern  u8      		timer_buf;
extern  u16     		difference[4];
extern  Remote_signal   joystick;
/*for gps*/
extern  u8  			gps_buf;
extern  sensor_gps		gps;
/*for imu*/
extern  u8      		correction_flag;
extern  sensor_acc		acc;
extern  sensor_gyro		gyr;
extern  sensor_magne    mag;




/*for ahrs*/
extern  EulerAngle 		ang;
extern  Quaternion      qua;




/*initial*/
extern  u8   			initial_flag;

















#endif

