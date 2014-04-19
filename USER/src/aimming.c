#include "config.h"
#include "aimming.h"



float vector_x = 0;
float vector_y = 0;
float vector_z = 0;

float vector_x_fc = 0;
float vector_y_fc = 0;
float vector_z_fc = 0;

float vector_body_x = 0;
float vector_body_y = 0;
float vector_body_z = 0;

void global_aimming_angle(float *yaw,float *pitch)
{
	float diffx=0;
	float diffy=0;
	float diffz=0;
	float diffxy=0;

	diffx 	= target.x - gps.x;
	diffy 	= target.y - gps.y;
	diffz 	= target.z - gps.z;
	/*vector in global frame*/
	vector_x = diffx;
	vector_y = diffy;
	vector_z = diffz;

	//printf("%f,%f,%f\r\n",vector_x,vector_y,vector_z);

	diffxy 	= sqrt(pow2(diffx)+pow2(diffy));
	*yaw 	= toDeg(atan2f(diffx,diffy));
	*pitch 	= toDeg(atan2f(diffz,diffxy));  
}

void body_aimming_angle()
{
	global_aimming_angle(&(global_yaw),&(global_pitch));
	
	
	/*transform global vetor form global to body*/
	
	vector_x_fc = vector_y;
	vector_y_fc = -vector_x;
	vector_z_fc = vector_z;

	//printf("%f,%f,%f\r\n",vector_x_fc,vector_y_fc,vector_z_fc);

	/*turn body by yaw,pitch,roll*/
	vector_body_x = Mq11*vector_x_fc + Mq12*vector_y_fc + Mq13*vector_z_fc;
	vector_body_y = Mq21*vector_x_fc + Mq22*vector_y_fc + Mq23*vector_z_fc;
	vector_body_z = Mq31*vector_x_fc + Mq32*vector_y_fc + Mq33*vector_z_fc;

	printf("%f,%f,%f\r\n",vector_body_x,vector_body_y,vector_body_z);

	//printf("global_yaw,%f,global_pitch,%f\r\n",global_yaw,global_pitch);

	body_yaw   = (atan2f(vector_body_y,vector_body_x));
	body_pitch = toDeg(atan2f(-vector_body_z,(arm_cos_f32(body_yaw) * vector_body_x + arm_sin_f32(body_yaw) * vector_body_y)));
	body_yaw   = toDeg(body_yaw);

	if(body_pitch > 90.0)
	{
		body_yaw  = body_yaw + 180.0;
		body_pitch= 180.0 - body_pitch;
	}
	else if(body_pitch < 0.0)
	{
		body_yaw  = body_yaw + 180.0;
		body_pitch= -body_pitch;
	}

	printf("body_yaw,%f,body_pitch,%f\r\n",body_yaw,body_pitch);

}