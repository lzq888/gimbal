#ifndef __AHRS_H
#define __AHRS_H

#include "config.h"

void AHRS_Init(Quaternion *pNumQ, EulerAngle *pAngE);
void ahrs_update();




#endif