#ifndef __DEV_GIMBAL_H
#define __DEV_GIMBAL_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    Gimbal_AxisX_Invert = 1 << 0,
    Gimbal_AxisY_Invert = 1 << 1,
} DevGimbal_Invert_List;

typedef struct
{
    uint8_t invert_reg;

    int16_t Offset_X;
    int16_t Offset_Y;
} DevGimbal_Obj_TypeDef;

typedef struct
{
    int16_t Gim_X;
    int16_t Gim_Y;
} DevGimbal_Val_TypeDef;

typedef struct
{
    bool (*open)(DevGimbal_Obj_TypeDef *gimbal);
    bool (*invert)(DevGimbal_Obj_TypeDef *gimbal);
    bool (*set_offset)(DevGimbal_Obj_TypeDef *gimbal, int16_t offset_x, int16_t offset_y);
    DevGimbal_Val_TypeDef (*get)(DevGimbal_Obj_TypeDef *gimbal);
} DevGimbal_TypeDef;

#endif
