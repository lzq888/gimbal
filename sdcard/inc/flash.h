#ifndef __FLASH_H
#define __FLASH_H			    
#include "stm32f10x.h"


#define SPI_FLASH_CS_PORT              GPIOA
#define SPI_FLASH_CS_CLK               RCC_APB2Periph_GPIOA  
#define SPI_FLASH_CS_PIN               GPIO_Pin_8

#define Set_SPI_FLASH_CS  {GPIO_SetBits(SPI_FLASH_CS_PORT,SPI_FLASH_CS_PIN);}
#define Clr_SPI_FLASH_CS  {GPIO_ResetBits(SPI_FLASH_CS_PORT,SPI_FLASH_CS_PIN);} 					 
////////////////////////////////////////////////////////////////////////////
//W25X16
#define FLASH_ID 0XEF14
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 

void SPI_Flash_Init(void);
u16  SPI_Flash_ReadID(void);  	    //讀取flashID
u8	 SPI_Flash_ReadSR(void);        //讀狀態既存器 
void SPI_FLASH_Write_SR(u8 sr);  	//寫狀態既存器
void SPI_FLASH_Write_Enable(void);  //使能寫 
void SPI_FLASH_Write_Disable(void);	//失能寫
void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //讀取flash
void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//寫入flash
void SPI_Flash_Erase_Chip(void);    	  //整片清除
void SPI_Flash_Erase_Sector(u32 Dst_Addr);//Sector清除
void SPI_Flash_Wait_Busy(void);           //等待忙碌結束
void SPI_Flash_PowerDown(void);           //省電模式
void SPI_Flash_WAKEUP(void);			  //叫醒功能

#endif

