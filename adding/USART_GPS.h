
#ifndef __USART_H
#define __USART_H
//#include "dev100pin.h"
#include "stm32f10x.h"


#define Max_Length 30

extern 	char 	SNMEA[Max_Length];
extern	char	Output_GPRMC[Max_Length];
void    GPS_Configuration(USART_TypeDef* USARTX, uint32_t BaudRate);
void    USART3_IRQHandler(void);

extern  int send;
extern  int data;
extern  char	GPS[20];

#endif