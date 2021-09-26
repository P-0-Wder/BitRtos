#include "Dev_Toggle.h"
#include "drv_gpio.h"
#include <string.h>

/* internal variable */
static uint8_t toggle_2level_sum = 0;
static uint8_t toggle_3level_sum = 0;

/* internal function */

/* external variable */
static bool Toggle_Open(Toggle_Obj_TypeDef *obj, DevToggle_Type_List type, DrvGPIO_Obj_TypeDef *io);
static DevToggle_Pos_List Toggle_Get(Toggle_Obj_TypeDef *obj);

static bool Toggle_Open(Toggle_Obj_TypeDef *obj, DevToggle_Type_List type, DrvGPIO_Obj_TypeDef *io)
{
    if ((obj == NULL) || ((type < Toggle_2Level) || (Toggle_2Level > Toggle_3Level)))
        return false;

    obj->level = type;

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

static DevToggle_Pos_List Toggle_Get(Toggle_Obj_TypeDef *obj)
{
    DevToggle_Pos_List val = Toggle_Err;

    if (obj == NULL)
        return Toggle_Err;

    for (uint8_t i = 0; i < obj->level; i++)
    {
    }

    return val;
}
