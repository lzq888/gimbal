
#include "config.h"

u16 	pwmvalue_a = 1000;
u16 	pwmvalue_b = 1450;	
u16 	pwmvalue_c;
u16 	pwmvalue_d;


void TIMER_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;


    /*TIMER5 */	
    TIM_TimeBaseStructure.TIM_Period =(uint16_t)(10000-1);			  			//period
  	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t)(72-1);  						//high voltage portion(between 0-period)	
  	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM5, ENABLE);
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE); 

	/*72MHz/((9999+1)*(72))=100Hz*/
						
    /*TIMER4 */	
    TIM_TimeBaseStructure.TIM_Period =(uint16_t)(1000-1);			  			//period
  	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t)(72-1);  						//high voltage portion(between 0-period)	
  	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM4, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);        				//enable timer interrupt
	
	/*72MHz/((999+1)*(72))=1000Hz*/	

    /*TIMER3 */	
    TIM_TimeBaseStructure.TIM_Period =19999;			  				//high voltage portion(between 0-period)
  	TIM_TimeBaseStructure.TIM_Prescaler = 71;		
  	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM3, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	/*72MHz/((999+1)*(71+1))=50Hz*/

	/*TIMER2 */	
	TIM_TimeBaseStructure.TIM_Period =99999;			  				//high voltage portion(between 0-period)
  	TIM_TimeBaseStructure.TIM_Prescaler = 719;		
  	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
		   			
	
	/*72MHz/((99999+1)*(719+1))=1Hz*/																																																							
}

void PWMoutputA(u16 i) 													
{
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse =i;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);		 //TIM_OC3(channel~¬dªí)Init(TIM4, &TIM_OCInitStructure);
}
void PWMoutputB(u16 j)
{
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse =j;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);         //TIM_OC3(channel~¬dªí
	
}

void PWMoutputC(u16 m)
{
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse =m;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);         //TIM_OC3(channel~¬dªí
	
}

