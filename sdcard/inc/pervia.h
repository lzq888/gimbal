#ifndef __PERVIA_H
#define __PERVIA_H

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include <stdio.h>

/*Function Prototype*/
//STM32 Initial
void Initial_STM32(void);//Initial STM32 
						 //@ 72 MHz
						 //@ NVIC_Group = 0
						 //@ LED1 and LED2 Pin Setting 
						 //@ 5-Direction Switch Pin Setting
//Delay Functions
void DELAY_mS( unsigned short ms );	//Delay function in ms
void DELAY_uS( unsigned short us );	//Delay function in us

#endif
