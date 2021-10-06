#include <string.h>
#include "Dev_Gimbal.h"
#include "drv_adc.h"
#include "drv_gpio.h"

/* internal variable */

/* external function */
static bool DevGimbal_Obj_Clear(DevGimbal_Obj_TypeDef *obj);
static bool DevGimbal_Open(DevGimbal_Obj_TypeDef *obj);
static bool DevGimbal_Invert(DevGimbal_Obj_TypeDef *obj, uint8_t invert_reg);
static bool DevGimbal_Set_Offset(DevGimbal_Obj_TypeDef *obj, int16_t offset_x, int16_t offset_y);
static DevGimbal_Val_TypeDef DevGimbal_Get(DevGimbal_Obj_TypeDef *obj);

/* external variable */
DevGimbal_TypeDef DevGimbal = {};

static bool DevGimbal_Obj_Clear(DevGimbal_Obj_TypeDef *obj)
{
    if (obj == NULL)
        return false;

    obj->Ch_X = ADC1_Channel_None;
    obj->Ch_Y = ADC1_Channel_None;

    obj->invert_reg = Gimbal_NoneAxis_Invert;

    obj->Offset_X = 0;
    obj->Offset_Y = 0;

    return true;
}

static bool DevGimbal_Open(DevGimbal_Obj_TypeDef *obj)
{
    if ((obj == NULL) || (obj->Ch_X == ADC1_Channel_None) || (obj->Ch_Y == ADC1_Channel_None) || (obj->Ch_X == obj->Ch_Y))
        return false;

    DrvADC.ctl(ADC_Open, obj->Ch_X);
    DrvADC.ctl(ADC_Open, obj->Ch_Y);

    return true;
}

static bool DevGimbal_Invert(DevGimbal_Obj_TypeDef *obj, uint8_t invert_reg)
{
    if (obj == NULL)
        return false;

    obj->invert_reg = invert_reg;

    return true;
}

static bool DevGimbal_Set_Offset(DevGimbal_Obj_TypeDef *obj, int16_t offset_x, int16_t offset_y)
{
    if (obj == NULL)
        return false;

    return true;
}

static DevGimbal_Val_TypeDef DevGimbal_Get(DevGimbal_Obj_TypeDef *obj)
{
    DevGimbal_Val_TypeDef Val_tmp;

    Val_tmp.error = false;
    Val_tmp.Gim_X = 0;
    Val_tmp.Gim_Y = 0;

    if (obj == NULL)
    {
        Val_tmp.error = true;
    }
    else
    {
        /* TODO */
        DrvADC.read();
    }

    return Val_tmp;
}
