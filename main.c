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

float 	magn_x;
float 	magn_y;
float 	magn_yaw;


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

	initial_AccGyro();
	//DELAY_ms(5000);
	initial_mag();	
	DELAY_ms(5000);
	AHRS_Init(&(qua),&(ang));

	initial_flag = 1;
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
	
		
		
		/*compass
		tempx = ((mag_time.x - mag_time.EllipseX0) * arm_cos_f32(mag_time.EllipseSita) - (mag_time.y - mag_time.EllipseY0) * arm_sin_f32(mag_time.EllipseSita)) / mag_time.EllipseA;
		tempy = ((mag_time.x - mag_time.EllipseX0) * arm_sin_f32(mag_time.EllipseSita) + (mag_time.y - mag_time.EllipseY0) * arm_cos_f32(mag_time.EllipseSita)) / mag_time.EllipseB;
		tempyaw	 = toDeg(atan2f(tempx, tempy));
		printf("1,%f,2,%f,3,%f\r\n",tempx,tempy,tempyaw);	
		//printf("1,%f,2,%f,3,%f,4,%f,5,%f\r\n",mag_time.EllipseSita,mag_time.EllipseX0,mag_time.EllipseY0,mag_time.EllipseA,mag_time.EllipseB);						
		//printf("mag_x,%f,mag_y,%f,mag_z,%f\r\n",mag_time.x, mag_time.y, mag_time.z);		
		*/



		//printf("1,%d,2,%d,3,%d,4,%d,\r\n",joystick.Remote_PWM_Pitch,joystick.Remote_PWM_Roll,joystick.Remote_PWM_Yaw,joystick.Remote_on_off);	
		
	


		//ang.Pitch = toDeg(-asinf(acc.x));
		//ang.Roll  = toDeg(atan2f(acc.y, acc.z));

		//tempx = mag.x*cosf(ang.Pitch) + mag.y*sinf(ang.Pitch)*sinf(ang.Roll) - mag.z*sinf(ang.Pitch)*cosf(ang.Roll);
		//tempy = mag.y*cosf(ang.Roll)  + mag.z*sinf(ang.Roll);

		//ang.Yaw   = toDeg(atan2f(mag.x, mag.y));
		

		//printf("Pitch,%f,Roll,%f,Yaw,%f\r\n",ang.Pitch, ang.Roll,ang.Yaw);		




	}
}






