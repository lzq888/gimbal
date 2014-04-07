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

float magne_correction(float ASA);
bool acc_gyro_correction_status();
void acc_gyro_correction_offset(float* acc_offset,float* gyro_offsets);
void mpu_9150_data();
void sensor_correction();

#endif