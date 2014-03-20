#include <stdlib.h>
#include "gps_functions.h"
#include <string.h>

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