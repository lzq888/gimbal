#include <stdlib.h>
#include "gps_functions.h"
#include "functions.h"
#include <string.h>
#include <math.h>
#include "global.h"
#include "bool.h"

gps_flags 		gga_flags;
gps_buffers 	gga_buffer;
u16    			word_count=0;

float 			x=0;
float 			y=0;
float   		z=0;

float m2dec_lad(char* buffer)
{
	int i=0;
	u8 deg_string[2];
	u8 min_string[8];
	float deg_value=0;
	float min_value=0;
	float dec_lad_value=0;

	for(i=0;i<2;i++)
	{
		deg_string[i]=buffer[i];
	}

	for(i=0;i<8;i++)
	{
		min_string[i]=buffer[i+2];
	}

	deg_value=atof(deg_string);
	min_value=atof(min_string);

	dec_lad_value=deg_value+min_value/60;

	return dec_lad_value;
}

float m2dec_lon(char* buffer)
{
	int i=0;
	u8 deg_string[3];
	u8 min_string[7];
	float deg_value=0;
	float min_value=0;
	float dec_lon_value=0;

	for(i=0;i<3;i++)
	{
		deg_string[i]=buffer[i];
	}

	for(i=0;i<7;i++)
	{
		min_string[i]=buffer[i+3];
	}

	deg_value=atof(deg_string);
	min_value=atof(min_string);

	dec_lon_value=deg_value+min_value/60;

	return dec_lon_value;
}

void gga2twd97(float lat,float lon,float *x,float *y)
{
	float e  = 0;
	float e2 = 0;
	float n  = 0;
	float nu = 0;
	float p  = 0;
	float p1 = 0;
	float A  = 0;
	float B  = 0;
	float C  = 0;
	float D  = 0;
	float E  = 0;
	float S  = 0;
	float K1 = 0;
	float K2 = 0;
	float K3 = 0;
	float K4 = 0;
	float K5 = 0;

    /*Symbols*/
	e = sqrt(1 - pow(Polar_Radius/Equatorial_Radius,2) );
	e2= pow(e,2)/(1-pow(e,2));
	n = (Equatorial_Radius-Polar_Radius)/(Equatorial_Radius+Polar_Radius);
	nu= Equatorial_Radius/sqrt( 1 - pow(e,2)*pow(sin(lat),2));
	p1= degree2radians(long0);
	p = lon-p1;
	/*Calculate the Meridional Arc*/
	A = Equatorial_Radius*(1 - n +(5/4.0)*(pow(n,2)-pow(n,3)) + (81/64.0)*(pow(n,4)-pow(n,5)));	 
	// A = a*(1 - n + (5/4.0)*(n**2 - n**3) + (81/64.0)*(n**4  - n**5))
	B = (3*Equatorial_Radius*n/2.0)*(1 - n + (7/8.0)*(pow(n,2) - pow(n,3)) + (55/64.0)*(pow(n,4) - pow(n,5)));
	// B = (3*a*n/2.0)*(1 - n + (7/8.0)*(n**2 - n**3) + (55/64.0)*(n**4 - n**5))
	C = (15*Equatorial_Radius*pow(n,2)/16.0)*(1 - n + (3/4.0)*(pow(n,2) - pow(n,3)));
	// C = (15*a*(n**2)/16.0)*(1 - n + (3/4.0)*(n**2 - n**3))
	D = (35*Equatorial_Radius*pow(n,3)/48.0)*(1 - n + (11/16.0)*(pow(n,2) - pow(n,3)));
	// D = (35*a*(n**3)/48.0)*(1 - n + (11/16.0)*(n**2 - n**3))
	E = (315*Equatorial_Radius*pow(n,4)/51.0)*(1- n);  
	// E = (315*a*(n**4)/51.0)*(1 - n)
	S = A*lat - B*sin(2*lat) + C*sin(4*lat) - D*sin(6*lat) + E*sin(8*lat);
	// S = A*lat - B*sin(2*lat) + C*sin(4*lat) - D*sin(6*lat) + E*sin(8*lat)

	/*Converting Latitude and Longitude to UTM*/
   	K1 = S*k0;
   	// K1 = S*k0
    K2 = k0*nu*sin(2*lat)/4.0;
    // K2 = k0*nu*sin(2*lat)/4.0
    K3 = (k0*nu*sin(lat)*pow(cos(lat),3)/24.0)*(5 - pow(tan(lat),2) + 9*e2*pow(cos(lat),2) + 4*pow(e2,2)*pow(cos(lat),4));
    // K3 = (k0*nu*sin(lat)*(cos(lat)**3)/24.0) * (5 - tan(lat)**2 + 9*e2*(cos(lat)**2) + 4*(e2**2)*(cos(lat)**4))
    K4 = k0*nu*cos(lat);
    // K4 = k0*nu*cos(lat)
    K5 = (k0*nu*pow(cos(lat),3)/6.0) * (1 - pow(tan(lat),2) + e2*pow(cos(lat),2));
    // K5 = (k0*nu*(cos(lat)**3)/6.0) * (1 - tan(lat)**2 + e2*(cos(lat)**2))
	*x = K4*p + K5*pow(p,3) + dx ;
	// x = K4*p + K5*(p**3) + self.dx
	*y = K1 + K2*pow(p,2) + K3*pow(p,4);
	// y = K1 + K2*(p**2) + K3*(p**4)
}

