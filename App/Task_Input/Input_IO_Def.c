#include "Input_IO_Def.h"
#include "drv_gpio.h"

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
    .CLK = encoder_io_clk,
    .Pin = encoder_io_a_pin,
    .Pin_Src = encoder_io_a_pinsource,
    .Port = encoder_port,
    .IO_Type = GPIO_Encoder,
    .AF_TIMx = GPIO_AF_TIM8,
};

DrvGPIO_Obj_TypeDef Encoder_B_Pin = {
    .CLK = encoder_io_clk,
    .Pin = encoder_io_b_pin,
    .Pin_Src = encoder_io_b_pinsource,
    .Port = encoder_port,
    .IO_Type = GPIO_Encoder,
    .AF_TIMx = GPIO_AF_TIM8,
};

/* for temp use exit */
DrvGPIO_Obj_TypeDef Encoder_Btn = {
    .CLK = encoder_key_clk,
    .Pin = encoder_key_pin,
    .Port = encoder_key_port,
    .IO_Type = GPIO_Input,
};

DrvGPIO_Obj_TypeDef Power_Btn = {
    .CLK = encoder_key_clk,
    .IO_Type = GPIO_Mode_IN,
    .Pin = encoder_key_pin,
    .Port = encoder_key_port,
};
