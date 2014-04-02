#ifndef __gps_functions_H
#define __gps_functions_H
#include <math.h>
#include "alientek.h"
#include "bool.h"

typedef struct{
	u8      	gps_start_flag;
	u8      	gps_info_flag;
	u8      	gpgga_flag;
	u16     	gps_word_count;
	u16     	checksum_value;
}gps_flags;

typedef struct{
	u8      	gps_buf_string[100];
	u8      	gps_buf_checksum_string[100];
	u8      	latitude_string[10];
	u8      	longtitude_string[10];
	u8      	height_string[10];
}gps_buffers;

typedef struct {
	float     	latitude_value;
	float     	longtitude_value;
	float     	height_value;
}gps_data;



#define 	pi 					3.1415926f 
#define 	Equatorial_Radius 	6378137.0f
#define 	Polar_Radius 	    6356752.314245f
#define 	long0 	    		121.0f
#define 	k0 	    		    0.9999f
#define     dx                  250000.0f


float 	m2dec_lad(char* buffer);
float 	m2dec_lon(char* buffer);
void  	gga2twd97(float lat,float lon,float *x,float *y);
bool 	findGPGGA();
void 	get_gps_data();
#endif

