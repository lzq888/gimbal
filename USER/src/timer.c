

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "timer.h"
#include "global.h"

u16  timer_counter=0;
u16  Mean_filter_output_last[4]={0,0,0,0};

u16 	pwmvalue_a;
u16 	pwmvalue_b;	
u16 	pwmvalue_c;
u16 	pwmvalue_d;

void TIMER_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;		
	
						
    /*TIMER4 */	
    TIM_TimeBaseStructure.TIM_Period =19999;			  			//period
  	TIM_TimeBaseStructure.TIM_Prescaler = 71;  						//high voltage portion(between 0-period)	
  	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM4, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);        				//enable timer interrupt
	
	/*72MHz/((19999+1)*(71+1))=50Hz*/	

    /*TIMER3 */	
    TIM_TimeBaseStructure.TIM_Period =4999;			  				//high voltage portion(between 0-period)
  	TIM_TimeBaseStructure.TIM_Prescaler = 719;		
  	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM3, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);		   				//enable timer interrupt
	
	/*72MHz/((4999+1)*(719+1))=20Hz*/																																																							
}

void PWMoutputA(u16 i) 													
{
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse =i;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);		 //TIM_OC3(channel~查表)Init(TIM4, &TIM_OCInitStructure);
}
void PWMoutputB(u16 j)
{
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse =j;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);         //TIM_OC3(channel~查表
	
}

void PWMoutputC(u16 m)
{
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse =m;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);         //TIM_OC3(channel~查表
	
}

void PWMoutputD(u16 n)
{
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse =n;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);         //TIM_OC3(channel~查表
	
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 
	{
		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

	}		
}


void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) 
	{	 		  
		
	
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

	}
}

