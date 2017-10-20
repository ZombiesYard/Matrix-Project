/*
*   连接:MSP430     P3.0   P3.1  GND   GND
*           |        |     |     |    |
*       MPU6050     SDA   SCL   GND  AD0
*       程序采用硬件I2C接口查询方式与MPU6050通信
*
*       调用UART串口程序，将采集的值通过串口打印在屏幕上。
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

unsigned char Send_Count; //串口需要发送的数据个数
unsigned char c;          //计数变量
//int Motor_0,Motor_1,Motor_2,Motor_3;
int ax,ay,az; //加速度原始数据
int gx,gy,gz; //角速度原始数据
int flag=0;
float aax,aay,aaz;//存储量化后的数据
float ggx,ggy,ggz; //角速度值

float Ax,Ay,Az;//单位 g(9.8m/s^2)

float Gx,Gy,Gz;//单位 °/s
float Angel_accX,Angel_accY,Angel_accZ;//存储加速度计算出的角度

//unsigned int i=0;
/*
 * 获取加速度值,及存储加速度计算出的角度
 */
void GetData_Accel(void)
{
    //加速度原始数据
    //delay_ms(1);
    ax = GetAccelX ();
    //delay_ms(1);
    ay = GetAccelY ();
    //delay_ms(1);
    az = GetAccelZ ();

    //======一下三行是对加速度进行量化，得出单位为g的加速度值
    Ax=ax/16384.00;
    Ay=ay/16384.00;
    Az=az/16384.00;

    //==========以下三行是用加速度计算三个轴和水平面坐标系之间的夹角
    Angel_accX=acos(Ax/(sqrt(Ax*Ax+Az*Az+Ay*Ay)))*180/3.14;//重力与各个轴的夹脚
    Angel_accY=acos(Ay/(sqrt(Ax*Ax+Ay*Ay+Az*Az)))*180/3.14;
    Angel_accZ=acos(Az/(sqrt(Ax*Ax+Ay*Ay+Az*Az)))*180/3.14;
}

//获取角速度
void GetData_Gyro(void)
{
    //读出陀螺仪的原始数据
    //delay_ms(1);
    gx = GetAnguX ();
    //delay_ms(1);
    gy = GetAnguY ();
    //delay_ms(1);
    gz = GetAnguZ ();

    //==========以下三行是对角速度做量化==========
    ggx=gx/16.40;
    ggy=gy/16.40;
    ggz=gz/16.40;

}

void main( void )
{
    //Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;


    Ucs_25Mhz();//定时器A0 做计时
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
	unsigned int l=0;             //判断油门行程是否设置
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


