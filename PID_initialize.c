/*
 * PID_initialize.c
 *
 *  Created on: 2017年10月18日
 *      Author: ZombiesYard
 */
#include <msp430f5529.h>
#include "PID_initialize.h"
#include "PID.h"
#include "IMU.h"
//unsigned int pwm_arry[4] = {12490,10625,9750,20};
/*
unsigned int Motor0;
unsigned int Motor1;
unsigned int Motor2;
unsigned int Motor3;
*/
unsigned int i;

void int_motor_gpio() //电机4路pwm的GPIO初始化设置
{

	P2SEL|=BIT4;   							//复用功能选择
    P2DIR|=BIT4;   							//输出
    P2DS |=BIT4; 							//全电流驱动
    P2OUT&=~BIT4;   						//初始化输出低电平


	P2SEL|=BIT5;   							//复用功能选择
    P2DIR|=BIT5;   							//输出
    P2DS |=BIT5; 							//全电流驱动
    P2OUT&=~BIT5;   						//初始化输出低电平

    P1SEL|=BIT4;   							//复用功能选择
    P1DIR|=BIT4;   							//输出
    P1DS |=BIT4; 							//全电流驱动
    P1OUT&=~BIT4;   						//初始化输出低电平


    P1SEL|=BIT5;   							//复用功能选择
    P1DIR|=BIT5;   							//输出
    P1DS |=BIT5; 							//全电流驱动
    P1OUT&=~BIT5;   						//初始化输出低电平



    P7SEL |= BIT4;                          //
    P7DIR |= BIT4;   					    //
    P7DS |= BIT4; 							//
    P7OUT &= ~BIT4;   						//

	P3SEL |= BIT5;                          //
    P3DIR |= BIT5;   					    //
    P3DS |= BIT5; 							//
    P3OUT &= ~BIT5;   						//

	P3SEL |= BIT6;                          //
    P3DIR |= BIT6;   					    //
    P3DS |= BIT6; 							//
    P3OUT &= ~BIT6;   						//

}

/*void int_array()
{
	int l;
	int k=0;
	for(l=12501;l>0;l=l-10)
	{
		Motor0[k]=l;
		Motor1[k]=l;
		Motor2[k]=l;
		Motor3[k]=l;
		k++;
	}
}*/

void delay()          //此函数设置启动的延时
{

  unsigned long long k;
  for(k=9998940;k>=1;k--);

}


/*void int_pwm()                              //此函数设置pwm的占空比
{

	TA2CCTL0=CCIE;     						//定时器A1的捕获比较器0开启CCIFG位中断
    TA2CCR0=25002;       					//置入计数终值，则PWM频率为500HZ
    TA2CCTL1=CCIE;     						//捕获比较器1开启中断
    //TA2CCR1=Motor0*10;        			//占空比为%
    TA2CTL|=TACLR;     						//将计时器A1清零
    TA2CTL|=TASSEL_2+MC_3;   				//定时器选择SMCLK作为时钟源，且为增减模式
    TA2CCTL1=OUTMOD_4; 						//定时器A1中的捕获比较器1，输出模式为4翻转

	TBCCTL0=CCIE;     						//定时器B的捕获比较器0开启CCIFG位中断
    TBCCR0=25002;       					//置入计数终值，则PWM频率为500HZ
	TBCCTL2=CCIE;     						//捕获比较器2开启中断
	TBCCTL5=CCIE;     						//捕获比较器5开启中断
	TBCCTL6=CCIE;     						//捕获比较器6开启中断

	//TBCCR2=Motor1*10;        				//
	//TBCCR5=Motor2*10;        				//
	//TBCCR6=Motor3*10;        				//
    TBCTL|=TBCLR;     						//将计时器b清零
    TBCTL|=TBSSEL_2+MC_3;   				//定时器选择SMCLK作为时钟源，且为增减模式

	TBCCTL2=OUTMOD_4; 						//定时器B中的捕获比较器2，输出模式为4翻转
	TBCCTL5=OUTMOD_4; 						//定时器B中的捕获比较器5，输出模式为4翻转
	TBCCTL6=OUTMOD_4; 						//定时器B中的捕获比较器6，输出模式为4翻转

}*/


