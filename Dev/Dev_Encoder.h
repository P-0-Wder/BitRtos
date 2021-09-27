#ifndef __DEV_ENCODER_H
#define __DEV_ENCODER_H

#include <stdint.h>
#include <stdbool.h>
#include "Input_IO_Def.h"

typedef enum
{
    Encoder_Dir_Invert = SetBit(0),
    Encoder_Btn_Invert = SetBit(1),
    Encoder_None_Invert = 0,
} Encoder_Invert_List;

typedef enum
{
    Encoder_Normal = 0,
    Encoder_Error,
} Encoder_State_List;

typedef enum
{
    Encoder_IO_A = 0,
    Encoder_IO_B,
    Encoder_IO_Btn,
    Encoder_IO_Sum,
} Encoder_IO_Serials_List;

typedef struct
{
    Encoder_State_List state;
    int16_t val;
    bool btn;
} Encoder_Data_TypeDef;

typedef struct
{
    bool init_state;
    uint8_t invert_reg;

    int16_t max;
    int16_t min;

    DrvGPIO_Obj_TypeDef IO[Encoder_IO_Sum];
} DevEncoder_Obj_TypeDef;

typedef struct
{
    bool (*open)(DevEncoder_Obj_TypeDef *obj, int16_t range_max, int16_t range_min, DrvGPIO_Obj_TypeDef *io, uint8_t io_num);
    bool (*invert)(DevEncoder_Obj_TypeDef *obj, uint8_t reg_val);
    Encoder_Data_TypeDef (*get)(DevEncoder_Obj_TypeDef *obj);
} DevEncoder_TypeDef;

#endif
