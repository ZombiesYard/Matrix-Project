/*  I2C通信协议，根据需求调用。
 *  接线:P3.0 --> SDA		P3.1 -->SCL
 *
 *  注意:1.请根据主频设置相应的时钟项，无需加上拉电阻。
 * 		2.UCB0I2CSA = 0x68;（40行） 0x68为MPU6050的地址，该寄存器保存的是从机的地址
 * 		需查询相应器件的手册对其修改即可。
 * 		3.UCB0BR0 = 40;UCB0BR1 = 0;（38，39行） 这两句控制分频，需根据主频保证I2C
 * 		的通信频率为100KHZ。上例为4MHZ,进行40分频，分频倍数=UCB0BR0+UCB0BR1×256。
 */
#include <MSP430f5529.H>
#include "I2C_F5529.h"

//根据需求删减
#include "MPU6050.h"

#define CPU_F ((double)4000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

#define uchar unsigned char
#define uint  unsigned int

/***********************************************
***函数名称：void I2C__Init(void)
***功能 ；   配置IIC总线
流程：
     1.置位 UCSWRST
     2.设置USCI寄存器
     3.设置端口
     4.复位 UCSWRST
     5. 使能接收或者发送中断
************************************************/
void I2C_Init(void)
{
	//P3DIR不设置
	P3SEL |= (BIT0+BIT1);                     // Assign I2C pins to USCI_B0,P3.0SDA,P3.1SCL
	UCB0CTL1 |= UCSWRST;                      // Enable SW reset,复位时应该对USCI进行设置
	UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode,默认7位地址
	UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
	UCB0BR0 = 63;                             // fSCL = SMCLK/250 = ~100k bps
	UCB0BR1 = 0;
	UCB0I2CSA = 0x68;                         // Slave Address is 0x68
	UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation，
  //IE2 |= UCB0RXIE+UCB0TXIE;               // Enable RX interrupt
}

//****************写入一个字节***********************

void Single_Write(uchar REG_Address,uchar REG_data)
{
    I2C_Init();                 		 //初始化，写入器件地址
	while(UCB0CTL1 & UCTXSTP);           //确定总线空闲

	UCB0CTL1 |= UCTR;                	 //发送模式
    UCB0CTL1 |= UCTXSTT;             	 //产生一个起始信号
	while((UCB0CTL1 & UCTXSTT) == 1);    //确认起始信号
 	while((UCB0IFG & UCTXIFG) == 0);  	 //等待发送结束

	UCB0TXBUF = REG_Address;             //写入寄存器地址
	while((UCB0IFG & UCTXIFG) == 0);  	 //等待发送结束

	UCB0TXBUF = REG_data;                //写入数据
    while((UCB0IFG & UCTXIFG) == 0);	 //等待写入结束
	//IFG2 &= ~UCB0TXIFG;                //清楚标志位
	//while(UCB0STAT&UCBBUSY);

    UCB0CTL1 |= UCTXSTP + UCTXNACK;   	 //停止信号，产生应答
    while((UCB0CTL1 & UCTXSTP) == 1);	 //等待结束（产生停止条件后，自动清除！）
	delay_us(200);
}

//***************读出一个字节****************************

unsigned char Single_Read(uchar REG_Address)
{

	unsigned char RX_data;

	I2C_Init();                        //初始化，写入器件地址
    while(UCB0CTL1 & UCTXSTP);         //确定总线空闲

    UCB0CTL1 |= UCTXSTT + UCTR;        //产生一个起始信号  1
    while((UCB0CTL1 & UCTXSTT)==1);    //确认起始信号
    while((UCB0IFG & UCTXIFG) == 0);    //等待发送结束
    UCB0IFG &= ~UCTXIFG;

    UCB0TXBUF = REG_Address;           //写入寄存器地址
    while((UCB0IFG & UCTXIFG) == 0);    //等待发送结束
    UCB0IFG &= ~UCTXIFG;                //清除标志位

	UCB0CTL1 &= ~UCTR;                 //接收模式
	while(UCB0CTL1 & UCTXSTP);         //确认总线空闲

	UCB0CTL1 |= UCTXSTT;               //发送起始信号         2
	while((UCB0CTL1 & UCTXSTT)==1);    //确认起始信号

	while((UCB0IFG & UCRXIFG)==0);
 	RX_data = UCB0RXBUF;               //读数据

 	while((UCB0IFG & UCRXIFG)==0);

 	UCB0CTL1 |= UCTXSTP + UCTXNACK;    //停止信号，产生应答
 	while((UCB0CTL1 & UCTXSTP)==1);
 	delay_us(200);

 	return  RX_data;
}


