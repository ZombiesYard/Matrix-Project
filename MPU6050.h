/*
 * MPU6050.h
 *
 *  Created on: 2015-7-17
 *      Author:  Czh
 */

#ifndef MPU6050_H_
#define MPU6050_H_

//****************************************
// 定义MPU6050内部地址
//****************************************
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)

#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)

#define	ACCEL_XOUT_H	0x3B    //加速度
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41    // 存储的最近温度传感器的测量值
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43    //角速度
#define	GYRO_XOUT_L		0x44
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define SlaveAddr		0x68    //MPU6050 address when AD0==0.

extern int MPU6050_ACC_LAST_X,MPU6050_ACC_LAST_Y,MPU6050_ACC_LAST_Z;
extern int MPU6050_GYRO_LAST_X,MPU6050_GYRO_LAST_Y,MPU6050_GYRO_LAST_Z;
extern unsigned char GYRO_OFFSET_OK;
extern unsigned char ACC_OFFSET_OK;

//****************************************
// 预定义MPU6050.c 函数
//****************************************
extern void MPU6050Init();
extern int Get16Bit(unsigned char address);
extern void MPU6050_Dataanl(void);
// X/Y/Z-Axis Acceleration
extern int GetAccelX ();
extern int GetAccelY ();
extern int GetAccelZ ();

// X/Y/Z-Axis Angular velocity
extern int GetAnguX ();
extern int GetAnguY ();
extern int GetAnguZ ();
#endif /* MPU6050_H_ */
