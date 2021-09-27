#include "Dev_Button.h"
#include "drv_gpio.h"
#include <string.h>

/* internal variable */
static uint8_t button_num = 0;

/* internal function */
static bool DevButton_Init(Button_Obj_TypeDef *Obj, DrvGPIO_Obj_TypeDef *io);
static bool DevButton_Invert(Button_Obj_TypeDef *Obj);
static Button_State_List DevButton_Get(Button_Obj_TypeDef *Obj);
static uint8_t DevButton_Get_Sum(void);

/* external variable */
DevButton_TypeDef DrvButton = {
    .Open = DevButton_Init,
    .Invert = DevButton_Invert,
    .Get = DevButton_Get,
    .get_button_sum = DevButton_Get_Sum,
};

static uint8_t DevButton_Get_Sum(void)
{
    return button_num;
}

static bool DevButton_Init(Button_Obj_TypeDef *Obj, DrvGPIO_Obj_TypeDef *io)
{
    if ((io == NULL) || (Obj == NULL))
    {
        Obj->init_state = false;
        return false;
    }

    Obj->io = io;
    Obj->invert = false;

    DrvGPIO.open(Obj->io, GPIO_Input, NULL);

    button_num++;

    Obj->init_state = true;
    return true;
}

static bool DevButton_Invert(Button_Obj_TypeDef *Obj)
{
    if ((Obj == NULL) || (!Obj->init_state))
        return false;

    Obj->invert = !Obj->invert;

    return true;
}

static Button_State_List DevButton_Get(Button_Obj_TypeDef *Obj)
{
    bool btn_state = false;

    if ((Obj == NULL) || (!Obj->init_state))
        return Btn_Err;

    if (DrvGPIO.get(Obj))
    {
        if (Obj->invert)
        {
            return Btn_Dwn;
        }
        else
            return Btn_Up;
    }
    else
    {
        if (Obj->invert)
        {
            return Btn_Up;
        }
        else
            return Btn_Dwn;
    }
}
