#include "Dev_Encoder.h"
#include "drv_gpio.h"
#include <string.h>

/* internal variable */
static uint8_t encoder_sum = 0;

/* internal function */

/* external funtion */

/* external variable */

static bool DevEncoder_Open(DevEncoder_Obj_TypeDef *obj, int16_t range_max, int16_t range_min, DrvGPIO_Obj_TypeDef *io, uint8_t io_num)
{
    if ((obj == NULL) || (io == NULL) || (io_num != Encoder_IO_Sum) || (range_max == range_min))
    {
        obj->init_state = false;
        return false;
    }

    obj->max = range_max;
    obj->min = range_min;
    obj->invert_reg = Encoder_None_Invert;

    for (uint8_t i = 0; i < io_num; i++)
    {
        DrvGPIO.open(&io[i], GPIO_Input, NULL);
    }

    obj->init_state = true;
    return true;
}

static bool DevEncoder_Invert(DevEncoder_Obj_TypeDef *obj, uint8_t invert_val)
{
    if ((obj == NULL) || (!obj->init_state) || (invert_val > 0x03))
        return false;

    obj->invert_reg = invert_val;

    return true;
}

static Encoder_Data_TypeDef DevEncoder_Get(DevEncoder_Obj_TypeDef *obj)
{
    Encoder_Data_TypeDef data_tmp;
    bool val_tmp[Encoder_IO_Sum];

    memset(val_tmp, NULL, sizeof(val_tmp));
    memset(&data_tmp, NULL, sizeof(data_tmp));
    data_tmp.state = Encoder_Normal;

    if ((obj == NULL) || (!obj->init_state))
    {
        data_tmp.state = Encoder_Error;
        return data_tmp;
    }

    /* check encoder data */
    for (uint8_t i = 0; i < Encoder_IO_Sum; i++)
    {
        DrvGPIO.get(&obj->IO[i]);
    }

    return data_tmp;
}