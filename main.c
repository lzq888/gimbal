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
		
		mpu_9150_data();		
		printf("acc_x,%f,acc_y,%f,acc_z,%f,gyro_x,%f,gyro_y,%f,gyro_z,%f\r\n mag_x,%f,mag_y,%f,mag_z,%f\r\n",
			acc.x, acc.y, acc.z,gyr.x, gyr.y, gyr.z,mag.x, mag.y, mag.z);		
			
	}
}






