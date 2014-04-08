/**
  ******************************************************************************
  * @file    USARTC.h
  * @author  Huang, DF
  * @version X
  * @date    10/21/2012
  * @brief   Set USART.send and get message 
  ******************************************************************************
  */ 
/*include header*/

#include "config.h"



	
void USART_Config(USART_TypeDef* USARTX, uint32_t BaudRate)
{


	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/*USART Function Configuration*/
	if (USARTX == USART1)
    {
		//Enable the Clock for USART
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

	}
	else if (USARTX == USART2)
	{
		 //Enable the Clock for USART
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	}
	else
	{
		//Enable the Clock for USART
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);

	}
	
	//Parameters for USART	
	USART_InitStructure.USART_BaudRate 				= BaudRate;
	USART_InitStructure.USART_WordLength 			= USART_WordLength_8b;
	USART_InitStructure.USART_StopBits 				= USART_StopBits_1;
	USART_InitStructure.USART_Parity 				= USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode 					= USART_Mode_Rx | USART_Mode_Tx;
	// Configure USART
	USART_Init(USARTX, &USART_InitStructure);	
	//Enable USART1 Interrup
	USART_ITConfig(USARTX, USART_IT_RXNE, ENABLE);
	//Enable USART1	
	USART_Cmd(USARTX, ENABLE);
	while(USART_GetFlagStatus(USARTX, USART_FLAG_TC) == RESET)	;

	/*USART Tx/Rx Pin Configuration*/
	//Enable USART Tx/Rx Pin Clock
	if((USARTX==USART1) || (USARTX==USART2))
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	}
	else if (USARTX==USART3)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	}
	
	/*USART Tx/Rx Pin*/	
	if (USARTX == USART1)
	{
		//Parameters for Rx Pin
		GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_10;									
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		//Parameters for Tx Pin
		GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_9;									
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}
	else if (USARTX == USART2)
	{
		//Parameters for Rx Pin
		GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_3;//GPIO_Pin_2;									
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		//Parameters for Tx Pin
		GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_2;//GPIO_Pin_3;									
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}
	else if (USARTX == USART3)
	{
		//Parameters for Rx Pin
		GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_11;//GPIO_Pin_10;									
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		//Parameters for Tx Pin
		GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_10;//GPIO_Pin_9;									
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
	}

	/*Interrupt Configuration*/	
	if(USARTX == USART1)
	{																																											
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;																																												
	}
	else if (USARTX == USART2)
	{
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	}
	else if (USARTX == USART3)
	{
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	}																																																																																						
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;																																												
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;																																												
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;																																												
	NVIC_Init(&NVIC_InitStructure);	
	
}
/**
  * @brief  send a character from the specific USART(USART1~3)
  * @param  USARTx: system defined (USART1, USART2 ,USART3)
  * @param  _Data: data
  *
  * @retval None
  */

void USART_SendData_u8(USART_TypeDef* USARTx, u8 _Data)
{   
    /* Check the parameters */
  	assert_param(IS_USART_ALL_PERIPH(USARTx));
	USARTx->DR = _Data;
	while((USARTx->SR&0X40)==0);  
}


void USART1_IRQHandler(void)                
{ 

	u8 U1_RxData=0;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{

		U1_RxData =USART_ReceiveData(USART1); //Get Data		
		
	  	USART_SendData(USART1,U1_RxData); 	//Send back the Rx Data

	  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)	;
	}
	else
	{
	U1_RxData =USART_ReceiveData(USART1); //Get Data Á×§K¦]¬°¹L¸ü¤¤Â_²£¥Í·í¾÷¦ý¬OÁÙ¬O¤£§¹¾ã                   
	}
}
void USART2_IRQHandler(void)                
{ 

	u8 U2_RxData=0;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{

		U2_RxData = USART_ReceiveData(USART2); //Get Data
		
		timer_buf = U2_RxData;
		
		get_joystck_command();
				
	}
	else
	{
	U2_RxData =USART_ReceiveData(USART2); //Get Data Á×§K¦]¬°¹L¸ü¤¤Â_²£¥Í·í¾÷¦ý¬OÁÙ¬O¤£§¹¾ã                   
	}
}
void USART3_IRQHandler(void)                
{ 

	u8 		U3_RxData=0;
//	u16 	length=0;

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{

		U3_RxData = USART_ReceiveData(USART3); //Get Data

		gps_buf = U3_RxData;

		get_gps_data();

	}

	else
	{
	U3_RxData =USART_ReceiveData(USART3); //Get Data Á×§K¦]¬°¹L¸ü¤¤Â_²£¥Í·í¾÷¦ý¬OÁÙ¬O¤£§¹¾ã                   
	}
}

