#include "config.h"

int 			i=0;
float 			acc_offset[3];
float 			gyro_offset[3];

float 			Ellipse[5] = {0};
imu_buffer   	mpu6050_buf;


volatile int16_t ACC_FIFO[3][256] = {{0}};
volatile int16_t GYR_FIFO[3][256] = {{0}};

volatile int16_t MAG_FIFO[2][256] = {{0}};

volatile int16_t MagDataX[8] = {0};
volatile int16_t MagDataY[8] = {0};




#define MagCorrect_time   100
#define MagCorrect_Ave    10

float magne_correction(float ASA)
{
	float ajust_value=0;
	ajust_value=((ASA-128)*0.5/128.0)+1;
	return ajust_value;
}


void initial_AccGyro()
{
	static float acc_buff[3];
	static float gyro_buff[3];

	for(int i=0;i<MagCorrect_time;i++)
	{

	MPU6050_GetRawAccelGyro(mpu6050_buf.buff,mpu6050_buf.magne);
	//delay(10);

		acc_offset[0] = (s16)MoveAve_WMA((s16)mpu6050_buf.buff[0], ACC_FIFO[0], MagCorrect_Ave);
		acc_offset[1] = (s16)MoveAve_WMA((s16)mpu6050_buf.buff[1], ACC_FIFO[1], MagCorrect_Ave);
		acc_offset[2] = (s16)MoveAve_WMA((s16)mpu6050_buf.buff[2], ACC_FIFO[2], MagCorrect_Ave);
		gyro_offset[0] = (s16)MoveAve_WMA((s16)mpu6050_buf.buff[3], GYR_FIFO[0], MagCorrect_Ave);
		gyro_offset[1] = (s16)MoveAve_WMA((s16)mpu6050_buf.buff[4], GYR_FIFO[1], MagCorrect_Ave);
		gyro_offset[2] = (s16)MoveAve_WMA((s16)mpu6050_buf.buff[5], GYR_FIFO[2], MagCorrect_Ave);

		acc_offset[0] = (acc_offset[0]/8192.0);//16384//8192
		acc_offset[1] = (acc_offset[1]/8192.0);//16384//8192
		acc_offset[2] = 1 - (acc_offset[2]/8192.0);//16384//8192
		gyro_offset[0] = (gyro_offset[0]/16.4);//16384//8192
		gyro_offset[1] = (gyro_offset[1]/16.4);//16384//8192
		gyro_offset[2] = (gyro_offset[2]/16.4);//16384//8192

		printf("acc_x,%f,acc_y,%f,acc_z,%f,gyr_x,%f,gyr_y,%f,gyr_z,%f\r\n",
			acc_offset[0],acc_offset[1],acc_offset[2],gyro_offset[0],gyro_offset[1],gyro_offset[2]);
	
	//delay(10);
	}

	printf("acc_x,%f,acc_y,%f,acc_z,%f,gyr_x,%f,gyr_y,%f,gyr_z,%f\r\n",
			acc_offset[0],acc_offset[1],acc_offset[2],gyro_offset[0],gyro_offset[1],gyro_offset[2]);

	printf("GYR,ACC calibration complete\r\n ready to calibrate magnetometer \r\n");

}

