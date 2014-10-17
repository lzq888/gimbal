#include "key.h"
#include "pervia.h"

//*********************************************************
// Function: Initial 5 Direction Switch
// Input:  N/A
// Return: N/A
// Remark: N/A
//*********************************************************
void Initial_5DSwitch(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//Enable Clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);		
	//Configure Pins
	GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;									
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//Pull High All Pins
	GPIO_SetBits(GPIOC,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);	
}

//*********************************************************
// Function: Scanning the key input status
// Input:  N/A
// Return: 	
//			@ 0 :No key input
//			@ 1 :Key Left
//			@ 2 :Key Up
//			@ 3 :Key Right
//			@ 4 :Key Down
//			@ 5 :key Press	
// Remark: N/A
//*********************************************************
u8 KeyScan(void)
{
	DELAY_mS(10); //To eliminate the vibration 
	if(KeyUp==0)	return KEY_UP ;
	else if (keyDown==0)  return KEY_DOWN;
	else if (keyLeft==0)  return KEY_LEFT;
	else if (keyRight==0) return KEY_RIGHT;
	else if (keyPress==0) return KEY_PRESS;
	else return 0 ;
}
