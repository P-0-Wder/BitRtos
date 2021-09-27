#ifndef __INPUT_IO_DEF_H
#define __INPUT_IO_DEF_H

#include <stdint.h>

#define SetBit(x) 1 << x

typedef enum
{
    GPIO_Output = 0,
    GPIO_Input,
    GPIO_EXTI_Input,
} DrvGPIO_CTL_TypeDef;

typedef struct
{
    uint32_t CLK;
    uint16_t Pin;
    void *Port;
    DrvGPIO_CTL_TypeDef IO_Type;
} DrvGPIO_Obj_TypeDef;

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
extern DrvGPIO_Obj_TypeDef Power_Btn;
//extern DrvGPIO_Obj_TypeDef Sync_Out;
//extern DrvGPIO_Obj_TypeDef Sync_In;

#endif
