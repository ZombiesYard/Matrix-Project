/*
 * 	UART_F5529.c
 *
 *	���߷�ʽ:TX-->P3.4(RX)  RX-->P3.3(TX)
 *
 * 	ע��:Ĭ��ʹ���ⲿ��Ƶ���� XT2 = 4MHZ  ������ 9600
 *		��Ҫ�޸Ĳ����ʣ�ֱ�Ӹ��ĵ� 37�е���ֵ����
 *		ʹ��ʱ��main()���ȵ��� UART0_INIT(void); ��ʼ������
 *
 *	����:(1)ͨ����������Է������ݣ����Ͱ����ַ��͡����Ρ������͡��ַ�����
 *  	ʹ��ʱֻ��������ͷ�ļ��еĺ������ɣ����и����Ϳ���ѡ�񾫶ȼ�С�����λ����
 *
 *  	(2)���ɵ��Է�����������Ƭ��������ʾ�ڴ��������С���Ӧ��ʽΪ�ж���Ӧ��
 *  	���幦�ܿɸ�����������޸�usart0_rx(void)���������ݡ�
 *
 *	�÷�:	UART_Sendchar('a');
 *		UART_Sendint(123);
 *		UART_Sendcharstring("abc");
 *  	UART_Senddouble(3.1415,2);  //����Ϊ2
 *
 *
 *  	Created on: 2015-6-13
 *      Author: XAUT_�Զ�����Э		CZH
 */
#include "msp430f5229.h"

void UART_Init(void)
{
	P3SEL |= BIT3 + BIT4;	//�ڶ����ܶ���

	// �� UCSWRST=1����������ô��ڣ�Ȼ�������� UCSWRST=0
	UCA0CTL1 |= UCSWRST;	//USCI���ָ�λ
	UCA0CTL0 = UCMODE_0;	//UARTģʽ
	UCA0CTL0 &= ~UC7BIT;	//����λ:8

	UCA0CTL1 = UCSSEL__SMCLK + UCRXEIE;	//USCIʱ��Դʹ��ϵͳ��ʱ�� + ���մ����ַ�ʹ���ж�
	UCA0BRW = 25000000 / 115200;	//Ĭ�ϲ�����   25M /115200

	UCA0CTL1 &= ~UCSWRST;	//�ر������λ
	UCA0IE |= UCRXIE;	//ʹ��USCI_A0��UART�����ж�
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

//����ת�ַ���
char *F2S(double d,int precision)
{
	d += 0.0001;  //���������͵ľ������
	char str[10];
	char str1[40];
	int j=0,k,i;
	i = (int)d;  //����������������
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

//��������
#pragma vector = USCI_A0_VECTOR
__interrupt void usart_rx(void)  //���ڽ����ж�
{
	//��UCA0TXBUFΪ��ʱ,UCTXIFG��λ��
	while(!(UCA0IFG & UCTXIFG));
	UCA0TXBUF = UCA0RXBUF;
}

