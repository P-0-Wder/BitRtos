#include <string.h>
#include "Dev_Gimbal.h"
#include "drv_adc.h"
#include "drv_gpio.h"

/* internal variable */
static uint8_t Gimbal_Num = 0;

/* external function */
static bool DevGimbal_Obj_Clear(DevGimbal_Obj_TypeDef *obj);
static bool DevGimbal_Open(DevGimbal_Obj_TypeDef *obj);
static bool DevGimbal_Invert(DevGimbal_Obj_TypeDef *obj, uint8_t invert_reg);
static DevGimbal_Val_TypeDef DevGimbal_Get(DevGimbal_Obj_TypeDef *obj);
static uint8_t DrvGimbal_GetNum(void);

/* external variable */
DevGimbal_TypeDef DevGimbal = {
    .obj_clear = DevGimbal_Obj_Clear,
    .open = DevGimbal_Open,
    .invert = DevGimbal_Invert,
    .get = DevGimbal_Get,
    .get_gimbal_num = DrvGimbal_GetNum,
};

static bool DevGimbal_Obj_Clear(DevGimbal_Obj_TypeDef *obj)
{
    if (obj == NULL)
        return false;

    obj->Ch_X = ADC1_Channel_None;
    obj->Ch_Y = ADC1_Channel_None;

    obj->invert_reg = Gimbal_NoneAxis_Invert;

    return true;
}

static uint8_t DrvGimbal_GetNum(void)
{
    return Gimbal_Num;
}

static bool DevGimbal_Open(DevGimbal_Obj_TypeDef *obj)
{
    if ((obj == NULL) || (obj->Ch_X == ADC1_Channel_None) || (obj->Ch_Y == ADC1_Channel_None) || (obj->Ch_X == obj->Ch_Y))
        return false;

    DrvADC.ctl(ADC_Open, obj->Ch_X);
    DrvADC.ctl(ADC_Open, obj->Ch_Y);

    Gimbal_Num++;

    return true;
}

static bool DevGimbal_Invert(DevGimbal_Obj_TypeDef *obj, uint8_t invert_reg)
{
    if (obj == NULL)
        return false;

    obj->invert_reg = invert_reg;

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
        Val_tmp.Gim_X = DrvADC.read((uint32_t)obj->Ch_X);
        Val_tmp.Gim_Y = DrvADC.read((uint32_t)obj->Ch_Y);

        if (obj->invert_reg & Gimbal_AxisX_Invert)
        {
            Val_tmp.Gim_X *= -1;
        }

        if (obj->invert_reg & Gimbal_AxisY_Invert)
        {
            Val_tmp.Gim_Y *= -1;
        }
    }

    return Val_tmp;
}
