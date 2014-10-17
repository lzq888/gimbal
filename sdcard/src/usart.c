#include "usart.h"

//*********************************************************
// Function: printf() function enable
// Input:  N/A
// Return: N/A
// Remark: @Adding th following codes to 
//		    support user calling printf() function.
//		   @Redirect the printf() port to USART1.
//*********************************************************
#pragma import(__use_no_semihosting)             
//Function for stdio lib.                
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//definition of _sys_exit() function    
_sys_exit(int x) 
{ 
	x = x; 
} 
//Redefine fputc() function. 
int fputc(int ch, FILE *f)
{      
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
    USART_SendData(USART1,(uint8_t)ch);   
	return ch;
}


//*********************************************************
// Function: Configure USART Setting
// Input:  N/A
// Return: N/A
// Remark: N/A
//*********************************************************
void USART_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 

  	//Enable Clock  	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//Rx Pin
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_10;	// USART1 Rx (PA.10)								
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//Tx Pin
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_9;		// USART1 Tx (PA.09)
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Parameters for USART1	
	USART_InitStructure.USART_BaudRate 				= 115200;
	USART_InitStructure.USART_WordLength 			= USART_WordLength_8b;
	USART_InitStructure.USART_StopBits 				= USART_StopBits_1;
	USART_InitStructure.USART_Parity 				= USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode 					= USART_Mode_Rx | USART_Mode_Tx;
	
	// Configure USART1
	USART_Init(USART1, &USART_InitStructure);	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	
	USART_Cmd(USART1, ENABLE);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)	;
	
	/* Enable the USART1 Interrupt */																																												
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;																																												
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;																																												
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;																																												
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;																																												
	NVIC_Init(&NVIC_InitStructure);			
}


//*********************************************************
// Function: USART1 Rx  Interrupt Handler
// Input:  N/A
// Return: N/A
// Remark: N/A
//*********************************************************
void USART1_IRQHandler(void)                
{
	u8 U1_RxData = 0; 
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
	  U1_RxData =USART_ReceiveData(USART1); //Get Data 
	}
	else
	{
	  U1_RxData =USART_ReceiveData(USART1); //Get Data 		
	}

}

