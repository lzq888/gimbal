#include "sd.h"			   
#include "spi.h"

u8  SD_Type=0;//SD¥dÃþ«¬ 

//¨Ì·ÓSD Card±µ¨ìªºSPI°µÅª¼g°Ê°µ
//data:­n¼g¤Jªº¼Æ¾Ú
//return:Åª¨ìªº¼Æ¾Ú
u8 SD_SPI_ReadWriteByte(u8 data)
{
	return SPIx_ReadWriteByte(data);
}
//SDªì©l¤Æªº®É­Ô»Ý­n¨Ï¥Î§C³t³]©w
void SD_SPI_SpeedLow(void)
{
 	SPIx_SetSpeed(SPI_BaudRatePrescaler_256);//³]¸m¨ì§C³t¼Ò¦¡	
}
//SD¥¿±`¹B§@®É¨Ï¥Î°ª³t
void SD_SPI_SpeedHigh(void)
{
 	SPIx_SetSpeed(SPI_BaudRatePrescaler_8);//³]¸m¹D°ª³t	
}
//SPI¥~³]ªì©l¤Æ
void SD_SPI_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	SD_CS_CLK,ENABLE);	 

    GPIO_InitStructure.GPIO_Pin = SD_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;//Push-pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SD_CS_PORT, &GPIO_InitStructure);
	Clr_SD_CS;
	SPIx_Init();
	Set_SD_CS;
}
///////////////////////////////////////////////////////////////////////////////////
//¨ú®øSD Card¿ï¾Ü
void SD_DisSelect(void)
{
	Set_SD_CS;
	SD_SPI_ReadWriteByte(0xff);//ªÅ¾Þ§@
}
//¿ï¾ÜSD Card¡Aµ¥«Ý¥d·Ç³Æ§¹¦¨¶i¦æ¤U¤@­Ó°Ê§@
//Retrun:0,¦¨¥\;1,¥¢±Ñ;
u8 SD_Select(void)
{
	Clr_SD_CS;
	if(SD_WaitReady()==0)return 0;//µ¥«Ý¦¨¥\
	//
	SD_DisSelect();
	return 1;//µ¥«Ý¥¢±Ñ
}
//µ¥«ÝSD Card·Ç³Æ¦n
//return:0,·Ç³Æ¦n;¨ä¥L­È,¥¢±Ñ
u8 SD_WaitReady(void)
{
	u32 t=0;
	do
	{
		if(SD_SPI_ReadWriteByte(0XFF)==0XFF)return 0;//OK
		t++;		  	
	}while(t<0XFFFFFF);//¥¢±Ñ 
	return 1;
}
//µ¥«ÝSD¥d¦^À³
//Response:­n±o¨ìªº¦^À³­È
//return:0,¦¨¥\±o¨ì¤F¸Ó¦^À³­È
//     ¨ä¥L,±o¨ì¦^À³­È¥¢±Ñ
u8 SD_GetResponse(u8 Response)
{
	u16 Count=0xFFF;//µ¥«Ý¦¸¼Æ	   						  
	while ((SD_SPI_ReadWriteByte(0XFF)!=Response)&&Count)Count--;//µ¥«Ý±o¨ì·Ç½Tªº¦^À³  	  
	if (Count==0)return MSD_RESPONSE_FAILURE;//±o¨ì¦^À³¥¢±Ñ   
	else return MSD_RESPONSE_NO_ERROR;//¥¿½T¦^À³
}
//±qSD¥dÅª¨ú¤@­Ó¼Æ¾Ú¥]ªº¤º®eÈÝ
//buf:¼Æ¾Ú½w½Ä°ÏÇø
//len:­nÅª¨úªº¼Æ¾Úªø«×
//Retrun:0,¦¨¥\;1,¥¢±Ñ;	
u8 SD_RecvData(u8*buf,u16 len)
{			  	  
	if(SD_GetResponse(0xFE))return 1;
    while(len--)
    {
        *buf=SPIx_ReadWriteByte(0xFF);
        buf++;
    }
 
    SD_SPI_ReadWriteByte(0xFF);
    SD_SPI_ReadWriteByte(0xFF);									  					    
    return 0;
}
//¦VSD¥dÁÂ¤J¤@­Ó¼Æ¾Ú¥]ªº¤º®e,512 Bytes
//buf:¼Æ¾Ú¼È¦s°ÏÇø
//cmd:«ü¥O
//Retrun:0,¦¨¥\;1,¥¢±Ñ;		
u8 SD_SendBlock(u8*buf,u8 cmd)
{	
	u16 t;		  	  
	if(SD_WaitReady())return 1;//µÈ´ý×¼±¸Ê§Ð§
	SD_SPI_ReadWriteByte(cmd);
	if(cmd!=0XFD)//²»ÊÇ½áÊøÖ¸Áî
	{
		for(t=0;t<512;t++)SPIx_ReadWriteByte(buf[t]);//Ìá¸ßËÙ¶È,¼õÉÙº¯Êý´«²ÎÊ±¼ä
	    SD_SPI_ReadWriteByte(0xFF);//ºöÂÔcrc
	    SD_SPI_ReadWriteByte(0xFF);
		t=SD_SPI_ReadWriteByte(0xFF);//½ÓÊÕÏìÓ¦
		if((t&0x1F)!=0x05)return 2;//ÏìÓ¦´íÎó									  					    
	}						 									  					    
    return 0;//Ð´Èë³É¹¦
}

