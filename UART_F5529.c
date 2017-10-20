/*
 * 	UART_F5529.c
 *
 *	接线方式:TX-->P3.4(RX)  RX-->P3.3(TX)
 *
 * 	注意:默认使用外部高频振荡器 XT2 = 4MHZ  波特率 9600
 *		若要修改波特率，直接更改第 37行的数值即可
 *		使用时在main()中先调用 UART0_INIT(void); 初始化串口
 *
 *	功能:(1)通过串口向电脑发送数据，类型包括字符型、整形、浮点型、字符串。
 *  	使用时只需分类调用头文件中的函数即可，其中浮点型可以选择精度即小数点后位数。
 *
 *  	(2)可由电脑发送数据至单片机，并显示在串口助手中。响应方式为中断响应，
 *  	具体功能可根据需求进行修改usart0_rx(void)函数中内容。
 *
 *	用法:	UART_Sendchar('a');
 *		UART_Sendint(123);
 *		UART_Sendcharstring("abc");
 *  	UART_Senddouble(3.1415,2);  //精度为2
 *
 *
 *  	Created on: 2015-6-13
 *      Author: XAUT_自动化科协		CZH
 */
#include "msp430f5229.h"

void UART_Init(void)
{
	P3SEL |= BIT3 + BIT4;	//第二功能定义

	// 在 UCSWRST=1的情况下设置串口，然后再设置 UCSWRST=0
	UCA0CTL1 |= UCSWRST;	//USCI保持复位
	UCA0CTL0 = UCMODE_0;	//UART模式
	UCA0CTL0 &= ~UC7BIT;	//数据位:8

	UCA0CTL1 = UCSSEL__SMCLK + UCRXEIE;	//USCI时钟源使用系统子时钟 + 接收错误字符使能中断
	UCA0BRW = 25000000 / 115200;	//默认波特率   25M /115200

	UCA0CTL1 &= ~UCSWRST;	//关闭软件复位
	UCA0IE |= UCRXIE;	//使能USCI_A0的UART接收中断
}

void UART_Sendchar(unsigned char Data)
{
    while(!(UCA0IFG & UCTXIFG));
	UCA0TXBUF = Data;
}

void UART_Sendint(long int Data)
{
	if(Data < 0)
	{
		Data = -Data;
		UART_Sendchar('-');
	}
	if(Data < 10)
	{
		UART_Sendchar(Data + '0');
	}
	else if((10 <= Data) && (Data < 100))
	{
		UART_Sendchar(Data / 10 + '0');
		UART_Sendchar(Data % 10 + '0');
	}
	else if((100 <= Data)&&(Data < 1000))
	{
		UART_Sendchar(Data / 100 + '0');
		UART_Sendchar(Data / 10 % 10 + '0');
		UART_Sendchar(Data % 10 + '0');
	}
	else if((1000 <= Data) && (Data < 10000))
	{
		UART_Sendchar(Data / 1000 + '0');
		UART_Sendchar(Data / 100 % 10 + '0');
		UART_Sendchar(Data / 10 % 10 + '0');
		UART_Sendchar(Data % 10 + '0');
	}
	else if((10000 <= Data) && (Data < 100000))
	{
		UART_Sendchar(Data / 10000 + '0');
		UART_Sendchar(Data / 1000 % 10 + '0');
		UART_Sendchar(Data / 100 % 10 + '0');
		UART_Sendchar(Data / 10 % 10 + '0');
		UART_Sendchar(Data % 10 + '0');
	}
}

void UART_Sendcharstring(unsigned char *str)
{
	char i = 0;
	while(str[i] != '\0')
	{
		UART_Sendchar(str[i++]);
	}
}

//浮点转字符串
char *F2S(double d,int precision)
{
	d += 0.0001;  //消除浮点型的精度误差
	char str[10];
	char str1[40];
	int j=0,k,i;
	i = (int)d;  //浮点数的整数部分
	while(i>0)
	{
		str1[j++] = i%10+'0';
		i /= 10;
	}
	for(k=0;k<j;k++)
		str[k] = str1[j-1-k]; //

	str[j++] = '.';

	d -= (int)d;
	for(i=0;i< precision;i++)
	{
		d *= 10;
		str[j++] = (int)d + '0';
		d -= (int)d;
	}
	str[j] = '\0';
	return str;
}

void UART_Senddouble(double Data_d,int precision)
{
	if(Data_d < 0)
	{
		Data_d = -Data_d;
		UART_Sendchar('-');
	}
	UART_Sendcharstring(F2S(Data_d,precision));
}

//回声测试
#pragma vector = USCI_A0_VECTOR
__interrupt void usart_rx(void)  //串口接收中断
{
	//当UCA0TXBUF为空时,UCTXIFG置位。
	while(!(UCA0IFG & UCTXIFG));
	UCA0TXBUF = UCA0RXBUF;
}

