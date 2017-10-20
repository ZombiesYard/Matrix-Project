/*
 * UART.h
 */
#ifndef _UART_H
#define _UART_H
#include "msp430f5529.h"

void UART_Init(void);
void UART_Sendchar(unsigned char Data);
void UART_Sendint(long int Data);
void UART_Sendcharstring(unsigned char *str);
void UART_Senddouble(double Data_d,int precision);
char *F2S(double d,int precision);    //浮点数，精度(小数位数)


#endif
