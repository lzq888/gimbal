#include "HMC5983.h"
#include "config.h"

void initial_HMC5983()
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	DELAY_mS(1);
	SPI2_ReadWriteByte(0x00);
	SPI2_ReadWriteByte(0xfc);		
	GPIO_SetBits(GPIOB,GPIO_Pin_12);

	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	DELAY_mS(1);
	SPI2_ReadWriteByte(0x02);
	SPI2_ReadWriteByte(0x00);		
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
}


void read_HMC5983(s16* MAG)
{
	u8 tmpBuffer_5983[6];

	GPIO_ResetBits(GPIOB,GPIO_Pin_12);	
	DELAY_mS(1);
		
	SPI2_ReadWriteByte(0xC3);

	tmpBuffer_5983[0] = SPI2_ReadWriteByte(0x00);
	tmpBuffer_5983[1] = SPI2_ReadWriteByte(0x00);
	tmpBuffer_5983[2] = SPI2_ReadWriteByte(0x00);
	tmpBuffer_5983[3] = SPI2_ReadWriteByte(0x00);
	tmpBuffer_5983[4] = SPI2_ReadWriteByte(0x00);
	tmpBuffer_5983[5] = SPI2_ReadWriteByte(0x00);
		
	DELAY_mS(1);

	GPIO_SetBits(GPIOB,GPIO_Pin_12);

	/* Get magne rate */
	for(int i=0; i<3; i++) 
      MAG[i]=((s16)((u16)tmpBuffer_5983[2*i] << 8) + tmpBuffer_5983[2*i+1]);
	
}


