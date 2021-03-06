#include "drv_gpio.h"
#include "periph_gpio.h"
#include "periph_exti.h"

/* internal function define */

/* external function define */
static bool DrvGPIO_Open(DrvGPIO_Obj_TypeDef *Obj, DrvGPIO_CTL_TypeDef type, uint32_t data);
static bool DrvGPIO_Close(DrvGPIO_Obj_TypeDef *Obj);
static bool DrvGPIO_Control(DrvGPIO_Obj_TypeDef *Obj, DrvGPIO_IO_Level_TypeDef level);
static DrvGPIO_IO_Level_TypeDef DrvGPIO_Get(DrvGPIO_Obj_TypeDef *Obj);

DrvGPIO_GenProcFunc_TypeDef DrvGPIO = {
    .open = DrvGPIO_Open,
    .close = DrvGPIO_Close,
    .set = DrvGPIO_Control,
    .get = DrvGPIO_Get,
};

static bool DrvGPIO_Open(DrvGPIO_Obj_TypeDef *Obj, DrvGPIO_CTL_TypeDef type, uint32_t data)
{
    Obj->IO_Type = type;

    switch ((uint8_t)type)
    {
    case GPIO_Output:
        GPIO_IO_Output_Init(Obj->CLK, Obj->Pin, Obj->Port);
        return true;

    case GPIO_Input:
        GPIO_IO_Input_Init(Obj->CLK, Obj->Pin, Obj->Port);
        return true;

    case GPIO_Encoder:
        GPIO_IO_Encoder_Init(Obj->Port, Obj->CLK, Obj->Pin, Obj->Pin_Src, Obj->AF_TIMx);
        return true;

    case GPIO_EXTI_Input:
        GPIO_IO_Input_Init(Obj->CLK, Obj->Pin, Obj->Port);

        /* use for sync signal input */
        periph_exit_init(Obj->Exti_Cfg);

        /* set callback function */
        return periph_exti_SetCallback(Obj->Exti_Cfg.exti_line, (exti_callback)data);

    default:
        return false;
    }
}

static bool DrvGPIO_Close(DrvGPIO_Obj_TypeDef *Obj)
{
    /* ToDo */
    return true;
}

static bool DrvGPIO_Control(DrvGPIO_Obj_TypeDef *Obj, DrvGPIO_IO_Level_TypeDef level)
{
    if (level == GPIO_LOW)
    {
        GPIO_Set_IO_LEVEL(Obj->Port, Obj->Pin, LO);
    }
    else
        GPIO_Set_IO_LEVEL(Obj->Port, Obj->Pin, HI);

    return true;
}

static DrvGPIO_IO_Level_TypeDef DrvGPIO_Get(DrvGPIO_Obj_TypeDef *Obj)
{
    if (GPIO_Get_IO_LEVEL(Obj->Port, Obj->Pin))
    {
        return GPIO_HIGH;
    }
    else
        return GPIO_LOW;
}
