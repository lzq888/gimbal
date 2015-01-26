#ifndef __SPI_H
#define __SPI_H
#include "stm32f10x.h"

/*Function Prototype*/ 				  	    													  
void SPIx_Init(void);			 
void SPI2_Init(void);
void SPIx_SetSpeed(u8 SpeedSet); //設置SPI速度
void SPI2_SetSpeed(u8 SpeedSet);    
u8 SPIx_ReadWriteByte(u8 TxData);//SPI匯流排讀寫一個位元組
u8 SPI2_ReadWriteByte(u8 TxData);//SPI匯流排讀寫一個位元組

		 
#endif


