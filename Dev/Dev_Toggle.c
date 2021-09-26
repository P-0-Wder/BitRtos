#include "Dev_Toggle.h"
#include "drv_gpio.h"
#include <string.h>

/* internal variable */
static uint8_t toggle_2level_sum = 0;
static uint8_t toggle_3level_sum = 0;

/* internal function */

/* external variable */
static bool Toggle_Open(Toggle_Obj_TypeDef *obj, DevToggle_Type_List type, DrvGPIO_Obj_TypeDef *io);
static bool Toggle_Invert(Toggle_Obj_TypeDef *obj);
static DevToggle_Pos_List Toggle_Get(Toggle_Obj_TypeDef *obj);

/* external function */
DevToggle_TypeDef DevToggle = {
    .Open = Toggle_Open,
    .Get = Toggle_Get,
    .Invert = Toggle_Invert,
};

static bool Toggle_Open(Toggle_Obj_TypeDef *obj, DevToggle_Type_List type, DrvGPIO_Obj_TypeDef *io)
{
    if ((obj == NULL) || ((type < Toggle_2Level) || (Toggle_2Level > Toggle_3Level)))
        return false;

    obj->level = type;
    obj->invert = false;

    memset(obj->IO_Ptr, NULL, sizeof(obj->IO_Ptr));

    /* set io data structure */
    for (uint8_t i = 0; i < (uint8_t)type; i++)
    {
        obj->IO_Ptr[i] = &io[i];

        /* ToDo IO init */
        DrvGPIO.open(&obj->IO_Ptr[i], GPIO_Input, NULL);
    }

    return true;
}

static bool Toggle_Invert(Toggle_Obj_TypeDef *obj)
{
    if (obj == NULL)
        return false;

    obj->invert = !obj->invert;

    return true;
}

static DevToggle_Pos_List Toggle_Get(Toggle_Obj_TypeDef *obj)
{
    DevToggle_Pos_List val = Toggle_Err;
    uint8_t io_read = 0;

    if (obj == NULL)
        return Toggle_Err;

    for (uint8_t i = 0; i < obj->level; i++)
    {
        if (DrvGPIO.get(obj) == GPIO_HIGH)
        {
            io_read |= 1 << i;
        }
    }

    switch (io_read)
    {
    case 0x03:
        return val;

    case 0x00:
        if (obj->level == Toggle_2Level)
        {
            if (!obj->invert)
            {
                return Toggle_Dwn;
            }
            else
                return Toggle_Up;
        }
        else
            return Toggle_Mid;

    case 0x01:
        if (!obj->invert)
        {
            return Toggle_Up;
        }
        else
            return Toggle_Dwn;

    case 0x02:
        if (!obj->invert)
        {
            return Toggle_Dwn;
        }
        else
            return Toggle_Up;
    }
}