bool findGPGGA()
{
       
    if(gga_buffer.gps_buf_string[0]=='G' && gga_buffer.gps_buf_string[1]=='P' && gga_buffer.gps_buf_string[2]=='G' && 
       gga_buffer.gps_buf_string[3]=='G' && gga_buffer.gps_buf_string[4]=='A')
    {
        
        return TRUE;
    }

        return FALSE;
}


void get_gps_data()
{

		if(gps_buf!=0)
		{	
			/*resort gps info (every kind)*/	
			if(gga_flags.gps_start_flag==1)
			{
				gga_buffer.gps_buf_string[word_count]=gps_buf;
				word_count++;
				gga_flags.gps_word_count++;
			}
			/*start word*/	
			if(gps_buf=='$')
			{
				gga_flags.gps_start_flag=1;
			}
			/*final word*/
			if(gps_buf=='\n')
			{
				gga_flags.gps_start_flag=0;
				word_count=0;
				gga_flags.gps_info_flag=1;
				/*store over*/
			}

			gps_buf=0;		
		}


		if(gga_flags.gps_info_flag!=0)
		{
			u16 	commond_count=0;
			/*check gpgga or not*/
			if(findGPGGA())
			{
				gga_flags.gpgga_flag=1;
			}
			else
			{	
				for(int i=0;i<100;i++)
				gga_buffer.gps_buf_string[i]=0;

			}
			if(gga_flags.gpgga_flag==1)
			{	   	
				/*yes! it is gpgga data*/

				for(int i=0;i<gga_flags.gps_word_count-5;i++)
				{	
					/*cut checksum value and \r\n*/	
					gga_buffer.gps_buf_checksum_string[i]=gga_buffer.gps_buf_string[i];
	  			}

				/*check data is wright or not*/
				gga_flags.checksum_value=hex2dec(gga_buffer.gps_buf_string[gga_flags.gps_word_count-4],gga_buffer.gps_buf_string[gga_flags.gps_word_count-3]);			
				if(gga_flags.checksum_value==(int)checksum(gga_buffer.gps_buf_checksum_string))
				{
					/*yes! check data is right*/
					int a=0;
					for(int i=0;i<gga_flags.gps_word_count-5;i++)
					{		
						
						if(commond_count==2)
						{/*get latitude and put in buffer*/
							gga_buffer.latitude_string[a]=gga_buffer.gps_buf_checksum_string[i];
							a++;

						}
						if(commond_count==4)
						{/*get longtitude and put in buffer*/
							gga_buffer.longtitude_string[a]=gga_buffer.gps_buf_checksum_string[i];
							a++;
						}
						if(commond_count==9)
						{/*get height and put in buffer*/
							gga_buffer.height_string[a]=gga_buffer.gps_buf_checksum_string[i];
							a++;
						}
						if(gga_buffer.gps_buf_checksum_string[i]==',') 
							{
								commond_count++;
								a=0;
						}
					}
					commond_count=0;

					/*convert string to value*/
					gga.latitude_value=atof(gga_buffer.latitude_string);
					gga.longtitude_value=atof(gga_buffer.longtitude_string);
					gga.height_value=atof(gga_buffer.height_string);

					gga2twd97(degree2radians(m2dec_lad(gga_buffer.latitude_string)),degree2radians(m2dec_lon(gga_buffer.longtitude_string)),&x,&y);
					z=gga.height_value;
					
					gps.x=x;
					gps.y=y;
					gps.z=z;

					//printf("x : %f\r\n",gps.x);
					//printf("y : %f\r\n",gps.y);
					//printf("z : %f\r\n",gps.z);
					//printf("latitude : %f\r\n",gga.latitude_value);
					//printf("longutitude : %f\r\n",gga.longtitude_value);
					//printf("height : %f\r\n",gga.height_value);
					
					
				}
				
				for(int i=0;i<100;i++)
				{	
					gga_buffer.gps_buf_checksum_string[i]=0;
					gga_buffer.gps_buf_string[i]=0;
				}

			gga_flags.gpgga_flag=0;
			}

			gga_flags.gps_info_flag=0;
			gga_flags.gps_word_count=0;
		}

}






