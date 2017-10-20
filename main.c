/*
*   ����:MSP430     P3.0   P3.1  GND   GND
*           |        |     |     |    |
*       MPU6050     SDA   SCL   GND  AD0
*       �������Ӳ��I2C�ӿڲ�ѯ��ʽ��MPU6050ͨ��
*
*       ����UART���ڳ��򣬽��ɼ���ֵͨ�����ڴ�ӡ����Ļ�ϡ�
*
*/
#include "msp430f5529.h"
#include "UART_F5529.h"
#include "I2C_F5529.h"
#include "PID_initialize.h"
#include "MPU6050.h"
#include "math.h"
#include "Ucs_Init_Set.h"
#include "IMU.h"
#include "stdio.h"
#include <stdint.h>


#define CPU_F ((double)25000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

unsigned char Send_Count; //������Ҫ���͵����ݸ���
unsigned char c;          //��������
//int Motor_0,Motor_1,Motor_2,Motor_3;
int ax,ay,az; //���ٶ�ԭʼ����
int gx,gy,gz; //���ٶ�ԭʼ����
int flag=0;
float aax,aay,aaz;//�洢�����������
float ggx,ggy,ggz; //���ٶ�ֵ

float Ax,Ay,Az;//��λ g(9.8m/s^2)

float Gx,Gy,Gz;//��λ ��/s
float Angel_accX,Angel_accY,Angel_accZ;//�洢���ٶȼ�����ĽǶ�

//unsigned int i=0;
/*
 * ��ȡ���ٶ�ֵ,���洢���ٶȼ�����ĽǶ�
 */
void GetData_Accel(void)
{
    //���ٶ�ԭʼ����
    //delay_ms(1);
    ax = GetAccelX ();
    //delay_ms(1);
    ay = GetAccelY ();
    //delay_ms(1);
    az = GetAccelZ ();

    //======һ�������ǶԼ��ٶȽ����������ó���λΪg�ļ��ٶ�ֵ
    Ax=ax/16384.00;
    Ay=ay/16384.00;
    Az=az/16384.00;

    //==========�����������ü��ٶȼ����������ˮƽ������ϵ֮��ļн�
    Angel_accX=acos(Ax/(sqrt(Ax*Ax+Az*Az+Ay*Ay)))*180/3.14;//�����������ļн�
    Angel_accY=acos(Ay/(sqrt(Ax*Ax+Ay*Ay+Az*Az)))*180/3.14;
    Angel_accZ=acos(Az/(sqrt(Ax*Ax+Ay*Ay+Az*Az)))*180/3.14;
}

//��ȡ���ٶ�
void GetData_Gyro(void)
{
    //���������ǵ�ԭʼ����
    //delay_ms(1);
    gx = GetAnguX ();
    //delay_ms(1);
    gy = GetAnguY ();
    //delay_ms(1);
    gz = GetAnguZ ();

    //==========���������ǶԽ��ٶ�������==========
    ggx=gx/16.40;
    ggy=gy/16.40;
    ggz=gz/16.40;

}

void main( void )
{
    //Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;


    Ucs_25Mhz();//��ʱ��A0 ����ʱ
	int_motor_gpio();

    int_buttons();
    UART_Init();
    MPU6050Init();

    _EINT();


   // while(1)
   /* {
    	CONTROL();
    	    	Get_Attitude();
    	    	Prepare_Data();

    	    	P4OUT ^= BIT7;
    	        GetData_Accel();
    	        GetData_Gyro();
        int_pwm();







    }*/
}



/*#pragma vector=PORT1_VECTOR
__interrupt void control(void)
{
	unsigned int l=0;             //�ж������г��Ƿ�����
	while(1)
	{
		switch(l)
		{
			case 0: int_throttle_or_zero_throttle();break;
			default: break;
		}
		l++;

		if((P1IN&BIT1)==0)
		{
			takeoff();
			P1IFG &=~BIT1;
		}

		if((P1IN&BIT1)!=0)
		{
			int_throttle_or_zero_throttle();
			P1IFG &=~BIT1;
		}

	}
}*/


#pragma vector=PORT1_VECTOR
__interrupt void control(void)
{



	while(1)
	{
		CONTROL();
								Get_Attitude();
								Prepare_Data();

								P4OUT ^= BIT7;
								GetData_Accel();
								GetData_Gyro();


								int_pwm();











	}









if((P1IN&BIT1)==0)					//
	{
		__delay_cycles(1000000);
		if((P1IN&BIT1)==0)
		{

			//int_throttle_or_zero_throttle();





			//P1IFG &=~BIT1;
		}
	}



                 if((P2IN&BIT1)==0)
	{
		__delay_cycles(1000000);			//
		if((P2IN&BIT1)==0)
		{


			//takeoff();
			P2IFG &=~BIT1;
		}
	}

}


