#include "Input_IO_Def.h"
#include "periph_gpio.h"

/*DrvGPIO_Obj_TypeDef Toggle_Left = {
    .CLK =,
    .IO_Type =,
    .Pin =,
    .Port =,
};

DrvGPIO_Obj_TypeDef Toggle_Right = {
    .CLK =,
    .IO_Type =,
    .Pin =,
    .Port =,
};

DrvGPIO_Obj_TypeDef DirBtn_Up = {
    .CLK =,
    .IO_Type =,
    .Pin =,
    .Port =,
};

DrvGPIO_Obj_TypeDef DirBtn_Dwn = {
    .CLK =,
    .IO_Type =,
    .Pin =,
    .Port =,
};

DrvGPIO_Obj_TypeDef DirBtn_Lft = {
    .CLK =,
    .IO_Type =,
    .Pin =,
    .Port =,
};

DrvGPIO_Obj_TypeDef DirBtn_Rgt = {
    .CLK =,
    .IO_Type =,
    .Pin =,
    .Port =,
};

DrvGPIO_Obj_TypeDef DirBtn_Mid = {
    .CLK =,
    .IO_Type =,
    .Pin =,
    .Port =,
};

DrvGPIO_Obj_TypeDef ShoulderBtn_Lft = {
    .CLK =,
    .IO_Type =,
    .Pin =,
    .Port =,
};

DrvGPIO_Obj_TypeDef ShoulderBtn_Rgt = {
    .CLK =,
    .IO_Type =,
    .Pin =,
    .Port =,
};

DrvGPIO_Obj_TypeDef Power_Buttion = {
    .CLK =,
    .IO_Type =,
    .Pin =,
    .Port =,
};

DrvGPIO_Obj_TypeDef Sync_Out = {
    .CLK =,
    .IO_Type =,
    .Pin =,
    .Port =,
};

DrvGPIO_Obj_TypeDef Sync_In = {
    .CLK =,
    .IO_Type =,
    .Pin =,
    .Port =,
};

DrvGPIO_Obj_TypeDef Encoder_Btn = {
    .CLK =,
    .IO_Type =,
    .Pin =,
    .Port =,
};*/

DrvGPIO_Obj_TypeDef Encoder_A_Pin = {
    .CLK = RCC_AHB1Periph_GPIOA,
    .IO_Type = GPIO_Mode_AF,
    .Pin = GPIO_Pin_6,
    .Port = GPIOA,
};

DrvGPIO_Obj_TypeDef Encoder_B_Pin = {
    .CLK = RCC_AHB1Periph_GPIOA,
    .IO_Type = GPIO_Mode_AF,
    .Pin = GPIO_Pin_4,
    .Port = GPIOA,
};

DrvGPIO_Obj_TypeDef Power_Btn = {
    .CLK = RCC_AHB1Periph_GPIOA,
    .IO_Type = GPIO_Mode_IN,
    .Pin = GPIO_Pin_0,
    .Port = GPIOA,
};