void PWMoutputD(u16 n)
{
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse =n;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);         //TIM_OC3(channel~¬dªí
	
}
/*1hz*/
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 
	{			  			
	  	if(initial_flag==1)	
		{
			//printf("acc_x,%f,acc_y,%f,acc_z,%f,gyr_x,%f,gyr_y,%f,gyr_z,%f\r\n",
			//acc.x,acc.y,acc.z,gyr.x,gyr.y,gyr.z);
			//printf("%f %f %f %f %f %f %f %f %f\r\n",acc.x,acc.y,acc.z,gyr.x,gyr.y,gyr.z,ang.Pitch,ang.Roll,ang.Yaw);			
			//printf("1,%f,2,%f,3,%f,4,%f,5,%f\r\n",mag.EllipseSita,mag.EllipseX0,mag.EllipseY0,mag.EllipseA,mag.EllipseB);						
			//printf("%f  %f  %f\r\n",mag_HMC5983.x, mag_HMC5983.y, mag_HMC5983.z);
			//printf("magne_Yaw,%f\r\n",ang.Yaw);
			//printf("x : %f\r\n",gps.x);
			//printf("y : %f\r\n",gps.y);
			//printf("z : %f\r\n",gps.z);
			//printf("x1,%f,y1,%f,z1,%f\r\n",target.x,target.y,target.z);
			//printf("pitch,%f,roll,%f,yaw,%f\r\n",ang.Pitch, ang.Roll, ang.Yaw);
			//printf("%f %f %f\r\n",ang.Pitch, ang.Roll, ang.Yaw);
			//printf("yaw,%f,pitch,%f\r\n",global_yaw,global_pitch);
			//printf("yaw,%f,pitch,%f\r\n",global_yaw,global_pitch);
		
			printf("%f %f %f %f %f %f %f %f \r\n",ang.Pitch,ang.Roll,ang.Yaw,gps.x,gps.y,gps.z,body_pitch,body_yaw);
		}

		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
/*50hz*/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 
	{
		
		/*pitch*/
		if(initial_flag==1)	
		{

			body_aimming_angle();
			/*aiming mode*/
			if(joystick.Remote_on_off==0)
			{
				if (body_pitch> 90 )	           
				{
					pwmvalue_a = 2000 ;
				}
				else if (body_pitch > 0 && body_pitch < 90)
				{
					pwmvalue_a = 1000 + (body_pitch) * 11.11;
				}			
				else if(body_pitch< 0 )  
				{
					pwmvalue_a = 1000;
				}

				PWMoutputA(pwmvalue_a);

		 		/*yaw*/

				if (body_yaw > 180 )
				{
					pwmvalue_b = 1000 + yaw_offset;
				}
				else if (body_yaw > -180 && body_yaw < 180) 
				{
					pwmvalue_b = 1500 - (body_yaw)*2.77 + yaw_offset;			
				}		
				else if(body_yaw < -180 )  
				{
					pwmvalue_b = 2000 + yaw_offset;
				}

				PWMoutputB(pwmvalue_b);
			}

			/*joystick mode*/
			if(joystick.Remote_on_off==1)
			{

				
				if (joystick.Remote_PWM_Pitch < 1000 )	           
				{
					pwmvalue_a += 2 ;
				}
				else if (joystick.Remote_PWM_Pitch > 1000 && joystick.Remote_PWM_Pitch < 1900)
				{
					pwmvalue_a +=1 ;
				}			
				else if(joystick.Remote_PWM_Pitch > 1900 && joystick.Remote_PWM_Pitch < 2100 )  
				{
					
				}
				else if(joystick.Remote_PWM_Pitch > 2100 && joystick.Remote_PWM_Pitch < 3000 )  
				{
					pwmvalue_a -= 1;
				}
				else if(joystick.Remote_PWM_Pitch > 3000 && joystick.Remote_PWM_Pitch < 4100 )  
				{
					pwmvalue_a -= 2;
				}

				if(pwmvalue_a > 2000) pwmvalue_a = 2000;
				if(pwmvalue_a < 1000) pwmvalue_a = 1000;

				PWMoutputA(pwmvalue_a);

		 		/*yaw*/

				if (joystick.Remote_PWM_Yaw < 1000 )	           
				{
					pwmvalue_b -= 4 ;
				}
				else if (joystick.Remote_PWM_Yaw > 1000 && joystick.Remote_PWM_Yaw < 1900)
				{
					pwmvalue_b -= 2 ;
				}			
				else if(joystick.Remote_PWM_Yaw > 1900 && joystick.Remote_PWM_Yaw < 2200 )  
				{
					
				}
				else if(joystick.Remote_PWM_Yaw > 2200 && joystick.Remote_PWM_Yaw < 3000 )  
				{
					pwmvalue_b += 2;
				}
				else if(joystick.Remote_PWM_Yaw > 3000 && joystick.Remote_PWM_Yaw < 4100 )  
				{
					pwmvalue_b += 4;
				}

				if(pwmvalue_b > 2000) pwmvalue_b = 2000;
				if(pwmvalue_b < 1000) pwmvalue_b = 1000;


				PWMoutputB(pwmvalue_b);

			}

			printf("PWM_P,%d,PWM_Y,%d\r\n",pwmvalue_a, pwmvalue_b);
	
		}
		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
		
}

/*1000hz*/
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) 
	{	
		delay_buf++;		  			
		if(initial_flag==1)	
		{
			
		}	
		
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
/*100hz*/
void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) 
	{			  			
		if(initial_flag==1)	
		{
			for(int i=0;i<100;i++)
			{
				get_gps_data(gps_buf1[i]);
			}
			mpu_9150_data();
			HMC5983_DATA();
			ahrs_update();
			for(int i=0;i<100;i++)
			{
				get_joystck_command(joystick_buffer[i]);
			}
		}	
		
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	}
}





