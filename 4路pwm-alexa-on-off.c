#include <msp430f5529.h>
unsigned int pwm_arry[4] = {6144,5000,3000,20};
unsigned int i;
void int_motor_gpio() //电机4路pwm的GPIO初始化设置
{
	
	P2SEL|=BIT4;   							//复用功能选择
    P2DIR|=BIT4;   							//输出
    P2DS |=BIT4; 							//全电流驱动
    P2OUT&=~BIT4;   						//初始化输出低电平
	
	
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

void delay()          //此函数设置启动的延时
{
	
  unsigned long int k;
  for(k=9999000;k>=1;k--);
  
}

void int_SMCLK()         					/* 此函数用于设置SMCLK晶振参考源为12MHz */
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

void int_pwm()                              //此函数设置pwm的占空比
{
	
	TA2CCTL0=CCIE;     						//定时器A1的捕获比较器0开启CCIFG位中断
    TA2CCR0=12288;       					//置入计数终值，则PWM频率为500HZ
    TA2CCTL1=CCIE;     						//捕获比较器1开启中断
    TA2CCR1=pwm_arry[i];        			//占空比为%
    TA2CTL|=TACLR;     						//将计时器A1清零
    TA2CTL|=TASSEL_2+MC_3;   				//定时器选择SMCLK作为时钟源，且为增减模式
    TA2CCTL1=OUTMOD_4; 						//定时器A1中的捕获比较器1，输出模式为4翻转
	
	TBCCTL0=CCIE;     						//定时器B的捕获比较器0开启CCIFG位中断
    TBCCR0=12288;       					//置入计数终值，则PWM频率为500HZ
	TBCCTL2=CCIE;     						//捕获比较器2开启中断
	TBCCTL5=CCIE;     						//捕获比较器5开启中断
	TBCCTL6=CCIE;     						//捕获比较器6开启中断
	
	TBCCR2=pwm_arry[i];        				//
	TBCCR5=pwm_arry[i];        				//
	TBCCR6=pwm_arry[i];        				//
    TBCTL|=TBCLR;     						//将计时器b清零
    TBCTL|=TBSSEL_2+MC_3;   				//定时器选择SMCLK作为时钟源，且为增减模式

	TBCCTL2=OUTMOD_4; 						//定时器B中的捕获比较器2，输出模式为4翻转
	TBCCTL5=OUTMOD_4; 						//定时器B中的捕获比较器5，输出模式为4翻转
	TBCCTL6=OUTMOD_4; 						//定时器B中的捕获比较器6，输出模式为4翻转
	
}


void int_buttons()                          //此函数设置GPIO
{
	
	P1DIR &=~BIT3;							// 
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

void int_throttle_or_zero_throttle()     //此函数用于设置零油门校准行程或者切断油门输出
{
	
	i = 0;
	int_pwm();
	delay();
	
}
void takeoff()      //起飞函数
{
	
	i=2;
    int_pwm();
	delay();
	
}

#pragma vector=PORT1_VECTOR
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