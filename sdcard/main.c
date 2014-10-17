/*************************************************************
************************A R M S*******************************
**************************************************************
Name	: SD card Driver Example
Author	: Alex Huang
Version	: V1.0
Description: example of how to creat a .txt file in the SD Card
			 via FatFs.
**************************************************************/
#include "pervia.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "spi.h"
#include "flash.h"
#include "sd.h"	
#include "ff.h"
#include "string.h"

/*全域變數*/
//FatFs
FATFS fs; 		// Work area (file system object) for logical drive
FIL fsrc, fdst; // file objects
FRESULT res; 	// FatFs function common result code
UINT br, bw; 	// File R/W count
DIR dir;
FILINFO fileInfo;
//txt
unsigned char buffer[512];
//根目錄
const char filedir[]="0:/";
//file path
char root_filedir[50][50]; //根目錄下文件夾路徑,預設最多50個文件夾. 不支援長檔名，文件夾名稱最多8個字
u8 nbof_rootDoc = 0;//紀錄根目錄下有幾個資料夾

int main(void)
{	
	float SD_Size = 0;//SD卡容量
	u32 a,br ;//讀取資料流計數

	Initial_STM32();//Initial STM32
	LED_Configuration();//Initial LED Pins
	USART_Configuration();//Initial USART
	SPI_Flash_Init();//Initial Flash
	while(SD_Initialize())//初始化SD卡
	{
		printf("Please insert SD Card.\r\n");
		DELAY_mS(1000);	
	}
		SD_Size = SD_GetSectorCount();	
		SD_Size = (SD_Size*512.0f)/1024.0f/1024.0f/1024.0f ;		
		//Show SD Card Size
		printf("SD Card Size:%f GB\r\n",SD_Size);
	/*讀取文件檔*/
	f_mount(0 , &fs);			  //註冊工作  掛載
	res = f_opendir(&dir,filedir);//打開指定目錄下文件夾
	if(res==FR_OK)
	{	
		
		res = f_open(&fsrc, "DCIM/109___01/IMG_5542.JPG", FA_READ);
		if(!res)
		{
			printf("open news.txt : %d\r\n",res);
			br=1;
			a=0;
			printf("File Content:\r\n");
			for (;;)
			{
				for(a=0; a<512; a++) buffer[a]=0;
				res = f_read(&fsrc, buffer, sizeof(buffer), &br);
				printf("%s\r\n",buffer);
				if (res || br < sizeof(buffer)) break; // error or eof
			}
		}
		f_close(&fsrc); //關閉資料流		
	}
	f_mount(0,NULL);//卸載儲存註冊區
	while(1)
	{	
	 	  
	  if(GPIO_ReadOutputDataBit(LED1_Port,LED1_Pin))
	  {
     	LED1_ON();
		LED2_OFF();
		DELAY_mS(1000);
	  }
	  else
	  {
	  	LED1_OFF();
		LED2_ON();
		DELAY_mS(1000);	  	
	  }
	}
}

