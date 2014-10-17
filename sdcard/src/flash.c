#include "flash.h" 
#include "spi.h"
#include "pervia.h"
//4Kbytes為一個Sector
//16個磁區為1個Block
//W25X16
//容量為2M位元組,共有32個Block,512個Sector 

//初始化SPI FLASH的IO口
int* ptr = NULL;

void SPI_Flash_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	SPI_FLASH_CS_CLK,ENABLE);	 

    GPIO_InitStructure.GPIO_Pin = SPI_FLASH_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_FLASH_CS_PORT, &GPIO_InitStructure);
	GPIO_SetBits(SPI_FLASH_CS_PORT,SPI_FLASH_CS_PIN);
	Clr_SPI_FLASH_CS;
	SPIx_Init();
	Set_SPI_FLASH_CS; 

	//設置SD Card的CS並且拉高
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
}  

//讀取SPI_FLASH的狀態寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:預設0,狀態寄存器保護位元,配合WP使用
//TB,BP2,BP1,BP0:FLASH區域防寫設置
//WEL:寫使能鎖定
//BUSY:忙標記位元(1,忙;0,空閒)
//默認:0x00
u8 SPI_Flash_ReadSR(void)   
{  
	u8 byte=0;   
	Clr_SPI_FLASH_CS;                            //使能器件   
	SPIx_ReadWriteByte(W25X_ReadStatusReg);    //發送讀取狀態寄存器命令    
	byte=SPIx_ReadWriteByte(0Xff);             //讀取一個位元組  
	Set_SPI_FLASH_CS;                            //取消片選     
	return byte;   
} 
//寫SPI_FLASH狀態寄存器
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以寫!!!
void SPI_FLASH_Write_SR(u8 sr)   
{   
	Clr_SPI_FLASH_CS;                            //使能器件   
	SPIx_ReadWriteByte(W25X_WriteStatusReg);   //發送寫取狀態寄存器命令    
	SPIx_ReadWriteByte(sr);               //寫入一個位元組  
	Set_SPI_FLASH_CS;                            //取消片選     	      
}   
//SPI_FLASH寫使能	
//將WEL置位   
void SPI_FLASH_Write_Enable(void)   
{
	Clr_SPI_FLASH_CS;                            //使能器件   
    SPIx_ReadWriteByte(W25X_WriteEnable);      //發送寫使能  
	Set_SPI_FLASH_CS;                            //取消片選     	      
} 
//SPI_FLASH寫禁止	
//將WEL清零  
void SPI_FLASH_Write_Disable(void)   
{  
	Clr_SPI_FLASH_CS;                            //使能器件   
    SPIx_ReadWriteByte(W25X_WriteDisable);     //發送寫禁止指令    
	Set_SPI_FLASH_CS;                            //取消片選     	      
} 			    
//讀取晶片ID W25X16的ID:0XEF14
u16 SPI_Flash_ReadID(void)
{
	u16 Temp = 0;	  
	Clr_SPI_FLASH_CS;				    
	SPIx_ReadWriteByte(0x90);//發送讀取ID命令	    
	SPIx_ReadWriteByte(0x00); 	    
	SPIx_ReadWriteByte(0x00); 	    
	SPIx_ReadWriteByte(0x00); 	 			   
	Temp|=SPIx_ReadWriteByte(0xFF)<<8;  
	Temp|=SPIx_ReadWriteByte(0xFF);	 
	Set_SPI_FLASH_CS;				    
	return Temp;
}   		    
//讀取SPI FLASH  
//在指定位址開始讀取指定長度的資料
//pBuffer:資料存儲區
//ReadAddr:開始讀取的位址(24bit)
//NumByteToRead:要讀取的位元組數(最大65535)
void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;    												    
	Clr_SPI_FLASH_CS;                            //使能器件   
    SPIx_ReadWriteByte(W25X_ReadData);         //發送讀取命令   
    SPIx_ReadWriteByte((u8)((ReadAddr)>>16));  //發送24bit地址    
    SPIx_ReadWriteByte((u8)((ReadAddr)>>8));   
    SPIx_ReadWriteByte((u8)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPIx_ReadWriteByte(0XFF);   //迴圈讀數  
    }
	Set_SPI_FLASH_CS;                            //取消片選     	      
}  
//SPI在一頁(0~65535)內寫入少於256個位元組的資料	(一頁為一個block)
//在指定位址開始寫入最大256位元組的資料
//pBuffer:資料存儲區
//WriteAddr:開始寫入的位址(24bit)
//NumByteToWrite:要寫入的位元組數(最大256),該數不應該超過該頁的剩餘位元組數!!!	 
void SPI_Flash_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;  
    SPI_FLASH_Write_Enable();                  //SET WEL 
	Clr_SPI_FLASH_CS;                            //使能器件   
    SPIx_ReadWriteByte(W25X_PageProgram);      //發送寫頁命令   
    SPIx_ReadWriteByte((u8)((WriteAddr)>>16)); //發送24bit地址    
    SPIx_ReadWriteByte((u8)((WriteAddr)>>8));   
    SPIx_ReadWriteByte((u8)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)SPIx_ReadWriteByte(pBuffer[i]);//迴圈寫數  
	Set_SPI_FLASH_CS;                            //取消片選 
	SPI_Flash_Wait_Busy();					   //等待寫入結束
} 
//無檢驗寫SPI FLASH 
//必須確保所寫的位址範圍內的資料全部為0XFF,否則在非0XFF處寫入的資料將失敗!
//具有自動換頁功能 
//在指定位址開始寫入指定長度的資料,但是要確保位址不越界!
//pBuffer:資料存儲區
//WriteAddr:開始寫入的位址(24bit)
//NumByteToWrite:要寫入的位元組數(最大65535)
//CHECK OK
void SPI_Flash_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //單頁剩餘的位元組數		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大於256個位元組
	while(1)
	{	   
		SPI_Flash_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//寫入結束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //減去已經寫入了的位元組數
			if(NumByteToWrite>256)pageremain=256; //一次可以寫入256個位元組
			else pageremain=NumByteToWrite; 	  //不夠256個位元組了
		}
	};	    
} 
//寫SPI FLASH  
//在指定位址開始寫入指定長度的資料
//該函數帶擦除操作!
//pBuffer:資料存儲區
//WriteAddr:開始寫入的位址(24bit)
//NumByteToWrite:要寫入的位元組數(最大65535)  		   
u8 SPI_FLASH_BUF[4096];
void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    

	secpos=WriteAddr/4096;//磁區位址 0~511 for w25x16 (第幾個sector, datasheet pp.09)
	secoff=WriteAddr%4096;//在磁區內的偏移
	secremain=4096-secoff;//磁區剩餘空間大小   

	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大於4096個位元組
	while(1) 
	{	
		SPI_Flash_Read(SPI_FLASH_BUF,secpos*4096,4096);//讀出整個磁區的內容
		for(i=0;i<secremain;i++)//校驗資料
		{
			if(SPI_FLASH_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			SPI_Flash_Erase_Sector(secpos);//擦除這個磁區
			for(i=0;i<secremain;i++)	   //複製
			{
				SPI_FLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			SPI_Flash_Write_NoCheck(SPI_FLASH_BUF,secpos*4096,4096);//寫入整個磁區  

		}else SPI_Flash_Write_NoCheck(pBuffer,WriteAddr,secremain);//寫已經擦除了的,直接寫入磁區剩餘區間. 				   
		if(NumByteToWrite==secremain)break;//寫入結束了
		else//寫入未結束
		{
			secpos++;//磁區位址增1
			secoff=0;//偏移位置為0 	 

		   	pBuffer+=secremain;  //指針偏移
			WriteAddr+=secremain;//寫位址偏移	   
		   	NumByteToWrite-=secremain;				//位元組數遞減
			if(NumByteToWrite>4096)secremain=4096;	//下一個磁區還是寫不完
			else secremain=NumByteToWrite;			//下一個磁區可以寫完了
		}	 
	};	 	 
}
//擦除整個晶片
//整片擦除時間:
//W25X16:25s 
//W25X32:40s 
//W25X64:40s 
//等待時間超長...
void SPI_Flash_Erase_Chip(void)   
{                                             
    SPI_FLASH_Write_Enable();                  //SET WEL 
    SPI_Flash_Wait_Busy();   
  	Clr_SPI_FLASH_CS;                            //使能器件   
    SPIx_ReadWriteByte(W25X_ChipErase);        //發送片擦除命令  
	Set_SPI_FLASH_CS;                            //取消片選     	      
	SPI_Flash_Wait_Busy();   				   //等待晶片擦除結束
}   
//擦除一個磁區
//Dst_Addr:磁區位址 0~511 for w25x16
//擦除一個扇區的最少時間:150ms
void SPI_Flash_Erase_Sector(u32 Dst_Addr)   
{   
	Dst_Addr*=4096;
    SPI_FLASH_Write_Enable();                  //SET WEL 	 
    SPI_Flash_Wait_Busy();   
  	Clr_SPI_FLASH_CS;                            //使能器件   
    SPIx_ReadWriteByte(W25X_SectorErase);      //發送磁區擦除指令 
    SPIx_ReadWriteByte((u8)((Dst_Addr)>>16));  //發送24bit地址    
    SPIx_ReadWriteByte((u8)((Dst_Addr)>>8));   
    SPIx_ReadWriteByte((u8)Dst_Addr);  
	Set_SPI_FLASH_CS;                            //取消片選     	      
    SPI_Flash_Wait_Busy();   				   //等待擦除完成
}  
//等待空閒
void SPI_Flash_Wait_Busy(void)   
{   
	while ((SPI_Flash_ReadSR()&0x01)==0x01);   // 等待BUSY位清空
}  
//進入掉電模式
void SPI_Flash_PowerDown(void)   
{ 
  	Clr_SPI_FLASH_CS;                            //使能器件   
    SPIx_ReadWriteByte(W25X_PowerDown);        //發送掉電命令  
	Set_SPI_FLASH_CS;                            //取消片選     	      
    DELAY_uS(3);                               //等待TPD  
}   
//喚醒
void SPI_Flash_WAKEUP(void)   
{  
  	Clr_SPI_FLASH_CS;                            //使能器件   
    SPIx_ReadWriteByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
	Set_SPI_FLASH_CS;                            //取消片選     	      
    DELAY_uS(3);                               //等待TRES1
}   

