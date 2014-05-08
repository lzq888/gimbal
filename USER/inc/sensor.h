#ifndef __sensor_H
#define __sensor_H
#include <math.h>
#include "alientek.h"
#include "bool.h"

typedef struct {
	int16_t 	buff[6];
	int16_t 	magne[3];
	float       magn_correct[3];
}imu_buffer;

float 	magne_correction(float ASA);
void 	initial_AccGyro();
void 	initial_mag();
void 	mpu_9150_data();
void 	HMC5983_DATA();
void 	fit_Gyro_with_Magn();
void   	EllipseFitting(float *Ans, s16 *MagDataX, s16 *MagDataY, u8 Num);

#endif