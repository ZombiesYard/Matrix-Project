/*
 * MPU6050.c
 *
 *  Created on: 2015-7-17
 *      Author:  Czh
 *
 *  mpu6050��ϸ����ѯ���� QQ:2021525982
 */
#include "MPU6050.h"
#include "I2C_F5529.h"
unsigned char   mpu6050_buffer[14];     //I2C��ȡ��������

int ACC_OFFSET_X,ACC_OFFSET_Y,ACC_OFFSET_Z;
int GYRO_OFFSET_X,GYRO_OFFSET_Y,GYRO_OFFSET_Z;

unsigned char	GYRO_OFFSET_OK = 1;
unsigned char	ACC_OFFSET_OK = 1;

int MPU6050_ACC_LAST_X,MPU6050_ACC_LAST_Y,MPU6050_ACC_LAST_Z;
int MPU6050_GYRO_LAST_X,MPU6050_GYRO_LAST_Y,MPU6050_GYRO_LAST_Z;

/**********************************************************/
//�������ƣ�void MPU6050_Dataanl
//��ڲ�������
//���ڲ�������
//�������ܣ�MPU6050���ݶ�ȡ������
/**********************************************************/
void MPU6050_Dataanl(void)
{
  MPU6050_ACC_LAST_X = GetAccelX() - ACC_OFFSET_X;
  MPU6050_ACC_LAST_Y = GetAccelY() - ACC_OFFSET_Y;
  MPU6050_ACC_LAST_Z = GetAccelZ() - ACC_OFFSET_Z;

  MPU6050_GYRO_LAST_X = GetAnguX() - GYRO_OFFSET_X;
  MPU6050_GYRO_LAST_Y = GetAnguY() - GYRO_OFFSET_Y;
  MPU6050_GYRO_LAST_Z = GetAnguZ() - GYRO_OFFSET_Z;

  if(!GYRO_OFFSET_OK)
  {
    static long int tempgx=0,tempgy=0,tempgz=0;
    static unsigned char cnt_g=0;

    if(cnt_g==0)
    {
      GYRO_OFFSET_X=0;
      GYRO_OFFSET_Y=0;
      GYRO_OFFSET_Z=0;
      tempgx = 0;
      tempgy = 0;
      tempgz = 0;
      cnt_g = 1;
    }
    tempgx+= MPU6050_GYRO_LAST_X;
    tempgy+= MPU6050_GYRO_LAST_Y;
    tempgz+= MPU6050_GYRO_LAST_Z;
    if(cnt_g==200)
    {
      GYRO_OFFSET_X=tempgx/cnt_g;
      GYRO_OFFSET_Y=tempgy/cnt_g;
      GYRO_OFFSET_Z=tempgz/cnt_g;
      cnt_g = 0;
      GYRO_OFFSET_OK = 1;

    }
    cnt_g++;
  }
  if(!ACC_OFFSET_OK)
  {
    static long int tempax=0,tempay=0,tempaz=0;
    static unsigned char cnt_a=0;

    if(cnt_a==0)
    {
      ACC_OFFSET_X = 0;
      ACC_OFFSET_Y = 0;
      ACC_OFFSET_Z = 0;
      tempax = 0;
      tempay = 0;
      tempaz = 0;
      cnt_a = 1;

    }
    tempax += MPU6050_ACC_LAST_X;//�ۼ�
    tempay += MPU6050_ACC_LAST_Y;
    tempaz += MPU6050_ACC_LAST_Z;
    if(cnt_a==200)
    {
      ACC_OFFSET_X = tempax/cnt_a;
      ACC_OFFSET_Y = tempay/cnt_a;
      ACC_OFFSET_Z = tempaz/cnt_a;
      cnt_a = 0;
      ACC_OFFSET_OK = 1;

    }
    cnt_a++;
  }
}


//��ʼ��6050
void MPU6050Init()
{
	I2C_Init();
	int i=0,j=0;
	//�ڳ�ʼ��֮ǰҪ��ʱһ��ʱ�䣬��û����ʱ����ϵ�����ϵ����ݿ��ܻ����
	for (i=0; i<112; i++)
	{
	    for (j=0; j<112; j++)
	    {
	        ;
	    }
	}
	Single_Write(PWR_MGMT_1,0x00); //�������״̬
	Single_Write(SMPLRT_DIV, 0x07);//�����ǲ����ʣ�1KHz
	Single_Write(CONFIG, 0x06); //��ͨ�˲��������ã���ֹƵ����1K��������5K
	Single_Write(GYRO_CONFIG, 0x18);//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
	Single_Write(ACCEL_CONFIG, 0x01);

}

//������
int Get16Bit (unsigned char address)
{
	unsigned char ho,lo;
	ho = Single_Read(address);
	lo = Single_Read(address+1);
	return (ho<<8)+lo;
}

// X/Y/Z-Axis Acceleration ���ٶ�
int GetAccelX ()
{
	return Get16Bit(ACCEL_XOUT_H);
}
int GetAccelY ()
{
	return Get16Bit(ACCEL_YOUT_H);
}
int GetAccelZ ()
{
	return Get16Bit(ACCEL_ZOUT_H);
}

// X/Y/Z-Axis Angular velocity
int GetAnguX ()    //���ٶ�
{
	return Get16Bit(GYRO_XOUT_H);
}
int GetAnguY ()
{
	return Get16Bit(GYRO_YOUT_H);
}
int GetAnguZ ()
{
	return Get16Bit(GYRO_ZOUT_H);
}

