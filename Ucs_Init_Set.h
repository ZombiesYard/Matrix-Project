/*
 * Ucs_Init_Set.h
 *
 *  Created on: 2015��7��17��
 *      Author: pfx
 */
#ifndef UCS_INIT_SET_H_
#define UCS_INIT_SET_H_
//***********���ܣ�ACLK = REFO = 32kHz, MCLK = SMCLK = 25MHz*******//
extern void Ucs_25Mhz(void); //��������һ���ļ��е��ô˺���
//***********���ܣ�ACLK = REFO = 32kHz, MCLK = SMCLK = 20MHz*******//
void Ucs_20Mhz(void);//��������һ���ļ��е��ô˺���
//***********���ܣ�ACLK = REFO = 32kHz, MCLK = SMCLK = 8MHz*******//
extern void Ucs_8Mhz(void); //��������һ���ļ��е��ô˺���
//***********���ܣ�ACLK = REFO = 32kHz, MCLK = SMCLK DCOCLKDIV=32768Hz*******//
//***********���ܣ�ACLK = REFO = 32kHz, MCLK = SMCLK = 12MHz*******//
extern void Ucs_12Mhz(void); //��������һ���ļ��е��ô˺���
//***********���ܣ�ACLK = REFO = 32kHz, MCLK = SMCLK = 4MHz*******//
extern void Sysclk_Init_4Mhz_XT2(void);////��������һ���ļ��е��ô˺���
//**********���ܣ�����Ӧ�ܽ����ʱ���ź�********//
extern void Sysclk_Signal_Outpt(void);

#endif /* UCS_INIT_SET_H_ */