void initial_mag()
{
	/*direction 1*/
	for(int i=0;i<MagCorrect_time;i++)
	{
	
		uint8_t 	tmp;
		MPU6050_GetRawAccelGyro(mpu6050_buf.buff,mpu6050_buf.magne);
		mag.x = (mpu6050_buf.magne[0]);//16384//8192
		mag.y = (mpu6050_buf.magne[1]);//16384//8192
		mag.z = (mpu6050_buf.magne[2]);//16384//8192

		/*magne correction start*/
		MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAX,7, 8, &tmp);
		mpu6050_buf.magn_correct[0]=tmp;
    	MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAY,7, 8, &tmp);
		mpu6050_buf.magn_correct[1]=tmp;
		MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAZ,7, 8, &tmp);
		mpu6050_buf.magn_correct[2]=tmp;

		mpu6050_buf.magn_correct[0] = ((mpu6050_buf.magn_correct[0]-128)*0.5/128.0)+1;
		mpu6050_buf.magn_correct[1] = ((mpu6050_buf.magn_correct[1]-128)*0.5/128.0)+1;
		mpu6050_buf.magn_correct[2] = ((mpu6050_buf.magn_correct[2]-128)*0.5/128.0)+1;
		/*magne correction over*/

		mag.x *= mpu6050_buf.magn_correct[0];//131/16.4
		mag.y *= mpu6050_buf.magn_correct[1];//131/16.4
		mag.z *= mpu6050_buf.magn_correct[2];//131/16.4


		MagDataX[0] = (s16)MoveAve_WMA((s16)mag.x, MAG_FIFO[0], MagCorrect_Ave);
		MagDataY[0] = (s16)MoveAve_WMA((s16)mag.y, MAG_FIFO[1], MagCorrect_Ave);
		
		printf("mag_x,%f,mag_y,%f,mag_z,%f\r\n",mag.x, mag.y, mag.z);		
	
	}	
		printf("MagDataX[0],%d,MagDataY[0],%d\r\n",MagDataX[0],MagDataY[0]);

	printf("direction 1 complete\r\n please turn to next direction \r\n");

	DELAY_ms(5000);
	/*direction 2*/

	for(int i=0;i<MagCorrect_time;i++)
	{
	
		uint8_t 	tmp;
		MPU6050_GetRawAccelGyro(mpu6050_buf.buff,mpu6050_buf.magne);
		mag.x = (mpu6050_buf.magne[0]);//16384//8192
		mag.y = (mpu6050_buf.magne[1]);//16384//8192
		mag.z = (mpu6050_buf.magne[2]);//16384//8192

		/*magne correction start*/
		MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAX,7, 8, &tmp);
		mpu6050_buf.magn_correct[0]=tmp;
    	MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAY,7, 8, &tmp);
		mpu6050_buf.magn_correct[1]=tmp;
		MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAZ,7, 8, &tmp);
		mpu6050_buf.magn_correct[2]=tmp;

		mpu6050_buf.magn_correct[0] = ((mpu6050_buf.magn_correct[0]-128)*0.5/128.0)+1;
		mpu6050_buf.magn_correct[1] = ((mpu6050_buf.magn_correct[1]-128)*0.5/128.0)+1;
		mpu6050_buf.magn_correct[2] = ((mpu6050_buf.magn_correct[2]-128)*0.5/128.0)+1;
		/*magne correction over*/

		mag.x *= mpu6050_buf.magn_correct[0];//131/16.4
		mag.y *= mpu6050_buf.magn_correct[1];//131/16.4
		mag.z *= mpu6050_buf.magn_correct[2];//131/16.4


		MagDataX[1] = (s16)MoveAve_WMA((s16)mag.x, MAG_FIFO[0], MagCorrect_Ave);
		MagDataY[1] = (s16)MoveAve_WMA((s16)mag.y, MAG_FIFO[1], MagCorrect_Ave);
		
		printf("mag_x,%f,mag_y,%f,mag_z,%f\r\n",mag.x, mag.y, mag.z);		
	
	}	
		printf("MagDataX[1],%d,MagDataY[1],%d\r\n",MagDataX[1],MagDataY[1]);
	
	printf("direction 2 complete\r\n please turn to next direction \r\n");

	DELAY_ms(5000);
	/*direction 3*/

	for(int i=0;i<MagCorrect_time;i++)
	{
	
		uint8_t 	tmp;
		MPU6050_GetRawAccelGyro(mpu6050_buf.buff,mpu6050_buf.magne);
		mag.x = (mpu6050_buf.magne[0]);//16384//8192
		mag.y = (mpu6050_buf.magne[1]);//16384//8192
		mag.z = (mpu6050_buf.magne[2]);//16384//8192

		/*magne correction start*/
		MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAX,7, 8, &tmp);
		mpu6050_buf.magn_correct[0]=tmp;
    	MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAY,7, 8, &tmp);
		mpu6050_buf.magn_correct[1]=tmp;
		MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAZ,7, 8, &tmp);
		mpu6050_buf.magn_correct[2]=tmp;

		mpu6050_buf.magn_correct[0] = ((mpu6050_buf.magn_correct[0]-128)*0.5/128.0)+1;
		mpu6050_buf.magn_correct[1] = ((mpu6050_buf.magn_correct[1]-128)*0.5/128.0)+1;
		mpu6050_buf.magn_correct[2] = ((mpu6050_buf.magn_correct[2]-128)*0.5/128.0)+1;
		/*magne correction over*/

		mag.x *= mpu6050_buf.magn_correct[0];//131/16.4
		mag.y *= mpu6050_buf.magn_correct[1];//131/16.4
		mag.z *= mpu6050_buf.magn_correct[2];//131/16.4


		MagDataX[2] = (s16)MoveAve_WMA((s16)mag.x, MAG_FIFO[0], MagCorrect_Ave);
		MagDataY[2] = (s16)MoveAve_WMA((s16)mag.y, MAG_FIFO[1], MagCorrect_Ave);
		
		printf("mag_x,%f,mag_y,%f,mag_z,%f\r\n",mag.x, mag.y, mag.z);		
	
	}	
		printf("MagDataX[2],%d,MagDataY[2],%d\r\n",MagDataX[2],MagDataY[2]);
	
	printf("direction 3 complete\r\n please turn to next direction \r\n");

	DELAY_ms(5000);

	/*direction 4*/

	for(int i=0;i<MagCorrect_time;i++)
	{
	
		uint8_t 	tmp;
		MPU6050_GetRawAccelGyro(mpu6050_buf.buff,mpu6050_buf.magne);
		mag.x = (mpu6050_buf.magne[0]);//16384//8192
		mag.y = (mpu6050_buf.magne[1]);//16384//8192
		mag.z = (mpu6050_buf.magne[2]);//16384//8192

		/*magne correction start*/
		MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAX,7, 8, &tmp);
		mpu6050_buf.magn_correct[0]=tmp;
    	MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAY,7, 8, &tmp);
		mpu6050_buf.magn_correct[1]=tmp;
		MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAZ,7, 8, &tmp);
		mpu6050_buf.magn_correct[2]=tmp;

		mpu6050_buf.magn_correct[0] = ((mpu6050_buf.magn_correct[0]-128)*0.5/128.0)+1;
		mpu6050_buf.magn_correct[1] = ((mpu6050_buf.magn_correct[1]-128)*0.5/128.0)+1;
		mpu6050_buf.magn_correct[2] = ((mpu6050_buf.magn_correct[2]-128)*0.5/128.0)+1;
		/*magne correction over*/

		mag.x *= mpu6050_buf.magn_correct[0];//131/16.4
		mag.y *= mpu6050_buf.magn_correct[1];//131/16.4
		mag.z *= mpu6050_buf.magn_correct[2];//131/16.4


		MagDataX[3] = (s16)MoveAve_WMA((s16)mag.x, MAG_FIFO[0], MagCorrect_Ave);
		MagDataY[3] = (s16)MoveAve_WMA((s16)mag.y, MAG_FIFO[1], MagCorrect_Ave);
		
		printf("mag_x,%f,mag_y,%f,mag_z,%f\r\n",mag.x, mag.y, mag.z);		
	
	}	
		printf("MagDataX[3],%d,MagDataY[3],%d\r\n",MagDataX[3],MagDataY[3]);
	
	printf("direction 4 complete\r\n please turn to next direction \r\n");

	DELAY_ms(5000);

	/*direction 5*/

	for(int i=0;i<MagCorrect_time;i++)
	{
	
		uint8_t 	tmp;
		MPU6050_GetRawAccelGyro(mpu6050_buf.buff,mpu6050_buf.magne);
		mag.x = (mpu6050_buf.magne[0]);//16384//8192
		mag.y = (mpu6050_buf.magne[1]);//16384//8192
		mag.z = (mpu6050_buf.magne[2]);//16384//8192

		/*magne correction start*/
		MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAX,7, 8, &tmp);
		mpu6050_buf.magn_correct[0]=tmp;
    	MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAY,7, 8, &tmp);
		mpu6050_buf.magn_correct[1]=tmp;
		MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAZ,7, 8, &tmp);
		mpu6050_buf.magn_correct[2]=tmp;

		mpu6050_buf.magn_correct[0] = ((mpu6050_buf.magn_correct[0]-128)*0.5/128.0)+1;
		mpu6050_buf.magn_correct[1] = ((mpu6050_buf.magn_correct[1]-128)*0.5/128.0)+1;
		mpu6050_buf.magn_correct[2] = ((mpu6050_buf.magn_correct[2]-128)*0.5/128.0)+1;
		/*magne correction over*/

		mag.x *= mpu6050_buf.magn_correct[0];//131/16.4
		mag.y *= mpu6050_buf.magn_correct[1];//131/16.4
		mag.z *= mpu6050_buf.magn_correct[2];//131/16.4


		MagDataX[4] = (s16)MoveAve_WMA((s16)mag.x, MAG_FIFO[0], MagCorrect_Ave);
		MagDataY[4] = (s16)MoveAve_WMA((s16)mag.y, MAG_FIFO[1], MagCorrect_Ave);
		
		printf("mag_x,%f,mag_y,%f,mag_z,%f\r\n",mag.x, mag.y, mag.z);		
	
	}	
		printf("MagDataX[4],%d,MagDataY[4],%d\r\n",MagDataX[4],MagDataY[4]);
	
	printf("direction 5 complete\r\n please turn to next direction \r\n");

	DELAY_ms(5000);

	/*direction 6*/

	for(int i=0;i<MagCorrect_time;i++)
	{
	
		uint8_t 	tmp;
		MPU6050_GetRawAccelGyro(mpu6050_buf.buff,mpu6050_buf.magne);
		mag.x = (mpu6050_buf.magne[0]);//16384//8192
		mag.y = (mpu6050_buf.magne[1]);//16384//8192
		mag.z = (mpu6050_buf.magne[2]);//16384//8192

		/*magne correction start*/
		MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAX,7, 8, &tmp);
		mpu6050_buf.magn_correct[0]=tmp;
    	MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAY,7, 8, &tmp);
		mpu6050_buf.magn_correct[1]=tmp;
		MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAZ,7, 8, &tmp);
		mpu6050_buf.magn_correct[2]=tmp;

		mpu6050_buf.magn_correct[0] = ((mpu6050_buf.magn_correct[0]-128)*0.5/128.0)+1;
		mpu6050_buf.magn_correct[1] = ((mpu6050_buf.magn_correct[1]-128)*0.5/128.0)+1;
		mpu6050_buf.magn_correct[2] = ((mpu6050_buf.magn_correct[2]-128)*0.5/128.0)+1;
		/*magne correction over*/

		mag.x *= mpu6050_buf.magn_correct[0];//131/16.4
		mag.y *= mpu6050_buf.magn_correct[1];//131/16.4
		mag.z *= mpu6050_buf.magn_correct[2];//131/16.4


		MagDataX[5] = (s16)MoveAve_WMA((s16)mag.x, MAG_FIFO[0], MagCorrect_Ave);
		MagDataY[5] = (s16)MoveAve_WMA((s16)mag.y, MAG_FIFO[1], MagCorrect_Ave);
		
		printf("mag_x,%f,mag_y,%f,mag_z,%f\r\n",mag.x, mag.y, mag.z);		
	
	}	
		printf("MagDataX[5],%d,MagDataY[5],%d\r\n",MagDataX[5],MagDataY[5]);
	
	printf("direction 6 complete\r\n please turn to next direction \r\n");

	DELAY_ms(5000);

	/*direction 7*/

	for(int i=0;i<MagCorrect_time;i++)
	{
	
		uint8_t 	tmp;
		MPU6050_GetRawAccelGyro(mpu6050_buf.buff,mpu6050_buf.magne);
		mag.x = (mpu6050_buf.magne[0]);//16384//8192
		mag.y = (mpu6050_buf.magne[1]);//16384//8192
		mag.z = (mpu6050_buf.magne[2]);//16384//8192

		/*magne correction start*/
		MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAX,7, 8, &tmp);
		mpu6050_buf.magn_correct[0]=tmp;
    	MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAY,7, 8, &tmp);
		mpu6050_buf.magn_correct[1]=tmp;
		MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAZ,7, 8, &tmp);
		mpu6050_buf.magn_correct[2]=tmp;

		mpu6050_buf.magn_correct[0] = ((mpu6050_buf.magn_correct[0]-128)*0.5/128.0)+1;
		mpu6050_buf.magn_correct[1] = ((mpu6050_buf.magn_correct[1]-128)*0.5/128.0)+1;
		mpu6050_buf.magn_correct[2] = ((mpu6050_buf.magn_correct[2]-128)*0.5/128.0)+1;
		/*magne correction over*/

		mag.x *= mpu6050_buf.magn_correct[0];//131/16.4
		mag.y *= mpu6050_buf.magn_correct[1];//131/16.4
		mag.z *= mpu6050_buf.magn_correct[2];//131/16.4


		MagDataX[6] = (s16)MoveAve_WMA((s16)mag.x, MAG_FIFO[0], MagCorrect_Ave);
		MagDataY[6] = (s16)MoveAve_WMA((s16)mag.y, MAG_FIFO[1], MagCorrect_Ave);
		
		printf("mag_x,%f,mag_y,%f,mag_z,%f\r\n",mag.x, mag.y, mag.z);		
	
	}	
		printf("MagDataX[6],%d,MagDataY[6],%d\r\n",MagDataX[6],MagDataY[6]);
	
	printf("direction 7 complete\r\n please turn to next direction \r\n");

	DELAY_ms(5000);

	/*direction 8*/

	for(int i=0;i<MagCorrect_time;i++)
	{
	
		uint8_t 	tmp;
		MPU6050_GetRawAccelGyro(mpu6050_buf.buff,mpu6050_buf.magne);
		mag.x = (mpu6050_buf.magne[0]);//16384//8192
		mag.y = (mpu6050_buf.magne[1]);//16384//8192
		mag.z = (mpu6050_buf.magne[2]);//16384//8192

		/*magne correction start*/
		MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAX,7, 8, &tmp);
		mpu6050_buf.magn_correct[0]=tmp;
    	MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAY,7, 8, &tmp);
		mpu6050_buf.magn_correct[1]=tmp;
		MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAZ,7, 8, &tmp);
		mpu6050_buf.magn_correct[2]=tmp;

		mpu6050_buf.magn_correct[0] = ((mpu6050_buf.magn_correct[0]-128)*0.5/128.0)+1;
		mpu6050_buf.magn_correct[1] = ((mpu6050_buf.magn_correct[1]-128)*0.5/128.0)+1;
		mpu6050_buf.magn_correct[2] = ((mpu6050_buf.magn_correct[2]-128)*0.5/128.0)+1;
		/*magne correction over*/

		mag.x *= mpu6050_buf.magn_correct[0];//131/16.4
		mag.y *= mpu6050_buf.magn_correct[1];//131/16.4
		mag.z *= mpu6050_buf.magn_correct[2];//131/16.4


		MagDataX[7] = (s16)MoveAve_WMA((s16)mag.x, MAG_FIFO[0], MagCorrect_Ave);
		MagDataY[7] = (s16)MoveAve_WMA((s16)mag.y, MAG_FIFO[1], MagCorrect_Ave);
		
		printf("mag_x,%f,mag_y,%f,mag_z,%f\r\n",mag.x, mag.y, mag.z);		
	
	}	
		printf("MagDataX[7],%d,MagDataY[7],%d\r\n",MagDataX[7],MagDataY[7]);
	
	printf("direction 8 complete\r\n please turn to next direction \r\n");

	for(int i=0;i<8;i++)
	{
		printf("%d,%d\r\n",MagDataX[i],MagDataY[i]);
	}

	DELAY_ms(1000);

	EllipseFitting(Ellipse, MagDataX, MagDataY, 8);

	mag.EllipseSita = Ellipse[0];
	mag.EllipseX0   = Ellipse[1];
	mag.EllipseY0   = Ellipse[2];
	mag.EllipseA    = Ellipse[3];
	mag.EllipseB    = Ellipse[4];

	printf("1,%f,2,%f,3,%f,4,%f,5,%f\r\n",mag.EllipseSita,mag.EllipseX0,mag.EllipseY0,mag.EllipseA,mag.EllipseB);

	
}


