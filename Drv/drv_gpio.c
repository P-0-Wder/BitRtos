#include "drv_gpio.h"
#include "periph_gpio.h"

/* internal function define */

/* external function define */
static bool DrvGPIO_Open(DrvGPIO_Obj_TypeDef *Obj, DrvGPIO_CTL_TypeDef type);
static bool DrvGPIO_Close(DrvGPIO_Obj_TypeDef *Obj);
static bool DrvGPIO_Control(DrvGPIO_Obj_TypeDef *Obj, DrvGPIO_IO_Level_TypeDef level);
static DrvGPIO_IO_Level_TypeDef DrvGPIO_Get(DrvGPIO_Obj_TypeDef *Obj);

DrvGPIO_GenProcFunc_TypeDef GenGPIO_Drv = {
    .open = DrvGPIO_Open,
    .close = DrvGPIO_Close,
    .set = DrvGPIO_Control,
    .get = DrvGPIO_Get,
};

static bool DrvGPIO_Open(DrvGPIO_Obj_TypeDef *Obj, DrvGPIO_CTL_TypeDef type)
{
    Obj->IO_Type = type;

    switch ((uint8_t)type)
    {
    case GPIO_Output:
        GPIO_IO_Output_Init(Obj->CLK, Obj->Pin, Obj->Port);
        break;

    case GPIO_Input:
        GPIO_IO_Input_Init(Obj->CLK, Obj->Pin, Obj->Port);
        break;

    default:
        break;
    }
}

static bool DrvGPIO_Close(DrvGPIO_Obj_TypeDef *Obj)
{
}

static bool DrvGPIO_Control(DrvGPIO_Obj_TypeDef *Obj, DrvGPIO_IO_Level_TypeDef level)
{
    if (Obj->IO_Type != GPIO_Output)
        return false;

    if (level == GPIO_LOW)
    {
        GPIO_Set_IO_LEVEL(Obj->Port, Obj->Pin, LO);
    }
    else
        GPIO_Set_IO_LEVEL(Obj->Port, Obj->Pin, HI);
}

static DrvGPIO_IO_Level_TypeDef DrvGPIO_Get(DrvGPIO_Obj_TypeDef *Obj)
{
    if (Obj->IO_Type != GPIO_Input)
        return GPIO_ERR;

    if (GPIO_Get_IO_LEVEL(Obj->Port, Obj->Pin))
    {
        return GPIO_HIGH;
    }
    else
        return GPIO_LOW;
}
