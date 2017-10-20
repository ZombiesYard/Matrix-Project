/*
 * Ucs_Init_Set.h
 *
 *  Created on: 2015年7月17日
 *      Author: pfx
 */
#ifndef UCS_INIT_SET_H_
#define UCS_INIT_SET_H_
//***********功能：ACLK = REFO = 32kHz, MCLK = SMCLK = 25MHz*******//
extern void Ucs_25Mhz(void); //可在另外一个文件中调用此函数
//***********功能：ACLK = REFO = 32kHz, MCLK = SMCLK = 20MHz*******//
void Ucs_20Mhz(void);//可在另外一个文件中调用此函数
//***********功能：ACLK = REFO = 32kHz, MCLK = SMCLK = 8MHz*******//
extern void Ucs_8Mhz(void); //可在另外一个文件中调用此函数
//***********功能：ACLK = REFO = 32kHz, MCLK = SMCLK DCOCLKDIV=32768Hz*******//
//***********功能：ACLK = REFO = 32kHz, MCLK = SMCLK = 12MHz*******//
extern void Ucs_12Mhz(void); //可在另外一个文件中调用此函数
//***********功能：ACLK = REFO = 32kHz, MCLK = SMCLK = 4MHz*******//
extern void Sysclk_Init_4Mhz_XT2(void);////可在另外一个文件中调用此函数
//**********功能：在相应管脚输出时钟信号********//
extern void Sysclk_Signal_Outpt(void);

#endif /* UCS_INIT_SET_H_ */