void mpu_9150_data()
{
	uint8_t 	tmp;
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


/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : EllipseFitting
**功能 : Ellipse Fitting
**輸入 : Ans, MagDataX, MagDataY, Num
**輸出 : None
**使用 : EllipseFitting(Ellipse, MagDataX, MagDataY, 8);
**=====================================================================================================*/
/*=====================================================================================================*/
void EllipseFitting(float *Ans, s16 *MagDataX, s16 *MagDataY, u8 Num)
{
	s8 i, j, k;
	float temp, temp1, temp2;
	float tempArrX[8] = {0};
	float tempArrY[8] = {0};

	float MAG_X1Y0 = 0.0f;
	float MAG_X2Y0 = 0.0f;
	float MAG_X3Y0 = 0.0f;
	float MAG_X0Y1 = 0.0f;
	float MAG_X0Y2 = 0.0f;
	float MAG_X0Y3 = 0.0f;
	float MAG_X0Y4 = 0.0f;
	float MAG_X1Y1 = 0.0f;
	float MAG_X2Y1 = 0.0f;
	float MAG_X1Y2 = 0.0f;
	float MAG_X1Y3 = 0.0f;
	float MAG_X2Y2 = 0.0f;
	float MAG_X3Y1 = 0.0f;

	float MagArr[5][6] = {{0}};

	for (i = 0; i < Num; i++) {
		tempArrX[i] = (float)MagDataX[i] / 1000.0f;
		tempArrY[i] = (float)MagDataY[i] / 1000.0f;
	}

	for (i = 0; i < Num; i++) {
		MAG_X1Y0 += tempArrX[i];
		MAG_X2Y0 += tempArrX[i] * tempArrX[i];
		MAG_X3Y0 += tempArrX[i] * tempArrX[i] * tempArrX[i];
		MAG_X0Y1 += tempArrY[i];
		MAG_X0Y2 += tempArrY[i] * tempArrY[i];
		MAG_X0Y3 += tempArrY[i] * tempArrY[i] * tempArrY[i];
		MAG_X0Y4 += tempArrY[i] * tempArrY[i] * tempArrY[i] * tempArrY[i];
		MAG_X1Y1 += tempArrX[i] * tempArrY[i];
		MAG_X2Y1 += tempArrX[i] * tempArrX[i] * tempArrY[i];
		MAG_X1Y2 += tempArrX[i] * tempArrY[i] * tempArrY[i];
		MAG_X1Y3 += tempArrX[i] * tempArrY[i] * tempArrY[i] * tempArrY[i];
		MAG_X2Y2 += tempArrX[i] * tempArrX[i] * tempArrY[i] * tempArrY[i];
		MAG_X3Y1 += tempArrX[i] * tempArrX[i] * tempArrX[i] * tempArrY[i];
	}

	MagArr[0][0] = MAG_X2Y2;
	MagArr[0][1] = MAG_X1Y3;
	MagArr[0][2] = MAG_X2Y1;
	MagArr[0][3] = MAG_X1Y2;
	MagArr[0][4] = MAG_X1Y1;
	MagArr[0][5] = -MAG_X3Y1;

	MagArr[1][0] = MAG_X1Y3;
	MagArr[1][1] = MAG_X0Y4;
	MagArr[1][2] = MAG_X1Y2;
	MagArr[1][3] = MAG_X0Y3;
	MagArr[1][4] = MAG_X0Y2;
	MagArr[1][5] = -MAG_X2Y2;

	MagArr[2][0] = MAG_X2Y1;
	MagArr[2][1] = MAG_X1Y2;
	MagArr[2][2] = MAG_X2Y0;
	MagArr[2][3] = MAG_X1Y1;
	MagArr[2][4] = MAG_X1Y0;
	MagArr[2][5] = -MAG_X3Y0;

	MagArr[3][0] = MAG_X1Y2;
	MagArr[3][1] = MAG_X0Y3;
	MagArr[3][2] = MAG_X1Y1;
	MagArr[3][3] = MAG_X0Y2;
	MagArr[3][4] = MAG_X0Y1;
	MagArr[3][5] = -MAG_X2Y1;

	MagArr[4][0] = MAG_X1Y1;
	MagArr[4][1] = MAG_X0Y2;
	MagArr[4][2] = MAG_X1Y0;
	MagArr[4][3] = MAG_X0Y1;
	MagArr[4][4] = Num;
	MagArr[4][5] = -MAG_X2Y0;

	for (i = 0; i < 5; i++)
		for (j = i + 1; j < 6; j++)
			for (k = 5; k > i - 1; k--)
				MagArr[j][k] = MagArr[j][k] - MagArr[j][i] / MagArr[i][i] * MagArr[i][k];

	for (i = 0; i < 5; i++) {
		temp = MagArr[i][i];

		for (j = i; j < 6; j++)
			MagArr[i][j] = MagArr[i][j] / temp;
	}

	for (j = 4; j > 0; j--)
		for (i = 0; i < j; i++)
			MagArr[i][5] = MagArr[i][5] - MagArr[i][j] * MagArr[j][5];

	temp = (1.0f - MagArr[1][5]) / MagArr[0][5];
	temp1 = temp + sqrtf(temp * temp + 1.0f);
	Ans[0] = atanf(temp1);	// Theta

	temp = MagArr[0][5] * MagArr[0][5] - 4 * MagArr[1][5];
	Ans[1] = (2.0f * MagArr[1][5] * MagArr[2][5] - MagArr[0][5] * MagArr[3][5]) / temp;	// X0
	Ans[2] = (2.0f * MagArr[3][5] - MagArr[0][5] * MagArr[2][5]) / temp;	    // Y0

	temp = arm_cos_f32(Ans[0]);
	temp2  = (Ans[1] * Ans[1] + MagArr[0][5] * Ans[1] * Ans[2] + MagArr[1][5] * Ans[2] * Ans[2] - MagArr[4][5]) * (temp1 * temp1 * temp1 * temp1 - 1.0f);
	Ans[3] = temp / sqrtf((MagArr[1][5] * temp1 * temp1 - 1) / temp2);	// a
	Ans[4] = temp / sqrtf((temp1 * temp1 - MagArr[1][5]) / temp2); // b

	Ans[1] = Ans[1] * 1000.0f;
	Ans[2] = Ans[2] * 1000.0f;
	Ans[3] = Ans[3] * 1000.0f;
	Ans[4] = Ans[4] * 1000.0f;
}

