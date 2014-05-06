#include "ahrs.h"
#include "config.h"

#define Kp 15.0f
#define Ki 0.020f//0.02f

#define w_mag 0.5f
#define w_imu 0.5f//0.02f

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

	//printf("GyrX,%f,GyrY,%f,GyrZ,%f\r\n",GyrX,GyrY,GyrZ);

	/*use correct gyr value to update quaternion*/
	Quaternion_RungeKutta(&qua, GyrX, GyrY, GyrZ, 0.005f);
	/*quaternion normalization*/
	Quaternion_Normalize(&qua);
	/*convert quaternion to angle*/
	Quaternion_ToAngE(&qua, &ang);
	
	/*compass yaw*/
	//magn_x = ((mag.x - mag.EllipseX0) * arm_cos_f32(mag.EllipseSita) - (mag.y - mag.EllipseY0) * arm_sin_f32(mag.EllipseSita)) / mag.EllipseA;
	//magn_y = ((mag.x - mag.EllipseX0) * arm_sin_f32(mag.EllipseSita) + (mag.y - mag.EllipseY0) * arm_cos_f32(mag.EllipseSita)) / mag.EllipseB;
	magn_x = (mag.x - mag.magx_offset);
	magn_y = (mag.y - mag.magy_offset);
	magn_z = (mag.z - mag.magz_offset);
	//magn_yaw = toDeg(atan2f(magn_x, magn_y));
	//printf("%f %f %f %f\r\n",magn_x,magn_y,magn_z,magn_yaw);

	/*mag_yaw_correction*/
	magn_x = magn_x * arm_cos_f32(ang.Pitch) + magn_y * arm_sin_f32(ang.Pitch) * arm_sin_f32(ang.Roll) - magn_z * arm_cos_f32(ang.Roll) * arm_sin_f32(ang.Pitch);
	magn_y = magn_y * arm_cos_f32(ang.Roll) + magn_z * arm_sin_f32(ang.Roll);
	magn_yaw = toDeg(atan2f(magn_x, magn_y));
	//printf("magne_Yaw,%f\r\n",magn_yaw);

	ang.Pitch = (ang.Pitch);
	ang.Roll  = (ang.Roll);
	ang.Yaw   = 2*(ang.Yaw);
	
	Mq11 = arm_cos_f32(ang.Pitch)*arm_cos_f32(ang.Yaw);
   	Mq12 = arm_cos_f32(ang.Pitch)*arm_sin_f32(ang.Yaw);
	Mq13 = -arm_sin_f32(ang.Pitch);
  	Mq21 = arm_cos_f32(ang.Yaw)*arm_sin_f32(ang.Pitch)*arm_sin_f32(ang.Roll) - arm_cos_f32(ang.Roll)*arm_sin_f32(ang.Yaw);
   	Mq22 = arm_sin_f32(ang.Yaw)*arm_sin_f32(ang.Pitch)*arm_sin_f32(ang.Roll) + arm_cos_f32(ang.Roll)*arm_cos_f32(ang.Yaw);
	Mq23 = arm_cos_f32(ang.Pitch)*arm_sin_f32(ang.Roll);
   	Mq31 = arm_cos_f32(ang.Roll)*arm_cos_f32(ang.Yaw)*arm_sin_f32(ang.Pitch) + arm_sin_f32(ang.Roll)*arm_sin_f32(ang.Yaw);
   	Mq32 = arm_sin_f32(ang.Yaw)*arm_sin_f32(ang.Pitch)*arm_cos_f32(ang.Roll) - arm_sin_f32(ang.Roll)*arm_cos_f32(ang.Yaw);
	Mq33 = arm_cos_f32(ang.Pitch)*arm_cos_f32(ang.Roll);

	/*altitude*/
	ang.Pitch = toDeg(ang.Pitch);
	ang.Roll  = toDeg(ang.Roll);
	ang.Yaw   = toDeg(ang.Yaw);

	if(ang.Yaw > 180)
	{
		ang.Yaw = ang.Yaw - 360;
	}
	else if(ang.Yaw < -180)
	{
		ang.Yaw = ang.Yaw + 360;
	}
	//printf("ang.Pitch,%f,ang.Roll,%f,ang.Yaw,%f\r\n",ang.Pitch,ang.Roll,ang.Yaw);
	//printf("imus_Yaw,%f,magne_Yaw,%f\r\n",ang.Yaw,magn_yaw);
	
	/*complementory filter*/
	//ang.Yaw = w_mag*magn_yaw + w_imu*ang.Yaw; 


}


