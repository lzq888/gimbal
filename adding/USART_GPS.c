
#include <stdlib.h>
#include "USART_GPS.h"
#include "global.h"
#include "bool.h"
#include <string.h>

int 	data = 0;
char	SNMEA[Max_Length];
char	Output_GPRMC[Max_Length];
u8		NMEA_Length			=0;
bool	IsSave				=FALSE;
bool	OutputOK			=FALSE;
char	USART3_Data;
int  	send = 0 , j = 0;

void GPS_Configuration(USART_TypeDef* USARTX, uint32_t BaudRate)
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

	//Enable AFIO Clock
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	//Parameters for USART	
	USART_InitStructure.USART_BaudRate 				= BaudRate;
	USART_InitStructure.USART_WordLength 			= USART_WordLength_8b;
	USART_InitStructure.USART_StopBits 				= USART_StopBits_1;
	USART_InitStructure.USART_Parity 				= USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode 					= USART_Mode_Rx | USART_Mode_Tx;
	// Configure USART
	USART_Init(USARTX, &USART_InitStructure);	
	//Enable USART3 Interrup
	USART_ITConfig(USARTX, USART_IT_RXNE, ENABLE);
	//Enable USART3	
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
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;																																												
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;																																												
	NVIC_Init(&NVIC_InitStructure);		
	
}


bool findGPRMC()
{
		
	if(SNMEA[1]=='G' && SNMEA[2]=='P' && SNMEA[3]=='G' && 
	   SNMEA[4]=='G' && SNMEA[5]=='A')
	{
		
		return TRUE;
	}

		return FALSE;
}

void parseGPS(char c1)
{
		if(c1=='$'){	
			IsSave=TRUE;
			NMEA_Length=0;
		}
		else{
		}

		if(IsSave==TRUE)
		{	
			if(NMEA_Length<Max_Length)
			{
				SNMEA[NMEA_Length]=c1;
				
                if(SNMEA[18] == ','){
	              if(findGPRMC()==TRUE)
			          {
                        int j = 0 ;
						for(j=0;j<NMEA_Length;j++)
						{		
							data = data +1 ;					
							Output_GPRMC[j]=SNMEA[j];
						}	
						send = 1;
						IsSave=FALSE;
						NMEA_Length=0;
                  }
                }  
              NMEA_Length++;  
			}

			else if(NMEA_Length == Max_Length){
				//send = 0;
				//data = data +1 ;
				if(findGPRMC()==TRUE)
					{
						//data = data +1 ;
						int j = 0 ;
						for(j=0;j<NMEA_Length;j++)
						{		
							data = data +1 ;					
							Output_GPRMC[j]=SNMEA[j];
						}	
						send = 1;
						IsSave=FALSE;
						NMEA_Length=0;
					}
				else
					{
					}
			}
/*			else if (NMEA_Length > Max_Length)
			{
				IsSave=FALSE;
				NMEA_Length=0;
			}*/
		}
		else{}
}

void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  	{

		USART3_Data = USART3->DR;
		parseGPS(USART3_Data);
	}
	
	else
	{	
		USART3_Data = 0x00;		
	}
}



/*
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  	{
  		//USART_ClearFlag(USART3,USART_FLAG_ORE);
		data = data +1;	
		USART3_Data = USART3->DR;
		//parseGPS(USART3_Data);
		GPS[i] =  USART_ReceiveData(USART3);
          i++;
          if(i==10){
            send = 1;
            i=0; 
            } 
	}
	
	else
	{	

		USART3_Data = 0x00;
		
	}
}
*/
