/*************************************************************
************************A R M S*******************************
**************************************************************
Name	: STM32 ADC example
Author	: Alex Huang
Version	: V1.0
Description: This example shows how to use ADC function to capture 1 channel analog data in STM32.
**************************************************************/
/*Header*/
#include "alientek.h"		   
#include "USART.h"
#include "timer.h"
#include "clock.h"
#include "gpio.h"
#include "nvic.h"
#include "dma.h"
#include "adc.h"
#include "initial.h"
#include "exti.h" 
#include "functions.h"
#include "global.h"
#include "stdlib.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "bool.h"
#include "printf.h"
#include "gps_functions.h"
#include "MPU6050.h"
#include "sensor.h"
#include <math.h>

//*********************************************************
// Function: Main Program
// Input:  N/A
// Output: N/A
// Remark: N/A
//*********************************************************	 


typedef struct{
	u8      	gps_start_flag;
	u8      	gps_info_flag;
	u8      	gpgga_flag;
	u16     	gps_word_count;
	u16     	checksum_value;
}gps_flags;

typedef struct{
	u8      	gps_buf_string[100];
	u8      	gps_buf_checksum_string[100];
	u8      	latitude_string[10];
	u8      	longtitude_string[10];
	u8      	height_string[10];
}gps_buffers;

typedef struct {
	float     	latitude_value;
	float     	longtitude_value;
	float     	height_value;
}gps_data;

gps_flags 		gga_flags;
gps_buffers 	gga_buffer;
gps_data 		gga;

float 	x=0;
float 	y=0;
float   z=0;
u16     word_count=0;


typedef struct {
	int16_t 	buff[6];
	int16_t 	magne[3];
	float 		acc[3];
	float   	gyro[3];
	float       magn[3];
	float       magn_correct[3];
}imu_buffer;

float 			acc_offset[3];
float 			gyro_offset[3];

imu_buffer   	mpu6050_buf;	
uint8_t         tmp,tmp1,tmp2;




bool findGPGGA()
{
       
    if(gga_buffer.gps_buf_string[0]=='G' && gga_buffer.gps_buf_string[1]=='P' && gga_buffer.gps_buf_string[2]=='G' && 
       gga_buffer.gps_buf_string[3]=='G' && gga_buffer.gps_buf_string[4]=='A')
    {
        
        return TRUE;
    }

        return FALSE;
}

void send_byte(uint8_t b)
{
	/* Send one byte */
	USART_SendData(USART1, b);

	/* Loop until USART2 DR register is empty */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}
void send_string(uint8_t *str)
{
	while ((*str) != '\0') {

		send_byte(*str);
		str++;
	}
}

