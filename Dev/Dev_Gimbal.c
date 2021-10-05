#include <string.h>
#include "Dev_Gimbal.h"
#include "drv_adc.h"
#include "drv_gpio.h"

/* internal variable */
static uint8_t gimbal_cnt = 0;

/* external function */
static bool DevGimbal_Open(DevGimbal_Obj_TypeDef *gimbal);
static bool DevGimbal_Invert(DevGimbal_Obj_TypeDef *gimbal, uint8_t invert_reg);
static bool DevGimbal_Set_Offset(DevGimbal_Obj_TypeDef *gimbal, int16_t offset_x, int16_t offset_y);
static DevGimbal_Val_TypeDef DevGimbal_Get(DevGimbal_Obj_TypeDef *gimbal);

/* external variable */
DevGimbal_TypeDef DevGimbal = {};

static bool DevGimbal_Open(DevGimbal_Obj_TypeDef *gimbal)
{
    if (gimbal == NULL)
        return false;

    DrvADC.ctl();

    return true;
}

static bool DevGimbal_Invert(DevGimbal_Obj_TypeDef *gimbal, uint8_t invert_reg)
{
    if (gimbal == NULL)
        return false;

    return true;
}

static bool DevGimbal_Set_Offset(DevGimbal_Obj_TypeDef *gimbal, int16_t offset_x, int16_t offset_y)
{
    if (gimbal == NULL)
        return false;

    return true;
}

static DevGimbal_Val_TypeDef DevGimbal_Get(DevGimbal_Obj_TypeDef *gimbal)
{
    DevGimbal_Val_TypeDef Val_tmp;

    Val_tmp.error = false;
    Val_tmp.Gim_X = 0;
    Val_tmp.Gim_Y = 0;

    if (gimbal == NULL)
    {
        Val_tmp.error = true;
    }
    else
    {
        /* TODO */
    }

    return Val_tmp;
}
