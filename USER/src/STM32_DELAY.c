/*º–¿Y¿…*/
#include "delay.h"
/**********************************************************
// STM32_DELAY.c
void DELAY_mS( unsigned short ms );
void DELAY_uS( unsigned short us );
**********************************************************/

//*********************************************************
// Function: delay Count x mS
// Input: Count
// Output: x
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
// Function: delay Count x 10uS
// Input: Count
// Output: x
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
