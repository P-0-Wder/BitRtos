#ifndef __IIC_H
#define __IIC_H

#include "stm32f4xx.h"

typedef enum
{
	IIC_1,
	IIC_2,
} IIC_List;

#if IIC_ENABLE

#define ACK_OK 1
#define ACK_ERR 0
#define IIC_DelayVaule 4

#define IIC_SCL(a)                               \
	if (a)                                       \
		GPIO_SetBits(IIC_SCL_PORT, IIC_SCL_PIN); \
	else                                         \
		GPIO_ResetBits(IIC_SCL_PORT, IIC_SCL_PIN)

#define IIC_SDA(a)                               \
	if (a)                                       \
		GPIO_SetBits(IIC_SDA_PORT, IIC_SDA_PIN); \
	else                                         \
		GPIO_ResetBits(IIC_SDA_PORT, IIC_SDA_PIN)

void IIC_GPIO_Config(void);
void IIC_SDA_Out(void);
void IIC_SDA_In(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_ACK(void);
void IIC_NoACK(void);
u8 IIC_WaitACK(void);
u8 IIC_WriteByte(u8 IIC_Data);
u8 IIC_ReadByte(u8 ACK_Mode);
#endif

#endif //__IIC_H
