#include <stdlib.h>
#include "exti.h"

void EXTI_Configuration(void)
{
  	EXTI_InitTypeDef EXTI_InitStructure;        						//EXTI初始化

	EXTI_ClearITPendingBit(EXTI_Line2);							    	//清除中斷標誌
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;				 			//路線選擇

	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;					//事件選擇
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling ;		//触發模式
	
 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;							//啟動中斷
 	EXTI_Init(&EXTI_InitStructure);										//初始化
	EXTI_GenerateSWInterrupt(EXTI_Line2);
}


void EXTI2_IRQHandler(void)
{
   	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
  	{

	LED1_OFF();
	DELAY_mS(100);
	LED1_ON();

	EXTI_ClearITPendingBit(EXTI_Line2); 
  	} 
}

