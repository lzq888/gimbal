#include "spi.h"

SPI_InitTypeDef  SPI1_InitStructure;
SPI_InitTypeDef  SPI2_InitStructure;


/*SPI1*/
void SPIx_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  	//Enable Clock
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE );	
 	//Pin Configuration
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //複用推挽輸出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
	//SPI Configuration
	SPI1_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //設置SPI單向或者雙向的資料模式:SPI設置為雙線雙向全雙工
	SPI1_InitStructure.SPI_Mode = SPI_Mode_Master;		//設置SPI工作模式:設置為主SPI
	SPI1_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//設置SPI的數據大小:SPI發送接收8位元幀結構
	SPI1_InitStructure.SPI_CPOL = SPI_CPOL_High;		//選擇了串列時鐘的穩態:時鐘懸空高
	SPI1_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//資料捕獲於第二個時鐘沿
	SPI1_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信號由硬體（NSS管腳）還是軟體（使用SSI位元）管理:內部NSS信號有SSI位元控制
	SPI1_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定義串列傳輸速率預分頻的值:串列傳輸速率預分頻值為256
	SPI1_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定資料傳輸從MSB位還是LSB位開始:資料傳輸從MSB位開始
	SPI1_InitStructure.SPI_CRCPolynomial = 7;	//CRC值計算的多項式
	SPI_Init(SPI1, &SPI1_InitStructure);  //根據SPI_InitStruct中指定的參數初始化外設SPIx寄存器
 
	SPI_Cmd(SPI1, ENABLE); //使能SPI外設
	
	SPIx_ReadWriteByte(0xff);//啟動傳輸		 
}   


/*SPI2*/
void SPI2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  	//Enable Clock
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB|RCC_APB1Periph_SPI2, ENABLE );	
 	//Pin Configuration
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //複用推挽輸出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	//SPI Configuration
	SPI2_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  
	SPI2_InitStructure.SPI_Mode = SPI_Mode_Master;		
	SPI2_InitStructure.SPI_DataSize = SPI_DataSize_8b;		
	SPI2_InitStructure.SPI_CPOL = SPI_CPOL_High;		//CPOL=1
	SPI2_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//CPHA=1
	SPI2_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS software
	SPI2_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定義串列傳輸速率預分頻的值:串列傳輸速率預分頻值為256
	SPI2_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定資料傳輸從MSB位還是LSB位開始:資料傳輸從MSB位開始
	SPI2_InitStructure.SPI_CRCPolynomial = 7;	//CRC值計算的多項式
	SPI_Init(SPI2, &SPI2_InitStructure);  //根據SPI_InitStruct中指定的參數初始化外設SPIx寄存器
 
	SPI_Cmd(SPI2, ENABLE); //使能SPI外設
	
	SPI2_ReadWriteByte(0xff);//啟動傳輸		 
}   
//SPI 速度設置函數
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分頻   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8分頻   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16分頻  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256分頻 (SPI 281.25K@sys 72M)
  
void SPIx_SetSpeed(u8 SpeedSet)
{
	SPI1_InitStructure.SPI_BaudRatePrescaler = SpeedSet ;
  	SPI_Init(SPI1, &SPI1_InitStructure);
	SPI_Cmd(SPI1,ENABLE);
} 

//SPI 速度設置函數
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分頻   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8分頻   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16分頻  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256分頻 (SPI 281.25K@sys 72M)
  
void SPI2_SetSpeed(u8 SpeedSet)
{
	SPI2_InitStructure.SPI_BaudRatePrescaler = SpeedSet ;
  	SPI_Init(SPI2, &SPI2_InitStructure);
	SPI_Cmd(SPI2,ENABLE);
}


u8 SPIx_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //檢查指定的SPI標誌位元設置與否:發送緩存空標誌位元
		{
			retry++;
			if(retry>200)return 0;
		}			  

	SPI_I2S_SendData(SPI1, TxData); //通過外設SPIx發送一個資料
	retry=0;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); //檢查指定的SPI標誌位元設置與否:接受緩存非空標誌位元
		{
			retry++;
			if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI1); //返回通過SPIx最近接收的資料
}






u8 SPI2_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //檢查指定的SPI標誌位元設置與否:發送緩存空標誌位元
		{
			retry++;
			if(retry>200)return 0;
		}			  

	SPI_I2S_SendData(SPI2, TxData); //通過外設SPIx發送一個資料
	retry=0;
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); //檢查指定的SPI標誌位元設置與否:接受緩存非空標誌位元
		{
			retry++;
			if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI2); //返回通過SPIx最近接收的資料
}
