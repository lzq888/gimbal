/*************************************************************
************************A R M S*******************************
**************************************************************
Name	: STM32 ADC example
Author	: Alex Huang
Version	: V1.0
Description: This example shows how to use ADC function to capture 1 channel analog data in STM32.
**************************************************************/
/*Header*/

#include "config.h"
//*********************************************************
// Function: Main Program
// Input:  N/A
// Output: N/A
// Remark: N/A
//*********************************************************	 
float 	tempx;
float 	tempy;
float 	tempyaw;

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
		/*
		if(gpgga_flag==1)
		{
			for(int i=0;i<buffer_length;i++)
			{
				USART_SendData(USART1,gps_buf_string[i]); 	
	  			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
			}	

			buffer_length=0;
			gpgga_flag=0;
		}
		*/
		//get_gps_data();
		
		//printf("x : %f\r\n",gps.x);
		//printf("y : %f\r\n",gps.y);
		//printf("z : %f\r\n",gps.z);
		
		
		mpu_9150_data();						
		//printf("acc_x,%f,acc_y,%f,acc_z,%f,gyro_x,%f,gyro_y,%f,gyro_z,%f\r\n mag_x,%f,mag_y,%f,mag_z,%f\r\n",
		//acc_time.x, acc_time.y, acc_time.z,gyr_time.x, gyr_time.y, gyr_time.z,mag_time.x, mag_time.y, mag_time.z);		
	
		//ang.Pitch = toDeg(-asinf(acc.x));
		//ang.Roll  = toDeg(atan2f(acc.y, acc.z));

		//tempx = mag.x*cosf(ang.Pitch) + mag.y*sinf(ang.Pitch)*sinf(ang.Roll) - mag.z*sinf(ang.Pitch)*cosf(ang.Roll);
		//tempy = mag.y*cosf(ang.Roll)  + mag.z*sinf(ang.Roll);

		//ang.Yaw   = toDeg(atan2f(mag.x, mag.y));
		//tempyaw	  = toDeg(atan2f(tempx, tempy));

		//printf("Pitch,%f,Roll,%f,Yaw,%f\r\n",ang.Pitch, ang.Roll,ang.Yaw);		




	}
}






