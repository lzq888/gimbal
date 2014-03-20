#ifndef __gps_functions_H
#define __gps_functions_H
#include <math.h>
#include "alientek.h"

#define 	pi 					3.1415926 
#define 	Equatorial_Radius 	6378137
#define 	Polar_Radius 	    6356752.314245
#define 	long0 	    		121
#define 	k0 	    		    0.9999
#define     dx                  250000


float m2dec_lad(char* buffer);
float m2dec_lon(char* buffer);
void  gga2twd97(float lat,float lon,float *x,float *y);
#endif

