
#include <stdlib.h>
#include "initial.h"
#include "stm32f10x.h"
#include "stm32f10x_conf.h"




void Initial_MCU(void)
{	
	RCC_Configuration();	// Enable Clock
	GPIO_Configuration();	// Define Indicator LED
 	/*Configure ADC2*/
	Configure_ADC1();
   	/*DMA Configuration*/
	DMA1_Channel3_Configuration();

	MPU6050_I2C_Init();
	MPU6050_Initialize();
	
	SPI2_Init();
	initial_HMC5983();


	TIMER_Configuration();

	USART_Config( USART1, 115200) ;  //for display on computer
	USART_Config( USART2, 115200) ;  //for transmition
	
	

    EXTI_Configuration();
	NVIC_Configuration();

	


}


