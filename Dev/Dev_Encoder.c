#include "Dev_Encoder.h"
#include "drv_gpio.h"
#include "drv_timer.h"
#include <string.h>

/* internal variable */
static uint8_t encoder_sum = 0;

/* internal function */

/* external funtion */

/* external variable */

static bool DevEncoder_Open(DevEncoder_Obj_TypeDef *obj, int16_t range_max, int16_t range_min, DrvGPIO_Obj_TypeDef *io, uint8_t btn_enable)
{
    if ((obj == NULL) || (io == NULL) || (range_max == range_min))
    {
        obj->init_state = false;
        return false;
    }

    obj->max = range_max;
    obj->min = range_min;
    obj->btn_en = btn_enable;
    obj->invert_reg = Encoder_None_Invert;

    if (btn_enable)
    {
        DrvGPIO.open(&io[Encoder_IO_Btn], GPIO_Input, NULL);
    }

    DrvGPIO.open(&io[Encoder_IO_A], GPIO_Encoder, NULL);
    DrvGPIO.open(&io[Encoder_IO_B], GPIO_Encoder, NULL);

    DrvTimer.obj_clear(&obj->TimerObj);
    /* set timerobj */
    obj->TimerObj.timerx = Timer_3;

    if (DrvTimer.ctl(DrvTimer_Encoder_Mode, (uint32_t)&obj->TimerObj, sizeof(obj->TimerObj)))
    {
        obj->init_state = true;
    }
    else
    {
        obj->init_state = false;
    }

    return obj->init_state;
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

    /* check encoder button */
    if (obj->btn_en)
    {
        DrvGPIO.get(&obj->IO[Encoder_IO_Btn]);
    }

    DrvTimer.get(obj->TimerObj.timerx);

    return data_tmp;
}