//¦VSD¥dµo°e¤@­Ó©R¥O
//¿é¤JÈë: u8 cmd   ©R¥O 
//      u32 arg  ©R¥O°Ñ¼Æ
//      u8 crc   CRCÀË¬d­È	   
//ªð¦^­È:SD¥d¦^¶Çªº­È														  
u8 SD_SendCmd(u8 cmd, u32 arg, u8 crc)
{
    u8 r1;	
	u8 Retry=0; 
	SD_DisSelect();//¨ú®ø¤ù¿ï
	if(SD_Select())return 0XFF;//¤ù¿ï¥¢±Ñ 
	//·¢ËÍ
    SD_SPI_ReadWriteByte(cmd | 0x40);//¤À§O¼g¤J©R¥O
    SD_SPI_ReadWriteByte(arg >> 24);
    SD_SPI_ReadWriteByte(arg >> 16);
    SD_SPI_ReadWriteByte(arg >> 8);
    SD_SPI_ReadWriteByte(arg);	  
    SD_SPI_ReadWriteByte(crc); 
	if(cmd==CMD12)SD_SPI_ReadWriteByte(0xff);//Skip a stuff byte when stop reading
    //µ¥«ÝSD¥d¦^À³¡A©Î¬O¶W®É°h¥X
	Retry=0X1F;
	do
	{
		r1=SD_SPI_ReadWriteByte(0xFF);
	}while((r1&0X80) && Retry--);	 
	//ªð¦^ª¬ºA­È
    return r1;
}		    																			  
//Åª¨úSD¥dªºCID«H®§¡A¥]¬A¨î³y°Ó«H®§
//¿é¤J: u8 *cid_data(¦s©ñCIDªº¤º¦s¡A¦Ü¤Ö16Byte¡^	  
//ªð¦^­È:0¡GNO_ERR
//		 1:¿ù»~														   
u8 SD_GetCID(u8 *cid_data)
{
    u8 r1;	   
    //µo°eCMD10©R¥O,ÅªCID
    r1=SD_SendCmd(CMD10,0,0x01);
    if(r1==0x00)
	{
		r1=SD_RecvData(cid_data,16);//±µ¦¬16­ÓByte¼Æ¾Ú	 
    }
	SD_DisSelect();
	if(r1)return 1;
	else return 0;
}																				  
//Åª¨úSD¥dªºCSD«H®§¡A¥]§t®e¶q»P³t«×«H®§
//¿é¤J: u8 *cid_data(¦s©ñCIDªº¤º¦s¡A¦Ü¤Ö16Byte¡^	  
//ªð¦^­È:0¡GNO_ERR
//		 1:¿ù»~														   
u8 SD_GetCSD(u8 *csd_data)
{
    u8 r1;	 
    r1=SD_SendCmd(CMD9,0,0x01);//µo°eCMD9©R¥O¡AÅªCSD
    if(r1==0)
	{
    	r1=SD_RecvData(csd_data, 16);//±µ¦¬16­ÓByte¼Æ¾Ú
    }
	SD_DisSelect();
	if(r1)return 1;
	else return 0;
}  
//Àò¨úSD¥dÁ`®°°Ï¼Æ¶q   
//ªð¦^­È:0,Åª¨ú®e¶q¥X¿ù
//       ¨ä¥L,SD¥d®e¶q(®°°Ï¼Æ/512Byte)
//¨C­Ó®°°ÏªºByte¼Æ¥²©w¬°512¡A¦pªG¤£¬O512«hªì©l¤£¯à³q¹L														  
u32 SD_GetSectorCount(void)
{
    u8 csd[16];
    u32 Capacity;  
    u8 n;
	u16 csize;  					    
	//Åª¨úCSD«H®§¡A¦pªG¤¤¶¡¥X¿ù«hªð¦^0
    if(SD_GetCSD(csd)!=0) return 0;	    
    //¦pªGºûSDHC¥d¡A«ö·Ó¤U­±¤è¦¡­pºâ
    if((csd[0]&0xC0)==0x40)	 //V2.00ªº¥d
    {	
		csize = csd[9] + ((u16)csd[8] << 8) + 1;
		Capacity = (u32)csize << 10;//®°°Ï¼Æ	 		   
    }else//V1.XXªº¥d
    {	
		n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
		csize = (csd[8] >> 6) + ((u16)csd[7] << 2) + ((u16)(csd[6] & 3) << 10) + 1;
		Capacity= (u32)csize << (n - 9);//±o¨ì®°°Ï¼Æ   
    }
    return Capacity;
}
u8 SD_Idle_Sta(void)
{
	u16 i;
	u8 retry;	   	  
    for(i=0;i<0xf00;i++);	 
    for(i=0;i<10;i++)SPIx_ReadWriteByte(0xFF); 
    retry = 0;
    do
    {	   
        i = SD_SendCmd(CMD0, 0, 0x95);
        retry++;
    }while((i!=0x01)&&(retry<200));
    if(retry==200)return 1; 
	return 0;				  
}
//ªì©l¤ÆSD¥d
u8 SD_Initialize(void)
{
    u8 r1;      // ¦s©ñSD¥dªð¦^­È
    u16 retry;  // ¥Î¨Ó¶i¦æ¶W®É­p¼Æ
    u8 buf[4];  
	u16 i;

	SD_SPI_Init();		//ªì©l¤Æ¾Þ§@IO
 	SD_SPI_SpeedLow();	//³]¸m¨ì§C³t¼Ò¦¡ 	  
	for(i=0;i<10;i++)SD_SPI_ReadWriteByte(0XFF);//³Ì¤Öµo°e74­Ó¯ß½Ä
	retry=20;
	do
	{
		r1=SD_SendCmd(CMD0,0,0x95);//¶i¤JIDLEª¬ºA
	}while((r1!=0X01) && retry--);
 	SD_Type=0;//¹w³]¨S¦³´¡¤JSD¥d
	if(r1==0X01)
	{
		if(SD_SendCmd(CMD8,0x1AA,0x87)==1)//SD V2.0
		{
			for(i=0;i<4;i++)buf[i]=SD_SPI_ReadWriteByte(0XFF);	//Get trailing return value of R7 resp
			if(buf[2]==0X01&&buf[3]==0XAA)//SD¥d¬O§_¤ä¤¸2.7~3.6V
			{
				retry=0XFFFE;
				do
				{
					SD_SendCmd(CMD55,0,0X01);	//µo°eCMD55
					r1=SD_SendCmd(CMD41,0x40000000,0X01);//µo°eCMD41
				}while(r1&&retry--);
				if(retry&&SD_SendCmd(CMD58,0,0X01)==0)//Å²§OSD2.0¥d
				{
					for(i=0;i<4;i++)buf[i]=SD_SPI_ReadWriteByte(0XFF);//±o¨ìªºOCR­È
					if(buf[0]&0x40)SD_Type=SD_TYPE_V2HC;    //ÀË¯ùCCS
					else SD_Type=SD_TYPE_V2;   
				}
			}
		}else//SD V1.x/ MMC	V3
		{
			SD_SendCmd(CMD55,0,0X01);	//µo°eCMD55
			r1=SD_SendCmd(CMD41,0,0X01);//µo°eCMD41
			if(r1<=1)
			{		
				SD_Type=SD_TYPE_V1;
				retry=0XFFFE;
				do //µÈ´ýÍË³öIDLEÄ£Ê½
				{
					SD_SendCmd(CMD55,0,0X01);	//µo°eCMD55
					r1=SD_SendCmd(CMD41,0,0X01);//µo°eCMD41
				}while(r1&&retry--);
			}else
			{
				SD_Type=SD_TYPE_MMC;//MMC V3
				retry=0XFFFE;
				do //µÈ´ýÍË³öIDLEÄ£Ê½
				{											    
					r1=SD_SendCmd(CMD1,0,0X01);//µo°eCMD1
				}while(r1&&retry--);  
			}
			if(retry==0||SD_SendCmd(CMD16,512,0X01)!=0)SD_Type=SD_TYPE_ERR;//¿ù»~ªº¥d
		}
	}
	SD_DisSelect();//¨ú®ø¤ù¿ï
	SD_SPI_SpeedHigh();//°ª«×¼Ò¦¡
	if(SD_Type)return 0;
	else if(r1)return r1; 	   
	return 0xaa;//¨ä¥L¼Ò¦¡
}
 
