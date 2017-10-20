/*  I2Cͨ��Э�飬����������á�
 *  ����:P3.0 --> SDA		P3.1 -->SCL
 *
 *  ע��:1.�������Ƶ������Ӧ��ʱ���������������衣
 * 		2.UCB0I2CSA = 0x68;��40�У� 0x68ΪMPU6050�ĵ�ַ���üĴ���������Ǵӻ��ĵ�ַ
 * 		���ѯ��Ӧ�������ֲ�����޸ļ��ɡ�
 * 		3.UCB0BR0 = 40;UCB0BR1 = 0;��38��39�У� ��������Ʒ�Ƶ���������Ƶ��֤I2C
 * 		��ͨ��Ƶ��Ϊ100KHZ������Ϊ4MHZ,����40��Ƶ����Ƶ����=UCB0BR0+UCB0BR1��256��
 */
#include <MSP430f5529.H>
#include "I2C_F5529.h"

//��������ɾ��
#include "MPU6050.h"

#define CPU_F ((double)4000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

#define uchar unsigned char
#define uint  unsigned int

/***********************************************
***�������ƣ�void I2C__Init(void)
***���� ��   ����IIC����
���̣�
     1.��λ UCSWRST
     2.����USCI�Ĵ���
     3.���ö˿�
     4.��λ UCSWRST
     5. ʹ�ܽ��ջ��߷����ж�
************************************************/
void I2C_Init(void)
{
	//P3DIR������
	P3SEL |= (BIT0+BIT1);                     // Assign I2C pins to USCI_B0,P3.0SDA,P3.1SCL
	UCB0CTL1 |= UCSWRST;                      // Enable SW reset,��λʱӦ�ö�USCI��������
	UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode,Ĭ��7λ��ַ
	UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
	UCB0BR0 = 63;                             // fSCL = SMCLK/250 = ~100k bps
	UCB0BR1 = 0;
	UCB0I2CSA = 0x68;                         // Slave Address is 0x68
	UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation��
  //IE2 |= UCB0RXIE+UCB0TXIE;               // Enable RX interrupt
}

//****************д��һ���ֽ�***********************

void Single_Write(uchar REG_Address,uchar REG_data)
{
    I2C_Init();                 		 //��ʼ����д��������ַ
	while(UCB0CTL1 & UCTXSTP);           //ȷ�����߿���

	UCB0CTL1 |= UCTR;                	 //����ģʽ
    UCB0CTL1 |= UCTXSTT;             	 //����һ����ʼ�ź�
	while((UCB0CTL1 & UCTXSTT) == 1);    //ȷ����ʼ�ź�
 	while((UCB0IFG & UCTXIFG) == 0);  	 //�ȴ����ͽ���

	UCB0TXBUF = REG_Address;             //д��Ĵ�����ַ
	while((UCB0IFG & UCTXIFG) == 0);  	 //�ȴ����ͽ���

	UCB0TXBUF = REG_data;                //д������
    while((UCB0IFG & UCTXIFG) == 0);	 //�ȴ�д�����
	//IFG2 &= ~UCB0TXIFG;                //�����־λ
	//while(UCB0STAT&UCBBUSY);

    UCB0CTL1 |= UCTXSTP + UCTXNACK;   	 //ֹͣ�źţ�����Ӧ��
    while((UCB0CTL1 & UCTXSTP) == 1);	 //�ȴ�����������ֹͣ�������Զ��������
	delay_us(200);
}

//***************����һ���ֽ�****************************

unsigned char Single_Read(uchar REG_Address)
{

	unsigned char RX_data;

	I2C_Init();                        //��ʼ����д��������ַ
    while(UCB0CTL1 & UCTXSTP);         //ȷ�����߿���

    UCB0CTL1 |= UCTXSTT + UCTR;        //����һ����ʼ�ź�  1
    while((UCB0CTL1 & UCTXSTT)==1);    //ȷ����ʼ�ź�
    while((UCB0IFG & UCTXIFG) == 0);    //�ȴ����ͽ���
    UCB0IFG &= ~UCTXIFG;

    UCB0TXBUF = REG_Address;           //д��Ĵ�����ַ
    while((UCB0IFG & UCTXIFG) == 0);    //�ȴ����ͽ���
    UCB0IFG &= ~UCTXIFG;                //�����־λ

	UCB0CTL1 &= ~UCTR;                 //����ģʽ
	while(UCB0CTL1 & UCTXSTP);         //ȷ�����߿���

	UCB0CTL1 |= UCTXSTT;               //������ʼ�ź�         2
	while((UCB0CTL1 & UCTXSTT)==1);    //ȷ����ʼ�ź�

	while((UCB0IFG & UCRXIFG)==0);
 	RX_data = UCB0RXBUF;               //������

 	while((UCB0IFG & UCRXIFG)==0);

 	UCB0CTL1 |= UCTXSTP + UCTXNACK;    //ֹͣ�źţ�����Ӧ��
 	while((UCB0CTL1 & UCTXSTP)==1);
 	delay_us(200);

 	return  RX_data;
}


