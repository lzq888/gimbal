#include "config.h"


u8 		input_buffer[28];
u8      checksum_buff[23];
u8  	final_flag=0;
u8		start_flag=0;
u8		count=0;
u8		length=0;

u8		usart_data[28];
u8		exor=0;
u16		dec_value=0;
u16		timercount=0;
u16     timercount_last=0;
u16     timming_flag=0;
u16		checksum_flag=0;
u8      difference_count=0;


void get_joystck_command(u8 data)
{


	if(data!=0)
	{	
     
	  if(start_flag==1)					   //start  store data into input_buffer
	  {	
	  	if(count==28)					   //if data is full without a stop sign
		{		
		 	count=0; 
			memset(input_buffer,0,sizeof(input_buffer));   //reset	 input_buffer
			start_flag=0;
			USART_SendData(USART2,'1'); 					//request transmit end to sent the previous data and present data
	  		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)	;
			USART_SendData(USART1,'\r'); 	
	  		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)	;
			USART_SendData(USART1,'\n'); 	
	  		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)	;
		}
		input_buffer[count] = data;  	
	  	count++;
		length++;  					
	  }
	    if(data=='$') 
	  	{
	  		start_flag=1;		//start sign received 
	  	}

	  	if(data=='\n') 
		{
		start_flag=0;
	  	final_flag=1;
		count=0;       //save finished 
		}  
	}		

	  if(final_flag==1)					 //receive  final  words
		{
			/*GET TIMERCOUNT VALUE DEC*/

			if(input_buffer[length-8]==32)
			{
				timercount=(input_buffer[length-7]-48);
			}
			else
			{
				timercount=(input_buffer[length-8]-48)*10+(input_buffer[length-7]-48);
			}

			//printf("x : %d\r\n",timercount);

			/* check continue or not and calculate how many packages are missing*/ 	

			/*
			timming_flag=1;		  normal condition or skip too many datas
			timming_flag=0;		  abnormal condition the number of skipped datas is less than five
			*/
			if(timercount-timercount_last==1||timercount_last-timercount==19)
			{ 
				timming_flag=1;
				difference_count='0';
			}
			else if(timercount-timercount_last==0)	                                 //the first data 
			{
				timming_flag=1;
				difference_count='0';
			}
			else if(timercount-timercount_last==2||timercount_last-timercount==18)
			{
				timming_flag=0;
				difference_count='1';
		 	}
			else if(timercount-timercount_last==3||timercount_last-timercount==17)
			{
				timming_flag=0;
				difference_count='2';
			}
			else if(timercount-timercount_last==4||timercount_last-timercount==16)
			{
				timming_flag=0;
				difference_count='3';
			}
			else if(timercount-timercount_last==5||timercount_last-timercount==15)
			{
				timming_flag=0;
				difference_count='4';
			}
			else
			{
				timming_flag=1;
				difference_count='0';
			}
			
			//printf("x : %c\r\n",difference_count);

			for(int i=0;i<length-5;i++)
			{	
				/*cut checksum value and \r\n*/	
				checksum_buff[i]=input_buffer[i];
	  		}

	  		if(input_buffer[length-4]==32)
			{
				dec_value=(input_buffer[length-3]-48);
			}
			else
			{
				dec_value=(input_buffer[length-4]-48)*10+(input_buffer[length-3]-48);
			}
			
			exor=(int)checksum(checksum_buff);

			/*check the  package is right or not*/
			if(exor==dec_value)	                                                       	//if package is right
        	{  
				if(timming_flag==1)													   	//if package is conitune
				{
					strncpy(usart_data,input_buffer,sizeof(input_buffer));				//copy from input_buffer to usart_data
					joystick_command(checksum_buff);
					/*this command get the value(int) of joystick and save to global(joystick)*/
/*
					for(int j=0;j<length;j++)
					{
						USART_SendData(USART1,usart_data[j]); 							//shown by usart1
	  					while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)	;
					}
*/

				}

				if(timming_flag==0)													  	//if package is disconitune
				{
	//				strncpy(usart_data,input_buffer,sizeof(input_buffer));				//copy from input_buffer to usart_data
	//				for(int j=0;j<length;j++)
	//				{
	//					USART_SendData(USART1,usart_data[j]); 							//shown by usart1
	// 					while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)	;
	//				}
						USART_SendData(USART2,difference_count); 	
	  					while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)	;
						USART_SendData(USART1,difference_count); 	
	  					while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)	;
						USART_SendData(USART1,'\r'); 	
	  					while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)	;
						USART_SendData(USART1,'\n'); 	
	  					while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)	;
										
				}

				timercount_last=timercount;	
        	}
																					   
        	else							                                            //if package is wrong      
        	{																						
				difference_count='5';						   															//ask to send again
				USART_SendData(USART2,difference_count); 							    //show the wrong message
	  			while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
        	} 
			/*reset the all the  counting parameters*/
			 memset(input_buffer,0,sizeof(input_buffer));
			 length=0;
			 final_flag=0;			
		}

}


void joystick_command(char* usart_data)
{
	u8 	ptich_string[4];
	u8 	roll_string[4];
	u8 	yaw_string[4];
	u8 	on_off[4];
	int a=0;
	u16 commond_count=0;
					

	for(int i=0;i<strlen(usart_data);i++)
	{		
						
		if(i<4 && a<5)
		{/*get latitude and put in buffer*/
			ptich_string[a]=usart_data[i];
			a++;
		}
		if(commond_count==1 && a<5)
		{/*get longtitude and put in buffer*/
			joystick.Remote_PWM_Pitch=atoi(ptich_string);
			roll_string[a]=usart_data[i];
			a++;
		}
		if(commond_count==2 && a<5)
		{/*get height and put in buffer*/
			joystick.Remote_PWM_Roll=atoi(roll_string);
			yaw_string[a]=usart_data[i];
			a++;
		}
		if(commond_count==3 && a<5)
		{/*get height and put in buffer*/
			joystick.Remote_PWM_Yaw=atoi(yaw_string);
			on_off[a]=usart_data[i];
			a++;
		}
		if(usart_data[i]==',') 
		{
			commond_count++;
			a=0;
		}	
	}
	joystick.Remote_on_off=atoi(on_off);

	//printf("1,%d,2,%d,3,%d,4,%d\r\n",
	//joystick.Remote_PWM_Pitch, joystick.Remote_PWM_Roll, joystick.Remote_PWM_Yaw,joystick.Remote_on_off);		


}