//ÅªSD¥d
//buf:¼Æ¾Ú¼È¦s°Ï
//sector:®°°ÏÇø
//cnt:®°°Ï¼Æ
//return:0,ok;¨ä¥L,¥¢±Ñ.
u8 SD_ReadDisk(u8 *buf,u32 sector,u8 cnt)
{
	u8 r1;
	if(SD_Type!=SD_TYPE_V2HC)sector <<= 9;
	if(cnt==1)
	{
		r1=SD_SendCmd(CMD17,sector,0X01);
		if(r1==0)
		{
			r1=SD_RecvData(buf,512);  
		}
	}else
	{
		r1=SD_SendCmd(CMD18,sector,0X01);//³sÄò©R¥O
		do
		{
			r1=SD_RecvData(buf,512);//±µ¦¬512Bytes	 
			buf+=512;  
		}while(--cnt && r1==0); 	
		SD_SendCmd(CMD12,0,0X01);	//µo°e°±¤î©R¥O
	}   
	SD_DisSelect();//¨ú®ø¤ù¿ï
	return r1;//
}

//¼gSD¥d
//buf:¼Æ¾Ú¼È¦s°ÏÇø
//sector:°_©l®°°ÏÇø
//cnt:®°°Ï¼Æ
//return:0,ok;¨ä¥L,¥¢±Ñ.

u8 SD_WriteDisk(u8 *buf,u32 sector,u8 cnt)
{
	u8 r1;
	if(SD_Type!=SD_TYPE_V2HC)sector *= 512;
	if(cnt==1)
	{
		r1=SD_SendCmd(CMD24,sector,0X01);
		if(r1==0)
		{	
			r1=SD_SendBlock(buf,0xFE);	   
		}
	}else
	{
		if(SD_Type!=SD_TYPE_MMC)
		{
			SD_SendCmd(CMD55,0,0X01);	
			SD_SendCmd(CMD23,cnt,0X01);	
		}
 		r1=SD_SendCmd(CMD25,sector,0X01);
		if(r1==0)
		{
			do
			{
				r1=SD_SendBlock(buf,0xFC);
				buf+=512;  
			}while(--cnt && r1==0);
			r1=SD_SendBlock(0,0xFD);
		}
	}   
	SD_DisSelect();
	return r1;
}