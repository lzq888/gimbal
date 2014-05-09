
#include <stdlib.h>
#include "gpio.h"

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/*timmer3 remap*/
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);

	//LED1
	GPIO_InitStructure.GPIO_Pin = LED1_Pin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED1_Port, &GPIO_InitStructure);
	//LED2
	GPIO_InitStructure.GPIO_Pin = LED2_Pin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED2_Port, &GPIO_InitStructure);

	//SPI-SS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
    /*PWM*/

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_7;		   	        // PWMOUT PB 7
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_6;		   	        // PWMOUT PB6
	GPIO_Init(GPIOC, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_9;		   	        
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_8;		   	        
	GPIO_Init(GPIOC, &GPIO_InitStructure);



	/*exit*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;					//外部中   PB2
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   	GPIO_Init(GPIOB, &GPIO_InitStructure);
   	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource2);


  /* ADC*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	 /*定義控制按鍵腳位 */	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_4|GPIO_Pin_10;		// KEY  PC4  PC10
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits( GPIOC, GPIO_Pin_4|GPIO_Pin_10);


}
