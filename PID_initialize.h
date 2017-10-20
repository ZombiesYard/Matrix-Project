/*
 * PID_initialize.h
 *
 *  Created on: 2017Äê10ÔÂ18ÈÕ
 *      Author: ZombiesYard
 */
#include "msp430f5529.h"
#ifndef PID_INITIALIZE_H_
#define PID_INITIALIZE_H_

extern void int_motor_gpio();
extern void delay();


//extern void int_pwm();
extern void int_buttons() ;
extern void CONTROL();
extern void int_throttle_or_zero_throttle();
extern void takeoff() ;
#define delay_us(x) __delay_cycles((long)(25001984*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(25001984*(double)x/1000.0))



#endif /* PID_INITIALIZE_H_ */
