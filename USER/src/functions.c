#include <stdlib.h>
#include "functions.h"
#include <string.h>
#include "gps_functions.h"

char  buffer[4];

void Int2Char(u16 number,char* buffer, int digitNumber )
{
    int j;
    int currNum ;
    for(j=0;j<digitNumber;j++)
    {
        currNum = number % 10;
        number 	= number / 10;


            switch(currNum)
            {
                case 0:
                    *(buffer+3-j)='0';
                    break;
                case 1:
                    *(buffer+3-j)='1';
                    break;
                case 2:
                    *(buffer+3-j)='2';
                    break;
                case 3:
                     *(buffer+3-j)='3';
                    break;
                case 4:
                     *(buffer+3-j)='4';
                    break;
                case 5:
                     *(buffer+3-j)='5';
                    break;
                case 6:
                     *(buffer+3-j)='6';
                    break;
                case 7:
                     *(buffer+3-j)='7';
                    break;
                case 8:
                     *(buffer+3-j)='8';
                    break;
                case 9:
                     *(buffer+3-j)='9';
                    break;
                default:
                     *(buffer+3-j)='X';
                    break;
            }
    }
}


int Hex2Dec(u8 return_value_double,u8 return_value_single)
{	
	int dec_value=0;
	int i=0;
    int buffer[2];
    buffer[0]=return_value_double;
    buffer[1]=return_value_single;
    
    for(i=0;i<sizeof(buffer);i++)
    {
        switch (buffer[i])
        {
        case 'A':buffer[i]=58;
                 break;
        case 'B':buffer[i]=59;
                 break;
        case 'C':buffer[i]=60;
                 break;
        case 'D':buffer[i]=61;
                 break;
        case 'E':buffer[i]=62;
                 break;
        case 'F':buffer[i]=63;
                 break;
        }
    }					  
    dec_value=(buffer[0]-48)*16+(buffer[1]-48);
    return dec_value;
}

int hex2dec(u8 double_digit,u8 single_digit)
{	
	int dec_value=0;
	int i=0;
    int buffer[2];
    buffer[0]=double_digit;
    buffer[1]=single_digit;
    
    for(i=0;i<sizeof(buffer);i++)
    {
        switch (buffer[i])
        {
        case 'A':buffer[i]=58;
                 break;
        case 'B':buffer[i]=59;
                 break;
        case 'C':buffer[i]=60;
                 break;
        case 'D':buffer[i]=61;
                 break;
        case 'E':buffer[i]=62;
                 break;
        case 'F':buffer[i]=63;
                 break;
        }
    }					  
    dec_value=(buffer[0]-48)*16+(buffer[1]-48);
    return dec_value;
}

char checksum(char* buffer)
{
    int     j=1;
    char    exor=0;
    exor=buffer[0];                           
        for(j=1;j<strlen(buffer);j++)
        {
            exor=exor^buffer[j];
        }
    return exor;
}

float degree2radians(float degree)
{
    float radians = 0;

    radians = degree*pi/180;

    return radians;
}

        






