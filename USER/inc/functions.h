#ifndef __functions_H
#define __functions_H

#include "alientek.h"
void Int2Char(u16 number,char* buffer, int digitNumber );
int  Hex2Dec(u8 return_value_double,u8 return_value_single);
int  dec2Dec(u8 return_value_double,u8 return_value_single);
char checksum(char* buffer);
#endif

