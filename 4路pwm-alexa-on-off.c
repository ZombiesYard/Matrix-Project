#include <msp430f5529.h>
unsigned int pwm_arry[4] = {6144,5000,3000,20};
unsigned int i;
void int_motor_gpio() //���4·pwm��GPIO��ʼ������
{
	
	P2SEL|=BIT4;   							//���ù���ѡ��
    P2DIR|=BIT4;   							//���
    P2DS |=BIT4; 							//ȫ��������
    P2OUT&=~BIT4;   						//��ʼ������͵�ƽ
	
	
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

void delay()          //�˺���������������ʱ
{
	
  unsigned long int k;
  for(k=9999000;k>=1;k--);
  
}

void int_SMCLK()         					/* �˺�����������SMCLK����ο�ԴΪ12MHz */
{
	
	P2DIR |= BIT2;                          // SMCLK set out to pins
    P2SEL |= BIT2;
    UCSCTL3 |= SELREF_2;                    // Set DCO FLL reference = REFO
    UCSCTL4 |= SELA_2;                      // Set ACLK = REFO
    __bis_SR_register(SCG0);                // Disable the FLL control loop
    UCSCTL0 = 0x0000;                       // Set lowest possible DCOx, MODx
    UCSCTL1 = DCORSEL_5;                    // Select DCO range 24MHz operation
    UCSCTL2 = FLLD_1 + 374;                 // Set DCO Multiplier for 12MHz
                                            // (N + 1) * FLLRef = Fdco
                                            // (374 + 1) * 32768 = 12MHz
                                            // Set FLL Div = fDCOCLK/2
    __bic_SR_register(SCG0);                // Enable the FLL control loop
    __delay_cycles(375000);
	
}

void int_pwm()                              //�˺�������pwm��ռ�ձ�
{
	
	TA2CCTL0=CCIE;     						//��ʱ��A1�Ĳ���Ƚ���0����CCIFGλ�ж�
    TA2CCR0=12288;       					//���������ֵ����PWMƵ��Ϊ500HZ
    TA2CCTL1=CCIE;     						//����Ƚ���1�����ж�
    TA2CCR1=pwm_arry[i];        			//ռ�ձ�Ϊ%
    TA2CTL|=TACLR;     						//����ʱ��A1����
    TA2CTL|=TASSEL_2+MC_3;   				//��ʱ��ѡ��SMCLK��Ϊʱ��Դ����Ϊ����ģʽ
    TA2CCTL1=OUTMOD_4; 						//��ʱ��A1�еĲ���Ƚ���1�����ģʽΪ4��ת
	
	TBCCTL0=CCIE;     						//��ʱ��B�Ĳ���Ƚ���0����CCIFGλ�ж�
    TBCCR0=12288;       					//���������ֵ����PWMƵ��Ϊ500HZ
	TBCCTL2=CCIE;     						//����Ƚ���2�����ж�
	TBCCTL5=CCIE;     						//����Ƚ���5�����ж�
	TBCCTL6=CCIE;     						//����Ƚ���6�����ж�
	
	TBCCR2=pwm_arry[i];        				//
	TBCCR5=pwm_arry[i];        				//
	TBCCR6=pwm_arry[i];        				//
    TBCTL|=TBCLR;     						//����ʱ��b����
    TBCTL|=TBSSEL_2+MC_3;   				//��ʱ��ѡ��SMCLK��Ϊʱ��Դ����Ϊ����ģʽ

	TBCCTL2=OUTMOD_4; 						//��ʱ��B�еĲ���Ƚ���2�����ģʽΪ4��ת
	TBCCTL5=OUTMOD_4; 						//��ʱ��B�еĲ���Ƚ���5�����ģʽΪ4��ת
	TBCCTL6=OUTMOD_4; 						//��ʱ��B�еĲ���Ƚ���6�����ģʽΪ4��ת
	
}


void int_buttons()                          //�˺�������GPIO
{
	
	P1DIR &=~BIT3;							// 
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

	P1DIR &=~BIT4;							// 
	P1IFG &=~BIT4;
	P1IE  |=BIT4;
	P1IES |=BIT4;
	P1OUT |=BIT4;
	P1REN |=BIT4;

	P2DIR &=~BIT1;							// 
	P2IFG &=~BIT1;
	P2IE  |=BIT1;
	P2IES |=BIT1;
	P2OUT |=BIT1;
	P2REN |=BIT1;
	
}

void int_throttle_or_zero_throttle()     //�˺�����������������У׼�г̻����ж��������
{
	
	i = 0;
	int_pwm();
	delay();
	
}
void takeoff()      //��ɺ���
{
	
	i=2;
    int_pwm();
	delay();
	
}

#pragma vector=PORT1_VECTOR
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
		
		if((P1IN&BIT2)==0)
		{
			takeoff();
			P1IFG &=~BIT2;
		}
		
		if((P1IN&BIT2)!=0)
		{
			int_throttle_or_zero_throttle();
			P1IFG &=~BIT2;
		}
		
	}
}

void main(void)
{
	WDTCTL=WDTPW+WDTHOLD;
	int_motor_gpio();
	int_SMCLK();
    int_buttons();
    _EINT();
    while(1);
	
}