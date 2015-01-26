#ifndef __USART_H
#define __USART_H

#include "alientek.h"
#include "bool.h"


void USART_Config(USART_TypeDef* , uint32_t );
void USART_SendData_u8(USART_TypeDef* USARTx, u8 _Data);
void send_byte(uint8_t b);
void send_string(uint8_t *str);

#endif
