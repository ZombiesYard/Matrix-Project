/*
 * PID_initialize.c
 *
 *  Created on: 2017��10��18��
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

void int_motor_gpio() //���4·pwm��GPIO��ʼ������
{

	P2SEL|=BIT4;   							//���ù���ѡ��
    P2DIR|=BIT4;   							//���
    P2DS |=BIT4; 							//ȫ��������
    P2OUT&=~BIT4;   						//��ʼ������͵�ƽ


	P2SEL|=BIT5;   							//���ù���ѡ��
    P2DIR|=BIT5;   							//���
    P2DS |=BIT5; 							//ȫ��������
    P2OUT&=~BIT5;   						//��ʼ������͵�ƽ

    P1SEL|=BIT4;   							//���ù���ѡ��
    P1DIR|=BIT4;   							//���
    P1DS |=BIT4; 							//ȫ��������
    P1OUT&=~BIT4;   						//��ʼ������͵�ƽ


    P1SEL|=BIT5;   							//���ù���ѡ��
    P1DIR|=BIT5;   							//���
    P1DS |=BIT5; 							//ȫ��������
    P1OUT&=~BIT5;   						//��ʼ������͵�ƽ



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

void delay()          //�˺���������������ʱ
{

  unsigned long long k;
  for(k=9998940;k>=1;k--);

}


/*void int_pwm()                              //�˺�������pwm��ռ�ձ�
{

	TA2CCTL0=CCIE;     						//��ʱ��A1�Ĳ���Ƚ���0����CCIFGλ�ж�
    TA2CCR0=25002;       					//���������ֵ����PWMƵ��Ϊ500HZ
    TA2CCTL1=CCIE;     						//����Ƚ���1�����ж�
    //TA2CCR1=Motor0*10;        			//ռ�ձ�Ϊ%
    TA2CTL|=TACLR;     						//����ʱ��A1����
    TA2CTL|=TASSEL_2+MC_3;   				//��ʱ��ѡ��SMCLK��Ϊʱ��Դ����Ϊ����ģʽ
    TA2CCTL1=OUTMOD_4; 						//��ʱ��A1�еĲ���Ƚ���1�����ģʽΪ4��ת

	TBCCTL0=CCIE;     						//��ʱ��B�Ĳ���Ƚ���0����CCIFGλ�ж�
    TBCCR0=25002;       					//���������ֵ����PWMƵ��Ϊ500HZ
	TBCCTL2=CCIE;     						//����Ƚ���2�����ж�
	TBCCTL5=CCIE;     						//����Ƚ���5�����ж�
	TBCCTL6=CCIE;     						//����Ƚ���6�����ж�

	//TBCCR2=Motor1*10;        				//
	//TBCCR5=Motor2*10;        				//
	//TBCCR6=Motor3*10;        				//
    TBCTL|=TBCLR;     						//����ʱ��b����
    TBCTL|=TBSSEL_2+MC_3;   				//��ʱ��ѡ��SMCLK��Ϊʱ��Դ����Ϊ����ģʽ

	TBCCTL2=OUTMOD_4; 						//��ʱ��B�еĲ���Ƚ���2�����ģʽΪ4��ת
	TBCCTL5=OUTMOD_4; 						//��ʱ��B�еĲ���Ƚ���5�����ģʽΪ4��ת
	TBCCTL6=OUTMOD_4; 						//��ʱ��B�еĲ���Ƚ���6�����ģʽΪ4��ת

}*/


void int_pwm()                              //�˺�������pwm��ռ�ձ�
{



	P2DIR |= BIT2;                          // SMCLK set out to pins
	P2SEL |= BIT2;

		TA2CCTL0=CCIE;     						//��ʱ��A1�Ĳ���Ƚ���0����CCIFGλ�ж�
		TA2CCR0=50004;       					//���������ֵ����PWMƵ��Ϊ500HZ
		TA2CCTL1=CCIE;     						//����Ƚ���1�����ж�

		TA2CTL|=TACLR;     						//����ʱ��A1����
		TA2CTL|=TASSEL_2+MC_1;   				//��ʱ��ѡ��SMCLK��Ϊʱ��Դ����Ϊ����ģʽ
		TA2CCTL1=OUTMOD_7; 						//��ʱ��A1�еĲ���Ƚ���1�����ģʽΪ4��ת

		TBCCTL0=CCIE;     						//��ʱ��A1�Ĳ���Ƚ���0����CCIFGλ�ж�
		TBCCR0=50004;       					//���������ֵ����PWMƵ��Ϊ500HZ
	    TBCCTL2=CCIE;     						//����Ƚ���1�����ж�
		TBCCTL5=CCIE;     						//����Ƚ���1�����ж�
		TBCCTL6=CCIE;     						//����Ƚ���1�����ж�

		TBCTL|=TBCLR;     						//����ʱ��A1����
		TBCTL|=TBSSEL_2+MC_1;   				//��ʱ��ѡ��SMCLK��Ϊʱ��Դ����Ϊ����ģʽ

	    TBCCTL2=OUTMOD_7; 						//��ʱ��A1�еĲ���Ƚ���1�����ģʽΪ4��ת
		TBCCTL5=OUTMOD_7; 						//��ʱ��A1�еĲ���Ƚ���1�����ģʽΪ4��ת
		TBCCTL6=OUTMOD_7; 						//��ʱ��A1�еĲ���Ƚ���1�����ģʽΪ4��ת

		if(flag==0)
		{
			int_throttle_or_zero_throttle();flag++;delay_ms(3000);
		}


	//delay_ms(3000);
       takeoff();

}



void int_buttons()                          //�˺�������GPIO
{

	P1DIR &=~BIT3  ;							//
	P1IFG &=~BIT3;
	P1IE  |=BIT3;
	P1IES |=BIT3;
	P1OUT |=BIT3;
	P1REN |=BIT3;

	P1DIR &=~BIT2;							//�����ͽ�����GPIO
	P1IFG &=~BIT2;
	P1IE  |=BIT2;
	P1IES |=BIT2;
	P1OUT |=BIT2;
	P1REN |=BIT2;


	P1DIR &=~BIT1;							//�����ͽ�����GPIO
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

void int_throttle_or_zero_throttle()     //�˺�����������������У׼�г̻����ж��������
{

	//TA0CCR3=25000;
	//TA0CCR4=25000;
	TA2CCR1=25000;       			//ռ�ձ�Ϊ%
	//TA2CCR2=25000;        				//


	TBCCR2=25000;
	TBCCR5=25000;        				//
	TBCCR6=25000;

 }
void takeoff()      //��ɺ���
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



