/*
 * I2C_F5529.h
 *
 *  Created on: 2015-7-17
 *      Author: Czh
 */
#ifndef I2C_F5529_H_
#define I2C_F5529_H_

extern void I2C_Init(void);  //IIC初始化
extern void Single_Write(unsigned char REG_Address,unsigned char REG_data);  //写一个字节
extern unsigned char Single_Read(unsigned char REG_Address);        //读一个字节

#endif /* I2C_F5529_H_ */
