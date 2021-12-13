#ifndef __DEV_ENCODER_H
#define __DEV_ENCODER_H

#include <stdint.h>
#include <stdbool.h>
#include "drv_gpio.h"
#include "drv_timer.h"

typedef int (*Encoder_Btn_Callback)(void);

typedef enum
{
    Encoder_Dir_Invert = SetBit(0),
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
    DrvTimer_Obj_TypeDef TimerObj;

    bool init_state;
    uint8_t invert_reg;
    bool btn_en;

    DrvGPIO_Obj_TypeDef IO[Encoder_IO_Sum];
    Encoder_Btn_Callback Btn_Callback;
} DevEncoder_Obj_TypeDef;

typedef struct
{
    bool (*open)(DevEncoder_Obj_TypeDef *obj, DrvGPIO_Obj_TypeDef *io, uint8_t btn_enable, uint8_t timerx, uint16_t ch_a, uint16_t ch_b);
    bool (*invert)(DevEncoder_Obj_TypeDef *obj, uint8_t reg_val);
    bool (*set_btn_callback)(DevEncoder_Obj_TypeDef *obj, Encoder_Btn_Callback callback);
    uint8_t (*num)(void);
    Encoder_Data_TypeDef (*get)(DevEncoder_Obj_TypeDef *obj);
} DevEncoder_TypeDef;

extern DevEncoder_TypeDef DevEncoder;

#endif
