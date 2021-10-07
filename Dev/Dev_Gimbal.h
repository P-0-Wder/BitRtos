#ifndef __DEV_GIMBAL_H
#define __DEV_GIMBAL_H

#include <stdint.h>
#include <stdbool.h>
#include "periph_adc.h"

typedef enum
{
    Gimbal_NoneAxis_Invert = 0,
    Gimbal_AxisX_Invert = 1 << 0,
    Gimbal_AxisY_Invert = 1 << 1,
} DevGimbal_Invert_List;

typedef struct
{
    uint8_t invert_reg;

    int16_t Offset_X;
    int16_t Offset_Y;

    ADC_Channel_List Ch_X;
    ADC_Channel_List Ch_Y;
} DevGimbal_Obj_TypeDef;

typedef struct
{
    bool error;
    int16_t Gim_X;
    int16_t Gim_Y;
} DevGimbal_Val_TypeDef;

typedef struct
{
    bool (*obj_clear)(DevGimbal_Obj_TypeDef *obj);
    bool (*open)(DevGimbal_Obj_TypeDef *obj);
    bool (*invert)(DevGimbal_Obj_TypeDef *obj, uint8_t invert_reg);
    bool (*set_offset)(DevGimbal_Obj_TypeDef *obj, int16_t offset_x, int16_t offset_y);
    uint8_t (*get_gimbal_num)(void);
    DevGimbal_Val_TypeDef (*get)(DevGimbal_Obj_TypeDef *obj);
} DevGimbal_TypeDef;

extern DevGimbal_TypeDef DevGimbal;
#endif
