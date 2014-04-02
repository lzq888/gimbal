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
		get_gps_data();
		//mpu_9150_data();
		//delay(100);
	}
}






