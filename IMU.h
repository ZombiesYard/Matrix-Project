#ifndef _IMU_H_
#define _IMU_H_

#include "mpu6050.h"
#include "math.h"

extern float Q_ANGLE_X,Q_ANGLE_Y,Q_ANGLE_Z;
extern int Motor_0,Motor_1,Motor_2,Motor_3;
extern int flag;
void Prepare_Data(void);
void Get_Attitude(void);
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);

#endif
