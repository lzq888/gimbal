#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
/*Define key input status*/ 
//Left
#define keyLeft 	GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4)
//Up
#define KeyUp 		GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)
//Right
#define keyRight	GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10)
//Down
#define keyDown		GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12)
//Press
#define keyPress	GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11)

#define KEY_LEFT	1	
#define KEY_UP		2
#define KEY_RIGHT	3
#define KEY_DOWN	4
#define KEY_PRESS	5

/*Function Prototype*/
void Initial_5DSwitch(void);
u8 KeyScan(void);

#endif
