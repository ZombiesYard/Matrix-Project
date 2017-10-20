#include "MPU6050.h"
#include "IMU.h"
#include <math.h>
#include "PID.h"



float rol_i=0,pit_i=0,yaw_p=0;
float thr=0;

/*********************************/
float Pitch_i,Roll_i,Yaw_i;                                   //积分项
float Pitch_old,Roll_old,Yaw_old;                 //角度保存
float Pitch_d,Roll_d,Yaw_d;          //微分项
float RC_Pitch,RC_Roll,RC_Yaw;                       //姿态角
float Pitch_shell_out,Roll_shell_out,Yaw_shell_out;//外环总输出
        //外环PID参数
float Pitch_shell_kp=200;//30 140
float Pitch_shell_kd=0;//
float Pitch_shell_ki=0;//
/*********************************/
float Roll_shell_kp=200;//30
float Roll_shell_kd=0;//10
float Roll_shell_ki=0;//0.08
/*********************************/
float Yaw_shell_kp=1.5;//10;//30
float Yaw_shell_kd=0;//10
float Yaw_shell_ki=0;//0.08;//0.08
float Gyro_radian_old_x,Gyro_radian_old_y,Gyro_radian_old_z;//陀螺仪保存
float pitch_core_kp_out,pitch_core_kd_out,Roll_core_kp_out,Roll_core_kd_out,Yaw_core_kp_out,Yaw_core_kd_out;//内环单项输出
float Pitch_core_out,Roll_core_out,Yaw_core_out;//内环总输出

//内环PID参数
//float Pitch_core_kp=0.040;
//float Pitch_core_kd=0.008;////0.007;//0.07;
float Pitch_core_kp=0.040;
float Pitch_core_kd=0.002;////0.007;//0.07;

float Roll_core_kp=0.040;//;
float Roll_core_kd=0.002;////0.007;//06;//0.07;

float Yaw_core_kp=0.046;//;
float Yaw_core_kd=0.012;////0.007;//06;//0.07;


int Motor_0,Motor_1,Motor_2,Motor_3;

float tempjd=0;
void CONTROL()
{

         RC_Pitch=0;

        ////////////////////////外环角度环(PID)///////////////////////////////
  Pitch_i+=(Q_ANGLE_X-RC_Pitch);
//-------------Pitch积分限幅----------------//
  if(Pitch_i>300) Pitch_i=300;
  else if(Pitch_i<-300) Pitch_i=-300;
//-------------Pitch微分--------------------//
  Pitch_d=Q_ANGLE_X-Pitch_old;
//-------------Pitch  PID-------------------//
  Pitch_shell_out = Pitch_shell_kp*(Q_ANGLE_X-RC_Pitch) + Pitch_shell_ki*Pitch_i + Pitch_shell_kd*Pitch_d;
//角度保存
  Pitch_old=Q_ANGLE_X;
/*********************************************************/

        RC_Roll=0;
        Roll_i+=(Q_ANGLE_Y-RC_Roll);
//-------------Roll积分限幅----------------//
  if(Roll_i>300) Roll_i=300;
  else if(Roll_i<-300) Roll_i=-300;
//-------------Roll微分--------------------//
  Roll_d=Q_ANGLE_Y-Roll_old;
//-------------Roll  PID-------------------//
  Roll_shell_out  = Roll_shell_kp*(Q_ANGLE_Y-RC_Roll) + Roll_shell_ki*Roll_i + Roll_shell_kd*Roll_d;
//------------Roll角度保存------------------//
  Roll_old=Q_ANGLE_Y;


        RC_Yaw=0;
//-------------Yaw微分--------------------//
  Yaw_d=MPU6050_GYRO_LAST_Z-Yaw_old;
//-------------Roll  PID-------------------//
  Yaw_shell_out  = Yaw_shell_kp*(MPU6050_GYRO_LAST_Z-RC_Yaw) + Yaw_shell_ki*Yaw_i + Yaw_shell_kd*Yaw_d;
//------------Roll角度保存------------------//
  Yaw_old=MPU6050_GYRO_LAST_Z;


        ////////////////////////内环角速度环(PD)///////////////////////////////
  pitch_core_kp_out = Pitch_core_kp * (Pitch_shell_out + MPU6050_GYRO_LAST_Y * 3.5);
  pitch_core_kd_out = Pitch_core_kd * (MPU6050_GYRO_LAST_Y   - Gyro_radian_old_y);

  Roll_core_kp_out  = Roll_core_kp  * (Roll_shell_out  + MPU6050_GYRO_LAST_X *3.5);
  Roll_core_kd_out  = Roll_core_kd  * (MPU6050_GYRO_LAST_X   - Gyro_radian_old_x);

  Yaw_core_kp_out  = Yaw_core_kp  * (Yaw_shell_out  + MPU6050_GYRO_LAST_Z * 1);
  Yaw_core_kd_out  = Yaw_core_kd  * (MPU6050_GYRO_LAST_Z   - Gyro_radian_old_z);


        Pitch_core_out = pitch_core_kp_out + pitch_core_kd_out;
  Roll_core_out  = Roll_core_kp_out  + Roll_core_kd_out;
  Yaw_core_out   = Yaw_core_kp_out   + Yaw_core_kd_out;

  Gyro_radian_old_y = MPU6050_GYRO_LAST_X;
  Gyro_radian_old_x = MPU6050_GYRO_LAST_Y;
  Gyro_radian_old_z = MPU6050_GYRO_LAST_Z;   //储存历史值

//--------------------将输出值融合到四个电机--------------------------------//



  Motor_2=(int)(thr - Roll_core_out - Pitch_core_out- Yaw_core_out);

  Motor_0=(int)(thr + Roll_core_out + Pitch_core_out- Yaw_core_out);

  Motor_3=(int)(thr + Roll_core_out - Pitch_core_out+ Yaw_core_out);

  Motor_1=(int)(thr - Roll_core_out + Pitch_core_out+ Yaw_core_out);


}



