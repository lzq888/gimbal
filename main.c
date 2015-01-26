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

	initial_AccGyro();
	DELAY_ms(5000);
	initial_mag();	
	DELAY_ms(5000);	
	AHRS_Init(&(qua),&(ang));
	target_initial();

	USART_Config( USART2, 115200) ;  //for joystick
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
	USART_Config( USART3, 115200);     //for gps
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);

	//SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx, ENABLE);
	joystick_initial();
	//sdcard_initial
	float SD_Size = 0;
	u32   str_count = 0;

	while(SD_Initialize())
	{
		printf("Please insert SD Card.\r\n");
		DELAY_mS(1000);	
	}
	SD_Size = SD_GetSectorCount();	
	SD_Size = (SD_Size*512.0f)/1024.0f/1024.0f/1024.0f ;		
	//Show SD Card Size
	printf("SD Card Size:%f GB\r\n",SD_Size);
	f_mount(0,&fs);
	res = f_open(&fsrc,"/new/data.txt",FA_WRITE | FA_CREATE_ALWAYS);
	while(res != FR_OK)
	{
		printf("create file error!!");
		DELAY_mS(1000);	
	}
	DELAY_ms(5000);	
	initial_flag = 1;
	while(1)
	{	
			
	}
}