void delay(uint32_t delay_count)
{
	while (delay_count) delay_count--;
}
	
 int main(void)
{
	/*Initial STM32*/
	Initial_MCU();


	while(1)
	{

			

		/*get gps information*/
		if(gps_buf!=0)
		{	
			/*resort gps info (every kind)*/	
			if(gga_flags.gps_start_flag==1)
			{
				gga_buffer.gps_buf_string[word_count]=gps_buf;
				word_count++;
				gga_flags.gps_word_count++;
			}
			/*start word*/	
			if(gps_buf=='$')
			{
				gga_flags.gps_start_flag=1;
			}
			/*final word*/
			if(gps_buf=='\n')
			{
				gga_flags.gps_start_flag=0;
				word_count=0;
				gga_flags.gps_info_flag=1;
				/*store over*/
			}

			gps_buf=0;		
		}

		if(gga_flags.gps_info_flag!=0)
		{
			u16 	commond_count=0;
			/*check gpgga or not*/
			if(findGPGGA())
			{
				gga_flags.gpgga_flag=1;
			}
			else
			{	
				for(int i=0;i<100;i++)
				gga_buffer.gps_buf_string[i]=0;

			}
			if(gga_flags.gpgga_flag==1)
			{	   	
				/*yes! it is gpgga data*/

				for(int i=0;i<gga_flags.gps_word_count-5;i++)
				{	
					/*cut checksum value and \r\n*/	
					gga_buffer.gps_buf_checksum_string[i]=gga_buffer.gps_buf_string[i];
	  			}

				/*check data is wright or not*/
				gga_flags.checksum_value=hex2dec(gga_buffer.gps_buf_string[gga_flags.gps_word_count-4],gga_buffer.gps_buf_string[gga_flags.gps_word_count-3]);			
				if(gga_flags.checksum_value==(int)checksum(gga_buffer.gps_buf_checksum_string))
				{
					/*yes! check data is right*/
					int a=0;
					for(int i=0;i<gga_flags.gps_word_count-5;i++)
					{		
						
						if(commond_count==2)
						{/*get latitude and put in buffer*/
							gga_buffer.latitude_string[a]=gga_buffer.gps_buf_checksum_string[i];
							a++;

						}
						if(commond_count==4)
						{/*get longtitude and put in buffer*/
							gga_buffer.longtitude_string[a]=gga_buffer.gps_buf_checksum_string[i];
							a++;
						}
						if(commond_count==9)
						{/*get height and put in buffer*/
							gga_buffer.height_string[a]=gga_buffer.gps_buf_checksum_string[i];
							a++;
						}
						if(gga_buffer.gps_buf_checksum_string[i]==',') 
							{
								commond_count++;
								a=0;
						}
					}
					commond_count=0;

					/*convert string to value*/
					gga.latitude_value=atof(gga_buffer.latitude_string);
					gga.longtitude_value=atof(gga_buffer.longtitude_string);
					gga.height_value=atof(gga_buffer.height_string);

					gga2twd97(degree2radians(m2dec_lad(gga_buffer.latitude_string)),degree2radians(m2dec_lon(gga_buffer.longtitude_string)),&x,&y);
					z=gga.height_value;

					//printf("x : %f\r\n",x);
					//printf("y : %f\r\n",y);
					//printf("z : %f\r\n",z);
					//printf("latitude : %f\r\n",gga.latitude_value);
					//printf("longutitude : %f\r\n",gga.longtitude_value);
					//printf("height : %f\r\n",gga.height_value);
				}
				for(int i=0;i<100;i++)
				{	
					gga_buffer.gps_buf_checksum_string[i]=0;
					gga_buffer.gps_buf_string[i]=0;
				}
			gga_flags.gpgga_flag=0;
			}

			gga_flags.gps_info_flag=0;
			gga_flags.gps_word_count=0;
		}
		

		//tmp=MPU6050_check_bit_status (0x6A,5); 
    	
    	//MPU6050_ReadBit(MPU6050_DEFAULT_ADDRESS, 0x6A, 5, &tmp);
    	//MPU6050_ReadBits(MPU6050_DEFAULT_ADDRESS, 0x6A, 7, 7, &tmp);
    	//mpu6050_buf.id=MPU6050_GetDeviceID();
    	//MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ST1,7, 8, &tmp1);
    	//delay(100);
    	//MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_HZH ,7, 8, &tmp2);
    	//delay(100);
		//printf("tmp1,%d\r\n",tmp1);
		//printf("tmp2,%d\r\n",tmp2);

		MPU6050_GetRawAccelGyro(mpu6050_buf.buff,mpu6050_buf.magne);
		delay(100);
		for ( int i = 0; i<3; i++)
			mpu6050_buf.acc[i] = (mpu6050_buf.buff[i]/8192.0);//16384//8192
		for ( int i = 0; i<3; i++)
			mpu6050_buf.gyro[i] = (mpu6050_buf.buff[i+3]/16.4);//131/16.4
		for ( int i = 0; i<3; i++)
			mpu6050_buf.magn[i] = (mpu6050_buf.magne[i]/1.0);//16384//8192
		for ( int i = 0; i<3; i++)
			mpu6050_buf.magn_correct[i] = (mpu6050_buf.magne[i+3]/1.0);//16384//8192
	
		/*correlation*/
		MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAX,7, 8, &tmp);
		mpu6050_buf.magn_correct[0]=tmp;
		delay(10);
    	MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAY,7, 8, &tmp);
		mpu6050_buf.magn_correct[1]=tmp;
		delay(10);
		MPU6050_ReadBits(AK8975_I2C_ADDR, AK8975_ASAZ,7, 8, &tmp);
		mpu6050_buf.magn_correct[2]=tmp;
		delay(10);

		mpu6050_buf.magn_correct[0]=magne_correction(mpu6050_buf.magn_correct[0]);
		mpu6050_buf.magn_correct[1]=magne_correction(mpu6050_buf.magn_correct[1]);
		mpu6050_buf.magn_correct[2]=magne_correction(mpu6050_buf.magn_correct[2]);
		
		if(acc_gyro_correction_status()==FALSE && correlation_flag==0)
		{
			  
			for ( int i = 0; i<3; i++)
			acc_offset[i] = mpu6050_buf.acc[i];
			for ( int i = 0; i<3; i++)
			gyro_offset[i] = mpu6050_buf.gyro[i];
			acc_gyro_correction_offset(&(acc_offset),&(gyro_offset));
		}

		for ( int i = 0; i<3; i++)
			mpu6050_buf.acc[i] -= acc_offset[i] ;//16384//8192
		for ( int i = 0; i<3; i++)
			mpu6050_buf.gyro[i] -= gyro_offset[i];//131/16.4
		for ( int i = 0; i<3; i++)
			mpu6050_buf.magn[i] *= mpu6050_buf.magn_correct[i];//131/16.4

		/*correlation done*/

				
		printf("acc_x,%f,acc_y,%f,acc_z,%f,gyro_x,%f,gyro_y,%f,gyro_z,%f\r\n mag_x,%f,mag_y,%f,mag_z,%f\r\n",
			mpu6050_buf.acc[0], mpu6050_buf.acc[1], mpu6050_buf.acc[2],
			mpu6050_buf.gyro[0], mpu6050_buf.gyro[1], mpu6050_buf.gyro[2],	
			mpu6050_buf.magn[0], mpu6050_buf.magn[1], mpu6050_buf.magn[2]);
		
		delay(100);











	}
}






