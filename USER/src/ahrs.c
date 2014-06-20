#include "ahrs.h"
#include "config.h"
/*mahony filter*/
#define Kp 15.0f
#define Ki 0.020f//0.02f
/*complementory filter*/
#define w_mag 0.5f
#define w_imu 0.5f//0.02f
/*low pass filter*/
#define tau 0.01f
#define dt  0.01f

#define declination  3.36
 
float 		last_GyrZ;
EulerAngle 	last_ang;

void AHRS_Init(Quaternion *pNumQ, EulerAngle *pAngE)
{
	pNumQ->q0 = 1.0f;
	pNumQ->q1 = 0.0f;
	pNumQ->q2 = 0.0f;
	pNumQ->q3 = 0.0f;

	pAngE->Pitch = 0.0f;
	pAngE->Roll  = 0.0f;
	pAngE->Yaw   = 0.0f;
}

void ahrs_update()
{
	float 	difference = 0;

	float 	magn_x = 0;
	float 	magn_y = 0;
	float   magn_z = 0;
	float 	magn_yaw = 0;

	float 	Normalize;
	float 	gx, gy, gz;
// float 	hx, hy, hz;
// float 	wx, wy, wz;
// float 	bx, bz;
	float 	ErrX, ErrY, ErrZ;
	float 	AccX, AccY, AccZ;
	float 	GyrX, GyrY, GyrZ;
// float 	MegX, MegY, MegZ;
//	float 	Mq11, Mq12, Mq13, Mq21, Mq22, Mq23, Mq31, Mq32, Mq33;
	float   M13, M23, M33 ;

	static float AngZ_Temp = 0.0f;
	static float exInt = 0.0f, eyInt = 0.0f, ezInt = 0.0f;

	float 	klpf = 0;
	/*ACC,GYRO altitude*/

	/*normalize acc value*/
	Normalize = invSqrtf(squa(acc.x) + squa(acc.y) + squa(acc.z));
	AccX = acc.x * Normalize;
	AccY = acc.y * Normalize;
	AccZ = acc.z * Normalize;
	//printf("AccX,%f,AccY,%f,AccZ,%f\r\n",AccX,AccY,AccZ);

	/*show gravity in quaternion*/

   	//Mq11 = qua.q0*qua.q0 + qua.q1*qua.q1 - qua.q2*qua.q2 - qua.q3*qua.q3;
   	//Mq12 = 2.0f*(qua.q1*qua.q2 + qua.q0*qua.q3);
	M13 = 2.0f * (qua.q1 * qua.q3 - qua.q0 * qua.q2);
  	//Mq21 = 2.0f*(qua.q1*qua.q2 - qua.q0*qua.q3);
   	//Mq22 = qua.q0*qua.q0 - qua.q1*qua.q1 + qua.q2*qua.q2 - qua.q3*qua.q3;
	M23 = 2.0f * (qua.q0 * qua.q1 + qua.q2 * qua.q3);
   	//Mq31 = 2.0f*(qua.q0*qua.q2 + qua.q1*qua.q3);
   	//Mq32 = 2.0f*(qua.q2*qua.q3 - qua.q0*qua.q1);
	M33 = qua.q0 * qua.q0 - qua.q1 * qua.q1 - qua.q2 * qua.q2 + qua.q3 * qua.q3;

	gx = M13;
	gy = M23;
	gz = M33;

	/*cross product:(acc value) X (gravity)*/

	ErrX = (AccY * gz - AccZ * gy)/* + (MegY*wz - MegZ*wy)*/;
	ErrY = (AccZ * gx - AccX * gz)/* + (MegZ*wx - MegX*wz)*/;
	ErrZ = (AccX * gy - AccY * gx)/* + (MegX*wy - MegY*wx)*/;

	/*data fusion(Mahony filter)*/

	exInt = exInt + ErrX * Ki;
	eyInt = eyInt + ErrY * Ki;
	ezInt = ezInt + ErrZ * Ki;

	GyrX = toRad(gyr.x);
	GyrY = toRad(gyr.y);
	GyrZ = toRad(gyr.z);

	GyrX = GyrX + Kp * ErrX + exInt;
	GyrY = GyrY + Kp * ErrY + eyInt;
	GyrZ = GyrZ + Kp * ErrZ + ezInt;

	/*GyrZ drift correction*/
	/*
	difference = (GyrZ - last_GyrZ);
	if(difference < 0.005 && difference > -0.005)
	{
		GyrZ = 0.0;
	}
	last_GyrZ = GyrZ;

	//printf("%f,%f\r\n",difference,GyrZ);

	/*use correct gyr value to update quaternion*/
	Quaternion_RungeKutta(&qua, GyrX, GyrY, GyrZ, 0.005f);
	/*quaternion normalization*/
	Quaternion_Normalize(&qua);
	/*convert quaternion to angle*/
	Quaternion_ToAngE(&qua, &ang);

	/*low pass filter*/
	
	klpf = dt/(tau+dt);
	ang.Pitch = last_ang.Pitch + klpf * (ang.Pitch - last_ang.Pitch);
	ang.Roll  = last_ang.Roll  + klpf * (ang.Roll  - last_ang.Roll );
	//ang.Yaw   = last_ang.Yaw   + klpf * (ang.Yaw   - last_ang.Yaw  );

	last_ang.Pitch = ang.Pitch;
	last_ang.Roll  = ang.Roll;
	//last_ang.Yaw   = ang.Yaw;

	/*compass yaw*/
	//magn_x = ((mag.x - mag.EllipseX0) * arm_cos_f32(mag.EllipseSita) - (mag.y - mag.EllipseY0) * arm_sin_f32(mag.EllipseSita)) / mag.EllipseA;
	//magn_y = ((mag.x - mag.EllipseX0) * arm_sin_f32(mag.EllipseSita) + (mag.y - mag.EllipseY0) * arm_cos_f32(mag.EllipseSita)) / mag.EllipseB;
	magn_x = (mag_HMC5983.x - mag_HMC5983.magx_offset);
	magn_y = (mag_HMC5983.y - mag_HMC5983.magy_offset);
	magn_z = (mag_HMC5983.z - mag_HMC5983.magz_offset);
	//magn_yaw = toDeg(atan2f(magn_x, magn_y));
	//printf("%f %f %f %f\r\n",magn_x,magn_y,magn_z,magn_yaw);

	/*mag_yaw_correction*/
	magn_x = magn_x * arm_cos_f32(ang.Pitch) + magn_y * arm_sin_f32(ang.Pitch) * arm_sin_f32(ang.Roll) - magn_z * arm_cos_f32(ang.Roll) * arm_sin_f32(ang.Pitch);
	magn_y = magn_y * arm_cos_f32(ang.Roll) + magn_z * arm_sin_f32(ang.Roll);
	magn_yaw = atan2f(magn_x, magn_y) + toRad(declination);
/*	
	ang.Pitch = (ang.Pitch);
	ang.Roll  = (ang.Roll);
	ang.Yaw   = 5.2*(ang.Yaw);
*/	
	Mq11 = arm_cos_f32(ang.Pitch)*arm_cos_f32(magn_yaw);
   	Mq12 = arm_cos_f32(ang.Pitch)*arm_sin_f32(magn_yaw);
	Mq13 = -arm_sin_f32(ang.Pitch);
  	Mq21 = arm_cos_f32(magn_yaw)*arm_sin_f32(ang.Pitch)*arm_sin_f32(ang.Roll) - arm_cos_f32(ang.Roll)*arm_sin_f32(magn_yaw);
   	Mq22 = arm_sin_f32(magn_yaw)*arm_sin_f32(ang.Pitch)*arm_sin_f32(ang.Roll) + arm_cos_f32(ang.Roll)*arm_cos_f32(magn_yaw);
	Mq23 = arm_cos_f32(ang.Pitch)*arm_sin_f32(ang.Roll);
   	Mq31 = arm_cos_f32(ang.Roll)*arm_cos_f32(magn_yaw)*arm_sin_f32(ang.Pitch) + arm_sin_f32(ang.Roll)*arm_sin_f32(magn_yaw);
   	Mq32 = arm_sin_f32(magn_yaw)*arm_sin_f32(ang.Pitch)*arm_cos_f32(ang.Roll) - arm_sin_f32(ang.Roll)*arm_cos_f32(magn_yaw);
	Mq33 = arm_cos_f32(ang.Pitch)*arm_cos_f32(ang.Roll);

	/*altitude*/
	ang.Pitch = toDeg(ang.Pitch);
	ang.Roll  = toDeg(ang.Roll);
	ang.Yaw =  toDeg(magn_yaw);
/*
	if(ang.Yaw > 180)
	{
		ang.Yaw = ang.Yaw - 360;
	}
	else if(ang.Yaw < -180)
	{
		ang.Yaw = ang.Yaw + 360;
	}
*/
	
	//printf("pitch,%f,roll,%f,magne_Yaw,%f\r\n",ang.Pitch,ang.Roll,magn_yaw);
	//printf("ang.Pitch,%f,ang.Roll,%f,ang.Yaw,%f\r\n",ang.Pitch,ang.Roll,ang.Yaw);
	//printf("imus_Yaw,%f,magne_Yaw,%f\r\n",ang.Yaw,magn_yaw);
	//printf("%f %f %f\r\n",ang.Pitch,ang.Roll,ang.Yaw);
	/*complementory filter*/
	//ang.Yaw = w_mag*magn_yaw + w_imu*ang.Yaw; 


}


