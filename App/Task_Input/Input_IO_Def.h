#ifndef __INPUT_IO_DEF_H
#define __INPUT_IO_DEF_H

#include <stdint.h>
#include "periph_gpio.h"

#define encoder_port GPIOC
#define encoder_io_clk RCC_AHB1Periph_GPIOC

#define encoder_io_a_pin GPIO_Pin_7
#define encoder_io_b_pin GPIO_Pin_6

#define encoder_io_a_pinsource GPIO_PinSource7
#define encoder_io_b_pinsource GPIO_PinSource6

#define encoder_key_port GPIOB
#define encoder_key_clk RCC_AHB1Periph_GPIOB
#define encoder_key_pin GPIO_Pin_2

//extern DrvGPIO_Obj_TypeDef Toggle_Left;
//extern DrvGPIO_Obj_TypeDef Toggle_Right;
//extern DrvGPIO_Obj_TypeDef DirBtn_Up;
//extern DrvGPIO_Obj_TypeDef DirBtn_Dwn;
//extern DrvGPIO_Obj_TypeDef DirBtn_Lft;
//extern DrvGPIO_Obj_TypeDef DirBtn_Rgt;
//extern DrvGPIO_Obj_TypeDef DirBtn_Mid;
//extern DrvGPIO_Obj_TypeDef ShoulderBtn_Lft;
//extern DrvGPIO_Obj_TypeDef ShoulderBtn_Rgt;
//extern DrvGPIO_Obj_TypeDef Sync_In_Pin;
//extern DrvGPIO_Obj_TypeDef Sync_Out_Pin;
extern DrvGPIO_Obj_TypeDef Encoder_Btn;
extern DrvGPIO_Obj_TypeDef Encoder_A_Pin;
extern DrvGPIO_Obj_TypeDef Encoder_B_Pin;
//extern DrvGPIO_Obj_TypeDef Power_Btn;
//extern DrvGPIO_Obj_TypeDef Sync_Out;
//extern DrvGPIO_Obj_TypeDef Sync_In;

#endif