void int_pwm()                              //此函数设置pwm的占空比
{



	P2DIR |= BIT2;                          // SMCLK set out to pins
	P2SEL |= BIT2;

		TA2CCTL0=CCIE;     						//定时器A1的捕获比较器0开启CCIFG位中断
		TA2CCR0=50004;       					//置入计数终值，则PWM频率为500HZ
		TA2CCTL1=CCIE;     						//捕获比较器1开启中断

		TA2CTL|=TACLR;     						//将计时器A1清零
		TA2CTL|=TASSEL_2+MC_1;   				//定时器选择SMCLK作为时钟源，且为增减模式
		TA2CCTL1=OUTMOD_7; 						//定时器A1中的捕获比较器1，输出模式为4翻转

		TBCCTL0=CCIE;     						//定时器A1的捕获比较器0开启CCIFG位中断
		TBCCR0=50004;       					//置入计数终值，则PWM频率为500HZ
	    TBCCTL2=CCIE;     						//捕获比较器1开启中断
		TBCCTL5=CCIE;     						//捕获比较器1开启中断
		TBCCTL6=CCIE;     						//捕获比较器1开启中断

		TBCTL|=TBCLR;     						//将计时器A1清零
		TBCTL|=TBSSEL_2+MC_1;   				//定时器选择SMCLK作为时钟源，且为增减模式

	    TBCCTL2=OUTMOD_7; 						//定时器A1中的捕获比较器1，输出模式为4翻转
		TBCCTL5=OUTMOD_7; 						//定时器A1中的捕获比较器1，输出模式为4翻转
		TBCCTL6=OUTMOD_7; 						//定时器A1中的捕获比较器1，输出模式为4翻转

		if(flag==0)
		{
			int_throttle_or_zero_throttle();flag++;delay_ms(3000);
		}


	//delay_ms(3000);
       takeoff();

}



void int_buttons()                          //此函数设置GPIO
{

	P1DIR &=~BIT3  ;							//
	P1IFG &=~BIT3;
	P1IE  |=BIT3;
	P1IES |=BIT3;
	P1OUT |=BIT3;
	P1REN |=BIT3;

	P1DIR &=~BIT2;							//启动和结束的GPIO
	P1IFG &=~BIT2;
	P1IE  |=BIT2;
	P1IES |=BIT2;
	P1OUT |=BIT2;
	P1REN |=BIT2;


	P1DIR &=~BIT1;							//启动和结束的GPIO
	P1IFG &=~BIT1;
	P1IE  |=BIT1;
	P1IES |=BIT1;
	P1OUT |=BIT1;
	P1REN |=BIT1;

	/*P1DIR &=~BIT4;							//
	P1IFG &=~BIT4;
	P1IE  |=BIT4;
	P1IES |=BIT4;
	P1OUT |=BIT4;
	P1REN |=BIT4;*/

	P2DIR &=~BIT1;							//
	P2IFG &=~BIT1;
	P2IE  |=BIT1;
	P2IES |=BIT1;
	P2OUT |=BIT1;
	P2REN |=BIT1;

}

void int_throttle_or_zero_throttle()     //此函数用于设置零油门校准行程或者切断油门输出
{

	//TA0CCR3=25000;
	//TA0CCR4=25000;
	TA2CCR1=25000;       			//占空比为%
	//TA2CCR2=25000;        				//


	TBCCR2=25000;
	TBCCR5=25000;        				//
	TBCCR6=25000;

 }
void takeoff()      //起飞函数
{

	/*TA2CCR1=35000;
    TBCCR2=35000;        				//
	TBCCR5=35000;        				//
	TBCCR6=35000;*/

	//TA0CCR3=35000+Motor_0*5;
	//TA0CCR4=35000+Motor_1*5;
	TA2CCR1=35000+Motor_0*4;
	//TA2CCR2=35000+Motor_3*5;


    TBCCR2=35000+Motor_1*4;        				//
	TBCCR5=35000+Motor_2*4;        				//
	TBCCR6=35000+Motor_3*4;


}



