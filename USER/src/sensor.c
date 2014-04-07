#include "sensor.h"
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "bool.h"
#include <math.h>
#include "global.h"
#include "MPU6050.h"

int 			i=0;
float 			acc_offset[3];
float 			gyro_offset[3];

imu_buffer   	mpu6050_buf;

float magne_correction(float ASA)
{
	float ajust_value=0;
	ajust_value=((ASA-128)*0.5/128.0)+1;
	return ajust_value;
}

bool acc_gyro_correction_status()
{      
    if(i>=1000)
    {
        
        return TRUE;
    }

        return FALSE;
}

void acc_gyro_correction_offset(float *acc_offset,float *gyro_offset)
{
	static float acc_buff[3];
	static float gyro_buff[3];

	for(int j=0;j<3;j++)
	{
		acc_buff[j]+=acc_offset[j];
		gyro_buff[j]+=gyro_offset[j];
	}
	i++;
	if(acc_gyro_correction_status())
	{
		for(int j=0;j<3;j++)
		{
			acc_offset[j]=acc_buff[j]/1000.0;
			gyro_offset[j]=gyro_buff[j]/1000.0;
			correction_flag=1;
		}
	}
}



void mpu_9150_data()
{

	MPU6050_GetRawAccelGyro(mpu6050_buf.buff,mpu6050_buf.magne);
	//delay(10);

	acc.x = (mpu6050_buf.buff[0]/8192.0);//16384//8192
	acc.y = (mpu6050_buf.buff[1]/8192.0);//16384//8192
	acc.z = (mpu6050_buf.buff[2]/8192.0);//16384//8192
	gyr.x = (mpu6050_buf.buff[3]/16.4);//16384//8192
	gyr.y = (mpu6050_buf.buff[4]/16.4);//16384//8192
	gyr.z = (mpu6050_buf.buff[5]/16.4);//16384//8192
	mag.x = (mpu6050_buf.magne[0]/1.0);//16384//8192
	mag.y = (mpu6050_buf.magne[1]/1.0);//16384//8192
	mag.z = (mpu6050_buf.magne[2]/1.0);//16384//8192

	sensor_correction();

	acc.x -= acc_offset[0] ;//16384//8192
	acc.y -= acc_offset[1] ;//16384//8192
	acc.z += acc_offset[2] ;//16384//8192
	gyr.x -= gyro_offset[0] ;//16384//8192
	gyr.y -= gyro_offset[1] ;//16384//8192
	gyr.z -= gyro_offset[2] ;//16384//8192
	mag.x *= mpu6050_buf.magn_correct[0];//131/16.4
	mag.y *= mpu6050_buf.magn_correct[1];//131/16.4
	mag.z *= mpu6050_buf.magn_correct[2];//131/16.4
		
	//delay(10);

}


void sensor_correction()
{
	uint8_t 	tmp;
	/*magne correction start*/
	MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAX,7, 8, &tmp);
	mpu6050_buf.magn_correct[0]=tmp;
	//delay(10);
    MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAY,7, 8, &tmp);
	mpu6050_buf.magn_correct[1]=tmp;
	//delay(10);
	MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAZ,7, 8, &tmp);
	mpu6050_buf.magn_correct[2]=tmp;
	//delay(10);
	
	mpu6050_buf.magn_correct[0] = magne_correction(mpu6050_buf.magn_correct[0]);
	mpu6050_buf.magn_correct[1] = magne_correction(mpu6050_buf.magn_correct[1]);
	mpu6050_buf.magn_correct[2] = magne_correction(mpu6050_buf.magn_correct[2]);
	/*magne correction over*/

	if(acc_gyro_correction_status()==FALSE && correction_flag==0)
		{		
			acc_offset[0] = acc.x;
			acc_offset[1] = acc.y;
			acc_offset[2] = (1.0 - acc.z);
			gyro_offset[0] = gyr.x;
			gyro_offset[1] = gyr.y;
			gyro_offset[2] = gyr.z;
			acc_gyro_correction_offset(&(acc_offset),&(gyro_offset));
		}

}

