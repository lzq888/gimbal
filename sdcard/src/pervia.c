#include "pervia.h"
//*********************************************************
// Function: Configure the main frequency at 72MHz
// Input:  N/A
// Return: N/A
// Remark: N/A
//*********************************************************
void RCC_Configuration(void)
{
    ErrorStatus HSEStartUpStatus;

	/* RCC system reset(for debug purpose) */
	RCC_DeInit();
	
	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);
	
	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	
	if(HSEStartUpStatus == SUCCESS)
	{
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		
		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);
		
		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 
		
		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1); 
		
		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config(RCC_HCLK_Div2);
		
		/* PLLCLK = 8MHz * 9 = 72 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
		
		/* Enable PLL */ 
		RCC_PLLCmd(ENABLE);
		
		/* Wait till PLL is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
			;
		
		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		
		/* Wait till PLL is used as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08)
			;
	}
}


//*********************************************************
// Function: Configure NVIC priority group and vector table
// Input:  N/A
// Return: N/A
// Remark: N/A
//*********************************************************
void NVIC_Configuration(void)
{																																																																																									
	#ifdef  VECT_TAB_RAM  																																												
		/* Set the Vector Table base location at 0x20000000 */ 																																												
		NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 																																												
	#else  /* VECT_TAB_FLASH  */																																												
		/* Set the Vector Table base location at 0x08000000 */ 																																												
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   																																												
	#endif 	
		
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
}

//*********************************************************
// Function: Initial STM32
// Input:  N/A
// Return: N/A
// Remark: N/A
//*********************************************************
void Initial_STM32(void)
{	
	RCC_Configuration();	// Enable Clock
	NVIC_Configuration();	// Configure the NVIC
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	//Disable JTAG pins 
}

//*********************************************************
// Function: delay Count x mS
// Input: Count
// Return: x
// Remark: Crystal = 8M
//*********************************************************
void DELAY_mS( unsigned short ms )
{
	unsigned short i;
	while(ms--)
    {
        for(i=0;i<8000;i++);
    }
}

//*********************************************************
// Function: delay Count x 1uS
// Input: Count
// Return: x
// Remark: Crystal = 8M
//*********************************************************
void DELAY_uS( unsigned short us )
{
	unsigned short i;
    while(us--)
    {
    	for(i=0;i<8;i++);
    }
}

