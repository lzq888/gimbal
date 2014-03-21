#ifndef __gps_functions_H
#define __gps_functions_H
#include <math.h>
#include "alientek.h"

#define 	pi 					3.1415926f 
#define 	Equatorial_Radius 	6378137.0f
#define 	Polar_Radius 	    6356752.314245f
#define 	long0 	    		121.0f
#define 	k0 	    		    0.9999f
#define     dx                  250000.0f


float m2dec_lad(char* buffer);
float m2dec_lon(char* buffer);
void  gga2twd97(float lat,float lon,float *x,float *y);
#endif

